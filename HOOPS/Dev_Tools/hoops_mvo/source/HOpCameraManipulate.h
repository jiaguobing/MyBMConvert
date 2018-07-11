// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCameraManipulate.h : interface of the HOpCameraManipulate class
// Manipulates the camera of current view usual a virtual trackball
// interface

#ifndef _HOPCAMERAManipulate_H
#define _HOPCAMERAManipulate_H

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
//! The HOpCameraManipulate class combines the Orbit, Pan and Zoom Operators 
/*! 
  HOpCameraManipulate implements three mouse event methods defined on the base class and maps the event information to 
  orbit, pan or zoom the camera. This operator has three modes of operation.
  <ol>
  <li>With no key press, the camera orbits.
  <li>With the Shift key down, the camera pans.
  <li>With the Ctrl key down, the camer zooms.
  </ol>
  More Detailed Description: see event methods.
*/

class MVO_API HOpCameraManipulate : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpCameraManipulate object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	  \param orbit A pointer to an HOpCameraOrbit object.  If you pass null, HOpCameraManipulate will make construct a new one.
	  \param pan A pointer to an HOpCameraPan object.  If you pass null, HOpCameraManipulate will make construct a new one.
	  \param zoom A pointer to an HOpCameraZoom object.  If you pass null, HOpCameraManipulate will make construct a new one.
	  \param relative A pointer to an HOpCameraRelativeOrbit object.  If you pass null, HOpCameraManipulate will make construct a new one.
	  \param AllowHandleActivation Pass true to allow manipulation using handles.
	*/  	
	HOpCameraManipulate (HBaseView* view, int DoRepeat=0, int DoCapture=1, HBaseOperator *orbit = 0, HBaseOperator *pan = 0, HBaseOperator *zoom = 0, HBaseOperator *relative = 0, bool AllowHandleActivation = false);
	virtual ~HOpCameraManipulate();

	/*! \return A pointer to a character string denoting the name of the operator which is 'HOpCameraManipulate'. */
	virtual const char * GetName();  


	/*!	
		This method handles the left mouse button down event.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown (HEventInfo &hevent);


	/*!
		This method handles the left mouse button down and move event.  It determines if the shift or 
		control key is down and then passes control over to the correct camera operator which can be 
		pan, orbit or zoom.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);



	/*!
		This method handles the left mouse button up event.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp (HEventInfo &hevent);

	
	/*!	
		This methods passes the event to the relative orbit camera operator's left button mouse down event handler.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonDown (HEventInfo &hevent);


	/*!
		This methods passes the event to the relative orbit camera operator's left button mouse down and move event handler.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonDownAndMove(HEventInfo &hevent);



	/*!
		This methods passes the event to the relative orbit camera operator's left button mouse up event handler.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnMButtonUp (HEventInfo &hevent);

	
	/*!	
		This methods passes the event to the pan camera operator's left button mouse down event handler.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDown (HEventInfo &hevent);


	/*!
		This methods passes the event to the pan camera operator's left button mouse down and move event handler.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonDownAndMove(HEventInfo &hevent);



	/*!
	This event handler passes the event to the pan camera operator's OnLButtonUp handler.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonUp (HEventInfo &hevent);

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCameraManipulate object.
	*/
	virtual HBaseOperator * Clone(); 

	virtual bool OperatorStarted();
 
    /*!  
	 \return A pointer to the orbit operator.
    */    
	HOpCameraOrbit * GetOrbitOperator() { return reinterpret_cast<HOpCameraOrbit *>(m_pOrbitOperator); }


    /*!  
	 This method is the double click function called from timer callback.
 	  \param requested_time The time that the event had requested.
      \param actual_time The actual time the event callback happened.    
    */    
        void HandleDblClk(float requested_time, float actual_time);

    /*!  
	 \return The state of the left button down flag.  
    */    
	bool GetLButtonDown() { return m_bLButtonDown; }
 	
protected:

	HTClient	*m_pDblClkTimer;	     /*!< A pointer to the timer client for detection of double click events. */
  
 	HBaseOperator	*m_pOrbitOperator;	     /*!< A pointer to the orbit operator. */
 	HBaseOperator	*m_pRelativeOrbitOperator;   /*!< A pointer to the relative orbit operator. */
	HBaseOperator	*m_pPanOperator;	     /*!< A pointer to the pan operator. */
	HBaseOperator	*m_pZoomOperator;	     /*!< A pointer to the zoom operator. */

	HPoint	m_ptRealOld;			     /*!< For internal use only. */
private:
	bool m_bAllowHandleActivation; 
	bool m_bLButtonDown;
	bool m_bLButtonUp;
	bool m_bForceDown;
	HPoint m_ptRealFirst;
	HEventInfo *m_LastEvent;
 	float		m_time;

};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




