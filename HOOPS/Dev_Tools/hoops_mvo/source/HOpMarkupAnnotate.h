// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupAnnotate.h : interface of the HOpMarkupAnnotate class
// inserts annotation into the current scene

#ifndef _HOpMarkupAnnotate_H
#define _HOpMarkupAnnotate_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HUtilityAnnotation.h"

//! The HOpMarkupAnnotate class inserts and manipulates annotation into the current scene
/*! 
  HOpMarkupAnnotate implements three mouse event methods defined on the base class and maps the event information
  to operator routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				create new annotation created or select existing annotation
  <li>Left Button Down and Drag:	dragging
  <li>Left Button Up:				operation ended
  </ol>
  More Detailed Description: see event methods
*/
class  MVO_API HOpMarkupAnnotate : public HBaseOperator
{
public:
	/*!
	  Constructs an HOpMarkupAnnotate object.
	  \param view A pointer to an HBaseView object
	  \param bAddToLayer A boolean indicating whether the annotation can be added to markup layers
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
	HOpMarkupAnnotate (HBaseView* view, bool bAddToLayer = false, int DoRepeat=0, int DoCapture=1, bool create_new_notes = true);

	 /*! Destructor */
    virtual ~HOpMarkupAnnotate();

	/*!
		\return A pointer to a character string denoting the name of the operator, 'HOpMarkupAnnotate'.
	*/
	virtual const char * GetName();  

	/*!
	OnLButtonDown either selects an existing annotation or inserts a new annotation at the current mouse position. 
	\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove computes the translation transform based on the distance between the first and current pointer 
		positions, and translates the annotation accordingly.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnRButtonDown finalizes movement of annotation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnRButtonDown (HEventInfo &hevent);
	
	/*!
		OnLButtonUp finalizes movement of annotation.
		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);

	/*!
		OnKeyDown adds and/or replaces text of the selected annotation with the character or key
		that was provided in hevent.
 		\param hevent An HEventInfo object containing information about the current event.
	  \return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnKeyDown(HEventInfo &hevent);

	/*! 
	  Creates a new operator which is associated with the same view as the current operator. 
	  The user is responsible for deleting the newly created operator.
	  \return A pointer to an HBaseOperator object
	*/
	virtual HBaseOperator * Clone(); 

protected:

	/*! Sets what type of background we're using. */
	int m_iBackgroundType;
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	int GetBackgroundType() const {
		return m_iBackgroundType;
	}
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	void SetBackgroundType(int const type){
		m_iBackgroundType = type;
	}

	/*! Selected annotation key */
 	HC_KEY m_pNoteKey;
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	HC_KEY GetNoteKey() const {
		return m_pNoteKey;
	}
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	void SetNoteKey(HC_KEY const key) {
		m_pNoteKey = key;
	}
	
	/*! The annotation object. */
	HUtilityAnnotation m_Annotation;
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	HUtilityAnnotation & GetAnnotation() {
		return m_Annotation;
	}
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	void SetAnnotation(HUtilityAnnotation const & rNote){
		m_Annotation = rNote;
	}

	/*! Allow annotation to be added to markup layers */
	bool m_bAddToLayer;
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	bool GetAddToLayer() const {
		return m_bAddToLayer;
	}
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	void SetAddToLayer(bool value){
		m_bAddToLayer = value;
	}

	/*! If the text was touched at some point, then we'll need to update m_Annotation. */
	bool m_bTouchedText;
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	bool GetTouchedText() const {
		return m_bTouchedText;
	}
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	void SetTouchedText(bool value){
		m_bTouchedText = value;
	}

	/*! Hide overlapped text doesn't seem to work with quickmoves so we need to keep track of it after we turn it off. */
	bool m_bHideOverlappedText;
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	bool GetHideOverlappedText() const {
		return m_bHideOverlappedText;
	}
	/*! Accessor method for this value (which probably should be private so use the accessor rather than directly accessing it). */
	void SetHideOverlappedText(bool onoff){
		m_bHideOverlappedText = onoff;
	}

	/*! If this is true, this operator will insert new node on the left mouse button. */
	bool m_bCreateNewNotes;
	/*! Accessor for this value. */
	bool GetCreateNewNotes() const {
		return m_bCreateNewNotes;
	};
	/*! Accessor for this value. */
	void SetCreateNewNotes(bool yesno) {
		m_bCreateNewNotes = yesno;
	};

	/*! Call this to set the operator to active. */
	void StartOp();

	/*! Call this to set the operator in an inactive state.
		\retval True of the operator was ended.  False if this was called
		when the operator was not in progress.
	*/
	bool EndOp();

	/*! Transmit markup layer to HNet. */
	void TransmitMarkupSegment();
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif




