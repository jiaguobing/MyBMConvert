// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//  HOpCreateNurbsCurve.h - interface for the HOpCreateNurbsCurve class
//  started GP 11/1/99

#ifndef _HOPCREATENURBSCURVE_H   
#define _HOPCREATENURBSCURVE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HOpConstructNurbsCurve.h"


//! The HOpCreateNurbsCurve class provides support for constructing a Non Uniform Rational B-Spline (NURBS) curve and inserting it into the model.
/*! 
  HOpCreateNurbsCurve uses event information to draw a NURBS curve.  The class employs all of the drawing functionality of 
  HOpConstructPolyline to draw a temporary, overlayed NURBS curve and associated 'control polyline' in the view plane, 
  and then uses the HOOPS routine HC_Insert_NURBS_Curve to add a curve to the model. 
  
  The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated and the first point of the curve is recorded.
  <li>No Button Down and Drag:		Rubberbands the line segment to a desired position for the next point.
  <li>Left Button Down:				The next point on curve is recorded and the cycle continues until a left button double click event occurs.
  <li>Left Button Double Click:		The curve is finalized, the temporary control polyline is flushed and the NURBS curve is inserted into the model.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpCreateNurbsCurve : public HOpConstructNurbsCurve
{
public:
	/*!
	  Constructs an HOpCreateNurbsCurve object.
	  \param view A pointer to an HBaseView object.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	  */
		HOpCreateNurbsCurve(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpCreateNurbsCurve();

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateNurbsCurve object.
	*/
	virtual HBaseOperator * Clone();  

	/*!
		\returns A pointer to a character string denoting the name of the operator, which is 'HOpCreateNurbsCurve'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDblClk directs the final mouse position into the temp array, then inserts the array into the database as a 
		NURBS curve.  This function also takes care of any cleanup.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int	OnLButtonDblClk(HEventInfo &hevent);

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif





