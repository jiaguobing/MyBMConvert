// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupAnnotate.h : interface of the HOpMarkupAnnotate class
// inserts annotation into the current scene

#ifndef _HOpMarkupPostIt_H
#define _HOpMarkupPostIt_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HOpMarkupAnnotate.h"


//! The HOpMarkupPostIt class inserts and manipulates annotation into the current scene.
/*! 
  HOpMarkupPostIt implements three mouse event methods defined on the base class and maps the event information
  to operator routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				new annotation created/annotation selected
  <li>Left Button Down and Drag:	dragging
  <li>Left Button Up:				operation ended
  </ol>
  More Detailed Description: see event methods
*/
class  MVO_API HOpMarkupPostIt : public HOpMarkupAnnotate
{
public:
	/*!
	  Constructs an HOpMarkupPostIt object.
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
		HOpMarkupPostIt (HBaseView* view, int DoRepeat=0, int DoCapture=1);

	/*!
		\return A pointer to a character string denoting the name of the operator, 'HOpMarkupPostIt'
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown either selects an existing annotation or inserts a new annotation into the current markup layer at the current mouse position. 
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif










