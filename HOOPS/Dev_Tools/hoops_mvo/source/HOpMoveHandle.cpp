// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HTools.h"
#include "HOpMoveHandle.h"
#include "HUtilityGeomHandle.h"
#include "HMarkupManager.h"
#include "HSelectionSet.h"
#include "HEventManager.h"
#include "HConstantFrameRate.h"
  

#include "stdio.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "hc.h"


/////////////////////////////////////////////////////////////////////////////
// HOpMoveHandle
//
// Translate items in the selection list in direction of mouse movement
// parallel to camera target plane.
//
// Currently supports translation if item if it is a segment




HOpMoveHandle::HOpMoveHandle(HBaseView* view, bool allowSelect, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{	
	HC_KEY kay[1000];
	m_pHandleKey = INVALID_KEY;
	m_pObjectKey = INVALID_KEY;
	m_bAllowSelect = allowSelect;
	if (allowSelect && 0)
	{
		HC_KEY key;
		HSelectionSet *ss = view->GetSelection();
		if (ss->GetSize())
		{
			int k;
			for (k=0;k<(int)ss->GetSize();k++)
				kay[k] = ss->GetAt(k);
			key = ss->GetAt(k);
			int msize = ss->GetSize();
			for (k=0;k<msize;k++)
			{
				key = kay[k];
				char type[MVO_BUFFER_SIZE];
				HC_Show_Key_Type(key, type);
				if (strcmp(type, "segment")!=0)
				{
					key = HC_KShow_Owner_By_Key(key);
				}
				
				ss->DeSelectAll(); 
				m_pObjectKey = key;
				SetupObject(m_pObjectKey);
				GetView()->Update();
			}
		}
	}
	
}

void HOpMoveHandle::SetupObject(HC_KEY key)
{  
	HUtilityGeomHandle::SetupManipulator(key, GetView(),0); 
}


HOpMoveHandle::~HOpMoveHandle()
{
//	HUtilityGeomHandle::ClearAllHandles(GetView());
 
	GetView()->Update();
}


HBaseOperator * HOpMoveHandle::Clone()
{
	return new HOpMoveHandle(GetView());
}

void HOpMoveHandle::Reset()
{
	m_pHandleKey = INVALID_KEY;
	m_pObjectKey = INVALID_KEY;

}


const char * HOpMoveHandle::GetName() { return "HOpMoveHandle"; }
 
/////////////////////////////////////////////////////////////////////////////
// HOpMoveHandle message handlers



HC_KEY HOpMoveHandle::FindHandle(HEventInfo &event)
{  
	HPoint new_pos = event.GetMouseWindowPos();
	return HUtilityGeomHandle::SelectHandle(GetView(), new_pos);

}
 

HC_KEY HOpMoveHandle::FindObject(HEventInfo & event, HC_KEY oldkey, bool exclude)
{
	HPoint  new_pos;  
	new_pos = event.GetMouseWindowPos();
	return HUtilityGeomHandle::SelectObject(GetView(),new_pos, oldkey, exclude);
}

 
int HOpMoveHandle::OnLButtonDown(HEventInfo & event)
{
	HUtilityGeomHandle ghandle;
	int ret = HLISTENER_PASS_EVENT;
	if (!OperatorStarted()) 
		SetOperatorStarted(true);
	else return ret;

	m_pHandleKey =FindHandle(event);
 

	if (m_pHandleKey == INVALID_KEY)
	{
	
		if (m_bAllowSelect == false)
		{
			SetOperatorStarted(false);
			return ret;
		}

 		HC_KEY okey = FindObject(event, m_pObjectKey, true);
		if (okey!=m_pObjectKey)
		{
			if (m_pObjectKey != INVALID_KEY)
			{
 				HC_Open_Segment_By_Key(m_pObjectKey);
				HC_Begin_Contents_Search(".", "segments");
				HC_KEY skey;
				char type[MVO_BUFFER_SIZE];
				while (HC_Find_Contents(type, &skey))
				{
					char text[MVO_BUFFER_SIZE];
					char ssegname[MVO_SEGMENT_PATHNAME_BUFFER];

						HC_Show_Segment(skey, text);
						HC_Parse_String(text, "/", -1, ssegname);

					if (ghandle.IsGeomHandle(skey ) != INVALID_KEY ||  strcmp(ssegname, "cpbox") == 0)
					{
						char text[MVO_BUFFER_SIZE];
						HC_Show_Segment(skey, text);
						HC_Delete_By_Key(skey);
					}

				}
				HC_End_Contents_Search();
				HC_Close_Segment();
			}
			m_pHandleKey = INVALID_KEY;

			m_pObjectKey = okey;
			if (m_pObjectKey != INVALID_KEY)
				SetupObject(m_pObjectKey);

		}
		SetOperatorStarted(false);
	}
	else
	{
		if (ghandle.GetConstraintType(m_pHandleKey) == HConstrainToOnOff)
		{
			HC_KEY owner = HC_KShow_Owner_By_Key(m_pHandleKey);
			HC_Delete_By_Key(owner);
			m_pHandleKey = INVALID_KEY;
			m_pObjectKey = INVALID_KEY;
			SetOperatorStarted(false);

		}
		else
		{
		    GetView()->GetConstantFrameRateObject()->SetActivityType(MovementActivity);
			ghandle.ShowHandle(m_pHandleKey,0.7f, m_pView);
			m_pObjectKey = HUtilityGeomHandle::GetMMSegment(m_pHandleKey);
			
			if( !GetView()->GetAllowInteractiveShadows() && (m_TempShadowMode = GetView()->GetShadowMode()) != HShadowNone )
				GetView()->SetShadowMode(HShadowNone);
			if( !GetView()->GetAllowInteractiveCutGeometry() && (m_TempCutGeometryVisibility = GetView()->GetCutGeometryVisibility()) != HCutGeometryVisibilityNone)
				GetView()->SetCutGeometryVisibility(HCutGeometryVisibilityNone);

		}
		ret = HLISTENER_CONSUME_EVENT;
	}
	if (m_pObjectKey != INVALID_KEY)
	{
		HC_Open_Segment_By_Key(m_pObjectKey);{
			if (GetView()->GetSpritingMode())
 				HC_Set_Heuristics("quick moves = spriting");	
		}HC_Close_Segment();
	}

	
    SetNewPoint(event.GetMouseWorldPos());
	SetFirstPoint(event.GetMouseWindowPos());

	if (m_pHandleKey != INVALID_KEY)
	{
		HC_KEY keys[MVO_BUFFER_SIZE];
		int kcount = HUtility::CalculateKeyPath(m_pHandleKey, GetView(), keys, MVO_BUFFER_SIZE);

		HPoint objSpaceHandleCenter;
		ghandle.GetCenterPoint(m_pHandleKey, objSpaceHandleCenter);

		float matrix[16];

		HC_Compute_Transform_By_Path(kcount, keys, "object", "outer window", matrix);
		HPoint winSpaceHandleCenter;
		HC_Compute_Transformed_Points(1, &objSpaceHandleCenter, matrix, &winSpaceHandleCenter);

		HPoint windowPos = event.GetMouseWindowPos();
		m_windowspaceDelta = winSpaceHandleCenter - windowPos;
		m_windowspaceDelta.z = 0;
	}
	else {
		m_windowspaceDelta = HPoint();
	}
	
	GetView()->Update();
	
    return ret;
}


int HOpMoveHandle::OnNoButtonDownAndMove(HEventInfo & event)
{
	int ret = HLISTENER_PASS_EVENT;
	HUtilityGeomHandle ghandle;
	HC_KEY ghandleKey = FindHandle(event);

	if (m_pHandleKey != INVALID_KEY && ghandleKey != m_pHandleKey)
	{		
		ghandle.ResetHandle(m_pHandleKey);
		GetView()->Update();
	}

	m_pHandleKey = ghandleKey;

	if (ghandleKey != INVALID_KEY)
	{
		ret = HLISTENER_CONSUME_EVENT;
		ghandle.ShowHandle(ghandleKey, 0.7f, m_pView);
		GetView()->Update();	
	}	

	return ret;
}


int HOpMoveHandle::OnLButtonDownAndMove(HEventInfo & event)
{
	if (!OperatorStarted()) return HLISTENER_PASS_EVENT;

    SetNewPoint(event.GetMouseWorldPos());
	HPoint tmp = event.GetMouseWindowPos();

	HUtilityGeomHandle ghandle;
	if (ghandle.GetConstraintType(m_pHandleKey) == HConstrainToPlane ||
		ghandle.GetConstraintType(m_pHandleKey) == HConstrainToRay ||
		ghandle.GetConstraintType(m_pHandleKey) == HConstrainToViewplane)
	{
		HPoint const windowPos = event.GetMouseWindowPos();

		HPoint adjustedWindowPos = windowPos + m_windowspaceDelta;
		HPoint adjustedWorldPos;
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
			HC_Compute_Coordinates(".", "outer window", &adjustedWindowPos, "world", &adjustedWorldPos);
		}HC_Close_Segment();

		if (ghandle.GetConstraintType(m_pHandleKey) == HConstrainToRay)
		{
			ghandle.AdjustPointToRay(m_pHandleKey, adjustedWorldPos, GetView()->GetSceneKey(), GetView());
			SetNewPoint(adjustedWorldPos);
		}

		ghandle.Move(m_pHandleKey, adjustedWorldPos, GetView()->GetSceneKey(), GetView());
	}
	else
	{
		if (ghandle.GetConstraintType(m_pHandleKey) == HConstrainToRotationVector)
		{
 			ghandle.Rotate(m_pHandleKey, (tmp.x - GetFirstPoint().x)*360.0f);
		}
		else
		{
			float sf = 1.0f - (tmp.x - GetFirstPoint().x)*5.0f;
			ghandle.Scale(m_pHandleKey,sf,sf,sf);
		}
	}
	HLISTENER_EVENT(HObjectManipulationListener, GetView()->GetEventManager(), ObjectMovedEvent(GetView(), m_pObjectKey,0));
 

	SetFirstPoint(tmp);
	GetView()->UpdateHighlighting();
	GetView()->SetGeometryChanged();
  	GetView()->Update();
	return HLISTENER_CONSUME_EVENT;
}





int HOpMoveHandle::OnLButtonUp(HEventInfo &event)
{

	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonUp(event);
	SetOperatorStarted(false);
	if (m_pObjectKey!= INVALID_KEY)
	{	
	HC_Open_Segment_By_Key(m_pObjectKey);
			    if (HUtility::ShowOneHeuristic("quick moves")) {
					    HC_UnSet_One_Heuristic("quick moves");
				    }
				HC_Close_Segment();
	}

	HUtilityGeomHandle ghandle;
	ghandle.ResetHandle(m_pHandleKey);
	if (m_pHandleKey != INVALID_KEY)
	{	
		if( !GetView()->GetAllowInteractiveCutGeometry() && m_TempCutGeometryVisibility != HCutGeometryVisibilityNone)
			GetView()->SetCutGeometryVisibility(m_TempCutGeometryVisibility);
		/* Drawing the shadows and cut geometry over again takes a long time, lets take them one at a time... */ 
	 	GetView()->ForceUpdate();
		if( !GetView()->GetAllowInteractiveShadows() && m_TempShadowMode != HShadowNone ) 
			GetView()->SetShadowMode(m_TempShadowMode);

	}

	m_pHandleKey = INVALID_KEY;
 	GetView()->Update();
    return HOP_READY;
}

 
