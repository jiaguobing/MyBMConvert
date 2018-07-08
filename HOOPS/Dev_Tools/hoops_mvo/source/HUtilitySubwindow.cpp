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
#include "HUtilitySubwindow.h"
#include "HBhvBehaviorManager.h"
#include "HBhvInterpolator.h"
#include "HBhvTimeline.h"
#include "HBhvAnimation.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

HSubwindow::HSubwindow(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture) , HObject()
{
    m_OuterKey = INVALID_KEY;
    m_WindowKey = INVALID_KEY;
    
    view->GetEventManager()->RegisterHandler((HMouseListener *)this, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 10000000);
    view->GetEventManager()->RegisterHandler((HUpdateListener *)this, HUpdateListener::GetType(), HLISTENER_PRIORITY_NORMAL);
    view->GetEventManager()->RegisterHandler((HFitWorldListener *)this, HFitWorldListener::GetType(), HLISTENER_PRIORITY_NORMAL);
    view->GetEventManager()->RegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType(), HLISTENER_PRIORITY_HIGH);

}

HSubwindow::HSubwindow() : HBaseOperator(0,0,0)
{
    m_OuterKey = INVALID_KEY;
    m_WindowKey = INVALID_KEY;    
}

HSubwindow::~HSubwindow()
{
    if (GetView())
    {    
	GetView()->GetEventManager()->UnRegisterHandler((HUpdateListener *)this, HUpdateListener::GetType());
	GetView()->GetEventManager()->UnRegisterHandler((HMouseListener *)this, HMouseListener::GetType());
	GetView()->GetEventManager()->UnRegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType());
    }


}

 
void HSubwindow::Reset()
{
    m_OuterKey = INVALID_KEY;
    m_WindowKey = INVALID_KEY;
}

bool HSubwindow::IsActive()
{
    if (m_OuterKey != INVALID_KEY)
	return true;
    else
	return false;
}
HC_KEY HSubwindow::GetWindowKey()
{
    return m_WindowKey;
}


void HSubwindow::Insert(float left, float right, float bottom, float top, int type, long appearance)
{
	m_Magnification = 0.0f;
	m_OldMagnification = 0.0f;
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
	ChangeType(type, appearance);
}


void HSubwindow::ChangeType(int type, long appearance)
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
    if (type == SUBWINDOW_MAGNIFIER || type == SUBWINDOW_SNAPSHOT || type == SUBWINDOW_POINTER)
    {
	BuildMagnifyController(m_Magnification);
        HC_Open_Segment_By_Key(m_WindowKey);
	HCLOCALE (sprintf(text, "wmag = %f", m_Magnification));
	HC_Set_User_Options(text); 
	HC_Close_Segment();
	if (type == SUBWINDOW_POINTER)
	{
	    HC_Open_Segment_By_Key(m_WindowKey);
	    sprintf(text, "wpointer = %f %f", m_Pointer.x, m_Pointer.y);
	    HC_Set_User_Options(text); 
	    HC_Close_Segment();
	}
	
    }
    
}
void HSubwindow::GetFromKey(HC_KEY key)
{
    m_OuterKey	= key;
    HC_Open_Segment_By_Key(key);
    m_WindowKey = HC_KCreate_Segment("window");
    HC_Close_Segment();   
    GetParameters();
  
}

void HSubwindow::ConvertFromSubwindowToView(HPoint &in, HPoint &out)
{
    float posx,posy, deltax, deltay;

    GetPosition(posx,posy);
    GetSize(deltax,deltay);
        
    out.Set(posx + deltax * ((in.x + 1.0f)/2.0f),
	posy - deltay * ((in.y + 1.0f)/2.0f),0);
 
}
void HSubwindow::ConvertFromViewToSubwindow(HPoint const & in, HPoint & out)
{
    float posx,posy, deltax, deltay;

    GetPosition(posx,posy);
    GetSize(deltax,deltay);
        
    out.Set(-1.0f + (in.x - posx) / deltax * 2.0f,
	    -1.0f + (in.y - (posy - deltay)) / deltay * 2.0f, 0);
}

void HSubwindow::ComputeCoordinates(HBaseView *view, const char *in_system, HPoint &from, const char *out_system, HPoint &to)	
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

void HSubwindow::ChangeMagnification(HBaseView *view, HPoint & p)
{
    HPoint out;
    ConvertFromViewToSubwindow(p, out);
    float m = (out.y + 1.0f) /2.0f;
    if (m < 0)
	m = 0.0f;
     ChangeMagnification(view, m);
      
}


void HSubwindow::ChangeMagnification(HBaseView *view, float magnification)
{


    HPoint out;
    m_Magnification = magnification;
    BuildMagnifyController(magnification);

    Magnify(view);
    if (m_Type == SUBWINDOW_MAGNIFIER || m_Type == SUBWINDOW_POINTER)
        CopyViewLight(view);
    char text[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key(m_WindowKey);
    HCLOCALE (sprintf(text, "wmag = %f", m_Magnification));
    HC_Set_User_Options(text); 
    HC_Close_Segment();
     
}
void HSubwindow::Magnify(HBaseView *view)
{
	
	HPoint position, target, upvector;
    float x,y;
    char projection[MVO_BUFFER_SIZE];
    if (m_Type == SUBWINDOW_MAGNIFIER || m_Type == SUBWINDOW_POINTER)
    {
		HC_Open_Segment_By_Key(view->GetSceneKey());
		HC_Show_Camera(&position, &target, &upvector, &x,&y,projection);
		HC_Close_Segment();
    }
    else
    {
		HC_Open_Segment_By_Key(m_WindowKey);
		HC_Open_Segment("snapshot");
		HC_Show_Camera(&position, &target, &upvector, &x,&y,projection);
		HC_Close_Segment();
		HC_Open_Segment("snapshotsav");
		HC_Show_Camera_Field(&x,&y);
		HC_Close_Segment();
		HC_Close_Segment();
    }
	
	HPoint delta;
	delta.Set(position.x- target.x, position.y - target.y, position.z - target.z);
	float vl = (float)HC_Compute_Vector_Length(&delta);
	HPoint in,out;
	if (m_Type == SUBWINDOW_POINTER)
	{
		out.Set(m_Pointer.x, m_Pointer.y, m_Pointer.z);
	}
	else
	{     
		in.Set(0,0,0);
		ConvertFromSubwindowToView(in,out);    
	}
	
	bool found = false;
	if (!found)
	{     
		if (m_Type == SUBWINDOW_MAGNIFIER || m_Type == SUBWINDOW_POINTER)
		{    
			HC_Open_Segment_By_Key(view->GetSceneKey());
			HC_Compute_Coordinates(".","world", &target, "viewpoint", &target);
			HC_Compute_Coordinates(".","world", &position, "viewpoint", &position);
			HC_Compute_Coordinates(".","outer window", &out, "viewpoint", &out);
			
			target.x = position.x = out.x;
			target.y = position.y = out.y;
			
			HC_Compute_Coordinates(".","viewpoint", &target, "world", &target);
			HC_Compute_Coordinates(".","viewpoint", &position, "world", &position);
			
			HC_Close_Segment();
		}
	}
	
	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("snapshot");
	
	HC_Set_Camera(&position, &target, &upvector,x,y,projection);
	if (m_Type != SUBWINDOW_MAGNIFIER && m_Type != SUBWINDOW_POINTER)
        HC_Dolly_Camera(0,0,-m_OldMagnification * vl);
	
	HC_Dolly_Camera(0,0,m_Magnification * vl);
	if (projection[0] == 'o')
	{ 
   	    HC_Set_Camera_Field(x / (m_Magnification*128.0f + 1.0f),y/ (m_Magnification*128.0f + 1.0f));
	}
	HC_Close_Segment();
	HC_Close_Segment();
	m_OldMagnification = m_Magnification;
}


void HSubwindow::Drag(HBaseView *view, HPoint p1, HPoint p2)
{
    HPoint position, target, upvector;
    float x,y;
    char projection[MVO_BUFFER_SIZE];
 
    
     HC_Open_Segment_By_Key(m_WindowKey);
     HC_Open_Segment("snapshot");
	HC_Show_Camera(&position, &target, &upvector, &x,&y,projection);
      HC_Close_Segment();
      HC_Close_Segment();
    

      ComputeCoordinates(view, "outer window", p1, "world", p1);
      ComputeCoordinates(view, "outer window", p2, "world", p2);

      HPoint delta;
      delta.Set(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
      if (projection[0] == 'p')
      {
       float t = 1.0f + (m_Magnification * 5.0f);
      delta.Set(delta.x/t, delta.y/t, delta.z/t);
	}
      
      HC_Open_Segment_By_Key(m_WindowKey);
     HC_Open_Segment("snapshot");
    HC_Set_Camera_Position(position.x - delta.x, position.y - delta.y, position.z - delta.z);
    HC_Set_Camera_Target(target.x - delta.x, target.y - delta.y, target.z - delta.z);
     HC_Close_Segment();
     HC_Close_Segment();
}




void HSubwindow::MakeCameraSnapshot(HBaseView *view)
{
    HPoint position, target, upvector;
    float x,y;
    char projection[MVO_BUFFER_SIZE];
    char visibility[MVO_BUFFER_SIZE];
    char ropt[MVO_BUFFER_SIZE];
   
    HC_Open_Segment_By_Key(view->GetSceneKey());
	HC_Show_Camera(&position, &target, &upvector, &x,&y,projection);
	HC_Show_Visibility(visibility);
	HC_Show_Rendering_Options(ropt);
      HC_Close_Segment();


    HC_Open_Segment_By_Key(m_WindowKey);
    HC_KEY snapshotkey = HC_KOpen_Segment("snapshot");
    HC_Flush_Contents(".", "everything");
    HC_Set_Camera(&position, &target, &upvector,x,y,projection);
    HC_Close_Segment();
    HC_Open_Segment("snapshotsav");
    HC_Set_Camera(&position, &target, &upvector,x,y,projection);
    HC_Close_Segment();
    HC_Close_Segment();

    HC_KEY segkey;
    HC_Open_Segment_By_Key(view->GetModelKey());
    HC_KEY mainkey = HC_KCreate_Segment("subwindows");
    HC_Begin_Contents_Search(".", "segments");
    while (HUtility::FindContents(segkey))
    {
	if (!IsSubwindow(segkey) && mainkey!= segkey)
	{	
	HC_Open_Segment_By_Key(snapshotkey);
	HC_Include_Segment_By_Key(segkey);
	HC_Close_Segment();
	}
    }
    HC_End_Contents_Search();
    HC_Close_Segment();

    CopyViewLight(view);

    HC_Open_Segment_By_Key(snapshotkey);
    if (view->GetHandedness() == HandednessRight)
	HC_Set_Handedness("right");
    else
	HC_Set_Handedness("left");
    HC_Set_Visibility(visibility);
//    HC_Set_Rendering_Options(ropt);
    HC_Close_Segment();
    m_Magnification = 0.0f;
    m_OldMagnification = 0.0f;
    ChangeMagnification(view,0.0f);
 
   
}





void HSubwindow::SyncViewToWindow(HBaseView *view)
{
    HPoint position, target, upvector;
    float x,y;
    char projection[MVO_BUFFER_SIZE];
    char visibility[MVO_BUFFER_SIZE];
    
    HC_Open_Segment_By_Key(m_WindowKey);
    HC_Open_Segment("snapshot");
	HC_Show_Camera(&position, &target, &upvector, &x,&y,projection);
	HC_Show_Visibility(visibility);
     HC_Close_Segment();
    HC_Close_Segment();

      HC_Open_Segment_By_Key(view->GetSceneKey());
    HC_Set_Camera(&position, &target, &upvector,x,y,projection);
    HC_Set_Visibility(visibility);
    HC_Close_Segment();
    view->CameraPositionChanged(true, true);


   
}


void HSubwindow::CopyViewLight(HBaseView *view)
{
    HC_KEY lightskey;
    HC_Open_Segment_By_Key(view->GetLightsKey());
    HC_Begin_Contents_Search(".", "lights");
    while (HUtility::FindContents(lightskey))
    {
 
	float x,y,z;
	HC_Show_Distant_Light(lightskey, &x,&y,&z);
	HC_Open_Segment_By_Key(m_WindowKey);
	HC_Open_Segment("snapshot");
	HC_Open_Segment("light");
	HC_Flush_Contents(".", "everything");
	HC_KEY lkey = HC_KInsert_Distant_Light(x,y,z);
	HC_Set_Geometry_Options(lkey, "camera relative");
 	HC_Close_Segment();	
 	HC_Close_Segment();	
 	HC_Close_Segment();	
    }
   HC_End_Contents_Search();
   HC_Close_Segment();
}
/*
void HSubwindow::MakeColorRamp(HBaseView *view)
{    
    float startx = -0.8f;
    float starty = -0.8f;
    float addery = 0.2f;

    float adderx = 0.6f;

    HC_Open_Segment_By_Key(m_WindowKey);   
    HC_Open_Segment("");
    HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
    HC_Set_Heuristics("no polygon handedness");
    HC_Set_Text_Font("name = arial.ttf, size =16px");
    HC_Set_Text_Alignment("<*");

    HC_Set_Visibility("faces = on");
    for (int i = 0;i<8;i++)
    {
	HPoint p[4];
	p[0].Set(startx,starty + addery * i,0);
	p[1].Set(startx + adderx,starty + addery * i,0);
	p[2].Set(startx + adderx,starty + addery * (i + 1),0);
	p[3].Set(startx,starty + addery * (i + 1),0);
	HC_Open_Segment("");
	HC_Insert_Polygon(4,p);
	HC_Set_Color_By_Value("faces", "rgb", 0.1f * i, 0,0);
	char text[MVO_BUFFER_SIZE];
	HCLOCALE (sprintf(text,"%2.2f", i*addery));
	HC_Insert_Text(startx + adderx +0.1, starty + (addery*i) + addery/2.0f,0, text);
	HC_Close_Segment();
    }
	HC_Close_Segment();
	HC_Close_Segment();

}
 
void HSubwindow::MakeTextField(HBaseView *view)
{    
 
    HC_Open_Segment_By_Key(m_WindowKey);   
    HC_Open_Segment("");
    HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
    HC_Set_Heuristics("no polygon handedness");
    HC_Set_Text_Font("name = arial.ttf, size =0.1wru");
    HC_Set_Text_Alignment("<*");
    HC_Set_Color("text = grey");
    HC_Insert_Text(-1,0.4,0, "HOOPS offers various render modes\nPainters\nZ-Sort\nZ-Buffer\nHidden Line");
    HC_Close_Segment();
    HC_Close_Segment();

}

   
*/


void HSubwindow::GetParameters()
{
    HUserOptions text;
	int matches;

    HC_Open_Segment_By_Key(m_WindowKey);
    text.Show_One_User_Option("wtype");
    sscanf(text.m_data, "%d", &m_Type);
    text.Show_One_User_Option("wappr");
    sscanf(text.m_data, "%ld", &m_Appearance);
    text.Show_One_User_Option("wsize");
    HCLOCALE (sscanf(text.m_data, "%f %f %f %f", &m_Left, &m_Right, &m_Bottom, &m_Top));
    
    if (m_Type == SUBWINDOW_MAGNIFIER || m_Type == SUBWINDOW_POINTER || m_Type == SUBWINDOW_SNAPSHOT)
    {
        text.Show_One_User_Option("wmag");
		HCLOCALE (sscanf(text.m_data, "%f", &m_Magnification));
		m_OldMagnification = m_Magnification;
		if (m_Type == SUBWINDOW_POINTER)
		{
			text.Show_One_User_Option("wpointer");
			HCLOCALE (matches = sscanf(text.m_data, "%f %f", &m_Pointer.x, &m_Pointer.y)); 
			if (matches != 2)
				HDB::ReportError ("unexpected formatting of HSubwindow option");
		}
    }
    
    HC_Close_Segment();
    
}



void HSubwindow::BuildBorder()
{
    HC_Open_Segment_By_Key(m_WindowKey);
    if (m_Appearance & SW_BORDER)
	HC_Set_Window_Frame("on");
    else
	HC_Set_Window_Frame("off");
    HC_Open_Segment("border");
    HC_Flush_Contents(".", "everything");
    HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
    HC_Set_Visibility("lines = on");
    HC_Set_Rendering_Options("depth range=(0,0.1)");
    
    HC_Set_Line_Weight(2.0f);
    int i;
    if (m_Appearance & SW_MOVE_FIELD)
    {    
	for (i=0;i<10;i++)
	{
	    HC_Insert_Line(-1.0f,1.0f - (i * 0.005f), 0,0.9f, 1.0f - (i * 0.005f),0);
	}
    }
    
    if (m_Appearance & SW_SIZE_FIELD)
    {    
	for (i=0;i<10;i++)
	{
	    HC_Insert_Line(0.9f + i * 0.01f, -1.0f,0, 1.0f, -0.9f - (i*0.01f),0);
	}
    }
    
    if (m_Appearance & SW_DELETE_FIELD)
    {    
        HC_Insert_Line(0.95,0.99,0, 0.99,0.95,0);
	HC_Insert_Line(0.95,0.95,0, 0.99,0.99,0);
    }
    HC_Close_Segment();
    HC_Close_Segment();
    
}



void HSubwindow::BuildMagnifyController(float p)
{
    
    
    float pos = 2.0f * p - 1;
    HC_Open_Segment_By_Key(m_WindowKey);
    
    HC_Open_Segment("magnifier");
    HC_Flush_Contents(".", "everything");
    if (m_Appearance & SW_MAGNIFIER_FIELD)
    {    
	HC_Set_Rendering_Options("depth range=(0,0.1)");
	HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
	HC_Set_Line_Weight(2.0f);
	HC_Set_Visibility("lines = on");
	HC_Insert_Line(-1.0f,pos,0, -0.9,pos,0);
    }
    HC_Close_Segment();
    HC_Close_Segment();
}
void HSubwindow::SetSize(float left, float right, float bottom, float top)
{ 
    
    m_Left = left;
    m_Right = right;
    m_Bottom = bottom;
    m_Top = top;
    HPoint p1,p2;
    if (m_Appearance & SW_BACKGROUND_BOX)      
    {    
	p1.Set(left+0.007f,top-0.007f,0.0f);
	p2.Set(right+0.007f,bottom-0.007f,0.0f);
    }
    else
    {    
	p1.Set(left+0.007f,top-0.007f,0.0f);
	p2.Set(right-0.007f,bottom+0.007f,0.0f);
    }

    
    HC_Open_Segment_By_Key(m_OuterKey);
    HC_Flush_Contents(".", "geometry");
     HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
    HC_KEY bkey = HUtility::InsertBox(&p1, &p2);
	HC_Open_Geometry(bkey);
    HC_Set_Color("faces = dark grey");
	HC_Close_Geometry();
    HC_Set_Visibility("cutting planes = off");

    HC_Open_Segment_By_Key(m_WindowKey);
    char text[MVO_BUFFER_SIZE];
    HCLOCALE (sprintf(text, "wsize = %f %f %f %f", left,right,bottom, top));
    HC_Set_User_Options(text); 
    HC_Set_Window(left, right, bottom, top);

    HC_Close_Segment();
    HC_Close_Segment();
    if (m_Type == SUBWINDOW_POINTER)
	ShowPointer();

    
}

void HSubwindow::ShowPointer()
{
    HC_Open_Segment_By_Key(m_OuterKey);
    HC_Open_Segment("mag");
    HC_Set_Selectability("everything = off");
    HC_Set_Color("faces = (transmission = light grey)");
    HC_Set_Selectability("everything = off");
    HC_Flush_Contents(".", "geometry");
    HC_Set_Rendering_Options("depth range=(0,0.1)");

    HC_Set_Heuristics("no polygon handedness");    
    HC_Set_Visibility("lines = on, edges =off, faces = on");
    HC_Open_Segment("marker");
    HC_Set_Selectability("markers = on");
    HC_Set_Visibility("markers = on");
    HC_Flush_Contents(".", "geometry");
    HC_Insert_Marker(m_Pointer.x, m_Pointer.y, m_Pointer.z);
    HC_Close_Segment();

    HPoint p[3];
    if (m_Pointer.x<m_Left)
    {
	p[0].Set(m_Left, m_Top,0);
	p[1].Set(m_Left, m_Bottom,0);
	p[2].Set(m_Pointer.x,m_Pointer.y,m_Pointer.z);
        HC_Insert_Polygon(3,p);

    }
    
    if (m_Pointer.x>m_Right)
    {
	p[0].Set(m_Right, m_Top,0);
	p[1].Set(m_Right, m_Bottom,0);
	p[2].Set(m_Pointer.x,m_Pointer.y,m_Pointer.z);
        HC_Insert_Polygon(3,p);

    }
    
    if (m_Pointer.y>m_Top)
    {
	p[0].Set(m_Left, m_Top,0);
	p[1].Set(m_Right, m_Top,0);
	p[2].Set(m_Pointer.x,m_Pointer.y,m_Pointer.z);
        HC_Insert_Polygon(3,p);

    }
    if (m_Pointer.y<m_Bottom)
    {
	p[0].Set(m_Left, m_Bottom,0);
	p[1].Set(m_Right, m_Bottom,0);
	p[2].Set(m_Pointer.x,m_Pointer.y,m_Pointer.z);
        HC_Insert_Polygon(3,p);

    }
     HC_Close_Segment();
    HC_Close_Segment();
}

void HSubwindow::GetPosition(float &x,float &y)
{
    x = m_Left;
    y = m_Top;
}
void HSubwindow::GetPointerPosition(float &x,float &y)
{
    x = m_Pointer.x;
    y = m_Pointer.y;
}


void HSubwindow::SetPointerPosition(float x,float y)
{
    char text[MVO_BUFFER_SIZE];
    m_Pointer.x = x;
    m_Pointer.y = y;
    HC_Open_Segment_By_Key(m_WindowKey);
    HCLOCALE (sprintf(text, "wpointer = %f %f", m_Pointer.x, m_Pointer.y));
    HC_Set_User_Options(text); 
    HC_Close_Segment();
    ShowPointer();
}

void HSubwindow::GetSize(float &x,float &y)
{
      x = m_Right - m_Left;
      y = m_Top - m_Bottom;
}

void HSubwindow::SetPosition(float x, float y)
{
    float deltax = m_Right - m_Left;
    float deltay = m_Top - m_Bottom;
    SetSize(x,x+deltax, y-deltay, y);
}
void HSubwindow::SetSize(float deltax, float deltay)
{
    SetSize(m_Left,m_Left+deltax, m_Top - deltay, m_Top);
}

bool HSubwindow::IsSubwindow(HC_KEY key)
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

bool HSubwindow::FindSubwindow(HBaseView *view, HPoint new_pos, bool &PointerSelected)
{
	int 	res;
	HC_KEY	 key;
	HC_KEY	 subwindowkey = INVALID_KEY;
	
	HC_Open_Segment_By_Key(view->GetViewKey());
		res = HC_Compute_Selection(".", "./scene/overwrite","v, selection level = entity, no related selection limit", new_pos.x, new_pos.y);
	HC_Close_Segment();
	// compute the selection using the HOOPS window coordinate of the the pick location
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

HC_KEY HSubwindow::FindSubwindowFromSegment(HC_KEY key)
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

bool HSubwindow::IsInSizeArea(HPoint &point)
{
    if (!(m_Appearance & SW_SIZE_FIELD))
	return false;

    if (point.x > (m_Right - 0.05f) && point.y < (m_Bottom + 0.05f))
	return true;
    else
	return false;
}
bool HSubwindow::IsInMagnifyArea(HPoint &point)
{
    if (!(m_Appearance & SW_MAGNIFIER_FIELD))
	return false;

    if (point.x < (m_Left + 0.07f))
	return true;
    else
	return false;
}
bool HSubwindow::IsInMoveArea(HPoint &point)
{
  
    if (!(m_Appearance & SW_MOVE_FIELD))
	return false;

    if ((point.y < (m_Top) && point.y > (m_Top - 0.07f)) ||
	(point.y > m_Bottom && point.y < (m_Bottom + 0.07f)))
	return true;
    else
	return false;
}

bool HSubwindow::IsInDragArea(HPoint &point)
{
    if (!(m_Appearance & SW_DRAG_FIELD))
	return false;
  
    if (point.y < (m_Top - 0.07f))
	return true;
    else
	return false;
}

bool HSubwindow::IsInCloseArea(HPoint &point)
{
    if (!(m_Appearance & SW_DELETE_FIELD))
	return false;
  
    if (point.y < (m_Top) && point.y > (m_Top - 0.07f) && point.x > (m_Right - 0.03f))
	return true;
    else
	return false;
}



const char * HSubwindow::GetStaticName() { return "subwindow"; }
const char * HSubwindow::GetName() { return GetStaticName(); }



int HSubwindow::OnLButtonDown(HEventInfo &event)
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

	if (pointerselected)
		m_Action  = SW_ACTION_POINTERMOVE;
	else
	{
		if (IsInCloseArea(new_pos))
		{
		HC_Delete_By_Key(GetKey());
		GetView()->Update();
    		SetOperatorStarted(false);
		return HLISTENER_CONSUME_EVENT;
		}
		else if ((GetType() == SUBWINDOW_MAGNIFIER || GetType() == SUBWINDOW_SNAPSHOT || 
		GetType() == SUBWINDOW_POINTER) && IsInMagnifyArea(new_pos))
		m_Action = SW_ACTION_MAGNIFYING;
		else
		{
		if (IsInSizeArea(new_pos))
			m_Action = SW_ACTION_SIZING;	
		else if (IsInMoveArea(new_pos))
			m_Action = SW_ACTION_MOVE;	
		else if (IsInDragArea(new_pos) && !(GetType() == SUBWINDOW_MAGNIFIER) && !(GetType() == SUBWINDOW_POINTER))
			m_Action = SW_ACTION_DRAGGING;	
		else
		{
    			SetOperatorStarted(false);
			return HLISTENER_PASS_EVENT;
		}

		}
	}
	m_ptLast = new_pos;
	SetFirstPoint(new_pos);
	return HLISTENER_CONSUME_EVENT;
}


int HSubwindow::OnLButtonDownAndMove(HEventInfo &event)
{
    if (!OperatorStarted())
		return HLISTENER_PASS_EVENT;
    
    SetNewPoint(event.GetMouseWindowPos());
    
    HPoint delta = GetNewPoint() - m_ptLast;
    
    if (m_Action == SW_ACTION_POINTERMOVE)
    {
		float x,y;
		GetPointerPosition(x,y);
		x+=delta.x;
		y+=delta.y;
		SetPointerPosition(x,y);
    }    
    else if (m_Action == SW_ACTION_MAGNIFYING)
    {
		HPoint point(GetNewPoint());
		ChangeMagnification(GetView(), point);
		SetNewPoint(point);
    }
    else if (m_Action == SW_ACTION_MOVE)
    {
		float x,y;
		GetPosition(x,y);
		x+=delta.x;
		y+=delta.y;
		SetPosition(x,y);
    }	
    else if (m_Action == SW_ACTION_SIZING)
    {    
		float x,y;
		GetSize(x,y);
		x+=delta.x;
		y-=delta.y;
		if (x<0.01f)
			x = 0.01f;
		if (y<0.01f)
			y = 0.01f;

		SetSize(x,y);
    }
    else if (m_Action == SW_ACTION_DRAGGING)
    {    
		Drag(GetView(), m_ptLast, GetNewPoint());
    }
        
    if (GetType() == SUBWINDOW_MAGNIFIER || GetType() == SUBWINDOW_POINTER)
        Magnify(GetView());

    GetView()->Update();
    m_ptLast = GetNewPoint();

    return HLISTENER_CONSUME_EVENT;
}


int HSubwindow::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted())
		return HBaseOperator::OnLButtonUp(event);

	if (m_Action == SW_ACTION_MOVE || m_Action == SW_ACTION_MAGNIFYING)	
	    MakeKeyframe();
    else if (m_Action == SW_ACTION_SIZING)
	    MakeScaleKeyframe();
	
 	SetOperatorStarted(false);      
	return HLISTENER_CONSUME_EVENT;
}

int HSubwindow::CameraChangedEvent(HBaseView *view)
{
    HC_Open_Segment_By_Key(view->GetModelKey());
    if (HUtility::SegmentExists("subwindows"))
    {
		HC_Open_Segment("subwindows");
		HC_Begin_Contents_Search(".", "segments");
		HC_KEY segkey;
		while (HUtility::FindContents(segkey))
		{
 			GetFromKey(segkey);
			Magnify(view);
		}
		HC_End_Contents_Search();
		HC_Close_Segment();
    }
    HC_Close_Segment();
    return HLISTENER_PASS_EVENT;
    
}

int HSubwindow::PreFitWorldEvent(HBaseView *view)
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

int HSubwindow::PostFitWorldEvent(HBaseView *view)
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


int HSubwindow::SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex) 
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


int HSubwindow::ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p) 
{
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
	
		ChangeMagnification(view, matrix[14]);

		if (GetType() == SUBWINDOW_MAGNIFIER || GetType() == SUBWINDOW_POINTER)
			Magnify(view);

		GetView()->Update();
		return HLISTENER_CONSUME_EVENT;
    }

    return HLISTENER_PASS_EVENT;
}

int HSubwindow::ObjectScaledEvent(HBaseView *view, HC_KEY key, HPoint &scale) 
{
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
	 
		if (GetType() == SUBWINDOW_MAGNIFIER || GetType() == SUBWINDOW_POINTER)
			Magnify(view);

		GetView()->Update();
        return HLISTENER_CONSUME_EVENT;
    }
    return HLISTENER_PASS_EVENT;

}


void HSubwindow::MakeKeyframe() 
{	

    bool res = false;
    HLISTENER_EVENT(HAnimationListener, GetView()->GetEventManager(), KeyframeEditorExistsQuery(res));

    if (res)
    {    
		HUtility::NameSegments(m_OuterKey, GetView());
	    
		char targetname[MVO_BUFFER_SIZE];	
		char objectname[MVO_BUFFER_SIZE];
		HBhvBehaviorManager *pBehaviorManager = GetView()->GetModel()->GetBhvBehaviorManager();
		HC_Show_Segment(m_OuterKey, targetname);
		HBhvTargetObject::CreateTargetString("SPATH", targetname, targetname);
	    
		sprintf(objectname, "%d", pBehaviorManager->GetFreeName());
		HBhvAnimation *animation = pBehaviorManager->FindAnimation(targetname, "Pos");
	    
		if (animation == 0)
		{
			HBhvInterpolatorPosition *i = new HBhvInterpolatorPosition();
			HPoint parray(0,0,0);
			int tline = 0;
			i->InsertCurve(parray, 0);			
			HBhvTimeline *t = new HBhvTimeline();
			t->AddKeyframe(tline);
			animation = pBehaviorManager->AddAnimation(objectname, targetname, t, i);
			animation->GetTarget()->SetPivot(0,0,0); 	
		}
		bool replace;
		int pos = animation->GetTimeline()->AddKeyframe((int)pBehaviorManager->GetCurrentTick(), replace);

		HC_Open_Segment_By_Key(m_OuterKey);{
			float x,y;
			GetPosition(x,y);    
			HPoint p(x,y,m_Magnification);
			if (replace)
				((HBhvInterpolatorPosition *)(animation->GetInterpolator()))->ReplaceCurve(p, pos);
			else
				((HBhvInterpolatorPosition *)(animation->GetInterpolator()))->InsertCurve(p, pos);
		}HC_Close_Segment();
	    

		HLISTENER_EVENT(HAnimationListener, GetView()->GetEventManager(), KeyframeAddedEvent());
    }
}



void HSubwindow::MakeScaleKeyframe() 
{	

    bool res = false;
    HLISTENER_EVENT(HAnimationListener, GetView()->GetEventManager(), KeyframeEditorExistsQuery(res));

    if (res)
    {    
		HUtility::NameSegments(m_OuterKey, GetView());
	    
		char targetname[MVO_BUFFER_SIZE];	
		char objectname[MVO_BUFFER_SIZE];
		HBhvBehaviorManager *pBehaviorManager = GetView()->GetModel()->GetBhvBehaviorManager();
		HC_Show_Segment(m_OuterKey, targetname);
		HBhvTargetObject::CreateTargetString("SPATH", targetname, targetname);
	    
		sprintf(objectname, "%d", pBehaviorManager->GetFreeName());
		HBhvAnimation *animation = pBehaviorManager->FindAnimation(targetname, "Scale");
	    
		if (animation == 0)
		{
			HBhvInterpolatorScale *i = new HBhvInterpolatorScale();
			HPoint parray(0,0,0);
			int tline = 0;
			i->Insert(parray, 0);			
			HBhvTimeline *t = new HBhvTimeline();
			t->AddKeyframe(tline);
			animation = pBehaviorManager->AddAnimation(objectname, targetname, t, i);
			animation->GetTarget()->SetPivot(0,0,0); 	
		}
		bool replace;
		int pos = animation->GetTimeline()->AddKeyframe((int)pBehaviorManager->GetCurrentTick(), replace);

		HC_Open_Segment_By_Key(m_OuterKey);{
			float x,y;
			GetSize(x,y);    
			HPoint p(x,y,m_Magnification);
			if (replace)
				((HBhvInterpolatorScale *)(animation->GetInterpolator()))->Replace(p, pos);
			else
				((HBhvInterpolatorScale *)(animation->GetInterpolator()))->Insert(p, pos);
		}HC_Close_Segment();

	HLISTENER_EVENT(HAnimationListener, GetView()->GetEventManager(), KeyframeAddedEvent());
    }
}
