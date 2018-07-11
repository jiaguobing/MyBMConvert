// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraManipulate.cpp : implementation of the HOpCameraManipulate class
//


#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HEventManager.h"
#include "HOpCameraManipulate.h"
#include "HOpCameraPan.h"
#include "HOpCameraZoom.h"
#include "HOpCameraRelativeOrbit.h"
#include "HUtility.h"
#include "HUtilityGeomHandle.h"
#include "HEventListener.h"
#include "HEventManager.h"
#include "HBhvBehaviorManager.h"
#include "HSelectionSet.h"



/////////////////////////////////////////////////////////////////////////////
// HSOpCameraWalk
//
// Operator for walking in a scene.  
// 
//
// Left button down - initiates free viewing in all directions with mouse movement
//
// Left + Right Mouse button - movement in view direction (reversed by pressing shift)
// 


// timer callback to say walk further
static bool DblClkTimer(float request_time, float actual_time, void * user_data)
{
	HOpCameraManipulate * op = static_cast<HOpCameraManipulate *>(user_data);
	op->HandleDblClk(request_time, actual_time);
	return true;
}



void HOpCameraManipulate::HandleDblClk(float requested_time, float actual_time)
{
	UNREFERENCED(requested_time);
	UNREFERENCED(actual_time);

	static bool first = true;
	double time_diff;

	if (first)
	{
		m_time = HUtility::GetTime();
		first = false;
	}
	time_diff = HUtility::GetTime() - m_time;

	if (m_bLButtonDown && m_bLButtonUp && time_diff > 0.2f)
	{
		m_bForceDown = true;
		OnLButtonUp(*m_LastEvent);
		m_time = HUtility::GetTime();
	}
	else
	{
		if (!m_bLButtonDown)
		m_time = HUtility::GetTime();
	}
}

 /////////////////////////////////////////////////////////////////////////////
// HOpCameraManipulate
//
// Operator that combines orbit pan and zoom operators into one
//
// No Keys  = Orbit Operator
//
// Shift    = Pan Operator
//
// Ctrl	    = Zoom Operator


//
HOpCameraManipulate::HOpCameraManipulate(
	HBaseView * view,
	int DoRepeat,
	int DoCapture,
	HBaseOperator * OrbitOperator,
	HBaseOperator * PanOperator,
	HBaseOperator * ZoomOperator,
	HBaseOperator * RelativeOrbitOperator,
	bool AllowHandleActivation)
	: HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LastEvent = new HEventInfo(view);
	m_bForceDown = false;
	m_bLButtonUp = false;
	m_bAllowHandleActivation = AllowHandleActivation;
	m_pDblClkTimer = new HTClient( 0.01f, HTCS_PeriodicSkip, DblClkTimer, this );
	HDB::GetHTManager()->RegisterClient( m_pDblClkTimer );

	if (!OrbitOperator)
		m_pOrbitOperator = new HOpCameraOrbit(view, DoRepeat, DoCapture);
	else
		m_pOrbitOperator = OrbitOperator;
	if(!PanOperator)
		m_pPanOperator = new HOpCameraPan(view, DoRepeat, DoCapture);
	else
		m_pPanOperator = PanOperator;
	
	if (!ZoomOperator)
		m_pZoomOperator = new HOpCameraZoom(view, DoRepeat, DoCapture);
	else
		m_pZoomOperator =  ZoomOperator;

	if (!RelativeOrbitOperator)
		m_pRelativeOrbitOperator = new HOpCameraRelativeOrbit(view, DoRepeat, DoCapture);
	else
		m_pRelativeOrbitOperator = RelativeOrbitOperator;

	m_bLButtonDown = false;
	m_time = HUtility::GetTime();

}

HOpCameraManipulate::~HOpCameraManipulate()
{
	HDB::GetHTManager()->UnRegisterClient( m_pDblClkTimer );
	delete m_pDblClkTimer;
	delete m_pOrbitOperator;
	delete m_pPanOperator;
	delete m_pZoomOperator;
	delete m_pRelativeOrbitOperator;
	delete m_LastEvent;
}


HBaseOperator * HOpCameraManipulate::Clone()
{
	return new HOpCameraManipulate(GetView());
}


const char * HOpCameraManipulate::GetName() {
	return "HOpCameraManipulate";
}


/////////////////////////////////////////////////////////////////////////////
// HOpCameraManipulate message handlers

int HOpCameraManipulate::OnLButtonDown(HEventInfo &event)
{
	UNREFERENCED(event);

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;

	m_bLButtonDown = true;
	return HOP_OK;
}



int HOpCameraManipulate::OnLButtonDownAndMove(HEventInfo &event)
{
	if (m_bLButtonDown)
	{
		m_bLButtonDown = false;

		if (event.Shift())
			return m_pPanOperator->OnLButtonDown(event);
		else if (event.Control())
			return m_pZoomOperator->OnLButtonDown(event);
		else
			return m_pOrbitOperator->OnLButtonDown(event);
	}

	if (event.Shift())
	{
		if (!m_pPanOperator->OperatorStarted())
		{
			m_pOrbitOperator->OnLButtonUp(event);
			m_pZoomOperator->OnLButtonUp(event);
			m_pPanOperator->OnLButtonDown(event);
		}
		return m_pPanOperator->OnLButtonDownAndMove(event);
	}
	else if (event.Control())
	{
		if (!m_pZoomOperator->OperatorStarted())
		{
			m_pOrbitOperator->OnLButtonUp(event);
			m_pPanOperator->OnLButtonUp(event);
			m_pZoomOperator->OnLButtonDown(event);
		}
		return m_pZoomOperator->OnLButtonDownAndMove(event);
	}
	else 
	{
		if (!m_pOrbitOperator->OperatorStarted())
		{
			m_pPanOperator->OnLButtonUp(event);
			m_pZoomOperator->OnLButtonUp(event);
			m_pOrbitOperator->OnLButtonDown(event);
		}
		return m_pOrbitOperator->OnLButtonDownAndMove(event);
	}
}


int HOpCameraManipulate::OnMButtonDown(HEventInfo &event)
{

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;

	return m_pRelativeOrbitOperator->OnLButtonDown(event);

}


int HOpCameraManipulate::OnRButtonDown(HEventInfo &event)
{

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;

	return m_pPanOperator->OnLButtonDown(event);

}


int HOpCameraManipulate::OnMButtonDownAndMove(HEventInfo &event)
{

	return m_pRelativeOrbitOperator->OnLButtonDownAndMove(event);

}
int HOpCameraManipulate::OnRButtonDownAndMove(HEventInfo &event)
{

	return m_pPanOperator->OnLButtonDownAndMove(event);

}

int HOpCameraManipulate::OnMButtonUp(HEventInfo &event)
{
	return m_pRelativeOrbitOperator->OnLButtonUp(event);
}

int HOpCameraManipulate::OnRButtonUp(HEventInfo &event)
{
	return m_pPanOperator->OnLButtonUp(event);
}


int HOpCameraManipulate::OnLButtonUp(HEventInfo &event)
{
	HPoint p;
	p = event.GetMouseWindowPos();

	if (m_bLButtonDown)
	{ 
		if (m_bAllowHandleActivation)
		{	
			if (m_bLButtonUp && !m_bForceDown)
			{
				m_bLButtonUp = false;
				m_bLButtonDown = false;
				GetView()->SetDynamicHighlighting(false);
				GetView()->GetSelection()->DeSelectAll();
				HC_KEY sel = HUtility::SelectUnderCursor(GetView(), p.x, p.y, false);
				if (sel != -1)
				{

					HC_Open_Segment_By_Key(HUtility::GrabSegment(sel));
					if (HUtility::SegmentExists("cpbox"))
						HUtilityGeomHandle::ClearAllHandles(0);
					else
					{
						sel = HUtility::GrabSegment(sel);
						HLISTENER_EVENT(HObjectManipulationListener, GetView()->GetEventManager(), SetupHandlesEvent(GetView(), sel, false));
					}
					HC_Close_Segment();

					GetView()->Update();
				}
			}
			else if (m_bForceDown)
			{
				m_bForceDown = false;
				HSelectionSet* selection = GetView()->GetSelection();
				if (!event.Control() && !event.Shift())
				{
					selection->DeSelectAll();
					GetView()->EmitDeSelectAllMessage();
				}

				HPoint p;

				m_bLButtonDown = false;
				m_bLButtonUp = false;
				GetView()->Update();
			}
			else
			{
				m_bLButtonUp = true;
				m_time = HUtility::GetTime();
				*m_LastEvent = event;
			}
		}
	}
	else
	{

		HPoint lastPoint;
		lastPoint = event.GetMouseWorldPos();
		if (event.Shift())
			return m_pPanOperator->OnLButtonUp(event);
		else if (event.Control())
			return m_pZoomOperator->OnLButtonUp(event);
		else
			return m_pOrbitOperator->OnLButtonUp(event);
	}

	return HOP_OK;

}


bool HOpCameraManipulate::OperatorStarted()
{
	return m_pOrbitOperator->OperatorStarted()
		|| m_pZoomOperator->OperatorStarted()
		|| m_pPanOperator->OperatorStarted();
}
