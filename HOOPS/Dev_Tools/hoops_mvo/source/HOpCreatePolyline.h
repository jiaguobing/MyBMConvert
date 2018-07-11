// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//	HOpCreatePolyline.h : interface of the HOpCreatePolyline class
//
//	Draws a polyline in the view plane based on pointer input
//	Polyline points are stored in protected member variables and ready after 
//	OnLButtonDblClk method has been called. 
//	Derived Classes should call this::OnLButtonDblClk in their overloaded implementation.
//	Polyline points are in window space.


#ifndef _HOPCreatePOLYLINE_H
#define _HOPCreatePOLYLINE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseModel.h"
#include "HBaseView.h"
#include "HBaseOperator.h"
#include "HEventInfo.h"
#include "HOpConstructPolyline.h"


//! The HOpCreatePolyline class draws a polyline. 
/*! 
  HOpCreatePolyline maps event information to the HOOPS routine HC_Insert_Polyline. This provides the basic functionality 
  for dynamically creating a temporary overlayed polyline in the view plane based on pointer input. The polyline points are stored in a 
  protected member array until a double click event is passed, then the array is inserted into the HOOPS database. 
  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated and the first point of the polyline is recorded.
  <li>No Button Down and Drag:		Rubberband line segment to the desired position of the next point.
  <li>Left Button Down:				The next point in polyline is recorded and continues until the user double clicks the left mouse button.
  <li>Left Button Double Click:		The polyline is finalized and the polyline primitive is flushed from the scene and a new one is inserted into the model.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpCreatePolyline : public HOpConstructPolyline
{
public:
	/*!
	  Constructs an HOpCreatePolyline object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
  	HOpCreatePolyline(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCreatePolyline();

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreatePolyline object.
	*/
	virtual HBaseOperator * Clone();

	/*!
		\returns A pointer to a character string denoting the name of the operator, which is 'HOpCreatePolyline'.
	*/
	virtual const char * GetName();  
	
	/*!
		OnLButtonDblClk places the final mouse position into the temp array, then inserts the array into the database as 
		a HOOPS polyline.  This function also takes care of any cleanup.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDblClk(HEventInfo &hevent);
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




