// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraZoomTouchTouch.cpp : implementation of the HOpCameraZoomTouch class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraZoomTouch.h"
#include "HBhvBehaviorManager.h"



/////////////////////////////////////////////////////////////////////////////
// HOpCameraZoomTouch
//
// Operator for zooming the scene's camera based on touch input
//


HOpCameraZoomTouch::HOpCameraZoomTouch(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LightFollowsCamera = false;
}

HOpCameraZoomTouch::~HOpCameraZoomTouch()
{
}


HBaseOperator * HOpCameraZoomTouch::Clone()
{
	return (HBaseOperator *)new HOpCameraZoomTouch(GetView());
}



const char * HOpCameraZoomTouch::GetName() { return "HOpCameraZoomTouch"; }

/////////////////////////////////////////////////////////////////////////////
// HOpCameraZoomTouch message handlers



int HOpCameraZoomTouch::OnTouchesDown(HEventInfo &event)
{
	HPoint target;

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
    
     
	
 	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		HC_Show_Net_Camera_Target(&m_ptCamera.x,&m_ptCamera.y,&m_ptCamera.z);
		HC_Show_Net_Camera_Position(&target.x,&target.y,&target.z);	
		m_ptDist.Set(target.x-m_ptCamera.x, target.y - m_ptCamera.y, target.z - m_ptCamera.z);
		m_fLength = (float)HC_Compute_Vector_Length(&m_ptDist);
		HC_Compute_Normalized_Vector(&m_ptDist,&m_ptDist);
		HC_Show_Net_Camera_Field(&m_Width, &m_Height);
	HC_Close_Segment();

	GetView()->PrepareForCameraChange();

 	return (HOP_OK);
}



int HOpCameraZoomTouch::OnTouchesMove(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnTouchesMove(event);

    
    if (m_pView->GetTouchManager()->GetActiveTouchCount() < 2)
		return HBaseOperator::OnTouchesMove(event);

    HTouchInfo *touches[2];    
    m_pView->GetTouchManager()->GetTouches(touches,2);
    
    
    HPoint delta1(touches[1]->m_x - touches[0]->m_x,touches[1]->m_y - touches[0]->m_y,0);
    HPoint delta2(m_LastTouch2.m_x - m_LastTouch1.m_x,m_LastTouch2.m_y - m_LastTouch1.m_y,0);
    

    float l1 = static_cast<float>(HC_Compute_Vector_Length(&delta1));
    float l2 = static_cast<float>(HC_Compute_Vector_Length(&delta2));
    float zoom_factor = l1 - l2;
    

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		
    float w, h;
    //zoom_factor = GetNewPoint().y - GetFirstPoint().y;

		HPoint target, position, diff; 
		char proj[64];
 
		HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
		HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
		HC_Show_Net_Camera_Field(&w, &h);
		HC_Show_Net_Camera_Projection(proj);

		// we only want to zoom in if we are still further out than
		// the maximum zoom level and we are actually zooming in
		float maxZoomLevel = GetView()->GetZoomLimit();
		if ((w > maxZoomLevel) || (h > maxZoomLevel) || (zoom_factor<0.0f))
		{

		    diff.Set(position.x - target.x, 
			    position.y - target.y, 
			    position.z - target.z);

		    // if zoom_factor greater than 1 than we will zoom past the camera target.  Extra check to avoid unneccessary 
		    // zoom in when the camera is close to target (sign of zoom factor indicates whether trying to zoom in/out) 
			// #5241, sometimes ACIS users DO want to make zooms that small
			// if ((HC_Compute_Vector_Length(&diff) > 0.0000001))
			if (zoom_factor < 0.99)
			{
				HPoint newPos(
					m_ptCamera.x + m_ptDist.x * (m_fLength-(m_fLength*zoom_factor)),
					m_ptCamera.y + m_ptDist.y * (m_fLength-(m_fLength*zoom_factor)),
					m_ptCamera.z + m_ptDist.z * (m_fLength-(m_fLength*zoom_factor)));
				if (newPos != target)
					HC_Set_Camera_Position(newPos.x, newPos.y, newPos.z);
			}

			if (streq(proj, "orthographic"))
			{
				if (!(zoom_factor > 0.99))
					HC_Set_Camera_Field(m_Width - (m_Width * zoom_factor), m_Height - (m_Height * zoom_factor));
			}
		}

	


		 
	HC_Close_Segment();

	// tell the view that the camera position has changed
	GetView()->CameraPositionChanged();

 	GetView()->Update();
	return (HOP_OK);
}


int HOpCameraZoomTouch::OnTouchesUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnTouchesUp(event);

 	SetOperatorStarted(false);
	GetView()->CameraPositionChanged(true, true);

    return(HOP_READY);
}





