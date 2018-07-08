// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreatePolyline.cpp : implementation of the HOpCreatePolyline class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HOpCreatePolyline.h"



/////////////////////////////////////////////////////////////////////////////
// HOpCreatePolyline
//
// Operator for rubberbanding a Polyline in the viewplane.
//
// Left button down:
//		Insert_Ink with position in window space in "?window space" segment
//
// NoButtonDownAndMove:
//		Rubber band line from last point to current point 
//
// Left Button Double Click: 
//		Polyline definition is done. 
//		Store the Polyline information; Points are in Window Space
//		Flush "?Window Space" segment
//
// HOpCreatePolyline.cpp : implementation of the HOpCreate class
//


HOpCreatePolyline::HOpCreatePolyline(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructPolyline(view, DoRepeat, DoCapture)
{
}

HOpCreatePolyline::~HOpCreatePolyline()
{
}


HBaseOperator * HOpCreatePolyline::Clone()
{
	return new HOpCreatePolyline(GetView());
}


const char * HOpCreatePolyline::GetName() { return "HOpCreatePolyline"; }

int HOpCreatePolyline::OnLButtonDblClk(HEventInfo &event)
{
	if (OperatorStarted()){ 
	    HOpConstructPolyline::OnLButtonDblClk(event);
		float matrix1[16], matrix2[16], transform[16];

		/* The points are stored in window space in HOpConstructPolyline.  We need to find the
		   transform from local window (or outer window) to world.  Do this in the scene key
		   as we need to know the camera as well as the window. */
		HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
			HC_Compute_Transform(".", "local window", "world", matrix1);
		}HC_Close_Segment();

		HC_Open_Segment_By_Key(GetView()->GetModelKey());{
			HC_Open_Segment(GetName());{
				/* Now go from world to object in case somebody (like HOpObjectRotate) put a
				   transform in this segment. */
				HC_Compute_Transform(".", "world", "object", matrix2);
				HC_Compute_Matrix_Product(matrix1, matrix2, transform);
				HC_Compute_Transformed_Points(m_PolylineCount, &m_pPolyline[0].x, transform, &m_pPolyline[0].x);

				HC_Insert_Polyline(m_PolylineCount, m_pPolyline);
				HC_Set_Visibility("lines=on");
				HC_Set_Rendering_Options ("nurbs curve = (budget = 10000, maximum angle = 10)");
			}HC_Close_Segment();
		}HC_Close_Segment();

		GetView()->Update();
	}

	return HOP_READY;
}


