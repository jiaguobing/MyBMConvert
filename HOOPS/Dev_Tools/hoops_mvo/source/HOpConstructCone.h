// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpConstructCone.h : interface of the HOpConstructCone class
// Constructs a cone using left mouse down and drag

#ifndef _HOPConstructCONE_H   
#define _HOPConstructCONE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpConstructCone class provides support for constructing a cone and inserting it into the model. 
/*! 
  HOpConstructCone implements four of the mouse button event handlers defined on the base class and maps the event information 
  to the local methods that rubberband a cone.  This class provides the basic functionality for temporarily drawing a three-dimensional, tesselated cone with a circular base and a number of triangular, 
  facetted sides with the default being 30.  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				Registers the center point of the cone base.
  <li>Left Button Down and Drag:	Rubberbands the circle to the desired base circumferences
  <li>Left Button Up:				The base circumference is finalized.
  <li>No Button Down and Drag:		Rubberbands the cone to desired height.
  <li>Left Button Down:				The height is finalized, the operation is ended and the primitive is removed from the scene.
  </ol>
  More Detailed Description: see event methods.
*/
class MVO_API HOpConstructCone : public HBaseOperator
{

private:
	void GenerateLinesThroughCircle( HPoint const &center, float radius );

protected:
	float   m_height;				/*!< Height of the cone, based on first and last pointer positions */
	float	m_RadiusLength;			/*!< Magnitude of the base radius vector, in screen coordinates  */
	HVector	m_WorldRadiusVector;	/*!< Base radius vector in world coordinates */

 
	/*!
		ConstructWireframeCone transforms the pointer coordinates from screen to world space and inserts a wireframe cone 
		into the scene. This function is called internally by OnNoButtonDownAndMove after the base dimensions have been 
		finalized.  
		\param center The center of the circle that forms base of the cone.
		\param radius The length of the radius of the cone's base.
		\param height The height of the cone.
	*/
	void ConstructWireframeCone(HPoint center, float radius, float height);
	
public:
	/*!
	  Constructs an HOpConstructCone object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
    HOpConstructCone (HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpConstructCone();


	/*!
		\return A pointer to a character string denoting the name of the operator, which is 'HOpConstructCone'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first pointer position and initiates the cone-drawing mechanism.  The first point is 
		used as the center of the cone's circular base.  If the drawing mechanism has already been started such as
		LButtonDown having been called once already, LButtonDown record the last point, calculates the height of the 
		cone, finalizes the operator and then clear the geometry from the scene.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int	OnLButtonDown(HEventInfo &hevent);

	/*!
		OnNoButtonDownAndMove draws rubberband lines from the circular base of the cone to the current mouse position.  
		Note that the basic work is done by internal calls to HOpConstructCone::ConstructWireframeCone.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove draws a rubberbanded line along the circumference of a circle centered on the first pointer
		position. Note that the basic drawing work is done by HUtility::CreateAxisCircle, while OnLButtonDownAndMove computes
		and keeps track of the radial vector that defines the base.
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
	  \return A pointer to the newly created HOpConstructCone object.
	*/
	virtual HBaseOperator * Clone();
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




