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
#include "HOpConstructCircle.h"
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


HOpConstructCircle::HOpConstructCircle(HBaseView* view, int DoRepeat, int DoCapture, bool UseCenterMarker) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bUseCenterMarker = UseCenterMarker;
}

HOpConstructCircle::~HOpConstructCircle()
{
}

HBaseOperator * HOpConstructCircle::Clone()
{
	return new HOpConstructCircle(GetView());
}


const char * HOpConstructCircle::GetName() { return "HOpConstructCircle"; }


/////////////////////////////////////////////////////////////////////////////
// HOpConstructCircle message handlers



int HOpConstructCircle::OnLButtonDown(HEventInfo &event)
{	
	SetFirstPoint(event.GetMouseViewpointPos());

	if (!OperatorStarted()) 
	{
		SetOperatorStarted(true);
	}
		
	return HOP_OK;
}




int HOpConstructCircle::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted())
		return HBaseOperator::OnLButtonDownAndMove(event);
  	
	// Get the new mouse position
    SetNewPoint(event.GetMouseViewpointPos());

	//make certain that new_point != first_point 
	if (GetNewPoint().x == GetFirstPoint().x &&
		GetNewPoint().y == GetFirstPoint().y)
		return HOP_CANCEL;

	HVector radius_vector(GetNewPoint() - GetFirstPoint());
	m_radius = (float)HC_Compute_Vector_Length(&radius_vector);

	// Insert a box on viewplane using the first and new mouse positions
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Geometry (".");
		HC_Set_Color("geometry = red");
		HUtility::CreateAxisCircle(GetFirstPoint(), m_radius, HUtility::XY);
	HC_Close_Segment();
    GetView()->Update();

    return HOP_OK;
}



int HOpConstructCircle::OnLButtonUp(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);
  	
	SetOperatorStarted(false);

	// Get the new mouse position
    SetNewPoint(event.GetMouseViewpointPos());

	//make certain that new_point != first_point 
	if (GetNewPoint().x == GetFirstPoint().x &&
		GetNewPoint().y == GetFirstPoint().y)
		return HOP_CANCEL;

	HVector radius_vector(GetNewPoint() - GetFirstPoint());
	m_radius = (float)HC_Compute_Vector_Length(&radius_vector);

    HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Geometry (".");
		HC_UnSet_One_Visibility ("markers");
	HC_Close_Segment();
    GetView()->Update();

    return HOP_READY;
}
