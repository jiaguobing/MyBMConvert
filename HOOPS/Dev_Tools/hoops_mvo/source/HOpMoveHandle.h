// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMoveHandle.h : interface of the HOpMoveHandle class
// selection/movement of geometry handles

#ifndef _HOpMoveHandle_H
#define _HOpMoveHandle_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HUtilityGeomHandle.h"

//! The HOpMoveHandle class selects/moves "handles" and their associated geometry
/*! 
  HOpMoveHandle implements three mouse event methods defined on the base class and maps the event information
  to operator routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The new handle object is selected.
  <li>Left Button Down and Drag:	Dragging the handle and associated geometry.
  <li>Left Button Up:				The operation ends.
  </ol>
  More Detailed Description: see event methods.
*/
class  MVO_API HOpMoveHandle : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpMoveHandle object.
	  \param view A pointer to an HBaseView object.
	  \param allowSelect Pass true to allow selection of different handle objects.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
      HOpMoveHandle (HBaseView* view, bool allowSelect = true, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpMoveHandle();


	/*!
		\returns A pointer to a character string denoting the name of the operator which is 'HOpMoveHandle'.
	*/
  	virtual const char * GetName();  

	/*!
		OnLButtonDown selects an existing handle if the mouse is over a handle or designates a new object
		for handle based movement. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove moves/rotates/scales the handle and it's associated geometry based on various constraints 
		(ray, plane, etc).
 		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnNoButtonDownAndMove activates handle in mouse cursor range.
 		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp finalizes mouse movement and deselectes the active handle.
 		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);
 

	//! This method finds a handle based on the mouse position.
	HC_KEY FindHandle(HEventInfo &hevent);

	/*!
		This method finds geometry based on mouse position.
 		\param hevent An HEventInfo object containing information about the current event.
 		\param key Key of currently selected object.
 		\param exclude Pass true to exclude currently selected object from search.
		\return The key of the geometry at mouse position that serves as new handle target.
	*/
	HC_KEY FindObject(HEventInfo &hevent, HC_KEY key, bool exclude = false);

	/*!
		Creates handles for the choosen geometry.
  		\param key  The key of the geometry to attach handles to.
 	*/
	void SetupObject(HC_KEY key);

	//! Reset the operator so that no object or handle is currently selected for manipulation.
	void Reset();	
	
	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpMoveHandle object.
	*/
	virtual HBaseOperator * Clone(); 

protected:
 
 	HC_KEY m_pHandleKey;	//!< The key of the active handle.
	HC_KEY m_pObjectKey;    //!< The key of the handle object.
	bool m_bAllowSelect;    //!< If the value is true, this class allows selection of different handle objects.  This is disabled for cutting planes.

private:
	HPoint m_windowspaceDelta;

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



