// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <stdlib.h>
#include <string.h>
 
#include "HTools.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HOpConstructSphere.h"
#include "HUtilityGeometryCreation.h"


/////////////////////////////////////////////////////////////////////////////
// HOpConstructSphere
//
// Operator for inserting a sphere into a newly Constructd Scene/Part segment.
// Mouse down - fix first position
// Mouse motion while down - rubberband sphere with radius defined by first point and
// current position in world space in the scene/construction segment 
// Mouse up inserts the sphere

HOpConstructSphere::HOpConstructSphere(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
}

HOpConstructSphere::~HOpConstructSphere()
{
}

HBaseOperator * HOpConstructSphere::Clone()
{
	return new HOpConstructSphere(GetView());
}


const char * HOpConstructSphere::GetName() { return "HOpConstructSphere"; }

/////////////////////////////////////////////////////////////////////////////
// HOpConstructSphere message handlers




int HOpConstructSphere::OnLButtonDown(HEventInfo &event)
{
	if (!OperatorStarted()) 
		SetOperatorStarted(true);

	SetFirstPoint(event.GetMouseViewpointPos());
	SetNewPoint(GetFirstPoint());

    return HOP_OK;
}


void HOpConstructSphere::GenerateLinesThroughSphere()
{
	HVector temp, temp2;

	temp.x = GetFirstPoint().x - m_radius;
	temp.y = GetFirstPoint().y;
	temp.z = GetFirstPoint().z;
	temp2.x = GetFirstPoint().x + m_radius;
	temp2.y = GetFirstPoint().y;
	temp2.z = GetFirstPoint().z;
	HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);
	HC_Compute_Coordinates(".", "viewpoint", &temp2, "world", &temp2);
	HC_Insert_Line( temp.x, temp.y, temp.z, temp2.x, temp2.y, temp2.z );

	temp.x = GetFirstPoint().x;
	temp.y = GetFirstPoint().y - m_radius;
	temp.z = GetFirstPoint().z;
	temp2.x = GetFirstPoint().x;
	temp2.y = GetFirstPoint().y + m_radius;
	temp2.z = GetFirstPoint().z;
	HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);
	HC_Compute_Coordinates(".", "viewpoint", &temp2, "world", &temp2);
	HC_Insert_Line( temp.x, temp.y, temp.z, temp2.x, temp2.y, temp2.z );

	temp.x = GetFirstPoint().x;
	temp.y = GetFirstPoint().y;
	temp.z = GetFirstPoint().z - m_radius;
	temp2.x = GetFirstPoint().x;
	temp2.y = GetFirstPoint().y;
	temp2.z = GetFirstPoint().z + m_radius;
	HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);
	HC_Compute_Coordinates(".", "viewpoint", &temp2, "world", &temp2);
	HC_Insert_Line( temp.x, temp.y, temp.z, temp2.x, temp2.y, temp2.z );
}


int HOpConstructSphere::OnLButtonDownAndMove(HEventInfo &event)
{
    //  Mouse is down and has moved  
    HVector radius_vector;

	if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);

    SetNewPoint(event.GetMouseViewpointPos());

	radius_vector.Set(GetNewPoint() - GetFirstPoint());

	m_radius = (float)HC_Compute_Vector_Length(&radius_vector);

	// only draw the wireframe rep if the radius is > 0
	if (m_radius > 0.0)
	{
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Geometry(".");
			ConstructWireframeSphere (GetFirstPoint(), m_radius);
			GenerateLinesThroughSphere();
		HC_Close_Segment();
	}

    GetView()->Update();
    return HOP_OK;
}



int HOpConstructSphere::OnLButtonUp(HEventInfo &event)
{
    // Mouse has come up 
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

	SetOperatorStarted(false);

	SetNewPoint(event.GetMouseViewpointPos());

	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();
 
	GetView()->SetGeometryChanged();
    GetView()->Update(); 

    return HOP_READY;
}

// 

void HOpConstructSphere::ConstructWireframeSphere (HPoint const & center, float radius)
{
	HUtility::CreateAxisCircle(center, radius, HUtility::XY);
	HUtility::CreateAxisCircle(center, radius, HUtility::XZ);
	HUtility::CreateAxisCircle(center, radius, HUtility::YZ);
}
