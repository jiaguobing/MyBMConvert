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

#ifndef _HOpCameraZoomTouch_H
#define _HOpCameraZoomTouch_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpCameraZoomTouch class zooms the camera toward a target.
/*! 
  HOpCameraZoomTouch implements three of the touch handlers defined on the base class and maps the event
  information to HOOPS camera routines based on a two finger pinching motion.  This provides the basic functionality for 
  interactively zooming in or out from a model in realtime.  

  More Detailed Description: see event methods.
*/
class MVO_API HOpCameraZoomTouch : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraZoomTouch object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCameraZoomTouch (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraZoomTouch();

	/*!
		\returns A pointer to a character string denoting the name of the operator which is 'HOpCameraZoomTouch'.
	*/
	virtual const char * GetName();  

	/*!
		OnTouchesDown records touch positions and calculates the 
		current camera target based on the first touch points. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnTouchesDown (HEventInfo &hevent);

	/*!
		OnTouchesDownAndMove zooms the camera in and out based on a pinching motion of the user.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnTouchesMove(HEventInfo &hevent);

	/*!
		OnLButtonUp updates the scene with current camera and lighting information and cleans up. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnTouchesUp (HEventInfo &hevent);

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
	  \return A pointer to the newly created HOpCameraZoomTouch object.
	*/
	virtual HBaseOperator * Clone();

private:
	bool	m_LightFollowsCamera;	
	HPoint	m_ptDist, m_ptCamera;
	float	m_Width, m_Height, m_fLength;
    HTouchInfo m_LastTouch1;
    HTouchInfo m_LastTouch2;
   
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



