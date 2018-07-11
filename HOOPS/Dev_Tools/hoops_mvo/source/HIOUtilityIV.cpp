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

 
#include "HIOUtilityIV.h"

#include "HUtility.h"
#include "HUtilityGeometryCreation.h"
#include "varray.h"

#ifdef USE_IMAGE_MAGICK 
#include "HIOUtilityMagick.h"
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

// local functions
static bool ivtoHOOPS(const wchar_t *filename, HC_KEY key);
HInputHandler * IVHandler = NULL;
unsigned long m_file_size = 0;

HFileInputResult HIOUtilityIV::FileInputByKey(const wchar_t * FileName, HC_KEY key, HInputHandlerOptions * options)
{
	UNREFERENCED(options);

	if(!FileName) return InputBadFileName;
	if(key == INVALID_KEY) return InputFail;

	SetStartingInput((char*)H_UTF8(FileName).encodedText());

	IVHandler = this;
	bool result = ivtoHOOPS(FileName, key);

	SetFinishedInput();

	if(result) 
		return InputOK;
	else
	{
		ReportInputInformation("Unknown error while reading .iv file. Could not load file");
		return InputFail;
	}
}



// macros
#define VERIFY_NEXT_TOKEN(__token_)		\
		if (!NextToken(__token_))		\
			return 0;

// macros
#define assert_NEXT_TOKEN(__token_, __expected__)	\
		if (!NextToken(__token_))					\
			return 0;								\
		if( !(streq(__token_,__expected__)))		\
		{											\
			char __msg[MVO_BUFFER_SIZE];				\
			strcat(__msg, "Expecting ");				\
			strcat(__msg, __expected__);				\
			return false;							\
		}

// hoops segment without name
#define ANONYMUS_NODE		""

// HOOPS path to store IV def nodes
#define DEF_NODE_LIB		"?Include Library/IV/Def/"
#define IV_TEXTURE_LIB	"/Include Library/IV/Textures"

#define IV_TRUE_STR		"TRUE"
#define IV_FALSE_STR		"FALSE"

#define streq(a,b)  (!strcmp(a,b))
#define BUFFERAD 100
#define TOKEN_BUFFER_SIZE	1024
#define IV_TEXTURE_BASE_NAME "iv_texture"



// global variables
static int fposition=1000;			// current position in FBUFFER
static unsigned char *fbuffer=0;	// File buffer
static int buffersize=-1;			// chunk to be read at a time
static FILE *fhandle;				// iv file

static int save_fposition=0;
static char *save_fbuffer=0;
static int save_fbuffer_length;



#define NOTINTOKEN 0
#define INTOKEN 1
#define COMMENT 2

class IV2HUtils
{
public:

	// here till these functions find a place in HUtility
	static void	GetIViewplaneIVectors(HC_KEY segment_key, float * view, float * up);
	static void	SetupModellingMatrix(HVector axis, HVector ref, HVector side, HPoint pos);

};


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


class IVNode 
{

public:

	IVNode(char *name = 0);
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0) = 0;
	virtual ~IVNode();

	char* m_name;
	char m_token[255];
	
	bool m_segopen;
	IVNode *m_child_node;
	int m_level;
	HC_KEY m_segkey;
	static char iv_file_path[MVO_BUFFER_SIZE];

	HC_KEY KCreateNamedSubsegment( HC_KEY parent_key, const char* alias = 0 );
	HC_KEY CreateNamedSubsegment( const char* segmentPath, const char* segmentName, const char* alias = 0 );
	bool IncludeNamedSegment( HC_KEY parent_key, const char* alias );
	bool IncludeNamedSegment( HC_KEY parentKey, const char* segmentPath, const char* segmentName );


	// common utility functions
	static bool NextToken(char *token);
	static bool NextToken(char *token, int skip);

	static IVNode* FindNode(bool *closebracket=0);
	static IVNode* GetNodeForToken(char* token);
	static IVNode* FindNodeSkipDEF(bool *closebracket=0);

	static int  HexStr2Int(const char *hex_str);
	static bool Get1Float(float &a) ;
	static bool Get3Floats(float &a, float &b, float &c);
	static bool Get4Floats(float &a, float &b, float &c, float &d);
	static bool GetBool(bool& true_or_false);
	static bool GetString(char *string);
	static bool GetQuotedString(char *token);
	static bool RemoveQuotes(char* string);
	static bool BuildPointList(int &pnum, int &pmax, float **llist);
	static bool BuildFaceList(int &pnum, int &pmax, int **llist);
	static bool BuildFaceList(int &pnum, int &pmax, int **llist, int &faces_number);
	static bool BuildColorList(int &material_list_length, float *&material_list);
	static bool BuildTexPointList(int& tc_num, float*& texture_coord_list);
	static bool Token2RGBA(char * token, const int n_components, unsigned char * RGBA);
	static bool GetStringInSquareBracket(char *string_in_bracket);

	// ASHISH.S. 19/02/09
	static bool BuildFaceListforFaceSet(int &pnum, int &pmax, int **llist, int &faces_number);

};
char IVNode::iv_file_path[] = "";


class IVIndexedFaceSet : public IVNode
{
public:

	// coordinates
	float *point_list;
	int point_list_length;

	// face list
	int *face_list;
	int face_list_length;
	int face_list_max;

	// normal list
	int *normal_index_list;
	int normal_index_list_length;
	int normal_index_list_max;
	int norm_num;

	// texture coordinates
	float *texture_coord_list;
	int texture_coord_list_length;

	// texture coordinate indices
	int *texture_index_list;
	int texture_index_list_length;
	int texture_index_list_max;

	// Material Indices
	int  *material_index_list;
	int material_index_list_length;
	int material_index_max;

	// material list 
	float *material_list;
	int material_list_length; 

	// color list
	float *clist;
	int clist_length;

	//material binding
	char material_binding[255];

	//texture binding
	char texture_binding[255];

	//normal binding
	char normal_binding[255];

	// normals
	float *normals_point_list;
	int normals_point_list_length;

	bool is_tristrip;
	 

	IVIndexedFaceSet(bool bIsTristrip = false) : IVNode() 
	{
		point_list = 0; point_list_length = 0;
		face_list = 0; face_list_length = 0; face_list_max = 0; 
		normal_index_list = 0; normal_index_list_length = 0; normal_index_list_max = 0; norm_num=0;

		texture_coord_list = 0; texture_coord_list_length = 0;
		texture_index_list = 0; texture_index_list_length = 0; texture_index_list_max = 0;

		material_list =0; material_list_length=0; 
		material_index_list =0; material_index_list_length =0; material_index_max = 0;

		clist = 0; clist_length = 0; 
		normals_point_list = 0; normals_point_list_length = 0; 

		strcpy(material_binding, "");
		strcpy(normal_binding, "");
		strcpy(texture_binding, "");

		is_tristrip = bIsTristrip;
		
	};

	virtual ~IVIndexedFaceSet(){}
	virtual  bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

protected:
	bool reprocess_coord_lists(float *&point_list,   int& point_list_length, 
											int   *&face_list,   int& face_list_length, 
											float *&texture_coord_list, int& texture_coord_list_length, 
											int   *&tlist,   int& tlist_length,
											VArray<float>&  new_point_list,
											VArray<int>&    new_face_list,
											VArray<float>&  new_tex_coord_list
											);

	bool reprocess_attribute_coord_lists(float *&point_list,   int& point_list_length, 
											int   *&face_list,   int& face_list_length, 
											float *&normals_point_list, int& normals_point_list_length, 
											int   *&normal_index_list,   int& normal_index_list_length,
											VArray<float>&  new_point_list,
											VArray<int>&    new_face_list,
											VArray<float>&  new_nlist
											);
	
	bool remove_vertex_sharing (	float *&point_list,   int& point_list_length, 
													int   *&face_list,   int& face_list_length, 
													VArray<float>&  new_point_list,
													VArray<int>&    new_face_list
											  );

};

class IVIndexedLineSet : public IVNode
{
public:
 	int *face_list;
	int face_list_length;
	int face_list_max;

	float *point_list;
	int point_list_length;

	float *clist;
	int clist_length;

	IVIndexedLineSet();

	virtual ~IVIndexedLineSet(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};

// ASHISH.S. 18/02/09
class IVFaceSet : public IVNode
{
public:
 	// face list
	int *face_list;
	int face_list_length;
	int face_list_max;

	IVFaceSet();

	virtual ~IVFaceSet(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};


class IVLineSet : public IVNode
{
public:

	IVLineSet();

	virtual ~IVLineSet(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};


class IVCoordinate: public IVNode
{
public:
	float *m_plist;
	int	m_plist_length;
	int	m_plist_max;

	IVCoordinate() : IVNode () { 	m_plist=0; m_plist_length=0; m_plist_max = 0; };

	virtual ~IVCoordinate(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};


class IVTextureCoordinate: public IVNode
{
public:
	float *m_tc_list;
	int	m_tc_list_length;

	IVTextureCoordinate() : IVNode () 
	{ 	
		m_tc_list = 0; 
		m_tc_list_length = 0; 
	};

	virtual ~IVTextureCoordinate(){}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};


class IVPixelTexture: public IVNode
{
public :

	IVPixelTexture() : IVNode (){};

	virtual ~IVPixelTexture(){}
	
    virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVTextureCoordinateBinding: public IVNode
{
public :

	//texture binding
	char texture_binding[255];

	IVTextureCoordinateBinding() : IVNode (){strcpy(texture_binding, "");};

	virtual ~IVTextureCoordinateBinding(){}
	
    virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVImageTexture: public IVNode
{
public :
	

	IVImageTexture() : IVNode ()
	{
		 
	};

	virtual ~IVImageTexture(){}
	
    virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVShape : public IVNode
{
public:
	IVShape() : IVNode() {};

	virtual ~IVShape(){}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};


class IVCube : public IVNode
{
public:
	IVCube();
	virtual ~IVCube();

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

	HPoint m_min;
	HPoint m_max;
};

class IVRotationXYZ : public IVNode
{
public:
	IVRotationXYZ()
	{rotation.Set(0,0,1,0);};
	
	HRotation rotation;
	
	virtual ~IVRotationXYZ(){;}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

	
};

class IVSphere : public IVNode
{
public:
	IVSphere();
	virtual ~IVSphere();

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

	HPoint m_center;
	float m_radius;
	int m_numSides;
	HPoint m_axis;
	HPoint m_ref;

};

class IVCylinder : public IVNode
{
public:
	IVCylinder();
	virtual ~IVCylinder();

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

	HPoint m_center;
	float m_radius;
	float m_height;
	int m_numSides;
	HPoint m_axis;
	HPoint m_ref;

};

class IVCone : public IVNode
{
public:
	IVCone();
	virtual ~IVCone();

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

	HPoint m_center;
	float m_radius;
	float m_height;
	int m_numSides;
	HPoint m_axis;
	HPoint m_ref;
};



class IVNormal: public IVNode
{
public:
	float *n_plist;
	int	n_plist_length;
	int	n_plist_max;

	IVNormal() : IVNode () { 	n_plist=0; n_plist_length=0; n_plist_max = 0; };
	virtual ~IVNormal(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};
//-----------------------------------------------------------------------------

class IVSwitch : public IVNode
{
public:
	IVSwitch() : IVNode() { };
	virtual ~IVSwitch(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

//-----------------------------------------------------------------------------
class IVGroup : public IVNode
{
public:
	IVGroup() : IVNode() { firstrun = true; };
	bool firstrun;
	virtual ~IVGroup(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVVertexProperty : public IVNode
{
public:

	float *m_plist;
	int	m_plist_length;
	int	m_plist_max;

	float *n_plist;
	int	n_plist_length;
	int	n_plist_max;

	float *m_tc_list; 
	int m_tclist_length;

	char material_binding[255];
	char normal_binding[255];

	float *m_material_list;
	int m_material_list_length;

	IVVertexProperty() : IVNode() 
	{
		m_plist=0; m_plist_length=0; m_plist_max = 0; 
		n_plist=0; n_plist_length=0; n_plist_max = 0; 
		m_material_list =0 ; m_material_list_length =0;
		m_tc_list = 0; m_tclist_length = 0; 

		strcpy(material_binding, "");
		strcpy(normal_binding, "");
	};
	
	virtual ~IVVertexProperty(){}
	
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVShapeHints : public IVNode
{
public:
	IVShapeHints() : IVNode(){}

	virtual ~IVShapeHints(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};
class IVMatrixTransform : public IVNode
{
public:
	IVMatrixTransform() : IVNode(){}

	virtual ~IVMatrixTransform(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};
class IVSeparator : public IVNode
{
public:

	float* m_coord3_list;
	int  m_coord3_list_length;

	//float* m_vertex_list;
	//int  m_vertex_list_length;
	
	float* normal_point_list;
	int  normal_point_list_length;

	// face list
	int *face_list;
	int face_list_length;
	int face_list_max;

	IVSeparator() : IVNode()
	{
		m_coord3_list = 0;
		m_coord3_list_length = 0;

		/*m_vertex_list = 0;
		m_vertex_list_length = 0;*/

		normal_point_list = 0;
		normal_point_list_length = 0;

		face_list = 0;
		face_list_length = 0;

		m_lMaterial_key = HC_KOpen_Segment("material key");
			HC_Set_Color("green");
		HC_Close_Segment();

		m_lTransformation_key = HC_KOpen_Segment("Transformation key");
			HC_Scale_Object(1.0,1.0,1.0);
		HC_Close_Segment();
	};

	virtual ~IVSeparator()
	{
		HC_Delete_By_Key(m_lMaterial_key);
		HC_Delete_By_Key(m_lTransformation_key);

	}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
	HC_KEY m_lMaterial_key;
	HC_KEY m_lTransformation_key;
};
class IVQuadMesh : public IVNode
{
public:
	
	// coordinates
	float *m_quad_plist;

	// normal list
	float *m_quad_nlist;
	int m_quad_nlist_length;

	float *m_quad_tclist;
	int m_quad_tclist_length;


	IVQuadMesh() : IVNode()
	{	
		m_quad_plist = 0;
		m_quad_nlist = 0; m_quad_nlist_length = 0; 
		m_quad_tclist = 0; m_quad_tclist_length = 0; 
	}

	virtual ~IVQuadMesh(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVTransform : public IVNode
{

public:

	IVTransform() : IVNode() { firstrun = true; 
							 center.Set(0,0,0); 
							 rotation.Set(0,0,1,0); 
							 scale.Set(1,1,1); 
							 scaleRotation.Set(0,0,1,0); 
							 translation.Set(0,0,0); };
	bool firstrun;
	virtual ~IVTransform(){}
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
	HPoint center;
	HPoint scale;
	HPoint translation;
	HRotation rotation;
	HRotation scaleRotation;
};


class IVChildren : public IVNode
{
public:
	IVChildren();
	virtual ~IVChildren();

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};




class IVDef : public IVNode
{
public:
	IVDef(){;};
	virtual ~IVDef(){;};

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};


class IVUse : public IVNode
{
public:
	IVUse(){;};
	virtual ~IVUse(){;}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};




class IVLOD : public IVNode
{
public:
	IVLOD() : IVNode() { firstrun = true; };
	virtual ~IVLOD(){}

	bool firstrun;
	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};





class IVColor: public IVNode
{
public:
	float *m_clist;
	int	m_clist_length;
	int	m_clist_max;

	IVColor() : IVNode () { 	m_clist=0; m_clist_length=0; m_clist_max = 0; };
	virtual ~IVColor(){}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};

class IVAppearance: public IVNode
{
public:
 
	IVAppearance() : IVNode() { };
	virtual ~IVAppearance(){}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};


class IVMaterial: public IVNode
{
public:
	float r,g,b;
 
	IVMaterial() : IVNode() { };
	virtual ~IVMaterial(){}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);
};




class IVDummy: public IVNode
{
public:
	float r,g,b;
 
	IVDummy() : IVNode() { };
	virtual ~IVDummy(){}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

};



class IVText: public IVNode
{
public:

	IVText(bool bIs3DText = false)
	{ 
		m_string = new char[MVO_BUFFER_SIZE];
		strcpy( m_string, "");

		m_spacing = 1.0;
		m_path.Set(1.0, 0.0, 0.0);
		m_isHorizontal = true;

		m_x = 0.0; m_y = 0.0; m_z = 0.0;

		is_3DText = bIs3DText;
	}

	~IVText()
	{
		H_SAFE_DELETE_ARRAY(m_string);
	}

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

protected:

	char* m_string;
	float m_x;
	float m_y;
	float m_z;
	float m_spacing; 
	HPoint m_path;
	bool m_isHorizontal;

	bool is_3DText;

	bool ReadString();
	bool ReadLeft2Right();
    
};


class IVFont : public IVNode
{
public:
	IVFont();
	virtual ~IVFont();

	virtual bool Convert2Hoops (HC_KEY key, IVNode* parent_node = 0);

protected:

	char* m_name;
	char* m_size;
	

	bool ReadFontName();
};

/* 
  GetIViewplaneIVectors computes the construction-plane-constrained vectors used for geometry insertion.
  \param seg_key Key to the construction segment
  \param *view A pointer to the double precision view vector.  Passed by reference.
  \param *up A pointer to the double precision up vector. Passed by reference.
*/
void IV2HUtils::GetIViewplaneIVectors(HC_KEY seg_key, float * view, float * up)
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


void IV2HUtils::SetupModellingMatrix(HVector axis, HVector ref, HVector side, HPoint pos)
{
	float mat[16] = {axis.x, axis.y, axis.z, 0, 
			ref.x, ref.y, ref.z, 0,
			side.x, side.y, side.z, 0,
			pos.x, pos.y, pos.z,1};

	HC_Set_Modelling_Matrix(mat);
}




// overloaded variant for those who do not care about have faces_number returned
 bool IVNode::BuildFaceList(int &pnum, int &pmax, int **llist)
{
	int faces_number;
	return IVNode::BuildFaceList(pnum, pmax, llist, faces_number);
}

// parses the IV face list array (where each face is terminated by -1) and converts
// it to HOOPS face list format (where each face begins with num of points in that face)
// and returns the face list and total number of faces
 bool IVNode:: BuildFaceList(int &pnum, int &pmax, int **llist, int &faces_number)
{
 	char token[MVO_BUFFER_SIZE];
	int *list=*llist;

	if (!IVNode::NextToken(token))
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
 
	assert(streq(token,"["));

	faces_number = 0;
	int face_num_idx = 0;
	int face_pt_count = 0;
	int i = 0;
	for (;;)
	{	
		if (!IVNode::NextToken(token))
			return false;

		if (streq(token,"]"))
			break;

		int itoken = atoi(token);

		i++;
		if (i >= pmax)
		{
			int *tmp = new int[(pmax+BUFFERAD)];
			memcpy(tmp,list,sizeof(int)*pmax);
			H_SAFE_DELETE_ARRAY(list);
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
/*****************************************************************************************************************/
// ASHISH.S. 19/02/09-
// defined new node FaceSet. facelist is different here. it has only numbers which is the number of vertices in that face
// and member vertices are just ordered one after one.
// e.g. Coordinate3 {
//            point       [ -281.872 413.484 -49,
//                         -286.569 419.93 -49,
//                         -252.834 428.654 -49 ]
//      }
//      FaceSet {
//            numVertices [ 3 ]
//       }
// here 3 is number of vertices and the face contains points as it is. Hoops facelist will be 3,0,1,2.
/*****************************************************************************************************************/
 bool IVNode::BuildFaceListforFaceSet(int &pnum, int &pmax, int **llist, int &faces_number)
 {
 	char token[MVO_BUFFER_SIZE];
	int *list=*llist;

	if (!IVNode::NextToken(token))
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
 
	assert(streq(token,"["));

	faces_number = 0;
	int face_num_idx = 0;
	int i = 0;
	for (;;)
	{	
		if (!IVNode::NextToken(token))
			return false;

		if (streq(token,"]"))
			break;

		int itoken = atoi(token);

		if (i >= pmax)
		{
			int *tmp = new int[(pmax+BUFFERAD)];
			memcpy(tmp,list,sizeof(int)*pmax);
			H_SAFE_DELETE_ARRAY(list);
			list=tmp;
			pmax+=BUFFERAD;
		}	
		
		list[i] = itoken;
		i++;
		for(int j = 0; j< itoken; j++)
		{
			list[i] = face_num_idx;
			face_num_idx++;
			i++;
		}
		faces_number++;

	}
	pnum = i;
	*llist = list;

	return true;
}

static bool BuildMatrialIndex(int &mnum, int &mmax, int **mlist)
{
 	char token[MVO_BUFFER_SIZE];
	int *list=*mlist;

	if (!IVNode::NextToken(token))
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

	assert(streq(token,"["));
 
	int i = 0;
	for (;;)
	{	
		if (!IVNode::NextToken(token))
			return false;

		if (streq(token,"]"))
			break;

		int itoken = atoi(token);

		if (i >= mmax)
		{
			int *tmp = new int[(mmax+BUFFERAD)];
			memcpy(tmp,list,sizeof(int)*mmax);
			H_SAFE_DELETE_ARRAY(list);
			list=tmp;
			mmax+=BUFFERAD;
		}	

		list[i] = itoken;
		i++;
	}
	 mnum = i;
	*mlist = list;

	return true;
}

bool IVIndexedFaceSet::reprocess_coord_lists(	float *&point_list,   int& point_list_length, 
												int   *&face_list,   int& face_list_length, 
												float *&texture_coord_list, int& texture_coord_list_length, 
												int   *&tlist,   int& tlist_length,
												VArray<float>&  new_point_list,
												VArray<int>&    new_face_list,
												VArray<float>&  new_tex_coord_list
											)
{
	UNREFERENCED(texture_coord_list_length);
	UNREFERENCED(tlist_length);

	int i_n_face_pts = new_face_list[0] = face_list[0];
	int i_cur_point = 1;
	int new_pt_count = point_list_length;
	for (int i = 1; i < face_list_length; i++)
	{
		if( face_list[i] != -2 )
		{
			// a new point has been encountered - copy it's current values
			new_point_list[(face_list[i]*3)]      = point_list[(face_list[i]*3)];  
			new_point_list[(face_list[i]*3)+1]    = point_list[(face_list[i]*3)+1];
			new_point_list[(face_list[i]*3)+2]    = point_list[(face_list[i]*3)+2];
			new_face_list[i]				 = face_list[i];

			new_tex_coord_list[face_list[i]*2]	= texture_coord_list[tlist[i]*2];
			new_tex_coord_list[face_list[i]*2+1]  = texture_coord_list[tlist[i]*2+1];

			// begin to scan the face_list from here to search for occurance of same point
			int j=i+1;
			int j_cur_point;
			int	j_n_face_pts;
			if( i_cur_point >= i_n_face_pts)
			{
				j_n_face_pts = face_list[j];
				j++;
				j_cur_point = 1;
			}
			else
			{
				j_cur_point = i_cur_point + 1;
				j_n_face_pts = i_n_face_pts;
			}

			for(; j < face_list_length; j++)
			{
				if( face_list[i] == face_list[j] )
				{
					// okay, we have got a duplicate point here
					//  lets check if they have same texture coordinates
					if(tlist[i] == tlist[j]) 
					{
						// they are same - no need to add a new point
						// just update our new face_list
						new_face_list[j] = face_list[j];
					}
					else
					{
						// add a new point at the end of pts array
						new_point_list[new_pt_count*3]   = point_list[(face_list[i]*3)];
						new_point_list[new_pt_count*3+1] = point_list[(face_list[i]*3)+1];
						new_point_list[new_pt_count*3+2] = point_list[(face_list[i]*3)+2];

						// update our new face_list
						new_face_list[j] = new_pt_count;

						// update the new texture coords array
						new_tex_coord_list[new_pt_count*2]	= texture_coord_list[tlist[j]*2];
						new_tex_coord_list[new_pt_count*2+1]  = texture_coord_list[tlist[j]*2+1];

						new_pt_count++;
					}

					face_list[j] = -2;			// marked as handled the point
				}
				j_cur_point++;
				if( j_cur_point > j_n_face_pts)
				{
					j++;
					j_n_face_pts = face_list[j];
					j_cur_point = 1;
				}
			}
		}

 		i_cur_point++;
		if( i_cur_point > i_n_face_pts)
		{
			i++;
			if( i < face_list_length )
			{
				i_n_face_pts = new_face_list[i] = face_list[i];
				i_cur_point = 1;
			}
		}

	}
	return true;
}

bool IVIndexedFaceSet::reprocess_attribute_coord_lists(	float *&point_list,   int& point_list_length, 
														int   *&face_list,   int& face_list_length, 
														float *&normals_point_list,   int& normals_point_list_length, 
														int   *&normal_index_list,   int& normal_index_list_length,
														VArray<float>&  new_point_list,
														VArray<int>&    new_face_list,
														VArray<float>&  new_nlist
													)
{
	UNREFERENCED(normals_point_list_length);
	UNREFERENCED(normal_index_list_length);
	UNREFERENCED(texture_coord_list_length);

	int i_n_face_pts = new_face_list[0] = face_list[0];
	int i_cur_point = 1;
	int new_pt_count = point_list_length;
	for (int i = 1; i < face_list_length; i++)
	{
		if( face_list[i] != -2 )
		{
			// a new point has been encountered - copy it's current values
			new_point_list[(face_list[i]*3)]      = point_list[(face_list[i]*3)];  
			new_point_list[(face_list[i]*3)+1]    = point_list[(face_list[i]*3)+1];
			new_point_list[(face_list[i]*3)+2]    = point_list[(face_list[i]*3)+2];
			new_face_list[i]				 = face_list[i];

			new_nlist[face_list[i]*3]	 = normals_point_list[(normal_index_list[i])*3];
			new_nlist[face_list[i]*3+1]  = normals_point_list[(normal_index_list[i]*3)+1];
			new_nlist[face_list[i]*3+2]  = normals_point_list[(normal_index_list[i]*3)+2];

			// begin to scan the face_list from here to search for occurance of same point
			int j=i+1;
			int j_cur_point;
			int	j_n_face_pts;
			if( i_cur_point >= i_n_face_pts)
			{
				j_n_face_pts = face_list[j];
				j++;
				j_cur_point = 1;
			}
			else
			{
				j_cur_point = i_cur_point + 1;
				j_n_face_pts = i_n_face_pts;
			}

			for(; j < face_list_length; j++)
			{
				if( face_list[i] == face_list[j] )
				{
					// okay, we have got a duplicate point here
					//  lets check if they have same normals
					if(normal_index_list[i] == normal_index_list[j]) 
					{
						// they are same - no need to add a new point
						// just update our new face_list
						new_face_list[j] = face_list[j];
					}
					else
					{
						// add a new point at the end of pts array
						new_point_list[new_pt_count*3]   = point_list[(face_list[i]*3)];
						new_point_list[new_pt_count*3+1] = point_list[(face_list[i]*3)+1];
						new_point_list[new_pt_count*3+2] = point_list[(face_list[i]*3)+2];

						// update our new face_list
						new_face_list[j] = new_pt_count;

						// update the new texture coords array
						new_nlist[new_pt_count*3]	 = normals_point_list[(normal_index_list[j]*3)];
						new_nlist[new_pt_count*3+1]  = normals_point_list[(normal_index_list[j]*3)+1];
						new_nlist[new_pt_count*3+2]  = normals_point_list[(normal_index_list[j]*3)+2];

						new_pt_count++;
					}

					face_list[j] = -2;			// marked as handled the point
				}
				j_cur_point++;
				if( j_cur_point > j_n_face_pts)
				{
					j++;
					j_n_face_pts = face_list[j];
					j_cur_point = 1;
				}
			}
		}

 		i_cur_point++;
		if( i_cur_point > i_n_face_pts)
		{
			i++;
			if( i < face_list_length )
			{
				i_n_face_pts = new_face_list[i] = face_list[i];
				i_cur_point = 1;
			}
		}

	}
	return true;
}

bool IVIndexedFaceSet::remove_vertex_sharing (	float *&point_list,   int& point_list_length, 
												int   *&face_list,   int& face_list_length, 
												VArray<float>&  new_point_list,
												VArray<int>&    new_face_list
											  )
{

	int i_n_face_pts = new_face_list[0] = face_list[0];
	int i_cur_point = 1;
	int new_pt_count = point_list_length;
	int i;
	for (i = 1; i < face_list_length; i++)
	{
		if( face_list[i] != -2 )
		{
			// a new point has been encountered - copy it's current values
			new_point_list[(face_list[i]*3)]      = point_list[(face_list[i]*3)];  
			new_point_list[(face_list[i]*3)+1]    = point_list[(face_list[i]*3)+1];
			new_point_list[(face_list[i]*3)+2]    = point_list[(face_list[i]*3)+2];
			new_face_list[i]				 = face_list[i];

			// begin to scan the face_list from here to search for occurance of same point
			int j=i+1;
			int j_cur_point;
			int	j_n_face_pts;
			if( i_cur_point >= i_n_face_pts)
			{
				j_n_face_pts = face_list[j];
				j++;
				j_cur_point = 1;
			}
			else
			{
				j_cur_point = i_cur_point + 1;
				j_n_face_pts = i_n_face_pts;
			}

			for(; j < face_list_length; j++)
			{
				if( face_list[i] == face_list[j] )
				{
					// add a new point at the end of pts array
					new_point_list[new_pt_count*3]   = point_list[(face_list[i]*3)];
					new_point_list[new_pt_count*3+1] = point_list[(face_list[i]*3)+1];
					new_point_list[new_pt_count*3+2] = point_list[(face_list[i]*3)+2];

					// update our new face_list
					new_face_list[j] = new_pt_count;

					new_pt_count++;

					face_list[j] = -2;			// marked as handled the point
				}
				j_cur_point++;
				if( j_cur_point > j_n_face_pts)
				{
					j++;
					j_n_face_pts = face_list[j];
					j_cur_point = 1;
				}
			}
		}

 		i_cur_point++;
		if( i_cur_point > i_n_face_pts)
		{
			i++;
			if( i < face_list_length )
			{
				i_n_face_pts = new_face_list[i] = face_list[i];
				i_cur_point = 1;
			}
		}

	}

 	i_cur_point++;
	if( i_cur_point > i_n_face_pts)
	{
		i++;
		if( i < face_list_length )
		{
			i_n_face_pts = new_face_list[i] = face_list[i];
			i_cur_point = 1;
		}
	}
	return true;
}

bool IVIndexedFaceSet::Convert2Hoops(HC_KEY key, IVNode* parent_node)
{
	strcpy(m_token,"empty");
   	m_segkey = key;
	bool has_points_list = false;
	bool has_normals_list = false;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	int faces_number = 0;
 	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"VertexProperty"))
		{
			m_child_node = GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
				return false;

				point_list = ((IVVertexProperty *)m_child_node)->m_plist;
				point_list_length = ((IVVertexProperty *)m_child_node)->m_plist_length;

				texture_coord_list = ((IVVertexProperty *)m_child_node)->m_tc_list;
			    texture_coord_list_length = ((IVVertexProperty *)m_child_node)->m_tclist_length;

				normals_point_list = ((IVVertexProperty *)m_child_node)->n_plist;
				normals_point_list_length = ((IVVertexProperty *)m_child_node)->n_plist_length;
				strcpy(normal_binding,((IVVertexProperty *)m_child_node)->normal_binding);

				material_list = ((IVVertexProperty *)m_child_node)->m_material_list;
				material_list_length = ((IVVertexProperty *)m_child_node)->m_material_list_length;
				strcpy(material_binding,((IVVertexProperty *)m_child_node)->material_binding);
						
			delete m_child_node;
			m_child_node = 0;

			if(point_list)
				has_points_list = true;

			if(normals_point_list)
				has_normals_list = true;
		}
		if (streq(m_token,"coordIndex"))
		{
			if (!BuildFaceList(face_list_length, face_list_max, &face_list,faces_number))
				return false;
		}
		
		else if (streq(m_token,"textureCoordIndex"))
		{
			if (!BuildFaceList(texture_index_list_length, texture_index_list_max, &texture_index_list))
				return false;
		}
		else if (streq(m_token,"materialIndex"))
		{
			if (streq(material_binding,"PER_FACE_INDEXED"))
			{
				if (!BuildMatrialIndex(material_index_list_length, material_index_max, &material_index_list))
					return false;
			}
			else 
			{
	   		    if (!BuildFaceList(material_index_list_length, material_index_max, &material_index_list))
					return false;

				if (material_index_list_length == 1 && material_index_list[0] == -1 )
				{
					material_index_list_length = face_list_length;
					material_index_list = new int[face_list_length];
					memcpy(material_index_list, face_list, sizeof(int)*material_index_list_length);
				}
			}
		}

		else if (streq(m_token,"normalIndex"))
		{
			if (streq(normal_binding,"PER_FACE_INDEXED"))
			{
				if (!BuildMatrialIndex(normal_index_list_length, normal_index_list_max, &normal_index_list))
					return false;
			}
			else  
			{
				
				if (!BuildFaceList(normal_index_list_length, normal_index_list_max, &normal_index_list))
					return false;

				if (normal_index_list_length == 1 && normal_index_list[0] == -1 )
				{
					normal_index_list_length = face_list_length;
					normal_index_list = new int[face_list_length];
					memcpy(normal_index_list, face_list, sizeof(int)*normal_index_list_length);
				}
			}
		}
	
	}


	if(!has_points_list && parent_node)
	{
		point_list = ((IVSeparator*)parent_node)->m_coord3_list;
		point_list_length = ((IVSeparator*)parent_node)->m_coord3_list_length;
	}

	// The following two lines are for handling VRML1.0 files.
	// Earlier it seems we did not support the case when we find the normals inside separator. 
	// We thought that we will always get it in VertexProperty which is not the case. In such a case we store the 
	// normals in separator an query it here. 

	if(!has_normals_list && parent_node)
	{
		normals_point_list = ((IVSeparator*)parent_node)->normal_point_list;
		normals_point_list_length = ((IVSeparator*)parent_node)->normal_point_list_length;
	}

	// reprocess the points and face list to adjust texture coordinates.
	if( texture_coord_list_length > 0 )
	{
		// if there is no texture coord index array then use coord index array (IV specs)
		bool has_separate_tlist = true;
		if( !texture_index_list )
		{
			has_separate_tlist = false;
			texture_index_list = face_list;
		}

		// create arrays to hold new lists
		VArray<float>  new_point_list(point_list_length*3);	// x, y, z
		VArray<int>    new_face_list(face_list_length);
		VArray<float>  new_tex_coord_list(texture_coord_list_length*2);	// u, v
		reprocess_coord_lists(point_list, point_list_length, face_list, face_list_length, 
							  texture_coord_list, texture_coord_list_length, texture_index_list, texture_index_list_length,
							  new_point_list, new_face_list, new_tex_coord_list);

		if(has_separate_tlist)
		{
			delete[] texture_index_list;
			texture_index_list = 0;
		}

		point_list_length = (int)(new_point_list.Count()/3);
		if (has_points_list)
			delete[] point_list;
		point_list = new float[point_list_length*3];
		new_point_list.GetArrayCopy(point_list);	

		face_list_length = (int)new_face_list.Count();
        delete[] face_list;
		face_list = new int[face_list_length];
		new_face_list.GetArrayCopy(face_list);

		delete[] texture_coord_list;
		texture_coord_list = new float[point_list_length*2];
		new_tex_coord_list.GetArrayCopy(texture_coord_list);
	}
	// The following two lines are for handling VRML1.0 files.
	// Here we have normals but there is no normal index specified (we handle the case by taking coordindex array).
	// Sometimes there is not even normal binding specified so to handle such a situation I am assuming default normal 
	// binding as "PER_VERTEX_INDEXED" and then reprocess the coordinates. Otherwise we can not set normals.
	if(streq(normal_binding, ""))
	{
		strcpy(normal_binding,"PER_VERTEX_INDEXED");
	}
	if( normals_point_list_length > 0 )
	{
		if(!( normal_index_list_length==0 ) && streq(normal_binding,"PER_VERTEX_INDEXED"))
		{
			// HOOPS does not allow setting of normals per-vertex-instance. In this case, 
			// we have to duplicate points which index different normals.

			VArray<float>  new_point_list(point_list_length*3);	// x, y, z
			VArray<int>    new_face_list(face_list_length);
			VArray<float>  new_nlist(normals_point_list_length*3);	// u, v
			reprocess_attribute_coord_lists(point_list, point_list_length, face_list, face_list_length, 
										normals_point_list, normals_point_list_length, normal_index_list, normal_index_list_length,
										new_point_list, new_face_list, new_nlist);


			point_list_length = (int)(new_point_list.Count()/3); 
			if (has_points_list)
				H_SAFE_DELETE_ARRAY(point_list);
			point_list = new float[point_list_length*3];
			has_points_list = true;
			new_point_list.GetArrayCopy(point_list);	

			face_list_length = (int)new_face_list.Count();
			H_SAFE_DELETE_ARRAY(face_list);
			face_list = new int[face_list_length];
			new_face_list.GetArrayCopy(face_list);

			if (has_normals_list)
				H_SAFE_DELETE_ARRAY(normals_point_list);	
			normals_point_list = new float[point_list_length*3];
			has_normals_list = true;
			new_nlist.GetArrayCopy(normals_point_list);
			normals_point_list_length = point_list_length;
		}
		else if (streq(normal_binding,"PER_FACE_INDEXED"))
		{
			// HOOPS does not allow setting of normals per-vertex-instance. In this case, 
			// we have to duplicate points which index different normals.

			VArray<float>  new_point_list(point_list_length*3);	// x, y, z
			VArray<int>    new_face_list(face_list_length);
			remove_vertex_sharing(point_list, point_list_length, face_list, face_list_length, 
								new_point_list, new_face_list);

			point_list_length = (int)(new_point_list.Count()/3); 
			if (has_points_list)
				H_SAFE_DELETE_ARRAY(point_list);
			point_list = new float[point_list_length*3];
			new_point_list.GetArrayCopy(point_list);	

			face_list_length = (int)new_face_list.Count();
			H_SAFE_DELETE_ARRAY(face_list);
			face_list = new int[face_list_length];
			new_face_list.GetArrayCopy(face_list);
		}
	}
	if( (material_list_length > 0) &&  streq(material_binding,"PER_VERTEX_INDEXED"))
	{
		// HOOPS does not allow setting of material per-vertex-instance. In this case, 
		// we have to duplicate points which index different normals.

		VArray<float>  new_point_list(point_list_length*3);	// x, y, z
		VArray<int>    new_face_list(face_list_length);
		VArray<float>  new_material_list(material_list_length*3);	// u, v
		reprocess_attribute_coord_lists(point_list, point_list_length, face_list, face_list_length, 
									 material_list, material_list_length, material_index_list, material_index_list_length,
 									 new_point_list, new_face_list, new_material_list);


		point_list_length = (int)(new_point_list.Count()/3); 
		if (has_points_list)
			H_SAFE_DELETE_ARRAY(point_list);
		point_list = new float[point_list_length*3];
		new_point_list.GetArrayCopy(point_list);	

		face_list_length = (int)new_face_list.Count();
		H_SAFE_DELETE_ARRAY(face_list);
		face_list = new int[face_list_length];
		new_face_list.GetArrayCopy(face_list);

		H_SAFE_DELETE_ARRAY(material_list);	
 		material_list = new float[point_list_length*3];
		new_material_list.GetArrayCopy(material_list);
		material_list_length =  point_list_length;
	}

	HC_Open_Segment_By_Key(m_segkey);

		HC_Set_Visibility("lines=on");

		// We cannot do this if there are texture coordinates.
		//Compute_Shell_Smoothing(&point_list, &point_list_length, &face_list, &face_list_length, 60);
		HC_KEY shell_key;
		
		if (is_tristrip)
			shell_key = HC_KInsert_Shell_By_Tristrips(point_list_length,point_list,face_list_length,face_list,0,0);
		else
			shell_key = HC_KInsert_Shell(point_list_length, point_list, face_list_length, face_list);
		
		if(texture_coord_list_length > 0)
		{
			HC_Open_Geometry(shell_key);
				HC_MSet_Vertex_Parameters (shell_key, 0, point_list_length, 2, texture_coord_list);
			HC_Close_Geometry();
		}
   
		if(normals_point_list_length > 0)
		{
			if(streq(normal_binding,"PER_VERTEX") || (streq(normal_binding,"PER_VERTEX_INDEXED")&&!( normal_index_list_length==0 )))
			{
				HC_Open_Geometry(shell_key);
					HC_MSet_Vertex_Normals (shell_key, 0, normals_point_list_length, normals_point_list);
				HC_Close_Geometry();
			}
			else if (streq(normal_binding,"PER_FACE_INDEXED"))
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
		}

		if(material_list_length > 0)
		{
			if(streq(material_binding,"PER_VERTEX") || streq(material_binding,"PER_VERTEX_INDEXED"))
			{
				if( material_list_length > point_list_length )
					material_list_length = point_list_length;

				HC_Open_Geometry(shell_key);
					HC_MSet_Vertex_Colors_By_Value (shell_key, "faces, edges, vertex", 0, "RGB", material_list_length, material_list);
				HC_Close_Geometry();
			}

			else if (streq(material_binding,"PER_FACE_INDEXED"))
			{
				HC_Set_Color_Map_By_Value ("RGB", material_list_length, material_list);

				HC_Open_Geometry(shell_key);
					for(int m=0; m<faces_number; m++)
					{
						HC_Open_Face(m);
							HC_Set_Color_By_Index ("faces", material_index_list[m]);
						HC_Close_Face();
					}
				HC_Close_Geometry();
			}
		}
		if(has_points_list)     
			H_SAFE_DELETE_ARRAY(point_list);
		if(has_normals_list)
			H_SAFE_DELETE_ARRAY(normals_point_list);
		H_SAFE_DELETE_ARRAY(face_list);
		H_SAFE_DELETE_ARRAY(texture_coord_list);
		H_SAFE_DELETE_ARRAY(texture_index_list);
		H_SAFE_DELETE_ARRAY(material_index_list);
		H_SAFE_DELETE_ARRAY(normal_index_list);
		H_SAFE_DELETE_ARRAY(material_list);
       
	HC_Close_Segment();

	return true;			
}



IVIndexedLineSet::IVIndexedLineSet()
{ 
	face_list = 0; 
	face_list_length = 0; 
	face_list_max = 0; 

	point_list = 0;
	point_list_length = 0;

	clist = 0;
	clist_length = 0;


}

IVLineSet::IVLineSet()
{ 
	
}

IVFaceSet::IVFaceSet()
{ 
	face_list = 0; 
	face_list_length = 0; 
	face_list_max = 0; 
}

bool IVIndexedLineSet::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	bool has_points_list = false;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

 	for (;;)
	{
	
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"coord") )
		{
			m_child_node = FindNode();
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
				return false;
			point_list = ((IVCoordinate *)m_child_node)->m_plist;
			point_list_length = ((IVCoordinate *)m_child_node)->m_plist_length;

			delete m_child_node;
			m_child_node = 0;
		}
		else if (streq(m_token,"VertexProperty"))
		{
			m_child_node = GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
				return false;

			point_list = ((IVVertexProperty *)m_child_node)->m_plist;
			point_list_length = ((IVVertexProperty *)m_child_node)->m_plist_length;

			has_points_list = true;

			delete m_child_node;
			m_child_node = 0;
		}
		else if (streq(m_token,"colorPerIVertex"))
		{
 			if (!GetString(m_token))
				return false;
		}
		else if (streq(m_token,"coordIndex"))
		{
			if (!BuildFaceList(face_list_length, face_list_max, &face_list))
				return false;
		}
		else if (streq(m_token,"color"))
		{
			m_child_node = FindNode();
			if (!m_child_node)
				return false;
			if (!m_child_node->Convert2Hoops (m_segkey))			
				return false;

			clist = ((IVColor *)m_child_node)->m_clist;
			clist_length = ((IVColor *)m_child_node)->m_clist_length;

			delete m_child_node;
			m_child_node = 0;
		}


	}

	if(!has_points_list && parent_node)
	{
		point_list = ((IVSeparator*)parent_node)->m_coord3_list;
		point_list_length = ((IVSeparator*)parent_node)->m_coord3_list_length;
	}
	
	HC_Open_Segment_By_Key(m_segkey); 
		int fpos = 0;
		if(point_list)
		{
			for (;;)
			{
				int plen = face_list[fpos++];
				HPoint *ppoints = new HPoint[plen];
				HPoint *opoints = (HPoint *)point_list;
				for (int i=0;i<plen;i++)
				{
					ppoints[i] = opoints[face_list[fpos]];
					fpos++;
				}
				HC_Insert_Polyline(plen,ppoints);
				H_SAFE_DELETE_ARRAY(ppoints);
				if (fpos>=face_list_length)
					break;
			}
		}
	HC_Close_Segment(); 

	if (has_points_list)
		H_SAFE_DELETE_ARRAY(point_list);

	H_SAFE_DELETE_ARRAY(face_list);
	H_SAFE_DELETE_ARRAY(clist);

 	return true;
}

bool IVFaceSet::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	int faces_number = 0;
 	for (;;)
	{
	
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"numVertices") )
		{
			if (!BuildFaceListforFaceSet(face_list_length, face_list_max, &face_list, faces_number))
				return false;
		}
	}

 	return true;			
}

bool IVLineSet::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	strcpy(m_token,"empty");
	m_segkey = key;

	int point_count = 0;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	for (;;)
	{

		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"numVertices") )
		{
			if (!GetString(m_token))
				return false;

			HCLOCALE(point_count = (int)atoi(m_token));
		}
	}

	if( parent_node)
	{
		HC_Open_Segment_By_Key(m_segkey); 
			HC_Insert_Polyline(point_count, ((IVSeparator*)parent_node)->m_coord3_list);
		HC_Close_Segment(); 
	}
	return true;			
}
bool IVCoordinate::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	if (m_child_node)
	{
		if (!m_child_node->Convert2Hoops (m_segkey))
			return false; 
		delete m_child_node;
		m_child_node = 0;

	}
 	for (;;)
	{
		if (m_level==0)
		{
			m_segkey = key;

			if (!GetString(m_token))
				return false;
			assert(streq(m_token,"{") || streq(m_token,"["));
		
			m_level = 1;
		}
		else if (m_level==1)
		{
			
			if (!GetString(m_token))
				return false;
			if (streq(m_token,"}") || streq(m_token,"]"))
				break;

		}
		
		if (streq(m_token,"point")||streq(m_token,"vertex") || m_level==2)
		{
			if (!BuildPointList(m_plist_length, m_plist_max, &m_plist))
			{
				m_level = 2;
				return false;
			}
			m_level = 1;
		}

	}
 	return true;			
}


bool IVTextureCoordinate::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(key);
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"point"))
		{
			if (!BuildTexPointList(m_tc_list_length, m_tc_list))
				return false;
		}

	}
 	return true;			
}

bool IVTextureCoordinateBinding::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(key);
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"value"))
		{
			if (!GetString(m_token))
				return false;
			if (streq(m_token,"PER_VERTEX"))
			{
				strcpy (texture_binding, m_token);
			}
		}

	}
 	return true;			
}


bool IVShape::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	HC_Open_Segment_By_Key(m_segkey);
	m_segkey = HC_KCreate_Segment("");
	HC_Close_Segment();

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

 	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"appearance") || streq(m_token,"geometry"))
		{
			m_child_node = FindNode();
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
 				return false;

 			delete m_child_node;
			m_child_node = 0;
		}
  
	}

	return true;			
}



IVCube::IVCube() 
{ 
	// this is the default IV 2.0 box size
	m_min.Set( -1.0, -1.0, -1.0 );
	m_max.Set( 1.0, 1.0, 1.0 );
}

IVCube::~IVCube()
{
}



bool IVCube::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));
	float a=0,b=0,c=0;
	
	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"width"))
		{
			if (!Get1Float(a))
				return false;
		}
		if (!GetString(m_token))
			return false;

		if(streq(m_token,"height"))
		{
			if (!Get1Float(b))
				return false;
		}
		
		if (!GetString(m_token))
			return false;

		if(streq(m_token,"depth"))
		{
			if (!Get1Float(c))
				return false;
		}
			m_min.Set( -a/2.0f, -b/2.0f, -c/2.0f );
			m_max.Set( a/2.0f, b/2.0f, c/2.0f );
		

	}

	HC_Open_Segment_By_Key(m_segkey);
	//	HUtility::InsertBox(&m_min, &m_max);
	// this will create a box which is flat shaded even in any 
	// shaded mode (Gouroud etc.) which is more realistic
	HUtilityGeometryCreation::CreateCuboid(&m_max, &m_min );
	HC_Close_Segment();

 	return true;			
}

bool IVRotationXYZ::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	char rot_axis[BUFFERAD];
	strcpy(m_token,"empty");
	m_segkey = key;
	float ang = 0.0f;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{")); 	
	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"axis"))
		{
			if (!IVNode::NextToken(m_token))
     			return false;
			{
				strcpy(rot_axis, m_token);
			}
		}
		else if (streq(m_token,"angle"))
		{
			
			if (!IVNode::Get1Float(ang))
				return false;
		}

	}

	if (streq(rot_axis,"X"))
	{
		rotation.Set(1,0,0,ang);
	}
	else if (streq(rot_axis,"Y"))
	{
		rotation.Set(0,1,0,ang);
	}
	else if (streq(rot_axis,"Z"))
	{
		rotation.Set(0,0,1,ang);
	}
	HC_Rotate_Object_Offaxis(rotation.x,rotation.y, rotation.z,180.0/3.1415927*rotation.rot);
		
 	return true;			
}

IVSphere::IVSphere() 
{ 
	// this are the default for a IV 2.0 sphere
	m_center.Set( 0.0, 0.0, 0.0 );
	m_radius = 1.0;
	m_axis.Set( 1.0, 0.0, 0.0 );
	m_ref.Set( 0.0, 1.0, 0.0 );

	// hoops default
	m_numSides = 20;
}

IVSphere::~IVSphere()
{
}


bool IVSphere::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	if (m_child_node)
	{
		if (!m_child_node->Convert2Hoops (m_segkey))
			return false; 
		delete m_child_node;
		m_child_node = 0;

	}

	m_segkey = key;


	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));
	
	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"radius"))
		{
			float a;
			if (!IVNode::Get1Float(a))
				return false;

			m_radius = a;
		}

	}

	HC_Open_Segment_By_Key(m_segkey);
	HUtilityGeometryCreation::CreateSphere( m_center, m_radius, m_numSides, m_axis, m_ref);
	HC_Close_Segment();

 	return true;			
}


IVCylinder::IVCylinder() 
{ 
	// this are the default for a IV 2.0 sphere
	m_center.Set( 0.0, 0.0, 0.0 );
	m_radius = 1.0;
	m_height = 2.0;
	m_axis.Set( 1.0, 0.0, 0.0 );
	m_ref.Set( 0.0, 1.0, 0.0 );

	// hoops default
	m_numSides = 20;
}



IVCylinder::~IVCylinder()
{
}


bool IVCylinder::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	HPoint Cylinder_point1;
	HPoint Cylinder_point2;

	if (m_child_node)
	{
		if (!m_child_node->Convert2Hoops (m_segkey))
			return false; 
		delete m_child_node;
		m_child_node = 0;

	}

	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));
	
	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;


		if(streq(m_token,"radius"))
		{
			float a;
			if (!IVNode::Get1Float(a))
				return false;
			m_radius = a;
		}
		else if(streq(m_token,"height"))
		{
			float a;
			if (!IVNode::Get1Float(a))
				return false;

			m_height = a;
		}
		// following sphere fields are currently ignored
		// field    SFBool    bottom  TRUE
		// field    SFBool    side    TRUE
		// field    SFBool    top     TRUE		
	}

	Cylinder_point1.Set(m_center.x, m_center.y+m_height/2, m_center.z);
	Cylinder_point2.Set(m_center.x, m_center.y-m_height/2, m_center.z);
	HC_Open_Segment_By_Key(m_segkey);
	//HUtilityGeometryCreation::CreateCylinder( m_center, m_radius, m_height, m_numSides, m_axis, m_ref);
	HC_Insert_Cylinder(&Cylinder_point1, &Cylinder_point2, m_radius, "both");
	HC_Close_Segment();

 	return true;			
}


IVCone::IVCone() 
{ 
	// this are the default for a IV 2.0 cone
	m_center.Set( 0.0, 0.0, 0.0 );
	m_radius = 1.0;
	m_height = 2.0;
	m_axis.Set( 1.0, 0.0, 0.0 );
	m_ref.Set( 0.0, 1.0, 0.0 );

	// hoops default
	m_numSides = 20;
}

IVCone::~IVCone()
{
}


bool IVCone::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	HPoint modified_center;

	if (m_child_node)
	{
		if (!m_child_node->Convert2Hoops (m_segkey))
			return false; 
		delete m_child_node;
		m_child_node = 0;

	}

	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));
	
	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;


		if(streq(m_token,"bottomRadius"))
		{
			float a;
			if (!IVNode::Get1Float(a))
				return false;
			m_radius = a;
		}
		else if(streq(m_token,"height"))
		{
			float a;
			if (!IVNode::Get1Float(a))
				return false;

			m_height = a;
		}
		// following sphere fields are currently ignored
		// field    SFBool    bottom  TRUE
		// field    SFBool    side    TRUE
	}

	// If we fix the issue (#6931) we can remove this workaround. The same is for cylinder also.
	modified_center.Set(m_center.x, m_center.y-(m_height/2), m_center.z);
	HC_Open_Segment_By_Key(m_segkey);
	HUtilityGeometryCreation::CreateCone( modified_center, m_radius, m_height, m_numSides, m_axis, m_ref);
	HC_Close_Segment();

 	return true;			
}

//-----------------------------------------------------------------------------

bool IVSwitch::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	strcpy(m_token,"empty");
	m_segkey = key;
	int temp_count = 0;
	int choice = -1;
	char seg_name[MVO_BUFFER_SIZE];
	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	if (!GetString(m_token))
		return false;

	if (streq(m_token,"whichChild"))
	{
		if (!GetString(m_token))
			return false;
		choice = atoi(m_token);
	}

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;
		
		sprintf(seg_name, "%ld_%d", m_segkey, temp_count);
		HC_Open_Segment(seg_name);

		HC_Set_Visibility("everything = off");
		//if (!GetString(m_token))
			//return false;
		
		m_child_node = GetNodeForToken( m_token );
		if (!m_child_node)
			return false;

		if (!m_child_node->Convert2Hoops(m_segkey, parent_node))			
			return false;

		delete m_child_node;
		m_child_node = 0;

		HC_Close_Segment();
		temp_count++;
	}

	if(choice == -3)
	{
		for (int i = 0; i<temp_count; i++)
		{
			sprintf(seg_name, "%ld_%d", m_segkey, i);
			HC_Open_Segment(seg_name);
			HC_Set_Visibility("everything = on");
			HC_Close_Segment();
		}
	}
	else if(choice != -1)
	{
		sprintf(seg_name, "%ld_%d", m_segkey, choice);
		HC_Open_Segment(seg_name);
		HC_Set_Visibility("everything = on");
		HC_Close_Segment();
	}
	return true;	
}

bool IVGroup::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;
		
		if (streq(m_token,"bboxCenter") || streq(m_token,"bboxSize") || m_level == 3)
		{
			float a,b,c;
			if (!Get3Floats(a, b, c))
				return false;
		}
		else 
		{
			m_child_node = GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
				return false;

			delete m_child_node;
			m_child_node = 0;
		}
	}
	return true;
}

bool IVVertexProperty::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

 	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"vertex"))
		{
			if (!BuildPointList(m_plist_length, m_plist_max, &m_plist))
				return false;
		}
		else if (streq(m_token,"texCoord"))
		{
			if (!BuildTexPointList(m_tclist_length, m_tc_list))
				return false;
		}
		else if (streq(m_token,"orderedRGBA"))
		{
			if (!BuildColorList(m_material_list_length, m_material_list))
				return false;
		}
		else if (streq(m_token,"materialBinding"))
		{
			if (!GetString(m_token))
				return false;

			strcpy (material_binding, m_token);
		}
		else if (streq(m_token,"normal"))
		{
			if (!BuildPointList(n_plist_length, n_plist_max, &n_plist))
       		return false;
		}
		else if (streq(m_token,"normalBinding"))
		{
			if (!GetString(m_token))
				return false;
			
			strcpy (normal_binding, m_token);
		}
			
	}
	
	return true;
}
bool IVShapeHints::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

 		for (;;)
		{
			if (!GetString(m_token))
			return false;
			if (streq(m_token,"}"))
				break;

			if (streq(m_token,"vertexOrdering"))
			{
				HC_Open_Segment_By_Key(m_segkey);

				if (!GetString(m_token))
			        return false;
				if (streq(m_token,"COUNTERCLOCKWISE"))
					    HC_Set_Heuristics ("polygon handedness = right");
				else if (streq(m_token,"CLOCKWISE"))
				    	HC_Set_Heuristics ("polygon handedness = left");
				
				HC_Close_Segment();
			}
		}
	return true;
}

bool IVMatrixTransform::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

 		for (;;)
		{
			if (!GetString(m_token))
			return false;
			if (streq(m_token,"}"))
				break;

			if (streq(m_token,"matrix"))
			{
				HVector axis;
				if (!Get3Floats(axis.x,axis.y, axis.z))
					return false;
				
				float a1;
				if (!Get1Float(a1))
					return false;

				HVector ref;
				if (!Get3Floats(ref.x,ref.y, ref.z))
					return false;
				
				float a2;
				if (!Get1Float(a2))
					return false;
				
				HVector side;
				if (!Get3Floats(side.x,side.y, side.z))
					return false;

				float a3;
				if (!Get1Float(a3))
					return false;

				HVector pos;
				if (!Get3Floats(pos.x,pos.y, pos.z))
					return false;

				float a4;
				if (!Get1Float(a4))
					return false;

				HC_Open_Segment_By_Key(m_segkey);
					IV2HUtils::SetupModellingMatrix(axis,ref,side,pos);	
				HC_Close_Segment();
			}
		}
	return true;
}

bool IVQuadMesh::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	int h_columns=0, h_rows=0;
	for (;;)
		{
			if (!GetString(m_token))
				return false;
			if (streq(m_token,"}"))
				break;
			
			if (streq(m_token,"VertexProperty"))
			{
				m_child_node = GetNodeForToken( m_token );
				if (!m_child_node)
					return false;

				if (!m_child_node->Convert2Hoops (m_segkey))			
					return false;


				m_quad_plist = ((IVVertexProperty *)m_child_node)->m_plist;

				m_quad_nlist = ((IVVertexProperty *)m_child_node)->n_plist;
				m_quad_nlist_length = ((IVVertexProperty *)m_child_node)->n_plist_length;

				m_quad_tclist = ((IVVertexProperty *)m_child_node)->m_tc_list;
			    m_quad_tclist_length = ((IVVertexProperty *)m_child_node)->m_tclist_length;

				delete m_child_node;
				m_child_node = 0;

			}
			else if (streq(m_token,"startIndex"))
			{
					if (!GetString(m_token))
						return false;
			}

			else if (streq(m_token,"verticesPerColumn"))
			{
					 if (!GetString(m_token))
						return false;
					 h_rows = atoi(m_token);
			}

			else if (streq(m_token,"verticesPerRow"))
			{
					 if (!GetString(m_token))
						return false;
					 h_columns = atoi(m_token);
			}
			
		}
				
		HC_Insert_Mesh (h_rows, h_columns, m_quad_plist );

	H_SAFE_DELETE_ARRAY(m_quad_plist);
	H_SAFE_DELETE_ARRAY(m_quad_nlist);
	H_SAFE_DELETE_ARRAY(m_quad_tclist);
	
	return true;
}

bool IVSeparator::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	bool result = true;

	bool transformation_exists = false;
	bool material_exists = false;
	strcpy(m_token,"empty");
	m_segkey = key;

	bool has_point_list = false;
	bool has_normal_list = false;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	HC_Open_Segment_By_Key(m_segkey);
		m_segkey = HC_KCreate_Segment("");
	HC_Close_Segment();

	//	if this separator is having parent, the coord list and normal list should be initialized to
	// the parent's list, this will help the reader to act as step machine.
	if(parent_node)
	{
		m_coord3_list = ((IVSeparator*)parent_node)->m_coord3_list;
		m_coord3_list_length = ((IVSeparator*)parent_node)->m_coord3_list_length;

		normal_point_list = ((IVSeparator*)parent_node)->normal_point_list;
		normal_point_list_length = ((IVSeparator*)parent_node)->normal_point_list_length;
	}

	HC_Open_Segment_By_Key(m_segkey);
 		for (;;)
		{
			if (!GetString(m_token))
			{
				result = false;
				break;
			}
			if (streq(m_token,"}"))
			{
				break;
			}
			
			m_child_node = GetNodeForToken( m_token );
			
			if (streq(m_token,"Coordinate3"))
			{
				if (m_coord3_list_length && has_point_list)
					H_SAFE_DELETE_ARRAY(m_coord3_list);

				m_child_node->Convert2Hoops (m_segkey);
				 
				m_coord3_list = ((IVCoordinate *)m_child_node)->m_plist;
				m_coord3_list_length = ((IVCoordinate *)m_child_node)->m_plist_length;

				has_point_list = true;

				delete m_child_node;
				m_child_node = 0;
				
			}
			else if (streq(m_token,"VertexProperty"))
			{
				if (m_coord3_list_length && has_point_list)
					H_SAFE_DELETE_ARRAY(m_coord3_list);

				m_child_node->Convert2Hoops (m_segkey);

				m_coord3_list = ((IVCoordinate *)m_child_node)->m_plist;
				m_coord3_list_length = ((IVCoordinate *)m_child_node)->m_plist_length;

				has_point_list = true;

				delete m_child_node;
				m_child_node = 0;

			}
			else if (streq(m_token,"Normal"))
			{
				normal_point_list = 0;
				normal_point_list_length = 0;

				m_child_node->Convert2Hoops (m_segkey);

				normal_point_list = ((IVNormal *)m_child_node)->n_plist;
				normal_point_list_length = ((IVNormal *)m_child_node)->n_plist_length;

				has_normal_list = true;

				delete m_child_node;
				m_child_node = 0;

			}
			// ASHISH.S. 18/02/09
			else if (streq(m_token,"FaceSet"))
			{
				face_list = 0; 
				face_list_length = 0; 
				face_list_max = 0;

				m_child_node->Convert2Hoops (m_segkey);
				
				face_list = ((IVFaceSet *)m_child_node)->face_list;
				face_list_length = ((IVFaceSet *)m_child_node)->face_list_length;
				face_list_max =  ((IVFaceSet *)m_child_node)->face_list_max;

				delete m_child_node;
				m_child_node = 0;

			}
			// ASHISH.S. 18/02/09
			else if (streq(m_token,"Transform") || streq(m_token,"Translation") || streq(m_token,"Rotation") || streq(m_token,"Scale"))
			{
				transformation_exists = true;
	
				m_child_node->Convert2Hoops (m_lTransformation_key);

				delete m_child_node;
				m_child_node = 0;
			}
			// VRML1.0 specs says Material node defines the current surface material properties for all subsequent shapes
			// Thats why we have adopted following strategy.
			else if (streq(m_token,"Material"))
			{
				material_exists = true;

				m_child_node->Convert2Hoops (m_lMaterial_key);

				delete m_child_node;
				m_child_node = 0;
			}
			else 
			{
				if(streq(m_token,"IVDummy")||strieq(m_token,"DEF")||strieq(m_token,"USE"))
				{
					if (!m_child_node || !m_child_node->Convert2Hoops (m_segkey, this))
					{ 
						result = false;
						break;
					}
				}
				else
				{
					HC_KEY new_segment = HC_KOpen_Segment("");
					HC_Close_Segment();

					if (!m_child_node || !m_child_node->Convert2Hoops (new_segment, this))
					{ 
						result = false;
						break;
					}
					if(material_exists) // to optimise
					{
						//  first copy colors from material segment.
						char color_attributes[MVO_BUFFER_SIZE];
						HC_Open_Segment_By_Key(m_lMaterial_key);
							HC_Show_Color(color_attributes);
						HC_Close_Segment();
						// Add colors to new segment and transformations to new segment
						HC_Open_Segment_By_Key(new_segment);
							HC_Set_Color(color_attributes);
						HC_Close_Segment();
					}
					if(transformation_exists) // to optimise
					{
						// first copy modeling transformation from transformation segment.
						float matrix[16] = {
											1,0,0,0,
											0,1,0,0,
											0,0,1,0,
											0,0,0,1
											};
						HC_Open_Segment_By_Key(m_lTransformation_key);
							HC_Show_Modelling_Matrix(matrix);
						HC_Close_Segment();
						// Add transformations to new segment
						HC_Open_Segment_By_Key(new_segment);
							HC_Set_Modelling_Matrix(matrix);
						HC_Close_Segment();
					}
				}
				delete m_child_node;
				m_child_node = 0;
			}
		}
		
		if(face_list && normal_point_list)
		{
			HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes(
			(HPoint*) m_coord3_list, face_list_length, face_list,
			(HPoint*)normal_point_list, face_list
			);
		}
		if(face_list && !normal_point_list)
		{
			HUtilityGeometryCreation::CreateShellWithFaceSpecificVertexAttributes( 
			(HPoint*) m_coord3_list, face_list_length, face_list
			);
		}

	if(has_point_list)
		H_SAFE_DELETE_ARRAY(m_coord3_list);
	if(has_normal_list)
		H_SAFE_DELETE_ARRAY(normal_point_list);
	H_SAFE_DELETE_ARRAY(face_list);

	HC_Close_Segment();
	return result;
}

//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 11:30:29 AM
//======================================================================
//	Function :		IVTransform::Create
//	Description :	Creates a new sub-segment corresponding to tranform
//					node of IV file.
//	Return :		bool	-	true if succeeds
//	Parameters :
//			HC_KEY key	-	key of the parent node
//	Note :
//======================================================================
bool IVTransform::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

 	float a,b,c,d;
	strcpy(m_token,"empty");

	// create a new sub-segment under given parent segment key
	//HC_Open_Segment_By_Key(key);
	//m_segkey = HC_KCreate_Segment("");
	//HC_Close_Segment();

	m_segkey = key;
	VERIFY_NEXT_TOKEN( m_token )
	assert(streq(m_token,"{"));

 	for (;;)
	{
		VERIFY_NEXT_TOKEN( m_token )
		if (streq(m_token,"}"))
			break;
		
		else if (streq(m_token,"center") )
		{
 			if (!Get3Floats(a, b, c))
				return false;
 			center.Set(a,b,c);
		}
		else if (streq(m_token,"scaleFactor") )
		{
 			if (!Get3Floats(a, b, c))
				return false;
 			scale.Set(a,b,c);
		}
		else if (streq(m_token,"translation") )
		{
 			if (!Get3Floats(a, b, c))
				return false;
 			translation.Set(a,b,c);
		}
		else if (streq(m_token,"rotation") )
		{
 			if (!Get4Floats(a, b, c,d))
				return false;
 			rotation.Set(a,b,c,d);
		}
		else if (streq(m_token,"scaleOrientation") )
		{
 			if (!Get4Floats(a, b, c,d))
				return false;
 			scaleRotation.Set(a,b,c,d);
		}
 
	}

	HC_Open_Segment_By_Key(m_segkey);
		
	HC_Translate_Object(-center.x, -center.y, -center.z);

	if(scaleRotation.rot!=0.0f)
		HC_Rotate_Object_Offaxis(scaleRotation.x,scaleRotation.y, scaleRotation.z,-180.0/3.1415927*scaleRotation.rot);

	HC_Scale_Object(scale.x,scale.y,scale.z);

	if(scaleRotation.rot!=0.0f)
		HC_Rotate_Object_Offaxis(scaleRotation.x,scaleRotation.y, scaleRotation.z,180.0/3.1415927*scaleRotation.rot);

	if(rotation.rot!=0.0f)
		HC_Rotate_Object_Offaxis(rotation.x,rotation.y, rotation.z,180.0/3.1415927*rotation.rot);

	HC_Translate_Object(center.x, center.y, center.z);

	HC_Translate_Object(translation.x, translation.y, translation.z);
	HC_Close_Segment();
	
	return true;
			
}

IVChildren::IVChildren() 
{ 
}

IVChildren::~IVChildren()
{
}


//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 10:26:47 AM
//======================================================================
//	Function :		IVChildren::Create
//	Description :	Corresponds to children node. Creates a HOOPS named 
//					segment and goes on to create the next node under it
//	Return :		bool - true if succeeds
//	Parameters :
//			HC_KEY key	-	
//	Note :			1. SPECIAL HANDLING FOR 'USE' - NO OPENING/CLOSING BRACE
//======================================================================
bool IVChildren::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	bool is_multiline = false;

	for(;;)
	{

		VERIFY_NEXT_TOKEN( m_token );

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
			m_child_node = GetNodeForToken( m_token );
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
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




//======================================================================
//	Author :Rajesh B			Created : 1/18/2001 10:42:47 AM
//======================================================================
//	Function :		IVDef::Create
//	Description :	Corresponds to DEF node. Creates a HOOPS named 
//					segment and goes on to create the next node under it
//	Return :		bool - true if succeeds
//	Parameters :
//			HC_KEY key	-	
//	Note :			TODO: HOW TO REPLACE AN EXISTING NAMED SEGMENT WITH
//					THE NEW DEFINITION ?
//======================================================================
bool IVDef::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");

	// get the name of the definition
	VERIFY_NEXT_TOKEN( m_token );

	// TODO: How delete any existing segment by this definition name
	// HC_Delete_Segment( m_token );

//	// create a new name segment
//	HC_Open_Segment_By_Key(m_segkey);
//	m_segkey = HC_KCreate_Segment(m_token);
//	HC_Close_Segment();

//	m_segkey = CreateNamedSubsegment( m_segkey, m_token );

	// create a new segment in the include library and use it here
	m_segkey = CreateNamedSubsegment( DEF_NODE_LIB, m_token, 0 );
	IncludeNamedSegment( key, DEF_NODE_LIB, m_token );

	// find the node and create it
	bool finish=false;
	m_child_node = FindNode(&finish);
	if (!m_child_node)
		return false;
	else
	{
		if (!m_child_node->Convert2Hoops (m_segkey))			
			return false;

		delete m_child_node;
		m_child_node = 0;
	}
	
	return true;
}



//======================================================================
//	Author :Rajesh B			Created : 1/18/2001 10:42:47 AM
//======================================================================
//	Function :		IVUse::Create
//	Description :	Instancing in IV 2.0. We create this node when 
//					the USE keyword is encountered. 
//	Return :		bool - true if succeeds
//	Parameters :
//			HC_KEY key	-	
//	Note :			It is assumed that a named segment is already in the 
//					HOOPS database (created when DEF keyword is encountered)
//======================================================================
bool IVUse::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	// get the name of the definition to use
	VERIFY_NEXT_TOKEN( m_token );

	IncludeNamedSegment( m_segkey, DEF_NODE_LIB, m_token );


 	return true;			
}


extern "C"
{
	extern bool gzip_buffer_ready;
	extern int gzip_buffersize;
	extern unsigned char *gzip_buffer;
}


//======================================================================
//	Author :Rajesh B			Created : 1/19/2001 3:16:23 PM
//======================================================================
//	Function :		IVNode::KCreateNamedSubsegment
//	Description :	Creates a new segment under given segment key and 
//					assigns the alias	
//	Return :HC_KEY-	key of new subsegment created
//	Parameters :
//			HC_KEY parent_key	-	key of super-segment
//			char* alias	-		alias for the new subsegment
//	Note :	TODO: HANDLE CASE WHERE THE ALIAS IS ALREADY DEFINED?
//======================================================================
HC_KEY IVNode::KCreateNamedSubsegment( HC_KEY parent_key, const char* alias )
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
//	Function :		IVNode::CreateNamedSubsegment
//	Description :	Creates a new segment under given segment Path and 
//					assigns the alias. If there is already a segment
//					by that name in include library, it is renamed by 
//					adding some unique number to it's name. This is done to comply 
//					with the IV specs (behaviour observed in Cosmo Player)
//	Return :HC_KEY-	key of new subsegment created
//	Parameters :
//			char* segmentPath - library path where the key needs 
//								to be stored
//			char* segmentName - Name for the new subsegment 
//			char* alias	-		alias for the new subsegment
//======================================================================
HC_KEY IVNode::CreateNamedSubsegment( const char* segmentPath, const char* segmentName, const char* alias )
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
//	Function :		IVNode::IncludeNamedSegment
//	Description :	Includes a name segment in given parent
//	Return :bool	-	true if succeeds
//	Parameters :
//			HC_KEY parent_key	-	key for parent segment
//			char* alias	-		alias of the name segment
//	Note :	ASSUMPTION:	ALIAS IS DEFINED FOR AN EXISITING SEGMENT
//======================================================================
bool IVNode::IncludeNamedSegment( HC_KEY parent_key, const char* alias )
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
//	Function :		IVNode::IncludeNamedSegment
//	Description :	Includes a name segment in given parent
//	Return :bool	-	true if succeeds
//	Parameters :
//			HC_KEY parent_key	-	key for parent segment
//			char* alias	-		alias of the name segment
//	Note :	ASSUMPTION:	ALIAS IS DEFINED FOR AN EXISITING SEGMENT
//			TODO: PERFORM A CHECK IF THE SEGMENT IS GEOMETRY OR STYLE
//======================================================================
bool IVNode::IncludeNamedSegment( HC_KEY parentKey, const char* segmentPath, const char* segmentName )
{
	// open parent for include
	HC_Open_Segment_By_Key(parentKey);
		
		// include segment - hoops requires alias to start with "?"
		char seg_fullpath[MVO_SEGMENT_PATHNAME_BUFFER];
		strcpy( seg_fullpath, segmentPath);
		strcat( seg_fullpath, segmentName );
		
//		HC_Open_Segment(seg_fullpath);
//		HC_Begin_Contents_Search("./...","geometry");
//		int count; 
//		HC_Show_Contents_Count(&count);
//		HC_End_Contents_Search();
//		HC_Close_Segment();

		HC_Include_Segment( seg_fullpath );
		HC_Style_Segment( seg_fullpath );

	HC_Close_Segment();

	return true;
}


bool IVNode::BuildPointList(int &pnum, int &pmax, float **llist)
{
 	char token[255];
	float *list=*llist;
	if (!pmax)
	{
		pmax = BUFFERAD;
		list = new float[pmax*3];
 		pnum = 0;

		if (!IVNode::GetString(token))
 			return false;
		
		if(!streq(token,"["))
		{
			// only one point exists (since there are no square brackets)
			HCLOCALE(list[pnum*3]= (float)atof(token));
 			
			if(!IVNode::NextToken(token))
				goto breakout;
  			HCLOCALE(list[pnum*3+1]=(float)atof(token));
 			
			if(!IVNode::NextToken(token))
				goto breakout;
  			HCLOCALE(list[pnum*3+2]=(float)atof(token));

			pnum++;

			*llist = list;
			return true;
		}	
	}

	assert(streq(token,"["));
 	for (;;)
	{
		if (pnum>=pmax)
		{
			float *tmp = new float[(pmax+BUFFERAD)*3];
			memcpy(tmp,list,sizeof(float)*3*pmax);
			delete [] list;
			list=tmp;
			pmax+=BUFFERAD;
		}
 		if(!IVNode::NextToken(token))
			goto breakout;
 		if (streq(token,"]"))
		{
			*llist = list;
			return true;
		}
		HCLOCALE(list[pnum*3]= (float)atof(token));
 		if(!IVNode::NextToken(token))
			goto breakout;
  		HCLOCALE(list[pnum*3+1]=(float)atof(token));
 		if(!IVNode::NextToken(token))
			goto breakout;
  		HCLOCALE(list[pnum*3+2]=(float)atof(token));
		pnum++;
	}

breakout:
 	*llist = 0;
	H_SAFE_DELETE_ARRAY(list);
	return false;
}

bool IVNode::BuildTexPointList(int& tc_num, float*& texture_coord_list)
{
 	char token[TOKEN_BUFFER_SIZE];

	if (!GetString(token))
		return false;
	assert(streq(token,"["));

	tc_num = 0;
	int tc_max = BUFFERAD;
	texture_coord_list = new float[tc_max*2];

	for (;;)
	{
		if (tc_num>=tc_max)
		{
			float *tmp = new float[(tc_max+BUFFERAD)*2];
			memcpy(tmp,texture_coord_list,sizeof(float)*2*tc_max);
			H_SAFE_DELETE_ARRAY(texture_coord_list);
			texture_coord_list=tmp;
			tc_max+=BUFFERAD;
		}

 		if(!IVNode::NextToken(token))
		{
			delete[] texture_coord_list;
			texture_coord_list = 0;
			return false;
		}

 		if (streq(token,"]"))
			break;

		HCLOCALE(texture_coord_list[tc_num*2]= (float)atof(token));

 		if(!IVNode::NextToken(token))
		{
			delete[] texture_coord_list;
			texture_coord_list = 0;
			return false;
		}

  		HCLOCALE(texture_coord_list[tc_num*2+1]=(float)atof(token));

		tc_num++;
   	}

	return true;
}
	

bool IVNode::BuildColorList(int &material_list_length, float *&material_list)
{
	material_list_length = 0;
	material_list = 0;

	char token[TOKEN_BUFFER_SIZE];
	strcpy(token,"empty");
	
	if (!IVNode::GetString(token))
			return false;

	assert(streq(token,"[")); 

	VArray<float>  hcolor_array(10*3);		// r, g, b
	int i = 0;
	for (;;)
	{
		if (!IVNode::GetString(token))
			return false;

		if (streq(token,"]"))
			break;

		unsigned char RGBA[4];

		if(!IVNode::Token2RGBA( token, 4, RGBA))
			return false;
			
		hcolor_array[i*3]   = RGBA[0]/255.0f;
		hcolor_array[i*3+1] = RGBA[1]/255.0f;
		hcolor_array[i*3+2] = RGBA[2]/255.0f;

		i++;
	}

	if( i > 0 )
	{
		material_list = new float[i*3];
		hcolor_array.GetArrayCopy(material_list);	
		material_list_length = i;
	}

	return true;
}



static bool fill_buffer()
{
	static int counter=-1;	
	static int buffer_fill_count=0;
	counter++;
	buffer_fill_count++;
	int size = 0;
// 	if ((counter % 2) == 0)
	{
	buffersize=5000;

	H_SAFE_DELETE_ARRAY(fbuffer);

	fbuffer = new unsigned char[buffersize+1];
	memset(fbuffer, 0, buffersize+1);
	size = (int)fread(fbuffer,1,buffersize,fhandle);

	if(buffer_fill_count > 25)
	{
		unsigned long now = ftell(fhandle);
		IVHandler->ReportInputPercentProgress(now/(float)m_file_size);
		buffer_fill_count = 0;
	}

	if( ferror( fhandle ) ) 
	{
		assert(!"Error reading file");
		return false;
	}

	if( feof( fhandle ) )
	{
		if(!size)
		{
			return false;	
		}
	}
	if (save_fbuffer_length)
	{
		unsigned char *temp = new unsigned char [buffersize+save_fbuffer_length];
		memcpy(temp,save_fbuffer,save_fbuffer_length);
		memcpy(temp+save_fbuffer_length,fbuffer,buffersize);
		buffersize+=save_fbuffer_length;
		H_SAFE_DELETE_ARRAY(fbuffer);
		fbuffer = temp;
	}

 	return true;
	}
}

 
 

bool IVNode::NextToken(char *token)
{
	// initialize the token with a null string. We will find nothing at the end of the file. 
	// But we dont want token to have garbage value.

	token[0] = '\0';

	int tokenstate=NOTINTOKEN;
	int tokenpos=0;
	for (;;)
	{
		ENSURE_BUFFER_READ

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
bool IVNode::NextToken(char *token, int skip)
{
	for( int i = 0; i <= skip; i++ )
	{
		if( !IVNode::NextToken(token) )
			return false;
	}
	return true;
}


bool IVNode::GetQuotedString(char *token)
{
	int tokenstate=NOTINTOKEN;
	int tokenpos=0;

	tokenstate=tokenstate;

	while( fbuffer[fposition]==' ' || fbuffer[fposition]== '\t')
		fposition++;

	if (fbuffer[fposition]!='\"')
	{
		assert(!"Expecting opening double quote");
		return false;
	}
		
	fposition++;

	tokenstate=INTOKEN;

	for (;;)
	{
		ENSURE_BUFFER_READ

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

 


static void MarkBuffer()
{
	save_fposition=fposition;
}

static void SaveBuffer()
{
	save_fbuffer_length = buffersize-save_fposition;
	H_SAFE_DELETE_ARRAY(save_fbuffer);

	save_fbuffer = new char[save_fbuffer_length];
	memcpy(save_fbuffer,fbuffer+save_fposition,save_fbuffer_length);
}


bool IVNode::GetString(char *string)
{
	if (IVNode::NextToken(string))
		return true;
	else
		return false;
}



bool IVNode::Token2RGBA(char * token, const int n_components, unsigned char * RGBA)
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

	char hex_str = static_cast<char>(IVNode::HexStr2Int(hex_pix));
	
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
	hex_str = static_cast<char>(IVNode::HexStr2Int(hex_pix));
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
	hex_str = static_cast<char>(IVNode::HexStr2Int(hex_pix));
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
	hex_str = static_cast<char>(IVNode::HexStr2Int(hex_pix));
	RGBA[0] = hex_str;

	return true;
}


int IVNode::HexStr2Int(const char *hex_str)
{
	
	int result;
	sscanf(hex_str,"%2x", &result);
	return result;
}


bool IVNode::GetBool(bool& true_or_false)
{
	char token[TOKEN_BUFFER_SIZE];
	VERIFY_NEXT_TOKEN( token );

	if(streq(token, IV_TRUE_STR))
		true_or_false = true;
	else if(streq(token, IV_FALSE_STR))
		true_or_false = false;
	else
		return false;

	return true;
}


bool IVNode::Get1Float(float &a)
{
	char token[255];
	MarkBuffer();

	if (!IVNode::NextToken(token))
	{
		SaveBuffer();
		return false;
	}
	HCLOCALE(a = (float)atof(token));
	return true;
}


bool IVNode::Get3Floats(float &a, float &b, float &c)
{
	char token[255];
	MarkBuffer();

	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(a = (float)atof(token));
	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(b = (float)atof(token));
	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(c = (float)atof(token));	
	return true;

no_token:	
	SaveBuffer();
	return false;
}

bool IVNode::Get4Floats(float &a, float &b, float &c, float &d)
{
	char token[255];
	MarkBuffer();

	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(a = (float)atof(token));
	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(b = (float)atof(token));
	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(c = (float)atof(token));	
	if (!IVNode::NextToken(token))
		goto no_token;
	HCLOCALE(d = (float)atof(token));	
	return true;

no_token:	
	SaveBuffer();
	return false;
}

static bool SkipBracket()
{
 	char btype_close = ' ';
	int level=1;
 	char btype_open = ' ';

 	for (;;)
	{
		ENSURE_BUFFER_READ
 		
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
		ENSURE_BUFFER_READ

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


//======================================================================
//	Author :Rajesh B			Created : 1/25/2001 10:01:17 AM
//======================================================================
//	Function :		IVNode::RemoveQuotes
//	Description :	Removes double quotes from the string. The resultant
//					string is returned in the argument itself
//	Return :bool	-	false if fails
//	Parameters :
//			char* string	-	
//	Note :			WORLDS SLOPPIEST WAY OF DOING THIS THING. I KNOW 
//					I COULD HAIVE DONE BETTER THAT THIS
//======================================================================
bool IVNode::RemoveQuotes(char* string)
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




IVNode* IVNode::FindNode(bool *closebracket)
{
	char token[255];
 	if (!IVNode::NextToken(token))
		return 0;
	if (closebracket && streq(token,"]"))
	{
		*closebracket = true;
		return 	(IVNode *)1;
	}
 
//	strcpy(currentname,"empty");

	return GetNodeForToken(token);

}


// find the next node, skip and DEF nodes encountered in between
IVNode* IVNode::FindNodeSkipDEF(bool *closebracket)
{
	char token[255];
 	if (!IVNode::NextToken(token))
		return 0;
	if (closebracket && streq(token,"]"))
	{
		*closebracket = true;
		return 	(IVNode *)1;
	}
	if( streq(token,"DEF") )
	{
		// skip the definition name and get the next token
 		if (!IVNode::NextToken(token, 1))
			return 0;
	}
 
//	strcpy(currentname,"empty");

	return GetNodeForToken(token);

}


//======================================================================
//	Author :Rajesh B			Created : 1/18/2001 4:20:59 PM
//======================================================================
//	Function :		IVNode::GetNodeForToken
//	Description :	Given a token, function creates a node object
//	Return :		IVNode*	- IVDummy if token is not supported node
//	Parameters :
//			char* token	-	token name
//	Note :
//======================================================================
IVNode* IVNode::GetNodeForToken(char* token)
{
 	if (streq(token,"Separator"))
		return new IVSeparator;
    else if (streq(token,"QuadMesh"))
		return new IVQuadMesh;
	else if (streq(token,"IndexedFaceSet"))
		return new IVIndexedFaceSet;
	else if (streq(token,"VertexProperty"))
		return new IVVertexProperty;
	else if (streq(token,"Material"))
		return new IVMaterial;
	else if (streq(token,"DEF"))
		return new IVDef;
	else if (streq(token,"USE"))
		return new IVUse;
	else if (streq(token,"Transform"))
		return new IVTransform;
	else if (streq(token,"Translation"))
		return new IVTransform;
	else if (streq(token,"Rotation"))
		return new IVTransform;
	else if (streq(token,"Scale"))
		return new IVTransform;
	else if (streq(token,"TextureCoordinate2"))
		return new IVTextureCoordinate;
	else if (streq(token,"TextureCoordinateBinding"))
		return new IVTextureCoordinateBinding;
 	else if (streq(token,"Texture2"))
		return new IVPixelTexture;
	else if (streq(token,"IndexedTriangleStripSet"))
		return new IVIndexedFaceSet(true);
	else if (streq(token,"ShapeHints"))
		return new IVShapeHints;
    else if (streq(token,"MatrixTransform"))
		return new IVMatrixTransform;
	else if (streq(token,"Cylinder"))
		return new IVCylinder;
	else if (streq(token,"Cube"))
		return new IVCube;
	else if (streq(token,"RotationXYZ"))
		return new IVRotationXYZ;
	else if (streq(token,"Cone"))
		return new IVCone;
	else if (streq(token,"Group"))
		return new IVGroup;
	else if (streq(token,"Switch"))
		return new IVSwitch;
	else if (streq(token,"Text2"))
		return new IVText;
	else if (streq(token,"Text3"))
		return new IVText(true);
	else if (streq(token,"Font"))
		return new IVFont;
	else if (streq(token,"Coordinate3"))
		return new IVCoordinate;
	else if (streq(token,"Collision"))
		return new IVGroup;
	else if (streq(token,"PixelTexture"))
		return new IVPixelTexture;
 	else if (streq(token,"LOD"))
		return new IVLOD;
	else if (streq(token,"Shape"))
		return new IVShape;
	else if (streq(token,"IndexedLineSet"))
		return new IVIndexedLineSet;
	else if (streq(token,"vertex"))
		return new IVCoordinate;
	else if (streq(token,"Color"))
		return new IVColor;
	else if (streq(token,"Appearance"))
		return new IVAppearance;
	else if (streq(token,"ImageTexture"))
		return new IVImageTexture;
	else if (streq(token,"Sphere"))
		return new IVSphere;
	else if (streq(token,"children"))
		return new IVChildren;
	else if (streq(token,"Normal"))
		return new IVNormal;
	else if (streq(token,"LineSet"))
		return new IVLineSet;
	else if (streq(token,"FaceSet"))// ASHISH.S. 19/02/09
		return new IVFaceSet;
	else return new IVDummy;
}

static bool ivtoHOOPS(const wchar_t *filename, HC_KEY key)
{
 	buffersize=-1;
	fposition=0;
	IVNode *this_node=0;
	fhandle = wfopen(filename, L"rb");
	if( !fhandle )
	{
		return false;
	}
		
	unsigned long now = ftell(fhandle);
	fseek(fhandle, 0, SEEK_END);
	unsigned long file_size = ftell(fhandle);
	m_file_size = file_size - now;
	fseek(fhandle, now, SEEK_SET);
	
	strcpy(IVNode::iv_file_path, (char*)H_UTF8(filename).encodedText());
	// IV uses a Cartesian, right-handed, 3-dimensional coordinate system. 
	HC_Open_Segment_By_Key(key);
		HC_Set_Handedness("Right");
	HC_Close_Segment();

	// cycle until end of file reached
	while(!feof( fhandle ))
	{
		this_node = IVNode::FindNode();
		assert(this_node);

		if (!this_node->Convert2Hoops(key))
			return false;

		delete this_node;
		this_node = 0;
	}

	H_SAFE_DELETE_ARRAY(save_fbuffer);
	H_SAFE_DELETE_ARRAY(fbuffer);
	fbuffer = 0;

	fclose(fhandle);
	return true;
 
}

 

IVNode::IVNode( char *name )
{
	UNREFERENCED(name);

	m_child_node = 0;
	m_segopen=false;
	m_level=0;
	m_segkey=0;
}

IVNode::~IVNode()
{
	if (m_segopen)
		HC_Close_Segment();

//	if( m_name )
//		delete[] m_name;
}

 



bool IVLOD::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));


//			if (strcmp(currentname,"empty")!=0)
//			{
//				HC_Open_Segment_By_Key(m_segkey);
//				m_segkey = HC_KCreate_Segment(currentname);
//				HC_Close_Segment();
//			}

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"level"))
		{
 			for (;;)
			{
				if (firstrun)
				{
					if (!IVNode::NextToken(m_token))
						return false;
				}
 
 				bool finish=false;
				m_child_node = FindNode(&finish);
 
				if (!m_child_node)
					return false;

				if (finish)
				{
					m_child_node = 0;
					break;
				}
				firstrun = false;
				if (!m_child_node->Convert2Hoops (m_segkey))			
					return false;

				delete m_child_node;
				m_child_node = 0;
				
			}
		}
		else if (streq(m_token,"center"))
		{
			float a,b,c;
			if (!Get3Floats(a, b, c))
				return false;
		}
		else if (streq(m_token,"range"))
		{
			for (;;)
			{
				if (!GetString(m_token))
					return false;

				if (streq(m_token,"]"))
					break;
			}
		}

 
	}
	return true;

			
}




bool IVColor::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"color"))
		{
			if (!BuildPointList(m_clist_length, m_clist_max, &m_clist))
				return false;

		}

	}
 	return true;			
}

 

bool IVAppearance::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{")); 

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"material"))
		{
			m_child_node = FindNode();
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
 				return false;

 			delete m_child_node;
			m_child_node = 0;
		}
  
		else if (streq(m_token,"texture"))
		{
            m_child_node = FindNode();
			if (!m_child_node)
				return false;

			if (!m_child_node->Convert2Hoops (m_segkey))			
 				return false;

 			delete m_child_node;
			m_child_node = 0;
		
		}
	}

	return true;			
}

static void generate_unique_texture_name(char * new_texture_name)
{
	  time_t ltime;
	  time( &ltime );
	  sprintf(new_texture_name,"%s%ld",IV_TEXTURE_BASE_NAME, ltime);
}


//======================================================================
//	Author : Pavan T			Created : 1/20/2006 
//======================================================================
//	Function :		IVNode::GetStringInSquareBracket
//	Description :	Removes square brackets (if present) and quotes and 
//					returns the string
//	Return :bool	-	false if fails
//	Parameters :    the string to be returned. 
//  NOTE:			The caller is assumed to have allocated enough memory 
//					to the return string (string_in_bracket argument)
//======================================================================
bool IVNode::GetStringInSquareBracket(char *string_in_bracket)
{
	if (!IVNode::NextToken(string_in_bracket))
		return false;

	if (streq(string_in_bracket,"["))
	{
		if( !IVNode::GetQuotedString(string_in_bracket))
			return false;
		
		char ignore_token[MVO_BUFFER_SIZE];
		assert_NEXT_TOKEN(ignore_token,"]");
	}
	else 
	{
		IVNode::RemoveQuotes( string_in_bracket );
	}

	return true;
}

bool IVImageTexture::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;
	char image_file_path[MVO_BUFFER_SIZE];
	bool is_repeat_S = false;
	bool is_repeat_T = false;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{")); 

	for (;;)
	{
		if (!GetString(m_token))
			return false;

		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"url"))
		{
			if (!IVNode::GetStringInSquareBracket(image_file_path))
						return false;	

			// figure out if this is URL or just a filename without any path.
			if( !strchr(image_file_path, ':' ))
			{
				// if this is just a simple name (e.g. "brick.jpg") then we will try 
				// to locate the texture file in the same directory as the iv file.

				// truncate at the first '\' to '/' from right
				char iv_file_dir[MVO_BUFFER_SIZE];
				strcpy(iv_file_dir, iv_file_path);
				char *end = iv_file_dir + strlen(iv_file_dir);
				while (end >= iv_file_dir)
				{
					if ((*end) == '/' || (*end) == '\\')
					{
						end++;
						break;
					}
					end--;			
				}
				*end = '\0';					

				strcat(iv_file_dir, image_file_path);
				strcpy(image_file_path, iv_file_dir);
			}
		}
		else if (streq(m_token,"repeatS"))
		{
			if(!GetBool(is_repeat_S))
				return false;
		}
		else if (streq(m_token,"repeatT"))
		{
			if(!GetBool(is_repeat_T))
				return false;
		}
	} 
	
#ifdef USE_IMAGE_MAGICK 

	HC_KEY model_tex_library_key = HC_KOpen_Segment(IV_TEXTURE_LIB);
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
	if (result != InputOK)
		return false;
	
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


bool IVPixelTexture::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	float width = 0;
	float height = 0;
	float n_components = 0;
	unsigned char *pixels = 0;
	bool is_repeat_S = false;
	bool is_repeat_T = false;

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{")); 

	for (;;)
	{
		if (!GetString(m_token))
			return false;

		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"image"))
		{
			if (!Get1Float(width))
				return false;
			if (!Get1Float(height))
				return false;
			if (!Get1Float(n_components))
				return false;


			pixels = new unsigned char[(int)(4*width*height)];

			int i = (int)height-1;
			int j = 0;
			while(i >= 0)
			{
				if (!GetString(m_token))
					return false;

				if(!IVNode::Token2RGBA( m_token, (int) n_components, &pixels[i*((int)width)*4 + j*4]))
					return false;

				j++;
				if(j >= width)
				{
					i--;
					j=0;
				}
			}
		}
		else if (streq(m_token,"wrapS"))
		{
			if(!GetBool(is_repeat_S))
				return false;
		}
		else if (streq(m_token,"wrapT"))
		{
			if(!GetBool(is_repeat_T))
				return false;
		}
	}


	if( width > 0 && height > 0 && n_components > 0 )
	{
		HC_Open_Segment(IV_TEXTURE_LIB);
		
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
	}

	if( pixels )
		delete[] pixels;

	return true;
}


bool IVMaterial::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{")); 

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;
		if (streq(m_token,"diffuseColor"))
		{
			float a,b,c;
			if (!Get3Floats(a, b, c))
				return false;

		// I am not sure about opening segment here. Initially when I was working for import of VRMl1.0 I commented the 
		// the code of segment open and close and it worked. We have changed the strategy to import matarials. See
		// the code in IVSeparator for more info(load example small.wrl). But now when I am working on problem in loading
		// IV file I found that uncommenting following two lines solved the problem for me. So now I am keeping the lines uncommented

			HC_Open_Segment_By_Key(m_segkey);
	 			HC_Set_Color_By_Value("geometry","RGB",a,b,c);
			HC_Close_Segment();
		}
		else if (streq(m_token,"specularColor"))
		{
			float a,b,c;
			if (!Get3Floats(a, b, c))
				return false;
			char spec_def[MVO_SMALL_BUFFER_SIZE];
			HCLOCALE(sprintf(spec_def, "faces = (specular = (R = %f G = %f B = %f))", a, a, a));
			HC_Open_Segment_By_Key(m_segkey);
				HC_Set_Color(spec_def);
			HC_Close_Segment();
		}
		else if (streq(m_token,"emissiveColor"))
		{
			float a,b,c;
			if (!Get3Floats(a, b, c))
				return false;
			//char emsi_def[MVO_SMALL_BUFFER_SIZE];
			//HCLOCALE(sprintf(emsi_def, "faces = (emissive = (R = %f G = %f B = %f))", a, a, a));
			//HC_Open_Segment_By_Key(m_segkey);
			//	HC_Set_Color(emsi_def);
			//HC_Close_Segment();
		}
		else if (streq(m_token,"transparency"))
		{
			float a;
			if (!Get1Float(a))
				return false;

			// set transparency only if has non-opaque value
			if( a > 0.0f)
			{
				char transp_def[MVO_SMALL_BUFFER_SIZE];
				HCLOCALE(sprintf(transp_def, "faces = (transmission = (R = %f G = %f B = %f))", a, a, a));
				HC_Open_Segment_By_Key(m_segkey);
					HC_Set_Color(transp_def);
				HC_Close_Segment();
			}
		}
		else if (streq(m_token,"ambientColor"))
		{
			// TODO: 
			//	1. We are assuming that there won't be any square brackets here
			//  2. We are also assuming that this color is not an array, but just a single r,g,b set.
			//  - Pavan Totala (6-Apr-06)
			float a,b,c;
			if (!Get3Floats(a, b, c))
				return false;

			char amb_def[MVO_SMALL_BUFFER_SIZE];
			HCLOCALE(sprintf(amb_def, "ambient = (R = %f G = %f B = %f)", a, b, c));
			HC_Open_Segment_By_Key(m_segkey);
				HC_Set_Color(amb_def);
			HC_Close_Segment();
			
		}
		else if (streq(m_token,"shininess") )
		{
			float a;
			if (!Get1Float(a))
				return false;

			char shin_def[MVO_SMALL_BUFFER_SIZE];
			HCLOCALE(sprintf(shin_def, "faces = (gloss = %f)", a*128)); //see lighting equation for IV2.0 
			HC_Open_Segment_By_Key(m_segkey);
				HC_Set_Color(shin_def);
			HC_Close_Segment();
		}
	}

	return true;			
}

bool IVDummy::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(key);
	UNREFERENCED(parent_node);

	return SkipBracket();			
}
 

bool IVNormal::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));

	for (;;)
	{
		if (!GetString(m_token))
			return false;
		if (streq(m_token,"}"))
			break;

		if (streq(m_token,"vector"))
		{
			if (!BuildPointList(n_plist_length, n_plist_max, &n_plist))
				return false;
		}
	}
	return true;
}

bool IVText::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));
	
	for (;;)
	{
		VERIFY_NEXT_TOKEN( m_token );
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"string"))
		{
			ReadString();
		}
		else if(streq(m_token,"justification"))
		{
			ReadLeft2Right();
		}
		else if(streq(m_token,"spacing"))
		{
			if (!IVNode::Get1Float(m_spacing))
				return false;
		}
		
		
	}
	
	HC_Open_Segment_By_Key(m_segkey);
		if (is_3DText)
		{
			HC_Open_Segment("3DText");
				HC_Set_Text_Font("transforms = on");
		}
		else
			HC_Open_Segment("2DText");

				HC_Insert_Text( m_x, m_y, m_z, m_string );
				HC_Set_Text_Spacing( m_spacing );
				HC_Set_Text_Path( m_path.x, m_path.y, m_path.z);

			HC_Close_Segment();

	HC_Close_Segment();

 	return true;			
}


//======================================================================
//	Author :Rajesh B			Created : 1/24/2001 3:49:07 PM
//======================================================================
//	Function :		IVText::ReadString
//	Description :	Reads the string argument of the Text node
//	Return :bool	-	false if fails
//	Parameters :
//	Note :			1. ASSUMES THAT string KEYWORD IS ALREADY PASSED
//					2. ASSUMES THAT ACTUAL STRING IS ENCLOSED IN SQUARE
//					BRACES AND DOUBLE-QUOTES. EX:  ["sample"]
//======================================================================
bool IVText::ReadString()
{
	// assumption: opening square bracket here, ignore it
	//assert_NEXT_TOKEN( m_token, "[" );
	
		VERIFY_NEXT_TOKEN( m_token );
		if(!IVNode::RemoveQuotes( m_token ))
			return false;             
		strcpy( m_string, m_token);

	//assert_NEXT_TOKEN( m_token, "]" );

	return true;
}



IVFont::IVFont() 
{ 
	// this are the default for a IV 2.0 fontstyle
	m_name = new char[MVO_BUFFER_SIZE];
	strcpy( m_name, "name = SansSerif");

	m_size = new char[MVO_BUFFER_SIZE];
	strcpy( m_size, "size = 1.0 oru");
	

}

IVFont::~IVFont()
{
	H_SAFE_DELETE_ARRAY( m_name );
	H_SAFE_DELETE_ARRAY( m_size );
}


bool IVFont::Convert2Hoops (HC_KEY key, IVNode* parent_node)
{
	UNREFERENCED(parent_node);

	strcpy(m_token,"empty");
	m_segkey = key;

	if (!GetString(m_token))
		return false;
	assert(streq(m_token,"{"));
	
	char font_size[MVO_BUFFER_SIZE];

	for (;;)
	{
		VERIFY_NEXT_TOKEN( m_token );
		if (streq(m_token,"}"))
			break;

		if(streq(m_token,"name"))
		{
			ReadFontName();
		}
		else if(streq(m_token,"size"))
		{
			VERIFY_NEXT_TOKEN( font_size );
		}
		
	}

	HC_Open_Segment_By_Key(m_segkey);
		// just passing on IV 2.0 font name to HOOPS. Hope they understand
		// each other wrt font names
        HC_Set_Text_Font( m_name );

		// IV files have different interpretation of size for 2D and 3D text.
		char font_size_opt_2D[MVO_BUFFER_SIZE];
		char font_size_opt_3D[MVO_BUFFER_SIZE];

		HC_Open_Segment("2DText");
			sprintf(font_size_opt_2D, "size = %s pt", font_size);
			HC_Set_Text_Font(font_size_opt_2D);
		HC_Close_Segment();

		HC_Open_Segment("3DText");
			sprintf(font_size_opt_3D, "size = %s oru", font_size);
			HC_Set_Text_Font(font_size_opt_3D);
		HC_Close_Segment();


		// VRML 2.0 has text which rotates/zooms/pans just like model objects
		// Not so sure about this in IV - commenting it out till it is clear - Pavan T. (20-Apr-06)
        //
	HC_Close_Segment();

 	return true;			
}

bool IVFont::ReadFontName()
{
	if( !IVNode::GetQuotedString(m_token) )
		return false;

	strcpy( m_name, "name = ");

	if(streq(m_token,"DEFAULT"))
		strcat( m_name, "Sans Serif");
	else
		strcat( m_name, m_token);

    return true;
}

//======================================================================
//	Author :Rajesh B			Created : 1/26/2001 11:19:43 AM
//======================================================================
//	Function :		IVFont::ReadLeft2Right
//	Description :	Given the font axis (X or Y), reads the font direction
//					For example, if it is horizontal, then is it in negative
//					or positive direction 
//	Return :bool	-	true if succeeds
//	Parameters :
//	Note :			Read IV2.0 specs for better understanding
//======================================================================
bool IVText::ReadLeft2Right()
{
	VERIFY_NEXT_TOKEN( m_token );
	if(streq(m_token, "RIGHT"))
	{
		m_path.Set( 1.0, 0.0, 0.0 );
	}
	else
	{
		m_path.Set( -1.0, 0.0, 0.0 );
	}

	return true;
}
#endif




