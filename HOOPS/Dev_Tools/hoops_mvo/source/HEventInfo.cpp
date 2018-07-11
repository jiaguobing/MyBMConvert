// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HEventInfo.cpp : implementation of the HEventInfo class
//

#include <math.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HUtility.h"
#include "vlist.h"


/////////////////////////////////////////////////////////////////////////////
// HEventInfo


HEventInfo::HEventInfo( HBaseView* view ) 
{	
    m_EventType = HE_NoEvent; 
    m_time = 0.0F; 
    m_flags = 0; 
    m_pView = view;
    m_TimerId = 0;
}


void HEventInfo::SetPoint(HEventType NewType, int x, int y, unsigned int NewFlags) 
{
  
    SetType(NewType);
    m_flags = NewFlags;
    m_time = HUtility::GetTime();

    m_ptPixel.x = (float)x; m_ptPixel.y = (float)y; m_ptPixel.z = 0.0F;

    HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	HC_Compute_Coordinates(".","local pixels", &m_ptPixel, "outer window", &m_ptWindow);
	HC_Compute_Coordinates(".","local pixels", &m_ptPixel, "world", &m_ptWorld);
	HC_Compute_Coordinates(".","local pixels", &m_ptPixel, "viewpoint", &m_ptViewpoint);
    HC_Close_Segment();

    // Note: m_ptWindow.z is being set to zero for backward compatilbility reasons. This function
    // is deprecated, you sould use SetPixelPos function.
    m_ptWindow.z = 0.0F;
    HUtility::ClampPointToWindow(&m_ptWindow);
}

void HEventInfo::SetPixelPos(HEventType NewType, const HPoint& NewPoint, unsigned int NewFlags)
{
    SetType(NewType);
    m_flags = NewFlags;
    m_time = HUtility::GetTime();

    m_ptPixel = NewPoint;

    HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	HC_Compute_Coordinates(".","local pixels", &m_ptPixel, "outer window", &m_ptWindow);
	HC_Compute_Coordinates(".","local pixels", &m_ptPixel, "world", &m_ptWorld);
	HC_Compute_Coordinates(".","local pixels", &m_ptPixel, "viewpoint", &m_ptViewpoint);
    HC_Close_Segment();

    HUtility::ClampPointToWindow(&m_ptWindow);
}

void HEventInfo::SetWindowPos(HEventType NewType, const HPoint& NewPoint, unsigned int NewFlags)
{
    SetType(NewType);
    m_flags = NewFlags;
    m_time = HUtility::GetTime();

    m_ptWindow = NewPoint;

    HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	HC_Compute_Coordinates(".","outer window", &m_ptWindow, "local pixels", &m_ptPixel);
	HC_Compute_Coordinates(".","outer window", &m_ptWindow, "world", &m_ptWorld);
	HC_Compute_Coordinates(".","outer window", &m_ptWindow, "viewpoint", &m_ptViewpoint);
    HC_Close_Segment();

    HUtility::ClampPointToWindow(&m_ptWindow);
}

void HEventInfo::SetWorldPos(HEventType NewType, const HPoint& NewPoint, unsigned int NewFlags)
{
    SetType(NewType);
    m_flags = NewFlags;
    m_time = HUtility::GetTime();

    m_ptWorld = NewPoint;

    HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	HC_Compute_Coordinates(".","world", &m_ptWorld, "local pixels", &m_ptPixel);
	HC_Compute_Coordinates(".","world", &m_ptWorld, "outer window", &m_ptWindow);
	HC_Compute_Coordinates(".","world", &m_ptWorld, "viewpoint", &m_ptViewpoint);
    HC_Close_Segment();

    HUtility::ClampPointToWindow(&m_ptWindow);
}

void HEventInfo::SetViewpointPos(HEventType NewType, const HPoint& NewPoint, unsigned int NewFlags)
{
    SetType(NewType);
    m_flags = NewFlags;
    m_time = HUtility::GetTime();

    m_ptViewpoint = NewPoint;

    HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	HC_Compute_Coordinates(".","viewpoint", &m_ptViewpoint, "local pixels", &m_ptPixel);
	HC_Compute_Coordinates(".","viewpoint", &m_ptViewpoint, "outer window", &m_ptWindow);
	HC_Compute_Coordinates(".","viewpoint", &m_ptViewpoint, "world", &m_ptWorld);
    HC_Close_Segment();

    HUtility::ClampPointToWindow(&m_ptWindow);
}

void HEventInfo::SetKey(HEventType NewType,unsigned int nChar, unsigned int nRepCnt, unsigned int NewFlags)
{
    SetType(NewType);
    m_flags = NewFlags;
    m_Char = nChar;
    m_RepCount = nRepCnt;
}


void HEventInfo::SetTimer(HEventType NewType,unsigned int nIDEvent)
{
    SetType(NewType);
    m_TimerId = nIDEvent;
}

void HEventInfo::SetMouseWheelDelta(int wheeldelta)
{
    m_WheelDelta = wheeldelta;
}


HTouchManager::HTouchManager()
{
    m_TouchList = new_vlist(malloc,free);   
    m_ActiveTouches = 0;
}

HTouchManager::~HTouchManager()
{
	delete_vlist(m_TouchList);
    
}


void HTouchManager::ClearTouchActivity()
{
    START_LIST_ITERATION(HTouchInfo,m_TouchList);
    temp->m_bActive = false;
    END_LIST_ITERATION(m_TouchList);
    m_ActiveTouches = 0;

}

void HTouchManager::ClearTouches()
{
    vlist_flush(m_TouchList);
    m_ActiveTouches = 0;
}

HTouchInfo *HTouchManager::AddTouch(float x, float y, float timestamp, int taps, MVO_POINTER_SIZED_UINT ident)
{
    
    HTouchInfo *res = 0;
    START_LIST_ITERATION(HTouchInfo,m_TouchList);
    if (temp->m_id == ident)  
         res = temp;
     END_LIST_ITERATION(m_TouchList);
    
    if (!res)
    {
         res = new HTouchInfo();
         vlist_add_last(m_TouchList,res);        
    }

    res->Init(x,y,timestamp,taps, ident);        
    m_ActiveTouches++;
	if (m_ActiveTouches > m_TouchList->count)
		m_ActiveTouches = m_TouchList->count;

    return res;
}

int HTouchManager::GetTouches(HTouchInfo **touches, int maxtouches, bool ActiveOnly)
{
    int i = 0;
    START_LIST_ITERATION(HTouchInfo,m_TouchList);
    if ((!ActiveOnly || temp->m_bActive) && i<maxtouches)
    {
        touches[i++] = temp;
    }
    END_LIST_ITERATION(m_TouchList);
    
    return i;
}

HTouchInfo *HTouchManager::GetTouch(int index, bool activeOnly)
{
	HTouchInfo *touch = NULL;
	int foundIndex = 0;
	START_LIST_ITERATION(HTouchInfo,m_TouchList);
	if ((!activeOnly || temp->m_bActive))
	{
		if (index == foundIndex)
		{
			touch = temp;
			break;
		}
		foundIndex++;
	}
	END_LIST_ITERATION(m_TouchList);

	return touch;
}

bool HTouchManager::RemoveTouch(MVO_POINTER_SIZED_UINT ident)
{
    START_LIST_ITERATION(HTouchInfo,m_TouchList);
    if (temp->m_id == ident)        
    {
        delete temp;
        vlist_remove_at_cursor(m_TouchList);        
		if (temp->m_bActive)
			m_ActiveTouches--;
         return true;
    }                     
    END_LIST_ITERATION(m_TouchList);

    return false;        
}

int HTouchManager::GetTouchCount()
{ 
    return m_TouchList->count; 
}

