// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateCone.h : interface of the HOpCreateCone class
// Creates a cone using left mouse down and drag

#ifndef _HOPCREATECONE_H   
#define _HOPCREATECONE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructCone.h"

//! The HOpCreateCone class provides support for constructing a cone and inserting it into the model. 
/*! 
  HOpCreateCone maps event information to insert a corresponding HOOPS shell into the HOOPS database.  This provides the 
  basic functionality for drawing a three-dimensional, tesselated cone with a circular base and a number of triangular, 
  facetted sides with the default being 30.  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the center point of the cone base.
  <li>Left Button Down and Drag:	Rubberbands the circle to the desired base circumferences
  <li>Left Button Up:				The base circumference is finalized.
  <li>No Button Down and Drag:		Rubberbands the cone to desired height.
  <li>Left Button Down:				The height is finalized, the operation is ended and a cone is inserted into the scene.	
  </ol>
  More Detailed Description: see event methods.
*/
class MVO_API HOpCreateCone : public HOpConstructCone
{
public:
	/*!
	  Constructs an HOpCreateCone object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCreateCone (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCreateCone();


	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpCreateCone'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first pointer position and initiates the cone-drawing mechanism.  The first point is used
		as the center of the cone's circular base.  If the drawing mechanism has already been started such as LButtonDown 
		having been called once already, LButtonDown draws a cone using HUtilityGeometryCreation::CreateCone.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int	OnLButtonDown(HEventInfo &hevent);

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateCone object.
	*/
		virtual HBaseOperator * Clone();  
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




