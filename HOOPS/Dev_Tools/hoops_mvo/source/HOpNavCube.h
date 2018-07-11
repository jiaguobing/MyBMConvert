// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpNavCube.h : interface of the HOpNavCube class, derived from HOpSelect
// Handles aperture selection

#ifndef _HOpNavCube_H
#define _HOpNavCube_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


//! The HOpNavCube class provides an interactive navigation cube for rapid viewpoint changes.
/*!
  HOpNavCube implements one of the mouse event handlers defined on the base class and maps the event to the HOOPS 
  routine HC_Compute_Selection.  This provides the basic functionality for calculating and displaying a navigation cube.
*/
class  MVO_API HOpNavCube : public HBaseOperator
{
private:
	HC_KEY	m_NavCubeSegKey;

public:
	/*!
	  Constructs an HOpNavCube object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
    HOpNavCube(HBaseView* view, int DoRepeat = 0, int DoCapture = 1);
	virtual ~HOpNavCube();

	/*!
		\returns A pointer to a character string denoting the name of the operator which is 'HOpNavCube'.
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonDown selects an arrow from one side of the cube and changes the viewpoint to the desired position.
 		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown(HEventInfo &hevent);

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpNavCube object.
	*/
	virtual HBaseOperator * Clone(); 
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




