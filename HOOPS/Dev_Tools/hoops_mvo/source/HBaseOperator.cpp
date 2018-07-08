// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HBaseOperator.cpp : implementation of the HBaseOperator class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HEventListener.h"
#include "HEventManager.h"
#include "HEventListener.h"


HBaseOperator::HBaseOperator(HBaseView* view, int DoRepeat, int DoCapture)
{
	m_pView = view;
    m_doRepeat = DoRepeat;
    m_doCapture = DoCapture;
	m_bOpStarted = false;
	m_bMapTouchesToMouseEvents = false;
//	m_pView->GetEventManager()->RegisterHandler(this, HMouseListener::GetType(), 0);

}



HBaseOperator::~HBaseOperator()
{
	if (m_pView)
	{
		m_pView->GetEventManager()->UnRegisterHandler(this, HMouseListener::GetType());
		if (m_pView->GetOperator() == this)
			m_pView->SetOperator(0);
	}

}




HBaseOperator * HBaseOperator::Clone() 
{ 
	return new HBaseOperator(m_pView); 
}



const char * HBaseOperator::GetName() 
{ 
    return "<Operator>";
}


bool HBaseOperator::IsSpecialKey( HC_KEY key )
{
	char keytype[MVO_BUFFER_SIZE], token[MVO_BUFFER_SIZE];
	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];

	HC_Show_Key_Type( key, keytype );
	if (!streq(keytype, "segment")) {
		key = HC_KShow_Owner_By_Key( key );
	}

	HC_Show_Segment( key, pathname );
	HC_Parse_String(pathname, "/", -1, token); 
	if( streq(token, "cutting grids"))
		return true;
	return false;

}	


/////////////////////////////////////////////////////////////////////////////
// HBaseOperator message handlers






int HBaseOperator::OnLButtonDown(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnLButtonDownAndMove(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnLRButtonDownAndMove(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnLButtonUp(HEventInfo &event) 
{  
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnLButtonDblClk(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnMButtonDown(HEventInfo &event)
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnMButtonDownAndMove(HEventInfo &event)
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnMButtonUp(HEventInfo &event)
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnMButtonDblClk(HEventInfo &event)
{   
	UNREFERENCED(event);
    return OnMButtonDown(event);
}



int HBaseOperator::OnMouseMove(HEventInfo &event) 
{  
 
    int ret = HOP_OK;
	if (event.LButton() && !event.RButton()) 
		ret = OnLButtonDownAndMove(event);
  	else if (event.RButton() && !event.LButton())
		ret = OnRButtonDownAndMove(event);
    else if (event.MButton() && !event.LButton() && !event.RButton() )
                ret = OnMButtonDownAndMove(event);
	else if (event.LButton() && event.RButton())
		ret = OnLRButtonDownAndMove(event);
	else
		ret = OnNoButtonDownAndMove(event);

    return ret;

}



int HBaseOperator::OnRButtonDown(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}

int HBaseOperator::OnMouseWheel(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}


int HBaseOperator::OnRButtonDownAndMove(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnRButtonUp(HEventInfo &event) 
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnRButtonDblClk(HEventInfo & event)
{
	UNREFERENCED(event);
    return OnRButtonDown(event);
}



int HBaseOperator::OnNoButtonDownAndMove(HEventInfo & event)
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnTimer(HEventInfo & event)
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}



int HBaseOperator::OnKeyDown(HEventInfo & event)
{
	unsigned int the_char = event.GetChar();

	HSelectionSet *selection = GetView()->GetSelection();

	if (selection && (the_char == '\b') && (selection->GetSize() > 0))
	{
		GetView()->DeleteSelectionList(true);
		return HOP_OK;
	}
	else	   
		return HOP_NOT_HANDLED;
}



int HBaseOperator::OnKeyUp(HEventInfo & event)
{
	UNREFERENCED(event);
    return HOP_NOT_HANDLED;
}




int HBaseOperator::OnTouchesDown(HEventInfo &event) 
{
    int ret = HOP_NOT_HANDLED;
    if (m_bMapTouchesToMouseEvents)
    {
        if (m_pView->GetTouchManager()->GetActiveTouchCount() > 0)
        {            
           ret = OnLButtonDown(event);
        }
    }
    return ret;

    
}



int HBaseOperator::OnTouchesMove(HEventInfo &event) 
{
    int ret = HOP_NOT_HANDLED;
    if (m_bMapTouchesToMouseEvents)
    {
        if (m_pView->GetTouchManager()->GetActiveTouchCount() > 0)
        {                      
            ret = OnLButtonDownAndMove(event);
        }
    }
    return ret;
}



int HBaseOperator::OnTouchesUp(HEventInfo &event) 
{
    int ret = HOP_NOT_HANDLED;
    if (m_bMapTouchesToMouseEvents)
    {
        if (m_pView->GetTouchManager()->GetActiveTouchCount() > 0)
        {                     
            ret = OnLButtonUp(event);
        }
    }
    return ret;
}



