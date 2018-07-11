// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#pragma once

#include <slapi/geometry.h>

// Yes - yet another point class.  But sketchup didn't have float versions of these
// These are used to convert sketchup's double versions into float

struct SUPoint3Df {
	float x;
	float y;
	float z;

	SUPoint3Df() : x(0), y(0), z(0) { }

	SUPoint3Df(SUPoint3D const &in)
		: x(static_cast<float>(in.x)), y(static_cast<float>(in.y)), z(static_cast<float>(in.z)) {}
};

struct SUVector3Df {
	float x;
	float y;
	float z;

	SUVector3Df() : x(0), y(0), z(0) { }

	SUVector3Df(SUVector3D const &in)
		: x(static_cast<float>(in.x)), y(static_cast<float>(in.y)), z(static_cast<float>(in.z)) {}
};
