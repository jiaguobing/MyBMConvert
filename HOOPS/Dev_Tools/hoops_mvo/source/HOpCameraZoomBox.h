// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraZoomBox.h : interface of the HOpCameraZoomBox class
// zooms the camera based on user-defined screen-space window

#ifndef _HOPCAMERAZOOMBOX_H
#define _HOPCAMERAZOOMBOX_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructRectangle.h"

//! The HOpCameraZoomBox class zooms the camera to a selected area.
/*! 
  HOpCameraZoomBox utilizes the drawing functionality of HOpConstructRectangle to define a selection rectangle, 
  and then extends HOpConstructRectangle::OnLButtonUp to calculate a zoom factor and direction. 
  
  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the first point of the selection rectangle.
  <li>Left Button Down and Drag:	Rubberbands the rectangle to desired dimensions.
  <li>Left Button Up:				The is viewpoint calculated, camera position changed and the operation ends.
  </ol>
  More Detailed Description: see event methods.
*/
class MVO_API HOpCameraZoomBox : public HOpConstructRectangle
{
public:
	/*!
	  Constructs an HOpCameraZoomBox object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
      HOpCameraZoomBox (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraZoomBox();

	/*! \returns A pointer to a character string denoting the name of the operator which is 'HOpCameraZoomBox'. */
	virtual const char * GetName();  

	/*!
		OnLButtonUp passes rectangle information from HOpConstructRectangle to the HOOPS routine #HC_Compute_Selection_By_Area,
		then uses the information from the selection list to move the camera and point it in the right direction.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);

	/*!
		OnLButtonDblClk points the camera at the location where the mouse is double clicked. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDblClk(HEventInfo &hevent);
	
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


protected:
	bool ComputeReasonableTarget(HPoint & new_tar, HPoint const & min, HPoint const & max, HPoint const & tar_orig);
	void ComputeNewField(float & width, float & height,
		HPoint min, HPoint max,
		HPoint const & new_tar
		);

	bool	m_LightFollowsCamera;	
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



