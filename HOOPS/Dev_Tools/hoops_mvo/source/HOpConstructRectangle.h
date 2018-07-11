// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructRectangle.h : interface of the HOpConstruction class
// draws a 2D box in view port based on pointer first position and last position
// Derived classes access the Points for the rectangle in HBaseOperator members
// m_ptFirst and m_ptLast; Points are stored in window space.


#ifndef _HOPCONSTRUCTRECTANGLE_H
#define _HOPCONSTRUCTRECTANGLE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpConstructRectangle class draws a rectangle. 
/*! 
  HOpConstructRectangle implements three of the mouse button event handlers and uses the event information to 
  draw a rectangle. This provides the basic functionality for drawing a temporary, overlayed 2-D box outline 
  in the view plane based on first and last pointer positions. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the first point of the rectangle.
  <li>Left Button Down and Drag:	Rubberbands the rectangle to the desired dimensions.
  <li>Left Button Up:				The rectangle finalized and the polyline primitive used to represent the rectangle is flushed from the scene.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpConstructRectangle : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpConstructRectangle object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	  \param UseCenterMarker Pass true if you want HOOPS to render a mark at the center of the rectangle.
	*/  	
  	HOpConstructRectangle(HBaseView* view, int DoRepeat=0, int DoCapture=1, bool UseCenterMarker=true);
	virtual ~HOpConstructRectangle();


	/*!
		\returns A pointer to a character string denoting the name of the operator, which is 'HOpConstructRectangle'.
	*/
	virtual const char * GetName();  

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpConstructRectangle object.
	*/
	virtual HBaseOperator * Clone();
     


	/*!
		OnLButtonDown records the first mouse position and initiates the rectangle-drawing mechanism.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove draws a rubberbanded line from the first point in the rectangle to the current mouse position. 
		Note that the basic drawing work is done by HUtility::InsertRectangle, while OnLButtonDownAndMove keeps track of 
		the current points and draws the rubberband centroid.
		\param hevent An HEventInfo object containing information about the current event.  
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);
  
	/*!
		OnLButtonUp finalizes the size of the rectangle and cleans up.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent); 
	/*!
		OnRButtonUp passes a right-button up event to OnLButtonUp.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnRButtonUp(HEventInfo &hevent); 

protected:
	
	bool	m_bRectangleExists; /*!< The boolean indicating whether the rectangle exists. */
	HPoint	m_ptRectangle[2];   /*!< The first and last points in the rectangle stored in window space. */
	char    m_pSavedVisibility[MVO_BUFFER_SIZE];
	char    m_pSavedColor[MVO_BUFFER_SIZE];

private:
	bool	m_bUseCenterMarker;	
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




