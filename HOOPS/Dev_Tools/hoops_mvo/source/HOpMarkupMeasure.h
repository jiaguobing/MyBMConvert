// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpMarkupMeasure.h : interface of the HOpMarkupMeasure class
// inserts measurement into the current scene

#ifndef _HOPMARKUPMEASURE_H
#define _HOPMARKUPMEASURE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HUtilityAnnotationMeasure.h"

//! The HOpMarkupAnnotate class inserts/manipulates annotation into the current scene
/*! 
  HOpMarkupAnnotate implements three mouse event methods defined on the base class and maps the event information
  to operator routines. This operation consists of the following steps:
  <ol>
  <li>Left Button Down:				new annotation created/annotation selected
  <li>Left Button Down and Drag:	dragging
  <li>Left Button Up:				operation ended
  </ol>
  More Detailed Description: see event methods
*/
class  MVO_API HOpMarkupMeasure : public HBaseOperator
{
public:
	
	/*! constructor
	*/
    HOpMarkupMeasure (HBaseView* view, bool AddToLayer = false, int DoRepeat=0, int DoCapture=1);

	/*! destructor */
    virtual ~HOpMarkupMeasure();

	/*! \return "HOpMarkupMeasure"
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown either selects an existing annotation or inserts a new annotation at the current mouse position 
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
    virtual int OnLButtonDown (HEventInfo &hevent);

	/*!
		OnLButtonDownAndMove computes the translation transform based on the distance between the first and current pointer 
		positions, and translates the annotation accordingly.
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo &hevent);

	/*!
		OnLButtonUp finalizes movement of annotation
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
    virtual int OnLButtonUp (HEventInfo &hevent);

	/*!
		OnRButtonDown finalizes movement of annotation
		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
    virtual int OnRButtonDown (HEventInfo &hevent);

	/*!
		OnKeyDown adds/replaces text of the selected annotation
 		\param hevent An HEventInfo object containing information about the current event.
		\return A value indicating the result of the event handling.
	*/
	virtual int OnKeyDown(HEventInfo &hevent);

	virtual HBaseOperator * Clone();  

protected:

	/*! StartOp sets the operator in an active state
	*/
	void StartOp();

	/*! EbdOp sets the operator in an inactive state
		\return true of the op was ended false if EndOp was called
		when the op was already ended.
	*/
	bool EndOp();

	/*! The annotation object */
	HUtilityAnnotationMeasure m_Annotation;
	
	/*! Keeps track of what background type we're using. */
	int m_iBackgroundType;

	/*! Keeps track of where this not is getting inserted and which segments it's styling. */
	bool m_bAddToLayer;

	/*! Set if the note is new of it it was selected. */
	bool m_bNewNote;

	/*! Keeps track of if we need to update the note or not. */
	bool m_bTouchedText;

	/*! Keeps track of the hide overlapped text setting since we explicitly unset it. */
	bool m_bHideOverlappedText;
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif










