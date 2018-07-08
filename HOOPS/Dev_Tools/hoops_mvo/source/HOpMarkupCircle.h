// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupCircle.h : interface of the HOpMarkupCircle class, derived from HOpSelect
// Handles area selection

#ifndef _HOPMarkupCircle_H
#define _HOPMarkupCircle_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructCircle.h" 


//! The HOpMarkupCircle class inserts a 2D circle into a markup layer.  If a layer is already active, then the operator will use that layer. Otherwise, the operator will create a new unnamed layer.
/*! 
  HOpMarkupCircle implements three mouse event methods defined on the base class and maps the event information
  to operator routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the center point of the circle.
  <li>Left Button Down and Drag:	Creates a circle of appropriate radius in the contruction layer.
  <li>Left Button Up:				Depending on existence of an active markup layer it either opens the existing or creates a new markup layer and inserts appropraitely sized circle into it.
  </ol>
  More Detailed Description: see event methods.
*/ 
class MVO_API HOpMarkupCircle : public HOpConstructCircle
{
public:
/*!
	  Constructs an HOpMarkupCircle object.
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
    HOpMarkupCircle(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpMarkupCircle();

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpMarkupCircle'.
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonUp maps circle information from HOpConstructCircle into the HOOPS routine 
		HC_Compute_Selection_By_Area.  This function also deselects any selected items that are not present in the 
		current selection list.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);  

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpMarkupCircle object.
	*/
	virtual HBaseOperator * Clone();  
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif










