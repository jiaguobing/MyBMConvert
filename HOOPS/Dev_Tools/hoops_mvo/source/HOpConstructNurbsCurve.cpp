// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructNurbsCurve.cpp: implementation of the HOpConstructNurbsCurve class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpConstructPolyline.h"
#include "HOpConstructNurbsCurve.h"

#define FEQ(a,b,eps) ((fabs((a)-(b))<(eps)))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HOpConstructNurbsCurve::HOpConstructNurbsCurve(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructPolyline (view, DoRepeat, DoCapture)
{
	m_Degree = 2;
	m_StartU = 0;
	m_EndU = 1;
	m_WeightsArray = 0;
	m_KnotsArray = 0;
	m_bFirstTimeThrough = true;

}//end constructor


HOpConstructNurbsCurve::~HOpConstructNurbsCurve()
{
	//flush any curves out of the construction key that haven't yet been committed.
	HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
		HC_Open_Segment ("curve");
			HC_Flush_Geometry(".");
		HC_Close_Segment();
	HC_Close_Segment();
	
}//end destructor



HBaseOperator * HOpConstructNurbsCurve::Clone()
{
	return new HOpConstructNurbsCurve(GetView());
}//end Clone




const char * HOpConstructNurbsCurve::GetName() 
{ 
	return "HOpConstructNurbsCurve";
}//end GetName




int HOpConstructNurbsCurve::OnLButtonDown(HEventInfo &event)
{		

	HOpConstructPolyline::OnLButtonDown(event);

		// Pack the Operators Polyline point array and count
	HC_Show_Polyline_Count(m_PolylineKey, &m_PolylineCount);

	if(m_PolylineCount > m_Degree)
	{
		H_SAFE_DELETE_ARRAY(m_pPolyline);

			// extract the polyline data into a new array
		m_pPolyline = new HPoint[m_PolylineCount];
		HC_Show_Polyline(m_PolylineKey, &m_PolylineCount, m_pPolyline);
			// transform the polyline points in to world space
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			for(int i = 0; i < m_PolylineCount; i++)
				HC_Compute_Coordinates (".", "local window", &m_pPolyline[i], "world", 
					&m_pPolyline[i]);
		HC_Close_Segment();

			// insert world space polyline array as a NURBS
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Open_Segment ("curve");
				HC_Flush_Geometry(".");
				HC_Insert_NURBS_Curve(m_Degree, m_PolylineCount, m_pPolyline, m_WeightsArray, 
					m_KnotsArray, m_StartU, m_EndU);
			HC_Close_Segment();
		HC_Close_Segment();

		
		GetView()->Update();
	}

	m_bFirstTimeThrough = true;

	return HOP_OK;
}// end OnLButtonDown




int HOpConstructNurbsCurve::OnNoButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HOpConstructPolyline::OnNoButtonDownAndMove(event);

	HOpConstructPolyline::OnNoButtonDownAndMove(event);
	
	//test to see if current and last points are equivalent
	SetNewPoint(event.GetMouseWindowPos());

	if (m_ptLast == GetNewPoint())
		return HOpConstructPolyline::OnNoButtonDownAndMove(event);
		
	// Pack the Operators Polyline point array and count
	HC_Show_Polyline_Count (m_PolylineKey, &m_PolylineCount);

	if(m_PolylineCount > m_Degree)
	{
		if (m_bFirstTimeThrough)
		{
			// delete the previous polyline
			H_SAFE_DELETE_ARRAY(m_pPolyline);

			m_pPolyline = new HPoint[m_PolylineCount + 1];
			HC_Show_Polyline(m_PolylineKey, &m_PolylineCount, m_pPolyline);
			m_pPolyline[m_PolylineCount] = event.GetMouseWindowPos();

			HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
				// first translate the points from window to world space coords
				for (int i=0; i<m_PolylineCount+1; i++)
					HC_Compute_Coordinates(".", "local window", &m_pPolyline[i], "world", 
						&m_pPolyline[i]);
			HC_Close_Segment();

			m_bFirstTimeThrough = false;
		}
		else
			m_pPolyline[m_PolylineCount] = event.GetMouseWorldPos();

		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Open_Segment("curve");
				HC_Flush_Geometry(".");
				HC_Insert_NURBS_Curve (m_Degree, m_PolylineCount+1, m_pPolyline, m_WeightsArray, 
					m_KnotsArray, m_StartU, m_EndU);
			HC_Close_Segment();
		HC_Close_Segment();
		
		GetView()->Update();
	}

	return HOP_OK;
}//end OnNoButtonDownAndMove




int HOpConstructNurbsCurve::OnLButtonDblClk(HEventInfo &event)
{		
	if (OperatorStarted()){
		HOpConstructPolyline::OnLButtonDblClk(event);
		//clean up the HOOPS segment tree
		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			HC_Flush_Geometry("...");
			HC_Delete_Segment("curve");
		HC_Close_Segment();

		SetOperatorStarted(false);
		m_bFirstTimeThrough = true;
		GetView()->Update(); // update the display

		return HOP_READY;
	}

	return HBaseOperator::OnLButtonDblClk(event);
}// end OnLButtonDblClk



int HOpConstructNurbsCurve::OnMButtonDown(HEventInfo &event)
{
	OnLButtonDown(event);
	return OnLButtonDblClk(event);
}// end OnMButtonDown




