// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateNurbsCurve.cpp: implementation of the HOpCreateNurbsCurve class.
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
#include "HOpCreateNurbsCurve.h"

#define FEQ(a,b,eps) ((fabs((a)-(b))<(eps)))

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HOpCreateNurbsCurve::HOpCreateNurbsCurve(HBaseView * view, int DoRepeat, int DoCapture) : HOpConstructNurbsCurve (view, DoRepeat, DoCapture)
{
}


HOpCreateNurbsCurve::~HOpCreateNurbsCurve()
{
}



HBaseOperator * HOpCreateNurbsCurve::Clone()
{
	return new HOpCreateNurbsCurve(GetView());
}




const char * HOpCreateNurbsCurve::GetName() 
{ 
	return "HOpCreateNurbsCurve";
}


int HOpCreateNurbsCurve::OnLButtonDblClk(HEventInfo &event)
{		
	int i;

	if (OperatorStarted() && m_PolylineCount > m_Degree)	{
		
		HOpConstructNurbsCurve::OnLButtonDblClk(event);

		HC_Open_Segment_By_Key(GetView()->GetConstructionKey());
			for(i = 0; i < m_PolylineCount; i++)
				HC_Compute_Coordinates(".", "local window", &m_pPolyline[i], "world", &m_pPolyline[i]);
		HC_Close_Segment();

		HC_Open_Segment_By_Key(GetView()->GetModelKey());
			HC_Open_Segment("curves");
				if(HC_Show_Existence("modelling matrix")!=0)
				{
					float fnurb_matrix[16], finv_nurb_matrix[16];
					
					HC_Show_Modelling_Matrix(fnurb_matrix);
					if(HC_Compute_Matrix_Determinant(fnurb_matrix)) HC_Compute_Matrix_Inverse(fnurb_matrix, finv_nurb_matrix);
					else HC_Compute_Matrix_Adjoint(fnurb_matrix, finv_nurb_matrix);

					HC_Compute_Transformed_Points(m_PolylineCount, m_pPolyline, finv_nurb_matrix, m_pPolyline);
				}
				HC_Insert_NURBS_Curve(m_Degree, m_PolylineCount, m_pPolyline, m_WeightsArray, 
					m_KnotsArray, m_StartU, m_EndU);
				HC_Set_Visibility("lines=on");
			HC_Close_Segment();
		HC_Close_Segment();

		GetView()->Update();
	}
	
	return HOP_OK;
}
