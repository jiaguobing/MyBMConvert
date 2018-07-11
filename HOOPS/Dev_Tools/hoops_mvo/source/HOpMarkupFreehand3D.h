// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//	HOpMarkupFreehand3D.h : interface of the HOpMarkupFreehand3D class
//
//	Draws a polyline in the view plane based on pointer input
//	Polyline points are stored in protected member variables and ready after 
//	OnLButtonDblClk method has been called. 
//	Derived Classes should call this::OnLButtonDblClk in their overloaded implementation.
//	Polyline points are in window space.

#ifndef _HOpMarkupFreehand3D_H
#define _HOpMarkupFreehand3D_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpMarkupFreehand3D inserts a 2D freehand polyline into a markup layer. If a layer is already active, then the operator will use that layer. Otherwise, the operator will create a new unnamed layer.
/*!
  HOpMarkupFreehand3D implements three of the mouse event handlers defined on the base class and maps the event information 
  to logic which inserts a polyline into the scene to serve as a 'MarkupFreehand'.   
  <ol>
  <li>Left Button Down:				operation initiated, markup begins
  <li>Left Button Down and Drag:	MarkupFreehand is incrementally built and inserted into the construction segment
  <li>Left Button Up:				depending on existence of an active markup layer it either opens the existing or creates a new markup layer and inserts freehand markup into it
  </ol>
  More Detailed Description: see event methods 
*/
class MVO_API HOpMarkupFreehand3D : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpMarkupFreehand3D object.
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
	HOpMarkupFreehand3D(HBaseView* view, int DoRepeat=0, int DoCapture=1);

	/*!
		CreateMarkupFreehand uses inserts the polyline representing the freehand drawing into the active markup layer.  If there is no active markup layer, then it creates a new unnamed layer.
		\param pView A pointer to the HBaseView object where the markup data should be inserted
		\param segment The name of segment within markup layer that the freehand data should be inserted into
		\param PolyLineLength Integer value indicating number of points in teh freehand polyline
		\param pPolyline An array of HPoints definind the freehand polyline 
		\param emit_message Determines if polyline creation is broadcasted to other HNet clients
	*/
	static void CreateMarkupFreehand(HBaseView *pView, const char *segment, unsigned int PolyLineLength, HPoint *pPolyline, bool emit_message);

	HBaseOperator * Clone();  /*! returns a pointer to a copy of the operator */
	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpMarkupFreehand3D'.
	*/
	virtual const char * GetName();  
	
	HC_KEY	m_lPolyline;		//!< key for HOOPS polyline


	/*! 
		OnLButtonDownAndMove computes the line segment to be incrementally added to the existing polyline in the
		current markup layer. Note this method acts only if
		selection items that are part of the associated model.
		\param hevent An HEventInfo object containing information about the current event.  Note this method acts only if
		selection items that are part of the associated model.
		\return A value indicating the result of the event handling.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonDown records the first pointer position and initializes the redlining mechanism
		by creating a polyline in the current markup layer which will be incrementally built.  Note this method acts only if
		selection items that are part of the associated model.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonUp cleans up the construction segments used during the redling and inserts the 
		polyline representing the 'MarkupFreehand' into the scene.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent);

protected:

    /*! Verifies if selected geometry is part of the associated model geometry. */

    bool IsModelGeometry();

	HPoint	m_ptLast; //!< most recent construction point
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



