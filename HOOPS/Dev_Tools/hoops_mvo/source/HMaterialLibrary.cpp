// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <string.h>
#include "HMaterialLibrary.h"
#include "vconf.h"
#include "vhash.h"
#include "utf_utils.h"
#include "HStream.h"
#include "BOpcodeHandler.h"

#if _MSC_VER
#include <windows.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#include <dlfcn.h>
#endif

//now required for proper hsf export
#define NAMED_STYLES

//every uncompiled material directory must contain an index file
#define MATERIAL_DAT_FILE L"index.dat"
#define ENVIRONMENT_DAT_FILE L"environment_index.dat"
#define MATERIAL_SHADER_NAME "material" //the name passed into define shader
#define ENVIRONMENT_SEGMENT "environment"
#define CONTAINER_SEGMENT "?style library/material_library"


#define TWEAKABLES_USER_INDEX     0x41240000    //a place to hang the list of tweakable variable names (comma separated string tokens)
#define TWEAK_DATA_USER_INDEX     0x41240001    //the data for the net tweaks performed so far (mixed array of ints and floats)
#define HEIGHTFIELD_INDEX		  0x41240002    //the names of height field textures, if any
#define HEIGHTFIELD_SCALE_INDEX  0x41240003    //the strength parameter for converting height fields into normal maps

#define HMAT_BUFFER_SIZE 4080 //4096 minus upper bound of 2*sizeof(void*)

#ifndef nobreak
#   define nobreak      // fall through explicitly, instead of nothing which might be a forgotten "break"
#endif
#undef H_ASSERT
#if defined (_DEBUG) && !defined (_WIN32_WCE)
#   include <assert.h>
#   define H_ASSERT(x)	assert(x)
#else
#   define H_ASSERT(x)
#endif


enum DatType {
	Dat_Invalid,   
	Dat_Shader,   
	Dat_Texture,  
	Dat_Cubemap,
	Dat_Environment,
	Dat_Light,
};

class Dat {
public:
	DatType		type;				//either a texture or a shader
	char		*source_file;		//hlsl or glsl for shader, jpg or tga for texture
	int			channel;			//always 0 for shaders, >= 0 for textures
	bool		multitexture;		//relevant to shaders only
	float		heightfield;		//relevant to textures only - convert height to normal on load using this "strength"
	char		*parameterization_source; //null means HOOPS default
	char		*tweakables;		//null means there aren't any
	char		*color;				//null means no special colors
	char		*options;			//null means no options

	Dat() {
		type = Dat_Invalid;
		source_file = NULL;
		channel = -1;
		multitexture = Dat_Invalid;
		heightfield = 0.0f;
		parameterization_source = NULL;
		tweakables = NULL;
		color = NULL;
		options = NULL;
	};
	virtual ~Dat() {
		delete [] source_file;
		delete [] parameterization_source;
		delete [] tweakables;
		delete [] color;
		delete [] options;
	};
};

typedef VList<Dat *> VDatList;


class SerializationNode {
public:
	SerializationNode (int size) { 
		m_Allocated = size;
		m_Buffer = new char[m_Allocated];
		m_Used = 0;

	}
	~SerializationNode() {
		delete[] m_Buffer;
	}

	char *m_Buffer;
	int m_Allocated;
	int m_Used;
};
typedef VList<SerializationNode *> VSerializationNodeList;


class MaterialDescriptor {
public:
	MaterialDescriptor () {
		m_Seen = false;
		m_HSF_filename = NULL;
		m_HSF_buffers = NULL;
		m_Instancee = NULL;
	};
	~MaterialDescriptor () {
		delete [] m_HSF_filename;
		if (m_HSF_buffers) {
			SerializationNode *node;
			while ((node = m_HSF_buffers->RemoveFirst()) != NULL)
				delete node;
			delete m_HSF_buffers;
		}
		delete [] m_Instancee;
	};

	void SetHSF (const wchar_t *hsf_in) {
		size_t length = wcslen (hsf_in);
		m_HSF_filename = new wchar_t[ length + 1 ];
		wcscpy (m_HSF_filename, hsf_in);
	};

	void SetInstancee (const char *instance_in) {
		size_t length = strlen (instance_in);
		m_Instancee = new char[ length + 1 ];
		strcpy (m_Instancee, instance_in);
	};
			
	wchar_t *m_HSF_filename;
	VSerializationNodeList *m_HSF_buffers;
	char *m_Instancee;
	bool m_Seen;
};




static void cleanup_material_record(
	void *val,
	const char *material_name,
	void *user_data)
{
	struct vhash_s * hash = (struct vhash_s *) user_data;
	MaterialDescriptor *md = (MaterialDescriptor *) val;

	delete md;
	vhash_remove_string_key(hash, material_name);
}


HMaterialLibrary::HMaterialLibrary()
{ 
	//protected
	m_MaterialDirectory = NULL;
	m_MaterialHash = new_vhash (32, malloc, free);
	m_EnvironmentHash = new_vhash (32, malloc, free);
	m_MaterialList = NULL;
	m_EnvironmentList = NULL;
	m_CurrentEnvironment = NULL;
	m_MaterialSegmentKey = INVALID_KEY;
	m_CurrentEnvironmentKey = INVALID_KEY;
	m_WriteFlags = Write_Flags_Default;
	m_ReadFlags = Read_Flags_Default;
	//private
	m_SerializationBufferList = NULL;
	m_tk = NULL;
	m_Valid = false;
}


HMaterialLibrary::~HMaterialLibrary()
{
	delete [] m_MaterialList;
	delete [] m_EnvironmentList;
	delete [] m_CurrentEnvironment;
	delete [] m_MaterialDirectory;
	if (m_MaterialHash) {
		vhash_string_key_map_function (m_MaterialHash, cleanup_material_record, (void*)m_MaterialHash);
		delete_vhash (m_MaterialHash);
	}
	if (m_EnvironmentHash) {
		vhash_string_key_map_function (m_EnvironmentHash, cleanup_material_record, (void*)m_EnvironmentHash);
		delete_vhash (m_EnvironmentHash);
	}
	if (m_SerializationBufferList) {
		SerializationNode *node;
		while ((node = (SerializationNode *)vlist_remove_first (m_SerializationBufferList)) != NULL) {
			delete node;
		}
		delete_vlist (m_SerializationBufferList);
	}
	delete m_tk;
}


HMaterialLibrary::ErrorCode HMaterialLibrary::slurp_dat_file(				   
		const wchar_t *index_file, 
		const wchar_t *subdir, 
		const wchar_t *path, 
		void *datlist)
{
	H_WCS full_file_name;
	bool success = true;
	VDatList *list = (VDatList *)datlist;


	full_file_name.format (L"%ls/%ls/%ls", path, subdir, index_file);

	vconf_t* v = new_vconf(malloc, free);
	vconf_set_delimiter(v, ':');
	vconf_set_file_style(v, VCONF_FILE_STYLE_CHUNKED);
	if (!vconf_read_file(v, (const char *)H_UTF8(full_file_name).encodedText())) {
		delete_vconf(v);
		FILE *file = fopen ((const char *)H_UTF8(full_file_name).encodedText(), "rb");
		if (file) {
			fclose (file);
			return HMatParseError;
		}
		else
			return HMatFileError;
	}

	unsigned int i;

	for (i=0; i<vconf_chunk_count(v); i++) {
		Dat *dat = new Dat;
		const char * option;

		vconf_set_chunk(v, i);

		if ((option = vconf_get_option(v, "CHANNEL")) != 0) {
			if (streq (option, "SHADER")) {
				dat->type = Dat_Shader;
				dat->channel = 0;
			}
			else if (strstr (option, "TEXTURE")) {
				dat->type = Dat_Texture;
				if (sscanf(option, "TEXTURE%d", &dat->channel) != 1)
					success = false;
			}
			else if (strstr (option, "CUBEMAP")) {
				dat->type = Dat_Cubemap;
				if (sscanf(option, "CUBEMAP%d", &dat->channel) !=1)
					success = false;
			}
			else if (strstr (option, "ENVIRONMENT")) {
				dat->type = Dat_Environment;
				dat->channel = -1;
			}
			else if (strstr (option, "LIGHT")) {
				dat->type = Dat_Light;
				if (sscanf(option, "LIGHT%d", &dat->channel) != 1)
					success = false;
			}
			else
				success = false;
		}
		else {
			success = false; //every chunk must map to a channel
		}


		//both textures and shaders require a source file of some sort
		if ( dat->type == Dat_Texture || dat->type == Dat_Cubemap || dat->type == Dat_Environment || dat->type == Dat_Shader )
		{
			if ((option = vconf_get_option(v, "SOURCE")) != 0) {
				char * tmp = new char [strlen(option)+1];
				strcpy(tmp, option);
				dat->source_file = tmp;
			}
			else
				success = false; 
		}

		// textures can be heightfields
		if ( dat->type == Dat_Texture )
		{
			if ((option = vconf_get_option(v, "HEIGHTFIELD")) != 0) {
				// Attempt to read one float for strength
				if ( sscanf(option, "%g", &dat->heightfield) != 1 )
					dat->heightfield = 0.0f;
			}
		}
		
		if ((option = vconf_get_option(v, "MULTITEXTURE")) != 0) {
			if (streq (option, "yes") ||
				streq (option, "on") ||
				streq (option, "true"))
				dat->multitexture = true;
		}

		if ((option = vconf_get_option(v, "PARAMETERIZATION_SOURCE")) != 0) {
			char * tmp = new char [strlen(option)+1];
			strcpy(tmp, option);
			dat->parameterization_source = tmp;
		}

		if ((option = vconf_get_option(v, "TWEAKABLES")) != 0) {
			char * tmp = new char [strlen(option)+1];
			strcpy(tmp, option);
			dat->tweakables = tmp;
		}

		if ((option = vconf_get_option(v, "COLOR")) != 0) {
			char *tmp = new char [strlen(option)+1];
			strcpy(tmp, option);
			dat->color = tmp;
		}

		if ((option = vconf_get_option(v, "OPTIONS")) != 0) {
			char *tmp = new char [strlen(option)+1];
			strcpy(tmp, option);
			dat->options = tmp;
		}

		if (!success)
			goto CLEANUP;

		//insert into sorted list: channels 0..n, but possibly followed by negative numbers
		if (dat->channel >= 0) {
			Dat *tempdat;
			list->ResetCursor ();
			while ((tempdat = list->PeekCursor()) != 0) {
				if (tempdat->channel == dat->channel)
					success = false;
				if (tempdat->channel > dat->channel ||
					tempdat->channel < 0)
					break;
				list->AdvanceCursor();
			}
			list->AddBeforeCursor (dat);
		}
		else
			list->AddLast(dat);
	}

CLEANUP:
	delete_vconf(v);
	if (success)
		return HMatOK;
	else
		return HMatParseError;
}



class TK_Material_Metadata : public TK_Comment
{
public:
	TK_Material_Metadata () : TK_Comment () {
		name[0] = '\0';
		base[0] = '\0';
		environment = false;
		resolution = -1;
	}

	TK_Status Read (BStreamFileToolkit & tk) {
		TK_Status status = TK_Comment::Read(tk);
		if (m_comment != NULL) {
			char *ptr;
			if ((ptr = strstr (m_comment,  "material name=")) != NULL) {
				ptr += 14;
				environment = false;
				strcpy (name, ptr);
				if ((ptr = strchr (name, ',')) != NULL)
					*ptr = '\0';
			}
			if ((ptr = strstr (m_comment,  "environment name=")) != NULL) {
				ptr += 17;
				environment = true;
				strcpy (name, ptr);
				if ((ptr = strchr (name, ',')) != NULL)
					*ptr = '\0';
			}
			if ((ptr = strstr (m_comment, "resolution=")) != NULL) {
				ptr += 11;
				sscanf (ptr, "%d", &resolution);
			}
			if ((ptr = strstr (m_comment, "base=")) != NULL) {
				ptr += 5;
				strcpy (base, ptr);
				if ((ptr = strchr (base, ',')) != NULL)
					*ptr = '\0';
			}
			if (name[0] != '\0')
				return TK_Complete;
		}
		return status;
	};

	char name[HMAT_BUFFER_SIZE];
	char base[HMAT_BUFFER_SIZE];
	int resolution;
	bool environment;
};


#define HSF_CHUNK 4096
//attempt to extract the file name out of the hsf data, and throw onto the hash if successful
void HMaterialLibrary::RecordHSFMaterial (
		const wchar_t *path, 
		const wchar_t *filename)
{
	BStreamFileToolkit tk;
	TK_Material_Metadata *metadata = NULL;
	char buffer[HSF_CHUNK];

	H_WCS full_file_name;
	full_file_name.format (L"%ls/%ls", path, filename);
	FILE *fp = wfopen (full_file_name.encodedText(), L"rb");
	if (!fp)
		return;
	fread (buffer, 1, HSF_CHUNK, fp);
	metadata = new TK_Material_Metadata;
	tk.SetOpcodeHandler (TKE_Comment, metadata);
	tk.ParseBuffer (buffer, HSF_CHUNK);
	
	if (metadata->name[0] != '\0') {
		MaterialDescriptor *md;
		if (metadata->environment) {
			if (vhash_lookup_string_key_item (m_EnvironmentHash, metadata->name, (void **)&md) != VHASH_STATUS_SUCCESS) {
				md = new MaterialDescriptor ();
				vhash_insert_string_key (m_MaterialHash, metadata->name, md);
			}
			H_ASSERT (metadata->base[0] == '\0');
		}
		else {
			if (vhash_lookup_string_key_item (m_MaterialHash, metadata->name, (void **)&md) != VHASH_STATUS_SUCCESS) {
				md = new MaterialDescriptor ();
				vhash_insert_string_key (m_MaterialHash, metadata->name, md);
				if ((metadata->base[0] != '\0'))
					md->SetInstancee (metadata->base);
			}
		}
		md->SetHSF ((wchar_t *)full_file_name.encodedText());
	}
	fclose (fp);
}


//not recursive.  finds only materials at top level
void HMaterialLibrary::FindHSFMaterials(
		const wchar_t *path)
{
#ifdef WINDOWS_SYSTEM
	HANDLE find_handle;
	wchar_t find_spec[HMAT_BUFFER_SIZE];
	WIN32_FIND_DATAW find_data;
	BOOL find_result;

	wcscpy (find_spec, path);
	wcscat (find_spec, L"\\*.hsf");

	find_handle = FindFirstFileW(find_spec, &find_data);
	if (find_handle != INVALID_HANDLE_VALUE) {
		do {
			RecordHSFMaterial (path, find_data.cFileName);
		} while ((find_result = FindNextFileW(find_handle, &find_data)) != 0);
		FindClose(find_handle);
	}
#else
	DIR	*dir = wopendir (path);
	struct dirent *entry;

	if (dir) {
		while ((entry = readdir(dir)) != 0){
			struct stat buf;
			if(-1 != stat(H_FORMAT_TEXT("%s/%s", H_UTF8(path).encodedText(), entry->d_name), &buf) && S_ISREG(buf.st_mode)){
				if(wcsstr(H_WCS(entry->d_name).encodedText(), L"hsf")){
					RecordHSFMaterial (path, (wchar_t const*)H_WCS(entry->d_name).encodedText());
				}
			}
		}
	}
	closedir(dir);
#endif
}


bool HMaterialLibrary::HasIndexFile (
		const wchar_t *path, 
		const wchar_t *subpath, 
		const wchar_t *subdir,
		const wchar_t *index_file)
{
	bool has_index_file = false;
#ifdef WINDOWS_SYSTEM
	wchar_t find_spec[HMAT_BUFFER_SIZE];
	WIN32_FIND_DATAW find_data;
	HANDLE temp_handle;

	wcscpy (find_spec, path);
	wcscat (find_spec, L"\\");
	wcscat (find_spec, subpath);
	wcscat (find_spec, L"\\");
	wcscat (find_spec, subdir);
	wcscat (find_spec, L"\\");
	wcscat (find_spec, index_file);

	temp_handle = FindFirstFileW (find_spec, &find_data);
	has_index_file = (temp_handle != INVALID_HANDLE_VALUE);
	FindClose (temp_handle);
#else
	wchar_t find_spec[HMAT_BUFFER_SIZE];
	wcscpy (find_spec, path);
	if(wcslen(subpath)> 0){
		wcscat (find_spec, L"/");
		wcscat (find_spec, subpath);
	}
	if(wcslen(subdir)> 0){
		wcscat (find_spec, L"/");
		wcscat (find_spec, subdir);
	}
	wcscat (find_spec, L"/");
	wcscat (find_spec, index_file);

	FILE* file = wfopen(find_spec, L"r");
	if(file){
		has_index_file = true;
		fclose(file);
	}
#endif
	return has_index_file;
}


void HMaterialLibrary::FindSubdirectories (
		const wchar_t *path, 
		const wchar_t *subpath)
{
#ifdef WINDOWS_SYSTEM
	HANDLE find_handle;
	wchar_t find_spec[HMAT_BUFFER_SIZE];
	WIN32_FIND_DATAW find_data;
	BOOL find_result;

	wcscpy (find_spec, path);
	wcscat (find_spec, L"\\");
	wcscat (find_spec, subpath);
	wcscat (find_spec, L"\\*");

	find_handle = FindFirstFileW(find_spec, &find_data);
	do {
		if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY &&
			!wcseq (find_data.cFileName, L".") &&
			!wcseq (find_data.cFileName, L"..") &&
			!wcseq (find_data.cFileName, L"CVS")) {
			struct vhash_s *hash = NULL;
			bool has_index_file = false;

			if (HasIndexFile (path, subpath, find_data.cFileName, MATERIAL_DAT_FILE)) {
				has_index_file = true;
				hash = m_MaterialHash;
			}
			else if (HasIndexFile (path, subpath, find_data.cFileName, ENVIRONMENT_DAT_FILE)) {
				has_index_file = true;
				hash = m_EnvironmentHash;
			}

			//only record as a valid name if the index file is present
			if (has_index_file) {
				char buffer[HMAT_BUFFER_SIZE];
				strcpy(buffer, (const char *)H_UTF8(subpath).encodedText());
				if (subpath[0] != '\0')
					strcat(buffer, "/");
				strcat(buffer, (const char *)H_UTF8(find_data.cFileName).encodedText());
				int lookup_status = vhash_lookup_string_key_item (hash, buffer, NULL);
				if (lookup_status != VHASH_STATUS_SUCCESS)
					vhash_insert_string_key_item (hash, buffer, new MaterialDescriptor());
			}

			wchar_t newsubpath[HMAT_BUFFER_SIZE];
			wcscpy (newsubpath, subpath);
			if (subpath[0] != '\0')
				wcscat (newsubpath, L"/");
			wcscat (newsubpath, find_data.cFileName);
			FindSubdirectories(path, newsubpath);
		}
	} while ((find_result = FindNextFileW(find_handle, &find_data)) != 0);
	FindClose(find_handle);
#else

	DIR* find_handle;
	wchar_t find_spec[HMAT_BUFFER_SIZE];
	struct dirent *find_data;

	wcscpy (find_spec, path);
	wcscat (find_spec, L"/");
	wcscat (find_spec, subpath);

	find_handle = wopendir(find_spec);

	if(find_handle){
		while ((find_data = readdir(find_handle)) != 0){
			struct stat buf;
			if(-1 != stat(H_FORMAT_TEXT("%s/%s", H_UTF8(find_spec).encodedText(), find_data->d_name), &buf) && S_ISDIR(buf.st_mode)){
				if (
					!wcseq (H_WCS(find_data->d_name).encodedText() , L".") &&
					!wcseq (H_WCS(find_data->d_name).encodedText() , L"..") &&
					!wcseq (H_WCS(find_data->d_name).encodedText() , L"CVS")) {
					struct vhash_s *hash = NULL;
					bool has_index_file = false;

					if (HasIndexFile (path, subpath, H_WCS(find_data->d_name).encodedText(), MATERIAL_DAT_FILE)) {
						has_index_file = true;
						hash = m_MaterialHash;
					}
					else if (HasIndexFile (path, subpath, H_WCS(find_data->d_name).encodedText() , ENVIRONMENT_DAT_FILE)) {
						has_index_file = true;
						hash = m_EnvironmentHash;
					}

					//only record as a valid name if the index file is present
					if (has_index_file) {
						char buffer[HMAT_BUFFER_SIZE];
						strcpy(buffer, (const char *)H_UTF8(subpath).encodedText());
						if (subpath[0] != '\0')
							strcat(buffer, "/");
						strcat(buffer, (const char *)H_UTF8(find_data->d_name).encodedText());
						int lookup_status = vhash_lookup_string_key_item (hash, buffer, NULL);
						if (lookup_status != VHASH_STATUS_SUCCESS)
							vhash_insert_string_key_item (hash, buffer, new MaterialDescriptor());
					}

					wchar_t newsubpath[HMAT_BUFFER_SIZE];
					wcscpy (newsubpath, subpath);
					if (subpath[0] != '\0')
#ifdef WINDOWS_SYSTEM
						wcscat (newsubpath, L"\\");
#else
						wcscat (newsubpath, L"/");
#endif
					wcscat (newsubpath, H_WCS(find_data->d_name).encodedText());
					FindSubdirectories(path, newsubpath);
				}
			}
		}
		closedir(find_handle);
	}
#endif
}


static bool slurp_text_file (
		const wchar_t *file, 
		const wchar_t *subdir, 
		const wchar_t *path, 
		char **results = NULL)
{
	FILE *fp;
	H_WCS full_file_name;
	size_t size_in_bytes, bytes_read;
	char *buffer;

	full_file_name.format (L"%ls/%ls/%ls", path, subdir, file);

	fp = wfopen (full_file_name.encodedText(), L"r");
	if (!fp)
		return false;

	//grab total file size
	fseek (fp, 0L, SEEK_END); 
	size_in_bytes = ftell(fp); 
	fseek (fp, 0L, SEEK_SET); 

	buffer = new char[size_in_bytes + 4];
	bytes_read = fread (buffer, sizeof(char), size_in_bytes, fp);
	buffer[bytes_read] = '\0';

	fclose (fp);

	*results = buffer;
	return true;
}


static bool slurp_binary_file (
		const wchar_t *file, 
		const wchar_t *subdir, 
		const wchar_t *path, 
		size_t *length,
		unsigned char **results = NULL)
{
	FILE *fp;
	H_WCS full_file_name;
	size_t size_in_bytes, bytes_read;
	unsigned char *buffer;

	full_file_name.format (L"%ls/%ls/%ls", path, subdir, file);

	fp = wfopen (full_file_name.encodedText(), L"rb");
	if (!fp)
		return false;

	//grab total file size
	fseek (fp, 0L, SEEK_END); 
	size_in_bytes = ftell(fp); 
	fseek (fp, 0L, SEEK_SET); 

	buffer = new unsigned char[size_in_bytes + 4]; //add 4 for EOF
	bytes_read = fread (buffer, sizeof(char), size_in_bytes, fp);

	fclose (fp);

	*length = bytes_read;
	*results = buffer;
	return true;
}


const char *HMaterialLibrary::GetContainerSegment () const
{
	return CONTAINER_SEGMENT;
}


struct HashToStringContext {
	int length;
	char *ptr;
	char *end;
	struct vhash_s *hash;
	bool wide_characters;
	bool status;
};


static void unset_material_descriptor_seen (
	void *val,
	const char *material_name,
	void *user_data)
{
	UNREFERENCED(material_name);
	UNREFERENCED(user_data);

	MaterialDescriptor *md = (MaterialDescriptor *) val;
	md->m_Seen = false;
}


static void hash_to_string_helper (
	void *val,
	const char *material_name,
	void *user_data)
{
	size_t length;
	HashToStringContext *ctx = (HashToStringContext *) user_data;
	MaterialDescriptor *md = (MaterialDescriptor *) val;
	const char *name = material_name;

	if (md->m_Seen)
		return;

	struct vlist_s *stack = new_vlist (malloc, free);
	for (;;) {
		if (md->m_Seen)
			break;
		md->m_Seen = true;
		length = strlen (name) + 1;
		ctx->length += (int) length;
		if (ctx->ptr != NULL)
			vlist_add_first (stack, (void *)name);
		if (md->m_Instancee == NULL)
			break;
		name = md->m_Instancee;
		if (vhash_lookup_string_key_item (ctx->hash, name, (void **)&md) != VHASH_STATUS_SUCCESS)
			break;
	} 
	while ((name = (const char *)vlist_remove_first (stack)) != NULL) {
		length = strlen (name) + 1;
		if (ctx->end != NULL && 
			(size_t)(ctx->end - ctx->ptr) < length) {
			ctx->status = false;
		}
		else {
			strcpy (ctx->ptr, H_FORMAT_TEXT ("%s;", name));
			ctx->ptr += length;
		}
	}
	delete_vlist (stack);
}


const char *HMaterialLibrary::HashToString (struct vhash_s *hash, char *buffer, int buffer_size) const
{
	HashToStringContext ctx; 

	ctx.hash = hash;
	ctx.end = 0;
	ctx.wide_characters = false;
	ctx.status = true;

	if (!buffer) {
		//once through to figure allocation size (if necessary)
		ctx.ptr = NULL;
		ctx.length = 0;
		vhash_string_key_map_function (hash, unset_material_descriptor_seen, NULL);
		vhash_string_key_map_function (hash, hash_to_string_helper, &ctx);
		buffer_size = ctx.length + 1;
		buffer = new char[buffer_size]; //extra 1 for the '\0'
		buffer[ctx.length] = 0;
	}
	ctx.ptr = buffer;
	if (buffer_size > 0)
		ctx.end = buffer + buffer_size;
	ctx.length = 0;

	vhash_string_key_map_function (hash, unset_material_descriptor_seen, NULL);
	vhash_string_key_map_function (hash, hash_to_string_helper, &ctx);

	return buffer;
}


const char *HMaterialLibrary::GetMaterialList (char *buffer, int buffer_len)
{
	if (m_MaterialList == NULL)
		m_MaterialList = HashToString (m_MaterialHash);
	if (buffer == NULL)
		return m_MaterialList; 
	if (m_MaterialList == NULL) {
		buffer[0] = '\0';
		return NULL;
	}
	if (buffer_len <= 0) {
		strcpy (buffer, m_MaterialList);
	}
	else {
		strncpy (buffer, m_MaterialList, buffer_len);
		if (buffer[buffer_len-1] != '\0') {
			//buffer wasn't big enough.
			//make sure it gets null-terminated, and kill the last (partial) name.
			char *ptr = strrchr (buffer, ';');
			if (ptr == NULL)
				ptr = buffer;
			ptr[1] = '\0';
			return NULL;
		}
	}
	return buffer;	
}


const char *HMaterialLibrary::GetEnvironmentList (char *buffer, int buffer_len)
{
	if (m_EnvironmentList == NULL)
		m_EnvironmentList = HashToString (m_EnvironmentHash);
	if (buffer == NULL)
		return m_EnvironmentList; 
	if (m_EnvironmentList == NULL) {
		buffer[0] = '\0';
		return NULL;
	}
	if (buffer_len <= 0) {
		strcpy (buffer, m_EnvironmentList);
	}
	else {
		strncpy (buffer, m_EnvironmentList, buffer_len);
		if (buffer[buffer_len-1] != '\0') {
			//buffer wasn't big enough.
			//make sure it gets null-terminated, and kill the last (partial) name.
			char *ptr = strrchr (buffer, ';');
			if (ptr == NULL)
				ptr = buffer;
			ptr[1] = '\0';
			return NULL;
		}
	}
	return buffer;	
}


bool HMaterialLibrary::ApplyStyles ()
{
	bool found = false;
	HC_KEY key, source;
	char type[512];

	if ( HC_KCreate_Segment(".") != m_MaterialSegmentKey ) {
		// Look for existing style
		HC_Begin_Contents_Search (".", "styles");
		while (HC_Find_Contents (type, &key)) {
			source = HC_KShow_Style_Segment (key);
			if (source == m_MaterialSegmentKey) {
				found = true;
				break;
			}
		}
		HC_End_Contents_Search ();
		if (!found){
			char temp[4096];
			HC_Show_Segment(m_MaterialSegmentKey, temp);
			HC_Style_Segment_By_Key (m_MaterialSegmentKey);
		}
	}

	// Include environment lights via hook segment
	HC_Open_Segment (ENVIRONMENT_SEGMENT); {
		// Only flush includes under our control
		HC_Flush_Contents (".", "includes");
		HC_Include_Segment_By_Key (m_CurrentEnvironmentKey);
	} HC_Close_Segment();

	return true;
}


void HMaterialLibrary::Init ()
{
	m_MaterialSegmentKey = HC_KOpen_Segment (CONTAINER_SEGMENT); {
		m_CurrentEnvironmentKey = HC_KOpen_Segment ("current_environment"); {
			HC_Set_Visibility ("images=off");
		} HC_Close_Segment ();
	} HC_Close_Segment ();
}


bool HMaterialLibrary::RegisterMaterials (
		const char *path)
{
	return RegisterMaterials (H_UNICODE_TEXT(path));
}


bool HMaterialLibrary::RegisterMaterials (
		const wchar_t *path)
{
	size_t length = wcslen (path);
	wchar_t *matdir = new wchar_t[length + 1];
	wcscpy (matdir, path);
	m_MaterialDirectory = matdir;

	if (m_MaterialSegmentKey == INVALID_KEY)
		Init();

#ifdef WINDOWS_SYSTEM

	if(path[0] == L'.'){
		wchar_t *exeFileName = new wchar_t[MVO_BUFFER_SIZE];

		GetModuleFileNameW(0, exeFileName,  MVO_BUFFER_SIZE );

		wchar_t *char_ptr = wcsrchr(exeFileName, '\\');
		*char_ptr = 0;

		wcscat (exeFileName, L"\\");
		wcscat (exeFileName, path);
		path = exeFileName;
		
		m_MaterialDirectory = exeFileName;
		delete [] matdir;
	}

#endif
	FindSubdirectories (path, L"");
	FindHSFMaterials(path); 
	if (m_MaterialSegmentKey != HC_KCreate_Segment ("."))
		HC_Style_Segment_By_Key (m_MaterialSegmentKey);
	if (m_MaterialHash->key_count == 0 && 
		m_EnvironmentHash->key_count == 0)
		return false; //return false if we don't find at least one material or environment
	m_Valid = true;
	return true;
}


static bool dat_to_options (
		Dat *dat, 
		char *buffer)
{
	char *ptr = buffer;

	buffer[0] = '\0';
	if (dat->multitexture) {
		strcat (ptr, "multitexture,");
		ptr += strlen (ptr);
	}
	if (dat->parameterization_source) {
		sprintf (ptr, "parameterization source=%s,", dat->parameterization_source);
		ptr += strlen (ptr);
	}
	if (ptr != buffer)
		ptr[-1] = '\0'; //remove the trailing comma;

	return true;
}


/*
 * Decode adjacent heights to get a normal. Central differences or more sophisticated filters could be used
 * here to get a higher-quality result, but this method is simple and fast.
 */
inline static void compute_normal( const int h0, const int hx, const int hy, const float flatness, unsigned char *packed_normal )
{
	// For unpacking heightfield entries.
	const float unpack = 1.0f / 255.0f;

	//
	// Assemble normal from heights. The idea is to form vectors from the centre point to the two
	// adjacent points and take the cross product to get a vector normal to their spanning plane.
	// k is the grid spacing.
	//
	// O = (0, 0, h0)
	// X = (k, 0, hx)
	// Y = (0, k, hy)
	// 
	// So OX = (k, 0, hx - h0)
	//    OY = (0, k, hy - h0)
	// 
	// OX^OY = (-k(hx - h0), -k(hy - h0), k^2)
	//       = k(h0-hx, h0-hy, k)
	// 
	// We are only interested in the direction of the vector, so we can ignore the outer K. Note 
	// that the grid spacing effectively determines the heightfield steepness, so we can set K
	// to large values for flatter bumps, and small values for more pronounced bumps.
	//
	
	float nx = unpack * (h0 - hx);
	float ny = unpack * (h0 - hy);
	float nz = flatness;

	// Now normalize what we created
	const float length_squared = nx*nx + ny*ny + nz*nz;
	if ( length_squared > 1e-10f ) { 
		const float oo_length = 1.0f/sqrt(length_squared);
		nx *= oo_length;
		ny *= oo_length;
		nz *= oo_length;
	}
	else {
		nx = 0; ny = 0; nz = 1;
	}

	// Now encode the normal as an RGB colour
	packed_normal[0] = static_cast<unsigned char>(255 * (nx * 0.5f + 0.5f));
	packed_normal[1] = static_cast<unsigned char>(255 * (ny * 0.5f + 0.5f));
	packed_normal[2] = static_cast<unsigned char>(255 * (nz * 0.5f + 0.5f));
}


/*
 * Insert a normal map image created from a heightfield bumpmap.
 */
static void insert_normalmap_from_heightfield( 
	int width, 
	int height, 
	unsigned char const *heightfield,
	const char *name,
	bool wrap,
	float scale )
{
	// Setup to read heightfield
	const int x_stride = 1;
	const int y_stride = width * x_stride;
	unsigned char const *src = heightfield;

	// Create buffer for normals and prepare to write it.
	const int dest_x_stride = 4;
	unsigned char *buffer = new unsigned char[dest_x_stride * width * height];
	unsigned char *dest = buffer;

	// For the loop
	int h0, hx, hy;

	// For each pixel
	for ( int j = 0; j < height; ++j ) {
		for ( int i = 0; i < width; ++i ) {

			// Read centre height.
			h0 = src[0];

			// Read neighbours.
			if ( i != width - 1 )
				hx = src[x_stride];
			else
				// Handle boundary.
				hx = wrap ? src[(1-width) * x_stride] : h0;

			if ( j != height - 1 )
				hy = src[y_stride];
			else
				// handle boundary
				hy = wrap ? src[(1-height) * y_stride] : h0;

			// Compute the normal and store it.
			compute_normal( h0, hx, hy, scale, dest );

			// Store original height alongside
			dest[3] = static_cast<unsigned char>(h0);

			// Next pixel
			src += x_stride;
			dest += dest_x_stride;
		}
	}

	H_FORMAT_TEXT options("rgba, name=%s_image, local", name);
	HC_Insert_Image( 0, 0, 0, options, width, height, buffer );

	// Clean up
	delete [] buffer;

} /* end function insert_normalmap_from_heightfield */



static void convert_to_lowercase (char *str)
{
	size_t len = strlen(str);
	for (size_t i = 0 ; i < len ; i++) {
		if (str[i] >= 'A' && str[i] <= 'Z')
			str[i] = str[i] - 'A' + 'a';
	}
}


static HC_KEY find_image_by_name (char const *segspec, char const *name) 
{
	HC_KEY image_key = INVALID_KEY;
	char format[HMAT_BUFFER_SIZE];
	char substring[HMAT_BUFFER_SIZE];

	HC_Begin_Contents_Search (segspec, "images"); {
		strcpy (substring, H_FORMAT_TEXT ("name=%s", name));
		convert_to_lowercase (substring);
		image_key = INVALID_KEY;
		while (HC_Find_Contents (NULL, &image_key)) {
			HC_Show_Image_Format (image_key, format);
			convert_to_lowercase (format);
			char *ptr = strstr (format, substring);
			if (ptr) {
				int substring_len = (int) strlen (substring);
				if (ptr[substring_len] == '\0' ||
					ptr[substring_len] == ',')
					break;
			}
		}
	} HC_End_Contents_Search ();
	return image_key;
}


bool create_normalmap_from_heightfield(HC_KEY image_key, char const *texture_name, float strength)
{
	// Determine uncompressed format so we can allocate pixel buffer
	char format[HMAT_BUFFER_SIZE];
	int width, height;

	if (image_key == INVALID_KEY) {
		image_key = find_image_by_name (".", H_FORMAT_TEXT ("%s_source", texture_name));
		if (image_key == INVALID_KEY)
			return false;
	}
	HC_Show_Image(image_key, NULL, NULL, NULL, format, &width, &height, NULL);

	// How many bytes per pixel
	int pixel_size = 0;
	int channel = 0;
	if ( !strncmp(format, "rgba", 4) )
	{
		pixel_size = 4;
		channel = 0;
	}
	else if ( !strncmp(format, "argb", 4) )
	{
		pixel_size = 4;
		channel = 1;
	}
	else if ( !strncmp(format, "rgb", 3) )
	{
		pixel_size = 3;
		channel = 0;
	}
	else if ( !strncmp(format, "grayscale", 9) || !strncmp(format, "mapped 8", 8) )
	{
		pixel_size = 1;
		channel = 0;
	}

	if ( pixel_size ) {
		// Create image buffer and read it
		unsigned char *pixels = new unsigned char[width * height * pixel_size];
		HC_Show_Image(image_key, NULL, NULL, NULL, NULL, NULL, NULL, pixels);

		// Create luminance buffer
		unsigned char *heightfield = new unsigned char[width * height];

		// Setup read/write
		unsigned char const *src = pixels + channel;
		unsigned char *dest = heightfield;

		// Build heightfield
		for ( int y = 0; y != height; ++y ) {
			// For each pixel
			for ( int x = 0; x != width; ++x ) {
				// Write height from single channel
				*dest++ = static_cast<unsigned char>(*src);

				// Next pixel
				src += pixel_size;
			}
		}

		// No longer needed
		delete [] pixels;

		// Clamp strength
		strength = MAX(0.0f, MIN(1.0f, strength));

		// Determine scale
		float scale = 0.1f + (1.0f - strength) * 5.0f;

		// Add new image
		insert_normalmap_from_heightfield (width, height, heightfield, texture_name, true, scale);

		//note: if it's ever possible in the future for materials to have more than 
		//one height field, this code will have to change
		int datalen = (int) strlen (texture_name) + 1;
		HC_Set_User_Data (HEIGHTFIELD_INDEX, texture_name, datalen);
		HC_Set_User_Data (HEIGHTFIELD_SCALE_INDEX, (void *)&strength, sizeof(float));

		// No longer needed
		delete [] heightfield;
	}
	return true;
}


// Search for an on-disk texture with the given name (without extension)
static bool insert_texture(
	   const char *texture_name,
	   const char *material_name, 
	   const char *options, 
	   const wchar_t *material_dir, 
	   const Dat *dat = NULL)
{
	struct FileType {
		const char *extension;
		const char *encoding;
	};
	
	// Supported file types
	static const FileType file_types[] = {
		{ "png", "png" },
		{ "jpg", "jpeg" },
		{ "jpeg", "jpeg" },
		{ "tga", "tga" },
		{ "tpic", "tga" },
	};
	const int type_count = sizeof(file_types)/sizeof(file_types[0]);

	// Take the first encoding which is successful.
	bool success = false;
	for(int i=0; i<type_count && !success;++i) {
		size_t image_data_size = 0;
		unsigned char *image_data = NULL;

		H_WCS file_name;
		file_name.format("%s.%s", texture_name, file_types[i].extension);
		H_UTF8 temp_material_name;
		temp_material_name.encodedText(material_name);
		success = slurp_binary_file(file_name.encodedText(), 
			H_WCS(temp_material_name).encodedText(), 
			material_dir, &image_data_size, &image_data);

		if (success) {
			// Handle conversions
			if (dat && dat->heightfield > 0.0f ) {
				// Use helper to generate and replace texture
				H_FORMAT_TEXT image_options("%s, name = %s_source, local", file_types[i].encoding, texture_name);
				HC_Insert_Compressed_Image (0, 0, 0, image_options, -1, -1, (int)image_data_size, image_data);
				create_normalmap_from_heightfield(INVALID_KEY, texture_name, dat->heightfield);
			}
			else {
				// Load texture with this encoding
				H_FORMAT_TEXT image_options("%s, name = %s_image, local", file_types[i].encoding, texture_name);
				HC_Insert_Compressed_Image (0, 0, 0, image_options, -1, -1, (int)image_data_size, image_data);
			}

			// Default to anisotropic but allow overrides
			if ( !options )
				options = "decimation filter = anisotropic";

			// Then define over the top to specify options
			H_FORMAT_TEXT texture_options = H_FORMAT_TEXT("source = %s_image, %s", texture_name, options);
			HC_Define_Local_Texture (texture_name, texture_options);
		}

		// Clean up (if necessary)
		if (image_data)
			delete[] image_data;
	}

	// Done
	return success;
}


bool HMaterialLibrary::Initialize (
	const char *name, 
	bool environment)
{
	ErrorCode status;
	bool success = true;
	bool error_reported = false;
	const wchar_t *index_file = environment ? ENVIRONMENT_DAT_FILE : MATERIAL_DAT_FILE;

	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		HC_Open_Segment (name); {
			VDatList list;
			Dat *dat;
			char diffuse_color[HMAT_BUFFER_SIZE];
			char other_color[HMAT_BUFFER_SIZE];
			char *ptr;

			strcpy (diffuse_color, "");
			strcpy (other_color, "");

			H_UTF8 temp_subdir;
			temp_subdir.encodedText(name);
			H_WCS w_subdir(temp_subdir);

			//grab and parse the dat file
			status = slurp_dat_file (index_file, w_subdir.encodedText(), m_MaterialDirectory, &list);
			if (status < 0)
				success = false;
			while (success &&
				  (dat = list.RemoveFirst()) != NULL) {
				char options[HMAT_BUFFER_SIZE];
				char channel_name[HMAT_BUFFER_SIZE];

				success = dat_to_options (dat, options);

				// Trim extension from file name, if it's provided.
				if (dat->source_file)	{
					strcpy (channel_name, dat->source_file); 
					ptr = strrchr(channel_name, '.');
					if (ptr != NULL)
						ptr[0] = '\0';
				}

				if (success) switch(dat->type) {
					case Dat_Shader : {
						char *shader_source = NULL;
						success &= slurp_text_file (H_UNICODE_TEXT(dat->source_file), w_subdir.encodedText(), m_MaterialDirectory, &shader_source);
						if (success)
							HC_Define_Shader (MATERIAL_SHADER_NAME, options, shader_source);
						delete [] shader_source;

						if (dat->tweakables)
							HC_Set_User_Data (TWEAKABLES_USER_INDEX, dat->tweakables, (long)strlen(dat->tweakables) + 1);
						else
							HC_Set_User_Data (TWEAKABLES_USER_INDEX, "", 1);

						strcat(diffuse_color, MATERIAL_SHADER_NAME);
						strcat(diffuse_color, ", ");
						if (dat->color) {
							strcat(other_color, dat->color);
							strcat(other_color, ", ");
						}
					}
					break;

					case Dat_Texture : {
						// Use helper
						success &= insert_texture(channel_name, name, dat->options, m_MaterialDirectory, dat);

						if (!success) {
							Error (H_FORMAT_TEXT ("Error processing or file not found for material %s: %s", name, dat->source_file));
							error_reported = true;
						}

						strcat(diffuse_color, channel_name);
						strcat(diffuse_color, ", ");
						if (dat->color) {
							strcat(other_color, dat->color);
							strcat(other_color, ", ");
						}
					}
					break;

					case Dat_Cubemap : {
						// Cubemap face labels
						const char * face_labels[] = {"negz", "posz", "negx", "posx", "negy", "posy"};
						const int face_count = sizeof(face_labels)/sizeof(face_labels[0]);

						// For each face until one fails
						for(int i=0;i<face_count && success;++i)
							success &= insert_texture(H_FORMAT_TEXT("%s_%s", channel_name, face_labels[i]), name, NULL, m_MaterialDirectory);

						// If they all succeeded, create the cubemap
						if ( success ) {
							// Default to anisotropic but allow overrides
							const char *options = "decimation filter = anisotropic";
							if ( dat->options )
								options = dat->options;

							H_FORMAT_TEXT name("environment%d", dat->channel);
							H_FORMAT_TEXT cubemap("source=(%s_negz,%s_posz,%s_negx,%s_posx,%s_negy,%s_posy), tiling=clamp, parameterization source=physical reflection, %s", 
												  channel_name, channel_name, channel_name, channel_name, channel_name, channel_name, options);
							HC_Define_Local_Texture(name, cubemap);
						}
					}
					break;

					case Dat_Light : {
						// Directional light
						float dx, dy, dz;
						if ( sscanf(dat->options, "%g %g %g", &dx, &dy, &dz) == 3 )
						{
							// Insert light into environment
							HC_KEY key = HC_KInsert_Distant_Light(dx, dy, dz);
							HC_Set_Geometry_Options(key, "camera relative");

							// Set light color, if specified.
							if ( dat->color )	
								HC_Set_Color (dat->color);
						}
					}
					break;

					case Dat_Environment: {
						strcat (other_color, "environment=");
						strcat (other_color, channel_name);
						strcat (other_color, ",");
					}
					break;

					default :
						H_ASSERT(0);
				};

				delete dat;
			}

			// Remove trailing comma
			if ( (ptr = strrchr(diffuse_color, ',')) != 0 )
				*ptr = '\0';
			if ( (ptr = strrchr(other_color, ',')) != 0 )
				*ptr = '\0';
			
			if (success) {
				if (!environment) {
					// materials get colors and definitions, but environments only get definitions
					if (diffuse_color[0] != '\0')
						HC_Set_Color (H_FORMAT_TEXT ("faces = (diffuse = (%s))", diffuse_color));
					//for environment and transmission colors
					if (other_color[0] != '\0')
						HC_Set_Color (H_FORMAT_TEXT ("faces = (%s)", other_color)); 

					// Style by environment
					HC_Style_Segment_By_Key (m_CurrentEnvironmentKey);
				}
			}
			else if (!error_reported) {
				if (status == HMatParseError)
					Error (H_FORMAT_TEXT ("Error parsing dat file for %s", name));
				else if (status == HMatFileError) {
					//do nothing.  the caller can try to find a partially matched material name
				}
				else
					Error (H_FORMAT_TEXT ("unexpected internal error code while processing %s", name));
			}

			while( (dat = list.RemoveFirst()) != NULL)
				delete dat;

		} HC_Close_Segment();

#ifdef NAMED_STYLES
 		if (success) {
 			//define it as a named style
 			HC_Define_Named_Style(name, name);
 		}
#endif // NAMED_STYLES

	} HC_Close_Segment();

	return success;
}


bool HMaterialLibrary::GetTweakablesString (const char *material_name, char *string_out)
{
	int length;
	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		HC_Open_Segment (material_name); {
			string_out[0] = '\0';
			length = HC_Show_One_User_Data (TWEAKABLES_USER_INDEX, string_out, HMAT_BUFFER_SIZE);
		} HC_Close_Segment();
	} HC_Close_Segment();
	if (length < HMAT_BUFFER_SIZE)
		return true;
	else
		return false;
}


bool HMaterialLibrary::GetShaderSource(const char *material_name, char *shader_source, char *options)
{
	strcpy (shader_source, "");
	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		HC_Open_Segment (material_name); {
			HC_Show_Shader (MATERIAL_SHADER_NAME, options, shader_source);
		} HC_Close_Segment();
	} HC_Close_Segment();
	return true;
}


bool HMaterialLibrary::GetTweakablesCount (const char *material_name, int *count_out)
{
	char tweakables[HMAT_BUFFER_SIZE];
	bool status = true;
	int count = 0;

	/* initialize the material if it isn't already */
	if (!EnsureMaterialInitialized (material_name))
		return false;
	if (!GetTweakablesString (material_name, tweakables))
		return false;

	if (!streq (tweakables, "")) {
		char *ptr = tweakables;
		do {
			ptr++;
			count++;
		} while ((ptr = strchr (ptr, ',')) != NULL);
	}

	if (count_out)
		*count_out = count;
	return status;
}


const char *find_declaration (const char *shader_source, const char *tweakable_name)
{
	const char *candidate, *ptr, *ptr2;

	candidate = shader_source;
	for(;;) {
		candidate = strstr (candidate + 1, tweakable_name);
		if (candidate == NULL)
			return NULL;
		ptr = candidate - 1;
		if (*ptr != ' ' && *ptr != '\t')
			continue;  //name must be preceded by at least one white space character
		ptr2 = candidate + strlen(tweakable_name);
		if (*ptr2 != ' ' && 
			*ptr2 != '\t' &&
			*ptr2 != '=')
			continue;  //name must be followed by either white space or assignment operator
		//find the begining of the line
		ptr2 = ptr;
		while (*ptr2 != '\n' && 
				ptr2 > shader_source) 
				ptr2--;
		ptr2 = strstr (ptr2, "//");
		if (ptr2 != NULL &&
			ptr2 < ptr)
			continue;  //name must not be in code that is commented out

		//eat white space
		while (*ptr == ' ' || *ptr == '\t')
			ptr--;
		//search for the start of the type
		int count = 0;
		while (ptr > shader_source && 
			   *ptr != 'f' && 
			   *ptr != '\n' && 
			   count < 7) {
			ptr--;
			count++;
		}
		if (!strncmp (ptr, "float", 5))
			return ptr;
	}
}



static bool get_tweakable_name (
	const char *string,
	int index,
	char *name)
{
	int count = 0;
	const char *ptr = NULL;
	if (string != NULL &&
		!streq (string, "")) {
		ptr = string;
		for(;;) {
			if (count == index)
				break;
			count++;
			ptr = strchr (ptr + 1, ',');
			if (ptr == NULL)
				break;
			ptr++;
			while (*ptr == ' ' || *ptr == '\t')
				ptr++;
		} 
	}
	if (ptr == NULL) 
		return false;

	const char *ptr2 = strchr (ptr + 1, ',');
	if (ptr2 != NULL) {
		strncpy (name, ptr, ptr2 - ptr);
		name[ptr2 - ptr] = '\0';
	}
	else
		strcpy (name, ptr);
	return true;
}


bool HMaterialLibrary::GetTweakableByIndex (
		const char *material_name, 
		int index, 
		char *tweakable_name, 
		char *tweakable_type)
{
	char shader_source[HMAT_BUFFER_SIZE];
	char tweakables[HMAT_BUFFER_SIZE];
	char tweakable_name_buffer[HMAT_BUFFER_SIZE];
	char tweakable_type_buffer[HMAT_BUFFER_SIZE];

	/* initialize the material if it isn't already */
	EnsureMaterialInitialized (material_name);

	if (tweakable_name == NULL)
		tweakable_name = tweakable_name_buffer;
	if (tweakable_type == NULL)
		tweakable_type = tweakable_type_buffer;

	if (!GetTweakablesString (material_name, tweakables))
		return false;
	if (!get_tweakable_name (tweakables, index, tweakable_name)) {
		Error (H_FORMAT_TEXT ("tweakable index %d is out of range for material %s", index, material_name));
		return false;
	}				
	
	if (!GetShaderSource (material_name, shader_source))
		return false;

	const char *ptr = find_declaration (shader_source, tweakable_name);
	if (ptr == NULL)
		return false;
	strncpy (tweakable_type, ptr, 6); 
	if (ptr[5] == ' ' || 
		ptr[5] == '\t')
		tweakable_type[5] = '\0'; //float 
	else
		tweakable_type[6] = '\0'; //floatN

	return true;
}


bool HMaterialLibrary::GetTweakableByName (
		const char *material_name, 
		char const *tweakable_name, 
		int *index_out, 
		char *tweakable_type)
{
	int i;
	char shader_source[HMAT_BUFFER_SIZE];
	char tweakables[HMAT_BUFFER_SIZE];
	char tweakable_name_buffer[HMAT_BUFFER_SIZE];
	char tweakable_type_buffer[HMAT_BUFFER_SIZE];

	/* initialize the material if it isn't already */
	EnsureMaterialInitialized (material_name);

	if (tweakable_name == NULL)
		tweakable_name = tweakable_name_buffer;
	if (tweakable_type == NULL)
		tweakable_type = tweakable_type_buffer;

	if (!GetTweakablesString (material_name, tweakables))
		return false;
	i = 0;
	for (;;) {
		if (!get_tweakable_name (tweakables, i, tweakable_name_buffer)) {
			Error (H_FORMAT_TEXT ("could not find tweakable %s in material %s", tweakable_name, material_name));
			return false;
		}
		if (streq (tweakable_name, tweakable_name_buffer))
			break;
		i++;
	}
	if (index_out != NULL)
		*index_out = i;
	
	if (!GetShaderSource (material_name, shader_source))
		return false;

	const char *ptr = find_declaration (shader_source, tweakable_name);
	if (ptr == NULL)
		return false;
	strncpy (tweakable_type, ptr, 6); 
	if (ptr[5] == ' ' || 
		ptr[5] == '\t')
		tweakable_type[5] = '\0'; //float 
	else
		tweakable_type[6] = '\0'; //floatN

	return true;
}




int HMaterialLibrary::CountFromType (const char *type) 
{
	int count, matches;
	matches = sscanf (type, "float%d", &count);
	if (matches == 1)
		return count;
	else if (!strncmp (type, "float", 5))
		return 1;
	else
		return -1;
}


bool HMaterialLibrary::GetTweakableDefaults (
		const char *material_name, 
		int index, 
		float *default_vals)
{
	int count = 0;
	float default_vals_buffer[4];
	char tweakables[HMAT_BUFFER_SIZE];
	char tweakable_name[HMAT_BUFFER_SIZE];
	char shader_source[HMAT_BUFFER_SIZE];

	/* initialize the material if it isn't already */
	EnsureMaterialInitialized (material_name);

	if (default_vals == NULL)
		default_vals = default_vals_buffer;

	if (!GetTweakablesString (material_name, tweakables))
		return false;
	if (!get_tweakable_name (tweakables, index, tweakable_name)) {
		Error (H_FORMAT_TEXT ("tweakable index %d is out of range for material %s", index, material_name));
		return false;
	}				
	
	if (!GetShaderSource (material_name, shader_source))
		return false;

	const char *ptr = find_declaration (shader_source, tweakable_name);
	const char *ptr2 = NULL;
	if (ptr == NULL)
		return false;


	ptr = strchr( ptr+1, '=');
	ptr2 = strchr( ptr+1, ';');
	if (ptr2 < ptr) {
		//we hit a semicolon before finding any default specifiers
		Error (H_FORMAT_TEXT ("default value for tweakable %s not found in shader source for material %s", tweakable_name, material_name));
	}
	for (count = 0 ; count < 4 ; count++) {
		while (*ptr < '0' || *ptr > '9') {
			if (*ptr == 'f')
				ptr += 6; //skip over floatN type declaration in initializer
			if (*ptr == ')' || *ptr =='}' || *ptr == ';')
				break;
			ptr++;
		}
		if (*ptr == ')' || *ptr == '}' || *ptr == ';')
			break;
		HCLOCALE (sscanf (ptr, "%f", &default_vals[count]));
		while ((*ptr >= '0' && *ptr <= '9') || *ptr == '.')
			ptr++;
	}
	return true;
}


bool HMaterialLibrary::Tweak (
		const char *material_name, 
		int tweakable_index, 
		const float *vals)
{
	bool status = true;
	char options[HMAT_BUFFER_SIZE];
	char shader_source[HMAT_BUFFER_SIZE];
	char buffer[HMAT_BUFFER_SIZE];
	char tweakable_name[MVO_SMALL_BUFFER_SIZE];
	char tweakable_type[MVO_SMALL_BUFFER_SIZE];
	int offset;
	int count;
	const char *declaration, *remainder;
	char *ptr;
	int i;

	EnsureMaterialInitialized (material_name);
	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		HC_Open_Segment (material_name); {
			int temparray[HMAT_BUFFER_SIZE];
			int length, arraycount, shift;
			int *dest = NULL;

			status = GetTweakableByIndex (material_name, tweakable_index, tweakable_name, tweakable_type);
			if (!status)
				goto cleanup;
			HC_Show_Shader (MATERIAL_SHADER_NAME, options, shader_source);
			declaration = find_declaration (shader_source, tweakable_name);
			remainder = strchr (declaration, ';');
			if (declaration == NULL || remainder == NULL) {
				status = false;
				goto cleanup;
			}
			remainder++;
			offset = static_cast<int>(declaration - shader_source);
			strncpy (buffer, shader_source, offset);
			ptr = buffer + offset;
			sprintf (ptr, "%s %s = %s( ", tweakable_type, tweakable_name, tweakable_type);
			ptr += strlen (ptr);
			count = CountFromType (tweakable_type);
			if (count < 0) {
				status = false;
				goto cleanup;
			}
			for (i = 0 ; i < count ; i++) {
				HCLOCALE (sprintf (ptr, "%f", vals[i]));
				if (i < count - 1)
					strcat (ptr, ", ");
				ptr += strlen (ptr);
			}
			strcat (ptr, ");");
			strcat (ptr, remainder);
			HC_Define_Shader (MATERIAL_SHADER_NAME, options, buffer);

			//pack the tweak data into a linear array of mixed float/int types
			length = HC_Show_One_User_Data (TWEAK_DATA_USER_INDEX, temparray, HMAT_BUFFER_SIZE * sizeof(int));
			arraycount = length >> 2;
			H_ASSERT (sizeof(int) == 4);
			shift = count + 2;
			if (!length) {
				temparray[0] = 1;
				dest = &temparray[1];
				arraycount += shift + 1;
			}
			else {
				dest = &temparray[1];
				//search for where to inject the data
				for (i = 0 ; i < temparray[0] ; i++) {
					if (*dest < tweakable_index)
						dest += dest[1] + 2;
					else 
						break;
				}
				if (i == temparray[0]) {
					//hit the end of the list
					dest = dest;
					temparray[0]++;
					arraycount += shift;
				}
				else if (*dest == tweakable_index) {
					//re-tweaking an old tweak.  just overwrite
				}
				else {
					int *end = temparray + arraycount + shift - 1;
					H_ASSERT (*dest > tweakable_index);
					//it's important that this loop run backwards
					while (end >= dest + shift) {
						*end = *end - shift;
						end--;						
					}
					temparray[0]++;
					arraycount += shift;
				}
			}
			if (dest) {
				float *fptr;
				dest[0] = tweakable_index;
				dest[1] = count;
				fptr = (float *)&dest[2];
				for (i = 0 ; i < count ; i++)
					fptr[i] = vals[i];
			}
			if (length)
				HC_UnSet_One_User_Data (TWEAK_DATA_USER_INDEX);
			HC_Set_User_Data (TWEAK_DATA_USER_INDEX, temparray, arraycount * sizeof(int));

cleanup:    
			/* do nothing */;
		} HC_Close_Segment ();
	} HC_Close_Segment ();
	return status;
}


bool HMaterialLibrary::Tweak (
		const char *name, 
		int index, 
		float val0, float val1, float val2, float val3)
{
	float vals[4];
	vals[0] = val0;  vals[1] = val1;  vals[2] = val2;  vals[3] = val3;
	return Tweak (name, index, vals);
}


bool HMaterialLibrary::EnsureMaterialInitialized (
		const char *name)
{
	bool status = true;

	if (m_MaterialSegmentKey == INVALID_KEY)
		Init();

	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		//the presence or absence of a segment with the material's name is what 
		//we use to indicate whether or not it has been initialized
		if (!HC_QShow_Existence (name, "self")) {
			status = Initialize (name);
		}
	} HC_Close_Segment();
	return status;
}


bool HMaterialLibrary::EnsureEnvironmentInitialized (
		const char *name)
{
	bool status = true;

	if (m_MaterialSegmentKey == INVALID_KEY)
		Init();

	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		//the presence or absence of a segment with the material's name is what 
		//we use to indicate whether or not it has been initialized
		if (!HC_QShow_Existence (name, "self")) {
			status = Initialize (name, true);
		}
	} HC_Close_Segment();
	return status;
}


bool HMaterialLibrary::CloneMaterial (
	const char *old_material_name, 
	char *new_material_name)
{
	int noconflict = 1;
	HC_KEY newkey, oldkey;
	char shader_name[HMAT_BUFFER_SIZE];

	if (!EnsureMaterialInitialized(old_material_name))
		return false;

	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		for(;;) {
			sprintf (new_material_name, "%s+%d", old_material_name, noconflict);
			if (!HC_QShow_Existence (new_material_name, "self"))
				break;
		}

		oldkey = HC_Create_Segment (old_material_name);
		newkey = HC_Open_Segment (new_material_name); {
			HC_Style_Segment_By_Key (oldkey);
		} HC_Close_Segment();
		HC_Open_Segment_By_Key (oldkey); {
			HC_Begin_Shader_Search (); {
				while (HC_Find_Shader (shader_name)) {
					char shader_options[HMAT_BUFFER_SIZE];
					char shader_source[MVO_BIG_BUFFER_SIZE];
					HC_Show_Shader (shader_name, shader_options, shader_source);
					HC_Open_Segment_By_Key (newkey); {
						HC_Define_Shader (shader_name, shader_options, shader_source);
					} HC_Close_Segment();
				}
			} HC_End_Shader_Search ();

			char tweakstrings[HMAT_BUFFER_SIZE];
			char tweakdata[HMAT_BUFFER_SIZE];
			int stringlen, datalen;
			stringlen = HC_Show_One_User_Data (TWEAKABLES_USER_INDEX, tweakstrings, HMAT_BUFFER_SIZE);
			datalen = HC_Show_One_User_Data (TWEAK_DATA_USER_INDEX, tweakdata, HMAT_BUFFER_SIZE);
			HC_Open_Segment_By_Key (newkey); {
				H_ASSERT (stringlen); //should always be present
				HC_Set_User_Data (TWEAKABLES_USER_INDEX, tweakstrings, stringlen);
				if (datalen)
					HC_Set_User_Data (TWEAKABLES_USER_INDEX, tweakdata, datalen);
			} HC_Close_Segment();

			if (HC_Show_Existence ("color")) {
				char color[HMAT_BUFFER_SIZE];
				HC_Show_Color (color);
				HC_Open_Segment_By_Key (newkey); {
					HC_Set_Color (color);
				} HC_Close_Segment();
			}
		} HC_Close_Segment();
#ifdef NAMED_STYLES
		HC_Define_Named_Style(new_material_name, new_material_name);
#endif // NAMED_STYLES
	} HC_Close_Segment();

	MaterialDescriptor *md = new MaterialDescriptor ();
	md->SetInstancee (old_material_name);
	vhash_insert_string_key_item (m_MaterialHash, new_material_name, md);

	delete [] m_MaterialList;
	m_MaterialList = NULL;

	return true;
}


bool HMaterialLibrary::RenameMaterial (
	const char *old_material_name, 
	const char *new_material_name)
{
	bool status = true;
	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		if (IsValidMaterial (old_material_name) &&
			!HC_QShow_Existence (new_material_name, "self")) {
			HC_Rename_Segment (old_material_name, new_material_name);

			delete [] m_MaterialList;
			m_MaterialList = NULL;

			void *item;
			vhash_remove_string_key_item (m_MaterialHash, old_material_name, &item);
			vhash_insert_string_key_item (m_MaterialHash, new_material_name, item);
#ifdef NAMED_STYLES
			HC_UnDefine_Named_Style(old_material_name);
			HC_Define_Named_Style(new_material_name, new_material_name);
#endif // NAMED_STYLES
		}
		else
			status = false;
	} HC_Close_Segment();

	return status;
}


const char *HMaterialLibrary::FindValidMaterial (char const * needle) const
{
	char tempstr[HMAT_BUFFER_SIZE];
	const char *ptr;
	
	tempstr[0] = ';';
	strncpy (tempstr+1, needle, HMAT_BUFFER_SIZE-2);
	strcat (tempstr, ";");
	//look for ";needle;" anywhere or "needle;" at the start of the haystack
	ptr = strstr(m_MaterialList, tempstr);
	if (ptr != NULL) {
		return ptr + 1; //skip over the leading ';'
	}
	else {
		if (strstr(m_MaterialList, &tempstr[1]) != m_MaterialList)
			return NULL;
		else
			return m_MaterialList; //needle was the first material in the list
	}
}


bool HMaterialLibrary::IsValidMaterial (char const * name) const
{
	int lookup_status = vhash_lookup_string_key_item (m_MaterialHash, name, NULL);
	return lookup_status == VHASH_STATUS_SUCCESS;
}


bool HMaterialLibrary::ApplyMaterial (const char * name)
{
	bool status = true;
	char scratch[HMAT_BUFFER_SIZE];

	//replace backslashes with forward slashes
	if (strchr (name, '\\')) {
		char *ptr;
		strcpy (scratch, name);
		name = scratch;
		ptr = scratch;
		while ((ptr = strchr (ptr, '\\')) != NULL)
			*ptr = '/';
	}

	status = EnsureMaterialInitialized (name);
	if (!status) {
		char material_names[HMAT_BUFFER_SIZE];

		GetMaterialList (material_names, HMAT_BUFFER_SIZE);
		if (IsValidMaterial (name)) {
			//prevent infinite recursion
			return false;
		}
		else {
			//see if we can partially match
			const char *ptr;
			char *ptr2, *ptr3;
				
			if ((ptr = strrchr (name, '/')) == NULL)
				ptr = name;
			if ((ptr2 = strstr (material_names, ptr)) == NULL)
				return false;
			if ((ptr3 = strchr (ptr2, ';')) != NULL)
				*ptr3 = '\0'; //null terminate at the semicolon
			while (ptr2[-1] != ';' && ptr2 != &material_names[0])
				ptr2--; //rewind to the beginning of the partially matched material name
			status = ApplyMaterial(ptr2); //recurse
			if (!status)
				Error (H_FORMAT_TEXT ("Failed to locate valid data for requested material: %s", name));
			return status;
		}
	}

	// Remove existing style and attach library's 
	HC_Flush_Contents( ".", "named style, color" );

#ifdef NAMED_STYLES
	HC_Named_Style_Segment (name);
#else
	HC_KEY style;
	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		style = HC_KCreate_Segment (name);
	} HC_Close_Segment ();
	HC_Style_Segment_By_Key (style);
#endif // NAMED_STYLES

	return true;
}


bool HMaterialLibrary::ReloadMaterial (
		const char *name)
{
	bool status = true;

	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		if (HC_QShow_Existence (name, "self")) 
			HC_Flush_Contents (name, "everything, no subsegments");
		status = Initialize (name);
	} HC_Close_Segment();

	if (!status)
		return status;

	return true;
}


bool HMaterialLibrary::IsValidEnvironment
		(char const *name) const
{
	int lookup_status = vhash_lookup_string_key_item (m_EnvironmentHash, name, NULL);
	return lookup_status == VHASH_STATUS_SUCCESS;
}



bool HMaterialLibrary::SetEnvironment (
		const char *name)
{
	bool status = true;
	HC_KEY key;
	char scratch[HMAT_BUFFER_SIZE];

	//replace backslashes with forward slashes
	if (strchr (name, '\\')) {
		char *ptr;
		strcpy (scratch, name);
		name = scratch;
		ptr = scratch;
		while ((ptr = strchr (ptr, '\\')) != NULL)
			*ptr = '/';
	}

	status = EnsureEnvironmentInitialized(name);
	if ( !status ) {
		char environment_names[HMAT_BUFFER_SIZE];

		GetEnvironmentList (environment_names, HMAT_BUFFER_SIZE);
		if (IsValidEnvironment (name)) {
			//prevent infinite recursion
			return false;
		}
		else {
			//see if we can partially match
			const char *ptr;
			char *ptr2, *ptr3;
				
			if ((ptr = strrchr (name, '/')) == NULL)
				ptr = name;
			if ((ptr2 = strstr (environment_names, ptr)) == NULL)
				return false;
			if ((ptr3 = strchr (ptr2, ';')) != NULL)
				*ptr3 = '\0'; //null terminate at the semicolon
			while (ptr2[-1] != ';' && ptr2 != &environment_names[0])
				ptr2--; //rewind to the beginning of the partially matched material name
			status = SetEnvironment(ptr2); //recurse
			if (!status)
				Error (H_FORMAT_TEXT ("Failed to locate valid data for requested environment: %s", name));
			return status;
		}
	}

	size_t length = strlen (name) + 1;
	delete [] m_CurrentEnvironment;
	char *buffer = new char[length];
	strcpy (buffer, name);
	m_CurrentEnvironment = buffer;

	HC_Open_Segment_By_Key (m_MaterialSegmentKey); {
		key = HC_KCreate_Segment (name); //get the key
		HC_Open_Segment_By_Key (m_CurrentEnvironmentKey); {
			// Style for attributes *and* include for lights.
			HC_Flush_Contents( "./...", "styles, includes" );
			HC_Style_Segment_By_Key (key);
			HC_Include_Segment_By_Key (key);
		} HC_Close_Segment();
	} HC_Close_Segment();

	return status;
}


const char *HMaterialLibrary::GetCurrentEnvironment(char *buffer, int buffer_length)
{
	//TODO
	UNREFERENCED(buffer);
	UNREFERENCED(buffer_length);

	return NULL;
}


extern "C" { typedef void (HC_CDECL * Error_Routine) (...); }

void HMaterialLibrary::Error( const char *message, int severity)
{
	int category = 104;	// HEC_EXTERNAL
	int specific = 419;	// HES_EXTERNAL
	const char *tempstr = "HMaterialLibrary::ReportError";
	
	Error_Routine error_handler;

	HC_Begin_Error_Handler_Search();

		while ( HC_Find_Error_Handler(&error_handler) )
		{
			error_handler(category,specific,severity,1,&message,1,&tempstr);
		}

	HC_End_Error_Handler_Search();
	
}


static void cleanup_string_keys(
	void *val,
	const char *material_name,
	void *user_data)
{
	struct vhash_s * hash = (struct vhash_s *) user_data;
	vhash_remove_string_key(hash, material_name);
	UNREFERENCED (val);
}


bool HMaterialLibrary::FindReferencedMaterials (
		HC_KEY segment_tree, 
		char *buffer, 
		int buffer_size)
{
	UNREFERENCED(segment_tree);

	HC_KEY key;
	char type[HMAT_BUFFER_SIZE];
	char name[HMAT_BUFFER_SIZE];
	MaterialDescriptor *md;
	int lookup_status1, lookup_status2;
	vhash_s *referenced_hash = new_vhash (32, malloc, free);

	HC_Begin_Contents_Search ("....", "named styles"); {
		while (HC_Find_Contents (type, &key)) {
			HC_Show_Named_Style_Segment (key, name); 
			lookup_status1 = vhash_lookup_string_key_item (m_MaterialHash, name, (void **)&md);
			if (lookup_status1 == VHASH_STATUS_SUCCESS) {
				lookup_status2 = vhash_lookup_string_key_item (referenced_hash, name, NULL);
				if (lookup_status2 != VHASH_STATUS_SUCCESS) {
					vhash_insert_string_key_item (referenced_hash, name, md);
					//now traverse the chain of dependencies
					while (md && md->m_Instancee != NULL) {
						strcpy (name, md->m_Instancee);
						lookup_status2 = vhash_lookup_string_key_item (referenced_hash, name, NULL);
						if (lookup_status2 != VHASH_STATUS_SUCCESS) {
							lookup_status2 = vhash_lookup_string_key_item (m_MaterialHash, name, (void **)&md);
							vhash_insert_string_key_item (referenced_hash, name, md);
						}
						else
							break;
					}
				}
			} 
		}
	} HC_End_Contents_Search (); 
	buffer[0] = '\0';
	HashToString (referenced_hash, buffer, buffer_size);

	vhash_string_key_map_function (referenced_hash, cleanup_string_keys, (void *)referenced_hash);
	delete_vhash (referenced_hash);
	return true;
}


static bool WriteObject(
		BBaseOpcodeHandler * handler, 
		BStreamFileToolkit * tk, 
		struct vlist_s *serialization_buffer_list) 
{
    TK_Status status = TK_Normal;
	SerializationNode *node = (SerializationNode *)vlist_peek_last(serialization_buffer_list);

	do {
		status = handler->Write(*tk);
		node->m_Used = tk->CurrentBufferLength();
		if (status == TK_Pending) {
			node = new SerializationNode (HMAT_BUFFER_SIZE);
			if (tk->PrepareBuffer(node->m_Buffer, node->m_Allocated) == TK_Error) {
				tk->Error ("error reported by PrepareBuffer call in material library");
				return false;
			}
			vlist_add_last (serialization_buffer_list, (void *)node);
		}
    } while (status == TK_Pending);
	if (status == TK_Error)
		return false;
	return true;
}


bool HMaterialLibrary::GenerateSerializationBufferList (
		const char *materials)
{
	SerializationNode *node;
	TK_Status status;
	BStreamFileToolkit tk;
	TK_Comment metadata;
	HTK_One_Material mat, env;
	TK_Terminator terminator(TKE_Termination);
	m_SerializationBufferList = new_vlist (malloc, free);

	node = new SerializationNode (HMAT_BUFFER_SIZE);
	vlist_add_last (m_SerializationBufferList, (void *)node);
	status = tk.PrepareBuffer(node->m_Buffer, node->m_Allocated);
	if (status == TK_Error)
		return false;

	metadata.SetComment (materials);
	if (!WriteObject (&metadata, &tk, m_SerializationBufferList))
		return false;

	env.Interpret (tk, this, m_CurrentEnvironment);
	if (!WriteObject (&env, &tk, m_SerializationBufferList))
		return false;

	char const *ptr = materials;
	char one_material[HMAT_BUFFER_SIZE];
	for (;;) {
		int i = 0;
		if (*ptr == '\0')
			break;
		for (;;) {
			one_material[i] = *ptr++;
			if (one_material[i] == ';') {
				one_material[i] = '\0';
				break;
			}
			else
				i++;
		} 
		mat.Reset();
		mat.Interpret (tk, this, one_material);
		if (!WriteObject (&mat, &tk, m_SerializationBufferList))
			return false;
	}
	if (!WriteObject (&terminator, &tk, m_SerializationBufferList))
		return false;

	return true;
}


bool HMaterialLibrary::InitWrite (
		const char *list, 
		int *size)
{
	int total = 0;
	SerializationNode *node;

	if (m_SerializationBufferList == NULL) {
		if (!GenerateSerializationBufferList (list))
			return false;
	}
	vlist_reset_cursor (m_SerializationBufferList);
	while ((node = (SerializationNode *)vlist_peek_cursor(m_SerializationBufferList)) != NULL) {
		total += node->m_Used;
		vlist_advance_cursor (m_SerializationBufferList);
	}
	*size = total;
	return true;
}


bool HMaterialLibrary::InitRead (int version)
{
	if (m_tk == NULL)
		m_tk = new BStreamFileToolkit;
	if (m_MaterialSegmentKey == INVALID_KEY)
		Init();
	m_tk->SetOpcodeHandler (TKE_Material, new HTK_One_Material(this));
	m_tk->SetReadVersion (version);
	return true;
}



TK_Status HMaterialLibrary::ParseBuffer (
		char const *buffer, 
		int buffer_size)
{
	TK_Status status;
	status = m_tk->ParseBuffer (buffer, buffer_size);
	if (status == TK_Complete || status == TK_Error) {
		delete m_tk;
		m_tk = NULL;
	}
	return status;
}


class TK_One_Material_Metadata : public TK_Comment
{
public:
	TK_One_Material_Metadata(char const *metadata) : TK_Comment() { set_comment (metadata); };
};


static int find_largest_image ()
{
	int w, h;
	HC_KEY key;
	int largest_dimension = -1;

	HC_Begin_Contents_Search (".", "images"); {
		while (HC_Find_Contents (NULL, &key)) {
			HC_Show_Image_Size (key, NULL, NULL, NULL, NULL, &w, &h);
			if (w > largest_dimension)
				largest_dimension = w;
			if (h > largest_dimension)
				largest_dimension = h;
		}
	} HC_End_Contents_Search ();
	return largest_dimension;
}


static void save_and_remove_styles(struct vlist_s *list)
{
	HC_KEY key;
	char tempstr[HMAT_BUFFER_SIZE];
	char *path;

	HC_Begin_Contents_Search (".", "styles"); {
		while (HC_Find_Contents (NULL, &key)) {
			HC_Show_Style_Segment (key, tempstr);
			path = new char[strlen (tempstr) + 1];
			strcpy (path, tempstr);
			vlist_add_last (list, (void *)path);
			HC_Delete_By_Key (key);
		}
	} HC_End_Contents_Search ();
}


static void delete_normalmap()
{
	HC_KEY image_key = INVALID_KEY;
	char tempstr[HMAT_BUFFER_SIZE];
	int length = HC_Show_One_User_Data (HEIGHTFIELD_INDEX, tempstr, HMAT_BUFFER_SIZE);
	if (length != 0) {
		image_key = find_image_by_name (".", H_FORMAT_TEXT ("%s_image", tempstr));
		if (image_key != INVALID_KEY) {
			HC_Delete_By_Key (image_key);
		}
	}
}


static void restore_normalmap()
{
	char tempstr[HMAT_BUFFER_SIZE];
	int length = HC_Show_One_User_Data (HEIGHTFIELD_INDEX, tempstr, HMAT_BUFFER_SIZE);

	if (length == 0) 
		return;

	float scale;
	int length2 = HC_Show_One_User_Data (HEIGHTFIELD_SCALE_INDEX, (void *)&scale, HMAT_BUFFER_SIZE);
	if (length2 == 0)
		scale = 1;

	create_normalmap_from_heightfield (INVALID_KEY, tempstr, scale);
}



static void restore_styles(struct vlist_s *list)
{
	char *path;
	while ((path = (char *)vlist_remove_first (list)) != NULL) {
		HC_Style_Segment (path);
		delete [] path;
	}
}


bool HMaterialLibrary::Compile (
		const char *name,
		void *buffer_list)
{
	HStreamFileToolkit tk;
	TK_Status status;
	char metadata[HMAT_BUFFER_SIZE];
	wchar_t full_path[HMAT_BUFFER_SIZE];
	wchar_t *ptr;
	MaterialDescriptor *md;
	bool environment = false;
	int resolution;
	struct vlist_s *styles_list;

	if (vhash_lookup_string_key_item (m_MaterialHash, name, (void **)&md) == VHASH_STATUS_SUCCESS) {
		if (!EnsureMaterialInitialized (name))
			return false;
	}
	else if (vhash_lookup_string_key_item (m_EnvironmentHash, name, (void **)&md) == VHASH_STATUS_SUCCESS) {
		environment = true;
		if (!EnsureEnvironmentInitialized (name))
			return false;
	}
	else
		return false;
		
	HC_Open_Segment (GetContainerSegment()); {
		HC_Open_Segment (name); {
			if (environment)
				strcpy (metadata, H_FORMAT_TEXT ("environment name=%s", name));
			else
				strcpy (metadata, H_FORMAT_TEXT ("material name=%s", name));
			resolution = find_largest_image ();
			if (resolution > 0)
				strcat (metadata, H_FORMAT_TEXT (", resolution=%d", resolution));
			if (md->m_Instancee != NULL)
				strcat (metadata, H_FORMAT_TEXT (", base=%s", md->m_Instancee));
			tk.SetPrewalkHandler(new TK_One_Material_Metadata(metadata));
			styles_list = new_vlist (malloc, free);
			save_and_remove_styles (styles_list);
			delete_normalmap ();

			if (buffer_list == NULL) {
				//compile to file
				wcscpy (full_path, m_MaterialDirectory);
				wcscat (full_path, L"/");
				ptr = full_path + wcslen(full_path);
				H_UTF8 name_utf8;
				name_utf8.encodedText (name);
				wcscpy (ptr, H_WCS(name_utf8).encodedText());
				while ((ptr = wcschr (ptr, L'/')) != NULL)
					*ptr = '.';
				wcscat (full_path, L".hsf");
				md->SetHSF (full_path);
				status = HTK_Write_Stream_File (full_path, &tk);
			}
			else {
				//compile to list of buffers
				VSerializationNodeList *nodelist = (VSerializationNodeList *)buffer_list;
				do {
					SerializationNode *node = new SerializationNode (HMAT_BUFFER_SIZE);
					nodelist->AddLast (node);
					status = tk.GenerateBuffer (node->m_Buffer, node->m_Allocated, node->m_Used);
				} while (status == TK_Pending);
			}

			restore_normalmap();
			restore_styles(styles_list);
			delete_vlist (styles_list);

		} HC_Close_Segment ();
	} HC_Close_Segment ();

	return status == TK_Complete;
}


bool HMaterialLibrary::Compile (
		const char *name)
{
	return Compile (name, NULL);
}


////////////////////////////////////////////////////////////////////////////////


TK_Status HTK_Material_Composite::Interpret(
		BStreamFileToolkit &tk, 
		ID_Key key, 
		int variant) 
{
	UNREFERENCED(key);
	UNREFERENCED(variant);

	char referenced_materials[HMAT_BUFFER_SIZE];
	int serialized_size;
	struct vlist_s *list;
	SerializationNode *node;
	TK_Status status;

	if (m_HMat == NULL)
		return TK_Normal; //nothing to do
	if (!m_HMat->FindReferencedMaterials (m_Root, referenced_materials, HMAT_BUFFER_SIZE))
		return tk.Error ("error reported from HMaterialLibrary::FindReferencedMaterials");
	if (referenced_materials[0] == '\0') {
		serialized_size = 0;
		return TK_Normal;
	}
	if (!m_HMat->InitWrite (referenced_materials, &serialized_size))
		return tk.Error ("error reported from HMaterialLibrary::ComputeSerializedSize");
	H_ASSERT (m_HMat->m_SerializationBufferList != NULL);
	list = m_HMat->m_SerializationBufferList;
	vlist_reset_cursor (list);
	while ((node = (SerializationNode *)vlist_peek_cursor (list)) != NULL) {
		if ((status = PushUserData (node->m_Buffer, node->m_Used)) != TK_Normal)
			return status;
		vlist_advance_cursor (list);
	}
	H_ASSERT (m_total_size == serialized_size);
	return TK_Normal;
}



TK_Status HTK_Material_Composite::Execute(BStreamFileToolkit & tk) 
{
	TK_Status status;
	const char *block;
	int size;

	m_HMat->InitRead(tk.GetVersion());
	do {
		status = GetBlock (&block, &size);
		if (size == 0)
			return tk.Error ("invalid material data");
		if (status != TK_Normal && 
			status != TK_Complete )
			return tk.Error ("error reading material data");
		status = m_HMat->ParseBuffer (block, size);
		if (status != TK_Normal && 
			status != TK_Complete && 
			status != TK_Pending)
			return tk.Error ("error parsing material data");
	} while (status != TK_Complete);

	return TK_Normal;
};


////////////////////////////////////////////////////////////////////////////////


void HTK_One_Material::Init ()
{
	m_HMat = NULL;
	m_MaterialDescriptor = 0;
	m_Name = 0;
	m_NameLength = 0;
	m_Instancee = 0;
	m_InstanceeLength = 0;
	m_TweakCount = 0;
	m_TweakNameLengths = 0;
	m_TweakNames = 0;
	m_TweakTypes = 0;
	m_Tweaks = 0;
	m_subop = 0;
	m_substage = 0;
	m_substage2 = 0;
	m_HSF = 0;
	m_subtk = 0;
	m_EmbeddingBufferUsed = 0;
	m_EmbeddedLength = 0;
	m_HeightFieldScale = 0;
}


HTK_One_Material::HTK_One_Material() : BBaseOpcodeHandler(TKE_Material) {
	//initialize for writing
	Init();
}


HTK_One_Material::HTK_One_Material(HMaterialLibrary *hmat) : BBaseOpcodeHandler(TKE_Material) {
	//initialize for reading
	Init();
	m_HMat = hmat; //on write side, needs to be passed to Interpret.  Read side needs it here.
}


HTK_One_Material::~HTK_One_Material() {
	Reset ();
}

TK_Status HTK_One_Material::Interpret(
		BStreamFileToolkit &tk, 
		HMaterialLibrary *hmat, 
		const char *name)
{
	bool embedded = false;
	MaterialDescriptor *md = NULL;
	char tempstr[HMAT_BUFFER_SIZE];
	int lookup_status = vhash_lookup_string_key_item (hmat->m_MaterialHash, name, (void **)&md);
	if (lookup_status != VHASH_STATUS_SUCCESS) {
		lookup_status = vhash_lookup_string_key_item (hmat->m_EnvironmentHash, name, (void **)&md);
		if (lookup_status != VHASH_STATUS_SUCCESS)
			return tk.Error ("invalid material in list given to HTK_One_Material::Interpret");
		else
			m_subop |= OMF_Environment;
	}
	m_MaterialDescriptor = md;

	m_NameLength = (int) strlen (name) + 1;
	m_Name = new char [m_NameLength];
	strcpy (m_Name, name);

	if (hmat->GetWriteFlags() & HMaterialLibrary::Embedded)
		embedded = true;
	if (embedded) {
		m_subop |= OMF_Embedded;
		if (!md->m_HSF_filename && !md->m_HSF_buffers) {
			md->m_HSF_buffers = new VSerializationNodeList;
			if (!hmat->Compile (name, md->m_HSF_buffers))
				return tk.Error (H_FORMAT_TEXT ("compile error reported by %s", name));
		}
		HC_Open_Segment_By_Key (hmat->m_MaterialSegmentKey); {
			HC_Open_Segment (name); {
				int length = HC_Show_One_User_Data (HEIGHTFIELD_INDEX, tempstr, HMAT_BUFFER_SIZE);
				if (length != 0) {
					int length2 = HC_Show_One_User_Data (HEIGHTFIELD_SCALE_INDEX, &m_HeightFieldScale, sizeof(float));
					if (length2)
						m_subop |= OMF_Height_Field;
				}
			} HC_Close_Segment ();
		} HC_Close_Segment ();
	}

	if (md->m_Instancee) {
		m_subop |= OMF_Instancee;
		m_InstanceeLength = (int) strlen (md->m_Instancee) + 1;
		m_Instancee = new char [m_InstanceeLength];
		strcpy (m_Instancee, md->m_Instancee);
	}

	//get tweaks, if any
	if (!(m_subop & OMF_Environment)) {
		HC_Open_Segment_By_Key (hmat->m_MaterialSegmentKey); {
			HC_Open_Segment (name); {
				int temparray[HMAT_BUFFER_SIZE];
				int length = HC_Show_One_User_Data (TWEAK_DATA_USER_INDEX, temparray, HMAT_BUFFER_SIZE * sizeof(int));
				int arraycount = length>>2;
				int veclen;
				int const *iptr;
				float const *fptr;
				int i, j;
				int tweak_index;
				if (arraycount != 0) {
					m_subop |= OMF_Tweaks;
					m_TweakCount = temparray[0];
					m_Tweaks = new float *[m_TweakCount];
					m_TweakNameLengths = new int [m_TweakCount];
					m_TweakNames = new char *[m_TweakCount];
					m_TweakTypes = new char [m_TweakCount];
					iptr = &temparray[1];
					for (i = 0 ; i < m_TweakCount ; i++) {
						tweak_index = iptr[0];
						hmat->GetTweakableByIndex (name, tweak_index, tempstr, NULL);
						m_TweakNameLengths[i] = (int) strlen (tempstr) + 1;
						m_TweakNames[i] = new char [m_TweakNameLengths[i]];
						strcpy (m_TweakNames[i], tempstr);
						veclen = iptr[1];
						m_TweakTypes[i] = (char)iptr[1];
						m_Tweaks[i] = new float [iptr[1]];
						fptr = (float *)&iptr[2];
						for (j = 0 ; j < veclen ; j++)
							m_Tweaks[i][j] = fptr[j];
						iptr += veclen + 2;
					}
				}
			} HC_Close_Segment ();
		} HC_Close_Segment ();
	}
	return TK_Normal;
}


TK_Status HTK_One_Material::Write (BStreamFileToolkit & tk) {
	TK_Status       status = TK_Normal;

	H_ASSERT (!tk.GetAsciiMode()); //not implemented
	switch (m_stage) {
		case 0: {
			if ((status = PutOpcode (tk, 0)) != TK_Normal)
				return status;
			m_stage++;
		}   nobreak;

		case 1: {
			if ((status = PutData (tk, m_NameLength)) != TK_Normal)
				return status;
			m_stage++;
		}   nobreak;

		case 2: {
			if ((status = PutData (tk, m_Name, m_NameLength)) != TK_Normal)
				return status;
			m_stage++;
		}   nobreak;

		case 3: {
			if ((status = PutData (tk, m_subop)) != TK_Normal)
				return status;
			m_stage++;
		}   nobreak;

		case 4: {
			if (m_subop & OMF_Instancee) {
				if ((status = PutData (tk, m_InstanceeLength)) != TK_Normal)
					return status;
			}
			m_stage++;
		}   nobreak;

		case 5: {
			if (m_subop & OMF_Instancee) {
				if ((status = PutData (tk, m_Instancee, m_InstanceeLength)) != TK_Normal)
					return status;
			}
			m_stage++;
		}   nobreak;


		case 6: {
			if (m_subop & OMF_Tweaks) {
				if ((status = PutData (tk, m_TweakCount)) != TK_Normal)
					return status;
			}
			m_substage = 0;
			m_substage2 = 0;
			m_stage++;
		}   nobreak;

		case 7: {
			if (m_subop & OMF_Tweaks) {
				while (m_substage < m_TweakCount) {
					switch (m_substage2) {
						case 0: {
							if ((status = PutData (tk, m_TweakNameLengths[m_substage])) != TK_Normal)
								return status;
							m_substage2++;
						} nobreak;

						case 1: {
							if ((status = PutData (tk, m_TweakNames[m_substage],  m_TweakNameLengths[m_substage])) != TK_Normal)
								return status;
							m_substage2++;
						} nobreak;

						case 2: {
							if ((status = PutData (tk, m_TweakTypes[m_substage])) != TK_Normal)
								return status;
							m_substage2++;
							m_progress = 0;
						} nobreak;

						default: {
							int floatcount = m_TweakTypes[m_substage]; //always floats for now;
							H_ASSERT (floatcount <= 4);
							while (m_progress < floatcount) {
								float val = m_Tweaks[m_substage][m_progress];
								if ((status = PutData (tk,val)) != TK_Normal)
									return status;
								m_progress++;
							}
							m_substage2 = 0;
						} break;
					}
					m_substage++;
				}
			}
			if (m_subop & OMF_Embedded) {
				MaterialDescriptor *md = (MaterialDescriptor *)m_MaterialDescriptor;
				if (md->m_HSF_filename) {
					m_HSF = wfopen(md->m_HSF_filename, L"rb");
					if (m_HSF == NULL)
						return tk.Error(H_FORMAT_TEXT ("cannot embed hsf file for material %s", m_Name));
					fseek(m_HSF,0,SEEK_END);
					m_EmbeddedLength = ftell(m_HSF);
					fseek(m_HSF,0,SEEK_SET);
				}
				else {
					SerializationNode *node;
					VSerializationNodeList *list = md->m_HSF_buffers;
					m_EmbeddedLength = 0;
					list->ResetCursor();
					while ((node = list->PeekCursor()) != NULL) {
						m_EmbeddedLength += node->m_Used;
						list->AdvanceCursor();
					}
					list->ResetCursor();
				}
			}
			m_stage++;
		}   nobreak;

		case 8: {
			if (m_subop & OMF_Embedded) {
				if ((status = PutData (tk, m_EmbeddedLength)) != TK_Normal)
					return status;
				m_progress = 0;
			}
			m_stage++;
		}   nobreak;

		case 9: {
			if (m_subop & OMF_Embedded) {
				if (m_HSF != NULL) {
					for (;;) {
						if (m_EmbeddingBufferUsed == 0) {
							m_EmbeddingBufferUsed = (int) fread(m_EmbeddingBuffer,sizeof(unsigned char),ONEMATBUFSIZE,m_HSF);
							if (m_EmbeddingBufferUsed == 0) {
								fclose (m_HSF);
								m_HSF = NULL;
								break;
							}
						}
						if ((status = PutData (tk, m_EmbeddingBuffer, m_EmbeddingBufferUsed)) != TK_Normal)
							return status;
						m_progress += m_EmbeddingBufferUsed;
						m_EmbeddingBufferUsed = 0;
					}
					H_ASSERT (m_progress == m_EmbeddedLength);
				} 
				else {
					SerializationNode *node;
					VSerializationNodeList *list = ((MaterialDescriptor *)m_MaterialDescriptor)->m_HSF_buffers;
					int chunk;

					while ((node = list->PeekCursor()) != NULL) {
						chunk = node->m_Used - m_progress;
						if (chunk > ONEMATBUFSIZE)
							chunk = ONEMATBUFSIZE;
						if ((status = PutData (tk, node->m_Buffer + m_progress, chunk)) != TK_Normal)
							return status;
						m_progress += chunk;
						if (m_progress == node->m_Used) {
							list->AdvanceCursor();
							m_progress = 0;
						}
					}
				}
			}
			m_stage++;
		}   nobreak;

		case 10: {
			if (m_subop & OMF_Height_Field) {
				if ((status = PutData (tk, m_HeightFieldScale)) != TK_Normal)
					return status;
			}
			m_stage++;
		}   nobreak;

		case 999: {
			m_stage = -1;
		} break;

		default:
			return tk.Error("internal error in HTK_One_Material::Write");
	}
	return status;
}


TK_Status HTK_One_Material::Read (BStreamFileToolkit & tk) {
	TK_Status       status = TK_Normal;

	H_ASSERT (!tk.GetAsciiMode()); //not implemented
	switch (m_stage) {
		case 0: {
			if ((status = GetData (tk, m_NameLength)) != TK_Normal)
				return status;
			if (m_NameLength == 0)
				return tk.Error ("invalid name length in HTK_One_Material::read");
			m_Name = new char[m_NameLength];
			m_stage++;
		}   nobreak;

		case 1: {
			if ((status = GetData (tk, m_Name, m_NameLength)) != TK_Normal)
				return status;
			m_stage++;
		}   nobreak;

		case 2: {
			if ((status = GetData (tk, m_subop)) != TK_Normal)
				return status;
			m_stage++;
		}   nobreak;

		case 3: {
			if (m_subop & OMF_Instancee) {
				if ((status = GetData (tk, m_InstanceeLength)) != TK_Normal)
					return status;
				if (m_InstanceeLength == 0)
					return tk.Error ("invalid instancee length in HTK_One_Material::read");
				m_Instancee = new char[m_InstanceeLength];
			}
			m_stage++;
		}   nobreak;

		case 4: {
			if (m_subop & OMF_Instancee) {
				if ((status = GetData (tk, m_Instancee, m_InstanceeLength)) != TK_Normal)
					return status;
			}
			m_stage++;
		}   nobreak;


		case 5: {
			if (m_subop & OMF_Tweaks) {
				if ((status = GetData (tk, m_TweakCount)) != TK_Normal)
					return status;
			}
			m_TweakNameLengths = new int[m_TweakCount];
			m_TweakNames = new char *[m_TweakCount];
			m_TweakTypes = new char[m_TweakCount];
			m_Tweaks = new float *[m_TweakCount];
			for (int i = 0 ; i < m_TweakCount ; i++)
				m_Tweaks[i] = new float [4];
			m_substage = 0;
			m_substage2 = 0;
			m_stage++;
		}   nobreak;

		case 6: {
			if (m_subop & OMF_Tweaks) {
				while (m_substage < m_TweakCount) {
					switch (m_substage2) {
						case 0: {
							if ((status = GetData (tk, m_TweakNameLengths[m_substage])) != TK_Normal)
								return status;
							if (m_TweakNameLengths[m_substage] == 0)
								return tk.Error ("invalid tweak name length in HTK_One_Material::read");
							m_TweakNames[m_substage] = new char[m_TweakNameLengths[m_substage]];
							m_substage2++;
						} nobreak;

						case 1: {
							if ((status = GetData (tk, m_TweakNames[m_substage],  m_TweakNameLengths[m_substage])) != TK_Normal)
								return status;
							m_substage2++;
						} nobreak;

						case 2: {
							if ((status = GetData (tk, m_TweakTypes[m_substage])) != TK_Normal)
								return status;
							m_substage2++;
							m_progress = 0;
						} nobreak;

						default: {
							int floatcount = m_TweakTypes[m_substage]; //always floats for now;
							H_ASSERT (floatcount <= 4);
							while (m_progress < floatcount) {
								float val;
								if ((status = GetData (tk,val)) != TK_Normal)
									return status;
								m_Tweaks[m_substage][m_progress] = val;
								m_progress++;
							}
							m_substage2 = 0;
						} break;
					}
					m_substage++;
				}
			}
			m_stage++;
		}   nobreak;

		case 7: {
			if (m_subop & OMF_Embedded) {
				if ((status = GetData (tk, m_EmbeddedLength)) != TK_Normal)
					return status;
				m_subtk = (void *)new HStreamFileToolkit;
				HC_Open_Segment_By_Key (m_HMat->m_MaterialSegmentKey);
				HC_Open_Segment (m_Name);
				HC_Flush_Contents (".", "everything");
			}
			m_progress = 0;
			m_stage++;
		}   nobreak;

		case 8: {
			if (m_subop & OMF_Embedded) {
				int chunk;
				char buffer[4096];
				HStreamFileToolkit *subtk = (HStreamFileToolkit *)m_subtk;
				while (m_progress < m_EmbeddedLength) {
					chunk = 4096;
					if (m_progress + chunk > m_EmbeddedLength)
						chunk = m_EmbeddedLength - m_progress;
					if ((status = GetData (tk, buffer, chunk)) != TK_Normal) {
						if (status == TK_Error) {
							HC_Close_Segment ();
							HC_Close_Segment ();
						}
						return status;
					}
					if (subtk->ParseBuffer (buffer, chunk) == TK_Error)
						return tk.Error(H_FORMAT_TEXT ("parse error in embedded hsf data for %s", m_Name));
					m_progress += chunk;
				}
			}
			m_stage++;
		}   nobreak;

		case 9: {
			if (m_subop & OMF_Embedded) {
				if (!(m_subop & OMF_Environment))
					HC_Style_Segment_By_Key (m_HMat->m_CurrentEnvironmentKey);
				HC_Close_Segment ();
				HC_Close_Segment ();
			}
			m_stage++;
		}   nobreak;

		case 10: {
			if (m_subop & OMF_Height_Field) {
				if ((status = GetData (tk, m_HeightFieldScale)) != TK_Normal)
					return status;
			}
			m_stage++;
		}   nobreak;

		case 999: {
			m_stage = -1;
		} break;

		default:
			return tk.Error("internal error in HTK_One_Material::Read");
	}
	return status;

} // end HTK_One_Material read


TK_Status HTK_One_Material::Execute (BStreamFileToolkit & tk) {
	if (m_Instancee != NULL) {
		HC_Open_Segment_By_Key (m_HMat->m_MaterialSegmentKey); {
			if (!HC_QShow_Existence (m_Name, "self")) {
				char tempstr[HMAT_BUFFER_SIZE];
				m_HMat->CloneMaterial (m_Instancee, tempstr);
				m_HMat->RenameMaterial (tempstr, m_Name);
			}
			else {
				m_HMat->EnsureMaterialInitialized (m_Instancee);
				HC_KEY basekey = HC_KCreate_Segment (m_Instancee);
				HC_Open_Segment (m_Name); {
					HC_Style_Segment_By_Key (basekey);
				} HC_Close_Segment ();
			}
		} HC_Close_Segment ();
	}

	if (m_subop & OMF_Environment) {
		m_HMat->SetEnvironment (m_Name);
	}
	else {
		m_HMat->EnsureMaterialInitialized (m_Name);
		if (m_subop & OMF_Tweaks) {
			for (int i = 0 ; i < m_TweakCount ; i++) {
				int index;
				if (m_HMat->GetTweakableByName (m_Name, m_TweakNames[i], &index, NULL)) {
					m_HMat->Tweak (m_Name, index, m_Tweaks[i]);
				}
			}
		}
		if (m_subop & OMF_Height_Field) {
			HC_Open_Segment_By_Key (m_HMat->m_MaterialSegmentKey); {
				HC_Open_Segment (m_Name); {
					//need to set it from HSF data rather than letting it hitch a ride so that the toolkit can resolve endianness for us
					HC_Set_User_Data (HEIGHTFIELD_SCALE_INDEX, &m_HeightFieldScale, sizeof(float));
					restore_normalmap();
				} HC_Close_Segment ();
			} HC_Close_Segment ();
		}
	}
	HC_Open_Segment_By_Key (m_HMat->m_MaterialSegmentKey); {
		HC_Define_Named_Style (m_Name, m_Name);
	} HC_Close_Segment ();
	UNREFERENCED(tk);
	return TK_Normal;
}


void HTK_One_Material::Reset () 
{
	delete [] m_Name;
	m_Name = NULL;
	m_NameLength = 0;

	if (m_Instancee) {
		delete [] m_Instancee;
		m_Instancee = NULL;
		m_InstanceeLength = 0;
	}

	m_MaterialDescriptor = NULL;
	for (int i = 0 ; i < m_TweakCount ; i++) {
		delete [] m_Tweaks[i];
		delete [] m_TweakNames[i];
	}
	delete [] m_Tweaks;
	delete [] m_TweakTypes;
	delete [] m_TweakNameLengths;
	delete [] m_TweakNames;
	m_Tweaks = NULL;
	m_TweakTypes = NULL;
	m_TweakNameLengths = NULL;
	m_TweakNames = NULL;
	m_TweakCount = 0;
	m_subop = 0;
	m_substage = 0;
	m_substage2 = 0;
	if (m_HSF) {
		fclose (m_HSF);
		m_HSF = NULL;
	}
	m_EmbeddingBufferUsed = 0;

	HStreamFileToolkit *subtk = (HStreamFileToolkit *)m_subtk;
	delete subtk;
	m_subtk = NULL;

	BBaseOpcodeHandler::Reset();
}
