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

#ifndef _HOPCAMERAPAN_H
#define _HOPCAMERAPAN_H  

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpCameraPan class pans the camera about a current view.
/*! 
  HOpCameraPan implements three mouse event methods defined on the base class and maps the event information
  to HOOPS camera routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The pan is initiated.
  <li>Left Button Down and Drag:	The camera pans.
  <li>Left Button Up:				The operation ends.
  </ol>
  More Detailed Description: see event methods
*/
class MVO_API HOpCameraPan : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraPan object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
    HOpCameraPan(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraPan();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpCameraPan'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the initial mouse position. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove records points as the mouse is moved and calculates the values used for panning the camera 
		by modifying the camera's target and position.  It also updates the position of the default light, depending 
		on whether you have set the light to follow the camera.
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
	  \return A pointer to the newly created HOpCameraPan object.
	*/
	virtual HBaseOperator * Clone();  

private:
	bool	m_LightFollowsCamera;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




