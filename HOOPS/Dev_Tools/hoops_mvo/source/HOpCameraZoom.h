// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HPanCamera.h : interface of the HPanCamera class
// zooms the camera of current view in realtime

#ifndef _HOPCAMERAZOOM_H
#define _HOPCAMERAZOOM_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpCameraZoom class zooms the camera toward a target.
/*! 
  HOpCameraZoom implements three of the mouse button handlers defined on the base class and maps the event
  information to HOOPS camera routines.  This provides the basic functionality for 
  interactively zooming in or out from a model in realtime.  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The zoom is initiated.
  <li>Left Button Down and Drag:	The camera zooms to the default target.
  <li>Left Button Up:				The operation ends.
  </ol>
  More Detailed Description: see event methods.
*/
class MVO_API HOpCameraZoom : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraZoom object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCameraZoom (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraZoom();

	/*!
		\returns A pointer to a character string denoting the name of the operator which is 'HOpCameraZoom'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown draws a bounding box around the scene, records the first mouse position, and calculates the 
		current camera target based on the first point. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove tests for left button down, records points as the mouse is moved, recalculates the current
		target for each point, and zooms the camera towards that current target. The method makes calculations to avoid 
		unneccessary zoom when the camera is close to the target. If this operator is set so that the lights follow the 
		camera, lighting targets will also be recalculated and lights reoriented.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp updates the scene with current camera and lighting information and cleans up. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);

	/*!
		SetLightFollowsCamera sets the lights to follow the camera or to stay in one place during orbit.  This has
		the effect of either lighting the scene during orbit or allowing the camera to move through shadow.  The default
		is off.
		\param follow Pass true to have the light follow the camera.
	*/
	void SetLightFollowsCamera(bool follow){m_LightFollowsCamera = follow;};

	/*! \return True if the light follows the camera or false if it doesn't. */
	bool GetLightFollowsCamera(){return m_LightFollowsCamera;};
	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCameraZoom object.
	*/
	virtual HBaseOperator * Clone();

private:
	bool	m_LightFollowsCamera;	
	HPoint	m_ptDist, m_ptCamera;
	float	m_Width, m_Height, m_fLength;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



