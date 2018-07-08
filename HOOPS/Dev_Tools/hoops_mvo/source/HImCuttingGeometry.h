// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HCuttingPlaneLinedata.h : interface of the HCuttingPlaneLinedata class
//
// supports creation of linked list of polylines which represent the lines
// of intersection with a cutting plane.  supports insertion of these lines
// into a segment within the scene in HBaseView

/*! \file HImCuttingGeometry.h
	Defines datatypes associated with the HImCuttingGeometry class  
*/

#ifndef _HIM_CUTTING_GEOMETRY_H
#define _HIM_CUTTING_GEOMETRY_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "hic_types.h"

/*! \enum HIMCG_Cutting_Type
    Tracks whether we cook to lines, polylines, or faces.  polylines and faces may be used together with a bitwise OR 
*/
enum HIMCG_Cutting_Type {
	HIM_CUTTING_GEOMETRY_TYPE_LINES			= 0,		//!< Line Type
	HIM_CUTTING_GEOMETRY_TYPE_POLYLINES		= 0x1,		//!< Polyline Type
	HIM_CUTTING_GEOMETRY_TYPE_FACES			= 0x2		//!< Face Type
};
/*! \enum HIMCG_Body_Type
    Determines where one body stops and the next begins (especially important for capping polygons).
    Bodies can start and stop at a segment level, or at a body level. 
*/
enum HIMCG_Body_Type {
	HIM_CUTTING_GEOMETRY_SEGMENT_BODY		= 1,		//!< Segment Body
	HIM_CUTTING_GEOMETRY_SHELL_BODY			= 2		//!< Shell Body
};


/*!
	This class implements capping polygons.
	Capping planes help extend the illusion to the user that he is dealing with
	actual solids, as opposed to what they really are -- boundary representations.

	<p>Capping planes are implemented in MVO via the HImCuttingGeometry class.  
	They work by setting an I.M. callback on cutting lines, then collecting those into
	closed loops from which capping polygons represented as shell faces can be derived.
	The HImCuttingGeometry class can be used to create polylines, faces or both.

	<p>Computation of capping polygons is an operation on solid bodies, so there needs to 
	be some notion of what geometry should be considered as part of the same solid.
	The normal modes of operation (decided, as described below, with an argument to the 
	CreateCuttingGeometry function) are to consider each shell to be a separate solid or
	take everything in a segment to collectively represent a solid.  In the latter mode,
	under certain circumstances as described below, entire segment trees can be considered 
	as one.

	<p>The main function where all of the magic happens is CreateCuttingGeometry().  This
	function has four arguments: a) source_key, b) target_key, c) cutting_type, 
	defaulted to HIM_CUTTING_GEOMETRY_TYPE_POLYLINES, and d) body_level, defaulted to 
	HIM_CUTTING_GEOMETRY_SHELL_BODY.

	<p>This class is intended to be used to recalculate geometry only when needed,
	as opposed to once on every update.  To that end, we suggest that the user
	create a dedicated segment for all of the temporary geometry, and keep a flag
	that is marked "dirty" whenever geometry, or cutting planes move.  Note that camera
	movements don't require recalculation.  In that segment, 
	<i>Set_Heuristics("no polygon handedness")</i> as well as 
	<i>Set_Heuristics("concave faces")</i> should be used.

	<p>With the HIM_CUTTING_GEOMETRY_SHELL_BODY body_level, a particular user index 
	set to a non-zero value has a special meaning.  If the user index defined by
	H_SOLID_BODY_MAGIC (see htools.h in MVO) is set to something other than zero, everything
	in and underneath the segment with that index will be considered as part of the same
	solid body, and merged together.  In case you're curious, H_SOLID_BODY_MAGIC is defined
	as (('b'<<24) | ('o'<<16) | ('d'<<8) | ('y')), which works out to be
	0x626f6479.  So <i>Set_User_Index( H_SOLID_BODY_MAGIC, 1 )</i>
	will cause everything underneath the currently open segment to get
	merged together as part
	of the same solid body.

	<p>Merging line pieces is asymptotically an O(n^2) operation, and the triangulation can
	also be quite expensive.  Once they are in place, however the cutting geometry is drawn
	just as fast as if it had been part of the original model until something happens
	that triggers a recalculation.  Segregating solids as much as possible by using the 
	appropriate setting of the body_level argument of CreateCuttingGeometry will help with
	performance by keeping the "n" in the O(n^2) small.

	<p>Proper segregation helps in another way as well: colors.  One solid body corresponds to at most one
	shell face for the capping polygon.  The color that will be set on that shell face is the
	net face color from the segment in which the body was started.  Inappropriate use
	of the H_SOLID_BODY_MAGIC user index at too high a segment can cause color settings from below
	to be missed, and consequently an inappropriate color applied to the capping polygon.

	<p>Under circumstances where only lines are needed, and it is known that the cutting lines 
	will be quickly invalidated, there are two approaches that may perform better.  The first is
	to use HIM_CUTTING_GEOMETRY_TYPE_LINES as the cutting type, which will save the effort of doing
	any merging.  The second possibility is to bypass the HImCuttingGeometry class entirely, with 
	<i>Set_Visibility( "cut lines=on" )</i>.  Keep in mind, though, that those approaches are **only**
	for when it is known that the cutting lines will be quickly invalidated (since otherwise
	the upfront computational cost will pay off in future updates). 

	<p>Numerical problems can create sets of lines that form "loops" with self-intersections.
	Triangulating such loops is a difficult problem, for which our current approach is not guaranteed
	to solve perfectly.  As a consequence, there will, on occasion, be very thin slivers missing
	from the capping polygon.  To help mitigate such numerical problems, two functions are provided 
	to control the tolerances of a) the smallest feature that is to be considered valid; and 
	b) the allowable distance between endpoints for a "closed" loop to be considered closed.
*/
class MVO_API HImCuttingGeometry
{
public:
	/*! Constructs an HImCuttingGeometry object.*/
	HImCuttingGeometry();
	~HImCuttingGeometry();

	/*! This method is the compute routine that uses HOOPS IM information during the process. 
		\param source_key The key to the root of the segment tree in which we want to find cutting geometry.
		\param target_key The key of the segment that is to contain the cutting geometry after completion.
		\param cutting_type Pass a #HIMCG_Cutting_Type which is a bitmask of geometry types (HIM_CUTTING_GEOMETRY_TYPE_POLYLINES or 
				HIM_CUTTING_GEOMETRY_TYPE_FACES) to be used for cutting geometry.  HIM_CUTTING_GEOMETRY_TYPE_LINES=0 
				is a special value that indicates that lines should not be merged into polylines (and certainly not 
				from there into faces).  This saves on initial computation but is slightly slower. During draw time, 
				the default is HIM_CUTTING_GEOMETRY_TYPE_POLYLINES.
		\param body_level This parameter determines what geometry should be considered to be part of the same solid body.  
				Pass a #HIMCG_Body_Type which can be either HIM_CUTTING_GEOMETRY_SEGMENT_BODY or HIM_CUTTING_GEOMETRY_SHELL_BODY.
				In the latter case, if there is a nonzero user index set at H_SOLID_BODY_MAGIC (e.g. 
				with  HC_Set_User_Index(H_SOLID_BODY_MAGIC,1) ), everything in and under that segment 
				will be merged together. */
	void CreateCuttingGeometry(HC_KEY source_key, HC_KEY target_key,
			int cutting_type=HIM_CUTTING_GEOMETRY_TYPE_POLYLINES,
			int body_level=HIM_CUTTING_GEOMETRY_SEGMENT_BODY);

	/*! This method is the compute routine that uses a callback and newer 3dgs functionality.
	\param source_key The key to the root of the segment tree in which we want to find cutting geometry.
	\param target_key The key of the segment that is to contain the cutting geometry after completion.
	\param honor_modelling_matrices Pass true to allow local transforms be applied to the cutting geometry or false not.
	*/
	void CreateCuttingGeometry(HC_KEY source_key, HC_KEY target_key, bool honor_modelling_matrices);

	/*! This method gives HOOPS pointers to the functions we are planning to have available.  This function should 
	be called just once, at the beginning of execution. */
	static void RegisterCallbacks();

	/*! <b>This feature has been deprecated.</b> This method sets the smallest feature not culled away to avoid numerical problems.
	\param smallest_feature Pass a float value defining the smallest feature.  The default value is 0.001 * camera width. */
	void SetSmallestFeature(float smallest_feature) { m_smallest_feature = smallest_feature; }
	/*! <b>This feature has been deprecated.</b> \return The smallest feature not culled away to avoid numerical problems. */
	float GetSmallestFeature() { return m_smallest_feature; }
	/*! This method sets the distance between two points that may be considered a match for the purposes of assembling 
	polylines and testing for closed loops.
	\param tolerance Pass a float value that defines the range between two points for matching purposes.  The default 
	value is 0.1 * camera width. */
	void SetTolerance(float tolerance) { m_tolerance = tolerance; }
	/*! \return The largest distance between two points that may be considered a match for the purposes of assembling 
	polylines and testing for closed loops. */
	float GetTolerance() { return m_tolerance; }

private:

	/* Collects all of the cutting lines into a list.  This list can either be directly displayed as a collection
	    of lines, or merged into a closed loop for the purposes of computing a capping polygon. */
	static void StoreCuttingPlaneLines (HIC_Rendition const *nr, int count, HIC_DC_Point const pts[]);
	/* If we are computing capping polygon(s), this routine will allow us to determine when one body stops
		and the next begins. */
	static void StopBody (HIC_Rendition const *nr);
	/* calls straight through to StopBody (see above) */
	static void ShellBody (HIC_Rendition const *nr, HIC_Polyhedron const *phon);
	/* calls straight through to StopBody (see above) */
	static void SegmentBody (HIC_Rendition const *nr, HIC_Segment_Info const *seg);

	/* does the actual work for the new CreateCuttingGeometry */
	static void GetCaps(HIC_Rendition const *nr, HIC_Polyhedron const *p);

	/* tracks whether we cook to lines, polylines, or faces.  polylines and faces may be used together with a bitwise OR */
	static int m_cutting_type;
	/* used only to set the handedness of the cutting geometry segment */
	static bool m_body_is_open;
	static float m_smallest_feature;
	/* the largest allowable distance between start and end to be considered a match for the purposes of 
	   assembling polylines and testing for closed loops */
	static float m_tolerance;
	static struct vlist_s * m_lines;     /* stores uncooked soup of lines */
	static struct vlist_s * m_polylines; /* stores lines merged together where possible */
	static struct vlist_s * m_faces;	/* stores (probably closed) loops to be used as shell faces */
	static HC_KEY m_target_segment;
	static bool m_honor_matrices;
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif








