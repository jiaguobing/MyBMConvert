// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCylinder.h : interface of the HOpConstructCylinder class
// Constructs a cylinder using left mouse down and drag

#ifndef _HOPConstructCYLINDER_H
#define _HOPConstructCYLINDER_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpConstructCylinder class provides support for constructing a cylinder and inserting it into the model. 
/*! 
  HOpConstructCylinder implements four of the mouse button event handlers defined on the base class and maps the event 
  information to local methods that rubberband a cylinder and insert a corresponding HOOPS shell into the HOOPS database.  
  This provides the basic functionality for drawing a temporary three-dimensional, tesselated cylinder with a circular 
  base, a circular cap of equivalent size, and a number of rectangular, facetted sides with the default as 30. The 
  operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Register the center point of the cylinder base.
  <li>Left Button Down and Drag:	Rubberbands the circular base to the desired circumference.
  <li>Left Button Up:				The base circumference finalized.
  <li>No Button Down and Drag:		Rubberbands the cylinder to the desired height.
  <li>Left Button Down:				The height is finalized, the cylinder is cleared from the scene and the operation ends.	
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpConstructCylinder : public HBaseOperator
{
private:
	void GenerateLinesThroughCircle( HPoint const &center, float radius );

protected:
	float   m_height;				/*!< The height of the cylinder based on first and last pointer positions. */
	float	m_RadiusLength;			/*!< The magnitude of the base radius vector in screen coordinates.  */
	HVector	m_WorldRadiusVector;	/*!< The base radius vector in world coordinates. */


	/*!
		ConstructWireframeCylinder transforms the pointer coordinates from screen to world space and inserts a wireframe 
		cylinder into the scene. This function is repeatedly called internally by OnNoButtonDownAndMove after the base 
		dimensions have been finalized.  
		\param center The center of the circular top and base of the cylinder.
		\param radius The radius of the cylinders's base.
		\param height The height of the cylinder.
	*/
	void ConstructWireframeCylinder(HPoint center, float radius, float height);

public:
	/*!
	  Constructs an HOpConstructCylinder object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpConstructCylinder (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpConstructCylinder();


	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpConstructCylinder'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first pointer position and initiates the cylinder-drawing mechanism.  The first point is 
		used as the center of the cylinder's circular base.  If the drawing mechanism has already been started such as 
		LButtonDown having been called once already, LButtonDown finalizes the height of the cylinder, clears the scene 
		and ends the operation.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int	OnLButtonDown(HEventInfo &hevent);

	/*!
		OnNoButtonDownAndMove draws rubberband lines from the base the cylinder to the cap.  
		Note that the basic work is done by repeated internal calls to HOpConstructCylinder::ConstructWireframeCylinder.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove draws a rubberbanded line along the circumference of a circle centered on the first pointer
		position. Note that the basic drawing work is done by HUtility::CreateAxisCircle, while OnLButtonDownAndMove 
		computes and keeps track of the radial vector that defines the cylinder's base.
		\param hevent An HEventInfo object containing information about the current event.  
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp finalizes the size of the base, sets the value of m_RadiusLength and m_radius_world_vector, and 
		cleans up.  This function is not called after the second call to OnLButtonDown.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int	OnLButtonUp(HEventInfo &hevent);

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpConstructCylinder object.
	*/
	virtual HBaseOperator * Clone();
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



