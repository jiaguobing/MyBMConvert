// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpSelectAperture.h : interface of the HOpSelectAperture class, derived from HOpSelect
// Handles aperture selection

#ifndef _HOPSELECTAPERTURE_H
#define _HOPSELECTAPERTURE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


//! The HOpSelectAperture class performs hit testing on a circular aperture and computes a selection list for positive hits.
/*!
  HOpSelectAperture implements one of the mouse event handlers defined on the base class and maps the mouse event to the 
  HOOPS routine HC_Compute_Selection.  This provides the basic functionality for interactively selecting HOOPS objects
  with a single click.  The aperture under which hit testing will be carried out is definable by the user via the HOOPS/3dGS 
  Driver Option \ref Set_Driver_Options_selection_proximity "selection proximity".  This is set by calling HC_Set_Driver_Options("selection proximity = <value>") 
  HOOPS/MVO sets the default selection proximity to 0.1 . The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				object selected, highlighted, placed in selection list
  </ol>

  This class contains code that allows the following to be true: when HOpSelectAperture 
  is the current operator, repeated calls to the internal function OnNoButtonDownAndMove 
  are made.  Therefore, when the mouse is moved without any buttons depressed, a selection 
  list is still being calculated for every point.  If this logic is desired, the user must 
  remove the conditional "#if 0" statement.
*/
class MVO_API HOpSelectAperture : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpSelectAperture object.
	  \param view A pointer to an HBaseView object
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
	HOpSelectAperture(HBaseView* view, int DoRepeat = 0, int DoCapture = 1);
	virtual ~HOpSelectAperture();

	/*!
		\return A pointer to a character string denoting the name of the operator, 'HOpSelectAperture'.
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonDown records the first pointer position, finds the item(s) occurring at that point, and highlights the
		item(s). It also deselects items that are currently highlighted, unless the control key is pressed.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown(HEventInfo &hevent);

 	virtual HBaseOperator * Clone(); /*! returns a pointer to a copy of the operator */
protected:

	/*!
		For the given <i>hevent</i>, this method determines if an object is selected.  If it is selected,
		it adds to the HSelectionSet associated with the given HBaseView.
		\param hevent An HEventInfo object containing information about the current event.
 	*/
	virtual void HandleSelection(HEventInfo &hevent);

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



