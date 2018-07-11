// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupFreehand.cpp : implementation of the HOpMarkupFreehand class
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HOpMarkupFreehand.h"
#include "hc.h"
#include "hic.h"
#include "HImManager.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HMarkupManager.h"


HOpMarkupFreehand::HOpMarkupFreehand(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	SetOperatorStarted(false);
}


HBaseOperator * HOpMarkupFreehand::Clone()
{
	return new HOpMarkupFreehand(GetView());
}


const char * HOpMarkupFreehand::GetName() { return "HOpMarkupFreehand"; }


int HOpMarkupFreehand::OnLButtonDown(HEventInfo &event)
{
	SetNewPoint(event.GetMouseWindowPos());

	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
		HC_Open_Segment("MarkupFreehand");

			if (!OperatorStarted()) 
			{
				SetOperatorStarted(true);
				m_lPolyline = HC_KInsert_Ink(GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);
			}
			else
				HC_Insert_Ink(GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);

			HC_Set_Rendering_Options("no frame buffer effects");

		HC_Close_Segment();
	HC_Close_Segment();
		
	m_ptLast.x = GetNewPoint().x;
	m_ptLast.y = GetNewPoint().y;
	m_ptLast.z = GetNewPoint().z;
    return HOP_OK;

}



int HOpMarkupFreehand::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnNoButtonDownAndMove(event);

	SetNewPoint(event.GetMouseWindowPos());
 
	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());
		HC_Open_Segment("MarkupFreehand");	
 			HC_Insert_Ink(GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);
		HC_Close_Segment();			
	HC_Close_Segment();
 		
	m_ptLast = GetNewPoint();
     
    GetView()->Update();    
    return HOP_OK;
}



int HOpMarkupFreehand::OnLButtonUp(HEventInfo &event)
{
	int iPolylineCount;
	if (!OperatorStarted()) 
	    return HBaseOperator::OnLButtonDblClk(event);

	// Pack the Operators Polyline point array and count
	HC_Show_Polyline_Count(m_lPolyline, &iPolylineCount);
	HPoint *pPolyline = new HPoint[iPolylineCount];
	HC_Show_Polyline(m_lPolyline, &iPolylineCount, pPolyline);

	// Clean up HOOPS segment tree
	HC_Open_Segment_By_Key(GetView()->GetWindowspaceKey());	
		HC_Flush_Segment("...");  	 		
	HC_Close_Segment ();
	GetView()->Update();

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		for (int i=0; i<iPolylineCount; i++)
			HC_Compute_Coordinates(".", "local window", &pPolyline[i], "world", &pPolyline[i]);
	HC_Close_Segment();

	CreateMarkupFreehand(GetView(),"Freehand",iPolylineCount,pPolyline, GetView()->EmittingMessages());
	// Update view and reset OpStarted Flag
	GetView()->Update();

	delete[] pPolyline;
	SetOperatorStarted(false);

	return HOP_READY;
}


void HOpMarkupFreehand::CreateMarkupFreehand(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message)
{
	HMarkupManager *markupManager;
	HC_KEY freehand=0;
	if ((markupManager = pView->GetMarkupManager()) != 0)
	{
		HC_KEY activelayerkey;
		if ((activelayerkey = markupManager->GetCurrentLayerKey()) == -1)
			activelayerkey = markupManager->OpenLayer("");
		
		HC_Open_Segment_By_Key(activelayerkey);
 			freehand = HC_KOpen_Segment (segment);
				// this is now hoops default behaviour but might be useful for "no absolute line weight"
				//HImSetCallback("draw dc polyline","HImUtility_device_independent_polyline_weights");
				HC_Insert_Polyline(PolyLineLength,pPolyline);
				// let's set a round end cap so that we 
				// don't get spikes in the polyline
				HC_Set_Line_Pattern("(----)");
 			HC_Close_Segment();
		HC_Close_Segment();	
	}

	if (freehand && emit_message)
		markupManager->EmitMarkupSegment(freehand);
}
