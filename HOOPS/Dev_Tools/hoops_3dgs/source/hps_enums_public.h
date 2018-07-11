// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HPS_ENUMS_PUBLIC_H
#define HPS_ENUMS_PUBLIC_H

#ifndef HPS_API
#	define HPS_API 
#endif

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4530)	// C++ exception handler used, but unwind semantics are not enabled.
#endif
#include <vector>
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

namespace HPS 
{

	// unimplemented


//Begin Info
/*! The Info class is a concept class for information, warning and error codes. */
class HPS_API Info
{
public:
	/*! \enum Code
	*	Enumerates the types of information, warning and error messages. */
	enum class Code {
		Unknown,			//!< Info type and source unknown.
		Exchange,			//!< Info from Exchange.
		Publish,			//!< Info from Publish.
		Data,				//!< Problem with provided data.
		Key,				//!< Problem with provided key.	
		Geometry,			//!< Problem with provided geometry.
		Database,			//!< Problem with scene.
		Driver,				//!< Problem with driver.
		Internal,			//!< Problem with internal system.
		Memory,				//!< Problem with memory.
		System,				//!< Problem operating environment.
		Unsupported,		//!< Use is unsupported.
	};

private:
	//! Private default constructor to prevent instantiation.
	Info();
};


//Begin Info
/*! The Emergency class is a concept class for Emergency codes. */
class HPS_API Emergency
{
public:
	/*! \enum Code
	*	This describe the types of conditions that can trigger an EmergencyHandler. */
	enum class Code {
		Unknown,			//!< Emergencies type unknown.
		Fatal,				//!< Fatal fault has occurred. Saving the Database is not possible and the EmergencyHandler must abort. After a fatal error, no Visualize functions may be called - to do so will result in a crash or deadlock.    
		SoftMemoryLimit,	//!< Soft memory limit breached. This limit is considered to be breached when Visualize has allocated the amount of memory specified by a previous call to HPS::Database::SetSoftMemoryLimit.
		HardMemoryLimit,	//!< Hard memory limit breached. This limit is considered to be breached when an operating system memory allocation has failed, and indicates that Visualize is not able to allocate memory required for proceeding with normal operation.
	};

private:
	//! Private default constructor to prevent instantiation.
	Emergency();
};


//Begin Window
/*! The Window class is a concept class for window-related enum classes. */
class HPS_API Window
{
public:
	/*! \enum Driver 
	 *	Enumerates the types of supported display drivers. We recommend that you use the Default3D driver type, and provide end-users with the ability to manually select from a range of 3d drivers in a driver-pulldown list. This will allow them to potentially work around graphics-card-specific problems that occur with a specific driver type.*/
	enum class Driver {
		Default3D,			//!< Visualize will perform runtime query of the 3D capabilities of the Operating System and graphics card and automatically choose a display driver.  Under Windows, preference will be given to DirectX API, followed by OpenGL. Under all other platforms, only the OpenGL API is supported. 
		OpenGL,				//!< Fixed-function driver which supports OpenGL 1.X+   Any functionality that requires shaders will not be supported when using this driver. Available on Windows, Linux and Mac OS X.
		OpenGL2,			//!< Shader-based driver which supports OpenGL 2.X+   Available on Windows, Linux, Mac OS X, iOS and Android.
		DirectX9,			//!< Shader-based driver which support DirectX 9.0c   Available on Windows.
		DirectX11,			//!< Shader-based driver which supports DirectX 10.X+   Available on Windows. 
	};

	/*! \enum UpdateStatus 
	 *	Enumerates the statuses that can be returned when performing an update. */
	enum class UpdateStatus { 
		InProgress,			//!< Update is not done yet.
		Completed,			//!< Update was successfully completed.
		TimedOut,			//!< Update timed out before it could be completed.
		Interrupted,		//!< Update was interrupted.
		Failed				//!< Update failed.
	};

	/*! \enum UpdateControl
	Controls the type of update that Visualize will attempt to perform
	*/
	enum class UpdateControl {
		DefaultUpdate,		//!<  sure that the screen is up-to-date with respect to the internal scene graph. Allows Visualize to automatically perform optimizations.

		/*! Redraws the entire scene regardles of whether any changes occurred in the scene-graph which may have triggeed (required) an update.  		It should only be used in specific situations where an update is required due to factors external to the scene-graph.  An example would be 		the situation where GUI event-handling logic encounters a 'GUI window expose' event which Visualize is not aware of, but the scene needs to be 
		forcefully redrawn to repair (via a complete redraw) the exposed portion of the window.
		*/
		CompleteUpdate,		

		RefreshUpdate,		//!< Pulls the back buffer onto the screen. This is a useful way to update the screen after an expose event like when a window that was occluding your scene is moved away.
		
		CompileOnlyUpdate,  //!< Only compiles static trees and display lists.

		/*!
		Will perform a complete update and also rebuild static trees and display lists that are enabled in the scene-graph.   		This should typically only be performed for the first update following a file load, or the effective equivalent, such as loading/creating another model 		inside an existing scene-graph that has the static setting.
		*/
		ExhaustiveUpdate,	
	};

	/*! \enum Mobility
	 *	Enumerates the mobility modes (i.e., resizing and positioning behaviors) for stand-alone windows. */
	enum class Mobility
	{
		Locked,				//!< The stand-alone window cannot be moved or resized.
		Floating,			//!< The stand-alone window can be moved, but cannot be resized.
		FixedRatio,			//!< The stand-alone window can be moved and resized, but the aspect ratio cannot be changed.
		Free				//!< The stand-alone window can be moved and resized, and the aspect ratio can be changed.
	};

	/*! \enum FrameSize
	Dimensions of the Window Frame
	*/
	enum class FrameSize
	{
		Single,				//!< Window Frame Size
		Double				//!< Window Frame Size
	};

	/*! \enum FrameOptions
	Window Frame Positioning
	*/
	enum class FrameOptions
	{
		Inset,				//!< Window Frame Positioning
		Overlay				//!< Window Frame Positioning
	};

private:
	//! Private default constructor to prevent instantiation.
	Window();
};

//Begin Subwindow
/*! This is the Subwindow Class*/
class HPS_API Subwindow
{
public:
	enum class Background
	{
		SolidColor,						//!< Subwindow Background
		Image,							//!< Subwindow Background
		Cubemap,						//!< Subwindow Background
		Blend,							//!< Subwindow Background
		Transparent,					//!< Subwindow Background
		Interactive,					//!< Subwindow Background

		GradientTopToBottom,			//!< Subwindow Background
		GradientBottomToTop,			//!< Subwindow Background
		GradientLeftToRight,			//!< Subwindow Background
		GradientRightToLeft,			//!< Subwindow Background
		GradientTopLeftToBottomRight,	//!< Subwindow Background
		GradientTopRightToBottomLeft,	//!< Subwindow Background
		GradientBottomLeftToTopRight,	//!< Subwindow Background
		GradientBottomRightToTopLeft,	//!< Subwindow Background
	};

	enum class Type
	{
		Standard,			//!< Subwindow Type
		Lightweight			//!< Subwindow Type
	};

	enum class Border
	{
		None,				//!< Subwindow Border
		Inset,				//!< Subwindow Border
		InsetBold,			//!< Subwindow Border
		Overlay,			//!< Subwindow Border
		OverlayBold			//!< Subwindow Border
	};


    /*! \enum RenderingAlgorithm  Specifies the Rendering Algorithm to use for drawing the scene. */
	enum class RenderingAlgorithm
	{
		/*! Instructs the system to use a z-buffering algoirhtm which is of O(n) complexity.
		 *	A hardware z-buffer will be used if the active display driver supports it, otherwise a software-buffer will be used. */
		ZBuffer,			

		/*! Instructs the system to display visible (i.e. unobscured) lines, edges, markers and text. Recommended when rendering an HLR result to any 2D driver device such as hardcopy.
		 *  Faces which are set to be visible are used to obscure the lines, edges, markers and text. It is O(n log n) complexity, and therefore computationally intensive when dealing with a
		 *  large amount of geometry in the scene.  
		 *  This rendering algorithm is not interruptible, and Visualize will perform a full update regardless of the amount of time required.  
		 *  Therefore it is not compatible with time-bound updates which are also used by Visualize to provide fixed-framerate support. */
		HiddenLine,			

		/*! Instructs the system to display visible (i.e. unobscured) lines, edges, markers and text. Recommended when you wish to display accurate HLR result on the screen.
		 *  Faces which are set to be visible are used to obscure the lines, edges, markers and text. It is O(n) complexity and uses a multi-pass, z-buffer rendering approach that takes
		 *  advantage of underlying 3D graphics hardware.  
		 *  This rendering algorithm is not interruptible, and Visualize will perform a full update regardless of the amount of time required.
		 *  Therefore it is not compatible with time-bound updates which are also used by Visualize to provide fixed-framerate support. */
		FastHiddenLine,		

		/*! Instructs the system to use the priority value specified by SegmentKey::SetPriority to determine the drawing order of objects. Objects with the highest priority are drawn on top. 
		 *	Segment-level priority:  Priority values only control the traversal order of sibling segments.
		 *  Geometry-level priority: Priority values applied to primitives are global and are honored irrespective of the priority values of their parent segments.
		 *  Include links: All primitives behind an include link are put into the global list according to the priority set on the include link
		 *  and then sorted relative to each other using their respective priorities.
		 *
		 *  The shader-based DirectX and OpenGL2 drivers have the ability to convert priority values into final z positions, thereby enabling
		 *  hardware-accelerated z-buffer rendering, and thus fast performance. Other drivers will produce the correct result
		 *  but rendering will be much slower since all geometry will be sorted during  scene-graph traversal.
		 *
		 *  This algorithm is primarily intended to control layering within 2D scenes and will cause undesired results when rendering non-planar objects
		 *  such as shells or meshes, that can have some parts overlapping others within the same primitive. */
		Priority			
	};

private:
	Subwindow();
};
//End Subwindow

/*! This is the Style Class */
class HPS_API Style
{
public:

	/*! \enum Type
	Style Type
	*/
	enum class Type
	{
		Segment,		//!< Style Type
		Named			//!< Style Type
	};

	/*! \enum AppendMode
	Style AppendMode
	*/
	enum class AppendMode
	{
		None,		//!< Style Append Mode
		And,		//!< Style Append Mode
		Or			//!< Style Append Mode
	};

private:
	Style();
};


//Begin Shell Attributes
/*! The Shell class is a concept class for shell-related enum classes. */
class HPS_API Shell
{
public:

	/*! \enum Component
	 *	Enumerates the shell components that vertex colors can be applied to. */
	enum class Component
	{
		Faces,		//!< The vertex colors applied to faces.
		Edges,		//!< The vertex colors applied to edges.
		Vertices	//!< The vertex colors applied to vertices.
	};

	/* \enum ToleranceUnits
	 * Enumerates the Tolerance Units used to determine if points can be merged during shell optimization. */
	enum class ToleranceUnits
	{
		ObjectSpace,			//!< Base tolerance on absolute distance in object space.
		FeatureSizePercentage	//!< Base tolerance on pecentage of the smallest non-zero distance between adjacent vertices or faces.
	};

	/* \enum HandednessOptimization
	 * Enumerates the shell optimization options with regards to handedness. */
	enum class HandednessOptimization
	{
		None,					//!< Do nothing with handedness.
		FixHandedness,			//!< Converts the handedness of the minority of the face to the handedness of the majority.
		ReverseHandedness		//!< Reverses the handedness of the shell's faces
	};

	/* \enum RelationResults
	 * Enumerates the results of a shell relation computation. */
	enum class Relation
	{
		On,						//!< The point is on this shell, within the specified tolerance.
		Off,					//!< The point is not on this shell, within the specified tolerance.
		In,						//!< The point is inside this shell.
		Out						//!< The point is outside this shell.
	};

	/* \enum RelationTest
	 * Enumerates the test options for a shell relation computation. */
	enum class RelationTest
	{
		Simple,					//!< The points will only be tested for whether they lie on this shell or not.
		Enclosure,				//!< The points will be tested for whether they lie on this shell, are enclosed within, or are outside it.
		Distance				//!< The shortest distance from each point to this shell will be computed.
	};

private:
	//! Private default constructor to prevent instantiation.
	Shell();
};
//End Shell Attributes

//Begin Mesh Attributes
/*! The Mesh class is a concept class for mesh-related enum classes. */
class HPS_API Mesh
{
public:
	/*! \enum Component
	 *	Enumerates the mesh components that vertex colors can be applied to. */
	enum class Component
	{
		Faces,		//!< The vertex colors applied to faces.
		Edges,		//!< The vertex colors applied to edges.
		Vertices	//!< The vertex colors applied to vertices.
	};

private:
	//! Private default constructor to prevent instantiation.
	Mesh();
};
//End Mesh Attributes

//Begin InfiniteLine Attributes
/*! The InfiniteLine class is a concept class for infinite-line-related enum classes. */
class HPS_API InfiniteLine
{
public:
	/*! \enum Type
	 *	Enumerates the types of infinite line objects. */
	enum class Type
	{
		Line,		//!< Infinite line which  extends infinitely in both directions along a vector.
		Ray			//!< Infinite ray which extends infinitely in one direction along a vector.
	};

private:
	//! Private default constructor to prevent instantiation.
	InfiniteLine();
};
//End InfiniteLine Attributes

/*! The Trim class is a concept class for trim-related enum classes. */
class HPS_API Trim
{
public:
	/*! \enum Type
	 *	Enumerates the types of curves that can be used to define a trim region for a NURBS surface. */
	enum class Type
	{
		Line,			//!< A line will act as the trim.
		Curve			//!< A NURBS curve will act as the trim.
	};

	/*! \enum Operation
	 *	Enumerates the operation the trim region will perform.  This indicates whether the NURBS surface interior to a trim region is kept or removed, and hence, whether the surface
	 *	exterior to a region is removed or kept. */
	enum class Operation
	{
		Keep,			//!< Keep the surface interior to the trim region and remove the surface exterior to the trim region.
		Remove			//!< Remove the surface interior to the trim region and keep the surface exterior to the trim region.
	};
private:
	//! Private default constructor to prevent instantiation.
	Trim();
};

//Begin Spotlight Attributes
/*! The Spotlight class is a concept class for spotlight-related enum classes. */
class HPS_API Spotlight
{
public:
	/*! \enum OuterConeUnits
		 *	Enumerates the units used when specifying an outer-cone size for a spotlight. */
	enum class OuterConeUnits
	{
		/*! The number of degrees from one edge of the cone, through the light-direction vector, to the opposite edge of the cone.  The corresponding size should be in the range
			*	<span class='code'>(0, 360]</span>.  It is possible to make a spotlight shine "behind" itself by specifying an angle larger than 180 degrees, and it is possible to
			*	have a spotlight which shines in all directions by specifying an angle of 360 degrees. */
		Degrees,
		/*! The radius (in object space units) of the circular intersection of the cone with the plane passing through the target point with a normal parallel to the light-direction vector.
			*	The corresponding size should be greater than or equal to 0.  Using this unit, it is not possible to specify a cone that shines "behind" the spotlight,
			*	since an infinitely large radius would correspond to an angular size of 180 degrees. */
		FieldRadius
	};

	/*! \enum InnerConeUnits
	*	Enumerates the units used when specifying an inner-cone size for a spotlight. */
	enum class InnerConeUnits
	{
		/*! The number of degrees from one edge of the cone, through the light-direction vector, to the opposite edge of the cone.  The corresponding size should be in the range
			*	<span class='code'>(0, 360]</span>.  It is possible to make a spotlight shine "behind" itself by specifying an angle larger than 180 degrees, and it is possible to
			*	have a spotlight which shines in all directions by specifying an angle of 360 degrees. */
		Degrees,
		/*! The radius (in object space units) of the circular intersection of the cone with the plane passing through the target point with a normal parallel to the light-direction vector.
			*	The corresponding size should be greater than or equal to 0.  Using this unit, it is not possible to specify a cone that shines "behind" the spotlight,
			*	since an infinitely large radius would correspond to an angular size of 180 degrees. */
		FieldRadius,
		/*! The percentage of the containing outer-cone size.  The corresponding size should be in the range <span class='code'>[0, 100]</span>.  A value of 0 results in a cone size equal to
			*	the light-direction vector itself, a value of 100 results in a cone size identical to the containing outer-cone size. */
		Percent
	};


private:
	//! Private default constructor to prevent instantiation.
	Spotlight();
};
//End Spotlight Attributes




//Begin Cylinder Attributes
/*! The Cylinder class is a concept class for cylinder-related enum classes. */
class HPS_API Cylinder
{
public:
	/*! \enum Component
	 *	Enumerates the cylinder components vertex colors can be applied to. */
	enum class Component
	{
		Faces,				//!< The vertex colors applied to faces.
		Edges,				//!< The vertex colors applied to edges.
	};

	/*! \enum Capping
	 *	Enumerates the type of capping geometry to use for cylinders. */
	enum class Capping
	{
		None,				//!< Neither cylinder end will be capped.
		First,				//!< Only the first cylinder end will be capped.
		Last,				//!< Only the second cylinder end will be capped.
		Both				//!< Both cylinder ends will be capped.
	};

	/*! \enum Orientation
	Cylinder Orientation
	*/
	enum class Orientation
	{
		Default,			//!< Cylinder Orientation
		DefaultRadii,		//!< Cylinder Orientation
		InvertRadii,		//!< Cylinder Orientation
		InvertRadiiOnly,	//!< Cylinder Orientation
		DefaultColors,		//!< Cylinder Orientation
		InvertColors,		//!< Cylinder Orientation
		InvertColorsOnly,	//!< Cylinder Orientation
		InvertAll			//!< Cylinder Orientation
	};

private:
	//! Private default constructor to prevent instantiation.
	Cylinder();
};


/*! The Search class is a concept class for search-related enum classes. */
class HPS_API Search
{
public:
	/*! \enum Space
	 *	Enumeration of which segments to look in when performing a search. */
	enum class Space
	{
		SegmentOnly,				//!< Search the current segment only.
		Subsegments,				//!< Search the current segment and any subsegments.
		SubsegmentsAndIncludes		//!< Search the current segment, any subsegments and any includes.
	};

	/*! \enum Behavior
	 *	Enumeration of the search behaviors. */
	enum class Behavior
	{
		Exhaustive,					//!< Find every instance of the specified types in the specified search space.
		FirstMatch					//!< Find the first instance of any of the specified types in the specified search space.
	};

	
	// IMPORTANT: Remember to add compile time asserts in hpsi.cpp to keep these synchronized to 3DF HK types (if they map to HK types)
	/*! \enum Type
	 *	Enumeration of the types that can be searched for. */

	enum class Type : uint32_t {

		/*	This enum classeration is divided into three parts.

			0xAABBBCCC

			The bits in A encode the most general category of the type (Geometry, Attribute).
			The bits in B encode the subtype (Culling, TextAttribute). Attributes alone have these so far.
			The bits in C encode the specific type (Shell, CullingVectorTolerance, TextAttributeSpacing).

			General category designators should have no bits in B or C.
			Subtype category designators should have bits in A and B but not C.
			Specific category designators should have bits in A and C and optionally B.

			Some types map to an HK type. To get the HK type you should mask out its subtype if it has one, else mask out its general type.
			These types are kept in sync with the 3DF HK types by using Compile-time assets in hpsi.cpp.

			Geometry have no subtype. Most, 'Ordinary Geometry' map to a 3DF HK type.
			The mutants or 'Special Geometry' (some lights and infinite ray) have no HK type to map to.

			Attributes come in a few forms.
			The 'Simple Attributes' map to an HK type and have no subtypes -- they are what they are. ModellingMatrix, Camera, etc.
			The 'Complex Attributes' have subtypes. A handful map to an HK type, but the majority are not.
		*/
		None										= 0x00000000,	//!< No type.
		Everything									= 0x00ffffff,	//!< Search for all attributes, geometry, segments, includes and includers.

		// Segment Stuff (Include & Segment map to HK types)
		Include										= 0x00000042,	//!< Search for include keys.
		Segment										= 0x00000043,	//!< Search for segments.

		Includer									= 0x00000100,	//!< Search for segments which include the current segment being searched.

		// Geometry
		Geometry									= 0x10000000,	//!< Search for all geometry types.

		// Ordinary Geometry (map to HK types) - (HK_* | Geometry)
		CuttingSection								= 0x10000026,	//!< Search for cutting sections.
		Shell										= 0x10000028,	//!< Search for shells.
		Mesh						 				= 0x10000029,	//!< Search for meshes.
		Grid										= 0x1000002a,	//!< Search for grids.
		NURBSSurface								= 0x1000002b,	//!< Search for NURBS surfaces.
		Cylinder									= 0x1000002d,	//!< Search for cylinders.
		Sphere										= 0x1000002e,	//!< Search for spheres.
		Polygon										= 0x1000002f,	//!< Search for polygons.
		Circle										= 0x10000030,	//!< Search for circles.
		//unused									= 0x10000031,
		CircularWedge								= 0x10000032,	//!< Search for circular wedges.
		Ellipse										= 0x10000033,	//!< Search for ellipses.
		Line										= 0x10000034,	//!< Search for lines.
		NURBSCurve									= 0x10000037,	//!< Search for NURBS curves.
		CircularArc									= 0x10000038,	//!< Search for circular arcs.
		EllipticalArc								= 0x10000039,	//!< Search for elliptical arcs.
		InfiniteLine								= 0x1000003a,	//!< Search for infinite lines.
//		Image										= 0x1000003b,	N/A since images live in portfolios
		Marker										= 0x1000003c,	//!< Search for markers.
		Text										= 0x1000003e,	//!< Search for text.
		Reference									= 0x10000040,	//!< Search for geometry references.

		// Special Geometry
		DistantLight								= 0x10000100,	//!< Search for distant lights.
		Spotlight									= 0x10000101,	//!< Search for spotlights.
		InfiniteRay									= 0x10000102,	//!< Search for infinite rays.

		// Attributes
		Attribute									= 0x20000000,	//!< Search for all attributes.
		
		Priority									= 0x20000001,	//!< Search for priority settings.

		// Simple Attributes (map to HK types)
		Material									= 0x2000000a,	//!< Search for material settings.
		Camera										= 0x2000000e,	//!< Search for cameras.
		ModellingMatrix								= 0x20000010,	//!< Search for modelling matrices.
		UserData									= 0x20000021,	//!< Search for user data settings.
		TextureMatrix								= 0x20000023,	//!< Search for texture matrices.

		// Complex Attributes
		Culling										= 0x20001000,	//!< Search for any culling attribute settings.
		CullingBackFace								= 0x20001001,	//!< Search for back face culling settings.
		CullingExtent								= 0x20001002,	//!< Search for extent culling settings.
		CullingVector								= 0x20001003,	//!< Search for vector culling settings.
		CullingVectorTolerance						= 0x20001004,	//!< Search for vector tolerance culling settings.
		CullingFrustum								= 0x20001005,	//!< Search for frustum culling settings.
		CullingDeferralExtent						= 0x20001006,	//!< Search for deferral extent culling settings.

		CurveAttribute								= 0x20002000,	//!< Search for any curve attribute settings.
		CurveAttributeBudget						= 0x20002001,	//!< Search for curve budget settings.
		CurveAttributeContinuedBudget				= 0x20002002,	//!< Search for curve continued budget settings.
		CurveAttributeViewDependent					= 0x20002003,	//!< Search for curve view dependent settings.
		CurveAttributeMaximumDeviation				= 0x20002004,	//!< Search for curve maximum deviation settings.
		CurveAttributeMaximumAngle					= 0x20002005,	//!< Search for curve maximum angle settings.
		CurveAttributeMaximumLength					= 0x20002006,	//!< Search for curve maximum length settings.

		CylinderAttribute							= 0x20003000,	//!< Search for any cylinder attribute settings.
		CylinderAttributeTessellation				= 0x20003001,	//!< Search for cylinder tessellation settings.
		CylinderAttributeOrientation				= 0x20003002,	//!< Search for cylinder orientation settings.

		// EdgeAttributes map to HK types
		EdgeAttribute								= 0x20004000,	//!< Search for any edge attribute settings.
		EdgeAttributePattern						= 0x20004015,	//!< Search for edge pattern settings.
		EdgeAttributeWeight							= 0x20004016,	//!< Search for edge weight settings.

		LightingAttribute							= 0x20005000,	//!< Search for any lighting attribute settings.
		LightingAttributeInterpolation				= 0x20005001,	//!< Search for lighting interpolation settings.

		// LineAttributes map to HK types
		LineAttribute								= 0x20006000,	//!< Search for any line attribute settings.
		LineAttributePattern						= 0x20006019,	//!< Search for line pattern settings.
		LineAttributeWeight							= 0x2000601a,	//!< Search for line weight settings.

		// MarkerAttributes map to HK types
		MarkerAttribute								= 0x20007000,	//!< Search for any marker attribute settings.
		MarkerAttributeSize							= 0x2000701b,	//!< Search for marker size settings.
		MarkerAttributeSymbol						= 0x2000701c,	//!< Search for marker symbol settings.

		SurfaceAttribute							= 0x20008000,	//!< Search for any surface attribute settings.
		SurfaceAttributeBudget						= 0x20008001,	//!< Search for surface budget settings.
		SurfaceAttributeMaximumFacetDeviation		= 0x20008002,	//!< Search for surface maximum facet deviation settings.
		SurfaceAttributeMaximumFacetAngle			= 0x20008003,	//!< Search for surface maximum facet angle settings.
		SurfaceAttributeMaximumFacetWidth			= 0x20008004,	//!< Search for surface maximum facet width settings.
		SurfaceAttributeTrimCurveBudget				= 0x20008005,	//!< Search for surface trim curve budget settings.
		SurfaceAttributeMaximumTrimCurveDeviation	= 0x20008006,	//!< Search for surface maximum trim curve deviation settings.

		Selectability								= 0x20009000,	//!< Search for any selectability settings.
		SelectabilityWindows						= 0x20009001,	//!< Search for window selectability settings.
		SelectabilityEdges							= 0x20009002,	//!< Search for edge selectability settings.
		SelectabilityFaces							= 0x20009003,	//!< Search for face selectability settings.
		SelectabilityLights							= 0x20009004,	//!< Search for light selectability settings.
		SelectabilityLines							= 0x20009005,	//!< Search for line selectability settings.
		SelectabilityMarkers						= 0x20009006,	//!< Search for marker selectability settings.
		SelectabilityVertices						= 0x20009007,	//!< Search for vertex selectability settings.
		SelectabilityText							= 0x20009008,	//!< Search for text selectability settings.

		SphereAttribute								= 0x2000a000,	//!< Search for any sphere attribute settings.
		SphereAttributeTessellation					= 0x2000a001,	//!< Search for sphere tessellation settings.

		Subwindow									= 0x2000b000,	//!< Search for any subwindow settings.
		SubwindowEitherType							= 0x2000b001,	//!< Search for either standard or lightweight subwindow settings.
		SubwindowStandard							= 0x2000b00d,	//!< Search for standard subwindow settings.
		SubwindowLightweight						= 0x2000b002,	//!< Search for lightweight subwindow settings.
		SubwindowBackground							= 0x2000b003,	//!< Search for subwindow background settings.
		SubwindowBorder								= 0x2000b004,	//!< Search for subwindow border settings.
		SubwindowRenderingAlgorithm					= 0x2000b005,   //!< Search for subwindow rendering algorithm.

		// TextAttributeAlignment, Path, & Spacing map to HK types
		TextAttribute								= 0x2000c000,	//!< Search for any text attribute settings.
		TextAttributeAlignment						= 0x2000c01d,	//!< Search for text alignment settings.
		TextAttributeBold							= 0x2000c002,	//!< Search for bold text settings.
		TextAttributeItalic							= 0x2000c003,	//!< Search for italic text settings.
		TextAttributeOverline						= 0x2000c004,	//!< Search for overline text settings.
		TextAttributeStrikethrough					= 0x2000c005,	//!< Search for strikethrough text settings.
		TextAttributeUnderline						= 0x2000c006,	//!< Search for underline text settings.
		TextAttributeSlant							= 0x2000c007,	//!< Search for text slant settings.
		TextAttributeLineSpacing					= 0x2000c008,	//!< Search for text line spacing settings.
		TextAttributeRotation						= 0x2000c00a,	//!< Search for text rotation settings.
		TextAttributeExtraSpace						= 0x2000c00b,	//!< Search for text extra space settings.
		TextAttributeGreeking						= 0x2000c00c,	//!< Search for text greeking settings.
		TextAttributeSizeTolerance					= 0x2000c00d,	//!< Search for text size tolerance settings.
		TextAttributeSize							= 0x2000c00e,	//!< Search for text size settings.
		TextAttributeFont							= 0x2000c00f,	//!< Search for text font settings.
		TextAttributeTransform						= 0x2000c010,	//!< Search for text transform settings.
		TextAttributeRenderer						= 0x2000c011,	//!< Search for text renderer settings.
		TextAttributePreference						= 0x2000c012,	//!< Search for text preference settings.
		TextAttributePath							= 0x2000c01f,	//!< Search for text path settings.
		TextAttributeSpacing						= 0x2000c020,	//!< Search for text spacing settings.

		Transparency								= 0x2000d000,	//!< Search for any transparency settings.
		TransparencyMethod							= 0x2000d001,	//!< Search for transparency method settings.
		TransparencyAlgorithm						= 0x2000d002,	//!< Search for transparency algorithm settings.
		TransparencyDepthPeelingLayers				= 0x2000d004,	//!< Search for depth peeling layers settings.
		TransparencyDepthPeelingMinimumArea			= 0x2000d005,	//!< Search for depth peeling minimum area settings.
		TransparencyDepthWriting					= 0x2000d006,	//!< Search for depth writing settings.

		Visibility									= 0x2000e000,	//!< Search for any visibility settings.
		VisibilityCuttingSections					= 0x2000e001,	//!< Search for cutting section visibility settings.
		VisibilityCutEdges							= 0x2000e002,	//!< Search for cut edge visibility settings.
		VisibilityCutFaces							= 0x2000e003,	//!< Search for cut face visibility settings.
		VisibilityWindows							= 0x2000e004,	//!< Search for window visibility settings.
		VisibilityText								= 0x2000e005,	//!< Search for text visibility settings.
		VisibilityLines								= 0x2000e006,	//!< Search for line visibility settings.
		VisibilityEdgeLights						= 0x2000e007,	//!< Search for edge light visibility settings.
		VisibilityMarkerLights						= 0x2000e008,	//!< Search for marker light visibility settings.
		VisibilityFaceLights						= 0x2000e009,	//!< Search for face light visibility settings.
		VisibilityGenericEdges						= 0x2000e00a,	//!< Search for generic edge visibility settings.
		VisibilityHardEdges							= 0x2000e00b,	//!< Search for hard edge visibility settings.
		VisibilityAdjacentEdges						= 0x2000e00c,	//!< Search for adjacent edge visibility settings.
		VisibilityInteriorSilhouetteEdges			= 0x2000e00d,	//!< Search for interior silhouette edge visibility settings.
		VisibilityShadowEmitting					= 0x2000e00e,	//!< Search for shadow emitting visibility settings.
		VisibilityShadowReceiving					= 0x2000e00f,	//!< Search for shadow receiving visibility settings.
		VisibilityShadowCasting						= 0x2000e010,	//!< Search for shadow casting visibility settings.
		VisibilityMarkers							= 0x2000e011,	//!< Search for marker visibility settings.
		VisibilityVertices							= 0x2000e012,	//!< Search for vertex visibility settings.
		VisibilityFaces								= 0x2000e013,	//!< Search for face visibility settings.
		VisibilityPerimeterEdges					= 0x2000e014,	//!< Search for perimeter edge visibility settings.
		VisibilityNonCulledEdges					= 0x2000e015,	//!< Search for non-culled edge visibility settings.
		VisibilityMeshQuadEdges						= 0x2000e016,	//!< Search for mesh quad edge visibility settings.

		VisualEffects								= 0x2000f000,	//!< Search for any visual effects settings.
		VisualEffectsPostProcessEffectsEnabled		= 0x2000f001,	//!< Search for visual effects post-process effects enabled settings.
		VisualEffectsAntiAliasing					= 0x2000f002,	//!< Search for visual effects anti-aliasing settings.
		VisualEffectsShadowMaps						= 0x2000f003,	//!< Search for visual effects shadow maps settings.
		VisualEffectsSimpleShadow					= 0x2000f004,	//!< Search for visual effects simple shadow settings.
		VisualEffectsSimpleShadowPlane				= 0x2000f005,	//!< Search for visual effects simple shadow plane settings.
		VisualEffectsSimpleShadowLightDirection		= 0x2000f006,	//!< Search for visual effects simple shadow light direction settings.
		VisualEffectsSimpleShadowColor				= 0x2000f007,	//!< Search for visual effects simple shadow color settings.
		VisualEffectsSimpleReflection				= 0x2000f008,	//!< Search for visual effects simple reflection settings.
		VisualEffectsSimpleReflectionPlane			= 0x2000f009,	//!< Search for visual effects simple reflection plane settings.
		VisualEffectsSimpleReflectionVisibility		= 0x2000f00a,	//!< Search for visual effects simple reflection visibility settings.

		Performance									= 0x20010000,	//!< Search for any performance settings.
		PerformanceDisplayLists						= 0x20010001,	//!< Search for performance display lists settings.
		PerformanceStaticModel						= 0x20010002,	//!< Search for performance static model settings.
		StaticModelSegment							= 0x40000043,	//!< Search for static model segments.

		// DrawingAttributeWorldHandedness map to HK types
		DrawingAttribute							= 0x20011000,	//!< Search for any drawing attribute settings.
		DrawingAttributePolygonHandedness			= 0x20011001,	//!< Search for drawing attribute polygon handedness settings.
		DrawingAttributeDepthRange					= 0x20011002,	//!< Search for drawing attribute depth range settings.
		DrawingAttributeFaceDisplacement			= 0x20011003,	//!< Search for drawing attribute face displacement settings.
		DrawingAttributeGeneralDisplacement			= 0x20011004,	//!< Search for drawing attribute general displacement settings.
		DrawingAttributeVertexDisplacement			= 0x20011005,	//!< Search for drawing attribute vertex displacement settings.
		DrawingAttributeOverlay						= 0x20011006,	//!< Search for drawing attribute overlay settings.
		DrawingAttributeDeferral					= 0x20011007,	//!< Search for drawing attribute deferral settings.
		DrawingAttributeWorldHandedness				= 0x20011018,	//!< Search for drawing attribute world handedness settings.
		DrawingAttributeClipRegion					= 0x20011019,	//!< Search for drawing attribute clip region settings.

		HiddenLineAttribute							= 0x20012000,	//!< Search for any hidden line attribute settings.
		HiddenLineAttributeColor					= 0x20012001,	//!< Search for hidden line color settings.
		HiddenLineAttributeDimFactor				= 0x20012002,	//!< Search for hidden line dim factor settings.
		HiddenLineAttributeFaceDisplacement			= 0x20012003,	//!< Search for hidden line face displacement settings.
		HiddenLineAttributeLinePattern				= 0x20012005,	//!< Search for hidden line pattern settings.
		HiddenLineAttributeRenderFaces				= 0x20012006,	//!< Search for hidden line render faces settings.
		HiddenLineAttributeRenderText				= 0x20012007,	//!< Search for hidden line render text settings.
		HiddenLineAttributeAlgorithm				= 0x20012008,	//!< Search for hidden line algorithm settings.
		HiddenLineAttributeSilhouetteCleanup		= 0x20012009, 	//!< Search for hidden line silhouette cleanup settings.
		HiddenLineAttributeVisibility				= 0x2001200a,	//!< Search for hidden line visibility settings.
		HiddenLineAttributeWeight					= 0x2001200b,	//!< Search for hidden line weight settings.
		HiddenLineAttributeTransparencyCutoff		= 0x2001200c,	//!< Search for hidden line transparency cutoff settings.

		SegmentStyle								= 0x20013001,	//!< Search for any segment style settings.
		NamedStyle									= 0x20013002,	//!< Search for any named style settings.
		MaterialPalette								= 0x20013003,	//!< Search for any material palette settings.
		Portfolio									= 0x20013004,	//!< Search for any portfolio settings.

		ContourLine									= 0x20014000,	//!< Search for any contour line settings.
		ContourLineVisibility						= 0x20014001,	//!< Search for contour line visibility settings.
		ContourLinePosition							= 0x20014002,	//!< Search for contour line position settings.
		ContourLineColor							= 0x20014003,	//!< Search for contour line color settings.
		ContourLinePattern							= 0x20014004,	//!< Search for contour line pattern settings.
		ContourLineWeight							= 0x20014005,	//!< Search for contour line weight settings.
		ContourLineLighting							= 0x20014006,	//!< Search for contour line lighting settings.

		Condition									= 0x20015000,	//!< Search for any condition settings.

		Bounding									= 0x20016000,	//!< Search for any bounding settings.
		BoundingVolume								= 0x20016001,	//!< Search for bounding volume settings.
		BoundingExclusion							= 0x20016002,	//!< Search for bounding exclusion settings.
		
		AttributeLock								= 0x20017000,	//!< Search for any attribute lock settings.
		AttributeLockSetting						= 0x20017001,	//!< Search for attribute lock setting settings.
		AttributeLockSubsegmentOverride				= 0x20017002,	//!< Search for attribute lock subsegment override settings.

		TransformMask								= 0x20018000,	//!< Search for any transform mask settings.
		TransformMaskCamera							= 0x20018001,	//!< Search for any transform mask camera settings.
		TransformMaskCameraTranslation				= 0x20018002,	//!< Search for transform mask translation settings.
		TransformMaskCameraScale					= 0x20018003,	//!< Search for transform mask scale settings.
		TransformMaskCameraOffset					= 0x20018004,	//!< Search for transform mask offset settings.
		TransformMaskCameraRotation					= 0x20018005,	//!< Search for transform mask rotation settings.
		TransformMaskCameraPerspectiveScale			= 0x20018006,	//!< Search for transform mask perspective scale settings.
		TransformMaskCameraProjection				= 0x20018007,	//!< Search for transform mask projection settings.
		TransformMaskModellingMatrix				= 0x20018008,	//!< Search for any transform mask modelling matrix settings.
		TransformMaskModellingMatrixTranslation		= 0x20018009,	//!< Search for transform mask modelling matrix translation settings.
		TransformMaskModellingMatrixScale			= 0x20018010,	//!< Search for transform mask modelling matrix scale settings.
		TransformMaskModellingMatrixOffset			= 0x20018011,	//!< Search for transform mask modelling matrix offset settings.
		TransformMaskModellingMatrixRotation		= 0x20018012,	//!< Search for transform mask modelling matrix rotation settings.

		ColorInterpolation							= 0x20019000,	//!< Search for any color interpolation settings.
		ColorInterpolationFaceColor					= 0x20019001,	//!< Search for color interpolation face color settings.
		ColorInterpolationEdgeColor					= 0x20019002,	//!< Search for color interpolation edge color settings.
		ColorInterpolationMarkerColor				= 0x20019003,	//!< Search for color interpolation marker color settings.
		ColorInterpolationFaceIndex					= 0x20019004,	//!< Search for color interpolation face index settings.
		ColorInterpolationEdgeIndex					= 0x20019005,	//!< Search for color interpolation edge index settings.
		ColorInterpolationMarkerIndex				= 0x20019006,	//!< Search for color interpolation marker index settings.

		CuttingSectionAttribute						= 0x2001a000,	//!< Search for any cutting section attribute settings.
		CuttingSectionAttributeCuttingLevel			= 0x2001a001,	//!< Search for cutting section cutting level settings.
		CuttingSectionAttributeCappingLevel			= 0x2001a002,	//!< Search for cutting section capping level settings.
		CuttingSectionAttributeMaterialPreference	= 0x2001a003,	//!< Search for cutting section material preference settings.

	};
	// IMPORTANT: Remember to add compile time asserts in hpsi.cpp to keep these synchronized to HK types (if they are map to HK types)

private:
	//! Private default constructor to prevent instantiation.
	Search();
};

/*! The Material class is a concept class for material-related enum classes. */
class HPS_API Material
{
public:
	/*! \enum Type 
	 *	Enumerates the types of materials that can be set on various Visualize entities. */
	enum class Type
	{
		InvalidMaterialIndex	= -1,		//!< An invalid material index was set.
		Nothing 				=  0,		//!< No material was set.
		FullMaterial,						//!< A full material was set.  The MaterialKit is valid.
		ExplicitRGBColor,					//!< An RGB color was set.  The RGBColor is valid.
		ExplicitRGBAColor,					//!< An RGBA color was set.  The RGBAColor is valid.
		MaterialIndex,						//!< A material index was set.  The material index (float value) is valid.
		TextureName,						//!< An unmodulated texture was set.  The texture name is valid.
		ModulatedTexture,					//!< A modulated texture was set.  Both the RGBColor (or RGBAColor) and texture name are valid.
		CubeMapName,						//!< An unmodulated cube map texture was set.  The cube map texture name is valid.
		ModulatedCubeMap,					//!< A modulated cube map was set.  Both the RGBColor (or RGBAColor) and cube map texture name are valid.
		GlossValue,							//!< A gloss value was set.  The gloss (float value) is valid.
		DiffuseChannelAlpha					//!< An alpha value was set.  The alpha (float value) is valid.
		
	};

	/*! \enum Channel 
	Material Channel
	*/
	enum class Channel
	{
		DiffuseColor = 1,		//!< Material Channel
		DiffuseTexture = 2,		//!< Material Channel
		Specular = 3,			//!< Material Channel
		Emission = 4,			//!< Material Channel
		Transmission = 5,		//!< Material Channel
		Mirror = 6,				//!< Material Channel
		Bump = 7,				//!< Material Channel
		EnvironmentTexture = 8,	//!< Material Channel
		EnvironmentCubeMap = 9,	//!< Material Channel
		Gloss = 10				//!< Material Channel
	};

	/*! The Material Color Class*/
	class HPS_API Color 
	{
	public:	
		/*! \enum Channel 
		Material Color Channel
		*/
		enum class Channel 
		{
			DiffuseColor = 1,				//!< Material Color Channel
			Specular = 3,					//!< Material Color Channel
			Emission = 4,					//!< Material Color Channel
			Mirror	 = 6					//!< Material Color Channel
		};

	private:
		//! Private default constructor to prevent instantiation.
		Color();
	};
	
	/*! The Material::Texture class is a concept class for texture-related enum classes. */
	class HPS_API Texture 
	{
	public:
		/*! \enum Channel 
		Material Texture Channel
		*/
		enum class Channel 
		{
			DiffuseTexture = 2,			//!< Material Texture Channel
			Specular = 3,				//!< Material Texture Channel
			Emission = 4,				//!< Material Texture Channel
			Transmission = 5,			//!< Material Texture Channel
			Mirror = 6,					//!< Material Texture Channel
			Bump = 7,					//!< Material Texture Channel
			EnvironmentTexture = 8,		//!< Material Texture Channel
			EnvironmentCubeMap = 9		//!< Material Texture Channel
		};

		/*! \enum Parameterization
		 *	Enumeration of the parameterization sources for textures. */
		enum class Parameterization 
		{
			/*! Use a cylindrical mapping for the texture. */
			Cylinder,
			/*! Texture Parameterization. */
			PhysicalReflection,
			/*! Use the <span class='code'>(x,y,z)</span> coordinates for the object before transforms have been applied for the texture parameters. */
			Object,
			/*! Use the natural mapping for meshes, NURBS surfaces and shells for the texture.
			 * 	For meshes, textures will be stretched in the range <span class='code'>[0,1]</span>.
			 * 	For NURBS surfaces, textures will be mapped in the range <span class='code'>[0,(control point count - degree)]</span>.
			 * 	For shells, textures will be mapped such that <span class='code'>u=x+z</span> and <span class='code'>v=x+y</span>
			 * 	where <span class='code'>(x,y,z)</span> is in object space. */
			NaturalUV,
			/*! Texture Parameterization. */
			ReflectionVector,
			/*! Texture Parameterization. */
			SurfaceNormal,
			/*! Use a spherical mapping for the texture. */
			Sphere,
			/*! Use the mapping explicitly defined on the geometry for the texture. */
			UV,
			/*! Use the <span class='code'>(x, y, z)</span> coordinates for the object after transforms have been applied for the texture parameters. */
			World
		};

		/*! \enum Tiling
		 *	Enumeration of the tiling modes for textures. */
		enum class Tiling 
		{
			/*! The texture will get mapped normally for parameters in the range <span class='code'>[0,1]</span>, but parameters less than <span class='code'>0</span>
			 *	will be clamped to <span class='code'>0</span> and parameters greater than <span class='code'>1</code> will be clamped to <span class='code'>1</span>. */
			Clamp,
			/*! The texture will get mapped repeatedly in a modulo fashion. */
			Repeat,
			/*! The texture will get mapped repeatedly in a modulo fashion.  Every other modulo will also be inverted. */
			Reflect,
			/*! The texture will get mapped normally for parameters in the range <span class='code'>[0,1]</span>, but parameters outside that range will act as if the
			 *	texture at that location is transparent. */
			Trim
		};

		/*! \enum Interpolation
		 *	Enumeration of the various interpolation filters for textures.  These are used when the texture needs to be magnified. */
		enum class Interpolation 
		{
			/*! The texture data from the four closest texture samples to a texture coordinate will be blended via a weighted average. */
			Bilinear,

			/*! The texture coordinate will map to the closest texture samples. */
			InterpolationOff
		};

		/*! \enum Decimation 
		 *	Enumeration of the various decimation filters for textures.  These are used when the texture needs to minimized. */
		enum class Decimation 
		{
			/*! The texture will be down-sampled to non-square image sizes which will get used for sampling and filtering based on the
			 *	angle between the normal of a textured surface and the view vector. */
			Anisotropic,
			/*! The texture will be down-sampled to square, power-of-two-sized images which will get used for sampling and filtering. */
			Mipmap,
			/*! No down-sampling will be performed.  Data will be retrieved via the specified Texture::Interpolation filter. */
			DecimationOff
		};

		/*! \enum ChannelMapping 
		Material Texture ChannelMapping
		*/
		enum class ChannelMapping 
		{
			Red,		//!< Material Texture ChannelMapping
			Green,		//!< Material Texture ChannelMapping
			Blue,		//!< Material Texture ChannelMapping
			Alpha,		//!< Material Texture ChannelMapping
			Zero,		//!< Material Texture ChannelMapping
			One,		//!< Material Texture ChannelMapping
			Luminance	//!< Material Texture ChannelMapping
		};

	private:	
		//! Private default constructor to prevent instantiation.
		Texture();
	};
private:
	//! Private default constructor to prevent instantiation.
	Material();
};

class HPS_API PostProcessEffects
{
public:
	class HPS_API AmbientOcclusion
	{
	public:
		/*! \enum Quality 
		Ambient Occlusion Quality
		*/
		enum class Quality
		{
			Fastest,			//!< Ambient Occulsion Quality
			Nicest				//!< Ambient Occulsion Quality
		};

	private:
		AmbientOcclusion();
	};

	class HPS_API Bloom
	{
	public:
		/*! \enum Shape 
		Bloom Shape
		*/
		enum class Shape
		{
			Star,			//!< Bloom Shape
			Radial			//!< Bloom Shape
		};

	private:
		Bloom();
	};

private:
	PostProcessEffects() {}
};

class HPS_API Performance
{
public:
	/*! \enum DisplayLists 
	 *	Display lists are a GPU hardware caching technology which can greatly improve rendering performance.
	 *	The underlying implementation depends on the 3D driver being used.*/
	enum class DisplayLists
	{
		/*! Display Lists will not be used for rendering the scene. */
		NoDisplayLists,

		/*! Display Lists will be created on a per-geometry basis.
		 *	If the geometry is modified, the display list will be regenerated. */
		GeometryDisplayLists,

		/*! Display Lists will be created on a per-segment basis.
		 *	If the geometry is modified, the display list will be regenerated. */
		SegmentDisplayLists
	};

	/*! \enum StaticModel
	 *	StaticModel is a technique used for improving rendering performance.
	 *	This setting tells the system that the segment tree affected by the attribute will remain 'static' or unchanging.
	 *  The system will create an internal, optimized segment tree which is used for rendering in lieu of the normal tree.
	 *	The original segment tree is untouched and can be used normally.
	 *
	 *	If changes are made in a part of the segment tree that is subject to the static model attribute, the internal tree will be regenerated, with a few exceptions:
	 *		If geometry is deleted or edited, the internal tree will not need to be regenerated. */
	enum class StaticModel
	{

		NoStaticModel,				//!< No static model will be used, rendering will be done from the segment tree.
		AttributeStaticModel,		//!< An optimized segment tree will be used for rendering.   The tree will be sorted by attributes.  
		AttributeSpatialStaticModel	//!< An optimized segment tree will be used for rendering.   The tree will be sorted based on both attributes and spatial locations of objects.  This is particularly useful for very large, spatially dispersed models.
	};

	
private:
	Performance() {}
};


class HPS_API AttributeLock
{
public:

	/*! \enum Type The types of attributes that can be locked */
	enum class Type : uint32_t
	{
		Everything							= 0x01000000,

		Visibility							= 0x02000000,

		VisibilityCuttingSections,
		VisibilityCutEdges,
		VisibilityCutFaces,
		VisibilityWindows,
		VisibilityText,
		VisibilityLines,
		VisibilityEdgeLights,
		VisibilityMarkerLights,
		VisibilityFaceLights,
		VisibilityGenericEdges,
		VisibilityHardEdges,
		VisibilityAdjacentEdges,
		VisibilityInteriorSilhouetteEdges,
		VisibilityShadowEmitting,
		VisibilityShadowReceiving,
 		VisibilityShadowCasting,
		VisibilityMarkers,
		VisibilityVertices,
		VisibilityFaces,
		VisibilityPerimeterEdges,
		VisibilityNonCulledEdges,
		VisibilityMeshQuadEdges,
		VisibilityCutGeometry,
		VisibilityEdges,
		VisibilityGeometry,
		VisibilityLights,
		VisibilityShadows,

		Material							= 0x03000000,

		MaterialGeometry,
		MaterialCutGeometry,

		MaterialAmbientLightUpColor,
		MaterialAmbientLightDownColor,
		MaterialAmbientLightColor,

		MaterialWindowColor,
		MaterialWindowContrastColor,

		MaterialLightColor,
		MaterialLineColor,
		MaterialMarkerColor,
		MaterialTextColor,
		MaterialCutEdgeColor,

		MaterialVertex,
		MaterialVertexDiffuse,
		MaterialVertexDiffuseColor,
		MaterialVertexDiffuseAlpha,
		MaterialVertexDiffuseTexture,
		MaterialVertexSpecular,
		MaterialVertexMirror,
		MaterialVertexTransmission,
		MaterialVertexEmission,
		MaterialVertexEnvironment,
		MaterialVertexBump,
		MaterialVertexGloss,

		MaterialEdge,
		MaterialEdgeDiffuse,
		MaterialEdgeDiffuseColor,
		MaterialEdgeDiffuseAlpha,
		MaterialEdgeDiffuseTexture,
		MaterialEdgeSpecular,
		MaterialEdgeMirror,
		MaterialEdgeTransmission,
		MaterialEdgeEmission,
		MaterialEdgeEnvironment,
		MaterialEdgeBump,
		MaterialEdgeGloss,

		MaterialFace,
		MaterialFaceDiffuse,
		MaterialFaceDiffuseColor,
		MaterialFaceDiffuseAlpha,
		MaterialFaceDiffuseTexture,
		MaterialFaceSpecular,
		MaterialFaceMirror,
		MaterialFaceTransmission,
		MaterialFaceEmission,
		MaterialFaceEnvironment,
		MaterialFaceBump,
		MaterialFaceGloss,

		MaterialBackFace,
		MaterialBackFaceDiffuse,
		MaterialBackFaceDiffuseColor,
		MaterialBackFaceDiffuseAlpha,
		MaterialBackFaceDiffuseTexture,
		MaterialBackFaceSpecular,
		MaterialBackFaceMirror,
		MaterialBackFaceTransmission,
		MaterialBackFaceEmission,
		MaterialBackFaceEnvironment,
		MaterialBackFaceBump,
		MaterialBackFaceGloss,

		MaterialFrontFace,
		MaterialFrontFaceDiffuse,
		MaterialFrontFaceDiffuseColor,
		MaterialFrontFaceDiffuseAlpha,
		MaterialFrontFaceDiffuseTexture,
		MaterialFrontFaceSpecular,
		MaterialFrontFaceMirror,
		MaterialFrontFaceTransmission,
		MaterialFrontFaceEmission,
		MaterialFrontFaceEnvironment,
		MaterialFrontFaceBump,
		MaterialFrontFaceGloss,

		MaterialCutFace,
		MaterialCutFaceDiffuse,
		MaterialCutFaceDiffuseColor,
		MaterialCutFaceDiffuseAlpha,
		MaterialCutFaceDiffuseTexture,
		MaterialCutFaceSpecular,
		MaterialCutFaceMirror,
		MaterialCutFaceTransmission,
		MaterialCutFaceEmission,
		MaterialCutFaceEnvironment,
		MaterialCutFaceBump,
		MaterialCutFaceGloss,

		Camera							= 0x04000000,
	};

private:
	AttributeLock() {}
};

class HPS_API Drawing
{
public:
	/*! \enum Handedness 
	 *	Handedness is used to define the front face of a polygon as well as the orientation of the z-axis relative to the x-y plane. */
	enum class Handedness
	{
		NoHandedness,			//!< No Handedness
		Left,					//!< Left Handed
		Right					//!< Right Handed
	};

	/*! \enum Overlay
	 *	Enumeration of the various overlay modes. */
	enum class Overlay
	{
		/*! No overlay will be used.  If geometry is moved, edited or highlighted, a full screen redraw will be triggered on the subsequent update. */
		NoOverlay,
		/*! Any geometry in a segment with this mode set will be redrawn on each update and it will appear on top of the scene regardless of whether it should appear behind other geometry.
		 *	This is the most efficient overlay mode. */
		NormalOverlay,
		/*! Any geometry in a segment with this mode set will be drawn on each update and it will be positioned correctly with respect to other geometry within the scene.
		 *	This is the most accurate overlay mode, but is not as efficient as NormalOverlay. */
		OverlayWithZValues,

		/*! This setting is useful when you are applying a transparent highlight style to overlay geometry.
		 * This setting suppresses the drawing of the geometry, instead drawing the highlight itself in its place.
		 * If you are not using a transparent highlight style, this setting has no visible effect. This setting is
		 * slightly more computationally expensive than the other overlay settings. */
		InPlace,
	};

	/*! \enum ClipOperation
	 * 	Enumerates the clip region operation types. */
	enum class ClipOperation
	{
		Keep,		//!< The geometry inside the clip region is drawn. Everything outside of it is clipped.
		Remove,		//!< The geometry outside the clip region is drawn. Everything inside of it is clipped.
	};

	/*! \enum ClipSpace
	 * 	Enumerates the coordinate spaces types for clip regions. */
	enum class ClipSpace
	{
		Window,		//!< The clip region is specified in window coordinates.
		World,		//!< The clip region is specified in world coordinated.
		Object,		//!< The clip region is specified in object coordinated.
	};

private:
	Drawing() {}
};


class HPS_API HiddenLine
{
public:
	/*! \enum Algorithm
	Specifies the sorting algorithm that should be used to sort the geometry which has been drawn as a result of setting SetRenderFaces to true. 
	*/
	enum class Algorithm
	{
		None,	//!< No sorting of faces will be performed
		ZBuffer,//!< Utilizes a z-buffering algorithm, defaulting to hardware z-buffer
		ZSort,	//!< Utilizes a z-sorting algorithm which sorts faces based on the z-value of each triangles centroid
	};

private:
	HiddenLine() {}
};


/*! The Selection class is a concept class for selection-related enum classes. */
class HPS_API Selection
{
public:
	/*! \enum Level
	 *	Enumeration of the various selection levels. */
	enum class Level
	{
		Segment,			//!< Return the owning segment of the selected geometry entities in SelectionResults.
		Entity,				//!< Return the selected geometry entities in SelectionResults.
		Subentity,			//!< Return the selected geometry entities and the associated faces, edges and vertices for shells or meshes in SelectionResults.
	};

	/*! \enum Algorithm 
	 *	Enumeration of the various selection algorithms. */
	enum class Algorithm
	{
		Visual,				//!< This algorithm will post-process the SelectionResults to remove items which are obscured by other objects in the scene.
		Analytic			//!< This algorithm will perform an analytic world space selection testing, and will not post-process the SelectionResults based on obscuration.  (Therefore, items which are not current visible can still be returned in the SelectionResults)
	};

	/*! \enum Granularity 
	 *	Enumeration of the various selection granularities. */
	enum class Granularity
	{
		General,			//!< Use the most efficient selection determination for curves, edges and lines.
		Detailed			//!< Use the most accurate selection determination for curves, edges and lines.
	};

private:
	//! Private default constructor to prevent instantiation.
	Selection();
};


//////////////////////////////////////////////////////////////////////
///////////////////////////// Attributes /////////////////////////////
//////////////////////////////////////////////////////////////////////

//Begin Camera
/*! This is the Camera Class*/
class HPS_API Camera
{
public:
/*!
  \addtogroup Camera_Enumerations
  @{
*/

	/*! \enum Type 
	Camera Type
	*/
	enum class Projection {
		Default=1,				//!< Camera Type
		Perspective,			//!< Camera Type
		Orthographic,			//!< Camera Type
		Stretched,				//!< Camera Type
	};
/*!  @}  */
	
private:
	Camera();
};


//Begin Selectability
/*! This is the Selectability Class*/
class HPS_API Selectability
{
public:
	/*! \enum Value
	Selectability Value
	*/
	enum class Value
	{
		Off=0,		//!< Not selectable.
		On,			//!< Selectable if visible.
		ForcedOn	//!< Selectable even if not visible. 
	};

private:
	Selectability();
};

// Begin Transparency
/*! This is the Transparency Class */
class HPS_API Transparency
{
public:
	/*! \enum Style 
	Transparency Style
	*/
	enum class Method
	{
		Off,			//!< This will disable transparency, overriding all other transparency settings and forcing geometry to be opaque.
		Blended,		//!< The transparent object will be blended with the color of the underlying object.
		ScreenDoor		//!< This is a pseudo-transparency algorithm that sacrifices visual quality to improve performance.
	};

	/*! \enum Algorithm 
	Transparency Algorithm
	*/
	enum class Algorithm
	{
		NoSorting,		//!< No sorting will be performed, but transparent objects will be deferred (drawn on top).  If overlapping transparent objects are present the rendering will not be accurate.
		Painters,		//!< Will produce a completely accurate rendering of transparent objects which are overlapping other transparenty objects, but tends to be slow.
		ZSortNicest,	//!< Sorts every transparent triangle in a shell by its mid-point.  This algorithm can result in rendering artifacts at locations where transparent objects intersect with one another.
		ZSortFastest,	//!< Sorts transparent tristrips in a shell by the mid-point of the entire tristrip.  This algorithm can result in rendering artifacts at locations where transparent objects intersect with one another.
		DepthPeeling	//!< Non-sorting technique based on multi-pass drawing, and leverages hardware acceleration to quickly produce an accurate rendering. Will give varying accuracy and performance results based on the number of 'layers' that are specified by the user.  
	};

	/*! \enum AreaUnits 
	Transparency Area Units
	*/
	enum class AreaUnits
	{
		Percent,	//!< Transparency Area Units
		Pixels		//!< Transparency Area Units
	};

private:
	Transparency() {}
};



//Begin Marker Attributes
/*! This is the Marker Class*/
class HPS_API Marker
{
public:
	/*! \enum SizeUnits 
	Marker Size Units
	*/
	enum class SizeUnits
	{
		ScaleFactor,			//!< A unitless linear scaling factor.  A value of 2.0 will cause markers to be rendered twice as large.  A value of 1.0 will result in a visually pleasing device-dependent marker size that is approximately 3% of the height of the outer window.  A value of 0 will result in a single pixel marker for display-devices, or the smallest size supported by any other device.
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels					//!< Number of pixels.

	};
private:
	Marker();
};

//Begin Sphere Attributes
/*! This is the Sphere Class*/
class HPS_API Sphere
{
public:
	
private:
	Sphere();
};


//Begin Lighting Attributes
/*! This is the Lighting Class*/
class HPS_API Lighting 
{
public:
	/*! \enum Interpolation Algorithm 
	Lighting Interpolation Algorithm
	*/
	enum class InterpolationAlgorithm 
	{
		Phong,		//!< Lighting Interpolation Algorithm
		Gouraud,	//!< Lighting Interpolation Algorithm
		Flat		//!< Lighting Interpolation Algorithm
	};

private:
		Lighting();
};

/*! The Text class is a concept class for text-related enum classes. */
class HPS_API Text
{
public:
	/*! \enum Alignment
	 *	Enumeration of the alignments used for text string.  These define what the insertion point for a text string is measured with respect to. */
	enum class Alignment
	{
		TopLeft,					//!< Insertion point is the top left corner of the text string.
		CenterLeft,					//!< Insertion point is the center of the left side of the text string.
		BottomLeft,					//!< Insertion point is the bottom left corner of the text string.
		TopCenter,					//!< Insertion point is the center of the top side of the text string.
		Center,						//!< Insertion point is the vertical and horizontal center of the text string.
		BottomCenter,				//!< Insertion point is the center of the bottom side of the text string.
		TopRight,					//!< Insertion point is the top right corner of the text string.
		CenterRight,				//!< Insertion point is the center of the right side of the text string.
		BottomRight					//!< Insertion point is the bottom right corner of the text string.
	};

	/*! \enum ReferenceFrame
	 *	Enumeration of the reference frames for text alignment and justification. */
	enum class ReferenceFrame
	{
		WorldAligned,				//!< Alignment and justification will be defined relative to a screen-facing box around the text.
		PathAligned					//!< Alignment and justification will be defined relative to a text-path-aligned box around the text.
	};
	
	/*! \enum Justification
	 *	Enumerates the justifications used for multiline text strings. */
	enum class Justification
	{
		LeftJustified,				//!< Multiline strings will be left justfied.
		RightJustified,				//!< Multiline strings will be right justfied.
		CenterJustified				//!< Multiline strings will be center justfied.
	};
	
	/*! \enum Transform
	 *	Enumerates the transform behaviors for text strings.  These define how the entire string will be positioned within 3D space. */
	enum class Transform
	{
		/*! Text will be subject to all modelling and projection matrices, essentially acting as a textured quad.  Text paths will be act as transformations in 3D space. */
		Transformable,
		/*! Only the text insertion point (see also Text::Alignment) will be subject to modelling and projection matrices.  Text will always be drawn in screen space.
		 *	Text paths will act as transformations in screen space. */
		NonTransformable,
		/*! Text will be subject to all modelling and projection matrices, however characters will always be drawn in screen space. */
		CharacterPositionOnly,
		/*! Equivalent to CharacterPositionOnly, but additionally will draw text upright when it would otherwise appear upside down based on the camera.  To get this behavior
		 *	Text::ReferenceFrame::PathAligned and Text::Rotation::FollowPath must be set, otherwise this will be equivalent to CharacterPositionOnly. */
		CharacterPositionAdjusted
	};

	/*! \enum Renderer
	 *	Enumerates the renderers for text strings.  These define the font-handling subsystems Visualize uses to find and render fonts. */ 
	enum class Renderer
	{
		DefaultRenderer,			//!< All font-handling subsystems will be used.
		DriverRenderer,				//!< Only driver fonts will be used.  If a non-driver font is requested, the stroked font will be used instead.
		TruetypeRenderer,			//!< Only TrueType fonts will be used.  If a non-TrueType font is requested, the stroked font will be used instead.
	};

	/*! \enum Preference
	 *	Enumerates the font type preferences for text strings.  These define the types of fonts Visualize will use. */
	enum class Preference
	{
		Default,					//!< Use the default font type for the current driver.
		Vector,						//!< Use vector fonts.
		Raster,						//!< Use raster or bitmap fonts.
		Exterior					//!< Use the font outline (characters will be unfilled).
	};

	/*! \enum Rotatation
	 *	Enumerates the rotation modes for characters within text strings. */
	enum class Rotation
	{
		NoRotation,					//!< Characters are not rotated.
		Rotate,						//!< Characters are rotated clockwise by a specified angle.
		FollowPath					//!< Characters are rotated such that they will be perpendicular to the text path.
	};


	/*! \enum SizeUnits
	*	Enumerates the units for font sizes. */ 
	enum class SizeUnits
	{
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels					//!< Number of pixels.
	};

	/*! \enum SizeToleranceUnits
	*	Enumerates the units for size tolerances. */
	enum class SizeToleranceUnits
	{
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels,					//!< Number of pixels.
		Percent					//!< Percentage of the requested font size defining the lower limit on the smallest font size that can be substituted in the requested's place.
	};

	/*! \enum GreekingUnits
	*	Enumerates the units for greeking limits. */
	enum class GreekingUnits
	{
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels					//!< Number of pixels.
	};

	/*! \enum GreekingMode
	*	Enumerates the greeking modes.  These define what to draw in place of characters once they fall below the greeking limit. */
	enum class GreekingMode
	{
		Nothing,				//!< Nothing will be drawn for characters below the greeking limit.
		Lines,					//!< A grid of lines will be drawn in place of characters below the greeking limit.
		Box						//!< A filled box will be drawn in place of characters below the greeking limit.
	};

	/*! \enum RegionAlignment
	 *	Enumerates the alignments for text regions.  These define how text strings will be positioned relative to the line defining the text region. */
	enum class RegionAlignment
	{
		RegionTop,					//!< Text will be drawn below the region line.
		RegionCenter,				//!< Text will be drawn such that the region line passes through its center.
		RegionBottom				//!< Text will be drawn above the region line.
	};

private:
	//! Private default constructor to prevent instantiation.
	Text();
};

// Begin Line Attributes

/*! This is the Line Class */
class HPS_API Line
{
public:
	/*! \enum SizeUnits 
	Line Size Units
	*/
	enum class SizeUnits
	{
		ScaleFactor,			//!< A unitless linear scaling factor.  A value of 2.0 will cause lines to be rendered twice as thick.  A value of 1.0 will result in a visually pleasing device-dependent line weight.  A value of 0 will result in the minimal weight line supported by the device.
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels					//!< Number of pixels.
	};
private:
	Line();
};


// Begin Edge Attributes

/*! This is the Edge Class */
class HPS_API Edge
{
public:
	/*! \enum SizeUnits 
	Edge Size Units
	*/
	enum class SizeUnits
	{
		ScaleFactor,			//!< A unitless linear scaling factor.  A value of 2.0 will cause edges to be rendered twice as thick.  A value of 1.0 will result in a visually pleasing device-dependent edge weight.  A value of 0 will result in the minimal weight edge supported by the device.
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels					//!< Number of pixels.
	};
private:
	Edge();
};


/*! The CuttingSection class is a concept class for cutting-section-related enum classes. */
class HPS_API CuttingSection
{
public:
	/*! \enum Mode
	 *	Enumerates the modes for cutting sections.  These are the shapes (if any) of the visualization geometry that will be drawn to represent cutting planes within cutting sections. */
	enum class Mode
	{
		None,		//!< No visualization geometry will be drawn for cutting planes in a cutting sections.
		Round,		//!< A circular plane will be drawn for cutting planes in a cutting section.  The size of the planes will be based on the size of the geometry being cut and the visualization scale.
		Square,		//!< A square plane will be drawn for cutting planes in a cutting section.  The size of the planes will be based on the size of the geometry being cut and the visualization scale.
		Plane		//!< An infinitely large plane will be drawn for cutting planes in a cutting section.
	};

	/*! \enum CappingLevel
	 * 	Enumerates the level at which capping geometry should be generated for cutting sections. */
	enum class CappingLevel
	{
		Entity,				//!< Each piece of geometry should be treated as if it encloses a volume.
		Segment,			//!< All geometry in a segment (but not subsegments) should be treated as if it encloses a single volume.
		SegmentTree			//!< All geometry in a segment tree should be treated as if it encloses a single volume.
	};

	/*! \enum CuttingLevel
	 * 	Enumerates the level at which cutting planes should operate. */
	enum class CuttingLevel
	{
		Global,				//!< Cutting sections will cut all geometry in the scene graph.
		Local,				//!< Cutting sections will only cut geometry contained in the segment and subsegments where the cutting section is inserted.
	};

	/*! \enum MaterialPreference
	 * 	Enumerates which material settings should be used for cut geometry generated for cutting sections. */
	enum class MaterialPreference
	{
		Explicit,			//!< Use the effective materials set for cut faces and cut edges in the segment tree.
		Implicit,			//!< Use the effective materials set for (regular) faces and (regular) edges in the segment tree.
	};

	/*! \enum GatheringLevel
	 * 	Enumerates the level at which capping geometry should be gathered. */
	enum class GatheringLevel
	{
		Segment,				//!< Capping geometry will be gathered only from a single segment.
		SegmentTree				//!< Capping geometry will be gathered from the segment and all its subsegments and includes.
	};

private:
	//! Private default constructor to prevent instantiation.
	CuttingSection();
};

/*! This is the ContourLine Class*/
class HPS_API ContourLine
{
public:
	/*! \enum Mode 
	ContourLine Mode
	*/
	enum class Mode
	{
		Repeating,	//!< CuttingSection Mode
		Explicit,	//!< CuttingSection Mode
	};

private:
	ContourLine();
};

/*! \enum IOResult
 *	Enumerates the import and export result values. */
enum class IOResult
{
	Success,						//!< File IO Success.
	Failure,						//!< File IO Failure.
	FileNotFound,					//!< File was not found.
	UnableToOpenFile,				//!< Unable to open file.
	InvalidOptions,					//!< Invalid options specified.
	InvalidSegment,					//!< Invalid import/export segment specified.
	InProgress,						//!< File IO currently in progress.
	Canceled,						//!< File IO was canceled.
	UnableToLoadLibraries,			//!< Unable to load or locate the necessary libraries.
	VersionIncompatibility,			//!< Either failed to query the library version, libraries are too old or too new, or failed to initialize the required version.
	InitializationFailed,			//!< Unable to initialize the necessary libraries.
	UnsupportedFormat,				//!< File format cannot be imported.
};

/*! The LinePattern class is a concept class for line-pattern-related enum classes. */
class HPS_API LinePattern
{
public:
	/*! \enum SizeUnits
	 *	Enumerates the units used when specifying length, weight and offsets for line patterns. */
	enum class SizeUnits
	{
		ObjectSpace,			//!< Object space units ignoring any scaling components in modelling matrices.
		SubscreenRelative,		//!< Fraction of the height of the outermost window.
		WindowRelative,			//!< Fraction of the height of the local window.
		WorldSpace,				//!< Object space units including any scaling components in modelling matrices and cameras.
		Points,					//!< Points units typically used for text size.  1 point corresponds to 1/72 inch.
		Pixels,					//!< Number of pixels.
		ScaleFactor				//!< Fraction of the default line width.  A value of <span class='code'>1</span> corresponds to 0.1 percent of the screen size.
	};

	/*! \enum InsetBehavior
	 *	Enumerates how glyphs interact with other line pattern elements within a line pattern. */
	enum class InsetBehavior
	{
		Overlap,				//!< Glyphs will overlap with surrounding line pattern elements.  Line pattern elements adjacent to glyphs will not be trimmed.
		Trim,					//!< Glyphs will overlap with surrounding line pattern elements.  Line pattern elements adjacent to glyphs will be trimmed.
		Inline					//!< Glyphs will not overlap with surrounding line pattern elements, and adjacent line pattern elements will not be trimmed.
	};

	/*! \enum Join
	 * 	Enumerates the types of join to use for a line pattern.  Joins will always be mitred at the half angle for the smaller angle defined by two line
	 *	segments.  This enum classeration defines what the join will look like for the larger angle defined by two line segments. */
	enum class Join
	{
		MitreJoin,				//!< The join for the larger angle will be mitred, i.e., the segments will meet at a point at the half angle of the larger angle.
		RoundJoin,				//!< The join for the larger angle will be rounded, i.e., there will be a circle that subtends the larger angle.
		BevelJoin				//!< The join for the larger angle will be beveled, i.e., there will be a line that subtends the larger angle.
	};

	/*! \enum Cap
	 *  Enumerates the predefined cap types that can be used at the ends of line segments within a line pattern. */
	enum class Cap
	{
		/*! Line segments will end with a flat cap perpendicular to the line segment vector.  This cap will occur immediately at the end of the line segment
		 * 	and will not extend its length. */
		ButtCap,
		/*! Line segments will end with half of a filled square cap whose side is perpendicular to the line segment vector.  This cap will extend the length of a
		 * 	line segment, but otherwise will appear the same as the ButtCap. */
		SquareCap,
		/*! Line segments will end with half of a filled circle cap whose diameter is perpendicular to the line segment vector.  This cap will extend the length
		 * 	of a line segment. */
		RoundCap,
		/*! Line segments will end with a filled triangle cap whose base is perpendicular to the line segment vector.  This cap will extend the length of a line segment. */
		MitreCap
	};

	/*! \enum Justification
	 *	Enumerates the justfication modes for a line pattern.  These are used to attempt to make the line pattern appear evenly spaced between end points of a line. */
	enum class Justification
	{
		/*! Attempts to ensure that the first element of the line pattern appears at both the beginning and the end of the line.  This will give the best results
		 *	if the first element in a line pattern makes up at least half the total length of the elements for the line pattern body. */
		Center,
		/*! Stretches the pattern and modifies the size of the line pattern elements to ensure even spacing. */
		Stretch
	};

	/*! \enum Modifier
	 * 	Enumerates the types of joins, start caps, and end caps that can be set when a line pattern attribute is set. */
	enum class Modifier
	{
		GlyphName,				//!< A glyph was set for the cap or join.  The glyph name is valid.
		Enumerated				//!< A predefined (enum classerated) cap or join was set.  The enum classeration type is valid.
	};


	/*! \enum Default
	 * 	<p>Enumerates the predefined line patterns in Visualize.  These can be accessed via the LinePatternKit::GetDefault function and their appearance can be seen
	 * 	<a href="http://docs.techsoft3d.com/hps/core_graphics/prog_guide/app/default_line_patterns.html">here</a>. Hardware acceleration for line patterns is available when the following conditions are met:</p>
        
        <ul>
            <li>You are using the DirectX11 or OpenGL2 driver</li>
            <li>You are using Visualize 2015 or later</li>
	        <li>Segment-level display lists are active</li>
	        <li>The line weight is 1</li>
	    </ul>
	
		<p>The following line patterns can be hardware accelerated: <span class="code">Dashed, Dotted, DashDot, Dash2Dot, Dash3Dot, LongDash, FineDot</span>. The pattern does not continue along adjacent edges. The pattern is restarted for each edge.</p>
	
	*/
	enum class Default
	{
		Solid,					//!< A solid unbroked line pattern.
		DashDot,				//!< A repeating pattern of 18 solid pixels, 5 blank pixels, 4 solid pixels, and 5 blank pixels.
		Dashed,					//!< A repeating pattern of 10 solid pixels and 6 blank pixels.
		Dotted,					//!< A repeating pattern of 5 solid pixels and 3 blank pixels.
		Dash2Dot,				//!< A repeating pattern of 15 solid pixels, 4 blank pixels, 3 solid pixels, 3 blank pixels, 3 solid pixels, and 4 blank pixels.
		Dash3Dot,				//!< A repeating pattern of 15 solid pixels, 2 blank pixels, 3 solid pixels, 2 blank pixels, 3 solid pixels, 2 blank pixels, 3 solid pixels, and 2 blank pixels.
		LongDash,				//!< A repeating pattern of 26 solid pixels and 6 blank pixels.
		LongDashShortDash,		//!< A repeating pattern of 100 solid pixels, 10 blank pixels, 20 solid pixels, and 10 blank pixels.
		LongDash2ShortDash,		//!< A repeating pattern of 86 solid pixels, 8 blank pixels, 15 solid pixels, 8 blank pixels, 15 solid pixels, and 8 blank pixels.
		FineDot,				//!< A repeating pattern of 1 solid pixel and 3 blank pixels.
	};

private:
	//! Private default constructor to prevent instantiation.
	LinePattern();
};


//Begin Glyphs
/*! The Glyph class is a concept class for glyph-related enum classes. */
class HPS_API Glyph
{
public:
	/*! \enum Fill
	 * 	Enumerates the fill modes that can apply to glyph elements. */
	enum class Fill
	{
		Unfilled,				//!< Closed geometry will not be filled.
		ContinuousFill,			//!< Closed geometry will be filled.  If previous geometry was already filled, the fill be be continued.
		NewFill					//!< Closed geometry will be filled.  If previous geometry was already filled, the fill will be restarted.
	};

	/*! \enum ColorSource
	 * 	Enumerates the color modes that can apply to glyph elements. */
	enum class ColorSource
	{
		Normal,					//!< Glyph element color will be inherited from the color of the owning geometry.
		Explicit,				//!< Glyph element color will be the specified RGBA color.
		Indexed					//!< Glyph element color will be the color at the specified index in the effective material palette.
	};

	/*! \enum Default
	 * 	Enumerates the predefined glyphs in Visualize.  These can be accessed via the GlyphKit::GetDefault function and their appearance can be seen
	 * 	<a href="http://docs.techsoft3d.com/hps/core_graphics/prog_guide/app/default_glyphs.html">here</a>. */
	enum class Default
	{
		Circle,					//!< An unfilled circle.
		CircleWithDot,			//!< An unfilled circle with a dot in the center.
		CircleWithPlus,			//!< An unfilled circle with a plus sign in the center.
		CircleWithTimes,		//!< An unfilled circle with a times sign (X-shaped) in the center.
		CircleWithCircle,		//!< An unfilled circle with a smaller unfilled circle inside it.
		SolidCircle,			//!< A filled circle.
		Dot,					//!< A single dot.
		Plus,					//!< A plus sign.
		Times,					//!< A times sign (X-shaped).
		Splat,					//!< An asterisk (an X shape with an additional horizonatal line through the intersection point).
		Box,					//!< An unfilled square.
		BoxWithDot,				//!< An unfilled square with a dot in the center.
		BoxWithTimes,			//!< An unfilled square with a times sign (X-shaped) in the center.
		SolidBox,				//!< A filled square.
		Diamond,				//!< An unfilled diamond.
		DiamondWithDot,			//!< An unfilled diamond with a dot in the center.
		DiamondWithPlus,		//!< An unfilled diamond with a plus sign in the center.
		SolidDiamond,			//!< A filled diamond.
		TriangleUp,				//!< An unfilled upward-pointing triangle.
		TriangleUpWithDot,		//!< An unfilled upward-pointing triangle with a dot in the center.
		SolidTriangleUp,		//!< A filled upward-pointing triangle.
		TriangleDown,			//!< An unfilled downward-pointing triangle.
		TriangleDownWithDot,	//!< An unfilled downward-pointing triangle with a dot in the center.
		SolidTriangleDown,		//!< A filled downward-pointing triangle.
		TriangleRight,			//!< An unfilled rightward-pointing triangle.
		TriangleRightWithDot,	//!< An unfilled rightward-pointing triangle with a dot in the center.
		SolidTriangleRight,		//!< A filled rightward-pointing triangle.
		TriangleLeft,			//!< An unfilled leftward-pointing triangle.
		TriangleLeftWithDot,	//!< An unfilled leftward-pointing triangle with a dot in the center.
		SolidTriangleLeft,		//!< A filled leftward-pointing triangle.
		TriangleUpV,			//!< An unfilled upward-pointing triangle with the top vertex at the glyph canvas origin.
		SolidTriangleUpV,		//!< A filled upward-pointing triangle with the top vertex at the glyph canvas origin.
		TriangleDownV,			//!< An unfilled downward-pointing triangle with the bottom vertex at the glyph canvas origin.
		SolidTriangleDownV,		//!< A filled downward-pointing triangle with the bottom vertex at the glyph canvas origin.
		TriangleRightV,			//!< An unfilled rightward-facing triangle with the right vertex at the glyph canvas origin.
		SolidTriangleRightV,	//!< A filled rightward-facing triangle with the right vertex at the glyph canvas origin.
		TriangleLeftV,			//!< An unfilled leftward-facing triangle with the left vertex at the glyph canvas origin.
		SolidTriangleLeftV,		//!< A filled leftward-facing triangle with the left vertex at the glyph canvas origin.
		LetterY,				//!< The letter Y.
		LetterZ,				//!< The letter Z.
		CircleLetterY,			//!< The letter Y circumscribed in an unfilled circle.
		CircleLetterZ,			//!< The letter Z circumscribed in an unfilled circle.
		VerticalBar,			//!< A vertical bar.
		HorizontalBar,			//!< A horizontal bar.
		SlantLeft,				//!< A left-slanting bar.
		SlantRight,				//!< A right-slanting bar.
		WidePlus,				//!< An unfilled plus sign.
		SolidWidePlus,			//!< A filled plus sign.
		WideTimes,				//!< An unfilled times sign (X-shaped).
		SolidWideTimes,			//!< A filled times sign (X-shaped).
		Pound,					//!< Two parallel vertical bars intersecting two parallel horizontal bars.
		Sphere,					//!< A phong-shaded sphere when vertex lighting is enabled on supported drivers. A solid circle in all other cases.
	};
	
private:
	//! Private default constructor to prevent instantiation.
	Glyph();
};


/*! This is the Bounding Class*/
class HPS_API Bounding
{
public:
	/*! \enum Type
	Bounding Type
	*/
	enum class Type
	{
		None,
		Sphere,		//!< Bounding Type
		Cuboid		//!< Bounding Type
	};
	
private:
	Bounding() {}
};

/*! The Grid class is a concept class for grid-related enum classes. */
class HPS_API Grid
{
public:
	/*! \enum Type
	 * 	Enumerates the grid types. */
	enum class Type
	{
		Quadrilateral,	//!< Grid composed of quadrilaterals.
		Radial			//!< Grid composed of circular wedges.
	};

private:
	Grid();
};


} // HPS namespace

#endif

