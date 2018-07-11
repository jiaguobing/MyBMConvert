// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupRectangle.h : interface of the HOpMarkupRectangle class, derived from HOpSelect
// Handles area selection

#ifndef _HOPMarkupRectangle_H
#define _HOPMarkupRectangle_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructRectangle.h" 


//! The HOpMarkupRectangle class inserts a 2D rectange into a markup layer.  If a  markup layer is already active then the operator will use that layer otherwise the operator will create a new, unamed layer.
/*! 
  HOpMarkupRectangle implements three mouse event methods defined on the base class and maps the event information
  to operator routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				register corner of rectangle
  <li>Left Button Down and Drag:	create an appropriately sized rectangle in the contruction layer
  <li>Left Button Up:				depending on existence of an active markup layer it either opens the existing or creates a new markup layer and inserts appropraitely sized rectangle into it
  </ol>
  More Detailed Description: see event methods
*/  
class MVO_API HOpMarkupRectangle : public HOpConstructRectangle
{
public:
	/*! constructor */
    HOpMarkupRectangle(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpMarkupRectangle();

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpMarkupRectangle'
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonUp maps rectangle information from HOpConstructRectangle into the HOOPS routine 
		HC_Compute_Selection_By_Area.  This function also deselects any selected items that are not present in the current
		selection list.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
    virtual int OnLButtonUp(HEventInfo &hevent);  
	
	virtual HBaseOperator * Clone();  /*! returns a pointer to a copy of the operator */
}; 


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




