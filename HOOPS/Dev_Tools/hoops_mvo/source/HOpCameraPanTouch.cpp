// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraPan.cpp : implementation of the HOpCameraPan class
//

#include <string.h>
#include <math.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraPanTouch.h"
#include "HBhvBehaviorManager.h"


/////////////////////////////////////////////////////////////////////////////
// HOpCameraPanTouch
//
// Operator for panning the scene's camera based on the user performing a two finger drag
//
//



HOpCameraPanTouch::HOpCameraPanTouch(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LightFollowsCamera = false;
}

HOpCameraPanTouch::~HOpCameraPanTouch()
{
}


HBaseOperator * HOpCameraPanTouch::Clone()
{
	return (HBaseOperator *)new HOpCameraPanTouch(GetView());
}



const char * HOpCameraPanTouch::GetName() { return "HOpCameraPanTouch"; }



int HOpCameraPanTouch::OnTouchesDown(HEventInfo &event)
{

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return (HOP_OK);

	if (!OperatorStarted()) 
		SetOperatorStarted(true);

    if (m_pView->GetTouchManager()->GetActiveTouchCount() < 2)
		return HBaseOperator::OnTouchesDown(event);
    
    
    HTouchInfo *touches[2];

    m_pView->GetTouchManager()->GetTouches(touches,2);
    
   

    HPoint touchpoint((touches[0]->m_x+touches[1]->m_x)/2.0f, (touches[0]->m_y+touches[1]->m_y)/2.0f,0);
    HC_Open_Segment_By_Key(GetView()->GetSceneKey());
    HC_Compute_Coordinates(".", "local window", &touchpoint, "viewpoint", &touchpoint);
    HC_Close_Segment();
    
    SetNewPoint(touchpoint); 

	SetFirstPoint(GetNewPoint());

	GetView()->PrepareForCameraChange();

 
    return (HOP_OK);
}



int HOpCameraPanTouch::OnTouchesMove(HEventInfo &event)
{
	
    if (!OperatorStarted()) 
		return HBaseOperator::OnTouchesMove(event);

    if (m_pView->GetTouchManager()->GetActiveTouchCount() < 2)
		return HBaseOperator::OnTouchesMove(event);

    
    HTouchInfo *touches[2];
    
    m_pView->GetTouchManager()->GetTouches(touches,2);
    
    HPoint touchpoint((touches[0]->m_x+touches[1]->m_x)/2.0f, (touches[0]->m_y+touches[1]->m_y)/2.0f,0);
    HC_Open_Segment_By_Key(GetView()->GetSceneKey());
    HC_Compute_Coordinates(".", "local window", &touchpoint, "viewpoint", &touchpoint);
    HC_Close_Segment();

    
    SetNewPoint(touchpoint);  

	HPoint delta(GetNewPoint() - GetFirstPoint());

    HC_Open_Segment_By_Key (GetView()->GetSceneKey());
	    HPoint camera, target;			 
    HC_Dolly_Camera(-delta.x, -delta.y, 0);
     

	HC_Close_Segment();

	GetView()->CameraPositionChanged();
    SetNewPoint(event.GetMouseViewpointPos()); 
    
	SetFirstPoint(GetNewPoint());

	GetView()->Update();
	return (HOP_OK);
}




int HOpCameraPanTouch::OnTouchesUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
	    return HBaseOperator::OnTouchesUp(event);

 	SetOperatorStarted(false);
	GetView()->CameraPositionChanged(true, true);

    return(HOP_READY);
}
