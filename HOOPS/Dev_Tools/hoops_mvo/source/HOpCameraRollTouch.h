// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraPan.h : interface of the HOpCameraPan class
// pans the camera of current view

#ifndef _HOPCAMERAROLLTOUCH_H
#define _HOPCAMERAROLLTOUCH_H  

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpCameraRollTouch class rolls the camera around the current view axis
/*! 
  HOpCameraPan implements three mouse event methods defined on the base class and maps the event information
  to HOOPS camera routines based on two finger input. 
  More Detailed Description: see event methods
*/
class MVO_API HOpCameraRollTouch : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraRollTouch object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
    HOpCameraRollTouch(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraRollTouch();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpCameraRollTouch'.
	*/
	virtual const char * GetName();  

	/*!
		OnTouchesDown records the initial touch position. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnTouchesDown (HEventInfo &hevent);

	/*!
		OnTouchesMove records two touch points and rolls the camera around the view axis based on a circular touch motio
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnTouchesMove(HEventInfo &hevent);

	/*!
		OnTouchesUp updates the scene with current camera and lighting information and cleans up. 
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
	  \return A pointer to the newly created HOpCameraPan object.
	*/
	virtual HBaseOperator * Clone();  

private:
    HTouchInfo m_LastTouch1;
    HTouchInfo m_LastTouch2;
	bool	m_LightFollowsCamera;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




