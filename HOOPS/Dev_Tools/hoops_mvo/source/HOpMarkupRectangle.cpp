// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupRectangle.cpp : implementation of the HOpMarkupRectangle class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HOpConstructRectangle.h"
#include "HOpMarkupRectangle.h"
#include "HEventInfo.h"
#include "HSelectionSet.h"
#include "HUtility.h"
#include "HMarkupManager.h"
#include "HImManager.h"

  
HOpMarkupRectangle::HOpMarkupRectangle(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructRectangle(view, DoRepeat, DoCapture, false)
{
}

HOpMarkupRectangle::~HOpMarkupRectangle()
{
}

HBaseOperator * HOpMarkupRectangle::Clone()
{
	return new HOpMarkupRectangle(GetView());
}



const char * HOpMarkupRectangle::GetName() { return "HOpMarkupRectangle"; }



int HOpMarkupRectangle::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

	HOpConstructRectangle::OnLButtonUp(event);

    SetNewPoint(event.GetMouseWindowPos());

	//make certain that new_point != first_point 
	if (GetNewPoint().x == GetFirstPoint().x &&
		GetNewPoint().y == GetFirstPoint().y)
		return HOP_CANCEL;

	// Store Rectangle Points in window space
	m_ptRectangle[0] = GetFirstPoint();
	m_ptRectangle[1] = GetNewPoint();
	m_bRectangleExists = true;

	// need to translate the points from window into world space
	HC_Open_Segment_By_Key (GetView()->GetSceneKey());
		
		HPoint point = GetFirstPoint();
		HC_Compute_Coordinates(".", "local window", &point, "viewpoint", &point);
		SetFirstPoint(point);

		point = GetNewPoint();
		HC_Compute_Coordinates(".", "local window", &point, "viewpoint", &point);
		SetNewPoint(point);

		HCamera camera;

		GetView()->GetCamera(&camera);

		// we need to ensure that we place the rectangle at the location of the camera target to
		// ensure that the rectangle appears in both perspective and orthographic modes
		double dist = sqrt((camera.position.x - camera.target.x)*(camera.position.x - camera.target.x) +
			(camera.position.y - camera.target.y)*(camera.position.y - camera.target.y) +
			(camera.position.z - camera.target.z)*(camera.position.z - camera.target.z));

		HPoint pts[5];

		pts[0].x = GetFirstPoint().x;		pts[0].y = GetFirstPoint().y;		pts[0].z = (float) dist;
		pts[1].x = GetFirstPoint().x;		pts[1].y = GetNewPoint().y;		pts[1].z = (float) dist;
		pts[2].x = GetNewPoint().x;		pts[2].y = GetNewPoint().y;		pts[2].z = (float) dist;
		pts[3].x = GetNewPoint().x;		pts[3].y = GetFirstPoint().y;		pts[3].z = (float) dist;
		pts[4].x = GetFirstPoint().x;		pts[4].y = GetFirstPoint().y;		pts[4].z = (float) dist;

		for (int i=0; i<5; i++)
		   HC_Compute_Coordinates(".", "viewpoint", &pts[i], "world", &pts[i]);

	HC_Close_Segment();

	HMarkupManager *markupManager;
	if ((markupManager = GetView()->GetMarkupManager()) != 0)
	{
		HC_KEY activelayerkey, rect;

		if ((activelayerkey = markupManager->GetCurrentLayerKey()) == -1)
			activelayerkey = markupManager->OpenLayer("");
		
		HC_Open_Segment_By_Key(activelayerkey); {
			rect = HC_KOpen_Segment("rectangle"); {
				// this is now hoops default behaviour but might be useful for "no absolute line weight" 
				//HImSetCallback("draw dc polyline","HImUtility_device_independent_polyline_weights");
				HC_Insert_Polyline (5, pts);
				HC_Set_Rendering_Options("no frame buffer effects");
			} HC_Close_Segment();
		}HC_Close_Segment();

		// Sent the Markup to HNet if we're sending messages
		if(GetView()->EmittingMessages())
			markupManager->EmitMarkupSegment(rect);
	}
	
    GetView()->Update(); 
    SetOperatorStarted(false);
	
    return HOP_READY;
}




