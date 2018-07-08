// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraOrbitTurntable.cpp : implementation of the HOpCameraOrbitTurntable class
//

#include <string.h>
#include <math.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraOrbitTurntable.h"
#include "HBhvBehaviorManager.h"
#include "HEventManager.h"
#include "HEventListener.h"


/////////////////////////////////////////////////////////////////////////////
// HOpCameraOrbitTurntable
//
// Operator for turntable style camera orbit
//
// Left button drag - rotation around the vertical axis
//
// mouse wheel - rotation around the horizontal axis
// 
//



HOpCameraOrbitTurntable::HOpCameraOrbitTurntable(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LightFollowsCamera = false;
	m_bDoOnce = true;
	SetRequestedPriority(HLISTENER_PRIORITY_HIGH + 10000);
	m_bCenterPointSet = false;
}

HOpCameraOrbitTurntable::~HOpCameraOrbitTurntable()
{
}


HBaseOperator * HOpCameraOrbitTurntable::Clone()
{
	return new HOpCameraOrbitTurntable(GetView());
}



const char * HOpCameraOrbitTurntable::GetName() { return "HOpCameraOrbitTurntable"; }



int HOpCameraOrbitTurntable::OnLButtonDown(HEventInfo &event)
{

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;

	if (event.Control())
	{
			HPoint spoint = event.GetMouseWindowPos();
			HC_Open_Segment_By_Key(m_pView->GetViewKey());
		 	int res = HC_Compute_Selection(".","./scene/overwrite","v", spoint.x, spoint.y);
			if (res)
			{
				float dummy;
				HC_Show_Selection_Position(&dummy, &dummy, &dummy, &m_Center.x, &m_Center.y, &m_Center.z);
				m_bCenterPointSet = true;
			}
			HC_Close_Segment();
	}
	else
	{	

			if (!OperatorStarted()) 
				SetOperatorStarted(true);

			SetNewPoint(event.GetMouseWindowPos()); 

			SetFirstPoint(GetNewPoint());

			GetView()->PrepareForCameraChange();
	}
 
    return HOP_OK;
}


 
int HOpCameraOrbitTurntable::OnMouseWheel(HEventInfo &event)
{
    int tick = event.GetMouseWheelDelta();
    if (!OperatorStarted())
    {

	    HC_Open_Segment_By_Key (GetView()->GetSceneKey());	
		HPoint cc,cc2,cc3;
		cc2.Set(1,0,0);
		cc.Set(0,0,0);
		HC_Compute_Coordinates(".", "local window", &cc, "world", &cc);
		HC_Compute_Coordinates(".", "local window", &cc2, "world", &cc2);
		HC_Close_Segment();
		cc3 = cc2 - cc;
		HC_Compute_Normalized_Vector(&cc3, &cc3);
 
		if (tick > 0)
			RotateAroundAxis(cc3,12 * GetView()->GetMouseWheelSensitivity());
		else
			RotateAroundAxis(cc3,-12 * GetView()->GetMouseWheelSensitivity());
		GetView()->Update();
		
    }
	
    return HLISTENER_CONSUME_EVENT;
    
}



void HOpCameraOrbitTurntable::RotateAroundAxis(HPoint ta, float rf)
{
     HC_Open_Segment_By_Key (GetView()->GetSceneKey());
	    HPoint camera, target, upv;
		
		HC_Show_Net_Camera_Target (&target.x, &target.y, &target.z);
		HC_Show_Net_Camera_Position (&camera.x, &camera.y, &camera.z);
		HC_Show_Net_Camera_Up_Vector (&upv.x, &upv.y, &upv.z);
		
		float matrix1[16];
		float matrix2[16];
		float matrix3[16];
		float matrix4[16];

		HPoint delta;
		if (m_bCenterPointSet)		
			delta = m_Center;		
		else
			delta = target;

	
   		HC_Compute_Offaxis_Rotation(ta.x, ta.y, ta.z,rf, matrix1);
		HC_Compute_Translation_Matrix(-delta.x, -delta.y, -delta.z, matrix2);
		HC_Compute_Matrix_Product(matrix2, matrix1, matrix3);
		HC_Compute_Translation_Matrix(delta.x,delta.y, delta.z, matrix2);
		HC_Compute_Matrix_Product(matrix3, matrix2, matrix4);
		HC_Compute_Transformed_Points(1, &camera, matrix4, &camera);
		HC_Compute_Transformed_Points(1, &upv, matrix1, &upv);
 		HC_Set_Camera_Position(camera.x, camera.y, camera.z);
		
		if (m_bCenterPointSet)
			{
  			HC_Compute_Offaxis_Rotation(ta.x, ta.y, ta.z,rf, matrix1);
			HC_Compute_Translation_Matrix(-delta.x, -delta.y, -delta.z, matrix2);
			HC_Compute_Matrix_Product(matrix2, matrix1, matrix3);
			HC_Compute_Translation_Matrix(delta.x,delta.y, delta.z, matrix2);
			HC_Compute_Matrix_Product(matrix3, matrix2, matrix4);
			HC_Compute_Transformed_Points(1, &target, matrix4, &target);
 
 			HC_Set_Camera_Target(target.x, target.y, target.z);
		}


		HC_Set_Camera_Up_Vector(upv.x, upv.y, upv.z);
//debug code
#if 0 	
		HC_Open_Segment("markers");
		HC_Set_Visibility("markers = on");
		HC_Insert_Marker(camera.x, camera.y, camera.z);
		HC_Open_Segment("markers2");
		HC_Set_Color("markers = yellow");
		HC_Insert_Marker(target.x, target.y, target.z);
		HC_Close_Segment();
		HC_Close_Segment();
#endif

 
	HC_Close_Segment();

}

int HOpCameraOrbitTurntable::OnLButtonDownAndMove(HEventInfo &event)
{
	
    if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

    SetNewPoint(event.GetMouseWindowPos());  

	HPoint delta2(GetNewPoint() - GetFirstPoint());


	HVector fa, ta;
	m_pView->GetViewAxis(&fa, &ta);
	RotateAroundAxis(ta, -delta2.x*250.0f);

	GetView()->CameraPositionChanged();
	SetFirstPoint(GetNewPoint());

	GetView()->Update();
	return HOP_OK;
}




int HOpCameraOrbitTurntable::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
	    return HBaseOperator::OnLButtonDownAndMove(event);

 	SetOperatorStarted(false);
	GetView()->CameraPositionChanged(true, true);

    return HOP_READY;
}
