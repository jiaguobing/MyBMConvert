// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateCylinder.cpp : implementation of the HOpCreateCylinder class
// 

#include <math.h>
#include <stdlib.h>
#include <string.h>
 
#include "HTools.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HOpCreateCylinder.h"
#include "HUtilityGeometryCreation.h"
#include "HStream.h"


/////////////////////////////////////////////////////////////////////////////
// HOpCreateCylinder

// Operator for inserting a cylinder into a newly created Scene/Part segment.
// Mouse down - fix first position
// Mouse motion while down - rubberband cylinder base defined by first point and
// current position in world space in the scene/construction segment 
// (cylinder base is orthogonal to camera target plane)
// Mouse up and motion- cylinder height defined by base and current position in world space
// (main cylinder axis is parallel to camera target plane)	



HOpCreateCylinder::HOpCreateCylinder(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructCylinder(view, DoRepeat, DoCapture)
{
}

HOpCreateCylinder::~HOpCreateCylinder()
{
}

HBaseOperator * HOpCreateCylinder::Clone()
{
	return new HOpCreateCylinder(GetView());
}


const char * HOpCreateCylinder::GetName() { return "HOpCreateCylinder"; }

/////////////////////////////////////////////////////////////////////////////
// CreateCylinder message handlers



int HOpCreateCylinder::OnLButtonDown(HEventInfo &event)
{
	if (OperatorStarted() && m_RadiusLength > 0.0){
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());	
			HVector pos, tar, up;		
			HC_Show_Net_Camera_Position(&pos.x, &pos.y, &pos.z);
			HC_Show_Net_Camera_Target(&tar.x, &tar.y, &tar.z);

			HPoint view;
			view.Set(pos.x-tar.x, pos.y-tar.y, pos.z-tar.z);
			HC_Compute_Normalized_Vector(&view, &view);

			HC_Show_Net_Camera_Up_Vector(&up.x, &up.y, &up.z);
		HC_Close_Segment();

		HC_Compute_Normalized_Vector(&view, &view);
		HC_Compute_Normalized_Vector(&up, &up);

		// this work is to make sure that the geometry takes into account 
		// any modelling matrices that may be in the scene
		HC_KEY keys[2];
		keys[0] = GetView()->GetModel()->GetModelKey();
		keys[1] = GetView()->GetSceneKey();
		HPoint radius_vector, height_vector, origin;
		origin.Set(0.0f, 0.0f, 0.0f);
		radius_vector.Set(m_RadiusLength, 0.0f, 0.0f);
		height_vector.Set(m_height, 0.0f, 0.0f);

		HPoint first_point = GetFirstPoint();
		HC_Compute_Coordinates_By_Path(2, keys, "viewpoint", &first_point,	"object", &first_point);
		SetFirstPoint(first_point);
		HC_Compute_Coordinates_By_Path(2, keys, "viewpoint", &origin,		"object", &origin);
		HC_Compute_Coordinates_By_Path(2, keys, "viewpoint", &radius_vector,"object", &radius_vector);
		HC_Compute_Coordinates_By_Path(2, keys, "viewpoint", &height_vector,"object", &height_vector);

		HPoint deltar, deltah;
		deltar.Set(radius_vector.x - origin.x, radius_vector.y - origin.y, radius_vector.z - origin.z);
		deltah.Set(height_vector.x - origin.x, height_vector.y - origin.y, height_vector.z - origin.z);

		float radius = (float) HC_Compute_Vector_Length(&deltar);
		float height = (float) HC_Compute_Vector_Length(&deltah);

		if (m_height < 0)
			height *= -1;

		HC_Open_Segment_By_Key (GetView()->GetModelKey());
			HC_Open_Segment ("cylinders");
				HUtilityGeometryCreation::CreateCylinder(GetFirstPoint(), radius, height, 20, view, up);
			HC_Close_Segment();
		HC_Close_Segment();
	}

	return HOpConstructCylinder::OnLButtonDown(event);
}
