// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateCylinder.h : interface of the HOpCreateCylinder class
// Creates a cylinder using left mouse down and drag

#ifndef _HOPCREATECYLINDER_H
#define _HOPCREATECYLINDER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructCylinder.h"

//! The HOpCreateCylinder class provides support for constructing a cylinder and inserting it into the model. 
/*! 
  HOpCreateCylinder implements four of the mouse button event handlers defined on the base class and maps the event 
  information to local methods that rubberband a cylinder and insert a corresponding HOOPS shell into the HOOPS 
  database.  This provides the basic functionality for drawing a  three-dimensional, tesselated cylinder with a 
  circular base, a circular cap of equivalent size, and a number of rectangular, facetted sides with the default 
  as 30. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Register the center point of the cylinder base.
  <li>Left Button Down and Drag:	Rubberbands the circular base to the desired circumference.
  <li>Left Button Up:				The base circumference finalized.
  <li>No Button Down and Drag:		Rubberbands the cylinder to the desired height.
  <li>Left Button Down:				The height is finalized, a cylinder is inserted into the model and the operation ends.	
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API  HOpCreateCylinder : public HOpConstructCylinder
{
public:
	/*!
	  Constructs an HOpCreateCylinder object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCreateCylinder (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCreateCylinder();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'Create Cylinder'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first pointer position and initiates the cylinder-drawing mechanism.  The first point is 
		used as the center of the cylinder's circular base.  If the drawing mechanism has already been started such as
		LButtonDown having been called once already, LButtonDown calls HUtilityGeometryCreation::CreateCylinder.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int	OnLButtonDown(HEventInfo &hevent);

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateCylinder object.
	*/
	virtual HBaseOperator * Clone(); 
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




