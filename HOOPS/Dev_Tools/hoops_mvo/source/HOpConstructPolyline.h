// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//	HOpConstructPolyline.h : interface of the HOpConstructPolyline class
//
//	Draws a polyline in the view plane based on pointer input
//	Polyline points are stored in protected member variables and ready after 
//	OnLButtonDblClk method has been called. 
//	Derived Classes should call this::OnLButtonDblClk in their overloaded implementation.
//	Polyline points are in window space.


#ifndef _HOPCONSTRUCTPOLYLINE_H
#define _HOPCONSTRUCTPOLYLINE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


//! The HOpConstructPolyline class draws a polyline. 
/*! 
  HOpConstructPolyline implements three of the mouse button event handlers defined on the base class and maps the event
  information to create a polyline. This provides the basic functionality for dynamically creating a 
  temporary, overlayed polyline in the view plane based on pointer input. The polyline points are stored in a 
  protected member array until a double click event is passe. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated and the first point of the polyline is recorded.
  <li>No Button Down and Drag:		Rubberband line segment to the desired position of the next point.
  <li>Left Button Down:				The next point in polyline is recorded and continues until the user double clicks the left mouse button.
  <li>Left Button Double Click:		The polyline is finalized and the geometry is flushed from the scene.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpConstructPolyline : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpConstructPolyline object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
  	HOpConstructPolyline(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpConstructPolyline();

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpConstructPolyline object.
	*/
	virtual HBaseOperator * Clone(); 

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpConstructPolyline'.
	*/
	virtual const char * GetName();  
	
	HC_KEY	m_PolylineKey;		/*!< The key for HOOPS polyline. */




	/*!
		OnNoButtonDownAndMove draws a rubberbanded line from the previous point in the polyline to the current mouse 
		position.  
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonDown records the first mouse position and initiates the polyline-drawing mechanism. The function also 
		records each successive point in the polyline in a temp array, and connects the points with a line.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonDblClk places the final mouse position into the temp array.  This function also takes care of any cleanup.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDblClk(HEventInfo &hevent);

protected:

	/*! the last point in the temporary polyline array (not m_pPolyline) behind the current mouse position. 
	See OnLButtonDown. */
	HPoint	m_ptLast;
	HPoint*	m_pPolyline;		/*!< A pointer to the array of points for HOOPS Polyline in World Space. */
	int		m_PolylineCount;	/*!< The number of points in the polyline. */
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



