// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupFreehand3D.cpp : implementation of the HOpMarkupFreehand3D class
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "HTools.h"
#include "hc.h"
#include "hic.h"
#include "HImManager.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpMarkupFreehand3D.h"
#include "HMarkupManager.h"


HOpMarkupFreehand3D::HOpMarkupFreehand3D(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bOpStarted = false;
}


HBaseOperator * HOpMarkupFreehand3D::Clone()
{
	return (HBaseOperator *)new HOpMarkupFreehand3D(m_pView);
}


const char * HOpMarkupFreehand3D::GetName() { return "HOpMarkupFreehand3D"; }

bool HOpMarkupFreehand3D::IsModelGeometry()
{
    int count;
 	    HC_Show_Selection_Keys_Count(&count);
	    HC_KEY *keys = new HC_KEY[count];
	    HC_Show_Selection_Keys(&count, keys);
	    bool found = false;
 	    for (int i=0;i<count;i++)
	    {
		if (keys[i] == m_pView->GetModel()->GetModelKey())
		    found =  true;
	    }
	    delete []  keys;
	    return found;
}
    

int HOpMarkupFreehand3D::OnLButtonDown(HEventInfo &event)
{
	HPoint window, world;
 	HC_KEY key;
	int num_selections = 0;
	
	m_ptNew = event.GetMouseWindowPos();
	
	HPoint WindowSpace = event.GetMouseWindowPos();
	HC_Open_Segment_By_Key(m_pView->GetViewKey());
		num_selections = HC_Compute_Selection (".", "./scene/overwrite", "v", WindowSpace.x, WindowSpace.y);
	HC_Close_Segment();
	if ((num_selections > 0) && IsModelGeometry()){
		HC_Show_Selection_Element(&key, NULL, NULL, NULL);
		HC_Show_Selection_Position (&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);	 									 		
		HC_Open_Segment_By_Key(m_pView->GetMarkupManager()->GetMarkupKey());
			HC_Open_Segment("freehand3d");
				HC_Set_Line_Weight (3.0);
				HC_Set_Color("lines=red");
				HC_Set_Visibility("lines=on");
				
				if (!m_bOpStarted) 
				{
					m_bOpStarted = true;
					m_lPolyline = HC_KInsert_Ink(world.x, world.y, world.z);
				}
				else
				{
					HC_Insert_Ink(world.x, world.y, world.z);
				}
			HC_Close_Segment();
		HC_Close_Segment();
	}
	else
	{
		HC_Open_Segment_By_Key(m_pView->GetMarkupManager()->GetMarkupKey());
			HC_Open_Segment("freehand3d");
				HC_Restart_Ink();
			HC_Close_Segment();	
		HC_Close_Segment();
	}

	return (HOP_OK);
	
}



int HOpMarkupFreehand3D::OnLButtonDownAndMove(HEventInfo &event)
{
 	HPoint	window, world;
 	HC_KEY    key;
	int num_selection = 0;

	if (!m_bOpStarted) 
		return HBaseOperator::OnNoButtonDownAndMove(event);
 
	HPoint WindowSpace = event.GetMouseWindowPos();
	HC_Open_Segment_By_Key(m_pView->GetViewKey());
		num_selection = HC_Compute_Selection (".", "./scene/overwrite", "v", WindowSpace.x, WindowSpace.y);
	HC_Close_Segment();
	if ((num_selection > 0) && IsModelGeometry()){
		HC_Show_Selection_Element(&key, NULL, NULL, NULL);
		HC_Show_Selection_Position (&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);	 									 		
		HC_Open_Segment_By_Key(m_pView->GetMarkupManager()->GetMarkupKey());
			HC_Open_Segment("freehand3d");
				if (!m_bOpStarted) 
				{
					m_bOpStarted = true;
					m_lPolyline = HC_KInsert_Ink(world.x, world.y, world.z);
				}
				else
				{
					HC_Insert_Ink(world.x, world.y, world.z);
				}
			HC_Close_Segment();
		HC_Close_Segment();
	}
	else
	{
		HC_Open_Segment_By_Key(m_pView->GetMarkupManager()->GetMarkupKey());
			HC_Open_Segment("freehand3d");
				HC_Restart_Ink();
			HC_Close_Segment();
		HC_Close_Segment();
	}
     
    m_pView->Update();    
    return (HOP_OK);
}


int HOpMarkupFreehand3D::OnLButtonUp(HEventInfo &event)
{

	if (!m_bOpStarted) 
	    return HBaseOperator::OnLButtonDblClk(event);

	// Pack the Operators Polyline point array and count
	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	    HC_Open_Segment_By_Key(m_pView->GetMarkupManager()->GetMarkupKey());
		HC_Open_Segment("freehand3d");
		  HC_Restart_Ink();
	        HC_Close_Segment();
	    HC_Close_Segment();
	HC_Close_Segment();

	// Clean up HOOPS segment tree

	m_pView->Update();

	m_bOpStarted = false;

	return(HOP_READY);
}


void HOpMarkupFreehand3D::CreateMarkupFreehand(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message)
{
	HMarkupManager *markupManager;
	HC_KEY freehand=0;
	if ((markupManager = pView->GetMarkupManager()))
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


