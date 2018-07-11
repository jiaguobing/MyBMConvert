// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraOrbit.cpp : implementation of the HOpCameraOrbit class
//


#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraOrbit.h"
#include "HUtility.h"
#include "HEventListener.h"
#include "HEventManager.h"
#include "HBhvBehaviorManager.h"



/////////////////////////////////////////////////////////////////////////////
// HOpCameraOrbit
//
// Operator for orbiting the scene's camera based on the user dragging the mouse
// with the left button down
//
// Left button down - find first position and bounds object with box
//
// Mouse motion while down - orbits camera based on mouse movement by mapping 
// the drag vector onto a virtual sphere and determine the corresponding orbit
// parameters
//
// Mouse up - removes object bounding box

HOpCameraOrbit::HOpCameraOrbit(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_bLightFollowsCamera = false;
	m_bSceneBoundingBoxVis = false;
	m_Angle1 = m_Angle2 = m_Angle3 = 0.0;
	m_bSingleClick = false;
}

HOpCameraOrbit::~HOpCameraOrbit()
{
}


HBaseOperator * HOpCameraOrbit::Clone()
{
	return new HOpCameraOrbit(GetView());
}


 

const char * HOpCameraOrbit::GetName() { return "HOpCameraOrbit"; }


/////////////////////////////////////////////////////////////////////////////
// HOpCameraOrbit message handlers


int HOpCameraOrbit::OnLButtonDown(HEventInfo &event)
{
	float tmp,vl;
	HPoint min, max;

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;


	if (!OperatorStarted()) 
		SetOperatorStarted(true);
	
	m_bSingleClick = true;
	if (m_bSceneBoundingBoxVis)
	{
	    // draw a bounding box

		HC_Open_Segment_By_Key(GetView()->GetModel()->GetModelKey());
		  HC_Compute_Circumcuboid (".", &min, &max);	
		HC_Close_Segment();
	  
		HC_Open_Segment_By_Key (GetView()->GetBBoxGeometryKey());
		  HC_Flush_Geometry (".");
		  HUtility::InsertBox (&max, &min);
		HC_Close_Segment ();
		     
		GetView()->Update();
	}
	
	// read mouse position
	SetFirstPoint(event.GetMouseWindowPos());

	// this is simply used to determine whether or not the mouse
	// moved buring it's interaction
	m_ptRealFirst = event.GetMouseWorldPos();
	
	// remember screen mouse position
	m_ptRealOld.x = GetFirstPoint().x;
	m_ptRealOld.y = GetFirstPoint().y;
	
	// map screen mouse points to sphere mouse points
	tmp = (GetFirstPoint().x * GetFirstPoint().x + GetFirstPoint().y * GetFirstPoint().y);
	vl = (float)sqrt(tmp);

	HPoint point = GetFirstPoint();
	if (vl > 1.0f) 
	{
		point.x /= vl;
		point.y /= vl;
		point.z = 0.0;
	}
	else {
		point.z = (float) sqrt (1.0f - tmp);
	}
	SetFirstPoint(point);

	GetView()->PrepareForCameraChange();

	return HOP_OK;

}



int HOpCameraOrbit::OnLButtonDownAndMove(HEventInfo &event)
{
	HPoint first_point, new_point, axis, vtmp, m_real_new;	
	float theta, dist, tmp, vl;

	if (!OperatorStarted()) return HBaseOperator::OnLButtonDownAndMove(event);

	m_bSingleClick = false;
	GetView()->SetViewMode(HViewUnknown);

	// read mouse position
	SetNewPoint(event.GetMouseWindowPos());
	
	// remember the real mouse positions
	m_real_new.x = GetNewPoint().x;
	m_real_new.y = GetNewPoint().y;

	// map screen mouse points to sphere mouse points
	tmp = GetNewPoint().x * GetNewPoint().x + GetNewPoint().y * GetNewPoint().y;
	vl = (float)sqrt(tmp);

	new_point = GetNewPoint();
	if (vl > 1.0f) 
	{
		new_point.x /= vl;
		new_point.y /= vl;
		new_point.z = 0.0;
	}
	else {
		new_point.z = (float) sqrt (1.0f - tmp);
	}
	SetNewPoint(new_point);

	// get the axis of rotation
	first_point = GetFirstPoint();
	HC_Compute_Cross_Product (&first_point, &new_point, &axis);
	
	// this is for screen mouse based movement
	vtmp.x = m_real_new.x - m_ptRealOld.x;
	vtmp.y = m_real_new.y - m_ptRealOld.y;
	dist = (float)sqrt(vtmp.x * vtmp.x + vtmp.y * vtmp.y) * 90.0f;
	
	if ((axis.x != 0.0 || axis.y != 0.0 || axis.z != 0)){

		if (GetView()->GetHandedness() == HandednessRight)
		{
			axis.y *= -1;
			axis.z *= -1;
		}

		HC_Compute_Normalized_Vector(&axis, &axis);

		HC_Open_Segment_By_Key(GetView()->GetSceneKey());
	
			// project axis of rotation onto yz plane 
			vtmp.x = 0.0;
			vtmp.y = axis.y;
			vtmp.z = axis.z;

			// calculate angle of x orbit
			tmp = (float)HC_Compute_Dot_Product(&axis, &vtmp);
			if(fabs(tmp) > 1.001f || fabs(tmp) < 0.999f)		
				theta = (float)H_ACOS(tmp);
			else
				theta = 0.0f;

		
			if (axis.x < 0.0)
				m_Angle2= -theta * dist;
			else
				m_Angle2=theta * dist;
			
			// project axis of rotation onto xz plane 
			vtmp.x = axis.x;
			vtmp.y = 0.0;
			vtmp.z = axis.z;

			// calculate angle of y orbit
			tmp = (float)HC_Compute_Dot_Product(&axis, &vtmp);
			if (fabs(tmp) > 1.001f || fabs(tmp) < 0.999f)		
				theta = (float)H_ACOS(tmp);
			else
				theta = 0.0f;
			
			if (axis.y < 0.0)
				m_Angle1=theta * dist;
			else
				m_Angle1=-theta * dist;
		
			// project axis of rotation onto xy plane 
			vtmp.x = axis.x;
			vtmp.y = axis.y;
			vtmp.z = 0.0;

			// calculate angle of z orbit
			tmp = (float)HC_Compute_Dot_Product(&axis, &vtmp);
			if(fabs(tmp) > 1.001f || fabs(tmp) < 0.999f)		
				theta = (float)H_ACOS(tmp);
			else
				theta = 0.0f;

			if (axis.z < 0.0)
				m_Angle3=theta * dist;
			else
				m_Angle3=-theta * dist;

 			HC_Orbit_Camera(m_Angle1,0);
 			HC_Orbit_Camera(0,m_Angle2);
			HC_Roll_Camera(m_Angle3);

 		HC_Close_Segment();

		// update default light
		GetView()->CameraPositionChanged();

	}

	// update sphere space mouse
	SetFirstPoint(GetNewPoint());

	// update screen space mouse
	m_ptRealOld.x = m_real_new.x;
	m_ptRealOld.y = m_real_new.y;
	
	GetView()->Update();
	return HOP_OK;

 }

int HOpCameraOrbit::OnLButtonUp(HEventInfo &event)
{

	HPoint lastPoint;
	lastPoint = event.GetMouseWorldPos();

	if (m_bSceneBoundingBoxVis)
	{
		HC_Open_Segment_By_Key (GetView()->GetBBoxGeometryKey());  
		  HC_Flush_Geometry(".");
		HC_Close_Segment();

		GetView()->Update();
	}
 
    SetOperatorStarted(false); 
	if(!OperatorStarted() || HUtility::PointsEqual(&lastPoint, &m_ptRealFirst))
	    return HBaseOperator::OnLButtonDownAndMove(event);

     GetView()->CameraPositionChanged(true, true);
	return HOP_OK;

}

