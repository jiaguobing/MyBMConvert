// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HWindowFilterOOC.h"


//////////////////////////////////////////////////////////////////////////


HWindowFilterOOC::HWindowFilterOOC (bool deep_filter)
	: deep_filter(deep_filter)
{}


bool HWindowFilterOOC::RejectPointsInMemory ()
{
	return false;
}


bool HWindowFilterOOC::RejectPointsOnDisk ()
{
	return !deep_filter;
}


bool HWindowFilterOOC::RejectNode (ooc::NodeHandle const & node_handle)
{
	UNREFERENCED(node_handle);
	return false;
}


bool HWindowFilterOOC::RejectBounding (ooc::Point const & min_bound, ooc::Point const & max_bound)
{
	ooc::Point world_bounds[8];
	WorldBoxCorners(min_bound, max_bound, world_bounds);

	size_t const count = window_infos.Count();
	for (size_t i = 0; i < count; ++i) {
		HWindowInfoOOC const & window_info = window_infos.GetData(i);
		if (!window_info.RejectBounding(world_bounds)) {
			return false;
		}
	}
	return true;
}


bool HWindowFilterOOC::AcceptPoint (ooc::Point const & point, size_t point_index)
{
	UNREFERENCED(point_index);

	size_t const count = window_infos.Count();
	for (size_t i = 0; i < count; ++i) {
		HWindowInfoOOC const & window_info = window_infos.GetData(i);
		if (window_info.AcceptPoint(point)) {
			return true;
		}
	}
	return false;
}


void HWindowFilterOOC::AddRectangleWindow (HBaseView & view, ooc::Point const & window_min, ooc::Point const & window_max)
{
	window_infos.Append(HWindowInfoOOC::Rectangle(view, window_min, window_max));
}


void HWindowFilterOOC::AddTriangleWindow (HBaseView & view, ooc::Point const & p1, ooc::Point const & p2, ooc::Point const & p3)
{
	window_infos.Append(HWindowInfoOOC::Triangle(view, p1, p2, p3));
}


void HWindowFilterOOC::ClearWindows ()
{
	window_infos = VArray<HWindowInfoOOC>();
}


bool HWindowFilterOOC::HasWindows () const
{
	return window_infos.Count() > 0;
}


void HWindowFilterOOC::WorldBoxCorners (ooc::Point const & world_min, ooc::Point const & world_max, ooc::Point (&out_world_points)[8])
{
	float const w = world_max.x - world_min.x;
	float const h = world_max.y - world_min.y;

	out_world_points[0] = world_min;
	out_world_points[1] = world_min + ooc::Point(w, 0, 0);
	out_world_points[2] = world_min + ooc::Point(0, h, 0);
	out_world_points[3] = world_min + ooc::Point(w, h, 0);

	out_world_points[4] = world_max;
	out_world_points[5] = world_max - ooc::Point(w, 0, 0);
	out_world_points[6] = world_max - ooc::Point(0, h, 0);
	out_world_points[7] = world_max - ooc::Point(w, h, 0);
}


