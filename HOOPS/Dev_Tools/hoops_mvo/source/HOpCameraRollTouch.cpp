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
#include "HOpCameraRollTouch.h"
#include "HBhvBehaviorManager.h"


/////////////////////////////////////////////////////////////////////////////
// HOpCameraRollTouch
//
// Operator for performing a camera roll based on rotation two fingers around 
// a center point
//




HOpCameraRollTouch::HOpCameraRollTouch(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LightFollowsCamera = false;
}

HOpCameraRollTouch::~HOpCameraRollTouch()
{
}


HBaseOperator * HOpCameraRollTouch::Clone()
{
	return (HBaseOperator *)new HOpCameraRollTouch(GetView());
}



const char * HOpCameraRollTouch::GetName() { return "HOpCameraRollTouch"; }





int HOpCameraRollTouch::OnTouchesDown(HEventInfo &event)
{

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return (HOP_OK);

    
    if (m_pView->GetTouchManager()->GetActiveTouchCount() < 2)
		return HBaseOperator::OnTouchesDown(event);
    
	if (!OperatorStarted()) 
		SetOperatorStarted(true);

    
    HTouchInfo *touches[2];
    
    m_pView->GetTouchManager()->GetTouches(touches,2);

    
    m_LastTouch1 = *touches[0];
    m_LastTouch2 = *touches[1];
    
    HPoint p1(m_LastTouch1.m_x, m_LastTouch1.m_y,0);
    HPoint p2(m_LastTouch2.m_x, m_LastTouch2.m_y,0);
    
    
    
    
    
    SetNewPoint(event.GetMouseWorldPos()); 

	SetFirstPoint(GetNewPoint());

	GetView()->PrepareForCameraChange();

 
    return (HOP_OK);
}



int HOpCameraRollTouch::OnTouchesMove(HEventInfo &event)
{
	
    if (!OperatorStarted()) 
		return HBaseOperator::OnTouchesMove(event);

    
    if (m_pView->GetTouchManager()->GetActiveTouchCount() < 2)
		return (HOP_OK);
    
    HTouchInfo *touches[2];
    
    m_pView->GetTouchManager()->GetTouches(touches,2);

    

    HPoint p1(m_LastTouch1.m_x, m_LastTouch1.m_y,0);
    HPoint p2(m_LastTouch2.m_x, m_LastTouch2.m_y,0);
    
    HPoint delta1 = p2 - p1;
    
    HPoint p3(touches[0]->m_x, touches[0]->m_y,0);
    HPoint p4(touches[1]->m_x, touches[1]->m_y,0);
    
    HPoint modelcenter((p3.x + p4.x)/2.0f,(p3.y + p4.y)/2.0f,(p3.z + p4.z)/2.0f);
    
    HPoint delta2 = p4 - p3;
    
    
    
    float angle = HUtility::ComputeAngleBetweenVector(delta1, delta2);
     
    angle= (atan2(delta2.x,delta2.y) - atan2(delta1.x,delta1.y)) * 180.0f/3.1415927f;
    m_LastTouch1 = *touches[0];
    m_LastTouch2 = *touches[1];
    
    
    HC_Open_Segment_By_Key (GetView()->GetSceneKey());
	    HPoint camera, target;
    HC_Compute_Coordinates(".", "local window", &modelcenter,"viewpoint", &modelcenter);
    HC_Dolly_Camera(modelcenter.x, modelcenter.y, modelcenter.z);
	#ifdef TARGET_OS_ANDROID
		HC_Roll_Camera(-angle);
	#else
	HC_Roll_Camera(angle);
	#endif
    HC_Dolly_Camera(-modelcenter.x, -modelcenter.y, -modelcenter.z);
			 
	HC_Close_Segment();

	GetView()->CameraPositionChanged();

	GetView()->Update();
	return (HOP_OK);
}




int HOpCameraRollTouch::OnTouchesUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
	    return HBaseOperator::OnTouchesUp(event);

 	SetOperatorStarted(false);
	GetView()->CameraPositionChanged(true, true);

    return(HOP_READY);
}
