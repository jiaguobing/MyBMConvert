// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateCuboid.h : interface of the HOpCreateCuboid class
// Creates a cuboid using left mouse down and drag

#ifndef _HOPCREATECUBOID_H
#define _HOPCREATECUBOID_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructCuboid.h"

//! The HOpCreateCuboid class creates a rectangular cube.
/*! 
  HOpCreateCuboid maps the event information to the HOOPS Routine #HC_Insert_Shell. This provides the basic functionality 
  for creating a three-dimensional, six-sided cuboid or rectangular box.  The cuboid that is generated includes two square 
  faces with the same length and width while the depth is variable. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The first point of square base.
  <li>Left Button Down and Drag:	Rubberband the base to the desired dimensions.
  <li>Left Button Up:				The base of the cuboid dimension is finalized.
  <li>No Button Down and Drag:		Rubberband the cuboid to the desired depth.
  <li>Left Button Down:				The depth of the cuboid finalized, the operation ends and a new cuboid is inserted into the model.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API  HOpCreateCuboid : public HOpConstructCuboid
{
public:
	/*!
	  Constructs an HOpCreateCuboid object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCreateCuboid (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCreateCuboid();

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpCreateCuboid'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first mouse position and initiates the cuboid-drawing mechanism.  It tests
		for the case that the mechanism has already been started such as OnLButtonDown having been called once already.  
		In this case, OnLButtonDown finalizes the cuboid's depth, inserts the cuboid into the 
		HOOPS database, updates the display, and ends the operation.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int	OnLButtonDown(HEventInfo &hevent);


	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateCuboid object.
	*/
	virtual HBaseOperator * Clone(); 
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




