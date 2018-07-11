// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityBoolean.h : interface of the HUtilityBoolean class
// Set of utility methods
// All methods are public and static


#ifndef _HUTILITYBOOLEAN_H
#define _HUTILITYBOOLEAN_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"

class HBaseView;

class HDPoint
{
public:
	double x;
	double y;
	double z;

	HDPoint() : x(0.), y(0.), z(0.) {}
	void Set( double _x, double _y, double _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
};

class EdgeObject{
public:
	int faces[2];
	int numfaces;
	int p[2];
	EdgeObject()
	{
	    faces[0] = -1;
	    faces[1] = -1;

	    numfaces = 0;
	    p[0] = -1;
	    p[1] = -1;
	}
	void Init()
	{
	    faces[0] = -1;
	    faces[1] = -1;
	    numfaces = 0;
	    p[0] = -1;
	    p[1] = -1;


	}
 	bool AddFace(int i)
	{
	    if (faces[0] == -1)
	    {	    
		faces[0] = i;
		return true;
	    }
	    else
	    {	    
		faces[1] = i;	 
		return false;
	    }
	}
	void SetPoints(int a, int b)
	{
	    p[0] = a;
	    p[1] = b;
	}
	 
};



class FaceObject{
public:
	int position;
	float angle;
 	FaceObject()
	{
	    position = 0;
	    angle = 0.0f;
	}
	void Init()
	{
	    position = 0;
	    angle = 0.0f;
	}
  	 
};

class SilhouetteCache
{
public:
    EdgeObject **edgematrix;
    int edgematrix_size;

    EdgeObject **validedges;
    int validedges_size;

    FaceObject *facelist;
    int facelist_size;

    HPoint *newpoints;
    int newpoints_size;

    int *newfaces;
    int newfaces_size;

    HPoint *colors;
    int colors_size;

    SilhouetteCache()
    :	edgematrix(0), edgematrix_size(0),
	validedges(0), validedges_size(0),
	facelist(0), facelist_size(0),
	newpoints(0), newpoints_size(0),
	newfaces(0), newfaces_size(0),
	colors(0), colors_size(0)
    {
    }

    ~SilhouetteCache()
    {
	delete [] colors;
        for (int i = 0; i < edgematrix_size; ++i)
	    delete[] edgematrix[i];
	delete[] edgematrix;
	delete[] validedges;
	delete[] newpoints;
	delete[] newfaces;
	delete[] facelist;
    }

    void CheckSizesAndClear( int new_edgematrix_size, int new_validedges_size,
		int new_facelist_size, int new_newpoints_size,
		int new_newfaces_size, int new_colors_size)
    {
	int i,j;

	if( new_edgematrix_size > edgematrix_size)
	{
	    for (i = 0; i < edgematrix_size; ++i)
		delete[] edgematrix[i];
	    delete[] edgematrix;

	    edgematrix_size = new_edgematrix_size;
	    edgematrix = new EdgeObject*[ edgematrix_size];
	    for (i = 0; i < edgematrix_size; ++i)
		edgematrix[i] = new EdgeObject[edgematrix_size];

	}

	for( i = 0; i < edgematrix_size; i++)
	    for( j = 0; j < edgematrix_size; j++)
		edgematrix[i][j].Init();

	if( new_validedges_size > validedges_size)
	{
	    delete [] validedges;
	    
	    validedges_size = new_validedges_size;
	    validedges = new EdgeObject *[validedges_size];
	}

	for( i = 0; i < validedges_size; i++)
	    validedges[i] = 0;

	if( new_facelist_size > facelist_size)
	{
	    delete [] facelist;
	    facelist_size = new_facelist_size;
	    facelist = new FaceObject[ facelist_size];
	}

	for( i = 0; i < facelist_size; i++)
	    facelist[i].Init();

	if( new_newpoints_size > newpoints_size)
	{
	    delete [] newpoints;
	    newpoints_size = new_newpoints_size;
	    newpoints = new HPoint[ newpoints_size];
	}

	for( i = 0; i < newpoints_size; i++)
	    newpoints[i].Set( 0, 0, 0);

	if( new_newfaces_size > newfaces_size)
	{
	    delete [] newfaces;
	    newfaces_size = new_newfaces_size;
	    newfaces = new int[ newfaces_size];
	}

	for( i = 0; i < newfaces_size; i++)
	    newfaces[i] = 0;


	if( new_colors_size > colors_size)
	{
	    delete [] colors;
	    colors_size = new_colors_size;
	    colors = new HPoint[colors_size];
	}
	
	for( i = 0; i < colors_size; i++)
	    colors[i].Set( 0, 0, 0);
    }
};




//! The HUtilityBoolean class contains a set of utility functions related to the polygonal boolean functionality 
/*! 
  HUtilityBoolean includes utility functions for using the 3DGS boolean functionality in interactive boolean
  Applications like n-degree milling.
*/
class MVO_API HUtilityBoolean
{
public:
 


	/*! 
	  	Splits a shell in all three dimensions to allow for localization of boolean operations. The newly created
		geometry will reside in child segments of the segment which contains the original shell.
 		\param view Pointer to HBaseView object
		\param targetshell Key of Shell which will be cut 
	 
	*/
	static void SplitTarget(HBaseView *view, HC_KEY targetshell);
	/*! 
	  	Identifies faces with inconsistent handedness inside a shell and "repairs" those faces
 		\param shellkey Shell Key
		\return new shell with consistent handedness
	*/
	static HC_KEY MakeWindingConsistent(HC_KEY shellkey);

	/*! 
	  	Splits a target shell along one of the three major axis
 		\param targetshell Shell to split
		\param side 0 to 3 indicating the split axis 0 = x, 1 = y, 2 = z
		\param slicenum Number of cuts along the indicated axis

 	*/
 	static void CutBlock(HBaseView *m_pHView, HC_KEY targetshell, int side, int slicenum);


	/*! 
	   Performs a boolean operation of one shell with another 
	   \param s1 Shell Key representing the "Tool" of the boolean operation
	   \param s2 Shell Key representing the "Target" of the boolean operation
	   \param whattype string passed into HC_Compute_Boolean_Shell function determening the type of boolean operation
	   \param doTransform If true the resulting shell is transformed by the modelling matrix of the currently open segment after the operation
	   \return Key to shell which represents the result of the boolean operation
 	*/
	static HC_KEY MakeBoolean(HC_KEY s1, HC_KEY s2, char *whattype, bool doTransform = false);

	/*! 
	   Projects shell by the modelling matrix of its containing segment
	   \param shelllkey Key to Shell
	   \param plen Length of Point Array which contains the projected points of the shell
	   \param points Point Array which contains the projected points of the shell
	   \param flen Length of Face List Array
	   \param faces Face List Array
  	*/
	static void ProjectShell(HC_KEY shellkey, int plen, HPoint *points, int flen, int *faces);
	/*! 
	   Modifies the point list of an existing shell by the inverse of the modelling matrix of its containing segment
	   \param shelllkey Key to Shell
   	*/
 	static void UnProjectShell(HC_KEY shellkey);
		/*! 
	   Performs a boolean operation of one shell with another using double precision
	   \param s1 Shell Key representing the "Tool" of the boolean operation
	   \param s2 Shell Key representing the "Target" of the boolean operation
	   \param whattype string passed into HC_Compute_Boolean_Shell function determening the type of boolean operation
	   \param doTransform If true the resulting shell is transformed by the modelling matrix of the currently open segment after the operation
	   \return Key to shell which represents the result of the boolean operation
 	*/
	static HC_KEY DMakeBoolean(HC_KEY s1, HC_KEY s2, char *whattype, bool doTransform = false);
	/*! 
	   Projects shell by the modelling matrix of its containing segment using double precision
	   \param shelllkey Key to Shell
	   \param plen Length of Point Array which contains the projected points of the shell
	   \param points Point Array which contains the projected points of the shell
	   \param flen Length of Face List Array
	   \param faces Face List Array
  	*/
	static void DProjectShell(HC_KEY shellkey, int plen, HDPoint *points, int flen, int *faces);
	/*! 
	   Modifies the point list of an existing shell by the inverse of the modelling matrix of its containing segment.
	   Uses Double Precision
	   \param shelllkey Key to Shell
   	*/
	static void DUnProjectShell(HC_KEY shellkey);
	/*! 
	   Calculates the silhouette for a given shell given the start and end point. The resulting shell will be placed
	   in a segment called "sil" underneath the segment containing the referenced shell. The result shell will be double precision
	   \param targetshell Key of shell to calculate the silhouette for
	   \param start Start Point for silhouette calculation
	   \param end End Point for silhouette calculation
	   \return Key of Shell representing the silhouette
   	*/
	static HC_KEY DCalculateSilhouette( HC_KEY targetshell, HPoint start, HPoint end);
	/*! 
	   Calculates the silhouette for a given shell given the start and end point. The resulting shell will be placed
	   in a segment called "sil" underneath the segment containing the referenced shell. 
	   \param targetshell Key of shell to calculate the silhouette for
	   \param start Start Point for silhouette calculation
	   \param end End Point for silhouette calculation
	   \return Key of Shell representing the silhouette
   	*/
	static HC_KEY CalculateSilhouette(HC_KEY targetshell, HPoint start, HPoint end);
	
	/*! 
	    This funcion should be called after an ObjectMovedEvent has been triggered from an HObjectManipulationListener 
	   \param view Pointer to HBaseView object
	   \param key Key of moved geometry
	   \param p New Position
	   \param posbeforemovedevent Old Position
 	   \return Event Return
   	*/
  	static int ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p, HPoint posbeforemovedevent);
	
	/*! 
		Determines if a collision occured between the bounding boxes of two shells
 	   \param key1 First shell to determine collision for
	   \param key2 Second shell to determine collision for
	   \param first If true testcollision is called for the first time and default parameters are calculated
	   \param doTransform1 If true perform a projection of first shell
	   \param doTransform1 If true perform a projection of second shell
	   \return True if collision occured
   	*/
	static bool TestCollision(HBaseView *view, HC_KEY key1, HC_KEY key2, bool first, bool doTransform1, bool doTransform2);


	
private:
	static void ShowShell(HC_KEY key, int &plen, HPoint *(&points), int &flen, int *(&faces));
 	static void ReorderFace(int *face);
	static double area3D_Polygon(HPoint* points, int *face);
	static HC_KEY InsertRectangle( const char	*	seg, float       x0, float       y0,
		float       x1, float       y1, float       z,	bool	       fill, bool inverse);
	static HC_KEY InsertRectangle2 ( const char	*	seg, float       z0, float       y0, float       z1,
		float       y1, float       x, bool	       fill, bool inverse);
	static HC_KEY InsertRectangle3 (const char	*	seg, float       x0, float       z0, float       x1,
		float       z1, float       y, bool	       fill, bool inverse);
	static void GetFaceColors(HC_KEY key, HPoint **box1);
	static void GetFaceFlags(HC_KEY key, unsigned short **box1);
	static HC_KEY CalculateSilhouetteInternal(int plen, HPoint *points, int flen, int *flist, HPoint start, HPoint end);
	static float ComputeAngleBetweenVector(HPoint *p1, HPoint *p2);
	static HC_KEY ConvertShellToDouble( HC_KEY targetshell, bool deleteOriginal);
	 static void enumerate_points (HC_KEY key1, HBaseView *view,  const HPoint * p1,
					  const HPoint * p2, HPoint &min, HPoint &max);
	 static SilhouetteCache *silCache;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



