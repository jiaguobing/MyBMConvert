// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCuboid.cpp : implementation of the HOpConstructCuboid class
// 



#include "HTools.h"
#include "HBaseView.h"
#include "HEventInfo.h"
#include "HOpCreateCuboid.h"
#include "HUtility.h"

 
#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "hc.h"


/////////////////////////////////////////////////////////////////////////////
// HOpConstructCuboid
//  
// Operator for inserting a rectangular cube into a newly created Scene/Part segment.
// Mouse down - fix first position
// Mouse motion while down - rubberband square base defined by first point and
// current position in world space in the scene/construction segment 
// (cylinder base is orthogonal to camera target plane)
// Mouse up and motion- cube height defined by base and current position in world space
// (main cube axis is parallel to camera target plane)	


HOpCreateCuboid::HOpCreateCuboid(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructCuboid(view, DoRepeat, DoCapture)
{
}

HOpCreateCuboid::~HOpCreateCuboid()
{
}

HBaseOperator * HOpCreateCuboid::Clone()
{
	return new HOpConstructCuboid(GetView());
}


const char * HOpCreateCuboid::GetName() { return "HOpCreateCuboid"; }

/////////////////////////////////////////////////////////////////////////////
// ConstructCuboid message handlers


int HOpCreateCuboid::OnLButtonDown(HEventInfo &event)
{
	SetNewPoint(event.GetMouseViewpointPos());

	if (OperatorStarted() && !(GetFirstPoint() == GetNewPoint())){
		CreateCuboid(GetNewPoint());

		HC_Open_Segment_By_Key(GetView()->GetModelKey());{
			HC_Open_Segment(GetName());{
				float transform[16];
				HC_Compute_Transform(".", "world", "object", transform);
				HC_Compute_Transformed_Points(8, &m_ptPointList[0].x, transform, &m_ptPointList[0].x);
				HC_Insert_Shell(8, m_ptPointList, 30, m_FaceList);
			}HC_Close_Segment();
		}HC_Close_Segment();
	}

	return HOpConstructCuboid::OnLButtonDown(event);
}
