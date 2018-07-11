// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtility.cpp : implementation of the HUtility class
//

#include "HDB.h"
#include "HBaseView.h"
#include "HBaseModel.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"

#include "HUtility.h"
#include "HTools.h"
#include "hc.h"
#include "hic.h"
#include "HImManager.h"
#include "HSelectionSet.h"
#include "HSelectionItem.h"
#include "HUtilityAnnotation.h"
#include "HStream.h"
#include "HModelInfo.h"
#include "vlist.h"
#include "vhash.h"
#include "varray.h"
#include "HUtilityLocaleString.h"

#include "HConstantFrameRate.h"
#include "HEventManager.h"
#include "HUtilityGeometryCreation.h"
#include "HDispatchXML.h"
#include "HUtilityTextParser.h"

#include "HIOManager.h"
#include "tempfile_utils.h"


#ifndef _WIN32_WCE
#include <time.h>
#endif

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wctype.h>
#include <wchar.h>
#include <stdio.h>
#include "zlib.h"

#include <map>
#include <string>

using namespace std;

#ifndef _MSC_VER
#include <sys/types.h>
#include <dirent.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <inttypes.h>
#else
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#include <windows.h>
#define int32_t __int32
#define int16_t __int16
#endif

#include "hmutex.h"
const float HUtility::PI=3.1415926f;
const float HUtility::EH_Epsilon=0.01f;

#define bool2BOOL(trueOrfalse)	\
	( trueOrfalse ) ? true : false


/////////////////////////////////////////////////////////////////////////////
// HOpConstruct

enum HClipPolygonType
{
	polygonInterior = 1,
	polygonBoundary = 0,
	polygonExterior = -1
};


#define boundaryEpsilon 1.0e-9F
	
void HUtility::Find_Relative_Path(HC_KEY from_key, HC_KEY to_key, char * path)
{
	char to_path[1024*8];
	char from_path[1024*8];
	char relative_path[1024*8];
	char buffer[1024*8];
	char * ptr;
	int i;

	HC_Show_Segment(to_key, to_path);
	HC_Show_Segment(from_key, from_path);

	for (i = 1; to_path[i] == from_path[i]; ++i);
	while(to_path[i-1] != '/') --i;
	strcpy(buffer, to_path +i); strcpy(to_path, buffer);
	strcpy(buffer, from_path +i-1); strcpy(from_path, buffer);
	for (i=0, ptr = from_path; *ptr != '\0'; ++ptr){
		if (*ptr == '/'){
			sprintf(relative_path + 3*i, "../");
			++i;
		}
	}
	sprintf(relative_path + 3*i, "%s", to_path);

	strcpy(path, relative_path);
}

void HUtility::Order (HPoint *one, HPoint *two) {

	if (one->x > two->x) {
		float tmp;
		tmp = one->x; one->x = two->x; two->x = tmp;
	}
	if (one->y > two->y) {
		float tmp;
		tmp = one->y; one->y = two->y; two->y = tmp;
	}
	if (one->z > two->z) {
		float tmp;
		tmp = one->z; one->z = two->z; two->z = tmp;
	}
}



float HUtility::GetTime()
{
#ifndef _WIN32_WCE
 	return  ((float)(clock())) /  (float) CLOCKS_PER_SEC;
#else
	// return  ((float)(wince_clock())) /  (float) CLOCKS_PER_SEC;
	return 0.0;
#endif
}



HC_KEY HUtility::InsertRectangle (
	const char	*	seg,
	float       x0,
	float       y0,
	float       x1,
	float       y1,
	float       z,
	bool        fill) 
{
	HPoint  pts[5];
	HC_KEY	key;

	pts[0].x = x0;   pts[0].y = y0;   pts[0].z = z;
	pts[1].x = x0;   pts[1].y = y1;   pts[1].z = z;
	pts[2].x = x1;   pts[2].y = y1;   pts[2].z = z;
	pts[3].x = x1;   pts[3].y = y0;   pts[3].z = z;
	pts[4].x = x0;   pts[4].y = y0;   pts[4].z = z;

	if (fill){
		int i;
		int flist[] = {4,0,1,2,3};
		key = HC_QKInsert_Shell(seg,4,pts,5, flist);
		HC_Open_Geometry (key);
		for (i = 0 ; i < 4 ; i++) {
			HC_Open_Vertex (i);
				HC_Set_Visibility ("off");
			HC_Close_Vertex ();
		}
		HC_Close_Geometry ();
	} else {
		key = HC_QKInsert_Polyline (seg, 5, pts);
	}
	return key;
}



bool HUtility::PointsEqual(HPoint *point1, HPoint *point2) {

	return *point1 == *point2;
}


void HUtility::ClampPointToWindow (HPoint *point) {

    if      (point->x < -1.0)  point->x = -1.0F;
    else if (point->x >  1.0)  point->x =  1.0F;

    if      (point->y < -1.0)  point->y = -1.0F;
    else if (point->y >  1.0)  point->y =  1.0F;

    if      (point->z < -1.0)  point->z = -1.0F;
    else if (point->z >  1.0)  point->z =  1.0F;


    return;

}



void HUtility::GenerateCuboidPoints(   
	HPoint const	*max,
    HPoint const	*min,
	HPoint			*points){

    points[0].x = min->x;
    points[0].y = min->y;
    points[0].z = min->z;

    points[1].x = max->x;
    points[1].y = min->y;
    points[1].z = min->z;

    points[2].x = max->x;
    points[2].y = max->y;
    points[2].z = min->z;

    points[3].x = min->x;
    points[3].y = max->y;
    points[3].z = min->z;

    points[4].x = min->x;
    points[4].y = min->y;
    points[4].z = max->z;
    
    points[5].x = max->x;
    points[5].y = min->y;
    points[5].z = max->z;

    points[6].x = max->x;
    points[6].y = max->y;
    points[6].z = max->z;

    points[7].x = min->x;
    points[7].y = max->y;
    points[7].z = max->z;
}



HC_KEY HUtility::InsertBox (
    HPoint const	*max,
    HPoint const	*min){
    HPoint 	points[8];
    int		face_list[30];

	if (!min || !max)
	{
		HPoint p1(-1,-1,-1), p2(1,1,1);
		GenerateCuboidPoints(&p1, &p2, points);
	}
	else
		GenerateCuboidPoints(max, min, points);

    face_list[0] = 4;
    face_list[1] = 0;
    face_list[2] = 1;
    face_list[3] = 2;
    face_list[4] = 3;
    
    face_list[5] = 4;
    face_list[6] = 7;
    face_list[7] = 6;
    face_list[8] = 5;
    face_list[9] = 4;
    
    face_list[10] = 4;
    face_list[11] = 1;
    face_list[12] = 5;
    face_list[13] = 6;
    face_list[14] = 2;
    
    face_list[15] = 4;
    face_list[16] = 4;
    face_list[17] = 0;
    face_list[18] = 3;
    face_list[19] = 7;
    
    face_list[20] = 4;
    face_list[21] = 3;
    face_list[22] = 2;
    face_list[23] = 6;
    face_list[24] = 7;
    
    face_list[25] = 4;
    face_list[26] = 4;
    face_list[27] = 5;
    face_list[28] = 1;
    face_list[29] = 0;

    return HC_KInsert_Shell (8, points, 30, face_list);
}

HC_KEY HUtility::InsertWireframeBox (
    HPoint	*max,
    HPoint	*min){
    HPoint 	points[8];
    HPoint 	pline_points[16];

	if (!min || !max)
	{
		HPoint p1(-1,-1,-1), p2(1,1,1);
		GenerateCuboidPoints(&p1, &p2, points);
	}
	else
		GenerateCuboidPoints(max, min, points);

    pline_points[0] = points[3];
	pline_points[1] = points[0];
	pline_points[2] = points[4];
	pline_points[3] = points[7];
	pline_points[4] = points[6];
	pline_points[5] = points[5];
	pline_points[6] = points[1];
	pline_points[7] = points[2];
	pline_points[8] = points[3];
	pline_points[9] = points[7];
	pline_points[10] = points[4];
	pline_points[11] = points[5];
	pline_points[12] = points[6];
	pline_points[13] = points[2];
	pline_points[14] = points[1];
	pline_points[15] = points[0];

    return HC_KInsert_Polyline(16, pline_points);
}



bool HUtility::CreateAxisCircle (HPoint const & center,float radius,int axis, bool convert)
{
	HPoint points[3];
	bool success;

	// create three points on the circle in the appropriate view plane

	success = HUtilityGeometryCreation::GeneratePointsOnCircle(points, center, radius, axis, 3, convert);
	if (!success)
		return success;

	HC_Insert_Circle(&points[0], &points[1], &points[2]);
	return true;

}



void HUtility::NormalizeDoublePrecisionVector(double* vector1)
{
	double length;
	length = sqrt(vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]);
	vector1[0]/=length;
	vector1[1]/=length;
	vector1[2]/=length;
}



double HUtility::ComputeDoublePrecisionVectorLength(double * vector1)
{
	return sqrt(vector1[0]*vector1[0]+vector1[1]*vector1[1]+vector1[2]*vector1[2]);
}



void HUtility::ComputeDoublePrecisionCrossProduct(double * vector1, double * vector2, double * up)
{
	up[0] = vector1[1] * vector2[2]  -  vector1[2] * vector2[1];
    up[1] = vector1[2] * vector2[0]  -  vector1[0] * vector2[2];
    up[2] = vector1[0] * vector2[1]  -  vector1[1] * vector2[0];
}



double HUtility::ComputeDoublePrecisionDotProduct(double * vector1, double * vector2)
{
	return vector1[0] * vector2[0]
		 + vector1[1] * vector2[1]
		 + vector1[2] * vector2[2];
}


// 

void HUtility::GetViewplaneVectors(HC_KEY seg_key, double * view, double * up)
{
	HPoint	position, target;
	HVector	up_vector, view_vector, orthogonal;
	float	x, y, z;
	double	ortho[3];

	HC_Open_Segment_By_Key(seg_key);

		HC_Show_Net_Camera_Up_Vector(&x, &y, &z);
		up_vector.Set(x, y, z);

		HC_Show_Net_Camera_Position (&x, &y, &z);
		position.Set(x, y, z);

		HC_Show_Net_Camera_Target (&x, &y, &z);
		target.Set(x, y, z);

	HC_Close_Segment();

	view_vector.Set(position.x - target.x,
					position.y - target.y,
					position.z - target.z);

	// find vector in plane orthogonal to view and up plane
	HC_Compute_Cross_Product(&view_vector, &up_vector, &orthogonal);

	// now do the casting to double
	view[0] = (double) position.x - (double) target.x;
	view[1] = (double) position.y - (double) target.y;
	view[2] = (double) position.z - (double) target.z;

	NormalizeDoublePrecisionVector(view);

	ortho[0] = (double) orthogonal.x;
	ortho[1] = (double) orthogonal.y;
	ortho[2] = (double) orthogonal.z;

	NormalizeDoublePrecisionVector(ortho);

	ComputeDoublePrecisionCrossProduct(view, ortho, up);
}



void HUtility::InsertRotatedArrow(float stick_length,float tip_length,  float stick_radius, float tip_radius, float distance,  int adelta, float rotang,  const char *stick_color, const char *tip_color, int num_sides, float tip_falloff, bool reverse)
{
	UNREFERENCED(stick_length);
	UNREFERENCED(tip_falloff);
	UNREFERENCED(reverse);

   	HPoint *stick_points = 0;
  	HPoint *tip_points = 0;
	HPoint top;
	int *stick_faces_list = 0;
	int *tip_faces_list = 0;

	top.Set(distance,0,0);

	stick_points= new HPoint[num_sides*adelta];				
	tip_points= new HPoint[num_sides+1];				
  	stick_faces_list= new int[num_sides*adelta*5];
  	tip_faces_list= new int[num_sides*adelta];
 
	HUtilityGeometryCreation::GeneratePointsOnCircle(stick_points, top, stick_radius, HUtility::XZ, num_sides,false);
 	 
	float rot=0;
	float matrix[16];
	int ppos = num_sides;
	int i;
	for (i=0;i<adelta-1;i++)
	{
		HC_Compute_Rotation_Matrix(0,0,rot, matrix);
		HC_Compute_Transformed_Points(num_sides,stick_points,matrix,&stick_points[ppos]);
		ppos+=num_sides;
		rot+=rotang;
	}

	HUtilityGeometryCreation::GeneratePointsOnCircle(tip_points, top, tip_radius, HUtility::XZ, num_sides,false);
	HPoint top2;
	top2.Set(top.x,tip_length,0);
 	tip_points[num_sides] = top2;

	HC_Compute_Rotation_Matrix(0,0,rot, matrix);
	HC_Compute_Transformed_Points(num_sides+1, tip_points,matrix,tip_points);
   
  
	int adder=0;
	for (i=0;i<adelta-1;i++)
	{

	for (int j=0;j<num_sides;j++)
	{
		
		 int base = i*num_sides+j;
		 stick_faces_list[adder]=4;
		 stick_faces_list[adder+1]=base;
		 if (j==num_sides-1)
		 {
			 stick_faces_list[adder+2]=i*num_sides;
			 stick_faces_list[adder+3]=i*num_sides+num_sides;
		 }
		 else
		 {
			stick_faces_list[adder+2]=base+1;
			stick_faces_list[adder+3]=base+num_sides+1;
		 }
		 stick_faces_list[adder+4]=base+num_sides;
 		 adder+=5;
	}
	}

	int adder2 = 0;
 	for (int j=0;j<num_sides;j++)
	{
		
		 int base = j;
		 tip_faces_list[adder2]=3;
		 tip_faces_list[adder2+1]=base;
		 if (j==num_sides-1)
		 {
			 tip_faces_list[adder2+2]=0;
			 tip_faces_list[adder2+3]=num_sides;
		 }
		 else
		 {
			tip_faces_list[adder2+2]=base+1;
			tip_faces_list[adder2+3]=num_sides;
		 }
  		 adder2+=4;
	}

 	HC_Set_Color(stick_color);

	HC_Insert_Shell(ppos, stick_points,adder,stick_faces_list);
 	HC_Open_Segment("tip");
	HC_Set_Color(tip_color);
	HC_Insert_Shell(num_sides+1, tip_points,adder2,tip_faces_list);
	HC_Close_Segment();

	delete [] stick_points;				
	delete [] tip_points;				
	delete [] stick_faces_list;
  	delete [] tip_faces_list;

}
 





void HUtility::InsertArrow(float stick_length,float tip_length, float stick_radius, float tip_radius, const char *text, const char *stick_color, const char *tip_color, int num_sides, float tip_falloff, bool reverse)
{
  	HPoint	bottom, top;	

  	HPoint *stick_points = 0;
  	HPoint *tip_points = 0;
	int *stick_faces_list = 0;
	int *tip_faces_list = 0;
	int i;

	stick_points= new HPoint[num_sides*2];				
	tip_points= new HPoint[num_sides+1];
  	stick_faces_list= new int[num_sides*5+num_sides+1];
  	tip_faces_list= new int[num_sides*4];

	bottom.Set(0.0,tip_length,0.0);
	top.Set(0.0,stick_length + tip_length,0.0);
	
	tip_points[num_sides].Set(0.0,0.0,0.0);

//	tip_points[num_sides].Set(0.0,top.y+tip_length,0.0);
  
	HUtilityGeometryCreation::GeneratePointsOnCircle(stick_points, top, stick_radius-tip_falloff, HUtility::XZ, num_sides,false);
	HUtilityGeometryCreation::GeneratePointsOnCircle(stick_points+num_sides, bottom, stick_radius, HUtility::XZ, num_sides,false);
	HUtilityGeometryCreation::GeneratePointsOnCircle(tip_points, bottom, tip_radius, HUtility::XZ, num_sides,false);
  
	for (i=0;i<num_sides;i++)
	{
		 stick_faces_list[i*5]=4;
		 stick_faces_list[i*5+1]=i;
		 stick_faces_list[i*5+2]=i+num_sides;
		 stick_faces_list[i*5+3]=((i+1)%num_sides)+num_sides;
		 stick_faces_list[i*5+4]=(i+1)%num_sides;
		 
		 tip_faces_list[i*4]=3;
		 tip_faces_list[i*4+1]=i;
		 tip_faces_list[i*4+2]=(i+1)%num_sides;
		 tip_faces_list[i*4+3]=num_sides;

		 stick_faces_list[num_sides*5+i+1]=i;		//this constructs the face for the bottom of
													//the cylinder (not really needed for the axis)
	}
	stick_faces_list[num_sides*5]=num_sides;

 	HC_Open_Segment("inner"); 
	if (reverse)
	{
		HC_Rotate_Object(180,0,0);
		HC_Translate_Object(0,stick_length + tip_length,0);
	}
	if (stick_color)
	{
	    HC_Open_Segment("");
		HC_Set_Color(stick_color);
		HC_Insert_Shell(num_sides*2,stick_points,num_sides*5+num_sides+1,stick_faces_list);
	    HC_Close_Segment();

	}else
	    HC_Insert_Shell(num_sides*2,stick_points,num_sides*5+num_sides+1,stick_faces_list);

		if (tip_color)
	{
	    HC_Open_Segment("");	
		HC_Set_Color(tip_color);
 		HC_Insert_Shell(num_sides+1,tip_points,num_sides*4,tip_faces_list);
 	    HC_Close_Segment();
	}else
	    HC_Insert_Shell(num_sides+1,tip_points,num_sides*4,tip_faces_list);
	HC_Close_Segment();

	if (text)	
	{
 	    HC_Set_Text_Font("name=stroked,size=12px");
	    HC_Insert_Text(0,top.y+0.1,0.0,text);
	    HC_Set_Color("text = white");
	}

	if (stick_points)
		delete []stick_points;
	if (tip_points)
		delete []tip_points;
	if (stick_faces_list)
		delete []stick_faces_list;
	if (tip_faces_list)
		delete []tip_faces_list;
}



void HUtility::RotateToVector(HPoint start, HPoint end, bool doTranslation)
//void InsertArrow(HPoint start, HPoint end, float scale)
{
		HPoint gl,gl2;
		HPoint a,b;
		gl.x = start.x-end.x;
		gl.y = start.y-end.y;
		gl.z = start.z-end.z;
//		float vlen=(float)HC_Compute_Vector_Length(&gl);
		gl2.x  = gl.x;
		gl2.y  = 0;
		gl2.z  = gl.z;
		a.x=gl.x; a.y= gl.y; a.z= gl.z;
		b.x=gl2.x; b.y= gl2.y; b.z= gl2.z;
		
		float degreeangley;
		float srtemp = (float)sqrt((a.x*a.x+a.y*a.y+a.z*a.z)*(b.x*b.x+b.y*b.y+b.z*b.z));
		if (!srtemp)
			srtemp = 0.0000001f;
		float multemp = (a.x*b.x + a.y*b.y + a.z*b.z) / srtemp;
		if (multemp>=1.0f)
			multemp = 0.99999f;
		degreeangley = (float)H_ACOS(multemp) * (180.0f/3.1415927f);
		gl.x = start.x-end.x;
		gl.y = 0;
		gl.z = start.z-end.z;
		
		gl2.x  = gl.x;
		gl2.y  = 0;
		gl2.z  = 0;
		a.x=gl.x; a.y= gl.y; a.z= gl.z;
		b.x=gl2.x; b.y= gl2.y; b.z= gl2.z;
		srtemp = (float)sqrt((a.x*a.x+a.y*a.y+a.z*a.z)*(b.x*b.x+b.y*b.y+b.z*b.z));
		if (!srtemp)
			srtemp = 0.0000001f;
		multemp = (a.x*b.x + a.y*b.y + a.z*b.z) / srtemp;
		if (multemp>=1.0f)
			multemp = 0.99999f;
		
		float degreeanglez = (float)H_ACOS(multemp) * (180.0f/3.1415927f);
		
		if (end.x<start.x && end.z<=start.z)	
			;
		else if (end.x<start.x && end.z>start.z)	
			degreeanglez=-degreeanglez;
		else if (end.x>start.x && end.z>start.z)	
			degreeanglez=-(180-degreeanglez);
		else 	
			degreeanglez=(180-degreeanglez);
		
		if (end.y>start.y)
			degreeangley=-degreeangley;
		
		//					HC_Translate_Object(0.0,-vlen/2.0,0.0);
		HC_Rotate_Object(90.0,0.0,0.0);
		HC_Rotate_Object(-degreeangley,0,0);
		HC_Rotate_Object(0.0,-(-(90-degreeanglez)),0);				
		
		if (doTranslation)
			HC_Translate_Object(start.x,start.y,start.z);
		
		
}

#if 0
void HUtility::RotateToVector(HPoint start, HPoint end, bool doTranslation)
{
	
	HPoint gl,gl2;
	HPoint a,b;
	float degreeangley, degreeanglez;
	gl.Set(start.x-end.x, start.y-end.y,start.z-end.z);
 	//float vlen=(float)HC_Compute_Vector_Length(&gl);
	gl2.Set(gl.x,0,gl.z);
	a.Set(gl.x,gl.y,gl.z);
	b.Set(gl.x,0,gl.z);


	float a_squared = a.x*a.x+a.y*a.y+a.z*a.z;
	float b_squared = b.x*b.x+b.y*b.y+b.z*b.z;

	if (a_squared == 0)
		a_squared =1;

	if (b_squared == 0)
		b_squared =1;


 	if (start.x == end.x && start.z == end.z && start.z == end.x && start.x == end.z)
		degreeangley = 90;
	else
		degreeangley = (float)H_ACOS((a.x*b.x + a.y*b.y + a.z*b.z) / sqrt(a_squared*b_squared))* (180.0f/3.1415927f);
  	a.Set(start.x-end.x,0,start.z-end.z);
	b.Set(start.x-end.x,0,0);
 
	if (start.x == end.x && start.y == end.y && start.y == end.x && start.y == end.x)
		degreeanglez = 90;
	else
		degreeanglez = (float)H_ACOS((a.x*b.x + a.y*b.y + a.z*b.z) / sqrt(a_squared*b_squared)) * (180.0f/3.1415927f);
	
	if (end.x<=start.x && end.z<=start.z)	
		;
	else if (end.x<=start.x && end.z>start.z)	
		degreeanglez=-degreeanglez;
	else if (end.x>start.x && end.z>start.z)	
		degreeanglez=-(180-degreeanglez);
	else 	
		degreeanglez=(180-degreeanglez);	
	if (end.y>start.y)
		degreeangley=-degreeangley;		 
	HC_Rotate_Object(-90.0,0.0,0.0);
	HC_Rotate_Object(-degreeangley,0,0);
	HC_Rotate_Object(0.0,-(-(90-degreeanglez)),0);				
	if (doTranslation)
		HC_Translate_Object(start.x,start.y,start.z);
	
}
#endif
/*
  {axis.x, axis.y, axis.z, 0, 
  ref.x, ref.y, ref.z, 0,
  side.x, side.y, side.z, 0,
  pos.x, pos.y, pos.z,1};

where:
  axis:  viewing vector (i.e. the camera target -camera position vector) 
  ref:    camera up vector
  side: the cross product of the axis and ref (i.e. orthogonal to the axis and ref vectors) 
  pos:  camera target


*/

void HUtility::GetCameraFromMatrix(float *matrix, HPoint *target, HPoint *position, HPoint *up_vector, HPoint *translation)
{
	UNREFERENCED(target);

//Target remains unchanged
//    HPoint view_vector;
//    view_vector.Set(position.x-target.x, position.y-target.y, position.z-target.z);

    position->x = matrix[0];
    position->y = matrix[1];
    position->z = matrix[2];

    up_vector->x = matrix[4];
    up_vector->y = matrix[5];
    up_vector->z = matrix[6];

    translation->x = matrix[12];
    translation->y = matrix[13];
    translation->z = matrix[14];
}

#if 0
static void transpose(float *in, float *out)
{
    float temp[16];

    temp[0] = in[0];
    temp[1] = in[4];
    temp[2] = in[8];
    temp[3] = in[12];
    temp[4] = in[1];
    temp[5] = in[5];
    temp[6] = in[9];
    temp[7] = in[13];
    temp[8] = in[2];
    temp[9] = in[6];
    temp[10] = in[10];
    temp[11] = in[14];
    temp[12] = in[3];
    temp[13] = in[7];
    temp[14] = in[11];
    temp[15] = in[15];

    memcpy(out, temp, 16*sizeof(float));

    return;
}
#endif



void HUtility::GetMatrixFromCamera(HPoint target, HPoint position, HPoint up_vector, HPoint translation, float *matrix)
{
    HPoint view_vector;
    view_vector.Set(target.x-position.x, target.y-position.y, target.z-position.z);

    HC_Compute_Normalized_Vector(&view_vector, &view_vector);
    HC_Compute_Normalized_Vector(&up_vector, &up_vector);

    HPoint cross1;

    HC_Compute_Cross_Product(&up_vector, &view_vector, &cross1);

    matrix[0] = cross1.x;
    matrix[1] = cross1.y;
    matrix[2] = cross1.z;
    matrix[3] = 0.0f;

    HPoint cross2;

    HC_Compute_Cross_Product(&view_vector, &cross1, &cross2);

    matrix[4] = cross2.x;
    matrix[5] = cross2.y;
    matrix[6] = cross2.z;
    matrix[7] = 0.0f;

    matrix[8] = view_vector.x;
    matrix[9] = view_vector.y;
    matrix[10] = view_vector.z;
    matrix[11] = 0.0f;
    
    matrix[12] = translation.x;
    matrix[13] = translation.y;
    matrix[14] = translation.z;
    matrix[15] = 1.0f;
}


#ifndef PI
#define PI 3.14159265358979
#endif

void HUtility::GetCameraFromGLFrustum(double const left, double const right, double const bottom, double const top, double const near_z, double const far_z, bool const perspective,
							   HPoint &pos, HPoint &target, HPoint &up, char * projection, double &width, double &height, double &near_limit)
{
	// initially set up the HOOPS camera position and target X values such it's looking perpendicular to the near plane
	pos.x = target.x = (float)(0.5 * (left + right));
	pos.y = target.y = (float)(0.5 * (bottom + top));

	// set the up vector to be pointing upwards
	up.x = 0.0; 
	up.y = 1.0;
	up.z = 0.0;

	// set up the HOOPS camera field
	width = right - left;
	height = top - bottom;

	target.z = (float)(0.5 * (near_z + far_z));

	if (perspective)
	{
		// set the z component of the HOOPS camera position to be at the origin, since an OGL frustrum (camera) always has it's z position at the origin
		pos.z = 0;

		near_limit = near_z/target.z;

		width/=near_limit;
		height/=near_limit;

		if (target.x == 0 && target.y == 0)
			strcpy(projection, "perspective");
		else
		{
			// it's an oblique perspective projection
			float angle_x, angle_y;

			angle_x = atan2(target.x, target.z) * 180/PI;
			angle_y = -atan2(target.y, target.z) * 180/PI;
			HCLOCALE (sprintf(projection, "oblique perspective = (%f, %f)", angle_x, angle_y));
		}
	}
	else
	{
		// use the eye-distance directly for the clipping distance in orthographic
		near_limit = 1.0;
		pos.z = (float)near_z;
		strcpy(projection, "orthographic");
	}
}

void HUtility::SmoothTransition(HCamera old_camera, const HCamera &new_camera, HBaseView *view)
{
	strcpy(old_camera.projection, new_camera.projection);
	view->SetCamera(old_camera);
	HUtility::SmoothTransition(
		old_camera.position, old_camera.target, old_camera.up_vector, old_camera.field_width, old_camera.field_height,
		new_camera.position, new_camera.target, new_camera.up_vector, new_camera.field_width, new_camera.field_height, view);
	view->SetCamera(new_camera);
}

bool HUtility::CamerasEqual(const HCamera &c1, const HCamera &c2)
{
	return c1.position.Equal(c2.position)
		&& c1.target.Equal(c2.target)
		&& c1.up_vector.Equal(c2.up_vector)
		&& c1.field_width == c2.field_width
		&& c1.field_height == c2.field_height
		&& strcmp(c1.projection, c2.projection) == 0
		;
}


#define PA(t,m,a) ((t)*(t) * 0.5f * (m) / (a))
#define PB(t,m,a) (PA((a),(m),(a)) + ((t) - (a)) * (m))
#define PC(t,m,a,b,c) (PB((a)+(b),(m),(a)) + ((t) - ((a)+(b))) * (m) * (1.0f - 0.5f * ((t) - ((a)+(b))) / (c)))


float HUtility::EaseInEaseOut(float t, float a, float b, float c)
{

    float res;
    float m = 2.0f / (a + 2.0f * b + c);

    if (t<=a)
	res = PA(t,m,a);
    else if (t>=a && t<=(a+b))
	res = PB(t,m,a);    
    else 
    	res = PC(t,m,a,b,c);
    if (res>1.0f)
	res = 1.0f;
    return res;
	
}



void HUtility::SmoothTransition(
	const HPoint &co, const HPoint &to, const HPoint &uo, float fieldwidtho, float fieldheighto, 
	const HPoint &cn, const HPoint &tn, const HPoint &un, float fieldwidthn, float fieldheightn, 
	HBaseView *view)
{ 
	if (view->GetRenderMode()==HRenderHiddenLine)
	{
 	    HC_Set_Camera_Target(tn.x,tn.y,tn.z);
 	    HC_Set_Camera_Position(cn.x,cn.y,cn.z);
 	    HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
		HC_Set_Camera_Field(fieldwidthn,fieldheightn);

 	    view->CameraPositionChanged();
	    view->Update();
	    return;
	}	

	HCamera old, nua;
	old.position = co;
	old.target = to;
	old.up_vector = uo;
	old.field_height = fieldheighto;
	old.field_width = fieldwidtho;
	sprintf(old.projection,"p");

	nua.position = cn;
	nua.target = tn;
	nua.up_vector = un;
	nua.field_height = fieldheightn;
	nua.field_width = fieldwidthn;
	sprintf(nua.projection,"p");

	// don't bother doing a smooth transition if there
	// is no difference in the new and old camera
	if (CamerasEqual(old, nua))
	{
	    view->Update();
	    return;
	}
	
	 
	float matrix_old[16],  matrix_new[16];
	HPoint view_vector_old, view_vector_new;

	view_vector_old.Set(to.x - co.x, to.y - co.y, to.z - co.z);
	view_vector_new.Set(tn.x - cn.x, tn.y - cn.y, tn.z - cn.z);
	float length = (float)HC_Compute_Vector_Length(&view_vector_old);


	float length_delta = (float)HC_Compute_Vector_Length(&view_vector_new) - (float)HC_Compute_Vector_Length(&view_vector_old);
  
	HC_Compute_Normalized_Vector(&view_vector_old,&view_vector_old);
	HC_Compute_Normalized_Vector(&view_vector_new,&view_vector_new);

	bool need_rotation=true;
	HPoint rotation_delta;

	rotation_delta.Set(view_vector_old.x - view_vector_new.x,
	    view_vector_old.y - view_vector_new.y,
	    view_vector_old.z - view_vector_new.z);


	HPoint up_delta;
	up_delta.Set(un.x - uo.x, un.y - uo.y, un.z - uo.z);

	if ((float)HC_Compute_Vector_Length(&rotation_delta) < 0.0001f && (float)HC_Compute_Vector_Length(&up_delta) < 0.0001f )
		need_rotation=false;
 
	HPoint translation;
	translation.Set(0.0f,0.0f,0.0f);

	HPoint position_move_delta;

	if (need_rotation)
	{
	    HUtility::GetMatrixFromCamera(to, co, uo, translation, matrix_old);
	    HUtility::GetMatrixFromCamera(tn, cn, un, translation, matrix_new);
	}
	else
	{
	    position_move_delta.Set(cn.x-co.x, cn.y-co.y, cn.z-co.z);
	}


	float	matrix_interim[16];
	float	quat_old[4], quat_new[4], quat_interim[4];

	if (need_rotation)
	{
	    HUtility::MatrixToQuaternion(matrix_old, quat_old);
	    HUtility::MatrixToQuaternion(matrix_new, quat_new);
	}

	HPoint target_move_delta;
	target_move_delta.Set(tn.x-to.x, tn.y-to.y, tn.z-to.z);

	float field_width_delta = fieldwidthn - fieldwidtho;
	float field_height_delta = fieldheightn - fieldheighto;

	float dot1, dot2;

	if (need_rotation)
	{
	    // Simple dot product to determine which direction the quaternion should 
	    // go if we want to take the shortest path.
	    dot1 = ((quat_old[0] - quat_new[0])*(quat_old[0] - quat_new[0])
			+ (quat_old[1] - quat_new[1])*(quat_old[1] - quat_new[1])
			+ (quat_old[2] - quat_new[2])*(quat_old[2] - quat_new[2])
			+ (quat_old[3] - quat_new[3])*(quat_old[3] - quat_new[3])); 

	    dot2 = ((quat_old[0] + quat_new[0])*(quat_old[0] + quat_new[0])
			+ (quat_old[1] + quat_new[1])*(quat_old[1] + quat_new[1])
			+ (quat_old[2] + quat_new[2])*(quat_old[2] + quat_new[2])
			+ (quat_old[3] + quat_new[3])*(quat_old[3] + quat_new[3])); 

	    if (dot2 < dot1) {
		    quat_new[0] = -quat_new[0];
		    quat_new[1] = -quat_new[1];
		    quat_new[2] = -quat_new[2];
		    quat_new[3] = -quat_new[3];
	    }
	}

	HPoint position, up_vector;
 
	float total_duration = view->GetSmoothTransitionDuration();							
//	float initial_delta = (float) 0.05 / total_duration;			//0.05 = 20fps	(guessing)
	float q,total_time_start, current_time;
	
	// 开始时间
	HC_Show_Time(&total_time_start);
	total_time_start-=0.1f;
	HC_Open_Segment_By_Key(view->GetSceneKey());
	
	do
	{
	    // 当前时间
	    HC_Show_Time(&current_time);		
		// 当前时间-开始时间 / 持续时间
	    q =  ((current_time - total_time_start) / total_duration);	    	    
	    if (q>=1.0)		
		q = 1.0;	    	    	    
	    q = EaseInEaseOut(q, 0.49f, 0.02f, 0.49f);		   
	    if (q<0)
		q=0; 
	 
		// 是否需要旋转
 	    if (need_rotation)
	    {
		HPoint to2;
		to2.Set(to.x + (q*target_move_delta.x),
		    to.y + (q*target_move_delta.y),
		    to.z + (q*target_move_delta.z));
		// first let use quaternions to take care of the interpolation 
		// of the rotation element of the camera movement
		// 四元数转换
		HUtility::TransitionQuaternion(quat_old, quat_new, q, quat_interim);
		// 四元数转矩阵
		HUtility::QuaternionToMatrix(quat_interim, matrix_interim);
		
		
		position.x = to2.x - (matrix_interim[8] * (length+length_delta*q));
		position.y = to2.y - (matrix_interim[9] *  (length+length_delta*q));
		position.z = to2.z - (matrix_interim[10] * (length+length_delta*q));
		
		
		up_vector.x = matrix_interim[4];
		up_vector.y = matrix_interim[5];
		up_vector.z = matrix_interim[6];
		
		HC_Set_Camera_Position(position.x, position.y, position.z);
		HC_Set_Camera_Up_Vector(up_vector.x, up_vector.y, up_vector.z);
	    }
	    else
	    {
		HC_Set_Camera_Position(co.x + (q*position_move_delta.x), 
		    co.y + (q*position_move_delta.y), 
		    co.z + (q*position_move_delta.z));
	    }
	    
	    // now linearly interpolate the camera target and field changes
	    HC_Set_Camera_Target(to.x + (q*target_move_delta.x), 
		to.y + (q*target_move_delta.y), 
		to.z + (q*target_move_delta.z));
	    
		float fw = fieldwidtho+(q*field_width_delta);
		float fh = fieldheighto+(q*field_height_delta);
		if (fw > 0.00001f && fh > 0.00001f)
		{
  	     HC_Set_Camera_Field(fw,fh);
	    q =  ((current_time - total_time_start) / total_duration);
	    
	    view->CameraPositionChanged();
	    if (view->GetConstantFrameRateObject())
		view->GetConstantFrameRateObject()->SetActivityType(GeneralActivity);
		
	    view->ForceUpdate();
		}
	}while (q < 1.0 - EPSILON);

	HC_Set_Camera_Target(tn.x,tn.y,tn.z);
    HC_Set_Camera_Position(cn.x,cn.y,cn.z);
    HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
	HC_Set_Camera_Field(fieldwidthn,fieldheightn);


    HC_Close_Segment();
 	HLISTENER_EVENT(HUpdateListener, view->GetEventManager(), SmoothTransitionFinishedEvent(view));
	
 	
 
	view->Update();
}






#if 0
void HUtility::SmoothTransition(HPoint &co,HPoint &to, HPoint &uo, HPoint &cn, HPoint &tn, HPoint &un, HBaseView *m_pHView)
{
	double total_duration = 1.0;
	double framerate = 0.01f;
	double distance = 0.0;
	double total_distance = 0.0;
	double duration = 0.0;

	if (!m_pHView->GetSmoothTransition() || m_pHView->GetRenderMode()==HRenderHiddenLine)
	{

 		HC_Set_Camera_Target(tn.x,tn.y,tn.z);
 		HC_Set_Camera_Position(cn.x,cn.y,cn.z);
 		HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
 		HC_Move_Distant_Light(m_pHView->GetDefaultLightKey(),
                              cn.x,cn.y,cn.z);
 
		m_pHView->Update();
		return;
	}

	// smooth transition modifies the location of the camera rather than simply changing the camera field.
	// consequently it does not have any effect if we're in perspective mode.  to workaround this we change
	// to perspective mode temporaily as we do the smooth transition
	bool orthographicmode = false;

	if (m_pHView->GetProjMode() != ProjPerspective)
	{
		m_pHView->SetProjMode(ProjPerspective);
		orthographicmode = true;
	}
	
	HPoint vt,vc,vu;
	vt.Set(tn.x-to.x,tn.y-to.y,tn.z-to.z);
	vc.Set(cn.x-co.x,cn.y-co.y,cn.z-co.z);
	vu.Set(un.x-uo.x,un.y-uo.y,un.z-uo.z);

	HConstantFrameRate *cframerate = m_pHView->GetConstantFrameRateObject();
 	float total_time_start;
	bool cfr_active = cframerate->GetActive();
	if (!cfr_active)
		cframerate->Start();

	HC_Show_Time(&total_time_start);
	while (1)
	{
		distance = (1.0-distance)/(total_duration-duration)*framerate;
		if (distance<0)
			distance=0.1;
		total_distance +=distance;
		if (total_distance>1.0)
			total_distance = 1.0;
 		HC_Set_Camera_Target(to.x+vt.x*total_distance,to.y+vt.y*total_distance,to.z+vt.z*total_distance);
		HC_Set_Camera_Position(co.x+vc.x*total_distance,co.y+vc.y*total_distance,co.z+vc.z*total_distance);
		HC_Set_Camera_Up_Vector(uo.x+vu.x*total_distance,uo.y+vu.y*total_distance,uo.z+vu.z*total_distance);

		m_pHView->CameraPositionChanged();

 		float update_time_start,update_time_end;
		HC_Show_Time(&update_time_start);
		m_pHView->Update();	
		HC_Show_Time(&update_time_end);
		framerate = update_time_end-update_time_start;
		if (framerate<0.01)
		{
			float pausetime_start;
			HC_Show_Time(&pausetime_start);
			while (1)
			{
				float pausetime_end;
				HC_Show_Time(&pausetime_end);
				if (pausetime_end-pausetime_start>(0.01-framerate))
					break;
			}
 			framerate = 0.01f;
			HC_Show_Time(&update_time_end);

		}
		duration = update_time_end - total_time_start;
//saftey measure if infinite loop problem occurs (should never happen!!!)
		if (duration>total_duration+total_duration/4)
		{
			HC_Set_Camera_Target(tn.x,tn.y,tn.z);
			HC_Set_Camera_Position(cn.x,cn.y,cn.z);
			HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
 			//HC_Move_Distant_Light(m_pHView->GetDefaultLightKey(),
	          //                    cn.x,cn.y,cn.z);
			m_pHView->CameraPositionChanged();
			m_pHView->Update();
			break;
		}


		if (total_distance>=1.0)
			break;

	}
	if (!cfr_active)
		cframerate->Stop();

	if (orthographicmode)
	{
		m_pHView->SetProjMode(ProjOrthographic);
		m_pHView->Update();
	}
}
#endif 

void HUtility::ComputeVectorToVectorRotationMatrix(float *matrix,HPoint p1,HPoint p2)
{
	HPoint p3;

	HC_Compute_Normalized_Vector(&p1,&p1);
	HC_Compute_Normalized_Vector(&p2,&p2);
	HC_Compute_Cross_Product(&p1,&p2,&p3);

	float dprod = (float)HC_Compute_Dot_Product(&p1,&p2);
	float l=(float)HC_Compute_Vector_Length(&p3);

	// Construct a perpendicular vector for anti-parallel case
	if (l > -EPSILON && l < EPSILON)
	{
		if (dprod < 0) {
			if (p1.x < -EPSILON || p1.x > EPSILON) {
				p3.x = p1.y;
				p3.y = -p1.x;
				p3.z = 0;
			} else if (p1.y < -EPSILON || p1.y > EPSILON) {
				p3.x = -p1.y;
				p3.y = p1.x;
				p3.z = 0;
			} else if (p1.z < -EPSILON || p1.z > EPSILON) {
				p3.x = -p1.z;
				p3.z = p1.x;
				p3.y = 0;
			} else {
				HC_Compute_Identity_Matrix(matrix);
				return;
			}
		} else {
			HC_Compute_Identity_Matrix(matrix);
			return;
		}
	}

	float ang = (float)atan2(l,dprod);
	ang*=(180.0f/(float)PI);

	HC_Compute_Offaxis_Rotation(p3.x,p3.y,p3.z,ang, matrix);
}
    	
bool HUtility::ExtractRGB(char *text,float &r, float &g, float &b)
{
	int l= (int) strlen(text);
	for (int i=0;i<l;i++) { text[i]=(char)toupper((int)text[i]); }
	char *rpos = strstr(text,"R=");
	if (!rpos)
		return false;
	HCLOCALE(r=(float)atof(rpos+2));
	rpos = strstr(text,"G=");
	HCLOCALE(g=(float)atof(rpos+2));
	rpos = strstr(text,"B=");
	HCLOCALE(b=(float)atof(rpos+2));
	return true;
}

#define X 0
#define Y 1
#define Z 2
#define W 3

void HUtility::QuaternionToMatrix(float * quaternion, float *matrix)
{	
	double s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

	s = 2.0/(quaternion[X]*quaternion[X] + quaternion[Y]*quaternion[Y] + quaternion[Z]*quaternion[Z] + quaternion[W]*quaternion[W]);

	xs = quaternion[X]*s;	ys = quaternion[Y]*s;		zs = quaternion[Z]*s;
	wx = quaternion[W]*xs;	wy = quaternion[W]*ys;		wz = quaternion[W]*zs;
	xx = quaternion[X]*xs;	xy = quaternion[X]*ys;		xz = quaternion[X]*zs;
	yy = quaternion[Y]*ys;	yz = quaternion[Y]*zs;		zz = quaternion[Z]*zs;

	matrix[0] = 1.0f - (float)(yy+zz);
	matrix[1] = (float)xy + (float)wz;
	matrix[2] = (float)xz - (float)wy;
	matrix[3] = 0.0f;

	matrix[4] = (float)xy - (float)wz;
	matrix[5] = 1.0f - (float)(xx + zz);
	matrix[6] = (float)yz + (float)wx;
	matrix[7] = 0.0f;

	matrix[8] = (float)xz + (float)wy;
	matrix[9] = (float)yz - (float)wx;
	matrix[10] = 1.0f - (float)(xx + yy);
	matrix[11] = 0.0f;

	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = 0.0f;
	matrix[15] = 1.0f;
}

void HUtility::MatrixToQuaternion(float *matrix, float *quaternion)
{
	double	tr, s;
	int		i, j, k;

	tr = matrix[0] + matrix[5] + matrix[10];
	if (tr > 0.0)
	{
		s = sqrt(tr + 1.0);
		quaternion[W] = (float)s * 0.5f;
		s = 0.5f/(float)s;

		quaternion[X] = (matrix[6] - matrix[9]) * (float)s;
		quaternion[Y] = (matrix[8] - matrix[2]) * (float)s;
		quaternion[Z] = (matrix[1] - matrix[4]) * (float)s;
	}
	else
	{
		i = X;
		if (matrix[(Y*4)+Y] > matrix[(X*4)+X])
			i = Y;
		if (matrix[(Z*4)+Z] > matrix[(i*4)+i])
			i = Z;

		int nxt[3] = {Y,Z,X};

		j = nxt[i];
		k = nxt[j];

		s = sqrt(matrix[(i*4)+i] - (matrix[(j*4)+j] + matrix[(k*4)+k]) + 1.0f);

		quaternion[i] = (float)s * 0.5f;
		s = 0.5f / s;

		quaternion[W] = (matrix[(j*4) + k] - matrix[(k*4) + j]) * (float)s;
		quaternion[j] = (matrix[(i*4) + j] + matrix[(j*4) + i]) * (float)s;
		quaternion[k] = (matrix[(i*4) + k] + matrix[(k*4) + i]) * (float)s;
	}
}

void HUtility::TransitionQuaternion(float *quat_old, float *quat_new, float t, float *qt)
{
	double omega, cosom, sinom, sclp, sclq;

	cosom = quat_old[X]*quat_new[X] + quat_old[Y]*quat_new[Y] + quat_old[Z]*quat_new[Z] + quat_old[W]*quat_new[W];

	if ((1.0 + cosom) > EPSILON)
	{
		if ((1.0 - cosom) > EPSILON)
		{
			omega = H_ACOS(cosom);
			sinom = sin(omega);
			sclp = sin((1.0 - t)*omega)/sinom;
			sclq = sin(t*omega)/sinom;
		}
		else
		{
			sclp = 1.0 - t;
			sclq = t;
		}

		for (int i=0; i<4; i++)
			qt[i] = (float)sclp*quat_old[i] +(float)sclq*quat_new[i];
	}
	else
	{
		qt[X] = -quat_old[Y];	qt[Y] = quat_old[X];
		qt[Z] = -quat_old[W];	qt[W] = quat_old[Z];
		sclp = sin((1.0 -t) * PI/2);
		sclq = sin(t * PI/2);
		for (int i=0; i<3; i++)
			qt[i] = (float)sclp*quat_old[i] + (float)sclq*qt[i];
	}
}




void HUtility::SmoothQuaternionTransition(HBaseView *m_pView, char *old_seg, char *new_seg, int num_of_transitions)
{
    float matrix_old[16], matrix_new[16];

    HC_Open_Segment(new_seg);
	HC_Show_Modelling_Matrix(matrix_new);
	matrix_new[12] = 0;
	matrix_new[13] = 0;
	matrix_new[14] = 0;
	matrix_new[15] = 1;
    HC_Close_Segment();

    // need to determine the scaling factor as the quaternions do not take scales into account
    float new_scale = (float) sqrt((matrix_new[0]*matrix_new[0])
				+(matrix_new[1]*matrix_new[1])
				+(matrix_new[2]*matrix_new[2]));

    // need to remove the scaling factor from the matrix before we turn it into a quaternion
    matrix_new[0] /= new_scale;
    matrix_new[1] /= new_scale;
    matrix_new[2] /= new_scale;
    matrix_new[4] /= new_scale;
    matrix_new[5] /= new_scale;
    matrix_new[6] /= new_scale;
    matrix_new[8] /= new_scale;
    matrix_new[9] /= new_scale;
    matrix_new[10] /= new_scale;

    // now get the old segments matrix.  translation has already been removed
    HC_Open_Segment(old_seg);
	HC_Show_Modelling_Matrix(matrix_old);
    HC_Close_Segment();

    // Again we need to determine and remove the scaling factor from the matrix
    float old_scale = (float) sqrt((matrix_old[0]*matrix_old[0])
				    +(matrix_old[1]*matrix_old[1])
				    +(matrix_old[2]*matrix_old[2]));

    matrix_old[0] /= old_scale;
    matrix_old[1] /= old_scale;
    matrix_old[2] /= old_scale;
    matrix_old[4] /= old_scale;
    matrix_old[5] /= old_scale;
    matrix_old[6] /= old_scale;
    matrix_old[8] /= old_scale;
    matrix_old[9] /= old_scale;
    matrix_old[10] /= old_scale;

    // now turn the matrices into quaternions
    float	matrix_interim[16];
    float	quat_old[4], quat_new[4], quat_interim[4];

    HUtility::MatrixToQuaternion(matrix_old, quat_old);
    HUtility::MatrixToQuaternion(matrix_new, quat_new);

    // Simple dot product to determine which direction the quaternion should 
    // go if we want to take the shortest path.
    float dot1 = ((quat_old[0] - quat_new[0])*(quat_old[0] - quat_new[0])
		    + (quat_old[1] - quat_new[1])*(quat_old[1] - quat_new[1])
		    + (quat_old[2] - quat_new[2])*(quat_old[2] - quat_new[2])
		    + (quat_old[3] - quat_new[3])*(quat_old[3] - quat_new[3])); 

    float dot2 = ((quat_old[0] + quat_new[0])*(quat_old[0] + quat_new[0])
		    + (quat_old[1] + quat_new[1])*(quat_old[1] + quat_new[1])
		    + (quat_old[2] + quat_new[2])*(quat_old[2] + quat_new[2])
		    + (quat_old[3] + quat_new[3])*(quat_old[3] + quat_new[3])); 

    if (dot2 < dot1) {
	    quat_new[0] = -quat_new[0];
	    quat_new[1] = -quat_new[1];
	    quat_new[2] = -quat_new[2];
	    quat_new[3] = -quat_new[3];
    }

    HC_Open_Segment(old_seg);
    // now loop throw the transition 20 times 
	for (int q=1; q<=num_of_transitions; q++)
	{
		HUtility::TransitionQuaternion(quat_old, quat_new, q/20.0f, quat_interim);
		HUtility::QuaternionToMatrix(quat_interim, matrix_interim);

		// linearly interpolate the scaling factor to the object
		float interim_scale = old_scale + (new_scale - old_scale)*q/20;

		matrix_interim[0] *= interim_scale;
		matrix_interim[1] *= interim_scale;
		matrix_interim[2] *= interim_scale;
		matrix_interim[4] *= interim_scale;
		matrix_interim[5] *= interim_scale;
		matrix_interim[6] *= interim_scale;
		matrix_interim[8] *= interim_scale;
		matrix_interim[9] *= interim_scale;
		matrix_interim[10] *= interim_scale;

		// now that we have a new matrix let's unset the existing matrix and 
		// set the new quaternion matrix on it.
		HC_UnSet_Modelling_Matrix();
		HC_Set_Modelling_Matrix(matrix_interim);
		m_pView->Update();
	}
    HC_Close_Segment();
}


void HUtility::SmoothMatrixTransition(HBaseView *m_pView, HC_KEY seg_key, float *new_matrix)
{
	HPoint tvect;
	HPoint cent_old, cent_new;
	float matrix_old[16], matrix_new[16], old_matrix[16];

	memcpy(matrix_new, new_matrix, 16*sizeof(float));

	// need to determine the scaling factor as the quaternions do not take scales into account
	float new_scale = (float) sqrt((matrix_new[0]*matrix_new[0])
				+(matrix_new[1]*matrix_new[1])
				+(matrix_new[2]*matrix_new[2]));
	
	// need to remove the scaling factor from the matrix before we turn it into a quaternion
	matrix_new[0] /= new_scale;
	matrix_new[1] /= new_scale;
	matrix_new[2] /= new_scale;
	matrix_new[4] /= new_scale;
	matrix_new[5] /= new_scale;
	matrix_new[6] /= new_scale;
	matrix_new[8] /= new_scale;
	matrix_new[9] /= new_scale;
	matrix_new[10] /= new_scale;

	matrix_new[12] /= new_scale;
	matrix_new[13] /= new_scale;
	matrix_new[14] /= new_scale;

	// now get the old segments matrix.  translation has already been removed
	HC_Open_Segment_By_Key(seg_key);
	HC_Show_Modelling_Matrix(matrix_old);
	HC_Show_Modelling_Matrix(old_matrix);
	HC_Close_Segment();

	// Again we need to determine and remove the scaling factor from the matrix
	float old_scale = (float) sqrt((matrix_old[0]*matrix_old[0])
					+(matrix_old[1]*matrix_old[1])
					+(matrix_old[2]*matrix_old[2]));

	matrix_old[0] /= old_scale;
	matrix_old[1] /= old_scale;
	matrix_old[2] /= old_scale;
	matrix_old[4] /= old_scale;
	matrix_old[5] /= old_scale;
	matrix_old[6] /= old_scale;
	matrix_old[8] /= old_scale;
	matrix_old[9] /= old_scale;
	matrix_old[10] /= old_scale;

	matrix_old[12] /= old_scale;
	matrix_old[13] /= old_scale;
	matrix_old[14] /= old_scale;

	HC_Open_Segment_By_Key(seg_key);
	{
		cent_old = GetTransformedCentroid(seg_key, matrix_old);
		cent_new = GetTransformedCentroid(seg_key, matrix_new);

		//calculate the distance to translate the object each time
		tvect.x = (cent_new.x - cent_old.x);
		tvect.y = (cent_new.y - cent_old.y);
		tvect.z = (cent_new.z - cent_old.z);
	}
	HC_Close_Segment();

	// now turn the matrices into quaternions
	float	matrix_interim[16];
	float	quat_old[4], quat_new[4], quat_interim[4];

	HUtility::MatrixToQuaternion(matrix_old, quat_old);
	HUtility::MatrixToQuaternion(matrix_new, quat_new);

	// Simple dot product to determine which direction the quaternion should 
	// go if we want to take the shortest path.
	float dot1 = ((quat_old[0] - quat_new[0])*(quat_old[0] - quat_new[0])
			+ (quat_old[1] - quat_new[1])*(quat_old[1] - quat_new[1])
			+ (quat_old[2] - quat_new[2])*(quat_old[2] - quat_new[2])
			+ (quat_old[3] - quat_new[3])*(quat_old[3] - quat_new[3])); 

	float dot2 = ((quat_old[0] + quat_new[0])*(quat_old[0] + quat_new[0])
			+ (quat_old[1] + quat_new[1])*(quat_old[1] + quat_new[1])
			+ (quat_old[2] + quat_new[2])*(quat_old[2] + quat_new[2])
			+ (quat_old[3] + quat_new[3])*(quat_old[3] + quat_new[3])); 

	if (dot2 < dot1) {
		quat_new[0] = -quat_new[0];
		quat_new[1] = -quat_new[1];
		quat_new[2] = -quat_new[2];
		quat_new[3] = -quat_new[3];
	}

	HC_Open_Segment_By_Key(seg_key);
	{
		float total_duration = m_pView->GetSmoothTransitionDuration();
		float q, total_time_start, current_time;
		HC_Show_Time(&total_time_start);
		do
		{
			HC_Show_Time(&current_time);
			q = ((current_time - total_time_start) / total_duration);
			if (q >= 1.0)
				q = 1.0;
			q = EaseInEaseOut(q, 0.49f, 0.02f, 0.49f);
			if (q<0)
				q=0; 

			HUtility::TransitionQuaternion(quat_old, quat_new, (float)q, quat_interim);
			HUtility::QuaternionToMatrix(quat_interim, matrix_interim);


			// linearly interpolate the scaling factor to the object
			float interim_scale = old_scale + (new_scale - old_scale)* (float)q;

			matrix_interim[0] *= interim_scale;
			matrix_interim[1] *= interim_scale;
			matrix_interim[2] *= interim_scale;
			matrix_interim[4] *= interim_scale;
			matrix_interim[5] *= interim_scale;
			matrix_interim[6] *= interim_scale;
			matrix_interim[8] *= interim_scale;
			matrix_interim[9] *= interim_scale;
			matrix_interim[10] *= interim_scale;

			// now that we have a new matrix let's unset the existing matrix and 
			// set the new quaternion matrix on it.
			HC_UnSet_Modelling_Matrix();
			HC_Set_Modelling_Matrix(matrix_interim);

			cent_new = GetTransformedCentroid(seg_key, matrix_interim);

			// figure out vector to target centroid
			HPoint vect; 
				vect.Set((cent_old.x + q*tvect.x - cent_new.x),
					(cent_old.y + q*tvect.y - cent_new.y), 
					(cent_old.z + q*tvect.z - cent_new.z));

			HC_Translate_Object(vect.x, vect.y, vect.z);

			m_pView->ForceUpdate();
		} while (q < 1.0 - EPSILON);

		// for the last iteration, just set the intended matrix, to eliminated any precision
		// errors that might have been accumulating.
		HC_Set_Modelling_Matrix(matrix_new);
		m_pView->ForceUpdate();
	}
	HC_Close_Segment();
}


void HUtility::MultiSmoothTransitions(HBaseView *m_pView, int num_keys, HC_KEY *seg_key, float *new_matrix, HCamera *new_camera)
{
	int i, j;

	// Matrix variables
	HPoint *tvect = new HPoint[num_keys],
		*cent_old = new HPoint[num_keys], 
		*cent_new = new HPoint[num_keys];
	float (*matrix_old)[16] = new float[num_keys][16],
		(*matrix_new)[16] = new float[num_keys][16], 
		(*old_matrix)[16] = new float[num_keys][16];

	float *new_scale = new float[num_keys];
	float *old_scale = new float[num_keys];

	float (*matrix_interim)[16] = new float[num_keys][16];
	float (*quat_old)[4] = new float[num_keys][4], 
		(*quat_new)[4] = new float[num_keys][4], 
		(*quat_interim)[4] = new float[num_keys][4];

	float *dot1 = new float[num_keys];
	float *dot2 = new float[num_keys];

	// Camera variables
	HCamera old_camera;
		m_pView->GetCamera(&old_camera);
	HPoint cn, co = old_camera.position;
	HPoint tn, to = old_camera.target;
	HPoint un, uo = old_camera.up_vector;
	float fieldwidthn = 0, fieldwidtho = old_camera.field_width;
	float fieldheightn = 0, fieldheighto = old_camera.field_height;

	HPoint position_move_delta;
	HPoint target_move_delta;
	HPoint position, up_vector;
	bool need_rotation = true;
	float cquat_old[4], cquat_new[4], cquat_interim[4];
	float cmatrix_interim[16];
	float clength = 0, clength_delta = 0;
	float field_width_delta = 0, field_height_delta = 0;


	//MATRIX preprocessing
	for (i = 0; i < num_keys; i++)
	{
		memcpy(matrix_new[i], &new_matrix[i*16], 16*sizeof(float));

		// need to determine the scaling factor as the quaternions do not take scales into account
		new_scale[i] = (float) sqrt((matrix_new[i][0]*matrix_new[i][0])
				+(matrix_new[i][1]*matrix_new[i][1])
				+(matrix_new[i][2]*matrix_new[i][2]));
		// need to remove the scaling factor from the matrix before we turn it into a quaternion
		for (j = 0; j < 16; j++)
			if ((j-3) % 4 != 0)
				matrix_new[i][j] /= new_scale[i];

		// now get the old segments matrix.  translation has already been removed
		HC_Open_Segment_By_Key(seg_key[i]);
		HC_Show_Modelling_Matrix(matrix_old[i]);
		HC_Show_Modelling_Matrix(old_matrix[i]);
		HC_Close_Segment();

		// Again we need to determine and remove the scaling factor from the matrix
		old_scale[i] = (float) sqrt((matrix_old[i][0]*matrix_old[i][0])
						+(matrix_old[i][1]*matrix_old[i][1])
						+(matrix_old[i][2]*matrix_old[i][2]));
		for (j = 0; j < 16; j++)
			if ((j-3) % 4 != 0)
				matrix_old[i][j] /= old_scale[i];

		//calculate the vector along which the centroid of the segment should move
		HC_Open_Segment_By_Key(seg_key[i]);
		{
			cent_old[i] = GetTransformedCentroid(seg_key[i], matrix_old[i]);
			cent_new[i] = GetTransformedCentroid(seg_key[i], matrix_new[i]);

			tvect[i].x = (cent_new[i].x - cent_old[i].x);
			tvect[i].y = (cent_new[i].y - cent_old[i].y);
			tvect[i].z = (cent_new[i].z - cent_old[i].z);
		}
		HC_Close_Segment();

		// now turn the matrices into quaternions
		HUtility::MatrixToQuaternion(matrix_old[i], quat_old[i]);
		HUtility::MatrixToQuaternion(matrix_new[i], quat_new[i]);

		// Simple dot product to determine which direction the quaternion should 
		// go if we want to take the shortest path.
		dot1[i] = ((quat_old[i][0] - quat_new[i][0])*(quat_old[i][0] - quat_new[i][0])
				+ (quat_old[i][1] - quat_new[i][1])*(quat_old[i][1] - quat_new[i][1])
				+ (quat_old[i][2] - quat_new[i][2])*(quat_old[i][2] - quat_new[i][2])
				+ (quat_old[i][3] - quat_new[i][3])*(quat_old[i][3] - quat_new[i][3])); 

		dot2[i] = ((quat_old[i][0] + quat_new[i][0])*(quat_old[i][0] + quat_new[i][0])
				+ (quat_old[i][1] + quat_new[i][1])*(quat_old[i][1] + quat_new[i][1])
				+ (quat_old[i][2] + quat_new[i][2])*(quat_old[i][2] + quat_new[i][2])
				+ (quat_old[i][3] + quat_new[i][3])*(quat_old[i][3] + quat_new[i][3])); 

		if (dot2[i] < dot1[i]) {
			quat_new[i][0] = -quat_new[i][0];
			quat_new[i][1] = -quat_new[i][1];
			quat_new[i][2] = -quat_new[i][2];
			quat_new[i][3] = -quat_new[i][3];
		}
	} //MATRIX preprocessing

	//CAMERA preprocessing
	if (new_camera)
	{
		cn = new_camera->position;
		tn = new_camera->target;
		un = new_camera->up_vector;
		fieldwidthn = new_camera->field_width;
		fieldheightn = new_camera->field_height;

		if (m_pView->GetRenderMode()==HRenderHiddenLine)
		{
			HC_Set_Camera_Target(tn.x,tn.y,tn.z);
			HC_Set_Camera_Position(cn.x,cn.y,cn.z);
			HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
			HC_Set_Camera_Field(fieldwidthn,fieldheightn);

			m_pView->CameraPositionChanged();
			m_pView->Update();
		}	
		else if (!CamerasEqual(old_camera, *new_camera))
		{
			float matrix_old[16],  matrix_new[16];
			HPoint view_vector_old, view_vector_new;

			view_vector_old.Set(to.x - co.x, to.y - co.y, to.z - co.z);
			view_vector_new.Set(tn.x - cn.x, tn.y - cn.y, tn.z - cn.z);
			clength = (float)HC_Compute_Vector_Length(&view_vector_old);
			clength_delta = (float)HC_Compute_Vector_Length(&view_vector_new) - (float)HC_Compute_Vector_Length(&view_vector_old);

			HC_Compute_Normalized_Vector(&view_vector_old,&view_vector_old);
			HC_Compute_Normalized_Vector(&view_vector_new,&view_vector_new);

			HPoint rotation_delta;

			rotation_delta.Set(view_vector_old.x - view_vector_new.x,
				view_vector_old.y - view_vector_new.y,
				view_vector_old.z - view_vector_new.z);

			if ((float)HC_Compute_Vector_Length(&rotation_delta) < 0.0001f)
				need_rotation=false;

			HPoint translation;
			translation.Set(0.0f,0.0f,0.0f);

			if (need_rotation)
			{
				HUtility::GetMatrixFromCamera(to, co, uo, translation, matrix_old);
				HUtility::GetMatrixFromCamera(tn, cn, un, translation, matrix_new);
			}
			else
			{
				position_move_delta.Set(cn.x-co.x, cn.y-co.y, cn.z-co.z);
			}
			
			if (need_rotation)
			{
				HUtility::MatrixToQuaternion(matrix_old, cquat_old);
				HUtility::MatrixToQuaternion(matrix_new, cquat_new);
			}
			
			target_move_delta.Set(tn.x-to.x, tn.y-to.y, tn.z-to.z);

			field_width_delta = fieldwidthn - fieldwidtho;
			field_height_delta = fieldheightn - fieldheighto;

			float dot1, dot2;

			if (need_rotation)
			{
				// Simple dot product to determine which direction the quaternion should 
				// go if we want to take the shortest path.
				dot1 = ((cquat_old[0] - cquat_new[0])*(cquat_old[0] - cquat_new[0])
					+ (cquat_old[1] - cquat_new[1])*(cquat_old[1] - cquat_new[1])
					+ (cquat_old[2] - cquat_new[2])*(cquat_old[2] - cquat_new[2])
					+ (cquat_old[3] - cquat_new[3])*(cquat_old[3] - cquat_new[3])); 

				dot2 = ((cquat_old[0] + cquat_new[0])*(cquat_old[0] + cquat_new[0])
					+ (cquat_old[1] + cquat_new[1])*(cquat_old[1] + cquat_new[1])
					+ (cquat_old[2] + cquat_new[2])*(cquat_old[2] + cquat_new[2])
					+ (cquat_old[3] + cquat_new[3])*(cquat_old[3] + cquat_new[3])); 

				if (dot2 < dot1) {
					cquat_new[0] = -cquat_new[0];
					cquat_new[1] = -cquat_new[1];
					cquat_new[2] = -cquat_new[2];
					cquat_new[3] = -cquat_new[3];
				}
			}
		}
	} //CAMERA preprocessing
	
	//this block does the smooth transitions	
	float total_duration = m_pView->GetSmoothTransitionDuration();
	float q, total_time_start, current_time;
	HC_Show_Time(&total_time_start);
	do
	{
		HC_Show_Time(&current_time);
		q = ((current_time - total_time_start) / total_duration);
		if (q >= 1.0)
			q = 1.0;
		q = EaseInEaseOut(q, 0.49f, 0.02f, 0.49f);
		if (q<0)
			q=0; 

		//camera transitions
		if (new_camera)
		{
			HC_Open_Segment_By_Key(m_pView->GetSceneKey());
			{
				if (need_rotation)
				{
					HPoint to2;
					to2.Set(to.x + (q*target_move_delta.x),
						to.y + (q*target_move_delta.y),
						to.z + (q*target_move_delta.z));
					// first let use quaternions to take care of the interpolation 
					// of the rotation element of the camera movement
					HUtility::TransitionQuaternion(cquat_old, cquat_new, q, cquat_interim);
					HUtility::QuaternionToMatrix(cquat_interim, cmatrix_interim);

					position.x = to2.x - (cmatrix_interim[8] * (clength+clength_delta*q));
					position.y = to2.y - (cmatrix_interim[9] *  (clength+clength_delta*q));
					position.z = to2.z - (cmatrix_interim[10] * (clength+clength_delta*q));

					up_vector.x = cmatrix_interim[4];
					up_vector.y = cmatrix_interim[5];
					up_vector.z = cmatrix_interim[6];

					HC_Set_Camera_Position(position.x, position.y, position.z);
					HC_Set_Camera_Up_Vector(up_vector.x, up_vector.y, up_vector.z);
				}
				else
				{
					HC_Set_Camera_Position(co.x + (q*position_move_delta.x), 
						co.y + (q*position_move_delta.y), 
						co.z + (q*position_move_delta.z));
				}

				// now linearly interpolate the camera target and field changes
				HC_Set_Camera_Target(to.x + (q*target_move_delta.x), 
					to.y + (q*target_move_delta.y), 
					to.z + (q*target_move_delta.z));

				float fw = fieldwidtho+(q*field_width_delta);
				float fh = fieldheighto+(q*field_height_delta);
				if (fw > 0.00001f && fh > 0.00001f)
				{
					HC_Set_Camera_Field(fw,fh);
					m_pView->CameraPositionChanged();
					if (m_pView->GetConstantFrameRateObject())
						m_pView->GetConstantFrameRateObject()->SetActivityType(GeneralActivity);
				}
			}
			HC_Close_Segment();
		}

		//matrix transitions
		for (i = 0; i < num_keys; i++)
		{
			//now do the actual transitional calculations
			HC_Open_Segment_By_Key(seg_key[i]);
			{
				HUtility::TransitionQuaternion(quat_old[i], quat_new[i], (float)q, quat_interim[i]);
				HUtility::QuaternionToMatrix(quat_interim[i], matrix_interim[i]);

				// linearly interpolate the scaling factor to the object
				float interim_scale = old_scale[i] + (new_scale[i] - old_scale[i])* (float)q;

				matrix_interim[i][0] *= interim_scale;
				matrix_interim[i][1] *= interim_scale;
				matrix_interim[i][2] *= interim_scale;
				matrix_interim[i][4] *= interim_scale;
				matrix_interim[i][5] *= interim_scale;
				matrix_interim[i][6] *= interim_scale;
				matrix_interim[i][8] *= interim_scale;
				matrix_interim[i][9] *= interim_scale;
				matrix_interim[i][10] *= interim_scale;

				// now that we have a new matrix let's unset the existing matrix and 
				// set the new quaternion matrix on it.
				HC_UnSet_Modelling_Matrix();
				HC_Set_Modelling_Matrix(matrix_interim[i]);

				cent_new[i] = GetTransformedCentroid(seg_key[i], matrix_interim[i]);

				HPoint vect; 
				vect.Set(
					(cent_old[i].x + q*tvect[i].x - cent_new[i].x),
					(cent_old[i].y + q*tvect[i].y - cent_new[i].y), 
					(cent_old[i].z + q*tvect[i].z - cent_new[i].z));

				HC_Translate_Object(vect.x, vect.y, vect.z);
			}
			HC_Close_Segment();
		}

		m_pView->ForceUpdate();
	} while (q < 1.0 - EPSILON);

	//these set the cameras/matrices to their exact locations
	if (new_camera)
	{
		HC_Set_Camera_Target(tn.x,tn.y,tn.z);
		HC_Set_Camera_Position(cn.x,cn.y,cn.z);
		HC_Set_Camera_Up_Vector(un.x,un.y,un.z);
		HC_Set_Camera_Field(fieldwidthn,fieldheightn);
	}
	for (i = 0; i < num_keys; i++)
	{
		HC_Open_Segment_By_Key(seg_key[i]);
			HC_Set_Modelling_Matrix(&new_matrix[i*16]);
		HC_Close_Segment();
	}
	m_pView->ForceUpdate();

	//cleanup
	delete [] tvect;
	delete [] cent_old;
	delete [] cent_new;
	delete [] matrix_old;
	delete [] matrix_new; 
	delete [] old_matrix;

	delete [] new_scale;
	delete [] old_scale;

	delete [] matrix_interim;
	delete [] quat_old;
	delete [] quat_new;
	delete [] quat_interim;

	delete [] dot1;
	delete [] dot2;

}




HPoint HUtility::GetCentroid(HC_KEY seg_key)
{
	HC_Open_Segment_By_Key(seg_key);
 		HPoint min, max, centroid, w_coord;

		// get centroid (in object space units)
 		HC_Compute_Circumcuboid (".", &min, &max);

		centroid.x = 0.5F * (min.x + max.x);
		centroid.y = 0.5F * (min.y + max.y);
		centroid.z = 0.5F * (min.z + max.z);
		HC_Compute_Coordinates(".","object",&centroid,"world",&centroid);
	HC_Close_Segment();
	
	return centroid;
}

HPoint HUtility::GetTransformedCentroid(HC_KEY seg_key, float *matrix)
{
	HC_Open_Segment_By_Key(seg_key);
 		HPoint min, max, centroid, w_coord;

		// get centroid (in object space units)
 		HC_Compute_Circumcuboid (".", &min, &max);

		centroid.x = 0.5F * (min.x + max.x);
		centroid.y = 0.5F * (min.y + max.y);
		centroid.z = 0.5F * (min.z + max.z);

		if (matrix)
			HC_Compute_Transformed_Points(1, &centroid, matrix, &centroid);
	HC_Close_Segment();
	
	return centroid;
}


void HUtility::URIdecode(const char * in, char * out, MVO_POINTER_SIZED_UINT * n)
{

    *n=0;

    for (;;){

	char c=*in;
	in++;

	if (c == '\0' || c==' ') 
	    return;

	if (c == '+') 
	    *out++=' ';
	
	else if (c == '%'){
	    unsigned int v=0;
	    bool first = true;

	    while(1){

		    c=*in++;

		    switch((int)c){
			    case 0x0030: v |= 0x0000; break;
			    case 0x0031: v |= 0x0001; break;
			    case 0x0032: v |= 0x0002; break;
			    case 0x0033: v |= 0x0003; break;
			    case 0x0034: v |= 0x0004; break;
			    case 0x0035: v |= 0x0005; break;
			    case 0x0036: v |= 0x0006; break;
			    case 0x0037: v |= 0x0007; break;
			    case 0x0038: v |= 0x0008; break;
			    case 0x0039: v |= 0x0009; break;
			    case 0x0041: v |= 0x000A; break;
			    case 0x0042: v |= 0x000B; break;
			    case 0x0043: v |= 0x000C; break;
			    case 0x0044: v |= 0x000D; break;
			    case 0x0045: v |= 0x000E; break;
			    case 0x0046: v |= 0x000F; break;
			    default: return;
		    }

		    if (first){
			    v <<= 4;
			    first=false;
		    }else 
			    break;
	    }
	    //another possibility for the above while loop would be the following block
	    /*
	    if (*in++ <= 0x39) {
		v |= (c - 0x39) << 4;
	    }
	    else {
		v |= (c - 0x41 + 0xA) << 4;
	    } 
	    if (*in++ <= 0x39) {
		v |= c - 0x39;
	    }
	    else {
		v |= c - 0x41 + 0xA;
	    }
	    */

	    *out++=(char)v;
	}
	else{
	    *out++ = c;
	}
		++*n;
    }
}

// protected characters: a--z, A--Z, 0--9, *_-.@
// blank is converted into +
// rest of non-protected char becomes %hex
void HUtility::URIencode(const char * in, MVO_POINTER_SIZED_UINT n, char * out)
{
    unsigned int i, j=0;

    for (i=0;i<n;i++) {

	    char c= (char)in[i];

	    if (c == ' ') {
		    out[j] = '+';
		    j++;

	    }
	    else if (c=='_' || c=='-' || c=='*' || c=='.' || c=='@'
		    || c>='a' && c<='z' || c>='A' && c<='Z' || c>='0' && c<='9'){

		    out[j] = c;
		    j++;

	    }
	    else
	    {
			const char * HEX = "0123456789ABCDEF";

			out[j] = '%';
			j++;
			out[j]=HEX[(((unsigned char)c)>>4)];
			j++;
			out[j]=HEX[(((unsigned char)c)&0x000F)];
			j++;
	    }
    }
    out[j] = '\0';
}



// gets "pushed" attribute setting from open segment
// internal use only
bool HUtility::RestoreAttributeFromOption(const char *attName, HUserOptions& attText)
{
	HUserOptions optionText;
	char extAttName[MVO_BUFFER_SIZE];
	sprintf(extAttName,"att %s", attName);
  	optionText.Show_User_Options();
	if (strstr(optionText.m_data, extAttName))
	{
 		attText.Show_One_User_Option(extAttName);
		HC_UnSet_One_User_Option(extAttName);
		ReplaceCharacter(attText.m_data, '$',',');
		return true;
	}
	return false;
}


//is there a string.h function that does this???
void HUtility::ReplaceCharacter(char *text, char orig, char target)
{
	for (unsigned int i=0;i<strlen(text);i++)
		if (text[i] == orig)
			text[i] = target;
}


//calls appropriate HC_Set... function based on attribute name
bool HUtility::SetAttribute(const char *attName, const char *attText)
{
	bool ret = true;
	if (strcmp(attName,"color") == 0)
	{
		HC_UnSet_Color();
		HC_Set_Color(attText);
	}
	else if (strcmp(attName,"visibility") == 0)
	{
		HC_UnSet_Visibility();
		HC_Set_Visibility(attText);
	}
	else if (strcmp(attName,"rendering options") == 0)
	{
		HC_UnSet_Rendering_Options();
		HC_Set_Rendering_Options(attText);
	}
	else if (strcmp(attName,"edge weight") == 0)
	{
		HC_UnSet_Edge_Weight();
		HCLOCALE(HC_Set_Edge_Weight(atof(attText)));
	}else
		ret = false;
	return ret;
}


//calls approriate HC_Show... function based on attribute name
//non string values need to be converted
bool HUtility::ShowAttribute(const char *attName, char *attText)
{
	bool ret = true;
	if (strcmp(attName,"color") == 0)
		HC_Show_Color(attText);
	else if (strcmp(attName,"visibility") == 0)
		HC_Show_Visibility(attText);
	else if (strcmp(attName,"rendering options") == 0)
		HC_Show_Rendering_Options(attText);
	else if (strcmp(attName,"edge weight") == 0)
	{
		float weight;
		HC_Show_Edge_Weight(&weight);
		HCLOCALE(sprintf(attText,"%f",weight));
	}
	else 
		ret = false;

	return ret;
}

  
bool HUtility::PushAttribute(const char *attName)
{
	char optionText[MVO_BUFFER_SIZE];
	char attText[MVO_BUFFER_SIZE];
 	if (HC_Show_Existence(attName))
	{
		if (ShowAttribute(attName, attText))
		{		
			sprintf(optionText,"att %s = %s",attName, attText);
			ReplaceCharacter(optionText, ',','$');
			HC_Set_User_Options(optionText);
			return true;
		}
	}
	return false; 
}

void HUtility::PushAttribute(const char *attName, const char *attText)
{
	char optionText[MVO_BUFFER_SIZE];
	
	sprintf(optionText,"att %s = %s",attName, attText);
	ReplaceCharacter(optionText, ',','$');
	HC_Set_User_Options(optionText);
}
 

bool HUtility::PopAttribute(const char *attName)
{
	HUserOptions attText;
	if (HC_Show_Existence("user options"))
	{
		if (RestoreAttributeFromOption(attName, attText))
		{
 			if (SetAttribute(attName, attText.m_data))
				return true;
		}
	}
	return false; 
  
}


bool HUtility::ModifyPushedAttribute(const char *attName, const char *newStyle)
{
	HUserOptions usrOpt;
	char attText[MVO_BUFFER_SIZE];
	char optionText[MVO_BUFFER_SIZE];

	if (HC_Show_Existence("user options"))
	{
		if (RestoreAttributeFromOption(attName, usrOpt))
		{
			HC_Open_Segment("/att_temp");
				SetAttribute(attName,usrOpt.m_data);
				SetAttribute(attName,newStyle);
				ShowAttribute(attName, attText);
			HC_Close_Segment();

			HC_Delete_Segment("/att_temp");
			sprintf(optionText,"att %s = %s",attName, attText);
			ReplaceCharacter(optionText, ',','$');
			HC_Set_User_Options(optionText);
			return true;
			
		}
	}
	return false;
}

bool HUtility::ShowPushedAttribute(const char *attName, HUserOptions& attText)
{
 	if (HC_Show_Existence("user options"))
	{
		if (RestoreAttributeFromOption(attName, attText))
			return true;
	}
	return false;
}

HC_KEY HUtility::Merge_Shells(bool do_recurse, bool delete_orig_shells , bool optimize_shell, bool generate_regions)
{
    HC_KEY	key;
    HVector	*v;
    HShell	total_info;
    HShell	max_info;
    HShell	optimized;
    int		i;
    
    Segment_Shell_Allocate (&total_info, &max_info, do_recurse);
    Recursive_Merge_Shells (&total_info, &max_info, do_recurse, delete_orig_shells);

	if (optimize_shell){
		HShell optimized_info;

		/* This is cleaned up in HShell::~HShell(); */
		optimized_info.point_list = new HPoint[total_info.point_list_len];
		optimized_info.normal_list = new HVector[total_info.point_list_len];
		optimized_info.face_list  = new int[total_info.face_list_len];
		
		HC_Compute_Optimized_Shell(
			total_info.point_list_len, total_info.point_list,
			total_info.normal_list,
			total_info.face_list_len,
			total_info.face_list,
			"fix handedness",
			&optimized_info.point_list_len,
			optimized_info.point_list,
			&optimized_info.face_list_len,
			optimized_info.face_list,
			0,
			0);

		key = HC_KInsert_Shell (optimized_info.point_list_len, 
								 optimized_info.point_list,
								 optimized_info.face_list_len, 
								 optimized_info.face_list);
	}
	else {
		key = HC_KInsert_Shell (total_info.point_list_len, 
								 total_info.point_list,
								 total_info.face_list_len, 
								 total_info.face_list);
	}

	if (generate_regions){
		HC_Open_Geometry(key);{
			for (i=0; i<total_info.region_count ; ++i){
				int first_face_offset, count;
				HC_Open_Region(i+1);{
					first_face_offset = total_info.region_list[2*i +0];
					count = total_info.region_list[2*i +1];
					HC_Set_Faces(first_face_offset, count);
				}HC_Close_Region();
			}
		}HC_Close_Geometry();
	}

	HC_Open_Geometry(key);{
		for (i = 0 ; i < total_info.point_list_len ; i++){
			v = &(total_info.normal_list[ i ]);
			if (v->x != 0 || v->y != 0 || v->z != 0) {
				HC_Open_Vertex(i);
				HC_Set_Normal(v->x,  v->y,  v->z);
				HC_Close_Vertex();
			}
		}
	}HC_Close_Geometry();
    
    return key;   
}

void HUtility::Shell_Search(HShell *total_info, HShell *max_info, bool do_recurse)
{
    int		local_point_list_len;
    int		local_face_list_len;
    HC_KEY	key;
    int		num_shells = 0;
    char	type[MVO_BUFFER_SIZE];
    char	child[MVO_SEGMENT_PATHNAME_BUFFER];
    
    HC_Begin_Segment_Search ("*");
    if (do_recurse)	
    {
	while (HC_Find_Segment (child)) 
	    {
		HC_Open_Segment (child);
		Shell_Search (total_info, max_info, do_recurse);
		HC_Close_Segment();
	    }
    }
    HC_End_Segment_Search();
    
    HC_Begin_Contents_Search (".", "shells");
    HC_Show_Contents_Count (&num_shells);
    while (HC_Find_Contents (type, &key)) 
    {
	HC_Show_Shell_Size(key, &local_point_list_len, &local_face_list_len);
	/* keep track of the totals  */
	total_info->point_list_len += local_point_list_len;
	total_info->face_list_len  += local_face_list_len;
	/* keep track of the maximums */
	if (max_info->point_list_len < local_point_list_len)
		max_info->point_list_len = local_point_list_len;
	if (max_info->face_list_len < local_face_list_len)
		max_info->face_list_len = local_face_list_len;
    }
    HC_End_Contents_Search ();
	
}

bool HUtility::Segment_Shell_Allocate(HShell *total_info, HShell *max_info, bool do_recurse)
{
    Shell_Search (total_info, max_info, do_recurse);
    
    max_info->point_list = new HPoint[max_info->point_list_len];
	max_info->normal_list = 0;
    max_info->face_list  = new int[max_info->face_list_len];
    total_info->point_list = new HPoint[total_info->point_list_len];
    total_info->normal_list = new HVector[total_info->point_list_len];
    total_info->face_list  = new int[total_info->face_list_len];
    
    /* clear the lengths of the structures */
    max_info->point_list_len	    = 0;
    max_info->face_list_len	    = 0;
	max_info->face_list_len       = 0;
    total_info->point_list_len    = 0;
    total_info->face_list_len	    = 0;
	total_info->face_list_len     = 0;
    
    /* if any of the allocations failed, free the memory for all and return false.  */
    if (!(max_info->point_list && max_info->face_list &&
         total_info->point_list && total_info->face_list &&
		 total_info->normal_list)){
	    return false;
	}

	return true;
	
}

void HUtility::Recursive_Merge_Shells(HShell *total_info, HShell *max_info, bool do_recurse, bool delete_orig_shells, bool insert_null_lods )
{
    int		polygon_length;
    int		i, j, index;
    char	type[MVO_BUFFER_SIZE];
    HC_KEY	key;
    HVector	*v;
    int		current_point_list_offset, current_face_list_offset, count;

    /* recursively find all of the child segments */

	if (do_recurse){
		HC_Begin_Contents_Search ("...", "shells");
		HC_Show_Contents_Count(&count);
		total_info->region_count += count;

		HC_Begin_Contents_Search ("...", "includes");
		while(HC_Find_Contents (type, &key)){
			HC_KEY included_segment = HC_KShow_Include_Segment(key);
			HC_Open_Segment_By_Key(included_segment);{
				Recursive_Merge_Shells(total_info, max_info, do_recurse, delete_orig_shells, insert_null_lods);
			}HC_Close_Segment();
		}
		HC_End_Contents_Search();

	} else {
		HC_Begin_Contents_Search (".", "shells");
		HC_Show_Contents_Count(&count);
		total_info->region_count = count;
	}
	
	if (!total_info->region_list)
		total_info->region_list = new int[2 * total_info->region_count];

	index = 0;
    while (HC_Find_Contents (type, &key)) 
    {
		{
			current_point_list_offset = total_info->point_list_len;
			current_face_list_offset  = total_info->face_list_len;
	    
			/*
			 *  use max_info for the scratch-space buffers
			 */
			HC_Show_Shell (key, 
						&max_info->point_list_len, 
						max_info->point_list, 
						&max_info->face_list_len, 
						max_info->face_list 
						);
			HC_Show_Shell_Face_Count(key, &max_info->face_count);

			if (insert_null_lods)
			{
				HC_Open_Geometry(key);
					HC_Open_LOD(1);
						HC_Insert_Shell(0,0,0,0);
					HC_Close_LOD();
				HC_Close_Geometry();
			}

			/* 
			 * Copy the point and face list into the main face list
			 * For each individual shell, the point list originally started 
			 *   at zero, but for the main list, it starts at 
			 *   current_point_list_offset.  
			 */

			/* here, we copy the point list */
			for (i = 0 ; i < max_info->point_list_len ; i++)
			{
				total_info->point_list[ current_point_list_offset + i].x = max_info->point_list[i].x;
				total_info->point_list[ current_point_list_offset + i].y = max_info->point_list[i].y;
				total_info->point_list[ current_point_list_offset + i].z = max_info->point_list[i].z;
			}
			HC_Open_Geometry(key);

			for (i = 0 ; i < max_info->point_list_len ; ++i)
			{
				v = &(total_info->normal_list[ current_point_list_offset + i]);
				HC_Open_Vertex(i);
				HC_Show_Net_Normal(&(v->x), &(v->y), &(v->z));
				HC_Close_Vertex();
			}
			HC_Close_Geometry();
		    
		    
			/* here, we copy the face list */
			i = 0;
			total_info->region_list[2*index +0] = total_info->face_count;
			total_info->region_list[2*index +1] = max_info->face_count;
			total_info->face_count += max_info->face_count;
			++index;
			while(i < max_info->face_list_len)
			{
				/*
				 * in this loop:
				 * main_face_list[current_face_list_offset]  
				 *      is the start of the individual shell
				 * main_face_list[current_face_list_offset + i]  
				 *     is the start of a polygon inside the shell
				 * main_face_list[current_face_list_offset + i + j]
				 *      is a point inside the polygon inside the shell
				 * the face list is made up of polygon entries.  For each entry,
				 *      the length comes first, then a number of data points
				 *      equal to the length.
				 */
				polygon_length = max_info->face_list[i];
				/* 
				 * hoops syntax: to subtract a polygon from a shell, use negative
				 *      polygon length.  We account for this here with abs. val.
				 */
				if (polygon_length < 0) polygon_length = -polygon_length;
				total_info->face_list[ i + current_face_list_offset ] =  
					max_info->face_list[i];
				for (j = 1 ; j <= polygon_length ; ++j) 
				{
					/*
					 * remember, the point list was moved, so the face list 
					 * has to be modified as it is copied
					 */
					total_info->face_list[ current_face_list_offset + i + j ] = 
						max_info->face_list[ i + j ] + current_point_list_offset;
				}
				i += polygon_length + 1;
			    
			} /* end while (i < local_face_list_len) */
		    
			/* update the current offsets and prepare to load the next shell. */
			total_info->point_list_len += max_info->point_list_len;
			total_info->face_list_len  += max_info->face_list_len;
		}
		    
    } /* end while (HC_Find_Contents(...)) */
    HC_End_Contents_Search ();


	if (delete_orig_shells){
		if (do_recurse)
			HC_Flush_Contents ("...", "shells");
		else
			HC_Flush_Contents (".", "shells");
	}
}


void HUtility::OptimizeSegmentTree(HC_KEY old_seg_key, HC_KEY *new_seg_key, bool transform_shells, bool merge_shells, bool create_shell_regions)
{
	UNREFERENCED(transform_shells);
	UNREFERENCED(create_shell_regions);

	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
	char seg_name[MVO_SEGMENT_PATHNAME_BUFFER];
	char style_segment[MVO_SEGMENT_PATHNAME_BUFFER];
	char type[MVO_BUFFER_SIZE];
	float r, g, b;
	char space[10];


	HC_KEY shell_key, style_key;

	float net_matrix[16];

	*new_seg_key = HC_KCreate_Segment("/new_tree");

	HC_Open_Segment_By_Key(old_seg_key);

		HC_Begin_Segment_Search("...");
		while (HC_Find_Segment(pathname))
		{
			HC_Open_Segment(pathname);

				// if a style exists, obtain it and directly set the color
				// as a result, the Show_Net_Color will produce the correct 
				// color value
				HC_Begin_Contents_Search(".", "style");
				while (HC_Find_Contents(type, &style_key)) 
				{
					HC_Show_Style_Segment(style_key, style_segment);
					HC_QShow_One_Net_Color_By_Value(style_segment, "faces=diffuse", space, &r, &g, &b);
					HC_Set_Color_By_Value("faces", space, r, g, b);
				}
				HC_End_Contents_Search();

				HC_Show_One_Net_Color_By_Value("faces=diffuse", space, &r, &g, &b);

				// find all shells in the segment, xform their points
				// and insert them into the appropriate colored subsegment
				// of the 'new_seg_key' segment 
				HC_Begin_Contents_Search(".", "shells");
				while (HC_Find_Contents(type, &shell_key))
				{
					HC_Show_Net_Modelling_Matrix(net_matrix);

 					int numpoints, numfaces; 

					HC_Show_Shell_Size(shell_key, &numpoints, &numfaces);
					HPoint *points = new HPoint[numpoints];
					int *flist = new int[numfaces];

					HC_Show_Shell(shell_key, &numpoints, points, &numfaces, flist);
					HC_Compute_Transformed_Points(numpoints, points, net_matrix, points);

					HC_Open_Segment_By_Key(*new_seg_key);
						HCLOCALE (sprintf(seg_name, "%f%f%f", r, g, b)); 
						HC_Open_Segment(seg_name);
						  HC_Set_Color_By_Value("faces", space, r, g, b);
						  HC_Insert_Shell(numpoints, points, numfaces, flist);
						HC_Close_Segment();
					HC_Close_Segment();

					delete []points;
					delete []flist;
				}	
				HC_End_Contents_Search();

			HC_Close_Segment();
		}
		HC_End_Segment_Search();

	HC_Close_Segment();

	// we've delete the subsugments of new_seg_key, now flush out that segment itself
	// we just want to flush geoemtry and attributes
	HC_Open_Segment_By_Key(old_seg_key);
	  HC_Flush_Contents(".", "everything");
	  HC_Move_By_Key(*new_seg_key, ".");
    HC_Close_Segment();


	// open the new master segment and merge shells as desired
	HC_Open_Segment_By_Key(*new_seg_key);

		HC_Begin_Segment_Search("...");
		while (HC_Find_Segment(pathname))
		{
			HC_Open_Segment(pathname);
			  if ((merge_shells) && HC_Show_Existence("shells"))
			    Merge_Shells(false, true);
			HC_Close_Segment();
		}
		HC_End_Segment_Search();	

	HC_Close_Segment();
}



/* 
 Local function which returns number of faces in a shell given its face list information.
 \param face_list_len number of elements in face_list array
 \param face_list connectivity information
*/
static int GetNumShellFaces(int face_list_len, int * face_list)
{
	int num_faces = 0;
	for (int i = 0; i < face_list_len;)
	{
		int num_face_pts = face_list[i];
		if (num_face_pts > 0)
			num_faces++;
		else
			num_face_pts = abs(num_face_pts);
		i = i + num_face_pts + 1;
	}
	return num_faces;

}

HC_KEY	HUtility::Merge_Shells_Generate_Regions(int num_shells, HC_KEY * shell_keys, bool delete_orig_shells)
{
	HC_KEY merged_shell_key = INVALID_KEY;
	if (num_shells < 1)
		return merged_shell_key;

	int shell_point_list_len = 0;
	int shell_face_list_len = 0;
	int total_point_list_len = 0;
	int total_face_list_len = 0;
	int max_shell_point_list_len = 0;
	int max_shell_face_list_len = 0;
	int optimized_point_list_len = 0;
	int optimized_face_list_len = 0;
	int total_point_list_offset = 0;
	int total_face_list_offset = 0;
	int * shell_face_counts = new int[num_shells];

	// gather how many points and faces we have in all the given shells
	for (int i_sh = 0; i_sh < num_shells; i_sh++)
	{
		HC_Show_Shell_Size(shell_keys[i_sh], &shell_point_list_len, &shell_face_list_len);
		total_point_list_len += shell_point_list_len;
		total_face_list_len += shell_face_list_len;
		if (max_shell_point_list_len < shell_point_list_len)
			max_shell_point_list_len = shell_point_list_len;
		if (max_shell_face_list_len < shell_face_list_len)
			max_shell_face_list_len = shell_face_list_len;
	}

	if (total_point_list_len && total_face_list_len)
	{
		HPoint *shell_point_list = (HPoint*) malloc (max_shell_point_list_len * sizeof(HPoint));
		int *shell_face_list = (int*) malloc (max_shell_face_list_len * sizeof(int));

		HPoint *total_point_list = (HPoint*) malloc (total_point_list_len * sizeof(HPoint));
		HPoint *total_normal_list = (HPoint*) malloc (total_point_list_len * sizeof(HPoint));
		int *total_face_list = (int*) malloc (total_face_list_len * sizeof(int));

		HPoint *optimized_point_list = (HPoint*) malloc (total_point_list_len * sizeof(HPoint));
		int *optimized_face_list = (int*) malloc (total_face_list_len * sizeof(int));
		
		int *point_mapping_list = (int *) malloc (total_point_list_len * sizeof (int));
		int *face_mapping_list = (int *) malloc (total_face_list_len * sizeof (int));

		int i;

		for (int j_sh = 0; j_sh < num_shells; j_sh++)
		{

			// get the shell point and normal data
			HC_Show_Shell(shell_keys[j_sh], &shell_point_list_len, shell_point_list, &shell_face_list_len, shell_face_list);
			HC_Open_Geometry(shell_keys[j_sh]);  // we'll be querying normals soon
				for (i = 0; i < shell_point_list_len; i++)
				{
					total_point_list[total_point_list_offset + i].x = shell_point_list[i].x;
					total_point_list[total_point_list_offset + i].y = shell_point_list[i].y;
					total_point_list[total_point_list_offset + i].z = shell_point_list[i].z;
					HC_Open_Vertex(i);
					HC_Show_Net_Normal(&total_normal_list[total_point_list_offset + i].x,
										&total_normal_list[total_point_list_offset + i].y,
										&total_normal_list[total_point_list_offset + i].z);
					HC_Close_Vertex();
				}
			HC_Close_Geometry();

			shell_face_counts[j_sh] = GetNumShellFaces(shell_face_list_len, shell_face_list);

			// get the face list data
			for (i = 0; i < shell_face_list_len;)
			{
				int j;
				assert(shell_face_list[i] > 0); // if negative, means subtract this poly from shell.
				total_face_list[total_face_list_offset + i] = shell_face_list[i];
				for (j = 1; j <= shell_face_list[i]; j++)
				{
					total_face_list[total_face_list_offset + i + j] = shell_face_list[i+j] + total_point_list_offset;
				}
				i += 1 + shell_face_list[i];
			}

			total_face_list_offset += shell_face_list_len;
			total_point_list_offset += shell_point_list_len;
			
			// get rid of original shell if so
			if (delete_orig_shells)
				HC_Delete_By_Key(shell_keys[j_sh]);
		}

		// list is assembled, now put it together into a single optimized shell.
		HC_Compute_Optimized_Shell(total_point_list_len, total_point_list,
							  (float *)total_normal_list,
							  total_face_list_len, total_face_list,
							  "tolerance = 0% fru, normal tolerance = 1degrees",
							  &optimized_point_list_len,optimized_point_list,
							  &optimized_face_list_len, optimized_face_list,
							  point_mapping_list, face_mapping_list);

		merged_shell_key = HC_KInsert_Shell (optimized_point_list_len, 
								optimized_point_list,
								optimized_face_list_len, 
								optimized_face_list);

		// set the normals
		HC_Open_Geometry(merged_shell_key);
			for (i = 0 ; i < total_point_list_len ; i++)
			{
				if (point_mapping_list[i] >= 0)
				{
					HC_Open_Vertex(point_mapping_list[i]);
						HC_Set_Normal(total_normal_list[i].x,
										total_normal_list[i].y,
										total_normal_list[i].z);
					HC_Close_Vertex();
				}
			}

			// open each face and now set approriate region for it
			int region_num = 0;
			int face_num = 0;
			for (i = 0; i < num_shells; i++)
			{
				int num_faces = shell_face_counts[i];
				for (int j = 0; j < num_faces; j++)
				{
					if (face_mapping_list[face_num] != -1)
					{
						HC_Open_Face(face_mapping_list[face_num]);
							HC_Set_Region(region_num);
						HC_Close_Face();
					}
					face_num++;
				}
				region_num++;
			}
		HC_Close_Geometry();

		
		// clean up
		if (shell_point_list)
			free(shell_point_list);
		if (shell_face_list)
			free(shell_face_list);

		if (total_point_list)
			free(total_point_list);
		if (total_normal_list)
			free(total_normal_list);
		if (total_face_list)
			free(total_face_list);

		if (optimized_point_list)
			free(optimized_point_list);
		if (optimized_face_list)
			free(optimized_face_list);

		if (point_mapping_list)
			free(point_mapping_list);
		if (face_mapping_list)
			free(face_mapping_list);
	}
	
	delete[] shell_face_counts;
	return merged_shell_key;
}

void HUtility::Generate_LODs(bool do_recurse, bool delete_orig_shells, bool insert_null_lods, 
							 int n_lod_levels, float reduction_ratio)
{
	UNREFERENCED(insert_null_lods);

	HShell merged_shell;

    HC_KEY shell_key = Merge_Shells (do_recurse, delete_orig_shells, true, false);

	HC_Show_Shell_Size(shell_key, &merged_shell.point_list_len, &merged_shell.face_list_len);

	merged_shell.point_list = new HPoint[merged_shell.point_list_len];
	merged_shell.face_list = new int[merged_shell.face_list_len];

	HC_Show_Shell(shell_key, &merged_shell.point_list_len, merged_shell.point_list,
		&merged_shell.face_list_len, merged_shell.face_list);
    
	char lod_ratio[MVO_BUFFER_SIZE];
	HCLOCALE(sprintf(lod_ratio,"lod = %f%%",reduction_ratio*100.0));

    for (int count = 1; count <= n_lod_levels; count++)
    {
	// let's only do this if we have a good sized merged shell
		if (merged_shell.point_list_len > 100)
		{

			// reduce each LOD to ratio% of the vertex size of the previous LOD
			HC_Compute_Optimized_Shell (merged_shell.point_list_len, merged_shell.point_list,
							 0,
							 merged_shell.face_list_len, merged_shell.face_list,
							 lod_ratio,
							 &(merged_shell.point_list_len), merged_shell.point_list,
							 &(merged_shell.face_list_len), merged_shell.face_list,
							 0, 0);

			HC_Open_Geometry(shell_key);
			HC_Open_LOD(count);
				HC_Flush_Contents(".", "everything");
				HC_Insert_Shell (merged_shell.point_list_len, 
							 merged_shell.point_list,
							 merged_shell.face_list_len, 
							 merged_shell.face_list);
			HC_Close_LOD(); 
			HC_Close_Geometry();
		}
    }

    return;
}


void HUtility::CountShellData (
    HC_KEY      shell_key,
    int *       faces,
    int *       vertices,
    int *       triangles,
    int *		tristrip_count) {

	if (vertices != 0 || triangles != 0) {
		int	ts_length = 0, numtriangles=0, numvertices=0;
		HC_Show_Shell_By_Tristrips_Size (shell_key, &numvertices, &ts_length, &numtriangles);

		if (triangles != 0)
			*triangles = numtriangles;
		if (vertices != 0)
			*vertices = numvertices;
	}
 
	if (faces != 0) {
		int	numfaces = 0;
		HC_Show_Shell_Face_Count(shell_key, &numfaces);
		*faces = numfaces;
	}

	if (tristrip_count != 0) {
		int	numtristrips = 0;
		HC_Show_Shell_Tristrip_Count(shell_key, &numtristrips);
	   *tristrip_count = numtristrips;
	}

}

void HUtility::FindFileNameExtension(const char * filename_in, char * ext_out, int size)
{
	UNREFERENCED(size);

	ext_out[0] = '\0';

	bool ext_is_num;
	int offset = -1;
	do 
	{
		ext_is_num = true;
		HC_Parse_String(filename_in, ".", offset, ext_out);

		// make it lower case
		for (size_t i = 0; i < strlen(ext_out); i++)
		{
			ext_out[i] = (char) tolower(ext_out[i]);
			if (ext_is_num && !::isdigit(ext_out[i])) // many file types end in .# suffixes, so ignore trailing .# extensions
				ext_is_num = false;
		}
		offset -= static_cast<int>(strlen(ext_out));
	}
	while (ext_is_num && offset >= - static_cast<int>(strlen(filename_in)));
}

void HUtility::FindFileNameExtension(wchar_t const * const filename_in, __wchar_t * ext_out, int size)
{
	HCLOCALE(
	ext_out[0] = L'\0';
	bool ext_is_num;
	wchar_t const * cur = filename_in + wcslen(filename_in);
	do 
	{
		ext_is_num = true;
		int len =0;
		while(*--cur != L'.' && cur >= filename_in)
			len++;

		cur++;

		if (cur == filename_in)
		{
			wcscpy(ext_out,filename_in);
			break;
		}
		
		for (int i = 0; i < len; i ++)
		{
			if(size != -1){
				if(size == 1)
				{
					ext_out[i] = L'\0';
					break;
				}
				--size;
			}
			ext_out[i] = towlower(cur[i]);
			if (ext_is_num && !::isdigit(ext_out[i])) // many file types end in .# suffixes, so ignore trailing .# extensions
				ext_is_num = false;
		}
		ext_out[len] = L'\0';
		cur--;
	}
	while (ext_is_num);
	); //HCLOCALE 
}

void HUtility::FindUnicodeFileNameExtension(const unsigned short * filename_in, char * ext_out)
{
	ext_out[0] = '\0';
	int len16 = H_UNICODE_STRLEN(filename_in);
	
	bool ext_is_num;
	const unsigned short * cur = filename_in + len16 -1;
	do
	{
		ext_is_num = true;
		int len =0;
		while(*--cur != '.' && cur >= filename_in)
			len++;

		cur++;

		if (cur == filename_in)
		{
			for (int i = 0; i < len16; i ++)
				ext_out[i] = filename_in[i];
			break;
		}
		
		for (int i = 0; i < len; i ++)
		{
			ext_out[i] = tolower(cur[i]);
			if (ext_is_num && !::isdigit(ext_out[i])) // many file types end in .# suffixes, so ignore trailing .# extensions
				ext_is_num = false;
		}
		ext_out[len] = '\0';
		cur--;
	}
	while (ext_is_num);
}


void HUtility::FindFileNameAndDirectory(const char * filename_in, char * dir_out, char * filename_out)
{
	strcpy(dir_out, filename_in);

	unsigned int i = static_cast<unsigned int>(strlen(filename_in));

	while(i!=0 && dir_out[i] != '/' && dir_out[i] != '\\') i--;

	if (i!=0) {

		//SKIP MULT SEPERATORS
		while (dir_out[i] != '\0' && (dir_out[i] == '/' || dir_out[i] == '\\'))i++;

		if (dir_out[i] == '\0') {
			//NO FILE
			if(filename_out) {
				filename_out[0] = '\0';
			}
		} else {
			if (filename_out) {
				strcpy(filename_out, static_cast<const char *>(&dir_out[i]));
			}
			dir_out[i]='\0';
		}

	}else{
		//NO DIR
		if (filename_out) {
			strcpy(filename_out, filename_in);
		}
		dir_out[i]='\0';
	}

}

void HUtility::FindFileNameAndDirectory(wchar_t const * filename_in, wchar_t * dir_out, wchar_t * filename_out)
{
	wcscpy(dir_out, filename_in);

	unsigned int i = (unsigned int)wcslen(filename_in);

	while(i!=0 && dir_out[i] != L'/' && dir_out[i] != L'\\') i--;

	if (i!=0) {

		//SKIP MULT SEPERATORS
		while (dir_out[i] != L'\0' && (dir_out[i] == L'/' || dir_out[i] == L'\\'))i++;

		if (dir_out[i] == L'\0') {
			//NO FILE
			if(filename_out) {
				filename_out[0] = L'\0';
			}
		} else {
			if (filename_out) {
				wcscpy(filename_out, (wchar_t const*)(&dir_out[i]));
			}
			dir_out[i] = L'\0';
		}

	}else{
		//NO DIR
		if (filename_out) {
			wcscpy(filename_out, filename_in);
		}
		dir_out[i] = L'\0';
	}
}


const char* HUtility::extra_pointer_format()
{
	char buffer[MVO_BUFFER_SIZE];

	sprintf (buffer, "%p", (void*) buffer);
	if (buffer[0] == '0' && buffer[1] == 'x')
		return "";
	else 
		return "0x";

}


int HUtility::CalculateKeyPath(HC_KEY startkey, HBaseView *view, HC_KEY *keyarray, int maxkeynum)
{
	HC_KEY newkey = startkey;
	int knum = 0;
	keyarray[knum++] = newkey;
	char type[MVO_BUFFER_SIZE];

	while (true)
	{
		if (knum >= maxkeynum)
			return 0;
		char path[MVO_SEGMENT_PATHNAME_BUFFER];

		HC_Show_Segment(newkey, path);
		if (newkey == view->GetModel()->GetModelKey())
		{
			keyarray[knum++] = view->GetIncludeLinkKey();
			break;
		}
		if (newkey == view->GetViewKey())
			return knum;
		HC_Show_Key_Type(newkey, type);
		if (streq(type, "segment"))
		{
  			newkey  = HC_Show_Owner_Original_Key(newkey);
			if (newkey == HC_ERROR_KEY)
				return knum;
			keyarray[knum++] = newkey;
		}
		else
			break;
	}
 
	keyarray[knum++] = view->GetSceneKey();
	return knum;
}
 	


void HUtility::scale_segment (HIC_Rendition	*rendition,HIC_Segment_Info * geo3) 
{
	HPoint dist;
	HUserOptions text;
	float matrix1[16], matrix2[16], matrix3[16], matrixt[16], netmatrix[16], netmatrixinverse[16];
  	float scale;
	HBaseView * view = (HBaseView *)HIC_Show_User_Index(rendition, H_HBASEVIEW_POINTER_MAGIC);
	
	void ** suppress_scaling;
	HC_KEY  segmentkey = HIC_Show_Segment_Key(geo3);
	HC_Open_Segment_By_Key(segmentkey);	    
 	    HC_Open_Segment("geom");	
		HC_Show_One_User_Index(H_SUPPRESS_SCALING_MAGIC,(void **)&suppress_scaling);
		HC_Set_User_Index(H_SUPPRESS_SCALING_MAGIC,(void *)0);
	    HC_Close_Segment();	
	HC_Close_Segment();

	if (!view)
		return;
	
//	if (!view->GetCameraChanged() && !suppress_scaling)
//	{    HIC_Draw_Segment(rendition, geo3);
//	    return;
//	}

	void * disable_scale = HIC_Show_User_Index(rendition, H_DISABLE_SCALE_MAGIC);

   	HC_Open_Segment_By_Key(segmentkey);	    
	HC_Show_Net_Modelling_Matrix(netmatrix);

	float s1 = (float)HC_Compute_Vector_Length(&netmatrix[0]);
	float s2 = (float)HC_Compute_Vector_Length(&netmatrix[4]);
	float s3 = (float)HC_Compute_Vector_Length(&netmatrix[8]);

 
	HC_Compute_Matrix_Inverse(netmatrix,netmatrixinverse);

 	HC_Open_Segment("geom");
	text.Show_One_User_Option("gpos");
 	HPoint orig;
	HCLOCALE (sscanf(text.m_data, "%f %f %f", &orig.x, &orig.y, &orig.z));
  	HC_Compute_Identity_Matrix(matrix2);
	matrix2[12] = orig.x;
	matrix2[13] = orig.y;
	matrix2[14] = orig.z;		
	
	HC_KEY keys[MVO_BUFFER_SIZE];
	int kcount = CalculateKeyPath(segmentkey, view, keys, MVO_BUFFER_SIZE);
	assert(kcount > 0);
 	HC_Compute_Coordinates_By_Path(kcount, keys, "object",&orig,"viewpoint",&dist);
 	float fx, fy;
	char proj[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(view->GetSceneKey());
	HC_Show_Net_Camera_Projection(proj);
//needs to be cleaned up!!!	
	HPoint pp,tt, dt;
	HC_Show_Net_Camera_Position(&pp.x, &pp.y, &pp.z);
	HC_Show_Net_Camera_Target(&tt.x, &tt.y, &tt.z);
	dt.Set(tt.x - pp.x, tt.y - pp.y, tt.z - pp.z);
	float vl = (float)HC_Compute_Vector_Length(&dt);

	if (disable_scale)
	{
		HC_Compute_Transformed_Points(1, &orig, netmatrix, &orig);
		float d =  1.0f;
 		HC_Compute_Scale_Matrix(d/s1,d/s2,d/s3, matrix1);
	}
	else
	{
	if (proj[0]=='p')
	{
		HC_Show_Net_Camera_Field(&fx,&fy);
		float ttt = (vl/fx) / 2.5f;
		scale = dist.z / ttt;
		HC_Compute_Scale_Matrix(scale/s1, scale/s2, scale/s3, matrix1);
	}
 
	else
	{
		HC_Show_Net_Camera_Field(&fx,&fy);
		scale = fx * 2.5f;
 		HC_Compute_Scale_Matrix(scale/s1, scale/s2, scale/s3, matrix1);
		
 
	}
	}
	HC_Close_Segment();

  	HC_Compute_Matrix_Product(matrix1, matrix2, matrixt);
 	HC_Compute_Matrix_Product(matrixt,netmatrixinverse, matrix3);
 	HC_Set_Modelling_Matrix(matrixt);
 	HC_Close_Segment();
	HC_Close_Segment();
    HIC_Draw_Segment(rendition, geo3);
}


void HUtility::activity_change (HIC_Segment *seg, int old_activity, int new_activity)
{
	UNREFERENCED (seg);
	UNREFERENCED (old_activity);
	UNREFERENCED (new_activity);
}


HC_KEY HUtility::CreateScaleIndependentSegment(const char *segname, bool setCallback)
{
	float matrix[16];
	HC_KEY sikey = HC_Open_Segment(segname);{
		HC_Open_Segment("contents");{
			HC_Open_Segment("geom");{
				HC_Set_User_Index(H_SUPPRESS_SCALING_MAGIC, (void *)1);
				HC_Compute_Identity_Matrix(matrix);
				HC_Set_Modelling_Matrix(matrix);
			}HC_Close_Segment();
			if (setCallback)
				HC_Set_Rendering_Options("mask transform = (camera = scale, perspective, model = scale)");
		}HC_Close_Segment();
	}HC_Close_Segment();
	return sikey;
}

HC_KEY HUtility::GetScaleIndependentGeomSegmentKey(HC_KEY segkey)
{
	HC_KEY geomkey;
	HC_Open_Segment_By_Key(segkey);{
		HC_Open_Segment("contents");{
			geomkey = HC_Create_Segment("geom");
		}HC_Close_Segment();
	}HC_Close_Segment();
	return geomkey;
}

void HUtility::SetScaleIndependentGeomPosition(HC_KEY segkey, float x, float y, float z)
{
	char text[256];
	HC_Open_Segment_By_Key(segkey);{
		float matrix[16];
		HC_Compute_Translation_Matrix(x, y, z, matrix);
		HC_Set_Modelling_Matrix(matrix);
		HC_Open_Segment("contents");{
			HC_Open_Segment("geom");{
				HCLOCALE(sprintf(text, "gpos = %f %f %f", x, y, z));
				HC_Set_User_Options(text); 
			}HC_Close_Segment();
		}HC_Close_Segment();
	}HC_Close_Segment();
}

void HUtility::GetScaleIndependentGeomPosition(HC_KEY segkey, float &x, float &y, float &z)
{
	HUserOptions text;
	HC_Open_Segment_By_Key(segkey);{
		HC_Open_Segment("contents");{
			HC_Open_Segment("geom");{
				text.Show_One_User_Option("gpos");
				HCLOCALE(sscanf(text.m_data, "%f %f %f", &x, &y, &z));
			}HC_Close_Segment();
		}HC_Close_Segment();
	}HC_Close_Segment();
}




void HUtility::RegisterCallbacks()
{
	HImRegisterCallback("HUtility_scale_segment", scale_segment);
	HImRegisterCallback("HUtility_activity_change", activity_change);
}


bool HUtility::IsModelKey(HC_KEY modelkey, HC_KEY objectkey)
{
	char owner[MVO_SEGMENT_PATHNAME_BUFFER];
	char modelsegment[MVO_SEGMENT_PATHNAME_BUFFER];
	HC_Show_Segment(modelkey, modelsegment);
	HC_Show_Owner_By_Key(objectkey, owner);
	return strncmp(modelsegment, owner, strlen(modelsegment)) == 0;
}

HC_KEY  HUtility::UserOptionExists(HC_KEY key, const char *attrib, char *result)
{
	HC_KEY newkey = key;
	HC_KEY oldkey = INVALID_KEY;
	
	while (1)
	{
		HC_Open_Segment_By_Key(newkey);
		
		HUserOptions uopt;
		uopt.Show_Net_User_Options();
		if (strstr(uopt.m_data, attrib) == 0)
		{
			HC_Close_Segment();
			break;
		}
		else
		{
		    if (result)
			HC_Show_One_Net_User_Option(attrib,result);
			oldkey = newkey;
			newkey = HC_KShow_Owner_By_Key(newkey);
		}
		HC_Close_Segment();
		
	}
	return oldkey;
}

bool HUtility::SegmentExists(const char * segment)
{
	int count = HC_QShow_Existence(segment, "self");
	return count != 0;
}

void HUtility::UnsetAttributes()
{
 	if (HC_Show_Existence("visibility"))
		HC_UnSet_Visibility();
 	if (HC_Show_Existence("rendering options"))
		HC_UnSet_Rendering_Options();
	if (HC_Show_Existence("heuristics"))
	{
		char heuristics[MVO_BUFFER_SIZE];
		HC_Show_Heuristics(heuristics);
		if (strstr(heuristics, "hidden"))
			HC_UnSet_One_Heuristic("hidden surfaces");
		if (strstr(heuristics, "culling"))
			HC_UnSet_One_Heuristic("culling");
	}
	HImUnSetOneCallback("draw segment");
	HImUnSetOneCallback("draw 3d geometry");
	HImUnSetOneCallback("draw segment tree");
}

bool HUtility::IsIdentityMatrix(float * matrix)
{
	float idMatrix[16] = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1,
	};
	// can't use memcmp because of -0.0f and +0.0f
	for (int i = 0; i < 16; ++i) {
		if (matrix[i] != idMatrix[i]) {
			return false;
		}
	}
	return true;
}


int HUtility::IsChildOf(HC_KEY owner, HC_KEY child, HC_KEY *klist)
{
	char type[MVO_BUFFER_SIZE];
	int i=0;
	HC_Show_Key_Type(child, type);
		if (strcmp(type, "segment")!= 0)
		{
			child = HC_KShow_Owner_By_Key(child);
			if (klist)
				klist[i] = child;
			i++;
		}
	while (1)
	{
		if (child == owner)
			return i;
		child = HC_KShow_Owner_By_Key(child);
		if (child == INVALID_KEY)
			break;
		if (klist)
			klist[i] = child;
		i++;
	}
	return 0;
}


void HUtility::ComputeShellNormal(HC_KEY skey, HPoint &tnormal)
{
	// if the key doesn't exist, normal doesn't either
	if (!HC_Show_Existence_By_Key(skey, "self"))
	{
		tnormal.Set(0,0,0);
		return;
	}

	char ktype[MVO_BUFFER_SIZE];
	HC_Show_Key_Type(skey,ktype);
	if (strcmp(ktype,"shell")!=0)
	{
		tnormal.Set(0,0,0);
		return;
	}
	
	
	int pcount,flen;
	HC_Show_Shell_Size(skey,&pcount,&flen);
	HPoint *points = new HPoint[pcount];
	int *flist = new int[flen];

	HC_Show_Shell(skey, &pcount, points, &flen, flist);	
	int flist_pos=0;
	unsigned short numfaces=0;
 	tnormal.Set(0,0,0);
 	while (flist_pos < flen)
	{
		HPoint fpoints[1000];
		int fpcount = flist[flist_pos++];

		for (int i=0;i<fpcount;i++)
		{
			fpoints[i].Set(points[flist[flist_pos]].x,points[flist[flist_pos]].y,points[flist[flist_pos]].z);
			flist_pos++;
		}
		HPoint normal;
 		HC_Compute_Polygon_Normal(fpcount,fpoints,"right",&normal);
		tnormal.x=tnormal.x+normal.x;
		tnormal.y=tnormal.y+normal.y;
		tnormal.z=tnormal.z+normal.z;
		numfaces++;
	}
	tnormal.x/=numfaces;
	tnormal.y/=numfaces;
	tnormal.z/=numfaces;
	
	if (fabs(tnormal.x)+fabs(tnormal.y)+fabs(tnormal.z)<0.1)
		tnormal.x = tnormal.y=tnormal.z = 0;
	else
 		HC_Compute_Normalized_Vector(&tnormal,&tnormal);

	if (flist)
		delete[] flist;
	if (points)
		delete[] points;
}


HC_KEY HUtility::SelectUnderCursor(HBaseView *view, float window_x,float window_y, bool doSelection, bool DeSelect)
{
    HC_KEY retkey = INVALID_KEY, key = INVALID_KEY;
	int count;

    HC_Open_Segment_By_Key(view->GetViewKey());
		count = HC_Compute_Selection(".","./scene/overwrite","v, selection level = entity", window_x, window_y);
    HC_Close_Segment();
	if (count)
	{
		HC_Show_Selection_Original_Key(&key);
		retkey=key;

		// build up an array of include keys to pass with the selection
		int skey_count;
		char skey_type[50];
		HC_Show_Selection_Keys_Count(&skey_count);

		HC_KEY *keys		= new HC_KEY[skey_count];
		HC_KEY *incl_keys	= new HC_KEY[skey_count];
		HC_Show_Selection_Original_Keys(&skey_count, keys);

		int i, incl_count = 0;
		for (i = skey_count-1; i >= 0; i--)
		{
			HC_Show_Key_Type(keys[i], skey_type);
			if (streq(skey_type, "include"))
				incl_keys[incl_count++] = keys[i];
			else if(streq(skey_type, "reference"))
				key = keys[i];
		}

		if (doSelection)
		{
			HSelectionSet* selection = view->GetSelection();
			bool old_sel_level = selection->GetAllowEntitySelection();
			selection->SetAllowEntitySelection(old_sel_level);
			
			if (!DeSelect)
			{
				if (!(selection->IsSelected(key)))  		
					selection->Select(key, incl_count, incl_keys, true);	
			}
			else
			{
				if ((selection->IsSelected(key)))  		
					selection->DeSelect(key);	
			}
			selection->SetAllowEntitySelection(old_sel_level);
		}

		delete [] keys;
		delete [] incl_keys;
	}
    return retkey;
}

HC_KEY HUtility::SmartSelectUnderCursor(HBaseView *view, float window_x,float window_y, bool doSelection, bool DeSelect)
{
    HC_KEY key = INVALID_KEY;
	int count;

    HC_Open_Segment_By_Key(view->GetViewKey());
		count = HC_Compute_Selection(".","./scene/overwrite","v, selection level = entity", window_x, window_y);
    HC_Close_Segment();
	if (count)
	{
		// Mouse went down; find the selected items
		HC_Show_Selection_Element (&key, 0, 0, 0);	
		// in case it is locally renumbered, grab the original HOOPS keys
		HC_Show_Selection_Original_Key(&key);

		// build up an array of include keys to pass with the selection
		if (doSelection)
		{
			int skey_count;
			char skey_type[MVO_BUFFER_SIZE];
			HSelectionSet* selection = view->GetSelection();
			int incl_count = 0;

			HC_Show_Selection_Keys_Count(&skey_count);

			HC_KEY *keys		= new HC_KEY[skey_count];
			HC_KEY *incl_keys	= new HC_KEY[skey_count];
			HC_Show_Selection_Original_Keys(&skey_count, keys);

			for (int i = skey_count-1; i >= 0; i--)
			{
				HC_Show_Key_Type(keys[i], skey_type);
				if (strstr(skey_type, "include"))
				{
					incl_keys[incl_count] = keys[i];
					incl_count++;
				}
				else if(streq(skey_type, "reference"))
					key = keys[i];
			}
			
			if (DeSelect)
			{
				if ((selection->IsSelected(key, incl_count, incl_keys)))  		
					selection->DeSelect(key, incl_count, incl_keys, true);	
			}
			else
			{
				if (!(selection->IsSelected(key, incl_count, incl_keys)) && !HUtilityAnnotation::IsNote(key))  		
					selection->Select(key, incl_count, incl_keys,true);	
			}

			delete[] keys;
			delete[] incl_keys;
		}

	}
	if (HUtilityAnnotation::IsNote(key))
		return INVALID_KEY;

    return key;
}

bool HUtility::ShowOneHeuristic(const char *type, char *res)
{
    char text[MVO_BUFFER_SIZE];
    if (HC_Show_Existence("heuristics"))
    {
	HC_Show_Heuristics(text);
	if (strstr(text, type))
	{
	    if (res)
		HC_Show_One_Heuristic(type, res);
	    return true;
	}
	else
	    return false;
    }
    return false;
}

bool HUtility::ShowOneRenderingOption(const char *type, char *res)
{
    char text[MVO_BUFFER_SIZE];
    if (HC_Show_Existence("rendering options"))
    {
	HC_Show_Rendering_Options(text);
	if (strstr(text, type))
	{
	    if (res)
		HC_Show_One_Rendering_Option(type, res);
	    return true;
	}
	else
	    return false;
    }
    return false;
}



void HUtility::UnwindIncludes(HC_KEY &newkey)
{
	HC_KEY *keys = 0;
	int count;
	
	HC_Show_Selection_Keys_Count (&count);
	keys = new HC_KEY[count];
	HC_Show_Selection_Keys (&count, keys);
	UnwindIncludes(newkey, keys, count);
	delete [] keys;

}

void HUtility::UnwindIncludes(HC_KEY &newkey, HC_KEY *keys, int count)
{
	HC_KEY lastowner = -1; 
	int i=count-1;
	char sname[4096];
	while (1)
	{	
 		HC_Show_Key_Type(keys[i], sname);	
 		if (strcmp(sname, "include") == 0 || i<=0)
			break;
		lastowner = keys[i];
		i--;
		
	}
	if (i==0)
	{
		newkey = keys[0];
		return;
	}
	i--;
	while (1)
	{	
		HC_Open_Segment_By_Key(lastowner);
		HC_Show_Key_Type(keys[i], sname);
		HC_Close_Segment();
		if (strcmp(sname, "segment") != 0)
		{
			newkey = lastowner;
			return;
		}
		HC_Open_Segment_By_Key(lastowner);
		HC_Show_Segment(keys[i], sname);
		lastowner = HC_KShow_Original_Key(keys[i]);
		HC_Close_Segment();

		if (i<=0)
			break;
		i--;

	}
	if (i==0)
	{
		newkey = lastowner;
		return;
	}

}
		
static bool is_special_char(char check)
{
	//we assume (hope) that no backquotes will be present
	char special[]={'"', '\'', '\\', '/'};
	int i, max=sizeof(special)/sizeof(char);

	for(i=0; i<max; ++i)
	{
		if(check==special[i])
			return true;
	}
	
	return false;
}

bool HUtility::SafeSetUserOption(const char * opt)
{
	const char * cursor=opt, * start;
	size_t new_size=2*strlen(opt), parens;
	char * new_opt=new char [new_size], * new_cursor;
	bool has_special, ret=true;
	new_cursor=new_opt;

	if(!opt || *opt=='\0')
	{
		ret=false;
		goto Cleanup;
	}
	
	while(*cursor!='\0')
	{
		//get the left side of the assignment
		start=cursor;
		has_special=false;
		while(*cursor!='\0' && *cursor!='=')
		{
			if(is_special_char(*cursor))
				has_special=true;
			++cursor;
		}
		
		if(has_special)
			*new_cursor++='`';
		memcpy(new_cursor, start, cursor-start);
		new_cursor+=cursor-start;
		if(has_special)
			*new_cursor++='`';
		
		//copy the = sign
		if(*cursor!='=')
		{
			ret=false;
			goto Cleanup;
		}
		else
			*new_cursor++=*cursor++;
		
		//get the right side
		start=cursor;
		has_special=false;
		parens=0;
		while(*cursor!='\0' && (*cursor!=',' || parens!=0))
		{
			if(is_special_char(*cursor))
				has_special=true;
			if(*cursor == '(')
				++parens;
			else if(*cursor == ')')
				--parens;
			++cursor;
		}
		
		if(has_special)
			*new_cursor++='`';
		memcpy(new_cursor, start, cursor-start);
		new_cursor+=cursor-start;
		if(has_special)
			*new_cursor++='`';
	}
	*new_cursor='\0';

Cleanup:
	if(ret)
		HC_Set_User_Options(new_opt);
	delete [] new_opt;	
	return ret;
}

bool HUtility::SafeShowUserOption(char * opt)
{
	char * rough_opt;
	int length, i, j;

	if(!HC_Show_Existence("user option"))
		return false;

	HC_Show_User_Options_Length(&length);
	rough_opt = new char [length+1];
	HC_Show_User_Options(rough_opt);
	i=j=0;

	while(rough_opt[i]!='\0')
	{
		if(rough_opt[i]!='`')
			opt[j++]=rough_opt[i++];
		else
			++i;
	}
	opt[j]='\0';
	delete rough_opt;
	return true;
}

void HUtility::ProjectShell(HC_KEY shellkey)
{
    float matrix[16];
    HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(shellkey));
    HC_Show_Net_Modelling_Matrix(matrix);
    HC_Close_Segment();
    int plen, flen;
    HPoint *points;
    int *faces;

    HC_Show_Shell_Size(shellkey, &plen, &flen);
    points = new HPoint[plen];
    faces = new int[flen];
    HC_Show_Shell(shellkey, &plen, points, &flen, faces);
    HC_Compute_Transformed_Points(plen, points, matrix, points);
    HC_Edit_Shell_Points(shellkey, 0, plen, plen, points);

    delete [] points;
    delete [] faces;
}



HC_KEY HUtility::MergeShells(HC_KEY shell1, HC_KEY shell2)
{
    int			plen1;
	int			flen1;
	int			plen2;
	int			flen2;
	int			plen3;
	int			flen3;
    HPoint *	points1;
    int *		faces1;
    HPoint *	points2;
    int *		faces2;

    HC_Show_Shell_Size(shell1, &plen1, &flen1);
    points1 = new HPoint[plen1];
    faces1 = new int[flen1];
    HC_Show_Shell(shell1, &plen1, points1, &flen1, faces1);

    HC_Show_Shell_Size(shell2, &plen2, &flen2);
    points2 = new HPoint[plen2];
    faces2 = new int[flen2];
    HC_Show_Shell(shell2, &plen2, points2, &flen2, faces2);

    plen3 = plen1 + plen2;
    flen3 = flen1 + flen2;
    
    HPoint *	points3 = new HPoint[plen3];
    int *		faces3 = new int[flen3];

	int i;
    for (i=0;i<plen1;i++)
		points3[i] = points1[i];
    for (i=0;i<plen2;i++)
		points3[i + plen1] = points2[i];

    for (i=0;i<flen1;i++)
		faces3[i] = faces1[i];

    i=0;
    while (i < flen2) {
		faces3[i+flen1] = faces2[i];
		int numpoints = faces2[i];
		i++;
		for (int j = 0;j<numpoints;j++)
			faces3[i+flen1 + j] = faces2[i + j] + plen1;
		i+=numpoints;
    }

    HC_KEY retkey = HC_KInsert_Shell(plen3, points3, flen3, faces3);

    delete [] points1;
    delete [] points2;
    delete [] points3;

    delete [] faces1;
    delete [] faces2;
    delete [] faces3;

    return retkey;
}


void HUtility::ShowModellingMatrix(float *segment_modelling_matrix)
{
    
    if (HC_Show_Existence("modelling matrix"))
		HC_Show_Modelling_Matrix(segment_modelling_matrix);
    else
		HC_Compute_Identity_Matrix(segment_modelling_matrix); 
}


void HUtility::ConvertLocalPixelsToLocalWindow(HBaseView *view, HPoint *in , HPoint *out)
{
    HC_Open_Segment_By_Key(view->GetSceneKey());
    if (out)
		HC_Compute_Coordinates(".", "local pixels", in, "local window", out);
    else
		HC_Compute_Coordinates(".", "local pixels", in, "local window", in);
    HC_Close_Segment();

}

HC_KEY HUtility::GrabSegment(HC_KEY key)
{
    char type[MVO_BUFFER_SIZE];
    if (key >= 0) {
		/* if it is a locally renumbered key for which we don't have the proper context, bail */
		if (HC_KShow_Original_Key(key) == INVALID_KEY)
			return INVALID_KEY;
	}
	
    HC_Show_Key_Type(key, type);
    if (strcmp(type, "segment") != 0)
		return HC_KShow_Owner_Original_Key(key);
    else
		return key;
}

void HUtility::RenameSegment(HC_KEY segkey, const char *newname)
{
    
    char name[MVO_BUFFER_SIZE];
    char oldname[MVO_BUFFER_SIZE];
    char temp[MVO_BUFFER_SIZE];
    HC_Show_Segment(segkey, name);
    strcpy(oldname, name);
    HC_Parse_String(name, "/", -1, temp);
    name[strlen(name) - strlen(temp)] = 0;
    strcat(name, newname);
    if (strcmp(newname,"") == 0)
        HC_Rename_Segment(oldname,"");
    else
		HC_Rename_Segment(oldname,name);
}


void HUtility::InterpolateNormalAlongEdge(HPoint &p1, HPoint &n1, HPoint &p2, HPoint &n2, HPoint &p3, HPoint &n3)
{
    
    HPoint delta1, delta2;
    delta1.Set(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    delta2.Set(p3.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    float l1 = (float)HC_Compute_Vector_Length(&delta1);
    float l2 = (float)HC_Compute_Vector_Length(&delta2);
    delta1.Set(n2.x - n1.x, n2.y - n1.y, n2.z - n1.z);
    delta2.Set(n3.x - n1.x, n2.y - n1.y, n2.z - n1.z);

//    HC_Compute_Normalized_Vector(&delta1, &delta1);
    float ft = 1.0f / l1 * l2;
    n3.Set(n1.x + ft *delta1.x, n1.y + ft *delta1.y, n1.z + ft *delta1.z);
    HC_Compute_Normalized_Vector(&n3, &n3);

}


void HUtility::ShowNormals(HC_KEY shellkey, float normallength)
{

    int plen, flen;
    HPoint *points;
    int *faces;

    HC_Show_Shell_Size(shellkey, &plen, &flen);
    points = new HPoint[plen];
    faces = new int[flen];
    HC_Show_Shell(shellkey, &plen, points, &flen, faces);

    for (int i=0;i<plen;i++)
    {
		HC_Open_Geometry(shellkey);
    
		HC_Open_Vertex(i);
		HPoint normal;
		HC_Show_Net_Normal(&normal.x, &normal.y, &normal.z);
		HC_Close_Vertex();  
		HC_Close_Geometry();
		HC_Compute_Normalized_Vector(&normal, &normal);
		normal.Set(normal.x * normallength, normal.y *normallength, normal.z * normallength);
		HC_Insert_Line(points[i].x, points[i].y, points[i].z, points[i].x + normal.x, 
		points[i].y + normal.y, points[i].z + normal.z);
    }
   
    delete [] points;
    delete [] faces;


}



bool HUtility::IsSegmentKey(HC_KEY key)
{
	char type[MVO_BUFFER_SIZE];
	HC_Show_Key_Type(key, type);
	return strcmp(type, "segment") == 0;
}



int HUtility::FindContents(HC_KEY & key)
{
	return HC_Find_Contents(0, &key);
}


int HUtility::CalculateMajorAxis(HBaseView *view)
{
	HPoint t;
	HC_Open_Segment_By_Key(view->GetSceneKey());{
		HC_PShow_Net_Camera_Up_Vector(0, 0, &t.x, &t.y, &t.z);
	}HC_Close_Segment();

	float x = fabs(t.x);
	float y = fabs(t.y);
	float z = fabs(t.z);

	if (z >= x && z >= y)
		return 2;
	else if (y >= x && y >= z)
		return 0;
	else
		return 1;
}

HC_KEY HUtility::FindOneObject(const char *type, bool l)
{
    HC_KEY key = INVALID_KEY;
	if (!l)
		HC_Begin_Contents_Search("...", type);
	else
		HC_Begin_Contents_Search(".", type);

    FindContents(key);
    HC_End_Contents_Search();
    return key;
}

    
  
void HUtility::CloseAllSegments(HC_KEY endkey)
{
    int count;
    HC_Begin_Open_Segment_Search();
    HC_Show_Open_Segment_Count (&count);
    HC_End_Open_Segment_Search();
    for (int i=0;i<count;i++)
    {
	HC_KEY key = HC_KCreate_Segment(".");
	if (key == endkey)
	{	
	    HC_Close_Segment();
	    return;
	}
	HC_Close_Segment();
    }
}


void HUtility::ReadjustTarget(HBaseView *view)
{
    HPoint p1,p2;
    HPoint target;

    HC_Open_Segment_By_Key(view->GetViewKey());
		int selection_count = HC_Compute_Selection(".", "./scene/overwrite","v, selection level = entity", 0,0);

		if (!selection_count)
		{
			selection_count = HC_Compute_Selection_By_Area(".", "./scene/overwrite","v, selection level = entity", -0.3, 0.3, -0.3, 0.3);
			if (!selection_count)
				selection_count = HC_Compute_Selection_By_Area(".", "./scene/overwrite","v, selection level = entity", -1, 1, -1, 1);
		}
		if (selection_count)
		{
			HC_Show_Selection_Position(&p1.x, &p1.y, &p1.z, &p2.x, &p2.y, &p2.z);
			HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
			HC_Compute_Coordinates(".", "world", &p2, "viewpoint", &p2);
			HC_Compute_Coordinates(".", "world", &target, "viewpoint", &target);
			target.z = p2.z;
			HC_Compute_Coordinates(".", "viewpoint", &target, "world", &target);
			HC_Set_Camera_Target(target.x, target.y, target.z);
		}
    HC_Close_Segment();
}


bool HUtility::GetAllowAnimation(HC_KEY key)
{
    HUserOptions temp;
    bool ret = false;
    HC_Open_Segment_By_Key (key);
	temp.Show_One_Net_User_Option("hnoani");
	if (strcmp(temp.m_data,"") == 0)
	   ret = true;
	HC_Close_Segment();
    return ret; 	
}

void HUtility::SuppressAnimation()
{
    HC_Set_User_Options("hnoani");
     
}

bool HUtility::ShowOneUserOption(const char *option, char *res)
{
	bool retval = false;

	if (res)
		strcpy(res, "");

	char * user_options_exist = new char[strlen("user options = ") + strlen(option) +1];
	sprintf(user_options_exist, "user options = %s", option);
	
    if (HC_Show_Existence(user_options_exist)) {

		retval =  true;

		if (res)
			HC_Show_One_User_Option(option, res);
		
	}
	
	delete [] user_options_exist;
	
    return retval;
}


int  HUtility::FindFaceByIndex(int findex, int *flist, int flen)
{
    int pos = 0;
    for (int i=0;i<findex;i++)
    {
	if (pos>=flen)
	    return -1;
	pos+=flist[pos] + 1;
    }
    return pos;
}

  

HC_KEY HUtility::GetNextRenumberedKey(HBaseView *view)
{
	HC_KEY minkey = 1;
	HC_KEY fkey = 0;
 	HC_Open_Segment_By_Key(view->GetModelKey());
	HC_Begin_Contents_Search("...", "segments");
	char stype[MVO_BUFFER_SIZE];
	while (HC_Find_Contents(stype, &fkey))
	{
		char result[MVO_BUFFER_SIZE];
 		HC_Show_Key_Status(fkey, result);
		if (strcmp(result, "global") == 0)
		{
			if (fkey>=minkey)
				minkey = fkey + 1;
		}
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	return minkey;
}


void HUtility::NameSegments(HC_KEY &keyn, HBaseView *view)
{
 	HC_KEY key = keyn;
	bool first = true;
	while (1)
	{
		char segment[MVO_BUFFER_SIZE];
		char result[MVO_BUFFER_SIZE];
 		HC_Show_Key_Status(key, result);
		if (strcmp(result, "global") != 0)
		{
			HC_Show_Segment(key, segment);
			HC_Parse_String(segment, "/", -1, result);
			if (result[0] == '@')
			{
				if (first)
				{
 					keyn = HC_KRenumber_Key(key, GetNextRenumberedKey(view), "global");
				}
				else
					HC_Renumber_Key(key, GetNextRenumberedKey(view), "global");
			}
		}
 
		key = HC_KShow_Owner_By_Key(key);
		if (key == INVALID_KEY)
			break;
		first = false;
	}
}

void HUtility::MoveToTemp(HC_KEY movekey, HC_KEY tempkey)
{

    char color[MVO_BUFFER_SIZE];
    char segmentname[MVO_BUFFER_SIZE];
    float matrix[16];
    float identmatrix[16];

    HC_Open_Segment_By_Key(movekey);
    HObjectManager::SetHObjectSegment("entity");
    HC_Close_Segment();


	
    HC_KEY ownerkey = HC_KShow_Owner_By_Key(movekey);
    HC_Open_Segment_By_Key(ownerkey);
    HC_Show_Segment(movekey, segmentname);
    HC_Show_Net_Color(color);
//    HC_Show_Net_Visibility(visibility);
    HC_Show_Net_Modelling_Matrix(matrix);
    HC_Close_Segment();

    HC_Open_Segment_By_Key(tempkey);
    HObjectManager::SetHObjectSegment("combined");

	HC_Compute_Identity_Matrix(identmatrix); 
	HC_Set_Modelling_Matrix(identmatrix);
	HC_Open_Segment("tempdata");
	HC_Open_Segment("");
    HC_Set_User_Index(H_TEMP_SEGMENT_MAGIC, (void *)ownerkey);
//    HC_Set_Visibility(visibility);
    HC_Set_Color(color);
    HC_Set_Modelling_Matrix(matrix);
    HC_Open_Segment("temp");
    HC_Move_Segment(segmentname, "./*");
    HC_Close_Segment();
    HC_Close_Segment();
    HC_Close_Segment();
    HC_Close_Segment();    
}


void HUtility::ShowContentsWithPathAddEntity(HC_KEY key, struct vlist_s *pathlist, int level, HC_KEY *patharray, bool storePath)
{
    
    HShowContentsWithPathType *pathcontainer = new HShowContentsWithPathType;
    if (storePath)
    {    
	pathcontainer->patharray = new HC_KEY[level + 1];
	pathcontainer->len = level + 1;   
	for (int i=0;i<level;i++)
	    pathcontainer->patharray[i] = patharray[i];
        pathcontainer->patharray[level] = key;

    }
    else
    {    
	pathcontainer->patharray = new HC_KEY[1];
	pathcontainer->len = 1;
	pathcontainer->patharray[0] = key;  
    }
     vlist_add_last(pathlist, (void *)pathcontainer);
}

void HUtility::ShowContentsWithPathRecursive(HC_KEY key, const char *entitytypes, struct vlist_s *pathlist, HC_KEY *patharray, int level, bool storePath, bool includeIncludes, bool filterIncludes)
{
    int count;
    int i;
    char keytype[MVO_BUFFER_SIZE];
    int origlevel = level;
    HC_Show_Key_Type(key, keytype);
    if (level > MVO_BUFFER_SIZE-2)		    //we allow for 4096 levels of recursion which should be plenty
		return;
    if (strcmp(keytype, "segment") != 0)
    {
        if (!filterIncludes)
		{	
			patharray[level] = key;
			level++;
		}
		char includee[MVO_BUFFER_SIZE];
		HC_Show_Include_Segment(key, includee);
		key = HC_KCreate_Segment(includee);
		
    }
    HC_Open_Segment_By_Key(key);
    patharray[level] = key;
    level++;
    HC_Begin_Contents_Search(".", entitytypes);
    HC_Show_Contents_Count(&count);
    for (i=0;i<count;i++)
    {
		static char type[128];
		HC_KEY ckey;
		HC_Find_Contents(type, &ckey);
		while(streq(type, "reference"))
		{
			ckey=HC_KShow_Reference_Geometry(ckey);
			HC_Show_Key_Type(ckey, type);
		}		
		ShowContentsWithPathAddEntity(ckey, pathlist, level, patharray, storePath);
    }
    HC_End_Contents_Search();
    if (includeIncludes)
		HC_Begin_Contents_Search(".", "segments, includes");
    else
		HC_Begin_Contents_Search(".", "segments");
	
    HC_Show_Contents_Count(&count);
    for (i=0;i<count;i++)
    {
		HC_KEY ckey;
		HUtility::FindContents(ckey);
		ShowContentsWithPathRecursive(ckey, entitytypes, pathlist, patharray, level, storePath, includeIncludes, filterIncludes);	
    }
    HC_End_Contents_Search();
    HC_Close_Segment();
    level = origlevel;    	
}


void HUtility::ShowContentsWithPath(const char * entitytypes, struct vlist_s **ret_pathlist, bool storePath, bool includeIncludes, bool filterIncludes)
{
    
    HC_KEY patharray[MVO_BUFFER_SIZE];
    HC_KEY key = HC_KCreate_Segment(".");
    struct vlist_s * pathlist = new_vlist(malloc, free);
    ShowContentsWithPathRecursive(key, entitytypes, pathlist, patharray, 0, storePath, includeIncludes, filterIncludes);	
    *ret_pathlist = pathlist;          
}

int HUtility::ustrlen(unsigned short *text)
{
    int len = 0;
    while (text[len]!=0)
	len++;
	
    return len;

}

unsigned short *HUtility::uconvert(const char *text)
{
    unsigned short *utext = new unsigned short[strlen(text)+1];
    for (unsigned int i=0;i<=strlen(text);i++)
	utext[i] = text[i];
    return utext;
}


bool HUtility::BuildIncludePath(HC_KEY objectkey, const HC_KEY *includelist, int ilength, HC_KEY *fullpath, int *fplength, HC_KEY matchkey)
{
    if (fplength)
	(*fplength) = 0;
    int i = 0;
    HC_KEY includesegment;
	char type[MVO_BUFFER_SIZE];
    
    if (ilength)
	{
		HC_Show_Key_Type(includelist[i], type);

		if (strstr(type, "include"))
			includesegment = HC_KShow_Include_Segment(includelist[i]);
		else 
			includesegment = includelist[i];
	}
	
    else
	includesegment = INVALID_KEY;
     HC_KEY currentkey = objectkey;
    if (fullpath)
	fullpath[(*fplength)++] = objectkey;
	 

    while (1)
    {
	if (matchkey == currentkey)
	    return true;
 
 	    
	if (currentkey == includesegment)
	{
 
	    currentkey = HC_KShow_Owner_Original_Key(includelist[i]); 
	    i++;
 	    if (i<ilength)
		{
			HC_Show_Key_Type(includelist[i], type);

			if (strstr(type, "include"))
				includesegment = HC_KShow_Include_Segment(includelist[i]);
			else 
				includesegment = includelist[i];
		}
	}
	else if (!HC_Show_Existence_By_Key(currentkey, "self"))
	{
		currentkey=INVALID_KEY;
		break;
	}
	else
	    currentkey = HC_KShow_Owner_Original_Key(currentkey);

  
	if (currentkey == INVALID_KEY || currentkey == 0)
	    break;
	if (fullpath)
	    fullpath[(*fplength)++] = currentkey;
    }
    return false;

}



void HUtility::BuildIncludePathFromSelection(HSmartSelItem *sitem, int &pathlen, HC_KEY *fullpath)
{
 	HC_KEY *keys = new HC_KEY[sitem->GetIncludeCount()];
	const HC_KEY *oldkeys = sitem->GetIncludeKeys();
    for (int i=0;i<sitem->GetIncludeCount();i++)
 	    keys[i] = oldkeys[sitem->GetIncludeCount()-1-i];
	
 	HUtility::BuildIncludePath(sitem->GetKey(), keys,sitem->GetIncludeCount(),fullpath,&pathlen,-1);	
	delete [] keys;
}

int HUtility::ConstrainToVector(const float *matrix_in,
										   const HVector &vector,
										   float *matrix_out) 
{
	float normalized_vector[3];
	float dot;
	const float *translation_component = &matrix_in[12];

	if (matrix_out != matrix_in)
		memcpy(matrix_out, matrix_in, 16 * sizeof(float));
	HC_Compute_Normalized_Vector((float *)&vector.x, normalized_vector);
	dot = (float) HC_Compute_Dot_Product(normalized_vector, translation_component);
	matrix_out[12] = normalized_vector[0] * dot;
	matrix_out[13] = normalized_vector[1] * dot;
	matrix_out[14] = normalized_vector[2] * dot;
	return HOP_OK;
}

void HUtility::MakeViewSnapshot(HBaseView * view, int width, int height, char ** data)
{
	char hsra_options[MVO_BUFFER_SIZE];
	char hlr_options[MVO_BUFFER_SIZE];
	char current_hsra[MVO_BUFFER_SIZE];

	/* first find out the relevant options associated with the view */
	HC_Open_Segment_By_Key(view->GetViewKey());
	{
		HC_Show_One_Net_Rendering_Optio("hidden surface removal algorithm", current_hsra);

		HRenderMode rndrmode = view->GetRenderMode();
		if (rndrmode == HRenderHiddenLine || rndrmode == HRenderHiddenLineFast || rndrmode == HRenderBRepHiddenLine)
		{
			HC_Show_One_Net_Rendering_Optio("hidden line removal options", hlr_options);
			sprintf(hsra_options, "hsra = hidden line, hidden line removal options = (%s)", hlr_options);
		}
		else
		{
			sprintf(hsra_options, "hsra = szb, technology = software frame buffer");
		}
	}
	HC_Close_Segment();
	char image_segment[MVO_SEGMENT_PATHNAME_BUFFER];
	HBestImageDriver driver_segment(view->GetDriverType());

	sprintf(image_segment,"?driver/null/hbaseview_%p", (void*)view);

	// prepare image for rendering
	HPixelRGB *image = 0;
	HC_KEY image_key = INVALID_KEY;

	HC_Open_Segment(image_segment);
	{
		image = new HPixelRGB[width * height];
		image_key = HC_KInsert_Image (0.0, 0.0, 0.0, "rgb", width, height, image);
	}
	HC_Close_Segment ();

	// prepare data to render
	HC_Open_Segment(driver_segment);
	{
		//HC_Set_Rendering_Options("attribute lock = (color = (window))");
		//HC_Set_Color_By_Value("windows", "RGB", m_WindowColor.x, m_WindowColor.y, m_WindowColor.z);
		HC_Set_Window_Frame("off");
		HC_Set_Rendering_Options(hsra_options);

		char buffer[MVO_BUFFER_SIZE];
		sprintf(buffer, "use window id = (image key = %s%p), subscreen = (-1, 1, -1, 1)", H_EXTRA_POINTER_FORMAT, (void*)image_key);
		HC_Set_Driver_Options(buffer);
		HC_Set_Driver_Options("isolated, no gamma correction");

		char window_material[512] = "";
		char window_pattern[128] = "";
		view->GetWindowMaterial(window_material, window_pattern);
		HC_Set_Color(window_material);
		HC_Set_Window_Pattern(window_pattern);

		HC_Include_Segment_By_Key (view->GetSceneKey());
	}
	HC_Close_Segment ();

	HC_Update_One_Display(driver_segment);

	float dummy;
	int dummy_int;
	char dummy_text[4096];
	char *imagedata = new char[width*height*3];
	HC_Show_Image(image_key, &dummy, &dummy, &dummy, dummy_text, &dummy_int, &dummy_int, imagedata);
	*data = imagedata;
	delete [] image;
	HC_Delete_Segment(driver_segment);
	HC_Delete_Segment(image_segment);
}

void HUtility::MakeViewSnapshot(HBaseView * view, HC_KEY image_key)
{    
    /* first find out the relevant options associated with the view */
	H_FORMAT_TEXT rendering_options;    
	HC_Open_Segment_By_Key(view->GetViewKey());{

		char transparency_options[4096] = {""};
		HC_Show_One_Net_Rendering_Optio("transparency options",transparency_options);
		rendering_options.Append("transparency options = (%s)",transparency_options);

	    HRenderMode rndrmode = view->GetRenderMode();
		if (rndrmode == HRenderHiddenLine ||
			rndrmode == HRenderHiddenLineFast ||
			rndrmode == HRenderBRepHiddenLine ||
			rndrmode == HRenderBRepHiddenLineFast)
		{
			char hlr_options[4096] = {""};
			HC_Show_One_Net_Rendering_Option("hidden line removal options", hlr_options);
			rendering_options.Append(", hsra = hidden line, hidden line removal options = (%s)", hlr_options);
		}
	}HC_Close_Segment();
    
    // prepare data to render
	HBestImageDriver dvrseg(view->GetDriverType());
	HC_Open_Segment(dvrseg);{

		HC_Set_Window_Frame("off");
		HC_Set_Rendering_Options(rendering_options);
    
		HC_Set_Driver_Options( H_FORMAT_TEXT(
											"isolated,"
											"no gamma correction,"
											"use window id = (image key = %s),"
											"subscreen = (-1, 1, -1, 1)",
											kto0xa(image_key)));
		char window_material[512] = "";
		char window_pattern[128] = "";
		view->GetWindowMaterial(window_material, window_pattern);
		HC_Set_Color(window_material);
		HC_Set_Window_Pattern(window_pattern);
    
	    HC_Include_Segment_By_Key(view->GetSceneKey());
		HC_Update_One_Display(".");
	}HC_Close_Segment();

	HC_Delete_Segment(dvrseg);
}


void HUtility::LocalPixelsToWorld(HCamera *camera, bool righthanded, float xres, float yres, int xwpix, int ywpix, float *result)
{
	float projection_matrix[16];
	float position_matrix[16];
	float complete_matrix[16];
	float matrix[16];
	
	
	HC_Compute_Identity_Matrix(result);
	float xfactor, yfactor;
	Camera_To_Transform(camera, position_matrix, projection_matrix, complete_matrix);
	
	float window_aspect = (float)ywpix / (float)xwpix	 *	xres / yres;
	
	float camera_aspect = camera->field_width/camera->field_height;
	//	float window_aspect = (float)xres/(float)yres;
	
	xfactor = 2.0f / (xwpix-1);
	yfactor = -2.0f / (ywpix-1);
	result[0] *= xfactor;
	result[4] *= xfactor;
	result[8] *= xfactor;
	result[12] *= xfactor;
	
	result[1] *= yfactor;
	result[5] *= yfactor;
	result[9] *= yfactor;
	result[13] *= yfactor;
	
	result[0] -= result[3];
	result[1] += result[3];
	result[4] -= result[7];
	result[5] += result[7];
	result[8] -= result[11];
	result[9] += result[11];
	result[12] -= result[15];
	result[13] += result[15];
	
	xfactor = yfactor = 1.0f;
	
	if (camera_aspect < window_aspect)
		yfactor *= camera_aspect / window_aspect;
	else
		xfactor *= window_aspect / camera_aspect;			
	
	if (righthanded) xfactor = -xfactor;
	
	xfactor = 1.0f / xfactor;
	yfactor = 1.0f / yfactor;
	
	/* scale */
	result[0] *= xfactor;
	result[4] *= xfactor;
	result[8] *= xfactor;
	result[12] *= xfactor;
	
	result[1] *= yfactor;
	result[5] *= yfactor;
	result[9] *= yfactor;
	result[13] *= yfactor;
	
	
	
	HC_Compute_Matrix_Inverse(projection_matrix, matrix);
	HC_Compute_Matrix_Product (result, matrix, result);
	
	HPoint eye, crosseye;
	eye.Set(camera->target.x - camera->position.x, camera->target.y - camera->position.y, 
		camera->target.z - camera->position.z);
	float eyedistance = HC_Compute_Vector_Length(&eye);
	
	result[10] += eyedistance * result[11];
	result[14] += eyedistance;
	
	result[10] -= eyedistance * result[11];
	result[14] -= eyedistance;
	
	HC_Compute_Matrix_Inverse (position_matrix, matrix);
	
	HC_Compute_Matrix_Product (result, matrix, result);
				
}

float HUtility::ComputeAngleBetweenVector(HPoint p1, HPoint p2)
{
	HPoint p3;
    HC_Compute_Normalized_Vector(&p1,&p1);
    HC_Compute_Normalized_Vector(&p2,&p2);
    HC_Compute_Cross_Product(&p1,&p2,&p3);
	
    float dprod = (float)HC_Compute_Dot_Product(&p1,&p2);
    float l=(float)HC_Compute_Vector_Length(&p3);
	float   ang = (float)atan2(l,dprod);
    ang*=(180.0f/(float)3.1415927f);
	return ang;
	
} 

void HUtility::CalculateBoundingInfo(HC_KEY startkey, HPoint &min, HPoint &max, HPoint &delta, HPoint &mid)
{								

	HC_Open_Segment_By_Key(startkey);
	HC_Compute_Circumcuboid(".",&min, &max);
	HC_Close_Segment();
 	if (min.x == 0 && min.y == 0 && min.z == 0 && max.x == 0 && max.y ==0 && max.z ==0)
	{
		min.x = min.y = min.z = -0.1f;
		max.x = max.y = max.z = 0.1f;
	}
  	mid.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	delta.Set(min.x - mid.x, min.y - mid.y, min.z - mid.z);
	min.Set(mid.x + delta.x, mid.y + delta.y, mid.z + delta.z);
	delta.Set(max.x - mid.x, max.y - mid.y, max.z - mid.z);
	max.Set(mid.x + delta.x, mid.y + delta.y, mid.z + delta.z);
 	mid.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);

}

void HUtility::Camera_To_Transform (HCamera	*camera , float *pos, float *projection_matrix, float *complete_matrix)
{	

 
 

//	camera->positioning_matrix.flags = false;

//	pos = (HT_Row_Vector *)camera->positioning_matrix.elements;
	HPoint eye, crosseye;
	eye.Set(camera->target.x - camera->position.x, camera->target.y - camera->position.y, 
		camera->target.z - camera->position.z);
 	float eyedistance = HC_Compute_Vector_Length(&eye);

	HC_Compute_Cross_Product(&camera->up_vector, &eye, &crosseye);
	HC_Compute_Normalized_Vector(&crosseye, &crosseye);
	HC_Compute_Normalized_Vector(&eye, &eye);

 

	pos[0] = crosseye.x;
	pos[4] = crosseye.y;
	pos[8] = crosseye.z;

	pos[1] = camera->up_vector.x;
	pos[5] = camera->up_vector.y;
	pos[9] = camera->up_vector.z;

	pos[2] = eye.x;
	pos[6] = eye.y;
	pos[10] = eye.z;

	pos[3] = 0.0f;
	pos[7] = 0.0f;
	pos[11] = 0.0f;
	pos[15] = 1.0f;


	/*
	 * Translate the target to the origin, if needed.
	 * Note that this will destroy orthogonality.
	 */

	if (camera->target.x == 0.0f &&
		camera->target.y == 0.0f &&
		camera->target.z == 0.0f) {
		pos[12] = 0.0f;
		pos[13] = 0.0f;
		pos[14] = 0.0f;
	}
	else {
		pos[12] = -(pos[0] * camera->target.x +
					  pos[4] * camera->target.y +
					  pos[8] * camera->target.z);
		pos[13] = -(pos[1] * camera->target.x +
					  pos[5] * camera->target.y +
					  pos[9] * camera->target.z);
		pos[14] = -(pos[2] * camera->target.x +
					  pos[6] * camera->target.y +
					  pos[10] * camera->target.z);

//		camera->positioning_matrix.contents |= Matrix_Contents_TRANSLATION;
	}





	/*
	 * Compute the projection portion of the camera
	 */
	HC_Compute_Scale_Matrix(2.0f / camera->field_width,
							2.0f / camera->field_height,
							1.0f / eyedistance,
							projection_matrix);

	if (strcmp(camera->projection, "perspective") == 0)
	{	
 		projection_matrix[11] = projection_matrix[10];
	}


	/*
	 * Put it all together
	 */

	HC_Compute_Matrix_Product(pos, projection_matrix, complete_matrix);
  

		 
}


int HUtility::strlen16(const unsigned short *p16)
{
	int count= -1;

	while(*(p16+(++count)) != 0) { }

	return count;
}



long HUtility::ClipPolygonAgainstPlane(long vertexCount, const HPoint *vertex,
		const float *plane, HPoint *result, bool *NoCut)
{
	HClipPolygonType location[256];
	long positive = 0;
	long negative = 0;
	if (NoCut)
		*NoCut = false;
	
	for (long a = 0; a < vertexCount; a++)
	{
		float d = plane[0] * vertex[a].x + plane[1] * vertex[a].y + 
			plane[2] * vertex[a].z + plane[3];

		if (d > boundaryEpsilon)
		{
			location[a] = polygonInterior;
			positive++;
		}
		else
		{
			if (d < -boundaryEpsilon)
			{
				location[a] = polygonExterior;
				negative++;
			}
			else
			{
				location[a] = polygonBoundary;
			}
		}
	}
	
	if (negative == 0)
	{
		if (NoCut)
			*NoCut = true;
 		for (long a = 0; a < vertexCount; a++) result[a] = vertex[a];
		return vertexCount;
	}
	else if (positive == 0)
	{
		return 0;
	}
	
	long count = 0;
	long previous = vertexCount - 1;
	for (long index = 0; index < vertexCount; index++)
	{
		long loc = location[index];
		if (loc == polygonExterior)
		{
			if (location[previous] == polygonInterior)
			{
				const HPoint& v1 = vertex[previous];
				const HPoint& v2 = vertex[index];
				HPoint dv(v2.x -v1.x, v2.y - v1.y, v2.z - v1.z);
		 		
				float t = (plane[0] * v2.x + plane[1] * v2.y + 
			plane[2] *v2.z + plane[3]) / (plane[0] * dv.x + plane[1] * dv.y + 
			plane[2] *dv.z);
				result[count].x = v2.x - dv.x * t;
				result[count].y = v2.y - dv.y * t;
				result[count].z = v2.z - dv.z * t;
				count++;
			}
		}
		else
		{
			const HPoint& v1 = vertex[index];
			if ((loc == polygonInterior) && (location[previous] == polygonExterior))
			{
				const HPoint& v2 = vertex[previous];
				HPoint dv(v2.x -v1.x, v2.y - v1.y, v2.z - v1.z);
				
				float t = (plane[0] * v2.x + plane[1] * v2.y + 
			plane[2] *v2.z + plane[3]) / (plane[0] * dv.x + plane[1] * dv.y + 
			plane[2] *dv.z);
				result[count].x = v2.x - dv.x * t;
				result[count].y = v2.y - dv.y * t;
				result[count].z = v2.z - dv.z * t;
				count++;
			}
			
			result[count++] = v1;
		}
		
		previous = index;
	}
	
	return count;
}



HC_KEY HUtility::OpenSegmentFromKey(HC_KEY key, char *segment)
{
	HC_Open_Segment_By_Key(key);
	HC_KEY rkey = HC_KCreate_Segment(segment);
	HC_Close_Segment();
	HC_Open_Segment_By_Key(rkey);
	return rkey;

}



void HUtility::MergePolylines()
{
	HC_KEY linekey;
	int linecount;
	HPoint **lines;
	int *length;
	int i,j,k;
	bool found;
	HC_Begin_Contents_Search(".", "polylines");
	HC_Show_Contents_Count(&linecount);
	lines = new HPoint*[linecount];
	length = new int[linecount];
	for (i=0;i<linecount;i++)
	{
		int len;
		HUtility::FindContents(linekey);
		HC_Show_Polyline_Count(linekey, &len);
		lines[i] = new HPoint[len];
		length[i] = len;
		HC_Show_Polyline(linekey, &len, lines[i]);
	}
	
	
	for (i=0;i<linecount;i++)
	{
	
restartMerge:
	found = false;
	if (lines[i])
	{			
	
		for (j=i+1;j<linecount;j++)
		{
			
			if (lines[j])
			{			
							
				int l1,l2;
				l1 = length[i];
				l2 = length[j];
				HPoint *newp =  0;
				if (lines[i][0].Equal(lines[j][0],0.001f))
				{			
					newp = new HPoint[l1+l2-1];
					for (k=0;k<l2;k++)				
						newp[k] = lines[j][l2-k-1];
					for (k=0;k<l1;k++)				
						newp[k+l2-1] = lines[i][k];
					found = true;
					
				}
				else if (lines[i][0].Equal(lines[j][l2-1], 0.001f))
				{			
					newp = new HPoint[l1+l2-1];
					for (k=0;k<l2;k++)				
						newp[k] = lines[j][k];
					for (k=0;k<l1;k++)				
						newp[k+l2-1] = lines[i][k];
					found = true;
					
				}
				else if (lines[i][l1-1].Equal(lines[j][0], 0.001f))
				{			
					newp = new HPoint[l1+l2-1];
					for (k=0;k<l1;k++)				
						newp[k] = lines[i][k];
					for (k=0;k<l2;k++)				
						newp[k+l1-1] = lines[j][k];
					found = true;
					
				}
				else if (lines[i][l1-1].Equal(lines[j][l2-1], 0.001f))
				{			
					newp = new HPoint[l1+l2-1];
					for (k=0;k<l1;k++)				
						newp[k] = lines[i][k];
					for (k=0;k<l2;k++)				
						newp[k+l1-1] = lines[j][l2-k-1];
					found = true;
					
				}
				if (found)
				{
					delete [] lines[i];
					delete [] lines[j];
					lines[i] = newp;
					length[i] = l1 + l2 -1;
					lines[j] = 0;							
					goto restartMerge;
				}
			}	
		}
	}
	}

	HC_Flush_Contents(".", "polylines");
	for (i=0;i<linecount;i++)
	{
		if (lines[i])
		{
			HC_Insert_Polyline(length[i], lines[i]);
			HC_Insert_Marker(lines[i][0].x, lines[i][0].y, lines[i][0].z);
			HC_Insert_Marker(lines[i][length[i]-1].x, lines[i][length[i]-1].y,lines[i][length[i]-1].z);
			delete [] lines[i];
		}
		
	}
	delete [] lines;
	delete [] length;
	
}

template <typename T>
static T CLAMP(T v, T lower, T upper)
{
	if(v < lower)
		return lower;
	if(v > upper)
		return upper;
	return v;
}

#if !TARGET_OS_IPHONE && !TARGET_OS_ANDROID

#include <hardcopy.h>

using namespace hardcopy;

class MVOHardCopy : public HardCopy
{
private:
	bool m_GrayScale;
	double m_fWindowColorBottom[3];
	double m_fWindowColorTop[3];
	double m_PageMargins[4];
	double m_ratio_cm_to_internal_units;
	HBaseView * m_pView;

	bool m_cleanup_adjust_subscreen;
	char m_cleanup_scene_projection[128];
	float m_cleanup_scene_camera_height, m_cleanup_scene_camera_width;

	HCamera m_cleanup_camera;

	bool m_cleanup_gooch;
	char m_cleanup_gooch_options[2048];

	void AdjustAnnotations();

protected:
	void AdjustSubscreen();
	void AdjustCamera();
	void Copy(HC_KEY src);
	void DriverSetup();
	void AdjustWindow(double x0, double x1, double y0, double y1) const;

public:
	MVOHardCopy(HBaseView * view, double w, double h, double dpi, bool wysiwyg, double plot_to_scale_ratio, bool exporting_2d_dwf);

	~MVOHardCopy();

	bool Output(HC_KEY driver);
	void BackgroundOptions();
	void SetGrayScale(bool b);

	void SetWindowColorTop(double r, double g, double b);
	void SetWindowColorBottom(double r, double g, double b);
	void SetPageMargins(double x0, double x1, double y0, double y1);

};

MVOHardCopy::MVOHardCopy(HBaseView * view, double w, double h, double dpi, bool wysiwyg, double plot_to_scale_ratio, bool exporting_2d_dwf)
	: HardCopy(w, h, dpi, wysiwyg, exporting_2d_dwf)
	, m_GrayScale(false)
	, m_ratio_cm_to_internal_units(plot_to_scale_ratio)
	, m_pView(view)
{
	for (int i = 0; i < 3; ++i) {
		m_fWindowColorBottom[i] = 1.0;
		m_fWindowColorTop[i] = 1.0;
	}

	for (int i = 0; i < 4; ++i) {
		m_PageMargins[i] = 0.0;
	}

	m_cleanup_adjust_subscreen = false;
	m_cleanup_gooch = false;
}

MVOHardCopy::~MVOHardCopy()
{
	//restore any font that was changed to its original size
	HardCopy::ResetFonts();

	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	//cleanup gooch options
	if (m_cleanup_gooch)
		HC_Set_Rendering_Options(H_FORMAT_TEXT("gooch options = (%s)", m_cleanup_gooch_options));
	else
		HC_UnSet_One_Rendering_Option("gooch options");

	//cleanup adjust subscreen
	if (m_cleanup_adjust_subscreen)
	{
		HC_Set_Camera_Projection(m_cleanup_scene_projection);
		HC_Set_Camera_Field(m_cleanup_scene_camera_width, m_cleanup_scene_camera_height);
	}
	
	//cleanup adjust camera
	if (m_ratio_cm_to_internal_units <= 0.0 && HC_Show_Existence("camera"))
		HC_Set_Camera_Field(m_cleanup_camera.field_width, m_cleanup_camera.field_height);
	HC_Close_Segment();

	//return annotations to their correct size
	if (m_pView)
	{
		HUtilityAnnotation::WindowResizeCallback (0, 0, m_pView);
		m_pView->Update();
	}
}

void MVOHardCopy::SetGrayScale(bool b)
{
	m_GrayScale = b;
}

void MVOHardCopy::Copy(HC_KEY src)
{
	HardCopy::Copy(src);

	HC_Open_Segment_By_Key(m_pView->GetSceneKey());
	if (HC_Show_Existence("rendering options = gooch options"))
	{
		HC_Show_One_Rendering_Option("gooch options", m_cleanup_gooch_options);
		m_cleanup_gooch = true;
	}
	HC_Set_Rendering_Options("gooch options = no color map segment");
	HC_Close_Segment();
	
	if (m_ratio_cm_to_internal_units <= 0.0)
		AdjustCamera();
}

void MVOHardCopy::AdjustAnnotations()
{
	HUtilityAnnotation::recursive_resize(m_MyScene, 1, &m_MySceneInclude, HC_Create_Segment("."));
}

void MVOHardCopy::AdjustSubscreen()
{
	if (m_ratio_cm_to_internal_units <= 0.0) {
		HardCopy::AdjustSubscreen();

		//Adjust page subscreen, for page margins
		double screen_left_offset = (2.0*m_PageMargins[0])/m_DestWidth;
		double screen_right_offset = (2.0*m_PageMargins[1])/m_DestWidth;
		double screen_bottom_offset = (2.0*m_PageMargins[2])/m_DestHeight;
		double screen_top_offset = (2.0*m_PageMargins[3])/m_DestHeight;

		screen_left_offset = CLAMP(screen_left_offset, 0.0, 2.0);
		screen_right_offset = CLAMP(screen_right_offset, 0.0, 2.0);
		screen_bottom_offset = CLAMP(screen_bottom_offset, 0.0, 2.0);
		screen_top_offset = CLAMP(screen_top_offset, 0.0, 2.0);

		if(screen_left_offset + screen_right_offset >= 2)
			screen_left_offset = screen_right_offset = 0;
		if(screen_bottom_offset + screen_top_offset >= 2)
			screen_bottom_offset = screen_top_offset = 0;

		if(fabs(-1.0 - m_subscreen.x0) < screen_left_offset)
			m_subscreen.x0 = -1.0f + screen_left_offset;
		if(fabs(1.0 - m_subscreen.x1) < screen_right_offset)
			m_subscreen.x1 = 1.0f - screen_right_offset;
		if(fabs(-1.0 - m_subscreen.y0) < screen_bottom_offset)
			m_subscreen.y0 = -1.0f + screen_bottom_offset;
		if(fabs(1.0 - m_subscreen.y1) < screen_top_offset)
			m_subscreen.y1 = 1.0f - screen_top_offset;
	}
	else {
		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
			//Print to Scale
			//ignore window information except for camera position and target
			//print based on a user-provided scale and print exactly to scale on the paper, 
			//including whatever fits on the paper, excluding the rest of the scene

			double page_ar = (double)m_DestWidth/(double)m_DestHeight;

			HC_Show_Camera_Projection(m_cleanup_scene_projection);
			HC_Show_Camera_Field(&m_cleanup_scene_camera_width, &m_cleanup_scene_camera_height);

			if (page_ar > 1.0) {
				HC_Set_Camera_Field(m_DestHeight / m_ratio_cm_to_internal_units*2.54, m_DestHeight / m_ratio_cm_to_internal_units*2.54);
			}
			else {
				HC_Set_Camera_Field(m_DestWidth / m_ratio_cm_to_internal_units*2.54, m_DestWidth / m_ratio_cm_to_internal_units*2.54);
			}
			HC_Set_Camera_Projection("ortho");

			m_cleanup_adjust_subscreen = true;
		HC_Close_Segment();
	}
}

void MVOHardCopy::SetPageMargins(double x0, double x1, double y0, double y1)
{
	m_PageMargins[0] = x0;
	m_PageMargins[1] = x1;
	m_PageMargins[2] = y0;
	m_PageMargins[3] = y1;
}

void MVOHardCopy::AdjustCamera()
{
	HC_Open_Segment_By_Key(m_pView->GetSceneKey());{
		if (HC_Show_Existence ("camera")) {
			HCamera camera;

			HC_Show_Camera(
				&camera.position,
				&camera.target,
				&camera.up_vector,
				&camera.field_width,
				&camera.field_height,
				camera.projection
				);

			m_cleanup_camera.field_width = camera.field_width;
			m_cleanup_camera.field_height = camera.field_height;

			double camera_ratio = camera.field_width/camera.field_height;

			if (m_PixelRatio > camera_ratio)
				camera.field_width *= m_PixelRatio/camera_ratio;
			else 
				camera.field_height *= camera_ratio/m_PixelRatio;

			HC_Set_Camera(
				&camera.position,
				&camera.target,
				&camera.up_vector,
				camera.field_width,
				camera.field_height,
				camera.projection
				);
		}
	}HC_Close_Segment();
}


void MVOHardCopy::DriverSetup()
{
	HardCopy::DriverSetup();

	if (m_GrayScale)
		HC_Set_Driver_Options("force grayscale");

	AdjustAnnotations();
}

void MVOHardCopy::BackgroundOptions()
{
	HardCopy::BackgroundOptions();
	
	if (m_GrayScale) {
		HC_Set_Driver_Options("force grayscale");
	}
}

bool MVOHardCopy::Output(HC_KEY driver)
{
	HC_Open_Segment_By_Key(driver);{
		OutputPage();
	}HC_Close_Segment();

	return true;
}

void MVOHardCopy::AdjustWindow(double x0, double x1, double y0, double y1) const
{
	UNREFERENCED(x0);
	UNREFERENCED(x1);

	HVector WindowColorTop(
			m_fWindowColorTop[0],
			m_fWindowColorTop[1],
			m_fWindowColorTop[2]);
		
	HVector WindowColorBottom(
			m_fWindowColorBottom[0],
			m_fWindowColorBottom[1],
			m_fWindowColorBottom[2]);
		
	HVector const dist = WindowColorTop - WindowColorBottom;
	HVector const top = dist * (float)(y1 + 1) / 2.0 + WindowColorBottom;
	HVector const bot = dist * (float)(y0 + 1) / 2.0 + WindowColorBottom;

	H_FORMAT_TEXT color("window = (r=%f g=%f b=%f), window contrast = (r=%f g=%f b=%f)", 
				top.x, top.y, top.z, bot.x, bot.y, bot.z);

	HC_Set_Color(color);
	HC_Set_Window_Pattern("Down");
}

void MVOHardCopy::SetWindowColorTop(double r, double g, double b)
{
	m_fWindowColorTop[0] = r;
	m_fWindowColorTop[1] = g;
	m_fWindowColorTop[2] = b;
}

void MVOHardCopy::SetWindowColorBottom(double r, double g, double b)
{
	m_fWindowColorBottom[0] = r;
	m_fWindowColorBottom[1] = g;
	m_fWindowColorBottom[2] = b;
}

HFileOutputResult HUtility::TwoPassPrint(const char *driver,
										 HOutputHandlerOptions *options)
{
	MVOHardCopy hc(
		options->m_pHBaseView,
		options->PaperWidth() <= 0 ? options->WindowWidth() : options->PaperWidth(),
		options->PaperHeight() <= 0 ? options->WindowHeight() : options->PaperHeight(),
		options->m_Paper_Dpi,
		options->UseSubscreen(),
		options->m_ratio_cm_to_internal_units,
		options->m_bExporting2DDwf
	);

	hc.SetDriverHint(options->View()->GetDriverType());
	hc.SetGrayScale(options->ForceGrayscale());
	if (options->UseWindowColor()) {
		HPoint WindowColorTop, WindowColorBottom;
		options->View()->GetWindowColor(WindowColorTop, WindowColorBottom);
		hc.SetWindowColorTop(WindowColorTop.x, WindowColorTop.y, WindowColorTop.z);
		hc.SetWindowColorBottom(WindowColorBottom.x, WindowColorBottom.y, WindowColorBottom.z);
	}
	hc.SetPageMargins(
		options->m_PageMargins[0],
		options->m_PageMargins[1],
		options->m_PageMargins[2],
		options->m_PageMargins[3]
	);
	hc.Input(options->View()->GetViewKey());
	hc.Output(HC_KCreate_Segment(driver));

	return OutputOK;
}

#else

static
int ComputeMaxImageSize(char const * driver, unsigned char * img_ref = 0)
{
#ifdef _MSC_VER
	return 4096;
#else
	int upper_limit = 4096;
	int the_limit = 256;
	unsigned char * white_line = new unsigned char[upper_limit * 4];
	unsigned char * img = img_ref;

	if(!img)
		img = new unsigned char[upper_limit * upper_limit * 4];

	memset(white_line, 0xff, upper_limit * 4);

	while(the_limit < upper_limit ){
		memset(img, 0, upper_limit * upper_limit * 4);

		HC_KEY tmp_seg, image_key;
		HC_Open_Segment("/");{
			tmp_seg = HC_KOpen_Segment("");{
				image_key = HC_KInsert_Image_By_Ref(0, 0, 0, "rgba", upper_limit, upper_limit, img);
			}HC_Close_Segment();
		}HC_Close_Segment();

		HBestImageDriver bid(driver);
		HC_Open_Segment(bid);{

			H_FORMAT_TEXT options(
				"no border, "
				"isolated, "
				"disable input = all, "
				"use window id = (image key = %s%p)",
				H_EXTRA_POINTER_FORMAT, image_key
			);
			HC_Set_Driver_Options(options);
			HC_Set_Window_Frame("off");
			HC_Set_Color("windows = white");

			HC_Update_One_Display(".");

		}HC_Close_Segment();

		HC_Delete_Segment(bid);
		HC_Delete_By_Key(tmp_seg);

		bool worked = true;
		for(int i = 0; i < upper_limit; ++i){
			if(memcmp(&img[i * upper_limit * 4], white_line, upper_limit * 4)){
				worked = false;
				break;
			}
		}

		if(worked)
			the_limit = upper_limit;
		else
			upper_limit *= 0.8;
	}

	if(img != img_ref)
		delete [] img;
	delete [] white_line;

	return the_limit;
#endif
}


static int fread_compressed(unsigned char * data, int size, int count, FILE * f)
{
	int csize = 0;
	if(fread(&csize, sizeof(int), 1, f) != 1)
		return -1;

	if((int)fread(data, 1, csize, f) != csize)
		return -2;

	if(csize == 0)
		return 0;

	int dsize = size * count;
	if(!HUtility::ZlibDecompress(csize, data, dsize, data))
		return -3;

	return count;
}

static int fwrite_compressed(unsigned char * data, int size, int count, FILE * f)
{
	int csize = size * count;
	
	if(!HUtility::ZlibCompress(csize, data, csize, data))
		return -1;

	if(csize > size * count)
		return -2;
	
	if((int)fwrite(&csize, sizeof(int), 1, f) != 1)
		return -3;

	if((int)fwrite(data, 1, csize, f) != csize)
		return -4;

	return count;
}

class BandedImageDriver
{
private:
	unsigned char * m_image;
	mutable unsigned char * m_band;
	HC_KEY m_hiddenSegKey;

	FILE ** m_bandFiles;
	int const y_bands, x_bands;
	int const one_y_step;
	int const one_x_step;
	int const m_blockCount;
    int const m_width, m_height;

	mutable int m_currentBand;
	int const BPP;
	int lines_per_file;
	HC_KEY m_driverKey;
	HPoint m_WindowColorTop;
	HPoint m_WindowColorBottom;
	H_FORMAT_TEXT m_imageName;
	H_FORMAT_TEXT m_textureName;

	bool MergeBlockRows(FILE ** blockFiles);
	BandedImageDriver const & operator = (BandedImageDriver const &);
	bool CompleteBandFile(FILE * band_file);
	bool UpdateTiledDisplay();
    void const * TiledImageRow(int i) const;
	void SetWindowColor(int x, int y);
	void DefineTexture();
	void UnDefineTexture();

public:
    BandedImageDriver(double width, double height, double dpi, HC_KEY model_seg, char const * driver = 0, int max_pixels = 4096);
    ~BandedImageDriver();
    void const * ImageRow(int i) const;
    int Height() const;
    int Width() const;
    bool UpdateDisplay();
	void SetRenderingOptions(char const * ro);
	void SetVisibility(char const * v);
	void SetWindowColor(HPoint const & WindowColorTop, HPoint const & WindowColorBottom);
	char const * GetImageName() const;
};


BandedImageDriver::BandedImageDriver(
	double width, double height, double dpi,
	HC_KEY model_seg, char const * driver, int max_pixels):

	/* Make sure our pointers are null */
	m_image(0),
	m_band(0),
	m_bandFiles(0),

	/* Make sure all the blocks are equal size (this is MUCH easier) */
	y_bands((dpi * height / (double)max_pixels) + 1),
	x_bands((dpi * width / (double)max_pixels) + 1),
	one_y_step(dpi * height / (double)y_bands +1),
	one_x_step(dpi * width / (double)x_bands +1),
	m_blockCount(y_bands * x_bands),

	m_width(dpi * width),
	m_height(dpi * height),
	m_currentBand(-1),

	/* 4 Bytes Per Pixel */
	BPP(4),
	lines_per_file(250),
	m_WindowColorTop(1, 1, 1),
	m_WindowColorBottom(1, 1, 1),
	m_imageName("TwoPassPrintImage %d", HDB::GetUniqueID()),
	m_textureName("TwoPassPrintTexture %d", HDB::GetUniqueID())

{
	/* Create some unnamed segment in which we'll put our image (to which we'll render). */
	HC_KEY image_key;
	HC_Open_Segment("/");{
		m_hiddenSegKey = HC_KCreate_Segment("");
		HC_Open_Segment_By_Key(m_hiddenSegKey);{
			/* We need RGBA to use opengl and dx9 image drivers. */
			char const * format = (BPP == 4 ? "rgba" : "rgb");
			m_image = new unsigned char[BPP * one_x_step * one_y_step];
                        memset(m_image, 0, BPP * one_x_step * one_y_step);
			image_key = HC_KInsert_Image_By_Ref(0, 0, 0,
				H_FORMAT_TEXT("name = %s, %s", (char const*)m_imageName, format),
				one_x_step, one_y_step, m_image);
		}HC_Close_Segment();
    }HC_Close_Segment();

	DefineTexture();

	/* Setup the driver. */
	HBestImageDriver bid(driver);
	m_driverKey = HC_KOpen_Segment(bid);{

		HC_Set_Window_Frame("off");
		HC_Set_Handedness("left");

		H_FORMAT_TEXT options(
			"no border, "
			"isolated, "
			"disable input = all, "
			"use window id = (image key = %s%p), "
			"physical size = (%f, %f), "
			"hardcopy resolution = %d",
			H_EXTRA_POINTER_FORMAT, image_key,
			2.54 * (width / (double)x_bands),
			2.54 * (height / (double)y_bands),
			(int)dpi
		);
		HC_Set_Driver_Options(options);

		HC_Open_Segment("tiled window");{
			HC_Open_Segment("full window");{
				HC_Set_Heuristics("culling = (no maximum extent, view frustum)");
				HC_Set_Rendering_Options("attribute lock = heuristics");
				HC_Include_Segment_By_Key(model_seg);
			}HC_Close_Segment();
		}HC_Close_Segment();

	}HC_Close_Segment();
}

void BandedImageDriver::SetWindowColor(HPoint const & WindowColorTop, HPoint const & WindowColorBottom)
{
	m_WindowColorTop = WindowColorTop;
	m_WindowColorBottom = WindowColorBottom;
}

void BandedImageDriver::SetRenderingOptions(char const * ro)
{
	HC_Open_Segment_By_Key(m_driverKey);{
		HC_Set_Rendering_Options(ro);
	}HC_Close_Segment();
}

void BandedImageDriver::SetVisibility(char const * v)
{
	HC_Open_Segment_By_Key(m_driverKey);{
		HC_Set_Visibility(v);
	}HC_Close_Segment();
}

BandedImageDriver::~BandedImageDriver()
{
	UnDefineTexture();

	delete [] m_band;

    HC_Delete_By_Key(m_hiddenSegKey);
	delete [] m_image;

	if(m_bandFiles){
		int band_file_count = m_height / lines_per_file + (m_height % lines_per_file ? 1 : 0);
		for(int i = 0; i < band_file_count; ++i){
			if(m_bandFiles[i])
				fclose(m_bandFiles[i]);
		}
		delete [] m_bandFiles;
	}
	HC_Delete_By_Key(m_driverKey);
}

void const * BandedImageDriver::ImageRow(int row) const
{
	/* If we only have one tile, UpdateDisplay simply copies the image data pointer to m_bands short circuiting
	 * the tmpfiles and compression/decompression step. */
	if(m_blockCount == 1){
		int offset = row * x_bands * one_x_step * BPP;
		return m_band + offset;
	}
	
	return TiledImageRow(row);
}

void const * BandedImageDriver::TiledImageRow(int row) const
{
	int file_index = row / lines_per_file;

	/* Do we need to load data from another band file? */
	if(m_currentBand != file_index)
	{
		m_currentBand = file_index;

		if(!m_band)
			m_band = new unsigned char[lines_per_file * one_x_step * x_bands * BPP];

		rewind(m_bandFiles[file_index]);

		for(int line = 0; line < lines_per_file; ++line){
			for(int x_band = 0; x_band < x_bands; ++x_band){

				int rc = fread_compressed(
					m_band + BPP * one_x_step * (line * x_bands + x_band),
					BPP * one_x_step, 1,
					m_bandFiles[file_index]);
				assert(rc >= 0);
				
				/* fread_compressed may return zero if there's less than lines_per_file
				 * in the band file. */
				if(rc == 0)
					goto DONE;
			}
		}
	}

DONE:
	/* Compute the offset in the band data which corrisponds to row in the big picture. */
	int file_line = row % lines_per_file;
	int offset = file_line * x_bands * one_x_step * BPP;
	return m_band + offset;
}

int BandedImageDriver::Height() const
{
    return m_height;
}

int BandedImageDriver::Width() const
{
    return m_width;
}

bool BandedImageDriver::CompleteBandFile(FILE * band_file)
{
	/* We need to write a zero after the last line so we know when we've read all the
	 * lines in the reading code (as there may be less than lines_per_file lines in the
	 * last file). */
	int zero = 0;
	if(1 != fwrite(&zero, sizeof(int), 1, band_file))
		return false;
	if(0 != fflush(band_file))
		return false;
	return true;
}

bool BandedImageDriver::MergeBlockRows(FILE ** blockFiles)
{
	bool retval = false;
	int one_line_size = -1;
	unsigned char * one_line = 0;
	FILE * band_file = 0;

	/* We're going to merge the blocks into bands lines_per_file pixels tall. */
	int band_file_count = m_height / lines_per_file + (m_height % lines_per_file ? 1 : 0);
	m_bandFiles = new FILE*[band_file_count];
	memset(m_bandFiles, 0, sizeof(FILE*) * band_file_count);

	/* What we're going to do here isn't so clear from the code.  What it's doing is taking a row of
	 * blocks, reading the top line from each block in the row, then writing those lines to the band
	 * file to form one full-width line.  We then advance to line 2 and continue in that fassion until
	 * we move past the last line of the block OR we exceed lines_per_file lines in the band file.  We
	 * know we've exceeded lines_per_file when we get a null file pointer in m_bandFiles[file_index]. */
	
	for(int y_band = 0; y_band < y_bands; ++y_band){ /* For each row of blocks... */
		
		for(int line = 0; line < one_y_step; ++line){ /* For each line in a row of blocks... */

			for(int x_band = 0; x_band < x_bands; ++x_band){ /* For each block in the row... */

				int block_index = y_band * x_bands + x_band;
				int count = 0;
				
				if(1 != fread(&count, sizeof(int), 1, blockFiles[block_index]))
					goto DONE;

				/* Make sure we have enough memory. */
				if(one_line_size < count){
					delete [] one_line;
					/* add one in case count happens to be zero. */
					one_line = new unsigned char[count+1];
					one_line_size = count+1;
				}

				if(count > 0){
					if(1 != fread(one_line, count, 1, blockFiles[block_index]))
						goto DONE;
				}

				int const file_index = (y_band * one_y_step + line) / lines_per_file;
				
				/* Make sure we have a band file ready.  Keep in mind that a row of blocks may be 2048 pixels
				 * high, but the band files are limited to lines_per_file high.  So the mapping from rows to
				 * bands isn't 1:1 (and the seems most likely dont line up either).  */
				if(!m_bandFiles[file_index]){
					if(band_file && !CompleteBandFile(band_file)){
						goto DONE;
					}
					band_file = m_bandFiles[file_index] = GenerateTmpFilePtr();
				}

				if(1 != fwrite(&count, sizeof(int), 1, band_file))
					goto DONE;
				if(count > 0){
					if(1 != fwrite(one_line, count, 1, band_file))
						goto DONE;
				}
			}
		}
	}
	if(band_file && !CompleteBandFile(band_file)){
		goto DONE;
	}
	retval = true;
DONE:
	delete [] one_line;
	return retval;
}

void BandedImageDriver::SetWindowColor(int x, int y)
{
	double r1;
	double r0;

	if(y_bands > 1){
		/* As y increases, we're moving down the window. */
		r1 = (y_bands - y - 0) / (double)y_bands;
		r0 = (y_bands - y - 1) / (double)y_bands;
	}
	else {
		r1 = 1;
		r0 = 0;
	}

	HVector const dist = m_WindowColorTop - m_WindowColorBottom;
	HVector const top = dist * r1 + m_WindowColorBottom;
	HVector const bot = dist * r0 + m_WindowColorBottom;

	H_FORMAT_TEXT color("windows=(r=%f g=%f b=%f), window contrast = (r=%f g=%f b=%f)", 
				top.x, top.y, top.z,
				bot.x, bot.y, bot.z);

	HC_Set_Color(color);
}

bool BandedImageDriver::UpdateDisplay()
{
	bool retval = true;

	/* With only one block, we can bypass the tiled code.  */
	if(m_blockCount == 1){
		HC_Open_Segment_By_Key(m_driverKey);{
			SetWindowColor(0, 0);
			HC_Set_Window_Pattern("Down");
			HC_Update_One_Display(".");
		}HC_Close_Segment();
		m_currentBand = 0;
		m_band = m_image;
		m_image = 0;
	}
	else
		retval = UpdateTiledDisplay();
	
	return retval;
}

bool BandedImageDriver::UpdateTiledDisplay()
{
	bool retval = false;
	FILE ** blockFiles = new FILE*[m_blockCount];
	memset(blockFiles, 0, sizeof(FILE*) * m_blockCount);

	for(int block = 0, y = 0; y < y_bands; ++y){

		double const yscale = 2 * m_height / (double)one_y_step;
		double const y1 = 1 + yscale * y * one_y_step / (double)m_height;
		double const y0 = y1 - yscale;

		for(int x = 0; x < x_bands; ++x, ++block){

			double const xscale = 2 * m_width / (double)one_x_step;
			double const x0 = -1 - xscale * x * one_x_step / (double)m_width;
			double const x1 = x0 + xscale;

			HC_Open_Segment_By_Key(m_driverKey);{
				HC_Open_Segment("tiled window");{
					/* Shift our 'window' into position */
					HC_Set_Window(x0, x1, y0, y1);
					SetWindowColor(x, y);
					HC_Set_Window_Pattern("Down");
				}HC_Close_Segment();
				HC_Update_One_Display(".");
			}HC_Close_Segment();

			/* Write the image to another block file */
			blockFiles[block] = GenerateTmpFilePtr();
			for(int row = 0; row < one_y_step; ++row){
				int rc = fwrite_compressed(
					m_image + row * BPP * one_x_step,
					BPP,
					one_x_step,
					blockFiles[block]);
				if(rc != one_x_step){
					goto DONE;
				}
			}
			if(0 != fflush(blockFiles[block])){
				goto DONE;
			}
			rewind(blockFiles[block]);
		}
	}

	retval = MergeBlockRows(blockFiles);
DONE:
	/* Block file have been merged into band files so this is no
	 * longer needed...go ahead and free it. */
	for(int i = 0; i < m_blockCount; ++i){
	    if(blockFiles[i])
			fclose(blockFiles[i]);
	}
	delete [] blockFiles;
	return retval;
}

static void imageCallback(int width, 
						int height,						
						int currentrow,
						int  *(rwidth),
						int  *(totalheight),
						int  *(rheight),
						char * *(data),
						int counter,
						BandedImageDriver *
						);

void BandedImageDriver::DefineTexture()
{
	/* With only one block, we can bypass the tiled code.  */
	if(m_blockCount == 1){
		HC_Define_Texture(m_textureName, H_FORMAT_TEXT("source = %s", (char const*)m_imageName));
	}
	else
	{
		HImRegisterCallback(m_textureName, imageCallback);
		HC_Define_Texture(m_textureName,
			H_FORMAT_TEXT(
						"source callback = %s, "
						"source callback data = %s",
						(char const*)m_textureName,
						kto0xa(this)
			)
		);
	}
}

void BandedImageDriver::UnDefineTexture()
{
	/* With only one block, we can bypass the tiled code.  */
	if(m_blockCount == 1){
		HC_UnDefine_Texture(m_textureName);
	}
	else {
		HC_UnDefine_Callback_Name(m_textureName);
		HC_UnDefine_Texture(m_textureName);
	}
}

char const * BandedImageDriver::GetImageName() const
{
	return m_textureName;
}

/* imageCallback() is for use with HUtility::TwoPassPrint()
 */
static void imageCallback(int width, 
						int height,						
						int currentrow,
						int  *(rwidth),
						int  *(totalheight),
						int  *(rheight),
						char * *(data),
						int counter,
						BandedImageDriver * pbid
						)
{
	if(rwidth)
		*rwidth = pbid->Width();

	if(totalheight)
		*totalheight = pbid->Height();

	if(data){
		void const * row = pbid->ImageRow(currentrow);
		char * c = (char*)(row);
		*data = c;
		if(rheight)
			*rheight = 1;
	}
	else if(rheight)
		*rheight = 0;
}

HFileOutputResult HUtility::TwoPassPrint(const char *driver,
										 HOutputHandlerOptions *options)
{	
	HFileOutputResult retval = OutputOK;
	HCamera camera, orig_camera;


/*******************************************************************************************
 *
 * Driver Setup
 *
 ******************************************************************************************/

	/* print in grayscale if appropriate */
	bool fgs_scene = false;
	bool fgs_axis = false;
	if (options->ForceGrayscale()){
		HC_Open_Segment_By_Key(options->m_pHBaseView->GetSceneKey());{
			fgs_scene = HUtility::PushAttribute("rendering options");
			HC_Set_Rendering_Options("force grayscale = on");
			HC_Open_Segment("axis");{
				fgs_axis = HUtility::PushAttribute("rendering options");
				HC_Set_Rendering_Options("force grayscale = on");
			}HC_Close_Segment();	
		}HC_Close_Segment();
	}

	HC_KEY driver_key, markup_key;
	HC_Open_Segment_By_Key(options->m_pHBaseView->GetOverwriteKey());
		markup_key = HC_KCreate_Segment("markup");
	HC_Close_Segment();

	/* If page size <= 0, page size = window size. */
	double paper_width =  options->PaperWidth() <= 0 ? options->WindowWidth() : options->PaperWidth();
	double paper_height = options->PaperHeight() <= 0 ? options->WindowHeight() : options->PaperHeight();

	double page_ar = (double)paper_width/(double)paper_height;
	if (options->WindowHeight() == 0){
		HC_Open_Segment_By_Key(options->View()->GetViewKey());{
			char size_str[64];
			HC_Show_Device_Info(".", "size", size_str);
			float width = 0, height = 0;
			HCLOCALE(sscanf(size_str, "%f,%f", &width, &height));
			options->WindowWidth(width);
			options->WindowHeight(height);
		}HC_Close_Segment();
	}

	double window_ar = (double)options->WindowWidth()/(double)options->WindowHeight();

	//Setup the driver segment
	double SubscreenXmin = -1.0f;
	double SubscreenXmax =  1.0f;
	double SubscreenYmin = -1.0f;
	double SubscreenYmax =  1.0f;

	if (options->UseSubscreen()){		
		if (window_ar > page_ar){
			SubscreenYmin = -page_ar/window_ar;
			SubscreenYmax = page_ar/window_ar;
		} else {
			SubscreenXmin = -window_ar/page_ar;
			SubscreenXmax = window_ar/page_ar;
		}
	}

	//Adjust page subscreen, for page margins
	double screen_left_offset = (2.0*options->m_PageMargins[0])/options->PaperWidth();
	double screen_right_offset = (2.0*options->m_PageMargins[1])/options->PaperWidth();
	double screen_bottom_offset = (2.0*options->m_PageMargins[2])/options->PaperHeight();
	double screen_top_offset = (2.0*options->m_PageMargins[3])/options->PaperHeight();

	screen_left_offset = CLAMP(screen_left_offset, 0.0, 2.0);
	screen_right_offset = CLAMP(screen_right_offset, 0.0, 2.0);
	screen_bottom_offset = CLAMP(screen_bottom_offset, 0.0, 2.0);
	screen_top_offset = CLAMP(screen_top_offset, 0.0, 2.0);

	if(screen_left_offset + screen_right_offset >= 2){
		screen_left_offset = screen_right_offset = 0;
	}

	if(screen_bottom_offset + screen_top_offset >= 2){
		screen_bottom_offset = screen_top_offset = 0;
	}
	
	if(fabs(-1.0 - SubscreenXmin) < screen_left_offset)
		SubscreenXmin = -1.0f + screen_left_offset;
	if(fabs(1.0 - SubscreenXmax) < screen_right_offset)
		SubscreenXmax = 1.0f - screen_right_offset;
	if(fabs(-1.0 - SubscreenYmin) < screen_bottom_offset)
		SubscreenYmin = -1.0f + screen_bottom_offset;
	if(fabs(1.0 - SubscreenYmax) < screen_top_offset)
		SubscreenYmax = 1.0f - screen_top_offset;

	// driver setup done, setup the camera now
	/*	We modify the scene camera to fit the page since the scene is
		is included further down.  This means we have to save the original
		camera and restore it when we're done. */
	options->View()->GetCamera(&orig_camera);
	camera = orig_camera;

	// this code is needed if the aspect ratio of the window changes. 
	HC_Open_Segment_By_Key(options->View()->GetViewKey());{
		double pixel_ratio, camera_ratio;
		HPoint test;
		test.Set(1, -1, 0);
		HC_Compute_Coordinates(".", "local window", &test, "local pixels", &test);
		//set the aspect ratio of the print to be the same as the current view
		pixel_ratio = test.x/test.y;
		camera_ratio = camera.field_width/camera.field_height;

		if (pixel_ratio > camera_ratio)
			camera.field_width *= pixel_ratio/camera_ratio;
		else 
			camera.field_height *= camera_ratio/pixel_ratio;

	}HC_Close_Segment();

	HC_Open_Segment_By_Key(options->View()->GetSceneKey());
		HC_Set_Camera (&camera.position, &camera.target, &camera.up_vector, camera.field_width, camera.field_height, camera.projection);
	HC_Close_Segment();

	double pwidth = paper_width * 0.5 * (SubscreenXmax-SubscreenXmin);
	double pheight = paper_height * 0.5 * (SubscreenYmax-SubscreenYmin);

	char current_hsra[MVO_BUFFER_SIZE] = {""};
	char framebuffer_effects_options[MVO_BUFFER_SIZE];
	int framebuffer_effects_border = 0;
	HC_Open_Segment_By_Key(options->View()->GetViewKey());{
		HC_Show_One_Net_Rendering_Optio("hsra", current_hsra);
		options->View()->GetFrameBufferEffectsOptions (framebuffer_effects_options, &framebuffer_effects_border); 
	}HC_Close_Segment();

	HPoint WindowColorTop, WindowColorBottom;
	options->View()->GetWindowColor(WindowColorTop, WindowColorBottom);

	driver_key = HC_KOpen_Segment(driver);{
		H_FORMAT_TEXT drvopt(
			"no border,"
			"disable input = all,"
			"no landscape orientation," 
			"subscreen stretching," 
			"isolated," 
			"absolute line weight," 
			"text preference = outline,"
			"physical size = (%f, %f)," 
			"hardcopy resolution = %d," 
			"subscreen = (%f, %f, %f, %f)",
			2.54 * paper_width,
			2.54 * paper_height,
			options->PaperDpi(),
			SubscreenXmin, SubscreenXmax,
			SubscreenYmin, SubscreenYmax);
		HC_Set_Driver_Options(drvopt);

		HC_KEY include_key = HC_KInclude_Segment_By_Key (options->View()->GetSceneKey());
		HC_KEY include_key_path[] = {options->View()->GetIncludeLinkKey(), include_key} ;

		HUtilityAnnotation::recursive_resize(
			options->View()->GetModelKey(), 
			2, include_key_path, 
			driver_key);
		HUtilityAnnotation::recursive_resize(markup_key, 1, &include_key, driver_key);

		//and now the handedness
		HandednessMode mode = options->View()->GetHandedness();
		if (mode == HandednessLeft)
			HC_Set_Handedness("left");	
		else if (mode == HandednessRight)
			HC_Set_Handedness("right");	

		if (options->WindowFrame())
			HC_Set_Window_Frame ("on");
		else 
			HC_Set_Window_Frame ("off");

		// we do not want the background on the subwindows to be screwed up
		// create so set the window pattern as blend before we include
		// the scene
		HC_Set_Window_Pattern("blend");

	}HC_Close_Segment();

	
	H_OUTPUT_TYPE output_type = options->OutputType();	

/*******************************************************************************************
 *
 * Pass 1/2
 * Generate a low resolution version of the background image and check it for non-white
 * pixels.  If there aren't any, we can skip pass one completely and set "no hidden
 * surfaces" in pass 2. 
 *
 ******************************************************************************************/
	bool doPassOne = false;
	if (output_type != H_SINGLE_PASS_ZSORT && output_type != H_SINGLE_PASS_NO_HIDDEN_SURFACES)
	{/* put a scope here so small_image_check will be cleaned up before we move on. */
		BandedImageDriver small_image_check(
			pwidth, pheight,
			24 /* DPI */,
			options->m_pHBaseView->GetSceneKey(),
			options->View()->GetDriverType());
		small_image_check.SetRenderingOptions(
			H_FORMAT_TEXT(
				"hsra = %s,"
				"attribute lock = (visibility = (lines, edges ,markers, text)),"
				"ignore subsegment locks = (visibility = (lines, edges ,markers, text)),"
				"force grayscale = %s",
				current_hsra,
				options->ForceGrayscale() ? "on" : "off")
			);
		small_image_check.SetVisibility("lines = off, edges = off, markers = off, text = off");

		if(options->UseWindowColor())
			small_image_check.SetWindowColor(WindowColorTop, WindowColorBottom);

		if(small_image_check.UpdateDisplay()){
			
			unsigned int * all_white = new unsigned int[small_image_check.Width()];
			int len = sizeof(unsigned int) * small_image_check.Width();
			memset(all_white, 0xff, len);

			for(int row = 0; row < small_image_check.Height(); ++row){
				unsigned int * img_line = (unsigned int *)small_image_check.ImageRow(row);
				if( 0 != memcmp(all_white, img_line, len) ){
					doPassOne = true;
					break;
				}
			}
			delete [] all_white;
		}
	}

/*******************************************************************************************
 *
 * Pass 1
 * Generate a full resolution version of the background image  
 *
 ******************************************************************************************/

	BandedImageDriver * pbid = 0;
	if(doPassOne){
		int const max_image_size = ComputeMaxImageSize(options->View()->GetDriverType());
		pbid = new BandedImageDriver(
			pwidth, pheight,
			options->PaperDpi(),
			options->m_pHBaseView->GetSceneKey(),
			options->View()->GetDriverType(),
			max_image_size);
		pbid->SetRenderingOptions(
			H_FORMAT_TEXT(
				"hsra = %s,"
				"attribute lock = (visibility = (lines, edges ,markers, text)),"
				"ignore subsegment locks = (visibility = (lines, edges ,markers, text)),"
				"force grayscale = %s",
				current_hsra,
				options->ForceGrayscale() ? "on" : "off")
			);
		pbid->SetVisibility("lines = off, edges = off, markers = off, text = off");
		if(options->UseWindowColor())
			pbid->SetWindowColor(WindowColorTop, WindowColorBottom);

		bool success = pbid->UpdateDisplay();

		if(!success){
			retval = OutputFail;
			goto CLEANUP;
		}
	}

/*******************************************************************************************
 *
 * Pass 2
 * Generate the vector (lines, edges, text) and overlay them on the background image 
 *
 ******************************************************************************************/

	driver_key = HC_KOpen_Segment(driver);{

		if(pbid){
			H_FORMAT_TEXT color("windows = %s", pbid->GetImageName());
			HC_Set_Color(color);
			HC_Set_Rendering_Options("hsra = hidden line, hlro = (visibility = off)");
			HC_Set_Rendering_Options(
				"attribute lock = heuristics,"
				"ignore subsegment locks = heuristics"
				);
		}
		else{

			if (options->OutputType() == H_SINGLE_PASS_ZSORT) {		

				HC_Set_Rendering_Options("hsra = z-sort only, transparency = style = off");
				HC_Set_Rendering_Options(
					"attribute lock = heuristics,"
					"ignore subsegment locks = heuristics"
					);
			}

			else if (options->OutputType() == H_SINGLE_PASS_NO_HIDDEN_SURFACES) {
				HC_Set_Heuristics("no hidden surfaces");
				HC_Set_Rendering_Options(
					"attribute lock = heuristics,"
					"ignore subsegment locks = heuristics"
					);
			}

			else if(strstr(current_hsra, "hidden line")){
				HC_Set_Rendering_Options("hsra = hidden line, hlro = (visibility = off)");
				HC_Set_Rendering_Options(
					"attribute lock = heuristics,"
					"ignore subsegment locks = heuristics"
					);
			}
			else
				HC_Set_Heuristics("no hidden surfaces");
		}

	}HC_Close_Segment();

	HC_Open_Segment_By_Key(options->m_pHBaseView->GetAxisTriadKey());{
		/* The render faces setting in the axis triad is there so we'll always see
		   the faces (even in hidden line mode).  As we're setting hidden line mode
		   here specifically to get the scene vectors, disable render faces. */
		HC_UnSet_One_Rendering_Option("hlro");
	}HC_Close_Segment();

	/* We don't want a shadow in pass two, but we do want it in pass 1/2 and 1, so don't
	   turn off the shadow until we're ready for pass 2. */
	options->View()->SetShadowMode(HShadowNone, false);

	HC_Control_Update (driver, "redraw everything");	
	HC_Update_One_Display (driver);


/*********************
 *
 * Cleanup
 *
 *********************/
CLEANUP:
	/* Restore the scene to the way it was before twopassprinting */
	if (options->ForceGrayscale()){
		HC_Open_Segment_By_Key(options->m_pHBaseView->GetSceneKey());{
			if (fgs_scene) HUtility::PopAttribute("rendering options");
			else HC_UnSet_One_Rendering_Option("force grayscale");
			HC_Open_Segment("axis");{
				if (fgs_axis) HUtility::PopAttribute("rendering options");
				else HC_UnSet_One_Rendering_Option("force grayscale");
			}HC_Close_Segment();
		}HC_Close_Segment();
	}

	HC_Open_Segment_By_Key(options->m_pHBaseView->GetAxisTriadKey());{
		HC_Set_Rendering_Options("hlro = render faces");
	}HC_Close_Segment();


	options->View()->SetShadowMode(options->m_pHBaseView->GetShadowMode());

	// set camera does an update so the annotation resize must come first
	HC_Open_Segment_By_Key(options->View()->GetSceneKey());
		HC_Set_Camera (&orig_camera.position, &orig_camera.target, &orig_camera.up_vector, orig_camera.field_width, orig_camera.field_height, orig_camera.projection);
	HC_Close_Segment();

	HUtilityAnnotation::WindowResizeCallback (0, 0, options->m_pHBaseView);

	delete pbid;

	return retval;
}

#endif


/* ax + by + cz + d > 0 */
#define TEST_POINT_PLANE(pt,plane) ((((pt)[0]*(plane)[0])+((pt)[1]*(plane)[1])+((pt)[2]*(plane)[2])+(plane)[3]) > 0)

#define DOT_FPOINT_FPLANE(pt,plane) (((pt)[0]*(plane)[0])+((pt)[1]*(plane)[1])+((pt)[2]*(plane)[2])+(plane)[3])

#define DOT_POINT_PLANE(pt,plane) (((pt)->x*(plane)->a)+((pt)->y*(plane)->b)+((pt)->z*(plane)->c)+(plane)->d)
/* test to see if "pt" lies within the prism defined by sweeping the polygon defined 
 * by {"points","len","iptr"} along the normal of its "plane".  If the point lies 
 * within "fudge" of the prism, return value is CS_MAYBE.  

  */
bool HUtility::IsPointInsideAllEdges(
		const float *pt,
		const float *points1, 
		int len,
		const int *face1,
		const float *plane1,
		float fudge)
{	 
	const float *pt1, *pt2, *pt3;
	int i, in = 0, firsttime;
	bool status;
	float halfspace[4];
	float diff[3];

	/* nothing can clash with a degenerate face */
	if (len < 3)
		return 0;

	firsttime = 1;
	for (i = 0 ; i < len ; i++) {	
		pt1 = points1 + 3*face1[i];
		if (i+1 < len)
			pt2 = points1 + 3*face1[i+1];
		else
			pt2 = points1 + 3*face1[i+1-len]; /* wrap around */
		diff[0] = pt2[0] - pt1[0];
		diff[1] = pt2[1] - pt1[1];
		diff[2] = pt2[2] - pt1[2];
 		status = HC_Compute_Normalized_Vector((HPoint *)diff, (HPoint *)diff);
		HC_Compute_Cross_Product((HPoint *)diff, (HPoint *)plane1, (HPoint *)halfspace);
		if (!HC_Compute_Normalized_Vector((HPoint *)halfspace, (HPoint *)halfspace))
			status = false;
 		if (!status)
			continue;
		/* d component is -(ax + by + cz) */
		halfspace[3] = -(pt2[0]*halfspace[0] + pt2[1]*halfspace[1] + pt2[2]*halfspace[2]);
		if (firsttime) {
			/* we don't know if the face is clockwise or counterclockwise, but we
			 * can assume that the 3rd point is "in", so we just go by that. We can get
			 * away with doing this only once, because "in" will be the same for all edges*/
			if (i+2 < len)
				pt3 = points1 + 3*face1[i+2];
			else
				pt3 = points1 + 3*face1[i+2-len]; /* wrap around */
			in = TEST_POINT_PLANE(pt3, halfspace);
			firsttime = 0;
		}
		if (in > 0) {
			if (DOT_FPOINT_FPLANE(pt,halfspace) < 0) {
				if (DOT_FPOINT_FPLANE(pt,halfspace) > -fudge)
					return true;
				else
					return false;
			}
		}
		else {
			if (DOT_FPOINT_FPLANE(pt,halfspace) > 0) {
				if (DOT_FPOINT_FPLANE(pt,halfspace) < fudge)
					return true;
				else
					return false;
			}
		}
	}
	if (firsttime)
		return false; /* nothing intersects a face whose points are all colinear */
	return true;
} 

void HUtility::DeleteManipulators(HC_KEY key)
{
	char type[512];
	bool isSegment = true;
	HC_Show_Key_Type(key, type);
	if (!streq(type, "segment"))
	{
		isSegment = false;
		key = HC_Show_Owner_By_Key(key, 0);
	}

	HC_Open_Segment_By_Key(key);
	{
		int cnt = HC_Show_Existence("geometry");
		if (isSegment || cnt == 1)
		{
			if (HC_Show_Existence("user options"))
			{
				char options[512];
				HC_Show_User_Options(options);
				if (strstr(options, "hashandles"))
					HC_Delete_Segment("cpbox");
			}
		}
	}
	HC_Close_Segment();
}


void HUtility::IntersectionLinePlane(HPoint *p1, HPoint *p2, HPlane *p, HPoint *res)
{
	float u = (p->a() * p1->x + p->b() * p1->y + p->c() * p1->z + p->d()) / 
		(p->a() * (p1->x - p2->x) + p->b() * (p1->y - p2->y) + p->c() * (p1->z - p2->z));

	res->Set(p1->x + u * (p2->x - p1->x), p1->y + u * (p2->y - p1->y), p1->z + u * (p2->z - p1->z));

}

void HUtility::MergePointsIntoShell(int num_shell_points, HPoint *plist1, int num_shell_facelist, int *flist1, 
									int num_merge_points, HPoint *merge_points, HPoint *merge_vectors, float tolerance, bool AddExtraPoints, int *newpoints, int *newpointslen)
{
	char text[MVO_BUFFER_SIZE];
	HCLOCALE (sprintf(text,"tolerance = %f, vertex vector, identify face", tolerance));
	int *result = new int[num_merge_points];

	if (newpointslen)
		*newpointslen = 0;
	HC_Compute_Points_Inside_Shell(num_merge_points, merge_points, merge_vectors, num_shell_points, plist1, num_shell_facelist,flist1, text,result);
	 
	int number_of_faces = 0;
	int f = 0,i;
	
	while (1)
	{

		number_of_faces++;
		f+=flist1[f] + 1;
		if (f>=num_shell_facelist)
			break;
	}

	HMergeShellFace * faces = new HMergeShellFace[number_of_faces];

	number_of_faces = 0;
	f = 0;
	while (1)
	{

		faces[number_of_faces++].Init(&flist1[f]);
 		f+=flist1[f] + 1;
		if (f>=num_shell_facelist)
			break;
	}


	HPoint *plist2 = new HPoint[num_merge_points * 5 + num_shell_points];

	for (i=0;i<num_shell_points;i++)
		plist2[i] = plist1[i];

	int pnum = num_shell_points;
 
	for (i=0;i<num_merge_points;i++)
	{
		if (result[i] >= 0) 
		{
		
			HPoint res;
			HMergeShellFace *face = faces[result[i]].FindHitFace(merge_points[i], merge_vectors[i], plist2, res);
			if (face)	
			{
				plist2[pnum] = res;
				face->MakeChildFaces(pnum, plist2, AddExtraPoints, newpoints, newpointslen);
			}

  
		}	
	}


	int totalflen = 0;
	for (i=0;i<number_of_faces;i++)
	{
		totalflen +=faces[i].GetLength();
	}
	int *newflist = new int[totalflen];

	int j = 0;
	for (i=0;i<number_of_faces;i++)
	{
		j+= faces[i].AddToFacelist(&newflist[j]);
	}

	HC_Insert_Shell(pnum, plist2, j, newflist);


	delete [] plist2;
	delete [] newflist;
	delete [] faces;
}


static void CalculateQuadrant(HPoint &p, HPoint &totalmin, HPoint &totaldeltadiv, 
							  HPoint &quadrant)
{
	quadrant.Set(int((p.x - totalmin.x) / totaldeltadiv.x), int((p.y - totalmin.y) / totaldeltadiv.y),
		int((p.z - totalmin.z) / totaldeltadiv.z));
	

}

static bool DivideFaceAgainstPlane(HPoint *points, int flen, HPoint &quadrant, HPoint &divi, HPoint &min, HPlane &plane, HPoint *newpolygon1, int &newlen1,
	HPoint *newpolygon2, int &newlen2)
{
	UNREFERENCED(quadrant);
	UNREFERENCED(divi);
	UNREFERENCED(min);

	bool divide = false;
	if (plane.a() > 0)
	{
		for (int i=0;i<flen;i++)
			if (points[i].x > -plane.d())
			{
			   divide = true; 
			   break; 
			}

	}
	else if (plane.a() < 0)
	{
		for (int i=0;i<flen;i++)
			if (points[i].x < plane.d())
			{
			   divide = true; 
			   break; 
			}
	}
	else if (plane.b() > 0)
	{
		for (int i=0;i<flen;i++)
			if (points[i].y > -plane.d())
			{
			   divide = true; 
			   break; 
			}

	}
	else if (plane.b() < 0)
	{
		for (int i=0;i<flen;i++)
			if (points[i].y < plane.d())
			{
			   divide = true; 
			   break; 
			}
	}
	else if (plane.c() > 0)
	{
		for (int i=0;i<flen;i++)
			if (points[i].z > -plane.d())
			{
			   divide = true; 
			   break; 
			}

	}
	else if (plane.c() < 0)
	{
		for (int i=0;i<flen;i++)
			if (points[i].z < plane.d())
			{
			   divide = true; 
			   break; 
			}
	}

	if (divide)
	{	
		bool NoCut;
		newlen1 = HUtility::ClipPolygonAgainstPlane(flen, points, (float *)&plane, newpolygon1, &NoCut);
		if (NoCut)
			return false;
		HPlane plane2;	
		plane2 = HPlane(-plane.a(), -plane.b(), -plane.c(), -plane.d());
		newlen2 = HUtility::ClipPolygonAgainstPlane(flen, points, (float *)&plane2, newpolygon2);
	}
	return divide;
}



static void DivideFaceAgainstBox(int *face, int flen, HPoint *points, HPoint &quadrant, HPoint &divi, HPoint &min, HShellWrapper &shellclip, HShellWrapper &shellleftover)
{
	HPlane plane;
	HPoint p[256];

	for (int i=0;i<flen;i++)
		p[i] = points[face[i]];

 	HPoint newpolygon1[256];
	HPoint newpolygon2[256];
	int len1 = 0,len2 = 0;

	plane.Set(-1,0,0,(min.x+divi.x * quadrant.x));
	if (DivideFaceAgainstPlane(p, flen, quadrant, divi, min, plane, newpolygon1, len1, newpolygon2, len2))
	{	
		shellleftover.AddFace(newpolygon1, len1);
		for (int i=0;i<len2;i++)
		p[i] = newpolygon2[i];
		flen = len2;
	}

	plane.Set(1,0,0,-(min.x+divi.x * quadrant.x + divi.x));
	if (DivideFaceAgainstPlane(p, flen, quadrant, divi, min, plane, newpolygon1, len1, newpolygon2, len2))
	{	
		shellleftover.AddFace(newpolygon1, len1);
		for (int i=0;i<len2;i++)
		p[i] = newpolygon2[i];
		flen = len2;
	}

	plane.Set(0,-1,0,(min.y+divi.y * quadrant.y));
	if (DivideFaceAgainstPlane(p, flen, quadrant, divi, min, plane, newpolygon1, len1, newpolygon2, len2))
	{	
		shellleftover.AddFace(newpolygon1, len1);
		for (int i=0;i<len2;i++)
		p[i] = newpolygon2[i];
		flen = len2;
	}

	plane.Set(0,1,0,-(min.y+divi.y * quadrant.y + divi.y));
	if (DivideFaceAgainstPlane(p, flen, quadrant, divi, min, plane, newpolygon1, len1, newpolygon2, len2))
	{	
		shellleftover.AddFace(newpolygon1, len1);
		for (int i=0;i<len2;i++)
		p[i] = newpolygon2[i];
		flen = len2;
	}
	plane.Set(0,0,-1,(min.z+divi.z * quadrant.z));
	if (DivideFaceAgainstPlane(p, flen, quadrant, divi, min, plane, newpolygon1, len1, newpolygon2, len2))
	{	
		shellleftover.AddFace(newpolygon1, len1);
		for (int i=0;i<len2;i++)
		p[i] = newpolygon2[i];
		flen = len2;
	}

	plane.Set(0,0,1,-(min.z+divi.z * quadrant.z + divi.z));
	if (DivideFaceAgainstPlane(p, flen, quadrant, divi, min, plane, newpolygon1, len1, newpolygon2, len2))
	{	
		shellleftover.AddFace(newpolygon1, len1);
		for (int i=0;i<len2;i++)
		p[i] = newpolygon2[i];
		flen = len2;
	}

	shellclip.AddFace(p, flen);



	
}
void HUtility::SubdivideOneShell(HShellWrapper &myshell, int numx, int numy, int numz, HPoint &totalmin,
								 HPoint &totaldeltadiv, HShellWrapper *shellmatrix, HShellWrapper &leftover, float boxdiagonallength, bool DoCut, int regionnum, ShellToRegionMapper *mapper, HC_KEY newkey)
{
	UNREFERENCED(boxdiagonallength);

	int i;
	float matrix[16];
	leftover.SetEmptyShell(newkey, true);

	if (myshell.m_Key != -1)
	{
		myshell.OpenOwnerSegment();
		HC_Show_Net_Modelling_Matrix(matrix);
		HC_Close_Segment();
		myshell.TransformPoints(matrix);		
	}
	myshell.BeginFaceIteration();
	int flen;
	int *face;
	HPoint mid;
	int lastkey[10000];
	for (i=0;i<10000;i++)
		lastkey[i] = -1;
	int storedkeys = 0;
	while ((flen = myshell.NextFace(&face)) != 0)
	{
		
		myshell.CalculateFaceCenter(face, flen, &mid);
		HPoint targetquadrant, quadrant;
		CalculateQuadrant(mid, totalmin, totaldeltadiv, targetquadrant);		
		if (targetquadrant.x >= numx)
			targetquadrant.x = numx-1;
		if (targetquadrant.y >= numy)
			targetquadrant.y = numy-1;
		if (targetquadrant.z >= numz)
			targetquadrant.z = numz-1;
		bool inside = true;
		

/*		for (i=0;i<flen;i++)
		{
			CalculateQuadrant(myshell.m_Points[face[i]], totalmin, totaldeltadiv, quadrant);
			if (!quadrant.Equal(targetquadrant))
			{
 				if (!DoCut)	
				{
					HPoint boxcenter, delta;
					boxcenter.Set(targetquadrant.x * totaldeltadiv.x + totaldeltadiv.x/2.0f,
						targetquadrant.y * totaldeltadiv.y + totaldeltadiv.y/2.0f, 
						targetquadrant.z * totaldeltadiv.z + totaldeltadiv.z/2.0f);
					delta = myshell.m_Points[face[i]] - boxcenter;
					if (HC_Compute_Vector_Length(&delta) > boxdiagonallength)
					{
						inside = false;
						break;
					}
				}
				else
				{
					inside = false;					
					break;
				}
			}
		}
		*/
		int shellindex = ((int)targetquadrant.x) + ((int)targetquadrant.y * numx) + ((int)targetquadrant.z * numx * numy);

		if (inside)
		{
 			shellmatrix[shellindex].AddFace(face, flen, regionnum, myshell.m_Points, myshell.m_VNormals, false);
			shellmatrix[shellindex].SetEmptyShell(newkey, true);

			bool alreadyin = false;
			for (i=0;i<storedkeys;i++)
			{
				if (lastkey[i] == shellindex)
					alreadyin = true;
			}
			if (!alreadyin)
			{
 				mapper->AddRegion(myshell.m_Key, shellmatrix[shellindex].m_Key, regionnum);
				lastkey[storedkeys++] = shellindex;
			}

 			
		}
		else
		{
			if (DoCut)
			{			
				DivideFaceAgainstBox(face, flen,myshell.m_Points, targetquadrant, totaldeltadiv, totalmin,
					shellmatrix[shellindex], leftover);
			}
			else
				leftover.AddFace(face, flen, regionnum,myshell.m_Points, myshell.m_VNormals, false);
		}
	}
	
}



 void HShellWrapper::FindExtents(HPoint &center, HPoint &min, HPoint &max)
{
	min = m_Points[0];
	max = m_Points[0];
	for (int i=0;i<m_plen;i++)
	{
		if (m_Points[i].x < min.x)
			min.x = m_Points[i].x;
		if (m_Points[i].y < min.y)
			min.y = m_Points[i].y;
		if (m_Points[i].z < min.z)
			min.z = m_Points[i].z;
		if (m_Points[i].x > max.x)
			max.x = m_Points[i].x;
		if (m_Points[i].y > max.y)
			max.y = m_Points[i].y;
		if (m_Points[i].z > max.z)
			max.z = m_Points[i].z;
	}
	center.Set((min.x + max.x)/2.0f,(min.y + max.y)/2.0f,(min.z + max.z)/2.0f);

}


void HShellWrapper::MakePolygon(int facelen, int *face, HPoint *points)
{
	for (int i=0;i<facelen;i++)
		points[i] = m_Points[face[i]];
}



void HUtility::SubdivideShells(HC_KEY sskey, HC_KEY startkey, HC_KEY newkey, int numx, int numy, int numz, ShellToRegionMapper *mapper,bool DoCut, bool DeleteOriginalShells, bool breakShells)
{
	HC_KEY shellkey;
 	
	int i;
	HPoint totalmin, totalmax, totaldelta, totaldeltadiv;

	HShellWrapper *shellmatrix = new HShellWrapper[numx * numy * numz];
	vlist_s **unbrokenlist = new vlist_s*[numx * numy * numz];
	for (i=0;i<numx * numy * numz;i++)
		unbrokenlist[i] = new_vlist(malloc,free);
	HC_Open_Segment_By_Key(sskey);
	HC_Compute_Circumcuboid(".", &totalmin, &totalmax);
	HC_Close_Segment();

	HC_Open_Segment_By_Key(startkey);

 	totaldelta = totalmax - totalmin;
	totaldeltadiv.Set(totaldelta.x / numx, totaldelta.y / numy, totaldelta.z / numz);
	float boxdiagonallength = HC_Compute_Vector_Length(&totaldeltadiv);

	int regionnum = 0;
	HShellWrapper *leftover = new HShellWrapper();
	HC_Begin_Contents_Search("...", "shells");
	while (HUtility::FindContents(shellkey))
	{
		HShellWrapper myshell(shellkey);
		bool doSub = true;
		if (!breakShells)
		{
			HPoint min,max,center;
			HPoint targetquadrant,tempq;
			doSub  = false;
			myshell.FindExtents(center, min,max);
			CalculateQuadrant(center, totalmin, totaldeltadiv, targetquadrant);	
			CalculateQuadrant(min, totalmin, totaldeltadiv, tempq);	
			if (0 && !tempq.Equal(targetquadrant))
				doSub = true;
			else
			{
				CalculateQuadrant(max, totalmin, totaldeltadiv, tempq);	

			if (0 && !tempq.Equal(targetquadrant))
					doSub = true;
			}
			if (!doSub)
			{
				int iii = ((int)targetquadrant.x) + ((int)targetquadrant.y * numx) + ((int)targetquadrant.z * numx * numy);
				vlist_add_last(unbrokenlist[iii], (void *)shellkey);
			}
		}

 		if (doSub)
 			SubdivideOneShell(myshell,numx, numy, numz,totalmin,totaldeltadiv,shellmatrix,*leftover, boxdiagonallength,DoCut, regionnum++, mapper, newkey);
		if (DeleteOriginalShells && doSub)
			HC_Delete_By_Key(shellkey);
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	if (DoCut)
	{	
	HShellWrapper *newleftover;
	while (1)
	{
		newleftover = new HShellWrapper();
 		SubdivideOneShell(*leftover,numx, numy, numz,totalmin,totaldeltadiv,shellmatrix,*newleftover, boxdiagonallength,DoCut,-1, mapper, newkey);
		if (newleftover->m_flen == 0)
		{
			delete leftover;
			delete newleftover;
			break;
		}
		delete leftover;
		leftover = newleftover;
	}
	}
	HC_Open_Segment_By_Key(newkey);
	for (i=0;i<(numx*numy*numz);i++)
	{		
		if (!breakShells)
		{		
			HC_KEY savethat = HC_KCreate_Segment("");
			shellmatrix[i].Regenerate(savethat, false);
			if (shellmatrix[i].m_Points)
				shellmatrix[i].Optimize((char*)"tolerance = 0.0001oru");
			START_LIST_ITERATION(HC_KEY, unbrokenlist[i]);
			HC_Open_Segment_By_Key(savethat);
			HC_Open_Segment("");
			HC_Move_By_Key((HC_KEY)temp,".");
			HC_Close_Segment();
			HC_Close_Segment();
			END_LIST_ITERATION(unbrokenlist[i]);
			HC_Open_Segment_By_Key(savethat);
			if (!HC_Show_Existence("segments"))
			{
				HC_Close_Segment();
				HC_Delete_By_Key(savethat);
			}
			else
				HC_Close_Segment();
			
			vlist_flush(unbrokenlist[i]);
			delete_vlist(unbrokenlist[i]);
		}
		else
		{
			shellmatrix[i].Regenerate(newkey, true, true);
			if (shellmatrix[i].m_Points)
				shellmatrix[i].Optimize((char*)"tolerance = 0.0001oru");	
		}


	}
	if (!DoCut)
	{
		leftover->Regenerate(newkey);
		if (leftover->m_Points)
		leftover->Optimize((char*)"tolerance = 0.0001oru");
		delete leftover;
	}
 	HC_Close_Segment();


	

	delete [] shellmatrix;
	delete [] unbrokenlist;

}


HUserOptions::HUserOptions()
{
	m_length = 0;
	m_data = 0;
	m_valid = false;
}

HUserOptions::HUserOptions(const HUserOptions& in)
{
	clear();
	m_valid = in.m_valid;
	m_length = in.m_length;
	if (m_length > 0){
		m_data = new char[m_length+1];
		strcpy(m_data, in.m_data);
	}
	if(!m_data) {
		m_length = (int)strlen("") +1;
		m_data = new char[m_length];
		strcpy(m_data, "");
		m_valid = false;
	}
}

HUserOptions::~HUserOptions()
{
	clear();
}

bool HUserOptions::IsValid()
{
	return m_valid;
}

const char * HUserOptions::Show_User_Options()
{
	clear();

	if (HC_Show_Existence("user options")){
		clear();
		HC_Show_User_Options_Length(&m_length);
		if (m_length > 0){
			m_data = new char[m_length+1];
			HC_Show_User_Options(m_data);
			m_valid = true;
		}
	}
	if(!m_data) {
		m_length = (int)strlen("") +1;
		m_data = new char[m_length];
		strcpy(m_data, "");
		m_valid = false;
	}

	return m_data;
}


const char * HUserOptions::Show_One_Net_User_Option(const char * which)
{
	Show_Net_User_Options();

	if (m_data && strstr(m_data, which)){
		clear();
		HC_Show_One_Net_User_Opt_Len(which, &m_length);
		if (m_length > 0){
			m_data = new char[m_length+1];
			HC_Show_One_Net_User_Option(which, m_data);
			m_valid = true;
		}
	} else {
		clear();
	}
	if(!m_data) {
		m_length = (int)strlen("") +1;
		m_data = new char[m_length];
		strcpy(m_data, "");
		m_valid = false;
	}

	return m_data;
}

const char * HUserOptions::Show_One_User_Option(const char * which)
{
	clear();

	if (HC_Show_Existence(H_FORMAT_TEXT("user options = %s", which))){
		HC_Show_One_User_Option_Length(which, &m_length);
		if (m_length > 0){
			m_data = new char[m_length+1];
			HC_Show_One_User_Option(which, m_data);
			m_valid = true;
		}
	}
	if(!m_data) {
		m_length = (int)strlen("") +1;
		m_data = new char[m_length];
		strcpy(m_data, "");
		m_valid = false;
	}

	return m_data;
}

const char * HUserOptions::Show_Net_User_Options()
{
	clear();

	HC_Show_Net_User_Opt_Length(&m_length);
	if (m_length > 0){
		m_data = new char[m_length+1];
		HC_Show_Net_User_Options(m_data);
		m_valid = true;
	}
	if(!m_data) {
		m_length = (int)strlen("") +1;
		m_data = new char[m_length];
		strcpy(m_data, "");
		m_valid = false;
	}

	return m_data;
}

TK_Dispatch_XML * HBaseModel::GetDispatchXML()
{
    if (!m_pDispatchXML)
	m_pDispatchXML = new TK_Dispatch_XML(this);
    return m_pDispatchXML;
}


TK_Dispatch_XML::TK_Dispatch_XML(HBaseModel * model) : TK_XML() 
{ 
    m_pModel = model;
    m_XMLHandlerList  = new_vlist(malloc,free);
    m_bCloned = false;
     
}


TK_Dispatch_XML::~TK_Dispatch_XML() 
{ 
    if (!m_bCloned)
    {	
	START_LIST_ITERATION(TK_XML, m_XMLHandlerList);
	delete temp;
	END_LIST_ITERATION(m_XMLHandlerList);
    }
    delete_vlist(m_XMLHandlerList);
}


void TK_Dispatch_XML::AddHandler(TK_XML *handler)
{
    vlist_add_last(m_XMLHandlerList, handler);
}



TK_Status   TK_Dispatch_XML::Interpret(BStreamFileToolkit &tk, ID_Key key, int variant) 
{ 
    m_pModel->GetDispatchXML()->m_bStopIteration = false;
    START_LIST_ITERATION(TK_XML, m_XMLHandlerList);
	temp->Interpret(tk, key, variant);
	if (m_pModel->GetDispatchXML()->m_bStopIteration)
	    break;
    END_LIST_ITERATION(m_XMLHandlerList);
    
 return TK_Normal;    
}
 
TK_Status   TK_Dispatch_XML::Execute (BStreamFileToolkit & tk) 
{
    m_pModel->GetDispatchXML()->m_bStopIteration = false;

     START_LIST_ITERATION(TK_XML, m_XMLHandlerList);
	temp->Execute(tk);
	if (m_pModel->GetDispatchXML()->m_bStopIteration)
	    break;
     END_LIST_ITERATION(m_XMLHandlerList);
    return TK_Normal;
}

TK_Status   TK_Dispatch_XML::Read(BStreamFileToolkit & tk) 
{
    TK_Status status;
    status = TK_XML::Read(tk);
    if (status == TK_Normal)
    {
	START_LIST_ITERATION(TK_XML, m_XMLHandlerList);
	temp->SetXML (m_size,m_data);
	END_LIST_ITERATION(m_XMLHandlerList);
    }
    return status;
}


TK_Dispatch_XML * TK_Dispatch_XML::MakeCopy()
{
    TK_Dispatch_XML *handler = new TK_Dispatch_XML(m_pModel);
    handler->m_bCloned = true;
     START_LIST_ITERATION(TK_XML, m_XMLHandlerList);
	vlist_add_last(handler->m_XMLHandlerList, temp);
     END_LIST_ITERATION(m_XMLHandlerList);
     return handler;
}



 
bool HCamera::CameraDifferent(HCamera &cam)
{

	if (!cam.position.Equal(position))
		return true;
	if (!cam.target.Equal(target))
		return true;
	if (!cam.up_vector.Equal(up_vector))
		return true;
	if (cam.field_width != field_width || cam.field_height != field_height)
		return true;
	if (strcmp(cam.projection, projection) != 0)
		return true;
	return false;
	
}

bool HCamera::CameraDifferentByAngle(HCamera &cam)
{

	HPoint delta1,delta2;
	delta1.Set(cam.target.x - cam.position.x, cam.target.y - cam.position.y, 
		cam.target.z - cam.position.z);
	delta2.Set(target.x - position.x, target.y - position.y, target.z - position.z);
	float angle = HUtility::ComputeAngleBetweenVector(delta1,delta2);
	if (angle > 8.0f)
		return true;
	
	if ((cam.field_width >= field_width+0.1f && cam.field_width <= field_width-0.1f) || (cam.field_height >= field_height + 0.1f && 
		cam.field_height <= field_height - 0.1f))
		return true;
	if (strcmp(cam.projection, projection) != 0)
		return true;
	return false;
	
}


void HCamera::GetFromView(HBaseView *view)
{
	HC_Open_Segment_By_Key(view->GetSceneKey());
		HC_Show_Net_Camera (&position, &target, &up_vector, &field_width, &field_height,projection);
	HC_Close_Segment();
	valid = true;
}


void HMergeShellFace::Init(int *flist)
{
	m_flen =  flist[0];
	m_pFlist = new int[m_flen];
	for (int i=0;i<m_flen;i++)
		m_pFlist[i] = flist[i+1];
}

void HMergeShellFace::Divide(int pnum, HPoint *points)
{
	HPoint newpoint;
	newpoint.Set((points[m_pFlist[0]].x + points[m_pFlist[1]].x) / 2.0f,
		(points[m_pFlist[0]].y + points[m_pFlist[1]].y) / 2.0f,
		(points[m_pFlist[0]].z + points[m_pFlist[1]].z) / 2.0f);

	points[pnum] = newpoint;
	m_pChildFaces = new HMergeShellFace[2];
 	int newflist[4];
	newflist[0] = 3;
	newflist[3] = pnum;
	newflist[2] = m_pFlist[2];
	newflist[1] = m_pFlist[1];		
	m_pChildFaces[0].Init(newflist);
	if (m_pChildFaces[0].CheckIfColinear(points))
	{
		m_pChildFaces[0].m_flen = 0;
	}
	newflist[0] = 3;
	newflist[3] = pnum;
	newflist[2] = m_pFlist[0];
	newflist[1] = m_pFlist[2];		
	m_pChildFaces[1].Init(newflist);
	if (m_pChildFaces[1].CheckIfColinear(points))
	{
		m_pChildFaces[1].m_flen = 0;
	}

	m_ChildFaceNum = 2;
	
 
}

void HMergeShellFace::MakeChildFaces(int &addpoint, HPoint *points, bool AddExtraPoints, int *newpoints, int *newpointslen)
{
	int idx;
	int newflist[4];
	m_pChildFaces = new HMergeShellFace[m_flen];
	int pnum = addpoint+1;
	m_ChildFaceNum = m_flen;
	if (newpoints)
		newpoints[(*newpointslen)++] = addpoint;
	for (int i=0;i<m_ChildFaceNum;i++)
	{
		if (i<m_flen-1)
			idx = i + 1;
		else
			idx = 0;
		
		newflist[0] = 3;
		newflist[1] = m_pFlist[i];
		newflist[2] = m_pFlist[idx];
		newflist[3] = addpoint;		
		m_pChildFaces[i].Init(newflist);
		if (m_pChildFaces[i].CheckIfColinear(points))
		{
 			m_pChildFaces[i].m_flen = 0;
		}
		else
		{		
			if (AddExtraPoints)
			{			
				m_pChildFaces[i].Divide(pnum, points);
				if (newpoints)
					newpoints[(*newpointslen)++] = pnum;
				pnum++;
			}
		}
	}
	addpoint = pnum;
}

bool HMergeShellFace::CheckIfColinear(HPoint *point)
{
	int i1,i2,i;
	HPoint delta, prevdelta;
	prevdelta.Set(point[m_pFlist[1]].x - point[m_pFlist[0]].x,
		point[m_pFlist[1]].y - point[m_pFlist[0]].y,point[m_pFlist[1]].z - point[m_pFlist[0]].z);
	HC_Compute_Normalized_Vector(&prevdelta,&prevdelta);
	for (i=1;i<m_flen-1;i++)
	{
		i1=i;
		i2=i+1;	 
		HPoint delta;
		delta.Set(point[m_pFlist[i2]].x - point[m_pFlist[i1]].x,
			point[m_pFlist[i2]].y - point[m_pFlist[i1]].y,point[m_pFlist[i2]].z - point[m_pFlist[i1]].z);
		HC_Compute_Normalized_Vector(&delta,&delta);

		HPoint deltainv(-delta.x, -delta.y, -delta.z);
		if (!delta.Equal(prevdelta) && !deltainv.Equal(prevdelta))
			break;
		delta = prevdelta;
	}
	if (i!=m_flen-1)
		return false;
	else
		return true;
}

bool HMergeShellFace::CheckHit(HPoint &p, HPoint &normal, HPoint *plist, HPoint &pres)
{
	HPoint pps[256];
	
	for (int j=0;j<m_flen;j++)
		pps[j] = plist[m_pFlist[j]]; 	
	HPoint pn;
	HPlane plane;
	HC_Compute_Polygon_Plane(m_flen, pps, "right",&plane);
	pn.Set(p.x + normal.x * 100.0f, p.y + normal.y * 100.0f, p.z + normal.z * 100.0f);
	HUtility::IntersectionLinePlane(&p,&pn,&plane, &pres);
	
	if (HUtility::IsPointInsideAllEdges((float *)&pres, (float *)plist,m_flen, m_pFlist, (float *)&plane,0.0f))
		return true;
	
	return false;
	
	
}


HMergeShellFace * HMergeShellFace::FindHitFaceRecursive(HPoint &p, HPoint &normal, HPoint *plist, HPoint &respoint)
{
	HMergeShellFace *result = 0;
	if (!m_pChildFaces)
	{
		if (CheckHit(p,normal,plist, respoint))
			return this;
		else
			return 0;
	}
	for (int i=0;i<m_ChildFaceNum;i++)
	{
		if (m_pChildFaces[i].m_flen)
		{		
			result = m_pChildFaces[i].FindHitFaceRecursive(p,normal,plist,respoint);
			if (result)
				break;
		}
		
	}
	return result;
	
}

int HMergeShellFace::GetLengthRecursive(int l)
{
	if (m_pChildFaces)
	{
		for (int i=0;i<m_ChildFaceNum;i++)
		{
			if (m_pChildFaces[i].m_flen)
				l = m_pChildFaces[i].GetLengthRecursive(l);
		}
		return l;
	}
	else
		return l + m_flen + 1;
}

int HMergeShellFace::AddToFacelistRecursive(int *start, int l)
{
	if (m_pChildFaces)
	{
		for (int i=0;i<m_ChildFaceNum;i++)
		{
			if (m_pChildFaces[i].m_flen)
				l = m_pChildFaces[i].AddToFacelistRecursive(start, l);
		}
		return l;
	}
	else
	{
		start[l] = m_flen;
		for (int i=0;i<m_flen;i++)
			start[l+i+1] = m_pFlist[i];
		return l + m_flen + 1;
	}

}


void HShellWrapper::SetEmptyShell(HC_KEY segkey, bool MakeSegment)
{
	if (m_Key!= -1)
		return;
	HC_Open_Segment_By_Key(segkey);
	if (MakeSegment)
		HC_Open_Segment("");
	m_Key = HC_KInsert_Shell(0,0,0,0);

	if (MakeSegment)
		HC_Close_Segment();
	HC_Close_Segment();

}

void HShellWrapper::BeginFaceIteration()
{
	m_facepointer = 0;
}

int HShellWrapper::NextFace(int **face)
{
	int facenum;
	if (m_facepointer>=m_flen)
		return 0;
	*face = &m_Flist[m_facepointer+1];
	facenum = m_Flist[m_facepointer];
	m_facepointer+= (abs(facenum) + 1);
	return facenum;
}

int HShellWrapper::GetFace(int facenum, int **face)
{
	int flen;
	BeginFaceIteration();
	for (int i=0;i<facenum;i++)
		flen = NextFace(face);

	flen = NextFace(face);
	return flen;
}



void HShellWrapper::TransformPoints(float *matrix)
{
	if (!m_plen)
		Show(0,0,0,0);
	HC_Compute_Transformed_Points(m_plen, m_Points, matrix, m_Points);
	HC_Edit_Shell_Points(m_Key, 0, m_plen, m_plen, m_Points);

}

HC_KEY HShellWrapper::Optimize2(char *options)
{
	int plen,flen;
	HPoint *points = new HPoint[m_plen];
	int *faces = new int[m_flen];
	int *fmap = new int[m_flen];

	HC_Compute_Optimized_Shell(m_plen, m_Points, 0, m_flen, m_Flist, options, &plen, points, &flen, faces,0,fmap);
	HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_Key));
 	HC_KEY key2 = HC_KInsert_Shell(plen, points, flen, faces);
	HC_Close_Segment();

	bool hasColor = false;
	
	HC_Open_Geometry(m_Key);
	HC_Open_Face(0);
 	if (HC_Show_Existence("color"))	
 	 	hasColor = true;
	HC_Close_Face();
	HC_Close_Geometry();
	
	if (hasColor && m_plen > 20)
	{
 		char text[MVO_BUFFER_SIZE];
		int cmcount;
		HPoint colormap[500];
 		HC_Show_Net_Color_Map_By_Value(text, &cmcount, colormap);
		int fp = 0;
		int fnum = 0;
		while (1)
		{		

			if (fmap[fnum]!= -1)
			{	
				bool colorFound = false;
				int ind;
				HC_Open_Geometry(m_Key);
				HC_Open_Face(fnum);
				if (HC_Show_Existence("color"))
					colorFound = true;
		 		HC_Show_Color_By_Index(text, &ind);
				HC_Close_Face();
				HC_Close_Geometry();
				HC_Open_Geometry(key2);
				HC_Open_Face(fmap[fnum]);
				if (colorFound)
			 	HC_Set_Color_By_Index("faces", ind);
 				HC_Close_Face();
				HC_Close_Geometry();
			}

			fp+=m_Flist[fp]+1;
			if (fp>=m_flen)
				break;
			fnum++;

		}
	}
	HC_Delete_By_Key(m_Key);
	delete [] m_Points;
	delete [] m_Flist;

	m_Points = points;
	m_Flist = faces;
	m_plen = plen;
	m_flen = flen;
	m_Key = key2;
 
	delete [] fmap;

	return m_Key;

}

HC_KEY HShellWrapper::Optimize(char *options)
{
	int plen,flen,i;
	HPoint *points = new HPoint[m_plen];
	HPoint *normals = 0;
 	int *faces = new int[m_flen];
	int *fmap = new int[m_flen];
	int *newregions = new int[m_flen];
	int *vertex_map = new int[m_plen];

	HC_Compute_Optimized_Shell(m_plen, m_Points, m_VNormals, m_flen, m_Flist, options, &plen, points, &flen, faces,vertex_map,fmap);

	int newnumfaces = 0;
	HC_Open_Geometry(m_Key);
	for (i=0;i<m_numfaceslen;i++)
	{

		int r;
		HC_Open_Face(i);
		HC_Show_Region(&r);
		HC_Close_Face();
		if (fmap[i] != -1)
		{
			newregions[fmap[i]] = m_faceregion[i];
			newnumfaces++;
		}
		
		
	}
	HC_Close_Geometry();

	if (m_VNormals)
	{	
		normals = new HPoint[m_plen];

		HC_Open_Geometry(m_Key);
		for (i=0;i<m_plen;i++)
		{
			
			HPoint n;
			HC_Open_Vertex(i);
			HC_Show_Normal(&n.x, &n.y, &n.z);
			HC_Close_Vertex();
			if (vertex_map[i] != -1)
			{
				normals[vertex_map[i]] = m_VNormals[i];
			}				
		}
		HC_Close_Geometry();
	}


	HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_Key));
	Replace(plen, points, flen, faces, normals);
  	HC_Close_Segment();
 


	float *vc = new float[m_plen];
//	ShowVertexColors(vc);

//	HC_Open_Geometry(key2);
//	for (int i=0;i<m_plen;i++)
//	{
//		HC_Open_Vertex(vertex_map[i]);
//		HC_Set_Color_By_FIndex("faces", vc[i]);
//		HC_Close_Vertex();
//	}
//	HC_Close_Geometry();

	delete [] vc; 
	delete [] vertex_map;
	delete [] normals;
	 
//	HC_Delete_By_Key(m_Key);
	delete [] m_Points;
	delete [] m_Flist;

	m_Points = points;
	m_Flist = faces;
	m_plen = plen;
	m_flen = flen;
//	m_Key = key2;


	HC_MSet_Face_Regions(m_Key, 0, newnumfaces, newregions);

//  	HC_Open_Geometry(m_Key);
//	for (i = 0;i<newnumfaces;i++)
//	{
//		HC_Open_Face(i);
//		HC_Set_Region(newregions[i]);
//		HC_Close_Face();
//	}
//	HC_Close_Geometry();

	m_numfaceslen = newnumfaces;
	delete [] newregions;
	
	delete [] fmap;

	return m_Key;

}

HC_KEY HShellWrapper::Insert(int plen, HPoint *points, int flen, int *flist, HC_KEY insertkey)
{
	if (insertkey!=-1)
 		HC_Open_Segment_By_Key(insertkey);
	m_Key = HC_KInsert_Shell(plen, points, flen, flist);
	if (insertkey!=-1)
 		HC_Close_Segment();
	return m_Key;
}

HC_KEY HShellWrapper::Show(int *plen, HPoint **points, int *flen, int **flist)
{

	if (m_Points)
		delete [] m_Points;
	if (m_Flist)
		delete [] m_Flist;
	if (m_VNormals)
		delete [] m_VNormals;
	HC_Show_Shell_Size(m_Key, &m_plen, &m_flen); 	
	m_totalplen = m_plen;
	m_totalflen = m_flen;

	m_Points = new HPoint[m_plen];	
 	m_Flist = new int[m_flen];
 	m_VNormals = 0;

	bool ex = false;
	HC_Show_Shell(m_Key, &m_plen, m_Points, &m_flen, m_Flist);
	if (m_plen)
	{
		HC_Open_Geometry(m_Key);
			HC_Open_Vertex(0);
				ex = (HC_Show_Existence("normal") != 0);
			HC_Close_Vertex();
		HC_Close_Geometry();
	}

	if (ex)
	{
 		m_VNormals = new HPoint[m_plen];
		HC_MShow_Vertex_Normals(m_Key, 0, m_plen, m_VNormals);
	}

	if (points)
		*points = m_Points;
	if (flist)
		*flist = m_Flist;
	if (plen)
		*plen = m_plen;
	if (flen)
		*flen = m_flen;

	return m_Key;

}


void HShellWrapper::Set(HC_KEY key)
{
	m_Key = key;
	if (m_Key == INVALID_KEY)
		m_Key = HC_KInsert_Shell(0,0,0,0);

	Show(0,0,0,0);
}

void HShellWrapper::OpenOwnerSegment()
{
	HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_Key));
}



void HShellWrapper::CalculateFaceCenter(int *face, int flen, HPoint *mid)
{

	HPoint total, min,max;
	min.Set(m_Points[face[0]].x, m_Points[face[0]].y,m_Points[face[0]].z);
	max.Set(m_Points[face[0]].x, m_Points[face[0]].y,m_Points[face[0]].z);


	for (int i=1;i<flen;i++)
	{
		if (m_Points[face[i]].x < min.x)
			min.x = m_Points[face[i]].x;
		if (m_Points[face[i]].y < min.y)
			min.y= m_Points[face[i]].y;
		if (m_Points[face[i]].z < min.z)
			min.z= m_Points[face[i]].z;

		if (m_Points[face[i]].x > max.x)
			max.x = m_Points[face[i]].x;
		if (m_Points[face[i]].y > max.y)
			max.y= m_Points[face[i]].y;
		if (m_Points[face[i]].z > max.z)
			max.z= m_Points[face[i]].z;
	}
	total.Set((max.x + min.x)/2.0f, (max.y + min.y)/2.0f, (max.z + min.z)/2.0f);
 	*mid = total;
}
	
 
void HShellWrapper::Replace(int plen, HPoint *points, int flen, int *flist, HPoint *normals, bool emptyshell)
{
	ReplaceWithParameters(plen, points, flen, flist, normals,0,0,0, emptyshell);
}


void HShellWrapper::ReplaceWithParameters(int plen, HPoint *points, int flen, int *flist, HPoint *normals, float *params, int paranum, bool emptyshell)
{
	HShellWrapper::ReplaceWithParameters(plen, points, flen, flist, normals, params, paranum, 0, emptyshell);
}

void HShellWrapper::ReplaceWithParameters(int plen, HPoint *points, int flen, int *flist, HPoint *normals, float *params, int paranum, HPoint *vcolors, bool emptyshell)
{

 	if (emptyshell)
	{
		HC_Edit_Shell_Points(m_Key, 0, 0,plen, points);
		HC_Edit_Shell_Faces(m_Key, 0, 0,flen,flist);
		if (normals)
			HC_MSet_Vertex_Normals(m_Key, 0, plen, normals);
		if (params && paranum == 3)
			HC_MSet_Vertex_Parameters(m_Key, 0, plen,3, params);
		if (vcolors)
			HC_MSet_Vertex_Colors_By_Value(m_Key, "faces", 0,"rgb", plen,vcolors);

  		m_plen = plen;
		m_flen = flen;
		
	}
	else
	{	
		int pl1,fl1;
		int pl2,fl2;
		HC_Show_Shell_Size(m_Key, &pl1, &fl1);
		HC_Edit_Shell_Points(m_Key, 0, pl1,plen, points);
		HC_Show_Shell_Size(m_Key, &pl2, &fl2);
		HC_Show_Shell_Face_Count(m_Key, &fl2);
		HC_Edit_Shell_Faces(m_Key, 0, fl2,flen,flist);
		if (normals)
			HC_MSet_Vertex_Normals(m_Key, 0, plen, normals);
		if (params && paranum == 3)
			HC_MSet_Vertex_Parameters(m_Key, 0, plen, 3,(float *)params);

	}
	m_plen = plen;
	m_flen = flen;

}

void HShellWrapper::Regenerate(HC_KEY newowner, bool makeSegment, bool replace)
{
	bool emptyshell = false;
	if (m_plen)
	{	
		if (m_Key != INVALID_KEY)
		{
			if (replace)
			{
				int plen, flen;
				HC_Show_Shell_Size(m_Key, &plen, &flen);
				if (!plen)
					emptyshell = true;

			}
			else
				HC_Delete_By_Key(m_Key);
		}

		if (newowner != INVALID_KEY)
		{		
			HC_Open_Segment_By_Key(newowner);
			if (makeSegment && !replace)
				HC_Open_Segment("");
		}
		if (replace)		
 				ReplaceWithParameters(m_plen, m_Points, m_flen, m_Flist, m_VNormals, (float *)m_VParams,3, m_VColors, emptyshell);
		else
			m_Key = HC_KInsert_Shell(m_plen, m_Points, m_flen, m_Flist);
		if (newowner != INVALID_KEY)
		{		

			if (makeSegment && !replace)
				HC_Close_Segment();
			HC_Close_Segment();
		}
	}
	if (m_faceregion && 0)
	{
		HC_MSet_Face_Regions(m_Key, 0, m_numfaceslen, m_faceregion);
//		HC_Open_Geometry(m_Key);
//		for (int i=0;i<m_numfaceslen;i++)
//		{
//			HC_Open_Face(i);
//			HC_Set_Region(m_faceregion[i]);
//			HC_Close_Face();
//			
//		}
//		HC_Close_Geometry();
	}
}


void HShellWrapper::GrowPointArray(int fnum, bool growparamarray, bool grownormalarray)
{
	if (m_plen + fnum +1 >= m_totalplen)
	{
		int newsize = 2*m_totalplen + 100;
		if (newsize <= (m_plen + fnum))
			newsize = m_plen + fnum;
		HPoint *newpointslist = new HPoint[newsize];
		for (int i=0;i<m_plen;i++)
			newpointslist[i] = m_Points[i];
		delete [] m_Points;
		m_Points = newpointslist;
		m_totalplen = newsize;

		if (growparamarray)
		{		
			HPoint *newpointslist = new HPoint[newsize];
			for (int i=0;i<m_plen;i++)
				newpointslist[i] = m_VParams[i];
			delete [] m_VParams;
			m_VParams = newpointslist;
		}
		if (grownormalarray)
		{		
			HPoint *newpointslist = new HPoint[newsize];
			for (int i=0;i<m_plen;i++)
				newpointslist[i] = m_VNormals[i];
			delete [] m_VNormals;
			m_VNormals = newpointslist;
		}
	

	}
}


void HShellWrapper::GrowPointArray2(int fnum, bool growparamarray, bool grownormalarray, bool growfnarray, bool growcolorarray)
{
	if (m_plen + fnum +1 >= m_totalplen)
	{
		int newsize = 2*m_totalplen + 100;
		if (newsize <= (m_plen + fnum))
			newsize = m_plen + fnum;
		HPoint *newpointslist = new HPoint[newsize];
		for (int i=0;i<m_plen;i++)
			newpointslist[i] = m_Points[i];
		delete [] m_Points;
		m_Points = newpointslist;
		m_totalplen = newsize;

		if (growparamarray)
		{		
			HPoint *newpointslist = new HPoint[newsize];
			for (int i=0;i<m_plen;i++)
				newpointslist[i] = m_VParams[i];
			delete [] m_VParams;
			m_VParams = newpointslist;
		}
		if (growcolorarray)
		{		
			HPoint *newpointslist = new HPoint[newsize];
			for (int i=0;i<m_plen;i++)
				newpointslist[i] = m_VColors[i];
			delete [] m_VColors;
			m_VColors = newpointslist;
		}
		if (grownormalarray)
		{		
			HPoint *newpointslist = new HPoint[newsize];
			for (int i=0;i<m_plen;i++)
				newpointslist[i] = m_VNormals[i];
			delete [] m_VNormals;
			m_VNormals = newpointslist;
		}
		if (growfnarray)
		{		
			FaceWithNormalInfo *newfnlist = new FaceWithNormalInfo[newsize];
			for (int i=0;i<m_plen;i++)
				newfnlist[i] = m_FNArray[i];
			delete [] m_FNArray;
			m_FNArray = newfnlist;
		}

	}
}


void HShellWrapper::GrowArrays(int fnum)
{
 
	GrowPointArray(fnum);
	GrowFaceArray(fnum);


}

void HShellWrapper::SetFaceRegion(int regionnum)
{
 	if (m_numfaceslen + 1>= m_totalnumfaceslen)
	{
		int newsize = 2*m_totalnumfaceslen + 100;
		int *newfacelist = new int[newsize];
		for (int i=0;i<m_numfaceslen;i++)
			newfacelist[i] = m_faceregion[i];
		if (m_faceregion)
			delete [] m_faceregion;
		m_faceregion = newfacelist;
		m_totalnumfaceslen = newsize;
	}
	m_faceregion[m_numfaceslen] = regionnum;
	m_numfaceslen++;


}


void HShellWrapper::GrowFaceArray(int fnum)
{
 
	if (m_flen + fnum +1>= m_totalflen)
	{
		int newsize = fnum + 2*m_totalflen + 100 ;
		int *newfacelist = new int[newsize];
		for (int i=0;i<m_flen;i++)
			newfacelist[i] = m_Flist[i];
		if (m_Flist)
			delete [] m_Flist;
		m_Flist = newfacelist;
		m_totalflen = newsize;
	}


}

void HShellWrapper::AddFace(HPoint *points, int fnum, bool AddImediatly)
{
	UNREFERENCED(AddImediatly);

	GrowArrays(fnum);
	m_Flist[m_flen] = fnum;
	for (int i=0;i<fnum;i++)
	{
		m_Points[i + m_plen] = points[i];
		m_Flist[i + m_flen + 1] = i+m_plen;
	}
	m_plen += fnum;
	m_flen += (fnum + 1);
}

void HShellWrapper::AddPoints(HPoint *points, int pnum,HPoint *param, HPoint *normals, bool AddImediatly)
{
	UNREFERENCED(AddImediatly);

	GrowPointArray(pnum, param != 0, normals != 0);
 
 	for (int i=0;i<pnum;i++)
	{
		m_Points[i + m_plen] = points[i];
		if (param)
			m_VParams[i + m_plen] = param[i];
		if (normals)
			m_VNormals[i + m_plen] = normals[i];
	}
	m_plen += pnum;
}


void HShellWrapper::SetVertexColors(float *findex)
{
	if (!m_plen)
		Show(0,0,0,0);
	HC_MSet_Vertex_Colors_By_FIndex(m_Key, "face", 0,  m_plen, findex);
}

void HShellWrapper::ShowVertexColors(HPoint *color)
{
	if (!m_plen)
		Show(0,0,0,0);
 	HC_MShow_Vertex_Colors_By_Value(m_Key, "face", 0,  m_plen, color);
}
void HShellWrapper::SetVertexColors(HPoint *color)
{
	if (!m_plen)
		Show(0,0,0,0);
	HC_MSet_Vertex_Colors_By_Value(m_Key, "face", 0, "rgb", m_plen, color);
}

 void HShellWrapper::ShowVertexColors(float *color)
{
	if (!m_plen)
		Show(0,0,0,0);
  	HC_MShow_Vertex_Colors_By_FInd(m_Key, "face", 0,  m_plen, color);
}

void HShellWrapper::BeginAddFaces(int plen)
{
	m_faceindirect = new int[plen];
	for (int i=0;i<plen;i++)
		m_faceindirect[i] = -1;
}
void HShellWrapper::EndAddFaces()
{
	delete [] m_faceindirect;
	m_faceindirect = 0;
}


void HShellWrapper::AddNextFaceWithDistinctNormals(HPoint const * points, HPoint const * normals, int const * face1, int const * face2, int fnum)
{
	int i;
	//HPoint newpoints[256];
	//HPoint newnormals[256];
	int nface[256];

	HPoint facenormal;
	if (!normals)
	{
		HPoint p[256];
		for (int i=0;i<fnum;i++)
			p[i] = points[face1[i]];
		HC_Compute_Polygon_Normal(fnum, p,"right",&facenormal);
	}
 	for (i=0;i<fnum;i++)
	{
		int pindex = face1[i];
		int nindex = face2[i];
		if (m_faceindirect[pindex] == -1)
		{
			GrowPointArray2(1,false,true,true);
			m_Points[m_plen] = points[pindex];
			if (!normals)
				m_VNormals[m_plen] = facenormal;
			else
				m_VNormals[m_plen] = normals[nindex];
			m_FNArray[m_plen].nindex = nindex;
			m_FNArray[m_plen].nextindex = -1;
			m_faceindirect[pindex] = m_plen;
			nface[i] = m_plen;
			m_plen++;
 		}
		else
		{		
			int pfound = m_faceindirect[pindex];
			while (1)
			{
				if (m_FNArray[pfound].nindex == nindex && normals)
				{
					nface[i] = pfound;
					break;
				}
				else
				{
					if (m_FNArray[pfound].nextindex == -1)
					{
						GrowPointArray2(1,false,true,true);
						m_Points[m_plen] = points[pindex];
						if (!normals)
							m_VNormals[m_plen] = facenormal;	
						else
							m_VNormals[m_plen] = normals[nindex];
						m_FNArray[m_plen].nindex = nindex;
						m_FNArray[m_plen].nextindex = -1;
						m_FNArray[pfound].nextindex = m_plen;
 						nface[i] = m_plen;
						m_plen++;
						break;
					}
					else
					{						
						pfound = m_FNArray[pfound].nextindex;
					}
 				}
			}
		}
	}	
	AddFaceDirect(nface, fnum);
}



void HShellWrapper::AddNextFaceWithDistinctNormalsAndTexture(HPoint const *points, HPoint const *normals, HPoint const *texcor,int const *face1, int const *face2, int const *face3, int fnum)
{
	//HPoint newpoints[256];
	//HPoint newnormals[256];
	int nface[256];

 	for (int i = 0; i < fnum; ++i)
	{
		int pindex = face1[i];
		int nindex = face2[i];		
		int tcindex = face3[i];
		if (m_faceindirect[pindex] == -1)
		{
			GrowPointArray2(1,true,true,true);
			m_Points[m_plen] = points[pindex];
			m_VNormals[m_plen] = normals[nindex];
			m_VParams[m_plen] = texcor[tcindex];
			m_FNArray[m_plen].nindex = nindex;
			m_FNArray[m_plen].tcindex = tcindex;
			m_FNArray[m_plen].nextindex = -1;
			m_faceindirect[pindex] = m_plen;
			nface[i] = m_plen;
			m_plen++;
 		}
		else
		{		
			int pfound = m_faceindirect[pindex];
			while (1)
			{
				if (m_FNArray[pfound].nindex == nindex && m_FNArray[pfound].tcindex == tcindex)
				{
					nface[i] = pfound;
					break;
				}
				else
				{
					if (m_FNArray[pfound].nextindex == -1)
					{
						GrowPointArray2(1,true,true,true);
						m_Points[m_plen] = points[pindex];
						m_VNormals[m_plen] = normals[nindex];
						m_VParams[m_plen] = texcor[tcindex];
						m_FNArray[m_plen].nindex = nindex;
						m_FNArray[m_plen].tcindex = tcindex;
						m_FNArray[m_plen].nextindex = -1;
						m_FNArray[pfound].nextindex = m_plen;
 						nface[i] = m_plen;
						m_plen++;
						break;
					}
					else
					{						
						pfound = m_FNArray[pfound].nextindex;
					}
 				}
			}
		}
	}	
	AddFaceDirect(nface, fnum);
}

HShellWrapper::~HShellWrapper() {
		if (m_Points)
			delete [] m_Points;
		if (m_Flist)
			delete [] m_Flist;
		if (m_VParams)
			delete [] m_VParams;
		if (m_VNormals)
			delete [] m_VNormals;
		if (m_VColors)
			delete [] m_VColors;
		if (m_FNArray)
			delete [] m_FNArray;
		if (m_faceindirect)	
			delete [] m_faceindirect;
	}

void HShellWrapper::AddNextFaceWithDistinctNormalsAndColor(HPoint const *points, HPoint const *normals,int const *face1, int const *face2, HPoint *face3, int fnum)
{
	//HPoint newpoints[256];
	//HPoint newnormals[256];
	int nface[256];

 	for (int i = 0; i < fnum; ++i)
	{
		int pindex = face1[i];
		int nindex = face2[i];		
 		if (m_faceindirect[pindex] == -1)
		{
			GrowPointArray2(1,false,true,true,true);
			m_Points[m_plen] = points[pindex];
			m_VNormals[m_plen] = normals[nindex];
 			m_VColors[m_plen] = face3[i];
			m_FNArray[m_plen].nindex = nindex;
 			m_FNArray[m_plen].nextindex = -1;
			m_faceindirect[pindex] = m_plen;
			nface[i] = m_plen;
			m_plen++;
 		}
		else
		{		
			int pfound = m_faceindirect[pindex];
			while (1)
			{
				if (m_FNArray[pfound].nindex == nindex)
				{
					nface[i] = pfound;
					break;
				}
				else
				{
					if (m_FNArray[pfound].nextindex == -1)
					{
						GrowPointArray2(1,false,true,true,true);
						m_Points[m_plen] = points[pindex];
						m_VNormals[m_plen] = normals[nindex];
						m_VColors[m_plen] = face3[i];
						m_FNArray[m_plen].nindex = nindex;
 						m_FNArray[m_plen].nextindex = -1;
						m_FNArray[pfound].nextindex = m_plen;
 						nface[i] = m_plen;
						m_plen++;
						break;
					}
					else
					{						
						pfound = m_FNArray[pfound].nextindex;
					}
 				}
			}
		}
	}	
	AddFaceDirect(nface, fnum);
}

								
								









void HShellWrapper::AddFace(int *face, int fnum, int regionnum, HPoint *points, HPoint *normals, bool AddImediatly)
{
	UNREFERENCED(AddImediatly);

	if (regionnum != -1)
		SetFaceRegion(regionnum);
 	GrowFaceArray(fnum);
	if (points)
		GrowPointArray(fnum, false, normals != 0);
	
	m_Flist[m_flen] = fnum;
	for (int i=0;i<fnum;i++)
	{
		if (points)
			m_Points[i + m_plen] = points[face[i]];
		if (normals)
			m_VNormals[i + m_plen] = normals[face[i]];
		m_Flist[i + m_flen + 1] = i+m_plen;
	}
	if (points)
		m_plen += fnum;
	m_flen += (fnum + 1);
 

}
void HShellWrapper::AddFaceDirect(int *face, int fnum, bool reverse, bool AddImediatly)
{
	UNREFERENCED(AddImediatly);
	 
	GrowFaceArray(fnum);
 	
	m_Flist[m_flen] = fnum;
	if (!reverse)
	{	
		for (int i=0;i<fnum;i++)
		{
			m_Flist[i + m_flen + 1] = face[i];
		}
	}
		else
		{
			for (int i=0;i<fnum;i++)
			{
				m_Flist[i + m_flen + 1] = face[fnum-i-1];
			}
		}
 	m_flen += (fnum + 1);

}



static float BilinearInterpolation(float *p, float u, float v)
{

	//return p[0] + p[1] * u + p[2] * v + p[3] *u *v;

	return  p[0] * u * v + p[1] * (1.0f - u) * v + 
				p[2] *(1.0f - u) * (1.0f - v) + p[3] * u * (1.0f - v);
}
static void BilinearInterpolation(HPoint *p, float u, float v, HPoint &res)
{
	res.x = p[0].x * u * v + p[1].x * (1.0f - u) * v + 
			p[2].x *(1.0f - u) * (1.0f - v) + p[3].x * u * (1.0f - v);
	res.y = p[0].y * u * v + p[1].y * (1.0f - u) * v + 
			p[2].y *(1.0f - u) * (1.0f - v) + p[3].y * u * (1.0f - v);
	res.z = p[0].z * u * v + p[1].z * (1.0f - u) * v + 
			p[2].z *(1.0f - u) * (1.0f - v) + p[3].z * u * (1.0f - v);		
}


int orx[8] = {-1, 0, 1,1,1,0,-1, -1};
int ory[8] = {-1,-1,-1,0,1,1, 1, 0};


static int FixOr(int oor)
{
	if (oor>7)
		oor = oor - 8;
	else if (oor<0)
		oor = 8 + oor;
	return oor;
}
bool SameRounded(float c, float col, float EPSI = 0.0f)
{
	if (c>=col - 0.5f - EPSI && c<= col + 0.5f + EPSI)
		return true;
	else
		return false;
}

static void ClipLine(float u0, float v0, float &u1, float &v1)
{
	HPoint p[2];
	p[0].Set(u0,v0,0);
	p[1].Set(u1,v1,0);
	float top = 1.0f;
	float bottom = 0.0f;
	float left = 0.0f;
	float right = 1.0f;
 
	 
	if ((right - p[0].x) * (right - p[1].x) < 0)
	{ 
		v1 = p[1].y+(p[0].y-p[1].y)*(right- p[1].x) / (p[0].x - p[1].x);
 		u1 = (float)right;
	}
	
   	else if ((left - p[0].x) * (left - p[1].x) < 0)
	{ 
		v1 = p[1].y+(p[0].y-p[1].y)*(left- p[1].x) / (p[0].x - p[1].x);
 		u1 = (float)left;
	}
	 
 	else if ((top - p[0].y) * (top - p[1].y) < 0)
	{ 
		u1 = p[1].x+(p[0].x-p[1].x)*(top- p[1].y) / (p[0].y - p[1].y);
 		v1 = (float)top;
	}
	
 	else if ((bottom - p[0].y) * (bottom - p[1].y) < 0)
	{ 
		u1 = p[1].x+(p[0].x-p[1].x)*(bottom- p[1].y) / (p[0].y - p[1].y);
 		v1 = (float)bottom;
	}

	 
 
}
		

#define EPSI 0.00001f



 
static bool Equal(float u,float v, float epsilon =  0.00001f) 
{ 
	return (u - v < epsilon) && (u - v > -epsilon);
}

static void ApproxSolution(float col, float u1, float v1, float u2, float v2, HPoint *p, float *c, 
					float &ures, float &vres)
{
	UNREFERENCED(p);

	float dx = 0,dy = 0;
	int counter = 0;
	float t1 = BilinearInterpolation(c,u1,v1);
	float t2 = BilinearInterpolation(c,u2,v2);
	
	if (t1 == col)
	{
		dx = u1;
		dy = v1;
		goto approx_end;
	}
	else if (t2 == col)
	{
		dx = u2;
		dy = v2;
		goto approx_end;
	}

   	while (1)
	{	
		dx = (u1+u2)/2.0f;
 		dy = (v1+v2)/2.0f;
 		float c1 = BilinearInterpolation(c,dx,dy);
		if ((t1 <=col && c1 >=col) || (t1 >=col && c1<=col))
		{ 		
 			u2 = dx;
			v2 = dy;
			if (fabs(col - c1) < 0.0001f)
				break;
		}
		else
		{
			if (fabs(col - c1) < 0.0001f)
				break;
			u1 = dx;
			v1 = dy;
		}
		counter++;
		if (counter >30)
			counter = counter;

		if (counter >50)
			break;
	}

approx_end:
	ures = dx;
	vres =dy;
	
	if (Equal(ures,1.0f,0.001f))
		ures = 1.0f;
	if (Equal(ures,0.0f,0.001f))
		ures = 0.0f;
	if (Equal(vres,1.0f, 0.001f))
		vres = 1.0f;
	if (Equal(vres,0.0f,0.001f))
		vres = 0.0f;
	

}



static bool IsOnSide(float u1, float v1, float u2, float v2)
{
	if (Equal(v1,0.0f) && Equal(v2,0.0f))
		return true;
	if (Equal(v1,1.0f) && Equal(v2,1.0f))
		return true;
	if (Equal(u1,1.0f) && Equal(u2,1.0f))
		return true;
	if (Equal(u1,0.0f) && Equal(u2,0.0f))
		return true;

	return false;
}

static bool IsInside(float u,float v)
{
	if (u<0.0f || u > 1.0f || v< 0.0f || v> 1.0f)
		return false;
	else 
		return true;

}

static void ApproximateOnSide(float col, float c1, float c2, 
							  float u1,float v1,float u2,float v2, HPoint *p, float *c, float &ures, float &vres)
{
	UNREFERENCED(c1);
		 
	if (c2<col-0.5f)
		ApproxSolution(col-0.5f, u1,v1,u2,v2,p,c,ures,vres);
	else 
		ApproxSolution(col+0.5f, u1,v1,u2,v2,p,c,ures,vres);
	if (ures >1.0f-EPSI)
		ures = 1.0f;
	if (ures<0.0f+EPSI)
		ures = 0.0f;
	if (vres>1.0f-EPSI)
		vres = 1.0f;
	if (vres<0.0f + EPSI)
		vres = 0.0f;
}


static bool ApproximateOnSide2(float col, float u1,float v1,float u2,float v2, HPoint *p, float *c, float &ures, float &vres)
{

		float c1 = BilinearInterpolation(c,u1,v1);
		float c2 = BilinearInterpolation(c,u2,v2);

		if (SameRounded(c1,col)  == SameRounded(c2,col))		
			return false;


		float compare;
		if (!SameRounded(c2,col))
			compare = c2;
		else
			compare = c1;
		 
		if (compare<col-0.5f)
			ApproxSolution(col-0.5f, u1,v1,u2,v2,p,c,ures,vres);
		else 
			ApproxSolution(col+0.5f, u1,v1,u2,v2,p,c,ures,vres);
		if (ures >1.0f-EPSI)
			ures = 1.0f;
		if (ures<0.0f+EPSI)
			ures = 0.0f;
		if (vres>1.0f-EPSI)
			vres = 1.0f;
		if (vres<0.0f + EPSI)
			vres = 0.0f;
		return true;
}

  
static bool ApproximateSlope(float col, int oor, float u1, float v1, float &u2, float &v2, float delta, HPoint *p, 
					  float *c, float &ures, float &vres, bool &inside, int &side)
{
	
	side = 0;
	inside = true;
	u2 = u1 + orx[oor] * delta;
	v2 = v1 + ory[oor] * delta;

	if (!IsInside(u2,v2))
	{
		inside = false;
		return false;
	}
	 
	float us1, vs1, us2, vs2, cs1, cs2;
	us1 = u2 + orx[FixOr(oor-2)] * delta;
	vs1 = v2 + ory[FixOr(oor-2)] * delta;
	us2= u2 + orx[FixOr(oor+2)] * delta;
	vs2= v2 + ory[FixOr(oor+2)] * delta;
	cs1 = BilinearInterpolation(c,us1,vs1);
	cs2 = BilinearInterpolation(c,us2,vs2);
		
 	if (SameRounded(cs1,col)  == SameRounded(cs2,col))	
		return false;

	if (SameRounded(cs1,col))
		side =1;
	else
		side =2;
	
	float compare;
	if (!SameRounded(cs2,col))
		compare = cs2;
	else
		compare = cs1;
	if (compare > col + 0.5f)
		ApproxSolution(col+0.5f, us1,vs1,us2,vs2,p,c,ures,vres);
	else
		ApproxSolution(col-0.5f, us1,vs1,us2,vs2,p,c,ures,vres);
	if (!IsInside(ures,vres))
	{
		inside = false;
	}
	
	return true;
}
 



static bool CalculateApproxColor(float col, int oor, float u1, float v1, float delta, HPoint *p, float *c,
								 float &ures, float &vres, bool  &special_set, float uold, float vold, int &oldside)
{

	special_set = false;
	float u2,v2, c1,c2;
	oor = FixOr(oor);
	u2 = u1 + orx[oor] * 0.1f;
	v2 = v1 + ory[oor] * 0.1f;
	c1 = BilinearInterpolation(c,u1,v1);
	c2 = BilinearInterpolation(c,u2,v2);
 
	if (IsOnSide(u1, v1,u2,v2))
	{
		if (!(IsInside(u1,v1) && SameRounded(c1, col, 0.1f)  && IsInside(u2,v2) && SameRounded(c2,col)))
		{		
			u2 = u1 + orx[oor] * delta;
			v2 = v1 + ory[oor] * delta;				
 			c2 = BilinearInterpolation(c,u2,v2);
			
		}
		if (IsInside(u1,v1))
		{
			if (SameRounded(c1, col, 0.1f))
			{

				if (!IsInside(u2,v2))
				{
					if (u2<0)
					{
						ures = 0;
						vres = v2;
					}
					else if (u2>1.0f)
					{
						ures = 1.0f;
						vres = v2;
					}
					else if (v2<0)
					{
						ures = u2;
						vres = 0;
					}
					else if (v2>1.0f)
					{
						ures = u2;
						vres = 1.0f;
					}
					else
						assert(!"Should never get here!!!");	
					special_set = true;		
					return false;
				}
				else
				{
					
					if (SameRounded(c2,col))
					{				
						ures = u2;
						vres = v2;
						return true;
					}
					else
					{
						if (SameRounded(c1,col))
						{						
							ApproximateOnSide(col, c1,c2, u1,v1,u2,v2, p, c, ures,vres);
							special_set = true;
							return false;
						}
					}
				}
			}
		}
	 
	}
	else
	{

		HPoint p1,p2;
 		bool inside = false, ret = false;
		float ures_temp = 0, vres_temp = 0;
		float dx = 0.1f;
		float angle = 0;
		int side;
		for (int i=0;i<4;i++)
		{		
			ret = ApproximateSlope(col, oor, u1, v1, u2, v2, dx, p,c,ures_temp, vres_temp, inside, side);
			p1.Set(u1-uold, v1-vold,0);
			p2.Set(ures_temp-u1, vres_temp-v1,0);
			angle = HUtility::ComputeAngleBetweenVector(p1,p2);
		/*	if (fabs(angle)<=90)
				angle = fabs(angle);
			else 
				angle = 180 - fabs(angle);*/
			if (ret && angle <10.0f)
				break;
			dx-=0.02f;
		}
		if (!ret || angle >= 10.0f)
			ret = ApproximateSlope(col, oor, u1, v1, u2, v2, delta, p,c,ures, vres, inside,side);
		else
		{
			ures = ures_temp;
			vres = vres_temp;
		}
		if (ret && !inside)
		{
			u2 = ures;
			v2 = vres;

		}
		if (ret && inside && 0)
		{
			
 			if (side && oldside && oldside!=side)
			{
				ures = u1;
				vres = v1;
				ret =  false;
			}
			oldside = side;
		}
		else
			oldside = 0;

		if (!inside)
		{		
		
			if ((u1 <= 0.0f && u2 < 0.0f) || (v1 <= 0.0f && v2 < 0.0f) ||
				(u1 >= 1.0f && u2 > 1.0f) || (v1 >= 1.0f && v2 > 1.0f))
				return false;
			float u2clipped = u2, v2clipped = v2;
			ClipLine(u1,v1,u2clipped,v2clipped);

  			if (u2<0 || u2>1.0f)
			{
				if (ApproximateOnSide2(col, u2clipped, v2clipped+delta, u2clipped, v2clipped-delta, p,c, ures, vres))
					return true;
				else
					return false;
			}
			else if (v2<0 || v2>1.0f)
			{
				if (ApproximateOnSide2(col, u2clipped+delta, v2clipped, u2clipped-delta, v2clipped, p,c, ures, vres))
					return true;
				else
					return false;
			}
			else
				assert(!"Should never get here!!!");	
		}
		return ret; 	

	}

 	return false;
}
static int IdentifyEdge(float u, float v)
{
	if (u<=0)
		return 1;
	else if (u>=1)
		return 2;
	else if (v<=0)
		return 3;
	else if (v>=1)
		return 4;
	else return 0;
}
				

 
static int levelmax;

static void TraceColorOutline(float col, float lastcol, float ustart, float vstart, HPoint *p, float *c, float delta, int oor, int dir,
							  float uoldstart, float voldstart, HPoint *startpoints, int startpointlen, HShellWrapper &ishell, bool generateIsolines)
{
	
 	HPoint polygon[1024];	
	int faceindex[1024];
	int plen = 0;
	float uold = -1, vold = -1;
	float uold2 = -1, vold2 = -1;
	float u=ustart,v=vstart;
  	int notfoundcounter=0;
	int startnew = 0;
	int currentnew = -1;
	int onedge = false;
	bool firstedge = true;
	float usav = 0,vsav = 0;
	int plensav = 0;
	int side = 0;
 	for (int i=0;i<startpointlen;i++)
	{
		polygon[i] = startpoints[i];
 		faceindex[i] = ishell.m_plen - startpointlen + i;
	}
	if (startpointlen && generateIsolines)
		HC_Insert_Polyline(startpointlen, polygon);

	col = int(col+0.4999f);	
	plen+=startpointlen;
	while (1)
	{
		bool special_set, res;		
 		
		res  = CalculateApproxColor(col, oor, u, v, delta, p,c,u, v, special_set, uold2, vold2, side);		
		
		if (u==uold2 && v==vold2)
		{
			special_set = false;
			res = false;
		}		
#if 0
		if (levelmax >200 || 1)
		{			
			HC_Open_Segment("test");
			HC_Flush_Contents(".", "everything");
			HC_Set_Visibility("lines = on ,faces = off");
			
			HC_Set_Color_By_FIndex("faces", col);
			if (plen)
				HC_Insert_Polyline(plen-startpointlen,&polygon[startpointlen]);
			HC_Open_Segment("test");
			HC_Set_Color("lines = red");
			HC_Insert_Polyline(startpointlen+1,polygon);
			HC_Close_Segment();
			
			HC_Close_Segment();
			HC_Update_Display();
		}
#endif
		if (!res)
		{
			notfoundcounter++;
			if (notfoundcounter>7)
				break;						
			if (special_set)
			{
				BilinearInterpolation(p,u,v,polygon[plen]);			
 				faceindex[plen] = ishell.m_plen;
				HPoint uv(u,v);
				ishell.AddPoints(&polygon[plen],1, &uv);
				uold2 = uold;
				vold2 = vold;
				uold = u;
				vold = v;
				plen++;
			}
 			oor =FixOr(oor+dir);
		}
		else
		{
			
			notfoundcounter = 0; 							
			
			BilinearInterpolation(p,u,v,polygon[plen]);		
			if (plen && !polygon[plen].Equal(polygon[plen-1], 0.0001f))
			{
				faceindex[plen] = ishell.m_plen;
				HPoint uv(u,v);
				ishell.AddPoints(&polygon[plen],1, &uv);								
				uold2 = uold; vold2 = vold;
				uold = u; vold = v;
				plen++;
			}
			if (uold2 == 0 && vold2 == 0 && plen>5)
				break;
			
			if (u>=1.0f || v>=1.0f || u<=0.0f || v<=0.0f)
			{
				int onedge2 = IdentifyEdge(u,v);
				
				if ((uold2 == 0 && vold2 ==0) || (uold2 == 1 && vold2 == 0) ||
					(uold2 == 0 && vold2 ==1) || (uold2 == 1 && vold2 == 1))
					
				{
					onedge2 = onedge;
					
					if (u>uoldstart-0.05f && u<uoldstart+0.05f && v>voldstart-0.05f && v<voldstart+0.05f && plen>5)
					{
						BilinearInterpolation(p,uoldstart,voldstart,polygon[plen]);		
						if (plen && !polygon[plen].Equal(polygon[plen-1], 0.0001f))						
						{						
							faceindex[plen] = ishell.m_plen;
							HPoint uv(uoldstart,voldstart);
							ishell.AddPoints(&polygon[plen],1,&uv);
							plen++;
						}
						break;
					}
				}
				
				if (onedge!=onedge2 && onedge)
				{
					usav = uold2;
					vsav = vold2;
					firstedge = false;
				}
				if ((!onedge || (onedge!=onedge2)) && !firstedge)
				{				
					
					
					if (onedge && (onedge != onedge2))
						plensav = plen-2;
					startnew++;
					int or2 = oor+(dir*0);
					
					or2 = FixOr(or2);
					float n = BilinearInterpolation(c,u,v);
					float ncol;
					if (n<col)
						ncol = col -1;
					else
						ncol = col + 1;
					if (ncol != lastcol || startnew>1)
					{					
						if (currentnew != ncol)
						{	
							TraceColorOutline(ncol,col,u,v,p,c,delta,or2,dir*(-1), usav, vsav, &polygon[plensav], plen-plensav, ishell, generateIsolines);
							currentnew = (int)ncol;					}
						else
							currentnew = -1;
					}
					
					
					
					if (u>uoldstart-0.05f && u<uoldstart+0.05f && v>voldstart-0.05f && v<voldstart+0.05f && plen>5)
					{
						BilinearInterpolation(p,uoldstart,voldstart,polygon[plen]);			
						faceindex[plen] = ishell.m_plen;
						HPoint uv(uoldstart,voldstart);
						ishell.AddPoints(&polygon[plen],1,&uv);
 						plen++;
						break;
					}
					
				}
				onedge = IdentifyEdge(u,v);								
			}
			else
			{
				
				if (onedge)
				{		
					lastcol = 999;
					if (uold2>uoldstart-0.02f && uold2<uoldstart+0.02f && vold2>voldstart-0.02f && vold2<voldstart+0.02f && plen>5)
					{
						BilinearInterpolation(p,uoldstart,voldstart,polygon[plen-1]);						
						if (polygon[plen-1].Equal(polygon[0]))
							plen--;
						break;
					}
					
					usav = uold2;
					vsav = vold2;
					plensav = plen-2;
					
					firstedge = false;
					HPoint ppp;
					BilinearInterpolation(p,u,v,ppp);			
					//					HC_Insert_Marker(ppp.x,ppp.y,ppp.z);
				}
				onedge = false;
			}
			
		}
		
		
		if (u<0.0001f)
			u=0.0f;
		if (v<0.0001f)
			v=0.0f;
		if (u>0.9999f)
			u=1.0f;
		if (v>0.9999f)
			v=1.0f;
		
	}
	
	if (notfoundcounter<=7)
	{	
	
		if (polygon[plen-1].Equal(polygon[0],0.0001f))
			plen--;
		if (levelmax ==5 || 1)
		{
		
		if (dir == 1)
			ishell.AddFaceDirect(faceindex, plen, false);
		else
			ishell.AddFaceDirect(faceindex, plen,true);
		}
		levelmax++;

		
#if 0
		HC_Open_Segment("");
		//	HC_Set_Visibility("edges = on, faces = on");
		
		HC_Set_Color_By_FIndex("faces", col);
		
		HC_Set_Visibility("faces = on");
#if 0
		for (int i=0;i<plen;i++)
		{
			HC_Insert_Polygon(i,polygon);
			HC_Insert_Marker(polygon[i].x, polygon[i].y, polygon[i].z);
			HC_Update_Display();
			float t,t2;
			HC_Show_Time(&t);
			while (1)
			{
				HC_Show_Time(&t2);
				if (t2-t>0.1f)
					break;
			}
			HC_Flush_Contents(".", "geometry");
			
		}
#endif
		HC_Insert_Polygon(plen,polygon);
		//	HC_Flush_Contents(".", "everything");
		HC_Close_Segment();
		
#endif
	}	
}


static void ApproxOnEdge(float col, float u1, float v1, float u2, float v2, HPoint *p, float *c, 
					float &ures, float &vres)
{
	UNREFERENCED(p);

	float dx = 0,dy = 0;
	int counter = 0;
	float t1 = BilinearInterpolation(c,u1,v1);
	float t2 = BilinearInterpolation(c,u2,v2);
	
	if (t1 == col)
	{
		dx = u1;
		dy = v1;
		goto approx_end;
	}
	else if (t2 == col)
	{
		dx = u2;
		dy = v2;
		goto approx_end;
	}else if ((t1 < col && t2 < col) || (t1 > col && t2 > col))
	{
		if (fabs(t1-col) < fabs (t2 - col))
		{
			ures = u1;
			vres = v1;
		}
		else
		{
			ures = u2;
			vres = v2;
		}

	}
 
   	while (1)
	{	
		dx = (u1+u2)/2.0f;
 		dy = (v1+v2)/2.0f;
 		float c1 = BilinearInterpolation(c,dx,dy);
		if ((t1 <=col && c1 >=col) || (t1 >=col && c1<=col))
		{ 		
 			u2 = dx;
			v2 = dy;
			if (fabs(col - c1) < 0.0001f)
				break;
		}
		else
		{
			if (fabs(col - c1) < 0.0001f)
				break;
			u1 = dx;
			v1 = dy;
		}
		counter++;
		if (counter >30)
			counter = counter;

		if (counter >50)
			break;
	}

approx_end:
	ures = dx;
	vres =dy;
	
	if (Equal(ures,1.0f,0.00001f))
		ures = 1.0f;
	if (Equal(ures,0.0f,0.00001f))
		ures = 0.0f;
	if (Equal(vres,1.0f, 0.00001f))
		vres = 1.0f;
	if (Equal(vres,0.0f,0.00001f))
		vres = 0.0f;
	

}



static int TraceOuterColors(HPoint *p, float *c, float *ures, float *vres, float *cres, HShellWrapper &ishell)
{
	int i=0;
	float u = 0;
	float v = 0;
	float ccur = 0;
	float udir = -1;
	float vdir = -1;
	bool record = false;
	float cfrom = 0, cto = 0;

	while (1)
	{
		if (u==0 && v==0)
		{
			if (udir != -1)
				break;

			udir = 1;
			vdir = 0;
 			record = true;
		}
		if (u==1 && v==0)
		{
			udir = 1;
			vdir = 1;
 			record = true;
		}
		if (u==1 && v==1)
		{
			udir = 0;
			vdir = 1;
 			record = true;
		}
		if (u==0 && v==1)
		{
			udir = 0;
			vdir = 0;
 			record = true;
		}
		if (record)
		{
			cfrom = BilinearInterpolation(c, u,v);
			cto = BilinearInterpolation(c, udir,vdir);
			if (!(i>0 && (u == ures[i-1] && v == vres[i-1])))
			{			
				ures[i] = u;
				vres[i] = v;
				cres[i] = BilinearInterpolation(c, u,v);
				HPoint pol;
				BilinearInterpolation(p,u,v, pol);
				HPoint uv(u,v);
				ishell.AddPoints(&pol, 1, &uv);
				i++;
			}
			ccur = int (BilinearInterpolation(c, u,v) + 0.5f);
			if (cto<cfrom)
				ccur -=0.5f;
			else 
 				ccur +=0.5f;
 			
		}
		else
		{
			if (cto<cfrom)
				ccur -=1.0f;
			else
 				ccur +=1.0f;
		}

 
		if ((cto<=cfrom && ccur <= cto) || (cto>=cfrom && ccur >= cto))
		{
			u = udir;
			v = vdir;
		}
		else
		{
			ApproxOnEdge(ccur, u,v, udir, vdir, p,c, u, v);
			float truecolor = BilinearInterpolation(c,u,v);

			ures[i] = u;
			vres[i] = v;
			if ((u==0 && v==0) || (u==1 && v==0) || (u==1 && v==1) || (u==0 && v==1))
				cres[i] = truecolor;
			else
				cres[i] = ccur;

			HPoint pol;
			BilinearInterpolation(p,u,v, pol);
 			HPoint uv(u,v);
			ishell.AddPoints(&pol, 1, &uv);
			i++;
		}

		record = false;
 
	}

	return i;
}

static bool	IsNextConnection(HPoint *p, float *c, float ccol, float ulast, float vlast, float clast,
							 float ures, float vres)
{
	UNREFERENCED(p);
	UNREFERENCED(clast);

	float u = ulast, v = vlast;
	if (v == 0)
		v+=0.01f;
	else if (v == 1.0f)
		v -=0.01f;
	if (u == 0)
		u+=0.01f;
	else if (u == 1.0f)
		u -=0.01f;

 	float color = BilinearInterpolation(c, u,v);
	if (!SameRounded(color, ccol,0.0001f))
	{
		if (vlast==0 && ures>ulast)
			return false;
		if (ulast==1 && vres>vlast)
			return false;
		if (vlast==1 && ures<ulast)
			return false;
		if (ulast==0 && vres<vlast)
			return false;

	}
	return true;
}


static void SubdivdeColorPieceEdge(HPoint *p, float *c, float col, float u1, float v1, float u2, float v2,   int level, HPoint *scratch,int div)
{


		if (level > 3)
			return;
		HPoint pol;

		HPoint pvec2, pvec;
 		float umid = (u1 + u2) / 2.0f;
		float vmid = (v1 + v2) / 2.0f;
		float vecu = u2 - u1;
		float vecv = v2 - v1;
		pvec2.Set(vecv,vecu * -1,0);
		HC_Compute_Normalized_Vector(&pvec2, &pvec2);
		float ures, vres, c1 = 0, c2 = 0, c3;
		float mul = 0.1f;
 		int lastsign2new = 0, lastsign3new = 0;
		float c2old = 0, c3old = 0;
		int first = 0;
		while (1)
		{		
			pvec.x = pvec2.x * mul;
			pvec.y = pvec2.y * mul;
 
 		c1 = BilinearInterpolation(c,umid,vmid);			
		c2 = BilinearInterpolation(c,umid+pvec.x,vmid+pvec.y);			
		c3 = BilinearInterpolation(c,umid-pvec.x,vmid-pvec.y);			
		if ((c1<=col && c2>= col) || (c1 >= col && c2 <= col) ||
			(c1<=col && c3>= col) || (c1 >= col && c3 <= col))
			break;
		mul+=0.05f;

		
		if (first >0)
		{		
			if (c2 > c2old)
				lastsign2new =1;
			else 
				lastsign2new = -1;
		
			if (c3 > c3old)
				lastsign3new =1;
			else 
				lastsign3new = -1;

		if (lastsign2new == lastsign3new)
			break;
 		}

  		c2old = c2;
		c3old = c3;
		first++;
 		
		}
		if ((c1<=col && c2>= col) || (c1 >= col && c2 <= col))
			ApproxOnEdge(col, umid, vmid, umid + pvec.x, vmid + pvec.y, p,c,ures, vres);
		else
			ApproxOnEdge(col, umid, vmid, umid - pvec.x, vmid - pvec.y, p,c,ures, vres);


		scratch[div].Set(ures, vres,0);

 		BilinearInterpolation(p,ures,vres, pol);
 
		
  		
 

		SubdivdeColorPieceEdge(p,c,col,u1,v1,ures,vres, level+1, scratch, div - 32/((level+1) * 2));
		SubdivdeColorPieceEdge(p,c,col,ures,vres,u2,v2, level+1, scratch, div + 32/((level+1) * 2));



 
}
ColorPieceList::ColorPieceList()
{
 	m_ColorPieceList = new_vlist(malloc, free);
 
}
ColorPieceList::~ColorPieceList()
{
	START_LIST_ITERATION(ColorPiece, m_ColorPieceList);
		delete temp;
	END_LIST_ITERATION(m_ColorPieceList);
	vlist_flush(m_ColorPieceList);
	delete_vlist(m_ColorPieceList);
  
}

static void TesselateColorPieces(HPoint *p, float *c, float *ures, float *vres, float *cres, HShellWrapper &ishell, ColorPieceList * colorlist, int *facecolorlist, int &fclnum, bool generateIsolins, int pstart)
{

	int faceindex[255];
  	while (1)
	{	
 	ColorPiece * cp = 0;
	START_LIST_ITERATION(ColorPiece, colorlist->m_ColorPieceList);
	if (!temp->m_done)
	{
		cp = temp;
		break;
	}
	END_LIST_ITERATION(colorlist->m_ColorPieceList);

	if (!cp)
		return;


 	HPoint pol;
	int fi = 0;
	for (int i=0;i<cp->m_len;i++)
	{
		int ii, ii2;
		if (i<cp->m_len-1)
		{		
			ii = cp->m_uvindex[i];
			ii2 = cp->m_uvindex[i+1];
		}
		else
		{		
			ii = cp->m_uvindex[i];
			ii2 = cp->m_uvindex[0];
		}


		BilinearInterpolation(p,ures[ii],vres[ii],pol);			
 		faceindex[fi++] = ii + pstart;
 
		if (!((ures[ii] == 0 && ures[ii2] == 0) || (ures[ii] == 1 && ures[ii2] == 1) ||
			(vres[ii] == 0 && vres[ii2] == 0) || (vres[ii] == 1 && vres[ii2] == 1)))
		{
			int rpos, rlen;
			bool reverse;
			if (!colorlist->FindMatch(cp->m_col, ii, ii2, rpos, rlen, reverse))
			{					
				int l;
				cp->m_flindex[i] = ishell.m_plen;
				HPoint scratch[64];
				for (l=0;l<64;l++)
					scratch[l].z = -1;

				SubdivdeColorPieceEdge(p,c,cres[ii], ures[ii], vres[ii], ures[ii2], vres[ii2],0,scratch, 32);

				for (l=0;l<64;l++)
				{
					
					if (!scratch[l].z)
					{					
						BilinearInterpolation(p,scratch[l].x,scratch[l].y, pol);										
						faceindex[fi++] = ishell.m_plen;
						HPoint uv(scratch[l].x,scratch[l].y,0);
						ishell.AddPoints(&pol, 1, &uv);
					}
				}

 				cp->m_fllen[i] = ishell.m_plen - cp->m_flindex[i];
				if (generateIsolins)
				{	
					HPoint pl[256];
					for (int ii=0;ii<cp->m_fllen[i];ii++)
					{
						pl[ii+1] = ishell.m_Points[cp->m_flindex[i] + ii];
					}
					pl[0] = ishell.m_Points[faceindex[fi - cp->m_fllen[i]-1]];
					BilinearInterpolation(p,ures[ii2],vres[ii2],pl[cp->m_fllen[i]+1]);			

					HC_Insert_Polyline(cp->m_fllen[i]+2, pl);
				}

			}
			else
			{
				cp->m_flindex[i] = rpos;
				cp->m_fllen[i] = rlen;
				if (!reverse)
				{				
					for (int k=0;k<rlen;k++)
					{
						faceindex[fi++] = rpos+k;
					}
				}
				else
				{				
					for (int k=0;k<rlen;k++)
					{
						faceindex[fi++] = rpos+rlen-1-k;
					}
				}
			}

		}
 

	}
	ishell.AddFaceDirect(faceindex, fi, false);
	facecolorlist[fclnum++] = cp->m_col;
	cp->m_done = true;
	}

	
}

ColorPiece * ColorPieceList::FindMatch(int col, int ii, int ii2, int &rpos, int &rlen, bool &reverse)
{
	START_LIST_ITERATION(ColorPiece, m_ColorPieceList);
	if (temp->m_done && (temp->m_col == col-1 || temp->m_col == col +1))
	{
		for (int i=0;i<temp->m_len;i++)
		{
			int jj, jj2;
			if (i<temp->m_len-1)
			{		
				jj = temp->m_uvindex[i];
				jj2 = temp->m_uvindex[i+1];
			}
			else
			{		
				jj = temp->m_uvindex[i];
				jj2 = temp->m_uvindex[0];
			}
			if (jj == ii && jj2 == ii2)
			{
				rpos = temp->m_flindex[i];
				rlen = temp->m_fllen[i];
				reverse = false;
				return temp;
				
			}
			if (jj == ii2 && jj2 == ii)
			{
				rpos = temp->m_flindex[i];
				rlen = temp->m_fllen[i];
				reverse = true;
				return temp;
				
			}
		}

	}
	END_LIST_ITERATION(m_ColorPieceList);
	return 0;


}

 
void ColorPieceList::AddColorPiece(int *index, int len, int col)
{
	ColorPiece *cp = new ColorPiece(index, len, col);
	vlist_add_last(m_ColorPieceList, cp);
}


static void FindColorPieces(HPoint *p, float *c, float *ures, float *vres, float *cres, int len, HShellWrapper &ishell, int *facecolorlist, int &fclnum, bool generateIsolines, int pstart)
{
	int cmin = (int)(c[0] + 0.4999f);
	int cmax = (int)(c[0] + 0.4999f);
	int i = 0;
	for (i=1;i<4;i++)
	{
		if (cmin>(c[i]+0.4999f))
			cmin = (int)(c[i] + 0.4999f);
		if (cmax<(c[i] + 0.4999f))
			cmax = (int)(c[i] + 0.4999f);
	}
	ColorPieceList colorlist;
	int ccol;
	bool done[256];
	int idx[20];
  	for (ccol = cmin; ccol <=cmax;ccol++)
	{
		for (int j=0;j<len;j++)
		{
			done[j] = false;
		}
		for (int round=0;round<2;round++)
		{		
		int ii=0;
		float lastmatch = false;	
		float ulast = 0, vlast = 0, clast = 0;
		for (i=0;i<len;i++)
		{	
		
			if (SameRounded(cres[i],ccol) && !done[i])
			{
 	
  				if (ures[i] == 0 && vres[i] == 0 || ii==0)
				{
					idx[ii++] = i;
					done[i] = true;
				}
				else if (i == idx[ii-1] + 1)
				{
					idx[ii++] = i;
					done[i] = true;
 
				}
				else if (clast == cres[i] && !(lastmatch))
				{
					int nextones = 0;
					for (int j=i;j<len;j++)
					{
							if (SameRounded(cres[i],ccol) && !done[i])
								nextones++;
					}
 					if (nextones <=2 || IsNextConnection(p,c, ccol, ulast, vlast, clast, ures[i], vres[i]))
					{
						idx[ii++] = i;
						done[i] = true;
					}					
				}
				
				ulast = ures[idx[ii-1]];
				vlast = vres[idx[ii-1]];
				clast = cres[idx[ii-1]];
				lastmatch = true;
  
			}
			else
				lastmatch = false;
		}
		if (ii)
			colorlist.AddColorPiece(idx, ii, (int)ccol);
		}


		
		
	}

	TesselateColorPieces(p,c,ures, vres, cres, ishell, &colorlist, facecolorlist, fclnum, generateIsolines, pstart);
	 
}



void HShellWrapper::SubdivideOneQuadFace(int *face, HShellWrapper &ishell, bool generateIsolines, int *facecolorlist, int &fclnum)
{
  
	HPoint p[4];
	float c[4];
	float *cte = new float[m_plen];
	ShowVertexColors(cte);
	for (int i=0;i<4;i++)
	{
		p[i] = m_Points[face[i]];
		c[i] = cte[face[i]];
	}
	OpenOwnerSegment();
	float uu[100];
	float vv[100];
	float cc[100];

	int pstart = ishell.m_plen;
	int len = TraceOuterColors(p,c,uu,vv,cc, ishell);
	FindColorPieces(p,c,uu,vv,cc,len, ishell, facecolorlist, fclnum, generateIsolines, pstart);
 
// 	float color = BilinearInterpolation(c, 0.0,0);
 //	levelmax = 0;
  //	TraceColorOutline(color, 99.0f, 0.0f,0,p,c,0.01f,3,1,0.0,0,0,0, ishell, generateIsolines);
 	HC_Close_Segment();

	delete [] cte;
//	SubdivideOneQuadFace2(face);

}
 


void HShellWrapper::SubdivideOneQuadFace2(int *face)
{
	float u=0.0f;
	float v=0.0f;

	HPoint p[4];
	float c[4];
	float *cc = new float[m_plen];
	ShowVertexColors(cc);
	for (int i=0;i<4;i++)
	{
		p[i] = m_Points[face[i]];
		c[i] = cc[face[i]];
	}

	OpenOwnerSegment();
	for (u=0;u<1.0f;u+=0.01f)
	{
		HPoint np,nc;
		for (v=0;v<1.0f;v+=0.01f)
		{

			BilinearInterpolation(p,u,v,np);
			nc.x = BilinearInterpolation(c, u,v);

 			HC_Open_Segment("");
			HC_Set_Color_By_FIndex("markers",nc.x);
			HC_Insert_Marker(np.x, np.y, np.z);
			HC_Close_Segment();

		}
	}

	HC_Close_Segment();

}
#include "varray.h"

void HShellWrapper::SubdivideQuadFaces(bool generateIsolines)
{
	int *face;
	float c[4];
	float *vc = new float[256];
	int vclen = 255;
	int i;
	Show(0,0,0,0);
	float *cte = new float[m_plen];
	ShowVertexColors(cte);

	int facecolortotal = 10000;
	int *facecolorlist = new int[10000];
	int fclnum = 0;
	BeginFaceIteration();
	HShellWrapper ishell;	
	int lastplen = 0;
	int ccc = 0;
	while (1)
	{	
		int flen = NextFace(&face);
		if (!flen)
			break;
	 
//		if (ccc>15300)
//			break;
		if (flen == 4)
		{
			ccc++;
	//		if (ccc==331)
			{			
		//	if (ccc ==2)
			SubdivideOneQuadFace(face, ishell, generateIsolines, facecolorlist, fclnum);					
			if (fclnum>=facecolortotal-100)
			{
				int *facecolorlist2 = new int[facecolortotal * 2];
				for (int i=0;i<fclnum;i++)
					facecolorlist2[i] = facecolorlist[i];
				delete [] facecolorlist;
				facecolorlist = facecolorlist2;
				facecolortotal*=2;
			}
			for (i=0;i<4;i++)
			{
		 		c[i] = cte[face[i]];
			}
			if (ishell.m_plen>=vclen)
			{
				float *newvc = new float[ishell.m_plen*2];
				for (int i=0;i<vclen;i++)
					newvc[i] = vc[i];
				delete [] vc;
				vc = newvc;
				vclen = ishell.m_plen*2;
			}			

			for (i=lastplen;i<ishell.m_plen;i++)
			{
				vc[i] =  BilinearInterpolation(c, ishell.m_VParams[i].x,ishell.m_VParams[i].y);
				if (Equal(int(vc[i]) + 0.5f, vc[i], 0.01f))
					vc[i] = int(vc[i]) + 0.5f;
				else if (Equal(int(vc[i]) - 0.5f, vc[i], 0.01f))
					vc[i] = int(vc[i]) - 0.5f;
			}
			lastplen = ishell.m_plen;			
			}
		}
	}
	EndFaceIteration();

 
	OpenOwnerSegment();
	
	ishell.Regenerate();

	if (ishell.m_Key != INVALID_KEY) {
	
#if 0
		HC_Open_Geometry(ishell.m_Key);
		for (int ii=0;ii<fclnum;ii++)
		{
			HC_Open_Face(ii);
			//HC_Set_Color_By_Index("faces", facecolorlist[ii]);
			HC_Close_Face();
		}
		HC_Close_Geometry();
#endif

#if 1

		HC_Open_Geometry(ishell.m_Key);
		ishell.BeginFaceIteration();
		int fccc = 0;
		int *face2;
		while (1)
		{
			fccc++;
			int i0,i1;
			int flen = ishell.NextFace(&face2);
			if (!flen)
				break;
			for (int i=0;i<flen;i++)
			{
				if (i==flen-1)
				{
					i0 = i;
					i1 = 0;
				}
				else
				{
					i0=i;
					i1=i+1;
				}
				HPoint p1,p2;
				p1 = ishell.m_VParams[face2[i0]];
				p2 = ishell.m_VParams[face2[i1]];
				if (IsOnSide(p1.x, p1.y, p2.x, p2.y))
				{
					HC_Open_Edge(face2[i0], face2[i1]);
						HC_Set_Visibility("off");
					HC_Close_Edge();
				}


			}
		}
		HC_Close_Geometry();
#endif

		ishell.SetVertexColors(vc);		
	}


	HUtility::MergePolylines();
//	ishell.Optimize("tolerance = 0.001oru");
	HC_Close_Segment();

	delete [] vc;
	delete [] cte;
	delete [] facecolorlist;

}

void HShellWrapper::MakeLod(int lodnumber, int percentage)
{
	
 	HPoint *plist_lod = new HPoint[m_plen];
	int *flist_lod = new int[m_flen*2];
	int plen_lod, flen_lod;
	
//	HC_Compute_Optimized_Shell(m_plen, m_Points, 0, m_flen, m_Flist, "tolerance = 0.0005oru", 
//		&plen_collapsed, plist_collapsed, &flen_collapsed, flist_collapsed, 0, 0);
	
	char options[4096];
	sprintf(options, "lod = %d%%, lod algorithm = nice", percentage);
	
	HC_Compute_Optimized_Shell(m_plen, m_Points, 0, m_flen, m_Flist, options, 
		&plen_lod, plist_lod, &flen_lod, flist_lod, 0, 0);
	
	
 
	if (plen_lod > 0)
	{																			
		HC_Open_Geometry(m_Key);
		HC_Open_LOD(lodnumber);
		HC_KInsert_Shell(plen_lod, plist_lod, flen_lod, flist_lod);
		HC_Close_LOD();
		HC_Close_Geometry();												
		
	}
 //	delete [] plist_collapsed;
//	delete [] flist_collapsed;
	
	delete [] plist_lod;
	delete [] flist_lod;
}

void
HUtility::ascii_to_unicode(char const *ascii, unsigned short *unicode)
{
	unsigned int i;
	unsigned int str_len = (unsigned int) strlen(ascii);
	for(i=0; i<=str_len; ++i){
		unicode[i] = (unsigned short)ascii[i] & (unsigned short)0x00FF;
	}
}

void
HUtility::unicode_to_ascii(unsigned short const *unicode, char *ascii)
{
	unsigned int i, len = 0;
	while(unicode[len]) ++len;

	for(i=0; i<=len; ++i){
		ascii[i] = (unsigned short)unicode[i];
	}
}

bool HUtility::RLECompress(
		int const in_len,
		unsigned char const * const in_buf,
		int * out_len,
		unsigned char * const out_buf)
{
	/* This is the run-length-encode filter for pdf */

	if (!out_buf || !in_buf){
		out_len = 0;
		return false;
	}

	/* Allocate a temporary working buffer if in_buf and out_buf are the same. */
	unsigned char * working = out_buf;
	if (in_buf == out_buf){
		working = new unsigned char[*out_len];
	}

	int c_index = 0, u_index = 0, run_length;
	while(u_index < in_len){
		if (in_buf[u_index] != in_buf[u_index+1]){
			int c_index_save = c_index++;
			run_length = 0;
			while(u_index < in_len -1 && in_buf[u_index] != in_buf[u_index+1] && run_length < 127){
				if (c_index < *out_len)
					working[c_index++] = in_buf[u_index];
				else {
					*out_len = c_index;
					return false;
				}
				++u_index; ++run_length;
			}
			working[c_index_save] = run_length -1;
			if (u_index == in_len -1){
				if (c_index < *out_len -1){
					working[c_index++] = in_buf[u_index];
					working[c_index++] = 0;
				} else 
					return false;
			} else return false;
		} else {
			run_length = 1;
			while(u_index < in_len -1 && in_buf[u_index] == in_buf[u_index+1] && run_length < 128){
				++u_index; ++run_length;
			}
			if (c_index < *out_len -1){
				working[c_index++] = 257 - run_length;
				working[c_index++] = in_buf[u_index];
			} else {
				*out_len = c_index;
				return false;
			}
			if (u_index == in_len -1){
				if (c_index < *out_len -1){
					working[c_index++] = in_buf[u_index];
					working[c_index++] = 0;
				} else 
					return false;
			} else return false;
			++u_index;
		}
	}

	if (c_index < *out_len)
		working[c_index++] = 128;
	else {
		*out_len = c_index;
		return false;
	}

	*out_len = c_index;

	if (in_buf == out_buf){
		memcpy(out_buf, working, *out_len);
		delete [] working;
	}

	return true;
}

bool HUtility::ZlibCompress(
		int const in_len,
		unsigned char const * const in_buf,
		int & out_len,
		unsigned char * const out_buf)
{
	if (!out_buf || !in_buf){
		out_len = 0;
		return false;
	}

	/* Initialize the thing. */
	struct z_stream_s zstream;
	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.data_type = Z_UNKNOWN;
	if (deflateInit(&zstream, Z_DEFAULT_COMPRESSION) != Z_OK){
		out_len = 0;
		return false;
	}
	
	/* Allocate a temporary working buffer if in_buf and out_buf are the same. */
	unsigned char * working = out_buf;
	if (in_buf == out_buf){
		working = new unsigned char[out_len];
	}

	/* Set the output buffer */
	zstream.next_out = working;
	zstream.avail_out = out_len;
	
	/* Set the input buffer */
	zstream.next_in = (Bytef *) in_buf;
	zstream.avail_in = in_len;

	int result = deflate(&zstream, Z_FINISH);

	bool retval = true;
	if (result == Z_STREAM_END){
		out_len = zstream.total_out;
		retval = true;
	} else {
		out_len = 0;
		retval = false;
	}

	deflateEnd(&zstream);

	if (in_buf == out_buf){
		memcpy(out_buf, working, out_len);
		delete [] working;
	}

	return retval;
}


bool HUtility::ZlibDecompress(
		int const in_len,
		unsigned char const * const in_buf,
		int & out_len,
		unsigned char * const out_buf)
{
	if (!out_buf || !in_buf){
		out_len = 0;
		return false;
	}

	/* Initialize the thing. */
	struct z_stream_s zstream;
	zstream.zalloc = Z_NULL;
	zstream.zfree = Z_NULL;
	zstream.data_type = Z_UNKNOWN;
	if (inflateInit(&zstream) != Z_OK){
		out_len = 0;
		return false;
	}
	
	/* Allocate a temporary working buffer if in_buf and out_buf are the same. */
	unsigned char * working = out_buf;
	if (in_buf == out_buf){
		working = new unsigned char[out_len];
	}

	/* Set the output buffer */
	zstream.next_out = working;
	zstream.avail_out = out_len;
	
	/* Set the input buffer */
	zstream.next_in = (Bytef *) in_buf;
	zstream.avail_in = in_len;

	int result = inflate(&zstream, Z_FINISH);

	bool retval = true;
	if (result == Z_STREAM_END){
		out_len = zstream.total_out;
		retval = true;
	} else {
		out_len = 0;
		retval = false;
	}

	inflateEnd(&zstream);

	if (in_buf == out_buf){
		memcpy(out_buf, working, out_len);
		delete [] working;
	}

	return retval;
}


void HUtility::BatchRead(HBaseModel * model, const char * directory, const char * extension)
{

	char ext[4096], new_dir[4096];
	if (extension[0]!='.')
		sprintf (ext, ".%s", extension);
	else 
		sprintf (ext, "%s", extension);

#ifdef _MSC_VER
#ifndef _WIN32_WCE
	HANDLE find_handle;
	char find_dir[4096], file_name[4096];
	WIN32_FIND_DATA find_data;
	BOOL find_result;
	size_t no;

	/* make sure directory is in the correct format */
	if (directory[strlen(directory)-1] == '\\')
		sprintf (find_dir, "%s*.*", directory);
	else
		sprintf (find_dir, "%s\\*.*", directory);



	/* start the search */
	find_handle = FindFirstFileA(find_dir, &find_data);

	if (find_handle == INVALID_HANDLE_VALUE)
		return;

	do
	{
#ifdef _UNICODE
		strcpy(filename, H_ASCII_TEXT(find_data.cFileName);
#else
		memcpy(file_name, find_data.cFileName, strlen(find_data.cFileName)+1);
#endif
		no=strlen(find_dir)-3;
		strncpy(new_dir, find_dir, no);
		new_dir[no]=0;
		strcat(new_dir, file_name);

		if (file_name[0]=='.') 
			continue;
		else if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			BatchRead(model, new_dir, ext);
		else if (strstr(file_name+strlen(file_name)-4, ext)) 
			model->Read(new_dir);

	} while((find_result = FindNextFile(find_handle, &find_data)) != 0);

	FindClose(find_handle);
#endif
#else
	
	DIR	*dir = opendir (directory);
	struct dirent	*entry;
	struct stat buf;

	if (!dir) return;

	while ((entry = readdir(dir))) 
	{
		sprintf(new_dir, "%s/%s", directory, entry->d_name);
		stat(entry->d_name, &buf);

		if (entry->d_name[0]=='.') 
			continue;
		else if (buf.st_mode & S_IFDIR)
			BatchRead(model, new_dir, ext);
		else if (strstr(entry->d_name+strlen(entry->d_name)-4, ext)) 
			model->Read(new_dir);
	}

	closedir(dir);
#endif
}

void
HUtility::AdjustPositionToPlane(HBaseView * pView, HPoint &position, HPoint point_in_plane)
{
	HPoint new_point;

	HC_Open_Segment_By_Key(pView->GetSceneKey());{

		HC_Compute_Coordinates (".", "world", &position, "projection", &position);
	 
 		HC_Compute_Coordinates (".", "world", &point_in_plane, "projection", &point_in_plane);
		position.z = point_in_plane.z;
		HC_Compute_Coordinates (".", "projection", &position, "world", &new_point);

	}HC_Close_Segment();

	position = new_point;
}

void HUtility::CalculateCornerImagePosition(HBaseView *view, int iwidth, int iheight, HWindowCorner corner, float &posx, float &posy)
{
 	HPoint p1,p2,p3;
	HC_Open_Segment_By_Key(view->GetViewKey());	

	if (corner == BottomRight)
	{	
		p1.Set(1,-1,1);
		HC_Compute_Coordinates(".", "local window",&p1, "local pixels", &p2);
		p2.x-=iwidth/2;
		p2.y-=iheight/2;
	}
	else if (corner == TopRight)
	{
		p1.Set(1,1,1);
		HC_Compute_Coordinates(".", "local window",&p1, "local pixels", &p2);
		p2.x-=iwidth/2;
		p2.y=iheight/2;
	}

	HC_Compute_Coordinates(".", "local pixels",&p2, "local window", &p3);

	HC_Close_Segment();
  	posx = p3.x;
	posy = p3.y;
	
}


void HUtility::CreateZebraStripeTexture(int width, int height, int stripwidth, bool horizontal, HPixelRGB colorstripes, HPixelRGB colorbackground)
{

	HPixelRGB *image = new HPixelRGB[width * height];
	HPixelRGB white_color(0,0,0);
	memset(image,0,width*height*sizeof(HPixelRGB));
	int snum = 0;
	bool onoff = false;
	for (int x=0;x<width;x++)
	{		   
		if (snum>stripwidth)
		{
			snum = 0;
			onoff = !onoff;
		}
		for (int y=0;y<height;y++)
		{
			if (horizontal)
			{
				if (onoff)
					image[x+y*width] = colorstripes;
				else
					image[x+y*width] = colorbackground;
			}
			else
			{
				if (onoff)
					image[y+x*width] = colorstripes;
				else
					image[y+x*width] = colorbackground;
			}			
		}
		snum++;
   }
	HC_Set_Visibility("images = off");
	HC_KInsert_Image (0.0, 0.0, 0.0, "rgb, local, name = zebra,no down-sample", width, height, image);
	HC_Define_Local_Texture("zebratexture", "source = zebra, parameterization source = reflection vector");
	delete [] image;
}







void HUtility::OptimizeModelByGrid(HBaseView *view, ShellToRegionMapper *mapper)
{
 	HC_KEY segmentkey;
	HC_Open_Segment_By_Key(view->GetModelKey());
 
	HC_Begin_Contents_Search(".", "segments");
	float t1,t2;
	HC_Show_Time(&t1);
	while (HUtility::FindContents(segmentkey))
	{	
		char text[4096];
		HC_Open_Segment_By_Key(segmentkey);
		HC_Show_Segment(segmentkey,text);

		float r,g,b, multi;
		HPoint min,max, delta, factor;
		HC_Show_Color_By_Value(text, text, &r, &g, &b);
		HC_Compute_Circumcuboid(".", &min, &max);
		delta.Set(max.x - min.x, max.y - min.y, max.z - min.z);
		if (delta.x <= delta.y && delta.x <= delta.z)
 			factor.Set(1.0f, delta.y / delta.x, delta.z/ delta.x);
 		else if (delta.y <= delta.x && delta.y <= delta.z)
 			factor.Set(delta.x / delta.y, 1.0f, delta.z/ delta.y);
 		else if (delta.z <= delta.x && delta.z <= delta.y)
 			factor.Set(delta.x / delta.z, delta.y/ delta.z, 1.0f); 			
		HC_Close_Segment();


		HModelInfo modelinfo(segmentkey); 

		multi = modelinfo.GetTotalFaces()/80000;
		if (multi < 1.0f)
			multi = 1.0f;
		
		factor.Set(factor.x * multi, factor.y * multi, factor.z * multi);

		if (factor.x > 20)
			factor.x = 20;
		if (factor.y > 20)
			factor.y = 20;
		if (factor.z > 20)
			factor.z = 20;


		HC_Open_Segment_By_Key(view->GetModelKey());
		HC_Open_Segment("putinhere");
		HC_KEY pih = HC_KOpen_Segment("");
		HUtility::SubdivideShells(segmentkey, segmentkey, pih, (int)factor.x, (int)factor.y, (int)factor.z, mapper, false, true, true);
 		HC_Set_Color_By_Value("faces", "rgb", r,g,b);
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Close_Segment();
		HC_Delete_By_Key(segmentkey);
	}
	HC_Show_Time(&t2);


	HC_End_Contents_Search();
 	HC_Close_Segment();
	
}

 

static delimiter_t first_tex_part[] = {{"\x21-\x7E", false, "=|,|", true, false}};
static delimiter_t second_tex_part[] = {{"\x21-\x7E", false, ",", true, false}};
static delimiter_t number_part[] = {{"\x30-\x39", false, ",|)|", true, false}};
 
void HUtility::DecipherTextureString(char *texdef, char *Source, bool *DownSampling, bool *DownSamplingSet, char *ParameterizationSource,
									 char *InterpolationFilter, char *DecimationFilter, char *Layout, char *Tiling, bool *ValueScaleSet,
									 bool *ValueScale, float *ValueScaleMin, float *ValueScaleMax, char *BlendingMode)
{
	UNREFERENCED(ValueScale);

    HUtilityTextParser tp;    
    tp.InsertBuffer(texdef, (unsigned int)strlen(texdef));

	if (DownSamplingSet)
 		*DownSamplingSet = false;
	if (ValueScaleSet)
		*ValueScaleSet = false;
    bool hasModulate = false;
    bool hasDecal = false;
    
    char leftside[MVO_BUFFER_SIZE];
    char rightside[MVO_BUFFER_SIZE];
    while (1)
    {    
		int dlum;
		bool res = tp.ParseBuffer(leftside, (int &)dlum, first_tex_part, 1);

		if (strcmp(leftside, "down sample") == 0)
		{	
			if (DownSampling)
			{
 				*DownSampling = true;
    			*DownSamplingSet = true;
			}
			if (!res)
			break;

		}
		else if (strcmp(leftside, "no down sample") == 0)
		{
			if (DownSampling)
			{
  				*DownSampling = false;
    			*DownSamplingSet = true;
			}
			if (!res)
				break;
		}
		else if (strcmp(leftside, "modulate") == 0)
		{
 			hasModulate = true;
			if (!res)
			break;   
		}
		else if (strcmp(leftside, "decal") == 0)
		{
 			hasDecal = true;
			if (!res)
			break;   
		}
		else
		{
			if (!res)
			break;
			res = tp.ParseBuffer(rightside, (int &)dlum, second_tex_part, 1);
		}
	    
		if (strcmp(leftside, "parameterization source") == 0)
		{
			if (ParameterizationSource)
				strcpy(ParameterizationSource, rightside);
		}
		if (strcmp(leftside, "interpolation filter") == 0)
		{
			if (InterpolationFilter)
				strcpy(InterpolationFilter, rightside);
		}
		if (strcmp(leftside, "decimation filter") == 0)
		{
			if (DecimationFilter)
				strcpy(DecimationFilter, rightside);
		}
	    
		if (strcmp(leftside, "down-sampling") == 0)
		{
			if (DownSampling)
			{
				if (strcmp(rightside, "on") == 0)
					*DownSampling = true;
				else
					*DownSampling = false;
				*DownSamplingSet = true;
			}

		}
		if (strcmp(leftside, "layout") == 0)
		{
			if (Layout)
				strcpy(Layout, rightside);
		}
		if (strcmp(leftside, "tiling") == 0)
		{
			if (Tiling)
				strcpy(Tiling, rightside);
		}
		if (strcmp(leftside, "source") == 0)
		{
			if (Source)
				strcpy(Source, rightside);
		}
		if (strcmp(leftside, "value scale") == 0)
		{
 			char rightside2[MVO_BUFFER_SIZE];
			res = tp.ParseBuffer(rightside2, (int &)dlum, second_tex_part, 1);
			strcat(rightside, ",");

			strcat(rightside, rightside2);

			 HUtilityTextParser tp2;    
			tp2.InsertBuffer(rightside, (unsigned int)strlen(rightside));

			char v1[MVO_BUFFER_SIZE];
			res = tp2.ParseBuffer(v1, (int &)dlum, number_part, 1);
			char v2[MVO_BUFFER_SIZE];
			res = tp2.ParseBuffer(v2, (int &)dlum, number_part, 1);
			if (ValueScaleSet)
			{
				*ValueScaleMin = atof(v1);
				*ValueScaleMax = atof(v2);
				*ValueScaleSet = true;
			}

		}
		if (BlendingMode)
		{
			if (hasDecal && hasModulate)
				strcpy(BlendingMode, "decal + modulate");
			else if (hasDecal)
				strcpy(BlendingMode, "decal");
			else if (hasModulate)
				strcpy(BlendingMode, "modulate");
		}
	}
}




HShellObject::HShellObject(HC_KEY kShellKey):m_kShellKey(kShellKey),m_pFaceIndexes(0) 
{
	HC_Show_Shell_Size(m_kShellKey, (int*)0, &m_iFlistLen);
	HC_Show_Geometry_Pointer(m_kShellKey, "face list", &m_pFlist);
}

HShellObject::HShellObject(HShellObject const & oShellObj)
:m_kShellKey(oShellObj.GetKey()),m_pFaceIndexes(0),
 m_iFlistLen(oShellObj.GetFlistLen())
{
	HC_Show_Geometry_Pointer(m_kShellKey, "face list", &m_pFlist);

	int const iFaceCount = GetFaceCount();
	if (iFaceCount)
	{
		m_pFaceIndexes = new int[iFaceCount];
		for (int i=0; i<iFaceCount; ++i)
		{
			m_pFaceIndexes[i] = oShellObj.GetFaceFlistIndex(i);
		}
	}
}

HShellObject::~HShellObject()
{
	if (m_pFaceIndexes)
		delete [] m_pFaceIndexes;
}

int HShellObject::GetFaceFlistIndex(int index) const
{
	if (!m_pFaceIndexes){
		int const iFlistLen = GetFlistLen();
		m_pFaceIndexes = new int[GetFaceCount()];
		int * pFaceIndex = m_pFaceIndexes;
		int flist_index = 0;

		/*	walk through the shell face list saving the index of where
			each face starts. */
		while(flist_index < iFlistLen){
			*pFaceIndex++ = flist_index;
			flist_index += m_pFlist[flist_index] +1;
		}
	}
	return m_pFaceIndexes[index];
}

int HShellObject::GetFaceCount() const
{
	int iFaceCount = 0;
	HC_Show_Shell_Face_Count(GetKey(), &iFaceCount);
	return iFaceCount;
}

HC_KEY HShellObject::GetKey() const
{
	return m_kShellKey;
}

int HShellObject::GetFlistLen() const
{
	return m_iFlistLen;
}

int const * HShellObject::GetFlist() const
{
	return m_pFlist;
}

void HShellObject::GetFlist (int * flist) const {
	memcpy(flist, m_pFlist, m_iFlistLen * sizeof(int));
}

HShellRegion const HShellObject::GetRegion(int const index) const
{
	return HShellRegion(this, index);
}

HShellRegion::HShellRegion(HShellObject const * pShell, int const iRegionIndex)
	: m_pShell(pShell)
	, m_iRegionIndex(iRegionIndex)
{
	HC_MShow_Region_Faces_Count(m_pShell->GetKey(), m_iRegionIndex, &m_iRegionFlistLength);
	m_pRegionFlist = new int[m_iRegionFlistLength];
	HC_MShow_Region_Faces(m_pShell->GetKey(), m_iRegionIndex, &m_iRegionFlistLength, m_pRegionFlist);
}

int HShellRegion::GetFaceCount() const
{
	int region_flist_len = 0;
	HC_MShow_Region_Faces_Count(m_pShell->GetKey(), m_iRegionIndex, &region_flist_len);
	return region_flist_len;
}

HShellFace const HShellRegion::GetFace(int index) const
{
	HShellFace oRetval(m_pShell, m_pRegionFlist[index]);
	return oRetval;
}

HShellFace::HShellFace(HShellObject const * pShell, int const iFaceIndex)
:m_pShell(pShell),m_iFaceIndex(iFaceIndex)
{
	int const * flist = m_pShell->GetFlist();
	m_iPointCount = flist[m_pShell->GetFaceFlistIndex(iFaceIndex)];
}

int HShellFace::GetPointCount() const
{
	return m_iPointCount;
}

int HShellFace::GetEdgeCount() const
{
	return m_iPointCount;
}

HShellEdge const HShellFace::GetEdge(int const iEdgeIndex) const
{
	int const * flist = m_pShell->GetFlist() + m_pShell->GetFaceFlistIndex(m_iFaceIndex);
	HShellEdge oRetval;

	if (iEdgeIndex == (m_iPointCount -1)){
		oRetval.Set(flist[1], flist[m_iPointCount]);
	} else if (iEdgeIndex >= 0){
		oRetval.Set(flist[iEdgeIndex +1], flist[iEdgeIndex +2]);
	} else {
		/* If we get here, the use passed us a bad index. */
	}
	return oRetval;
}

void HShellFace::OpenEdge(int const iEdgeIndex) const
{
	HC_Open_Geometry(m_pShell->GetKey());
	HShellEdge const oEdge = GetEdge(iEdgeIndex);
	HC_Open_Edge(oEdge.GetFirst(), oEdge.GetSecond());
}

void HShellFace::CloseEdge() const 
{
	HC_Close_Edge();
	HC_Close_Geometry();
}
 



ShellToRegionMapper::ShellToRegionMapper()
{
		m_RegionToShellHash = new_vhash(10, malloc, free);
		m_ShellToRegionHash = new_vhash(10, malloc, free);
		m_ShellToSegmentHash = new_vhash(10, malloc, free);
}


typedef struct
{
	HC_KEY newshell;
	HC_KEY origshell;
}shellshellpair;

typedef struct
{
	HC_KEY newshell;
	int region;
}shellregionpair;


void ShellToRegionMapper::AddSegmentMapping(HC_KEY originalshell, HC_KEY segmentkey)
{
 	
  		vhash_insert_item(m_ShellToSegmentHash, (void *)originalshell , (void *)segmentkey);	

}


void ShellToRegionMapper::AddRegion(HC_KEY originalshell, HC_KEY newshell, int regionnum, bool check)
{
	bool found = false;
	int n = 0;
	shellshellpair *spair;
	shellregionpair *rpair;
	

	if (check)
	{	
		n=0;
		while (1)
		{
			if ((vhash_lookup_nth_item(m_RegionToShellHash, MVO_I2V(regionnum) ,n,  (void **)&spair) == VHASH_STATUS_SUCCESS))
			{
				if (spair->newshell == newshell)
				{
					found = true;
					break;
				}
				
			}
			else
				break;
			n++;
		}
		
	}
	else
		found = false;
	if (!found)
	{
		spair = new shellshellpair;
 		spair->newshell = newshell;
		spair->origshell = originalshell;
 		vhash_insert_item(m_RegionToShellHash, MVO_I2V(regionnum) , (void *)spair);
	}




	found = false;
	n=0;

	if (check)
	{	
	while (1)
	{
		if ((vhash_lookup_nth_item(m_ShellToRegionHash, MVO_I2V(originalshell) ,n,  (void **)&rpair) == VHASH_STATUS_SUCCESS))
		{
			if (rpair->newshell == newshell && rpair->region == regionnum)
			{
				found = true;
				break;
			}

		}
		else
			break;
		n++;
	}
	}
	else
		found = false;

	if (!found)
	{
		rpair = new shellregionpair;
 		rpair->region = regionnum;
		rpair->newshell = newshell;
 		vhash_insert_item(m_ShellToRegionHash, (void *)originalshell , (void *)rpair);
	}




}

HC_KEY ShellToRegionMapper::GetOriginalShell(HC_KEY newshell, int regionnum)
{

	shellshellpair *spair;
 	
	int n=0;
	while (1)
	{
		if ((vhash_lookup_nth_item(m_RegionToShellHash, MVO_I2V(regionnum) ,n,  (void **)&spair) == VHASH_STATUS_SUCCESS))
		{
			if (spair->newshell == newshell)
				return spair->origshell;			
		}
		else
			break;
		n++;
	}


	return INVALID_KEY;
}



int ShellToRegionMapper::GetRegionsFromOriginalShell(HC_KEY originalshell, HC_KEY **newshells, int &regionnum)
{

	int length =0;
 
 	shellregionpair *rpair;
	
	int n=0;

	while (1)
	{
		if ((vhash_lookup_nth_item(m_ShellToRegionHash, (void *)originalshell ,n,  (void **)&rpair) == VHASH_STATUS_SUCCESS))		
			n++;		
		else
			break;
	}
	


	HC_KEY *ns = new HC_KEY[n];

	length = n;
	n=0;
	while (1)
	{
		if ((vhash_lookup_nth_item(m_ShellToRegionHash, (void *)originalshell ,n,  (void **)&rpair) == VHASH_STATUS_SUCCESS))
		{
			regionnum = rpair->region;
			ns[n] = rpair->newshell;
			n++;
		}
		else
			break;
	}

	*newshells = ns;
	return length;


}


void ShellToRegionMapper::BuildSegmentTreeRecursive(HC_KEY okey, HC_KEY nkey)
{
	HC_KEY segkey, shellkey;
	HC_Open_Segment_By_Key(okey);
	HC_Begin_Contents_Search(".", "segments");
	while (HUtility::FindContents(segkey))
	{
		HC_Open_Segment_By_Key(nkey);
		HC_KEY temp = HC_KOpen_Segment("");
		HC_Set_User_Value((POINTER_SIZED_INT)segkey);
		HC_Close_Segment();
		HC_Open_Segment_By_Key(segkey);
		HC_Begin_Contents_Search(".", "shells");
		while (HUtility::FindContents(shellkey))
		{
			HC_Open_Segment_By_Key(temp);
			HC_Open_Segment("sxxcnt");
			HC_KEY ssegment = HC_KOpen_Segment("");
			HC_Set_User_Value((POINTER_SIZED_INT)shellkey);
			AddSegmentMapping(shellkey, ssegment);
			HC_Close_Segment();
			HC_Close_Segment();
			HC_Close_Segment();
		}
		HC_End_Contents_Search();
		HC_Close_Segment();
		BuildSegmentTreeRecursive(segkey, temp);
	}
	HC_End_Contents_Search();
	HC_Close_Segment();

}

void ShellToRegionMapper::MapSegmentTree(HC_KEY modelkey) 
{


	HC_KEY segmentkey;
	HC_KEY originalkey = -1;
	HC_KEY copykey;
	HC_Open_Segment_By_Key(modelkey);
	HC_Begin_Contents_Search(".", "segments");
	while (HUtility::FindContents(segmentkey))
	{
		originalkey = HC_KOpen_Segment("original");
		HC_Move_By_Key(segmentkey, ".");
		HC_Close_Segment();		


	}
	HC_End_Contents_Search();
	copykey = HC_KCreate_Segment("copy");
	HC_Close_Segment();
	BuildSegmentTreeRecursive(originalkey, copykey);



}
 
void HShellVertexData::SetKey(HC_KEY const key){
	if (key == INVALID_KEY){
		ResetMyself();
	}
	else {
		m_Key = key;
		HC_Show_Key_Type(key, m_sType);
		HC_Show_Shell_Size(key, &m_VertexCount, 0);
		m_pFIndexArray = new float[m_VertexCount];
		memset(m_pFIndexArray, 0, sizeof(float) * m_VertexCount);
		
	}
	return;
}

void HShellVertexData::SetFIndexData(int const size, float const * data){
	if (m_pFIndexArray)
		delete [] m_pFIndexArray;
	m_pFIndexArraySize = size;
	m_pFIndexArray = new float[size];
	memcpy(m_pFIndexArray, data, size * sizeof(float));
	return;
}

void HShellVertexData::ResetMyself(){
	m_Key = INVALID_KEY;
	m_VertexCount = 0;
	if (m_pFIndexArray)
		delete [] m_pFIndexArray;
	m_pFIndexArray = 0;
	m_sType[0] = '\0';
	return;
}

void HShellVertexData::GetFIndexData(float *data) const {
	memcpy(data, m_pFIndexArray, m_pFIndexArraySize * sizeof(float));
}


class HVerticalUtilityInternal :
	public HUtility
{
public:
	static void ComparePointsToShell(HC_KEY const points, HC_KEY const shell, VArray<float> &);

	class CompareCloudToShell{
	private:
		VArray<float> * m_pResults;
		HC_KEY m_cloud;
	public:
		CompareCloudToShell(HC_KEY cloud, VArray<float> * const result) : m_cloud(cloud), m_pResults(result) {};
		void operator () (HC_KEY const) const;
	};

	class CompareCloudToShells
	{
	private:
		VArray<HC_KEY> * m_shells;
	public:
		CompareCloudToShells(VArray<HC_KEY> * const shells) : m_shells(shells) {};
		void operator () (HC_KEY const) const;
	};

	class ComparePointsToShellFunc {
	private:
		int m_pcount;
		HPoint const * m_points;
		double m_tolerance;
		int m_shell_pcount;
		HPoint const * m_shell_points;
		int m_flist_len;
		int const * m_flist;
		float * m_results;

		bool FindMinMax(float & min, float & max) const;

	public:
		ComparePointsToShellFunc(int, HPoint const *, double, int, HPoint const *, int, int const *, float *);
		void operator () ();
	};
};



void HVerticalUtilityInternal::CompareCloudToShell::operator () (HC_KEY const shell) const {
	ComparePointsToShell(m_cloud, shell, *m_pResults);
};

void HVerticalUtilityInternal::CompareCloudToShells::operator () (HC_KEY const cloud) const {

	HC_Open_Geometry(cloud);{
		if(HC_Show_Existence("user options = colorized")){
			char options[100] = {""};
			HC_Show_One_User_Option("colorized", options);
			if(streq(options, "true")){
				HC_Close_Geometry();
				return;
			}
		}
	}HC_Close_Geometry();

	VArray<float> results;
	CompareCloudToShell compare_func(cloud, &results);
	int count = (int)m_shells->Count();
	while(count--){
		compare_func(m_shells->GetData(count));
	}

	HC_MSet_Vertex_Colors_By_FIndex(cloud, "markers", 0, (int)results.Count(), &results[0]);

	HC_Open_Geometry(cloud);{
		HC_Set_User_Options("colorized = true");
	}HC_Close_Geometry();

	HC_Update_Display();
};


void HVerticalUtility::ColorizePointCloud(HC_KEY const segment){
	HC_Open_Segment_By_Key(segment);{

		VArray<HC_KEY> shell_keys;
		VArray<HC_KEY> point_cloud_keys;

		shell_keys.SetCount(0);
		point_cloud_keys.SetCount(0);

		HC_Begin_Contents_Search("...", "shells");{
			HC_KEY key;
			while(HC_Find_Contents(0, &key)){
				int flist_len, point_count;
				HC_Show_Shell_Size(key, &point_count, &flist_len);
				if(point_count == 0) continue;
				if(flist_len > 0) {
					shell_keys.Append(key);
				} else {
					point_cloud_keys.Append(key);
				}
			}
		}HC_End_Contents_Search();

		if(shell_keys.Count() > 0 && point_cloud_keys.Count() > 0){
			HVerticalUtilityInternal::CompareCloudToShells compare_func(&shell_keys);
			int count = (int)point_cloud_keys.Count();
			while(count--){
				compare_func(point_cloud_keys.GetData(count));
			}
		}

	}HC_Close_Segment();
}

void HVerticalUtilityInternal::ComparePointsToShell(HC_KEY const cloud, HC_KEY const shell, VArray<float> & results){

	int cloud_pcount, not_used;
	HC_Show_Shell_Size(cloud, &cloud_pcount, &not_used);

	results.SetCount(0);
	results.EnsureSize(cloud_pcount);

	HPoint * cloud_points;
	HC_Show_Geometry_Pointer(cloud, "points", &cloud_points);

	int shell_pcount, shell_flist_len;
	HC_Show_Shell_Size(shell, &shell_pcount, &shell_flist_len);

	HPoint * shell_points;
	HC_Show_Geometry_Pointer(shell, "points", &shell_points);

	int * shell_flist;
	HC_Show_Geometry_Pointer(shell, "face list", &shell_flist);

	ComparePointsToShellFunc compair_func(
		cloud_pcount,
		cloud_points,
		500, 
		shell_pcount, shell_points, shell_flist_len, shell_flist,
		&results[0]);
	compair_func();

	results.SetCount(cloud_pcount);
}

HVerticalUtilityInternal::ComparePointsToShellFunc::ComparePointsToShellFunc
									(int pcount, HPoint const * points,
									 double tolerance,
									 int shell_pcount, HPoint const * shell_points, int flist_len, int const * flist,
									 float * results)
									 :	m_pcount(pcount), m_points(points),
										m_tolerance(tolerance),
										m_shell_pcount(shell_pcount), m_shell_points(shell_points),
										m_flist_len(flist_len), m_flist(flist),
										m_results(results){
}

bool HVerticalUtilityInternal::ComparePointsToShellFunc::FindMinMax(float & min, float & max) const {
	float min_dist = -1, max_dist = -1;
	int index = 0;
	bool retval = false;

	while(index < m_pcount){
		if(m_results[index] >= 0){
			min_dist = max_dist = m_results[index];
			++index;
			retval = true;
			break;
		}
		++index;
	}

	while(index < m_pcount){
		if(m_results[index] >= 0){
			if(m_results[index] < min_dist){
				min_dist = m_results[index];
			}
			if(m_results[index] > max_dist){
				max_dist = m_results[index];
			}
		}
		++index;
	}

	min = min_dist;
	max = max_dist;

	return retval;
}

void HVerticalUtilityInternal::ComparePointsToShellFunc::operator () () {


	HC_Compute_Points_Distance_From_Shell(m_pcount, m_points,m_tolerance,
		m_shell_pcount, m_shell_points, m_flist_len, m_flist,"mode = grid",
		&m_results[0]);

	float min_dist = -1, max_dist = -1;
	if(FindMinMax(min_dist, max_dist)){
		for(int i = 0; i < m_pcount; ++i){
			if(m_results[i] < 0)
				m_results[i] = 13;
			else {
				m_results[i] *= 12/500.0f;
			}
		}
	} else {
		for(int i = 0; i < m_pcount; ++i){
			m_results[i] = 13;
		}
	}		
}







bool HAbsoluteWindow::SetWindow(HBaseView *view, float xpos, float ypos, float width, float height,  AbsoluteWindowType xpos_type, 
		AbsoluteWindowType ypos_type, AbsoluteWindowType width_type, AbsoluteWindowType height_type, bool sb)
{
	HC_KEY key = HC_KCreate_Segment(".");
	HAbsoluteWindow aw(view, key, xpos, ypos, width, height, xpos_type, ypos_type, width_type, height_type,sb);
	return aw.Adjust();
}


 


HAbsoluteWindow::HAbsoluteWindow(HBaseView *view, HC_KEY key, float xpos, float ypos, float width, float height,  AbsoluteWindowType xpos_type, 
		AbsoluteWindowType ypos_type, AbsoluteWindowType width_type, AbsoluteWindowType height_type, bool showbackground)
{
	m_key = key;
	m_pView = view;
	m_bShowBackground = showbackground;
	char text[MVO_BUFFER_SIZE];
	sprintf(text,"aw_xpos = %f, aw_ypos = %f, aw_width = %f, aw_height = %f, aw_xpost = %d,aw_ypost = %d, aw_widtht = %d, aw_heightt = %d, showbg = %d",
		xpos, ypos, width, height, xpos_type, ypos_type, width_type, height_type, m_bShowBackground);
	HC_Open_Segment_By_Key(key);
	HC_Set_User_Options(text);
	DecipherOptions();
	HC_Close_Segment();
}
	
HAbsoluteWindow::HAbsoluteWindow(HBaseView *view, HC_KEY key)
{
	m_key = key;
	m_pView = view;
	HC_Open_Segment_By_Key(key);
 	DecipherOptions();
	HC_Close_Segment();
}

 

void HAbsoluteWindow::DecipherOptions()
{
	char text[MVO_BUFFER_SIZE];
	HC_Open_Segment_By_Key(m_key);
	HC_Show_One_User_Option("aw_xpos", text);
	m_xpos = atof(text);
	HC_Show_One_User_Option("aw_ypos", text);
	m_ypos = atof(text);
	HC_Show_One_User_Option("aw_width", text);
	m_width = atof(text);
	HC_Show_One_User_Option("aw_height", text);
	m_height = atof(text);
	HC_Show_One_User_Option("showbg", text);
	m_bShowBackground = bool2BOOL(atoi(text));
	HC_Show_One_User_Option("aw_xpost", text);
	m_xpos_type = (AbsoluteWindowType)atoi(text);
	HC_Show_One_User_Option("aw_ypost", text);
	m_ypos_type = (AbsoluteWindowType)atoi(text);
	HC_Show_One_User_Option("aw_widtht", text);
	m_width_type = (AbsoluteWindowType)atoi(text);
	HC_Show_One_User_Option("aw_heightt", text);
	m_height_type = (AbsoluteWindowType)atoi(text);
 	HC_Close_Segment();
 }

static void MakeKeyPath(HBaseView *view, HC_KEY key, HC_KEY *path, int &pathlen)
{
	pathlen = 0;
	HC_KEY current_key = key;
	while (1)
	{
		path[pathlen++] = current_key;
		if (current_key == view->GetModelKey())
			current_key = view->GetOverwriteKey();
		else if (current_key == view->GetViewKey())
			break;
		else
			current_key = HC_KShow_Owner_By_Key(current_key);
	}
}


void HAbsoluteWindow::AdjustX(float invalue, AbsoluteWindowType wt, float &outvalue)
{
	HC_KEY keypath[256];
	int pathlen;
	MakeKeyPath(m_pView, HC_KShow_Owner_By_Key(m_key), keypath, pathlen);
	HPoint pos, pos_res, pos_res_temp;
	if (wt == RelativeLeft)
	{
		pos.Set(invalue, 0,0);
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_key));
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local pixels", &pos, "local window", &pos_res);
		HC_Close_Segment();
	}
	else if (wt == RelativeRight)
	{
		pos.Set(1, 0,0);
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_key));
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local window", &pos, "local pixels", &pos_res_temp);
			pos_res_temp.x-=invalue;
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local pixels", &pos_res_temp, "local window", &pos_res);
		HC_Close_Segment();
	}
	outvalue = pos_res.x;
}

void HAbsoluteWindow::AdjustY(float invalue, AbsoluteWindowType wt, float &outvalue)
{
	HPoint pos, pos_res, pos_res_temp;
	HC_KEY keypath[256];
	int pathlen;
	MakeKeyPath(m_pView, HC_KShow_Owner_By_Key(m_key), keypath, pathlen);

	if (wt == RelativeTop)
	{
		pos.Set(0,invalue,0);
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_key));
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local pixels", &pos, "local window", &pos_res);
		HC_Close_Segment();
	}
	else if (wt == RelativeBottom)
	{
		pos.Set(0, -1,0);
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_key));
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local window", &pos, "local pixels", &pos_res_temp);
			pos_res_temp.y-=invalue;
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local pixels", &pos_res_temp, "local window", &pos_res);
		HC_Close_Segment();
	}
	outvalue = pos_res.y;
}



bool HAbsoluteWindow::Adjust()
{
	float left, right, bottom, top;
	if (m_xpos_type == NotRelative)
		left = m_xpos;
	else
 		AdjustX(m_xpos, m_xpos_type, left);
 
	AdjustY(m_ypos, m_ypos_type, top);
 
	if (m_width_type == RelativeLeft)
	{
		if (m_xpos_type == RelativeLeft)
			AdjustX(m_xpos + m_width, m_width_type, right);
		else if (m_xpos_type == RelativeRight)
			AdjustX(m_xpos - m_width, RelativeRight, right);
		else
		{
			HC_KEY keypath[256];
			int pathlen;
			MakeKeyPath(m_pView, HC_KShow_Owner_By_Key(m_key), keypath, pathlen);
			HPoint pos(m_xpos,0,0), pos_res;
			HC_Compute_Coordinates_By_Path(pathlen, keypath, "local window", &pos, "local pixels", &pos_res);
			AdjustX(pos_res.x + m_width, m_width_type, right);
		}
	}
	else
 		AdjustX(m_width, m_width_type, right);

	if (m_height_type == RelativeTop)
	{
		if (m_ypos_type == RelativeTop)
			AdjustY(m_ypos + m_height, m_height_type, bottom);
		else
			AdjustY(m_ypos - m_height, RelativeBottom, bottom);
	}
	else
		AdjustY(m_height, m_height_type, bottom);
	bool outside = false;
	HC_Open_Segment_By_Key(m_key);
	if ((left < -0.99f && right < -0.99f) || (left > 0.99f && right > 0.99f) ||
		(top < -0.99f && bottom < -0.99f) || (top > 0.99f && bottom > 0.99f) ||
		(top == 0.0f && bottom == 0.0f && left == 0.0f && right == 0.0f) || 
		(bottom > top))
		outside = true;
	else
	HC_Set_Window(left, right, bottom, top);
//	Set_Screen_Range_Window(left, right, bottom, top);
	HC_Close_Segment();

	if (m_bShowBackground && !outside)
	{
		HPoint p1,p2;
		HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(m_key));
		p1.Set(left+0.005f,top-0.007f,0.0f);
		p2.Set(right+0.005f,bottom-0.007f,0.0f);
    
		   HC_Flush_Contents(".", "geometry");
		HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
		HC_KEY bkey = HUtility::InsertBox(&p1, &p2);
		HC_Open_Geometry(bkey);
		HC_Set_Color("faces = dark grey");
		HC_Close_Geometry();
		HC_Set_Visibility("cutting planes = off");
		HC_Close_Segment();
	}

	return outside;

}


void HAbsoluteWindow::AdjustAll(HBaseView *view)
{
	HC_KEY segkey;
	HC_Begin_Contents_Search("...", "segments");
	while (HUtility::FindContents(segkey))
	{
		HC_Open_Segment_By_Key(segkey);
		if (HC_Show_Existence("user options = aw_xpos"))
		{
			HAbsoluteWindow aw(view, segkey);
			aw.Adjust();
		}
		HC_Close_Segment();
	}
	HC_End_Contents_Search();
}



bool HAbsoluteWindow::IsPositionInWindow(float x, float y)
{
	HPoint pos, pos_res, pos_res_temp;
	HC_KEY keypath[256];
	int pathlen;
	MakeKeyPath(m_pView, m_key, keypath, pathlen);
	HPoint inpoint(x,y,0), outpoint;
	HC_Compute_Coordinates_By_Path(pathlen, keypath, "outer window", &inpoint, "local window", &outpoint);
	if (outpoint.x >=-1.0f && outpoint.x <=1.0f && outpoint.y >=-1.0f && outpoint.y <=1.0f)
		return true;
	else
		return false;
}

bool HAbsoluteWindow::IsPositionInWindow(HBaseView *view, float x, float y)
{
	HC_KEY segkey = HC_KCreate_Segment(".");
	if (!HC_Show_Existence("user options = aw_xpos"))
		return false;
 	HAbsoluteWindow aw(view, segkey);
	return aw.IsPositionInWindow(x,y);
}

void HAbsoluteWindow::CalculateLocalWindowPos(float x, float y, float &xout, float &yout)
{
	HPoint pos, pos_res, pos_res_temp;
	HC_KEY keypath[256];
	int pathlen;
	MakeKeyPath(m_pView, m_key, keypath, pathlen);
	HPoint inpoint(x,y,0), outpoint;
	HC_Compute_Coordinates_By_Path(pathlen, keypath, "outer window", &inpoint, "local window", &outpoint);
	xout = outpoint.x;
	yout = outpoint.y;
 }

void HAbsoluteWindow::CalculateOuterWindowPos(float x, float y, float &xout, float &yout)
{
	UNREFERENCED(x);
	UNREFERENCED(y);

	HPoint pos, pos_res, pos_res_temp;
	HC_KEY keypath[256];
	int pathlen;
	MakeKeyPath(m_pView, m_key, keypath, pathlen);
	HPoint inpoint(-1,-1,-1), outpoint;
	HC_Compute_Coordinates_By_Path(pathlen, keypath, "local window", &inpoint, "outer window", &outpoint);
	xout = outpoint.x;
	yout = outpoint.y;
 }

void HAbsoluteWindow::CalculateLocalWindowPos(HBaseView *view, float x, float y, float &xout, float &yout)
{
	HC_KEY segkey = HC_KCreate_Segment(".");
	HAbsoluteWindow aw(view, segkey);
	aw.CalculateLocalWindowPos(x,y, xout, yout);
}
void HAbsoluteWindow::CalculateOuterWindowPos(HBaseView *view, float x, float y, float &xout, float &yout)
{
	HC_KEY segkey = HC_KCreate_Segment(".");
	HAbsoluteWindow aw(view, segkey);
	aw.CalculateOuterWindowPos(x,y, xout, yout);
}

 
HUnicodeOptions::HUnicodeOptions()
{
	m_length = 0;
	m_data = 0;
	m_valid = false;
}

HUnicodeOptions::HUnicodeOptions(const HUnicodeOptions& in)
{
	clear();
	m_length = in.m_length;
	m_valid = in.m_valid;
	if (m_length > 0)
	{
		m_data = new wchar_t[m_length+1];
		wcscpy(m_data, in.m_data);
	}
}

HUnicodeOptions::~HUnicodeOptions()
{
	clear();
}

bool HUnicodeOptions::IsValid()
{
	return m_valid;
}

const wchar_t* HUnicodeOptions::Show_Unicode_Options()
{
	clear();
	H_UTF16 utf16((L"user options"));
	if (HC_Show_Existence_Unicode((const unsigned short*)utf16.encodedText()))
	{
		clear();
		HC_Show_Unicode_Options_Length(&m_length);
		if (m_length > 0)
		{
			unsigned short * tmp = new unsigned short[m_length+1];
			HC_Show_Unicode_Options(tmp);
			H_UTF16 utf16_data;
			utf16_data.encodedText((utf16_char const*)tmp);
			delete [] tmp;
			H_WCS wcs(utf16_data);
			m_data = new wchar_t[wcs.length() +1];
			wcscpy(m_data, wcs.encodedText());
			m_valid = true;
		}
	}

	return m_data;
}

#ifdef _MSC_VER
const unsigned short * HUnicodeOptions::Show_One_Net_Unicode_Option(const unsigned short * which) {
	return reinterpret_cast<unsigned short const *>(
		Show_One_Net_Unicode_Option(reinterpret_cast<wchar_t const *>(which))
	);
}
#endif

const wchar_t* HUnicodeOptions::Show_One_Net_Unicode_Option( const wchar_t * which)
{
	Show_Net_Unicode_Options();

	if (m_data && wcsstr(m_data, which))
	{
		clear();
		H_UTF16 utf16_which(which);
		HC_Show_One_Net_Unicode_Option_Length((const unsigned short*)utf16_which.encodedText(), &m_length);
		if (m_length > 0)
		{
			unsigned short * tmp = new unsigned short[m_length+1];
			HC_Show_One_Net_Unicode_Option((const unsigned short*)utf16_which.encodedText(), tmp);
			H_UTF16 utf16_data;
			utf16_data.encodedText((utf16_char const*)tmp);
			delete [] tmp;
			H_WCS wcs(utf16_data);
			m_data = new wchar_t[wcs.length() +1];
			wcscpy(m_data, wcs.encodedText());
			m_valid = true;
		}
	} 
	else 
	{
		clear();		
	}


	return m_data;
}

#ifdef _MSC_VER
const unsigned short * HUnicodeOptions::Show_One_Unicode_Option(const unsigned short * which){
	return reinterpret_cast<unsigned short const *>(
		Show_One_Unicode_Option(reinterpret_cast<wchar_t const *>(which))
	);
}
#endif

const wchar_t * HUnicodeOptions::Show_One_Unicode_Option(const wchar_t * which)
{
	clear();
	
	wchar_t * option_string;
	m_length = static_cast<int>(wcslen(L"user options = ") + wcslen(which));
	option_string = new wchar_t[m_length+1];
	swprintf(option_string, m_length+1, L"user options = %ls", which);
	
	H_UTF16 utf16_option_string(option_string);
	if (HC_Show_Existence_Unicode((const unsigned short*)utf16_option_string.encodedText()))
	{
		clear();
		H_UTF16 utf16_which(which);
		HC_Show_One_Unicode_Option_Length((const unsigned short*)utf16_which.encodedText(), &m_length);
		if (m_length > 0)
		{
			unsigned short * tmp = new unsigned short[m_length+1];
			HC_Show_One_Unicode_Option((const unsigned short*)utf16_which.encodedText(), tmp);
			H_UTF16 utf16_data;
			utf16_data.encodedText((utf16_char const*)tmp);
			delete [] tmp;
			H_WCS wcs(utf16_data);
			m_data = new wchar_t[wcs.length() +1];
			wcscpy(m_data, wcs.encodedText());
			m_valid = true;
		}
	}

	H_SAFE_DELETE_ARRAY(option_string);
	return m_data;
}

const wchar_t* HUnicodeOptions::Show_Net_Unicode_Options()
{
	clear();

	HC_Show_Net_Unicode_Options_Length(&m_length);
	if (m_length > 0)
	{
		unsigned short * tmp = new unsigned short[m_length+1];
		HC_Show_Net_Unicode_Options(tmp);
		H_UTF16 utf16_data;
		utf16_data.encodedText((utf16_char const*)tmp);
		delete [] tmp;
		H_WCS wcs(utf16_data);
		m_data = new wchar_t[wcs.length() +1];
		wcscpy(m_data, wcs.encodedText());
		m_valid = true;
	} 

	return m_data;
}


BREP_Face::BREP_Face(HC_KEY facekey)
{
	m_facekey = facekey;	
	m_CoEdgeList = new_vlist(malloc, free);
}

BREP_Face::~BREP_Face()
{
	START_LIST_ITERATION(BREP_CoEdge, m_CoEdgeList);
	delete temp;
	END_LIST_ITERATION(m_CoEdgeList);
 	delete_vlist(m_CoEdgeList);
}
BREP_Edge * BREP_Face::AddCoEdge(BREP_Topology *topol, void *edgeid, HC_KEY edgekey)
{
	BREP_Edge *edge = topol->FindEdge(edgeid);
	if (!edge)
		edge = topol->AddEdge(edgeid, edgekey);

	BREP_CoEdge *coedge = new BREP_CoEdge(edge);
 	vlist_add_last(m_CoEdgeList, (void *)coedge);
	return edge;

}


BREP_Edge *BREP_Topology::FindEdge(void *edgeid)
{
	BREP_Edge *edge;
	int res = vhash_lookup_item(m_EdgeHash, (void *)edgeid , (void **)&edge);
	if  (res == VHASH_STATUS_SUCCESS)
		return edge;
	else
		return 0;
}

BREP_Edge *BREP_Topology::AddEdge(void *edgeid, HC_KEY edgekey)
{

	BREP_Edge *edge = new BREP_Edge(edgekey);
	vhash_insert_item(m_EdgeHash, (void *)edgeid  , (void *)edge);
	return edge;
}

HC_KEY BREP_Topology::FindVertex(void *vertexid)
{
	HC_KEY vertexkey;
	int res = vhash_lookup_item(m_VertexHash, (void *)vertexid , (void **)&vertexkey);
	if (res == VHASH_STATUS_SUCCESS)
		return vertexkey;
	else 
		return INVALID_KEY;
}

HC_KEY BREP_Topology::AddVertex(void *vertexid, HPoint vpos)
{
	HC_KEY vertexkey;

	vertexkey = HC_KInsert_Marker(vpos.x, vpos.y, vpos.z);
	vhash_insert_item(m_VertexHash, (void *)vertexid  , (void *)vertexkey);
	return vertexkey;
}


BREP_Topology::BREP_Topology()
{
	m_FaceHash = new_vhash(10,malloc, free);
	m_VertexHash = new_vhash(10,malloc, free);
	m_EdgeHash = new_vhash(10,malloc, free);
}
BREP_Topology::~BREP_Topology()
{
	vlist_s *temp_list = new_vlist(malloc,free);
	vhash_to_vlist(m_FaceHash, temp_list, malloc);
	START_LIST_ITERATION(vhash_pair_t, temp_list);
	delete (BREP_Face *)temp->item;
	delete temp;
 	END_LIST_ITERATION(temp_list);
	delete_vhash(m_FaceHash);
	delete_vlist(temp_list);

	temp_list = new_vlist(malloc,free);
	vhash_to_vlist(m_EdgeHash, temp_list, malloc);
	START_LIST_ITERATION(vhash_pair_t, temp_list);
	delete (BREP_Edge *)temp->item;
	delete temp;
	END_LIST_ITERATION(temp_list);
	delete_vhash(m_EdgeHash);
	delete_vlist(temp_list);
	
	delete_vhash(m_VertexHash);

}


BREP_Face * BREP_Topology::GetFace(HC_KEY facekey)
{
	BREP_Face *face;
	int res = vhash_lookup_item(m_FaceHash, (void *)facekey , (void **)&face);
	if  (res == VHASH_STATUS_SUCCESS)
		return face;
	else
		return 0;
}



BREP_Face * BREP_Topology::AddFace(HC_KEY facekey)
{
	BREP_Face *face = new BREP_Face(facekey);
	m_CurrentFace = face;
	vhash_insert_item(m_FaceHash, (void *)facekey  , (void *)face);
	return face;
}


HBestImageDriver::HBestImageDriver(char const * prefer_type) : m_dvr(0)
{
	if(prefer_type && (strstr(prefer_type, "opengl") || strstr(prefer_type, "dx") || strstr(prefer_type, "image"))){
		if(HC_QShow_Existence(H_FORMAT_TEXT ("/driver/%s", prefer_type), "self")){
			m_dvr = new H_FORMAT_TEXT("/driver/%s/image %d", prefer_type, HDB::GetUniqueID());
			HC_Open_Segment(*m_dvr);
				HC_Set_Driver_Options("debug = 0x100000");//USE WINDOW IS IMAGE 
			HC_Close_Segment();
			return;
		}
	}

	if(HC_QShow_Existence("/driver/dx9", "self")){
		m_dvr = new H_FORMAT_TEXT("/driver/dx9/image %d", HDB::GetUniqueID());
		HC_Open_Segment(*m_dvr);
			HC_Set_Driver_Options("debug = 0x100000");//USE WINDOW IS IMAGE 
		HC_Close_Segment();

		return;
	}

	if(HC_QShow_Existence("/driver/dx11", "self")){
		m_dvr = new H_FORMAT_TEXT("/driver/dx11/image %d", HDB::GetUniqueID());
		HC_Open_Segment(*m_dvr);
			HC_Set_Driver_Options("debug = 0x100000");//USE WINDOW IS IMAGE 
		HC_Close_Segment();

		return;
	}

	if(HC_QShow_Existence("/driver/opengl2", "self")){
		m_dvr = new H_FORMAT_TEXT("/driver/opengl2/image %d", HDB::GetUniqueID());
		HC_Open_Segment(*m_dvr);
			HC_Set_Driver_Options("debug = 0x100000");//USE WINDOW IS IMAGE 
		HC_Close_Segment();
		return;
	}

	if(HC_QShow_Existence("/driver/opengl", "self")){
		m_dvr = new H_FORMAT_TEXT("/driver/opengl/image %d", HDB::GetUniqueID());
		HC_Open_Segment(*m_dvr);
			HC_Set_Driver_Options("debug = 0x100000");//USE WINDOW IS IMAGE 
		HC_Close_Segment();
		return;
	}

	m_dvr = new H_FORMAT_TEXT("/driver/image/image %d", HDB::GetUniqueID());
}

HBestImageDriver::~HBestImageDriver()
{
	delete m_dvr;
}

HC_KEY HBestImageDriver::GetKey() const
{
	HC_KEY key = HC_KOpen_Segment(*m_dvr);
	HC_Close_Segment();
	return key;
}

HBestImageDriver::operator char const* () const
{
	return *m_dvr;
}

#ifndef REG_NONE
#define REG_NONE                    ( 0 )   /* No value type */
#define REG_SZ                      ( 1 )   /* Unicode nul terminated string */
#define REG_EXPAND_SZ               ( 2 )   /* Unicode nul terminated string */
                                            /* (with environment variable references) */
#define REG_BINARY                  ( 3 )   /* Free form binary */
#define REG_DWORD                   ( 4 )   /* 32-bit number */
#define REG_DWORD_LITTLE_ENDIAN     ( 4 )   /* 32-bit number (same as REG_DWORD) */
#define REG_DWORD_BIG_ENDIAN        ( 5 )   /* 32-bit number */
#define REG_LINK                    ( 6 )   /* Symbolic Link (unicode) */
#define REG_MULTI_SZ                ( 7 )   /* Multiple Unicode strings */
#define REG_RESOURCE_LIST           ( 8 )   /* Resource list in the resource map */
#define REG_FULL_RESOURCE_DESCRIPTOR ( 9 )  /* Resource list in the hardware description */
#define REG_RESOURCE_REQUIREMENTS_LIST ( 10 )
#define REG_QWORD                   ( 11 )  /* 64-bit number */
#define REG_QWORD_LITTLE_ENDIAN     ( 11 )  /* 64-bit number (same as REG_QWORD) */
#endif

struct offsets {
    int32_t  block_size;
    char  block_type[2]; /* "lf" "il" "ri" */
    int16_t count;
    int32_t  first;
    int32_t  hash;
};

struct key_block  {
    int32_t  block_size;
    char  block_type[2]; /* "nk" */
    char  dummya[18];
    int32_t   subkey_count;
    char  dummyb[4];
    int32_t   subkeys;
    char  dummyc[4];
    int32_t   value_count;
    int32_t   offsets;
    char  dummyd[28];
    int16_t len;
    int16_t du;
    char  name;
};

struct value_block {
    int32_t  block_size;
    char  block_type[2]; /* "vk" */
    int16_t name_len;
    int32_t  size;
    int32_t offset;
    int32_t value_type;
    int16_t flags;
    int16_t dummy;
    char  name;
};

static void HPVOOptions_walk ( map<string, string> & value_map, char* path, struct key_block* key ) {
    struct offsets * item = 0;
    static char * root = 0;
    static char * full = 0;
    int32_t index = 0;

    root = (char*)key-0x20;
    full = path;

    /* add current key name to printed path */
    memcpy(path++, "/", 2);
    memcpy(path, &key->name, key->len);
    path += key->len;

    /* print all contained values */
    for(index = 0; index < key->value_count; ++index){
        struct value_block* val = (struct value_block*)(((int32_t*)(key->offsets + root + 4))[index] + root);
        int32_t size = val->size;
        int32_t i = 0;
        char * data = 0;

        /* For small values MS added optimization where
        // if bit 31 is set data are contained wihin the key itself to save space */
        if(size & 0x80000000) {
            size &= 0x7fffffff;
            data = (char*)&val->offset;
        } else {
            /* data are usually in separate blocks without types */
            data = root + val->offset + 4;
        }

        /* notice that we use memcopy for key/value names everywhere instead of strcat
        // reason is that malware/wiruses often write non nulterminated strings to
        // hide from win32 api */
        *path = '/';
        if(!val->name_len)
            *path=' ';
        memcpy(path+1, &val->name, val->name_len);
        path[val->name_len +1] = '\0';

        switch(val->value_type){
            case REG_MULTI_SZ:
            case REG_SZ :{
                string tmp;
                int j = 0;
                tmp.resize(size +1, '\0');
                for(i = 0; i < size; ++i){
                    if(data[i])
                        tmp[j++] = data[i];
                }
                value_map[full] = tmp;
            }break;

            case REG_DWORD: {
                char int_value[15] = {""};
                sprintf(int_value, "%d", *(int32_t*)&val->offset);
                value_map[full] = int_value;
            }break;

            default:
                   assert(0); /* unhandled type. */
        }
    }

    /* for simplicity we can imagine keys as directories in filesystem and values
    // as files.
    // and since we already dumped values for this dir we will now iterate
    // thru subdirectories in the same way */

    item = (struct offsets*)(root+key->subkeys);
    for(index = 0; index < item->count; ++index){
        /* in case of too many subkeys this list contain just other lists */
        struct offsets* subitem = (struct offsets*)((&item->first)[index]+root);
        int32_t i = 0;

        /* usual directory traversal */
        if(item->block_type[1] == 'f' || item->block_type[1] == 'h') {
            /* for now we skip hash codes (used by regedit for faster search) */
            HPVOOptions_walk(value_map, path, (struct key_block*)((&item->first)[i*2] + root));
        } else {
            int32_t j;
            for(j = 0; j < subitem->count; ++j) {
                /* also ms had chosen to skip hashes altogether in this case */
                HPVOOptions_walk(value_map, path, (struct key_block*)((&subitem->first)[item->block_type[1] == 'i' ? j * 2 : j] + root));
            }
        }
    }
}

static int HPVOOptions_map_hive_values(map<string, string> & value_map, char const * filename){
    char path[0x1000] = {""};
    char *data = 0;
    int32_t size;
    FILE *f;

    if((f = fopen(filename,"rb")) == 0)
        return -1;

    fseek(f,0,SEEK_END);
    if((size = ftell(f)) == 0){
        fclose(f);
        return -2;
    }

    rewind(f);
    data = (char*)malloc(size);
    if(fread(data, size, 1, f) != 1){
        fclose(f);
        return -3;
    }
    fclose(f);

    /* we just skip 1k header and start walking root key tree */
    HPVOOptions_walk (value_map, path, (struct key_block*)(data+0x1020));
    free(data);

    return 0;
}

HPVOOptions::HPVOOptions(char const * filename) : impl(0) {
    impl = new map<string, string>;
    HPVOOptions_map_hive_values(*(map<string, string>*)impl, filename);
};

HPVOOptions::~HPVOOptions(){
    delete (map<string, string>*)impl;
}

char const * HPVOOptions::getOption(char const * option) const {
    map<string, string>::const_iterator iter = ((map<string, string>*)impl)->find(option);
    if(iter == ((map<string, string>*)impl)->end())
        return 0;
    return iter->second.c_str();
}
