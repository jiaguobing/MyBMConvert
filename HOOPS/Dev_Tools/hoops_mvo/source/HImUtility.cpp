// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <stdlib.h>
#include <stdio.h>

#include "HImManager.h"
#include "HImUtility.h"
#include "HUtility.h"
#ifndef TEST_RIG_1
#include "HUtilityAnnotation.h"
#include "HUtilityAnnotationMeasure.h"
#else
#define BUILDING_HOOPS_INTERNALS
#endif
#include "HBaseView.h"
#include "hc.h"
#include "hic.h"

 

void HImUtility::RegisterCallbacks() 
{
	HImRegisterCallback("HImUtility_set_clip_rectangle", set_clip_rectangle);
#ifndef TEST_RIG_1
	HImRegisterCallback("HImUtility_suppress_3d_geometry", suppress_3d_geometry);
	HImRegisterCallback("HImUtility_draw_annotation_lines", draw_annotation_lines);
	HImRegisterCallback("HImUtility_draw_segment_in_background", draw_segment_in_background);
	HImRegisterCallback("HImUtility_draw_gradiated_window_background", draw_gradiated_window_background);
	HImRegisterCallback("HImUtility_device_independent_polyline_weights", draw_device_independent_line_weights);
	HImRegisterCallback("HImUtility_draw_text_infront", drawTextInfront);
	HImRegisterCallback("HImUtility_visible_hlr_polyline", visible_hlr_polyline);
	HImRegisterCallback("HIMUtility_drawline", HUtilityAnnotation::drawLineCallback);	
	HImRegisterCallback("HIMUtility_drawlineclip", HUtilityAnnotation::drawLineCallbackClip);	
	HImRegisterCallback("HIMUtility_drawlinedummy", HUtilityAnnotationMeasure::drawLineCallbackDummy);	
	HImRegisterCallback("HIMUtility_drawmarker", HUtilityAnnotation::drawMarkerCallback);	
#endif

	HImRegisterCallback("HImUtility_draw_dc_polyline_infront", draw_dc_polyline_infront);
	// this is here for legacy reasons.  Users should use "HImUtility_set_clip_rectangle"
	HImRegisterCallback("SetClipRectangle", set_clip_rectangle);	
}

#ifndef TEST_RIG_1
void HImUtility::draw_gradiated_window_background (HIC_Rendition const *rendition, HIC_Int_Rectangle const *extent) 
{
	if(HIC_Show_Window_Pattern(rendition) != FP_SOLID) {

		if(HIC_Show_Window_Pattern(rendition) != FP_WINDOW_TRANSPARENT) 
			HIC_Draw_Window(rendition, extent);

		return;
	}

	// first of all let's get the values from the HBaseView object
	void *view = HIC_Show_User_Index(rendition, H_HBASEVIEW_POINTER_MAGIC);

	HPoint wTop, wBottom;
	int percentage;

	HIC_Unsafe_Threading(rendition, true);
	((HBaseView *)view)->GetWindowColor(wTop, wBottom, percentage);
	HIC_Unsafe_Threading(rendition, false);

	float new_bottom = (float)extent->bottom + 
		((extent->top - extent->bottom) - (extent->top - extent->bottom) * (float)percentage / 100.0f) - 1;
 
	HIC_Rendition const *new_rend = HIC_New_Rendition(rendition);
	HIC_Set_Face_Displacement(new_rend,0);

	HIC_DC_Point pts1[3];
	HIC_RGBAS32 colors[3];

	pts1[0].z = pts1[1].z = pts1[2].z = (float)0.0f;

	pts1[0].x = (float)extent->left-1;
	pts1[0].y = new_bottom;

	pts1[1].x = (float)extent->right+1;		
	pts1[1].y = new_bottom;

	pts1[2].x = (float)extent->left-1;
	pts1[2].y = (float)extent->top+1;		


	HIC_RGBAS32 top = HIC_Pack_RGBAS32(char (255*wTop.x), char (255*wTop.y), char (255*wTop.z), 255);
	HIC_RGBAS32 bottom = HIC_Pack_RGBAS32(char (255*wBottom.x),char (255*wBottom.y),char (255*wBottom.z),255);

	colors[0] = bottom;
	colors[1] = bottom;
	colors[2] = top;

	HIC_Draw_DC_Gouraud_Triangle (new_rend, pts1, colors);

	// now let's modify the points/colors for the other triangle
	pts1[0].x = (float)extent->right+1;		
	pts1[0].y = new_bottom;

	pts1[1].x = (float)extent->left-1;
	pts1[1].y = (float)extent->top+1;
	
	pts1[2].x = (float)extent->right+1;		
	pts1[2].y = (float)extent->top+1;

	colors[1] = top;

	HIC_Draw_DC_Gouraud_Triangle (new_rend, pts1, colors);
 
	colors[0] = bottom;
	colors[1] = bottom;
	colors[2] = bottom;

	pts1[0].x = (float)extent->left-1;
	pts1[0].y = new_bottom;

	pts1[1].x = (float)extent->right+1;		
	pts1[1].y = new_bottom;

	pts1[2].x = (float)extent->left-1;
	pts1[2].y = (float)extent->bottom - 1;		

 
	HIC_Draw_DC_Gouraud_Triangle (new_rend, pts1, colors);

	// now let's modify the points/colors for the other triangle
	pts1[0].x = (float)extent->right+1;		
	pts1[0].y = new_bottom;

	pts1[1].x = (float)extent->left-1;
	pts1[1].y = (float)extent->bottom - 1;
	
	pts1[2].x = (float)extent->right+1;		
	pts1[2].y = (float)extent->bottom - 1;

 
	HIC_Draw_DC_Gouraud_Triangle (new_rend, pts1, colors);
	HIC_Free_Rendition(new_rend);


}
#endif


// This callback checks the segment we are being called from for some user values that
// are defined on the segment which has the callback set on it.  define the left, right, top and bottom clipping region values (in world space) 
void HImUtility::set_clip_rectangle (HIC_Rendition const *nr, HIC_Segment_Info  const *si)
{
	static int in_call = 0;

	if (in_call) {
		HIC_Draw_Segment(nr, si);
		return;
	}

	HIC_Rendition const *new_rend = HIC_New_Rendition(nr);

	HC_KEY seg = HIC_Show_Segment_Key (si);

	char sval[512];
	HPoint bl, ur;
	
	// upper right and bottom left corners of the clip region
	HIC_Show_One_User_Option_By_Key(new_rend, seg, "left", sval);
	HCLOCALE(bl.x = (float)atof(sval));
	HIC_Show_One_User_Option_By_Key(new_rend, seg, "bottom", sval);
	HCLOCALE(bl.y = (float)atof(sval));
	bl.z = 0.0f;

	HIC_Show_One_User_Option_By_Key(new_rend, seg, "right", sval);
	HCLOCALE(ur.x = (float)atof(sval));
	HIC_Show_One_User_Option_By_Key(new_rend, seg, "top", sval);
	HCLOCALE(ur.y = (float)atof(sval));
	ur.z = 0.0f;

	// Cannot use Compute_Coordinates since seg is in the include library consequently we have
	// to do the mapping from world to device coordinates manually.
	const float *matrix;
	matrix = (float const *)HIC_Show_Modelling_Matrix(new_rend);

	float	inverse[16];
	HIC_Compute_Matrix_Inverse(new_rend, matrix, inverse);

	HIC_Compute_Transformed_Points(new_rend, 1, (HIC_Point const*)&bl, inverse, (HIC_Point *)&bl);
	HIC_Compute_Transformed_Points(new_rend, 1, (HIC_Point const*)&ur, inverse, (HIC_Point *)&ur);

	matrix = (float const *)HIC_Show_Transform_Matrix(new_rend);

	HIC_Compute_Transformed_Points(new_rend, 1, (HIC_Point const*)&bl, matrix, (HIC_Point *)&bl);
	HIC_Compute_Transformed_Points(new_rend, 1, (HIC_Point const*)&ur, matrix, (HIC_Point *)&ur);

/*
	// This will draw a polyline around the clip region	
	// This draws a bounding box around the clip region
	HT_DC_Point	points[5];
	
	points[0].x = bl.x;	points[0].y = bl.y;	points[0].z = 0.0f;
	points[1].x = ur.x;	points[1].y = bl.y;	points[1].z = 0.0f;
	points[2].x = ur.x;	points[2].y = ur.y;	points[2].z = 0.0f;
	points[3].x = bl.x;	points[3].y = ur.y;	points[3].z = 0.0f;
	points[4].x = bl.x;	points[4].y = bl.y;	points[4].z = 0.0f;

	HIC_Draw_DC_Polyline(new_rend, 5, points);
*/

	HPoint min, max;

	if (ur.x > bl.x)
	{
		min.x=bl.x;
		max.x=ur.x;
	}
	else
	{
		min.x=ur.x;
		max.x=bl.x;
	}

	if (ur.y > bl.y)
	{
		min.y=bl.y;
		max.y=ur.y;
	}
	else
	{
		min.y=ur.y;
		max.y=bl.y;
	}

	HIC_Set_Clipping (new_rend, (int)min.x, (int)max.x, (int)min.y, (int)max.y);

	in_call = 1;
	HIC_Draw_Segment(new_rend, si);
	in_call = 0;
	HIC_Free_Rendition(new_rend);
}

#ifndef TEST_RIG_1
void HImUtility::draw_segment_in_background(HIC_Rendition const *nr, HIC_DC_Point const *points, HIC_RGBA const *colors, HIC_Plane const *planes, HIC_Parameter const*params, HIC_Integer32 param_width, HIC_Parameter_Flags param_flags)
{
	float depth = HIC_Show_Current_Z_Max(nr);
	float back_z = depth - 24.0f;
	HIC_DC_Point	new_pts[3];

	// set face displacement to 0 so that 'wrapping' does
	// not occur
	HIC_Rendition const *new_rend = HIC_New_Rendition(nr);
	HIC_Set_Face_Displacement(new_rend,0);

	for (int i=0;i<3;i++)
	{
		new_pts[i].x = points[i].x;
		new_pts[i].y = points[i].y;
		new_pts[i].z = back_z;
	}

	HIC_Draw_DC_Reshaded_Triangle(new_rend, new_pts, colors, planes, params, param_width, param_flags);

	HIC_Free_Rendition(new_rend);
}



void HImUtility::suppress_3d_geometry(HIC_Rendition const *rendition,HIC_Geometry const* geo3)
{
	UNREFERENCED(rendition);
	UNREFERENCED(geo3);
}

// don't draw lines if text visibility is turned off
void HImUtility::draw_annotation_lines(HIC_Rendition const *nr, HIC_Polyline const*poly)
{
	int vis_flags;

	vis_flags = HIC_Show_Visibility(nr);

	if (vis_flags & 0x00002000L)
		HIC_Draw_3D_Polyline (nr, poly);

	return;
}
#endif


void HImUtility::draw_dc_polyline_infront (HIC_Rendition const *rendition,int count, HIC_DC_Point const* geo3)
{
	float       new_z = 0;

	if (HIC_Using_Analytic_HSRA(rendition))
		new_z = -1.0e5f;


	// first let's put the geometry in the front z bucket
	for (int i = 0;i < count; i++)
		((HIC_DC_Point *)geo3)[i].z = new_z;

	// set face displacement to 0 so that 'wrapping' does
	// not occur
	HIC_Rendition const*new_rend = HIC_New_Rendition(rendition);
	HIC_Set_Face_Displacement(new_rend,0);

	// now let's do the weighting so that we have device independent line line weights
	float	resolution		= HIC_Show_Device_Max_Pixels_X(rendition)/HIC_Show_Device_Max_Size_X(rendition);
	int display_weight = HIC_Show_Line_Weight(rendition);
	float new_weight = resolution/40*display_weight;
	HIC_Set_Line_Weight(new_rend, (int)new_weight);
	HIC_Draw_DC_Polyline(new_rend,count,geo3);

	HIC_Free_Rendition(new_rend);
}

#ifndef TEST_RIG_1
void HImUtility::draw_device_independent_line_weights (HIC_Rendition const*rendition,int count, HIC_DC_Point const* geo3)
{
	float	resolution		= HIC_Show_Device_Max_Pixels_X(rendition)/HIC_Show_Device_Max_Size_X(rendition);

	int		display_weight	= HIC_Show_Line_Weight(rendition);
	float	new_weight		= resolution/40*display_weight;

	HIC_Rendition const *new_rend = HIC_New_Rendition(rendition);
	HIC_Set_Line_Weight(new_rend, (int)new_weight);
	HIC_Draw_DC_Polyline(new_rend,count,geo3);
	HIC_Free_Rendition(new_rend);
}




void HImUtility::drawTextInfront (HIC_Rendition const*rendition, HIC_Text_Info const* text_info)
{
	HIC_Point new_p = *HIC_Show_Text_Position(text_info);
	
	if (HIC_Using_Analytic_HSRA(rendition))
		new_p.z = -1.0e5f;
	else
		new_p.z = 0.0f;

	HIC_Set_Text_Position(text_info, &new_p);

 	HIC_Draw_Text(rendition,text_info);
}
#endif


void HImUtility::visible_hlr_polyline(HIC_Rendition const*nr, int count, HIC_DC_Point const*points)
{
    //void *view = HIC_Show_User_Index(nr, H_HBASEVIEW_POINTER_MAGIC);
    bool colorHLRData = false;
    HC_KEY newKey = 0L;
    static float color[3];

    if (colorHLRData)
    {
	//HC_KEY segKey = HIC_Show_Geometry_Owner(nr);

	static HC_KEY origKey = 0;
	
	newKey = HIC_Show_Geometry_Key(nr);

	if (newKey != origKey)
	{
	    color[0] = (float)rand()/(float)RAND_MAX;
	    color[1] = (float)rand()/(float)RAND_MAX;
	    color[2] = (float)rand()/(float)RAND_MAX;

	    origKey = newKey;
	}
    }

    HPoint *pts = new HPoint[count];
    for (int i=0; i<count; i++) {

		((HIC_DC_Point *)points)[i].z = 0.0f;
		int pixelHeight = HIC_Show_Device_Pixels_Y (nr);
		((HIC_DC_Point *)points)[i].y = pixelHeight - points[i].y;

		H_FORMAT_TEXT dvrseg("/driver/image/%s+%d", HDB::GetInstanceBase(), HDB::GetUniqueID());
		HC_Open_Segment(dvrseg);
			HC_Compute_Coordinates(".", "local pixels", &points[i], "world", &pts[i]);
		HC_Close_Segment();
		HC_Delete_Segment(dvrseg);
    }

    HC_Open_Segment("/include library/cachedHiddenLines");

    if (colorHLRData)
    {
	char buf[MVO_BUFFER_SIZE];
	sprintf(buf, "%ld", newKey);
	HC_Open_Segment(buf);
	    if (!HC_Show_Existence("color"))
	    {
		HC_Set_Color_By_Value("lines", "RGB", color[0], color[1], color[2]);
		HC_Set_Visibility("lines=on");
		HC_Set_Rendering_Options("depth range = (0.0,0.001)");
	    }
	    HC_Insert_Polyline(count, pts);
        HC_Close_Segment();
    }
    else 
        HC_Insert_Polyline(count, pts);

    HC_Close_Segment();

    delete []pts;
}
