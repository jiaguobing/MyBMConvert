// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityGeomHandle implementation of the HUtilityGeomHandle class
//
#define TVALUEX 0.001f
#define TVALUEY -0.0779593f
#define TVALUEZ 0.324654f

#include <math.h>
#include "string.h"
#include "stdio.h"

#include "hc.h"
#include "hic.h"
#include "HImManager.h"
#include "HEventManager.h"
#include "HBaseView.h"
#include "HBaseModel.h"

#include "HUtility.h" 
#include "HUtilityGeomHandle.h"
 

#define EPS 1.0e-6

template <typename T>	inline	T const &	Max (T const & a, T const & b)	{ return  a > b ? a : b; }
template <typename T>	inline	T const &	Max (T const & a, T const & b, T const & c)	{ return  Max(Max(a,b),c); }
template <typename T>	inline	T const &	Max (T const & a, T const & b, T const & c, T const & d) { return Max(Max(a, b, c),d); }

/*
   Calculate the line segment PaPb that is the shortest route between
   two lines P1P2 and P3P4. Calculate also the values of mua and mub where
      Pa = P1 + mua (P2 - P1)
      Pb = P3 + mub (P4 - P3)
   Return FALSE if no solution exists.
*/
int LineLineIntersect(
   HPoint const &p1,HPoint const &p2,HPoint const &p3,HPoint const &p4,HPoint *pa,HPoint *pb,
   double *mua, double *mub)
{
   HPoint p13,p43,p21;
   double d1343,d4321,d1321,d4343,d2121;
   double numer,denom;

   float peps = Max(fabs(p1.x), fabs(p1.y), fabs(p1.z));
   peps = Max(peps, (float)fabs(p2.x), (float)fabs(p2.y), (float)fabs(p2.z));
   peps = Max(peps, (float)fabs(p3.x), (float)fabs(p3.y), (float)fabs(p3.z));
   peps = Max(peps, (float)fabs(p4.x), (float)fabs(p4.y), (float)fabs(p4.z));
   double eps = peps * EPS;

   p13.x = p1.x - p3.x;
   p13.y = p1.y - p3.y;
   p13.z = p1.z - p3.z;
   p43.x = p4.x - p3.x;
   p43.y = p4.y - p3.y;
   p43.z = p4.z - p3.z;
   if (fabs(p43.x)  < eps && fabs(p43.y)  < eps && fabs(p43.z)  < eps)
      return false;
   p21.x = p2.x - p1.x;
   p21.y = p2.y - p1.y;
   p21.z = p2.z - p1.z;
   if (fabs(p21.x)  < eps && fabs(p21.y)  < eps && fabs(p21.z)  < eps)
      return false;

   d1343 = p13.x * p43.x + p13.y * p43.y + p13.z * p43.z;
   d4321 = p43.x * p21.x + p43.y * p21.y + p43.z * p21.z;
   d1321 = p13.x * p21.x + p13.y * p21.y + p13.z * p21.z;
   d4343 = p43.x * p43.x + p43.y * p43.y + p43.z * p43.z;
   d2121 = p21.x * p21.x + p21.y * p21.y + p21.z * p21.z;

   denom = d2121 * d4343 - d4321 * d4321;
   if (fabs(denom) < eps * eps)
      return false;
   numer = d1343 * d4321 - d1321 * d4343;

   *mua = numer / denom;
   *mub = (d1343 + d4321 * (*mua)) / d4343;

   pa->x = (float)(p1.x + *mua * p21.x);
   pa->y = (float)(p1.y + *mua * p21.y);
   pa->z = (float)(p1.z + *mua * p21.z);
   pb->x = (float)(p3.x + *mub * p43.x);
   pb->y = (float)(p3.y + *mub * p43.y);
   pb->z = (float)(p3.z + *mub * p43.z);

   return true;
}


void HUtilityGeomHandle::RegisterCallbacks()
{
	HImRegisterCallback("HUtilityGeomHandle_draw_triangle", drawTriangleCallback);
   
}



void HUtilityGeomHandle::drawTriangleCallback (HIC_Rendition    *rendition, HIC_DC_Point * p)
{

#if 0
	float min;
	if (p[0].z <= p[1].z && p[0].z <= p[2].z)
		min = p[0].z;
	else 	if (p[1].z <= p[0].z && p[1].z <= p[2].z)
		min = p[1].z;
	else 
		min = p[2].z;

	p[0].z = p[0].z;
	p[1].z = p[1].z;
	p[2].z = p[2].z;
#endif
 	HIC_Draw_DC_Triangle(rendition,p);
	
}

 

void HUtilityGeomHandle::SetupGeomHandleSegment(bool edges)
{
	HC_Set_User_Options("GeomHandle = true");
//  	HC_Set_Rendering_Options("depth range=(0,0.1)");
	HC_Set_Rendering_Options("lod = off, no frame buffer effects");
	HC_Set_Heuristics("no backplane culling");
	HC_Set_Selectability("everything = off, polygons = on!, markers = on!");
	if (edges)
  		HC_Set_Visibility("markers = off, cutting planes = off, edges = on, faces = off");
	else
  		HC_Set_Visibility("markers = off, cutting planes = off, edges = off, faces = on");
	HC_Set_Marker_Symbol("(.)");
	HC_Set_Color("markers = yellow");
	HC_Set_Marker_Size(0.5f);

}



 
void HUtilityGeomHandle::CalculateTransformedPosition(HC_KEY handlekey, HPoint &t, HBaseView *view)
{
	float matrix[16];
	HC_KEY keys[MVO_BUFFER_SIZE];
	int kcount = HUtility::CalculateKeyPath(handlekey, view, keys, MVO_BUFFER_SIZE);

	HC_Compute_Transform_By_Path(kcount, keys, "object","world", matrix);

 	HC_Open_Segment_By_Key(handlekey);
 	HC_Compute_Transformed_Points(1,&t, matrix, &t);

	HC_Close_Segment();
}


void HUtilityGeomHandle::ShowBasePosition(HC_KEY handlekey, HPoint & bp)
{
	HC_KEY markerkey;
	HC_Open_Segment_By_Key(handlekey);{
		HC_Begin_Contents_Search(".", "markers");{
			HC_Find_Contents(0, &markerkey);
		}HC_End_Contents_Search();
		HC_Show_Marker(markerkey, &bp.x, &bp.y, &bp.z);
	}HC_Close_Segment();
}



HC_KEY HUtilityGeomHandle::IsGeomHandle(HC_KEY key)
{
	char type[MVO_BUFFER_SIZE];
	char result[MVO_BUFFER_SIZE];
 	HC_Show_Key_Type(key,type);
	if (strcmp(type,"segment") == 0)
		return HUtility::UserOptionExists(key, "geomhandle", result);
	else if (strcmp(type,"marker") == 0 || strcmp(type, "shell") == 0)
	{
		HC_KEY owner = HC_KShow_Owner_By_Key(key);
		return HUtility::UserOptionExists(owner, "geomhandle", result);
	}
	return INVALID_KEY;
}


bool HUtilityGeomHandle::ContainsGeomHandles(HC_KEY key)
{
	char type[MVO_BUFFER_SIZE];
	bool found = false;
	HC_KEY segkey;
	HC_Open_Segment_By_Key(key);
	HC_Begin_Contents_Search("...", "segments");
	while (HC_Find_Contents(type, &segkey))
	{
	    if (IsGeomHandle(segkey) != INVALID_KEY)
	    {
		found = true;
		break;
	    }
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	return found;
}

 

HC_KEY HUtilityGeomHandle::Insert(HPoint position, bool edges, int level)
{
   	float matrix[16];		
 	
	HC_KEY handlekey = HC_KOpen_Segment("");
   	SetupGeomHandleSegment(edges);
 	HC_Insert_Marker(position.x , position.y , position.z);
	HC_Set_User_Options ("H_HANDLE_C_TYPE = 0");
	char text[MVO_BUFFER_SIZE];
	HC_Set_User_Options ("H_HANDLE_C_PARAM = 0");
	sprintf(text, "H_HANDLE_C_MMLEVEL = %d", level);
	HC_Set_User_Options (text);
	HC_Close_Segment();
	HC_KEY ownerkey = HC_KShow_Owner_By_Key(handlekey);
	HC_Open_Segment_By_Key(ownerkey);
 	if (!HC_Show_Existence("modelling matrix"))
	{
		HC_Compute_Identity_Matrix(matrix);
		HC_Set_Modelling_Matrix(matrix);
	}
 
 	HC_Close_Segment();
	HC_Open_Segment_By_Key(GetMMSegment(handlekey));
	HC_Set_User_Options("hashandles");
	HC_Close_Segment();
 
 	return handlekey;
}


void HUtilityGeomHandle::GetConstraintParameters(HC_KEY handlekey)
{
	char constrains[MVO_BUFFER_SIZE];
	char constrainparam[MVO_BUFFER_SIZE];
 
 	HC_Open_Segment_By_Key(handlekey);
	HC_Show_One_User_Option("H_HANDLE_C_TYPE", constrainparam);		
	HC_Show_One_User_Option("H_HANDLE_C_PARAM", constrains);		
 	int temp;
	sscanf(constrainparam,"%d", &temp);
	m_ConstraintType = (HConstrainType)temp;

	if (m_ConstraintType == HConstrainToRay)
	{
		HCLOCALE (sscanf(constrains,"%f %f %f %f %f %f", &m_rpoint[0].x, &m_rpoint[0].y, &m_rpoint[0].z,
			&m_rpoint[1].x, &m_rpoint[1].y, &m_rpoint[1].z));
	}
	if (m_ConstraintType == HConstrainToScale)
	{
		HCLOCALE (sscanf(constrains,"%f %f %f %f %f %f", &m_rpoint[0].x, &m_rpoint[0].y, &m_rpoint[0].z,
			&m_rpoint[1].x, &m_rpoint[1].y, &m_rpoint[1].z));
	}
	if (m_ConstraintType == HConstrainToRotationVector)
	{
		HCLOCALE (sscanf(constrains,"%f %f %f %f %f %f", &m_rpoint[0].x, &m_rpoint[0].y, &m_rpoint[0].z,
			&m_rpoint[1].x, &m_rpoint[1].y, &m_rpoint[1].z));
	}
	else if (m_ConstraintType == HConstrainToPlane)
	{
		HCLOCALE (sscanf(constrains,"%f %f %f %f %f %f %f %f %f", &m_rpoint[0].x, &m_rpoint[0].y, &m_rpoint[0].z,
			&m_rpoint[1].x,&m_rpoint[1].y,&m_rpoint[1].z,&m_rpoint[2].x,&m_rpoint[2].y,&m_rpoint[2].z));
	}
	HC_Close_Segment();
 
}


void HUtilityGeomHandle::Rotate(HC_KEY handlekey, float rotangle)
{
	GetConstraintParameters(handlekey);

    HPoint tvalue;
	FindTranslation(handlekey, tvalue);
	float offaxisrotmat[16], segmat[16], resmat[16];	
	float transmat[16],offaxisrottransmat[16];
 	HC_KEY ownerkey = GetMMSegment(handlekey);
 	HPoint rpoint[2];
	rpoint[0].Set(m_rpoint[0].x + tvalue.x, m_rpoint[0].y + tvalue.y,m_rpoint[0].z + tvalue.z);
	rpoint[1].Set(m_rpoint[1].x + tvalue.x, m_rpoint[1].y + tvalue.y,m_rpoint[1].z + tvalue.z);
 
	HC_Open_Segment_By_Key(ownerkey);
 	HPoint rotaxis;
	rotaxis.Set(rpoint[1].x - rpoint[0].x, rpoint[1].y - rpoint[0].y, rpoint[1].z - rpoint[0].z);
 	HC_Compute_Translation_Matrix(-rpoint[0].x,-rpoint[0].y,-rpoint[0].z, transmat);
	HC_Compute_Offaxis_Rotation(rotaxis.x, rotaxis.y, rotaxis.z, rotangle, offaxisrotmat); 
	HC_Compute_Matrix_Product(transmat, offaxisrotmat, offaxisrottransmat);
	HC_Compute_Translation_Matrix(rpoint[0].x,rpoint[0].y,rpoint[0].z, transmat);
	HC_Compute_Matrix_Product(offaxisrottransmat, transmat, offaxisrotmat);
	HUtility::ShowModellingMatrix(segmat);
	HC_Compute_Matrix_Product(offaxisrotmat, segmat, resmat);
	HC_Set_Modelling_Matrix(resmat);
 	HC_Close_Segment();
}
  	
void HUtilityGeomHandle::Scale(HC_KEY handlekey, float Scalex, float Scaley, float Scalez)
{
	HPoint tvalue;
	FindTranslation(handlekey, tvalue);

	float scalemat[16], segmat[16], resmat[16];
	float transmat[16],matres[16];
	GetConstraintParameters(handlekey);

	HC_KEY ownerkey = GetMMSegment(handlekey);
 
	HPoint rpoint[2];
	rpoint[0].Set(m_rpoint[0].x + tvalue.x, m_rpoint[0].y + tvalue.y,m_rpoint[0].z + tvalue.z);
	rpoint[1].Set(m_rpoint[1].x + tvalue.x, m_rpoint[1].y + tvalue.y,m_rpoint[1].z + tvalue.z);

	HC_Open_Segment_By_Key(ownerkey);
	
	HC_Compute_Translation_Matrix(-rpoint[0].x,-rpoint[0].y,-rpoint[0].z, transmat);
	if (m_rpoint[1].x<1)
	    Scalex = 1.0f;
	if (m_rpoint[1].y<1)
	    Scaley = 1.0f;
	if (m_rpoint[1].z<1)
	    Scalez = 1.0f;

 	HC_Compute_Scale_Matrix(Scalex,Scaley, Scalez, scalemat);
	
	HC_Compute_Matrix_Product(transmat, scalemat, matres);
	HC_Compute_Translation_Matrix(rpoint[0].x,rpoint[0].y,rpoint[0].z, transmat);
	HC_Compute_Matrix_Product(matres, transmat, scalemat);
	HUtility::ShowModellingMatrix(segmat);
	HC_Compute_Matrix_Product(scalemat, segmat, resmat);
	if (fabs(resmat[0])>0.001f)
		HC_Set_Modelling_Matrix(resmat);
 
  
	HC_Close_Segment();
}
	
void HUtilityGeomHandle::TurnOff(HC_KEY handlekey)
{
 	GetConstraintParameters(handlekey);

	HC_KEY ownerkey = HC_KShow_Owner_By_Key(handlekey);
 	HC_Open_Segment_By_Key(ownerkey);
	HC_Begin_Contents_Search(".", "segments");
	HC_KEY skey;
	char type[MVO_BUFFER_SIZE];
	while (HC_Find_Contents(type, &skey))
	{
			if (IsGeomHandle(skey ) && skey != handlekey)
				HC_Delete_By_Key(skey);
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	HC_Delete_By_Key(handlekey);
}

 
/*
static void DebugMarker(HBaseView *view, HPoint p)
{

	HC_Open_Segment_By_Key(view->GetSceneKey());
			HC_Open_Segment("temp");
			HC_Set_Visibility("markers = on");
			HC_Insert_Marker(p.x, p.y, p.z);
			HC_Close_Segment();
			HC_Close_Segment();
}
*/


void HUtilityGeomHandle::Move (
	HC_KEY handleKey,
	HPoint worldSpaceCenter,
	HC_KEY cameraSegKey,
	HBaseView * view)
{
	GetConstraintParameters(handleKey);

	HPoint translation;
	FindTranslation(handleKey, translation);

	HC_KEY keys[MVO_BUFFER_SIZE];
	int kcount = HUtility::CalculateKeyPath(handleKey, view, keys, MVO_BUFFER_SIZE);

	float worldToObject[16];
	float objectToWorld[16];
	HC_Compute_Transform_By_Path(kcount, keys, "world", "object", worldToObject);
	HC_Compute_Transform_By_Path(kcount, keys, "object", "world", objectToWorld);

	switch (m_ConstraintType)
	{
		case HConstrainToRay: {
			HPoint const & rayPt1 = m_rpoint[0];
			HPoint const & rayPt2 = m_rpoint[1];

			HPoint objectSpaceCenter;
		
			HC_Compute_Transformed_Points(1, &worldSpaceCenter, worldToObject, &objectSpaceCenter);
			AdjustPositionToRay(handleKey, objectSpaceCenter, rayPt1, rayPt2);

			//HPoint basePos;
			//ShowBasePosition(handleKey, basePos);
			//objectSpaceCenter -= basePos;

			HC_Compute_Transformed_Points(1, &objectSpaceCenter, objectToWorld, &worldSpaceCenter);
		
			break;
		}
		case HConstrainToPlane: {
			float newplane[4];
			float plane[4];

			HC_Compute_Polygon_Plane(3, m_rpoint, "right", plane);

			HC_Compute_Transformed_Plane(plane, objectToWorld, newplane);

			HPoint objectSpaceCenter;

			AdjustPositionToPlane(handleKey, worldSpaceCenter, newplane[0], newplane[1], newplane[2], newplane[3], cameraSegKey);			
			HC_Compute_Transformed_Points(1, &worldSpaceCenter, worldToObject, &objectSpaceCenter);

			HC_Open_Segment_By_Key(handleKey);{
				//HPoint basePos;
				//ShowBasePosition(handleKey, basePos);
				//objectSpaceCenter -= basePos;

				HC_Compute_Transformed_Points(1, &objectSpaceCenter, objectToWorld, &worldSpaceCenter);
			}HC_Close_Segment();

			break;
		}
		case HConstrainToViewplane: {
			HPoint basePos;
			ShowBasePosition(handleKey, basePos);

			HC_Open_Segment_By_Key(handleKey);{
				HC_Compute_Transformed_Points(1, &basePos, objectToWorld, &basePos);
			}HC_Close_Segment();

			HPoint windowSpaceCenter;

			HC_Open_Segment_By_Key(cameraSegKey);{
				HC_Compute_Coordinates(".", "world", &basePos, "local window", &basePos);
				HC_Compute_Coordinates(".", "world", &worldSpaceCenter, "local window", &windowSpaceCenter);
				windowSpaceCenter.z = basePos.z;
				HC_Compute_Coordinates(".", "local window", &windowSpaceCenter, "world", &worldSpaceCenter);
			}HC_Close_Segment();

			HPoint objectSpaceCenter;

			HC_Open_Segment_By_Key(handleKey);{
				HC_Compute_Transformed_Points(1, &worldSpaceCenter, worldToObject, &objectSpaceCenter);
			}HC_Close_Segment();

			HC_Open_Segment_By_Key(handleKey);{
				//ShowBasePosition(handleKey, basePos);
				//objectSpaceCenter -= basePos;
				HC_Compute_Transformed_Points(1, &objectSpaceCenter, objectToWorld, &worldSpaceCenter);
			}HC_Close_Segment();

			break;
		}
		default: {
			break;
		}
	}

	HC_Open_Segment_By_Key(GetMMSegment(handleKey));{

#ifndef _NDEBUG
		char pathname[MVO_BUFFER_SIZE];
		HC_Show_Pathname_Expansion(".", pathname);
#endif

		float matrix[16];
		float invMatrix[16];

		HC_PShow_Net_Modelling_Matrix(0, 0, matrix);
		HC_Compute_Matrix_Inverse(matrix, invMatrix);

		HPoint pos;

		HC_Compute_Transformed_Points(1, &worldSpaceCenter, invMatrix, &pos);

		HUtility::ShowModellingMatrix(matrix);

		float translationMatrix[16];
		HC_Compute_Translation_Matrix(
			pos.x - translation.x,
			pos.y - translation.y,
			pos.z - translation.z,
			translationMatrix);

		float newMatrix[16];
		HC_Compute_Matrix_Product(translationMatrix, matrix, newMatrix);
		HC_Set_Modelling_Matrix(newMatrix);

	}HC_Close_Segment();
}


void HUtilityGeomHandle::ConstrainToPlane(HC_KEY handlekey, HPoint p1, HPoint p2, HPoint p3,HBaseView *view)
{
	UNREFERENCED(view);

	m_ConstraintType = HConstrainToPlane;
	m_rpoint[0] = p1;
	m_rpoint[1] = p2;
	m_rpoint[2] = p3;
	
	char constrains[MVO_BUFFER_SIZE];
	char constrainparam[MVO_BUFFER_SIZE];
	HCLOCALE (sprintf(constrainparam,"H_HANDLE_C_PARAM = %f %f %f %f %f %f %f %f %f", p1.x, p1.y, p1.z, 
		p2.x, p2.y, p2.z, p3.x, p3.y, p3.z));

	sprintf(constrains, "H_HANDLE_C_TYPE = %d",(int)m_ConstraintType);
 	HC_Open_Segment_By_Key(handlekey);
 			HC_Set_User_Options (constrainparam);
			HC_Set_User_Options (constrains);
	HC_Close_Segment();
	ShowHandle(handlekey, 0.5);

}

void HUtilityGeomHandle::ConstrainToViewplane(HC_KEY handlekey,HBaseView *view)
{
	UNREFERENCED(view);

	m_ConstraintType = HConstrainToViewplane;	
	char constrains[MVO_BUFFER_SIZE];
  
	sprintf(constrains, "H_HANDLE_C_TYPE = %d",(int)m_ConstraintType);
 	HC_Open_Segment_By_Key(handlekey);
		 	HC_Set_Marker_Symbol("(*)");
 			HC_Set_User_Options (constrains);
			HC_Set_Color("markers = white");
	HC_Close_Segment();
	ShowHandle(handlekey,0.5);

}


void HUtilityGeomHandle::ConstrainToRotationVector(HC_KEY handlekey, HPoint p1, HPoint p2,HBaseView *view)
{
	UNREFERENCED(view);

	m_ConstraintType = HConstrainToRotationVector;
	m_rpoint[0] = p1;
	m_rpoint[1] = p2;
	 
	char constrains[MVO_BUFFER_SIZE];
	char constrainparam[MVO_BUFFER_SIZE];
	HCLOCALE (sprintf(constrainparam, "H_HANDLE_C_PARAM = %f %f %f %f %f %f", 
		p1.x, p1.y, p1.z, p2.x, p2.y, p2.z));
	sprintf(constrains, "H_HANDLE_C_TYPE = %d",(int)m_ConstraintType);

 	HC_Open_Segment_By_Key(handlekey);
 	HC_Set_Marker_Symbol("(.)");
 			HC_Set_User_Options (constrainparam);
			HC_Set_User_Options (constrains);	 			
	HC_Close_Segment();
	ShowHandle(handlekey, 0.5);

}
void HUtilityGeomHandle::ConstrainToScale(HC_KEY handlekey, HPoint p1, HPoint p2,HBaseView *view)
{
	UNREFERENCED(view);

	m_ConstraintType = HConstrainToScale;
	m_rpoint[0] = p1;
	m_rpoint[1] = p2;
	 
	char constrains[MVO_BUFFER_SIZE];
	char constrainparam[MVO_BUFFER_SIZE];
	HCLOCALE (sprintf(constrainparam, "H_HANDLE_C_PARAM = %f %f %f %f %f %f", 
		p1.x, p1.y, p1.z, p2.x, p2.y, p2.z));
	sprintf(constrains, "H_HANDLE_C_TYPE = %d",(int)m_ConstraintType);

 	HC_Open_Segment_By_Key(handlekey);
 	HC_Set_Marker_Symbol("(.)");
 			HC_Set_User_Options (constrainparam);
			HC_Set_User_Options (constrains);			
	HC_Close_Segment();
	ShowHandle(handlekey, 0.5);

}

void HUtilityGeomHandle::ConstrainToOnOff(HC_KEY handlekey,HBaseView *view)
{
	UNREFERENCED(view);

	m_ConstraintType = HConstrainToOnOff;
  	 
	char constrains[MVO_BUFFER_SIZE];
  	sprintf(constrains, "H_HANDLE_C_TYPE = %d",(int)m_ConstraintType);

 	HC_Open_Segment_By_Key(handlekey);
 	HC_Set_Marker_Symbol("(.)");
	HC_Set_Color("markers = red");
	HC_Set_User_Options (constrains);
	 
			
	HC_Close_Segment();
}

 




void HUtilityGeomHandle::ShowHandle(HC_KEY handlekey,  float sf,HBaseView *view)
{
	GetConstraintParameters(handlekey);

 	HC_Open_Segment_By_Key(handlekey);
	HC_Set_Marker_Size(0.5f);
	
	switch (m_ConstraintType)
	{
	case HConstrainToRay:
		{
			HPoint p1,p2;
			p1 = m_rpoint[0];
			p2 = m_rpoint[1];

			HC_Open_Segment("hobjects");
			
			
			if (view && view->GetSpritingMode())
				HC_Set_Heuristics("quick moves = spriting");
 			HC_Set_Visibility("markers = off");
			HPoint bp, bp2;
			ShowBasePosition(handlekey, bp);
			bp2=bp;
 
			HC_KEY key = HUtility::CreateScaleIndependentSegment("xxx", false);
			HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
		 	HUtility::SetScaleIndependentGeomPosition(key, bp2.x,bp2.y,bp2.z);

			if (!HC_Show_Existence("segment"))
			{			
  				HC_Open_Segment("tempover");
  				HC_Open_Segment("xxx");
 
 			HC_Set_Color("edges = white");
			HUtility::RotateToVector(p1,p2, false);
  			HUtility::InsertArrow(0.075f*sf, 0.05f*sf, 0.014f*sf, 0.03f*sf,0,"faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black), edges = white", "faces = (diffuse = r=0.5 g=0.5 b=0.5, transmission = black) , edges = white",6,0,true);
 
			HC_Open_Segment("arrow2");
 			HUtility::InsertArrow(0.075f*sf, 0.05f*sf, 0.014f*sf, 0.03f*sf,0,"faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black), edges = white", "faces = (diffuse = r=0.5 g=0.5 b=0.5, transmission = black) , edges = white",6,0,true);
 
 			HC_Rotate_Object(180,0,0);
		
//			HC_Define_System_Options( "no bounding volumes" );
  			HC_Close_Segment();
  			HC_Close_Segment();
  			HC_Close_Segment();
			}
 			else
			{
  				HC_Open_Segment("tempover");
				if (HC_Show_Existence("modelling matrix"))
					HC_UnSet_Modelling_Matrix();
				float s = (sf - 0.5f) * 2.0f;
				HC_Scale_Object(1.0 + s ,1.0 + s,1.0 + s);
				HC_Close_Segment();

			}
			HC_Close_Segment();
			HC_Close_Segment();
		}
 
		break;

	case HConstrainToScale:
		{
			HPoint p1,p2;
 
			HC_Open_Segment("hobjects");
			HC_Set_Color("faces = white");
			HC_Flush_Contents(".","everything");
			HC_Set_Visibility("markers = off");
			HPoint bp, bp2;
			ShowBasePosition(handlekey, bp);
			bp2=bp;
 
			HC_KEY key = HUtility::CreateScaleIndependentSegment("xxx", false);
			HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
		 	HUtility::SetScaleIndependentGeomPosition(key, bp2.x,bp2.y,bp2.z);

			HC_Open_Segment("");
 			HC_Set_Color("edges = white");
 			p1.Set(-0.01f*sf, -0.01f*sf, -0.01f*sf);
			p2.Set(0.01f*sf, 0.01f*sf, 0.01f*sf);
			HC_Set_Color("faces = (transmission = grey)");
			HUtility::InsertBox(&p1,&p2);
			p1.Set(-0.02f*sf, -0.02f*sf, -0.02f*sf);
			p2.Set(0.02f*sf, 0.02f*sf, 0.02f*sf);
			HUtility::InsertBox(&p1,&p2);
			p1.Set(-0.03f*sf, -0.03f*sf, -0.03f*sf);
			p2.Set(0.03f*sf, 0.03f*sf, 0.03f*sf);
			HUtility::InsertBox(&p1,&p2);

   			HC_Close_Segment(); 
			HC_Close_Segment();
			HC_Close_Segment();
		}
 
		break;

	case HConstrainToOnOff:
		{
			HC_Open_Segment("hobjects");
			HC_Set_Color("faces = white");
			HC_Flush_Contents(".","everything");
			HC_Set_Visibility("markers = off");
			HPoint bp, bp2;
			ShowBasePosition(handlekey, bp);
			bp2=bp;
  

			HC_KEY key = HUtility::CreateScaleIndependentSegment("xxx");
			HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
		 	HUtility::SetScaleIndependentGeomPosition(key, bp2.x,bp2.y,bp2.z);

			HC_Open_Segment("");
 			HC_Set_Color("edges = white");
 			HC_Set_Text_Font("name = stroked, size = 14pts");
			HC_Insert_Text(0,0,0,"DEL");
 
   			HC_Close_Segment(); 
			HC_Close_Segment();
			HC_Close_Segment();
		}
 
		break;

	case HConstrainToViewplane:
		{
			HPoint p1,p2;
 
			HC_Open_Segment("hobjects");
			HC_Set_Color("faces = white");
			HC_Flush_Contents(".","everything");
			HC_Set_Visibility("markers = off");
			HPoint bp, bp2;
			ShowBasePosition(handlekey, bp);
			bp2=bp;
 
			HC_KEY key = HUtility::CreateScaleIndependentSegment("xxx", false);
			HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
		 	HUtility::SetScaleIndependentGeomPosition(key, bp2.x,bp2.y,bp2.z);

			HC_Open_Segment("");
			HC_Set_Color("faces = grey");
			HPoint center(0,0,0);
			HPoint axis(0,1,0);
			HPoint ortho(1,0,0);
			float radius = 0.04f * sf;

			HC_Insert_Sphere(&center, radius, &axis, &ortho);

   			HC_Close_Segment(); 
			HC_Close_Segment();
			HC_Close_Segment();
  
		}
 
		break;
	case HConstrainToPlane:
		{
	HC_Open_Segment("hobjects");

			float rotmat[16];
			float tplane1[4];
			float tplane2[4];
			float plane[4];

			HC_Compute_Polygon_Plane(3,m_rpoint,"right",plane);

			HC_Compute_Rotation_Matrix(90,0,0,rotmat);
			HC_Compute_Transformed_Plane(plane,rotmat, tplane1);
			HC_Compute_Rotation_Matrix(0,0,90,rotmat);
			HC_Compute_Transformed_Plane(plane,rotmat, tplane2);
 


			HC_Flush_Contents(".","everything");
			HC_Set_Visibility("markers = off");
			HPoint bp, bp2;
			ShowBasePosition(handlekey, bp);
			bp2=bp;
  

			HC_KEY key = HUtility::CreateScaleIndependentSegment("xxx", false);
			HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
		 	HUtility::SetScaleIndependentGeomPosition(key, bp2.x,bp2.y,bp2.z);

			HC_Open_Segment("");
 			HC_Set_Color("edges = white");
			HPoint p1,p2;
			p2.Set(m_rpoint[1].x - m_rpoint[0].x,m_rpoint[1].y - m_rpoint[0].y,
				m_rpoint[1].z - m_rpoint[0].z);
 			p1.Set(0+0.00001f,0+0.0000001f,0+0.00001f);
		 
			HC_Open_Segment("arrow1");
			HUtility::RotateToVector(p1,p2, false);
  			HUtility::InsertArrow(0.075f*sf, 0.05f*sf, 0.014f*sf, 0.04f*sf,0,"faces = (r=1.0 g=1.0 b=1.0), edges = white", "faces = (r=0.5 g=0.5 b=0.5) , edges = white",6,0,true);
			HC_Open_Segment("arrow2");
 			HUtility::InsertArrow(0.075f*sf, 0.05f*sf, 0.014f*sf, 0.04f*sf,0,"faces = (r=1.0 g=1.0 b=1.0), edges = white", "faces = (r=0.5 g=0.5 b=0.5) , edges = white",6,0,true);
			HC_Rotate_Object(180,0,0);
  			HC_Close_Segment();
 
			HC_Close_Segment();
 
			HC_Open_Segment("arrow2");
			p2.Set(m_rpoint[2].x - m_rpoint[0].x,m_rpoint[2].y - m_rpoint[0].y,
				m_rpoint[2].z - m_rpoint[0].z);
			p1.Set(0+0.00001f,0+0.000002f,0+0.000001f);
			HUtility::RotateToVector(p1,p2, false);
 			HUtility::InsertArrow(0.075f*sf, 0.05f*sf, 0.014f*sf, 0.04f*sf,0,"faces = (r=1.0 g=1.0 b=1.0), edges = white", "faces = (r=0.5 g=0.5 b=0.5) , edges = white",6,0,true);
// 			HC_Define_System_Options( "no bounding volumes" );
			HC_Open_Segment("arrow2");
 			HUtility::InsertArrow(0.075f*sf, 0.05f*sf, 0.014f*sf, 0.04f*sf,0,"faces = (r=1.0 g=1.0 b=1.0), edges = white", "faces = (r=0.5 g=0.5 b=0.5) , edges = white",6,0,true);
			HC_Rotate_Object(180,0,0);
  			HC_Close_Segment();
  			HC_Close_Segment();
  			HC_Close_Segment();
  
			HC_Close_Segment();
			HC_Close_Segment();
			}
		break;
	case HConstrainToRotationVector:
		{
			HPoint p1,p2;
			p1 = m_rpoint[0];
			p2 = m_rpoint[1];

	HC_Open_Segment("hobjects");
			HC_Set_Heuristics("quick moves = spriting");

//			HC_Flush_Contents(".","everything");
			HC_Set_Visibility("markers = off");
			HPoint bp, bp2;
			ShowBasePosition(handlekey, bp);
			bp2=bp;
  	HC_Set_Color("lines = yellow");
	HC_Set_Visibility("lines = on");
  
// 	HC_Insert_Line(m_rpoint[0].x, m_rpoint[0].y, m_rpoint[0].z, m_rpoint[1].x, m_rpoint[1].y, m_rpoint[1].z);


			HC_KEY key = HUtility::CreateScaleIndependentSegment("xxx", false);
			HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
		 	HUtility::SetScaleIndependentGeomPosition(key, bp2.x,bp2.y,bp2.z);

			if (!HC_Show_Existence("segment"))
			{			
  				HC_Open_Segment("tempover");
  				HC_Open_Segment("xxx");
 			HC_Set_Color("edges = white");
			HUtility::RotateToVector(p1,p2, false);
 			HC_Open_Segment("");
			HC_Rotate_Object(90,0,0);
			HC_Set_Color("faces = (r=1.0 g=1.0 b=1.0), edges = white");
 			HUtility::InsertRotatedArrow(0.075f*sf,0.05f*sf,0.014f*sf, 0.020f*sf, 0.02f, 135, 2.0f,"faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black), edges = white","faces = (diffuse = r=0.5 g=0.5 b=0.5, transmission = black), edges = white",6,1.0f,false);
			HC_Close_Segment();
// 			HC_Define_System_Options( "no bounding volumes" );
  			HC_Close_Segment();
			HC_Close_Segment();
			}
			else
			{
  				HC_Open_Segment("tempover");
				if (HC_Show_Existence("modelling matrix"))
					HC_UnSet_Modelling_Matrix();
				float s = (sf - 0.5f) * 2.0f;
				HC_Scale_Object(1.0 + s ,1.0 + s,1.0 + s);
				HC_Close_Segment();

			}
			HC_Close_Segment();
			HC_Close_Segment();

		}
 
		break;

	default:
		;
	}
	HC_Close_Segment();

}

void HUtilityGeomHandle::ResetHandle(HC_KEY handlekey, float scale, HBaseView *view)
{
	GetConstraintParameters(handlekey);

 	HC_Open_Segment_By_Key(handlekey);
	HC_Set_Marker_Size(scale);

	switch (m_ConstraintType)
	{
	case HConstrainToOnOff:

	HC_Open_Segment("hobjects");
			HC_Flush_Contents(".","everything");
	HC_Close_Segment();
	break;
	case HConstrainToViewplane:
		ShowHandle(handlekey, scale, view);
	break;

	case HConstrainToRay:
		{
	 
	ShowHandle(handlekey, scale, view);
 
		}
 
		break;
	case HConstrainToPlane:
		{
	ShowHandle(handlekey, scale, view);
		}
		break;
	case HConstrainToRotationVector:
		{
	ShowHandle(handlekey,scale, view);
		}
		break;
	case HConstrainToScale:
	ShowHandle(handlekey, scale, view);

 		break;
	default:
		;
	}
	
	HC_Close_Segment();
}


void HUtilityGeomHandle::ConstrainToRay(HC_KEY handlekey, HPoint p1, HPoint p2,HBaseView *view)
{
	m_ConstraintType = HConstrainToRay;
	m_rpoint[0] = p1;
	m_rpoint[1] = p2;
	 
	char constrains[MVO_BUFFER_SIZE];
	char constrainparam[MVO_BUFFER_SIZE];
	HCLOCALE (sprintf(constrainparam, "H_HANDLE_C_PARAM = %f %f %f %f %f %f", 
		p1.x, p1.y, p1.z, p2.x, p2.y, p2.z));
	sprintf(constrains, "H_HANDLE_C_TYPE = %d",(int)m_ConstraintType);

 	HC_Open_Segment_By_Key(handlekey);
	HC_Set_Marker_Symbol("(.)");
 	HC_Set_User_Options (constrainparam);
	HC_Set_User_Options (constrains);
			
	HC_Close_Segment();
	ShowHandle(handlekey, 0.5, view);

}

void HUtilityGeomHandle::AdjustPositionToRay(HC_KEY handlekey, HPoint &C, HPoint A, HPoint B)
{
	UNREFERENCED(handlekey);

	HPoint b,c;
 	b.Set(B.x-A.x, B.y - A.y, B.z-A.z);
	c.Set(C.x-A.x, C.y - A.y, C.z-A.z);
 	float bc_dot = (float)HC_Compute_Dot_Product(&b, &c);
	float b_length = (float)HC_Compute_Vector_Length(&b);
	C.Set(A.x + ((bc_dot)/(b_length*b_length)) * b.x, A.y + ((bc_dot)/(b_length*b_length)) * b.y,
		A.z + ((bc_dot)/(b_length*b_length)) * b.z);	
}

float HUtilityGeomHandle::CalculatePlaneZ(HC_KEY handlekey, HPoint point)
{
	HPoint temp;
 
	HC_Open_Segment_By_Key(handlekey);
	temp = point;
	HC_Close_Segment();

 	HC_Compute_Coordinates (".", "world", &temp,
		"projection", &temp);
	return temp.z;
}
										 
void HUtilityGeomHandle::AdjustPointToRay(HC_KEY geomhandlekey, HPoint & poi, HC_KEY camerasegment, HBaseView *view)
{

 	
	HPoint s;
 	GetConstraintParameters(geomhandlekey);
 	HC_Open_Segment_By_Key(camerasegment); 	
	HC_Show_Net_Camera_Position(&s.x, &s.y , &s.z);
	HC_Close_Segment();
	
 	HPoint pa1, pa2;
	double dummy1, dummy2;
	HPoint mr1, mr2;
	mr1 = m_rpoint[0];
	mr2 = m_rpoint[1];

	CalculateTransformedPosition(geomhandlekey, mr1, view);
	CalculateTransformedPosition(geomhandlekey, mr2, view);

	
	HPoint temp, temp2;
	
	if (view->GetProjMode() != ProjPerspective)
	{
	    HC_Open_Segment_By_Key(camerasegment); 	
	    HC_Compute_Coordinates(".", "world", &poi, "viewpoint", &temp);
	    HC_Compute_Coordinates(".", "world", &mr1, "viewpoint", &temp2);
	    temp.z = temp2.z;
	    HC_Compute_Coordinates(".", "viewpoint", &temp, "world", &temp);
	    HC_Close_Segment();
	}
	else
	    temp = poi;


	LineLineIntersect(mr1,mr2, s,temp, &pa1, &pa2, &dummy1, &dummy2);
	poi = pa1;
//	Move(geomhandlekey,pa1);
}			
	


void HUtilityGeomHandle::CalculatePositionOnPlane(HC_KEY handlekey, HPoint &C, float a, float b, float c, float d)
{
	UNREFERENCED(handlekey);

	float D;
 	HPoint vec(a,b,c);
	HC_Compute_Normalized_Vector(&vec, &vec);
	HPoint q(vec.x * (-d), vec.y * (-d), vec.z * (-d));
	HPoint pmq(C.x - q.x, C.y - q.y, C.z - q.z);
	D = (float)HC_Compute_Dot_Product(&vec, &pmq);	
 	C.Set(C.x - D * a, C.y - D * b, C.z - D * c);    	
}

  										 
void HUtilityGeomHandle::AdjustPositionToPlane(HC_KEY handlekey, HPoint &poi, float A, float B, float C, float D, HC_KEY camerasegment)
{
	HPoint s, d, poi2;
	float a,b, M;
 	HC_Open_Segment_By_Key(camerasegment); 
	char projection[MVO_BUFFER_SIZE];
	HC_Show_Camera_Projection(projection);
	if (projection[0] != 'p')
	{
		HC_Compute_Coordinates(".","world",&poi,"viewpoint", &poi2);
		poi2.z = 0;
		HC_Compute_Coordinates(".","viewpoint",&poi2,"world", &s);
	}
	else
		HC_Show_Net_Camera_Position(&s.x, &s.y , &s.z);
	HC_Close_Segment();

 	HC_Open_Segment_By_Key(handlekey); 	
  	d.Set(poi.x - s.x, poi.y - s.y, poi.z - s.z);
	HC_Compute_Normalized_Vector(&d,&d);

	a = A * d.x + B * d.y + C * d.z;
 
	b = A * s.x + B * s.y + C * s.z + D;	
	M = -b/a;
	HPoint res;

	res.Set(s.x + d.x * M,s.y + d.y * M, s.z + d.z * M);
	HPoint temp;
 	HC_Open_Segment_By_Key(camerasegment); 	
	HC_Compute_Coordinates(".","world",&res,"viewpoint", &temp);
	HC_Close_Segment();
		
   	if (a==0 || b==0 || temp.z < 0)
		CalculatePositionOnPlane(handlekey,poi, A,  B, C, D);
	else
		poi = res;
  	HC_Close_Segment();        
   	
}



void HUtilityGeomHandle::SetupObject(HC_KEY key, HBaseView *view, float boxzoom)
{
	HPoint min, max;
	bool wireframe;

	if (view->GetRenderMode() == HRenderBRepHiddenLine || view->GetRenderMode() == HRenderHiddenLine || view->GetRenderMode() == HRenderHiddenLineFast)
		wireframe = true;
	else
		wireframe = false;

	HC_KEY owner;
	
	if (view->GetCuttingPlanesKey() == key)
		owner = key;
	else
		owner = HC_KShow_Owner_By_Key(key);
 	HUtilityGeomHandle ghandle;
 
	HC_Open_Segment_By_Key(key);
	HC_Compute_Circumcuboid(".",&min, &max);
	if (min.x == 0 && min.y == 0 && min.z == 0 && max.x == 0 && max.y ==0 && max.z ==0)
	{
		min.x = min.y = min.z = -0.1f;
		max.x = max.y = max.z = 0.1f;
	}
	HPoint mid;
 	mid.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	HPoint delta(min.x - mid.x, min.y - mid.y, min.z - mid.z);
	min.Set(mid.x + delta.x * boxzoom, mid.y + delta.y * boxzoom, mid.z + delta.z * boxzoom);
	delta.Set(max.x - mid.x, max.y - mid.y, max.z - mid.z);
	max.Set(mid.x + delta.x * boxzoom, mid.y + delta.y * boxzoom, mid.z + delta.z * boxzoom);
	HPoint p1,r1,r2,r3;
	HC_KEY hkey;

	if (view->GetCuttingPlanesKey() != owner)
	{
	HC_Open_Segment("cpbox");
	HC_Set_Visibility("faces = on, edges = on");
	HC_Set_Heuristics("no backplane culling");
 	HUtility::InsertBox(&min, &max);
	HC_Set_Visibility("cutting planes = off");
 	HC_Set_Color("faces = (transmission = light grey)");
	HC_Close_Segment();

  
	hkey = ghandle.Insert(max, wireframe); 	
	r1.Set(max.x, max.y, max.z);
	r2.Set(max.x, max.y -1 , max.z);
	ghandle.ConstrainToRay(hkey, r1,r2);

	p1.Set(max.x, min.y, max.z);
	hkey = ghandle.Insert(p1, wireframe);
 	
	r1.Set(max.x, min.y, max.z);
	r2.Set(max.x-1, min.y , max.z);
	ghandle.ConstrainToRay(hkey, r1,r2);

	p1.Set(min.x, max.y, max.z);
	hkey = ghandle.Insert(p1, wireframe);
	r1.Set(min.x, max.y, max.z);
	r2.Set(min.x, max.y , max.z-1);
	ghandle.ConstrainToRay(hkey, r1,r2);
/*
 	p1.Set(max.x, min.y, max.z);
	hkey = ghandle.Insert(p1);
	r1.Set(max.x, min.y, max.z);
	r2.Set(max.x, min.y , max.z-1);
	ghandle.ConstrainToRay(hkey, r1,r2);
*/	
	

 

 	hkey = ghandle.Insert(min, wireframe);
 	r1.Set(min.x, min.y, min.z);
	r2.Set(min.x + 1.0f, min.y, min.z);
	r3.Set(min.x,min.y,min.z+1.0f);
	ghandle.ConstrainToPlane(hkey, r1,r2,r3);
  

	p1.Set(max.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);
  

	p1.Set(mid.x,mid.y,min.z);
 	hkey = ghandle.Insert(p1, wireframe);
	r1.Set(mid.x,mid.y,min.z);
	r2.Set(mid.x,mid.y,max.z+0.00001f);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);


	p1.Set(mid.x,min.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);	
 	r1.Set(mid.x,min.y,mid.z);
	r2.Set(mid.x,max.y+0.00001f,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

/*	p1.Set(mid.x,max.y,mid.z);
 	hkey = ghandle.Insert(p1);	
 	r1.Set(mid.x,min.y,mid.z);
	r2.Set(mid.x,max.y+0.00001f,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(min.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);
*/ 	
	p1.Set(mid.x,max.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);	
 	ghandle.ConstrainToViewplane(hkey);


	p1.Set(min.x,max.y,min.z);
	HPoint p2(1,1,1);
  	hkey = ghandle.Insert(p1, wireframe);	
 	ghandle.ConstrainToScale(hkey, mid,p2);
	}
	else
	{
	hkey = ghandle.Insert(max, wireframe); 	
	r1.Set(max.x, max.y, max.z);
	r2.Set(max.x, max.y -1 , max.z);
	ghandle.ConstrainToRay(hkey, r1,r2);
 
	hkey = ghandle.Insert(min, wireframe); 	
	r1.Set(min.x, min.y, min.z);
	r2.Set(min.x, min.y -1 , min.z);
	ghandle.ConstrainToRay(hkey, r1,r2);

	p1.Set(max.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(min.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(mid.x,mid.y,min.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(mid.x,mid.y,max.z);
	r2.Set(mid.x,mid.y,min.z+0.0001f);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(mid.x,mid.y,max.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(mid.x,mid.y,max.z);
	r2.Set(mid.x,mid.y,min.z+0.0001f);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);


	}

	HC_Close_Segment();
 
}
 

 


void HUtilityGeomHandle::SetupCuttingPlane(HC_KEY key, HBaseView *view)
{
	HPoint min, max;
	float boxzoom = 1.0f;
	bool wireframe;

	if (view->GetRenderMode() == HRenderBRepHiddenLine || view->GetRenderMode() == HRenderHiddenLine || view->GetRenderMode() == HRenderHiddenLineFast)
		wireframe = true;
	else
		wireframe = false;

	HC_KEY owner;
	
	owner = key;
 
	HC_Open_Segment_By_Key(owner);
	HUtilityGeomHandle ghandle;
	HC_Begin_Contents_Search(".", "segments");
	char type[MVO_BUFFER_SIZE];
	while (HC_Find_Contents(type, &key))
	{
		HC_Open_Segment_By_Key(key);
	
	HC_Compute_Circumcuboid(".",&min, &max);
	if (min.x == 0 && min.y == 0 && min.z == 0 && max.x == 0 && max.y ==0 && max.z ==0)
	{
		min.x = min.y = min.z = -0.1f;
		max.x = max.y = max.z = 0.1f;
	}
	HPoint mid;
 	mid.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	HPoint delta(min.x - mid.x, min.y - mid.y, min.z - mid.z);
	min.Set(mid.x + delta.x * boxzoom, mid.y + delta.y * boxzoom, mid.z + delta.z * boxzoom);
	delta.Set(max.x - mid.x, max.y - mid.y, max.z - mid.z);
	max.Set(mid.x + delta.x * boxzoom, mid.y + delta.y * boxzoom, mid.z + delta.z * boxzoom);
	HPoint p1,r1,r2;
	HC_KEY hkey;
  
	hkey = ghandle.Insert(max, wireframe); 	
	r1.Set(max.x, max.y, max.z);
	r2.Set(max.x, max.y -1 , max.z);
	ghandle.ConstrainToRay(hkey, r1,r2);
 
	hkey = ghandle.Insert(min, wireframe); 	
	r1.Set(min.x, min.y, min.z);
	r2.Set(min.x, min.y -1 , min.z);
	ghandle.ConstrainToRay(hkey, r1,r2);

	p1.Set(max.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(min.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(mid.x,mid.y,min.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(mid.x,mid.y,max.z);
	r2.Set(mid.x,mid.y,min.z+0.0001f);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	p1.Set(mid.x,mid.y,max.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(mid.x,mid.y,max.z);
	r2.Set(mid.x,mid.y,min.z+0.0001f);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);

	HC_Close_Segment();
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
}



void HUtilityGeomHandle::ClearAllHandles(HBaseView *view)
{
	HUtilityGeomHandle note;
	HC_KEY skey;
	char type[MVO_BUFFER_SIZE];
	char text[MVO_BUFFER_SIZE];
	char ssegname[MVO_SEGMENT_PATHNAME_BUFFER];

	if (view)
		HC_Open_Segment_By_Key(view->GetSceneKey());
	HC_Begin_Contents_Search("...", "segments");
	while (HC_Find_Contents(type, &skey))
	{
		HC_Open_Segment_By_Key(skey);
		if (HUtility::ShowOneUserOption("hashandles",0))
			HC_UnSet_One_User_Option("hashandles");
		HC_Close_Segment();

		if (skey >= 0)
			continue; //geometry handles (for now, at least) are never renumbered

		HC_Show_Segment(skey, text);
		HC_Parse_String(text, "/", -1, ssegname);

		if (note.IsGeomHandle(skey )!=INVALID_KEY ||  strcmp(ssegname, "cpbox") == 0)
		{
			char text[MVO_BUFFER_SIZE];
			HC_Show_Segment(skey, text);
			HC_Delete_By_Key(skey);
		}
	}
	HC_End_Contents_Search();
	if (view)
		HC_Close_Segment();
	else
		return;

	HC_Open_Segment_By_Key(view->GetModel()->GetModelKey());
		HC_Begin_Contents_Search("...", "segments");
		while (HC_Find_Contents(type, &skey))
		{

			if (skey >= 0)
				continue; //geometry handles (for now, at least) are never renumbered

			HC_Show_Segment(skey, text);
			HC_Parse_String(text, "/", -1, ssegname);

			if (note.IsGeomHandle(skey )!=INVALID_KEY ||  strcmp(ssegname, "cpbox") == 0)
			{
				char text[MVO_BUFFER_SIZE];
				HC_Show_Segment(skey, text);
				HC_Delete_By_Key(skey);
			}
		}
		HC_End_Contents_Search();
	HC_Close_Segment();
}


HC_KEY HUtilityGeomHandle::SelectHandle(HBaseView *view, HPoint new_pos)
{
	HUtilityGeomHandle note;
    int		res;
	HC_KEY   key;
	HC_KEY NoteKey = INVALID_KEY;

	HC_Open_Segment_By_Key(view->GetViewKey());
		res = HC_Compute_Selection(".", "./scene/overwrite", "v, no related selection limit, visual selection = off, selection level = entity, selection proximity = 0.3", new_pos.x, new_pos.y);
    HC_Close_Segment();

	// compute the selection using the HOOPS window coordinate of the the pick location
	if (res > 0)
	{
		do	// process each item in the selection queue
		{
			HC_Show_Selection_Element (&key, 0, 0, 0);
			char type[MVO_BUFFER_SIZE];
			HC_Show_Key_Type(key,type);
			HC_KEY temp = note.IsGeomHandle(key);
			if (temp != INVALID_KEY)
			{
				NoteKey = temp;
				break;
			}
		} while (HC_Find_Related_Selection());
	}

 	return NoteKey;
}


static void UnwindIncludes(HC_KEY &newkey)
{
	HC_KEY *keys = 0;
	int count;
	
	HC_Show_Selection_Keys_Count (&count);
	keys = new HC_KEY[count];

	HC_Show_Selection_Keys (&count, keys);
	bool oneinclude = false;
	for (int i=count-1;i>=0;i--)
	{
		char sname[MVO_SEGMENT_PATHNAME_BUFFER];
 		HC_Show_Key_Type(keys[i], sname);
		if (strcmp(sname, "include") == 0)
		{
			if (oneinclude)
				newkey = keys[i + 1];
			else
				oneinclude = true;

		}
	}
	delete [] keys;
}
		




HC_KEY HUtilityGeomHandle::SelectObject(HBaseView *view, HPoint new_pos, HC_KEY oldkey, bool exclude, float proximity)
{
	HUtilityGeomHandle note;

    int		res;
	HC_KEY    key;

	HC_KEY ObjectKey = INVALID_KEY;

 	HC_Open_Segment_By_Key(view->GetViewKey());

		if(proximity<0)
			res = HC_Compute_Selection(".", "./scene/overwrite", "v, no related selection limit, visual selection = off, selection level = entity, no selection proximity", new_pos.x, new_pos.y);
		else {
			char opt[MVO_BUFFER_SIZE];
			HCLOCALE(sprintf(opt, "v, no related selection limit, visual selection = off, selection level = entity, selection proximity=%f", proximity));
			res = HC_Compute_Selection(".", "./scene/overwrite", opt, new_pos.x, new_pos.y);
		}

	HC_Close_Segment();


	// compute the selection using the HOOPS window coordinate ofthe the pick location
	if (res > 0)
	{
		do	// process each item in the selection queue
		{
			HC_Show_Selection_Element (&key, 0, 0, 0);
			char type[MVO_BUFFER_SIZE];
			HC_Show_Key_Type(key,type);
			if (strcmp(type, "segment")!=0)
				key = HC_KShow_Owner_By_Key(key);
			UnwindIncludes(key);
			if (note.IsGeomHandle(key) == INVALID_KEY)
			{					
				char segmentname[MVO_SEGMENT_PATHNAME_BUFFER];
				char ssegname[MVO_SEGMENT_PATHNAME_BUFFER];
				HC_Show_Segment(key, segmentname);
				
				HC_Parse_String(segmentname, "/", -1, ssegname);
				if (strcmp(ssegname, "cpbox") == 0 || strcmp(ssegname, "cutting grids") == 0)
					key = HC_KShow_Owner_By_Key(key);
				if (ObjectKey == INVALID_KEY)
					ObjectKey = key;
				if (exclude && oldkey == key)
					ObjectKey = INVALID_KEY;
				else
				{
					if (oldkey == key)
					{
						ObjectKey = key;
						break;
					}
				}
			}
			
		} while (HC_Find_Related_Selection());
	}
	ObjectKey = HObjectManager::FindHObjectSegment(ObjectKey);

	return ObjectKey;
}




void HUtilityGeomHandle::SetupTextObject(HC_KEY key, HBaseView *view, float boxzoom, float scale)
{
	HPoint min, max;
	bool wireframe;

	if (view->GetRenderMode() == HRenderBRepHiddenLine || view->GetRenderMode() == HRenderHiddenLine || view->GetRenderMode() == HRenderHiddenLineFast)
		wireframe = true;
	else
		wireframe = false;

 	HUtilityGeomHandle ghandle;
 
	HC_Open_Segment_By_Key(key);

	min.x = min.y = min.z = -0.5f;
	max.x = max.y = max.z = 0.5f;
	HPoint mid;
 	mid.Set((min.x + max.x) / 2.0f, (min.y + max.y) / 2.0f, (min.z + max.z) / 2.0f);
	HPoint delta(min.x - mid.x, min.y - mid.y, min.z - mid.z);
	min.Set(mid.x + delta.x * boxzoom, mid.y + delta.y * boxzoom, mid.z + delta.z * boxzoom);
	delta.Set(max.x - mid.x, max.y - mid.y, max.z - mid.z);
	max.Set(mid.x + delta.x * boxzoom, mid.y + delta.y * boxzoom, mid.z + delta.z * boxzoom);
	HPoint p1,r1,r2,r3;
	HC_KEY hkey;

	 
	HC_Open_Segment("cpbox");
	HC_Set_Visibility("faces = off, edges = on");
	HC_Set_Heuristics("no backplane culling");
 	HUtility::InsertBox(&min, &max);
	HC_Set_Visibility("cutting planes = off");
 	HC_Set_Color("faces = (transmission = light grey)");
	HC_Close_Segment();

  
	hkey = ghandle.Insert(max, wireframe); 	
	r1.Set(max.x, max.y, max.z);
	r2.Set(max.x, max.y -1 , max.z);
	ghandle.ConstrainToRay(hkey, r1,r2);
	ghandle.ShowHandle(hkey, scale, view);
 
	p1.Set(max.x, min.y, max.z);
	hkey = ghandle.Insert(p1, wireframe); 	
	r1.Set(max.x, min.y, max.z);
	r2.Set(max.x-1, min.y , max.z);
	ghandle.ConstrainToRay(hkey, r1,r2);
	ghandle.ShowHandle(hkey, scale, view);


	p1.Set(min.x, max.y, max.z);
	hkey = ghandle.Insert(p1, wireframe);
	r1.Set(min.x, max.y, max.z);
	r2.Set(min.x, max.y , max.z-1);
	ghandle.ConstrainToRay(hkey, r1,r2);
	ghandle.ShowHandle(hkey, scale, view);


 	hkey = ghandle.Insert(min, wireframe);
 	r1.Set(min.x, min.y, min.z);
	r2.Set(min.x + 1.0f, min.y, min.z);
	r3.Set(min.x,min.y,min.z+1.0f);
	ghandle.ConstrainToPlane(hkey, r1,r2,r3);
	ghandle.ShowHandle(hkey, scale, view);
  

	p1.Set(max.x,mid.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);
 	r1.Set(min.x,mid.y,mid.z);
	r2.Set(max.x+0.00001f,mid.y,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);
	ghandle.ShowHandle(hkey, scale, view);
  

	p1.Set(mid.x,mid.y,min.z);
 	hkey = ghandle.Insert(p1, wireframe);
	r1.Set(mid.x,mid.y,min.z);
	r2.Set(mid.x,mid.y,max.z+0.00001f);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);
	ghandle.ShowHandle(hkey, scale, view);


	p1.Set(mid.x,min.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);	
 	r1.Set(mid.x,min.y,mid.z);
	r2.Set(mid.x,max.y+0.00001f,mid.z);
	ghandle.ConstrainToRotationVector(hkey, r1,r2);
	ghandle.ShowHandle(hkey, scale, view);
 
	p1.Set(mid.x,max.y,mid.z);
 	hkey = ghandle.Insert(p1, wireframe);	
 	ghandle.ConstrainToViewplane(hkey);
	ghandle.ShowHandle(hkey, scale, view);


	p1.Set(min.x,max.y,min.z);
 	hkey = ghandle.Insert(p1, wireframe);	
 	ghandle.ConstrainToScale(hkey, mid,mid);
	ghandle.ShowHandle(hkey, scale, view);

	
	 

	HC_Close_Segment();
 
}



HC_KEY HUtilityGeomHandle::SetupManipulator(HC_KEY startkey, HBaseView *view, HPoint *midpoint, bool AddNonUniformScale, bool AddScale, bool AddPlane, bool AddAxisMove, bool AddAxisRotate, bool AddViewplaneMove)
{
 	float tvaluex, tvaluey, tvaluez;
    startkey = HObjectManager::FindHObjectSegment(startkey);
	if (startkey == INVALID_KEY )
		return INVALID_KEY;
    char seg[MVO_SEGMENT_PATHNAME_BUFFER];
    HC_Show_Segment(startkey, seg);
    HC_Open_Segment_By_Key(startkey);    
		char ropt[4096];
		if (HUtility::ShowOneRenderingOption("simple shadow", ropt))
		{
			if (strcmp(ropt,"off") != 0)
				HC_Set_Rendering_Options ("simple shadow=auto");
		}	
		else
			HC_Set_Rendering_Options ("simple shadow=auto");


		HC_KEY key = HUtility::CreateScaleIndependentSegment("cpbox"); 
		HC_Open_Segment_By_Key(key);
			HObjectManager::SetHObjectInfo("skip");

			HC_Set_Rendering_Options("no simple shadow, no frame buffer effects");
			HC_Set_Visibility("shadows = off");
			HC_Set_Visibility("cutting planes = off");
			HC_Set_Heuristics("culling = maximum extent = 0");
			HC_Set_Heuristics("culling = (no obscuration)");
		HC_Close_Segment();

		HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
			HC_Set_Rendering_Options("no simple shadow");
			HC_Set_Visibility("shadows = off");
			HC_Set_Heuristics("exclude bounding");
 			HC_Set_Rendering_Options("hidden line removal options = (render faces)" );
			HC_Set_Rendering_Options("attribute lock = (color = off)");
			HC_Set_Color("geometry = white");
			HC_Set_Heuristics("quick moves = spriting");

			bool wireframe= false;
			HPoint min, max, delta,mid;
			//float boxzoom = 1.0f;
			HUtilityGeomHandle ghandle;

			HUtility::CalculateBoundingInfo(startkey, min, max, delta, mid);

			HPoint p1,r1,r2,r3;
			HC_KEY hkey;	

			if (midpoint)
				mid = *midpoint;
		//	mid.Set(0,0,0);

			tvaluex = mid.x;
			tvaluey = mid.y;
			tvaluez = mid.z;
			mid.Set(0,0,0);
			float length;
			if (!AddAxisMove)
				length = 0.1f;
			else
				length = 0.17f;

			HC_Insert_Line(mid.x,mid.y,mid.z, mid.x,mid.y,mid.z + length);
			HC_Insert_Line(mid.x,mid.y,mid.z, mid.x,mid.y + length,mid.z);
			HC_Insert_Line(mid.x,mid.y,mid.z, mid.x+length,mid.y,mid.z);
		  
			HPoint ppp,ppp2, sf;

			if (AddAxisMove)
			{	
				ppp.Set(mid.x,mid.y,mid.z+0.17f);
 				hkey = ghandle.Insert(ppp, wireframe); 	
				r1.Set(mid.x,mid.y,mid.z);
				r2.Set(mid.x,mid.y,mid.z+0.17f);
				ghandle.ConstrainToRay(hkey, r1,r2, view);
			}

			if (AddAxisRotate)
			{	
				ppp.Set(mid.x,mid.y,mid.z+0.08f);
  				hkey = ghandle.Insert(ppp, wireframe);
				r1.Set(mid.x,mid.y,mid.z);
				r2.Set(mid.x,mid.y,mid.z+0.17f);
				ghandle.ConstrainToRotationVector(hkey, r1,r2, view);
			}

			if (AddNonUniformScale)
			{	
				ppp.Set(mid.x,mid.y,mid.z+0.045f);
 				ppp2.Set(mid.x,mid.y,mid.z);
  				hkey = ghandle.Insert(ppp, wireframe);	
				sf.Set(0,0,1);
				ghandle.ConstrainToScale(hkey, ppp2,sf, view);
			}

			if (AddAxisMove)
			{		
				ppp.Set(mid.x,mid.y+0.17f,mid.z);
				hkey = ghandle.Insert(ppp, wireframe); 	
				r1.Set(mid.x,mid.y,mid.z);
				r2.Set(mid.x,mid.y+0.17f,mid.z);
				ghandle.ConstrainToRay(hkey, r1,r2, view);
			}

			if (AddAxisRotate)
			{	
				ppp.Set(mid.x,mid.y+0.08f,mid.z);
  				hkey = ghandle.Insert(ppp, wireframe);
				r1.Set(mid.x,mid.y,mid.z);
				r2.Set(mid.x,mid.y+0.17f,mid.z);
				ghandle.ConstrainToRotationVector(hkey, r1,r2,view);
			}
			if (AddNonUniformScale)
			{	
				ppp.Set(mid.x,mid.y+0.045f,mid.z);
 				ppp2.Set(mid.x,mid.y,mid.z);
  				hkey = ghandle.Insert(ppp, wireframe);	
				sf.Set(0,1,0);
				ghandle.ConstrainToScale(hkey, ppp2,sf, view);
			}

			if (AddAxisMove)
			{	
				ppp.Set(mid.x + 0.17f,mid.y,mid.z);
				hkey = ghandle.Insert(ppp, wireframe); 	
				r1.Set(mid.x,mid.y,mid.z);
				r2.Set(mid.x + 0.17f,mid.y,mid.z);
				ghandle.ConstrainToRay(hkey, r1,r2, view);
			}

			if (AddAxisRotate)
			{	
				ppp.Set(mid.x + 0.08f,mid.y,mid.z);
  				hkey = ghandle.Insert(ppp, wireframe);
				r1.Set(mid.x,mid.y,mid.z);
				r2.Set(mid.x + 0.17f,mid.y,mid.z);
				ghandle.ConstrainToRotationVector(hkey, r1,r2, view);
			}

			if (AddNonUniformScale)
			{	
				ppp.Set(mid.x+0.045f,mid.y,mid.z);
 				ppp2.Set(mid.x,mid.y,mid.z);
  				hkey = ghandle.Insert(ppp, wireframe);	

				sf.Set(1,0,0);
				ghandle.ConstrainToScale(hkey, ppp2,sf, view);
			}

			if (AddViewplaneMove)
			{	
				ppp.Set(mid.x,mid.y,mid.z);
 				hkey = ghandle.Insert(ppp, wireframe);	
 				ghandle.ConstrainToViewplane(hkey, view);	
			}
		 
			if (AddPlane)
			{
				ppp.Set(mid.x - 0.17f,mid.y,mid.z);	
 				hkey = ghandle.Insert(ppp, wireframe);
 				r1.Set(mid.x, mid.y, mid.z);
				r2.Set(mid.x + 1.0f, mid.y, mid.z);
				r3.Set(mid.x,mid.y,mid.z+1.0f);
				ghandle.ConstrainToPlane(hkey, r1,r2,r3, view);
				
				ppp.Set(mid.x,mid.y - 0.17f,mid.z);	
 				hkey = ghandle.Insert(ppp, wireframe);
 				r1.Set(mid.x, mid.y, mid.z);
				r2.Set(mid.x + 1.0f, mid.y, mid.z);
				r3.Set(mid.x,mid.y + 1.0f,mid.z);
				ghandle.ConstrainToPlane(hkey, r1,r2,r3, view);
			
				ppp.Set(mid.x,mid.y,mid.z - 0.17f);	
 				hkey = ghandle.Insert(ppp, wireframe);
 				r1.Set(mid.x, mid.y, mid.z);
				r2.Set(mid.x, mid.y, mid.z + 1.0f);
				r3.Set(mid.x,mid.y + 1.0f,mid.z);
				ghandle.ConstrainToPlane(hkey, r1,r2,r3, view);
			}
			
			if (AddScale)
			{
 				ppp.Set(mid.x+0.01f,mid.y + 0.17f,mid.z + 0.1f);
 				ppp2.Set(mid.x,mid.y,mid.z);
  				hkey = ghandle.Insert(ppp, wireframe);	
				HC_Open_Segment_By_Key(hkey);
 				HC_Set_Color("faces = light blue");
				HC_Close_Segment();
				sf.Set(1,1,1);
				ghandle.ConstrainToScale(hkey, ppp2,sf, view);
			}

			HC_Set_Color("faces = blue");
			HUtility::SetScaleIndependentGeomPosition(key, tvaluex,tvaluey,tvaluez);

  			HC_Set_Rendering_Options("depth range=(0,0.1)");
		  
			HC_Set_Visibility("faces = off, edges = on, lines = on");
		HC_Close_Segment();
    HC_Close_Segment();
    view->SetHasObjects(true);
    return HUtility::GetScaleIndependentGeomSegmentKey(key);
}
 



void HUtilityGeomHandle::SetupCuttingPlaneManipulator(HC_KEY startkey, HBaseView *view, HPoint *midpoint, bool omit_rotate)
{
	float tvaluex, tvaluey, tvaluez;
     HC_Open_Segment_By_Key(startkey);    
     HC_KEY key = HUtility::CreateScaleIndependentSegment("cpbox"); 
	HC_Open_Segment_By_Key(key);
    HObjectManager::SetHObjectInfo("skip");
    HC_Set_Rendering_Options("no simple shadow");
	HC_Set_Visibility("cutting planes = off");
	HC_Set_Heuristics("culling = maximum extent = 0");
	HC_Set_Heuristics("culling = (no obscuration)");
    HC_Close_Segment();

    HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
 	HC_Set_Visibility("cutting planes = off");
     HC_Set_Rendering_Options("attribute lock = (color = off), no frame buffer effects");
	HC_Set_Rendering_Options("hidden line removal options = (render faces)" );
    HC_Set_Color("geometry = white");
	HC_Set_Heuristics("quick moves = spriting, exclude bounding");


    bool wireframe= false;
    HPoint min, max, mid, delta;
    //float boxzoom = 1.0f;
	HUtilityGeomHandle ghandle;

	HC_KEY ownerx = HC_KShow_Owner_By_Key(key);
	ownerx = HC_KShow_Owner_By_Key(ownerx);
 	HC_Set_Visibility("cutting planes = off");
	HUtility::CalculateBoundingInfo(startkey, min, max, delta, mid);
 	if (midpoint)
	    mid = *midpoint;

	HPoint p1,r1,r2,r3;
	HC_KEY hkey;	
 	tvaluex = mid.x;
	tvaluey = mid.y;
	tvaluez = mid.z;
	mid.Set(0,0,0);
	if (!omit_rotate)
	{
		HC_Insert_Line(mid.x,mid.y,mid.z, mid.x,mid.y,mid.z + 0.17f);
		HC_Insert_Line(mid.x,mid.y ,mid.z, mid.x,mid.y + 0.17f,mid.z);
 		HC_Insert_Line(mid.x,mid.y,mid.z, mid.x+0.17f,mid.y,mid.z);
	}
  

	
	HPoint ppp;

 

	if (!omit_rotate)
	{
		ppp.Set(mid.x,mid.y,mid.z+0.17f);
  		hkey = ghandle.Insert(ppp, wireframe);
		r1.Set(mid.x,mid.y,mid.z);
		r2.Set(mid.x,mid.y,mid.z+0.17f);
		ghandle.ConstrainToRotationVector(hkey, r1,r2, view);

	
		ppp.Set(mid.x,mid.y+0.17f,mid.z);
		hkey = ghandle.Insert(ppp, wireframe); 	
		r1.Set(mid.x,mid.y,mid.z);
		r2.Set(mid.x,mid.y+0.17f,mid.z);
		ghandle.ConstrainToRay(hkey, r1,r2, view);

 
		ppp.Set(mid.x + 0.17f,mid.y,mid.z);
  		hkey = ghandle.Insert(ppp, wireframe);
		r1.Set(mid.x,mid.y,mid.z);
		r2.Set(mid.x + 0.07f,mid.y,mid.z);
		ghandle.ConstrainToRotationVector(hkey, r1,r2, view);
 	}
	else
	{
			ppp.Set(mid.x,mid.y+0.05f,mid.z);
		hkey = ghandle.Insert(ppp, wireframe); 	
		r1.Set(mid.x,mid.y,mid.z);
		r2.Set(mid.x,mid.y+0.05f,mid.z);
		ghandle.ConstrainToRay(hkey, r1,r2, view);
	}



	
	HUtility::SetScaleIndependentGeomPosition(key, tvaluex,tvaluey,tvaluez);

//    	HC_Set_Rendering_Options("hsra = z-sort only");
  	HC_Set_Rendering_Options("depth range=(0,0.1)");



//     HUtilityGeomHandle::SetupObject(HUtility::GetScaleIndependentGeomSegmentKey(key), view, 1.0f); 

//	HC_Set_Line_Weight(4.0f);
  
    HC_Set_Visibility("faces = off, edges = on, lines = on");
     HC_Close_Segment();
    HC_Close_Segment();
    view->SetHasObjects(true);
}
 



void HUtilityGeomHandle::FindTranslation(HC_KEY handleKey, HPoint & trans)
{
	trans = HPoint();

	if (GetMMLevel(handleKey) < 3)
	{
		return;
	}

	handleKey = HC_Show_Owner_By_Key(handleKey, 0);
	handleKey = HC_Show_Owner_By_Key(handleKey, 0);

#ifndef _NDEBUG
	char segname[MVO_BUFFER_SIZE];
	HC_Show_Segment(handleKey, segname);
#endif

	HC_Open_Segment_By_Key(handleKey);{
		HC_KEY key = HC_Open_Segment("geom");{
			if (HUtility::UserOptionExists(key, "gpos", 0) != HC_ERROR_KEY)
			{
				char gposOpt[256];
				HC_Show_One_User_Option("gpos", gposOpt);
				HCLOCALE(sscanf(gposOpt, "%f %f %f", &trans.x, &trans.y, &trans.z));
			}
		}HC_Close_Segment();
	}HC_Close_Segment();
}


HC_KEY HUtilityGeomHandle::GetMMSegment(HC_KEY handlekey)
{ 
    int level = GetMMLevel(handlekey);
	for (int i=0;i<level;i++)	
	    handlekey = HC_KShow_Owner_By_Key(handlekey);

	return handlekey;
}

int HUtilityGeomHandle::GetMMLevel(HC_KEY handlekey)
{
    char level[MVO_BUFFER_SIZE];
    int l;
	HC_Open_Segment_By_Key(handlekey);
 	HC_Show_One_User_Option("H_HANDLE_C_MMLEVEL", level);		
	sscanf(level, "%d", &l);
	HC_Close_Segment();
	return l;
}

void HUtilityGeomHandle::GetCenterPoint(HC_KEY objectkey, HPoint &centerpoint)
{
    HC_Open_Segment_By_Key(objectkey);
    HC_KEY key = HC_KOpen_Segment("cpbox");
	HC_Close_Segment();
	HC_Close_Segment();
    //HUtility::GetScaleIndependentGeomSegmentKey(key)
    HUtility::GetScaleIndependentGeomPosition(key, centerpoint.x, centerpoint.y, centerpoint.z);
}

void HUtilityGeomHandle::SetVisibilityForAll(HC_KEY startkey, const char *visibility)
{
    HC_KEY segkey;
    char res[MVO_BUFFER_SIZE];
    HC_Open_Segment_By_Key(startkey);
    HC_Begin_Contents_Search("...", "segments");
    while (HUtility::FindContents(segkey))
    {
	HC_Open_Segment_By_Key(segkey);
	if (HUtility::ShowOneUserOption("geomhandle", res))
	{
	    HC_Set_Visibility(visibility);
	}
	HC_Close_Segment();
	
    }
    HC_End_Contents_Search();
    HC_Close_Segment();

}





