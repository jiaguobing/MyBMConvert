// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCircle.cpp : implementation of the HOpConstructCircle class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCreateCircle.h"
#include "HUtility.h"
#include "HUtilityGeometryCreation.h"




/////////////////////////////////////////////////////////////////////////////
// HOpConstructCircle.cpp : implementation of the HOpConstruction class
//
// Operator for rubberbanding a Circle based on mouse input.
//
// Left button down:
//		Find first position
//
// Left Button down and motion:
//		Rubber band box is inserted in HOOPS "windowspace" segment 
//
//		"Windowspace" is a HOOPS segment with a camera that has a streteched projection
//		and a field width and height of 2 units. Thus window coordinates and world 
//		coordinates with z=0 value are the same. This is convenient for drawing 
//		temporary graphics.
//
//
// Left Button Up:
//		Store points for Circle in window space; 
//
//


HOpCreateCircle::HOpCreateCircle(HBaseView* view, int DoRepeat, int DoCapture, bool UseCenterMarker) : HOpConstructCircle(view, DoRepeat, DoCapture)
{
	UNREFERENCED(UseCenterMarker);
}

HOpCreateCircle::~HOpCreateCircle()
{
}

HBaseOperator * HOpCreateCircle::Clone()
{
	return new HOpConstructCircle(GetView());
}


const char * HOpCreateCircle::GetName() { return "HOpConstructCircle"; }


//HUtility::CreateAxisCircle(GetFirstPoint(), m_radius, HUtility::XY);
int HOpCreateCircle::OnLButtonUp(HEventInfo &event)
{
	HVector radius_vector;

	if (OperatorStarted()) {
		HOpConstructCircle::OnLButtonUp(event);
		HPoint tmp(GetFirstPoint());
		float w2o[16];

		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Compute_Coordinates(".", "viewpoint", &tmp, "world", &tmp);
		HC_Close_Segment();

		HC_Open_Segment_By_Key(GetView()->GetModelKey());{
			HC_Open_Segment(GetName());{

				HC_Compute_Coordinates(".", "world", &tmp, "object", &tmp);
				HC_Compute_Transform(".", "world", "object", w2o);

				HCamera camera;
				GetView()->GetCamera(&camera);
				HPoint normal = camera.target - camera.position;
				HPlane plane(normal, 0);
				HC_Compute_Transformed_Plane(&plane, w2o, &plane);
				plane.CalculateNormal(normal);

				HC_Insert_Circle_By_Radius(&tmp, m_radius, &normal);
			}HC_Close_Segment();
		}HC_Close_Segment();
		GetView()->Update();
	}

    return HOP_READY;
}
