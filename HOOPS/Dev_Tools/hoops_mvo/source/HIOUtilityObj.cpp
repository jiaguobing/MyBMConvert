// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "hc.h"

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HDB.h"
#include "HIOUtilityObj.h"
#include "HUtilityLocaleString.h"
#include "HUtilitySparseShell.h"
#include "HUtilityGeometryCreation.h"
#include "varray.h"

#include <algorithm>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>
#include <wchar.h>



#define countof(x) (sizeof(x) / sizeof(*x))

#ifdef _MSC_VER
#define ASSERT(x) ((!(x))?__debugbreak():1)
#else
#define ASSERT(x) assert(x)
#endif

// Whether to export the color interpolated geometry by faking it with a texture
//#define HU3D_COLOR_INTERPOLATION_NOT_VIA_TEXTURE

#define HOBJ_RED_GRAYNESS					0.3125f
#define HOBJ_GREEN_GRAYNESS					0.5000f       /* or .59? (NTSC) */
#define HOBJ_BLUE_GRAYNESS					0.1875f       /* or .11? (NTSC) */

#define HOBJ_RGB_TO_GRAY_TRANSPARENCY(r, g, b)    \
	(HOBJ_RED_GRAYNESS*(r) + HOBJ_GREEN_GRAYNESS*(g) + HOBJ_BLUE_GRAYNESS*(b)) 

#define HOBJ_RGB_TO_GRAY_OPACITY(r, g, b)    \
	(1.0f - HOBJ_RGB_TO_GRAY_TRANSPARENCY(r, g, b))

// Uncomment the following line if you dont want to import lines.
// By default lines are exported.
//#define HOBJ_EXPORT_LINES


namespace {
	class Strtok { // threadsafe version of strtok
	public:
		char * operator() (char * src, char const * delims);
	private:
		char * str;
	};

	char * Strtok::operator() (char * str, char const * delims) {
		if (str) {
			this->str = str;
		}
		if (!this->str) {
			return 0;
		}
		char * begTok = this->str;
		while (*begTok && strchr(delims, *begTok)) {
			++begTok;
		}
		char * endTok = begTok;
		while (*endTok && !strchr(delims, *endTok)) {
			++endTok;
		}
		if (*endTok == '\0') {
			this->str = 0;
		}
		else {
			this->str = endTok + 1;
		}
		*endTok = '\0';
		return *begTok ? begTok : 0;	
	}
}


static void getDirAndBaseName (wchar_t const * filePath, wchar_t * dir, wchar_t * baseName) {
	wcscpy(dir, filePath);
	wchar_t * pathSepFwd = wcsrchr(dir, L'/');
	wchar_t * pathSepBak = wcsrchr(dir, L'\\');
	wchar_t * pathSep = pathSepFwd ? pathSepFwd : pathSepBak;
	if (pathSepFwd && pathSepBak) {
		if (pathSepFwd > pathSepBak) {
			pathSep	= pathSepFwd;
		}
		else {
			pathSep = pathSepBak;
		}
	}
	if (pathSep) {
		wcscpy(baseName, pathSep + 1);
		*pathSep = L'\0';
	}
	else {
		wcscpy(baseName, dir);
		dir[0] = L'\0';
	}
}


static HPoint ReadPoint(FILE * in)
{
	char line[1000];
	fgets(line, 1000, in);

	float values[3] = {0, 0, 0};
	int count = 0;

	Strtok tokenizer;
	char * tok = tokenizer(line, " \t");

	while(tok && count < 3)
	{
		values[count++] = (float)atof(tok);
		tok = tokenizer(0, " \t");
	}

	HPoint retval(0, 0, 0);

	switch (count){
	case 2:
		retval.y = values[1];
	case 1:
		retval.x = values[0];
		break;

	default:
		retval = HPoint(values[0], values[1], values[2]);
	}

	return retval;
}

static void ReadFaceVertexIndexes(int indexes[3], char const * const input)
{
	indexes[0] = 0;
	indexes[1] = 0;
	indexes[2] = 0;

	// The face list values can have negative values in OBJ files
	// Modifying the default value set to the index to zero
	if (strchr(input, '/')) {
		if (strchr(input, '/') == strrchr(input, '/')) {
			// We're in the case: a/b
			sscanf(input, "%d/%d", &indexes[0], &indexes[1]);
			indexes[2] = 0;
		}
		else if (strstr(input, "//")) {
			// We're in the case: a//c
			sscanf(input, "%d//%d", &indexes[0], &indexes[2]);
			indexes[1] = 0;
		}
		else {
			// We're in the case: a/b/c
			sscanf(input, "%d/%d/%d", &indexes[0], &indexes[1], &indexes[2]);
		}
	}
	else {
		sscanf(input, "%d", &indexes[0]);
	}
}

namespace {
	class ObjFaces
	{
	private:
		VIntArray m_facelist;
		VIntArray m_texCoordIndexes;
		VIntArray m_Normallist;
		int lowest_vertex_index_seen;
		bool fresh_batch;

		void check_for_minimum_vert_idx(VIntArray const & list)
		{
			//set the bar if we haven't yet
			if (fresh_batch)
			{
				fresh_batch = false;
				lowest_vertex_index_seen = list[1];
			}

			for(size_t i = 1; i < list.Count(); i++)
			{
				if (list[i] < lowest_vertex_index_seen)
					lowest_vertex_index_seen = list[i];
			}
		}

	public:
		ObjFaces()
			: lowest_vertex_index_seen(0)
			,fresh_batch(true)
		{}

		int getLowestVertexIndexSeen() {
			return lowest_vertex_index_seen;
		}

		void addFace(VIntArray const & flist) {
			check_for_minimum_vert_idx(flist);
			m_facelist.AppendArray(&flist[0], flist.Count());
		}

		void addNormal(VIntArray const & nlist) {
			check_for_minimum_vert_idx(nlist);
			m_Normallist.AppendArray(&nlist[0], nlist.Count());
		}

		void addTextureCoordIndexes(VIntArray const & coords) {
			check_for_minimum_vert_idx(coords);
			m_texCoordIndexes.AppendArray(&coords[0], coords.Count());
		}

		void setFace(int index, int face_index) {
			m_facelist[index] = face_index;
		}

		void setNormal(int index, int normal_index) {
			m_Normallist[index] = normal_index;
		}

		void setTextureCoordIndexes(int index, int texture_index) {
			m_texCoordIndexes[index] = texture_index;
		}

		int const * faceList() const {
			return &m_facelist[0];
		}

		int const * normalList() const {
			return &m_Normallist[0];
		}

		int const * texCoordList() const {
			return &m_texCoordIndexes[0];
		}

		int faceListLength() const {
			return (int)m_facelist.Count();
		}

		int normalListLength() const {
			return (int)m_Normallist.Count();
		}

		int texCoordListLength() const {
			return (int)m_texCoordIndexes.Count();
		}

		void clear() {
			m_facelist.SetCount(0);
			m_texCoordIndexes.SetCount(0);
			m_Normallist.SetCount(0);
			lowest_vertex_index_seen = 0;
			fresh_batch = true;
		}

		void fill_face_list(int * new_face_list) {
			m_facelist.GetArrayCopy(new_face_list);
		}

		void fill_normal_list(int * new_normal_list) {
			m_Normallist.GetArrayCopy(new_normal_list);
		}

		void fill_texCoord_list(int * new_texCoocrd_list) {
			m_texCoordIndexes.GetArrayCopy(new_texCoocrd_list);
		}
	};
}

static void ReadFace(ObjFaces & faces, FILE * in, size_t point_count, size_t tex_count, size_t normal_count)
{
	VIntArray vertexIndexes;
	VIntArray texCoordIndexes;
	VIntArray normalIndexes;
	int indexes[3];
	vertexIndexes.Append(0); // for the face count
	normalIndexes.Append(0);
	texCoordIndexes.Append(0);

	char line[4000];
	fgets(line, 4000, in);
	Strtok tokenizer;
	char * tok = tokenizer(line, " \t");

	while (tok && !strchr("\t\n\r \\", tok[0])) {
		ReadFaceVertexIndexes(indexes, tok);
		++vertexIndexes[0];
		++normalIndexes[0];
		++texCoordIndexes[0];

		if (indexes[0] < 0)
			vertexIndexes.Append(indexes[0] + static_cast<int>(point_count)); // the face list should be in continuation with last vertex count
		else if (indexes[0] > 0)
			vertexIndexes.Append(indexes[0]-1);

		if (indexes[1] < 0)
			texCoordIndexes.Append(indexes[1] + static_cast<int>(tex_count));
		else if (indexes[1] > 0)
			texCoordIndexes.Append(indexes[1]-1);

		if (indexes[2] < 0)
			normalIndexes.Append(indexes[2] + static_cast<int>(normal_count));
		else if (indexes[2] > 0)
			normalIndexes.Append(indexes[2]-1);

		tok = tokenizer(0, " \t");
	}
	if (texCoordIndexes.Count() > 1) // to be sure that this face really has texcoords
		faces.addTextureCoordIndexes(texCoordIndexes);
	
	if (normalIndexes.Count() > 1) // to be sure that this face really has normalIndexes
		faces.addNormal(normalIndexes);

	if (vertexIndexes.Count() > 1) 
		faces.addFace(vertexIndexes); 
}

namespace {
	inline void CLAMP(float & x, float const a, float const b)
	{
		if (x < a) {
			x = a;
		}
		else if (x > b) {
			x = b;
		}
	}

	void trim(char * dest, char const * src)
	{
		while (isspace(*src)) {
			++src;
		}
		char * end = dest + sprintf(dest, "%s", src) - 1;
		while (end > dest && isspace(*end)) {
			--end;
		}
		++end;
		*end = '\0';
	}
}

static void LoadTexture(char const * name, wchar_t const * path)
{
	wchar_t ext[64] = L"";
	HUtility::FindFileNameExtension(path, ext);
	HInputHandler * image_handler = HDB::GetHIOManager()->GetInputHandler(ext);
	if (image_handler) {
		HC_KEY key;
		HC_KEY seg_key = HC_Open_Segment(".");
		HC_Close_Segment();
		HInputHandlerOptions options;
		H_FORMAT_TEXT image_name("image_%s", (char*)H_UTF8(name).encodedText());
		options.m_pImageName = (char const*)image_name;
		image_handler->FileInputToImageKey(path, &key, seg_key, &options);
		HC_Define_Local_Texture(name, H_FORMAT_TEXT("source = %s", (char const*)image_name));
		HC_Set_Visibility("images = off");
	}
}

static void LoadMtlLibrary(wchar_t const * const name)
{
	FILE * in = wfopen(name, L"r");

	if (!in)
		return;

	char buffer[MVO_BUFFER_SIZE];
	fgets(buffer, MVO_BUFFER_SIZE, in);

	HC_Open_Segment("default");
	char segment_name[MVO_BUFFER_SIZE];

	while (!feof(in)) {
		char const delim[] = " \t";
		Strtok tokenizer;
		char const * tok = tokenizer(buffer, delim);
		while (tok) {
			if (streq(tok, "newmtl")) {
				HC_Close_Segment();
				char const * newmtl_tok = tokenizer(0, "\r\n");
				if (!newmtl_tok) break;
				trim(segment_name, newmtl_tok);
				HC_Open_Segment(H_FORMAT_TEXT("`%s`", segment_name));

				break;
			}
			else if (strieq(tok, "Kd") || strieq(tok, "Ks") || strieq(tok, "Ka")|| strieq(tok, "Tf")) {
				char * color_tok = tokenizer(0, delim);
				if (!color_tok) break;
				else if (streq(color_tok, "spectral")) break;
				else if (streq(color_tok, "xyz")) break;
				else {
					float r, g, b;
					r = g = b = static_cast<float>(atof(color_tok));
					color_tok = tokenizer(0, delim);
					if (color_tok) {
						g = b = static_cast<float>(atof(color_tok));
						color_tok = tokenizer(0, delim);
						if (color_tok)
							b = static_cast<float>(atof(color_tok));
					}
					CLAMP(r, 0, 1);
					CLAMP(g, 0, 1);
					CLAMP(b, 0, 1);
				
					char color_name[MVO_BUFFER_SIZE];
					if (strieq(tok, "Kd"))
						HCLOCALE(sprintf(color_name, "faces = (diffuse = (r=%f g=%f b=%f))", r, g, b));
					if (strieq(tok, "Ks"))
						HCLOCALE(sprintf(color_name, "faces =(specular = (r=%f g=%f b=%f))", r, g, b));
					if (strieq(tok, "Ka"))
						HCLOCALE(sprintf(color_name, "ambient = (r=%f g=%f b=%f)", r, g, b));
					if (strieq(tok, "Tf"))
						HCLOCALE(sprintf(color_name, "faces = (transmission = (r=%f g=%f b=%f))", r, g, b));
					HC_Set_Color(color_name);
				}
				break;
			}
			else if (streq(tok, "map_Kd")){
				char const * filename_tok = tokenizer(0, delim);
				if (!filename_tok) break;
				char image_name[MVO_BUFFER_SIZE]= {""};
				sscanf(filename_tok, "%s", image_name);
				wchar_t image_path[MVO_BUFFER_SIZE]= {L""};
				wcscpy(image_path, name);
				wchar_t * file = wcsrchr(image_path, L'/');
				if (!file)
					file = wcsrchr(image_path, L'\\');
				++file;
				wcscpy(file, (wchar_t *)H_WCS(image_name).encodedText());

				LoadTexture(segment_name, image_path);

				HC_Set_Color(H_FORMAT_TEXT("faces = `%s`", segment_name));
				break;
			}

			else if (streq(tok, "refl")){
				tokenizer(NULL, " \t");
				tokenizer(NULL, " \t");
				char const * filename_tok = tokenizer(0, delim);
				if (!filename_tok) break;
				char image_name[MVO_BUFFER_SIZE]= {""};
				sscanf(filename_tok, "%s", image_name);
				wchar_t image_path[MVO_BUFFER_SIZE]= {L""};
				wcscpy(image_path, name);
				wchar_t * file = wcsrchr(image_path, L'/');
				if (!file)
					file = wcsrchr(image_path, L'\\');
				++file;
				wcscpy(file, (wchar_t *)H_WCS(image_name).encodedText());

				char texture_name[MVO_BUFFER_SIZE] = "";
				sprintf(texture_name, "%s", segment_name);

				LoadTexture(segment_name, image_path);

				if (!streq(texture_name, "")) 
					HC_Set_Color(H_FORMAT_TEXT("faces = (mirror = (r=0.5 g=0.5 b=0.5), environment = %s)", texture_name));
				
				break;
			}

			tok = tokenizer(NULL, " \t");
		}
		fgets(buffer, MVO_BUFFER_SIZE, in);
	}
	HC_Close_Segment();

	fclose(in);
}

static bool AddShellThroughUtility(
	VArray<HPoint> & points, VArray<HPoint> & normals, VFloatArray & texcoords, ObjFaces & faces)
{
	HPoint * new_vertex = new HPoint[points.Count()];
	points.GetArrayCopy(new_vertex);

	int * new_face_list = new int[faces.faceListLength()];
	faces.fill_face_list(new_face_list);

	HPoint * new_normals = nullptr;
	int * new_normal_list = nullptr;
	if (faces.normalListLength() == faces.faceListLength())
	{
		new_normals = new HPoint[normals.Count()];
		normals.GetArrayCopy(new_normals);

		new_normal_list = new int[faces.normalListLength()];
		faces.fill_normal_list(new_normal_list);
	}

	int * new_texCoord_list = nullptr;
	float * texCoordarray = nullptr;
	if (faces.texCoordListLength() == faces.faceListLength())
	{
		new_texCoord_list = new int[faces.texCoordListLength()];
		faces.fill_texCoord_list(new_texCoord_list);

		texCoordarray = new float[texcoords.Count()];
		texcoords.GetArrayCopy(texCoordarray);
	}

	HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes(
		new_vertex + faces.getLowestVertexIndexSeen(),
		faces.faceListLength(),
		new_face_list,
		new_normals ? new_normals + faces.getLowestVertexIndexSeen() : nullptr,
		new_normal_list,
		nullptr,
		nullptr,
		nullptr,
		nullptr,
		texCoordarray ? texCoordarray + 2 * faces.getLowestVertexIndexSeen() : nullptr,
		new_texCoord_list,
		new_texCoord_list ? 2 : 0);

	delete [] new_vertex;
	delete [] new_face_list;
	delete [] new_normals;
	delete [] new_normal_list;
	delete [] new_texCoord_list;
	delete [] texCoordarray;

	return true;
}

static void normalize_face_indices(ObjFaces & faces)
{
	int num_unused_vertices = faces.getLowestVertexIndexSeen();

	if (num_unused_vertices > 0)
	{
		int count_of_points_on_this_face = 0;

		for (int i = 0; i < faces.faceListLength(); i += count_of_points_on_this_face+1)
		{
			count_of_points_on_this_face = (faces.faceList())[i];
			for (int j = 1; j <= count_of_points_on_this_face; j++)
			{
				faces.setFace(i+j, (faces.faceList())[i+j] - num_unused_vertices);
			}
		}
		for (int i = 0; i < faces.normalListLength(); i += count_of_points_on_this_face+1)
		{
			count_of_points_on_this_face = (faces.normalList())[i];
			for (int j = 1; j <= count_of_points_on_this_face; j++)
			{
				faces.setNormal(i+j, (faces.normalList())[i+j] - num_unused_vertices);
			}
		}
		for (int i = 0; i < faces.texCoordListLength(); i += count_of_points_on_this_face+1)
		{
			count_of_points_on_this_face = (faces.texCoordList())[i];
			for (int j = 1; j <= count_of_points_on_this_face; j++)
			{
				faces.setTextureCoordIndexes(i+j, (faces.texCoordList())[i+j] - num_unused_vertices);
			}
		}
	}
}

HFileInputResult HIOUtilityObj::FileInputByKey(
	wchar_t const * FileName, HC_KEY key, HInputHandlerOptions * options) 
{
	if (!FileName) return InputBadFileName;
	if (key == INVALID_KEY) return InputFail;

	char handedness[8];
	HBaseView * view = options ? options->m_pHBaseView : 0;
	if (view) {
		HC_KEY keyPath[] = { key, view->GetIncludeLinkKey() };
		HC_PShow_Net_Handedness(countof(keyPath), keyPath, handedness);
	}
	else {
		HC_PShow_Net_Handedness(1, &key, handedness);
	}
	bool const isLeftHanded = strcmp(handedness, "left") == 0;

	char line[MVO_BUFFER_SIZE];
	VCharArray tok(MVO_BUFFER_SIZE);
	VArray<HPoint> points;
	VFloatArray texCoords; // Texture Coordinates
	VArray<HPoint> normals;

	HUtilitySparseShell im(false, false);
	ObjFaces faces;
	char material_name[MVO_BUFFER_SIZE] = { "default" };

	SetStartingInput(FileName);

	FILE * f = wfopen(FileName, L"r");
	if (f == NULL) 
		return InputBadFileName;

	fseek(f, 0, SEEK_END);
	long const file_size = ftell(f);
	fseek(f, 0, SEEK_SET);

	float start_time;
	HC_Show_Time(&start_time);

	int reports = 0;

	auto locale_action = [&] () {
		fscanf(f, "%s", static_cast<char *>(tok));
		while (!feof(f))
		{
			long const position = ftell(f);
			float const percent_complete = position / (float)file_size;
			if (reports++ % 1000 == 0)
				ReportInputPercentProgress(percent_complete);

			if (streq(tok, "v"))
			{
				//it's a vertex
				HPoint point = ReadPoint(f);
				if (isLeftHanded) {
					point.x = -point.x; // OBJ uses a right-handed system
				}
				points.Append(point);
			}
			else if (streq(tok, "vt"))
			{
				// It's a texture coordinate
				HPoint point = ReadPoint(f);
				texCoords.Append(point.x);
				texCoords.Append(point.y);
			}
			else if (streq(tok, "vn"))
			{
				// It's a Normal
				HPoint point = ReadPoint(f);
				normals.Append(point);		
			}
			else if (streq(tok, "f"))
			{
				// It's a face
				// AS: 14/01/09 To count the verices, used in creating face list
				ReadFace(faces, f, points.Count(), texCoords.Count()/2, normals.Count());
			}
			else if (streq(tok, "usemtl"))
			{
				// It's a material name

				// Insert the geometry for the last material
				if (faces.faceListLength())
				{
					normalize_face_indices(faces);

					HC_Open_Segment_By_Key(key);{
						HC_Open_Segment(H_FORMAT_TEXT("`%s`", material_name));{
							AddShellThroughUtility(points, normals, texCoords, faces);
						}HC_Close_Segment();
					}HC_Close_Segment();
				}

				faces.clear();

				// Set the new material name
				fgets(line, MVO_BUFFER_SIZE, f);
				trim(material_name, line);
			}
			else if (streq(tok, "mtllib")) {
				wchar_t path[MVO_BUFFER_SIZE] = {};
				wchar_t simpleName[MVO_BUFFER_SIZE] = {};
				getDirAndBaseName(FileName, path, simpleName);
				if (path[0]) {
					wcscat(path, L"/");
				}

				fscanf(f, "%s", line);
				wcscat(path, (wchar_t const *)H_WCS(line).encodedText());

				HC_Open_Segment_By_Key(key);{
					LoadMtlLibrary(path);
				}HC_Close_Segment();
			}
			else if (streq(tok, "g"))
			{
				// It's a new object 
				// If we have data, create a shell then flush the data
				if (faces.faceListLength() > 1)
				{
					normalize_face_indices(faces);

					HC_Open_Segment_By_Key(key);{
						HC_Open_Segment(H_FORMAT_TEXT("`%s`", material_name));{
							AddShellThroughUtility(points, normals, texCoords, faces);
						}HC_Close_Segment();
					}HC_Close_Segment();
				}

				faces.clear();

				fgets(line, MVO_BUFFER_SIZE, f);
			}
			else
				fgets(line, MVO_BUFFER_SIZE, f);
			fscanf(f, "%s", static_cast<char *>(tok));
		}
	};
	HCLOCALE(locale_action());

	// Create a shell
	normalize_face_indices(faces);

	HC_Open_Segment_By_Key(key);{
		HC_Open_Segment(H_FORMAT_TEXT("`%s`", material_name));{
			AddShellThroughUtility(points, normals, texCoords, faces);
		}HC_Close_Segment();
	}HC_Close_Segment();

	faces.clear();

	fclose(f);

	float end_time;
	HC_Show_Time(&end_time);
	float total_time = end_time - start_time;
	ReportInputInformation(H_FORMAT_TEXT("Time elapsed while loading: %0.2f seconds.", total_time));
	ReportInputPercentProgress(1.0);
	SetFinishedInput();

	return InputOK;
}


//////////////////////////////////////////////////////////////////////////


namespace {
	class QualifiedFileName {
	public:
		QualifiedFileName ();
		QualifiedFileName (wchar_t const * dir);
		void setBaseName (char const * baseName);
		void setBaseName (wchar_t const * baseName);
		operator wchar_t const * () const;
	private:
		wchar_t fullPath[4096];
		size_t baseNameOffset;
	};
}


QualifiedFileName::QualifiedFileName () {
	*fullPath = L'\0';
	baseNameOffset = 0;
}


QualifiedFileName::QualifiedFileName (wchar_t const * dir) {
	wcscpy(fullPath, dir);
	baseNameOffset = wcslen(fullPath);
	if (baseNameOffset == 0) {
		return;
	}
	wchar_t last = fullPath[baseNameOffset - 1];
	if (last != L'/' && last != L'\\') {
		fullPath[baseNameOffset++] = L'/';
		fullPath[baseNameOffset] = L'\0';
	}
}


void QualifiedFileName::setBaseName (char const * baseName) {
	wchar_t * dest = fullPath + baseNameOffset;
	while (true) {
		*dest = *baseName;
		if (*baseName == '\0') {
			break;
		}
		++dest;
		++baseName;
	}
}


void QualifiedFileName::setBaseName (wchar_t const * baseName) {
	wcscpy(fullPath + baseNameOffset, baseName);
}


QualifiedFileName::operator wchar_t const * () const {
	return fullPath;
}


//////////////////////////////////////////////////////////////////////////


namespace {
	class SceneWalker {
	public:
		enum WalkAction { WALK_BRANCH, SKIP_BRANCH, ABORT_WALK };
		SceneWalker ();
		virtual ~SceneWalker ();
	protected:
		void walk (HC_KEY startSegment);
		void walk (size_t keyCount, HC_KEY const * initialKeyPath);
		HC_KEY const * segPath () const;
		size_t segPathCount () const;
		bool conditionSatisfied (char const * condition) const;
		virtual WalkAction visitSegment ();
		virtual WalkAction visitInclude ();
		virtual void unvisitSegment ();
		virtual void unvisitInclude ();
	private:
		bool walkInternal (HC_KEY segmentKey);
		bool findChild (HC_KEY & key, char * type) const;
		void pushAncestors (HC_KEY segmentKey);
		void popAncestors (HC_KEY segmentKey);
		void pushVisiting (HC_KEY segmentOrInclKey);
		void popVisiting ();
		HC_KEY includeToSegment (HC_KEY includeKey) const;
	private:
		HC_KEY segmentPath[256];
		size_t pathCount;
	};
}


SceneWalker::SceneWalker ()
	: pathCount(0)
{}


SceneWalker::~SceneWalker () {}


HC_KEY const * SceneWalker::segPath () const {
	return &segmentPath[countof(segmentPath) - pathCount];
}


size_t SceneWalker::segPathCount () const {
	return pathCount;
}


void SceneWalker::pushAncestors (HC_KEY segmentKey) {
	segmentKey = HC_Show_Owner_Original_Key(segmentKey);
	if (segmentKey != HC_ERROR_KEY) {
		pushAncestors(segmentKey);
		pushVisiting(segmentKey);
	}
}


void SceneWalker::popAncestors (HC_KEY segmentKey) {
	segmentKey = HC_Show_Owner_Original_Key(segmentKey);
	while (segmentKey != HC_ERROR_KEY) {
		popVisiting();
		segmentKey = HC_Show_Owner_Original_Key(segmentKey);
	}
}


void SceneWalker::pushVisiting (HC_KEY segmentOrInclKey) {
	ASSERT(pathCount < countof(segmentPath));
	segmentPath[countof(segmentPath) - ++pathCount] = segmentOrInclKey;
}


void SceneWalker::popVisiting () {
	ASSERT(pathCount > 0);
	--pathCount;
}


HC_KEY SceneWalker::includeToSegment (HC_KEY includeKey) const {
	char path[4096];
	HC_KEY key = HC_Show_Include_Segment(includeKey, path);
	if (key < 0) {
		return key;
	}
	HC_Open_Segment(path);{
		HC_Open_Segment("..");{
			key = HC_Show_Original_Key(key);
		}HC_Close_Segment();
	}HC_Close_Segment();
	return key;
}


void SceneWalker::unvisitSegment () {}


void SceneWalker::unvisitInclude () {}


SceneWalker::WalkAction SceneWalker::visitSegment () {
	return WALK_BRANCH;
}


SceneWalker::WalkAction SceneWalker::visitInclude () {
	HC_KEY inclKey = segPath()[0];
	char pathname[4096];
	char condition[4096];
	HC_Show_Conditional_Include(inclKey, pathname, condition);
	if (condition[0] == '\0' || conditionSatisfied(condition)) {
		return WALK_BRANCH;
	}
	return SKIP_BRANCH;
}


bool SceneWalker::findChild (HC_KEY & key, char * type) const {
	if (!HC_Find_Contents(type, &key)) {
		return false;
	}
	if (key >= 0) {
		key = HC_Show_Original_Key(key);
	}
	return true;
}


bool SceneWalker::conditionSatisfied (char const * condition) const {
	// Instead of manually parsing conditions and deciding if they are
	// true, leverage HOOPS to decide if a set of conditions hold by
	// creating a temporary conditional style and seeing if the style
	// is applied with the conditions.
	float const baseWeight = 1;
	float const styleWeight = 2;
	char conds[4096];
	bool satisfied;

	HC_PShow_Net_Conditions(static_cast<int>(segPathCount()), segPath(), conds);

	HC_Open_Segment("/");{
		HC_KEY tmp = HC_Open_Segment("");{
			HC_Open_Segment("");{
				HC_Set_Line_Weight(baseWeight);

				HC_KEY styleSeg = HC_Open_Segment("");{
					HC_Set_Line_Weight(styleWeight);
				}HC_Close_Segment();

				HC_Open_Segment("");{
					HC_Set_Conditions(conds);
					HC_Conditional_Style_By_Key(styleSeg, condition);
					float actualWeight;
					HC_PShow_Net_Line_Weight(0, 0, &actualWeight);
					ASSERT(actualWeight == baseWeight || actualWeight == styleWeight);
					satisfied = actualWeight == styleWeight;
				}HC_Close_Segment();
			}HC_Close_Segment();
		}HC_Close_Segment();

		HC_Delete_By_Key(tmp);
	}HC_Close_Segment();

	return satisfied;
}


void SceneWalker::walk (HC_KEY startSegment) {
	pushAncestors(startSegment);{
		startSegment = HC_Show_Original_Key(startSegment);
		pushVisiting(startSegment);{
			walkInternal(startSegment);
		}popVisiting();
	}popAncestors(startSegment);
}


void SceneWalker::walk (size_t keyCount, HC_KEY const * initialKeyPath) {
	if (keyCount == 0) {
		return;
	}

	HC_KEY const topmostKey = initialKeyPath[keyCount - 1];
	HC_KEY const startSegment = HC_Show_Original_Key(initialKeyPath[0]);

	if (topmostKey != HC_ERROR_KEY) {
		pushAncestors(topmostKey);
	}
	for (size_t i = 0; i < keyCount - 1; ++i) {
		pushVisiting(initialKeyPath[keyCount - 1 - i]);
	}

	pushVisiting(startSegment);{
		walkInternal(startSegment);
	}popVisiting();

	for (size_t i = 0; i < keyCount - 1; ++i) {
		popVisiting();
	}
	if (topmostKey != HC_ERROR_KEY) {
		popAncestors(topmostKey);
	}
}


bool SceneWalker::walkInternal (HC_KEY segmentKey) {
	bool success = true;

	HC_Open_Segment_By_Key(segmentKey);{
		WalkAction action = visitSegment();{
			if (action == ABORT_WALK) {
				success = false;
			}
			if (success && action == WALK_BRANCH) {
				HC_Begin_Contents_Search(".", "subsegment");{
					HC_KEY foundKey;
					char type[32];
					while (success && findChild(foundKey, type)) {
						pushVisiting(foundKey);{
							if (strcmp(type, "include") == 0) {
								WalkAction action = visitInclude();{
									if (action == ABORT_WALK) {
										success = false;
									}
									if (success && action == WALK_BRANCH) {
										HC_KEY segKey = includeToSegment(foundKey);
										pushVisiting(segKey);{
											if (!walkInternal(segKey)) {
												success = false;
											}
										}popVisiting();
									}
								}unvisitInclude();
							}
							else {
								if (!walkInternal(foundKey)) {
									success = false;
								}
							}
						}popVisiting();
					}
				}HC_End_Contents_Search();
			}
		}unvisitSegment();
	}HC_Close_Segment();

	return success;
}


//////////////////////////////////////////////////////////////////////////


#define TEXTURE_FILE_TYPE "tiff"


namespace {
	template <typename T, size_t DEFAULT_SIZE>
	class Buffer {
	public:
		Buffer ()
			: pBuff(buff)
			, size(DEFAULT_SIZE)
		{}
		~Buffer () {
			releaseBuffer();
		}
		void ensureCapacity (size_t size) {
			if (this->size < size) {
				this->size = size;
				releaseBuffer();
				pBuff = new T[size];
			}
		}
		operator T * () {
			return pBuff;
		}
	private:
		void releaseBuffer () {
			if (pBuff != buff) {
				delete [] pBuff;
			}
		}
	private:
		T buff[DEFAULT_SIZE];
		T * pBuff;
		size_t size;
	};
}


namespace {
	class ObjExporter : public SceneWalker {
		enum EntityType { ENT_SEGMENT, ENT_CURVE, ENT_SURFACE, ENT_REFERENCE, ENT_UNKNOWN };
		enum TextureType { TXR_NONE, TXR_ENVIRONMENT, TXR_DIFFUSE };

		struct MaterialColor {
			float diffuse[3];
			float specular[3];
			float transmission[3];
		};

		struct Material {
			unsigned int id;
			float ambient[3];
			MaterialColor surface;
			MaterialColor curve;
		};

		struct Env {
			float matrix[16];
			Material material;
			EntityType type;
		};

		class AutoEnv {
			AutoEnv & operator= (AutoEnv const &);
		public:
			AutoEnv (std::vector<Env> & envs, bool cleanup = true);
			~AutoEnv ();
			Env * operator-> ();
			operator Env & ();
		private:
			std::vector<Env> & envs;
			size_t envOffset;
			bool cleanup;
		};

		struct WrittenMaterialDefintionInfo {
			unsigned int lastWrittenId;
			bool surfaceColorWritten;
			bool curveColorWritten;
		};
	public:
		ObjExporter ();
		bool run (size_t keyCount, HC_KEY const * keyPath, wchar_t const * dir, wchar_t const * filename);
		bool run (HC_KEY startSeg, wchar_t const * dir, wchar_t const * objFileName);
	protected:
		virtual WalkAction visitSegment ();
		virtual void unvisitSegment ();
	private:
		static void invalidate (float (&rgb)[3]);
		static void invalidate (MaterialColor & color);
		static bool isValid (float const (&rgb)[3]);

		void getNetTexture (TextureType & textureType, char * textureName) const;

		bool getRgbByValue (char const * type, float (&rgb)[3]) const;
		bool getExplicitRgb (char const * type, char const * channel, float (&rgb)[3]) const;

		bool updateMaterial (Material & material, EntityType parentType, HC_KEY geomKey);
		bool updateMatrix (float (&matrix)[16]) const;

		bool isVisible (char const * type) const;

		bool exportCurve (HC_KEY curveKey, char const * type);
		bool exportSurface (HC_KEY surfaceKey, char const * type);
		bool exportReferenceGeom (HC_KEY refKey);
		bool exportShell (HC_KEY shellKey);
		bool exportMesh (HC_KEY meshKey);
		bool exportPolyline (HC_KEY plineKey);
		bool exportLine (HC_KEY plineKey);
		bool exportMarker (HC_KEY markerKey);
		bool exportPolygon (HC_KEY polygonKey);

		static char const * getMaterialName (Env const & env, char * buffer);
		bool exportMaterialUse (Env const & env);
		bool exportMaterialDefinition (Env const & env);
	private:
		std::vector<Env> envs;
		QualifiedFileName qfname;
		FILE * objFile;
		FILE * mtlFile;
		HC_KEY tempSeg;
		unsigned int nextMaterialId;
		WrittenMaterialDefintionInfo writtenInfo;
		bool exportCurves;
		bool failed;
	};
}


ObjExporter::AutoEnv::AutoEnv (std::vector<Env> & envs, bool cleanup)
	: envs(envs)
	, cleanup(cleanup)
{
	envOffset = envs.size();
	envs.push_back(envs.back());
}


ObjExporter::AutoEnv::~AutoEnv () {
	if (cleanup) {
		ASSERT(!envs.empty());
		ASSERT(&envs.back() == &envs[envOffset]);
		envs.pop_back();
	}
}


ObjExporter::Env * ObjExporter::AutoEnv::operator-> () {
	return &envs[envOffset];
}


ObjExporter::AutoEnv::operator ObjExporter::Env & () {
	return envs[envOffset];
}


static char const * const curveGeometry = "polyline,line,marker,nurbs curve,elliptical arc,circular arc";
static char const * const surfaceGeometry = "shell,mesh,polygon,polycylinder,sphere,cylinder,ellipse,circle,circular chord,circular wedge,nurbs surface,grid";


static float det3x3 (float const (&M)[16]) {
	return M[0]*M[5]*M[10] + M[1]*M[6]*M[8] + M[2]*M[4]*M[9]
		- M[2]*M[5]*M[8] - M[1]*M[4]*M[10] - M[0]*M[6]*M[9];
}


static bool orientationChanged (float const (&matrix)[16]) {
	return det3x3(matrix) < 0;
}


ObjExporter::ObjExporter ()
	: exportCurves(true)
{}


void ObjExporter::invalidate (float (&rgb)[3]) {
	rgb[0] = -1.0f;
	rgb[1] = -1.0f;
	rgb[2] = -1.0f;
}


void ObjExporter::invalidate (MaterialColor & color) {
	invalidate(color.diffuse);
	invalidate(color.specular);
	invalidate(color.transmission);
}


bool ObjExporter::isValid (float const (&rgb)[3]) {
	return rgb[0] >= 0.0f;
}


static void changeExtension (wchar_t const * filename, wchar_t const * newExt, wchar_t * buff) {
	wcscpy(buff, filename);
	wchar_t * p = wcsrchr(buff, L'.');
	if (!p) {
		p = buff + wcslen(buff);
	}
	if (*newExt != L'.') {
		*p++ = L'.';
	}
	wcscpy(p, newExt);
}


bool ObjExporter::run (size_t keyCount, HC_KEY const * keyPath, wchar_t const * dir, wchar_t const * objFileName) {
	failed = false;

	do {
		wchar_t mtlFileName[4096] = {};
		changeExtension(objFileName, L"mtl", mtlFileName);

		qfname = QualifiedFileName(dir);

		qfname.setBaseName(objFileName);
		objFile = wfopen(qfname, L"w+");
		if (!objFile) {
			failed = true;
			break;
		}

		qfname.setBaseName(mtlFileName);
		mtlFile = wfopen(qfname, L"w+");
		if (!mtlFile) {
			failed = true;
			break;
		}

		fprintf(objFile, "mtllib %s\n", (char const *)H_UTF8(mtlFileName).encodedText());
		qfname.setBaseName(objFileName);

		{
			HC_Open_Segment("/");{
				tempSeg = HC_Create_Segment("");
			}HC_Close_Segment();

			nextMaterialId = 0;

			Env env;
			env.type = ENT_UNKNOWN;
			env.material.id = nextMaterialId++;

			invalidate(env.material.ambient);
			invalidate(env.material.surface);
			invalidate(env.material.curve);

			char handedness[8];
			HC_PShow_Net_Handedness(static_cast<int>(keyCount), keyPath, handedness);
			if (strcmp(handedness, "left") == 0) {
				HC_Compute_Scale_Matrix(-1, 1, 1, env.matrix);
			}
			else {
				HC_Compute_Identity_Matrix(env.matrix);
			}

			writtenInfo.lastWrittenId = env.material.id;
			writtenInfo.surfaceColorWritten = false;
			writtenInfo.curveColorWritten = false;

			envs.push_back(env);{
				HCLOCALE(walk(keyCount, keyPath));
			}envs.pop_back();

			HC_Delete_By_Key(tempSeg);
		}
	} while (false);

	if (mtlFile) {
		fclose(mtlFile);
	}
	if (objFile) {
		fclose(objFile);
	}

	return !failed;
}


bool ObjExporter::run (HC_KEY startSeg, wchar_t const * dir, wchar_t const * objFileName) {
	bool success;

	HC_Open_Segment("/");{
		HC_KEY virtualRoot = HC_Open_Segment("");{
			char handedness[8];
			HC_PShow_Net_Handedness(1, &startSeg, handedness);
			HC_Set_Handedness(handedness);

			HC_KEY virtualIncl = HC_Include_Segment_By_Key(startSeg);

			HC_KEY keyPath[] = { startSeg, virtualIncl };
			success = run(countof(keyPath), keyPath, dir, objFileName);
		}HC_Close_Segment();
		HC_Delete_By_Key(virtualRoot);
	}HC_Close_Segment();

	return success;
}


bool ObjExporter::getExplicitRgb (char const * type, char const * channel, float (&rgb)[3]) const {
	char queryType[128];
	sprintf(queryType, "color=%s=%s", type, channel);
	if (HC_Show_Existence(queryType) > 0) {
		char opts[32];
		static char garbage[1024];
		HC_Show_Explicit_Color(type, channel, rgb, garbage, opts);
		if (*opts == '\0') {
			// Easy RGB case
			return true;
		}
		else if (strcmp(opts, "ignore color") == 0) {
			// Texture case
			return false;
		}
		else {
			ASSERT(strcmp(opts, "findex") == 0);
			// Color map case... just acquire RGB via another means
			char typeChannel[64];
			sprintf(typeChannel, "%s=%s", type, channel);
			return getRgbByValue(typeChannel, rgb);
		}
	}
	return false;
}


bool ObjExporter::getRgbByValue (char const * type, float (&rgb)[3]) const {
	char queryType[128];
	sprintf(queryType, "color=%s", type);
	if (HC_Show_Existence(queryType) > 0) {
		char space[4];
		if (HC_Show_One_Color_By_Value(type, space, rgb, rgb + 1, rgb + 2)) {
			if (strcmp(space, "rgb") != 0) {
				HC_Compute_Color_By_Value(space, rgb, "rgb", rgb);
			}
			return true;
		}
	}
	return false;
}


void ObjExporter::getNetTexture (TextureType & textureType, char * textureName) const {
	char opts[32];

	HC_PShow_Net_Explicit_Color(
		static_cast<int>(segPathCount()), segPath(),
		"faces", "diffuse texture=0",
		0, textureName, opts);
	if (*textureName) {
		textureType = TXR_DIFFUSE;
		return;
	}

	HC_PShow_Net_Explicit_Color(
		static_cast<int>(segPathCount()), segPath(),
		"faces", "environment",
		0, textureName, opts);
	if (*textureName) {
		textureType = TXR_ENVIRONMENT;
		return;
	}

	textureType = TXR_NONE;
}


bool ObjExporter::updateMaterial (Material & material, EntityType parentType, HC_KEY geomKey) {
	struct Data {
		char const * type;
		MaterialColor * color;
	};

	bool const isGeom = geomKey != HC_ERROR_KEY;

	if (isGeom) {
		char keyType[32];
		HC_Show_Key_Type(geomKey, keyType);
		if (parentType == ENT_REFERENCE && strcmp(keyType, "reference") != 0) {
			return false;
		}
	}

	bool changed = false;

	if (isGeom) {
		HC_Open_Geometry(geomKey);
	}
	{
		if (HC_Show_Existence("color=face=diffuse")) {
			// The color might be a texture color. To be safe, set as changed.
			changed = true;
		}

		// Sadly, you cannot consolidate getRgbByValue and getExplicitRgb
		// uses to one or the other due to limitations of each one.

		Data const data[] = {
			{ "face", &material.surface },
			{ "line", &material.curve },
		};
		for (Data const * d = data; d < data + countof(data); ++d) {
			changed |= getExplicitRgb(d->type, "diffuse", d->color->diffuse);
			changed |= getExplicitRgb(d->type, "specular", d->color->specular);
			changed |= getExplicitRgb(d->type, "transmission", d->color->transmission);
		}
		changed |= getRgbByValue("ambient", material.ambient);
	}
	if (isGeom) {
		HC_Close_Geometry();
	}

	if (changed) {
		material.id = nextMaterialId++;
	}

	return changed;
}


bool ObjExporter::updateMatrix (float (&matrix)[16]) const {
	if (HC_Show_Existence("modelling matrix")) {
		float tmpMatrix[16];
		HC_Show_Modelling_Matrix(tmpMatrix);
		HC_Compute_Matrix_Product(tmpMatrix, matrix, matrix);
		return true;
	}
	return false;
}


bool ObjExporter::isVisible (char const * type) const {
	char vis[1024];
	HC_PShow_One_Net_Visibility(static_cast<int>(segPathCount()), segPath(), type, vis);
	return strcmp(vis, "on") == 0;
}


SceneWalker::WalkAction ObjExporter::visitSegment () {	
#if 0 && defined(_DEBUG)
	std::vector<std::string> path;
	for (size_t i = segPathCount(); i > 0; --i) {
		HC_KEY key = segPath()[i - 1];
		char type[64];
		HC_Show_Key_Type(key, type);
		if (strcmp(type, "segment") == 0) {
			char segName[4096];
			HC_Show_Segment(key, segName);
			path.push_back(segName);
		}
		else {
			path.push_back(">>>");
		}
	}
	std::string const & simpleSegname = path.back();
#endif

	AutoEnv env(envs, false);

	HC_Begin_Contents_Search(".", "style");{
		HC_KEY styleKey;
		while (HC_Find_Contents(0, &styleKey)) {
			char condition[4096];
			HC_Show_Conditional_Style(styleKey, 0, condition);
			if (conditionSatisfied(condition)) {
				HC_Open_Segment_By_Key(HC_Show_Style_Segment(styleKey, 0));{
					updateMaterial(env->material, env->type, HC_ERROR_KEY);
					updateMatrix(env->matrix);
				}HC_Close_Segment();
			}
		}
	}HC_End_Contents_Search();

	updateMaterial(env->material, env->type, HC_ERROR_KEY);
	updateMatrix(env->matrix);
	env->type = ENT_SEGMENT;

	bool success = true;

	if (exportCurves) {
		HC_Begin_Contents_Search(".", curveGeometry);{
			HC_KEY curveKey;
			char type[32];
			while (success && HC_Find_Contents(type, &curveKey)) {
				if (!exportCurve(curveKey, type)) {
					success = false;
				}
			}
		}HC_End_Contents_Search();
	}

	HC_Begin_Contents_Search(".", surfaceGeometry);{
		HC_KEY surfaceKey;
		char type[32];
		while (success && HC_Find_Contents(type, &surfaceKey)) {
			if (!exportSurface(surfaceKey, type)) {
				success = false;
			}
		}
	}HC_End_Contents_Search();

	HC_Begin_Contents_Search(".", "reference");{
		HC_KEY refKey;
		while (success && HC_Find_Contents(0, &refKey)) {
			if (!exportReferenceGeom(refKey)) {
				success = false;
			}
		}
	}HC_End_Contents_Search();

	if (success) {
		return WALK_BRANCH;
	}
	else {
		failed = true;
		return ABORT_WALK;
	}
}


void ObjExporter::unvisitSegment () {
	envs.pop_back();
}


bool ObjExporter::exportReferenceGeom (HC_KEY refKey) {
	AutoEnv env(envs);
	updateMaterial(env->material, env->type, refKey);
	env->type = ENT_REFERENCE;

	HC_Open_Geometry(refKey);{
		updateMatrix(env->matrix);
	}HC_Close_Geometry();

	HC_KEY actualKey = HC_Show_Reference_Geometry(refKey);
	char type[32];
	HC_Show_Key_Type(actualKey, type);
	if (strcmp(type, "segment") == 0) {
		bool success = true;
		HC_Open_Segment_By_Key(actualKey);{
			if (exportCurves) {
				HC_Begin_Contents_Search(".", curveGeometry);{
					HC_KEY curveKey;
					while (success && HC_Find_Contents(type, &curveKey)) {
						if (!exportCurve(curveKey, type)) {
							success = false;
						}
					}
				}HC_End_Contents_Search();
			}

			HC_Begin_Contents_Search(".", surfaceGeometry);{
				HC_KEY surfaceKey;
				while (success && HC_Find_Contents(type, &surfaceKey)) {
					if (!exportSurface(surfaceKey, type)) {
						success = false;
					}
				}
			}HC_End_Contents_Search();

			HC_Begin_Contents_Search(".", "reference");{
				HC_KEY refKey;
				while (success && HC_Find_Contents(0, &refKey)) {
					if (!exportReferenceGeom(refKey)) {
						success = false;
					}
				}
			}HC_End_Contents_Search();
		}HC_Close_Segment();
		return success;
	}
	else {
		if (exportSurface(actualKey, type)) {
			return true;
		}
		if (exportCurve(actualKey, type)) {
			return true;
		}
		if (strcmp(type, "reference") == 0) {
			return exportReferenceGeom(actualKey);
		}
		return false;
	}
}


bool ObjExporter::exportCurve (HC_KEY curveKey, char const * type) {
	if (strcmp(type, "polyline") == 0) {
		return exportPolyline(curveKey);
	}
	if (strcmp(type, "line") == 0) {
		return exportLine(curveKey);
	}
	if (strcmp(type, "marker") == 0) {
		return exportMarker(curveKey);
	}

	if (strcmp(type, "nurbs curve") == 0 ||
		strcmp(type, "elliptical arc") == 0 ||
		strcmp(type, "circular arc") == 0) 
	{
		HC_KEY plineKey;
		HC_Open_Segment_By_Key(tempSeg);{
			plineKey = HC_Generate_Polyline_From_Geometry(curveKey, "");
		}HC_Close_Segment();
		if (plineKey == HC_ERROR_KEY) {
			return false;
		}
		bool success = exportPolyline(plineKey);
		HC_Delete_By_Key(plineKey);

		return success;
	}

	return false;
}


bool ObjExporter::exportSurface (HC_KEY surfaceKey, char const * type) {
	if (strcmp(type, "shell") == 0) {
		return exportShell(surfaceKey);
	}
	if (strcmp(type, "mesh") == 0) {
		return exportMesh(surfaceKey);
	}
	if (strcmp(type, "polygon") == 0) {
		return exportPolygon(surfaceKey);
	}

	if (strcmp(type, "polycylinder") == 0 ||
		strcmp(type, "sphere") == 0 ||
		strcmp(type, "cylinder") == 0 ||
		strcmp(type, "ellipse") == 0 ||
		strcmp(type, "circle") == 0 ||
		strcmp(type, "circular chord") == 0 ||
		strcmp(type, "circular wedge") == 0 ||
		strcmp(type, "nurbs surface") == 0 ||
		strcmp(type, "grid") == 0)
	{
		HC_KEY shellKey;
		HC_Open_Segment_By_Key(tempSeg);{
			shellKey = HC_Generate_Shell_From_Geometry(surfaceKey, "");
		}HC_Close_Segment();
		if (shellKey == HC_ERROR_KEY) {
			return false;
		}
		bool success = exportShell(shellKey);
		HC_Delete_By_Key(shellKey);

		return success;
	}

	return false;
}


bool ObjExporter::exportLine (HC_KEY lineKey) {
	if (isVisible("line")) {
		AutoEnv env(envs);
		env->type = ENT_CURVE;
		exportMaterialUse(env);

		float ps[6];
		HC_Show_Line(lineKey, ps+0, ps+1, ps+2, ps+3, ps+4, ps+5);
		HC_Compute_Transformed_Points(2, ps, env->matrix, ps);
		for (int i = 0; i < 2; ++i) {
			float * p = ps + (3 * i);
			fprintf(objFile, "v %f %f %f\n", p[0], p[1], p[2]);
		}
		fprintf(objFile, "l");
		for (int i = 2; i > 0; --i) {
			fprintf(objFile, " -%d", i);
		}
		fprintf(objFile, "\n");
	}

	return true;
}


bool ObjExporter::exportPolyline (HC_KEY plineKey) {
	if (isVisible("line")) {
		AutoEnv env(envs);
		env->type = ENT_CURVE;
		exportMaterialUse(env);

		int count;
		HC_Show_Polyline_Count(plineKey, &count);
		float * ps = new float[3 * count];
		HC_Show_Polyline(plineKey, &count, ps);
		HC_Compute_Transformed_Points(count, ps, env->matrix, ps);
		for (int i = 0; i < count; ++i) {
			float * p = ps + (3 * i);
			fprintf(objFile, "v %f %f %f\n", p[0], p[1], p[2]);
		}
		fprintf(objFile, "l");
		for (int i = count; i > 0; --i) {
			fprintf(objFile, " -%d", i);
		}
		fprintf(objFile, "\n");
		delete [] ps;
	}
	return true;
}


bool ObjExporter::exportShell (HC_KEY shellKey) {
	if (isVisible("face")) {
		AutoEnv env(envs);
		updateMaterial(env->material, env->type, shellKey);
		env->type = ENT_SURFACE;
		exportMaterialUse(env);

		int pCount;
		int fCount;
		HC_Show_Shell_Size(shellKey, &pCount, &fCount);
		float * ps = new float[3 * pCount];
		int * fs = new int[fCount];
		HC_Show_Shell(shellKey, &pCount, ps, &fCount, fs);
		HC_Compute_Transformed_Points(pCount, ps, env->matrix, ps);
		for (int i = 0; i < pCount; ++i) {
			float * p = ps + (3 * i);
			fprintf(objFile, "v %f %f %f\n", p[0], p[1], p[2]);
		}
		delete [] ps;

		int vnCount = HC_Show_Existence_By_Key(shellKey, "vertex normals");
		if (vnCount > 0) {
			float * vns = new float[3 * pCount];
			HC_MShow_Net_Vertex_Normals(shellKey, 0, pCount, vns);
			for (int i = 0; i < pCount; ++i) {
				float * vn = vns + (3 * i);
				float plane[4] = { vn[0], vn[1], vn[2], 0 };
				HC_Compute_Transformed_Plane(plane, env->matrix, plane);
				fprintf(objFile, "vn %f %f %f\n", plane[0], plane[1], plane[2]);
			}
			delete [] vns;
		}

		int vpSize;
		HC_MShow_Vertex_Parameter_Size(shellKey, &vpSize);
		if (vpSize > 0) {
			ASSERT(vpSize == 2 || vpSize == 3);
			float * vps = new float[pCount * vpSize];
			HC_MShow_Vertex_Parameters(shellKey, 0, pCount, &vpSize, vps);
			for (int i = 0; i < pCount; ++i) {
				float * vp = vps + (vpSize * i);
				fprintf(objFile, "vt %f %f %f\n", vp[0], vp[1], vpSize == 3 ? vp[2] : 0.0f);
			}
			delete [] vps;
		}

		bool const reverseFaces = orientationChanged(env->matrix);
		Buffer<int, 16> hoopsFacePointIndices;
		for (int i = 0; i < fCount; /*empty*/) {
			int const n = fs[i++];
			if (n < 0) {
				i -= n;
				continue;
			}
			hoopsFacePointIndices.ensureCapacity(n);
			fprintf(objFile, "f");
			for (int j = 0; j < n; ++j) {
				hoopsFacePointIndices[j] = fs[i + j];
			}
			if (reverseFaces) {
				std::reverse(hoopsFacePointIndices + 0, hoopsFacePointIndices + n);
			}
			for (int j = 0; j < n; ++j) {
				int const objPointOffset = hoopsFacePointIndices[j] - pCount;
				if (vnCount == 0) {
					if (vpSize == 0) {
						fprintf(objFile, " %d", objPointOffset);
					}
					else {
						fprintf(objFile, " %d/%d", objPointOffset, objPointOffset);
					}
				}
				else {
					if (vpSize == 0) {
						fprintf(objFile, " %d//%d", objPointOffset, objPointOffset);
					}
					else {
						fprintf(objFile, " %d/%d/%d", objPointOffset, objPointOffset, objPointOffset);
					}
				}
			}
			fprintf(objFile, "\n");
			i += n;
		}

		delete [] fs;
	}
	return true;
}


bool ObjExporter::exportMesh (HC_KEY meshKey) {
	if (isVisible("face")) {
		AutoEnv env(envs);
		updateMaterial(env->material, env->type, meshKey);
		env->type = ENT_SURFACE;
		exportMaterialUse(env);

		int rows, cols;
		HC_Show_Mesh_Size(meshKey, &rows, &cols);
		int const pCount = rows * cols;
		float * ps = new float [3 * pCount];
		HC_Show_Mesh(meshKey, &rows, &cols, ps);
		HC_Compute_Transformed_Points(pCount, ps, env->matrix, ps);
		for (int i = 0; i < pCount; ++i) {
			float * p = ps + (3 * i);
			fprintf(objFile, "v %f %f %f\n", p[0], p[1], p[2]);
		}

		int vnCount = HC_Show_Existence_By_Key (meshKey, "vertex normals");
		if (vnCount > 0) {
			float * vns = new float[3 * pCount];
			HC_MShow_Net_Vertex_Normals(meshKey, 0, pCount, vns);
			for (int i = 0; i < vnCount; ++i) {
				float * vn = vns + (3 * i);
				fprintf(objFile, "vn %f %f %f\n", vn[0], vn[1], vn[2]);
			}
			delete [] vns;
		}

		int vpSize;
		HC_MShow_Vertex_Parameter_Size(meshKey, &vpSize);
		if (vpSize > 0) {
			ASSERT(vpSize == 2 || vpSize == 3);
			float * vps = new float[vpSize * pCount];
			HC_MShow_Vertex_Parameters(meshKey, 0, pCount, &vpSize, vps);
			for (int i = 0; i < pCount; ++i) {
				float * vp = vps + (3 * i);
				fprintf(objFile, "vt %f %f %f\n", vp[0], vp[1], vpSize == 3 ? vp[2] : 0.0f);
			}
			delete [] vps;
		}

		bool const reverseFaces = orientationChanged(env->matrix);
		for (int r = 0; r < rows - 1; ++r) {
			for (int c = 0; c < cols - 1; ++c) {
				float quadIndices[] = {
					static_cast<float>(r * cols + c),
					static_cast<float>(r * cols + c + 1),
					static_cast<float>((r + 1) * cols + c + 1),
					static_cast<float>((r + 1) * cols + c),
				};
				if (reverseFaces) {
					std::reverse(quadIndices, quadIndices + countof(quadIndices));
				}
				fprintf(objFile, "f");
				for (int i = 0; i < 4; ++i) {
					int pointOffset = static_cast<int>(quadIndices[i] - pCount);
					if (vnCount == 0) {
						if (vpSize == 0) {
							fprintf(objFile, " %d", pointOffset);
						}
						else {
							fprintf(objFile, " %d/%d", pointOffset, pointOffset);
						}
					}
					else {
						if (vpSize == 0) {
							fprintf(objFile, " %d//%d", pointOffset, pointOffset);
						}
						else {
							fprintf(objFile, " %d/%d/%d", pointOffset, pointOffset, pointOffset);
						}
					}
				}
				fprintf(objFile, "\n");
			}
		}

		delete [] ps;
	}
	return true;
}


bool ObjExporter::exportPolygon (HC_KEY polygonKey) {
	if (isVisible("face")) {
		AutoEnv env(envs);
		env->type = ENT_SURFACE;
		exportMaterialUse(env);

		int pCount;
		HC_Show_Polygon_Count(polygonKey, &pCount);
		float * ps = new float[3 * pCount];
		HC_Show_Polygon(polygonKey, &pCount, ps);
		HC_Compute_Transformed_Points (pCount, ps, env->matrix, ps);
		for (int i = 0; i < pCount; ++i) {
			float * p = ps + (3 * i);
			fprintf(objFile, "v %f %f %f\n", p[0], p[1], p[2]);
		}

		bool const reverseFaces = orientationChanged(env->matrix);
		fprintf(objFile, "f");
		if (reverseFaces) {
			for (int i = 1; i <= pCount; ++i) {
				fprintf(objFile, " -%d", i);
			}			
		}
		else {
			for (int i = pCount; i > 0; --i) {
				fprintf(objFile, " -%d", i);
			}
		}

		fprintf(objFile, "\n");
		delete [] ps;
	}

	return true;
}


bool ObjExporter::exportMarker (HC_KEY markerKey) {
	if (isVisible("marker only")) {
		AutoEnv env(envs);
		env->type = ENT_CURVE;
		exportMaterialUse(env);

		float ps[3];
		HC_Show_Marker(markerKey, ps, ps + 1, ps + 2);
		HC_Compute_Transformed_Points(1, ps, env->matrix, ps);
		fprintf(objFile, "v %f %f %f\np -1\n", ps[0], ps[1], ps[2]);
	}

	return true;
}


char const * ObjExporter::getMaterialName (Env const & env, char * buffer) {
	ASSERT(env.type == ENT_SURFACE || env.type == ENT_CURVE);

	sprintf(buffer, "%c%X", (env.type == ENT_SURFACE ? 'f' : 'l'), env.material.id);
	return buffer;
}


bool ObjExporter::exportMaterialUse (Env const & env) {
	ASSERT(env.type == ENT_SURFACE || env.type == ENT_CURVE);

	if (writtenInfo.lastWrittenId == env.material.id) {
		if (writtenInfo.surfaceColorWritten && env.type == ENT_SURFACE) {
			return true;
		}
		if (writtenInfo.curveColorWritten && env.type == ENT_CURVE) {
			return true;
		}
		if (!exportMaterialDefinition(env)) {
			return false;
		}
	}
	else if (env.material.id > writtenInfo.lastWrittenId) {
		writtenInfo.surfaceColorWritten = false;
		writtenInfo.curveColorWritten = false;
		if (!exportMaterialDefinition(env)) {
			return false;
		}
	}

	char buffer[32];
	fprintf(objFile, "usemtl %s\n", getMaterialName(env, buffer));
	writtenInfo.lastWrittenId = env.material.id;

	return true;
}


bool ObjExporter::exportMaterialDefinition (Env const & env) {
	ASSERT(env.type == ENT_SURFACE || env.type == ENT_CURVE);

	if (env.type == ENT_SURFACE) {
		writtenInfo.surfaceColorWritten = true;
	}
	else {
		writtenInfo.curveColorWritten = true;
	}

	Material const & m = env.material;
	MaterialColor const & mc = env.type == ENT_SURFACE ? m.surface: m.curve;

	char buffer[32];
	fprintf(mtlFile, "newmtl %s\n", getMaterialName(env, buffer));

	if (isValid(m.ambient)) {
		fprintf(mtlFile, "Ka %f %f %f\n", m.ambient[0], m.ambient[1], m.ambient[2]);
	}
	if (isValid(mc.diffuse)) {
		fprintf(mtlFile, "Kd %f %f %f\n", mc.diffuse[0], mc.diffuse[1], mc.diffuse[2]);
	}
	if (isValid(mc.specular)) {
		fprintf(mtlFile, "Ks %f %f %f\n", mc.specular[0], mc.specular[1], mc.specular[2]);
	}
	if (isValid(mc.transmission)) {
		fprintf(mtlFile, "Tf %f %f %f\n", mc.transmission[0], mc.transmission[1], mc.transmission[2]);
	}

	if (env.type == ENT_SURFACE) {
		// Ideally, getNetTexture would be stored in the material itself, but
		// unfortunately there is no good way to incrementally update the texture
		// value one segment at a time while walking the tree. (ie: There is no
		// way to do Show_Existence for textures. The only way to do this is
		// Show_Existence on diffuse face colors and then check to see if the
		// returned color is a texture name. This I believe could be rather slow
		// and even worse is that I would have to re-obtain texture names for each
		// segment due to the existence of local textures.)
		TextureType textureType;
		char textureName[1024];
		getNetTexture(textureType, textureName);

		if (textureType != TXR_NONE) {
			HC_KEY imageKey = HC_ERROR_KEY;
			HC_PShow_Net_Texture(static_cast<int>(segPathCount()), segPath(), textureName, 0, &imageKey);

			char tilingVal[8];
			HC_PShow_One_Net_Texture(static_cast<int>(segPathCount()), segPath(), textureName, "tiling", tilingVal);
			bool clamped = strcmp(tilingVal, "clamp") == 0;

			strcat(textureName, "." TEXTURE_FILE_TYPE);
			qfname.setBaseName(textureName);

			if (textureType == TXR_DIFFUSE) {
				fprintf(mtlFile, "map_Kd");
			}
			else if (textureType == TXR_ENVIRONMENT) {
				fprintf(mtlFile, "refl -type sphere");
			}
			if (clamped) {
				fprintf(mtlFile, " -clamp on");
			}
			fprintf(mtlFile, " %s\n", textureName);

			HOutputHandler * handler = HDB::GetHIOManager()->GetOutputHandler(TEXTURE_FILE_TYPE);
			HFileIOResult res = handler->FileOutputFromImageKey(qfname, imageKey, 0);
			if (res != HIO_OK) {
				return false;
			}
		}
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////


HFileIOResult HIOUtilityObj::FileOutputByKey(wchar_t const * wFilePath, HC_KEY startSeg, HOutputHandlerOptions * options) {
	wchar_t outObjFile[4096] = {};
	wchar_t outObjDir[4096] = {};
	getDirAndBaseName(wFilePath, outObjDir, outObjFile);

	bool success;
	SetStartingOutput();{
		ObjExporter exporter;
		HBaseView * view = options ? options->m_pHBaseView : 0;
		if (view) {
			HC_KEY keyPath[] = { startSeg, view->GetIncludeLinkKey() };
			success = exporter.run(countof(keyPath), keyPath, outObjDir, outObjFile);
		}
		else {
			success = exporter.run(startSeg, outObjDir, outObjFile);
		}
	}SetFinishedOutput();

	if (success) {
		return HIO_OK;
	}
	else {
		ReportOutputInformation("An error occurred while writing .obj file. Could not save file.");
		return HIO_Fail;
	}
}




