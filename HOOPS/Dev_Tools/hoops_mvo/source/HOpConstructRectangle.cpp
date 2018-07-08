// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructRectangle.cpp : implementation of the HOpConstructRectangle class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpConstructRectangle.h"
#include "HConstantFrameRate.h"
#include "HUtility.h"




/////////////////////////////////////////////////////////////////////////////
// HOpConstructRectangle.cpp : implementation of the HOpConstruction class
//
// Operator for rubberbanding a rectangle based on mouse input.
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
//		Store points for Rectangle in window space; 
//
//


HOpConstructRectangle::HOpConstructRectangle(HBaseView* view, int DoRepeat, int DoCapture, bool UseCenterMarker) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bUseCenterMarker = UseCenterMarker;
	m_pSavedVisibility[0] = '\0';
	m_pSavedColor[0] = '\0';
}

HOpConstructRectangle::~HOpConstructRectangle()
{
}

HBaseOperator * HOpConstructRectangle::Clone()
{
	return new HOpConstructRectangle(GetView());
}


const char * HOpConstructRectangle::GetName() { return "HOpConstructRectangle"; }


/////////////////////////////////////////////////////////////////////////////
// HOpConstructRectangle message handlers



int HOpConstructRectangle::OnLButtonDown(HEventInfo &event)
{	
	SetFirstPoint(event.GetMouseWindowPos());
	GetView()->GetConstantFrameRateObject()->SetDisableIncreaseTemp(true);
	if (!OperatorStarted()) 
	{
		SetOperatorStarted(true);
		m_bRectangleExists = false;
		HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
			if (HC_Show_Existence ("color"))
				HC_Show_Color (m_pSavedColor);
			if (HC_Show_Existence ("visibility"))
				HC_Show_Visibility (m_pSavedVisibility);
		HC_Close_Segment();
	}
		
	return HOP_OK;
}



int HOpConstructRectangle::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted())
		return HBaseOperator::OnLButtonDownAndMove(event);
  	
	// Get the new mouse position
    SetNewPoint(event.GetMouseWindowPos());

	// Insert a box on viewplane using the first and new mouse positions
    HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());

		HC_Flush_Geometry (".");

		//we specify filled, which means that we get a shell, but the face visibility is often
		//turned off in the windowspace segment
		HUtility::InsertRectangle (".", GetFirstPoint().x, GetFirstPoint().y, GetNewPoint().x, GetNewPoint().y, 0, true);

		if (m_bUseCenterMarker) 
		{
			HPoint center;

			center.x = 0.5F * (GetFirstPoint().x + GetNewPoint().x);
			center.y = 0.5F * (GetFirstPoint().y + GetNewPoint().y);
			center.z = 0.5F * (GetFirstPoint().z + GetNewPoint().z);

			HC_Insert_Marker (center.x, center.y, center.z);
			//HC_Set_Visibility ("markers = on"); this is the default
		}

	HC_Close_Segment();
    GetView()->Update();

    return HOP_OK;
}



int HOpConstructRectangle::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonUp(event);
	  
	SetOperatorStarted(false); 

	// Clean up HOOPS Segment Tree

	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
		HC_Flush_Geometry (".");

		if (HC_Show_Existence ("color")) {
			char	test_Color[MVO_BUFFER_SIZE];
			HC_Show_Color (test_Color);
			if (!streq(test_Color, m_pSavedColor)) {
				HC_UnSet_Color();
				HC_Set_Color (m_pSavedColor);
			}
		}
		if (HC_Show_Existence ("visibility")) {
			char	test_Visibility[MVO_BUFFER_SIZE];
			HC_Show_Visibility (test_Visibility);
			if (!streq(test_Visibility, m_pSavedVisibility)) {
				HC_UnSet_Visibility();
				HC_Set_Visibility (m_pSavedVisibility);
			}
		}	
		
	HC_Close_Segment();

    SetNewPoint(event.GetMouseWindowPos());

	//make certain that new_point != first_point 
	if (GetNewPoint().x == GetFirstPoint().x &&
		GetNewPoint().y == GetFirstPoint().y)
		return HOP_CANCEL;

	// Store Rectangle Points in window space
	m_ptRectangle[0] = GetFirstPoint();
	m_ptRectangle[1] = GetNewPoint();
	m_bRectangleExists = true;
	GetView()->GetConstantFrameRateObject()->SetDisableIncreaseTemp(false);

    GetView()->Update(); 
 
    return HOP_READY;
}

int HOpConstructRectangle::OnRButtonUp(HEventInfo &event)
{
	return OnLButtonUp(event);
}
