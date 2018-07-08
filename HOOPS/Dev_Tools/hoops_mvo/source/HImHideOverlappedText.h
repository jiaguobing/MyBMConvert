// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HUTILITY_TEXT_LIST_H
#define _HUTILITY_TEXT_LIST_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "hic_types.h"

/*!
	HImHideOverlappedText is a class for "hidden text" mode.  The idea is to have text obscure
	other text but not geometry.  The requirement that it not obscure other geometry 
	precludes a simple rectangle inserted behind the text, and pushes us into this more
	sophisticated mechanism.  

	The first step is that we set an IM callback to intercept all text drawing and store
	all such calls into a linked list during the normal update.  At the end of the update,
	we z-sort everything and insert the text calls into a quad tree to resolve what text 
	should be drawn with as few comparisons as possible.

	This class is a filter that can be turned of and on per driver segment
	to use call FilterOn and FilterOff from a open driver segment
*/
class MVO_API HImHideOverlappedText 
{

public:

	/*! This method enables the processing of overlapped text.  Call this from within an open driver segment. */
	static void FilterOn();

	/*! This method disables the processing of overlapped text.  Call this from within an open driver segment. */
	static void FilterOff();

	/*! \return True if hide overlapped text is on or false if otherwise. */
	static bool IsFilterOn();

	/*!  This method sets up callbacks that will be used to intercept and draw text. */
	static void RegisterCallbacks();

private:
	/* private members are also doxygenated, as this class is also meant to serve as an example of IM usage. */

	/*! This function saves away all information that will eventually be needed for a call to
		HIC_Draw_Text.	This method is declared as static and passed to HOOPS as a callback.
		It requires the existence of global_text_list and global_text_list_length.
		\param nr Top-level graphics state at the time we encountered this particular piece of text.
		\param text_info All other information about the text (string, font, size, etc...).
	*/
	static void defer_text(HIC_Rendition const *nr, HIC_Text_Info const *text_info);

	/*!	This function is passed to HOOPS as a "draw segment tree" callback. It is 
		necessary so that we can receive notification of when the normal update cycle 
		is complete, so that we can begin to draw all of the text that has been deferred.
		\param nr Top-level graphics state.
		\param seg The segment.  Needed in order to call HIC_Draw_Segment (the normal update function)
	*/
	static void update(HIC_Rendition const *nr, HIC_Segment_Info const *seg );

	static struct vlist_s * text_list;
	static struct vlist_s * annotation_list;
	static bool m_bFilterOn;

	/*!	This method takes the list of strings that we built up during the course of processing an 
		Update(), and we finally get to draw them. This should be called 
		at the end of the "draw segment tree" callback  (i.e. after the normal 
		update cycle has completed).
		\param nr Top level graphics state.  Used only to extract a screen-space bounding
					rectangle of the current window (and hence the starting size of quad tree cells).
		\param textlistlen The number of items to be drawn (should be the same
					as #HImHideOverlappedText::global_text_list_length.
	*/
	static void draw_text(HIC_Rendition const *nr );

	/*!	This method checks each segment to see if it is an annotation segment tree.  If it is,
		it is defered so it can be drawn later when we're not checking for overlapped text. */
	static void defer_annotation(HIC_Rendition const *nr, HIC_Segment_Info const * si);

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif






