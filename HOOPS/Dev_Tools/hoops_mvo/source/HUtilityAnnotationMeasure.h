// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityAnnotationMeasure.h : interface of the HUtilityAnnotationMeasure class

#ifndef _HUtilityAnnotationMeasure_H
#define _HUtilityAnnotationMeasure_H
 
#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "hic_types.h"

#include "HTools.h"
#include "HUtilityAnnotation.h"

//! The HUtilityAnnotationMeasure class defines and manages annotations
/*!
	This class simplified the creation of measurement annotations.
*/ 
class MVO_API HUtilityAnnotationMeasure : public HUtilityAnnotation
{
public:
	/*! Returns "HUtilityAnnotationMeasure" */
	virtual char const* GetType() {return "HUtilityAnnotationMeasure";};

	/*! Inserts a note
		\param target where the arrow should point
		\param position where the text is positioned
		\param AnnotateThis key to what this annotation is annotating
		\param pView pointer to the view
		\param layer_key style segment for this note
		\param bg_type one of four stock background types
		\param useArrow sets the visibility of the arrow
		\param key_path is the key path returned by HC_Show_Selection_Original_Keys
		\param key_path_length is the length returned by HC_Show_Selection_Original_Keys
		\return key to the root of the note segment tree */
	virtual HC_KEY Insert(
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key = INVALID_KEY,
		int bg_type = BG_QUAD,
		bool useArrow = true,
		HC_KEY * key_path = (HC_KEY*)0,
		int key_path_length = 0);

	/*! Inserts a note
		\param text initial text for this note
		\param target where the arrow should point
		\param position where the text is positioned
		\param AnnotateThis key to what this annotation is annotating
		\param pView pointer to the view
		\param layer_key style segment for this note
		\param bg_type one of four stock background types
		\param useArrow sets the visibility of the arrow
		\param key_path is the key path returned by HC_Show_Selection_Original_Keys
		\param key_path_length is the length returned by HC_Show_Selection_Original_Keys
		\return key to the root of the note segment tree */
	virtual HC_KEY Insert(
		char const * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key = INVALID_KEY,
		int bg_type = BG_QUAD,
		bool useArrow = true,
		HC_KEY * key_path = (HC_KEY*)0,
		int key_path_length = 0);

	/*! Inserts a note
		\param text initial text for this note
		\param target where the arrow should point
		\param position where the text is positioned
		\param AnnotateThis key to what this annotation is annotating
		\param pView pointer to the view
		\param layer_key style segment for this note
		\param bg_type one of four stock background types
		\param useArrow sets the visibility of the arrow
		\param key_path is the key path returned by HC_Show_Selection_Original_Keys
		\param key_path_length is the length returned by HC_Show_Selection_Original_Keys
		\return key to the root of the note segment tree */
	virtual HC_KEY InsertWithEncoding(
		unsigned short const * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key = INVALID_KEY,
		int bg_type = BG_QUAD,
		bool useArrow = true,
		HC_KEY * key_path = (HC_KEY*)0,
		int key_path_length = 0);

	/*! Move the text and measuring line to position and leave dashed leader lines to the original points a and b */
	virtual void MoveText(HPoint position);

	/*! Sets the start and end point of the measurement and places the text half way between. */
	virtual void MoveText(HPoint A, HPoint B);

	/*! Returns true if the key passed in is a measurement annotation key */
	static bool IsMeasurement(HC_KEY key);

	/*! Sets the annotation class to refer to the annotation with key noteskey.  Before passing a key to this
		function be sure that IsMeasurement return true on that key.  */
	virtual void SetNote(HC_KEY noteskey, HBaseView *view);
 	static void drawLineCallbackDummy (HIC_Rendition const *rendition,int count, HIC_DC_Point p[]);

protected:
	static void mid_point(HPoint &A, HPoint &B, HPoint &mid);

	HPoint m_pnt_a1, m_pnt_b1;
	HPoint m_pnt_a2, m_pnt_b2, m_position;
};


#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



