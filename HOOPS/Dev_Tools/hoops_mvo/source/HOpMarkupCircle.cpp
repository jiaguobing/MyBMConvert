// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupCircle.cpp : implementation of the HOpMarkupCircle class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HOpConstructCircle.h"
#include "HOpMarkupCircle.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HUtility.h"
#include "HMarkupManager.h"
#include "HImManager.h"

  
HOpMarkupCircle::HOpMarkupCircle(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructCircle(view, DoRepeat, DoCapture, false)
{
}

HOpMarkupCircle::~HOpMarkupCircle()
{
}

HBaseOperator * HOpMarkupCircle::Clone()
{
	return new HOpMarkupCircle(GetView());
}



const char * HOpMarkupCircle::GetName() { return "HOpMarkupCircle"; }



int HOpMarkupCircle::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

	HOpConstructCircle::OnLButtonUp(event);

    SetNewPoint(event.GetMouseViewpointPos());

//  make certain that new_point != first_point 
	if (GetNewPoint() == GetFirstPoint())
		return HOP_CANCEL;

	HMarkupManager *markupManager;
	HC_KEY circle=0;
	if ((markupManager = GetView()->GetMarkupManager()) != 0)
	{
		HC_KEY activelayerkey;
		if ((activelayerkey = markupManager->GetCurrentLayerKey()) == -1)
			activelayerkey = markupManager->OpenLayer("");
			
		HC_Open_Segment_By_Key(activelayerkey); {
			circle = HC_KOpen_Segment("circle"); {
			    	// this is now hoops default behaviour but might be useful for "no absolute line weight"
				//HImSetCallback("draw dc edge","HImUtility_device_independent_polyline_weights");
				HC_Set_Heuristics("no backplane culling");
				HUtility::CreateAxisCircle(GetFirstPoint(), m_radius, HUtility::XY);
				HC_Set_Rendering_Options("no frame buffer effects");
			} HC_Close_Segment();
		}HC_Close_Segment();

		// Send the Markup to HNet if we're sending messages
		if(GetView()->EmittingMessages())
			markupManager->EmitMarkupSegment(circle);
	}

    GetView()->Update(); 
    SetOperatorStarted(false); 
	
    return HOP_READY;
}
		
