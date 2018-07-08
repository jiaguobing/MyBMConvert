// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HOpObjectClash.h : interface of the HOpObjectClash class
// translate items in the selection list of the associated view 
//
// currently supports translation of item if it is a segment
/*! \file HOpObjectClash.h
	This class dectect object clashes.
*/
#ifndef _HOPOBJECTCLASH_H
#define _HOPOBJECTCLASH_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "HBaseOperator.h"
#include "HOpObjectTranslate.h"

class HSelectionSet;

/*!
    The HOpObjectClash class detects and highlights objects that intersect a set of translated objects.
    The set of objects to be translated, a.k.a. "the part", is assumed to be selected at the segment level 
    prior to activating  this operator.  The computation can be done in several ways as described in #HOOC_SelectMode 
    but the most important mode that this operator demonstrates is #HOOC_Shell.  In that mode, shells from 
    the scene, a.k.a "the environment", are selected and highlighted if their faces cross the part's faces.

    This operator currently makes the (sometimes false) assumption that shells in the part are affected only by
    modelling matrices in their ancestry, as opposed to via some include path.

    HOpObjectClash employs the transform functionality of HOpObjectTranslate to move an object around a scene, and  
    extends two of the mouse event handlers defined on HOpObjectTranslate to perform clash detection. The operation 
    consists of the following steps:
    <ol>
    <li>Left Button Down:				The operation is initiated.  The translation objects are selected.
    <li>Left Button Down and Drag:		Clashed objects are highlighted.
	<li>Left Button Up:					The operation is ended.  The clashed objects are deselected.
    </ol>
    More Detailed Description: see event methods.
*/
class MVO_API HOpObjectClash : public HOpObjectTranslate
{
protected:
	/*! The selection can be computed in several different ways.
	*/
	enum HOOC_SelectMode {
		HOOC_World =	 1,	//!< This selection mode computes the selection in world coordinates prior to the camera transformation and then looks for bounding box intersections. Use this mode for speed.
		HOOC_Screen =	 2,	//!< This selection mode computes the selection in screen coordinates and looks for bounding box intersections.
		HOOC_Shell =	 3,	//!< This selection mode looks for shells that intersect the faces of the current selection set in HBaseView.  This is the default mode and is most accurate.
		HOOC_Enclosure = 4,	//!< This selection mode detects if the selection is completely enclosed by the translated object.
	};
	/*! HOOC_Result is the result code for the object clash calculation.
	*/
	enum HOOC_Result {
		HOOC_Unknown =	-1,	//!< The relationship between the objects is unknown.
		HOOC_In =		 1,	//!< The translated objects are completely enclosed by the highlighted object.
		HOOC_Out =		 2,	//!< The translated objects is not completely enclosed by the highlighted object.
		HOOC_Clash =	 3,	//!< The selected objects intersect wit the translated objects.
	};


protected:
	/*! This method computes the polyline of intersection between two shells. 
		\param key1 The key of the shell to find intersection with the selection shell.
		\param transformed_points1 The point list of the shell.
		\return The total number of intersecting line segments.
	*/
	int IntersectPolyline( HC_KEY key1, float * transformed_points1 );

	/*! This method loops through the selected object and all of the results from #HC_Find_Related_Selection, 
	and send them to the appropriate #HSelectionSet.
	\param current_shell The key of the shell we want to highlight.
	\param transformed_points The point list of the shell.
	*/
	void ProcessSelectionResults( HC_KEY current_shell = HC_ERROR_KEY, float * transformed_points = 0 );

	/*!This is a helper function for ProcessSelectionResults().  It tests if the hit object encloses the shell.
	\param current_shell_key The key of the shell that we want to test if it is enclosed by another object.
	\param transformed_points The point list of the shell.
	\return An #HOOC_Result describing if result of the enclosure test.
	*/
	int TestEnclosure( HC_KEY current_shell_key, float * transformed_points );
	
	/*!This method gets the screen space volume of the given segment, as seen by driver in the current view.
	\param seg The key of segment in which you want to compute the bounding box.
	\param min Returns the minimum point of the bounding box volume.
	\param max Returns the maximum point of the bounding box volume.
	\return An #HOperatorReturn indicating the status of the event.
	*/
	int GetScreenVolume( HC_KEY seg, HPoint & min, HPoint & max );
	
	/*!This method finds if a given segment in screen space clashes with any other objects in the scene and then selects them. 
	The selected items will be added to the current view's selection list and highlighted as usual.
	\param seg The key of the segment to perform clash detection on.
	\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int SelectByScreenVolume( HSmartSelItem * sel_item );
	
	/*!This method finds if a given segment in world space clashes with any other object in the scene and then selects them.  
	The selected items will be added to the current view's selection list and highlighted as usual.
	\param seg The key of the segment to select.
	\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int SelectByWorldVolume( HSmartSelItem * sel_item );
	
	/*!This method find if a given segment is fully enclosed by any other objects and then selects them. 	The selected items will 
	be added to the current view's selection list and highlighted as usual.
	\param seg The key of the segment to select.
	\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int SelectByEnclosure( HSmartSelItem * sel_item );
	
	/*!This method finds if a given shell clashes with any other shells and selects them. After selection, the selection 
	list will contain all shells in the model that intersect the faces of the shells in the segment given by "seg". 
	\param seg The key of the segment to check for clashes with other shells.
	\return An #HOperatorReturn indicating the status of the event.
		*/
	virtual int SelectByShell( HSmartSelItem * sel_item );
	
	/*!This method sets the type of computing selection described in #HOOC_SelectMode enum.  #HOOC_Shell is the default.
	\param mode The mode in which you would like to perform selection and clash detection.
	*/
	void SetSelectMode( int mode );
	
	/*!This method gets the world space volume of the given segment. This is a helper function to #SelectByWorldVolume.
	\param seg The key of segment in which you want to compute the bounding box.
	\param min Returns the minimum point of the bounding box volume.
	\param max Returns the maximum point of the bounding box volume.
	\return An #HOperatorReturn indicating the status of the event.
	*/
	int GetWorldVolume( HC_KEY seg, HPoint & min, HPoint & max );

	/*!This method temporary "steals" the selection set from the HBaseView and replaces it with a new empty one.*/
	void GrabSelectionSet();
	
	/*!This method restores the selection set back to the HBaseView.*/
	void RestoreSelectionSet();

	/*!The spriting mode before this operator started up which is restored on deletion.*/
	bool m_save_spriting_mode;
	
	/*!The set of objects being dragged around.*/
	HSelectionSet * m_pClashSelection;
	
	/*!The type of computing selection described in #HOOC_SelectMode enum.  #HOOC_Shell is the default.*/
	int m_SelectMode;
	
	/*! A boolean to indicate whether or not to calculate the polylines of intersection between objects that we find clashing.*/
	int m_IntersectPolylinesVisible;
	
	/*! The key to the segment into which the text overlay, with a transparent window, is put.*/
	HC_KEY m_TempSegKey;

	/*! This method performs a translation on the selected objects and then highlights objects that intersected with them.
		\param hevent An HEventInfo object containing information about the current event. */
	void ClashDetect (HEventInfo & hevent);

public:

	/*! Constructs an HOpObjectClash object.
	  \param view A pointer to an HBaseView object,
	  \param DoRepeat An integer denoting whether this is a repeatable operator.  This parameter has been deprecated.
	  \param DoCapture An integer denoting whether the mouse state should be captured, which means that all mouse events should be received after a mousedown, even if it leaves the window.	
	*/
	HOpObjectClash (HBaseView * view, int doRepeat = 0, int doCapture = 1);

	//!The destructor restores the HBaseView's HSelectionSet to its state before this operator was initialized.
	virtual ~HOpObjectClash();

	/*!
		\return A pointer to a character string denoting the name of the operator which 'HOpObjectClash'.
	*/
	virtual const char * GetName();  

	/*!
		OnLButtonDown records the first pointer position and initializes the translation mechanism.
		It then calls through to #ClashDetect.  It "steals" the view's selection set and sets it 
		as a member variable so that the objects with which collisions are detected can be put there. 
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDown (HEventInfo & hevent);

	/*!
		This method calls through to #ClashDetect to do most of its work.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonDownAndMove(HEventInfo & hevent);

	/*!
		OnLButtonUp deselects the highlighted objects.
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnLButtonUp (HEventInfo & hevent);

	/*! This method grabs the following hot keys and then passes the rest to HBaseOperator.
	<ul>
	<li><b>d</b> : This hot key puts the operator into screen selection mode where it computes the selections in screen space 
	and looks for bounding box intersections.
	</li>
	<li><b>e</b> : This hot key puts the operator into enclosure selection mode where it detect if the original shell completely 
	encloses the selected shell.
	</li>
	<li><b>p</b> : This hot key toggles on and off the indicate whether to calculate the polylines of intersection between 
	objects that we find clashing.
	</li>
	<li><b>s</b> : This hot key puts the operator into shell selection mode where it looks for shells that intersect the 
	faces of the current selection set in HBaseView. Use shell selection mode for accuracy.
	</li>
	<li><b>w</b> : This hot key puts the operator into world selection mode where it computes the selections in world space 
	prior to camera tranformations and looks for bounding box intersections. Use world selection mode for speed.
	</li>
	</ul>
		\param hevent An HEventInfo object containing information about the current event.
		\return An #HOperatorReturn indicating the status of the event.
	*/
	virtual int OnKeyDown (HEventInfo & hevent);

	/*! 
	  Creates a new operator which is a copy of the current operator.  The user is 
	  responsible for deleting the newly created operator.
	  \return A pointer to the newly created HOpObjectClash object.
	*/
	virtual HBaseOperator * Clone(); 

	/*!	
		This method is used to update the list of keys to translate when the selection list changes.
	*/
	virtual void UpdateKeyList(void * data);

};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



