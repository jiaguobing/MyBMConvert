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
#include "HOpCameraPan.h"
#include "HBhvBehaviorManager.h"


/////////////////////////////////////////////////////////////////////////////
// HOpCameraPan
//
// Operator for panning the scene's camera based on the user dragging the mouse
// with the left button down
//
// Left button down - find first position and bounds object with box
//
// Mouse motion while down - pans camera based on difference between previous and 
// current world space point
//



HOpCameraPan::HOpCameraPan(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LightFollowsCamera = false;
}

HOpCameraPan::~HOpCameraPan()
{
}


HBaseOperator * HOpCameraPan::Clone()
{
	return new HOpCameraPan(GetView());
}



const char * HOpCameraPan::GetName() { return "HOpCameraPan"; }



int HOpCameraPan::OnLButtonDown(HEventInfo &event)
{

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;

	if (!OperatorStarted()) 
		SetOperatorStarted(true);

    SetNewPoint(event.GetMouseWorldPos()); 

	SetFirstPoint(GetNewPoint());

	GetView()->PrepareForCameraChange();

 
    return HOP_OK;
}



int HOpCameraPan::OnLButtonDownAndMove(HEventInfo &event)
{
	
    if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

    SetNewPoint(event.GetMouseWorldPos());  

	HPoint delta(GetNewPoint() - GetFirstPoint());

    HC_Open_Segment_By_Key (GetView()->GetSceneKey());
	    HPoint camera, target;
		
		HC_Show_Net_Camera_Target (&target.x, &target.y, &target.z);
		HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
	
		HC_Set_Camera_Target (target.x - delta.x, target.y - delta.y, target.z - delta.z);
		HC_Set_Camera_Position (camera.x - delta.x, camera.y - delta.y, camera.z - delta.z);

	HC_Close_Segment();

	GetView()->CameraPositionChanged();

	GetView()->Update();
	return HOP_OK;
}




int HOpCameraPan::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
	    return HBaseOperator::OnLButtonDownAndMove(event);

 	SetOperatorStarted(false);
	GetView()->CameraPositionChanged(true, true);

    return HOP_READY;
}
