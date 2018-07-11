// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once


#ifdef HMFC_STATIC_LIB
#	error "Code cannot be statically linked."
#endif


#ifdef H_PACK_8
#	pragma pack(push)
#	pragma pack(8)
#endif


//////////////////////////////////////////////////////////////////////////

# if !defined(__APPLE__) && defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8))
#	define OLD_GCC
#	define OVERRIDE 
# else
#	define OVERRIDE override
# endif

#include "PointCloudAPI.h"
#include "HWindowInfoOOC.h"
#include "varray.h"


//////////////////////////////////////////////////////////////////////////


/*! The HWindowFilterOOC class filters OOC points based on window-space areas.
	These areas can be a combination of rectangles and triangles.
*/
class HWindowFilterOOC : public ooc::query::Filter {
public:
	/*! Constructs an HWindowFilterOOC object without any window-space areas to filter OOC points with.
		\param deep_filter Determines whether or not the constructed object will accept OOC points that are not loaded in memory.
	*/
	HWindowFilterOOC (bool deep_filter);

	/*! Never rejects points in memory.
	*/
	virtual bool RejectPointsInMemory () OVERRIDE;
	
	/*! Rejects points on disk if deep_filter is not set through the constructor.
	*/
	virtual bool RejectPointsOnDisk () OVERRIDE;

	/*! Never rejects a node based on its node handle.
	*/
	virtual bool RejectNode (ooc::NodeHandle const & node_handle) OVERRIDE;

	/*! Rejects boundings based on any supplied rectangle or triangle window-space areas.
		The boundings are projected to window-space and tested against each of these areas.
		If none overlap, the bounding is rejected.
	*/
	virtual bool RejectBounding (ooc::Point const & min_bound, ooc::Point const & max_bound) OVERRIDE;

	/*! Rejects a point based on any supplied rectangle or triangle window-space areas.
		The point is projected to window-space and tested against each of these areas.
		If any of these areas contains the point, it is accepted.
	*/
	virtual bool AcceptPoint (ooc::Point const & point, size_t point_index) OVERRIDE;

	/*! Adds a rectangle window-space area to filter points with.
	*/
	void AddRectangleWindow (HBaseView & view, ooc::Point const & window_min, ooc::Point const & window_max);

	/*! Adds a triangle window-space area to filter points with.
	*/
	void AddTriangleWindow (HBaseView & view, ooc::Point const & p1, ooc::Point const & p2, ooc::Point const & p3);

	/*! Discards all existing rectangle or triangle window-space areas that have been added to this object.
	*/
	void ClearWindows ();

	/*! \return Returns true if this object has any rectangle or triangle window-space areas added to it.
	*/
	bool HasWindows () const;

private:
	void WorldBoxCorners (ooc::Point const & world_min, ooc::Point const & world_max, ooc::Point (&out_world_points)[8]);

private:
	VArray<HWindowInfoOOC> window_infos;
	bool deep_filter;
};



