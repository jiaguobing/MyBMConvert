// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraZoom.cpp : implementation of the HOpCameraZoom class
//

#include <math.h>
#include <string.h>

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraZoom.h"
#include "HBhvBehaviorManager.h"



/////////////////////////////////////////////////////////////////////////////
// HOpCameraZoom
//
// Operator for zooming the scene's camera based on the user mouse input
//
// Left button down - find first position and bounds object with box
//
// Mouse motion while down - zooms camera based on directon of movement
// mouse movement to right zooms in, movement to left zooms out
//


HOpCameraZoom::HOpCameraZoom(HBaseView* view, int DoRepeat,int DoCapture) : HBaseOperator(view, DoRepeat, DoCapture)
{
	m_LightFollowsCamera = false;
}

HOpCameraZoom::~HOpCameraZoom()
{
}


HBaseOperator * HOpCameraZoom::Clone()
{
	return new HOpCameraZoom(GetView());
}



const char * HOpCameraZoom::GetName() { return "HOpCameraZoom"; }

/////////////////////////////////////////////////////////////////////////////
// HOpCameraZoom message handlers



int HOpCameraZoom::OnLButtonDown(HEventInfo &event)
{
	HPoint target;

	if (GetView()->GetModel()->GetBhvBehaviorManager()->IsPlaying() && GetView()->GetModel()->GetBhvBehaviorManager()->GetCameraUpdated())
		return HOP_OK;
	
	if (!OperatorStarted()) 
		SetOperatorStarted(true);

	SetNewPoint(event.GetMouseWindowPos());
	SetFirstPoint(GetNewPoint());

 	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		HC_Show_Net_Camera_Target(&m_ptCamera.x,&m_ptCamera.y,&m_ptCamera.z);
		HC_Show_Net_Camera_Position(&target.x,&target.y,&target.z);	
		m_ptDist.Set(target.x-m_ptCamera.x, target.y - m_ptCamera.y, target.z - m_ptCamera.z);
		m_fLength = (float)HC_Compute_Vector_Length(&m_ptDist);
		HC_Compute_Normalized_Vector(&m_ptDist,&m_ptDist);
		HC_Show_Net_Camera_Field(&m_Width, &m_Height);
	HC_Close_Segment();

	GetView()->PrepareForCameraChange();

 	return HOP_OK;
}



int HOpCameraZoom::OnLButtonDownAndMove(HEventInfo &event)
{
	if (!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

	SetNewPoint(event.GetMouseWindowPos());

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());
		
		float w, h, zoom_factor = GetNewPoint().y - GetFirstPoint().y;

		HPoint target, position, diff; 
		char proj[64];
 
		HC_Show_Net_Camera_Position(&position.x, &position.y, &position.z);
		HC_Show_Net_Camera_Target(&target.x, &target.y, &target.z);
		HC_Show_Net_Camera_Field(&w, &h);
		HC_Show_Net_Camera_Projection(proj);


		double diagonal_len = sqrt(w*w + h*h);

		/* If the camera is about to be too big... */
		if (diagonal_len > fabs(MVO_SQRT_MAX_FLOAT)) {
			HC_Close_Segment();
			return HOP_OK;
		}

		HVector dir_to_position = position - target;
		HC_Compute_Normalized_Vector(&dir_to_position, &dir_to_position);

		double const min_camera = GetView()->GetZoomLimit();

		// we only want to zoom in if we are still further out than
		// the maximum zoom level and we are actually zooming in
		double maxZoomLevel = min_camera;
		if ((w > maxZoomLevel) || (h > maxZoomLevel) || (zoom_factor<0.0f))
		{

		    diff.Set(position.x - target.x, 
			    position.y - target.y, 
			    position.z - target.z);

			//prevent zooming out if it will create a camera with an unsquareable eye_length
			if (zoom_factor < 0.f && HC_Compute_Vector_Length(&diff) > fabs(MVO_SQRT_MAX_FLOAT)) {
				HC_Close_Segment();
				return HOP_OK;
			}

		    // if zoom_factor greater than 1 than we will zoom past the camera target.  Extra check to avoid unneccessary 
		    // zoom in when the camera is close to target (sign of zoom factor indicates whether trying to zoom in/out) 
			// #5241, sometimes ACIS users DO want to make zooms that small
			// if ((HC_Compute_Vector_Length(&diff) > 0.0000001))
			if (zoom_factor < 0.99)
			{
				HPoint newPos(
					m_ptCamera.x + m_ptDist.x * (m_fLength-(m_fLength*zoom_factor)),
					m_ptCamera.y + m_ptDist.y * (m_fLength-(m_fLength*zoom_factor)),
					m_ptCamera.z + m_ptDist.z * (m_fLength-(m_fLength*zoom_factor)));
				if (newPos != target)
					HC_Set_Camera_Position(newPos.x, newPos.y, newPos.z);
			}

			if (streq(proj, "orthographic"))
			{
				if (!(zoom_factor > 0.99))
					HC_Set_Camera_Field(m_Width - (m_Width * zoom_factor), m_Height - (m_Height * zoom_factor));
			}
		}

	


		/******* AUTO FOCUS **************
		char stereomode[128];
		HC_Show_One_Net_Rendering_Option("stereo", stereomode);
		
		if(streq("on", stereomode)){
			float w = 0.0f, h = 0.0f;
			HC_Open_Segment_By_Key(GetView()->GetViewKey());
				HC_Show_Net_Camera_Field(&w, &h);
			HC_Close_Segment();

			char rendering_opts[128];
			float separation = w * 0.3f;
		

			char type[MVO_BUFFER_SIZE];
			char QueryString[MVO_BUFFER_SIZE];
			int count, off1, off2, off3, hits = 0;
			HC_KEY key = 0;
			HPoint	awindow, world, object, avg, camera;
			avg.Set(0.0f, 0.0f, 0.0f);

			float pick_coords[] = {0.0f,0.5f,  -0.25f,0.25f,  0.0f,0.25f, 0.25f,0.25f,
				-0.5,0.0f,  -0.25f,0.0f, 0.0f,0.0f, 0.25f,0.0f,  0.5f,0.0f,
				-0.25f,-0.25f,  0.0f,-0.25f,  0.25f,-0.25f,  0.0f,-0.5f};

			for(int i = 0; i < 26; i+=2){
				HC_Open_Segment_By_Key (GetView()->GetViewKey());
					HC_QSet_Selectability ("./scene/overwrite", "geometry=on");
	
					count = HC_Compute_Selection (".", "./scene/overwrite", "v, selection level = entity", pick_coords[i], pick_coords[i+1]);
	
					HC_QUnSet_Selectability ("./scene/overwrite");
				HC_Close_Segment();
				HC_Open_Segment_By_Key (GetView()->GetSceneKey());
					if (count > 0)
					{
						HC_Show_Selection_Element(&key, &off1, &off2, &off3);
						HC_Show_Selection_Original_Key (&key);
						HC_Show_Selection_Position(&awindow.x, &awindow.y, &awindow.z, &world.x, &world.y, &world.z);
						avg = avg + world;
						HC_Show_Key_Type(key, type);
						hits++;
					}
				HC_Close_Segment();
			}
			avg.x = avg.x / (float)hits;
			avg.y = avg.y / (float)hits;
			avg.z = avg.z / (float)hits;

			HC_Open_Segment_By_Key (GetView()->GetSceneKey());
				HC_Show_Camera_Position (&camera.x, &camera.y, &camera.z);
			HC_Close_Segment();
			awindow = avg - camera;
			float distance = sqrt((awindow.x * awindow.x) + (awindow.y * awindow.y) + (awindow.z * awindow.z));
		

			HC_Open_Segment_By_Key(GetView()->GetSceneKey());
				sprintf(rendering_opts, "stereo, stereo distance = %f", distance * 1.0);
				HC_Set_Rendering_Options(rendering_opts);
			HC_Close_Segment(); 
		}
		
		****** END AUTO FOCUS ***********/

	HC_Close_Segment();

	// tell the view that the camera position has changed
	GetView()->CameraPositionChanged();

 	GetView()->Update();
	return HOP_OK;
}


int HOpCameraZoom::OnLButtonUp(HEventInfo &event)
{
	if(!OperatorStarted()) 
		return HBaseOperator::OnLButtonDownAndMove(event);

 	SetOperatorStarted(false);
	GetView()->CameraPositionChanged(true, true);

    return HOP_READY;
}





