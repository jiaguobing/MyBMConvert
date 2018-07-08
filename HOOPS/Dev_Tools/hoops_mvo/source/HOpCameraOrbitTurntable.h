// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraOrbitTurntable.h : interface of the HOpCameraOrbitTurntable class
// pans the camera of current view

#ifndef _HOpCameraOrbitTurntable_H
#define _HOpCameraOrbitTurntable_H  

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpCameraOrbitTurntable class orbits the camera around a predefined vertical and horizontal axis
/*! 
  HOpCameraOrbitTurntable takes the fixed "Top Axis" of its associated view and allows the user to rotate around it 
  by dragging the mouse horizontally across the screen. With the mouse wheel the user can also rotate the scene around
  the horizontal view axis which is dependent on the current camera position
 
  HOpCameraOrbitTurntable implements three mouse event methods defined on the base class and maps the event information
  to HOOPS camera routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The orbit is initiated.
  <li>Left Button Down and Drag:	The camera orbits.
  <li>Left Button Up:				The operation ends.
  </ol>
  More Detailed Description: see event methods
*/
class MVO_API HOpCameraOrbitTurntable : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraOrbitTurntable object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
    HOpCameraOrbitTurntable(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraOrbitTurntable();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpCameraOrbitTurntable'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the initial mouse position. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove records points as the mouse is moved and calculates the values used for rotating the camera
		by modifying the camera's target and position.  
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp Clean up. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);

	/*!
		OnMouseWheel OnMouseWheel performs rotation around the horizontal view axis
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnMouseWheel (HEventInfo &hevent);
 	
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
		Sets the center of rotation for the horizontal and vertical rotation
		\param center Center Point
	*/
	void SetCenter(HPoint center) { m_Center = center; m_bCenterPointSet = true; }
	/*!
		Indicates if center of rotation is set externally. If not set center of rotation will be calculated
		based on scene bounding box
	*/
	void UnSetCenter() { m_bCenterPointSet = false; }
	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCameraOrbitTurntable object.
	*/
	virtual HBaseOperator * Clone();  

protected:

	/*! 
	  Rotates camera around given axis
	  \param axis Axis to rotate around
	  \param rf Angle of rotation
 	*/
	void RotateAroundAxis(HPoint axis, float rf);

private:

 
	bool	m_LightFollowsCamera;
	bool	m_bDoOnce;
	bool	m_bCenterPointSet;
	HPoint	m_Center;

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




