// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateSphere.h : interface of the HOpCreateSphere class
// Creates a sphere using left mouse down and drag

#ifndef _HOPCREATESPHERE_H
#define _HOPCREATESPHERE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructSphere.h"


//! The HOpCreateSphere class provides support for constructing a sphere and inserting it into the model.
/*!
  HOpCreateSphere uses event information to draw a sphere. This class provides the basic functionality for drawing 
  a three-dimensional, tesselated sphere. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the center point of the sphere.
  <li>Left Button Down and Drag:	Rubberbands the sphere to the desired circumference.
  <li>Left Button Up:				Circumference is finalized, clears the scene and inserts a sphere into the HOOPS data base. The operation ends.	
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API  HOpCreateSphere : public HOpConstructSphere
{

public:
	/*!
	  Constructs an HOpCreateSphere object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
    HOpCreateSphere (HBaseView* view, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpCreateSphere ();

	/*!
		\returns A pointer to a character string denoting the name of the operator, which is 'HOpCreateSphere'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonUp finalizes the size of the sphere and inserts that sphere into the HOOPS database as a shell. 
		Note that most of the work in this function is done by a call to the public function HUtilityGeometryCreation::CreateSphere. 
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent); 

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateSphere object.
	*/
	virtual HBaseOperator * Clone(); 
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




