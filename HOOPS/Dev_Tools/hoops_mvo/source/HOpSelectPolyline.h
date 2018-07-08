// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//	HOpSelectPolyline.h : interface of the HOpSelectPolyline class
//
//	Draws a polyline in the view plane based on pointer input
//	Polyline points are stored in protected member variables and ready after 
//	OnLButtonDblClk method has been called. 
//	Derived Classes should call this::OnLButtonDblClk in their overloaded implementation.
//	Polyline points are in window space.


#ifndef _HOPSELECTPOLYLINE_H
#define _HOPSELECTPOLYLINE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HOpConstructPolyline.h"


//! The HOpSelectPolyline class computes a selection list for objects intersected by a user-defined polyline.
/*!
  HOpSelectPolyline employs all of the drawing functionality of HOpConstructPolyline to define a temporary, overlayed polyline, then maps the 
  polyline to the HOOPS routine HC_Compute_Selection_By_Polylin. This provides the basic functionality for selecting a group
  of objects based on their intersection with a line. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				operation initiated, first point of the polyline recorded
  <li>No Button Down and Drag:		rubberband line segment to desired position of next point
  <li>Left Button Down:				next point in polyline recorded, etc.
  <li>Left Button Double Click:		polyline completed and flushed from scene, selection list computed, objects highlighted, operation ended
  </ol>
  More Detailed Description: see event methods
  Functional Note:  This operator does not provide exact results for HOOPS' Shell and Mesh primitives.
*/
class MVO_API HOpSelectPolyline : public HOpConstructPolyline
{
public:
	/*!
	  Constructs an HOpSelectPolyline object.
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
  	HOpSelectPolyline(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpSelectPolyline();

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpSelectPolyline'.
	*/
	virtual const char * GetName();  

	virtual HBaseOperator * Clone(); /*! returns a pointer to a copy of the operator */

	/*!
		OnLButtonDblClk passes the polyline array from HOpConstructPolyline::OnLButtonDblClk into the HOOPS routine
		HC_Compute_Selection_By_Polyline.  Currently selected items are highlighted, and items previously highlighted are
		de-selected.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
 	virtual int OnLButtonDblClk(HEventInfo &hevent);
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




