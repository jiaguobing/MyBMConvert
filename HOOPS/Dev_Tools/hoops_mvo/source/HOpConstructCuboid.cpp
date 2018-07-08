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
#include "HOpConstructCuboid.h"
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


HOpConstructCuboid::HOpConstructCuboid(HBaseView* view, int DoRepeat, int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	int temp[30] = {
		4, 0, 1, 2, 3,  // we know the way the points array is going
		4, 4, 5, 1, 0,  // to be packed so can initialise the face  
		4, 5, 6, 2, 1,  // list here.
		4, 7, 6, 5, 4,
		4, 7, 4, 0, 3,
		4, 6, 7, 3, 2,
	};

	for (int i=0; i<30; i++)
		m_FaceList[i] = temp[i];
}

HOpConstructCuboid::~HOpConstructCuboid()
{
}

HBaseOperator * HOpConstructCuboid::Clone()
{
	return new HOpConstructCuboid(GetView());
}


const char * HOpConstructCuboid::GetName() { return "HOpConstructCuboid"; }

/////////////////////////////////////////////////////////////////////////////
// ConstructCuboid message handlers


int HOpConstructCuboid::OnLButtonDown(HEventInfo &event)
{
	SetNewPoint(event.GetMouseViewpointPos());

	if (OperatorStarted())
	{
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Contents (".", "geometry");
		HC_Close_Segment();

		SetOperatorStarted(false);
		GetView()->SetGeometryChanged();
		GetView()->Update();
	}
	else 
	{
		SetFirstPoint(GetNewPoint());
		SetOperatorStarted(true);
	}

	return HOP_OK;
}


int HOpConstructCuboid::OnLButtonDownAndMove(HEventInfo &event)
{
    if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);
  	 
    SetNewPoint(event.GetMouseViewpointPos());

	if (GetNewPoint() != GetFirstPoint())
	{
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Contents (".", "geometry");
			CreateSquare (GetFirstPoint(), GetNewPoint());
			HC_Set_Visibility("on");
		HC_Close_Segment();

	    GetView()->Update();
	}

    return HOP_OK;
}


int HOpConstructCuboid::OnNoButtonDownAndMove(HEventInfo &event)
{
    if (!OperatorStarted()) 
		return HBaseOperator::OnNoButtonDownAndMove(event);
  	 
    SetNewPoint(event.GetMouseViewpointPos());

	float height = GetNewPoint().y - GetFirstPoint().y;

		if (height != 0)
		{
			HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
				HC_Flush_Contents (".", "geometry");
				CreateCuboid(GetNewPoint());
				//HC_Insert_Shell(8, m_ptPointList, 30, m_FaceList);
				for(int i=0; i<8; ++i)
					HC_Insert_Marker(m_ptPointList[i].x, m_ptPointList[i].y, m_ptPointList[i].z);

				HC_Set_Visibility("on");
			HC_Close_Segment();

			GetView()->Update();
		}

    return HOP_OK;
}



int HOpConstructCuboid::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonUp(event);

    SetNewPoint(event.GetMouseViewpointPos());

    HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Flush_Contents (".", "geometry");
	HC_Close_Segment();

    // make certain that new_point != first_point 
	if (GetFirstPoint() == GetNewPoint())
	{
		SetOperatorStarted(false);
		return HOP_CANCEL;
	}

	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		CreateSquare(GetFirstPoint(), GetNewPoint());
	HC_Close_Segment();

	m_ptEdge = GetNewPoint();

    GetView()->Update(); // update the display
    return HOP_READY;
}


void HOpConstructCuboid::CreateCuboid(HPoint const & last)
{
	float width = GetFirstPoint().x - m_ptEdge.x;
	float height = last.y - GetFirstPoint().y;

	if (height != 0)
	{
		m_ptPointList[0].Set(GetFirstPoint().x,	GetFirstPoint().y,			GetFirstPoint().z);
		m_ptPointList[1].Set(m_ptEdge.x,	GetFirstPoint().y,			GetFirstPoint().z);
		m_ptPointList[2].Set(m_ptEdge.x,	GetFirstPoint().y,			GetFirstPoint().z + width);
		m_ptPointList[3].Set(GetFirstPoint().x,	GetFirstPoint().y,			GetFirstPoint().z + width);
		m_ptPointList[4].Set(GetFirstPoint().x,	GetFirstPoint().y + height,	GetFirstPoint().z);
		m_ptPointList[5].Set(m_ptEdge.x,	GetFirstPoint().y + height,	GetFirstPoint().z);
		m_ptPointList[6].Set(m_ptEdge.x,	GetFirstPoint().y + height,	GetFirstPoint().z + width);
		m_ptPointList[7].Set(GetFirstPoint().x,	GetFirstPoint().y + height,	GetFirstPoint().z + width);

		// translate the points from camera/viewpoint to world space
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());
			for (int i=0; i<8; i++)	
				HC_Compute_Coordinates(".", "viewpoint", &m_ptPointList[i], "world", &m_ptPointList[i]);
		HC_Close_Segment();

	}
}

void HOpConstructCuboid::CreateSquare(HPoint const & first, HPoint const & last)
{
	HPoint	square[4];

	float delta = first.x - last.x;

	square[0].Set(first.x,	first.y,	first.z);
	square[1].Set(last.x,	first.y,	first.z);
	square[2].Set(last.x,	first.y,	first.z + delta);
	square[3].Set(first.x, first.y,	first.z + delta);

	// translate the points from camera/viewpoint to world space
	int i;
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		for (i=0; i<4; i++)
		HC_Compute_Coordinates(".", "viewpoint", &square[i], "world", &square[i]);
	HC_Close_Segment();

	for(i=0; i<4; ++i)
		HC_Insert_Marker(square[i].x, square[i].y, square[i].z);
}
