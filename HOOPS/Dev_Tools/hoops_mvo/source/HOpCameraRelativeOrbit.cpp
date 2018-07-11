// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraRelativeOrbit.cpp : implementation of the HOpCameraRelativeOrbit class
//


#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpCameraRelativeOrbit.h"
#include "HOpCameraOrbit.h"

#include "HTools.h"
#include "HUtility.h"
#include "hc.h"

#include <math.h>
#include <string.h>

#define ACCEL 3.0f

/////////////////////////////////////////////////////////////////////////////
// HOpCameraRelativeOrbit
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



HOpCameraRelativeOrbit::HOpCameraRelativeOrbit(HBaseView* view, int doRepeat, int doCapture)
	: HOpCameraOrbit(view, doRepeat, doCapture)
	, targetType(TARGET_IS_CAMERA_TARGET)
{}


HOpCameraRelativeOrbit::~HOpCameraRelativeOrbit()
{}


HBaseOperator * HOpCameraRelativeOrbit::Clone()
{
	return new HOpCameraRelativeOrbit(GetView());
}

 
const char * HOpCameraRelativeOrbit::GetName()
{
	return "HOpCameraRelativeOrbit";
}


void HOpCameraRelativeOrbit::SetCenter(HPoint center)
{
	fauxTarget = center;
	targetType = TARGET_IS_USER_DEFINED;
}


HPoint HOpCameraRelativeOrbit::GetCenter()
{
	switch (targetType)	{
		case TARGET_IS_USER_DEFINED: {
			return fauxTarget;
		}
		case TARGET_IS_SELECTION_DEFINED: {
			return fauxTarget;
		}
		case TARGET_IS_CAMERA_TARGET: {
			HPoint target;
			HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
				HC_Show_Camera_Target(&target.x, &target.y, &target.z);
			}HC_Close_Segment();
			return target;
		}
		default: {
			assert(false);
			return HPoint();
		}
	}
}


int HOpCameraRelativeOrbit::OnLButtonDown(HEventInfo &event)
{
	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		HC_Open_Segment("relorbit");{
			HC_Flush_Contents(".", "everything");
		}HC_Close_Segment();
	}HC_Close_Segment();

	switch (targetType) {
		case TARGET_IS_SELECTION_DEFINED:
		case TARGET_IS_CAMERA_TARGET: {
			HPoint winPos = event.GetMouseWindowPos();
			HC_Open_Segment_By_Key(GetView()->GetViewKey());{
				int res = HC_Compute_Selection(".", "scene/overwrite", "v, selection level = entity", winPos.x, winPos.y);
				if (res > 0) {
					HPoint window;
					HPoint world;
					do {
						HC_Show_Selection_Position(&window.x, &window.y, &window.z, &world.x, &world.y, &world.z);			
					} while (HC_Find_Related_Selection());
					fauxTarget = world;
					targetType = TARGET_IS_SELECTION_DEFINED;
				}
				else {
					HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
						HPoint target;
						HC_Show_Camera_Target(&target.x, &target.y, &target.z);
						fauxTarget = target;
						targetType = TARGET_IS_CAMERA_TARGET;
					}HC_Close_Segment();
				}
			}HC_Close_Segment();
			break;
		}
		case TARGET_IS_USER_DEFINED: {
			break;
		}
		default: {
			assert(false);
		}
	}
	
	return HOpCameraOrbit::OnLButtonDown(event);
}


int HOpCameraRelativeOrbit::OnLButtonDownAndMove(HEventInfo &event)
{
	switch (targetType) {
		case TARGET_IS_CAMERA_TARGET: {
			return HOpCameraOrbit::OnLButtonDownAndMove(event);
		}
		case TARGET_IS_SELECTION_DEFINED: {
			break;
		}
		case TARGET_IS_USER_DEFINED: {
			break;
		}
		default: {
			assert(false);
		}
	}

	int res;

	HC_Open_Segment_By_Key(GetView()->GetSceneKey());{
		HPoint target;
		HC_Show_Camera_Target(&target.x, &target.y, &target.z);

		HPoint xformTarget;
		HPoint xformFauxTarget;
		HC_Compute_Coordinates(".", "world", &target, "viewpoint", &xformTarget);
		HC_Compute_Coordinates(".", "world", &fauxTarget, "viewpoint", &xformFauxTarget);
		
		HVector dollyDelta = xformFauxTarget - xformTarget;

		HC_Dolly_Camera(dollyDelta.x, dollyDelta.y, dollyDelta.z);{
			GetView()->SetSuppressUpdate(true);{
				res = HOpCameraOrbit::OnLButtonDownAndMove(event);
			}GetView()->SetSuppressUpdate(false);
		}HC_Dolly_Camera(-dollyDelta.x, -dollyDelta.y, -dollyDelta.z);

	}HC_Close_Segment();

	GetView()->CameraPositionChanged();
	GetView()->Update();

	return res;
}





