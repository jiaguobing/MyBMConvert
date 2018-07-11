// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//	HOpObjectAnnotate.h : interface of the HOpObjectAnnotate class
//
//	Draws a polyline in the view plane based on pointer input
//	Polyline points are stored in protected member variables and ready after 
//	OnLButtonDblClk method has been called. 
//	Derived Classes should call this::OnLButtonDblClk in their overloaded implementation.
//	Polyline points are in window space.


#ifndef _HOPOBJECTANNOTATE_H
#define _HOPOBJECTANNOTATE_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"

//! The HOpObjectAnnotate class provides support for attaching text to an object.
/*!
  HOpObjectAnnotate implements three mouse event handlers and one keypress handler defined on the base class and maps the 
  event information to HOOPS routines which insert an annotation line and text.  This second point in this line is used as the insertion point for 
  text.  This provides the basic functionality for annotating objects with useful information. The operation consists of 
  the following steps:
  <ol>
  <li>Left Button Down:				The operation is initiated.  The first point of the line is recorded.
  <li>Left Button Down and Drag:	Rubberband the line segment to desired position of the next point.
  <li>Left Button Down:				The last point of the line is recorded.
  <li>Key Down:						The text string is recorded until the carriage return keypress.
  </ol>
  More Detailed Description: see event methods.
*/
class MVO_API HOpObjectAnnotate : public HBaseOperator
{
public:
	/*! Constructs an HOpObjectAnnotate object.
	  \param view A pointer to an HBaseView object,
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.
	  This parameter has been deprecated.
	*/
  	HOpObjectAnnotate(HBaseView* view, int DoRepeat=0, int DoCapture=1);
	virtual ~HOpObjectAnnotate();

	/*!
		\return A pointer to a character string denoting the name of the operator which is 'HOpObjectAnnotate'.
	*/
	virtual const char * GetName();  


	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpObjectAnnotate object.
	*/
	virtual HBaseOperator * Clone();  

    /*! This is a utility function which creates an annotation. 
		\param pView A pointer to HBaseView object.
		\param segment The name of the segment to insert text into.
		\param AnnotationText The annotation text string to insert.
		\param lineStart The location of the beginning of the annotation line.
		\param lineEnd The location of the end of the annotation line.
		\param emit_message  Pass true to indicate that a message should be sent to the emit_message_function, which (if implemented)
 						   would typically use the HOOPS/Net 'Send' function to dispatch the message to clients.	
	*/
	static void AnnotateObject(HBaseView *pView,const char *segment, char *AnnotationText, HPoint lineStart, 
									   HPoint lineEnd, bool emit_message);
	/*!
		This method annotates an object based on the information in the message.
		\param pView A pointer to HBaseView object.
		\param in_data A character pointer containing the annotation information.
		\param data_length The size of the <i>in_data</i>.
	*/
	static void AnnotateObjectFromMessage(HBaseView *pView, const char *in_data, unsigned int data_length);

	/*!
		OnLButtonDown records the first pointer position and initiates the line-drawing mechanism.  If the annotation line 
		has already been started, OnLButtonDown finishes the line and prepares for text to be entered as described in 
		#OnKeyDown.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown(HEventInfo &hevent);

	/*!
		OnNoButtonDownAndMove draws a rubberbanded line from the first point to the current mouse position. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnNoButtonDownAndMove(HEventInfo &hevent);


	/*!
		OnKeyDown inserts annotation text near the second point in the annotation line. The text field is terminated by a 
		carriage return event. To erase, use the backspace key.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnKeyDown(HEventInfo &hevent);

protected:

	/*! This is a cleanup method that moves annotation from construction segment to the include segment. */
	void finalize_text();

private:
	char	*m_pString;
	HPoint	m_ptLast;
	char	m_sSegment[MVO_BUFFER_SIZE];
	bool	m_bObject;
	static int		m_iPathKeysCount;
	static HC_KEY	*m_pSelectionPath;
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif









