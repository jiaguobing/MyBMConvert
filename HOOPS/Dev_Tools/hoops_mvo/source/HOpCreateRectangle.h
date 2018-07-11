// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateRectangle.h : interface of the HOpCreateion class
// draws a 2D box in view port based on pointer first position and last position
// Derived classes access the Points for the rectangle in HBaseOperator members
// m_ptFirst and m_ptLast; Points are stored in window space.

#ifndef _HOPCREATERECTANGLE_H
#define _HOPCREATERECTANGLE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructRectangle.h"

//! The HOpCreateRectangle class draws a rectangle. 
/*! 
  HOpCreateRectangle uses event information to insert a rectangle into the HOOPS Database. This provides the basic 
  functionality for drawing a temporary, overlayed 2-D box outline in the view plane based on 
  first and last pointer positions. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the first point of the rectangle.
  <li>Left Button Down and Drag:	Rubberbands the rectangle to the desired dimensions.
  <li>Left Button Up:				The rectangle finalized and the polyline primitive used to represent the rectangle is flushed from the scene and a new rectangle is inserted into the model.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpCreateRectangle : public HOpConstructRectangle
{
public:
	/*!
	  Constructs an HOpCreateRectangle object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	  \param UseCenterMarker Pass true if you want HOOPS to render a mark at the center of the rectangle.
	*/  	
  	HOpCreateRectangle(HBaseView* view, int DoRepeat=0, int DoCapture=1, bool UseCenterMarker=true);
	virtual ~HOpCreateRectangle();

	/*!
		\returns A pointer to a character string denoting the name of the operator, which is 'HOpCreateRectangle'.
	*/
	virtual const char * GetName();  

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateRectangle object.
	*/
	virtual HBaseOperator * Clone(); 
     
	/*!
		OnLButtonUp finalizes the size of the rectangle, cleans up the scene and inserts a new rectangle into the model.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent); 
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



