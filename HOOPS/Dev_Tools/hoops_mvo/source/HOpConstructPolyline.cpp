// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructPolyline.cpp : implementation of the HOpConstructPolyline class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpConstructPolyline.h"



/////////////////////////////////////////////////////////////////////////////
// HOpConstructPolyline
//
// Operator for rubberbanding a Polyline in the viewplane.
//
// Left button down:
//		Insert_Ink with position in window space in "?window space" segment
//
// NoButtonDownAndMove:
//		Rubber band line from last point to current point 
//
// Left Button Double Click: 
//		Polyline definition is done. 
//		Store the Polyline information; Points are in Window Space
//		Flush "?Window Space" segment
//
// HOpConstructPolyline.cpp : implementation of the HOpConstruct class
//


HOpConstructPolyline::HOpConstructPolyline(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	SetOperatorStarted(false);
	m_pPolyline = 0;
}

HOpConstructPolyline::~HOpConstructPolyline()
{

	if(OperatorStarted()) 
	{
		// Clean up HOOPS segment tree
		HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());	
			HC_Flush_Segment(".");  	 		
			HC_Delete_Segment ("line");	
		HC_Close_Segment ();

		// Update view and reset OpStarted Flag
		GetView()->Update();
		SetOperatorStarted(false);
	}

	H_SAFE_DELETE_ARRAY(m_pPolyline);
}


HBaseOperator * HOpConstructPolyline::Clone()
{
	return new HOpConstructPolyline(GetView());
}



const char * HOpConstructPolyline::GetName() { return "HOpConstructPolyline"; }

/////////////////////////////////////////////////////////////////////////////
// HOpConstructPolyline message handlers


int HOpConstructPolyline::OnNoButtonDownAndMove(HEventInfo &event)
{    
	if (!OperatorStarted()) 
		return HBaseOperator::OnNoButtonDownAndMove(event);

    SetNewPoint(event.GetMouseWindowPos());

    HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
		HC_Open_Segment ("line");
			HC_Flush_Segment(".");
			HC_Insert_Line(m_ptLast.x, m_ptLast.y, m_ptLast.z, 
						   GetNewPoint().x,  GetNewPoint().y,  GetNewPoint().z);
		HC_Close_Segment();
	HC_Close_Segment();

    GetView()->Update();

    return HOP_OK;
}


int HOpConstructPolyline::OnLButtonDown(HEventInfo &event)
{
	SetNewPoint(event.GetMouseWindowPos());

	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
	
		if (!OperatorStarted()) 
		{
			SetOperatorStarted(true);
			m_PolylineKey = HC_KInsert_Ink(GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);
		}
		else
			HC_Insert_Ink(GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);
			
	HC_Close_Segment();
		
	m_ptLast = GetNewPoint();
    
    return HOP_OK;
}



int HOpConstructPolyline::OnLButtonDblClk(HEventInfo &event)
{
	if (!OperatorStarted()) 
	    return HBaseOperator::OnLButtonDblClk(event);

	H_SAFE_DELETE_ARRAY(m_pPolyline);

	// Pack the Operators Polyline point array and count
	HC_Show_Polyline_Count(m_PolylineKey, &m_PolylineCount);
	m_pPolyline = new HPoint[m_PolylineCount];
	HC_Show_Polyline(m_PolylineKey, &m_PolylineCount, m_pPolyline);

	// Clean up HOOPS segment tree
	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());	
	    HC_Flush_Geometry("...");  	 		
            HC_Open_Segment ("line");
                HC_Flush_Segment(".");
            HC_Close_Segment();
	HC_Close_Segment ();

	// Update view and reset OpStarted Flag
	GetView()->Update();
	SetOperatorStarted(false);

	return HOP_READY;
}


