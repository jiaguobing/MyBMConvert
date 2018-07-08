// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCylinder.cpp : implementation of the HOpConstructCylinder class
// 

#include <math.h>
#include <stdlib.h>
#include <string.h>
 
#include "HTools.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HOpConstructCylinder.h"
#include "HUtilityGeometryCreation.h"
#include "HStream.h"


/////////////////////////////////////////////////////////////////////////////
// HOpConstructCylinder

// Operator for inserting a cylinder into a newly Constructd Scene/Part segment.
// Mouse down - fix first position
// Mouse motion while down - rubberband cylinder base defined by first point and
// current position in world space in the scene/construction segment 
// (cylinder base is orthogonal to camera target plane)
// Mouse up and motion- cylinder height defined by base and current position in world space
// (main cylinder axis is parallel to camera target plane)	



HOpConstructCylinder::HOpConstructCylinder(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
}

HOpConstructCylinder::~HOpConstructCylinder()
{
}

HBaseOperator * HOpConstructCylinder::Clone()
{
	return new HOpConstructCylinder(GetView());
}


const char * HOpConstructCylinder::GetName() { return "HOpConstructCylinder"; }

/////////////////////////////////////////////////////////////////////////////
// ConstructCylinder message handlers



int HOpConstructCylinder::OnLButtonDown(HEventInfo &event)
{
	if (OperatorStarted())
	{
	    HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		    HC_Flush_Contents (".", "geometry");
	    HC_Close_Segment();
	    
	    SetOperatorStarted(false);
		GetView()->SetGeometryChanged();
	    GetView()->Update();
	} else {
	    SetNewPoint(event.GetMouseViewpointPos());
	    SetFirstPoint(GetNewPoint());
	    SetOperatorStarted(true);
	}

	return HOP_OK;
}



void HOpConstructCylinder::GenerateLinesThroughCircle(HPoint const &center, float radius)
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


int HOpConstructCylinder::OnLButtonDownAndMove(HEventInfo &event)
{
    //  Mouse is down and has moved  
    HVector	radius_vector;
	float radius;

    if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);
  	 
    SetNewPoint(event.GetMouseViewpointPos());

	radius_vector.Set (GetNewPoint().x - GetFirstPoint().x,
						 GetNewPoint().y - GetFirstPoint().y,
						 GetNewPoint().z - GetFirstPoint().z);

	radius = (float)HC_Compute_Vector_Length(&radius_vector);

	// only draw the wireframe rep if the radius is > 0
	if (radius > 0.0)
	{
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Contents (".", "geometry");
			HUtility::CreateAxisCircle (GetFirstPoint(), radius, HUtility::XZ);
			GenerateLinesThroughCircle( GetFirstPoint(), radius );
		HC_Close_Segment();
	}

    GetView()->Update();
    return HOP_OK;
}



int HOpConstructCylinder::OnNoButtonDownAndMove(HEventInfo &event)
{
    //  Mouse is down and has moved  
    HVector	radius_vector;

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
			ConstructWireframeCylinder(GetFirstPoint(), m_RadiusLength , m_height);
		}

	HC_Close_Segment();

    GetView()->Update();
    return HOP_OK;
}




int HOpConstructCylinder::OnLButtonUp(HEventInfo &event)
{
    // Mouse has come up 

    HPoint   world_edge_point, world_center_point;
	HVector	radius_vector;

	if(!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);

    SetNewPoint(event.GetMouseViewpointPos());
	world_edge_point = event.GetMouseWorldPos();

    HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();

    // make certain that new_point != first_point 
	if (GetNewPoint().x == GetFirstPoint().x && GetNewPoint().y == GetFirstPoint().y && GetNewPoint().z == GetFirstPoint().z)
	{
		SetOperatorStarted(false);
		return HOP_CANCEL;
	}

	radius_vector.Set (GetNewPoint().x - GetFirstPoint().x,
						 GetNewPoint().y - GetFirstPoint().y,
						 GetNewPoint().z - GetFirstPoint().z);


	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
			m_RadiusLength = (float) HC_Compute_Vector_Length (&radius_vector);
			
			HPoint tmp(GetFirstPoint());
			HC_Compute_Coordinates(".", "viewpoint", &tmp, "world", &world_center_point);

			m_WorldRadiusVector.Set(world_edge_point.x - world_center_point.x,
									world_edge_point.y - world_center_point.y,
									world_edge_point.z - world_center_point.z);
	HC_Close_Segment();

    GetView()->Update(); // update the display
    return HOP_READY;
}



void HOpConstructCylinder::ConstructWireframeCylinder(HPoint center, float radius, float height)
{
	HPoint	bottom, top;
	
	bottom = center;
	HUtility::CreateAxisCircle(bottom, radius, HUtility::XZ);
	GenerateLinesThroughCircle( bottom, radius );

	top.Set(bottom.x, bottom.y + height, bottom.z);
	HUtility::CreateAxisCircle(top, radius, HUtility::XZ);
	GenerateLinesThroughCircle( top, radius );

	float angle = 2.0f * HUtility::PI / 8.0f;

	for (int i = 0; i < 8; i++) {
		HPoint top, bottom;
		top.Set(center.x + radius * (float)cos (i * angle),
						center.y,
						center.z + radius * (float)sin (i * angle));

		bottom.Set(top.x,
						top.y + height,
						top.z);

		HC_Compute_Coordinates(".", "viewpoint", &bottom, "world", &bottom);
		HC_Compute_Coordinates(".", "viewpoint", &top, "world", &top);

		HC_Insert_Line(top.x, top.y, top.z, bottom.x, bottom.y, bottom.z);
	}
}

