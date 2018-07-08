// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraManipulateTouch.cpp : implementation of the HOpCameraManipulateTouch class
//


#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HEventManager.h"
#include "HOpCameraManipulateTouch.h"
#include "HOpCameraOrbit.h"
#include "HOpCameraPanTouch.h"
#include "HOpCameraZoomTouch.h"
#include "HOpCameraRollTouch.h"
#include "HUtility.h"
#include "HUtilityGeomHandle.h"
#include "HEventListener.h"
#include "HEventManager.h"
#include "HBhvBehaviorManager.h"
#include "HSelectionSet.h"
#include "HOpSelectArea.h"

/////////////////////////////////////////////////////////////////////////////
// HOpCameraManipulateTouch
//
// Operator that combines orbit pan and zoom operators into one
//


//
HOpCameraManipulateTouch::HOpCameraManipulateTouch(HBaseView* view, int DoRepeat,int DoCapture, HBaseOperator *OrbitOperator, HBaseOperator *PanOperator, HBaseOperator *ZoomOperator, HBaseOperator *RollOperator) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LastEvent = new HEventInfo(view);
    
	if (!OrbitOperator)
	    m_pOrbitOperator = new HOpCameraOrbit(view, DoRepeat, DoCapture);
	else
	    m_pOrbitOperator = OrbitOperator;
	if(!PanOperator)
 	    m_pPanOperator = new HOpCameraPanTouch(view, DoRepeat, DoCapture);
	else
	    m_pPanOperator = PanOperator;
	
	if (!ZoomOperator)
	    m_pZoomOperator = new HOpCameraZoomTouch(view, DoRepeat, DoCapture);
	else
	    m_pZoomOperator =  ZoomOperator;
    
	if (!RollOperator)
	    m_pRollOperator = new HOpCameraRollTouch(view, DoRepeat, DoCapture);
	else
	    m_pRollOperator =  RollOperator;
    
	m_time = HUtility::GetTime();
    
}

HOpCameraManipulateTouch::~HOpCameraManipulateTouch()
{
    delete m_pOrbitOperator;
	delete m_pPanOperator;
	delete m_pZoomOperator;
	delete m_pRollOperator;
}


HBaseOperator * HOpCameraManipulateTouch::Clone()
{
	return (HBaseOperator *)new HOpCameraManipulateTouch(GetView());
}




const char * HOpCameraManipulateTouch::GetName() { return "HOpCameraManipulateTouch"; }


/////////////////////////////////////////////////////////////////////////////
// HOpCameraManipulateTouch message handlers

int HOpCameraManipulateTouch::OnTouchesDown(HEventInfo &event)
{
    
	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return (HOP_OK);
    
    HTouchInfo *touches[1];
    
    if (m_pView->GetTouchManager()->GetTouchCount() == 0)
        HBaseOperator::OnTouchesDown(event);        

    else if (m_pView->GetTouchManager()->GetTouchCount() == 1)    
        m_pOrbitOperator->SetOperatorStarted(false); 
    
    else if (m_pView->GetTouchManager()->GetTouchCount() == 2)
    {
        m_pRollOperator->SetOperatorStarted(false); 
        m_pZoomOperator->SetOperatorStarted(false); 
        m_pPanOperator->SetOperatorStarted(false);         
    }
    
    m_pView->GetTouchManager()->GetTouches(touches,1);
    
    if (touches[0]->m_taps ==2)
    {
        m_pView->FitWorld();
        m_pView->Update();
        return (HOP_OK);
    }
    
        
 	return HOP_OK;
    
}



int HOpCameraManipulateTouch::OnTouchesMove(HEventInfo &event)
{
    
    if (m_pView->GetTouchManager()->GetTouchCount() == 0)
		return HBaseOperator::OnTouchesMove(event);

    else if (m_pView->GetTouchManager()->GetTouchCount() == 1)
    {
        if (!m_pOrbitOperator->OperatorStarted())
            m_pOrbitOperator->OnLButtonDown(event);        
        
        return m_pOrbitOperator->OnLButtonDownAndMove(event);
    }
    
    else if (m_pView->GetTouchManager()->GetTouchCount() == 2)
    {
        
        HTouchInfo *touches[2];

        m_pView->SetSuppressUpdate(true);

        m_pView->GetTouchManager()->GetTouches(touches,2);

            
        HPoint p1(touches[0]->m_x, touches[0]->m_y,0);
        HPoint p2(touches[1]->m_x, touches[1]->m_y,0);
        HPoint delta;        
        delta.Set((p2.x + p1.x)/2.0f, (p2.y + p1.y)/2.0f, 0.0f);
        event.SetWindowPos(HE_MouseMove, delta, MVO_LBUTTON);
        
        
        if (!m_pRollOperator->OperatorStarted())
            m_pRollOperator->OnTouchesDown(event);
        m_pRollOperator->OnTouchesMove(event);
        
              
        if (!m_pZoomOperator->OperatorStarted())
            m_pZoomOperator->OnTouchesDown(event);
        m_pZoomOperator->OnTouchesMove(event);
      
        
        if (!m_pPanOperator->OperatorStarted())
            m_pPanOperator->OnTouchesDown(event);
        m_pPanOperator->OnTouchesMove(event);
        m_pView->SetSuppressUpdate(false);

        GetView()->Update();
        
        
    }
     
    return HOP_OK;
}



int HOpCameraManipulateTouch::OnTouchesUp(HEventInfo &event)
{
    
    //    if (event.GetTouchesCount() == 0)
    //		return HBaseOperator::OnTouchesUp(event);
    
    m_pOrbitOperator->OnLButtonUp(event);
    m_pPanOperator->OnTouchesUp(event);
    m_pRollOperator->OnTouchesUp(event);
    m_pZoomOperator->OnTouchesUp(event);
    
    
    return HOP_OK;
	
	
}


bool HOpCameraManipulateTouch::OperatorStarted( void )
{
	return (m_pOrbitOperator->OperatorStarted() || m_pZoomOperator->OperatorStarted()
            || m_pPanOperator->OperatorStarted());
}
