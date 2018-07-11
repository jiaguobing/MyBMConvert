// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCircle.h : interface of the HOpConstruction class
// draws a 2D box in view port based on pointer first position and last position
// Derived classes access the Points for the Circle in HBaseOperator members
// m_ptFirst and m_ptLast; Points are stored in window space.


#ifndef _HOPCONSTRUCTCIRCLE_H
#define _HOPCONSTRUCTCIRCLE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpConstructCircle class draws a circle. 
/*! 
  HOpConstructCircle implements three of the mouse button event handlers and uses the event information to draw a circle. 
  This class provides the basic functionality for drawing a temporary, overlayed 2-D box outline in the view plane based on 
  first and last pointer positions. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Identifies the center point of the circle.
  <li>Left Button Down and Drag:	Rubberband circle to desired dimensions.
  <li>Left Button Up:				Circle finalized and the polyline primitive used to represent the circle is flushed from the scene.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpConstructCircle : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpConstructCircle object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	  \param UseCenterMarker Pass true if you want HOOPS to draw a mark at the center.
	*/  	
	HOpConstructCircle(HBaseView* view, int DoRepeat=0, int DoCapture=1, bool UseCenterMarker=true);
	virtual ~HOpConstructCircle();

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpConstructCircle'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first mouse position and initiates the circle-drawing mechanism.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove draws rubberbanded lines along the circumference of the circle centered on the first 
		pointer position.  Note that the basic dynamic drawing work is done by repeated calls to the HUtility::CreateAxisCircle
		while OnLButtonDownandMove keeps track the changing radius value.
		\param hevent An HEventInfo object containing information about the current event.  
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp finalizes the size of the circle and cleans up the geometry that was drawn under the construction key.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent); 

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpConstructCircle object.
	*/
	virtual HBaseOperator * Clone(); 
 
    
protected:

	float	m_radius;       //!< The radius of the circle.
	HPoint	m_ptCircle[2];  //!< The first and last points in the circle which are stored in window space.

private:
	bool	m_bUseCenterMarker;	// Indicates whether a center marker should be drawn.

};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






