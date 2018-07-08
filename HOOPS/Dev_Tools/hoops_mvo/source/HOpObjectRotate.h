// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpObjectRotate.h : interface of the HOpObjectRotate class
// rotate items in the selection list of the associated view 

#ifndef _HOPOBJECTROTATE_H
#define _HOPOBJECTROTATE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


//! The HOpObjectRotate class rotates a selected object.
/*!
  HOpObjectRotate implements three of the mouse event handlers defined on the base class and utilizes the event 
  information to rotate the currently selected objects.  This provides the basic functionality for applying a rotation 
  transform to an object (as opposed to a global camera transform which affects the entire scene). The operation 
  consists of the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated and an object is selected.
  <li>Left Button Down and Drag:	The object is rotated.
  <li>Left Button Up:				The rotation is finalized and the operation ends.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpObjectRotate : public HBaseOperator
{
public:
	/*! Constructs an HOpObjectRotate object.
	  \param view A pointer to an HBaseView object,
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
    HOpObjectRotate (HBaseView* view, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpObjectRotate();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpObjectRotate'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown draws a bounding box around the object to be rotated.  It records the mouse position and sets up the 
		transform environment.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove computes the rotation transform based on the distance between the first and current pointer 
		positions, and rotates the object accordingly.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp cleans up the construction segments used during the transformations.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);

	/*!
		OnLButtonDblClk moves the object back to its original location
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDblClk (HEventInfo &hevent);

	/*!Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpObjectRotate object.
	  */
	virtual HBaseOperator * Clone(); 

protected:
	HPoint m_ptCentroid;			/*!< The centroid of the segment to be rotated in window space. */
	HPoint m_ptCentroidWorld;	    /*!< The centroid of the segment to be rotated in world space. */
	HPoint m_ptCentroidViewpoint;	/*!< The centroid of the segment to be rotated in viewpoint space. */
	float  m_radius;                /*!< The radius of the trackball (world/viewpoint dimension). */
	bool	m_bTemporarySelection;	/*!< This boolean indicates whether the object was temporarily selected. */
	bool m_bInitialMatrix;			/*!< This stores the initial modelling matrix. */

private:
	void RotateSegmentLocally (HC_KEY	key, HPoint	*rotate_object, float theta);
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






