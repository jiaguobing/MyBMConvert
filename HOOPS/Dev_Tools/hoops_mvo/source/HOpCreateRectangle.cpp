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
#include "HOpCreateRectangle.h"
#include "HUtility.h"




/////////////////////////////////////////////////////////////////////////////
// HOpCreateRectangle.cpp : implementation of the HOpCreateRectangle class
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


HOpCreateRectangle::HOpCreateRectangle(HBaseView* view, int DoRepeat, int DoCapture, bool UseCenterMarker) : HOpConstructRectangle(view, DoRepeat, DoCapture)
{
	UNREFERENCED(UseCenterMarker);
}

HOpCreateRectangle::~HOpCreateRectangle()
{
}

HBaseOperator * HOpCreateRectangle::Clone()
{
	return new HOpCreateRectangle(GetView());
}


const char * HOpCreateRectangle::GetName() { return "HOpCreateRectangle"; }


/////////////////////////////////////////////////////////////////////////////
// HOpConstructRectangle message handlers



int HOpCreateRectangle::OnLButtonUp(HEventInfo &event)
{
	if(OperatorStarted()){
		HOpConstructRectangle::OnLButtonUp(event);

		HPoint points[4];

		points[0] = m_ptRectangle[0];
		points[1].x = m_ptRectangle[0].x;
		points[1].y = m_ptRectangle[1].y;
		points[2] = m_ptRectangle[1];
		points[3].x = m_ptRectangle[1].x;
		points[3].y = m_ptRectangle[0].y;

		HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
			HC_Compute_Coordinates(".","outer window", &points[0], "world", &points[0]);
			HC_Compute_Coordinates(".","outer window", &points[1], "world", &points[1]);
			HC_Compute_Coordinates(".","outer window", &points[2], "world", &points[2]);
			HC_Compute_Coordinates(".","outer window", &points[3], "world", &points[3]);
		}HC_Close_Segment();

		HC_Open_Segment_By_Key(GetView()->GetModelKey());{
			HC_Open_Segment(GetName());{
				HC_Compute_Coordinates(".","world", &points[0], "object", &points[0]);
				HC_Compute_Coordinates(".","world", &points[1], "object", &points[1]);
				HC_Compute_Coordinates(".","world", &points[2], "object", &points[2]);
				HC_Compute_Coordinates(".","world", &points[3], "object", &points[3]);
				HC_Insert_Polygon(4, points);
			}HC_Close_Segment();
		}HC_Close_Segment();

		GetView()->Update(); 
	}
 
    return HOP_READY;
}

