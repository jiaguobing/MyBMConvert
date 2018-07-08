// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraManipulateTouch.h : interface of the HOpCameraManipulateTouch class
// Manipulates the camera of current view usual a virtual trackball
// interface

#ifndef _HOpCameraManipulateTouch_H
#define _HOpCameraManipulateTouch_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
class HOpCameraRelativeOrbit;
class HOpCameraOrbit;
class HOpCameraPan;
class HOpCameraZoom;
class HTClient;
class HEventInfo;
//! The HOpCameraManipulateTouch class combines the touch versions of the Orbit, Pan and Zoom Operators 
/*! 
  HOpCameraManipulateTouch implements three touch event methods defined on the base class and maps the event information to 
  the touch versions of orbit, pan, zoom and roll of the camera. The current implementation uses a single touch for orbit and two touches for pan, zoom and roll
  respectively
  More Detailed Description: see event methods.
*/

class MVO_API HOpCameraManipulateTouch : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraManipulateTouch object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	  \param orbit A pointer to an HOpCameraOrbitTouch object.  If you pass null, HOpCameraManipulateTouch will make construct a new one.
	  \param pan A pointer to an HOpCameraPanTouch object.  If you pass null, HOpCameraManipulateTouch will make construct a new one.
	  \param zoom A pointer to an HOpCameraZoomTouch object.  If you pass null, HOpCameraManipulateTouch will make construct a new one.
	  \param roll A pointer to an HOpCameraRollTouch object.  If you pass null, HOpCameraManipulateTouch will make construct a new one.
	*/  	
	HOpCameraManipulateTouch (HBaseView* view, int DoRepeat=0, int DoCapture=1, HBaseOperator *orbit = 0, HBaseOperator *pan = 0, HBaseOperator *zoom = 0, HBaseOperator *roll = 0);
	virtual ~HOpCameraManipulateTouch();

	/*! \return A pointer to a character string denoting the name of the operator which is 'HOpCameraManipulateTouch'. */
	virtual const char * GetName();  


	/*!	
		This method handles the touches down event.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnTouchesDown (HEventInfo &hevent);


	/*!
		This method handles the touches moved event
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnTouchesMove(HEventInfo &hevent);



	/*!
		This method handles the touches up event.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnTouchesUp (HEventInfo &hevent);

	
	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCameraManipulateTouch object.
	*/
	virtual HBaseOperator * Clone(); 

	virtual bool OperatorStarted( void );
 


    
protected:

  
 	HBaseOperator	*m_pOrbitOperator;	     /*!< A pointer to the orbit operator. */
 	HBaseOperator	*m_pPanOperator;	     /*!< A pointer to the pan operator. */
	HBaseOperator	*m_pZoomOperator;	     /*!< A pointer to the zoom operator. */
	HBaseOperator	*m_pRollOperator;	     /*!< A pointer to the zoom operator. */

private:
	HEventInfo *m_LastEvent;
 	float		m_time;

};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




