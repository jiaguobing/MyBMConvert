// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityAntialiasing implementation of the HUtilityAntialiasing class
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "HUtility.h"
#include "HUtilityAntialiasing.h"
#include "HBaseView.h"
#include "HImManager.h"
 
#include "hc.h"
#include "hic.h"

#define SIZELIMIT 1024*768
 

void HUtilityAntialiasing::skip_segment_tree (HIC_Rendition *rendition, HIC_Segment_Info *seg)
{
	UNREFERENCED(rendition);
	UNREFERENCED(seg);
}

/*
void HUtilityAntialiasing::draw_thick_lines (HT_Rendition *rendition, HT_Segment_Info * geo3)
{
 //	HIC_Set_Edge_Weight(rendition, 6);
//	HIC_Set_Line_Weight(rendition,6); 
	HIC_Draw_Segment(rendition, geo3);
}
*/
static int g_factor;
void HUtilityAntialiasing::draw_thick_polyedges(HIC_Rendition *rendition, int count, HIC_DC_Point *points)
{
 	int ew = HIC_Show_Edge_Weight(rendition);
 	int lw = HIC_Show_Line_Weight(rendition);
	HIC_Set_Line_Weight(rendition, ew*(g_factor+2));
	HIC_Draw_DC_Polyline(rendition, count, points);
	HIC_Set_Line_Weight(rendition, lw);

}
void HUtilityAntialiasing::draw_thick_polylines(HIC_Rendition *rendition, int count, HIC_DC_Point *points)
{
 	int lw = HIC_Show_Line_Weight(rendition);
	HIC_Set_Line_Weight(rendition, lw*(g_factor+2));
	HIC_Draw_DC_Polyline(rendition, count, points);
	HIC_Set_Line_Weight(rendition, lw);

}

void HUtilityAntialiasing::RegisterCallbacks()
{
	HImRegisterCallback("HUtilityAntialiasing_skip_segment_tree", skip_segment_tree);
 	HImRegisterCallback("HUtilityAntialiasing_draw_thick_polyedges", draw_thick_polyedges);
	HImRegisterCallback("HUtilityAntialiasing_draw_thick_polylines", draw_thick_polylines);
}


/////////////////////////////////////////////////////////////////////////////
// HUtilityAntialiasing


HUtilityAntialiasing::HUtilityAntialiasing(HBaseView *view)
{
	m_pHView = view;

	m_pAAImageData				= 0;
	m_width						= 0;
	m_height					= 0;
 	m_bIsShown					= false;
	m_factor					= 4;
	m_AntialisedImageSegmentKey = 0;
	strcpy(m_drawsegmenttree_callback_name,"");
	m_bFirst = true;
	strcpy(largeImageSegment,"");
	strcpy(imagedriver,"");
	m_bFirstFirst = true;

}




HUtilityAntialiasing::~HUtilityAntialiasing()
{
    if (strcmp(largeImageSegment,"")!=0)
    {
	    HC_Delete_Segment (largeImageSegment);
	    HC_Delete_Segment (imagedriver);
    }

	delete [] m_pAAImageData;
	if (m_AntialisedImageSegmentKey)
		HC_Delete_By_Key(m_AntialisedImageSegmentKey);
}



void HUtilityAntialiasing::GetWindowSize(int &xmax, int &ymax)
{
	HC_Open_Segment_By_Key(m_pHView->GetViewKey());

	char data[MVO_BUFFER_SIZE], temp[MVO_BUFFER_SIZE];
	HC_Show_Device_Info(".","pixels",data);
	HC_Parse_String(data,",",0,temp);
	xmax = atoi(temp);
	HC_Parse_String(data,",",-1,temp);
	ymax = atoi(temp);
	HC_Close_Segment();
}





bool HUtilityAntialiasing::CreateImage()
{
	int al_x; 
	int al_y;  
	char					buffer[MVO_BUFFER_SIZE];
 	char					wstring[MVO_BUFFER_SIZE];
   
	HPoint					window_color;

	float dummy;
	int x,y;
	bool success = true;
 
	GetWindowSize(x,y);
	m_pHView->SetUpdateInterrupted(false);

	g_factor = m_factor;
	if (x*y > SIZELIMIT)
		g_factor = 2;
	else
		g_factor = 2;
  	if (x!=m_width || y!= m_height)
	{
	    m_bFirst = true;
	    if (!m_bFirstFirst)
	    {	    
	    HC_Delete_Segment (largeImageSegment);
	    HC_Delete_Segment (imagedriver);
	    }
	}

	m_bFirstFirst = false;
	al_x = x * g_factor;
	al_y = y * g_factor;
 	HC_Parse_String(m_pHView->GetDriverPath(),"/",-1,wstring);
 
 	sprintf(largeImageSegment, "?driver/null/temp/%s", wstring);
 	
	if (m_bFirst)
	{
 	HC_Open_Segment (largeImageSegment);
		image_key = HC_KInsert_Image(0.0, 0.0, 0.0, "rgb, name=al_image", al_x,al_y, 0);
	HC_Close_Segment ();
	}
	m_pHView->GetWindowColor(window_color);
  	
	if (streq(m_pHView->GetDriverType(), "opengl"))			
	{


		sprintf(imagedriver, "?driver/opengl/image%s", wstring);
 		char callbackname[MVO_BUFFER_SIZE];
		strcpy(callbackname,"");


		HPoint rgbtop, rgbbottom;
		char color[MVO_BUFFER_SIZE];	

	    HC_Open_Segment (imagedriver);
		
		if (m_bFirst)
		{
		    
		    HC_Flush_Contents(".", "includes");
		    m_pHView->GetWindowColor(rgbtop,rgbbottom);
		    HCLOCALE(sprintf(color, "windows=(r=%f g=%f b=%f), window contrast = (r=%f g=%f b=%f)", rgbtop.x, rgbtop.y, rgbtop.z, rgbbottom.x, rgbbottom.y, rgbbottom.z));
		    HC_Set_Color(color);
		    HC_Set_Window_Pattern("Down");
		    
		    
		    
		    HC_Set_Color_By_Value("windows", "RGB", window_color.x, window_color.y, window_color.z);
		    sprintf (buffer, "debug = 0x1100000, use window id = (image key = %s%p), subscreen = (-1, 1, -1, 1)", H_EXTRA_POINTER_FORMAT, (void*)image_key);
		    HC_Set_Driver_Options (buffer);
		    HC_Set_Driver_Options ("isolated");
		    HC_Set_Window_Frame("off");
		    HImSetCallback("draw dc edges","HUtilityAntialiasing_draw_thick_polyedges");
		    HImSetCallback("draw dc polyline","HUtilityAntialiasing_draw_thick_polylines");
		    
		    HC_Open_Segment_By_Key(m_pHView->GetViewKey());
		    HImShowCallback("draw window", callbackname); 
		    HC_Close_Segment();
		    if (strcmp(callbackname, "") != 0)
			HImSetCallback("draw window", callbackname);
		    
		    if (m_pHView->GetHandedness() == HandednessRight)
			HC_Set_Handedness("right");
		    else
			HC_Set_Handedness("left");
		    HC_Set_User_Index(H_HBASEVIEW_POINTER_MAGIC, m_pHView);
		    
		    HC_Include_Segment_By_Key (m_pHView->GetSceneKey());
		    
		}	
		HC_Close_Segment ();
		HC_Update_One_Display (imagedriver);
		
		
	}
	else
	{
	    sprintf(imagedriver, "?driver/image/%s", wstring);

	HC_Open_Segment (imagedriver);	

	if (m_bFirst)
	{
 	HC_Flush_Contents(".", "includes");
	
		HPoint rgbtop, rgbbottom;
		char color[MVO_BUFFER_SIZE];
 	    m_pHView->GetWindowColor(rgbtop,rgbbottom);
	     HCLOCALE(sprintf(color, "windows=(r=%f g=%f b=%f), window contrast = (r=%f g=%f b=%f)", rgbtop.x, rgbtop.y, rgbtop.z, rgbbottom.x, rgbbottom.y, rgbbottom.z));
 		HC_Set_Color(color);
		HC_Set_Window_Pattern("Down");

 		HC_Control_Update_By_Key(m_pHView->GetViewKey(), "unset shadow regeneration");
 		HC_Control_Update(".", "unset shadow regeneration");

		HC_Set_Driver_Options("no gamma correction");
		HC_Set_Driver_Options("update interrupts");
		HC_Set_Color_By_Value("windows", "RGB", window_color.x, window_color.y, window_color.z);
		sprintf (buffer, "use window id = (image key = %s%p)", H_EXTRA_POINTER_FORMAT, (void*)image_key);
		if (m_bFirst)
		    HC_Set_Driver_Options (buffer);
		HC_Set_Driver_Options ("isolated");
		HC_Set_Rendering_Options("hsra = szb");
		HC_Set_Window_Frame("off");
  		HImSetCallback("draw dc edges","HUtilityAntialiasing_draw_thick_polyedges");
		HImSetCallback("draw dc polyline","HUtilityAntialiasing_draw_thick_polylines");	
		char callbackname[MVO_BUFFER_SIZE];
		strcpy(callbackname,"");
		HC_Open_Segment_By_Key(m_pHView->GetViewKey());
		HImShowCallback("draw window", callbackname); 

		HC_Close_Segment();
		if (strcmp(callbackname, "") != 0)
			HImSetCallback("draw window", callbackname);
		
		if (m_pHView->GetHandedness() == HandednessRight)
			HC_Set_Handedness("right");
		else
			HC_Set_Handedness("left");
		HC_Set_User_Index(H_HBASEVIEW_POINTER_MAGIC, m_pHView);

 		HC_Include_Segment_By_Key (m_pHView->GetSceneKey());
		}
	HC_Close_Segment ();
		
 		
		m_pHView->SetUpdateInterrupted(false);
		
		if (m_pHView->GetEventCheckerCallback())
		{	
		    	HC_Define_System_Options ("no event checker");	
			char options[MVO_BUFFER_SIZE];
			sprintf (options, "event checker = %s%p", H_EXTRA_POINTER_FORMAT, (void*)m_pHView->GetEventCheckerCallback());
	//		HC_Define_System_Options(options);
			
			m_pHView->SetUpdateInterrupted(false);
			
			HC_Update_One_Display (imagedriver);
//			HC_Update_Display();
			HC_Define_System_Options ("no event checker");		
		}
		else		
			HC_Update_One_Display (imagedriver);

	}
	
	if (m_pHView->GetUpdateInterrupted())
	{
		success = false;
	}
	else
	{
		
		HPixelRGB * image_data = new HPixelRGB[al_x*al_y];
		
		if (!m_pAAImageData || x != m_width  || y != m_height || 1)
		{
			m_width =  x;
			m_height = y;
			
			if (!m_AntialisedImageSegmentKey || 1)
			{
				char					AntialisedImageSegment[MVO_BUFFER_SIZE];
				sprintf(AntialisedImageSegment, "?driver/null/antialised/%s", wstring);
				m_AntialisedImageSegmentKey = HC_KCreate_Segment(AntialisedImageSegment);
			}
			
			if (m_pAAImageData)
				delete [] m_pAAImageData;
			m_pAAImageData = new HPixelRGB[m_width*m_height];
			HC_Open_Segment_By_Key(m_AntialisedImageSegmentKey);
			HC_Flush_Contents(".", "everything");
			sprintf(buffer, "rgb, name = %s", wstring);
			HC_Insert_Image_By_Ref(0,0,0,buffer, m_width, m_height, m_pAAImageData);
			HC_Close_Segment();
			
			
		}
		else
		{
			HC_Open_Segment_By_Key(m_AntialisedImageSegmentKey);
			HC_Flush_Contents(".", "everything");
			sprintf(buffer, "rgb, name = %s", wstring);
			HC_Insert_Image_By_Ref(0,0,0,buffer, m_width, m_height, m_pAAImageData);
			HC_Close_Segment();

		}

	char	format[MVO_BUFFER_SIZE];
	HC_Show_Image(image_key, &dummy,&dummy,&dummy,format, &al_x, &al_y, image_data);
	int div = g_factor * g_factor;
	m_pHView->SetUpdateInterrupted(false);

	for (int i=0;i<x; i++)
	{
//		if (m_pHView->GetEventCheckerCallback())
//	   		(m_pHView->GetEventCheckerCallback())();
  		if (m_pHView->GetUpdateInterrupted())
		{
			success = false;
			break;
		}
 
		for (int j=0;j<y;j++)
		{
			int ii = i*g_factor;
			int jj = j*g_factor;
			int r,g,b;
			r = 0;
			g = 0;
			b = 0;
			
			for (int f=0;f<g_factor;f++)
			{
				for (int gg=0;gg<g_factor;gg++)
				{
					r += image_data[ii+gg+(jj+f)*al_x].r();
					g += image_data[ii+gg+(jj+f)*al_x].g();
					b += image_data[ii+gg+(jj+f)*al_x].b();
				}
			}
			
			m_pAAImageData[i+j*x] = HPixelRGB(
				static_cast<unsigned char>(r/div),
				static_cast<unsigned char>(g/div),
				static_cast<unsigned char>(b/div));
		}
	}
 	delete [] image_data;
 
	}
//	if (m_bFirst)
	{
//	    HC_Delete_Segment (largeImageSegment);
//	    HC_Delete_Segment (imagedriver);
	}
	m_bFirst = false;
 	return success;
   	
}
 


void HUtilityAntialiasing::Show(int factor)
{
	char					buffer[MVO_BUFFER_SIZE];
 	char					wstring[MVO_BUFFER_SIZE];

	m_factor = factor;

 	if (CreateImage())
	{

  	HC_Open_Segment_By_Key(m_pHView->GetSceneKey());
	HImShowCallback("draw segment tree", m_drawsegmenttree_callback_name); 
	HImSetCallback("draw segment tree","HUtilityAntialiasing_skip_segment_tree");
 		HC_Control_Update(".", "unset shadow regeneration");

//     HC_Control_Update (".", "redraw everything");
	HC_Close_Segment();
 
 	HC_Open_Segment_By_Key(m_pHView->GetViewKey());
	HC_Set_Visibility("lights = off");
	HImShowCallback("draw window", m_drawwindow_callback_name); 
	if (strcmp(m_drawwindow_callback_name,"") != 0)
		HImUnSetOneCallback("draw window");
 		HC_Control_Update(".", "unset shadow regeneration");

  	HC_Parse_String(m_pHView->GetDriverPath(),"/",-1,wstring);
	sprintf(buffer, "window = %s", wstring);
  	HC_Set_Color(buffer);
	HC_Close_Segment();
 
 	m_bIsShown = true;
	}
	else
	{
	 	m_bIsShown = false;
	}

}


void HUtilityAntialiasing::Hide()
{
	HPoint					window_color;


	HC_Open_Segment_By_Key(m_pHView->GetViewKey());
	m_pHView->GetWindowColor(window_color);
		HC_Set_Color_By_Value("windows", "RGB", window_color.x, window_color.y, window_color.z);

	if (strcmp(m_drawwindow_callback_name,"") != 0)
		HImSetCallback("draw window", m_drawwindow_callback_name);

  	HC_Close_Segment();
	HC_Open_Segment_By_Key(m_pHView->GetSceneKey());

	if (strcmp(m_drawsegmenttree_callback_name,"") != 0)
		HImSetCallback("draw segment tree", m_drawsegmenttree_callback_name)
	else
		HImUnSetOneCallback ("draw segment tree");
 
  	HC_Close_Segment();

  	m_bIsShown = false;

}


bool HUtilityAntialiasing::GetShowState()
{
	return m_bIsShown;
}
