// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// CreateCuttingPlane.h : interface of the CreateCuttingPlane class
// Creates a CuttingPlane using left mouse down and drag left right

#ifndef _HOPCREATECUTTINGPLANE_H
#define _HOPCREATECUTTINGPLANE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpObjectTranslate.h"


//! The HOpCreateCuttingPlane class provides support for construction a cutting plane and inserting it into the scene. 
/*! 
  HOpCreateCuttingPlane implements two of the mouse button event handlers defined on the base class.  In this case, the 
  operator uses the LButtonDown event to begin the cutting plane insertion mechanism, but position info is not used. This 
  provides the basic functionality for viewing cut-away scenes, cross-sections, and slices of models.  Upon insertion, the
  default position of the cutting plane lies parallel to the view plane and intersects the z-value of the scene's centerpoint.  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The cutting plane is inserted through center of the scene.
  <li>Left Button Up:				All geometry is deselected and the operation ends.
  </ol>
  More Detailed Description: see event methods. 
*/
class  MVO_API HOpCreateCuttingPlane : public HOpObjectTranslate
{
public:
	/*! Constructs an HOpCreateCuttingPlane object.
	  \param view A pointer to an HBaseView object,
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.	
	*/
	HOpCreateCuttingPlane (HBaseView* view, int DoRepeat=0, int DoCapture=1);
  
	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpCreateCuttingPlane'.
	*/
	virtual const char * GetName();  
	
	/*!
		OnLButtonDown begins the cutting plane insertion mechanism and calculates the center of the scene.  The center 
		point's z-value is passed internally to initiate the plane's construction. In addition, the plane is placed in 
		the current selection list and will be highlighted until the left mouse button is released. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonUp ends the operation and removes the cutting plane from the current selection list.  The function also
		deselects every item in the scene upon completion.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent);

	/*!Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateCuttingPlane object.
	  */	
	virtual HBaseOperator * Clone();
	
private:
	void MakeCuttingPlane(float z, float rad);

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






