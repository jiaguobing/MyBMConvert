// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HIM_UTILITY_H
#define _HIM_UTILITY_H

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif

#include "HTools.h"
#include "hic_types.h"

/*! 
    HImUtility  contains a number of widely used IM callbacks.  Callbacks are attributes of a segment and can be set in 
	one of two ways.  The first is by calling the 3dGS function #HC_Set_Callback("callback_point = callbback_name") or 
	the by using the MVO method #HImSetCallback("callback_point", "callback_name").  If you are using MVO then it is 
	recommended that you use the HImSetCallback approach.  The callback name and callback point are both documented 
	for each callback below.
*/
class MVO_API HImUtility
{
	private:

		HImUtility() {};

	public:
	/*! This method registers all the callbacks in this class with 3dGS and the MVO IM Manager */
		static void RegisterCallbacks();

	/*! This callback sets a rectilinear clip region on the segment.  When you set this callback, you must
	set the user options of "left=i, right=j, top=k, bottom=l" on the segment that you want the clip 
	performed. I, j, k and l are floating point numbers which define the world space borders of the clip region.  
	The callback is called "HImUtility_set_clip_rectangle" and the callback point is "draw segment tree".
	*/
		static void set_clip_rectangle (HIC_Rendition const *nr, HIC_Segment_Info const *si);

	/*! This callback does not draw anything and consequently causes no geometry to be drawn for the segment 
	it is set on.  The callback is called "HImUtility_suppress_3d_geometry" and the callback point is 
	"draw 3d geometry".
	*/
		static void suppress_3d_geometry(HIC_Rendition const *rendition, HIC_Geometry const *geo3);

	/*! This callback draws the window background in a gradiated fashion.  It is no longer needed as this window 
	coloring scheme is supported in 3dGS but is included for backward compatibility reasons.  The callback is 
	called "HImUtility_draw_gradiated_window_background" and the callback point is "draw window".
	*/
		static void draw_gradiated_window_background (HIC_Rendition const *rendition, HIC_Int_Rectangle const *extent);

	/*! This callback only draws the lines in a segment if the visibility of text is on.  Developers use it to easily 
	turn off the visibility of leaders of annotation data.  The callback is called "HImUtility_draw_annotation_lines" 
	and the callback point is "draw 3d polyline".
	*/
		static void draw_annotation_lines(HIC_Rendition const *nr, HIC_Polyline const*poly);

	/*! This callback draws all the faces of the shells in the segment in the back 10% of the scene forcing them to 
	appear behind everything else in the scene. The callback is called "HImUtility_draw_segment_in_background" and 
	the callback point is "draw dc reshaded triangle".
	*/
		static void draw_segment_in_background(HIC_Rendition const *nr, HIC_DC_Point const points[], HIC_RGBA const colors[], HIC_Plane const planes[], HIC_Parameter const params[], HIC_Integer32 param_width, HIC_Parameter_Flags param_flags);

	/*! This callback draws the lines in the front 1% of the scene forcing the lines to appear on top of everything 
	else in the scene.  The callback is called "HImUtility_draw_dc_polyline_infront" and the callback point is 
	"draw dc polyline".
	*/
		static void draw_dc_polyline_infront (HIC_Rendition const *rendition,int count, HIC_DC_Point const geo3[]);

	/*! This callback draws the lines and edges with device independent weights.  This is particularily useful in cases 
	where you are using line/edge weights greater than 1 and want WYSIWYG printing.	 The callback is called 
	"HImUtility_device_independent_polyline_weight" and the callback point is "draw dc polyline".
	*/
		static void draw_device_independent_line_weights (HIC_Rendition const *rendition,int count, HIC_DC_Point const geo3[]);

	/*! This callback draws the text in the front 1% of the scene forcing the text to appear on top of everything else in 
	the scene.  The callback is called "HImUtility_draw_text_infront" and the callback point is "draw text".
	*/
		static void drawTextInfront (HIC_Rendition const *rendition, HIC_Text_Info const * text_info);

	/*!
		This callback was written to show users how to collect the output from a hidden line rendering of a scene.
	*/
	  static void visible_hlr_polyline (HIC_Rendition const *rendition,int count, HIC_DC_Point const geo3[]);
 
};



#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif







