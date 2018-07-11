// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpObjectTranslate.h : interface of the HOpObjectTranslate class
// translate items in the selection list of the associated view 
//
// currently supports translation of item if it is a segment

#ifndef _HOPOBJECTTRANSLATE_H
#define _HOPOBJECTTRANSLATE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

class HKeyList;

//! The HOpObjectTranslate class translates a selected object.
/*!
  HOpObjectTranslates implements three of the mouse event handlers defined on the base class and maps the event 
  information to HOOPS routines which translates the currently selected objects. This provides the basic functionality 
  for applying a translation transform to an object (as opposed to a global panning of the camera which affects 
  the entire scene). The operation consists of the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated and an object is selected.
  <li>Left Button Down and Drag:	The object is translated using spriting.
  <li>Left Button Up:				The translation is finalized and the operation ends.
  </ol>
  More Detailed Description: see event methods. 
*/
class MVO_API HOpObjectTranslate : public HBaseOperator
{

public:
	/*! Constructs an HOpObjectTranslate object.
	  \param view A pointer to an HBaseView object,
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.	
	*/
    HOpObjectTranslate (HBaseView* view, int DoRepeat=0, int DoCapture=1);
    virtual ~HOpObjectTranslate();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpObjectTranslate'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first pointer position and initializes the translation mechanism.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove computes the translation transform based on the distance 
		between the first and current pointer positions, and translates the object accordingly.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp cleans up the construction segments used during the transformation and ends the operation.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);

	/*!
		OnLButtonDblClk translates the object back to its original location.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDblClk (HEventInfo &hevent);


	HPoint m_ptRecent;		/*!< A pointer to the current position in world coordinates. */
	HPoint m_ptRecentVP;	/*!< A pointer to the current position in viewpoint (screen) coordinates. */

	/*!Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpObjectTranslate object.
	  */	
	virtual HBaseOperator * Clone(); 

	/*!	
		This method is used to update the list of keys to translate when the selection list changes.
	*/
	virtual void UpdateKeyList(void * data);

protected:
	/*! This method initializes the selected entities and turns on spriting.
	*/
	void Init();

	/*! This method cleans up the selected entities and turns off spriting if appropriate.
	*/
	void Cleanup();

	/*! <b>For internal use only.</b>
		This method does the computation work to compose a translation matrix that makes objects follow
		the mouse pointer.  It copies the matrix contents into matrix_out so that they may be applied
		to arbitrary targets.  It takes key as an argument because the required transformation can 
		depend on segments' pre-existing modelling matrices.  Currently, it assumes that the input key is
		a segment.
		\param hevent An HEventInfo object containing information about the current event.
		\param key_count The number of keys in the full segment path.
		\param full_path_keys The keys of each segment in the full segment path.
		\param matrix_out A pointer to an array of 16 floats.  Called by reference, returned to user.
		\return An integer value indicating success or failure.
	*/
	int GetMatrix(HEventInfo &hevent, int key_count, HC_KEY const *full_path_keys, float matrix_out[]);

	/*! <b>For internal use only.</b>
		This method updates the internal state needed for matrix calculations.
		\param hevent An HEventInfo object containing information about the current event.
		\return An integer value indicating success or failure.
	*/
	int UpdateMousePos(HEventInfo &hevent);

	bool m_bTemporarySelection;		/*!< This boolean indicates whether object was temporarily selected. */
	bool m_bInitialMatrix;			/*!< The initial modelling matrix. */
	HKeyList * m_KeyList;			/*!< A List of keys to translate (copied from selection set). */
	HPoint m_ptLast;
private:
	unsigned long m_cookie[2];		/*!< Cookies from notification subscription in HBaseView. */
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




