// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// HUtilityAnnotation.h : interface of the HUtilityAnnotation class



#ifndef _HUtilityAnnotation_H
#define _HUtilityAnnotation_H
 
#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "hic_types.h"

#include "HTools.h"

class HPoint;
class HBaseView;
class HCamera;

#define BG_CIRCLE	0
#define BG_QUAD		1
#define BG_CLOUD	2
#define BG_NONE		3


//! The HUtilityAnnotation class defines and manages annotations
/*!
	This class simplified the creation of annotations.
*/ 
class MVO_API HUtilityAnnotation
{
public:
	HUtilityAnnotation() : m_MyTextKey(INVALID_KEY) {;};

	virtual ~HUtilityAnnotation() {;};

	/*! Adds a char to the note */
	virtual void AddChar(char the_char);

	/*! Adds a unicode char to the note */
	virtual void AddChar(unsigned short the_char);

	/*! Returns a key to the background segment.  In the segment a user-defined background
		can be set.  Assume the text will range from x=[-1, 1] y=[-1, 1] and the background
		will be scaled to fit the text */
	virtual HC_KEY GetBackgroundSegmentKey();

	/*! Returns the key to the root of the note sub-tree */
	virtual HC_KEY GetKey() {return m_MyKey;}

	/*! Returns the key to the root of the note sub-tree giving a key in that subtree. */
	static HC_KEY GetKey(HC_KEY);

	/*! Fills text with the current note text 
		\param text a pointer to where the text should be written (may be null)
		\param length return values for how much text was put in the buffer */
	virtual void GetText(char * text, int &length);

	/*! Returns just the length of the text */
	virtual int  GetTextLength();

	/*! Fills text with the current note text 
		\param text a pointer to where the text should be written (may be null)
		\param length return values for how much text was put in the buffer */
	virtual void GetTextWithEncoding(unsigned short * text, int &length);

	/*! Returns "HUtilityAnnotation" */
	virtual char const* GetType() {return "HUtilityAnnotation";};

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
		const char * text,
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
		const unsigned short * text,
		HPoint target,
		HPoint position,
		HC_KEY AnnotateThis,
		HBaseView * pView,
		HC_KEY layer_key = INVALID_KEY,
		int bg_type = BG_QUAD,
		bool useArrow = true,
		HC_KEY * key_path = (HC_KEY*)0,
		int key_path_length = 0);
	
	/*! Returns true if the key is an Annotation object (also returns true for objects
		deriving from this class.  If this function returns try on this key, SetNote may
		be called on this key to do some limited things to that note such as changing the
		text and calling update. */
	static bool IsAnnotation(HC_KEY key);

	/*! Returns true if noteskey refers to a note of this type.  This key can then be
		passed to SetNote() */
	static bool IsNote(HC_KEY noteskey);

	/*! Change the font on all annotations in the s*/
	static void ChangeAllFonts(HBaseView * view, char const * const font, HC_KEY seg_key = INVALID_KEY);

	/*! Repositions the text */
	virtual void MoveText(HPoint const & pos, HCamera const * camera = 0);

	/*!  Repositions the text and target.  */
	virtual void MoveText(HPoint const & tar, HPoint const & pos, HCamera const * camera = 0);

	/*! Sets the background type to one of the four predefined types
		\param type can be 0, 1, 2, 3 or BG_CIRCLE, BG_QUAD, BG_CLOUD, BG_NONE */
	virtual void SetBackgroundType(long type);

	/*! sets the color at the root of the note segment tree
		\param color can be anything that HC_Set_Color accepts */
	virtual void SetColor(char const * color);

	/*! Sets the edge weight at the root of the note segment tree */
	virtual void SetEdgeWeight(float weight);

	/*! Sets the extents of the background.  The text will be scaled to fit within these extents */
	virtual void SetExtents(int xpixels, int ypixels, HC_KEY view_key);

	/*! Sets the face pattern at the root of the note segment tree
		\param pattern can be anything that HC_Set_Face_Pattern accepts */
	virtual void SetFacePattern(char const * pattern);

	/*! Sets the line pattern at the root of the note segment tree 
		\param pattern can be anything that HC_Set_Line_Pattern accepts */
	virtual void SetLinePattern(char const * pattern);

	/*! Sets the line weight at the root of the note segment tree */
	virtual void SetLineWeight(float weight);

	/*! Changes the note the class refers to
		\param noteskey should be a key that IsNote returns true on and is the key to the note we want this
			class to refer to and manipulate
		\param view_key The view key where a camera can be found by compute text extents */
	virtual void SetNote(HC_KEY noteskey, HC_KEY view_key);

	/*! Changes the note the class refers to
		\param noteskey should be a key that IsNote returns true on and is the key to the note we want this
			class to refer to and manipulate
		\param pView The view where a camera can be found by compute text extents */
	virtual void SetNote(HC_KEY noteskey, HBaseView* pView);
	
	/*! Sets this note on top of "normal" geometry
		\param t if true, the note will be on top, false will unset the note as on top. */
	virtual void SetOnTop(bool t);

	/*! This return if the note on top of "normal" geometry
		\return If note is on top */
	virtual bool GetOnTop();

	/*! Sets the text of this note */
	virtual void SetText(char const * text);

	/*! Sets the font for this note.  If no font is set the font is inherited from it's parent.
		\param font is anything that is accepted by HC_Set_Text_Font Setting points or pixels *may*
			result in incorrectly sized text when printing. */
	virtual void SetTextFont(char const * font);

	/*! Sets unicode text for this note */
	virtual void SetTextWithEncoding(unsigned short const * text);

	/*! Returns the position of the text in world space */
	virtual void ShowPosition(HPoint &p);

	/*! Returns the position of the text in world space */
	virtual void ShowPosition(float *x, float *y, float *z);

	/*! Shows the target point in world space.  The target point is the point that this note is annotating */
	virtual void ShowTarget(HPoint &p);

	/*! Shows the target point in world space.  The target point is the point that this note is annotating */
	virtual void ShowTarget(float *x, float *y, float *z);

	/*! Unsets the extents set on this note if an extent is set. */ 
	virtual void UnSetExtents();

	/*! This must be called after a note is modified and before the scene is updated.  This method updates
		the size of the background and fits it to the text.  If you're note aren't looking like you expect,
		you may be forgetting to update the notes with this method.  */
	virtual void Resize(int include_count, HC_KEY include_keys[], HC_KEY view_key);

	virtual void Resize(HC_KEY include_keys, HC_KEY view_key) {
		HC_KEY key = include_keys;
		Resize(1, &key, view_key);
	};

	/* This is the direction the note was facing the last time it was moved (if the caller passed
	 * in a camera on the last move). */
	virtual HPoint GetFacing() const;

	/* This is the up direction of the note the last time it was moved (if the caller passed
	 * in a camera on the last move). */
	virtual HPoint GetUp() const;

	/*! This method is registered to listen for window resize events.  If the window
	    is resized the all annotations with points or pixel sized fonts must be updated */
	static void WindowResizeCallback(int signal, void * signal_data, void * user_data);

	/*! This method is normally called from WindowResizeCallback, but it can also get called
		by user code where there's no real view (such is in two pass printing). */
	static void recursive_resize(HC_KEY seg_key, int include_count, HC_KEY include_keys[], HC_KEY view_key);

 	static void drawLineCallback (HIC_Rendition const *rendition,int count, HIC_DC_Point p[]);
	static void drawLineCallbackClip (HIC_Rendition const *rendition,int count, HIC_DC_Point p[]);
 	static void drawMarkerCallback (HIC_Rendition const *rendition, HIC_DC_Point *p);
private:

 
 
	/*! this is called by ChangeAllFonts to change the fonts of all the annotations in a subtree. */
	static void recursive_change_font(HC_KEY seg_key, int include_count, HC_KEY include_keys[], HC_KEY view_key, char const * const font);

	void SetConditionalVisibility(HBaseView * pView, HC_KEY const key_path[], int const key_path_length);
	
protected:
	static int	 Clip_Line_Inverse(HIC_DC_Point  *p, HIC_DC_Point  *po, int top, int left, int bottom, int right);
	static int	 Clip_Line(HIC_DC_Point  *p, HIC_DC_Point  *po, int top, int left, int bottom, int right);

	HC_KEY m_MyKey;
	HC_KEY m_MyTextKey;
	static HC_KEY g_segmentkey;
	static int g_width;
	static int g_height;

	HPoint m_target, m_position;
};

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif



