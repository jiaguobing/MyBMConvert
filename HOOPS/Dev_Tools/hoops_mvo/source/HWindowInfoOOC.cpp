// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HWindowInfoOOC.h"
#include <algorithm>


//////////////////////////////////////////////////////////////////////////


#undef min
#undef max
#undef ASSERT


#ifdef _WIN32
#	pragma warning(disable: 4127)
#endif


#ifndef ASSERT
#	ifdef NDEBUG
#		define ASSERT(x)		do {} while (false)
#	else
#if _WIN32
#		define ASSERT(x)		((!(x)) ? __debugbreak() : 1)
#else
#		include <assert.h>
#		define ASSERT			assert
#endif
#	endif
#endif


#define countof(x)				(sizeof(x) / sizeof(x[0]))


//////////////////////////////////////////////////////////////////////////


HWindowInfoOOC::HWindowInfoOOC (HBaseView & view, Type type)
	: type(type)
{
	HC_KEY const include_keys[] = { view.GetIncludeLinkKey() };
	HC_Compute_Transform_By_Path(countof(include_keys), include_keys, "world", "local window", world_to_window);
}


HWindowInfoOOC HWindowInfoOOC::Rectangle (HBaseView & view, ooc::Point const & window_min, ooc::Point const & window_max)
{
	HWindowInfoOOC info(view, RECTANGLE);

	info.window_min = window_min;
	info.window_max = window_max;

	return info;
}


HWindowInfoOOC HWindowInfoOOC::Triangle (HBaseView & view, ooc::Point const & p1, ooc::Point const & p2, ooc::Point const & p3)
{
	HWindowInfoOOC info(view, TRIANGLE);

	info.window_min.x = std::min(p1.x, std::min(p2.x, p3.x));
	info.window_min.y = std::min(p1.y, std::min(p2.y, p3.y));

	info.window_max.x = std::max(p1.x, std::max(p2.x, p3.x));
	info.window_max.y = std::max(p1.y, std::max(p2.y, p3.y));

	double const a = p2.y - p3.y;
	double const b = p3.x - p2.x;
	double const c = p3.y - p1.y;
	double const d = p1.x - p3.x;
	double const e = p1.y - p3.y;

	double const inverse_denom = 1.0 / ((a * d) + (b * e));

	auto & bi = info.barycentric_info;

	bi.p3 = p3;
	bi.inverse_denom = static_cast<float>(inverse_denom);
	bi.alpha_1 = static_cast<float>(a);
	bi.alpha_2 = static_cast<float>(b);
	bi.beta_1 = static_cast<float>(c);
	bi.beta_2 = static_cast<float>(d);

	return info;
}


bool HWindowInfoOOC::RejectBounding (ooc::Point const (&world_bounds)[8]) const
{
	ooc::Point window_bounds[8];
	HC_Compute_Transformed_Points(8, world_bounds, world_to_window, window_bounds);

	ooc::Point window_min_bound = window_bounds[0];
	for (ooc::Point const & window_bound : window_bounds) {
		if (window_bound.x < window_min_bound.x) {
			window_min_bound.x = window_bound.x;
		}
		if (window_bound.y < window_min_bound.y) {
			window_min_bound.y = window_bound.y;
		}
	}

	ooc::Point window_max_bound = window_bounds[0];
	for (ooc::Point const & window_bound : window_bounds) {
		if (window_bound.x > window_max_bound.x) {
			window_max_bound.x = window_bound.x;
		}
		if (window_bound.y > window_max_bound.y) {
			window_max_bound.y = window_bound.y;
		}
	}

	if (window_max.x < window_min_bound.x) {
		return true;
	}
	if (window_min.x > window_max_bound.x) {
		return true;
	}

	if (window_max.y < window_min_bound.y) {
		return true;
	}
	if (window_min.y > window_max_bound.y) {
		return true;
	}

	return false;
}


bool HWindowInfoOOC::AcceptPoint (ooc::Point const & world_point) const
{
	if (type == RECTANGLE) {
		return RectangleAcceptPoint(world_point);
	}
	else {
		ASSERT(type == TRIANGLE);
		return TriangleAcceptPoint(world_point);
	}
}


bool HWindowInfoOOC::RectangleAcceptPoint (ooc::Point const & world_point) const
{
	ooc::Point window_point;
	HC_Compute_Transformed_Points(1, &world_point, world_to_window, &window_point);

	if (window_point.x < window_min.x) {
		return false;
	}
	if (window_point.y < window_min.y) {
		return false;
	}

	if (window_point.x > window_max.x) {
		return false;
	}
	if (window_point.y > window_max.y) {
		return false;
	}

	return true;
}


bool HWindowInfoOOC::TriangleAcceptPoint (ooc::Point const & world_point) const
{
	ooc::Point window_point;
	HC_Compute_Transformed_Points(1, &world_point, world_to_window, &window_point);

	auto const & bi = barycentric_info;
	ooc::Point const & p = window_point;

	float const x_delta = p.x - bi.p3.x;
	float const y_delta = p.y - bi.p3.y;

	float const alpha = (bi.alpha_1 * x_delta + bi.alpha_2 * y_delta) * bi.inverse_denom;
	if (alpha < 0.0f) {
		return false;
	}

	float const beta = (bi.beta_1 * x_delta + bi.beta_2 * y_delta) * bi.inverse_denom;
	if (beta < 0.0f) {
		return false;
	}

	if (alpha + beta > 1) {
		return false;
	}

	return true;
}





