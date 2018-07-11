// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCone.cpp : implementation of the ConstructCone class
//

#include <math.h>
#include <stdlib.h>
#include <string.h>
 
#include "HTools.h"
#include "HEventInfo.h"
#include "HBaseView.h"
#include "HOpConstructCone.h"
#include "HUtilityGeometryCreation.h"


/////////////////////////////////////////////////////////////////////////////
// HOpConstructCone

// Operator for inserting a cone into a newly Constructd Scene/Part segment.
// Mouse down - fix first position
// Mouse motion while down - rubberband cone base defined by first point and
// current position in world space in the scene/construction segment 
// (cone base is orthogonal to camera target plane)
// Mouse up and motion- cone height defined by base and current position in world space
// (main cone axis is parallel to camera target plane)		



HOpConstructCone::HOpConstructCone(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
}

HOpConstructCone::~HOpConstructCone()
{
}


HBaseOperator * HOpConstructCone::Clone()
{
	return new HOpConstructCone(GetView());
}



const char * HOpConstructCone::GetName() { return "HOpConstructCone"; }


/////////////////////////////////////////////////////////////////////////////
// ConstructCone message handlers



int HOpConstructCone::OnLButtonDown(HEventInfo &event)
{
	if (OperatorStarted()){
		// now need the points in world coordinate system since the
		// parasolid entity is Constructd in world space
					
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Contents (".", "geometry");
		HC_Close_Segment();

		SetOperatorStarted(false);
		GetView()->SetGeometryChanged();
		GetView()->Update(); // update the display
	} else {
		SetNewPoint(event.GetMouseViewpointPos());
		SetFirstPoint(GetNewPoint());
		SetOperatorStarted(true);
	}

	return HOP_OK;
}


void HOpConstructCone::GenerateLinesThroughCircle(HPoint const &center, float radius)
{
	HVector temp, temp2;

	temp.x = center.x - radius;
	temp.y = center.y;
	temp.z = center.z;
	temp2.x = center.x + radius;
	temp2.y = center.y;
	temp2.z = center.z;
	HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);
	HC_Compute_Coordinates(".", "viewpoint", &temp2, "world", &temp2);
	HC_Insert_Line( temp.x, temp.y, temp.z, temp2.x, temp2.y, temp2.z );

	temp.x = center.x;
	temp.y = center.y;
	temp.z = center.z - radius;
	temp2.x = center.x;
	temp2.y = center.y;
	temp2.z = center.z + radius;
	HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);
	HC_Compute_Coordinates(".", "viewpoint", &temp2, "world", &temp2);
	HC_Insert_Line( temp.x, temp.y, temp.z, temp2.x, temp2.y, temp2.z );
}


int HOpConstructCone::OnLButtonDownAndMove(HEventInfo &event)
{
    //  Mouse is down and has moved  
    HVector radius_vector;
	float	m_radius;
  	 
	if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);

	SetNewPoint(event.GetMouseViewpointPos());

	radius_vector.Set (GetNewPoint() - GetFirstPoint());

	m_radius = (float) HC_Compute_Vector_Length (&radius_vector);

	if (m_radius > 0.0)
	{
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Contents (".", "geometry");
			HUtility::CreateAxisCircle (GetFirstPoint(), m_radius, HUtility::XZ);
			GenerateLinesThroughCircle(GetFirstPoint(), m_radius);
		HC_Close_Segment();
	}

	GetView()->Update();
    return HOP_OK;
}



int HOpConstructCone::OnNoButtonDownAndMove(HEventInfo &event)
{
    //  Mouse is down and has moved  
    HVector radius_vector;
  	 
	if (!OperatorStarted()) return HBaseOperator::OnNoButtonDownAndMove(event);

	SetNewPoint(event.GetMouseViewpointPos());

	radius_vector.Set (GetNewPoint().x - GetFirstPoint().x,
						 GetNewPoint().y - GetFirstPoint().y,
						 GetNewPoint().z - GetFirstPoint().z);

	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		m_height = GetNewPoint().y - GetFirstPoint().y;

		if (m_height != 0)
		{
			HC_Flush_Contents (".", "geometry");
			ConstructWireframeCone(GetFirstPoint(), m_RadiusLength , m_height);
		}
	HC_Close_Segment();

	GetView()->Update();
    return HOP_OK;
}



int HOpConstructCone::OnLButtonUp(HEventInfo &event)
{
    // Mouse has come up 
	if(!OperatorStarted())
		return HBaseOperator::OnLButtonDownAndMove(event);

    SetNewPoint(event.GetMouseViewpointPos());
	HPoint world_edge_point(event.GetMouseWorldPos());

    HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();

    // make certain that new_point != first_point 
	if (GetNewPoint() == GetFirstPoint())
	{
		SetOperatorStarted(false);
		return HOP_CANCEL;
	}

	HVector radius_vector(GetNewPoint() - GetFirstPoint());

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		m_RadiusLength = (float) HC_Compute_Vector_Length (&radius_vector);

		HPoint world_center_point;
		HPoint first_point = GetFirstPoint();
		HC_Compute_Coordinates(".", "viewpoint", &first_point, "world", &world_center_point);
		m_WorldRadiusVector = world_edge_point - world_center_point;

	}HC_Close_Segment();
	
    GetView()->Update(); // update the display
    return HOP_READY;
}



void HOpConstructCone::ConstructWireframeCone(HPoint center, float radius, float height)
{
	HPoint	bottom, top;
	
	bottom = center;
	HUtility::CreateAxisCircle(bottom, radius, HUtility::XZ);
	GenerateLinesThroughCircle(bottom, radius);

	top.Set(bottom.x, bottom.y + height, bottom.z);

	float angle = 2.0f * HUtility::PI / 8.0f;

	for (int i = 0; i < 8; i++) {
		HPoint top, bottom;
		top.Set(center.x + radius * (float)cos (i * angle),
						center.y,
						center.z + radius * (float)sin (i * angle));
		bottom.Set(center.x,
						center.y + height,
						center.z);

		HC_Compute_Coordinates(".", "viewpoint", &bottom, "world", &bottom);
		HC_Compute_Coordinates(".", "viewpoint", &top, "world", &top);

		HC_Insert_Line(top.x, top.y, top.z, bottom.x, bottom.y, bottom.z);
	}
}


