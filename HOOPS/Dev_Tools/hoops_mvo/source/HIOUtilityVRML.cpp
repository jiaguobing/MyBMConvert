// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _WIN32_WCE

// standard includes
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <wchar.h>
 
#include "HIOUtilityVRML.h"

#include "HUtility.h"
#include "HUtilityGeometryCreation.h"
#include "varray.h"

#include "vhash.h"
#include "vlist.h"

#ifdef USE_IMAGE_MAGICK 
#include "HIOUtilityMagick.h"
#endif

#ifdef VRML10_READ_SUPPORT
#include "HIOUtilityIV.h"
#endif


// Refills the read buffer if it has been already processed 
// and then resets the fposition. Call this whenever you 
// are processing contents of fbuffer in a loop.
#define ENSURE_BUFFER_READ  if (fposition>=buffersize)	\
							{							\
								if (fill_buffer())		\
									fposition=0;		\
								else					\
									return false;		\
							}							

class VNode;

//-----------------------------------------------------------------------------
class V2HUtils
{
public:

	V2HUtils(HInputHandler* handler)
	{
		fposition = 1000;
		fbuffer = 0;
		buffersize = -1;
		fhandle = 0;

		save_fposition = 0;
		save_fbuffer = 0;
		save_fbuffer_length = 0;
		m_file_size = 0;
		m_pHandler = handler;
	}

	int fposition;			// current position in FBUFFER
	unsigned char *fbuffer;	// File buffer
	int buffersize;			// chunk to be read at a time
	FILE *fhandle;			// wrl file

	int save_fposition;
	char *save_fbuffer;
	int save_fbuffer_length;
	unsigned long  m_file_size;
	HInputHandler* m_pHandler;

	// common utility functions
	bool NextToken(char *token);
	bool NextToken(char *token, int skip);

	VNode* FindNode(bool *closebracket=0);
	VNode* GetNodeForToken(char* token);
	VNode* FindNodeSkipDEF(bool *closebracket=0);
	VNode* FindNodeGetDEForUSE(char* def_or_use_name, bool &is_def, bool &is_use);
	VNode* FindNodeGetDEForUSEorNULL(char* def_or_use_name, bool &ref_is_def, bool &ref_is_use, bool &ref_is_null);
	int  HexStr2Int(const char *hex_str);
	bool Get1Float(float &a) ;
	bool Get3Floats(float &a, float &b, float &c);
	bool Get4Floats(float &a, float &b, float &c, float &d);
	bool GetBool(bool& true_or_false);
	bool GetString(char *string);
	bool GetQuotedString(char *token);
	bool RemoveQuotes(char* string);
	bool BuildPointList(int &pnum, int &pmax, float **llist);
	bool BuildTexPointList(int& tc_num, float*& tc_list);
	bool BuildFaceList(int &, int &, int **, int &);
	bool BuildFaceList(int &pnum, int &pmax, int **llist);
	bool BuildMatrialIndex(int &mnum, int &mmax, int **mlist, bool ifPerVertex);
	bool Token2RGBA(char * token, const int n_components, unsigned char * RGBA);
	bool GetStringInSquareBracket(char *string_in_bracket);
	bool fill_buffer();
	void MarkBuffer();
	void SaveBuffer();
	bool SkipBracket();
	bool ReadLine();
	bool vrmltoHOOPS(const wchar_t *filename, HC_KEY key);

	// here till these functions find a place in HUtility
	static void	GetViewplaneVectors(HC_KEY segment_key, float * view, float * up);
	static void	SetupModellingMatrix(HVector axis, HVector ref, HVector side, HPoint pos);
};
//-----------------------------------------------------------------------------

HFileInputResult HIOUtilityVRML::FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options)
{
	UNREFERENCED(options);

	if(!FileName) return InputBadFileName;
	if(key == INVALID_KEY) return InputFail;

#ifdef VRML10_READ_SUPPORT
	//testing to see if VRML file is v1.0
	//if so, send it to Inventor reader for import
	FILE *fhandle = fopen(FileName,"rb");
	char vrml10[MVO_BUFFER_SIZE];
	fscanf(fhandle, "%[^\n]%*c", vrml10);
	if(strstr(vrml10, "#VRML V1.0"))
	{
		HIOUtilityIV vrml10read;
		vrml10read.FileInputByKey(FileName, key, NULL);
	}
	fclose(fhandle);
#endif
	SetStartingInput();

	V2HUtils v2hread(this);
	bool result;
	HCLOCALE(result = v2hread.vrmltoHOOPS(FileName, key));

	SetFinishedInput();

	if(result) 
		return InputOK;
	else
		return InputFail;
}

//-----------------------------------------------------------------------------
// macros
#define VERIFY_NEXT_TOKEN(__token_, v2hread)		\
		if (!v2hread->NextToken(__token_))			\
			return 0;

#define assert_NEXT_TOKEN(__token_, __expected__, v2hread)	\
		if (!v2hread->NextToken(__token_))					\
			return 0;								\
		if( !(streq(__token_,__expected__)))		\
		{											\
			char __msg[MVO_BUFFER_SIZE];				\
			strcat(__msg, "Expecting ");				\
			strcat(__msg, __expected__);				\
			return false;							\
		}
#define ASSERT(x) 

// hoops segment without name
#define ANONYMUS_NODE		""

// HOOPS path to store VRML def nodes
#define DEF_NODE_LIB		"?Include Library/VRML/Def/"
#define VRML_TEXTURE_LIB	"/Include Library/VRML/Textures"

#define VRML_TRUE_STR		"TRUE"
#define VRML_FALSE_STR		"FALSE"

#define streq(a,b)  (!strcmp(a,b))
#define BUFFERAD 100
#define TOKEN_BUFFER_SIZE	1024
#define VRML_TEXTURE_BASE_NAME "vrml_texture"

#define NOTINTOKEN 0
#define INTOKEN 1
#define COMMENT 2
//-----------------------------------------------------------------------------

class HRotation 
{ 
public:
	float		x;	//!< The x-coordinate of a 3-dimensional point.
	float		y;	//!< The y-coordinate of a 3-dimensional point.
	float		z;	//!< The z-coordinate of a 3-dimensional point.
	float		rot;
	/* 
	Set packs an HPoint object with coordinate values.
	*/
	inline void Set(float X,float Y,float Z, float r) { x=X,y=Y,z=Z; rot = r; }
	/* 
	Add increases or decreases the coordinate values of an existing HPoint object.
	*/
	inline void Add(float X,float Y,float Z=0.0) { x+=X;y+=Y;z+=Z; }
}; 
//-----------------------------------------------------------------------------

class VNode 
{
public:

	VNode(char *name = 0);
	virtual ~VNode();

	virtual bool Create(HC_KEY key, V2HUtils *v2hread) = 0;

	// You will need to implement this if you think your node handler may be DEF'd
	// refer to VCoord for an example
	virtual void CleanUp() = 0;
	
	HC_KEY KCreateNamedSubsegment( HC_KEY parent_key, const char* alias = 0 );
	HC_KEY CreateNamedSubsegment( const char* segmentPath, const char* segmentName, const char* alias = 0 );
	bool IncludeNamedSegment( HC_KEY parent_key, const char* alias );
	bool IncludeNamedSegment( HC_KEY parentKey, const char* segmentPath, const char* segmentName );

	char* m_name;
	char m_token[255];
	bool m_segopen;
	VNode *m_child_node;
	int m_level;
	HC_KEY m_segkey;

	static wchar_t wrl_file_path[MVO_BUFFER_SIZE];

	// DEF names and the corresponding VNode* handlers of all the DEF nodes 
	// we have encountered so far
	static struct vhash_s * def_handlers;

};


wchar_t VNode::wrl_file_path[] = {L""};
//-----------------------------------------------------------------------------

class VIndexedFaceSet : public VNode
{
public:
	// coordinates
	float *plist;
	int plist_length;

	// face list
	int *flist;
	int flist_length;
	int flist_max;
	int faces_number;

	// texture coordinates
	float *tc_list;
	int tclist_length;

	// texture coordinate indices
	int *texture_index_list;
	int texture_index_list_length;
	int texture_index_list_max;

	float *clist;
	int clist_length;

	// normals
	float *normals_point_list;
	int normals_point_list_length;

	// normal list
	int *normal_index_list;
	int normal_index_list_length;
	int normal_index_list_max;
	int norm_num;

	// color Indices
	int  *color_index_list;
	int color_index_list_length;
	int color_index_max;

	// color list 
	float *color_list;
	int color_list_length; 

	float creaseAngle;

	VIndexedFaceSet() : VNode() 
	{
		plist = 0; plist_length = 0;
		flist = 0; flist_length = 0; flist_max = 0; 

		tc_list = 0; tclist_length = 0;
		texture_index_list = 0; texture_index_list_length = 0; texture_index_list_max = 0;

		normals_point_list = 0; normals_point_list_length = 0; 
		normal_index_list = 0; normal_index_list_length = 0; normal_index_list_max = 0; norm_num=0;

		color_list =0; color_list_length=0; 
		color_index_list =0; color_index_list_length =0; color_index_max = 0;

		clist = 0; clist_length = 0; 

		creaseAngle =0.0f;
	};

	virtual ~VIndexedFaceSet(){}
	virtual void CleanUp(){};
	bool Create(HC_KEY key, V2HUtils *v2hread);

	
};
//-----------------------------------------------------------------------------

class VIndexedLineSet : public VNode
{
public:
 	int *flist;
	int flist_length;
	int flist_max;

	float *plist;
	int plist_length;

	int  *color_index_list;
	int color_index_list_length;
	int color_index_max;

	float *color_list;
	int color_list_length;

	VIndexedLineSet();

	virtual ~VIndexedLineSet(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

};
//-----------------------------------------------------------------------------

class VCoordinate: public VNode
{
public:
	float *m_plist;
	int	m_plist_length;
	int	m_plist_max;

	VCoordinate() : VNode () { 	m_plist=0; m_plist_length=0; m_plist_max = 0; };
	void CleanUp(){ H_SAFE_DELETE_ARRAY(m_plist);};

	virtual ~VCoordinate(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);

};
//-----------------------------------------------------------------------------

class VNormal: public VNode
{
public:
	float *n_plist;
	int	n_plist_length;
	int	n_plist_max;

	VNormal() : VNode () { 	n_plist=0; n_plist_length=0; n_plist_max = 0; };
	void CleanUp(){ H_SAFE_DELETE_ARRAY(n_plist);};
	virtual ~VNormal(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);

};
//-----------------------------------------------------------------------------

class VTextureCoordinate: public VNode
{
public:
	float *m_tc_list;
	int	m_tc_list_length;

	VTextureCoordinate() : VNode () 
	{ 	
		m_tc_list = 0; 
		m_tc_list_length = 0; 
	};

	virtual ~VTextureCoordinate(){}
	void CleanUp(){ H_SAFE_DELETE_ARRAY(m_tc_list);};
	bool Create(HC_KEY key, V2HUtils *v2hread);
};
//-----------------------------------------------------------------------------

class VPixelTexture: public VNode
{
public :
	VPixelTexture() : VNode (){};
	virtual ~VPixelTexture(){}
    bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VImageTexture: public VNode
{
public :
	VImageTexture() : VNode() {};
	virtual ~VImageTexture(){}
    bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VShape : public VNode
{
public:
	VShape() : VNode() {};
	virtual ~VShape(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VBox : public VNode
{
public:
	VBox();
	virtual ~VBox();

	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

	HPoint m_min;
	HPoint m_max;
};
//-----------------------------------------------------------------------------

class VSphere : public VNode
{
public:
	VSphere();
	virtual ~VSphere();

	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

	HPoint m_center;
	float m_radius;
	int m_numSides;
	HPoint m_axis;
	HPoint m_ref;
};
//-----------------------------------------------------------------------------

class VCylinder : public VNode
{
public:
	VCylinder();
	virtual ~VCylinder();

	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

	HPoint m_center;
	float m_radius;
	float m_height;
	int m_numSides;
	HPoint m_axis;
	HPoint m_ref;
};
//-----------------------------------------------------------------------------

class VCone : public VNode
{
public:
	VCone();
	virtual ~VCone();

	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

	HPoint m_center;
	float m_radius;
	float m_height;
	int m_numSides;
	HPoint m_axis;
	HPoint m_ref;
};
//-----------------------------------------------------------------------------

class VGroup : public VNode
{
public:
	VGroup() : VNode() { firstrun = true; };
	bool firstrun;
	virtual ~VGroup(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

};
//-----------------------------------------------------------------------------

class VSwitch : public VNode
{
public:
	VSwitch() : VNode() { };
	virtual ~VSwitch(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VTransform : public VNode
{
public:

	VTransform() : VNode() { firstrun = true; 
							 center.Set(0,0,0); 
							 rotation.Set(0,0,1,0); 
							 scale.Set(1,1,1); 
							 scaleRotation.Set(0,0,1,0); 
							 translation.Set(0,0,0); };
	bool firstrun;
	virtual ~VTransform(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};


	HPoint center;
	HPoint scale;
	HPoint translation;
	HRotation rotation;
	HRotation scaleRotation;
};
//-----------------------------------------------------------------------------

class VChildren : public VNode
{
public:
	VChildren();
	virtual ~VChildren();
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VDef : public VNode
{
public:
	VDef();
	virtual ~VDef();
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VUse : public VNode
{
public:
	VUse();
	virtual ~VUse();
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VInline : public VNode
{
public:
	VInline(){};
	virtual ~VInline(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VLOD : public VNode
{
public:
	VLOD() : VNode() { firstrun = true; };
	virtual ~VLOD(){}
	bool firstrun;
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VColor: public VNode
{
public:
	float *m_clist;
	int	m_clist_length;
	int	m_clist_max;
	void CleanUp(){ H_SAFE_DELETE_ARRAY(m_clist);};
	VColor() : VNode () { 	m_clist=0; m_clist_length=0; m_clist_max = 0; };
	virtual ~VColor(){}

	bool Create(HC_KEY key, V2HUtils *v2hread);
};
//-----------------------------------------------------------------------------

class VAppearance: public VNode
{
public:
 	VAppearance() : VNode() { };
	virtual ~VAppearance(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VMaterial: public VNode
{
public:
	float r,g,b;
 
	VMaterial() : VNode() { };
	virtual ~VMaterial(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VDummy: public VNode
{
public:
	float r,g,b;
 
	VDummy() : VNode() { };
	virtual ~VDummy(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------

class VRoute: public VNode
{
public:
 	VRoute() : VNode() { };
	virtual ~VRoute(){}
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};
};
//-----------------------------------------------------------------------------
class VText : public VNode
{
public:
	VText();
	virtual ~VText();
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

protected:
	char* m_string;
	float m_x;
	float m_y;
	float m_z;

	bool ReadString(V2HUtils *v2hread);
};
//-----------------------------------------------------------------------------

class VFontStyle : public VNode
{
public:
	VFontStyle();
	virtual ~VFontStyle();
	bool Create(HC_KEY key, V2HUtils *v2hread);
	virtual void CleanUp(){};

protected:

	char* m_name;
	char* m_size;
	float m_spacing;
	bool m_isHorizontal;
	HPoint m_path;

	bool ReadFamily(V2HUtils *v2hread);
	bool ReadSize(V2HUtils *v2hread);
	bool ReadAlignment(V2HUtils *v2hread);
	bool ReadLeft2Right(V2HUtils *v2hread);
	bool ReadTop2Bottom(V2HUtils *v2hread);
};
//-----------------------------------------------------------------------------
// Function to convert the per face attributes to per index. Actually we are just replicating the attributes as many times as the  
// number of vertices of the face so that each vertex can have its own set of attributes. We need to do this because we are using the function 
// "HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes" which inserts the per vertex attributes on a shell.

bool static  convert_per_face_attrib_to_per_vertex(const int* face_index_list, const int attribute_index_list_length, 
												   const int* attribute_index_list, int*& new_attribute_index_list)
{
	int fidx = 0;
	for(int m = 0; m < attribute_index_list_length - 1; m++)
	{
		int n_pts = new_attribute_index_list[fidx] = face_index_list[fidx];
		fidx++;

		for( int n = 0; n < n_pts; n++ )
		{
			new_attribute_index_list[fidx] = attribute_index_list[m];
			fidx++;
		}
	}
	return true;
}

/* 
  GetViewplaneVectors computes the construction-plane-constrained vectors used for geometry insertion.
  \param seg_key Key to the construction segment
  \param *view A pointer to the double precision view vector.  Passed by reference.
  \param *up A pointer to the double precision up vector. Passed by reference.
*/
void V2HUtils::GetViewplaneVectors(HC_KEY seg_key, float * view, float * up)
{
	HPoint	position, target;
	HVector	up_vector, view_vector, orthogonal;
	float	x, y, z;
	float	ortho[3];

	HC_Open_Segment_By_Key(seg_key);

		HC_Show_Net_Camera_Up_Vector(&x, &y, &z);
		up_vector.Set(x, y, z);

		HC_Show_Net_Camera_Position (&x, &y, &z);
		position.Set(x, y, z);

		HC_Show_Net_Camera_Target (&x, &y, &z);
		target.Set(x, y, z);

	HC_Close_Segment();

	view_vector.Set(position.x - target.x,
					position.y - target.y,
					position.z - target.z);

	// find vector in plane orthogonal to view and up plane
	HC_Compute_Cross_Product(&view_vector, &up_vector, &orthogonal);

	// now do the casting to double
	view[0] = position.x - target.x;
	view[1] = position.y - target.y;
	view[2] = position.z - target.z;

	HC_Compute_Normalized_Vector(view,view);
 
	ortho[0] = orthogonal.x;
	ortho[1] = orthogonal.y;
	ortho[2] = orthogonal.z;

	HC_Compute_Normalized_Vector(&ortho,&ortho);
	HC_Compute_Cross_Product(view,&ortho,up);
}

void V2HUtils::SetupModellingMatrix(HVector axis, HVector ref, HVector side, HPoint pos)
{
	float mat[16] = {axis.x, axis.y, axis.z, 0, 
			ref.x, ref.y, ref.z, 0,
			side.x, side.y, side.z, 0,
			pos.x, pos.y, pos.z,1};

	HC_Set_Modelling_Matrix(mat);
}

bool V2HUtils::BuildFaceList(int &pnum, int &pmax, int **llist)
{
	int faces_number;
	return BuildFaceList(pnum, pmax, llist, faces_number);
}

// parses the IV face list array (where each face is terminated by -1) and converts
// it to HOOPS face list format (where each face begins with num of points in that face)
// and returns the face list and total number of faces
bool V2HUtils::BuildFaceList(int &pnum, int &pmax, int **llist, int &faces_number)
{
 	char token[MVO_BUFFER_SIZE];
	int *list=*llist;

	if (!NextToken(token))
		return false;

	if (!pmax)
	{
		pmax = BUFFERAD;
		list = new int[pmax];
 		pnum = 0;

		if(!streq(token,"["))
		{
			// only one point exists (since there are no square brackets)
			int itoken = atoi(token);
			list[0] = itoken;
			*llist = list;
 			pnum = 1;
			return true;
		}	
	}
 
	ASSERT(streq(token,"["));

	faces_number = 0;
	int face_num_idx = 0;
	int face_pt_count = 0;
	int i = 0;
	int itoken = -1;
	for (;;)
	{	
		if (!NextToken(token))
			return false;

		if (streq(token,"]"))
		{
			// if the previous token was not -1, then we need to update the face point count
			if( itoken != -1 )
			{
				i++;
				list[face_num_idx] = face_pt_count;
				face_num_idx = i;
				face_pt_count = 0;
				faces_number++;
			}
			break;
		}

		 itoken = atoi(token);

		i++;
		if (i >= pmax)
		{
			int *tmp = new int[(pmax+BUFFERAD)];
			memcpy(tmp,list,sizeof(int)*pmax);
			delete []list;
			list=tmp;
			pmax+=BUFFERAD;
		}	

		if( itoken == -1 )
		{
			list[face_num_idx] = face_pt_count;
			face_num_idx = i;
			face_pt_count = 0;
			faces_number++;
		}
		else
		{
			list[i] = itoken;
			face_pt_count++;
		}

	}
	pnum = i;
	*llist = list;

	return true;
}

// This is the function which converts the VRML material index list into the format compatible to hoops.
// Returns the newly formed material index list.
bool V2HUtils::BuildMatrialIndex(int &mnum, int &mmax, int **mlist, bool ifcolorPerVertex)
{
	char token[MVO_BUFFER_SIZE];
	int *list=*mlist;

	if (!NextToken(token))
		return false;

	if (!mmax)
	{
		mmax = BUFFERAD;
		list = new int[mmax];
		mnum = 0;

		if(!streq(token,"["))
		{
			// only one point exists (since there are no square brackets)

			int itoken = atoi(token);
			list[0] = itoken;
			*mlist = list;
			mnum=1;
			return true;
		}	
	}

	ASSERT(streq(token,"["));

	int mat_num_idx = 0;
	int mat_pt_count = 0;
	int i = 0;
	int itoken = -1;

	//If colorPerVertex is TRUE, colors are applied to each vertex, as follows: 
	if(ifcolorPerVertex)
	{
		for (;;)
		{	
			if (!NextToken(token))
				return false;

			if (streq(token,"]"))
			{
				// if the previous token was not -1, then we need to update the face point count
				if( itoken != -1)
				{
					i++;
					list[mat_num_idx] = mat_pt_count;
					mat_num_idx = i;
					mat_pt_count = 0;
				}
				break;
			}

			itoken = atoi(token);

			i++;
			if (i >= mmax)
			{
				int *tmp = new int[(mmax+BUFFERAD)];
				memcpy(tmp,list,sizeof(int)*mmax);
				delete []list;
				list=tmp;
				mmax+=BUFFERAD;
			}	

			if( itoken == -1 )
			{
				list[mat_num_idx] = mat_pt_count;
				mat_num_idx = i;
				mat_pt_count = 0;
			}
			else
			{
				list[i] = itoken;
				mat_pt_count++;
			}
		}
	}
	//If colorPerVertex is FALSE, colors are applied to each face. No separator. 
	else
	{
		for (;;)
		{	
			if (!NextToken(token))
				return false;

			if (streq(token,"]"))
				break;

			int itoken = atoi(token);

			if (i >= mmax)
			{
				int *tmp = new int[(mmax+BUFFERAD)];
				memcpy(tmp,list,sizeof(int)*mmax);
				delete []list;
				list=tmp;
				mmax+=BUFFERAD;
			}	

			list[i] = itoken;
			i++;
		}

	}

	mnum = i;
	*mlist = list;

	return true;
}

//-----------------------------------------------------------------------------
bool VIndexedFaceSet::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");

	m_segkey = key;
	bool normal_per_vertex = true;
	bool color_per_vertex = true;
	bool has_color_index = false;
	bool has_normal_index = false;
	bool has_texture_index = false;
	bool is_def_coord = false;
	bool is_use_coord = false;
	bool is_def_color = false;
	bool is_use_color = false;
	bool is_def_normal = false;
	bool is_use_normal = false;
	bool is_def_tc = false;
	bool is_use_tc = false;

	char def_or_use_name[MVO_BUFFER_SIZE];
	def_or_use_name[0] = '\0';

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

 	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"coord"))
		{
			bool is_null_coord = false;
			m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, is_def_coord, is_use_coord, is_null_coord);

			if (!m_child_node && !is_null_coord)
					return false;
			
			if(!is_null_coord)
			{
				if( !is_use_coord )
				{
				if (!m_child_node->Create(m_segkey, v2hread))			
						return false;
				}

				plist = ((VCoordinate *)m_child_node)->m_plist;
				plist_length = ((VCoordinate *)m_child_node)->m_plist_length;

				if( !is_def_coord && !is_use_coord)
				{
					delete m_child_node;
					m_child_node = 0;
				}

				if(is_def_coord)
					vhash_insert_string_key_item(VNode::def_handlers, def_or_use_name, m_child_node);
			}
			else
			{
				plist = 0;
				plist_length = 0;
			}

		}

		else if (streq(m_token,"coordIndex"))
		{
			if (!v2hread->BuildFaceList(flist_length, flist_max, &flist,faces_number))
				return false;
		}


		else if (streq(m_token,"texCoord"))
		{
			bool is_null_tc = false;
			m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, is_def_tc, is_use_tc, is_null_tc);

			if (!m_child_node && !is_null_tc)
				return false;

			if(!is_null_tc)
			{

				if( !is_use_tc )
				{
					if (!m_child_node->Create(m_segkey, v2hread))			
						return false;
				}

				tc_list = ((VTextureCoordinate *)m_child_node)->m_tc_list;
				tclist_length = ((VTextureCoordinate *)m_child_node)->m_tc_list_length;

				if( !is_def_tc && !is_use_tc)
				{
					delete m_child_node;
					m_child_node = 0;
				}

				if(is_def_tc)
					vhash_insert_string_key_item(VNode::def_handlers, def_or_use_name, m_child_node);
			}
			else
			{
				tc_list = 0;
				tclist_length = 0;
			}
		}

		else if (streq(m_token,"texCoordIndex"))
		{
			if (!v2hread->BuildFaceList(texture_index_list_length, texture_index_list_max, &texture_index_list))
				return false;

			if(texture_index_list_length > 0)
				has_texture_index = true;
		}

		else if (streq(m_token,"creaseAngle")) 
		{
			if (!v2hread->Get1Float(creaseAngle))
				return false;
		}

		else if (streq(m_token,"solid") || streq(m_token,"ccw") || streq(m_token,"convex"))
		{
 			if (!v2hread->GetString(m_token))
				return false;
			m_level = 1;
		}


		else if (streq(m_token,"normal"))
		{
				bool is_null_normal = false;
				m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, is_def_normal, is_use_normal, is_null_normal);

				if (!m_child_node && !is_null_normal)
						return false;

				if( !is_null_normal )
				{
					if( !is_use_normal )
					{
						if (!m_child_node->Create(m_segkey, v2hread))			
						return false;
					}

					normals_point_list = ((VNormal *)m_child_node)->n_plist;
					normals_point_list_length = ((VNormal *)m_child_node)->n_plist_length;

					if( !is_def_normal && !is_use_normal)
					{
						delete m_child_node;
						m_child_node = 0;
					}

					if(is_def_normal)
						vhash_insert_string_key_item(VNode::def_handlers, def_or_use_name, m_child_node);
				}
				else
				{
					normals_point_list = 0;
					normals_point_list_length = 0;
				}

				
		}
		else if (streq(m_token,"normalIndex"))
		{	
			if (!v2hread->BuildFaceList(normal_index_list_length, normal_index_list_max, &normal_index_list))
				return false;

			if(normal_index_list_length > 0)
				has_normal_index = true;
		}
		
		else if (streq(m_token,"colorIndex"))
		{  	// fourth argument color_per_vertex,- {is default true. I am assuming that colorPerVertex field has already 
			// appeared, in case it is false. Alternate to this is available, but for sake of simplicity using this one. //ASHISH.S. 10/03/09}
			if (!v2hread->BuildMatrialIndex(color_index_list_length, color_index_max, &color_index_list, color_per_vertex))
				return false;

			if(color_index_list_length > 0)
				has_color_index = true;
		}

		else if (streq(m_token,"color"))
		{
			bool is_null_color = false;
			m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, is_def_color, is_use_color, is_null_color);

			if (!m_child_node && !is_null_color)
					return false;

			if( !is_null_color )
			{
				if( !is_use_color )
				{
					if (!m_child_node->Create(m_segkey, v2hread))			
						return false;
				}

				color_list = ((VColor *)m_child_node)->m_clist;
				color_list_length = ((VColor *)m_child_node)->m_clist_length;

				if( !is_def_color && !is_use_color)
				{
					delete m_child_node;
					m_child_node = 0;
				}

					if(is_def_color)
						vhash_insert_string_key_item(VNode::def_handlers, def_or_use_name, m_child_node);
			}
			else
			{
				color_list = 0;
				color_list_length = 0;
			}
		}

		else if (streq(m_token,"normalPerVertex"))
		{
			if (!v2hread->GetString(m_token))
				return false;

			if(streq(m_token,"FALSE"))
				normal_per_vertex = false;
		}

		else if (streq(m_token,"colorPerVertex"))
		{
			if (!v2hread->GetString(m_token))
				return false;

			if(streq(m_token,"FALSE"))
				color_per_vertex = false;
		}
	}

	// The following piece of code examines the various possible combinations of setting normals, color and texture attributes.
	// If any of the above said three attributes is per vertex the other attributes are also made per vertex and 
	// HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes function is used to set the per vertex attributed shell.
	// Otherwise, per face attributes are applied.

	HC_KEY shell_key = INVALID_KEY;

	HC_Open_Segment_By_Key(m_segkey);

		HC_Set_Visibility("lines=on");

		//if any of the color, normals or textures are by "per vertex", we need to break down the other 
		//"per face" attributes to per vertex by replicating it. Here we are checking if this is the case

		if ((normal_per_vertex == true) || (color_per_vertex == true) || (tclist_length != 0))
		{
			// Now if there is no color index specified and if there is color we need to get the color index as
			// 1. If the color is by vertex simply take the face index list as color index list.
			// 2. But if the color is not by vertex (which is per face) we need to apply the color serially but
			//    as we dont have color index we will make one  as shown below in the else case

			if((has_color_index == false) && (color_list_length != 0))
			{
				if(color_per_vertex == true)
				{
					color_index_list_length = flist_length;
					color_index_list = new int[color_index_list_length];
					memcpy(color_index_list, flist, sizeof(int)*color_index_list_length);
				}
				else
				{
					color_index_list_length = faces_number;
					color_index_list = new int[color_index_list_length];
					for(int face_num = 0; face_num < color_index_list_length; face_num++)
					{
						color_index_list[face_num] = face_num;
					}
				}
			}

			// The same thing we are doing here for normals as for the color.

			if((has_normal_index == false) && (normals_point_list_length != 0))
			{
				if(normal_per_vertex == true)
				{
					normal_index_list_length = flist_length;
					normal_index_list = new int[normal_index_list_length];
					memcpy(normal_index_list, flist, sizeof(int)*normal_index_list_length);
				}
				else
				{
					normal_index_list_length = faces_number;
					normal_index_list = new int[normal_index_list_length];
					for(int face_num = 0; face_num < normal_index_list_length; face_num++)
					{
						normal_index_list[face_num] = face_num;
					}
				}
			}
			// Textures are always per vertex so no problem. Just take the face index list as texture index list

			if((has_texture_index == false) && (tclist_length != 0))
			{
				texture_index_list = new int[flist_length];
				memcpy(texture_index_list, flist, sizeof(int)*flist_length);
			}

			int new_normal_index_list_length = 0;
			int* new_normal_index_list = 0;

			int new_color_index_list_length = 0;
			int* new_color_index_list = 0;

			int texture_param = 2;

			if (tclist_length == 0)
				 texture_param = 0;

			// we have to have all the attributes (normals, colors) in per-vertex format. Convert them
			// if they are in per-face format. Here we are having new_normal_index_list because we have to break down 
			// the per face to per vertex and the length is equal to face_index_list_length.

			//If colorPerVertex is TRUE, colours are applied to each vertex, as follows:

			//1. If the colorIndex field is not empty, then colours are applied to each vertex of the IndexedFaceSet 
			//	in exactly the same manner that the coordIndex field is used to choose coordinates for each vertex from the Coordinate node.
			//	The colorIndex field must contain at least as many indices as the coordIndex field, and must contain end-of-face markers (-1) 
			//	in exactly the same places as the coordIndex field. If the greatest index in the colorIndex field is N, then 
			//	there must be N+1 colours in the Color node.
			//2. If the colorIndex field is empty, then the coordIndex field is used to choose colours from the Color node. If the greatest 
			//	index in the coordIndex field is N, then there must be N+1 colours in the Color node. 

			if(normals_point_list_length != 0)
			{
				if(normal_per_vertex == false)
				{
					new_normal_index_list_length = flist_length;
					new_normal_index_list = new int[new_normal_index_list_length];
					memcpy(new_normal_index_list, flist, sizeof(int)*new_normal_index_list_length);

					convert_per_face_attrib_to_per_vertex(flist, normal_index_list_length, normal_index_list, new_normal_index_list);
				}
				else
				{
					new_normal_index_list = normal_index_list;
				}
			}
			
			if(color_list_length != 0)
			{
				if(color_per_vertex == false)
				{
					new_color_index_list_length = flist_length;
					new_color_index_list = new int[new_color_index_list_length];
					memcpy(new_color_index_list, flist, sizeof(int)*new_color_index_list_length);

					convert_per_face_attrib_to_per_vertex(flist, color_index_list_length, color_index_list, new_color_index_list);
				}

				// Else if color_per_vertex is true the old color_index_list is new_color_index_list so just assigning it
				else
				{
					new_color_index_list = color_index_list;
				}
   			}
			
			
			// normal_index_list_length should be equal to the flist_length ideally, but in case less no of normals indices are defined,
			// application crashes. To safeguard against this, added following condition.

			if(flist_length <= normal_index_list_length || normal_index_list_length == 0)	//ASHISH.S.:: 05/02/2009
			{
				shell_key = HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes( 
				(HPoint*) plist, flist_length, flist,
				(HPoint*) normals_point_list, new_normal_index_list,
				(HPoint*) color_list, new_color_index_list, 
				0, 0,
				tc_list, texture_index_list, texture_param,
				static_cast<float>(creaseAngle * 180.0 / 3.14159265));
			}
			else	//ASHISH.S.:: 05/02/2009
			{
				shell_key = HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes( 
				(HPoint*) plist, flist_length, flist,
				0, 0,
				(HPoint*) color_list, new_color_index_list, 
				0, 0,
				tc_list, texture_index_list, texture_param,
				static_cast<float>(creaseAngle * 180.0 / 3.14159265));
			}
			//else
			//{
			//	char message[4096];
			//	char const *messages = message;
			//	sprintf(message, "IndexedFaceSet has too few normalIndex values (have %d, need %d)",normal_index_list_length,flist_length);
			//	HC_Report_Error(2,7,3,1,&messages,0,0);
			//}

			// new_color_index_list and new_normal_index_list are created only when if the color or normal per vertex is false
			// So deleting accordingly
			H_SAFE_DELETE_ARRAY(color_index_list);
			if(color_per_vertex == false)
				H_SAFE_DELETE_ARRAY(new_color_index_list);

			H_SAFE_DELETE_ARRAY(normal_index_list);
			if(normal_per_vertex == false)
				H_SAFE_DELETE_ARRAY(new_normal_index_list);

		}

		else
		{
			//If colorPerVertex is FALSE, colours are applied to each face, as follows:

			//  1. If the colorIndex field is not empty, then one colour is used for each face of the IndexedFaceSet.
			//   There must be at least as many indices in the colorIndex field as there are faces in the IndexedFaceSet.
			//   If the greatest index in the colorIndex field is N, then there must be N+1 colours in the Color node. 
			//   The colorIndex field must not contain any negative entries.
			//	2. If the colorIndex field is empty, then the colours in the Color node are applied to each face of 
			//   the IndexedFaceSet in order. There must be at least as many colours in the Color node as there are faces. 
			
			shell_key = HC_KInsert_Shell(plist_length, plist, flist_length, flist);

			if(normals_point_list_length != 0)
			{
				if(normal_index_list_length != 0)
				{
					HC_Open_Geometry(shell_key);
						for(int n=0; n<faces_number; n++)
						{
							HC_Open_Face(n);
								HC_Set_Normal(normals_point_list[(normal_index_list[n]*3)],normals_point_list[(normal_index_list[n]*3)+1],normals_point_list[(normal_index_list[n]*3)+2]);
							HC_Close_Face();
						}
					HC_Close_Geometry();
				}
				else
				{
					HC_Open_Geometry(shell_key);
					for(int n=0; n<faces_number; n++)
					{
						HC_Open_Face(n);
							HC_Set_Normal(normals_point_list[n*3],normals_point_list[(n*3)+1],normals_point_list[(n*3)+2]);
						HC_Close_Face();
					}
					HC_Close_Geometry();

				}
			}

			if(color_list_length != 0)
			{
				HC_Set_Color_Map_By_Value ("RGB", color_list_length, color_list);

				if(color_index_list_length !=0)
				{
					HC_Open_Geometry(shell_key);
					for(int m=0; m<faces_number; m++)
					{
						HC_Open_Face(m);
							HC_Set_Color_By_Index ("faces", color_index_list[m]);
						HC_Close_Face();
					}
					HC_Close_Geometry();
				}
				else
				{
					HC_Open_Geometry(shell_key);
					for(int m=0; m<faces_number; m++)
					{
						HC_Open_Face(m);
							HC_Set_Color_By_Index ("faces", m);
						HC_Close_Face();
					}
					HC_Close_Geometry();
				}
			}

			H_SAFE_DELETE_ARRAY(normal_index_list);
			H_SAFE_DELETE_ARRAY(color_index_list);
		}
		
	HC_Close_Segment();

	// clean up
	if((is_def_coord == false) && (is_use_coord == false))
		H_SAFE_DELETE_ARRAY(plist);
	if((is_def_normal == false) && (is_use_normal == false) )
		H_SAFE_DELETE_ARRAY(normals_point_list);
	if((is_def_color == false) && (is_use_color == false))
		H_SAFE_DELETE_ARRAY(color_list);
	if((is_def_tc == false) && (is_use_tc == false))
		H_SAFE_DELETE_ARRAY(tc_list);

	H_SAFE_DELETE_ARRAY(texture_index_list);
	H_SAFE_DELETE_ARRAY(flist);

	return true;			
}
//-----------------------------------------------------------------------------

VIndexedLineSet::VIndexedLineSet()
{ 
	flist = 0; 
	flist_length = 0; 
	flist_max = 0; 

	plist = 0;
	plist_length = 0;

	color_list = 0;
	color_list_length = 0;

	color_index_list =0; color_index_list_length =0; color_index_max = 0;
}

bool VIndexedLineSet::Create(HC_KEY key, V2HUtils *v2hread)
{
	bool color_per_vertex = true;
	bool is_def_coord = false;
	bool is_use_coord = false;
	bool is_def_color = false;
	bool is_use_color = false;
#if 0
	bool has_color_index = false;
#endif

	char def_or_use_name[MVO_BUFFER_SIZE];
	def_or_use_name[0] = '\0';

	strcpy(m_token,"empty");
	m_segkey = key;
//	bool colorpervertex_flag = true;


	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

 	for (;;)
	{
	
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"coord") )
		{
			bool is_null_coord = false;
			m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, is_def_coord, is_use_coord, is_null_coord);

			if (!m_child_node && !is_null_coord)
				return false;

			
			if(!is_null_coord)
			{
				if( !is_use_coord )
				{
					if (!m_child_node->Create(m_segkey, v2hread))			
						return false;
				}

				plist = ((VCoordinate *)m_child_node)->m_plist;
				plist_length = ((VCoordinate *)m_child_node)->m_plist_length;

				if( !is_def_coord && !is_use_coord)
				{
					delete m_child_node;
					m_child_node = 0;
				}

				if(is_def_coord)
					vhash_insert_string_key_item(VNode::def_handlers, def_or_use_name, m_child_node);
			}
			else
			{
				plist = 0;
				plist_length = 0;

			}
		}

		else if (streq(m_token,"coordIndex"))
		{
			int count = 0;
			//if (!v2hread->BuildMatrialIndex(flist_length, flist_max, &flist, color_per_vertex))
			if (!v2hread->BuildFaceList(flist_length, flist_max, &flist, count))
				return false;
			/*HC_Open_Segment_By_Key(m_segkey); 
			int * face_list_pointer = flist;
			for(int j = 0; j<flist_length; )
			{
				HC_Insert_Polyline(flist[j], face_list_pointer);
				face_list_pointer = face_list_pointer+flist[j];
				j = j+flist[j]; 
			}
			HC_Close_Segment();*/

		}
		else if (streq(m_token,"color"))
		{
			bool is_null_ref = false;
			m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, is_def_color, is_use_color, is_null_ref);

			if (!m_child_node && !is_null_ref)
				return false;

			if( !is_null_ref )
			{
				if( !is_use_color )
				{
					if (!m_child_node->Create(m_segkey, v2hread))			
						return false;
				}

				color_list = ((VColor *)m_child_node)->m_clist;
				color_list_length = ((VColor *)m_child_node)->m_clist_length;

				if( !is_def_color && !is_use_color)
				{
					delete m_child_node;
					m_child_node = 0;
				}

				if(is_def_color)
					vhash_insert_string_key_item(VNode::def_handlers, def_or_use_name, m_child_node);
			}
			else
			{
				color_list = 0;
				color_list_length = 0;
			}
		}

		else if (streq(m_token,"colorPerVertex"))
		{
			if (!v2hread->GetString(m_token))
				return false;

			if(streq(m_token,"FALSE"))
				color_per_vertex = false;
		}

		// spec appears to allow this, but no code for transferring to HOOPS
		else if (streq(m_token,"colorIndex"))
		{  	// fourth argument color_per_vertex,- {is default true. I am assuming that colorPerVertex field has already 
			// appeared, in case it is false. Alternate to this is available, but for sake of simplicity using this one. //ASHISH.S. 10/03/09}
			if (!v2hread->BuildMatrialIndex(color_index_list_length, color_index_max, &color_index_list, color_per_vertex))
				return false;

#if 0
			if(color_index_list_length > 0)
				has_color_index = true;
#endif
		}
	}
		
	HC_Open_Segment_By_Key(m_segkey); 

	int fpos = 0;
	if(flist_length)
	{
		for (;;)
		{
			int plen = flist[fpos++];
			HPoint *ppoints = new HPoint[plen];
			HPoint *opoints = (HPoint *)plist;
			for (int i=0;i<plen;i++)
			{
				ppoints[i] = opoints[flist[fpos]];
				fpos++;
			}
			HC_Insert_Polyline(plen,ppoints);
			delete []ppoints;
			if (fpos>=flist_length)
				break;
		}
	}
#if 0
	else
	{
		// do nothing, need more example files of this case.. //ASHISH.S. 09/03/09
		//HC_Insert_Polyline(plist_length, plist);
	}
#endif
	HC_Close_Segment();

		// clean up
		H_SAFE_DELETE_ARRAY(color_index_list);

		if((is_def_coord == false) && (is_use_coord == false))
			H_SAFE_DELETE_ARRAY(plist);
		if((is_def_color == false) && (is_use_color == false))
			H_SAFE_DELETE_ARRAY(color_list);
		
		H_SAFE_DELETE_ARRAY(flist);

 	return true;			
}
//-----------------------------------------------------------------------------

bool VCoordinate::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	if (m_child_node)
	{
		if (!m_child_node->Create(m_segkey, v2hread))
			return false; 
		delete m_child_node;
		m_child_node = 0;
	}
 	for (;;)
	{
		if (m_level==0)
		{
			m_segkey = key;

			if (!v2hread->GetString(m_token))
				return false;
			ASSERT(streq(m_token,"{"));
		
			m_level = 1;
		}
		else if (m_level==1)
		{
			
			if (!v2hread->GetString(m_token))
				return false;
			if (streq(m_token,"}"))
				break;

		}
		if (streq(m_token,"point") || m_level==2)
		{
			if (!v2hread->BuildPointList(m_plist_length, m_plist_max, &m_plist))
			{
				m_level = 2;
				return false;
			}
			m_level = 1;
		}
	}
 	return true;			
}
//-----------------------------------------------------------------------------

bool VNormal::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

 	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"vector"))
		{
			if (!v2hread->BuildPointList(n_plist_length, n_plist_max, &n_plist))
				return false;
		}
	}
	return true;
}
//-----------------------------------------------------------------------------

bool VTextureCoordinate::Create(HC_KEY key, V2HUtils *v2hread)
{
	UNREFERENCED(key);

	strcpy(m_token,"empty");

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"point"))
		{
			if (!v2hread->BuildTexPointList(m_tc_list_length, m_tc_list))
				return false;
		}
	}
 	return true;			
}
//-----------------------------------------------------------------------------

bool VShape::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	HC_Open_Segment_By_Key(m_segkey);
	m_segkey = HC_KCreate_Segment("");
	HC_Close_Segment();

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

 	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"appearance") || streq(m_token,"geometry"))
		{
			m_child_node = v2hread->FindNode();
			if (!m_child_node)
				return false;

			if (!m_child_node->Create(m_segkey, v2hread))			
 				return false;

 			delete m_child_node;
			m_child_node = 0;
		}

	}
	return true;			
}
//-----------------------------------------------------------------------------

VBox::VBox() 
{ 
	// this is the default VRML 2.0 box size
	m_min.Set( -1.0, -1.0, -1.0 );
	m_max.Set( 1.0, 1.0, 1.0 );
}

VBox::~VBox()
{
}

bool VBox::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");

	if (m_child_node)
	{
		if (!m_child_node->Create(m_segkey, v2hread))
			return false; 
		delete m_child_node;
		m_child_node = 0;
	}
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	
	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"size"))
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;

			m_min.Set( -a/2.0f, -b/2.0f, -c/2.0f );
			m_max.Set( a/2.0f, b/2.0f, c/2.0f );
		}
	}

	HC_Open_Segment_By_Key(m_segkey);
	//	HUtility::InsertBox(&m_min, &m_max);
	// this will create a box which is flat shaded even in any 
	// shaded mode (Gouroud etc.) which is more realistic
	HUtilityGeometryCreation::CreateCuboid(&m_max, &m_min );
	HC_Close_Segment();

 	return true;			
}
//-----------------------------------------------------------------------------

VSphere::VSphere() 
{ 
	// this are the default for a VRML 2.0 sphere
	m_center.Set( 0.0, 0.0, 0.0 );
	m_radius = 1.0;
	m_axis.Set( 1.0, 0.0, 0.0 );
	m_ref.Set( 0.0, 1.0, 0.0 );

	// hoops default
	m_numSides = 20;
}

VSphere::~VSphere()
{
}

bool VSphere::Create(HC_KEY key ,V2HUtils *v2hread)
{
	strcpy(m_token,"empty");

	if (m_child_node)
	{
		if (!m_child_node->Create(m_segkey, v2hread))
			return false; 
		delete m_child_node;
		m_child_node = 0;
	}
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	
	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"radius"))
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;

			m_radius = a;
		}
	}
	HC_Open_Segment_By_Key(m_segkey);
		HUtilityGeometryCreation::CreateSphere( m_center, m_radius, m_numSides, m_axis, m_ref);
// Should we use this instead ?
		//HC_Insert_Sphere (&m_center, m_radius, &m_axis, &m_ref);
	HC_Close_Segment();

 	return true;			
}
//-----------------------------------------------------------------------------

VCylinder::VCylinder() 
{ 
	// this are the default for a VRML 2.0 sphere
	m_center.Set( 0.0, 0.0, 0.0 );
	m_radius = 1.0;
	m_height = 2.0;
	m_axis.Set( 1.0, 0.0, 0.0 );
	m_ref.Set( 0.0, 1.0, 0.0 );

	// hoops default
	m_numSides = 20;
}

VCylinder::~VCylinder()
{
}

bool VCylinder::Create(HC_KEY key, V2HUtils *v2hread)
{
	HPoint Cylinder_point1;
	HPoint Cylinder_point2;

	strcpy(m_token,"empty");

	if (m_child_node)
	{
		if (!m_child_node->Create(m_segkey, v2hread))
			return false; 
		delete m_child_node;
		m_child_node = 0;
	}
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	
	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"radius"))
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;
			m_radius = a;
		}
		else if(streq(m_token,"height"))
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;

			m_height = a;
		}
		// following cylinder fields are currently ignored
		// field    SFBool    bottom  TRUE
		// field    SFBool    side    TRUE
		// field    SFBool    top     TRUE		
	}

	//in some datasets we see a cylinder of height 0. we assume these are meant to look like discs.
	//treat these as very very short cylinders. this way they will get rendered *and* will turn up as cylinders in a search.
	if(m_height == 0.0f)
		m_height = m_radius * 0.000001f;

	Cylinder_point1.Set(m_center.x, m_center.y+m_height/2, m_center.z);
	Cylinder_point2.Set(m_center.x, m_center.y-m_height/2, m_center.z);
	HC_Open_Segment_By_Key(m_segkey);
// 		HUtilityGeometryCreation::CreateCylinder( m_center, m_radius, m_height, m_numSides, m_axis, m_ref);
		HC_Insert_Cylinder(&Cylinder_point1, &Cylinder_point2, m_radius, "both");
	HC_Close_Segment();

 	return true;			
}
//-----------------------------------------------------------------------------

VCone::VCone() 
{ 
	// this are the default for a VRML 2.0 cone
	m_center.Set( 0.0, 0.0, 0.0 );
	m_radius = 1.0;
	m_height = 2.0;
	m_axis.Set( 1.0, 0.0, 0.0 );
	m_ref.Set( 0.0, 1.0, 0.0 );

	// hoops default
	m_numSides = 20;
}

VCone::~VCone()
{
}

bool VCone::Create(HC_KEY key, V2HUtils *v2hread)
{
	HPoint modified_center;
	strcpy(m_token,"empty");

	if (m_child_node)
	{
		if (!m_child_node->Create(m_segkey, v2hread))
			return false; 
		delete m_child_node;
		m_child_node = 0;

	}
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	
	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"bottomRadius"))
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;
			m_radius = a;
		}
		else if(streq(m_token,"height"))
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;

			m_height = a;
		}
		// following sphere fields are currently ignored
		// field    SFBool    bottom  TRUE
		// field    SFBool    side    TRUE
	}
	modified_center.Set(m_center.x, m_center.y-(m_height/2), m_center.z);
	HC_Open_Segment_By_Key(m_segkey);
		HUtilityGeometryCreation::CreateCone( modified_center, m_radius, m_height, m_numSides, m_axis, m_ref);
	HC_Close_Segment();

 	return true;			
}
//-----------------------------------------------------------------------------

bool VGroup::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

 	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;
		
		if (streq(m_token,"children") || streq(m_token,"Transform"))
		{
			m_child_node = v2hread->GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Create(m_segkey, v2hread))			
				return false;

			delete m_child_node;
			m_child_node = 0;
		}
		else if (streq(m_token,"bboxCenter") || streq(m_token,"bboxSize") || m_level == 3)
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;
		} 
	}
	return true;			
}
//-----------------------------------------------------------------------------

bool VSwitch::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;
	char seg_name[MVO_BUFFER_SIZE];
	int temp_count = 0;
	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	bool only_one_choice = false;

 	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;
		
		if (streq(m_token,"choice"))
		{
			if (!v2hread->GetString(m_token))
				return false;
			if(!streq(m_token,"["))
				only_one_choice = true;
			//ASSERT(streq(m_token,"["));

			sprintf(seg_name, "%ld_%d", m_segkey, temp_count);
			HC_Open_Segment(seg_name);

			if(only_one_choice == true)
			{
				HC_Set_Visibility("everything = off");
				//if (!v2hread->GetString(m_token))
				//	return false;

				m_child_node = v2hread->GetNodeForToken( m_token );
				if (!m_child_node)
					return false;

				if (!m_child_node->Create(m_segkey, v2hread))			
					return false;

				delete m_child_node;
				m_child_node = 0;

			}
			else
			{
			for (;;)
			{
				HC_Set_Visibility("everything = off");
				if (!v2hread->GetString(m_token))
					return false;
				if (streq(m_token,"]"))
					break;

				m_child_node = v2hread->GetNodeForToken( m_token );
				if (!m_child_node)
					return false;

				if (!m_child_node->Create(m_segkey, v2hread))			
					return false;

				delete m_child_node;
				m_child_node = 0;
			}
			}
			HC_Close_Segment();
		}
		if (streq(m_token,"whichChoice"))
		{
			if (!v2hread->GetString(m_token))
				return false;
			int choice = atoi(m_token);
			if(choice != -1)
			{
				sprintf(seg_name, "%ld_%d", m_segkey, choice);
				HC_Open_Segment(seg_name);
					HC_Set_Visibility("everything = on");
				HC_Close_Segment();
			}
		}
	}
	return true;			
}
//-----------------------------------------------------------------------------
//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 11:30:29 AM
//======================================================================
//	Function :		VTransform::Create
//	Description :	Creates a new sub-segment corresponding to tranform
//					node of VRML file.
//	Return :		bool	-	true if succeeds
//	Parameters :
//			HC_KEY key	-	key of the parent node
//	Note :
//======================================================================
bool VTransform::Create(HC_KEY key, V2HUtils *v2hread)
{
 	float a,b,c,d;
	strcpy(m_token,"empty");

	// create a new sub-segment under given parent segment key
	HC_Open_Segment_By_Key(key);
	m_segkey = HC_KCreate_Segment("");
	HC_Close_Segment();

	VERIFY_NEXT_TOKEN( m_token, v2hread )
	ASSERT(streq(m_token,"{"));

 	for (;;)
	{
		VERIFY_NEXT_TOKEN( m_token, v2hread )
		if (streq(m_token,"}"))
			break;
		
		if( streq(m_token,"children") )
		{
			m_child_node = v2hread->GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Create(m_segkey, v2hread))			
				return false;

			delete m_child_node;
			m_child_node = 0;

		}
		else if (streq(m_token,"center") )
		{
 			if (!v2hread->Get3Floats(a, b, c))
				return false;
 			center.Set(a,b,c);
		}
		else if (streq(m_token,"scale") )
		{
 			if (!v2hread->Get3Floats(a, b, c))
				return false;
 			scale.Set(a,b,c);
		}
		else if (streq(m_token,"translation") )
		{
 			if (!v2hread->Get3Floats(a, b, c))
				return false;
 			translation.Set(a,b,c);
		}
		else if (streq(m_token,"rotation") )
		{
 			if (!v2hread->Get4Floats(a, b, c,d))
				return false;
 			rotation.Set(a,b,c,d);
		}
		else if (streq(m_token,"scaleOrientation") )
		{
 			if (!v2hread->Get4Floats(a, b, c,d))
				return false;
 			scaleRotation.Set(a,b,c,d);
		}

	}

	HC_Open_Segment_By_Key(m_segkey);

		HC_Translate_Object(-center.x, -center.y, -center.z);

		if(scaleRotation.rot!=0)
			HC_Rotate_Object_Offaxis(scaleRotation.x,scaleRotation.y, scaleRotation.z,-180.0/3.1415927*scaleRotation.rot);

		HC_Scale_Object(scale.x,scale.y,scale.z);

		if(scaleRotation.rot!=0)
			HC_Rotate_Object_Offaxis(scaleRotation.x,scaleRotation.y, scaleRotation.z,180.0/3.1415927*scaleRotation.rot);

		if(rotation.rot!=0)
			HC_Rotate_Object_Offaxis(rotation.x,rotation.y, rotation.z,180.0/3.1415927*rotation.rot);

		HC_Translate_Object(center.x, center.y, center.z);

		HC_Translate_Object(translation.x, translation.y, translation.z);

	HC_Close_Segment();

	return true;
}
//-----------------------------------------------------------------------------

VChildren::VChildren() 
{ 
}

VChildren::~VChildren()
{
}

//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 10:26:47 AM
//======================================================================
//	Function :		VChildren::Create
//	Description :	Corresponds to children node. Creates a HOOPS named 
//					segment and goes on to create the next node under it
//	Return :		bool - true if succeeds
//	Parameters :
//			HC_KEY key	-	
//	Note :			1. SPECIAL HANDLING FOR 'USE' - NO OPENING/CLOSING BRACE
//======================================================================
bool VChildren::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	bool is_multiline = false;

	for(;;)
	{
		VERIFY_NEXT_TOKEN( m_token, v2hread );

		// ignore the opening bracket
		if (streq(m_token,"["))
		{
			is_multiline = true;
			continue;
		}
		else if (streq(m_token,"]"))
		{
			// break if it is closing bracket
			m_child_node = 0;
			break;
		}
		else
		{
			m_child_node = v2hread->GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Create(m_segkey, v2hread))			
				return false;

			delete m_child_node;
			m_child_node = 0;
			
			// for single line children (USE & DEF), we dont have any 
			// closing braces, so just get off right here
			if ( is_multiline == false )
				break;
		}
	}

	return true;
}
//-----------------------------------------------------------------------------

VDef::VDef() 
{ 
}

VDef::~VDef()
{
}

//======================================================================
//	Author :Rajesh B			Created : 1/18/2001 10:42:47 AM
//======================================================================
//	Function :		VDef::Create
//	Description :	Corresponds to DEF node. Creates a HOOPS named 
//					segment and goes on to create the next node under it
//	Return :		bool - true if succeeds
//	Parameters :
//			HC_KEY key	-	
//	Note :			TODO: HOW TO REPLACE AN EXISTING NAMED SEGMENT WITH
//					THE NEW DEFINITION ?
//======================================================================
bool VDef::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");

	// get the name of the definition
	VERIFY_NEXT_TOKEN( m_token, v2hread );

	// TODO: How delete any existing segment by this definition name
	// HC_Delete_Segment( m_token );

	// create a new segment in the include library and use it here
	m_segkey = CreateNamedSubsegment( DEF_NODE_LIB, m_token, 0 );
	IncludeNamedSegment( key, DEF_NODE_LIB, m_token );

	// find the node and create it
	bool finish=false;
	m_child_node = v2hread->FindNode(&finish);
	if (!m_child_node)
		return false;
	else
	{
		if (!m_child_node->Create(m_segkey, v2hread))			
			return false;

		delete m_child_node;
		m_child_node = 0;
	}
	return true;
}
//-----------------------------------------------------------------------------

VUse::VUse() 
{ 
}

VUse::~VUse()
{
}

//======================================================================
//	Author :Rajesh B			Created : 1/18/2001 10:42:47 AM
//======================================================================
//	Function :		VUse::Create
//	Description :	Instancing in VRML 2.0. We create this node when 
//					the USE keyword is encountered. 
//	Return :		bool - true if succeeds
//	Parameters :
//			HC_KEY key	-	
//	Note :			It is assumed that a named segment is already in the 
//					HOOPS database (created when DEF keyword is encountered)
//======================================================================
bool VUse::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	// get the name of the definition to use
	VERIFY_NEXT_TOKEN( m_token, v2hread );

	IncludeNamedSegment( m_segkey, DEF_NODE_LIB, m_token );

	return true;			
}
//-----------------------------------------------------------------------------

extern "C"
{
	extern bool gzip_buffer_ready;
	extern int gzip_buffersize;
	extern unsigned char *gzip_buffer;
}

//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 3:16:23 PM
//======================================================================
//	Function :		VNode::KCreateNamedSubsegment
//	Description :	Creates a new segment under given segment key and 
//					assigns the alias	
//	Return :HC_KEY-	key of new subsegment created
//	Parameters :
//			HC_KEY parent_key	-	key of super-segment
//			char* alias	-		alias for the new subsegment
//	Note :	TODO: HANDLE CASE WHERE THE ALIAS IS ALREADY DEFINED?
//======================================================================
HC_KEY VNode::KCreateNamedSubsegment( HC_KEY parent_key, const char* alias )
{
	HC_KEY new_seg_key = 0;

	// open up the parent 
	HC_Open_Segment_By_Key(parent_key);

		// create the new subsegment
		new_seg_key = HC_KCreate_Segment(alias);

		// define alias - hoops requires alias to start with "?"
		if( alias )
		{
			// open the new subsegment
			HC_Open_Segment_By_Key(new_seg_key);

				// get the full path
				char path[MVO_SEGMENT_PATHNAME_BUFFER];
				HC_Show_Pathname_Expansion(".", path);

				char hp_alias[MVO_BUFFER_SIZE];
				strcpy( hp_alias, "?");
				strcat( hp_alias, alias );
				HC_Define_Alias( hp_alias, path );

			HC_Close_Segment();
		}

	HC_Close_Segment();

	return new_seg_key;
}

//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 3:16:23 PM
//======================================================================
//	Function :		VNode::CreateNamedSubsegment
//	Description :	Creates a new segment under given segment Path and 
//					assigns the alias. If there is already a segment
//					by that name in include library, it is renamed by 
//					adding some unique number to it's name. This is done to comply 
//					with the VRML specs (behaviour observed in Cosmo Player)
//	Return :HC_KEY-	key of new subsegment created
//	Parameters :
//			char* segmentPath - library path where the key needs 
//								to be stored
//			char* segmentName - Name for the new subsegment 
//			char* alias	-		alias for the new subsegment
//======================================================================
HC_KEY VNode::CreateNamedSubsegment( const char* segmentPath, const char* segmentName, const char* alias )
{
	HC_KEY new_seg_key = 0;

	char seg_fullpath[MVO_SEGMENT_PATHNAME_BUFFER];
	strcpy( seg_fullpath, segmentPath);
	strcat( seg_fullpath, segmentName );

	// if such a segment already exists, rename it
	if( HC_QShow_Existence(seg_fullpath, "self") )
	{
		char appendage_string[512];
		static long int unique_appendage = 0;
		sprintf( appendage_string, "%ld",  unique_appendage );
		unique_appendage++;

		HC_KOpen_Segment( seg_fullpath );
			char new_name_for_prev_seg[MVO_BUFFER_SIZE];
			strcpy( new_name_for_prev_seg, seg_fullpath );
			strcat( new_name_for_prev_seg, appendage_string);

			HC_Rename_Segment(seg_fullpath,new_name_for_prev_seg);
		HC_Close_Segment();
	}

	// create the new subsegment
	new_seg_key = HC_KCreate_Segment(seg_fullpath);

	// define alias - hoops requires alias to start with "?"
	if( alias )
	{
		char hp_alias[MVO_BUFFER_SIZE];
		strcpy( hp_alias, "?");
		strcat( hp_alias, alias );
		HC_Define_Alias( hp_alias, seg_fullpath );
	}

	return new_seg_key;
}

//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 3:59:27 PM
//======================================================================
//	Function :		VNode::IncludeNamedSegment
//	Description :	Includes a name segment in given parent
//	Return :bool	-	true if succeeds
//	Parameters :
//			HC_KEY parent_key	-	key for parent segment
//			char* alias	-		alias of the name segment
//	Note :	ASSUMPTION:	ALIAS IS DEFINED FOR AN EXISITING SEGMENT
//======================================================================
bool VNode::IncludeNamedSegment( HC_KEY parent_key, const char* alias )
{
	// open parent for include
	HC_Open_Segment_By_Key(parent_key);
		
		// include segment - hoops requires alias to start with "?"
		char hp_alias[MVO_BUFFER_SIZE];
		strcpy( hp_alias, "?");
		strcat( hp_alias, alias );
		HC_Include_Segment( hp_alias );

	HC_Close_Segment();

	return true;
}

//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 3:59:27 PM
//======================================================================
//	Function :		VNode::IncludeNamedSegment
//	Description :	Includes a name segment in given parent
//	Return :bool	-	true if succeeds
//	Parameters :
//			HC_KEY parent_key	-	key for parent segment
//			char* alias	-		alias of the name segment
//	Note :	ASSUMPTION:	ALIAS IS DEFINED FOR AN EXISITING SEGMENT
//			TODO: PERFORM A CHECK IF THE SEGMENT IS GEOMETRY OR STYLE
//======================================================================
bool VNode::IncludeNamedSegment( HC_KEY parentKey, const char* segmentPath, const char* segmentName )
{
	// open parent for include
	HC_Open_Segment_By_Key(parentKey);
		
		// include segment - hoops requires alias to start with "?"
		char seg_fullpath[MVO_SEGMENT_PATHNAME_BUFFER];
		strcpy( seg_fullpath, segmentPath);
		strcat( seg_fullpath, segmentName );
		
		HC_Include_Segment( seg_fullpath );
		HC_Style_Segment( seg_fullpath );

	HC_Close_Segment();

	return true;
}
//-----------------------------------------------------------------------------

bool V2HUtils::BuildPointList(int &pnum, int &pmax, float **llist)
{
 	char token[MVO_BUFFER_SIZE];
	float *list=*llist;
	if (!pmax)
	{
		pmax = BUFFERAD;
		list = new float[pmax*3];
 		pnum = 0;

		if (!GetString(token))
 			return false;
		
		if(!streq(token,"["))
		{
			// only one point exists (since there are no square brackets)
			list[pnum*3]= (float)atof(token);
 			
			if(!V2HUtils::NextToken(token))
				goto breakout;
  			list[pnum*3+1]=(float)atof(token);
 			
			if(!V2HUtils::NextToken(token))
				goto breakout;
  			list[pnum*3+2]=(float)atof(token);

			pnum++;

			*llist = list;
			return true;
		}	
	}

	ASSERT(streq(token,"["));
 	for (;;)
	{
		if (pnum>=pmax)
		{
			float *tmp = new float[(pmax+BUFFERAD)*3];
			memcpy(tmp,list,sizeof(float)*3*pmax);
			delete []list;
			list=tmp;
			pmax+=BUFFERAD;
		}
 		if(!V2HUtils::NextToken(token))
			goto breakout;
 		if (streq(token,"]"))
		{
			*llist = list;
			return true;
		}
		list[pnum*3]= (float)atof(token);
 		if(!V2HUtils::NextToken(token))
			goto breakout;
  		list[pnum*3+1]=(float)atof(token);
 		if(!V2HUtils::NextToken(token))
			goto breakout;
  		list[pnum*3+2]=(float)atof(token);
		pnum++;
	}

breakout:
 	*llist = 0;
	H_SAFE_DELETE_ARRAY(list);
	return false;
}

bool V2HUtils::BuildTexPointList(int& tc_num, float*& tc_list)
{
 	char token[TOKEN_BUFFER_SIZE];

	if (!GetString(token))
		return false;
	ASSERT(streq(token,"["));

	tc_num = 0;
	int tc_max = BUFFERAD;
	tc_list = new float[tc_max*2];


	for (;;)
	{
		if (tc_num>=tc_max)
		{
			float *tmp = new float[(tc_max+BUFFERAD)*2];
			memcpy(tmp,tc_list,sizeof(float)*2*tc_max);
			delete []tc_list;
			tc_list=tmp;
			tc_max+=BUFFERAD;
		}

 		if(!V2HUtils::NextToken(token))
		{
			delete[] tc_list;
			tc_list = 0;
			return false;
		}

 		if (streq(token,"]"))
			return true;

		tc_list[tc_num*2]= (float)atof(token);

 		if(!V2HUtils::NextToken(token))
		{
			delete[] tc_list;
			tc_list = 0;
			return false;
		}

  		tc_list[tc_num*2+1]=(float)atof(token);

		tc_num++;
	}
}
	

bool V2HUtils::fill_buffer()
{
	static int counter=-1;	
	static int buffer_fill_count = 0;
	counter++;
	int size = 0;
// 	if ((counter % 2) == 0)
	{
	buffersize=5000;

	if (fbuffer)
		delete[] fbuffer;

	fbuffer = new unsigned char[buffersize+1];
	memset( fbuffer, 0, buffersize+1 );
	size = (int)fread(fbuffer,1,buffersize,fhandle);
	buffer_fill_count++;
	if(buffer_fill_count > 25)
	{
		unsigned long now = ftell(fhandle);
		m_pHandler->ReportInputPercentProgress(now/(float)m_file_size);
		buffer_fill_count = 0;
	}
	if( ferror( fhandle ) ) 
	{
		ASSERT(!"Error reading file");
		return false;
	}
	if( feof( fhandle ) )
	{
		if(!size)
		{
			wchar_t err_msg[MVO_BUFFER_SIZE];
			swprintf(err_msg,MVO_BUFFER_SIZE, L"VRML Import Error: Reached end of file unexpectedly");
			m_pHandler->ReportInputInformation(err_msg);
			return false;
		}
	}

	if (save_fbuffer_length)
	{
		unsigned char *temp = new unsigned char [buffersize+save_fbuffer_length];
		memset( temp, 0, buffersize+save_fbuffer_length );
		memcpy(temp,save_fbuffer,save_fbuffer_length);
		memcpy(temp+save_fbuffer_length,fbuffer,buffersize);
		buffersize+=save_fbuffer_length;
		delete[] fbuffer;
		fbuffer = temp;
	}

 	return true;
	}

/*	if (fbuffer)
		free (fbuffer);
	while (!gzip_buffer_ready)
		Sleep(50);
	fbuffer = gzip_buffer;*/

//	fbuffer = new unsigned char[gzip_buffersize];
//	memcpy(fbuffer,gzip_buffer,gzip_buffersize);
//	buffersize = gzip_buffersize;
//	gzip_buffer_ready=false;
}

bool V2HUtils::NextToken(char *token)
{
	// initialize the token with a null string. We will find nothing at the end of the file. 
	// But we dont want token to have garbage value.

	token[0] = '\0';

	int tokenstate=NOTINTOKEN;
	int tokenpos=0;
	for (;;)
	{
		ENSURE_BUFFER_READ;
		
		if (tokenstate==COMMENT)
		{
			if (fbuffer[fposition]==10)
				tokenstate=NOTINTOKEN;
			fposition++;
		}
		else if (fbuffer[fposition]== 0)
		{
			break;
		}
		else if (fbuffer[fposition]=='#')
		{
			tokenstate=COMMENT;
		}
		else if (tokenstate==NOTINTOKEN)
		{
			if (fbuffer[fposition]==' ' || fbuffer[fposition]==',' || fbuffer[fposition]==10 || fbuffer[fposition]==13 || fbuffer[fposition]==9)
				fposition++;
			else		
				tokenstate=INTOKEN;
		}
		else
		{
			if (fbuffer[fposition]==' ' || fbuffer[fposition]==',' || fbuffer[fposition]==10 || fbuffer[fposition]==13 || fbuffer[fposition]==9)
				break;
			if (fbuffer[fposition]=='{' || fbuffer[fposition]=='}' || fbuffer[fposition]=='[' || fbuffer[fposition]==']')
			{
				if (tokenpos==0)
		 			token[tokenpos++]=fbuffer[fposition++];
				break;
			}
 			token[tokenpos++]=fbuffer[fposition++];
		}
	}
	if (tokenpos)
		token[tokenpos]=0;
	return true;
}

// find the next token, skip given number of tokens
bool V2HUtils::NextToken(char *token, int skip)
{
	for( int i = 0; i <= skip; i++ )
	{
		if( !V2HUtils::NextToken(token) )
			return false;
	}
	return true;
}

bool V2HUtils::GetQuotedString(char *token)
{
	int tokenstate=NOTINTOKEN;
	int tokenpos=0;

	tokenstate=tokenstate;
	
	for(;;)
	{
		ENSURE_BUFFER_READ;
		
		if (fbuffer[fposition]==' ' )
			fposition++;
		else if( fbuffer[fposition]==',')
			fposition++;
		else if(fbuffer[fposition]==10)
			fposition++;
		else if (fbuffer[fposition]==13)
			fposition++;
		else if (fbuffer[fposition]==9)
			fposition++;
		else
			break;
		
	}

	if (fbuffer[fposition]!='\"')
	{
		ASSERT(!"Expecting opening double quote");
		return false;
	}
	fposition++;

	tokenstate=INTOKEN;

	for (;;)
	{
		ENSURE_BUFFER_READ;

		if (fbuffer[fposition]=='\"')
		{
			fposition++;
			break;
		}
		else
	 		token[tokenpos++]=fbuffer[fposition++];
	}
	
	if (tokenpos)
		token[tokenpos]=0;
	return true;
}

void V2HUtils::MarkBuffer()
{
	save_fposition=fposition;
}

void V2HUtils::SaveBuffer()
{
	save_fbuffer_length = buffersize-save_fposition;
	if (save_fbuffer)
	{
		delete []save_fbuffer;
		save_fbuffer = 0;
	}
	save_fbuffer = new char[save_fbuffer_length];
	memcpy(save_fbuffer,fbuffer+save_fposition,save_fbuffer_length);
}

bool V2HUtils::GetString(char *string)
{
	if (V2HUtils::NextToken(string))
		return true;
	else
		return false;
}

bool V2HUtils::Token2RGBA(char * token, const int n_components, unsigned char * RGBA)
{
	// Set all components to 0
	RGBA[0] = 0;
	RGBA[1] = 0;
	RGBA[2] = 0;
	RGBA[3] = 255;

	if( n_components < 1 || n_components > 4 )
		return false;

	// eliminate the "0x" prefix
	if( token[1] == 'x' || token[1] == 'X')
		token+=2;
	
	// loop till we are done
	char * end = token + strlen(token);
	char hex_pix[3];

	// break if string ends
	if(end == token)
		return true;

	// read and convert a hex
	end-=2; 
	hex_pix[0] = end[0];
	hex_pix[1] = end[1];
	hex_pix[2] = '\0';

	char hex_str = static_cast<char>(V2HUtils::HexStr2Int(hex_pix));
	
	if(n_components == 1)
	{
		RGBA[0] = RGBA[1] = RGBA[2] = hex_str;
		return true;
	}
	else if(n_components == 2 || n_components == 4)
		RGBA[3] = hex_str;
	else if(n_components == 3)
		RGBA[2] = hex_str;

	if(end == token)
		return true;

	// read and convert a hex
	end-=2; 
	hex_pix[0] = end[0];
	hex_pix[1] = end[1];
	hex_pix[2] = '\0';
	hex_str = static_cast<char>(V2HUtils::HexStr2Int(hex_pix));
	if(n_components == 2)
	{
		RGBA[0] = RGBA[1] = RGBA[2] = hex_str;
		return true;
	}
	else if(n_components == 3)
		RGBA[1] = hex_str;
	else if (n_components == 4)
		RGBA[2] = hex_str;

	if(end == token)
		return true;

	// read and convert a hex
	end-=2; 
	hex_pix[0] = end[0];
	hex_pix[1] = end[1];
	hex_pix[2] = '\0';
	hex_str = static_cast<char>(V2HUtils::HexStr2Int(hex_pix));
	if(n_components == 3)
	{
		RGBA[0] = hex_str;
		return true;
	}
	else if (n_components == 4)
		RGBA[1] = hex_str;

	if(end == token)
		return true;

	// read and convert a hex
	end-=2; 
	hex_pix[0] = end[0];
	hex_pix[1] = end[1];
	hex_pix[2] = '\0';
	hex_str = static_cast<char>(V2HUtils::HexStr2Int(hex_pix));
	RGBA[0] = hex_str;

	return true;
}

int V2HUtils::HexStr2Int(const char *hex_str)
{
	
	int result;
	sscanf(hex_str,"%2x", &result);
	return result;
}

bool V2HUtils::GetBool(bool& true_or_false)
{
	char token[TOKEN_BUFFER_SIZE];
	VERIFY_NEXT_TOKEN( token, this );

	if(streq(token, VRML_TRUE_STR))
		true_or_false = true;
	else if(streq(token, VRML_FALSE_STR))
		true_or_false = false;
	else
		return false;

	return true;
}

bool V2HUtils::Get1Float(float &a)
{
	char token[MVO_BUFFER_SIZE];
	MarkBuffer();

	if (!V2HUtils::NextToken(token))
	{
		SaveBuffer();
		return false;
	}
	a = (float)atof(token);
	return true;
}

bool V2HUtils::Get3Floats(float &a, float &b, float &c)
{
	char token[MVO_BUFFER_SIZE];
	MarkBuffer();

	if (!V2HUtils::NextToken(token))
		goto no_token;
	a = (float)atof(token);
	if (!V2HUtils::NextToken(token))
		goto no_token;
	b = (float)atof(token);
	if (!V2HUtils::NextToken(token))
		goto no_token;
	c = (float)atof(token);
	return true;

no_token:	
	SaveBuffer();
	return false;
}

bool V2HUtils::Get4Floats(float &a, float &b, float &c, float &d)
{
	char token[MVO_BUFFER_SIZE];
	MarkBuffer();

	if (!V2HUtils::NextToken(token))
		goto no_token;
	a = (float)atof(token);
	if (!V2HUtils::NextToken(token))
		goto no_token;
	b = (float)atof(token);
	if (!V2HUtils::NextToken(token))
		goto no_token;
	c = (float)atof(token);
	if (!V2HUtils::NextToken(token))
		goto no_token;
	d = (float)atof(token);
	return true;

no_token:	
	SaveBuffer();
	return false;
}

bool V2HUtils::SkipBracket()
{
 	char btype_close = ' ';
	int level=1;
 	char btype_open = ' ';

 	for (;;)
	{
		ENSURE_BUFFER_READ;
		
		if (fbuffer[fposition]=='{' || fbuffer[fposition]=='[' )
		{
			btype_open = fbuffer[fposition];
			fposition++;	
			if (btype_open == '{')
				btype_close = '}';
			if (btype_open == '[')
				btype_close = ']';

			break;
		}
 		fposition++;
	}

 	for (;;)
	{
		ENSURE_BUFFER_READ;
		
		if (fbuffer[fposition]==btype_open)
		{
			fposition++;
			level++;
		}
		else if (fbuffer[fposition]==btype_close)
		{
			fposition++;
			level--;
			if (level==0)
			break;
		}
		else
			fposition++;
	}
	return true;
}

bool V2HUtils::ReadLine()
{
	for(;;)
	{
		ENSURE_BUFFER_READ;
		
		if((fbuffer[fposition]==13)||(fbuffer[fposition]==10))
		{
			fposition++;
			break;
		}

		else if( fbuffer[fposition]==0 )
			break;

		else 
			fposition++;
 	}
	
	return true;
}
//======================================================================
//	Author :Rajesh B			Created : 1/25/2001 10:01:17 AM
//======================================================================
//	Function :		VNode::RemoveQuotes
//	Description :	Removes double quotes from the string. The resultant
//					string is returned in the argument itself
//	Return :bool	-	false if fails
//	Parameters :
//			char* string	-	
//	Note :			WORLDS SLOPPIEST WAY OF DOING THIS THING. I KNOW 
//					I COULD HAVE DONE BETTER THAT THIS
//======================================================================
bool V2HUtils::RemoveQuotes(char* string)
{
	char temp[MVO_BUFFER_SIZE];
	unsigned int pos = 1;
	size_t len = strlen(string);
 	while( pos <= len && string[pos] != '\"')
	{
		temp[pos-1] = string[pos];
 		pos++;
	}
	temp[pos-1] = 0;
	strcpy( string, temp );
	return true;
}

VNode* V2HUtils::FindNode( bool *closebracket)
{
	char token[MVO_BUFFER_SIZE];
 	if (!NextToken(token))
		return 0;
	if (closebracket && streq(token,"]"))
	{
		*closebracket = true;
		return 	(VNode *)1;
	}
 
	return GetNodeForToken(token);
}

// find the next node, skip and DEF nodes encountered in between
VNode* V2HUtils::FindNodeSkipDEF( bool *closebracket)
{
	char token[MVO_BUFFER_SIZE];
 	if (!NextToken(token))
		return 0;
	if (closebracket && streq(token,"]"))
	{
		*closebracket = true;
		return 	(VNode *)1;
	}
	if( streq(token,"DEF") )
	{
		// skip the definition name and get the next token
 		if (!NextToken(token, 1))
			return 0;
	}
 
	return GetNodeForToken(token);

}

//Returns the next node and DEF name if DEF is found and sets the corresponding flag
//Returns the node corresponding to the DEF name if USE is found
//Returns the node corresponding to the token if neither DEF or USE is found

VNode* V2HUtils::FindNodeGetDEForUSE(char* def_or_use_name, bool &ref_is_def, bool &ref_is_use)
{
	ref_is_def = false;
	ref_is_use = false;

	char token[MVO_BUFFER_SIZE];
	
 	if (!NextToken(token))
		return 0;

	/*if (closebracket && streq(token,"]"))
	{
		*closebracket = true;
		return 	(VNode *)1;
	}*/

	if( streq(token,"DEF") )
	{
		// Get the definition name 
 		if (!NextToken(def_or_use_name, 0))
			return 0;
		ref_is_def = true;
		
		if (!NextToken(token))
			return 0;
		return GetNodeForToken(token);
	}
 	else if( streq(token,"USE") )
	{
		// Get the definition name 
 		if (!NextToken(def_or_use_name, 0))
			return 0;
		ref_is_use = true;
		
		VNode* def_node = 0;
		vhash_lookup_string_key_item(VNode::def_handlers, def_or_use_name, (void**)&def_node);
		return def_node;
	}
	else 
	{
		return GetNodeForToken(token);
	}
}

//Returns the next node and DEF name if DEF is found and sets the corresponding flag
//Returns the node corresponding to the DEF name if USE is found
//Returns the node corresponding to the token if neither DEF or USE is found
//Returns null if NULL is found
VNode* V2HUtils::FindNodeGetDEForUSEorNULL(char* def_or_use_name, bool &ref_is_def, bool &ref_is_use, bool &ref_is_null)
{
	ref_is_def = false;
	ref_is_use = false;

	char token[MVO_BUFFER_SIZE];
	
 	if (!NextToken(token))
		return 0;

	if( streq(token,"DEF") )
	{
		// Get the definition name 
 		if (!NextToken(def_or_use_name, 0))
			return 0;
		ref_is_def = true;
		
		if (!NextToken(token))
			return 0;
		return GetNodeForToken(token);
	}
 	else if( streq(token,"USE") )
	{
		// Get the definition name 
 		if (!NextToken(def_or_use_name, 0))
			return 0;
		ref_is_use = true;
		
		VNode* def_node = 0;
		vhash_lookup_string_key_item(VNode::def_handlers, def_or_use_name, (void**)&def_node);
		return def_node;
	}
 	else if( streq(token,"NULL") )
	{
		def_or_use_name = 0;
		ref_is_null = true;
		return 0;
	}
	else 
	{
		return GetNodeForToken(token);
	}
}



//======================================================================
//	Author :Rajesh B			Created : 1/18/2001 4:20:59 PM
//======================================================================
//	Function :		VNode::GetNodeForToken
//	Description :	Given a token, function creates a node object
//	Return :		VNode*	- VDummy if token is not supported node
//	Parameters :
//			char* token	-	token name
//	Note :
//======================================================================


VNode* V2HUtils::GetNodeForToken(char* token)
{
 	if (streq(token,"Group"))
		return new VGroup;
	if (streq(token,"Switch"))
		return new VSwitch;
 	else if (streq(token,"Collision"))
		return new VGroup;
 	else if (streq(token,"Transform"))
		return new VTransform;
 	else if (streq(token,"LOD"))
		return new VLOD;
	else if (streq(token,"Shape"))
		return new VShape;
	else if (streq(token,"IndexedFaceSet"))
		return new VIndexedFaceSet;
	else if (streq(token,"IndexedLineSet"))
		return new VIndexedLineSet;
	else if (streq(token,"Coordinate"))
		return new VCoordinate;
	else if (streq(token,"TextureCoordinate"))
		return new VTextureCoordinate;
	else if (streq(token,"Color"))
		return new VColor;
	else if (streq(token,"Appearance"))
		return new VAppearance;
	else if (streq(token,"Material"))
		return new VMaterial;
	else if (streq(token,"Normal"))
		return new VNormal;
	else if (streq(token,"PixelTexture"))
		return new VPixelTexture;
	else if (streq(token,"ImageTexture"))
		return new VImageTexture;
	else if (streq(token,"Box"))
		return new VBox;
	else if (streq(token,"Inline"))
		return new VInline;
	else if (streq(token,"Sphere"))
		return new VSphere;
	else if (streq(token,"Cylinder"))
		return new VCylinder;
	else if (streq(token,"Cone"))
		return new VCone;
	else if (streq(token,"DEF"))
		return new VDef;
	else if (streq(token,"USE"))
		return new VUse;
	else if (streq(token,"children"))
		return new VChildren;
	else if (streq(token,"Text"))
		return new VText;
	else if (streq(token,"FontStyle"))
		return new VFontStyle;
	else if (streq(token,"ROUTE"))
		return new VRoute;
	else return new VDummy;
}

struct vhash_s* VNode::def_handlers = 0;

static void vnode_cleanup_helper(void * item, const char * key, void * user_data)
{
	UNREFERENCED(key);
	UNREFERENCED(user_data);

	VNode* pVNode = (VNode*)item;
	pVNode->CleanUp();
	delete pVNode;
}

bool V2HUtils::vrmltoHOOPS(const wchar_t * filename, HC_KEY key)
{
	unsigned long now;
	unsigned long file_size;

	bool success = true;
	VNode::def_handlers = new_vhash(1, malloc, free);
 	buffersize = -1;
	fposition = 0;
	VNode * this_node = 0;
	fhandle = wfopen(filename, L"rb");
	if (!fhandle)
	{
		ASSERT(!"Failed to open file");
		success = false;
		goto Done;
	}
	wcscpy(VNode::wrl_file_path,filename);
	// VRML uses a Cartesian, right-handed, 3-dimensional coordinate system. 
	HC_Open_Segment_By_Key(key);
		HC_Set_Handedness("Right");
	HC_Close_Segment();


	now = ftell(fhandle);
	fseek(fhandle, 0, SEEK_END);
	file_size = ftell(fhandle);
	m_file_size = file_size - now;
	fseek(fhandle, now, SEEK_SET); 

	do
	{
		this_node = FindNode();
		ASSERT(this_node);

		if((fbuffer[fposition] == 0) && (feof( fhandle )))
		{
			if(this_node)
			{
				delete this_node;
				this_node = 0;
			}
			break;
		}

		if (!this_node->Create(key,this)) {
			success = false;
			goto Done;
		}

		delete this_node;
		this_node = 0;
		
	}while(1);

	if(save_fbuffer)
	{
   		delete[] save_fbuffer;
   		save_fbuffer = 0;
   	}

	if(fbuffer)
	{
		delete[] fbuffer;
		fbuffer = 0;
	}

Done:
	if (fhandle)
		fclose(fhandle);
	// Just to cleanup the vhash - this much code is required
	  if(VNode::def_handlers)
	  {
		vhash_string_key_map_function(VNode::def_handlers, vnode_cleanup_helper, 0);
		delete_vhash(VNode::def_handlers);
		VNode::def_handlers=0;
	  } 
	
	return success;
}
//----------------------------------------------------------------------------- 

VNode::VNode( char *name )
{
	UNREFERENCED(name);

	m_child_node = 0;
	m_segopen=false;
	m_level=0;
	m_segkey=0;
}

VNode::~VNode()
{
	if (m_segopen)
		HC_Close_Segment();
}

bool VLOD::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"level"))
		{
 			
			for (;;)
			{
				bool finish=false;
				if (firstrun)
				{
					if (!v2hread->NextToken(m_token))
						return false;

					if(streq(m_token, "["))	//ASHISH.S. 26/02/09
						m_child_node = v2hread->FindNode(&finish);
					else
					{
						m_child_node = v2hread->GetNodeForToken(m_token); // there is only one group in level
						if (!m_child_node->Create(m_segkey, v2hread))			
							return false;

						delete m_child_node;
						m_child_node = 0;
						break;
					}
					firstrun = false;
				}
				else // Since Multiple LOD's are not handled, rest of the LODs are ignored 
				{
					// Get the next node but kill it and forcefully send Dummy for further
					// processing
					m_child_node = v2hread->FindNode(&finish);
					if (m_child_node && !finish)
					{
						delete m_child_node; 
						m_child_node = new VDummy();
					}
				}
 
				if (!m_child_node)
					return false;

				if (finish)
				{
					m_child_node = 0;
					break;
				}
				
				if (!m_child_node->Create(m_segkey, v2hread))			
					return false;

				delete m_child_node;
				m_child_node = 0;
				
			}
		}
		else if (streq(m_token,"center"))
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;
		}
		else if (streq(m_token,"range"))
		{
			bool first_cycle = true; //ASHISH.S.::

			for (;;)
			{
				if (!v2hread->GetString(m_token))
					return false;

				// if there is no opening square bracket there is only one value
				// read it and proceed //ASHISH.S.:: 10/02/09
				if(!streq(m_token,"[") && first_cycle)
				{
					break;
				}

				if (streq(m_token,"]"))
					break;

				first_cycle = false; //ASHISH.S.::
			}
		}

 
	}
	return true;
}
//-----------------------------------------------------------------------------

bool VColor::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"color"))
		{
			if (!v2hread->BuildPointList(m_clist_length, m_clist_max, &m_clist))
				return false;

		}
	}
 	return true;			
}
//-----------------------------------------------------------------------------

bool VAppearance::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{")); 

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;
		
		if (streq(m_token,"texture") || streq(m_token,"textureTransform"))
		{
			bool is_null_ref = false;
			bool def_ref_dummy = false;
			bool use_ref_dummy = false;
			char def_or_use_name[MVO_BUFFER_SIZE];
			m_child_node = v2hread->FindNodeGetDEForUSEorNULL( def_or_use_name, def_ref_dummy, use_ref_dummy, is_null_ref);
			
			if (!m_child_node && !is_null_ref)
				return false;

			if(!is_null_ref)
			{
				if (!m_child_node->Create(m_segkey, v2hread))			
					return false;
			}
				
		}
		else
		{
			m_child_node = v2hread->FindNode();
			if (!m_child_node)
				return false;

			if (!m_child_node->Create(m_segkey, v2hread))			
			return false;
		}
		
		delete m_child_node;
		m_child_node = 0;
		
	}

	return true;			
}
//-----------------------------------------------------------------------------

static void generate_unique_texture_name(char * new_texture_name)
{
	  time_t ltime;
	  time( &ltime );
	  sprintf(new_texture_name,"%s%ld",VRML_TEXTURE_BASE_NAME, ltime);
}

//======================================================================
//	Author : Pavan T			Created : 1/20/2006 
//======================================================================
//	Function :		VNode::GetStringInSquareBracket
//	Description :	Removes square brackets (if present) and quotes and 
//					returns the string
//	Return :bool	-	false if fails
//	Parameters :    the string to be returned. 
//  NOTE:			The caller is assumed to have allocated enough memory 
//					to the return string (string_in_bracket argument)
//======================================================================
bool V2HUtils::GetStringInSquareBracket(char *string_in_bracket)
{
	if (!V2HUtils::NextToken(string_in_bracket))
		return false;

	if (streq(string_in_bracket,"["))
	{
		if( !V2HUtils::GetQuotedString(string_in_bracket))
			return false;
		
		char ignore_token[MVO_BUFFER_SIZE];
		assert_NEXT_TOKEN(ignore_token,"]", this);

		return true;
	}
	else 
	{
		V2HUtils::RemoveQuotes( string_in_bracket );
		return true;
	}
}

bool VImageTexture::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;
	wchar_t image_file_path[MVO_BUFFER_SIZE];
	bool is_repeat_S = false;
	bool is_repeat_T = false;

	if (!v2hread->GetString(m_token))
		return false;
	assert(streq(m_token,"{")); 

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;

		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"url"))
		{
			char tmp[MVO_BUFFER_SIZE] = {""};
			if (!v2hread->GetStringInSquareBracket(tmp))
						return false;
			wcscpy(image_file_path, (wchar_t*)H_WCS(tmp).encodedText());

			// figure out if this is URL or just a filename without any path.
			if( !wcschr(image_file_path, L':' ))
			{
				// if this is just a simple name (e.g. "brick.jpg") then we will try 
				// to locate the texture file in the same directory as the wrl file.

				// truncate at the first '\' to '/' from right
				wchar_t wrl_file_dir[MVO_BUFFER_SIZE];
				wcscpy(wrl_file_dir, wrl_file_path);
				wchar_t *end = wrl_file_dir + wcslen(wrl_file_dir);
				while (end >= wrl_file_dir)
				{
					if ((*end) == L'/' || (*end) == L'\\')
					{
						end++;
						break;
					}
					end--;			
				}
				*end = L'\0';					

				wcscat(wrl_file_dir, image_file_path);
				wcscpy(image_file_path, wrl_file_dir);
			}
		}
		else if (streq(m_token,"repeatS"))
		{
			if(!v2hread->GetBool(is_repeat_S))
				return false;
		}
		else if (streq(m_token,"repeatT"))
		{
			if(!v2hread->GetBool(is_repeat_T))
				return false;
		}
	} 
	
#ifdef USE_IMAGE_MAGICK 

	HC_KEY model_tex_library_key = HC_KOpen_Segment(VRML_TEXTURE_LIB);
	HC_Close_Segment();

	char texture_opts[MVO_BUFFER_SIZE];
	char texture_name[MVO_BUFFER_SIZE];
	char image_format[MVO_BUFFER_SIZE];
	char texture_def[MVO_BUFFER_SIZE];
	HIOUtilityMagick magick;
	HC_KEY image_key;

	generate_unique_texture_name(texture_name);
	strcpy(image_format,"RGBA" );

	HInputHandlerOptions options;
	options.m_pImageName = texture_name;
	options.m_pImageFormat = image_format;
	
	HFileInputResult result = magick.FileInputToImageKey(image_file_path, 
										&image_key, model_tex_library_key, &options );
	(void)result;
	/* Don't quit just because the texture is missing... We'll just use the black and white
	checker pattern instead... */
	//if (result != InputOK){
	//	return false;
	//}

	if(is_repeat_S == true && is_repeat_T == true)
			sprintf (texture_opts, "tiling = repeat");
		else 
			sprintf (texture_opts, "tiling = clamp");
	
	sprintf(texture_def," %s, no interpolation filter,source = %s",texture_opts,texture_name);
	
	HC_Define_Texture (texture_name, texture_def); 

	HC_Open_Segment_By_Key(m_segkey);
		char color_opts[MVO_BUFFER_SIZE];
		sprintf (color_opts, "faces = %s", texture_name);
        HC_Set_Color(color_opts);

	HC_Close_Segment();

#endif

	return true;
}
//-----------------------------------------------------------------------------

bool VInline::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;
	wchar_t inline_file_path[MVO_BUFFER_SIZE];
	
	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{")); 

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;

		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"url"))
		{
			char tmp[MVO_BUFFER_SIZE] = {""};
			if (!v2hread->GetStringInSquareBracket(tmp))
						return false;
			wcscpy(inline_file_path, (wchar_t*)H_WCS(tmp).encodedText());

			// figure out if this is URL or just a filename without any path.
			if( !wcschr(inline_file_path, L':'))
			{
				// if this is just a simple name (e.g. "brick.jpg") then we will try 
				// to locate the texture file in the same directory as the wrl file.

				// truncate at the first '\' to '/' from right
				wchar_t wrl_file_dir[MVO_BUFFER_SIZE];
				wcscpy(wrl_file_dir, wrl_file_path);
				wchar_t *end = wrl_file_dir + wcslen(wrl_file_dir);
				while (end >= wrl_file_dir)
				{
					if ((*end) == L'/' || (*end) == L'\\')
					{
						end++;
						break;
					}
					end--;			
				}
				*end = L'\0';					

				wcscat(wrl_file_dir, inline_file_path);
				wcscpy(inline_file_path, wrl_file_dir);
			}
		}
		else if (streq(m_token,"bboxCenter"))
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;
		}
		else if (streq(m_token,"bboxSize"))
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;
		}
	} 

	HIOUtilityVRML xref_vrml;
	HFileInputResult fileip_res = xref_vrml.FileInputByKey(inline_file_path, m_segkey, 0);
	ASSERT( fileip_res == InputOK );
	(void)fileip_res;
	
	return true;
}
//-----------------------------------------------------------------------------

bool VPixelTexture::Create(HC_KEY key, V2HUtils *v2hread)
{
	float width = 0;
	float height = 0;
	float n_components = 0;
	unsigned char *pixels = 0;
	bool is_repeat_S = false;
	bool is_repeat_T = false;

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{")); 

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;

		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"image"))
		{
			if (!v2hread->Get1Float(width))
				return false;
			if (!v2hread->Get1Float(height))
				return false;
			if (!v2hread->Get1Float(n_components))
				return false;

			pixels = new unsigned char[(int)(4*width*height)];

			int i = (int)height-1;
			int j = 0;
			while(i >= 0)
			{
				if (!v2hread->GetString(m_token))
					return false;

				if(!v2hread->Token2RGBA( m_token, (int) n_components, &pixels[i*((int)width)*4 + j*4]))
					return false;

				j++;
				if(j >= width)
				{
					i--;
					j=0;
				}
			}
		}
		else if (streq(m_token,"repeatS"))
		{
			if(!v2hread->GetBool(is_repeat_S))
				return false;
		}
		else if (streq(m_token,"repeatT"))
		{
			if(!v2hread->GetBool(is_repeat_T))
				return false;
		}
	}

	HC_Open_Segment(VRML_TEXTURE_LIB);
	
		char texture_opts[MVO_BUFFER_SIZE];
		char texture_name[MVO_BUFFER_SIZE];
		char texture_def[MVO_BUFFER_SIZE];
		generate_unique_texture_name(texture_name);

		if(is_repeat_S == true && is_repeat_T == true)
			sprintf (texture_opts, "tiling = repeat");
		else 
			sprintf (texture_opts, "tiling = clamp");
		
		sprintf(texture_def,"RGBA,  name = %s,texture options = (%s, no interpolation filter)",texture_name,texture_opts);
		HC_Insert_Image (0,0,0,texture_def, (int) width, (int) height, pixels); 
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_segkey);
		char color_opts[MVO_BUFFER_SIZE];
		sprintf (color_opts, "faces = %s", texture_name);
        HC_Set_Color(color_opts);
	HC_Close_Segment();

	if( pixels )
		delete[] pixels;

	return true;
}

bool VMaterial::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{")); 

	for (;;)
	{
		if (!v2hread->GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"diffuseColor"))
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;

			HC_Open_Segment_By_Key(m_segkey);

 			HC_Set_Color_By_Value("edges,faces,lines","RGB",a,b,c);
			HC_Close_Segment();
		}
		else if (streq(m_token,"specularColor") || streq(m_token,"emissiveColor"))
		{
			float a,b,c;
			if (!v2hread->Get3Floats(a, b, c))
				return false;
		}
		else if (streq(m_token,"transparency"))
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;

			// set transparency only if has non-opaque value
			if( a > 0.0f)
			{
				char transp_def[MVO_SMALL_BUFFER_SIZE];
				sprintf(transp_def, "faces = (transmission = (R = %f G = %f B = %f))", a, a, a);
				HC_Open_Segment_By_Key(m_segkey);
					HC_Set_Color(transp_def);
				HC_Close_Segment();
			}
		}
		else if (streq(m_token,"ambientIntensity"))
		{
			// REMOVE THE TWO LINES BELOW AND UNCOMMENT THE REST OF THE LINES
			// WHEN YOU IMPLEMENT COMPLETE LIGHTING MODEL IN THIS READER - RAJESH B
 			if (!v2hread->GetString(m_token))
				return false;
//			float a;
//			if (!Get1Float(a))
//				return false;
//
//			char amb_def[MVO_SMALL_BUFFER_SIZE];
//			sprintf(amb_def, "ambient = (R = %f G = %f B = %f)", a, a, a);
//			HC_Open_Segment_By_Key(m_segkey);
//				HC_Set_Color(amb_def);
//			HC_Close_Segment();
		}
		else if (streq(m_token,"shininess") )
		{
			float a;
			if (!v2hread->Get1Float(a))
				return false;

			char shin_def[MVO_SMALL_BUFFER_SIZE];
			if(a > 0.0) // ASHISH.S. 26/02/09
			{
				sprintf(shin_def, "faces = (gloss = %f)", a*128); //see lighting equation for VRML2.0 
				HC_Open_Segment_By_Key(m_segkey);
					HC_Set_Color(shin_def);
				HC_Close_Segment();
			}
		}
  
	}

	return true;			
}

bool VDummy::Create(HC_KEY key, V2HUtils *v2hread)
{
	UNREFERENCED(key);

	return v2hread->SkipBracket();
}

bool VRoute::Create(HC_KEY key, V2HUtils *v2hread)
{
	UNREFERENCED(key);

	return v2hread->ReadLine();
}

VText::VText() 
{ 
	// this are the default for a VRML 2.0 text

	// text content
	m_string = new char[MVO_BUFFER_SIZE];
	strcpy( m_string, "");

	// text pos
	m_x = 0.0; m_y = 0.0; m_z = 0.0;
}

VText::~VText()
{
	if(m_string)
	{
		delete[] m_string;
		m_string = 0;
	}
}

bool VText::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	
	for (;;)
	{
		VERIFY_NEXT_TOKEN( m_token, v2hread );
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"string"))
		{
			ReadString(v2hread);
		}
		if(streq(m_token,"fontStyle"))
		{
			assert_NEXT_TOKEN( m_token, "FontStyle", v2hread );
			m_child_node = v2hread->GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Create(m_segkey, v2hread))			
				return false;

			delete m_child_node;
			m_child_node = 0;
		}
		else
		{
			
		}

	}

	HC_Open_Segment_By_Key(m_segkey);
		HC_Insert_Text( m_x, m_y, m_z, m_string );
	HC_Close_Segment();

 	return true;			
}


//======================================================================
//	Author :Rajesh B			Created : 1/24/2001 3:49:07 PM
//======================================================================
//	Function :		VText::ReadString
//	Description :	Reads the string argument of the Text node
//	Return :bool	-	false if fails
//	Parameters :
//	Note :			1. ASSUMES THAT string KEYWORD IS ALREADY PASSED
//					2. ASSUMES THAT ACTUAL STRING IS ENCLOSED IN SQUARE
//					BRACES AND DOUBLE-QUOTES. EX:  ["sample"]
//======================================================================
bool VText::ReadString(V2HUtils *v2hread)
{
	if( !v2hread->GetQuotedString( m_string ))
		return false;
	return true;
}
//-----------------------------------------------------------------------------

VFontStyle::VFontStyle() 
{ 
	// this are the default for a VRML 2.0 fontstyle
	m_name = new char[MVO_BUFFER_SIZE];
	strcpy( m_name, "name = SansSerif");

	m_size = new char[MVO_BUFFER_SIZE];
	strcpy( m_size, "size = 1.0 oru");
	m_spacing = 1.0;

	// horizontal text flow by default
	m_isHorizontal = true;
	m_path.Set(1.0, 0.0, 0.0);
}

VFontStyle::~VFontStyle()
{
	H_SAFE_DELETE_ARRAY( m_name );
	H_SAFE_DELETE_ARRAY( m_size );
}

bool VFontStyle::Create(HC_KEY key, V2HUtils *v2hread)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	if (!v2hread->GetString(m_token))
		return false;
	ASSERT(streq(m_token,"{"));
	
	for (;;)
	{
		VERIFY_NEXT_TOKEN( m_token, v2hread );
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"family"))
		{
			ReadFamily(v2hread);
		}
		else if(streq(m_token,"size"))
		{
			ReadSize(v2hread);
		}
		else if(streq(m_token,"horizontal"))
		{
			ReadAlignment(v2hread);
		}
		else if(streq(m_token,"leftToRight"))
		{
			ReadLeft2Right(v2hread);
		}
		else if(streq(m_token,"topToBottom"))
		{
			ReadTop2Bottom(v2hread);
		}
		else if(streq(m_token,"spacing"))
		{
			if (!v2hread->Get1Float(m_spacing))
				return false;
		}
		else
		{
			
		}

	}

	HC_Open_Segment_By_Key(m_segkey);
		// just passing on VRML 2.0 font name to HOOPS. Hope they understand
		// each other wrt font names
        HC_Set_Text_Font( m_name );
        HC_Set_Text_Path( m_path.x, m_path.y, m_path.z);
        
		HC_Set_Text_Font( m_size );
        HC_Set_Text_Spacing( m_spacing );

		// VRML 2.0 has text which rotates/zooms/pans just like model objects
        HC_Set_Text_Font("transforms = on");
	HC_Close_Segment();

 	return true;			
}

bool VFontStyle::ReadFamily(V2HUtils *v2hread)
{
	strcpy( m_name, "name = ");
	VERIFY_NEXT_TOKEN( m_token, v2hread );
	v2hread->RemoveQuotes( m_token );
	if(streq(m_token,"DEFAULT"))
		strcat( m_name, "SansSerif");
	else
		strcat( m_name, m_token);

	return true;
}

bool VFontStyle::ReadSize(V2HUtils *v2hread)
{
	strcpy( m_size, "size = ");
	VERIFY_NEXT_TOKEN( m_token, v2hread );
	strcat( m_size, m_token);
	strcat( m_size, " oru");

	return true;
}

//======================================================================
//	Author :Rajesh B			Created : 1/26/2001 11:19:43 AM
//======================================================================
//	Function :		VFontStyle::ReadFontAlignment
//	Description :	Reads the font path. VRML2.0 has only two paths, 
//					horizontal and vertical. Sets the m_path variable
//					accordingly
//	Return :bool	-	true if succeeds
//	Parameters :
//	Note :			ASSUMPTION: THERE IS NO OTHER INFO
//======================================================================
bool VFontStyle::ReadAlignment(V2HUtils *v2hread)
{
	VERIFY_NEXT_TOKEN( m_token, v2hread );
	if(streq(m_token,VRML_FALSE_STR))
	{
		m_isHorizontal = false;
		m_path.Set(1.0, 0.0, 0.0);
	}
	else
	{
		m_isHorizontal = true;
		m_path.Set(0.0, 1.0, 0.0);
	}


	return true;
}

//======================================================================
//	Author :Rajesh B			Created : 1/26/2001 11:19:43 AM
//======================================================================
//	Function :		VFontStyle::ReadLeft2Right
//	Description :	Given the font axis (X or Y), reads the font direction
//					For example, if it is horizontal, then is it in negative
//					or positive direction 
//	Return :bool	-	true if succeeds
//	Parameters :
//	Note :			Read VRML2.0 specs for better understanding
//======================================================================
bool VFontStyle::ReadLeft2Right(V2HUtils *v2hread)
{
	VERIFY_NEXT_TOKEN( m_token, v2hread );
	if(streq(m_token, VRML_TRUE_STR))
	{
		if( m_isHorizontal == true )
			m_path.Set( 1.0, 0.0, 0.0 );
	}
	else
	{
		if( m_isHorizontal == true )
			m_path.Set( -1.0, 0.0, 0.0 );
	}

	return true;
}

//======================================================================
//	Author :Rajesh B			Created : 1/26/2001 11:19:43 AM
//======================================================================
//	Function :		VFontStyle:: ReadTop2Bottom
//	Description :	Given the font axis (X or Y), reads the font direction
//					For example, if it is vertical, then is it in negative
//					or positive direction 
//	Return :bool	-	true if succeeds
//	Parameters :
//	Note :			Read VRML2.0 specs for better understanding
//======================================================================
bool VFontStyle::ReadTop2Bottom(V2HUtils *v2hread)
{
	VERIFY_NEXT_TOKEN( m_token, v2hread );
	if(streq(m_token, VRML_TRUE_STR))
	{
		if( m_isHorizontal == false )
			m_path.Set( 0.0, -1.0, 0.0 );
	}
	else
	{
		if( m_isHorizontal == false )
			m_path.Set( 0.0, 1.0, 0.0 );
	}

	return true;
}

#endif


