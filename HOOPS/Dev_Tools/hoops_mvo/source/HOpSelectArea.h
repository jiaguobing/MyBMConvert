// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpSelectArea.h : interface of the HOpSelectArea class, derived from HOpSelect
// Handles area selection

#ifndef _HOPSELECTAREA_H
#define _HOPSELECTAREA_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructRectangle.h" 


//! The HOpSelectArea class computes a selection list for objects inside a rectangular area.
/*!
  HOpSelectArea employs all of the drawing functionality of HOpConstructRectangle to define a temporary, overlayed rectangular selection 
  area, then maps the rectangle information to the HOOPS routine HC_Compute_Selection_By_Area.  This class provides the basic
  functionality for window selection.
  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				first point of rectangle
  <li>Left Button Down and Drag:	rubberband rectangle to desired dimensions
  <li>Left Button Up:				rectangled completed and flushed from scene, selection list computed, objects highlighted, operation ended
  </ol>
  More Detailed Description: see event methods 
*/
class MVO_API HOpSelectArea : public HOpConstructRectangle
{
public:
	/*!
	  Constructs an HOpSelectArea object
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
    HOpSelectArea(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpSelectArea();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpSelectArea'.
	*/
	virtual const char * GetName();  

	/*!
		This method calls through to HOpConstructRectangle's OnLButtonDown method to record the first point and initiates
		the rectangle drawing mechanism.  Then it sets attribute values for color and visibility that 
		are specific to the selection by area.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		This method maps rectangle information from HOpConstructRectangle into the HOOPS routine 
		HC_Compute_Selection_By_Area to update the selection set in the associated view.  It also 
		deselects any selection items that are not present in the current selection list.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);  

	/*! 
	  Creates a new operator which is associated with the same view as the current operator. 
	  The user is responsible for deleting the newly created operator.
	  \return A pointer to a copy of this operator
	  */
	virtual HBaseOperator * Clone(); 
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




