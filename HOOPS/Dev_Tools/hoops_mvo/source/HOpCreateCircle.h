// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HOPCreateCIRCLE_H
#define _HOPCreateCIRCLE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructCircle.h"

//! The HOpCreateCircle class draws a circle. 
/*! 
 HOpCreateCircle maps the event information to the HOOPS routine #HC_Insert_Circle_By_Radius. This provides the basic functionality 
 for drawing a temporary, overlayed 2-D box outline in the view plane based on first and last pointer positions. Once the 
 operation is finalized, a circle will be inserted into the model.  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Identifies the center point of the circle.
  <li>Left Button Down and Drag:	Rubberband circle to desired dimensions.
  <li>Left Button Up:				Circle finalized, the polyline primitive used to represent the circle is flushed from the scene and a new one is created in the model.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpCreateCircle : public HOpConstructCircle
{
public:
	/*!
	  Constructs an HOpCreateCircle object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	  \param UseCenterMarker Pass true if you want HOOPS to draw a mark at the center.
	  
	*/  	
  	HOpCreateCircle(HBaseView* view, int DoRepeat=0, int DoCapture=1, bool UseCenterMarker=true);
	virtual ~HOpCreateCircle();

	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpConstructCircle'.
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonUp finalizes the size of the circle, cleans up the geometry draw under the construction key and
		inserts a circle into the current model using #HC_Insert_Circle_By_Radius. 
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent); 

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateCircle object.
	*/
	virtual HBaseOperator * Clone();  
 };

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




