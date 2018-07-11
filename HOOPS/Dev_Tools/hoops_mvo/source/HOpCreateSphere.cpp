// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#include <math.h>
#include <stdlib.h>
#include <string.h>
 
#include "HTools.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HOpCreateSphere.h"
#include "HUtilityGeometryCreation.h"


/////////////////////////////////////////////////////////////////////////////
// HOpCreateSphere
//
// Operator for inserting a sphere into a newly created Scene/Part segment.
// Mouse down - fix first position
// Mouse motion while down - rubberband sphere with radius defined by first point and
// current position in world space in the scene/construction segment 
// Mouse up inserts the sphere

HOpCreateSphere::HOpCreateSphere(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructSphere(view, DoRepeat, DoCapture)
{
}

HOpCreateSphere::~HOpCreateSphere()
{
}

HBaseOperator * HOpCreateSphere::Clone()
{
	return new HOpCreateSphere(GetView());
}


const char * HOpCreateSphere::GetName() { return "HOpCreateSphere"; }

/////////////////////////////////////////////////////////////////////////////
// HOpCreateSphere message handlers




int HOpCreateSphere::OnLButtonUp(HEventInfo &event)
{
    // Mouse has come up 
	if(!OperatorStarted()) 
		return HOpConstructSphere::OnLButtonDownAndMove(event);

	SetNewPoint(event.GetMouseViewpointPos());

    HC_Open_Segment_By_Key(GetView()->GetSceneKey());	

		HVector pos, tar, up;		
 		HC_Show_Net_Camera_Position(&pos.x, &pos.y, &pos.z);
 		HC_Show_Net_Camera_Target(&tar.x, &tar.y, &tar.z);

		HPoint view;
		view.Set(pos.x-tar.x, pos.y-tar.y, pos.z-tar.z);
		float length = (float)HC_Compute_Vector_Length(&view);
		HPoint tmp = GetFirstPoint();
		tmp.z = length;
		SetFirstPoint(tmp);
		tmp = GetNewPoint();
		tmp.z = length;
		SetNewPoint(tmp);

		HC_Compute_Normalized_Vector(&view, &view);
 		HC_Show_Net_Camera_Up_Vector(&up.x, &up.y, &up.z);
		HC_Compute_Normalized_Vector(&up, &up);

    HC_Close_Segment();

	// do the following woprk to make sure that the object is drawn in
	// the view plane correctly
	HC_KEY keys[2];
	keys[0] = GetView()->GetModel()->GetModelKey();
	keys[1] = GetView()->GetSceneKey();

	HPoint point = GetFirstPoint();
	HC_Compute_Coordinates_By_Path(2, keys, "viewpoint",&point,"object",&point);
	SetFirstPoint(point);
	point = GetNewPoint();
	HC_Compute_Coordinates_By_Path(2, keys, "viewpoint",&point,"object",&point);
	SetNewPoint(point);

	HVector radius_vector = GetNewPoint() - GetFirstPoint();
#if 0
	radius_vector.Set (GetNewPoint().x - GetFirstPoint().x,
						 GetNewPoint().y - GetFirstPoint().y,
						 GetNewPoint().z - GetFirstPoint().z);
#endif
	m_radius = (float)HC_Compute_Vector_Length(&radius_vector);

	if(m_radius > 0.f)
	{
		HC_Open_Segment_By_Key (GetView()->GetModelKey());
			HC_Open_Segment ("spheres");
				HUtilityGeometryCreation::CreateSphere(GetFirstPoint(), m_radius, 20, view, up);
			HC_Close_Segment();
		HC_Close_Segment();
	}
	
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
	HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();

	GetView()->SetGeometryChanged();
    GetView()->Update(); 

    return HOpConstructSphere::OnLButtonUp(event);
}
