// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityShadow.cpp : implementation of the HShadow (and related) classes
//

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
  
#include "hc.h"

#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HUtility.h"
#include "HUtilityShadow.h"
 
static int texnum = 1;  

#define SHADOW_BRIGHTNESS 180
#define FILTER_SIZE 7
#define TEXTURE_SIZE 64

static float ident[16] = { 1,0,0,0,
						   0,1,0,0,
						   0,0,1,0,
						   0,0,0,1};


	
static	float vcor[12] = { 0.0, 0.0, 0.0,
							0.0, 1.0, 0.0,
							1.0, 0.0, 0.0,
							1.0, 1.0, 0.0};
	


static int filter[FILTER_SIZE*FILTER_SIZE] = {  1,1,1,1,1,1,1,
										    	1,1,1,1,1,1,1,
												1,1,1,2,1,1,1,
												1,1,2,4,2,1,1,
												1,1,1,2,1,1,1,
												1,1,1,1,1,1,1,
												1,1,1,1,1,1,1};



/////////////////////////////////////////////////////////////////////////////
// HShadow


HShadow::HShadow(HC_KEY ModelSegmentKey, HC_KEY ShadowSegmentKey, float *ObjectModellingMatrix)
{
	m_ModelSegmentKey	= ModelSegmentKey;
	m_ShadowSegmentKey	= ShadowSegmentKey;
	m_ShadowBrightness = SHADOW_BRIGHTNESS;
	
	if (ObjectModellingMatrix)
	{
		for (int i=0;i<16;i++)
			m_ObjectModellingMatrix[i] = ObjectModellingMatrix[i];
	}
	else
		SetIdentityMatrix(m_ObjectModellingMatrix);
	
}

 

void HShadow::CalculateTotalModelExtents()
{
	m_bFirstCoordinate = true;		
  	CalculateModelExtents(m_ModelSegmentKey,(float *)m_ObjectModellingMatrix);	
}

 


 
void HShadow::CalculateModelExtents(HC_KEY segkey, float *totalmatrix)
{

	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
	char type[MVO_BUFFER_SIZE];
	HC_KEY key;
 
	float newtotalmatrix[16];
 	HC_Show_Segment(segkey, pathname);
	
	ShowModellingMatrix(segkey, totalmatrix, newtotalmatrix);
 

	HC_Begin_Contents_Search(pathname,"shells, include,segment");

	while (HC_Find_Contents(type,&key))
		{
		if (streq(type, "segment"))
 			CalculateModelExtents(key, (float *)newtotalmatrix);
		else if (streq(type,"include"))
 			CalculateModelExtents(HC_KShow_Include_Segment(key), (float *)newtotalmatrix);
 		else
		{
 				int numpoints, numfaces; 
				HC_Show_Shell_Size(key, &numpoints,&numfaces);
				HPoint *points = new HPoint[numpoints];
				HC_Show_Shell(key, &numpoints,points,&numfaces,0);
				HC_Compute_Transformed_Points(numpoints, points, newtotalmatrix,points);
				if (m_bFirstCoordinate)
				{
					m_ObjectBoundingMin = m_ObjectBoundingMax = points[0];
					m_bFirstCoordinate = false;
				}
				ComputeObjectExtents(m_ObjectBoundingMin, m_ObjectBoundingMax, points,numpoints );
				delete []points;
		}
	}	
	HC_End_Contents_Search();
}




void HShadow::ShowModellingMatrix(HC_KEY segkey, float *oldmatrix, float *newmatrix)
{
	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
   
  	HC_Show_Segment(segkey, pathname);
	if (HC_Show_Existence_By_Key(segkey, "modelling matrix"))
	{
		float matrix[16];

		HC_QShow_Modelling_Matrix(pathname,(float *)matrix);
		HC_Compute_Matrix_Product((float *)matrix, oldmatrix, (float *)newmatrix);
	}
	else
	{
		for (int i=0;i<16;i++)
			newmatrix[i] = oldmatrix[i];
	}
}



void HShadow::ComputeObjectExtents(HPoint &min, HPoint &max, HPoint *points, int numpoints)
{
	for (int i=0;i<numpoints;i++)
	{
 		if (points[i].x < min.x) min.x = points[i].x;
		if (points[i].y < min.y) min.y = points[i].y;
		if (points[i].z < min.z) min.z = points[i].z;

		if (points[i].x > max.x) max.x = points[i].x;
		if (points[i].y > max.y) max.y = points[i].y;
		if (points[i].z > max.z) max.z = points[i].z;
	}
}



void HShadow::SetIdentityMatrix(float *matrix)
{
	for (int i=0;i<16;i++)
		matrix[i] = ident[i];
}





void HShadow::RotateToGroundPlane(HPlane plane, float *matrix)
{
	HPoint orig,target;
	
 	orig.x = plane.a();
	orig.y = plane.b();
	orig.z = plane.c();
 
 	target.Set(0.0000001f, 0.0000001f,-1.0);
 	HUtility::ComputeVectorToVectorRotationMatrix(matrix,target,orig);	
}


void HShadow::ShadowMatrix(float *mm, HPlane plane, HPoint light)
{
	float m[4][4];
	float dot = plane.a()*light.x + plane.b()*light.y +
		plane.c()*light.z + plane.d()*1.0f;
	m[0][0] = dot - light.x*plane.a();
	m[1][0] = - light.x*plane.b();
	m[2][0] = - light.x*plane.c();
	m[3][0] = - light.x*plane.d();
	m[0][1] = - light.y*plane.a();
	m[1][1] = dot - light.y*plane.b();
	m[2][1] = - light.y*plane.c();
	m[3][1] = - light.y*plane.d();
	m[0][2] = - light.z*plane.a();
	m[1][2] = - light.z*plane.b();
	m[2][2] = dot - light.z*plane.c();
	m[3][2] = - light.z*plane.d();
	m[0][3] = - 1.0f*plane.a();
	m[1][3] = - 1.0f*plane.b();
	m[2][3] = - 1.0f*plane.c();
	m[3][3] = dot - 1.0f*plane.d();
 	for (int x=0;x<4;x++)
	{
		for (int y=0;y<4;y++)
		{
			mm[x+y*4]=m[y][x];
		}
	}
	
}



void HShadow::ShadowMatrix2(float *mm, HPlane plane, HPoint d)
{
	float m[4][4];
 	m[0][0] = plane.b() *d.y + plane.c() *d.z;
	m[1][0] = -plane.b() *d.x;
	m[2][0] = -plane.c() *d.x;
	m[3][0] = -plane.d() *d.x;

	m[0][1] = -plane.a() *d.y;
	m[1][1] =  plane.a() *d.x + plane.c() *d.z;
	m[2][1] = -plane.c() *d.y;
	m[3][1] = -plane.d() *d.y;

	m[0][2] = -plane.a() *d.z;
	m[1][2] = -plane.b() *d.z;
	m[2][2] =  plane.a() *d.x + plane.b() *d.y;
	m[3][2] = -plane.d() *d.z;

	m[0][3] =  0;
	m[1][3] =  0;
	m[2][3] =  0;
	m[3][3] =  plane.a()*d.x + plane.b() *d.y +plane.c() *d.z;

  	for (int x=0;x<4;x++)
	{
		for (int y=0;y<4;y++)
		{
			mm[x+y*4]=m[y][x];
		}
	}
	
}



HC_KEY HShadow::IsolateShell()
{

	char type[MVO_BUFFER_SIZE];
	HC_Show_Key_Type(m_ModelSegmentKey, type);
 	if (strcmp(type,"shell") == 0)
	{
		HC_KEY owner = HC_KShow_Owner_By_Key(m_ModelSegmentKey);
		HC_Open_Segment_By_Key(owner);
		HC_KEY tkey = HC_KOpen_Segment("temp");
		HC_Move_By_Key(m_ModelSegmentKey,".");
		HC_Close_Segment();
		HC_Close_Segment();
		return tkey;
	}
	else
		return m_ModelSegmentKey;
}


void HShadow::RestoreShell()
{

		char type[MVO_BUFFER_SIZE];
		HC_Show_Key_Type(m_ModelSegmentKey, type);
 		if (strcmp(type,"shell") == 0)
		{
			HC_KEY firstowner = HC_KShow_Owner_By_Key(m_ModelSegmentKey);
			HC_Open_Segment_By_Key(HC_KShow_Owner_By_Key(firstowner));
			HC_Move_By_Key(m_ModelSegmentKey,".");
			HC_Close_Segment();
			HC_Delete_By_Key(firstowner);
		}
}
		


bool HShadow::HasShell(HC_KEY segKey)
{
	bool b_result = false;

	char type[MVO_BUFFER_SIZE];
	HC_KEY key;
 
 	HC_Open_Segment_By_Key(segKey);
		HC_Begin_Contents_Search(".","shells, cylinders, nurbs surfaces, mesh, include, segment");
			while (HC_Find_Contents(type,&key))
			{
				if (streq(type, "segment"))
				{
 					if( HasShell(key) )
					{
						b_result = true;
						break;
					}
				}
				else if (streq(type,"include"))
				{
 					if( HasShell(HC_KShow_Include_Segment(key)) )
					{
						b_result = true;
						break;
					}
				}
 				else 
				{
					b_result = true;
					break;
				}
 
			}	
		HC_End_Contents_Search();
	HC_Close_Segment();

	return b_result;
}

/////////////////////////////////////////////////////////////////////////////
// HSmoothShadow


HSmoothShadow::	HSmoothShadow(HC_KEY ModelSegmentKey, HC_KEY ShadowSegmentKey, bool AllowOpenglShadow) : HShadow(ModelSegmentKey, ShadowSegmentKey)  
{
		m_FilterSize = FILTER_SIZE;
		m_TextureSize = TEXTURE_SIZE;
 		m_filter = filter;
		m_ImageKey = 0;	
		m_bAllowOpenglShadow = AllowOpenglShadow;
		int ktemp = (int)ModelSegmentKey;
		sprintf (m_texname, "%dshadow%d", ktemp, texnum);

}

HSmoothShadow::~HSmoothShadow()
{
	CleanUp();
}
	
void HSmoothShadow::CleanUp()
{
	if (HC_QShow_Existence("/shadowimages", "self") && m_ImageKey)
	HC_Delete_By_Key(m_ImageKey);
	m_ImageKey = 0;
}

void HSmoothShadow::UnDefineShadowTexture()
{
	HC_UnDefine_Texture (m_texname);
}
void HSmoothShadow::UnDefineAllShadowTextures(HC_KEY ModelSegmentKey)
{
	char texname[MVO_BUFFER_SIZE];
	char texfind[MVO_BUFFER_SIZE];
	int ktemp = (int)ModelSegmentKey;
	sprintf (texfind, "%dshadow", ktemp);

	HC_Begin_Texture_Search();
	while (HC_Find_Texture(texname))
	{
		if (strstr(texname, texfind))
			HC_UnDefine_Texture (texname);
	}
	HC_End_Texture_Search();

}
 

void HSmoothShadow::Create(ShadowPlaneType ptype, int TextureSize, int FilterSize, int *filter) 
{  	
	// if there are no shells in the model segment, we don't cast any shadows
	if( !HasShell( m_ModelSegmentKey) )
		return;

 	if (TextureSize)
		m_TextureSize = TextureSize;
	if (FilterSize>=0)
	{
		m_FilterSize = FilterSize;
		m_filter = filter;
	}


   	HC_Open_Segment_By_Key(m_ShadowSegmentKey);
  		CalculateTotalModelExtents();
 		switch (ptype)
		{
			case XZ_PLANE:
				m_ShadowGroundPlane.Set(0.0f,1.0f,0.0f, -m_ObjectBoundingMin.y);
				m_ShadowLightPos.Set((m_ObjectBoundingMin.x + m_ObjectBoundingMax.x)/2.0f,m_ObjectBoundingMax.y+10000.0f,(m_ObjectBoundingMin.z+m_ObjectBoundingMax.z)/2.0f);
 				break;
			case XY_PLANE:
				m_ShadowGroundPlane.Set(0.0f,0.0f,1.0f, -m_ObjectBoundingMin.z);
				m_ShadowLightPos.Set((m_ObjectBoundingMin.x+m_ObjectBoundingMax.x)/2.0f,(m_ObjectBoundingMin.y+m_ObjectBoundingMax.y)/2.0f,m_ObjectBoundingMax.z+10000.0f);
  				break;
			case YZ_PLANE:
				m_ShadowGroundPlane.Set(1.0f,0.0,0.0, -m_ObjectBoundingMin.x);
				m_ShadowLightPos.Set(m_ObjectBoundingMax.x+10000.0f, (m_ObjectBoundingMin.y+m_ObjectBoundingMax.y)/2.0f,(m_ObjectBoundingMin.z+m_ObjectBoundingMax.z)/2.0f);
 				break;
				
		}
   
		CalculateTotalShadowExtents();	
  			
		CreateSoftShadow();

		// set the shadow segment attributes
		HC_Set_Selectability("everything = off");

 	HC_Close_Segment();  
 	
}



void HSmoothShadow::Create() 
{  	
	// if there are no shells in the model segment, we don't cast any shadows
	if( !HasShell( m_ModelSegmentKey) )
		return;
	
   	HC_Open_Segment_By_Key(m_ShadowSegmentKey);

	   	CalculateTotalShadowExtents();				
		CreateSoftShadow();

		// set the shadow segment attributes
		HC_Set_Selectability("everything = off");

 	HC_Close_Segment();  
 	
}
 
 


void HSmoothShadow::CalculateShadowExtents(HC_KEY segkey,float *totalmatrix)
{

	char pathname[MVO_SEGMENT_PATHNAME_BUFFER];
	char type[MVO_BUFFER_SIZE];
	HC_KEY key;
 
	float newtotalmatrix[16];
 	HC_Show_Segment(segkey, pathname);
	ShowModellingMatrix(segkey, totalmatrix, newtotalmatrix);
 

	HC_Begin_Contents_Search(pathname,"shells, cylinders, nurbs surfaces, mesh, include,segment");

	while (HC_Find_Contents(type,&key))
		{
		if (streq(type, "segment"))
 			CalculateShadowExtents(key, (float *)newtotalmatrix);
		else if (streq(type,"include"))
 			CalculateShadowExtents(HC_KShow_Include_Segment(key), (float *)newtotalmatrix);
 		else
 			CalculateOneShadowExtent(key, newtotalmatrix);		
	}	
	HC_End_Contents_Search();
}


void HSmoothShadow::CalculateTotalShadowExtents()
{
	m_bFirstCoordinate = true;
  	CalculateShadowExtents(m_ModelSegmentKey,(float *)m_ObjectModellingMatrix);	
}


void HSmoothShadow::CalculateOneShadowExtent(HC_KEY shellkey, float *modellingMatrix)
{
	float groundmatrix[16];
 	int shellpoints_length, shellfaces_length;
	HPoint *shellpoints;

	RotateToGroundPlane(m_ShadowGroundPlane, groundmatrix);

	char type[MVO_BUFFER_SIZE];
	HC_Show_Key_Type(shellkey, type);
 	if (strcmp(type, "shell") != 0)
	{
		HC_KEY ownerkey;
		ownerkey = HC_KShow_Owner_By_Key(shellkey);
 		shellpoints = new HPoint[2]; 
		shellpoints_length = 2;

 		HC_Open_Segment("/cdummy");
		HC_Move_By_Key(shellkey, ".");
 		HC_Compute_Circumcuboid(".", &shellpoints[0], &shellpoints[1]);
		HC_Close_Segment();
		HC_Open_Segment_By_Key(ownerkey);
		HC_Move_By_Key(shellkey, ".");
		HC_Close_Segment();
	}
	else
	{
  		HC_Show_Shell_Size(shellkey, &shellpoints_length,&shellfaces_length);
 		shellpoints = new HPoint[shellpoints_length]; 
	
	HC_Show_Shell(shellkey,&shellpoints_length, shellpoints, &shellfaces_length,0); 	
	}
	float mat[16];
	float totalmatrix[16];
	float totalmatrix2[16];

	ShadowMatrix2(mat, m_ShadowGroundPlane,m_ShadowLightPos);

	HC_Compute_Matrix_Product(modellingMatrix, mat, totalmatrix);
	HC_Compute_Matrix_Product(totalmatrix, groundmatrix, totalmatrix2);
	HC_Compute_Transformed_Points(shellpoints_length,shellpoints,totalmatrix2,shellpoints);				

 	if (m_bFirstCoordinate)
	{
		m_bFirstCoordinate = false;
 		m_ShadowBoundingMin = m_ShadowBoundingMax = shellpoints[0];
	}
	
	ComputeObjectExtents(m_ShadowBoundingMin,m_ShadowBoundingMax,shellpoints,shellpoints_length);

	delete shellpoints;

}

void HSmoothShadow::CreateSoftShadow()
{
	float groundmatrix[16];


	if (m_bAllowOpenglShadow)
		 HC_Open_Segment (H_FORMAT_TEXT ("/driver/opengl/%s+%d", HDB::GetInstanceBase(), HDB::GetUniqueID()));
	else
		 HC_Open_Segment (H_FORMAT_TEXT ("/driver/image/%s+%d", HDB::GetInstanceBase(), HDB::GetUniqueID()));

		HC_Set_Handedness("left");
		HC_Set_Rendering_Options("attribute lock = handedness");
		HC_Set_Color_By_Value("windows", "RGB", 0, 0, 0);
		HC_Set_Color("faces = blue, edges = blue");
		HC_Set_Rendering_Options("attribute lock = color, no lighting");
 		HC_Set_Rendering_Options("hsra = priority");
		HC_Set_Color("faces = white,edges = white");
		HC_Set_Visibility("lights=off, faces = on ,edges = off, lines = off, markers = off");
 		HC_Set_Camera_Projection("stretched");
		HC_Set_Heuristics("no hidden surfaces");
 		RotateToGroundPlane(m_ShadowGroundPlane, groundmatrix);
		float mat[16];
		float totalmatrix[16];
 		ShadowMatrix2(mat, m_ShadowGroundPlane,m_ShadowLightPos);
		HC_Compute_Matrix_Product(mat, groundmatrix, totalmatrix);
		HC_Append_Modelling_Matrix(totalmatrix);		
 		HC_Include_Segment_By_Key(m_ModelSegmentKey);
  	HC_Close_Segment();

	CreateShadowTexture(); 	
	float matrix2[4][4];		
	HC_Compute_Matrix_Inverse((float *)groundmatrix, (float *)matrix2);
	HC_Append_Modelling_Matrix((float *)matrix2);
   	HC_Set_Heuristics("polygon handedness = right");
	HC_Set_Visibility("lights=off, faces = on ,edges = off, lines = off, markers = off");
}
 
 



void HSmoothShadow::CreateShadowTexture()
{
	HPoint middle;
	middle.x = (m_ShadowBoundingMin.x + m_ShadowBoundingMax.x) / 2.0f;
	middle.y = (m_ShadowBoundingMin.y + m_ShadowBoundingMax.y) / 2.0f;
	middle.z = (m_ShadowBoundingMin.z + m_ShadowBoundingMax.z) / 2.0f;
	float sizex = m_ShadowBoundingMax.x - m_ShadowBoundingMin.x;
	float sizey = m_ShadowBoundingMax.y - m_ShadowBoundingMin.y;
	float factor = (float)sizex / (float)sizey;
	int width,height;
 	if (sizex>sizey)
	{
				
		width = int(m_TextureSize * factor);
		height = m_TextureSize;
	}
	else
	{
		width = m_TextureSize;
		height = int(m_TextureSize/factor);
	}
	int mult_w  = width / 64;
	int mult_h  = height / 64;
	if (mult_w > 4)
		mult_w = 4;
	if (mult_h > 4)
		mult_h = 4;

	width			= 64 *  (mult_w) - (m_FilterSize - 1);
	height			= 64 * (mult_h) - (m_FilterSize - 1);	
  	int extwidth	= width + (m_FilterSize - 1);
	int extheight	= height + (m_FilterSize - 1);
	
 	char text[MVO_BUFFER_SIZE];

	HPixelRGB	*image;
	HPixelRGBA	*smoothImage;
	
	HC_KEY image_key;
	
	H_FORMAT_TEXT tmpseg("/driver/null/%d", HDB::GetUniqueID());
    HC_Open_Segment (tmpseg);
	image = new HPixelRGB[width * height];
	if (m_bAllowOpenglShadow)
		image_key = HC_KInsert_Image (0.0, 0.0, 0.0, "rgb, name = dummy", width, height, 0);
	else
		image_key = HC_KInsert_Image_By_Ref (0.0, 0.0, 0.0, "rgb, name = dummy", width, height, image);
	HC_Close_Segment ();
	
	H_FORMAT_TEXT dvrseg;
	if (m_bAllowOpenglShadow)
	{
		dvrseg = H_FORMAT_TEXT ("/driver/opengl/%s+%d", HDB::GetInstanceBase(), HDB::GetUniqueID());
		HC_Open_Segment (dvrseg);
		HC_Set_Driver_Options (H_FORMAT_TEXT("no border, use window id = (image key = %s%p), isolated, subscreen = (-1, 1, -1, 1)", H_EXTRA_POINTER_FORMAT, image_key));
	}
	else
	{
		dvrseg = H_FORMAT_TEXT ("/driver/image/%s+%d", HDB::GetInstanceBase(), HDB::GetUniqueID());
		HC_Open_Segment (dvrseg);
		HC_Set_Driver_Options (H_FORMAT_TEXT("use window id = (image key = %s%p), isolated, subscreen = (-1, 1, -1, 1)", H_EXTRA_POINTER_FORMAT, image_key));
	}

	HC_Set_Window_Frame("off");
	HC_Set_Camera_Target(middle.x, middle.y,m_ShadowBoundingMin.z); 	 
	HC_Set_Camera_Position(middle.x, middle.y, m_ShadowBoundingMin.z -0.0001);
	HC_Set_Camera_Up_Vector(0,1,0);
	HC_Set_Camera_Field(sizex, sizey);	 
	
	HC_Close_Segment();
 
	HC_Update_One_Display(dvrseg);
	if (m_bAllowOpenglShadow)
	{	
		float dummy;
		int dummy2;
		char format[100];
		HC_Show_Image(image_key, &dummy, &dummy, &dummy, format, &dummy2, &dummy2, image);
	}
	HC_Delete_Segment (dvrseg);	

 	HC_Flush_Contents(".","everything");		
	HC_Delete_Segment (tmpseg);

	
	SmoothImage(image, &smoothImage, width, height,m_FilterSize/2, m_filter);
	
	delete [] image; 	
	
	HC_Open_Segment("/shadowimages");
 		HC_Set_Visibility("everything = off");
 		sprintf(text, "rgba, name = image-%s",m_texname);
		CleanUp();
 		m_ImageKey = HC_KInsert_Image (0.0, 0.0, 0.0, text, extwidth, extheight, smoothImage);
 	HC_Close_Segment();

	delete [] smoothImage;

	float mx, my;
	mx = sizex/2.0f;
	my = sizey/2.0f;
	
	float mesh_points[] = {	-mx, -my, 0,
		-mx, my, 0,
		mx, -my, 0,
		mx, my, 0};

	sprintf(text, "source = image-%s, interpolation filter = on",m_texname);

	HC_Define_Texture(m_texname,text);

	HC_KEY key=HC_KInsert_Mesh(2,2,mesh_points);
	HC_Set_Callback("draw dc reshaded triangle = HImUtility_draw_segment_in_background");
 
	HC_MSet_Vertex_Parameters(key,0,4,3,vcor);
	sprintf(text,"faces=(diffuse=white %s)",m_texname);
 	HC_Set_Color(text);			
	HC_Set_Rendering_Options("lighting interpolation=gouraud");

	if (factor > 0)
	{
		if (factor < 0.2f)			//limits the shadow scaling
			factor = 0.2f;			//of the shorter direction
		else if (factor > 5.0f)
			factor = 5.0f;

		if (factor<1.0)
			HC_Scale_Object(1.0+0.15/factor,1.0+0.15,1.0);
		else
			HC_Scale_Object(1.0+0.15,1.0+0.15*factor,1.0);
	}


	HC_Translate_Object(middle.x, middle.y, m_ShadowBoundingMin.z-(m_ShadowBoundingMax.z-m_ShadowBoundingMin.z)/1000.0); 
	texnum++;
}

 


void HSmoothShadow::SmoothImage(HPixelRGB *origImage, HPixelRGBA **smoothImage, int width, int height, int filterSize, int *filter)
{
	HPixelRGB *image = origImage;
 	int extwidth = width + filterSize*2;
	int extheight = height + filterSize*2;

	HPixelRGBA *image2 = new HPixelRGBA[extwidth * extheight];
 

	int x,y,x2,y2;
 	for (x=0;x<extwidth;x++)
	{
		for (y=0;y<extheight;y++)
		{
			float r = 0;
			int divi=0;	
			int fx, fy;
			for (x2=-filterSize, fx = 0;x2<=filterSize;x2++, fx++)
			{
				for (y2=-filterSize, fy = 0;y2<=filterSize;y2++, fy++)
				{								
					int xc = x + x2;
					int yc = y + y2;	
					float p; 
				
					int ffactor = filter[fx + (filterSize*2+1)*fy];
					if (!(xc>=filterSize && yc>=filterSize && xc<(extwidth-filterSize) && yc<(extheight-filterSize)))
						p = 0;
					else
					{
						if (image[(xc-filterSize)+(yc-filterSize)*width].r() || 
							image[(xc-filterSize)+(yc-filterSize)*width].g() ||
							image[(xc-filterSize)+(yc-filterSize)*width].b())
							p=255.0f*(float)ffactor;
						else
							p=0.0f;
					}
					divi+=ffactor;
					r+=p;										
				}
			}	
 			int pos = x + y * extwidth;			
			r /=divi;
  			if (r>=255)
				r = 255;

 			image2[pos] = HPixelRGBA(
				(unsigned char)m_ShadowBrightness,
				(unsigned char)m_ShadowBrightness,
				(unsigned char)m_ShadowBrightness,
				(unsigned char)r);
		}
	} 
	*smoothImage = image2;
}

/////////////////////////////////////////////////////////////////////////////
// HHardShadow


void HHardShadow::Create(ShadowPlaneType ptype) 
{  	
	// if there are no shells in the model segment, we don't cast any shadows
	if( !HasShell( m_ModelSegmentKey) )
		return;

   	HC_Open_Segment_By_Key(m_ShadowSegmentKey);
 
 		CalculateTotalModelExtents();
 		switch (ptype)
		{
			case XZ_PLANE:
				m_ShadowGroundPlane.Set(0.0f,1.0f,0.0f, -m_ObjectBoundingMin.y);
				m_ShadowLightPos.Set((m_ObjectBoundingMin.x + m_ObjectBoundingMax.x)/2.0f,m_ObjectBoundingMax.y+10000.0f,(m_ObjectBoundingMin.z+m_ObjectBoundingMax.z)/2.0f);
 				break;
			case XY_PLANE:
				m_ShadowGroundPlane.Set(0.0f,0.0f,1.0f, -m_ObjectBoundingMin.z);
				m_ShadowLightPos.Set((m_ObjectBoundingMin.x+m_ObjectBoundingMax.x)/2.0f,(m_ObjectBoundingMin.y+m_ObjectBoundingMax.y)/2.0f,m_ObjectBoundingMax.z+10000.0f);
  				break;
			case YZ_PLANE:
				m_ShadowGroundPlane.Set(1.0f,0.0,0.0, -m_ObjectBoundingMin.x);
				m_ShadowLightPos.Set(m_ObjectBoundingMax.x+10000.0f, (m_ObjectBoundingMin.y+m_ObjectBoundingMax.y)/2.0f,(m_ObjectBoundingMin.z+m_ObjectBoundingMax.z)/2.0f);
 				break;
				
		}
 
		CreateHardShadow();

		// set the shadow segment attributes
		HC_Set_Selectability("everything = off");

  	HC_Close_Segment();
  	
  	
}


void HHardShadow::Create() 
{  	
	// if there are no shells in the model segment, we don't cast any shadows
	if( !HasShell( m_ModelSegmentKey) )
		return;

   	HC_Open_Segment_By_Key(m_ShadowSegmentKey);
 		CreateHardShadow();
  	HC_Close_Segment();
  	
  	
}


void HHardShadow::CreateHardShadow()
{
 		float mat[16];
		HC_Flush_Contents(".","geometry");

//		HC_Set_Color("faces = light grey, edges = light grey, lines = light grey, markers = light grey");
		HC_Set_Rendering_Options("attribute lock = (color), no lighting");
  		HC_Set_Visibility("lights=off, faces = on ,edges = off, lines = off, markers = off");
		HC_Set_Heuristics("no backplane culling");
 
 		ShadowMatrix2(mat, m_ShadowGroundPlane,m_ShadowLightPos);
		HC_Append_Modelling_Matrix((float *)mat);
		HC_Include_Segment_By_Key(m_ModelSegmentKey);		
}





 
 
