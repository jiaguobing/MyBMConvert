// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUTILITYGEOMETRYCREATION_H
#define _HUTILITYGEOMETRYCREATION_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HUtility.h"
#include "varray.h"
#include <map>

#define BOOLEAN_TYPE_NONE		0
#define BOOLEAN_TYPE_INTERSECT	1
#define BOOLEAN_TYPE_SUBTRACT	2
#define BOOLEAN_TYPE_UNION		3

class Vertex_List_Node
{
public:
	Vertex_List_Node() : next(0) {}

	~Vertex_List_Node() {}

	int ID;
	HPoint point;
	HPoint color_value;
	float color_findex;
	HPoint normal;
	VArray<float> params;

	VArray<int> in_face_array;
	int in_face_current;

	Vertex_List_Node * next;
};

typedef std::multimap<int, Vertex_List_Node *>::iterator nodes_iterator;

//! This class provides utility functions for creating construction geometry as well as simple primitives (spheres, cones, cylinders, etc...)
class MVO_API HUtilityGeometryCreation
{
public:

	/*! 
	  GeneratePointOnCircle generates a single point along a circle based on a centerpoint, a radius, and an angle.
	  
	  \param point A pointer to an HPoint object which denotes the generated point.
	  \param center The center point of the circle.
	  \param radius The length of the radius of the circle.
	  \param axis XY, XZ, or YZ.  This defines the view plane in which the circle will be drawn, as well as the axis of
	  rotation for the angular offset of the point.  XY defines the z-axis, XZ defines the y-axis, and YZ defines the x-axis.
	  \param angle Radians measured from 0 about the axis defined by the 'axis' parameter.
	  \param convert Boolean indicating whether to convert the point from viewpoint space to world space.  The default is true.
	  \return A boolean value indicating success or failure
	*/
    static bool	GeneratePointOnCircle(HPoint * point, HPoint const & center, float radius, int axis, float angle, bool convert=true);
	
	/*! 
	  GeneratePointsOnCircle generates multiple points along a circle based a center point and a radius. The desired number of 
	  points must be specified, and must be equal to the size of the points array if the size of that array has been defined. 
	  Note that the basic work in this function is accomplished by repeated calls to #GeneratePointOnCircle.
	  \param points A pointer to an array of HPoint objects which denote the generated points.
	  \param center The center point of the circle.
	  \param radius The radius of the circle.
	  \param axis XY, XZ, or YZ. This defines the view plane in which the circle will be drawn.
	  \param numpoints The desired number of points in the circle.
	  \param convert Pass true if you want HOOPS to convert the point from viewpoint space to world space or false if you don't.  The default is true.
	  \return True if the operation was a success or false if it failed.
	*/   
	static bool	GeneratePointsOnCircle (HPoint points[], HPoint const & center, float radius, int axis, int numpoints, bool convert=true);


	/*! 
	  GeneratePointsOnCircularArc generates multiple points along an arc based a center point and a radius. The desired number of 
	  points must be specified, and must be equal to the size of the points array if the size of that array has been defined. 
	  Note that the basic work in this function is accomplished by repeated calls to #GeneratePointOnCircle.
	  \param points A pointer to an array of HPoint objects which denote the generated points.
	  \param center The center point of the arc.
	  \param radius The radius of the arc.
	  \param axis XY, XZ, or YZ. This defines the view plane in which the arc will be drawn.
	  \param numpoints The desired number of points in the arc.
	  \param convert Pass true if you want HOOPS to convert the point from viewpoint space to world space or false if you don't.  The default is true.
	  \param start_angle In radians. Angle about the arc axis (in anticlockwise direction) from where the arc starts
	  \param end_angle In radians. Angle about the arc axis (in anticlockwise direction) where the arc starts. For e.g. if the start_angle is 0 and end_angle is 360, the arc will form a full circle.
	  \return true if the operation was a success or false if it failed.
	*/   
	static bool GeneratePointsOnCircularArc (HPoint points[], HPoint const & center, float radius, int axis, int numpoints, bool convert, float start_angle, float end_angle);

	/*!
	CreateWireframeCircleWithEllipse inserts a HOOPS/3dGS ellipse that representing a circle into the currently open segment.
	\param x1 An HPoint object which lies on the circumference of the circle.
	\param x2 An HPoint object which lies on the circumference of the circle.
	\param x3 An HPoint object which lies on the circumference of the circle.
	\return The HC_KEY of the HOOPS geometry inserted if the process was successful or INVALID_KEY if it failed.
	*/	
	static HC_KEY CreateWireframeCircleWithEllipse(const HPoint &x1, const HPoint &x2, const HPoint &x3 ) ;

	/*!
		CreateSphere inserts a HOOPS/3dGS shell representing a sphere into the specified segment.
		\param segment The segment to insert the sphere into.
		\param center The coordinate representing the center of the sphere.
		\param radius The length of the sphere's radius.
		\param num_faces The number of longitudinal strips that will make up the sides of the sphere.
		\return The HC_KEY of the HOOPS geometry inserted if the process was successful or INVALID_KEY if it failed.
	*/	
	static HC_KEY CreateSphere(const char * segment, HPoint center, float radius, int num_faces);
  
	/*!
		CreateSphere inserts a HOOPS/3dGS shell representing a sphere oriented about an arbitrary axis into a no-name 
		subsegment of the currently open segment.  The two reference axes must be orthogonal.
		\param center An HPoint object denoting the center of the sphere.
		\param radius The sphere radius.
		\param numsides The number of longitudinal strips that will make up the sides of the sphere.
		\param axis The first reference axis used to define the orientation of the geometry.
		\param ref The second reference axis used to define the orientation of the geometry. 
		\return The HC_KEY of the HOOPS geometry inserted if the process was successful or INVALID_KEY if it failed.
	*/	
	static HC_KEY CreateSphere(HPoint center, float radius, int numsides, HPoint axis, HPoint ref);
    
	/*!
		CreateCone inserts a HOOPS/3dGS shell (representing a cone oriented about an arbitrary axis) into a no-name subsegment
		of the currently open segment. The two reference axes must be orthogonal.
		\param center An HPoint object denoting the center of the cone.
		\param radius The radius of the cone's base.
		\param height The height of the cone.
		\param numsides The number of longitudinal strips that will make up the sides of the cone.
		\param axis The first reference axis used to define the orientation of the geometry.
		\param ref The second reference axis used to define the orientation of the geometry. 
		\return The HC_KEY of the HOOPS geometry inserted if the process was successful or INVALID_KEY if it failed.
	*/	
	static HC_KEY CreateCone(HPoint center, float radius, float height, int numsides, HPoint axis, HPoint ref);
    
	/*!
		CreateCylinder inserts a HOOPS/3dGS shell representing a cylinder oriented about an arbitrary axis into a no-name subsegment
		of the currently open segment. The two reference axes must be orthogonal.
		\param center An HPoint object denoting the center of the cylinder's base.
		\param radius The radius of the cylinder.
		\param height The height of the cylinder.
		\param numsides The number of longitudinal strips that will make up the sides of the cone.
		\param axis This reference axis define the orientation of the cylinder's base.
		\param ref This reference axis define the orientation of the cylinder's altitude. 
		\return The HC_KEY of the HOOPS geometry inserted if the process was successful or INVALID_KEY if it failed.
	*/	
	static HC_KEY CreateCylinder(HPoint center, float radius, float height, int numsides, HPoint axis, HPoint ref);
    
	/*!
		CreateCuboid creates a solid box spanning diagonally from min to max point.  NOTE: The difference between 
		this routine and the HUtility::InsertBox is that each face does not share any corner points.  Therefore the 
		cuboid appears flat shaded even in smooth shading mode which is realistic.
		\param max Max The coordinates that represent one end of the cuboid.
		\param min Min The coordinates that lie diagonal from <i>Max</i> on the opposite face of the cuboid.
		\return The HC_KEY of the HOOPS geometry inserted if the process was successful or INVALID_KEY if it failed.
	*/	
	static HC_KEY CreateCuboid( HPoint const * max, HPoint const * min);

	/*! Sets a modelling matrix based on the reference axes and position. */
    static void	SetupModellingMatrix(HVector axis, HVector ref, HVector side, HPoint pos);

	/*!
		This function creates a cutting plane in the cutting plane segment of the view.
 		\param view A pointer to the HBaseView object.
 		\param segmentname The name you want to give the cutting plane segment.
		\return The key to the newly created cutting plane segment.
	*/	
	static HC_KEY CreateCuttingPlane(HBaseView *view, const char *segmentname = 0, bool createplane = true);



	/*!
		Sets the given visibility for all cutting planes beginning with the given <i>startkey</i>.
 		\param startkey The cutting plane key start key.
 		\param visibility The visibility setting as described in #HC_Set_Visibility().
 	*/	
     static void SetCuttingPlaneVisibilityForAll(HC_KEY startkey, const char *visibility);

	/*!
		This method changes attribute settings for the cutting planes based on the current rendering mode.  If render mode is
		HRenderHiddenLine, HRenderHiddenLineFast or HRenderBRepHiddenLine, then edges are visible and faces aren't.  If the render 
		mode is anything else, then faces are visible and edges aren't.
 		\param view A pointer to the HBaseView object.
 	*/	
    static void AdjustCuttingPlaneRendering(HBaseView * view);

	/*!This method takes a given key representing intersecting cutting planes and converts them into a cutting 
	section that insect and affect each other.
	\param view A pointer to the HBaseView object.
	\param key The key to the interesecting cutting planes.
	*/
	static void AdjustSection(HBaseView *view, HC_KEY key);

	/*!This method creates a three sided cutting section.  This cutting section is more 
	advanced form of cutting plane, where the cutting planes intersect and affect each other.
	\param view A pointer to the HBaseView object.
	*/
	static void CreateThreeSidedSection(HBaseView *view);

	/*!This method creates a two sided cutting section.  This cutting section is more 
	advanced form of cutting plane, where the cutting planes intersect and affect each other.
	\param view A pointer to the HBaseView object.
	*/
	static void CreateTwoSidedSection(HBaseView *view);
	static void CreateSlice(HBaseView *view, float relative_thickness = 0.1f);

	/*! Creates and inserts a shell with any of a variety of user options.
		\param points A pointer to an array of HPoint objects which will be used to build the shell.
		\param face_list_length Total number of integers in the face_vertex_indices array.
		\param face_vertex_indices Encoded description of how to connect the points to build the faces of the 
				shell as described in #HC_Insert_Shell.
		\param normals A pointer to an array of HPoint objects which will be used as normal values.
		\param per_face_vertex_normal_indices Encoded description of which normals should be applied. This should 
				be in the same format as the face_vertex_indices.
		\param color_values A pointer to an array of color triples.
		\param per_face_vertex_color_value_indices Encoded description of which color_values should be applied. 
				This should be in the same format as the face_vertex_indices.
		\param color_findices A pointer to an array of floating-index values.
		\param per_face_vertex_color_findex_indices Encoded description of which the color_findices should be 
				applied. This should be in the same format as the face_vertex_indices.
		\param params A pointer to a two-dimensional array of floating point vertex parameters.
		\param per_face_vertex_param_indices Encoded description of which of the params should be applied where. 
				This should be in the same format as the face_vertex_indices.
		\param param_number The number of parameters to be set on each vertex of the shell.  This is the width of the params array.
		\param crease_angle If the angle between the normals of two neighboring faces that share a vertex exceeds this value 
				(in degrees), the vertex will be duplicated to prevent the normals from being averaged.  Default is off.
	*/
	static HC_KEY CreateShellWithFaceSpecificVertexAttributes(
		float points[],
		int face_list_length, int face_vertex_indices[],
		float normals[]=0, int per_face_vertex_normal_indices[]=0,
		float color_values[]=0, int per_face_vertex_color_value_indices[]=0,
		float color_findices[]=0, int per_face_vertex_color_findex_indices[]=0,
		float params[]=0, int per_face_vertex_param_indices[]=0, int param_number=0,
		float crease_angle = 0.f);

#ifndef SWIG
	/*! Creates and inserts a shell with any of a variety of user options.
		\param points A pointer to an array of HPoint objects which will be used to build the shell.
		\param face_list_length Total number of integers in the face_vertex_indices array.
		\param face_vertex_indices Encoded description of how to connect the points to build the faces of the 
				shell as described in #HC_Insert_Shell.
		\param normals A pointer to an array of HPoint objects which will be used as normal values.
		\param per_face_vertex_normal_indices Encoded description of which normals should be applied. This should 
				be in the same format as the face_vertex_indices.
		\param color_values A pointer to an array of color triples.
		\param per_face_vertex_color_value_indices Encoded description of which color_values should be applied. 
				This should be in the same format as the face_vertex_indices.
		\param color_findices A pointer to an array of floating-index values.
		\param per_face_vertex_color_findex_indices Encoded description of which the color_findices should be 
				applied. This should be in the same format as the face_vertex_indices.
		\param params A pointer to a two-dimensional array of floating point vertex parameters.
		\param per_face_vertex_param_indices Encoded description of which of the params should be applied where. 
				This should be in the same format as the face_vertex_indices.
		\param param_number The number of parameters to be set on each vertex of the shell.  This is the width of the params array.
		\param crease_angle If the angle between the normals of two neighboring faces that share a vertex exceeds this value 
				(in degrees), the vertex will be duplicated to prevent the normals from being averaged.  Default is off.
	*/
	static HC_KEY CreateShellWithFaceSpecificVertexAttributes(
		HPoint points[],
		int face_list_length, int face_vertex_indices[],
		HPoint normals[]=0, int per_face_vertex_normal_indices[]=0,
		HPoint color_values[]=0, int per_face_vertex_color_value_indices[]=0,
		float color_findices[]=0, int per_face_vertex_color_findex_indices[]=0,
		float params[]=0, int per_face_vertex_param_indices[]=0, int param_number=0,
		float crease_angle = 0.f);
#endif

	/*! Creates a new shell in the currently open segment with its faces tesselated/colored to represent a binary interpolation
		of the per vertex color parameters of that shell. The original shell is not changed and should
		be deleted after this call in most cases.
  		\param definingShell	The key of shell to interpolate.
		\param generateIsolines Pass true to create isolines as explicit polylines.
		\param algorithm Pass bilinear or an empty string.
		\return The key to the newly created shell.
 	*/	
	static HC_KEY GenerateFEAShell( HC_KEY definingShell, bool generateIsolines, const char *algorithm);

	/*! Creates and inserts into the currently open segment a shell with vertices
	    split where they would otherwise straddle a hard edge.  Most usefully, this
		process allows Gouraud shading to display hard edges.
		\param PointCount The number of valid points in <i>Points</i>.
		\param Points The list of vertices for building the shell.
		\param FaceListLength The number of integers in <i>FaceList</i>.
		\param FaceList Encoded description of how to connect the points to build the faces of the shell as described in #HC_Insert_Shell().
		\param creaseAngle The intersection angle, in degrees, beyond which an edge is considered "hard".
	*/
	static HC_KEY CreateSmoothedShell(
		int PointCount, 
		const HPoint Points[], 
		int FaceListLength, 
		const int FaceList[], 
		float creaseAngle);

	/*! 
	  This is a demonstration function that shows how a developer might use HOOPS to map analysis data to the object on which
	  the analysis was performed.  The mapped data could be results from some analysis that an application performed on a 
	  dataset (e.g.,  FEA thermal analysis of a crankshaft).
  
	  ComputeData looks at a given segment and for each shell in the segment computes data that is then attached to the shell
	  vertices.  In this example, the attached data are simply the y-values of the vertex normal vector. The data are then 
	  mapped into the interval [0.0, 12.0].  These values are directly used for color interpolation and color index interpolation 
	  rendering to provide color contouring.

	  A note on the pShellVertexData parameter: In addition to allocating it prior to the call as an HShellVertexData object, 
	  you must also allocate the array member, i.e., m_pFIndexArray, of this data structure.  It must be able to hold 
	  (number of vertices + number of animation frames/cycles) floats (see MemorySizeForDemoAnimation).
	  \param entity_key Key to the geometry for which one wishes to compute the demo FIndex data.
	  \param pShellVertexData Pointer to HShellVertexData object allocated by application.
	  \param data_cycles Number of steps in a complete colormap or vertex-index cycle.  Must be greater than zero.
	  \return A boolean value indicating success or failure.
	*/
	static bool ComputeFIndexDemoData(HShellVertexData *pShellVertexData, int data_cycles = 30);

	
	/*!
		This function performs the heavy lifting for the partviewer selection operations that compute booleans.
		It does handles all the set-up before and clean-up after calling HC_KCompute_Boolean_Shell.
		Both tool and target should be keys of manifold shells.  If either is not a shell key, the return
		value will be INVALID_KEY.

		\param target This is the target of the boolean operation.
		\param tool This is the tool for the boolean operation.  In the case of subtraction, 
			this shell will be subtracted from the target.  
		\param bool_type This is the type of boolean operation to be performed.  Should be one of the following: 
			BOOLEAN_TYPE_INTERSECT, BOOLEAN_TYPE_SUBTRACT, BOOLEAN_TYPE_UNION.
	*/
	static HC_KEY CreateBoolean(HC_KEY target, HC_KEY tool, int bool_type);

	/*!
		CreateShellFromRotationalSweep functions creates a spun geometry (shell). It takes a profile (closed) and spins it 
		about an axis (about X or Y or Z axis). This api is very similar to HC_Insert_Polyclyliner - which 
		takes points on the axis and the radii of polycylinder at each of this point. The polycylinder is just  
		a polyline spun around, here you can spin a shape. If the profile is not closed, the function will automatically close it. 
		\param nPoints - no of points in the given profile.
		\param profile - Array of points that defines the profile which is required to be spun. These are not the coordinates of actual profile points, but 
		the profile points projected onto the spin axis (similar to polycylinder api) 
		\param radii - Array of distances of each point from axis. 
		\param start_angle - The starting Angle for spin, in radians.
		\param end_angle - The end Angle for spin in radians.
		\param axis -  axis around which the profile will be rotated e.g. pass HUtility::XY for Z axis and so on.
		\param hard - array of booleans passed with points, determines whether to keep the given edge of the profile hard. Points will be duplicated 
		at hard edges for the lighting to appear appropriately. If null is passed, all the edges will be considered as hard edges.
		\param sides - number of sides the spun geometry should have. Imagine the projection of this spun geometry along the spin axis.
		This is the total number of sides this polygon should have. It should be more than two. The quality and number of triangles 
		will depend upon this parameter
		set of points on that edge.
	*/
	static HC_KEY CreateShellFromRotationalSweep(int nPoints, const HPoint profile[], const float radii[], float start_angle, float end_angle, int axis, const bool hard[], int n_sides);


	/*!
		Creates a surface which is the result of sweeping of the given profile along the give path (extrusion).
		The profile is divided into two parts - the inner and the outer. This provides an ability to sweep hollow 
		elements (viz. pipes). For a solid geometry, the inner profile can be null.
		NOTE: If the inner profile is non-null, then the number of points in the inner and outer profiles should be same
		\param nProfilePoints -		number of points on outer profile
		\param pOuterProfilePoints -	array of profile points of outer profile
		\param pInnerProfilePoints -	array of profile points of inner profile (can be NULL for solid core)
		\param nPathPoints -		number of points in the sweep path.
		\param pSweepPathPoints -	array of points on path along which the profile is to be swept
		\param bIsProfileClosed -	If false, the first and last points of the outer and inner profiles will be joined automatically
		NOTE:The profile should always be placed at the start point of the path otherwise the resulting geometry will be incorrect.
		*/
	
	static HC_KEY CreateShellFromPathSweep(int nProfilePoints, const HPoint pOuterProfilePoints[], const HPoint pInnerProfilePoints[], int nPathPoints, const HPoint pSweepPathPoints[], const bool bIsProfileClosed);

private:

	/* 
	  GenerateCuboidPoints creates the vertices of a cuboid based on two input points.
	  NOTE: I have to generate extra points here for each corner so that the 
	  block appear to be flat shaded even in Goroud shading mode which is the only
	  realistic appearance for blocks - Rajesh B (16-Feb-01)
	  \param max A pointer to an HPoint object.
	  \param min A pointer to an HPoint object.
	  \param points A pointer to an array of HPoint objects.  Passed by reference.
	*/
    static void GenerateCuboidPoints( HPoint const * max, HPoint const * in, HPoint points[]);


 
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif





