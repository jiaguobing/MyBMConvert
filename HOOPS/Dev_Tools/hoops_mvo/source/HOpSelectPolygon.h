// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//	HOpSelectPolygon.h : interface of the HOpSelectPolygon class
//
//	Draws a polyline in the view plane based on pointer input
//	Polyline points are stored in protected member variables and ready after 
//	OnLButtonDblClk method has been called. 
//	Derived Classes should call this::OnLButtonDblClk in their overloaded implementation.
//	Polyline points are in window space.


#ifndef _HOPSELECTPOLYGON_H
#define _HOPSELECTPOLYGON_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HOpConstructPolyline.h"


//! The HOpSelectPolygon class computes a selection list for objects inside a user-defined polygonal area.
/*!
  HOpSelectPolygon employs all of the drawing functionality of HOpConstructPolyline to define a temporary, overlayed polygonal selection 
  area, and then maps the polygon information to the HOOPS routine HC_Compute_Selection_By_Polygon. The operation consists 
  of the following steps:
  <ol>
  <li>Left Button Down:				operation initiated, first point of the polygon recorded
  <li>No Button Down and Drag:		rubberband line segment to desired position of next point
  <li>Left Button Down:				next point in polygon recorded, etc.
  <li>Left Button Double Click:		polygon completed, and flushed from scene, selection list computed, objects highlighted, operation ended
  </ol>
  More Detailed Description: see event methods 
  Functional Note:  This operator does not provide exact results for HOOPS' Shell and Mesh primitives.
*/
class MVO_API HOpSelectPolygon : public HOpConstructPolyline
{
public:
	/*!
	  Constructs an HOpSelectPolygon object.
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
	HOpSelectPolygon(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpSelectPolygon();

	/*!
		\return A pointer to a character string denoting the name of the operator 'HOpSelectPolygon'.
	*/
	virtual const char * GetName();  

	virtual HBaseOperator * Clone(); /*! returns a pointer to a copy of the operator */


	/*!
		OnLButtonDblClk passes the polyline array from HOpConstructPolyline::OnLButtonDblClk into the HOOPS routine
		HC_Insert_Polygon and inserts the polygon into the scene.  The polyline information is also passed to
		Compute_Selection_By_Polygon.  Currently selected items are highlighted, and items previously highlighted are
		de-selected.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
 	virtual int OnLButtonDblClk(HEventInfo &hevent);

	/*!
		OnLButtonDown initiates the polygon selection mechanism.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent); 

private:
	bool	m_bFirstTimeThrough;	// bool used to determine if we flush out the selection polygon
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




