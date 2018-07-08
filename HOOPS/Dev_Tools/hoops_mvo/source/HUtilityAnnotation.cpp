// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hc.h"
#include "hic.h"
#include "HUtility.h"
#include "HImUtility.h"
#include "HImManager.h"
#include "HBaseView.h"
#include "HUtilityAnnotation.h"
#include "HEventManager.h"
#include "HObjects.h"
#include "HUtilityLocaleString.h"
#include "utf_utils.h"
#include "varray.h"



//general clipping routine that takes an array (p)  of size npts points and clips it to given 
//boundaries (top,left,bottom,right). The result is a pointer to the clipped point array (pout).
int HUtilityAnnotation::Clip_Line(HIC_DC_Point *pin, HIC_DC_Point *pout, int top, int left, int bottom, int right)
{
	int j = 0;
 	HIC_DC_Point a[2], b[2];
	HIC_DC_Point * p = a;
	HIC_DC_Point * po = b;
	p[0] = pin[0]; p[1] = pin[1];

	if ((right - p[0].x) * (right - p[1].x) < 0)
	{ 
		po[0].y = p[1].y+(p[0].y-p[1].y)*(right- p[1].x) / (p[0].x - p[1].x);
		po[0].z = p[1].z+(p[0].z-p[1].z)*(right- p[1].x) / (p[0].x - p[1].x);
		po[0].x = (float)right;
		j=1;
	}
	
	if (p[0].x<=right)
		po[j++] = p[0];
  	if (p[1].x<=right)
		po[j++] = p[1];
		
	if (j<2)
		return false;

	HIC_DC_Point *tmp;
	tmp = p; p = po; po = tmp; j=0;
	if ((left - po[0].x) * (left - p[1].x) < 0)
	{ 
		po[0].y = p[1].y+(p[0].y-p[1].y)*(left- p[1].x) / (p[0].x - p[1].x);
		po[0].z = p[1].z+(p[0].z-p[1].z)*(left- p[1].x) / (p[0].x - p[1].x);
		po[0].x = (float)left;
		j=1;
	}
	
	if (p[0].x>=left)
		po[j++] = p[0];
 	if (p[1].x>=left)
		po[j++] = p[1];
  
	if (j<2)
		return false;
 
	tmp = p; p = po; po = tmp; j=0;
	if ((top - p[0].y) * (top - p[1].y) < 0)
	{ 
		po[0].x = p[1].x+(p[0].x-p[1].x)*(top- p[1].y) / (p[0].y - p[1].y);
		po[0].z = p[1].z+(p[0].z-p[1].z)*(top- p[1].y) / (p[0].y - p[1].y);
		po[0].y = (float)top;
		j=1;
	}
	
	if (p[0].y>=top)
		po[j++] = p[0];
 
 	if (p[1].y>=top)
		po[j++] = p[1];
  
 	if (j<2)
		return false;

	tmp = p; p = po; po = tmp; j=0;
	if ((bottom - p[0].y) * (bottom - p[1].y) < 0)
	{ 
		po[0].x = p[1].x+(p[0].x-p[1].x)*(bottom- p[1].y) / (p[0].y - p[1].y);
		po[0].z = p[1].z+(p[0].z-p[1].z)*(bottom- p[1].y) / (p[0].y - p[1].y);
		po[0].y = (float)bottom;
		j=1;
	}
 
	if (p[0].y<=bottom)
		po[j++] = p[0];
  	if (p[1].y<=bottom)
		po[j++] = p[1];

 	if (j<2)
		return false;
 	else
	{
		pout[0] = po[0];
		pout[1] = po[1];
		return true;
	}
}


static bool TestRight(HIC_DC_Point *p, HIC_DC_Point *po, int top, int left, int bottom, int right)
{
	UNREFERENCED(left);

	HIC_DC_Point tmp;
	tmp.y =  p[1].y+(p[0].y-p[1].y)*(right- p[1].x) / (p[0].x - p[1].x);
	if (tmp.y >= top && tmp.y <= bottom)
	{
		po[0].z = p[1].z+(p[0].z-p[1].z)*(right- p[1].x) / (p[0].x - p[1].x);
		po[0].y = tmp.y;
		po[0].x = (float)right;
		if (p[0].x > (float)right)
			po[1] = p[0];
		else
			po[1] = p[1];
		return true;
	}
	return false;
}

static bool TestLeft(HIC_DC_Point *p, HIC_DC_Point *po, int top, int left, int bottom, int right)
{
	UNREFERENCED(right);

	HIC_DC_Point tmp;
	tmp.y =  p[1].y+(p[0].y-p[1].y)*(left- p[1].x) / (p[0].x - p[1].x);
	if (tmp.y >= top && tmp.y <= bottom)
	{
		po[0].z = p[1].z+(p[0].z-p[1].z)*(left- p[1].x) / (p[0].x - p[1].x);
		po[0].y = tmp.y;
		po[0].x = (float)left;

		if (p[0].x < left)
			po[1] = p[0];
		else
			po[1] = p[1];		
		return true;
	}
	return false;
}



static bool TestTop(HIC_DC_Point *p, HIC_DC_Point *po, int top, int left, int bottom, int right)
{
	UNREFERENCED(bottom);

	HIC_DC_Point tmp;
	tmp.x =  p[1].x+(p[0].x-p[1].x)*(top - p[1].y) / (p[0].y - p[1].y);
	if (tmp.x >= left && tmp.x <= right)
	{
		po[0].z = p[1].z+(p[0].z-p[1].z)*(top- p[1].y) / (p[0].y - p[1].y);
		po[0].x = tmp.x;
		po[0].y = (float)top;
		if (p[0].y < (float)top)
			po[1] = p[0];
		else
			po[1] = p[1];
		return true;		
	}
	return false;
}

static bool TestBottom(HIC_DC_Point *p, HIC_DC_Point *po, int top, int left, int bottom, int right)
{
	UNREFERENCED(top);

	HIC_DC_Point tmp;

	tmp.x =  p[1].x+(p[0].x-p[1].x)*(bottom - p[1].y) / (p[0].y - p[1].y);
	if (tmp.x >= left && tmp.x <= right)
	{
		po[0].z = p[1].z+(p[0].z-p[1].z)*(bottom- p[1].y) / (p[0].y - p[1].y);
		po[0].x = tmp.x;
		po[0].y = (float)bottom;
		if (p[0].y > (float)bottom)
			po[1] = p[0];
		else
			po[1] = p[1];		
		return true;
	}
	return false;
}
//general clipping routine that takes an array (p)  of size npts points and clips it to given 
//boundaries (top,left,bottom,right). The result is a pointer to the clipped point array (pout).
int HUtilityAnnotation::Clip_Line_Inverse(HIC_DC_Point *p, HIC_DC_Point *po, int top, int left, int bottom, int right)
{ 
	int c = 0;
	HIC_DC_Point newp[2];
	for (int i=0;i<2;i++)
	{
		if (i==0)
		{
			newp[0] = p[1];
			newp[1] = p[0];
		}
		else
		{
			newp[0] = p[0];
			newp[1] = p[1];
		}
	if (newp[1].x > right && newp[1].y > bottom)
	{
		if (TestRight(newp,po+c,top, left, bottom, right))
			c+=2;
		else
		{
			if (TestBottom(newp,po+c,top, left, bottom, right))
				c+=2;
		}
	}
	else if (newp[1].x > right && newp[1].y < top)
	{
		if (TestRight(newp,po+c,top, left, bottom, right))
			c+=2;
		else
		{
			if (TestTop(newp,po+c,top, left, bottom, right))
				c+=2;
		}
	}	
	else if (newp[1].x > right && (newp[1].y <= bottom && newp[1].y >= top))
	{
		if (TestRight(newp,po+c,top, left, bottom, right))
			c+=2;
	}
	else if (newp[1].x >= left && newp[1].x <= right && newp[1].y < top)
	{		
		if (TestTop(newp,po+c,top, left, bottom, right))
			c+=2;		
	}	
	else if (newp[1].x >= left && newp[1].x <= right && newp[1].y > bottom)
	{		
		if (TestBottom(newp,po+c,top, left, bottom, right))
			c+=2;		
	}	
	else if (newp[1].x < left && newp[1].y > bottom)
	{
		if (TestLeft(newp,po+c,top, left, bottom, right))
			c+=2;
		else
		{
			if (TestBottom(newp,po+c,top, left, bottom, right))
				c+=2;
		}
	}
	else if (newp[1].x < left && newp[1].y < top)
	{
		if (TestLeft(newp,po+c,top, left, bottom, right))
			c+=2;
		else
		{
			if (TestTop(newp,po+c,top, left, bottom, right))
				c+=2;
		}
	}	
	else if (newp[1].x < left && (newp[1].y <= bottom && newp[1].y >= top))
	{
		if (TestLeft(newp,po+c,top, left, bottom, right))
			c+=2;
	}

	}
	return c;

}


void HUtilityAnnotation::drawMarkerCallback (HIC_Rendition const  *rendition, HIC_DC_Point * p)
{	
	UNREFERENCED(rendition);
	UNREFERENCED(p);

#if 0
 	int key = (int)HIC_Show_Geometry_Key(rendition);
if (key < 0) return;	// HACK
	float * pos = (float *)HIC_Show_User_Index(rendition, H_MARKUP_POS_MAGIC);
if (!pos) return;		// HACK
	pos[(key -1) * 3]  = (*p).x;
	pos[(key-1) * 3+1]  = (*p).y;
	pos[(key-1) * 3+2]  = (*p).z;	
#endif
}

#if 1
void HUtilityAnnotation::drawLineCallbackClip (HIC_Rendition const  *rendition,int count, HIC_DC_Point * old_points)
{   
	UNREFERENCED(count);

	HC_KEY geo = HIC_Show_Original_Geometry_Key(rendition);

	if (geo == INVALID_KEY)
		return;

	HC_KEY start = HIC_Show_Owner_Original_Key_By_Key(rendition, geo);

	if (start == INVALID_KEY)
		return;

	HC_KEY key = HIC_Find_Relative_Segment(rendition, start, "../matrix/mask transform/background");

	if (key == INVALID_KEY)
		return;

	
	HIC_Segment_Path const * walk_path =  HIC_Show_Segment_Path(rendition);

	HC_KEY path[128];
	int ii=0;

	for(int i = 0; i < walk_path->count; i++) {

		path[ii++] = walk_path->keys[i];

		if (walk_path->keys[i] == start)
			path[ii++] = key;	
	}
	
	HIC_Free_Segment_Path(walk_path);

	HIC_Point p[4];
	
	p[0].x = -1;
	p[0].y = -1;
	p[0].z = 0;

	p[1].x = 1;
	p[1].y = -1;
	p[1].z = 0;

	p[2].x = 1;
	p[2].y = 1;
	p[2].z = 0;

	p[3].x = -1;
	p[3].y = 1;
	p[3].z = 0;

	// needs y flip
	float matrix[16];

	if (!HIC_Compute_Transform_By_Path(rendition, ii, path, "object", "outer pixels", matrix))
		return;

	HIC_Compute_Transformed_Points (rendition, 4, p, matrix, p);

	int y_fudge = HIC_Show_Device_Pixels_Y(rendition) - 1;

	p[0].y = y_fudge - p[0].y;
	p[1].y = y_fudge - p[1].y;
	p[2].y = y_fudge - p[2].y;
	p[3].y = y_fudge - p[3].y;


	int		left, right,  bottom, top;

	if (p[0].x < p[1].x) {
		left = (int)p[0].x;
		right = (int)p[1].x;
	}
	else {
		left = (int)p[1].x;
		right = (int)p[0].x;
	}

	if (p[1].y > p[2].y) {
		bottom = (int)p[1].y;
		top = (int)p[2].y;
	}
	else {
		bottom = (int)p[2].y;
		top = (int)p[1].y;
	}



 	HIC_DC_Point lo2[4];
	HIC_DC_Point inv[2]; 	
 				

	inv[0] = old_points[1];
	inv[1] = old_points[0];
	int cnum;
	if ((cnum = Clip_Line_Inverse((HIC_DC_Point *)old_points,lo2, top, left, bottom, right)) != 0)
	{
		inv[0] = lo2[1];
		inv[1] = lo2[0];
		HIC_Draw_DC_Polyline (rendition, 2, (HIC_DC_Point *)inv);
		if (cnum>2)
		{
			inv[0] = lo2[3];
			inv[1] = lo2[2];
			HIC_Draw_DC_Polyline (rendition, 2, (HIC_DC_Point *)inv);
		}
	}
  		  	
}
#endif



void HUtilityAnnotation::drawLineCallback (HIC_Rendition const  *rendition, int count, HIC_DC_Point * old_points)
{   
	UNREFERENCED(count);

	//HBaseView *view = (HBaseView *)HIC_Show_User_Index(rendition, H_HBASEVIEW_POINTER_MAGIC);

	//if (!view)
	//	return;

	HC_KEY geo = HIC_Show_Original_Geometry_Key(rendition);

	if (geo == INVALID_KEY)
		return;

	HC_KEY start = HIC_Show_Owner_Original_Key_By_Key(rendition, geo);

	if (start == INVALID_KEY)
		return;

	HC_KEY key = HIC_Find_Relative_Segment(rendition, start, "../matrix/mask transform/background");

	if (key == INVALID_KEY)
		return;

	
	HIC_Segment_Path const * walk_path =  HIC_Show_Segment_Path(rendition);

	HC_KEY path[128];
	int ii=0;

	for(int i = 0; i < walk_path->count; i++) {

		path[ii++] = walk_path->keys[i];

		if (walk_path->keys[i] == start)
			path[ii++] = key;	
	}
	
	HIC_Free_Segment_Path(walk_path);

	HIC_Point p[4];
	
	p[0].x = -1;
	p[0].y = -1;
	p[0].z = 0;

	p[1].x = 1;
	p[1].y = -1;
	p[1].z = 0;

	p[2].x = 1;
	p[2].y = 1;
	p[2].z = 0;

	p[3].x = -1;
	p[3].y = 1;
	p[3].z = 0;

	// needs y flip
	float matrix[16];

	if (!HIC_Compute_Transform_By_Path(rendition, ii, path, "object", "outer pixels", matrix))
		return;

	HIC_Compute_Transformed_Points (rendition, 4, p, matrix, p);

	int y_fudge = HIC_Show_Device_Pixels_Y(rendition) - 1;

	p[0].y = y_fudge - p[0].y;
	p[1].y = y_fudge - p[1].y;
	p[2].y = y_fudge - p[2].y;
	p[3].y = y_fudge - p[3].y;
	

	float left, right;
	if (p[0].x <= p[1].x) {
		left = p[0].x;
		right = p[1].x;
	}
	else {
		left = p[1].x;
		right = p[0].x;
	}

	HIC_DC_Point pnew[2];
	pnew[0] = old_points[0];
	if (old_points[0].x < old_points[1].x)
		pnew[1].x = left;
	else
		pnew[1].x = right;
	if (old_points[0].y < old_points[1].y)
		pnew[1].y = p[1].y;
	else
		pnew[1].y = p[2].y;

 	pnew[1].z = old_points[1].z;

	if (!(old_points[0].x > left && old_points[0].x < right && old_points[0].y>p[1].y && old_points[0].y < p[2].y))
		HIC_Draw_DC_Polyline (rendition, 2, pnew);

 	
}


int HUtilityAnnotation::g_width = 0;;
int HUtilityAnnotation::g_height = 0;



void HUtilityAnnotation::AddChar(char the_char)
{
	unsigned short unicode_char = (unsigned short)the_char;
	AddChar(unicode_char);
}


void 
HUtilityAnnotation::AddChar(unsigned short the_char){
	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/matrix/text");{
			unsigned short * text;
			int text_length = GetTextLength();

			if(the_char == '\b'){
				/* back space */
				text = new unsigned short[text_length +2];
				GetTextWithEncoding(text, text_length);
				if(text_length > 0)
					text[text_length/2-1] = 0;
			} else {
				text = new unsigned short[text_length +4];
				GetTextWithEncoding(text, text_length);
				text[text_length/2] = (unsigned short)the_char;
				text[text_length/2+1] = 0;
			}
			
			HC_Delete_By_Key(m_MyTextKey);
			m_MyTextKey = HC_KInsert_Text_With_Encoding(0, 0, 0, "unicode", (char const*)text);
			
			delete [] text;

		}HC_Close_Segment();
	}HC_Close_Segment();
}

HC_KEY HUtilityAnnotation::Insert(
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key,
		int bg_type,
		bool useArrow,
		HC_KEY *key_path,
		int key_path_length)
{
	return InsertWithEncoding(
		(unsigned short*)H_UTF16("Note").encodedText(),
			target,
			position,
			AnnotateThis,
			pView,
			layer_key ,
			bg_type ,
			useArrow,
			key_path,
			key_path_length);
}


HC_KEY HUtilityAnnotation::Insert(
		const char * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key,
		int bg_type,
		bool useArrow,
		HC_KEY *key_path,
		int key_path_length)
{
	HC_KEY retval;
	retval = InsertWithEncoding(
		(unsigned short*)H_UTF16(text).encodedText(),
			target,
			position,
			AnnotateThis,
			pView,
			layer_key ,
			bg_type ,
			useArrow,
			key_path,
			key_path_length);
	return retval;
}


HC_KEY HUtilityAnnotation::InsertWithEncoding(
		const unsigned short * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key,
		int bg_type,
		bool useArrow,
		HC_KEY *key_path,
		int key_path_length)
{
	HC_Open_Segment_By_Key(AnnotateThis);{
		m_MyKey = HC_KOpen_Segment("");{

 			HC_Set_Rendering_Options("no simple shadow, display lists = off, no frame buffer effects");
			HC_Set_Heuristics("no static model");
			HC_Set_Visibility ("shadows = off");  //no shadow map casting or receiving
			HC_Set_Conditional_Action ("prune segment=segment", "disable_annotation_highlighting"); 

			HObjectManager::SetHObjectSegment("Annotation");
			HObjectManager::SetHObjectInfo(GetType());

			HC_Open_Segment("conditions");{

				SetConditionalVisibility(pView, key_path, key_path_length);
				HC_Create_Segment("matrix");
				HC_Open_Segment("xarrow");
					//HC_Set_Rendering_Options("general displacement = -2");
					if(!useArrow)
						HC_Set_Visibility("off");
					HC_Set_Marker_Size(0.3);
				HC_Close_Segment();


				HC_Set_Heuristics("exclude bounding");
				HC_Set_Visibility ("no lights");

				char const lcap_def[] = {10,0,0, -1, 4,	0,0, 16,8,	16,-8, 0,0, 0};
				//	char const rcap_def[] = {10,0,0, -1, 4,	0,0, -8,4, -8,-4, 0,0, 0};

				HC_Define_Glyph("HUtilityAnnotation lcap_def", sizeof(lcap_def), lcap_def);
				//	HC_Define_Glyph("HUtilityAnnotation rcap_def", sizeof(rcap_def), rcap_def);
				HC_Define_Line_Style("HUtilityAnnotation line style", 
								"start cap = HUtilityAnnotation lcap_def,");
				HC_Set_Line_Pattern("HUtilityAnnotation line style");

				float matrix[16];
				if(key_path_length && key_path)
					HC_PShow_Net_Modelling_Matrix(key_path_length, key_path, matrix);
				else
					HC_Show_Net_Modelling_Matrix(matrix);
				HC_Compute_Matrix_Inverse(matrix, matrix);
				HC_Set_Modelling_Matrix(matrix);
				

				HC_Set_Heuristics("culling = off, no backplane cull");

				if(layer_key != INVALID_KEY){
					char style_path[1024];
					HUtility::Find_Relative_Path(m_MyKey, layer_key, style_path);
					HC_Style_Segment(style_path);
				}

				HC_Set_Rendering_Options("no lighting");
				HC_Set_Selectability("off");

				HC_Set_Line_Weight(1);
				HC_Set_Edge_Weight(1);
				SetColor("faces = (white, mirror=black), lines = red, edges = red, text = black, transmission=black");
								
				HC_Open_Segment("matrix");{
					char matrix_segment_face_displacement[MVO_BUFFER_SIZE];			
					strcpy(matrix_segment_face_displacement, "face displacement = 8");

					HC_Set_Rendering_Options(matrix_segment_face_displacement);		

					HC_Open_Segment("text");{
						HC_Set_Text_Alignment("**");
						char font[1024*4];
						HC_Open_Segment_By_Key(pView->GetSceneKey());
							HC_Show_Net_Text_Font(font);
						HC_Close_Segment();
						HC_Set_Text_Font(font);
						HC_Set_Text_Font("transforms = off");

					}HC_Close_Segment();

					HC_Open_Segment("mask transform");{

						HC_Open_Segment("background");
							HC_Set_Heuristics("no backplane culling");
						HC_Close_Segment();

						HC_Set_Selectability("text = on, faces = on");

						char font_size[128], units[64];
						float size;

						HC_QShow_One_Text_Font("../text", "size", font_size);
						HCLOCALE (sscanf(font_size, "%f %s", &size, units)); 

						HC_UnSet_One_Rendering_Option("mask transform");
						if(streq(units,"oru") ){
							HC_Set_Rendering_Options("mask transform = (camera = (rotate, perspective), modelling matrix = (rotate, scale))");
						} else {
							HC_Set_Rendering_Options("mask transform = (rotate, scale, perspective)");
						}

					}HC_Close_Segment();

				}HC_Close_Segment();
			}HC_Close_Segment();
		}HC_Close_Segment();
	}HC_Close_Segment();

	m_position = position;
	m_target = target;
	HCamera cam;
	pView->GetCamera(&cam);
	MoveText(position, &cam);
	SetBackgroundType(bg_type);
	SetTextWithEncoding(text);

	return GetKey();
}

void
HUtilityAnnotation::SetConditionalVisibility(HBaseView * pView,
											 HC_KEY const * key_path,
											 int const key_path_length)
{
	HC_Define_System_Options("no update control");

	// tag all the includers and prepare our final condition tag
	char sel_tag[MVO_BUFFER_SIZE];

	sprintf(sel_tag, "annotation_%lu", (unsigned long)GetKey());

	bool includes_found = false;
	for(int i = 0; i < key_path_length -1; ++i){
		if(IsAnnotation(key_path[i]))
			continue;

		if(key_path[i] == pView->GetModelKey()){
			break;
		}

		char type[32];
		HC_Show_Key_Type(key_path[i], type);
		if(streq(type, "include")){
			includes_found = true;
			HC_Open_Segment_By_Key(HC_KShow_Owner_Original_Key(key_path[i]));{
				HC_Set_Conditions(sel_tag);
			}HC_Close_Segment();

		}	
	}

	if(includes_found){
		HC_Open_Segment("conditional visibility");{
			HC_Set_Visibility("off, cutting planes=on, faces=on, lines=on, edge=on, text=on, markers = on");
		}HC_Close_Segment();

		HC_Open_Segment_By_Key(GetKey());{
			HC_Set_Visibility("off");
		}HC_Close_Segment();
		
		HC_KConditional_Style("conditional visibility", sel_tag);
	}
	else {
		HC_Set_Visibility("off, cutting planes=on, faces=on, lines=on, edge=on, text=on, markers = on");
	}

	HC_Define_System_Options("update control");
}


HC_KEY 
HUtilityAnnotation::GetBackgroundSegmentKey(){
	HC_KEY retval;

	HC_Open_Segment_By_Key(m_MyKey);{
		retval = HC_KOpen_Segment("conditions/matrix/mask transform/background");{
		}HC_Close_Segment();
	}HC_Close_Segment();

	return retval;
}


HC_KEY
HUtilityAnnotation::GetKey(HC_KEY key){

	if(HUtilityAnnotation::IsNote(key))
		return HObjectManager::FindHObjectSegment(key);
	else
		return INVALID_KEY;
}


void
HUtilityAnnotation::GetText(char * text, int &length){
	length = GetTextLength();
	unsigned short * unicode = new unsigned short[length +32];
	GetTextWithEncoding(unicode, length);
	strcpy(text, H_ASCII_TEXT((wchar_t*)unicode));
	delete [] unicode;
}


int
HUtilityAnnotation::GetTextLength(){
	int retval;
	HC_Show_Text_Length(m_MyTextKey, &retval);
	return retval;
}

void
HUtilityAnnotation::GetTextWithEncoding(unsigned short * text, int &length){
	float fdummy;
	length = GetTextLength();
	if(text){
		char encoding[64];
		HC_Show_Text_With_Encoding(m_MyTextKey, &fdummy, &fdummy, &fdummy, encoding, (char*)text);
	}
}

bool 
HUtilityAnnotation::IsAnnotation(HC_KEY key){
	char type[512];

	if(key == INVALID_KEY)
		return false;

	HObjectManager::FindHObjectSegment(key, type);

	if(streq(type, "Annotation") || streq(type, "`Annotation`"))
		return true;
	return false;
}

bool 
HUtilityAnnotation::IsNote(HC_KEY key){
	char type[512];
	char subtype[512];

	if(key == INVALID_KEY)
		return false;

	HObjectManager::FindHObjectSegment(key, type, subtype);

	if(streq(type, "Annotation") && streq(subtype, "HUtilityAnnotation"))
		return true;
	return false;
}

HPoint HUtilityAnnotation::GetFacing() const
{
	HPoint to_camera(0, 0, 0);

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/xarrow");{
			char options[512] = {""};
			if(HC_Show_Existence("user options = vector to camera")){
				HC_Show_One_User_Option("vector to camera", options);
				HCLOCALE(sscanf(options, "(%f, %f, %f)", &to_camera.x, &to_camera.y, &to_camera.z));
			}
		}HC_Close_Segment();
	}HC_Close_Segment();
	HC_Compute_Normalized_Vector(&to_camera, &to_camera);

	return to_camera;
}

HPoint HUtilityAnnotation::GetUp() const
{
	HPoint up(0, 0, 0);

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/xarrow");{
			char options[512] = {""};
			if(HC_Show_Existence("user options = up direction")){
				HC_Show_One_User_Option("up direction", options);
				HCLOCALE(sscanf(options, "(%f, %f, %f)", &up.x, &up.y, &up.z));
			}
		}HC_Close_Segment();
	}HC_Close_Segment();
	HC_Compute_Normalized_Vector(&up, &up);

	return up;
}


void 
HUtilityAnnotation::MoveText(HPoint const & pos, HCamera const * camera)
{
	m_position = pos;

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/xarrow");{
			HC_Flush_Contents(".","geometry");

			HC_Insert_Line(m_target.x, m_target.y, m_target.z,
				m_position.x, m_position.y, m_position.z);
			HImSetCallback("draw dc polyline","HIMUtility_drawline");

			if(camera){
				/* This will be used if the annotation is going to be exported to U3D. */
				HPoint to_camera = HPoint(camera->position.x - camera->target.x,
										  camera->position.y - camera->target.y,
										  camera->position.z - camera->target.z);
				char options[512] = {""};
				sprintf(options, 
					"vector to camera = (%g, %g, %g),"
					"up direction = (%g, %g, %g)",
					to_camera.x, to_camera.y, to_camera.z,
					camera->up_vector.x, camera->up_vector.y, camera->up_vector.z);
				HC_Set_User_Options(options);
			}
		}HC_Close_Segment();

		HC_Open_Segment("conditions/matrix");{
			HC_UnSet_Modelling_Matrix();
			HC_Translate_Object(m_position.x, m_position.y, m_position.z);
		}HC_Close_Segment();
	}HC_Close_Segment();
}

void
HUtilityAnnotation::recursive_resize(HC_KEY seg_key, int include_count, HC_KEY *include_keys, HC_KEY view_key)
{
	static char static_char_buffer[MVO_BUFFER_SIZE];
	HC_KEY key;

	if (!HC_Show_Existence_By_Key(seg_key, "self"))
		return;

	HC_Open_Segment_By_Key(seg_key);{
		HC_Begin_Contents_Search(".", "segments");{
			
			while(HC_Find_Contents(0, &key)){
				if(HUtilityAnnotation::IsAnnotation(key)){
					HUtilityAnnotation tmp;
					tmp.SetNote(HObjectManager::FindHObjectSegment(key), view_key);
					if (tmp.m_MyTextKey != INVALID_KEY) {
						HC_Open_Segment_By_Key(tmp.GetKey()); {
							HC_Open_Segment("conditions/matrix"); {

								strcpy(static_char_buffer, "");
								HC_Show_One_Net_Rendering_Option("depth range", static_char_buffer);
								static char const * text_segment_general_displacement;
								static char const *  matrix_segment_general_displacement;
								if (strcmp(static_char_buffer, "0,1") == 0)
								{
									matrix_segment_general_displacement = "general displacement = -8";
									text_segment_general_displacement = "general displacement = -64";
								}
								else
								{
									matrix_segment_general_displacement = "general displacement = -2";
									text_segment_general_displacement = "general displacement = -8";
								}
								HC_Set_Rendering_Options(matrix_segment_general_displacement);
								HC_Open_Segment("text"); {
									HC_Set_Rendering_Options(text_segment_general_displacement);
									HC_Set_Rendering_Options("hlro = (visibility, dim factor = 0)");
									tmp.Resize(include_count, include_keys, view_key);
								}HC_Close_Segment();
							}HC_Close_Segment();
						}HC_Close_Segment();
					}
				} 
				else {
					recursive_resize(key, include_count, include_keys, view_key);
				}
			}
		}HC_End_Contents_Search();

		HC_Begin_Contents_Search(".", "includes");{
			while(HC_Find_Contents(0, &key)){
				HC_Show_Include_Segment(key, static_char_buffer);
				key=HC_KCreate_Segment(static_char_buffer);		
				recursive_resize(key, include_count, include_keys, view_key);
			}
		}HC_End_Contents_Search();

	}HC_Close_Segment();
}

void
HUtilityAnnotation::ChangeAllFonts(HBaseView * view, char const * const font, HC_KEY seg_key)
{
	if(seg_key == INVALID_KEY)
		seg_key = view->GetModelKey();

	HC_KEY include_key = view->GetIncludeLinkKey();

	recursive_change_font(seg_key, 1, &include_key, view->GetViewKey(), font);
	recursive_resize(seg_key, 1, &include_key, view->GetViewKey());
}

void
HUtilityAnnotation::recursive_change_font(HC_KEY seg_key, int include_count, HC_KEY *include_keys, HC_KEY view_key, char const * const font)
{
   	if (!HC_Show_Existence_By_Key(seg_key, "self"))
		return;

	HC_Open_Segment_By_Key(seg_key);{
		HC_Begin_Contents_Search(".", "segments, includes");{
			/* This is defined static to save stack space */
			VCharArray unused(128);
			VCharArray full_path_name(MVO_BUFFER_SIZE);
			/* This key can be static since be the time we recurse, we're done with it */
	 		static HC_KEY key;
			while(HC_Find_Contents(unused, &key)){
				if(HUtilityAnnotation::IsAnnotation(key)){
					HUtilityAnnotation tmp;
					tmp.SetNote(HObjectManager::FindHObjectSegment(key), view_key);
					tmp.SetTextFont(font);
				} 
				else if(streq(unused, "include"))
				{
					HC_Show_Include_Segment(key, full_path_name);
					key=HC_KCreate_Segment(full_path_name);		
					recursive_change_font(key, include_count, include_keys, view_key, font);
				}
				else {
					recursive_change_font(key, include_count, include_keys, view_key, font);
				}
			}
		}HC_End_Contents_Search();
	}HC_Close_Segment();
}

void 
HUtilityAnnotation::SetBackgroundType(long type){

	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/matrix/mask transform/background");{
			HC_Set_User_Index(0, (void*)(MVO_POINTER_SIZED_INT)type);
			HC_Flush_Contents(".","geometry");
#if 0
			HC_Open_Segment("markers");
			HC_Flush_Contents(".","geometry");
			HC_KEY k1 = HC_KInsert_Marker(-1,-1,0);
			HC_KEY k2 = HC_KInsert_Marker(1,-1,0);
			HC_KEY k3 = HC_KInsert_Marker(1,1,0);
			HC_KEY k4 = HC_KInsert_Marker(-1,1,0);
			HC_Renumber_Key(k1,1,"local");
			HC_Renumber_Key(k2,2,"local");
			HC_Renumber_Key(k3,3,"local");
			HC_Renumber_Key(k4,4,"local");
			HImSetCallback("draw dc marker","HIMUtility_drawmarker");
			HC_Set_Visibility("markers = on");
			HC_Close_Segment();
#endif
 			switch(type){
				case BG_QUAD:{
					float points[][3] = {
						{-1, -1, 0},
						{1, -1, 0},
						{1, 1, 0},
						{-1, 1, 0},
					};
				
					HC_Insert_Polygon(4, points);
				}break;

				case BG_CLOUD:{
					int const POINT_COUNT = 180;
					float points[POINT_COUNT*3];
					float theta, r;
					int i;
					for(i=0; i<POINT_COUNT; ++i){
						theta = static_cast<float>(i/POINT_COUNT * 2 * 3.1415926535);
						r = static_cast<float>(fabs(cos((5*theta))/5.0f) + 1.25);
						points[i*3 +0] = r * cos((float)theta);
						points[i*3 +1] = r * sin((float)theta);
						points[i*3 +2] = 0;
					}
					HC_Insert_Polygon(POINT_COUNT, points); 
					HC_Set_Heuristics("concave polygons");
				};break;

				case BG_CIRCLE:{
					float center[] = {0,0,0};
					float normal[] = {0,0,1};
					HC_Insert_Circle_By_Radius(center, 1.42, normal);
				}break;

				case BG_NONE:break;
				default:break;
			}
		}HC_Close_Segment();
	}HC_Close_Segment();
}


void 
HUtilityAnnotation::SetColor(char const * color){
	HC_Open_Segment_By_Key(GetKey());{
		HC_Set_Color(color);
		HC_Set_Rendering_Options("attribute lock=(no color)");
	}HC_Close_Segment();
}


void 
HUtilityAnnotation::SetEdgeWeight(float weight){
	HC_Open_Segment_By_Key(GetKey());{
		HC_Set_Edge_Weight(weight);
	}HC_Close_Segment();
}


void
HUtilityAnnotation::SetExtents(int xpixels, int ypixels, HC_KEY view_key){
	UNREFERENCED(view_key);

	char buffer[1024];

	HC_Open_Segment_By_Key(m_MyKey);
#if 0
		HC_UnSet_One_User_Option("xpixels");
		HC_UnSet_One_User_Option("ypixels");
#endif
		sprintf(buffer, "xpixels=%d,ypixels=%d", xpixels, ypixels);
		HC_Set_User_Options(buffer);
	HC_Close_Segment();
}


void 
HUtilityAnnotation::SetFacePattern(char const * pattern){
	HC_Open_Segment_By_Key(GetKey());{
		HC_Set_Face_Pattern(pattern);
	}HC_Close_Segment();
}



void 
HUtilityAnnotation::SetLinePattern(char const * pattern){
	HC_Open_Segment_By_Key(GetKey());{
		HC_Set_Line_Pattern(pattern);
	}HC_Close_Segment();
}


void 
HUtilityAnnotation::SetLineWeight(float weight){
	HC_Open_Segment_By_Key(GetKey());{
		HC_Set_Line_Weight(weight);
	}HC_Close_Segment();
}

void
HUtilityAnnotation::SetNote(HC_KEY Annotation, HC_KEY view_key)
{
	UNREFERENCED(view_key);

	char buffer[MVO_BUFFER_SIZE];

	HC_Show_Key_Type(Annotation, buffer);
	
	if( streq(buffer, "segment")){
		m_MyKey = Annotation;
	} else {
		HC_Show_Owner_By_Key(Annotation, buffer);
		HC_Open_Segment(buffer);{
			m_MyKey = HC_KOpen_Segment("../../../..");{
			}HC_Close_Segment();
		}HC_Close_Segment();
	}

	HC_Open_Segment_By_Key(m_MyKey);{

		HC_KEY key;
		HC_Open_Segment("conditions");
		if (HUtility::SegmentExists("arrow"))
		{
			HC_Open_Segment("..");
			if (HC_Show_Existence("heuristics = quick moves"))
				HC_UnSet_One_Heuristic("quick moves");
			HC_Close_Segment();
			HC_Define_Line_Style("HUtilityAnnotation line style", 
							"start cap = HUtilityAnnotation lcap_def,");

			HC_Rename_Segment("arrow", "xarrow");
			HC_Open_Segment("xarrow");
			HImSetCallback("draw dc polyline","HIMUtility_drawline");
			HC_Close_Segment();
			HC_Open_Segment("matrix/mask transform/background");
			HImSetCallback("draw dc marker","HIMUtility_drawmarker");
			HC_Close_Segment();
			HC_Open_Segment_By_Key(GetBackgroundSegmentKey());			
			SetBackgroundType(1);
			HC_Close_Segment();
		}
		HC_Close_Segment();

 
		HC_Open_Segment("conditions/xarrow");{
			HC_Begin_Contents_Search(".","lines");{
				if (HC_Find_Contents(buffer, &key))
				{
					HC_Show_Line(key, &m_target.x, &m_target.y, &m_target.z,
								 &m_position.x, &m_position.y, &m_position.z);
				}
			}HC_End_Contents_Search();
		}HC_Close_Segment();
		
		HC_Open_Segment("conditions/matrix/text");{
			HC_Begin_Contents_Search(".","text");{
				HC_Find_Contents(buffer, &m_MyTextKey);
			}HC_End_Contents_Search();

			if (m_MyTextKey == INVALID_KEY)
			{
				HC_KEY referenceKey, originalKey;
				HC_Begin_Contents_Search(".","reference");{
					if (HC_Find_Contents(buffer, &referenceKey)) {
						originalKey = HC_Show_Reference_Geometry(referenceKey);
						HC_Show_Key_Type(originalKey, buffer);
						if (streq(buffer, "text"))
							m_MyTextKey = originalKey;
						else if (streq(buffer, "segment"))
						{
							HC_Open_Segment_By_Key(originalKey);
							HC_Begin_Contents_Search(".", "text");
							{
								HC_Find_Contents(buffer, &m_MyTextKey);
							}
							HC_End_Contents_Search();
							HC_Close_Segment();
						}
					}
				}HC_End_Contents_Search();
			}
		}HC_Close_Segment();
	
	}HC_Close_Segment();

}

void HUtilityAnnotation::SetNote(HC_KEY noteskey, HBaseView* pView)
{
	SetNote(noteskey, pView->GetViewKey()); 
}

void
HUtilityAnnotation::SetOnTop(bool on_top)
{
	HC_Open_Segment_By_Key(GetKey());{
		if(on_top){
			HC_Set_Rendering_Options("depth range = (0.0,0.1)");
			HC_Set_User_Options("on top");
		}else{
			HC_UnSet_One_Rendering_Option("depth range");
			HC_UnSet_One_User_Option("on top");
		}
	}HC_Close_Segment();
}


bool
HUtilityAnnotation::GetOnTop()
{
	char buffer[MVO_BUFFER_SIZE];

	HC_Open_Segment_By_Key(GetKey());{
		if (HC_Show_Existence("user option=on top"))
			HC_Show_One_User_Option("on top", buffer);
		else
			return false;
	}HC_Close_Segment();

	if(streq(buffer, "on"))
		return  true;
	
	return false;
}


void 
HUtilityAnnotation::SetText(char const * text){
	SetTextWithEncoding((unsigned short*)H_UTF16(text).encodedText());
}


void
HUtilityAnnotation::SetTextFont(char const * font){
	HC_Open_Segment_By_Key(m_MyKey);{

		HC_Open_Segment("conditions/matrix");
		
			HC_Open_Segment("text");{
				HC_Set_Text_Font(font);
				HC_Set_Text_Font("transforms = off");
			}HC_Close_Segment();

			HC_Open_Segment("mask transform");{

				char font_size[128], units[64];
				float size;

				HC_QShow_One_Text_Font("../text", "size", font_size);
				HCLOCALE (sscanf(font_size, "%f %s", &size, units)); 

				HC_UnSet_One_Rendering_Option("mask transform");
				if( streq(units,"oru") ){
					HC_Set_Rendering_Options("mask transform = (camera = (rotate, perspective), modelling matrix = (rotate, scale))");
				} else {
					HC_Set_Rendering_Options("mask transform = (rotate, scale, perspective)");
				}
			}HC_Close_Segment();

		HC_Close_Segment();

	}HC_Close_Segment();
}


void 
HUtilityAnnotation::SetTextWithEncoding(unsigned short const * text){
	HC_Open_Segment_By_Key(m_MyKey);{
		HC_Open_Segment("conditions/matrix/text");{
			HC_Begin_Contents_Search(".","text");{
				char type[64];
				HC_KEY key;
				while(HC_Find_Contents(type, &key))
					HC_Delete_By_Key(key);
			}HC_End_Contents_Search();
			m_MyTextKey = HC_KInsert_Text_With_Encoding(0, 0, 0, "unicode", (char const*)text);
		}HC_Close_Segment();
	}HC_Close_Segment();
}

void
HUtilityAnnotation::ShowPosition(HPoint &p){
	p = m_position;
}

void
HUtilityAnnotation::ShowPosition(float *x, float *y, float *z){
	*x = m_position.x, *y = m_position.y, *z = m_position.z;
}

void
HUtilityAnnotation::MoveText(HPoint const & tar, HPoint const & pos, HCamera const * camera){
	m_target = tar;
	MoveText(pos, camera);
}

void
HUtilityAnnotation::ShowTarget(HPoint &p){
	p = m_target;
}

void
HUtilityAnnotation::ShowTarget(float *x, float *y, float *z){
	*x = m_target.x, *y = m_target.y, *z = m_target.z;
}

void
HUtilityAnnotation::UnSetExtents(){
	
	HC_Open_Segment_By_Key(m_MyKey);
		HC_UnSet_One_User_Option("xpixels");
		HC_UnSet_One_User_Option("ypixels");
	HC_Close_Segment();
}


void HUtilityAnnotation::Resize(int include_count, HC_KEY *include_keys, HC_KEY view_key)
{
	int i;
	HC_KEY keys[16];
	int key_count = include_count+2;
	for(i=0;i<include_count;i++) {
		keys[i+1] = include_keys[i];
	}
	keys[key_count-1] = view_key;

	int text_size = GetTextLength();
	unsigned short * unicode = new unsigned short[text_size +2];
	GetTextWithEncoding(unicode, text_size);

	char buffer[256] = {""};
	int win_pixels_x = 0, win_pixels_y = 0;
	HC_Open_Segment_By_Key(view_key);
		HC_Show_Device_Info(".","pixels", buffer);
		sscanf(buffer, "%d,%d", &win_pixels_x, &win_pixels_y);
	HC_Close_Segment();

	HC_Open_Segment_By_Key(m_MyKey);
		float xfrac = 0, yfrac = 0;
		float Xxfrac = 0, Xyfrac = 0;
		char font_size[128] = {""}, units[64] = {""};
		float size = 0;

		HC_QShow_One_Text_Font("conditions/matrix/text", "size", font_size);
		HCLOCALE (sscanf(font_size, "%f %s", &size, units)); 

		keys[0] = HC_KOpen_Segment("conditions/matrix/text");

			HC_Compute_Text_Extent_Enc_Path(key_count, keys, "unicode", (char const*)unicode, &xfrac, &yfrac);
			HC_Compute_Text_Extent_By_Path(key_count, keys, "X", &Xxfrac, &Xyfrac);
			// add in our own extra space
			xfrac += Xxfrac;
			yfrac += Xyfrac;
		HC_Close_Segment();

		if (HC_Show_Existence("user options = xpixels") && HC_Show_Existence("user options = ypixels")) {

			HC_Show_One_User_Option("xpixels", buffer);
			int xpixels = atoi(buffer);

			HC_Show_One_User_Option("ypixels", buffer);
			int ypixels = atoi(buffer);

			// remove unwanted extra space
			xfrac -= Xxfrac;
			yfrac -= Xyfrac;
			
			if (win_pixels_x != 0 && win_pixels_y != 0) {
				float m_fUserXFrac, m_fUserYFrac;
				
				m_fUserXFrac = xpixels/(float)win_pixels_x;
				m_fUserYFrac = ypixels/(float)win_pixels_y;
				
				if (fabs((float)(xfrac - m_fUserXFrac)) > 0.01 || 
					fabs((float)(yfrac > m_fUserYFrac)) > 0.01) {
					
					if (fabs(xfrac - m_fUserXFrac) < fabs(yfrac - m_fUserYFrac))
						size *= m_fUserXFrac / xfrac;
					else
						size *= m_fUserYFrac / yfrac;
					
					size *= 0.5f;
					
					if (size > 0.0f){
						char font[128];
						HCLOCALE (sprintf(font, "size=%f %s", size, units));
						HC_QSet_Text_Font("conditions/matrix/text", font);
					}
				}
				xfrac = m_fUserXFrac;
				yfrac = m_fUserYFrac;
			}
		}

		HC_Open_Segment("conditions/matrix/mask transform/background");
			HC_UnSet_Modelling_Matrix();

			HPoint ws(xfrac, yfrac);
			
			if (streq (units, "oru"))
				HC_Compute_Coordinates_By_Path(key_count, keys, "local window", &ws, "viewpoint", &ws);
			else {
				float field_width, field_height, field_aspect;
				float win_aspect;

				HC_Compute_Coordinates_By_Path(key_count, keys, "local window", &ws, "projection", &ws);

				HC_PShow_Net_Camera_Field(key_count, keys, &field_width, &field_height);

				field_aspect = field_height/field_width;

				win_aspect = (float)win_pixels_y/(float)win_pixels_x;

				if (win_aspect > field_aspect) {
					ws.y /= win_aspect/field_aspect;
					ws.x /= win_aspect/field_aspect;
				}

				ws.x /= field_aspect;


#if 0
				if (field_aspect < win_aspect)
					ws.y *= field_height/field_width;
				else
					ws.x *= field_width/field_height;
#endif
			}

			float xscale = ws.x;
			float yscale = ws.y;
	
			HC_Scale_Object( fabs(xscale), fabs(yscale), 0);
		HC_Close_Segment();

		HC_QSet_User_Options("conditions/matrix/text", H_FORMAT_TEXT("text scale = (%g, %g)", fabs(xscale), fabs(yscale)));

	HC_Close_Segment();

	delete [] unicode;
}


void
HUtilityAnnotation::WindowResizeCallback(int signal, void * signal_data, void * user_data)
{
	UNREFERENCED(signal);
	UNREFERENCED(signal_data);

	HBaseView * view = (HBaseView*)user_data;
	HC_KEY markup_key;

	HC_Open_Segment_By_Key(view->GetOverwriteKey());
		markup_key=HC_KCreate_Segment("markup");
	HC_Close_Segment();

	HC_KEY include_key = view->GetIncludeLinkKey();

	if (include_key == -1)
		include_key = view->GetModelKey();		
	recursive_resize(view->GetModelKey(), 1, &include_key, view->GetViewKey());
	recursive_resize(markup_key, 0, 0, view->GetViewKey());
	
	view->Update();
}
