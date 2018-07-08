// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtility.h : interface of the HUtility class
// Set of utility methods
// All methods are public and static


#ifndef _HUTILITY_H
#define _HUTILITY_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#include "HTools.h"
#include "hic_types.h"

class HBaseView;
class HOutputHandlerOptions;
class HSmartSelItem;
class HBaseModel;

class HShellObject;


enum AbsoluteWindowType
{
	RelativeLeft,
	RelativeRight,
	RelativeTop,
	RelativeBottom,
	NotRelative
};

class FaceWithNormalInfo
{
public:
	FaceWithNormalInfo()
	{
		nindex = -1;
		tcindex = -1;
		nextindex = -1;
	}
	int nindex;
	int nextindex;
	int tcindex;
};
 

class MVO_API ShellToRegionMapper
{
public:
	ShellToRegionMapper();
	void AddRegion(HC_KEY originalshell, HC_KEY newshell, int regionnum, bool check = true);
	HC_KEY GetOriginalShell(HC_KEY newshell, int regionnum);
	int GetRegionsFromOriginalShell(HC_KEY originalshell, HC_KEY **newshells, int &regionnum);
	void BuildSegmentTreeRecursive(HC_KEY okey, HC_KEY nkey);
	void MapSegmentTree(HC_KEY modelkey);
	void AddSegmentMapping(HC_KEY originalshell, HC_KEY segmentkey);

	struct vhash_s *		m_RegionToShellHash;
	struct vhash_s *		m_ShellToRegionHash;
	struct vhash_s *		m_ShellToSegmentHash;

};





//! The HShellEdge class is a simple wrapper for a shell's edge.
/*! 
  HShellEdge allows you to easily get information about an edge in a shell.
  */
class MVO_API HShellEdge {
public:
	/*!Constructs an HShellEdge object.
	\param iFirst The index of the first vertex of the edge.  Note the first index is always smaller than the second index.
	\param iSecond The index of the second vertex of the edge.
	*/
	HShellEdge(int iFirst = 0, int iSecond = 0){
		Set(iFirst, iSecond);
	};
	/*! \return The first vertex of the edge.*/
	int GetFirst() const { return m_iFirst; };
	/*! \return The second vertex of the edge.*/
	int GetSecond() const { return m_iSecond; };
	/*! This method define the new vertices for the edge.
	\param first The index of the first vertex of the edge.  Note the first index is always smaller than the second index.
	\param second The index of the second vertex of the edge.
	*/
	void Set(int first, int second) {
		/* first should always be < second to eliminate ambiguity. */
		if(first < second){
			m_iFirst = first;
			m_iSecond = second;
		} else {
			m_iFirst = second;
			m_iSecond = first;
		}
	};

	bool operator<(HShellEdge const & rhs) const {
		if(m_iFirst != rhs.m_iFirst)
			return m_iFirst < rhs.m_iFirst;
		return m_iSecond < rhs.m_iSecond;
	}

private:
	int m_iFirst;
	int m_iSecond;
};

//! The HShellFace class is a simple wrapper for a shell's face.
/*! 
  HShellFace allows you to easily get information about a face in a shell.
  */
class MVO_API HShellFace{
public:
	/*!Construcs an HShellFace object.
	\param pShell A pointer the HShellObject associated with this face.
	\param iFaceIndex The index to the face list that describes this face.
	*/
	HShellFace(HShellObject const * pShell, int const iFaceIndex);

	/*! \return The number of vertices in the face.*/
	int GetPointCount() const ;

	/*! \return The number of edges that the face has.*/
	int GetEdgeCount() const ;
	/*! This method return the information about a given edge in the face.
	\param iEdgeIndex The index that describes a given edge.
	\return The HShellEdge that wrap information about the edge associated with the given index.
	*/
	HShellEdge const GetEdge(int const iEdgeIndex) const;
	/*! This method opens an edge in the HOOPS database given an index so that its attributes can be modified.
	\param iEdgeIndex The index that identifies the edge.
	*/
	void OpenEdge(int const iEdgeIndex) const;
	/*! This method closes the open edge and end the editing process.  Call this method after #OpenEdge when you are finished 
	modifying edge attributes.
	*/
	void CloseEdge() const;

private:
	HShellObject const * m_pShell;
	int m_iFaceIndex;
	int m_iPointCount;
};

//! The HShellRegion class is a simple wrapper for a shell's region.
/*! 
  HShellRegion allows you to easily get information about the regions in a shell.
  */
class MVO_API HShellRegion{
public:
	/*! Constructs an HShellRegion given a pointer to HShellObject and region index.
	\param pShell A pointer the the HShellObject that represents a shell.
	\param iRegionIndex The index number that identifies a specific region.
		*/
	HShellRegion(HShellObject const * pShell, int const iRegionIndex);
	~HShellRegion(){
		delete [] m_pRegionFlist;
	}
	/*! \return The number of faces in the region.
	*/
	int GetFaceCount() const;
	/*! This method finds the face associated with the given index.
	\param index The index associated with the face in the region.
	\return The HShellFace that encapsulates information about the face associated with the passed index.
	*/
	HShellFace const GetFace(int index) const;

	//void Open() const;
	//void Close() const;
private:
	HShellObject const * m_pShell;
	int m_iRegionIndex;

	int m_iRegionFlistLength;
	int *m_pRegionFlist;
};


//! The HShellObject class is a simple wrapper for a shell.
/*! 
  HShellObject allows you to easily get about a shell's properties like faces and regions.
  */
class MVO_API HShellObject {
public:
	/*! Constructs an HShellObject from a given key.
	\param kshellKey Pass the key you want to get shell information from.
	*/
	HShellObject(HC_KEY kShellKey);
	HShellObject(HShellObject const & oShellObj);
	~HShellObject();
	
	/*!This method returns an HShellRegion given an index.
	\param index Pass the index number of the region you want to examine.
	\return An HShellRegion associated with the passed index.
	*/
	HShellRegion const GetRegion(int const index) const;

	/*! \return The number of faces in the shell. */
	int GetFaceCount() const;
	/*! This method return the index into the face list given a face number.
	\param index The face number you wish to get information about.
	\return The index into the face list for the given face number.
	*/
	int GetFaceFlistIndex(int index) const;

	//void Open() const;
	//void Close() const;

	/*! \return The key associated with this HShellObject.*/
	HC_KEY GetKey() const ;
	/*! \return The length of the face list.*/
	int GetFlistLen() const ;
	/*! \return A pointer to the face list.*/
	int const * GetFlist() const ;
	/*! \param An array of ints (returned to the user) representing the face list. Note that the caller 
		       must ensure that the array is of the appropriate size, e.g, via the GetFlistLen function*/
	void GetFlist (int flist[]) const;

private:
	int m_iFlistLen;
	int * m_pFlist;
	mutable int * m_pFaceIndexes;

	HC_KEY m_kShellKey;
};


//! The HShell class is the data type of a HOOPS Shell.
/*! 
  HShell represents all the data structures that are needed for a HOOPS Shell such as the points, the connectivity list and the normals.
*/
class MVO_API HShell {
public:
	HShell()
		:point_list_len(0),
		point_list(),
		normal_list(),
		face_list_len(0),
		face_list(0),
		face_count(0),
		region_count(0),
		region_list(0)
	{}
	~HShell()
	{
		/* No need to check for null pointers.  delete already does that. */
		delete [] point_list;
		delete [] normal_list;
		delete [] face_list;
		delete [] region_list;
	}
	int	point_list_len;   //!< The length of the point list,
	HPoint	*point_list;  //!< The list of vertices that define the shell.
	HVector	*normal_list; //!< The list of normal vectors associated with the shell.
	
	int	face_list_len;    //!< The length of the face list.
	int	*face_list;       //!< The face list that defines how faces are formed in the shell.
	int face_count;		  //!< The number of faces in the shell.

	int region_count;	  //!< The number of regions in the shell.
	int *region_list;	  //!< The list that defines the set of regions in the shell.
};

//! The HShellWrapper class is wraps shell information and provides methods to manipulate this information.
/*! 
  HShellWrapper class contains all the information needed to describe a shell.  It also have methods that allows you to 
  manipulate  and find out shell information such as adding points, iterating through the face list.
*/
class MVO_API HShellWrapper {
	
public:
	/*!
		Constructs an HShellWrapper object.
		\param key The key associated with the shell for this object.
	*/
	HShellWrapper(HC_KEY key) {
		m_Points = 0;
		m_Flist = 0;
		m_VParams = 0;
		m_VNormals = 0;
		m_VColors = 0;
		m_faceregion = 0;
		m_totalnumfaceslen = 0;
		m_numfaceslen = 0;
		m_FNArray = 0;
		m_faceindirect = 0;
		Set(key);
	}
	/*!
		Construcst an HShellWrapper object.
	*/
	HShellWrapper() {
		m_Key = INVALID_KEY;
		m_Points = 0;
		m_Flist = 0;
		m_VParams = 0;
		m_VNormals = 0;
		m_VColors = 0;
		m_flen = 0;
		m_plen = 0;
		m_totalplen = 0;
		m_totalflen = 0;
		m_faceregion = 0;
		m_totalnumfaceslen = 0;
		m_numfaceslen = 0;
		m_FNArray = 0;
		m_faceindirect = 0;
	}
	~HShellWrapper();

	/*!
	This method insert a new shell into the HOOPS database given a points list and face list as described in 
	#HC_Insert_Shell.  
	\param plen The number of points that describe the shell.
	\param points The list of points that describe the shell.
	\param flen The length of the face list.
	\param flist The list that describes the faces in relation to the points as specified in #HC_Insert_Shell.
	\param insertkey Pass a key if you want to insert the shell under a specific segment other wise HOOPS will insert 
	the shell into the currently open segment.
	*/
	HC_KEY Insert(int plen, HPoint points[], int flen, int flist[], HC_KEY insertkey = INVALID_KEY);
	/*!
	This method get the current information associated with the shell from the HOOPS database.  
	\param plen Returns the length of the points list.
	\param points Returns a pointer to the array of points that describe the shell.
	\param flen Returns the length of the face list.
	\param flist Returns a pointer to an array that describes the faces as specified in #HC_Insert_Shell.
	*/
	HC_KEY Show(int *plen, HPoint **points, int *flen, int **flist);
	/*!
		\return The key associated with the shell.
	*/
	HC_KEY GetKey() { return m_Key; }
	/*! This method performs a transformation of the shells given a transformation matrix.
	\param matrix The matrix that describes the transformation.
	*/
	void  TransformPoints(float matrix[]);

	/*! This method performs optimization on the shell based on a set of given options.
	\param options A pointer to the list of options in which you want the shell to be optimized.  The available 
	options are specified in #HC_Compute_Optimized_Shell.
	*/
	HC_KEY Optimize(char *options);
	/*!This method performs optimization on the shell based on a set of given options as well as doing created an 
	optimized color map for the faces.
	\param options A pointer to the list of options in which you want the shell to be optimized.  The available 
	options are specified in #HC_Compute_Optimized_Shell.
	*/
	HC_KEY Optimize2(char *options);

	void MakePolygon(int facelen, int face[], HPoint points[]);


	/*!This method opens the owner segment of the shell.
	*/
	void OpenOwnerSegment();

	/*!Call this method to initialize member variables before making calls to #NextFace.
	*/
	void BeginFaceIteration();
	
	/*! Call this method to end the face iteration process.
	*/
	void EndFaceIteration() { ; };
	int GetFace(int facenum, int **face);
	/*! Call this method after #BeginFaceIteration to iterate through the shell's face list.
	\param face Returns a pointer to the position in the face list that describes this face.
	\return The number of vertices that the face has.
	*/
	int NextFace(int **face);
	
	/*!This method determines the centroid of a given face.
	\param face A pointer the beginning of the list of points that describe the face in the face list.
	\param flen The number of vertices that face has.
	\param mid Returns the coordinates for the center of the face.
	*/
	void CalculateFaceCenter(int face[], int flen, HPoint *mid);
  	/*!This method adds the given points to the points list.
	\param points A pointer to the list of points to add.
	\param pnum The number of entries in points.
	\param param A pointer to the list of parameters to add.
	\param AddImmediatly Pass true to add the points immediately.
	*/
	void AddPoints(HPoint points[], int pnum, HPoint param[] = 0,  HPoint normals[] = 0, bool AddImmediately= false);
	/*!This method adds a new face to the shell given the face and point list.
	\param face A pointer to the face list as describe in #HC_Insert_Shell.
	\param fnum The number of vertices for the face.
	\param points The list of vertices for the face. 
	\param AddImmediately Pass true to add the face immediately.
	*/
	void AddFace(int face[], int fnum, int regionnum, HPoint points[], HPoint normals[] = 0, bool AddImediately= false);
	/*!This method add a new face to the shell given a face list.  Note that no new points are added to the point list.
	\param face A pointer to the face list as described in #HC_Insert_Shell.
	\param fnum The number of vertices that define the face.
	\param reverse Pass true if you want to add the vertices in the face list in reverse order.
	\param AddImmediately Pass true to add the face immediately.
	*/
 	void AddFaceDirect(int face[], int fnum, bool reverse = false, bool AddImmediatly= false);
	/*!This method adds a new face to the shell given the point list.  A face list is derived from the point list 
	by ordering the points as they appear in the point list.
	\param points The list of vertices that define the face.
	\param fnum The number of vertices.
	\param AddImmediately Pass true to add the points immediately.
	*/
	void AddFace(HPoint points[], int fnum, bool AddImmediatly= false);
	/*!This method sets the key for the class.
	\param key Pass the key that defines the shell you want to manipulate.  If you pass INVALID_KEY, this method will 
	insert a new shell and record the key that was generated.
	*/
	void Set(HC_KEY key);
 	/*!This method regenerates the shell in the HOOPS database.
	\param newowner Pass the key in which you want the shell to be recreated in.  If you want it the be created the current location pass 
	INVALID_KEY.
	\param makeSegment Pass true to create a new segment under the new owner key.  Note this parameter only work if a newowner key is not 
	INVALID_KEY.
	*/
	void Regenerate(HC_KEY newowner = INVALID_KEY, bool makeSegment = true, bool regenerate = false);
	/*!This is a helper function used by the AddFace and AddPoints methods.  Given an additional number of points, the 
	functions determines if the face and points lists sizes need to be increased and by the optimal amount.
	\param fnum The number of points that will be added.
	*/
 	void GrowArrays(int fnum);
 	/*!This method is a helper functions used by the AddFace and AddPoints methods.  Given an additional number of points, the 
	functions determines if the points list size need to be increased and by the optimal amount.
	\param fnum The number of points that will be added.
	\param growvparams Pass true if you will be adding parameter pointers in addition to vertices.
	*/
	void GrowPointArray(int fnum, bool gorwvparams = false, bool grownormals = false);

	void GrowPointArray2(int fnum, bool growparamarray = false, bool grownormalarray = false, bool growfnarray = false, bool growcolorarray = false);
	void AddNextFaceWithDistinctNormals(HPoint const * points, HPoint const * normals, int const * face1, int const * face2, int fnum);
	void AddNextFaceWithDistinctNormalsAndTexture(HPoint const * points, HPoint const * normals, HPoint const * texcor,int const * face1, int const *face2, int const *face3, int fnum);
	void AddNextFaceWithDistinctNormalsAndColor(HPoint const *points, HPoint const *normals,int const *face1, int const *face2, HPoint *face3, int fnum);
 

	void BeginAddFaces(int pnum);
	void EndAddFaces();

	/*!This is a helper function used by the AddFace and AddPoints methods.  Given an additional number of points, the 
	functions determines if the face list size need to be increased and by the optimal amount.
	\param fnum The number of points that will be added.
	*/
 	void GrowFaceArray(int fnum);

	/*!This is a helper function to SubdivideQuadFaces and is for <b>internal use only</b>.*/
	void SubdivideOneQuadFace(int face[], HShellWrapper &wrapper, bool generateIsolines, int *facecolorlist, int &fclnum);

	/*!This is a helper function to SubdivideQuadFaces and is for <b>internal use only</b>.*/
	void SubdivideOneQuadFace2(int face[]);
	/*!This method retesselates a shell whose faces are all quads so that isolines can be drawn.  Note that this will not work for shells 
	whose faces are triangles or other types of polygons.
	\param GenerateIsolines Pass true to generate isolines.
	*/
	void SubdivideQuadFaces(bool GenerateIsolines);

	/*!This method sets the shell's vertices with the given color.
	\param color Pass an HPoint populated with the r, g, and b values in the x, y, z position respectively.
	*/
	void SetVertexColors(HPoint *color);
	/*!This method show the shell's vertices with the given color. 
	\param color Pass an HPoint populated with the r, g, and b values in the x, y, z position respectively.
	*/
	void ShowVertexColors(HPoint *color);
	/*!This method sets the shell's v
	ertices with the given color.
	\param findex Pass the index value in the colormap that indicates the color you want for this shell's vertices.
	*/
	void SetVertexColors(float *findex);
	/*!This method show the shell's vertices with the given color. 
	\param color Pass the index value in the colormap that indicates the color you want for this shell's vertices.
	*/
	void ShowVertexColors(float *color);
	/*!This method inserts a new level of detail given a number and percentage of simplification.
	\param lodnum The level of the LOD.
	\param percent The percent that the number of total triangle will be reduced to in the shell as described by 
	#HC_Compute_Optimized_Shell.
	*/
	void MakeLod(int lodnum, int percent);
	/*!This method finds the center, minimum and maximum points for the given shell.
	\param center Pass by reference.  This method returns the center point for the shell.
	\param min Pass by reference.  This method returns the minimum point for the shell.
	\param max Pass by reference.  This method returns the maxinum point for the shell.
	*/
	void FindExtents(HPoint &center, HPoint &min, HPoint &max);
	void ReplaceWithParameters(int plen, HPoint points[], int flen, int flist[], HPoint normals[] = 0, float params[] = 0, int paranum = 3, HPoint vcolors[] = 0, bool emptyshell = false);
	void ReplaceWithParameters(int plen, HPoint points[], int flen, int flist[], HPoint normals[], float params[], int paranum, bool emptyshell);
	void Replace(int plen, HPoint points[], int flen, int flist[], HPoint normals[] = 0, bool emptyshell = false);

	void SetFaceRegion(int regionnum);

	void SetEmptyShell(HC_KEY segmentkey, bool MakeSegment);


	int m_plen;//!<The number of points in the points list.
 	int m_flen;//!<The length of the face list.
 	HPoint *m_Points;//<! The list of pointst that define the shell.
 	HPoint *m_VParams;
 	HPoint *m_VColors;
  	HPoint *m_VNormals;
 	int *m_Flist;//!< The list that defines the configuration of the faces in the shell in relation to the points as described in #HC_Insert_Shell.
	HC_KEY m_Key;//!< The key of the shell
 
	/*! <b>For Internal Use Only</b>*/
	int m_facepointer;
	/*! <b>For Internal Use Only</b>*/
	int m_totalflen;
	/*! <b>For Internal Use Only</b>*/
	int m_totalplen;
	int m_totalnumfaceslen;
	int m_numfaceslen;
	int *m_faceregion;
	int *m_faceindirect;
	FaceWithNormalInfo *m_FNArray;

};


//! The HCamera class is the data type of a HOOPS Camera Attribute.
/*! 
  HCamera represents all the data needed to define a HOOPS Camera. 
*/
class MVO_API HCamera {
public:
	bool valid;			 //!< True if the object has been initialized.
	HPoint position;     //!< The location of the camera.
	HPoint target;       //!< The area that the camera is directed at.
	HPoint up_vector;    //!< The direction of up.  
	float field_width;   //!< The width of the field of view.
	float field_height;  //!< The height of the field of view.
	char projection[128]; //!< The projection mode which can be "orthographic", "perspective" or "stretched".

	HCamera () {
		projection[0] = '\0';
		valid = false;
	}

	/*! This method invalidates the current camera settings.*/
	inline void InvalidateCamera() { projection[0]='\0'; valid = false; }

	/*! This method compares the setting of a given camera with this camera's settings.
		\param cam The camera to compare with.
		\return True if this camera is different in anyway from the given camera or false if they are the same. */
	bool CameraDifferent(HCamera &cam);

	/*! This method compares the setting of a given camera with this camera's settings based on the camera orientation only.
		\param cam The camera to compare with.
		\return True if this camera is different in anyway from the given camera or false if they are the same. */
	bool CameraDifferentByAngle(HCamera &cam);


	/*! This method initializes the camera with the same camera settings under the view's scene key.
		\param view A pointer to the HBaseView object from which to extract camera information. */
	void GetFromView(HBaseView * view);

	/*!The is method extracts the camera from the currently open segment and populates into this camera's settings. */
	void Show() {
		HC_Show_Camera (&position, &target, &up_vector, &field_width, &field_height, projection);
		valid = true;
	};

	/*! This method sets the camera on the currently open segment based on this object's member variables. */
	void Set() const {
		HC_Set_Camera (&position, &target, &up_vector, field_width, field_height, projection);
	};
};


//! The HBSurfVertex class is the data type of a B-Surface vertex.
/*! 
  HBSurfVertex represents a B-Surface vertex and requires a knot value w in addition to x, y and z coordinates.
*/
class MVO_API HBSurfVertex
{
public:
	double		x;	//!< The x-coordinate of a B-Surface vertex.
	double		y;	//!< The y-coordinate of a B-Surface vertex.
	double		z;	//!< The z-coordinate of a B-Surface vertex.
	double		w;	//!< The knot value of a B-Surface vertex.
	/*! 
		Set packs an HBSurfVertex object with coordinate and knot values.
	*/
	inline void Set(double X, double Y, double Z, double W) { x=X, y=Y, z=Z, w=W; }
};

//! The HPolyline class is the data type for a three-dimensional polyline.
/*! 
  The HPolyline class encapsulates the x, y, and z coordinates of a 3-dimensional polyline. 
  The 'points' member can be passed to native HOOPS routines such as HC_Insert_Polyline.
*/
class MVO_API HPolyline 
{
public:
    int				m_PointCount;		//!< Number of points in the polyline.
    HPoint			*m_pHPointArray;	//!< A pointer to an array of HPoint objects.
    HPolyline		*m_pNextPolyline;	//!< A pointer to a single HPolyline object.
};

//! The HShellVertexData class encapsulates colormap index information associated with the vertices of a shell.  
/*! 
  HShellVertexData simplifies the management of shell data.  

*/
class MVO_API HShellVertexData 
{
public:
	/* Constructor initializes this object to an invalid state. */
	HShellVertexData():m_VertexCount(0), m_pFIndexArraySize(0), m_pFIndexArray(0), m_Key(INVALID_KEY){
		m_sType[0] = '\0';
	}

	/* Destructor free up the resources this object is using. */
	~HShellVertexData(){
		ResetMyself();
	}

	/* Sets the key and fills in the members with the correct data for this shell. */
	void SetKey(HC_KEY const key);

	/* Return a const string for the type of shell. */
	char const * const GetType() const {
		return m_sType;
	}

	/* Returns the vertex count for this shell. */
	int const GetVertexCount() const {
		return m_VertexCount;
	}

	/* Return the key for the shell */
	HC_KEY const GetKey() const {
		return m_Key;
	}

	/* Return the size the findex array */
	int const GetFIndexDataSize() const {
		return m_pFIndexArraySize;
	}

	/* Returns a const array for the findex values in this shell. */
	float const * const GetFIndexData() const {
		return m_pFIndexArray;
	}

	/* Returns a const array (as an argument) for the findex values in this shell. 
	   Note that the array must be of the appropriate size, e.g., via a call to GetFIndexDataSize. */
	void GetFIndexData(float data[]) const;

	void SetFIndexData(int const size, float const data[]);

private:
	void ResetMyself();

	int         	m_VertexCount;		//!< Number of vertices in the shell.
	int				m_pFIndexArraySize; //!< Number of elements in f-indices array
    float       	*m_pFIndexArray;	//!< Pointer to an array of f-indices associated with the vertices.
    HC_KEY			m_Key;			//!< Key to a given shell.
    char			m_sType[32];	    //!< type of object (mesh or shell)
};


//! The HShowContentsWithPathType struct is used by HUtility to store a keypath
/*! 
*/
typedef struct
{
    HC_KEY *patharray;		//!< Array of Keys
    int len;			//!< length of key array
}HShowContentsWithPathType;


enum HWindowCorner 
{
	TopLeft,        
	BottomLeft,        
	BottomRight,
	TopRight
};







//! The HUserOptions class is a helper class which wraps up memory management for variable size user options returned from HOOPS.
/*! 
  HUserOptions uses *Show_User_Option_Length* (or variation) to get the length of user option and 
  manages memory allocation and free for Show_User_Option. Following shows an example usage
  
  // without HUserOption class: problem is assumption that option is max 1024
  char * options[MVO_BUFFER_SIZE];
  HC_Show_User_Option(options);
  if( strstr(options, "true")
	return true;

  // with HUserOption class: automatically allocates enough memory required (by using Show_XXX_Length routene) and free the memory
  // in the destructor
  HUserOptions options;
  options.Show_User_Option();
  if( strstr(options.m_data, "true")
	return true;
*/

class MVO_API HUserOptions 
{
public:

	char * m_data;	//!< actual char* to the user options
	int m_length;	//!< length of current user option
	bool m_valid;

	/*! constructor */
    HUserOptions();

	/*! copy constructor */
    HUserOptions(const HUserOptions& in_data);


	/*! destructor */
    ~HUserOptions();


	/*!
	  IsValid returns true if the user option held in this object is valid i.e. if m_length > 0
	 */
	bool IsValid();

	/*!
	  Show_User_Options Wraps HC_Show_User_Options to use Show_XXX_Length routene and perform memory management.
	  Please refer to the documentation of HC_Show_User_Options from HOOPS/3dGS reference manual for details
	  \return A char * to the user options string. You can also access the same via HUserOptions.m_data
	 */
    const char * Show_User_Options();

	/*!
	  Show_One_Net_User_Option Wraps HC_Show_One_Net_User_Option to use Show_XXX_Length routene and perform memory management.
	  Please refer to the documentation of HC_Show_One_Net_User_Option from HOOPS/3dGS reference manual for details
	  \return A char * to the user options string. You can also access the same via HUserOptions.m_data
	 */
	const char * Show_One_Net_User_Option(const char * which);

	/*!
	  Show_One_User_Option Wraps HC_Show_One_User_Option to use Show_XXX_Length routene and perform memory management.
	  Please refer to the documentation of HC_Show_One_User_Option from HOOPS/3dGS reference manual for details
	  \return A char * to the user options string. You can also access the same via HUserOptions.m_data
	 */
	const char * Show_One_User_Option(const char * which);

	/*!
	  Show_Net_User_Options Wraps HC_Show_Net_User_Options to use Show_XXX_Length routene and perform memory management.
	  Please refer to the documentation of HC_Show_Net_User_Options from HOOPS/3dGS reference manual for details
	  \return A char * to the user options string. You can also access the same via HUserOptions.m_data
	 */
	const char * Show_Net_User_Options();


private:

	void clear() {
		delete [] m_data;
		m_data = 0;
		m_length = 0;
	};	//!< frees the memory. used internally.

};



//! The HUtility class contains a set of utility functions and data structures used widely in MVO classes.
/*! 
  HUtility includes utility functions for double-precision vector math, creation of construction geometry, etc...
*/
class MVO_API HUtility
{
public:
#undef PI
	static const float PI;			//!<	default=3.1415926f

	enum{X, Y, Z, W};	//!<	Enumerated datatypes defining the different cartesian axes.
	enum{XY, XZ, YZ};	//!<	Enumerated datatypes defining the different cartesian planes.

	static const float EH_Epsilon;	//!<	default=0.01


	/*! 
	  This method orders the input points in each dimension.  The resulting points are minimum and maximum for bounding 
	  box input.
	  \param one A pointer to the first point to order.
	  \param two A pointer to the second point to order.
	*/
	static void		Order(HPoint* one, HPoint* two);

	/*!
		This method returns a relative path for the <i>from_seg_key</i> to the <i>to_seg_key</i>.
		\param from_seg_key The key to the start segment.
		\param to_seg_key The key to the end segment.
		\param path Pass an array that will be filled with the relative path.
	*/
	static void		Find_Relative_Path(HC_KEY from_seg_key, HC_KEY to_seg_key, char * path);

	/*! 
	  \return The system time in seconds.
	*/
	static float	GetTime();
	

	/*! 
	  ClampPointToWindow clamps a point to its nearest corner in window space.
	  \param point A pointer to an HPoint object.
	*/
	static void		ClampPointToWindow (HPoint * point);

	/*! 
	  InsertBox inserts a six-sided, rectangular cuboid based on 2 points.
	  \param max A pointer to an HPoint describing a point on the box.
	  \param min A pointer to an HPoint describing a point on the opposite face diagonal from the first point.
	  \return The key to the newly created box.
	*/
	static HC_KEY		InsertBox (HPoint const * max, HPoint const * min);

	/*! 
	  InsertWireframeBox uses a polyline to insert a six-sided, rectangular cuboid wireframe based on 2 points. 

	  \param max A pointer to an HPoint describing a point on the box.
	  \param min A pointer to an HPoint describing a point on the opposite face diagonal from the first point.
	  \return The key to the newly created box.
	*/
	static HC_KEY		InsertWireframeBox (HPoint * max, HPoint * min);

	/*! 
	  GenerateCuboidPoints creates the vertices of a cuboid based on two input points.
	  \param max A pointer to the first point that describes the cuboid.
	  \param min A pointer to the point the is diagonally opposite the first point.
	  \param points A pointer to an array that can hold the 8 HPoints that describe the cuboid.  Passed by reference.
	*/
	static void		GenerateCuboidPoints (HPoint const * max, HPoint const * min, HPoint * points);

	/*! 
	  InsertRectangle inserts a 2d rectangle as a HOOPS Polyline.
	  \param seg The segment name into which the rectangle should be inserted.
	  \param x0 X-value of vertex 1 of the rectangle.
	  \param y0 Y-value of vertex 1 of the rectangle.
	  \param x1 X-value of vertex 3 of the rectangle.
	  \param y1 Y-value of vertex 3 of the rectangle.
	  \param z The z coordinate shared by vertext 1 and 3.
	  \param fill Pass true to create a filled rectangle.
	  \return The key to the new created rectangle. 
	  */
	static HC_KEY		InsertRectangle (const char * seg, float x0, float y0, float x1, float y1, float z = 0.0f, bool fill = false);


	/*! 
	  InsertArrow inserts an arrow consisting of a cylinder and a cone at the origin. This function is used by the Axis 
	  display.  The arrow is completely customizable.
	  \param stick_length The length of the cylindrical shaft of the arrow.
	  \param tip_length The length of the conical tip of the arrow.
	  \param stick_radius The radius of the cylindrical shaft of the arrow.
	  \param arrow_radius The radius of the circular base of the arrow's conical tip.
	  \param text A pointer to a string that annotates the arrow.
	  \param stick_color The color of the shaft of the arrow.
	  \param tip_color The color of the tip of the arrow.
	  \param num_sides The number of sides of arrow.  The default is 6.
	  \param tip_falloff The radius reduction value on stick.  The default is 0.05.
	  \param reverse Pass true to reverse the orientation.
	*/
	static void		InsertArrow(float stick_length, float tip_length, float stick_radius, float arrow_radius, const char *text, const char *stick_color, const char *tip_color, int num_sides = 6, float tip_falloff = 0.05, bool reverse = true);

	/*! 
	  ComputeDoublePrecisionCrossProduct computes the cross product of two double precision vectors.
	  \param vector1 A pointer to a double precision vector.
	  \param vector2 A pointer to a double precision vector.
	  \param up Pass a pointer to a double.  This method will populate <i>up</i> with the result of the cross product.
	*/
	static void		ComputeDoublePrecisionCrossProduct(double * vector1, double * vector2, double * up);

	/*! 
	  ComputeDoublePrecisionDotProduct computes a double precision dot product of two vectors.
	  \param vector1 A pointer to a double precision vector.
	  \param vector2 A pointer to a double precision vector.
	  \return The double precision dot product of the two vectors.
	*/
	static double	ComputeDoublePrecisionDotProduct(double * vector1, double * vector2);

	/*! 
	  ComputeDoublePrecisionVectorLength computes the length of a double precision vector.
	  \param vector1 A pointer to a double precision vector.
	  \return The double precision length of the vector.
	*/
	static double	ComputeDoublePrecisionVectorLength(double * vector1);

	/*! 
	  GetViewplaneVectors computes the construction-plane-constrained vectors used for geometry insertion.
	  \param segment_key The key to the construction segment.
	  \param view A pointer to the double precision view vector.  Passed by reference.
	  \param up A pointer to the double precision up vector. Passed by reference.
	*/
	static void		GetViewplaneVectors(HC_KEY segment_key, double * view, double * up);

	/*! 
	  NormalizeDoublePrecisionVector normalizes a double-precision vector in place so that its magnitude is 1. 
	  \param vector1 A pointer to a double precision vector.
	*/
	static void		NormalizeDoublePrecisionVector(double* vector1);

	/*! 
	  PointsEqual compares the equality of two (x,y,z) points.
	  \param point1 A pointer to the first point to compare.
	  \param point2 A pointer to the second point to compare.
	  \return True if the points are equal or false if they aren't.
	*/

	static bool PointsEqual(HPoint * point1, HPoint * point2);

	/*! 
	  CreateAxisCircle creates a circle based on a centerpoint and a radius. The centerpoint is assumed to be in viewpoint space.
	  Note this function actually inserts a circle into the HOOPS scene using the HOOPS routine HC_Insert_Circle, 
	  whereas #HUtilityGeometryCreation::GeneratePointsOnCircle only generates an array of 
	  points along a circular trajectory.
	  \param center The center of the desired circle.
	  \param radius The length of the radius of the circle.
	  \param axis XY, XZ, or YZ. This defines the view plane in which the circle will be drawn.
	  \param convert Pass true to convert to viewpoint space.
	  \return True if the operation was a success or false if it failed.
	*/
	static bool		CreateAxisCircle (HPoint const & center, float radius, int axis, bool convert = true);
	

	/*! 
	   This method rotates the contents of the currently opened segment to a given vector in space. 
	  \param start The first point in space that defines vector.
	  \param end The second point in space that defines vector.
	  \param doTranslation Pass to perform translation in the segment via #HC_Translate_Object.
	*/
	static void		RotateToVector(HPoint start, HPoint end, bool doTranslation = true);

	/*! 
	   SmoothTransition takes two camera definitions and uses quaternions to smoothly move the 
	   camera on the HBaseView object from the first camera position to the next.
	  \param co The initial camera position.
	  \param to The initial camera target.
	  \param uo The initial camera up vector.
  	  \param fieldwidtho The initial camera field width.
  	  \param fieldheighto The initial camera field heigth.
	  \param cn The final camera position.
	  \param tn The final camera target.
	  \param un The final camera up vector.
  	  \param fieldwidthn The final camera field width.
  	  \param fieldheightn The final camera field heigth.
	  \param view A pointer to the HBaseView object you want the camera movements to be applied.
	*/
	static void		SmoothTransition(const HPoint &co, const HPoint &to, const HPoint &uo, float fieldwidtho, float fieldheighto, const HPoint &cn, const HPoint &tn, const HPoint &un, float fieldwidthn, float fieldheightn, HBaseView *view);

	/*! 
	   SmoothTransition takes two camera definitions and uses quaternions to smoothly move the camera on the 
	   HBaseView object from the first camera position to the next. 
	   \param old_camera The initial camera definition.
	  \param new_camera The final camera definition.
	  \param view A pointer to the HBaseView object you want the camera movements to be applied.
	*/
	static void		SmoothTransition(HCamera old_camera, const HCamera &new_camera, HBaseView *view);

	/*! 
	  The CamerasEqual method tests two camera definitions for equality.
	  \param camera1 The first camera definition.
	  \param camera2 The second camera definition.
	  \return True if the cameras have the same definition or false if they don't.
	*/
	static bool		CamerasEqual(const HCamera &camera1, const HCamera &camera2);

	/*! 
	  This method computes a rotation matrix that rotates between the two supplied vectors.
 	  \param matrix	Pass an array that can hold a 4 x 4 matrix.  This method will populate the matrix with the values
	  for the rotation matrix.
	  \param v1 The vector to rotate from.
	  \param v2 The vector to rotate to.
	*/
	static void		ComputeVectorToVectorRotationMatrix(float matrix[],HPoint v1,HPoint v2);

	/*!
	  This method parses the input string with the format "R=0.23, G=0.34, B=0.12" for the red, green and blue color values.
	  \param text A pointer to null terminated char array with color information.
	  \param r Returns the red color component.
	  \param g Returns the green color component.
	  \param b Returns the blue color component.
	  \return True if the operation was successful or false if it failed.
	*/
	static bool		ExtractRGB(char *text,float &r, float &g, float &b);

	/*!
	  TransitionQuaternion takes a start and end quaternion which represent two orientations in cartesian space.  
	  Based on a value t between 0 and 1, it comes up with a third quaternion vector that represents an orientation 
	  in cartesian space which is along the shortest, smooth path between the two initial orientations.
	  \param quat_old An array of 4 by 1 floats representing the starting orientation in quaterion space.
	  \param quat_new An array of 4 by 1 floats representing the final orientation in quaterion space.
	  \param t A floating point value between 0 and 1 indicating the amount you want the returned quaternion to be along 
	  the interpolated path.
	  \param qt A pointer which contains the 4 by 1 vector of the floating point values representing the transition quaternion.  This value is returned to the user.
	*/
	static void		TransitionQuaternion(float quat_old[], float quat_new[], float t, float qt[]);

	/*!
	  MatrixToQuaternion converts a 4 by 4 matrix to it's equivalent 4 by 1 vector in quaternion space.
	  \param matrix A pointer to a 4 by 4 matrix of floating point values which typically represent a camera projection.
	  \param quaternion A pointer which contains the 4 by 1 vector of floating point values representing the matrix in quaternion space.  This value is returned to the user.
	*/
	static void		MatrixToQuaternion(float matrix[], float quaternion[]);

	/*!
	  QuaternionToMatrix converts an 4 by 1 array of floating point values to its equivalent 4 by 4 matrix in cartesian space.
	  \param quaternion A pointer which contains the 4 by 1 vector of floating point values representing the matrix in quaternion space.  
  	  \param matrix A pointer to a 4 by 4 matrix of floating point values which typically represent a camera projection.  This value is returned to the user.
	*/
	static void		QuaternionToMatrix(float quaternion[], float matrix[]);

	/*! 
	   This method animates the rotation of an object from the object space of one segment to the object space of another.  
	   It uses quaternions to make the transition appear smooth.
	  \param m_pView A pointer to the HBaseView object.
	  \param old_seg The segment from which the transition should occur.
	  \param new_seg The segment which has the modelling matrix which we want to transition to.
	  \param num_of_transitions The numer of frames in the animation.
	*/
	static void		SmoothQuaternionTransition(HBaseView *m_pView, char *old_seg, char *new_seg, int num_of_transitions);
	
	/*! 
	   This method transitions an object to the target modelling matrix.  It uses quaternions to 
	   make the transition appear smooth.
	  \param m_pView A pointer to the HBaseView object.
	  \param seg_key The key of the segment to which the matrix should be applied.
	  \param new_matrix The matrix to apply to the segment.
	*/
	static void	SmoothMatrixTransition(HBaseView *m_pView, HC_KEY seg_key, float new_matrix[]);

	/*! 
	   This method transitions multiple segments to multiple target modelling matrices.  It uses quaternions to 
	   make the transition appear smooth.
	  \param m_pView A pointer to the HBaseView object.
	  \param num_keys The number of segments in seg_key.
	  \param seg_key A pointer to the array of segments to which the matrices should be applied.
	  \param new_matrix A pointer to an array of matrices to apply to the given segments.
	  \param new_camera If non-null, then the method will also smoothly transition the camera to this new camera.
	*/
	static void	MultiSmoothTransitions(HBaseView *m_pView, int num_keys, HC_KEY *seg_key, float new_matrix[], HCamera *new_camera = 0);

	/*! 
	   This method returns the centroid of a segment in world space.
	  \param seg_key The segment to get centroid from.
	  \return The coordinate of the centroid.
	*/
	static HPoint GetCentroid(HC_KEY seg_key);
	
	/*! 
	   This method returns the centroid of a segment, transformed by a given matrix.
	  \param seg_key The segment to get centroid from.
	  \param matrix The transformation to apply to the centroid.
	  \return The coordinate of the centroid.
	*/
	static HPoint GetTransformedCentroid(HC_KEY seg_key, float matrix[] = 0);

	/*!
	  	This function takes an char buffer and create a URI encoded string.
	 
		\param in_string A unencoded string.
		\param n The length of the in buffer.
		\param out_buffer A write back buffer of size 3*n+1.
	*/
	static void URIencode(const char * in_string, MVO_POINTER_SIZED_UINT n, char * out_buffer); 
	/*!
	  	This function decodes a URI encoded string.
	 
		\param in_string A null terminated encoded string.
		\param out_buffer A write back buffer of size strlen(in)+1 after call this contains unencoded string.
		\param n A write back value with the length of the out_buffer.
	*/
	static void URIdecode(const char * in_string, char * out_buffer, MVO_POINTER_SIZED_UINT * n);

	/*!
	  This method extracts the camera attributes from the input transformation matrix.
	  \param matrix A pointer to the transformation matrix.
	  \param target A pointer to HPoint object representing the camera target.
	  \param position A pointer to HPoint object representing the camera position which this method will populate.
	  \param up_vector A pointer to HPoint object representing the view-up vector which this method will populate.
	  \param translation A pointer to HPoint object representing the translation which this method will populate.
	*/
	static void		GetCameraFromMatrix(float matrix[], HPoint *target, HPoint *position, HPoint *up_vector, HPoint *translation);
	
	/*!
	  This method composes a transformation martix from the given camera attributes.
	  \param target A pointer to HPoint object representing camera target.
	  \param position A pointer to HPoint object representing camera position.
	  \param up_vector A pointer to HPoint object representing view-up vector.
	  \param translation A pointer to HPoint object representing translation.
	  \param *matrix Resultant transformation matrix.  Pass a pointer to an array that can accomodate a 4 x 4 matrix.
	*/
	static void		GetMatrixFromCamera(HPoint target, HPoint position, HPoint up_vector, HPoint translation, float matrix[]);


	/*!
	  This method returns a set of HOOPS camera values and a HOOPS camera near-plane value based on a glFrustum or 
	  glOrtho definition.  The resulting HOOPS camera values should be set within #HC_Set_Camera, AND the resulting 
	  near-plane should be set by #HC_Set_Camera_Near_Limit.
	
		\param left			The same parameter used by the glFrustum or glOrtho calls.
		\param right		The same parameter used by the glFrustum or glOrtho calls.
		\param bottom		The same parameter used by the glFrustum or glOrtho calls.
		\param top			The same parameter used by the glFrustum or glOrtho calls.
		\param near_z		The same parameter used by the glFrustum or glOrtho calls.
		\param far_z		The same parameter used by the glFrustum or glOrtho calls.

		\param perspective	If set to true, then a perspective frustrum (set via glFrustum) is assumed.  If false, then an 
							orthographic frustum (set via glOrtho) is assumed.
	    \param pos			The HOOPS camera position which is passed by reference and returned to the user.
		\param target		The HOOPS camera target which is passed by reference and returned to the user.
		\param up			The HOOPS camera up vector which is passed by reference and returned to the user.
		\param projection	The HOOPS camera projection which is passed by reference and returned to user.  The caller 
							should pass in a char array of 64 characters.  This is returned because the HOOPS camera 
							projection might not be a simple choice between 'perspective' or 'orthographic' and thus 
							won't match what was passed in via the 'perspective' boolean.  For example, the result HOOPS 
							projection could be 'oblique perspective' based on the glFrustum values.
		\param width		The HOOPS camera field width which passed by reference and returned to the user.
		\param height		The HOOPS camera field height which passed by reference and returned to the user.
		\param near_limit   The HOOPS camera near limit which passed by reference and returned to the user.  This value 
							represents the location of the HOOPS camera's near clip plane.


	  Assumptions regarding the glFrustum values:<br>
		right > left<br>
		top > bottom<br>
		far > near, and for perspective both should be positive.
	*/
	static void		GetCameraFromGLFrustum(double const left, double const right, double const bottom , double const top, double const near_z, double const far_z, bool const perspective,
							   HPoint &pos, HPoint &target, HPoint &up, char * projection, double &width, double &height, double &near_limit );
	
	/*!
	   This method stores a local attribute as a user option in the currently open segment for later retrieval.
	   This is useful for processes like highlighting and selection.
	  \param attName name of the attribute that needs to be stored.  The currently supported attribute names are color, 
	   visibility, rendering options and edge weight.
	  \return True if the local attribute exist or false if it doesn't.
	*/
 	static bool		PushAttribute(const char *attName);

	/*!
	   This method stores a local attribute as a user option in the currently open segment for later retrieval.
	   This function takes the attribute as a string instead of getting it from the segment directly
	   This is useful for processes like highlighting and selection.
	   \param attName name of the attribute that needs to be stored.  The currently supported attribute names are color, 
	   visibility, rendering options and edge weight.
	   \param attText The attribute definition (e.g. "faces = red, edges = blue").
	*/
	static void		PushAttribute(const char *attName, const char *attText);
	
	/*!
	   This method replaces the requested attribute in the currently open segment with the stored attribute found in user options.
	   The stored user option associated with that attribute will be deleted.	
	   \param attName The name of the attribute that needs to be stored.  The currently supported attribute names are color, 
	   visibility, rendering options and edge weight.
	   \return True if the stored attribute exists or false if it doesn't.
	*/
	static bool		PopAttribute(const char *attName);

 	/*!
		This method modifies the stored attribute in the currently open segment.<br>
		Example:<br> 
		The stored attribute is "faces = red".<br>
		The newStyle is "lines = yellow".<br>
		The result is "faces = red, lines = yellow".
		\param attName The name of the stored attribute that needs to be modified. 
		\param newStyle The modification string. 
		\return True if the stored attribute exists.
	*/
	static bool		ModifyPushedAttribute(const char *attName, const char *newStyle);

 	/*!
		This method retrieves the stored attribute in the currently open segment if present.
		\param attName The name of the stored attribute that needs to be retrieved.
		\param attText This method returns the attribute string. 
		\return True if the stored attribute exists or false if it doesn't exist.
	*/
	static bool		ShowPushedAttribute(const char *attName, HUserOptions& attText);

	/*!
	 This method steps through the child segments, and merges all of the shells into one.  
	 Note that this method assumes that the proper amount of memory has been allocated.  You can 
	 use #Segment_Shell_Allocate to allocate the correct amount of memory.
	 
	 \param total_info A pointer to where the newly merged shell. 
	 \param max_info A pointer to the largest shell data.
	 \param do_recurse Pass true to recurse through the segment tree beneath or false to only merge shells in 
	 the currently open segment.
	 \param delete_orig_shells Pass true to delete original shells that have been merged or false to keep them.
	 \param insert_null_lods  Pass true to insert placeholder LODs.
	*/
	static void	Recursive_Merge_Shells( HShell *total_info, HShell *max_info, bool do_recurse, bool delete_orig_shells = false, bool insert_null_lods = false );

	/*!
	 This method extracts all of the shells from the currently open segment and its children, and merges them into one.
	 \param do_recurse A boolean value indicating if the children segments should also be searched for shells.
	 \param delete_orig_shells Pass true if you want the original shells to be deleted after the merge.
	 \return The key to the newly merged shell.
	 */
	static HC_KEY	Merge_Shells( bool do_recurse, bool delete_orig_shells = false , bool optimize_shell = true, bool generate_regions = false);

	/*! 
	 This method merges the given shells into a single shell. The original shells are marked as regions in the merged shell.
	 \param num_shells The number of elements in the shell_keys array. 
	 \param shell_keys The HOOPS keys of the shells to be merged.
	 \param delete_orig_shells Pass true if you want the original shells to be deleted after the merge.
	 \return The key to the newly merged shell.
	*/
	static HC_KEY	Merge_Shells_Generate_Regions( int num_shells, HC_KEY * shell_keys, bool delete_orig_shells = false );

	/*!
	 This function merges all the shells in the segement and generates LODs for the merged shell. These
	 LODs are then attached to one of the shells in the segment.
	 \param do_recurse Pass true to recursively searched for shells.
	 \param delete_orig_shells Pass true to erase the original shells once they have been merged into a new shell.
	 \param insert_null_lods Pass true to attach null shells as LOD geometry for all the shells except one.
	 \param n_lod_levels The number of lod levels to be generated.
	 \param reduction_ratio The ratio of the number of triangles in an LOD vs. the number of triangles in the previous LOD.
	*/
	static void		Generate_LODs( bool do_recurse = true, bool delete_orig_shells = false, bool insert_null_lods = false, int n_lod_levels = 2, float reduction_ratio = 0.20 );

	/*!
	 This method allocates memory for merging the shells contained below the segment open in the current segment tree for
	 use in method like #Recursive_Merge_Shells.
	 \param total_info Pass a pointer in which this method can allocate memory large enough to hold all the shell data.
	 \param max_info Pass a pointer in which this method will allocate memory large enough to hold the largest shell data.
	 \param do_recurse Pass true if you want the segment to be recursively searched for shells and allocated memory for.
	*/
	static bool		Segment_Shell_Allocate( HShell *total_info, HShell *max_info, bool do_recurse );

	/*!
	 This method is a helper function to #Segment_Shell_Allocate. It finds how much memory is necessary for the current 
	 and child segments shells. 
	 \param total_info A pointer to the HShell that should be big enough to hold all the data for all of the shells put together.
	 \param max_info A pointer to the HShell that should be big enough for the largest shell.
	 \param do_recurse Pass true for the segment to be recursively searched for shells.
	*/
	static void		Shell_Search( HShell *total_info, HShell *max_info, bool do_recurse );



	/*!
	 \b This is currently an INTERNAL USE function that is in development.  Walks down the tree starting with 'new_seg_key', and builds a new tree
	 underneath a new segment identified by 'new_seg_key'.  The 'old_seg_key' contents are flushed.  The logic groups graphical primtives into segments based on attributes,
	 and transforms primitives and merges shells depending on the passed-in parameters.
	 \param old_seg_key HOOPS key of the segment tree to be optimized
	 \param new_seg_key HOOPS key of the top of the new segment tree; this is a subsegment of the 'old_seg_key'.  Passed by reference, returned to user.
	 \param transform_shells A boolean value indicating that shell points should be xformed through any local modeling matrix
	 \param merge_shells A boolean value indicating that shells in each of the final segments should be merged into a single shell
	 \param create_shell_regions A boolean value indicating that if original shells were merged, to mark them as regions in the final shell
	*/
	static void		OptimizeSegmentTree(HC_KEY old_seg_key, HC_KEY *new_seg_key, bool transform_shells = true, bool merge_shells = true, bool create_shell_regions = false);


	/*!
		This method counts the number of faces, vertices and triangles in a given shell primitive. 
		\param shell_key The HOOPS key to a given shell.
		\param faces Returns the number of faces in the given shell.  Passed by reference.
		\param vertices Returns the number of vertices in the shell, including duplicates.  Passed by reference
		\param triangles Returns the number of triangles in the shell after HOOPS triangulation calculation.  Passed by reference.
	    \param tristrips Returns the number of tristrips in the shell.  Pass by reference.
	*/
	static void		CountShellData(HC_KEY shell_key, int * faces, int * vertices, int * triangles, int * tristrips = 0);
 
	/*! 
	  	Given a file name, this method returns the file extention.  The extentions is returned in lowercase only.	

		\param filename_in A null terminated string holding a filename.
		\param ext_out Pass a char pointer of size > strlen(filename_int)+1.  This method will return the file extension.
	*/
	static void		FindFileNameExtension(const char * filename_in, char * ext_out, int size = -1);

	/*! 
	  	Given a file name, this method returns the file extention.  The extentions is returned in lowercase only.	

		\param filename_in A null terminated string holding a filename.
		\param ext_out Pass a char pointer of size > strlen(filename_int)+1.  This method will return the file extension.
	*/
	static void		FindFileNameExtension(__wchar_t const * const filename_in, __wchar_t * ext_out, int size = -1);
#ifdef H_USHORT_OVERLOAD
	static void		FindFileNameExtension(unsigned short const * const filename_in, unsigned short * ext_out, int size = -1){
		FindFileNameExtension((wchar_t const * const)filename_in, (wchar_t*)ext_out, size);
	}
#endif
	/*! 
	  	Given a unicode file name, this method returns the file extention.  The extentions is returned in lowercase only.	
		\param filename_in A null terminated UNICODE string holding a filename.
		\param ext_out Pass a char pointer of size > strlen(filename_int)+1.  This method will return the file extension.
	*/
	static void		FindUnicodeFileNameExtension(const unsigned short * filename_in, char * ext_out);

	/*! 
	*	Given a filename with path, this method returns the filename and directory.

		\param filename_in A null terminated string holding a filename.
		\param dir_out Pass a char pointer of size > strlen(filename_int)+1.  The method returns the directory part of the filename.
		\param filename_out Pass a char pointer of size > strlen(filename_int)+1.  The method returns the short filename part of the filename.
	*/
	static void		FindFileNameAndDirectory(const char * filename_in, char * dir_out, char * filename_out);
	/*! 
	*	Given a filename with path, this method returns the filename and directory.

		\param filename_in A null terminated string holding a filename.
		\param dir_out Pass a char pointer of size > strlen(filename_int)+1.  The method returns the directory part of the filename.
		\param filename_out Pass a char pointer of size > strlen(filename_int)+1.  The method returns the short filename part of the filename.
	*/
	static void		FindFileNameAndDirectory(__wchar_t const * filename_in, __wchar_t * dir_out, __wchar_t * filename_out);

	/*!
		HOOPS likes window handles and colormaps to be printed as
		hex value with the form 0xXXXXXXXX but some systems sprintf
		functions print pointer with out the 0x part.

		So use sprintf(buffer,"%s%p",extra_pointer_format(),pointer) to
		ensure correct formating for HOOPS.
		
		\return A system dependent pointer printing extention either "" or "0x".
	*/
	static const char* extra_pointer_format();
	/*! 
		<b>This method is for internal use only.</b>  
	*/
	static HC_KEY CreateScaleIndependentSegment(const char *segname, bool SetCallback = true);

	/*! 
		<b>This is for internal use only.</b>
	*/
	static HC_KEY GetScaleIndependentGeomSegmentKey(HC_KEY segkey);

	/*! 
		<b>This is for internal use only.</b>
	*/
	static void SetScaleIndependentGeomPosition(HC_KEY segkey, float x, float y, float z);
 
	/*! 
		<b>This is for internal use only.</b>
	*/
	static void GetScaleIndependentGeomPosition(HC_KEY segkey, float &x, float &y, float &z);

	/*! 
		This method register callbacks for HUtility with the #HImManager.
	*/
	static void RegisterCallbacks();

	/*! 
		This method determines if the given key is underneath the model segment.
 		\param modelkey They key to the model segment.
		\param objectkey The key to test.
		\return True if the key is underneath the model segment or false if otherwise.
	*/
 	static bool IsModelKey(HC_KEY modelkey, HC_KEY objectkey);

	/*! 
		This method determine if user options exists in or above the current segment. 
 		\param key The segment to begin the search.
		\param attrib The attribute to search for.
		\param result The attribute value of user option if it exists.
		\return The highest segment where the user option was set or INVALID_KEY if no option existed.
	*/
	static HC_KEY UserOptionExists(HC_KEY key, const char *attrib, char *result);

	/*! 
	  Inserts a circular arrow consisting of a cylinder and a cone 
	  display.  The arrow is completely customizable.
	  \param stick_length The length of the cylindrical shaft of the arrow.
	  \param tip_length The length of the conical tip of the arrow.
	  \param stick_radius The radius of the cylindrical shaft of the arrow.
	  \param tip_radius The radius of the circular base of the arrow's conical tip.
	  \param distance The x coordinate position of the center of the shaft.
 	  \param adelta The number of angular steps.
 	  \param rotang The angular interval.  Multily this value by adelta to get maximum arrow radius.
	  \param stick_color The color of the shaft of the arrow.
	  \param tip_color The color of the tip of the arrow.
	  \param num_sides The number of sides of the arrow.  The default is six.
	  \param tip_falloff The radius reduction value on the stick.  The default is 0.05.
	  \param reverse Pass true to invert the arrow's orientation.
	*/
 	static void InsertRotatedArrow(float stick_length, float tip_length,  float stick_radius, float tip_radius, float distance,  int adelta, float rotang, const char *stick_color, const char *tip_color, int num_sides, float tip_falloff, bool reverse);

	/*! 
	  This function returns a list of keys from the current segment to the top level scene segment.
 	  \param startkey The segment to start the keylist from.
 	  \param view A pointer to the current HBaseView object.
	  \param keyarray Pass a pointer to an array that will hold the keylist.  This method wil populate
	  the array.
	  \param maxarray The maximum size of array to calculate.
	  \return The size of the final keylist.
	*/
	static int CalculateKeyPath(HC_KEY startkey, HBaseView *view, HC_KEY *keyarray, int maxarray);

	/*!	This method determines if the segment with given name exists underneath the currently open segment.
		\param segment The name of the segment to test existence for.
		\return True if the segment does exist underneath the currently open segment or false if it doesn't.
	*/
	static			bool SegmentExists(const char *segment);

	/*! The method unsets various attributes in current segment. Currently, it unsets visibility, heuristics, and 
	rendering options. */
	static			void UnsetAttributes();  

	/*! This method determines if a given segment is a child of the given <i>owner</i> segment.
	   \param owner The key of the owner segment.
	   \param child The key of the potential child segment.
	   \param klist A pointer to a list of keys.  If non-null, the list of keys of segments between the 
	                child and owner will get written to the list.
	   \return True if the <i>child</i> segment is indeed a child of the <i>owner</i> segment.
	 */
	static			int IsChildOf(HC_KEY owner, HC_KEY child, HC_KEY klist[] = 0);

	/*!
		This function returns true if the matrix passed in is an identity matrix and false otherwise.
		\param matrix the matrix to be queried.
		\return True if matrix is an identity, false otherwise.
	*/
	static			bool IsIdentityMatrix(float matrix[]);

	/*! 
	  The method determines an appropriate normal vector for the given shell.  For closed objects, the normal can be 
	  somewhat random.  This is used for Zoom to Shell like functionality.
	  \param skey The key to the shell you want a normal for.
	  \param tnormal A pointer to an HPoint object.  The method will populate this parameter with the calculated normal vector.
	*/ 
	static			void ComputeShellNormal(HC_KEY skey, HPoint &tnormal);
	
	/*!
	  \b Deprecated.  Please use #SmartSelectUnderCursor. Puts item under cursor in selection list.
 	  \param view Pointer to HBaseView Object.
	  \param x  Window X Coordinate for selection testing
	  \param y  Window Y Coordinate for selection testing
	  \param doSelection  Add Geometry to Selection List
	  \param DeSelect  De Select Geometry if already selected
	  \return Key to selected geometry
	*/ 
	static			HC_KEY  SelectUnderCursor(HBaseView *view, float x, float y, bool doSelection = true, bool DeSelect = false);


	/*!
	  This method puts the item under cursor in the selection list. This api uses improved selection capability which 
	  helps the selection set deal with instanced segments and entities.
 	  \param view A pointer to HBaseView Object.
	  \param x  The window x coordinate for the selection testing.
	  \param y  Window Y Coordinate for selection testing.
	  \param doSelection  Add Geometry to Selection List.
	  \param DeSelect  Pass true to deselect the geometry if it already selected.
	  \return The key to the selected geometry.
	*/ 
	static			HC_KEY  SmartSelectUnderCursor(HBaseView *view, float x, float y, bool doSelection = true, bool DeSelect = false);


	/*!
	  This method shows the specificed heuristic setting in the currently open segment.
 	  \param type The heuristic Setting to query (e.g. "quick moves").
	  \param res The result of query (e.g. "spriting").
	  \return True if the heuristic setting is set or false if it is not set.
 	*/ 
	static			bool ShowOneHeuristic(const char *type, char *res = 0);
	/*!
	  This method shows the specified rendering option setting in the currently open segment.
 	  \param type The rendering option setting to query (e.g. "lighting interpolation").
	  \param res The result of the query (e.g. "phong")
	  \return True if the rendering option setting is set or false if it is not set.
 	*/ 
	static			bool ShowOneRenderingOption(const char *type, char *res = 0);

	/*!
	  This method calculates the EaseIn/EaseOut for a given interval and speed.  This ensure that animations have a 
	  smooth transition at the beginning and end.
 	  \param t The current interval which is from 0 to 1.
 	  \param a The ease in duration given a normalized interval.
	  \param b The constant speed duration.
	  \param c The ease out duration given a normalized interval.
	  \return The newly recalculated interval.
 	*/ 
	static			float EaseInEaseOut(float t, float a, float b, float c);

	/*!
	  From the current selection, this method walks up the segment tree until all includes are resolved.
 	  \param key Segment to unwind from.
 	*/ 
	static			void UnwindIncludes(HC_KEY &key);

	/*!
	This method is a helper function for #UnwindIncludes(HC_KEY &key).  
	\param newkey The key to unwind from.
	\param keys The list of keys in the selection list.  
	\param count The number of keys in the selection list.
	*/
	static			void UnwindIncludes(HC_KEY &newkey, HC_KEY *keys, int count);


	/*!
	This method acts as a wrapper around HC_Set_User_Options and protects and validates input to it.
	It acts on the currently open segment.
	\param opt The user option to be input.
	\return true if the option was set successfully, false otherwise.
	*/
	static			bool SafeSetUserOption(const char * opt);

	/*!
	This method acts as a wrapper around HC_Show_User_Options and strips out extra characters
	inserted by SafeSetUserOption.  It acts on the currently open segment.
	\param opt The user option (if any).  Returned to user.
	\return true if a user option was found, false otherwise.
	*/
	static			bool SafeShowUserOption(char * opt);


	/*!
	 This method projects the given shell to the net modelling matrix of the current segment.
 	  \param shellkey The key to the shell.
  	*/ 
	static void ProjectShell(HC_KEY shellkey);

	/*!
	 This method merges two shells into one.
 	  \param shell1 The key to the first shell.
 	  \param shell2 The key to the secon shell.
	  \return The key of the newly merged shell.
  	*/ 
	static HC_KEY MergeShells(HC_KEY shell1, HC_KEY shell2);

	/*!
	  This method returns the modelling matrix of the currently open segment or identity matrix in none exists.
	  \param segment_modelling_matrix Pass a pointer to an array that can hold a 4 x 4 matrix.  The method will populate 
	  it with the values of the currently opened segment's modelling matrix.	  
  	*/ 
	static void ShowModellingMatrix(float *segment_modelling_matrix);
	
	/*!
	  This method converts a point from local pixels space to local window space.
	  \param view A pointer to the HBaseView Object.
	  \param in_system The coordinates in Local Pixels.  The method will do an in place conversion if you pass null for out_system.
	  \param out_system Pass a pointer an HPoint distinct from in_system if you do not want an in place conversion.  If you do, pass null.
	  */ 
	static void ConvertLocalPixelsToLocalWindow(HBaseView * view, HPoint *in_system , HPoint *out_system = 0);


	/*!
	  This method returns the key of the input object's owner. If the input is itself a segment key, it 
	  is returned as is.  
	  \param key The object key which can be segment or geometry.
	  \return The owner segment key.
    	*/ 
	static HC_KEY GrabSegment(HC_KEY key);

	/*! \def MakeSegment
	    provides backwards compatibility for apps that were relying on MakeSegment, the old 
	    name for GrabSegment.  MakeSegment was a poor choice for a name since it wasn't actually 
	    making anything */
#	define MakeSegment(key) GrabSegment(key)

	/*!
	  This method renames a segment given by the key.
	  \param segkey The key to the segment you want to rename.
	  \param newname The new name of the segment.
    	*/ 
	static void RenameSegment(HC_KEY segkey, const char *newname);

	
	/*!
	  Given two points and the normals at those points, this method calculates 
	  the interpolated normals given an additional third point.
	  \param p1 The first point used for interpolation.
	  \param n1 The first normal used for interpolation.
	  \param p2 The second point used for interpolation.
	  \param n2 The second normal used for interpolation.
	  \param p3 The third point used for interpolation,
	  \param n3 Returns the calculated normal.
     	*/ 	
	static void InterpolateNormalAlongEdge(HPoint &p1, HPoint &n1, HPoint &p2, HPoint &n2, HPoint &p3, HPoint &n3);

	/*!
	  This method renders the normals of a given shell as lines.
	  \param shellkey The key of the shell to show the normals for.
	  \param normallength The length of the lines that will be rendered to represent the normals.
     	*/ 
	static void ShowNormals(HC_KEY shellkey, float normallength);

	/*!
	  This method determines if the given key belongs to a segment.
	  \param key The key to identify.
	  \return True if the key is a segment key or false if it is not.	  
     	*/ 
 	static bool IsSegmentKey(HC_KEY key);

	/*!
	  This method is a wrapper for #HC_Find_Contents.  Note you must call HC_Begin_Contents_Search before this call and 
	  #HC_End_Contents_Search after this call.
	  \param key The key to identify.
	  \return True if items were found.	  
     	*/ 
	static int FindContents(HC_KEY &key);

	/*!
	  This method calculates the major axis of the current camera.
	  \param view A pointer to the HBaseView object.
	  \return The value 0 if the major axis is X or the value 1 if the major axis is Y or the value 2 if the major axis is
	  Z. 
     	*/ 
	static int CalculateMajorAxis(HBaseView *view);

	
	/*!
	 This method find a objects of given type in the current segment or globally.
	  \param type HOOPS entity type like segment, shell, etc.
	  \param l Pass true to search local or false to search globally.
	  \return The key to the matching HOOPS entity or INVALID_KEY if no object was found.
     	*/ 

	static HC_KEY FindOneObject(const char *type, bool l = 0);
	/*!
	  This method closes the current segment and then walks up the database tree closing the any 
	  segments that are open until it reaches the top level segment or the end key if one is passed.
	  \param endkey Pass a key that will end the method.
      	*/ 
	static void CloseAllSegments(HC_KEY endkey = INVALID_KEY);

	/*!
	  This method resets the camera target to be in the center of the current view.
	  \param view A pointer to the HBaseView Object.
      	*/ 
	static void ReadjustTarget(HBaseView *view);

	/*!This method returns whether the user option to suppress animation is set for a given segment.
	  \param key The key of the segment in which you want to check the suppress animation option.
	  \return True if the suppress animation option was set or false if it wasn't.
      	*/ 
	static bool GetAllowAnimation(HC_KEY key);

	/*!This method sets the user option to suppress animation for the currently opened segment.
      	*/ 
	static void SuppressAnimation();
	
	/*!
	  Given Face Index (0,1,2...), this method finds position of face in face list array as described in #HC_Insert_Shell.
	  \param findex The face index.
	  \param flist The face list array.
	  \param flen The length of face list array.
	  \return Position of the face in the face list array.
       	*/ 
 	static int  FindFaceByIndex(int findex, int *flist, int flen);

	/*!
	  This method shows the specified user option in the currently opened segment.
	  \param option The user option to search for.
	  \param res The result of the query.
	  \return True if the user option setting is set or false if it is not set.
      	*/ 
	static bool ShowOneUserOption(const char *option, char *res);

	/*!
	  This method finds the next unused globally renumbered key.
 	  \param view A pointer to the HBaseView object.
 	  \return The next available renumbered key.
       	*/ 	
	static HC_KEY GetNextRenumberedKey(HBaseView *view);

	/*!
	  This method gives unique names to the passed segment and all its parents.
	  \param key The key to the start segment.
 	  \param view A pointer to the HBaseView object.
       	*/ 	
	static void NameSegments(HC_KEY &key, HBaseView *view);
	
	/*!
	  This method moves a given segment to a temporary segment.
	  \param movekey The segment you want to move.
 	  \param tempkey The temporary segment to move to.	  
       	*/ 	
	static void MoveToTemp(HC_KEY movekey, HC_KEY tempkey);

	/*!
	 This method recursively finds all entities in and below the current segment and returns each entity with its 
	 complete segment path. 
	  \param entitytypes The entities to search for.  
	  \param ret_pathlist Pass a pointer to a vlist.  This method will populate it with all found entities. Each 
	  list element is a pointer to a #HShowContentsWithPathType structure and needs to be freed by the caller.
	  \param storePath Pass true if you want to the method to store the complete path in addition to entity key.
 	  \param includeIncludes Pass true to search included segments.
	  \param filterIncludes	Pass true to filter include keys from the list.
       	*/ 	
 	static void ShowContentsWithPath( const char * entitytypes, struct vlist_s **ret_pathlist, bool storePath = false, bool includeIncludes = true, bool filterIncludes = true);
	
	
	/*!
	    This method returns the size of the unicode string.
	    \param text The unicode text string.
	    \return Size of the string excluding the delimiter.
       	*/ 	
	static int ustrlen(unsigned short *text);

	/*! This method compresses the given data using RLE, run length encoding. 
	 *  \param in_len The length of the input buffer.
	 *  \param in_buf A pointer to the data to be compressed.
	 *  \param out_len Pass the length of the output buffer.  When this method returns,
	 it will populate the out_len with the value of the lenght of the compressed data.
	 *  \param out_buf Pass a pointer so that this method can return where the output 
	 data is to be stored.  This can be the same as in_buf if you want to do an in place compresion.. 
		\return True if the operation is was success or false if it failed.
	 */
	static bool RLECompress(int const in_len,
							unsigned char const * const in_buf,
							int * out_len,
							unsigned char * const out_buf);
	
	/*! This method compresses the input buffer using the 'zlib' compression library.
	 *  \param in_len The length of the input buffer.
	 *  \param in_buf The data to be compressed.
	 *  \param out_len Pass the length of the output buffer.  On return, out_len is the 
	 length of the compressed data.
	 *  \param out_buf The output data is stored here.  To do an in place compression, you can
	 pass the same pointer as in_buf. 
	 \return True if the operation was a success or false if it failed.
	 */
	static bool ZlibCompress(int const in_len,
							 unsigned char const * const in_buf,
							 int & out_len,
							 unsigned char * const out_buf);

	/*! This method decompresses the input buffer using the 'zlib' compression library.
	 *  \param in_len The length of the input buffer.
	 *  \param in_buf The data to be decompressed.
	 *  \param out_len Pass the length of the output buffer.  On return, out_len is the 
	 length of the decompressed data.
	 *  \param out_buf The output data is stored here.  To do an in place decompression, you can
	 pass the same pointer as in_buf. 
	 \return True if the operation was a success or false if it failed.
	 */
	static bool ZlibDecompress(int const in_len,
							 unsigned char const * const in_buf,
							 int & out_len,
							 unsigned char * const out_buf);
/*!
	   This method converts an 8-bit string to a unicode string.
	   \param text The 8-bit string to convert.
	   \return A pointer to the newly created unicode string.
       	*/ 	
	static unsigned short *uconvert(const char *text);

	/*!
	  Given an array of include keys, this method builds a full path consisting of segment keys and includes.
	  \param objectkey The key to start segment or geometry.
	  \param includelist The list of includes.
	  \param ilength The length of include list.
	  \param fullpath Return the full key path derived from the object key and include list.
	  \param fplength Returns the length of the key path.
	  \param matchkey Pass a key that when found by this method will stop building the path.  If you want the method to walk the entire tree, pass INVALID_KEY. 

       	*/ 	
	static bool BuildIncludePath(HC_KEY objectkey, const HC_KEY *includelist, int ilength, HC_KEY *fullpath, int *fplength, HC_KEY matchkey = INVALID_KEY);

	/*!
	This method builds the include path for a given selection item.
	\param sitem A pointer to the selection item.
	\param plen Returns the length of the include path.
	\param fullpath Return the full key path of the selection item.
	*/
	static void BuildIncludePathFromSelection(HSmartSelItem *sitem, int &plen, HC_KEY *fullpath);


	/*!
	  This method creates an image snapshot of the contents of a given view.
	  \param view A pointer to the view you want a snapshot of.
	  \param width The width of the image.
	  \param height The height of the image.
	  \param data Pass a double pointer so that HOOPS can return the character pointer used to store 
	  the image data. Note you must free this memory with delete []. 
       	*/ 	
	static void MakeViewSnapshot(HBaseView * view, int width, int height, char ** data);

	/*!
	  This method creates an image snapshot of the contents of a given view.
	  \param view A pointer to the view you want a snapshot of.
	  \param image_key to which we will render.  Create this image before calling MakeViewSnapshot with this call: HC_KEY image_key = HC_KInsert_Image(0, 0, 0, "rgba", width, height, 0).
       	*/ 	
	static void MakeViewSnapshot(HBaseView * view, HC_KEY image_key);

	/*! 
		Given an input matrix and a vector, this method returns the original matrix but with the
		translation component constrained so that translation is along the provided vector.
		\param matrix_in A pointer to the modelling matrix.
		\param vector A vector along which translation should be constrained to.
		\param matrix_out Pass a pointer to an array that can hold a 4 x 4 matrix.  The method will populate this array with 
		the constrained matrix.
		*/
	static int ConstrainToVector( const float matrix_in[], const HVector &vector, float matrix_out[]);

	/*! 
		Given a camera, this method extracts the position, projection and full matrix information and returns it to the caller.
		\param camera A pointer to the camera information.
		\param pos Pass a pointer to an array that can hold a 4 x 4 matrix.  The method will populate it with the position information.
		\param projection_matrix Pass a pointer to an array that can hold a 4 x 4 matrix.  The method will populate it with the projection information.
		\param complete_matrix Pass a pointer to an array that can hold a 4 x 4 matrix.  The method will populate it with the complete matix information.	
		*/
	static void Camera_To_Transform (HCamera *camera , float *pos, float projection_matrix[], float complete_matrix[]);
	/*! 
		This method calculates the the transformation matrix for local pixels to world coordinates.
		\param camera A pointer to the camera information.
		\param righthanded Pass true if the handedness setting is right or false if the handedness is left.
		\param xres The horizontal screen resolution. 
		\param yres The vertical screen resolution. 
		\param xwpix The number of horizontal pixels.
		\param ywpix The number of vertical pixels.
 		\param result Pass an array that can hold a 4 x 4 matrix.  This method will return the transformation matrix.
 		*/
	static void LocalPixelsToWorld(HCamera *camera, bool righthanded, float xres, float yres, int xwpix, int ywpix, float result[]);
	/*! 
		This method calculates the angle between two vectors.
		\param p1 The first vector.
		\param p2 The second Vector.
		\return The angle between the two vectors. 
 		*/
	static float ComputeAngleBetweenVector(HPoint p1, HPoint p2);
	

	/*! 
		This method calculates the length of the give 16-bit string.
		\param p16 The 16-bit character string.
 		\return The string length.
 		*/	
	static int strlen16( const unsigned short *p16);

	/*! 
		This method uses #HC_Compute_Circumcuboid to calculate bounding information for a given segment hierachy.
		\param startkey The key of the segment to calculate bounding for.
		\param min Returns the minimum point of the bounding box.
		\param max Returns the maximum point of the bounding box.
		\param delta Returns the extent of the bounding box.
		\param mid Returns the center point of the bounding box.
  		*/
	static void CalculateBoundingInfo(HC_KEY startkey, HPoint &min, HPoint &max, HPoint &delta, HPoint &mid);

	/*! 
		This method clips a polygon against a plane described by the planar equation ax + by + cz + d = 0.
		\param vertexCount The number of vertices in the input polygon.
		\param vertex The vertices of input polygon.
		\param plane A pointer to an array that holds the five parameters a, b, c and d represented in the planar equation
		ax + by + cz + d = 0 respectively.
		\param result Returns the points of the newly clipped polygon.
		\return The number of points in the clipped polygon.
 		*/
	static long ClipPolygonAgainstPlane(long vertexCount, const HPoint vertex[],
		const float plane[], HPoint result[], bool *noCut = 0);

	/*! 
		This method performs a two pass print.
		\param driver The driver to print to.
		\param options A pointer to the options structure containing miscellaneous options.
		\return The result of the printings.
 		*/
	static HFileOutputResult TwoPassPrint(const char *driver, HOutputHandlerOptions *options);

	/*! 
		This method opens a segment with the given name underneath the segment given by <i>key</i>.  
		If the segment doesn't exist, a new one is created.
		\param key The key of segment which contains the newly created/opened segment.
		\param segment The name the segment you want to open.
		\return The key to <i>segment</i>.
 		*/
	static HC_KEY OpenSegmentFromKey(HC_KEY key, char *segment);


	/*! 
		This method determine if the given point enclosed by all the given edges.
		\param pt The point to test.
		\param points1 The points array.
		\param len The length of the points array.
		\param face1 The face connectivity array as described in #HC_Insert_Shell.
		\param plane1 A pointer to an array of four values that specify the a, b, c and d values of the
		planar equation ax + by + cz + d = 0.
		\param fudge The threshold to decide in the point is inside or outside the plane.  
 		\return True if the point is inside the edges or false if it isn't.
 		*/
	static bool IsPointInsideAllEdges( const float *pt,const float points1[], 
		int len, const int face1[], const float plane1[], float fudge );
	/*! 
		This method calculates the intersection point between a line and a plane.
		\param p1 The starting point of the line.
		\param p2 The end point of the line.
		\param p A pointer to the plane.
		\param res Returns the intersection point.
  		*/
	static void IntersectionLinePlane(HPoint *p1, HPoint *p2, HPlane *p, HPoint *res);

	/*!
		This method removes manipulator handles from the geometry or segment identified by key
		\param key The key to the geometry or segment with manipulators
	*/

	static void DeleteManipulators(HC_KEY key);

	 
	/*! 
		This method merges an array of points into a given shell.
		\param num_shell_points The mumber of points in the shell
		\param plist1 The shell point list.
		\param num_shell_facelist The length of the face list
		\param flist1 The face list as described in #HC_Insert_Shell.
		\param num_merge_points The number of points to merge.
		\param merge_points A pointer to an array of points to merge.
		\param merge_vectors An optional vector array to find correct merge location on face.
		\param tolerance Pass a value that indicates if two points are within the tolerance distance, they will be merged into one point.
		\param AddExtraPoints Pass true to allow additional points to be added to improve tesselation.
		\param newpoints Pass an array for this method to populate with the newly created points.
		\param newpointslen This method returns length of the newpoints array after it has been populated.
   		*/
	static void MergePointsIntoShell(int num_shell_points, HPoint plist1[], int num_shell_facelist, int flist1[], 
									int num_merge_points, HPoint merge_points[], HPoint merge_vectors[], 
									float tolerance, bool AddExtraPoints, int newpoints[], int *newpointslen);


	/*! <b>This function is still in development.</b>
		Given a segment, this method subdivides the shells found into smaller shells according to how they are situated 
		spatially.  Given a cube that is divided into smaller sections, the new shells are created such that they are 
		bounded by these smaller sections. 
		\param startkey The key of segment to search for shells in.
		\param newkey key of segment to place subdivided geometry in.
		\param numx The subdivision sections in the X direction.
		\param numy The subdivision sections in Y direction.
		\param numz The subdivision sections  in Z direction.
		\param DoCut Pass true to cut a shell that lies in between multiple sections or false to keep it in tact.  Note that if 
		you pass true, the image may not appear as seamless as the original.
		\param DeleteOriginalShells Pass true to delete the original shells or false to leave them.
		\param breakshells 
  		*/	
	static void SubdivideShells(HC_KEY sskey, HC_KEY startkey, HC_KEY newkey, int numx, int numy, int numz, ShellToRegionMapper *mapper = 0, bool DoCut = true, bool DeleteOriginalShells = false, bool breakShells = false);
 
	/*! This method merges all the polylines in the currently open segment. */
	static void MergePolylines();
	static void CreateZebraStripeTexture(int width, int height, int stripwidth, bool horizontal, HPixelRGB colorstripes, HPixelRGB colorbackground);

	/*! 
		This method converts an ASCII string to a unicode string.  Please use H_UNICODE_TEXT in place of this
		function as this may be removed in the next release.
		\param ascii The ASCII string you want to convert.
		\param unicode A pointer to the array that will hold the converted unicode string.  Note this array 
		should be at least as long as the ASCII string.
   		*/			
	static void ascii_to_unicode(char const *ascii, unsigned short *unicode);

	/*! 
		This method converts a unicode string into an ASCII string.  Please use H_ASCII_TEXT in place of this
		function as this may be removed in the next release.
 		\param unicode The unicode string you want to convert.
		\param ascii A pointer to the array that will hold the converted ASCII String.  Note this array shold be at least 
		as long as the unicode string.
   		*/			
	static void unicode_to_ascii(unsigned short const *unicode, char *ascii);

	/*! 
	This method loads all files with a particular extension into the model segment.  It starts reading files in 
	the given directory and then descends down the directory until it has read all files with the given extension.
	\param model A pointer to the model object to be used.
	\param directory The directory to start searching for files.
	\param extension The extension of the files to be loaded.  All files encountered 
	 with this extension will be loaded.
	*/			
	static void BatchRead(HBaseModel * model, const char * directory, const char * extension);

	/*!
	This function projects a point in world space onto the plane that contains point_in_plane and is
	parallel to the near clipping plane which is perpindicular to the view vector.
	\param position The point to be projected into the plane.  This function returns the new adjusted coordinates.
	\param point_in_plane A point in the plane (which is perpindicular to the view vector) to which
	we're going to project out world space position point.
	*/
	static void AdjustPositionToPlane(HBaseView * view, HPoint &position, HPoint point_in_plane);

	/*! This method calculates the exact coordinates to place an image in a given corner of a window.
	\param view A pointer to the HBaseView object.
	\param iwidth The width of the image.
	\param iheight The height of the image.
	\param corner The corner in which you want to place the window which can be TopRight or BottomRight.
	\param posx Returns the x coordinate for the image.
	\param posy Returns the y coordinate for the image.
	*/
	static void CalculateCornerImagePosition(HBaseView *view, int iwidth, int iheight, HWindowCorner corner, float &posx, float &posy);
	static void OptimizeModelByGrid(HBaseView *view, ShellToRegionMapper *mapper);
	static void DecipherTextureString(char *texdef, char *Source, bool *DownSampling = 0, bool *DownSamplingSet = 0, char *ParameterizationSource = 0,
									 char *InterpolationFilter = 0, char *DecimationFilter = 0, char *Layout = 0, char *Tiling = 0, bool *ValueScaleSet = 0,
									 bool *ValueScale = 0,  float *ValueScaleMin =0, float *ValueScaleMax = 0, char *BlendingMode = 0);


private:
	static void		scale_segment (HIC_Rendition *rendition,HIC_Segment_Info * geo3); 
	static void		activity_change (HIC_Segment *seg, int old_activity, int new_activity); 
 	static bool		RestoreAttributeFromOption(const char *attName, HUserOptions& attText);
	static void		ReplaceCharacter(char *text, char orig, char target);
 	static bool		SetAttribute(const char *attName, const char *attText);
	static bool		ShowAttribute(const char *attName, char *attText);
	static void		ShowContentsWithPathAddEntity(HC_KEY key, struct vlist_s *pathlist, int level, HC_KEY *patharray, bool storePath);
	static void		ShowContentsWithPathRecursive(HC_KEY key, const char *entitytypes, struct vlist_s *pathlist, HC_KEY *patharray, int level, bool storePath, bool includeIncludes, bool filterIncludes);
	static void		SubdivideOneShell(HShellWrapper &myshell, int numx, int numy, int numz, HPoint &totalmin,
								 HPoint &totaldeltadiv, HShellWrapper *shellmatrix, HShellWrapper &leftover, float boxdiagonallength, bool DoCut, int regionnum, ShellToRegionMapper *mapper, HC_KEY newkey);


};


class MVO_API HQualifiedKey
{
public:
	HQualifiedKey(HC_KEY key, HC_KEY includelist[], int includelistlength)
	{
		m_Key = key;
		m_pIncludeList = 0;
		m_pIncludeList = new HC_KEY[includelistlength];
		for (int i = 0; i < includelistlength; i++)
			m_pIncludeList[i] = includelist[i];
		m_IncludeListLength = includelistlength;
	}
	~HQualifiedKey()
	{
		delete [] m_pIncludeList;
	}

	long MakeHashKey()
	{
		return MakeHashKey(m_Key, m_IncludeListLength,m_pIncludeList);
	}

	static long MakeHashKey(HC_KEY key, int includelength, HC_KEY includes[])
	{
		long hashkey = static_cast<long>(key);
		for (int i = 0; i < includelength; i++)
		{
 			hashkey += static_cast<long>(includes[i]);
		}
		return hashkey;
	}

	HC_KEY m_Key;
	HC_KEY * m_pIncludeList;
	int m_IncludeListLength;
};




class MVO_API HMergeShellFace
{

public:
	HMergeShellFace()
	{
		m_pChildFaces = 0;
		m_ChildFaceNum = 0;
	}
	~HMergeShellFace()
	{
		if (m_pChildFaces)
			delete [] m_pChildFaces;
		if (m_pFlist)
			delete [] m_pFlist;

	}
 	void Init(int * flist);
	void MakeChildFaces(int & addpoint, HPoint * points, bool AddExtraPoints, int * newpoints, int * newpointslen);
	void Divide(int pnum, HPoint * points);

	bool CheckHit(HPoint & p, HPoint & normal, HPoint * plist, HPoint & pres);
 	HMergeShellFace * FindHitFaceRecursive(HPoint & p, HPoint & normal, HPoint * plist, HPoint & respoint);
	HMergeShellFace * FindHitFace(HPoint & p, HPoint & normal, HPoint * plist, HPoint & respoint)
	{		 
		return FindHitFaceRecursive(p, normal, plist,respoint);	 
	}
	bool CheckIfColinear(HPoint * point);
	
	int GetLengthRecursive(int l);

	int GetLength()
	{
 		return GetLengthRecursive(0);

	}

	int AddToFacelistRecursive(int * start, int l);
			
	int AddToFacelist(int * start)
	{
 		return AddToFacelistRecursive(start, 0);

	}
 
protected:
	HMergeShellFace * m_pChildFaces;
 	int m_flen;
 	int * m_pFlist;
	int m_ChildFaceNum;
};



class MVO_API ColorPiece{

public:
	ColorPiece(int index[], int len, int col)
	{
		m_uvindex = new int[len];
		m_fllen = new int[len];
		m_flindex = new int[len];
		for (int i=0;i<len;i++)
			m_uvindex[i] = index[i];
		m_len = len; 
		m_col = col;
		m_done = false;

	}
	~ColorPiece()
	{
		delete [] m_uvindex;
		delete [] m_fllen;
		delete [] m_flindex;
	}
	int *m_uvindex;
	int *m_fllen;
	int *m_flindex;
	int m_len;
	int m_col;

	bool	m_done;

};

class MVO_API ColorPieceList
{
public:
	ColorPieceList();
	~ColorPieceList();
	void AddColorPiece(int index[], int len, int col);
	ColorPiece * FindMatch(int col, int ii, int ii2, int &rpos, int &rlen, bool &reverse);
	
	struct vlist_s*			m_ColorPieceList;	
	
	
};

class MVO_API HVerticalUtility :
	public HUtility
{
public:

	static void ColorizePointCloud(HC_KEY const segment);
};



class MVO_API HAbsoluteWindow
{
public:
	HAbsoluteWindow(HBaseView *view, HC_KEY key,float xpos, float ypos, float width, float height,  AbsoluteWindowType xpos_type = RelativeLeft, 
		AbsoluteWindowType ypos_type = RelativeTop, AbsoluteWindowType width_type = RelativeLeft,
		AbsoluteWindowType height_type = RelativeTop, bool sb = false);
	HAbsoluteWindow(HBaseView *view, HC_KEY key); 
	void DecipherOptions();
	void AdjustX(float invalue, AbsoluteWindowType wt, float &outvalue);
	void AdjustY(float invalue, AbsoluteWindowType wt, float &outvalue);
	bool Adjust();
	static void AdjustAll(HBaseView *view);
	static bool SetWindow(HBaseView *view,float xpos, float ypos, float width, float height,  AbsoluteWindowType xpos_type = RelativeLeft, 
		AbsoluteWindowType ypos_type = RelativeTop, AbsoluteWindowType width_type = RelativeLeft,
		AbsoluteWindowType height_type = RelativeTop, bool sb = false);
	bool IsPositionInWindow(float x, float y);
	static bool IsPositionInWindow(HBaseView *view, float x, float y);
	static void CalculateLocalWindowPos(HBaseView *view, float x, float y, float &xout, float &yout);
	void CalculateLocalWindowPos(float x, float y, float &xout, float &yout);
	static void CalculateOuterWindowPos(HBaseView *view, float x, float y, float &xout, float &yout);
	void CalculateOuterWindowPos(float x, float y, float &xout, float &yout);

	

	HBaseView *m_pView;
	float m_width;
	float m_height;
	float m_xpos;
	float m_ypos;
	AbsoluteWindowType m_xpos_type;
	AbsoluteWindowType m_ypos_type;
	AbsoluteWindowType m_width_type;
	AbsoluteWindowType m_height_type;

	bool m_bShowBackground;

	HC_KEY m_key;

};



//! The HUnicodeOptions class is a helper class which wraps up memory management for variable size Unicode options returned from HOOPS.
/*! 
  HUnicodeOptions uses *Show_Unicode_Options_Length* (or variation) to get the length of user option and 
  manages memory allocation and free for Show_Unicode_Options. Following shows an example usage
  
  // without HUnicodeOptions class: problem is assumption that option is max 1024
  unsigned short * options[MVO_BUFFER_SIZE];
  HC_Show_Unicode_Options(options);
  

  // with HUnicodeOptions class: automatically allocates enough memory required (by using Show_XXX_Length routene) and free the memory
  // in the destructor
  HUnicodeOptions options;
  options.Show_Unicode_Options();
 
*/
class MVO_API HUnicodeOptions 
{
public:

	wchar_t* m_data;	//!< actual char* to the user options
	int m_length;	//!< length of current user option
	bool m_valid;

	/*! constructor */
    HUnicodeOptions();

	/*! copy constructor */
    HUnicodeOptions(const HUnicodeOptions& in_data);


	/*! destructor */
    ~HUnicodeOptions();


	/*!
	  IsValid returns true if the user option held in this object is valid i.e. if m_length > 0
	 */
	bool IsValid();

	/*!
	  Show_Unicode_Options Wraps HC_Show_User_Options to use Show_XXX_Length routene and perform memory management.
	  \return A  __wchar_t* to the user options string. You can also access the same via HUserOptions.m_data
	 */
    const __wchar_t * Show_Unicode_Options();

	/*!
	  Show_One_Net_Unicode_Option Wraps HC_Show_One_Net_Unicode_Option to use Show_XXX_Length routene and perform memory management.
	  \return A  __wchar_t* to the user options string. You can also access the same via HUserOptions.m_data
	 */
	const wchar_t* Show_One_Net_Unicode_Option(const __wchar_t* which);
#ifdef _MSC_VER
	const unsigned short * Show_One_Net_Unicode_Option(const unsigned short * which);
#endif

	/*!
	  Show_One_Unicode_Option Wraps HC_Show_One_Unicode_Option to use Show_XXX_Length routene and perform memory management.
	  \return A Show_One_Unicode_Option * to the user options string. You can also access the same via HUserOptions.m_data
	 */
	const wchar_t * Show_One_Unicode_Option(const __wchar_t * which);
#ifdef _MSC_VER
	const unsigned short * Show_One_Unicode_Option(const unsigned short * which);
#endif
	/*!
	  Show_Net_Unicode_Options Wraps HC_Show_Net_User_Options to use Show_XXX_Length routene and perform memory management.
	  \return A  __wchar_t* to the user options string. You can also access the same via HUserOptions.m_data
	 */
	const __wchar_t* Show_Net_Unicode_Options();

private:

	void clear() {
		delete [] m_data;
		m_data = 0;
		m_length = 0;
		m_valid = false;
	}

};



class BREP_Topology;

class BREP_Edge
{
public:
 	BREP_Edge(HC_KEY edgekey)
	{
		m_edgekey = edgekey;
 		m_vertex1 = INVALID_KEY;
		m_vertex2 = INVALID_KEY;
	}

	HC_KEY m_edgekey;
	HC_KEY m_vertex1;
	HC_KEY m_vertex2;
 
};

class BREP_CoEdge
{
public:
 	BREP_CoEdge(BREP_Edge *edge)
	{
		m_edge = edge;
	}

	BREP_Edge * m_edge;
};


class MVO_API BREP_Face
{
public:
 	BREP_Face(HC_KEY facekey);
 	~BREP_Face();

 	BREP_Edge * AddCoEdge(BREP_Topology *topol, void *edgeid, HC_KEY edgekey);

	
	struct vlist_s *		m_CoEdgeList;
	HC_KEY m_facekey;
};

class MVO_API BREP_Topology
{
public:

	struct vhash_s *		m_FaceHash;

	struct vhash_s *		m_EdgeHash;
	struct vhash_s *		m_VertexHash;

	BREP_Face *m_CurrentFace;
 	BREP_Topology();
 	~BREP_Topology();
	BREP_Face *AddFace(HC_KEY facekey);
	BREP_Face *GetFace(HC_KEY facekey);
	BREP_Edge *FindEdge(void *edgeid);
	BREP_Edge *AddEdge(void *edgeid, HC_KEY edgekey);
	HC_KEY AddVertex(void *vertexid, HPoint vpos);
	HC_KEY FindVertex(void *vertexid);



	BREP_Face *GetCurrentFace() { return m_CurrentFace; }

};

class H_FORMAT_TEXT;

class MVO_API HBestImageDriver
{
private:
	H_FORMAT_TEXT * m_dvr;
public:
	HBestImageDriver(char const * prefer_type = 0);
	~HBestImageDriver();

	HC_KEY GetKey() const;

	operator char const* () const;
};


class MVO_API HPVOOptions
{
    private:
        void * impl;

    public:
        HPVOOptions(char const * filename);
        ~HPVOOptions();
        char const * getOption(char const * option) const;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
