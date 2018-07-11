// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraZoomBox.cpp : implementation of the HOpCameraZoomBox class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraZoomBox.h"
#include "HUtility.h"
#include "HBhvBehaviorManager.h"




/////////////////////////////////////////////////////////////////////////////
// HOpCameraZoomBox
//
// Operator for modifying the scene's camera based on user defined screen-space
// window
//
// Left button down - find first position
//
// Mouse motion while button down - rubber band box is inserted in "?window space" 
// segment. This segment has a stretched camera from (-1, 1, -1, 1) i.e. the 
// segment's camera is set so that object space is equal to window space.
//
// Mouse up - set camera target, position and field to box defined by first point and 
// current mouse position.


HOpCameraZoomBox::HOpCameraZoomBox(HBaseView* view, int DoRepeat, int DoCapture) : HOpConstructRectangle (view, DoRepeat, DoCapture, true)
{
}

HOpCameraZoomBox::~HOpCameraZoomBox()
{
}


HBaseOperator * HOpCameraZoomBox::Clone()
{
	return new HOpCameraZoomBox(GetView());
}



const char * HOpCameraZoomBox::GetName() { return "HOpCameraZoomBox"; }


/* This tries selecting this in the bounding box.  It averages the selection positions, then projects
the selectionbox's center point into the same plane as the average selection position.  */
bool HOpCameraZoomBox::ComputeReasonableTarget(HPoint & new_tar, HPoint const & min, HPoint const & max, HPoint const & tar_orig)
{
	bool anything_selected = false;
	HPoint new_target;

	HPoint const center = (max + min) / 2.0f;

	char selectability_sav[4096] = {""};

	if (HC_Show_Existence("selectability"))
		HC_Show_Selectability (selectability_sav);

	HC_Set_Selectability ("geometry=on");

	bool const selected = HC_Compute_Selection ("..", ".",
		"v, related selection limit = 0, internal selection limit=-1, selection level = entity, visual selection = off",
		center.x, center.y) != 0;	

	if(selected){
		HC_Show_Selection_Position(0, 0, 0, &new_target.x, &new_target.y, &new_target.z);
		anything_selected = true;
	}
	else {
		/* It's best not to use odd numbers for the steps as we'd just be computing selection on the center of the box again... */
		int const X_STEPS = 8;
		int const Y_STEPS = 8;
		int count = 0;
		HPoint found(0, 0, 0);

		for (int row = 0; row < Y_STEPS; ++row){
			float const yy = (row/(float)(Y_STEPS-1)) * (max.y - min.y) + min.y;

			for (int col = 0; col < X_STEPS; ++col){
				float const xx = (col/(float)(X_STEPS-1)) * (max.x - min.x) + min.x;

				if (HC_Compute_Selection ("..", ".", 
					"v, related selection limit = 0, internal selection limit=-1, selection level = entity, visual selection = off",
					xx, yy)){
					float x, y, z;
					HC_Show_Selection_Position(0, 0, 0, &x, &y, &z);
					found += HPoint(x, y, z);
					++count;
				}
			}
		}

		new_target = (min + max) / 2.0;
		HC_Compute_Coordinates(".", "outer window", &new_target, "world", &new_target);

		if(count > 0){
			HUtility::AdjustPositionToPlane(GetView(), new_target, found / static_cast<float>(count));
			anything_selected = true;
		}
		else {
			HUtility::AdjustPositionToPlane(GetView(), new_target, tar_orig);
		}
	}

	if (selectability_sav[0] != '\0')
		HC_Set_Selectability (selectability_sav);	

	new_tar = new_target;
	return anything_selected;
}

void HOpCameraZoomBox::ComputeNewField(float & width, float & height,
									   HPoint min, HPoint max,
									   HPoint const & new_tar)
{
	HPoint const center = (max + min) / 2.0f;
	HPoint xmax = HPoint(max.x, center.y, center.z);
	HPoint ymax = HPoint(center.x, max.y, center.z);
	HPoint xmin = HPoint(min.x, center.y, center.z);
	HPoint ymin = HPoint(center.x, min.y, center.z);

	HC_Compute_Coordinates(".", "outer window", &xmax, "world", &xmax);
	HC_Compute_Coordinates(".", "outer window", &ymax, "world", &ymax);
	HC_Compute_Coordinates(".", "outer window", &xmin, "world", &xmin);
	HC_Compute_Coordinates(".", "outer window", &ymin, "world", &ymin);

	HUtility::AdjustPositionToPlane(GetView(), xmax, new_tar);
	HUtility::AdjustPositionToPlane(GetView(), ymax, new_tar);
	HUtility::AdjustPositionToPlane(GetView(), xmin, new_tar);
	HUtility::AdjustPositionToPlane(GetView(), ymin, new_tar);

	HVector x = xmax - xmin;
	HVector y = ymax - ymin;

	width = static_cast<float>(HC_Compute_Vector_Length(&x));
	height = static_cast<float>(HC_Compute_Vector_Length(&y));

#if 0
	/* This code is for debugging.  It'll show where the zoom box was in the model.  */
	HC_Open_Segment_By_Key(GetView()->GetModelKey());{
		HC_Open_Segment("test");{
			HC_Flush_Contents(".", "geometry");
			HC_Set_Visibility("markers = on, lines = on");
			HC_Set_Color("markers = red");
			HC_Set_Marker_Size(3);
			HC_Set_Marker_Symbol("X");
			HC_Insert_Marker(xmin.x, xmin.y, xmin.z);
			HC_Insert_Marker(xmax.x, xmax.y, xmax.z);
			HC_Insert_Marker(ymin.x, ymin.y, ymin.z);
			HC_Insert_Marker(ymax.x, ymax.y, ymax.z);
			HC_Set_Rendering_Options("depth range = (0, 0.05)");
			HC_Insert_Line(xmin.x, xmin.y, xmin.z, xmax.x, xmax.y, xmax.z);
			HC_Insert_Line(ymin.x, ymin.y, ymin.z, ymax.x, ymax.y, ymax.z);
			HC_Set_Selectability("off");
		}HC_Close_Segment();
	}HC_Close_Segment();
#endif

}


#define MINIMUM_FIELD_SIZE (1e-9f)

int HOpCameraZoomBox::OnLButtonUp(HEventInfo &event)
{
	HOpConstructRectangle::OnLButtonUp(event);

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && 
		GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;

	if (!m_bRectangleExists) 
		return HBaseOperator::OnLButtonUp(event);

	HUtility::Order(&m_ptRectangle[0], &m_ptRectangle[1]);
	HUtility::ClampPointToWindow(&m_ptRectangle[0]);
	HUtility::ClampPointToWindow(&m_ptRectangle[1]);

	if (m_ptRectangle[1].x - m_ptRectangle[0].x < MINIMUM_FIELD_SIZE ||
		m_ptRectangle[1].y - m_ptRectangle[0].y < MINIMUM_FIELD_SIZE )
		return HOP_OK;

	HC_Open_Segment_By_Key (GetView()->GetSceneKey());{
		/* Save the original camera for smooth transition. */
		HCamera orig;
		HC_Show_Net_Camera (&orig.position, &orig.target, &orig.up_vector, &orig.field_width, &orig.field_height, orig.projection);

		/* We need to fill in these values with the new camera. */
		HCamera adjusted = orig;

		// Compute a Selection_By_Area using the Rectangle to find the object closest to the viewer


		// THIS IS NOW DONE IN ComputeReasonableTarget
		// Save selection settings if locally set
// 		char heur_sav[4096] = {""};
// 		char selectability_sav[4096] = {""};
// 
// 		if (HC_Show_Existence("selectability")) {
// 			HC_Show_Selectability (selectability_sav);
// 		}
// 
// 		if (HC_Show_Existence("heuristics")) {
// 			HC_Show_Heuristics(heur_sav);
// 		}

// 		HC_Set_Heuristics ("related selection limit = 0, internal selection limit=-1");
// 		HC_Set_Selectability ("geometry=on");

		bool anything_selected = ComputeReasonableTarget(adjusted.target, m_ptRectangle[0], m_ptRectangle[1], orig.target);

		if(anything_selected){

			ComputeNewField(adjusted.field_width, adjusted.field_height,
				m_ptRectangle[0], m_ptRectangle[1],
				adjusted.target);

			float diagonal_len = sqrt(pow(adjusted.field_width, 2) + pow(adjusted.field_height, 2));
			HVector viewingVector = orig.position - orig.target;
			HC_Compute_Normalized_Vector(&viewingVector, &viewingVector);
			adjusted.position = adjusted.target + viewingVector * 2.5 * diagonal_len;

			double const camera_dist = HC_Compute_Vector_Length(&adjusted.target) +1;
			double const min_camera = 0.0005 * camera_dist;

			/* If the camera is about to be too small... */
			if (diagonal_len < min_camera) {
				double x = min_camera / diagonal_len;
				adjusted.field_width *= static_cast<float>(x);
				adjusted.field_height *= static_cast<float>(x);
				diagonal_len = sqrt(pow(adjusted.field_width, 2) + pow(adjusted.field_height, 2));
				HVector dir_to_position = orig.position - orig.target;
				HC_Compute_Normalized_Vector(&dir_to_position, &dir_to_position);
				adjusted.target = adjusted.position - dir_to_position * 2.5 * diagonal_len;
			}

			if (orig.CameraDifferent(adjusted)){
				GetView()->PrepareForCameraChange();
				if (GetView()->GetSmoothTransition())
				{
					HUtility::SmoothTransition( orig.position, orig.target, orig.up_vector, orig.field_width, orig.field_height,
												adjusted.position, adjusted.target, adjusted.up_vector, adjusted.field_width, adjusted.field_height,
												GetView() );
				}
				else {
					HC_Set_Camera_Position(adjusted.position.x, adjusted.position.y, adjusted.position.z);
					HC_Set_Camera_Target(adjusted.target.x, adjusted.target.y, adjusted.target.z);
					HC_Set_Camera_Field(adjusted.field_width, adjusted.field_height);
				}
				GetView()->CameraPositionChanged(true, GetView()->GetSmoothTransition());
			}
		}

// 		HC_UnSet_Heuristics();
// 
// 		if (!streq(heur_sav,""))
// 			HC_Set_Heuristics(heur_sav);	
// 
// 		HC_UnSet_Selectability ();
// 
// 		if (!streq(selectability_sav,""))
// 			HC_Set_Selectability (selectability_sav);	

	}HC_Close_Segment();
   
	GetView()->Update();
	
	return HOP_READY;
}



int HOpCameraZoomBox::OnLButtonDblClk(HEventInfo &event)
{
	SetNewPoint(event.GetMouseWindowPos());

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		HPoint tmp(GetNewPoint());	
		HC_Compute_Coordinates (".", "outer window", &tmp,"world", &tmp);
		SetNewPoint(tmp);
		HC_Set_Camera_Target  (GetNewPoint().x, GetNewPoint().y, GetNewPoint().z);
	}HC_Close_Segment();

	GetView()->Update(); // update the display

	GetView()->CameraPositionChanged(true);

	return HBaseOperator::OnLButtonDblClk(event);
}
