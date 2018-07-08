// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraOrbit.h : interface of the HOpCameraOrbit class
// orbits the camera of current view usual a virtual trackball
// interface

#ifndef _HOPCAMERAORBIT_H
#define _HOPCAMERAORBIT_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


//! The HOpCameraOrbit class provides interactive orbiting of the camera around a scene.
/*! 
  HOpCameraOrbit implements three mouse event methods defined on the base class and maps the event information to 
  the HOOPS routine #HC_Orbit_Camera. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The orbit is initiated.
  <li>Left Button Down and Drag:	The camera orbits.
  <li>Left Button Up:				The operation ends.
  </ol>
  As the mouse moves through the scene, the 2D position of the pointer is mapped into 3D space like a 
  trackball around the model.  More Detailed Description: 
  see event methods.
*/
class MVO_API HOpCameraOrbit : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraOrbit object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
  	HOpCameraOrbit (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCameraOrbit();

	/*!
		\returns A pointer to a character string denoting the name of the operator which is 'HOpCameraOrbit'.
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonDown draws a bounding box around the scene, records the mouse position, and maps the mouse 
		position to spherical coordinates.  
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown (HEventInfo &hevent);


	/*!
		OnLButtonDownAndMove records points as the mouse is moved, maps the points to spherical coordinates, and calculates 
		the appropriate values for #HC_Orbit_Camera and #HC_Roll_Camera.  It also updates the position of the default light 
		depending on if you have set the light to follow the camera.   
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);



	/*!
		OnLButtonUp updates the scene with the most recent camera and lighting information and cleans up. 
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
	void SetLightFollowsCamera(bool follow){m_bLightFollowsCamera = follow;};

	/*! \return True if the  light follows the camera or false if it doesn't. */
	bool GetLightFollowsCamera(){return m_bLightFollowsCamera;};

	/*!
		SetSceneBoundingBoxVis controls whether a bounding box denoting the scene extents should be drawn during 
		a camera orbit.  
		\param vis Pass true if you want HOOPS to draw the bounding box or false if you don't.
	*/
	void SetSceneBoundingBoxVis(bool vis){m_bSceneBoundingBoxVis = vis;};

	/*! \return True if the bounding box is visible or false if it isn't. */
	bool GetSceneBoundingBoxVis(){return m_bSceneBoundingBoxVis;};


	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCameraOrbit object.
	*/
	virtual HBaseOperator * Clone();  

	/*! For internal use only! */
	float m_Angle2, m_Angle1, m_Angle3;  /*these variable should not be public*/

	/*! \return True if the mouse has moved between the left mouse button down and left mouse button up events. */
	bool GetSingleClick() { return m_bSingleClick; }

protected:

	bool	m_bLightFollowsCamera;  /*!< This member indicates whether the light should follow the camera. */
	bool	m_bSceneBoundingBoxVis; /*!< This member indicates whether a bounding box denoting the scene extents should be drawn during a camera orbit. */
	bool	m_bSingleClick;		/*!< This member indicates whether the mouse has moved in between lbuttondown/lbuttonup. */

	HPoint	m_ptRealOld;			/*!< For internal use only. */

private:
	HPoint m_ptRealFirst;
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



