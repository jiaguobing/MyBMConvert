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
#include "HBhvAnimation.h"
#include "HBhvUtility.h"
#include "HBhvInterpolator.h"
#include "HBhvBehaviorManager.h"
#include "HEventInfo.h"
#include "HTools.h"
#include "HOpMoveHandle.h"
#include "HUtilityGeomHandle.h"
#include "HUtilityGeometryCreation.h"
#include "HMarkupManager.h"
#include "HSelectionSet.h"
#include "HObjects.h"

#include "stdio.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "hc.h"
#include "vlist.h"


const char * HLightsObject::GetStaticName() { return "light"; }
const char * HLightsObject::GetName() { return GetStaticName(); }

HLightsObject::HLightsObject(HBaseView *view) : HObject()
{
    view->GetEventManager()->RegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType(), HLISTENER_PRIORITY_HIGH);
	m_pView = view;
}

HLightsObject::~HLightsObject()
{
    m_pView->GetEventManager()->UnRegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType());
}


int HLightsObject::SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex) 
{
	UNREFERENCED(complex);

    char type[MVO_BUFFER_SIZE];
    key = HObjectManager::FindHObjectSegment(key, type);
     if (strcmp(type, GetStaticName()) == 0)
    {    	
        if (HC_Show_Existence_By_Key(key, "distant lights, first match"))
		HUtilityGeomHandle::SetupManipulator(key, view, 0, false, false, false,false, true, true);
        else if (HC_Show_Existence_By_Key(key, "local lights, first match"))
		HUtilityGeomHandle::SetupManipulator(key, view, 0, false, false, false,true, false, true);
        else if (HC_Show_Existence_By_Key(key, "spot lights, first match"))
		HUtilityGeomHandle::SetupManipulator(key, view, 0, false, false, false,true, true, true);
        return HLISTENER_CONSUME_EVENT;

    }
    else
        return HLISTENER_PASS_EVENT;
}

void HLightsObject::SetupLightSegmentFromKey(HBaseView *view, HPoint &pos, HC_KEY &outerkey, HC_KEY &geometrykey)
{
    if (!view->GetHObjectManager()->GetHObject(HLightsObject::GetStaticName()))
		view->GetHObjectManager()->AddHObject(new HLightsObject(view));

    HPoint p1(0,0,0);
    HPoint p2(0,0,1);
	HC_Open_Segment_By_Key(outerkey);
	HC_Set_Rendering_Options("no simple shadow");
	HC_Set_Heuristics("exclude bounding");

	HC_Set_Color("ambient = grey");    
	HC_Translate_Object(pos.x, pos.y, pos.z);
    HObjectManager::SetHObjectSegment(GetStaticName());
    HC_KEY key = HUtility::CreateScaleIndependentSegment("xxxx"); 
    HUtility::SetScaleIndependentGeomPosition(key, 0,0,0);
    
    HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));    
    HC_Set_Visibility("edges = off, lights = off");

    geometrykey = HC_KOpen_Segment("");
 //   HUtility::RotateToVector(p1,p2, false);
    HC_Set_Heuristics("no backplane cull");

  
    HC_Open_Segment("dummybox");
    HPoint p3(-1.0f,-1.0f,-1.0f), p4(1.0f,1.0f,1.0f);
    HUtility::InsertBox(&p3,&p4);
    HC_Set_Visibility("everything = off");
    HUtility::CloseAllSegments(outerkey);

}
void HLightsObject::SetupLightSegment(HBaseView *view, HPoint &pos, HC_KEY &outerkey, HC_KEY &geometrykey)
{
	outerkey =  HC_KOpen_Segment("");
	HC_Close_Segment();
	SetupLightSegmentFromKey(view, pos, outerkey, geometrykey);
}

void HLightsObject::TurnLightsOnOff(HBaseView *view,bool onoff)
{
    HC_Open_Segment_By_Key(view->GetModelKey());
    HC_Begin_Contents_Search("...", "local light, spot light, distant light");
	HC_KEY key;
	char type[4096];
	while (HC_Find_Contents(type, &key)) {

		char obname[4096];
		HC_KEY topkey = HObjectManager::FindHObjectSegment(HUtility::GrabSegment(key), obname);
		if (strcmp(obname, "light") == 0)
		{
		    HC_Open_Segment_By_Key(topkey);
		    if (onoff)
				HC_Set_Visibility("faces = on");
		    else
				HC_Set_Visibility("faces = off");
		    HC_Close_Segment();	
		}
	}
	HC_End_Contents_Search();
	HC_Close_Segment();
	view->Update();
}

void HLightsObject::InsertDistantLight(HBaseView *view, HPoint &pos)
{
    HC_KEY lightsegmentkey, geometrykey;
    SetupLightSegment(view, pos, lightsegmentkey, geometrykey);
    HC_Open_Segment_By_Key(lightsegmentkey);
    HC_Insert_Distant_Light(0,-1,0);    
    HC_Close_Segment();

    HC_Open_Segment_By_Key(geometrykey);
     HUtility::InsertArrow(0.10f, 0.075f, 0.017f, 0.085f,0,"faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black), edges = black", "faces = (diffuse = r=0.8 g=0.8 b=0.8, transmission = black) , edges = black",8,0,true);
    HC_Close_Segment();
 
}

void HLightsObject::InsertLocalLight(HBaseView *view, HPoint &pos)
{
    HC_KEY lightsegmentkey, geometrykey;
    SetupLightSegment(view, pos, lightsegmentkey, geometrykey);
    HC_Open_Segment_By_Key(lightsegmentkey);
    HC_Insert_Local_Light(0,0,0);
    HC_Close_Segment();

    HC_Open_Segment_By_Key(geometrykey);
    HPoint center(0,0,0);
    HPoint axis(0,1,0);
    HPoint ref(0,0,1);
    HUtilityGeometryCreation::CreateSphere(center, 0.07f,10, axis,ref);
    HC_Set_Color("faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black)");
	HC_Close_Segment();
 
}
void HLightsObject::InsertSpotLight(HBaseView *view, HPoint &pos)
{
    HPoint p1,p2;
    p1.Set(0,0,0);
    p2.Set(0,1,0);
    HC_KEY lightsegmentkey, geometrykey;
    SetupLightSegment(view, pos, lightsegmentkey, geometrykey);
    HC_Open_Segment_By_Key(lightsegmentkey);
    HC_Insert_Spot_Light(&p1, &p2, "");
    HC_Close_Segment();

    HC_Open_Segment_By_Key(geometrykey);
    HC_Open_Segment("");
    HPoint center(0,0,0);
    HPoint axis(0,1,0);
    HPoint ref(0,0,1);
    HC_Set_Color("faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black)");
    HUtilityGeometryCreation::CreateSphere(center, 0.07f,10, axis,ref);
    HUtility::InsertArrow(0.07f, 0.1f, 0.044f, 0.11f,0,"faces = (diffuse = r=1.0 g=1.0 b=1.0, transmission = black), edges = black", "faces = (diffuse = r=0.8 g=0.8 b=0.8, transmission = black) , edges = black",8,0,true);
 
    HC_Close_Segment();
    HC_Close_Segment();
 
}

const char * HDraggerObject::GetStaticName() { return "dragger"; }
const char * HDraggerObject::GetName() { return GetStaticName(); }

HDraggerObject::HDraggerObject(HBaseView *view) : HObject()
{
    if (view)
		view->GetEventManager()->RegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType(), HLISTENER_PRIORITY_HIGH);
    m_pView = view;
}

HDraggerObject::~HDraggerObject()
{
    if (m_pView)
		m_pView->GetEventManager()->UnRegisterHandler((HObjectManipulationListener *)this, HObjectManipulationListener::GetType());    
}


void HDraggerObject::SetupDraggerSegment(HBaseView *view, HPoint &pos, float *rotmat, HC_KEY &outerkey, HC_KEY &geometrykey)
{
    
    if (!view->GetHObjectManager()->GetHObject(HDraggerObject::GetStaticName()))
		view->GetHObjectManager()->AddHObject(new HDraggerObject(view));

    HPoint p1(0,0,0);
    HPoint p2(0,0,1);
    outerkey =  HC_KOpen_Segment("");
    float transmat[16];
    float finalmat[16];
    HC_Compute_Translation_Matrix(pos.x, pos.y, pos.z, transmat);
    HC_Compute_Matrix_Product(rotmat, transmat, finalmat);
    HC_Set_Modelling_Matrix(finalmat);
//    HC_Translate_Object(pos.x, pos.y, pos.z);
    HObjectManager::SetHObjectSegment(GetStaticName());
	HUtility::SuppressAnimation();
    HC_KEY key = HUtility::CreateScaleIndependentSegment("xxxx"); 
    HUtility::SetScaleIndependentGeomPosition(key, 0,0,0);    
    HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));    
    geometrykey = HC_KOpen_Segment("");
    HC_Set_Heuristics("no polygon handedness");
    HC_Set_Visibility("lights = on, edges = off, markers = off"); 
    HC_Open_Segment("dummybox");
    HPoint p3(-0.1f,-0.1f,-0.1f), p4(0.1f,0.1f,0.1f);
    HUtility::InsertBox(&p3,&p4);
    HC_Set_Visibility("everything = off");
    HUtility::CloseAllSegments(outerkey);
     
}

HC_KEY HDraggerObject::Insert(HBaseView *view, HPoint &pos, float *rotmatrix, HBhvAnimation *ani, HBhvAnimation *rotani, int kfr, bool AllowPos)
{
    
    HC_KEY outerkey, geometrykey;
    float rottemp[16];
    if (rotmatrix == 0)
    {    
		HC_Compute_Identity_Matrix(rottemp);
		SetupDraggerSegment(view, pos, rottemp,  outerkey, geometrykey);
    }
    else
		SetupDraggerSegment(view, pos, rotmatrix,  outerkey, geometrykey);

	HC_Open_Segment_By_Key(outerkey);
	char text[MVO_BUFFER_SIZE];
	sprintf(text, "kfr = %d", kfr);
	HC_Set_User_Options(text);

    if(AllowPos)
		HC_Set_User_Options("allowpos = true");
    else
 		HC_Set_User_Options("allowpos = false");

    HC_Set_User_Index(999,(void *)ani);
    HC_Set_User_Index(998,(void *)rotani);

	HC_Close_Segment();

    HC_Open_Segment_By_Key(geometrykey);
    HPoint center(0,0,0);
    HPoint axis(0,1,0);
    HPoint ref(0,0,1);
    HC_Set_Color("faces = (diffuse = r=0.0 g=0.0 b=1.0, transmission = black)");
    if (rotani)
    {
		HC_Open_Segment("");
		HPoint p1(0,0,0),p2(0,0,1);
		HUtility::RotateToVector(p1,p2, false);
		HUtility::InsertArrow(0.15f, 0.100f, 0.017f, 0.045f,0,"faces = red", "faces = light red",3,0,true);
		HC_Close_Segment();
		HC_Open_Segment("");
		p1.Set(0,0,0);
		p2.Set(0,-1,0);
		HUtility::RotateToVector(p1,p2, false);
		HUtility::InsertArrow(0.10f, 0.075f, 0.017f, 0.045f,0,"faces = grey", "faces = grey",3,0,true);
		HC_Close_Segment();
		HC_Open_Segment("");
		p1.Set(0,0,0);
		p2.Set(1,0,0);
		HUtility::RotateToVector(p1,p2, false);
		HUtility::InsertArrow(0.10f, 0.075f, 0.017f, 0.045f,0,"faces = grey", "faces = grey",3,0,true);
		HC_Close_Segment();
    }

    if (AllowPos)
		HUtilityGeometryCreation::CreateSphere(center, 0.04f,6, axis,ref);

	HC_Close_Segment();
	return outerkey;
}


int HDraggerObject::SetupHandlesEvent(HBaseView *view, HC_KEY key, bool complex) 
{
	UNREFERENCED(complex);

    char type[MVO_BUFFER_SIZE];
    key = HObjectManager::FindHObjectSegment(key, type);
    HUserOptions text;
    if (strcmp(type, GetStaticName()) == 0)
    {    	
		HBhvAnimation *rotani;
		HC_Open_Segment_By_Key(key);
			HC_Show_One_User_Index(998, (void **)&rotani);
		HC_Close_Segment();
		HC_Open_Segment_By_Key(key);
			text.Show_One_User_Option("allowpos");
		HC_Close_Segment();

		bool allowrot = false;
		if (rotani)
			allowrot = true;
		bool allowpos = false;
		if (strcmp(text.m_data, "true") == 0)
 			allowpos  = true;

  	    HUtilityGeomHandle::SetupManipulator(key, view,0, false, false, false,allowpos, allowrot, true);

        return HLISTENER_CONSUME_EVENT;
    }
    else
        return HLISTENER_PASS_EVENT;
}

 

int HDraggerObject::ObjectMovedEvent(HBaseView *view, HC_KEY key, HPoint *p) 
{
	UNREFERENCED(p);

    char type[MVO_BUFFER_SIZE];
    HUserOptions res;
    key = HObjectManager::FindHObjectSegment(key, type);
    if (strcmp(type, GetStaticName()) == 0)
    {    
		HUserOptions allowpos;

		HC_Open_Segment_By_Key(key);
			allowpos.Show_One_User_Option("allowpos");
 		HC_Close_Segment();

		HC_Open_Segment_By_Key(key);
		res.Show_One_User_Option("kfr");
		int kfr = atoi(res.m_data);
		HBhvAnimation *ani;





		HC_Show_One_User_Index(999, (void **)&ani);


	HBhvAnimation *rotani;
	HC_Show_One_User_Index(998, (void **)&rotani);

	HBhvInterpolatorQuatSquad *rotint = 0;
	HBhvInterpolatorPosition *posint = 0;

	START_LIST_ITERATION(HBhvInterpolator, ani->GetInterpolatorList());{
		if (!temp)
			break;
	    if (strcmp(temp->GetType(),"QuatRot") == 0)
			rotint = (HBhvInterpolatorQuatSquad *)temp;
		else
			if (strcmp(temp->GetType(),"Pos") == 0)	
				posint = ( HBhvInterpolatorPosition *)temp;
	}END_LIST_ITERATION(ani->GetInterpolatorList());


 		HKeyframeChannel *kcn = (HKeyframeChannel *)posint->GetAt(kfr);
		float matrix[16];
		HC_Show_Net_Modelling_Matrix(matrix);

		HBhvTargetObject *target = ani->GetTarget();
		HPoint pivot;
		target->GetPivot(pivot);


		if (strcmp(allowpos.m_data,"true") == 0)
				kcn->m_cp.Set(matrix[12] + pivot.x, matrix[13] + pivot.y, matrix[14] + pivot.z);
		HC_Close_Segment();
		ani->Reset();


		HC_Open_Segment_By_Key(key);
	



		if (rotani)
		{    
		
			if (!rotint)
				rotint = (HBhvInterpolatorQuatSquad *)rotani->GetInterpolator();
			HKeyframeQuatSquad *kcn = (HKeyframeQuatSquad *)rotint->GetAt(kfr);
			float matrix[16];
			float fquat[4];
			HC_Show_Net_Modelling_Matrix(matrix);
			HQuat quat;
			HUtility::MatrixToQuaternion(matrix, fquat);
			quat.Set(fquat[0], fquat[1], fquat[2], fquat[3]);
			kcn->m_quat = quat;    
			rotani->Reset();
		}
		HC_Close_Segment();
 


		if (strcmp(allowpos.m_data,"true") == 0)
		{
			HC_Open_Segment_By_Key(view->GetModelKey());
			HC_Open_Segment("tempplot");
			ani->PlotCurve(true);

			HC_Close_Segment();
			HC_Close_Segment();
		}
		view->Update();
		return HLISTENER_CONSUME_EVENT;
    }

    return HLISTENER_PASS_EVENT;
}

const char * HEdgeVertexSelectionObject::GetStaticName() { return "ev_select"; }
const char * HEdgeVertexSelectionObject::GetName() { return GetStaticName(); }

HEdgeVertexSelectionObject::HEdgeVertexSelectionObject(HBaseView *view) : HObject()
{
	m_pView = view;
	view->GetEventManager()->RegisterHandler((HMouseListener *)this, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 100);
	m_bFound = false;
	m_blButtonDown = false;
}

HEdgeVertexSelectionObject::~HEdgeVertexSelectionObject()
{
    m_pView->GetEventManager()->UnRegisterHandler((HMouseListener *)this, HMouseListener::GetType());    
}
 
int HEdgeVertexSelectionObject::OnLButtonDown(HEventInfo &event)
{
	if (m_bFound && event.Shift()  && event.Control())
		m_blButtonDown = true;       

    return HLISTENER_PASS_EVENT;
}

int HEdgeVertexSelectionObject::OnMouseMove(HEventInfo &event)
{    
	HC_KEY key;
	int o1,o2,o3;
	char type[MVO_BUFFER_SIZE];
	int count;
	
    if (!event.LButton() && !event.RButton() && !event.MButton() && event.Shift()  && event.Control())
    {
		m_blButtonDown = false;
		m_bFound = false;

		HPoint new_pos = event.GetMouseWindowPos();
		HC_Open_Segment_By_Key(m_pView->GetViewKey());

		count = HC_Compute_Selection(".", "./scene/overwrite", "v, selection level = entity, related selection limit = 1", new_pos.x, new_pos.y);

		HC_Close_Segment();
		if (count)
		{
			HC_Show_Selection_Element(&key, &o1, &o2, &o3);
			HC_Show_Key_Type(key, type);
			HC_KEY skey = HUtility::GrabSegment(key);
			HUtility::UnwindIncludes(skey);
			
			if (strcmp(type, "shell") == 0)
			{
				int plen, flen;
				HPoint *points;
				int *faces;
				float matrix[16];
				HC_Show_Shell_Size(key, &plen, &flen);
				points = new HPoint[plen];
				faces = new int[flen];
				HC_Show_Shell(key, &plen, points, &flen, faces);
				
				HC_Open_Segment_By_Key(skey);
					HC_Show_Net_Modelling_Matrix(matrix);
				HC_Close_Segment();
				m_Point1 = points[o1];
				m_Point2 = points[o2];
				m_SegKey = skey;
				
				
				m_bFound = true;
				HC_Open_Segment_By_Key(m_pView->GetSceneKey());
					HC_Open_Segment("edgevertex");
						HC_Flush_Contents(".", "everything");
						
						HC_Set_Visibility("markers = on, lines = on");
						HC_Set_Line_Weight(2.0f);
						HC_Set_Marker_Size(0.5f);
						
						IncludeEntity(m_pView,skey);
						int findex =  HUtility::FindFaceByIndex(o3, faces, flen);
						HPoint *oneface = new HPoint[faces[findex] + 1];
						
						for (int i=0;i<faces[findex];i++)		
							oneface[i] = points[faces[findex + i + 1]];
						
						oneface[faces[findex]] = points[faces[findex + 1]];
						HC_Compute_Transformed_Points(faces[findex] + 1, oneface, matrix, oneface);
						
						HC_Open_Segment("");
							HC_Set_Rendering_Options("depth range=(0,0.1)");
							HC_Insert_Polyline(faces[findex] + 1, oneface);
							
							HC_Compute_Transformed_Points(1, &points[o1], matrix, &points[o1]);
							HC_Compute_Transformed_Points(1, &points[o2], matrix, &points[o2]);
							
							HC_Insert_Marker(points[o1].x, points[o1].y, points[o1].z);
							HC_Insert_Marker(points[o2].x, points[o2].y, points[o2].z);
							HC_Insert_Line(points[o1].x, points[o1].y,points[o1].z, points[o2].x, points[o2].y,
								points[o2].z);
						HC_Close_Segment();
					HC_Close_Segment();
				HC_Close_Segment(); 

 				delete [] oneface;
				delete [] points;
				delete [] faces;
			}
		}
		else
		{
			HC_Open_Segment_By_Key(m_pView->GetSceneKey());
				HC_Open_Segment("edgevertex");
					HC_Flush_Contents(".", "everything");
				HC_Close_Segment();
			HC_Close_Segment(); 
			
		}
		m_pView->Update();	
		
		return HLISTENER_CONSUME_EVENT;
    }
    else
    {
		if (m_bFound)
		{    
			HC_Open_Segment_By_Key(m_pView->GetSceneKey());
				HC_Open_Segment("edgevertex");
					HC_Flush_Contents(".", "everything");
				HC_Close_Segment();
			HC_Close_Segment(); 
			m_pView->Update();
			m_bFound = false;
			return HLISTENER_CONSUME_EVENT;
		}
    }
    return HLISTENER_PASS_EVENT;
}


int HEdgeVertexSelectionObject::OnLButtonUp(HEventInfo &event)
{
   
 
    if (m_bFound && event.Shift()  && event.Control() && m_blButtonDown)
    {
		m_blButtonDown = false;
		HC_KEY startkey = HObjectManager::FindHObjectSegment(m_SegKey);
		HC_Open_Segment_By_Key(startkey);  
		HUtilityGeomHandle::ClearAllHandles(0);
		HC_Close_Segment();

		HPoint midpoint;
		midpoint.Set((m_Point2.x + m_Point1.x)/2.0f,(m_Point2.y + m_Point1.y)/2.0f,(m_Point2.z + m_Point1.z)/2.0f );

		HC_Open_Segment_By_Key(HUtilityGeomHandle::SetupManipulator(m_SegKey, m_pView, &midpoint, false, false, false, true, false, true));
//       
//
//     HC_Open_Segment_By_Key(startkey);  
//     HUtilityGeomHandle::ClearAllHandles(0);
//       HC_KEY key = HUtility::CreateScaleIndependentSegment("cpbox"); 
//       HC_Open_Segment_By_Key(HUtility::GetScaleIndependentGeomSegmentKey(key));
//  
		HUtilityGeomHandle ghandle;

		HPoint ppp,ppp2, sf;

		midpoint.Set((m_Point2.x + m_Point1.x)/2.0f,(m_Point2.y + m_Point1.y)/2.0f,(m_Point2.z + m_Point1.z)/2.0f );

		HPoint delta(m_Point2 - m_Point1);
		HC_Compute_Normalized_Vector(&delta, &delta);	
		HC_Open_Segment("lines");
		HC_Insert_Line(-delta.x * 0.05, -delta.y * 0.05, -delta.z * 0.05, delta.x * 0.05, delta.y * 0.05, delta.z * 0.05);
		HC_Set_Line_Weight(3.0f);
		HC_Set_Color("lines = red");
		HC_Set_Rendering_Options("depth range=(0,1.0)");
		HC_Set_Visibility("lines = on");
	
		HC_Close_Segment();

		ppp.Set(delta.x*0.005f,delta.y*0.005f,delta.z*0.005f);
		HC_KEY hkey = ghandle.Insert(ppp, false);
		HPoint r1,r2;
		r1.Set(delta.x*0.005f,delta.y*0.005f,delta.z*0.005f);
		r2.Set(delta.x,delta.y,delta.z);
		ghandle.ConstrainToRotationVector(hkey, r1,r2);

 
//	HC_Set_Color("faces = blue");
//	HUtility::SetScaleIndependentGeomPosition(key, m_Point1.x,m_Point1.y,m_Point1.z);
//   	HC_Set_Rendering_Options("depth range=(0,0.1)");

		HC_Open_Segment_By_Key(m_pView->GetSceneKey());
		HC_Open_Segment("edgevertex");
		HC_Flush_Contents(".", "everything");
		HC_Close_Segment();
		HC_Close_Segment(); 


		HC_Close_Segment();
		HUtility::CloseAllSegments();
		m_pView->Update();	
		m_bFound = false;
		return HLISTENER_CONSUME_EVENT;
    }

	m_blButtonDown = false;

	return HLISTENER_PASS_EVENT;
}


void HEdgeVertexSelectionObject::IncludeEntity(HBaseView *view, HC_KEY movekey)
{

	char color[MVO_BUFFER_SIZE];
	char segmentname[MVO_SEGMENT_PATHNAME_BUFFER];
	float matrix[16];
	float identmatrix[16];

	HC_Open_Segment_By_Key(view->GetSceneKey());
	HC_Open_Segment("edgevertex");
	HC_KEY tempkey = HC_KCreate_Segment("osel_temp");
	HC_Close_Segment();
	HC_Close_Segment();

	
	HC_KEY ownerkey = HC_KShow_Owner_By_Key(movekey);
	HC_Open_Segment_By_Key(ownerkey);
	HC_Show_Segment(movekey, segmentname);
	HC_Show_Net_Color(color);
	//    HC_Show_Net_Visibility(visibility);
	HC_Show_Net_Modelling_Matrix(matrix);
	HC_Close_Segment();

	HC_Open_Segment_By_Key(tempkey);
	HC_Flush_Contents(".", "everything");
	HC_Set_Visibility("faces = off, edges = (perimeters=on), lines = off, markers = off");
	HC_Set_Color("edges = black");
	HC_Set_Rendering_Options("attribute lock = color");
	HC_Set_Edge_Weight(1.0f);
	HC_Set_Selectability("everything = off");
	HC_Compute_Identity_Matrix(identmatrix); 
	HC_Set_Modelling_Matrix(identmatrix);
	HC_Open_Segment("tempdata");
	HC_Open_Segment("");
	HC_Set_Modelling_Matrix(matrix);
	HC_Open_Segment("temp");
	HC_Include_Segment_By_Key(movekey);
	HC_Close_Segment();
	HC_Open_Segment("temp2");
	HC_Include_Segment_By_Key(movekey);
	HC_Set_Edge_Pattern("- -");
	HC_Set_Rendering_Options("depth range=(0,0.1)");    

	HC_Close_Segment();

	HUtility::CloseAllSegments(tempkey);
}



const char * HInfoWindowObject::GetStaticName() { return "info_window"; }
const char * HInfoWindowObject::GetName() { return GetStaticName(); }

HInfoWindowObject::HInfoWindowObject(HBaseView *view, float startposx, float startposy, int maxlines, int displaylength) : HObject()
{
	m_pView = view;
	HC_Open_Segment_By_Key(m_pView->GetViewKey());
	m_InfoWindowKey =  HC_KOpen_Segment("");
		HC_Set_Heuristics("quick moves");
	 	HC_Set_Handedness("left");
		HC_Set_Line_Weight(1);
		HC_Set_Edge_Weight(1);
	  	HC_Set_Rendering_Options("depth range=(0,0.1)");
		HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
		HC_Set_Text_Font("name = typewriter, size = 12pt");
		HC_Set_Text_Alignment("<v");
		HC_Set_Color("text = grey");
 	HC_Close_Segment();
	HC_Close_Segment();

    m_InfoWindowTimer = new HTClient( 0.2f, HTCS_PeriodicSkip, InfowWindowTimerClbk, this );
    HDB::GetHTManager()->RegisterClient( m_InfoWindowTimer );

	m_LineNum = 0;
	m_LineMax = maxlines;
	m_StartPosX = startposx;
	m_StartPosY = startposy;
	m_bEraseAfterDisplay = false;
	m_TempDisplayLength = displaylength;
	m_DefaultDisplayLength = displaylength;
 	for (int i=0;i<m_LineMax;i++)
	{
	
		m_InfoText[i] = 0;
		m_InfoTextFont[i] = 0;
		m_InfoTextColor[i] = 0;
	}

	HC_Show_Time(&m_TimeStamp);
	m_pView->SetSignalNotify( HSignalResize, (HSignalNotifyFunction)HInfoWindowObject::WindowResizeCallback, (void*)this);

}


void
HInfoWindowObject::WindowResizeCallback(int signal, void * signal_data, void * user_data)
{
	UNREFERENCED(signal);
	UNREFERENCED(signal_data);

	HInfoWindowObject * iw = (HInfoWindowObject*)user_data;
	if (!iw->m_pView->GetSuppressUpdate())
		iw->UpdateInfoBox();
	
}

void HInfoWindowObject::Flush()
{
	for (int i=0;i<m_LineMax;i++)
	{
		if (m_InfoText[i])
			delete [] m_InfoText[i];
		m_InfoText[i] = 0;
		if (m_InfoTextFont[i])
			delete [] m_InfoTextFont[i];
		m_InfoTextFont[i] = 0;
		if (m_InfoTextColor[i])
			delete [] m_InfoTextColor[i];
		m_InfoTextColor[i] = 0;
	}
	HC_Show_Time(&m_TimeStamp);
	UpdateInfoBox();

}

void HInfoWindowObject::FlushInternal()
{
	for (int i=0;i<m_LineMax;i++)
	{
		if (m_InfoText[i])
			delete [] m_InfoText[i];
		m_InfoText[i] = 0;
		if (m_InfoTextFont[i])
			delete [] m_InfoTextFont[i];
		m_InfoTextFont[i] = 0;
		if (m_InfoTextColor[i])
			delete [] m_InfoTextColor[i];
		m_InfoTextColor[i] = 0;
	}
}
void HInfoWindowObject::DoTimer()
{
	float currenttime;
	HC_Show_Time(&currenttime);


	if ((currenttime - m_TimeStamp) > m_TempDisplayLength)
	{	
		m_TempDisplayLength = m_DefaultDisplayLength;
		if (m_bEraseAfterDisplay)
		{
			Flush();
			m_bEraseAfterDisplay = false;
		}
 		HC_Open_Segment_By_Key(m_InfoWindowKey);
			if (HC_Show_Existence("geometry, segments"))
			{
				HC_Flush_Contents(".", "geometry, segments");
				m_pView->Update();
			}
		HC_Close_Segment();
		HC_Show_Time(&m_TimeStamp);
	}

}
// timer callback to say walk further
bool HInfoWindowObject::InfowWindowTimerClbk(float request_time, float actual_time, void * user_data)
{
	UNREFERENCED(request_time);
	UNREFERENCED(actual_time);
	
	HInfoWindowObject* op = (HInfoWindowObject*) user_data;
	op->DoTimer();
	return true;
}

void HInfoWindowObject::UpdateInfoBox()
{
	HC_Open_Segment_By_Key(m_InfoWindowKey);
	HC_Flush_Contents(".", "geometry, segments");

	char totaltext[20000];
	float xfrac, yfrac;
 	float pos = m_StartPosY;
	
	strcpy(totaltext,"");
	HC_Compute_Text_Extent(".", "X", &xfrac, &yfrac);

	for (int i=0;i<m_LineMax;i++)
	{
 
 			HC_Open_Segment("");
			if (m_InfoTextFont[i])
					HC_Set_Text_Font(m_InfoTextFont[i]);			
			if (m_InfoTextColor[i])
				HC_Set_Color(m_InfoTextColor[i]);
 			pos+=(yfrac+0.01f);

			if (m_InfoText[i])
			{
 				HC_Insert_Text(m_StartPosX, pos,0, m_InfoText[i]);
			}
			HC_Compute_Text_Extent(".", "X", &xfrac, &yfrac);

			HC_Close_Segment();
	}
//	HC_Insert_Text(m_StartPosX, m_StartPosY, 0, totaltext);

	HC_Close_Segment();

	m_pView->Update();
	

}
void HInfoWindowObject::AddLine(char const *text, char const *font, char const *color, bool EraseAfterDisplay, int DisplayLength)
{
	if (m_bEraseAfterDisplay)
		Flush();
	if (m_InfoText[m_LineMax-1])
	{
		delete [] m_InfoText[m_LineMax-1];
		if (m_InfoTextColor[m_LineMax-1])
		{
			delete [] m_InfoTextColor[m_LineMax-1];
			m_InfoTextColor[m_LineMax -1] = 0;
		}
		if (m_InfoTextFont[m_LineMax-1])
		{
			delete [] m_InfoTextFont[m_LineMax-1];
			m_InfoTextFont[m_LineMax -1] = 0;
		}
	}

	for (int i=m_LineMax-1;i>0;i--)
	{
		m_InfoText[i] = m_InfoText[i-1];
		m_InfoTextFont[i] = m_InfoTextFont[i-1];
		m_InfoTextColor[i] = m_InfoTextColor[i-1];
	}

	m_InfoText[0] = new char[strlen(text)+1];
	strcpy(m_InfoText[0], text);
	if (font)
	{	
		m_InfoTextFont[0] = new char[strlen(font)+1];
		strcpy(m_InfoTextFont[0], font);
	}
	else
		m_InfoTextFont[0] = 0;
	if (color)
	{	
		char t[4096];
		sprintf(t, "text = %s", color);
		m_InfoTextColor[0] = new char[strlen(t)+1];
		strcpy(m_InfoTextColor[0], t);
	}
	else
		m_InfoTextColor[0] = 0;
	HC_Show_Time(&m_TimeStamp);

	UpdateInfoBox();
	m_bEraseAfterDisplay = EraseAfterDisplay;
	if (DisplayLength > 0)
		m_TempDisplayLength = DisplayLength;
	else
		m_TempDisplayLength = m_DefaultDisplayLength;
}

 
HInfoWindowObject::~HInfoWindowObject()
{
	if (m_InfoWindowTimer)
			HDB::GetHTManager()->UnRegisterClient( m_InfoWindowTimer ); 

	delete m_InfoWindowTimer;

	FlushInternal();
	

}




const char * HImagePanelManagerObject::GetStaticName() { return "imagepanel"; }
const char * HImagePanelManagerObject::GetName() { return GetStaticName(); }


HImagePanelManagerObject::HImagePanelManagerObject() : HObject()
{
	m_ImagePanelList = new_vlist(malloc, free);

}
HImagePanelManagerObject::~HImagePanelManagerObject()
{
	START_LIST_ITERATION(HImagePanel, m_ImagePanelList);{
		delete temp;	
	}END_LIST_ITERATION(m_ImagePanelList);
	delete_vlist(m_ImagePanelList);	

}

void HImagePanelManagerObject::AddImagePanel(HImagePanel *panel)
{
	vlist_add_last(m_ImagePanelList, panel);
}



bool HImagePanelManagerObject::DeleteImagePanel(char const *name, bool doDelete, HImagePanel **ip)
{
	bool deleted = false;

start_iterate:
	START_LIST_ITERATION(HImagePanel, m_ImagePanelList);{
		if (strcmp(temp->GetPanelName(), name) == 0)
		{		
			vlist_remove(m_ImagePanelList, temp);
			if (doDelete)
				delete temp;	
			deleted = true;
			if (doDelete)
				goto start_iterate;
			else
			{
				if (ip)
					*ip = temp;
				break;
			}

		}
	}END_LIST_ITERATION(m_ImagePanelList);
 	return deleted;
}


bool HImagePanelManagerObject::DeleteImagePanel(HImagePanel *ip, bool doDelete)
{
	int res = false;
 	if ((res = vlist_remove(m_ImagePanelList, ip)) != 0 && doDelete)
		delete ip;

	if (res)
		return true;
	else
		return false;
}


		
HImagePanel * HImagePanelManagerObject:: GetImagePanelByName(char const *name)
{
	START_LIST_ITERATION(HImagePanel, m_ImagePanelList);{
		if (strcmp(temp->GetPanelName(), name) == 0)				
			return temp;
	}END_LIST_ITERATION(m_ImagePanelList);
	return 0;
}

void HImagePanel::SetPosition(float x, float y, float z)
{
	HC_Open_Segment_By_Key(m_Key);
		HC_UnSet_Modelling_Matrix();
		HC_Translate_Object(x,y,z);
	HC_Close_Segment();
}

HImagePanel::HImagePanel(HBaseView *view, HC_KEY start_key, int width, int height, float xpos, float ypos,char const * const name) : HBaseOperator(view,0,0)
{
	if (start_key == -1)
		m_Key = HC_KCreate_Segment("");
	else
		m_Key = start_key;
	m_ImageRegionList = new_vlist(malloc, free);
	

	m_NormalImageKey = 0;
	m_HighlightedImageKey = 0;
	m_RegionImageKey = 0;


	m_ImageAction = 0;
	m_ImageActionData = 0;
	m_width = width;
	m_height = height;
	HC_Open_Segment_By_Key(m_Key);
	HC_Set_Rendering_Options("no display lists");
	HC_Set_Heuristics("quick moves = spriting");
	HC_Set_Rendering_Options("no frame buffer effects");
  	HC_Set_Rendering_Options("depth range=(0,0.1)");

	HC_Set_Visibility("images = on");
	char *temp = new char[width*height*4];
	memset(temp,0,width*height*4);
	m_WorkerImageKey = HC_KInsert_Image(0,0,0,"rgba",width,height, temp);
	delete [] temp;
	HC_Set_Camera_By_Volume ("stretched", -1.0, 1.0, -1.0, 1.0);
	HC_Translate_Object(xpos,ypos,0);
	HC_Close_Segment();
	m_pView = view;
	view->GetEventManager()->RegisterHandler((HMouseListener *)this, HMouseListener::GetType(), HLISTENER_PRIORITY_HIGH + 20000);
	m_Name = new char[strlen(name)+1];
	strcpy(m_Name, name);
	m_pButtonTimer = 0;
}



void HImagePanel::SetImageFromFile(char const *path, HC_KEY &image_key)
{
	HC_Open_Segment("");
  	HC_Read_Metafile(path, ".", "");
	HC_Begin_Contents_Search(".", "images");
	HUtility::FindContents(image_key);
	HC_End_Contents_Search();
	HC_Close_Segment();
 
}

void HImagePanel::SetImages(HC_KEY NormalImageKey, HC_KEY HighlightedImageKey, HC_KEY RegionImageKey)
{
	m_NormalImageKey = NormalImageKey;
	m_HighlightedImageKey = HighlightedImageKey;
	m_RegionImageKey = RegionImageKey;
	ComposeWorkerImage(0);
}


void HImagePanel::SetImagesFromFile(char const *normal_image_path, char const *highlighted_image_path, char const *region_image_path)
{

	char path[4096];
	char text[4096];
	HC_Show_One_System_Option("metafile directory", path);
	HC_Define_System_Options("metafile directory = .");



	HC_Open_Segment_By_Key(m_Key);
	HC_Open_Segment("images");
	HC_Set_Visibility("images = off");
	SetImageFromFile(normal_image_path, m_NormalImageKey);
	SetImageFromFile(highlighted_image_path, m_HighlightedImageKey);
	SetImageFromFile(region_image_path, m_RegionImageKey);	
	sprintf(text, "metafile directory = %s", path);
	HC_Define_System_Options(text);
	ComposeWorkerImage(0);
	HC_Close_Segment();
	HC_Close_Segment();
	ZoomIn();
}

void HImagePanel::ZoomIn()
{
	char text[MVO_BUFFER_SIZE];
	float starttime, currenttime;
	float scaletime = 0.7f;
	HC_Show_Time(&starttime);
	HC_Open_Segment_By_Key(m_Key);
	while (1)		
	{
		HC_Show_Time(&currenttime);
		float x = (currenttime-starttime)/scaletime;
		if (x>=1.0f)
			break;
		sprintf(text,"image scale = %f", x);
		HC_Set_Rendering_Options(text);
		m_pView->ForceUpdate();
		
	}
 	HC_Set_Rendering_Options("image scale = 1.0");
	m_pView->ForceUpdate();
	HC_UnSet_One_Rendering_Option("image scale");
	HC_Close_Segment();

		
}
void HImagePanel::ZoomOut()
{
	char text[MVO_BUFFER_SIZE];
	float starttime, currenttime;
	float scaletime = 0.7f;
	HC_Show_Time(&starttime);
	HC_Open_Segment_By_Key(m_Key);
	while (1)		
	{
		HC_Show_Time(&currenttime);
		float x = 1.0f - (currenttime-starttime)/scaletime;
		if (x<=0.0f)
			break;
		sprintf(text,"image scale = %f", x);
		HC_Set_Rendering_Options(text);
		m_pView->ForceUpdate();
		
	}
 	HC_Set_Rendering_Options("image scale = 0.0");
	m_pView->ForceUpdate();
	HC_UnSet_One_Rendering_Option("image scale");
	HC_Close_Segment();

		
}

void HImagePanel::AddImageRegion(int identifier, char const * const name)
{
	HImageRegion *region = new HImageRegion(identifier, name);
	vlist_add_last(m_ImageRegionList, region);
}

HImagePanel::~HImagePanel()
{
 	if (m_pButtonTimer)
	{
		HDB::GetHTManager()->UnRegisterClient( m_pButtonTimer );
		delete m_pButtonTimer;
		m_pButtonTimer = 0;
	}

    m_pView->GetEventManager()->UnRegisterHandler((HMouseListener *)this, HMouseListener::GetType());    
	START_LIST_ITERATION(HImageRegion, m_ImageRegionList);{
		delete temp;
	}END_LIST_ITERATION(m_ImageRegionList);
	delete_vlist(m_ImageRegionList);	

	HC_Delete_By_Key(m_Key);
	delete [] m_Name;
}






HImageRegion::HImageRegion(int identifier, char const *image_name)
{
 	m_Name = 0;
	if (image_name)
	{
		m_Name = new char[strlen(image_name)+1];
		strcpy(m_Name, image_name);
	}
	m_Id = identifier;
}

HImageRegion::~HImageRegion()
{
	delete [] m_Name;
}

bool HImageRegion::IsSelected(float xpos, float ypos, HC_KEY region_image_key, int width, int height)
{
	bool res = false;
	HPoint p1(xpos,ypos,0), p2,p3;
	HC_KEY segkey = HC_KShow_Owner_By_Key(region_image_key);
	HC_Open_Segment_By_Key(segkey);
	HC_Compute_Coordinates(".", "local window", &p1, "local pixels", &p2);
	p1.Set(0,0,0);
	HC_Compute_Coordinates(".", "object", &p1, "local pixels", &p3);
	HC_Close_Segment();
	p3.Set(p3.x - width/2, p3.y - height/2,0);
	p2.Set(p2.x - p3.x, p2.y - p3.y,0);

	char format[4096];
	int w,h;
	float x,y,z;
	HPixelRGBA *timage = new HPixelRGBA[width * height];
	HC_Show_Image(region_image_key, &x, &y, &z, format, &w,&h, timage);
	if (timage[(int)p2.x + (int)p2.y * width].r() == m_Id && timage[(int)p2.x + (int)p2.y * width].a() != 0)
		res = true;
	delete [] timage;
	return res;
}



bool HImagePanel::ButtonTimerClbk(float request_time, float actual_time, void * user_data)
{
	UNREFERENCED(request_time);
	UNREFERENCED(actual_time);

	HImagePanel* op = (HImagePanel*) user_data;
	if (op->m_CurrentImage && op->m_ImageAction)
		op->m_ImageAction(op->m_CurrentImage, op->m_ImageActionData, ButtonPressed);

	return true;
}

int HImagePanel::OnLButtonDown(HEventInfo &event)
{
	
	char path[MVO_BUFFER_SIZE];
	m_ptLast = event.GetMouseWindowPos();
	HC_Show_Segment(m_Key, path);
	HC_Open_Segment_By_Key(m_pView->GetViewKey());
	HC_Set_Selectability("images = on");

	int res = HC_Compute_Selection(".",path,"v, selection level = entity", m_ptLast.x,  m_ptLast.y);

	if (res)
	{
		HC_KEY key;
		int o1,o2,o3;
		HC_Show_Selection_Element(&key, &o1, &o2, &o3);
		m_CurrentImage = FindSelectedRegion(m_ptLast.x, m_ptLast.y);
		SetOperatorStarted(true);
	}
	HC_Close_Segment();

	if (res)
	{
 		int action_res = true;
 		if (m_ImageAction && m_CurrentImage)		
			action_res = m_ImageAction(m_CurrentImage, m_ImageActionData, ButtonDown);
		if (!action_res)
		{		
			ZoomOut();
 			HImagePanelManagerObject *ipo =   (HImagePanelManagerObject *) m_pView->GetHObjectManager()->GetHObject("imagepanel");
			ipo->DeleteImagePanel(m_Name, true);			
			
  			return HLISTENER_CONSUME_EVENT;
		}
		else
		{					
			m_pButtonTimer = new HTClient( 0.005f, HTCS_PeriodicSkip, ButtonTimerClbk, this );
			HDB::GetHTManager()->RegisterClient( m_pButtonTimer );
		}

		if (!m_CurrentImage)
		{
			HC_Open_Segment_By_Key(m_Key);
			HC_Set_Rendering_Options("image tint = light grey");
			HC_Close_Segment();
		}
		m_pView->Update();
		return HLISTENER_CONSUME_EVENT;
	}
	else
		return HLISTENER_PASS_EVENT;

}

void HImagePanel::ComposeWorkerImage(HImageRegion *reg)
{
	float x,y,z;
	int w,h;
	char format[4096];
	int id = 128;
	if (reg)
		id = reg->GetID();
	HPixelRGBA *px = new HPixelRGBA[m_width * m_height];
	HC_Show_Image(m_NormalImageKey, &x,&y,&z, format, &w,&h,px);
	HPixelRGBA *px2 = new HPixelRGBA[m_width * m_height];
	HC_Show_Image(m_RegionImageKey, &x,&y,&z, format, &w,&h,px2);
	HPixelRGBA *px3 = new HPixelRGBA[m_width * m_height];
	HC_Show_Image(m_HighlightedImageKey, &x,&y,&z, format, &w,&h,px3);
	HPixelRGBA *px4 = new HPixelRGBA[m_width * m_height];
 	for (int i=0;i<m_width * m_height;i++)
	{
		if (px2[i].r()!=id || px3[i].a() ==0 || px2[i].a() == 0)
			px4[i] = px[i];
		else 
			px4[i] = px3[i];
	}
	HC_Edit_Image(m_WorkerImageKey, 0,0,m_width, m_height, px4);
	delete [] px;
	delete [] px2;
	delete [] px3;
	delete [] px4;

}

HImageRegion * HImagePanel::FindSelectedRegion(float xpos, float ypos)
{
	HImageRegion *ii = 0;
	START_LIST_ITERATION(HImageRegion, m_ImageRegionList);{
		bool res = temp->IsSelected(xpos, ypos,m_RegionImageKey, m_width, m_height);
		if (res)
		{
			ii = temp;
			ComposeWorkerImage(temp);
			break;
		}
	}END_LIST_ITERATION(m_ImageRegionList);
	m_pView->ForceUpdate();
	return ii;

}
int HImagePanel::OnLButtonUp(HEventInfo &event)
{
	if (OperatorStarted())
	{	
		if (!m_CurrentImage)
		{
			HC_Open_Segment_By_Key(m_Key);
			HC_UnSet_One_Rendering_Option("image tint");
			HC_Close_Segment();
		}
		SetOperatorStarted(false);

		HDB::GetHTManager()->UnRegisterClient( m_pButtonTimer );
		delete m_pButtonTimer;
		m_pButtonTimer = 0;

	ComposeWorkerImage(0);
	m_pView->Update();
		return HLISTENER_CONSUME_EVENT;
	}
	else
		return HBaseOperator::OnLButtonUp(event);

}
int HImagePanel::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);
	if (!m_CurrentImage)
	{	
		SetNewPoint(event.GetMouseWindowPos());
		HC_Open_Segment_By_Key(m_Key);{
			HPoint delta(GetNewPoint() - m_ptLast);
			HC_Translate_Object(delta.x, delta.y, delta.z);
		}HC_Close_Segment(); 

		m_ptLast = GetNewPoint();
		m_pView->Update();
	}
    return HLISTENER_CONSUME_EVENT;
}
