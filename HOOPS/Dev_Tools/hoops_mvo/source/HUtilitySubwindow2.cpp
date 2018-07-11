// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include "hc.h"
#include "vhash.h"
#include "vlist.h"
#include "HEventInfo.h"
#include "HBaseView.h"
#include "HEventManager.h"
#include "HUtilitySubwindow2.h"
#include "HBhvBehaviorManager.h"
#include "HBhvInterpolator.h"
#include "HBhvTimeline.h"
#include "HBhvAnimation.h"
#include "HUtilityGeometryCreation.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>


#define TILEHEIGHT 0.2f

HSubwindow2::HSubwindow2(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture) , HObject()
{
    m_OuterKey = INVALID_KEY;
    m_WindowKey = INVALID_KEY;
    
    view->GetEventManager()->RegisterHandler((HMouseListener *)this, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 10000000);
    view->GetEventManager()->RegisterHandler((HUpdateListener *)this, HUpdateListener::GetType(), HLISTENER_PRIORITY_NORMAL);
    view->GetEventManager()->RegisterHandler((HFitWorldListener *)this, HFitWorldListener::GetType(), HLISTENER_PRIORITY_NORMAL);
    view->GetEventManager()->RegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType(), HLISTENER_PRIORITY_HIGH);
	strcpy(m_title, "");

	m_ScrollHeight = 10;
	m_ScrollPos = 0;
	m_MinWidth = 0;
	m_MinHeight = 0;
}

HSubwindow2::HSubwindow2() : HBaseOperator(0,0,0)
{
    m_OuterKey = INVALID_KEY;
    m_WindowKey = INVALID_KEY;    
}

HSubwindow2::~HSubwindow2()
{
    if (GetView())
    {    
	GetView()->GetEventManager()->UnRegisterHandler((HUpdateListener *)this, HUpdateListener::GetType());
	GetView()->GetEventManager()->UnRegisterHandler((HMouseListener *)this, HMouseListener::GetType());
	GetView()->GetEventManager()->UnRegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType());
    }


}

 
void HSubwindow2::Reset()
{
    m_OuterKey = INVALID_KEY;
    m_WindowKey = INVALID_KEY;
}

bool HSubwindow2::IsActive()
{
    if (m_OuterKey != INVALID_KEY)
	return true;
    else
	return false;
}
HC_KEY HSubwindow2::GetWindowKey()
{
    return m_WindowKey;
}


void HSubwindow2::Insert(float left, float right, float bottom, float top,char const * title,float minwidth, float minheight, int type, long appearance)
{
	if (title)
		strcpy(m_title, title);
 	m_OuterKey	= HC_KOpen_Segment("");
	HObjectManager::SetHObjectSegment(GetStaticName());
	m_Pointer.Set(0,0,0);
	HC_Set_User_Options("swin = true"); 
	HC_Set_Handedness("left");
	HC_Set_Visibility("lights = off, faces = on");
	HC_Set_Rendering_Options("no simple shadow, hsra = hzb");
	HC_Set_Selectability("faces = on");
	m_WindowKey = HC_KCreate_Segment("window");
	HC_Close_Segment(); 
	m_Left = left; m_Right = right; m_Bottom = bottom; m_Top = top;
	m_MinWidth = minwidth; m_MinHeight = minheight;
	ChangeType(type, appearance);
	UpdateContent();
}


void HSubwindow2::UpdateContent()
{

}

void HSubwindow2::ChangeType(int type, long appearance)
{
    char text[MVO_BUFFER_SIZE];
    m_Appearance = appearance;
    m_Type = type;
    SetSize(m_Left, m_Right, m_Bottom, m_Top);
    BuildBorder();
    HC_Open_Segment_By_Key(m_WindowKey);
    sprintf(text, "wtype = %d", m_Type);
    HC_Set_User_Options(text); 
    HC_Close_Segment();
    
    HC_Open_Segment_By_Key(m_WindowKey);
    sprintf(text, "wappr = %ld", m_Appearance);
    HC_Set_User_Options(text); 
    HC_Close_Segment();
    HC_Open_Segment_By_Key(m_WindowKey);
    sprintf(text, "wcaption = %s", m_title);
    HC_Set_User_Options(text); 
    HC_Close_Segment();
    
}
void HSubwindow2::GetFromKey(HC_KEY key)
{
    m_OuterKey	= key;
    HC_Open_Segment_By_Key(key);
    m_WindowKey = HC_KCreate_Segment("window");
    HC_Close_Segment();   
    GetParameters();
  
}

void HSubwindow2::ConvertFromSubwindowToView(HPoint &in, HPoint &out)
{
    float posx,posy, deltax, deltay;


    GetPosition(posx,posy);
    GetSize(deltax,deltay);
        
    out.Set(posx + deltax * ((in.x + 1.0f)/2.0f),
	posy - deltay * ((in.y + 1.0f)/2.0f),0);
 
}
void HSubwindow2::ConvertFromViewToSubwindow(HPoint const & in, HPoint & out)
{
    float posx,posy, deltax, deltay;

    GetPosition(posx,posy);
    GetSize(deltax,deltay);
        
    out.Set(-1.0f + (in.x - posx) / deltax * 2.0f,
	    -1.0f + (in.y - (posy - deltay)) / deltay * 2.0f, 0);
}

void HSubwindow2::ComputeCoordinates(HBaseView *view, const char *in_system, HPoint &from, const char *out_system, HPoint &to)	
{
    HC_KEY path[10];
    HC_Open_Segment_By_Key(m_WindowKey);
    path[0] = HC_KCreate_Segment("snapshot");
    HC_Close_Segment();
    path[1] = m_WindowKey;
    path[2] = m_OuterKey;
    path[3] = view->GetSceneKey();
    path[4] = view->GetViewKey();
  
    HC_Compute_Coordinates_By_Path(5, path, in_system, &from, out_system, &to);
 
}



void HSubwindow2::GetParameters()
{
    HUserOptions text;
 
    HC_Open_Segment_By_Key(m_WindowKey);
    text.Show_One_User_Option("wtype");
    sscanf(text.m_data, "%d", &m_Type);
    text.Show_One_User_Option("wappr");
    sscanf(text.m_data, "%ld", &m_Appearance);
    text.Show_One_User_Option("wsize");
    HCLOCALE (sscanf(text.m_data, "%f %f %f %f %f %f", &m_Left, &m_Right, &m_Bottom, &m_Top, &m_MinWidth, &m_MinHeight));
    HC_Show_One_User_Option("wcaption", m_title);
    HC_Close_Segment();
    
}

void HSubwindow2::DrawScrollBox()
 {
    HC_Open_Segment_By_Key(m_WindowKey);
    HC_Open_Segment("border");
	HC_Open_Segment("scrollbar");
	HC_Open_Segment("box");
	HC_Flush_Contents(".", "everything");
	HC_Set_Visibility("edges = on");
	HC_Set_Edge_Weight(2.0f);
	HC_Set_Color("faces = light light blue, lines = white, edges = white");
     HC_Set_Line_Weight(1.0f);

	float delta = 2.0f / m_ScrollHeight;
	HUtility::InsertRectangle(".",-1.0f, 0.97f - delta * m_ScrollPos, 0.8f, 0.97f - delta * m_ScrollPos - delta,0,true);
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
}



void HSubwindow2::BuildBorder()
{
    HC_Open_Segment_By_Key(m_WindowKey);
	HC_Set_Color("window = grey, window contrast = black, text = black, lines = black");

    if (m_Appearance & SW_BORDER)
	HC_Set_Window_Frame("on");
    else
	HC_Set_Window_Frame("off");
	HC_Open_Segment("content");	
 	HAbsoluteWindow::SetWindow(m_pView, 0,20,20,0, RelativeLeft, RelativeTop, RelativeRight, RelativeBottom);
	HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
	HC_Set_Window_Frame("inset");
	HC_Close_Segment();

    HC_Open_Segment("border");
    HC_Flush_Contents(".", "everything");
    HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
    HC_Set_Visibility("lines = on");
    HC_Set_Rendering_Options("depth range=(0,0.1)");
     HC_Set_Line_Weight(2.0f);
    int i;

	

    if (m_Appearance & SW_MOVE_FIELD)
    {    
 		HC_Open_Segment("top");
	    HC_Set_Line_Weight(1.0f);
	 	HC_Set_Rendering_Options("anti-alias=(text,no lines)");

 		HAbsoluteWindow::SetWindow(m_pView, 0,0,0,20, RelativeLeft, RelativeTop, RelativeRight, RelativeTop);
	    HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
		HC_Set_Window_Frame("inset");
		HC_Insert_Text(0,0,0,m_title);
		HC_Set_Text_Alignment("**");
		HC_Set_Text_Font("name = sans serif, size = 10pixels");
		HC_Set_Color("window = blue, window contrast = light blue, text = white, lines = black");
  		HC_Close_Segment();
     }
    
    if (m_Appearance & SW_SIZE_FIELD)
    {    
		HC_Open_Segment("sizefield");
		HAbsoluteWindow::SetWindow(m_pView, 20,20,20,20, RelativeRight, RelativeBottom, RelativeLeft, RelativeTop);

		 HC_Set_Line_Weight(1.0f);
		HC_Set_Color("faces = white, lines = black, window = light grey, window contrast = grey");
		HUtility::InsertRectangle(".", -0.9f,-0.9f, 0.9f, 0.9f,true);
		HUtility::InsertRectangle(".", -0.7f,-0.7f,0.7f, 0.7f,false);
 		for (i=0;i<10;i++)
		{
//		    HC_Insert_Line(0.9f + i * 0.01f, -1.0f,0, 1.0f, -0.9f - (i*0.01f),0);
		}
		HC_Close_Segment();
    }
    
    if (m_Appearance & SW_DELETE_FIELD)
    {    
	HC_Open_Segment("top");
	HC_Open_Segment("delete");
	HC_Set_Color("lines = white, window = red");
	HC_Set_Rendering_Options("anti-alias=(no text,lines)");
		HAbsoluteWindow::SetWindow(m_pView, 15,0,0,15, RelativeRight, RelativeTop, RelativeRight, RelativeTop);
	 HC_Insert_Line(-1,-1,0, 1,1,0);
	HC_Insert_Line(-1,1,0, 1,-1,0);
	HC_Close_Segment();
	HC_Close_Segment();
    }
    if (m_Appearance & SW_SCROLL_FIELD)
    {    
	HC_Open_Segment("scrollbar");
		HAbsoluteWindow::SetWindow(m_pView, 18,20,18,20, RelativeRight, RelativeTop, RelativeLeft, RelativeBottom);
	    HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
		HC_Set_Window_Frame("inset");
 	//	HC_Set_Window(0.9,1,-0.89,0.9);	
		HC_Set_Color("window = light light grey, window contrast = light light grey, text = black, lines = black");
 	HC_Close_Segment();
    }

	
	
	HC_Close_Segment();
    HC_Close_Segment();
	 DrawScrollBox();
}



void HSubwindow2::SetSize(float left, float right, float bottom, float top)
{ 
    

    m_Left = left;
    m_Right = right;
    m_Bottom = bottom;
    m_Top = top;
    HPoint p1,p2;
   

    
    HC_Open_Segment_By_Key(m_OuterKey);
	HC_Open_Segment_By_Key(m_WindowKey);
    char text[MVO_BUFFER_SIZE];
    HCLOCALE (sprintf(text, "wsize = %f %f %f %f %f %f", left,right,bottom, top,m_MinWidth, m_MinHeight));
    HC_Set_User_Options(text); 
	if (right < m_MinWidth)
		right = m_MinWidth;
	if (bottom < m_MinHeight)
		bottom = m_MinHeight;


	HAbsoluteWindow::SetWindow(m_pView, left, top, right, bottom, RelativeLeft, RelativeTop,
		RelativeLeft, RelativeTop, true);
    //HC_Set_Window(left, right, bottom, top);

     HC_Close_Segment();
#if 0
    HC_Flush_Contents(".", "geometry");
     HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
    HC_KEY bkey = HUtility::InsertBox(&p1, &p2);
	HC_Open_Geometry(bkey);
    HC_Set_Color("faces = dark grey");
	HC_Close_Geometry();
#endif

    HC_Set_Visibility("cutting planes = off");

  
    HC_Close_Segment();
 
    
}


void HSubwindow2::GetPosition(float &x,float &y)
{
    x = m_Left;
    y = m_Top;
}

void HSubwindow2::GetSize(float &x,float &y)
{
      x = m_Right;
      y = m_Bottom;
}

void HSubwindow2::SetPosition(float x, float y)
{
    SetSize(x,m_Right, m_Bottom, y);
}
void HSubwindow2::SetSize(float deltax, float deltay)
{
    SetSize(m_Left,deltax, deltay, m_Top);
}

bool HSubwindow2::IsSubwindow(HC_KEY key)
{
    bool res;
    HC_Open_Segment_By_Key(key);
    if (HUtility::SegmentExists("window"))
	res = true;
    else
	res = false;
    HC_Close_Segment();
    return res;
}

bool HSubwindow2::FindSubwindow(HBaseView *view, HPoint new_pos, bool &PointerSelected)
{
	int 	res;
	HC_KEY	 key;
	HC_KEY	 subwindowkey = INVALID_KEY;
	
	HC_Open_Segment_By_Key(view->GetViewKey());
		res = HC_Compute_Selection(".", "./scene/overwrite","v, selection level = entity, no related selection limit", new_pos.x, new_pos.y);
	HC_Close_Segment();
	// compute the selection using the HOOPS window coordinate ofthe the pick location
	if (res > 0)
	{
		do	// process each item in the selection queue
		{
			HC_Show_Selection_Element (&key, 0, 0, 0);
			char type[255];
			HC_Show_Key_Type(key,type);
			key = HUtility::GrabSegment(key);
			if (IsSubwindow(key))
			{			   
				subwindowkey = key;
				break;
			}
			else
			{
				key = FindSubwindowFromSegment(key);
				if (key!=-1)
				{
					subwindowkey = key;
					PointerSelected = true;
					break;
				}
			}
		} while (HC_Find_Related_Selection());
	}
	if (subwindowkey != INVALID_KEY)
	{
		GetFromKey(subwindowkey);
		return true;
	}
	return false;
}

HC_KEY HSubwindow2::FindSubwindowFromSegment(HC_KEY key)
{
    HUserOptions text;
    HC_KEY owner = key;
    HC_KEY result = INVALID_KEY;
    HC_Open_Segment_By_Key(owner);
    text.Show_One_Net_User_Option("swin");
    HC_Close_Segment();

    if (!text.IsValid()){
		return INVALID_KEY;
	} else {
		
		
		
		while (1)
			{
				HC_Open_Segment_By_Key(owner);
				if (HUtility::SegmentExists("window"))
					{
						result = owner;
						HC_Close_Segment();
						break;
					}
				HC_Close_Segment();
				owner = HC_KShow_Owner_By_Key(owner);
				
			}
    }
    return result;
	
}

bool HSubwindow2::IsInSizeArea(HPoint &point)
{
    if (!(m_Appearance & SW_SIZE_FIELD))
	return false;

 	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("border");
	HC_Open_Segment("sizefield");
	bool ret = HAbsoluteWindow::IsPositionInWindow(m_pView, point.x, point.y);
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
	return ret;

}

bool HSubwindow2::IsInMoveArea(HPoint &point)
{
  
    if (!(m_Appearance & SW_MOVE_FIELD))
	return false;

	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("border");
	HC_Open_Segment("top");
	bool ret = HAbsoluteWindow::IsPositionInWindow(m_pView, point.x, point.y);
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
	return ret;
	
}

bool HSubwindow2::IsInScrollArea(HPoint &point)
{
  
    if (!(m_Appearance & SW_SCROLL_FIELD))
	return false;

	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("border");
	HC_Open_Segment("scrollbar");
	bool ret = HAbsoluteWindow::IsPositionInWindow(m_pView, point.x, point.y);
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
	return ret;
}


bool HSubwindow2::IsInDragArea(HPoint &point)
{
    if (!(m_Appearance & SW_DRAG_FIELD))
	return false;
  
    if (point.y < (m_Top - 0.07f))
	return true;
    else
	return false;
}

bool HSubwindow2::IsInCloseArea(HPoint &point)
{
    if (!(m_Appearance & SW_DELETE_FIELD))
	return false;
  
	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("border");
	HC_Open_Segment("top");
	HC_Open_Segment("delete");
	bool ret = HAbsoluteWindow::IsPositionInWindow(m_pView, point.x, point.y);
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();
	return ret;
}



const char * HSubwindow2::GetStaticName() { return "subwindow"; }
const char * HSubwindow2::GetName() { return GetStaticName(); }



int HSubwindow2::OnLButtonDown(HEventInfo &event)
{
    
    if (OperatorStarted()) 
		return HLISTENER_PASS_EVENT;

	SetOperatorStarted(true);
   
    HPoint new_pos = event.GetMouseWindowPos();
    bool pointerselected = false;
    m_Action = SW_ACTION_MOVE;
    if (!FindSubwindow(GetView(), new_pos, pointerselected))    
    {
		SetOperatorStarted(false);
		return HLISTENER_PASS_EVENT;
    }

	if (IsInScrollArea(new_pos))
	{	
		
		HPoint o;
		m_Action = SW_ACTION_SCROLLING;
		HC_Open_Segment_By_Key(m_WindowKey);
		HC_Open_Segment("border");
		HC_Open_Segment("scrollbar");
			HAbsoluteWindow::CalculateLocalWindowPos(m_pView, new_pos.x, new_pos.y,
			o.x, o.y);
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();
 		o.y = -o.y;		
 		float delta = 2.0f / m_ScrollHeight;
 		m_ScrollPos = (int)((o.y + 1.0f)/delta);
	  	DrawScrollBox();
		UpdateContent();
		m_pView->Update();
		return HLISTENER_CONSUME_EVENT;
	}
	if (IsInCloseArea(new_pos))
	{
	HC_Delete_By_Key(GetKey());
	GetView()->Update();
    	SetOperatorStarted(false);
	return HLISTENER_CONSUME_EVENT;
	}
	else
	{
	if (IsInSizeArea(new_pos))
		m_Action = SW_ACTION_SIZING;	
	else if (IsInMoveArea(new_pos))
		m_Action = SW_ACTION_MOVE;	
	else if (IsInDragArea(new_pos))
		m_Action = SW_ACTION_DRAGGING;	
	else
	{
    		SetOperatorStarted(false);
		return HLISTENER_PASS_EVENT;
	}

	}
	
	m_ptLast = new_pos;
	SetFirstPoint(new_pos);
	return HLISTENER_CONSUME_EVENT;
}


int HSubwindow2::OnLButtonDownAndMove(HEventInfo &event)
{
    if (!OperatorStarted())
		return HLISTENER_PASS_EVENT;
    
    SetNewPoint(event.GetMouseWindowPos());
    
	HPoint lastinpixels, newinpixels;
	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	HC_Compute_Coordinates(".", "local window", &m_ptLast, "local pixels", &lastinpixels);
	HC_Compute_Coordinates(".", "local window", &m_ptNew, "local pixels", &newinpixels);
	HC_Close_Segment();

    HPoint delta2 = newinpixels - lastinpixels;
    

    if (m_Action == SW_ACTION_SCROLLING)
    {
		HPoint o;
		HPoint new_pos = event.GetMouseWindowPos();
		HC_Open_Segment_By_Key(m_WindowKey);
		HC_Open_Segment("border");
		HC_Open_Segment("scrollbar");
			HAbsoluteWindow::CalculateLocalWindowPos(m_pView, new_pos.x, new_pos.y,
			o.x, o.y);
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();
 		o.y = -o.y;		
 		float delta = 2.0f / m_ScrollHeight;
 		m_ScrollPos = (int)((o.y + 1.0f)/delta);
	  	DrawScrollBox();
		UpdateContent();
  	}
     if (m_Action == SW_ACTION_MOVE)
    {
		float x,y;
		GetPosition(x,y);
		x+=delta2.x;
		y+=delta2.y;
		SetPosition(x,y);
    }	
    else if (m_Action == SW_ACTION_SIZING)
    {    
		float x,y;
		GetSize(x,y);
		x+=delta2.x;
		y+=delta2.y;
		if (x<0.01f)
			x = 0.01f;
		if (y<0.01f)
			y = 0.01f;

		SetSize(x,y);
		BuildBorder();
		UpdateContent();


    }
  
 
    GetView()->Update();
    m_ptLast = GetNewPoint();

    return HLISTENER_CONSUME_EVENT;
}


int HSubwindow2::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted())
		return HBaseOperator::OnLButtonUp(event);
	
 	SetOperatorStarted(false);      
	return HLISTENER_CONSUME_EVENT;
}


int HSubwindow2::PreFitWorldEvent(HBaseView *view)
{
    HC_Open_Segment_By_Key(view->GetModelKey());  
    if (HUtility::SegmentExists("subwindows"))
    {   
		HC_Open_Segment("subwindows");
		HC_Set_Visibility("everything = off");
		HC_Set_Rendering_Options("attribute lock = visibility");
		HC_Close_Segment();
    }	
    HC_Close_Segment();
    return HLISTENER_PASS_EVENT;
}

int HSubwindow2::PostFitWorldEvent(HBaseView *view)
{
    HC_Open_Segment_By_Key(view->GetModelKey());  
    if (HUtility::SegmentExists("subwindows"))
    {   
		HC_Open_Segment("subwindows");
		HC_UnSet_Visibility();
		HC_UnSet_Rendering_Options();
 		HC_Close_Segment();
    }
    HC_Close_Segment();
    return HLISTENER_PASS_EVENT;

}


int HSubwindow2::SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex) 
{
	UNREFERENCED(view);
	UNREFERENCED(complex);

	char type[MVO_BUFFER_SIZE];
	key = HObjectManager::FindHObjectSegment(key, type);
	if (strcmp(type, GetStaticName()) == 0)	
		return HLISTENER_CONSUME_EVENT;
	else
		return HLISTENER_PASS_EVENT;
}


int HSubwindow2::ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p) 
{
	UNREFERENCED(view);

    char type[MVO_BUFFER_SIZE];
    key = HObjectManager::FindHObjectSegment(key, type);
    if (strcmp(type, GetStaticName()) == 0)
    {
		float matrix[16];
		HC_Open_Segment_By_Key(key);
		HUtility::ShowModellingMatrix(matrix);
		HC_UnSet_Modelling_Matrix();
		HC_Close_Segment();
		GetFromKey(key);

		if (p)
			SetPosition(p->x,p->y);
		else
			SetPosition(matrix[12],matrix[13]);
	

		GetView()->Update();
		return HLISTENER_CONSUME_EVENT;
    }

    return HLISTENER_PASS_EVENT;
}

int HSubwindow2::ObjectScaledEvent(HBaseView *view, HC_KEY key, HPoint &scale) 
{
	UNREFERENCED(view);

    char type[MVO_BUFFER_SIZE];
    key = HObjectManager::FindHObjectSegment(key, type);
    if (strcmp(type, GetStaticName()) == 0)
    {
		float matrix[16];
		HC_Open_Segment_By_Key(key);
		HUtility::ShowModellingMatrix(matrix);
		HC_UnSet_Modelling_Matrix();
		HC_Close_Segment();
		GetFromKey(key);
		SetSize(scale.x, scale.y);
	 
 
		GetView()->Update();
        return HLISTENER_CONSUME_EVENT;
    }
    return HLISTENER_PASS_EVENT;

}



HTexturePickerWindow::HTexturePickerWindow(HBaseView* view, int DoRepeat,int DoCapture) : HSubwindow2(view, DoRepeat, DoCapture)
{

}

HTexturePickerWindow::HTexturePickerWindow() : HSubwindow2()
{
  
}

void ShowSphere()
{
     
    HC_Set_Camera_Position(0,0,-4.2);
    HC_Set_Camera_Projection("stretched");
	HC_Orbit_Camera(45,45);
	
	HC_Flush_Contents(".", "geometry, segments");
    HUtilityGeometryCreation::CreateSphere(HPoint(0.0f,0.0f,0.0f), 0.8f,25,HPoint(0.0f,1.0f,0.0f), HPoint(0.0f,0.0f,1.0f));
    HC_Insert_Line(-1,-1,-1,1,1,1);
    HC_Insert_Line(-1,1,-1,1,-1,1);
    HC_Insert_Distant_Light(0,0,-1);
    
}


void HTexturePickerWindow::UpdateContent()
{

	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("content");
	HC_KEY tkey = HC_KOpen_Segment("textures");
	HC_Flush_Contents(".", "everything");
 	HC_Set_Visibility("lights = on");
	HC_Close_Segment();
	HC_Close_Segment();
	HC_Close_Segment();


	HC_Open_Segment_By_Key(m_pView->GetModelKey());
	HC_Open_Segment("styles");
	HC_Begin_Contents_Search(".", "segments");
	HC_KEY segmentkey;
	int i=0;
	int i2=0;
	char text[4096];
	while (HUtility::FindContents(segmentkey))
	{
		
		HC_Show_Segment(segmentkey, text);
		if (!strstr(text, "textures"))
		{	
			if (i>=m_ScrollPos)
			{
				HC_Open_Segment_By_Key(tkey);
				char text[4096];
				sprintf(text,"%d", i2);
				//if (0.9-i2*delta > -1)
				{
					HC_Open_Segment(text);
					ShowSphere();
					HAbsoluteWindow::SetWindow(m_pView, 0.0f,i2*80.0f,0.0f,80.0f, RelativeLeft, RelativeTop, RelativeRight, RelativeTop);				
					HC_Style_Segment_By_Key(segmentkey);
 					HC_Close_Segment();
					
				}
				HC_Close_Segment();
				i2++;
			}
			i++;			
		}
		
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	HC_Close_Segment();
	
}


int HTexturePickerWindow::OnLButtonDown(HEventInfo &event)
{
	int ret = HSubwindow2::OnLButtonDown(event);
	if (m_Action == SW_ACTION_DRAGGING)
	{
		char text[4096];
		int i;
	    HPoint new_pos = event.GetMouseWindowPos();
	    HPoint new_pos2 = event.GetMousePixelPos();
		HPoint o;
		HC_Open_Segment_By_Key(m_WindowKey);
		HC_Open_Segment("content");
		HC_Open_Segment("textures");
		for ( i=0;i<30;i++)
		{
			sprintf(text,"%d", i);
			HC_Open_Segment(text);
			if (HAbsoluteWindow::IsPositionInWindow(m_pView, new_pos.x, new_pos.y))
			{
				HC_Close_Segment();
				break;
			}
			HC_Close_Segment();
		}

		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();

  		sprintf(text,"%d", i);
		HC_KEY stylekey = -1;
		HC_Open_Segment_By_Key(m_WindowKey);
		HC_Open_Segment("content");
		HC_Open_Segment("textures");
		HC_Open_Segment(text);
		HC_Begin_Contents_Search(".", "styles");
		int count;
		HC_Show_Contents_Count(&count);
		if (count)
			HUtility::FindContents(stylekey);
		HC_End_Contents_Search();
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();

		
		if (count)
		{
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("tempwindow");
		HC_Flush_Contents(".", "everything");
		ShowSphere();
		HC_Show_Style_Segment(stylekey, text);
		HC_Style_Segment(text);
		HC_Set_Rendering_Options("attribute lock = color");
 		HC_Set_Color("transmission = light grey");
	
		HAbsoluteWindow::SetWindow(m_pView, new_pos2.x-40,new_pos2.y-40, 80,80, RelativeLeft, RelativeTop, RelativeLeft, RelativeTop);
//		HC_Set_Window(new_pos.x - deltax/2.0f, new_pos.x + deltax/2.0f,
//			new_pos.y - TILEHEIGHT/2.0f,new_pos.y + TILEHEIGHT/2.0f);
		HC_Set_Window_Pattern("clear");
		HC_Close_Segment();
		HC_Close_Segment();
		}
		else
			m_Action = SW_ACTION_NOACTION;

	

	}
			
	return ret;
}

int HTexturePickerWindow::OnLButtonDownAndMove(HEventInfo &event)
{

    if (m_Action == SW_ACTION_DRAGGING && OperatorStarted())
	{

	    HPoint new_pos2 = event.GetMousePixelPos();
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("tempwindow");
 		HAbsoluteWindow::SetWindow(m_pView, new_pos2.x-40,new_pos2.y-40, 80,80, RelativeLeft, RelativeTop, RelativeLeft, RelativeTop);

//		HC_Set_Window(new_pos.x - deltax/2.0f, new_pos.x + deltax/2.0f,
//			new_pos.y - TILEHEIGHT/2.0f,new_pos.y + TILEHEIGHT/2.0f);
		HC_Close_Segment();
		HC_Close_Segment();


		if (event.Shift())
		{
	    HPoint new_pos = event.GetMouseWindowPos();
		char text[4096];
		HC_KEY stylekey;
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("tempwindow");
 		HC_Begin_Contents_Search(".", "styles");
		HUtility::FindContents(stylekey);
		HC_End_Contents_Search();
		HC_Show_Style_Segment(stylekey, text);
		HC_Close_Segment();
		HC_Close_Segment();
  		HC_KEY selkey = HUtility::SelectUnderCursor(m_pView, new_pos.x, new_pos.y, false, false);
		if (selkey != -1)
		{
			HC_Open_Segment_By_Key(HUtility::GrabSegment(selkey));
			HC_Flush_Contents(".", "styles");
			HC_Style_Segment(text);
			HC_Close_Segment();
		}

		}


		m_pView->Update();	

	

        return HLISTENER_CONSUME_EVENT;

	}
	else 
		return HSubwindow2::OnLButtonDownAndMove(event);
}


int HTexturePickerWindow::OnLButtonUp(HEventInfo &event)
{
     if (m_Action == SW_ACTION_DRAGGING && OperatorStarted())
	{
	    HPoint new_pos = event.GetMouseWindowPos();
		char text[4096];
		HC_KEY stylekey;
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("tempwindow");
 		HC_Begin_Contents_Search(".", "styles");
		HUtility::FindContents(stylekey);
		HC_End_Contents_Search();
		HC_Show_Style_Segment(stylekey, text);
		HC_Close_Segment();
		HC_Delete_Segment("tempwindow");
		HC_Close_Segment();
		HC_KEY selkey = HUtility::SelectUnderCursor(m_pView, new_pos.x, new_pos.y, false, false);
		if (selkey != -1 && !IsSubwindow(HUtility::GrabSegment(selkey)))
		{
			HC_Open_Segment_By_Key(HUtility::GrabSegment(selkey));
			HC_Flush_Contents(".", "styles");
			HC_Style_Segment(text);
			HC_Close_Segment();
		}
		


		m_pView->Update();	

	 }
	return HSubwindow2::OnLButtonUp(event);
}
