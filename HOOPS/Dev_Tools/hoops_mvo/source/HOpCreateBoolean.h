// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpCreateBoolean.h : interface of the HOpCreateBoolean class
// Exposes the boolean functionality of the partviewer

#ifndef _HOPCREATEBOOLEAN_H
#define _HOPCREATEBOOLEAN_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

#define BOOLEAN_TYPE_NONE		0
#define BOOLEAN_TYPE_INTERSECT	1
#define BOOLEAN_TYPE_SUBTRACT	2
#define BOOLEAN_TYPE_UNION		3

//! The HOpCreateBoolean class allows to perform the binary operations intersect, subtract and union on two shells in a scene.
/*!
  The HOpCreateBoolean class performs either an intersection, subtraction or union on two selected shells in the scene.  
  To determine the operation type, pass BOOLEAN_TYPE_INTERSECT, BOOLEAN_TYPE_SUBTRACT or BOOLEAN_TYPE_UNION to the 
  constructor.  If the operator type is BOOLEAN_TYPE_INTERSECT, HOOPS will find the set of points common to the two shells
  and removed the other areas from the scene.  If the operator type is BOOLEAN_TYPE_SUBTTRACT, HOOPS will find the set of 
  points common to the two shells and then it will remove that area from the scene as well as the the second shell that
  was selected.  If the operator type is BOOLEAN_TYPE_UNION, HOOPS will create a shell that represents the combined area of 
  two selected shells.
  
  This class implements the OnLButtonDown handler to determine when an item has been selected for operation.  The order of 
  event goes as follows:
  <ol>
  <li>Left Button Down: Finds the shell that is selected.
  <li>Left Button Down: Finds the second shell that is selected.  Performs the selected binary operation and returns the result.
  </ol>
  
  
*/
class MVO_API  HOpCreateBoolean : public HBaseOperator
{

public:
	/*!
	  Constructs an HOpCameraZoomBox object.
	  \param view A pointer to an HBaseView object.
	  \param nBooleanType The BooleanType used to compare the two selections.  The values can be 
	  BOOLEAN_TYPE_INTERSECT, BOOLEAN_TYPE_SUBTRACT or BOOLEAN_TYPE_UNION.
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events 
	  should be received after a mousedown, even if it leaves the window.  This parameter has been deprecated.
	*/  	
     HOpCreateBoolean (HBaseView* view, int nBooleanType, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpCreateBoolean ();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpCreateBoolean'.
	*/
	virtual const char * GetName();  

	/*!  This method get the first shell selection.  If the first shell has already been selected, then it looks for 
	the second selection.  Then it performs the operation indicated by the BooleanType passed into the operation.  The 
	first object selected is the tool, the second object is the target.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent); 

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpCreateBoolean object.
	*/
	virtual HBaseOperator * Clone();  

private:
	//the type of Boolean operation we're going to be performing
	int m_BooleanType;
	
	//The keys of the two shells involved.  This is necessary because the selection list 
	//stores the key of the containing segment, making it difficult to retrieve the shell again.
	HC_KEY m_FirstSel, m_SecondSel;

	//This method perform selection handling.  This is a slightly modified version of what is in HOpSelectAperture.
	void HandleSelection(HEventInfo &event);
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




