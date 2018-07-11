// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructSphere.h : interface of the HOpConstructSphere class
// Constructs a sphere using left mouse down and drag

#ifndef _HOPCONSTRUCTSPHERE_H
#define _HOPCONSTRUCTSPHERE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"


//! The HOpConstructSphere class provides support for constructing a sphere and inserting it into the model.
/*!
  HOpConstructSphere implements three of the mouse event handlers defined on the base class and maps the events to local 
  methods that rubberband a sphere and insert a corresponding HOOPS shell into the HOOPS database.  This provides the basic functionality for drawing a three-dimensional,
  tesselated sphere. The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the center point of the sphere.
  <li>Left Button Down and Drag:	Rubberbands the sphere to the desired circumference.
  <li>Left Button Up:				Circumference is finalized, the scene is cleared and the operation ends.	
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpConstructSphere : public HBaseOperator
{
private:
	void GenerateLinesThroughSphere();

protected:
	float m_radius;		/*!< Magnitude of the sphere's radius */

	/*!
		ConstructWireframeSphere generates a wireframe sphere using three circles. Note that the work done in this function
		is accomplished by three calls to HUtility::CreateAxisCircle. ConstructWireframeSphere is repeatedly called internally by 
		OnLButtonDownAndMove.
		\param center An HPoint object.
		\param radius Length of the sphere's radius.
	*/
	void ConstructWireframeSphere (HPoint const & center, float radius);

public:
	/*!
	  Constructs an HOpConstructSphere object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
    HOpConstructSphere (HBaseView* view, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpConstructSphere ();

	/*!
		\returns A pointer to a character string denoting the name of the operator which is 'Construct Sphere'.
	*/
	virtual const char * GetName();  


	/*!
		OnLButtonDown records the first pointer position and initiates the sphere-drawing mechanism.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove draws rubberbanded lines along the circumference of three great circles centered on the first 
		pointer position:  one equatorial, one along the prime meridian, and one 90-degrees from the prime meridian. Note 
		that the basic dynamic drawing work is done by repeated calls to the protected function 
		HOpConstructSphere::ConstructWireframeSphere.
		\param hevent An HEventInfo object containing information about the current event.  
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp finalizes the size of the sphere and cleans up.  
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp(HEventInfo &hevent); 

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpConstructSphere object.
	*/
	virtual HBaseOperator * Clone(); 
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



