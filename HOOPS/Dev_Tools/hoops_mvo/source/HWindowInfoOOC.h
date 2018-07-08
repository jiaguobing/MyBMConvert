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


#include "HBaseView.h"
#include "PointCloudAPI.h"


/*! This class represents either a rectangular or triangular window-space area to filter OOC points with.
*/
class HWindowInfoOOC {
private:
	enum Type { INVALID, RECTANGLE, TRIANGLE };

public:
	/*! Constructs an invalid HWindowInfoOOC object.
	*/
	HWindowInfoOOC ()
		: type(INVALID)
	{}

	/*! Constructs a rectangular window-space area.
		\param view A reference to an HBaseView object.
		\param window_min The minimum corner of the rectangle to use.
		\param window_max The maximum corner of the rectangle to use.
		\return Returns an HWindowInfoOOC object represeting the given rectangle.
	*/
	static HWindowInfoOOC Rectangle (HBaseView & view, ooc::Point const & window_min, ooc::Point const & window_max);

	/*! Constructs a triangular window-space area.
		\param view A reference to an HBaseView object.
		\param p1 The first point of the triangle to use.
		\param p2 The second point of the triangle to use.
		\param p3 The third point of the triangle to use.
		\return Returns an HWindowInfoOOC object representing the given triangle.
	*/
	static HWindowInfoOOC Triangle (HBaseView & view, ooc::Point const & p1, ooc::Point const & p2, ooc::Point const & p3);

	/*! Determines if a bounding box is rejected based on intersection with the area this object represents.
		\param world_bounds The corners of the bounding box to test against.
		\return Returns true if the area this object represents does not intersect with world_bounds.
	*/
	bool RejectBounding (ooc::Point const (&world_bounds)[8]) const;

	/*! Determines if a point is rejected based on intersection with the area this object represents.
		\param world_point The point to test against.
		\return Returns true if the area this object represents does not contain world_point.
	*/
	bool AcceptPoint (ooc::Point const & world_point) const;

private:
	HWindowInfoOOC (HBaseView & view, Type type);

	bool RectangleAcceptPoint (ooc::Point const & world_point) const;
	bool TriangleAcceptPoint (ooc::Point const & world_point) const;

private:
	Type type;
	float world_to_window[16];
	ooc::Point window_min;
	ooc::Point window_max;

	struct {
		ooc::Point p3;
		float inverse_denom;
		float alpha_1;
		float alpha_2;
		float beta_1;
		float beta_2;
	} barycentric_info;
};



