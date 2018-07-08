// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HD_PROTO_DEFINED
#define HD_PROTO_DEFINED 1

#ifndef DRIVER_DEFINED
static_assert(false, "now included from driver.h only!");
#endif


extern "C" bool HC_CDECL HD_CGM3_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_DX11_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_DX9_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_HPGL2_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_HPGL_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_Image_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_MSW_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_OpenGL_Driver (Thread_Data * thread_data, Actor * actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_OpenGL2_Driver (Thread_Data * thread_data, Actor * actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_PostScript_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_PDF_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_Printf_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_QT_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_Quartz_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_Whip2D_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_Win32_OpenGL_Driver (Thread_Data * thread_data, Actor * actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_Win32_OpenGL2_Driver (Thread_Data * thread_data, Actor * actor, int request, void * request_info);
extern "C" bool HC_CDECL HD_X11_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);



GLOBAL_FUNCTION Actor * HD_Create_Preview_Actor(Display_Context * dc, Driver_Action driver_action);

GLOBAL_FUNCTION int HD_Acquire_Clip_Points (
	Net_Rendition const &	nr,
	Rectangle const &		soft_clip,
	Bounding const &		bbox,
	int						count,
	Point const *			object_points,
	DPoint const *			dobject_points,
	DC_Point *				clip_points,
	float *					ws,
	unsigned char *			clip_case);

GLOBAL_FUNCTION void HD_Allow_For_Border (Display_Context *dc,
		 int delta_left,
		 int delta_right,
		 int delta_bottom,
		 int delta_top);
GLOBAL_FUNCTION bool HD_Artcore_Renderer (Net_Rendition const & nr, struct ht_renderer_actions *renderer);
GLOBAL_FUNCTION bool HD_Box_Outside_Cutting_Planes (Transform_Rendition const & tr,
		 float x,
		 float y,
		 float z,
		 int width,
		 int depth);
GLOBAL_FUNCTION bool HD_Box_Within_Cutting_Planes (Transform_Rendition const  &tr,
		 float x,
		 float y,
		 float z,
		 int width,
		 int depth);
GLOBAL_FUNCTION void HD_Buffer_DC_Polydot_C08_Z00 (Net_Rendition const & nr, int count, DC_Point const *ipoint);
GLOBAL_FUNCTION void HD_Buffer_DC_Polydot_C08_Z16 (Net_Rendition const & nr, int count, DC_Point const *ipoint);
GLOBAL_FUNCTION void HD_Buffer_DC_Polydot_C24_Z00 (Net_Rendition const & nr, int count, DC_Point const *ipoint);
GLOBAL_FUNCTION void HD_Buffer_DC_Polydot_C24_Z16 (Net_Rendition const & nr, int count, DC_Point const *ipoint);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C08_Z00_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C08_Z00_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C08_Z00_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C08_Z16_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *color,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C08_Z16_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C08_Z16_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C24_Z00_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C24_Z00_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C24_Z00_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C24_Z16_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C24_Z16_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polyline_C24_Z16_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Raster_C08_Z00 (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int row_bytes,
		 unsigned char const *raster);
GLOBAL_FUNCTION void HD_Buffer_DC_Raster_C08_Z16 (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int row_bytes,
		 unsigned char const *raster);
GLOBAL_FUNCTION void HD_Buffer_DC_Raster_C24_Z00 (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int row_bytes,
		 RGBAS32 const *raster);
GLOBAL_FUNCTION void HD_Buffer_DC_Raster_C24_Z16 (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int row_bytes,
		 RGBAS32 const *raster);
GLOBAL_FUNCTION void HD_Buffer_DC_Raster_C32_Z00 (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int row_bytes,
		 RGBAS32 const *raster);
GLOBAL_FUNCTION void HD_Buffer_DC_Raster_C32_Z16 (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int row_bytes,
		 RGBAS32 const *raster);
GLOBAL_FUNCTION void HD_Buffer_DC_Rectangle_C08 (Net_Rendition const & nr, int left, int right, int bottom, int top);
GLOBAL_FUNCTION void HD_Buffer_DC_Rectangle_C24 (Net_Rendition const & nr, int left, int right, int bottom, int top);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C08_Z00_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C08_Z00_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C08_Z00_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C08_Z16_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C08_Z16_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C08_Z16_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C24_Z00_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C24_Z00_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C24_Z00_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C24_Z16_C (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors,
		 bool single);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C24_Z16_G (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Buffer_DC_Polytris_C24_Z16_U (Net_Rendition const & nr, int count,  DC_Point const *points);
GLOBAL_FUNCTION void HD_Calculate_Outer_Window (Net_Rendition & nr);
GLOBAL_FUNCTION bool HD_Calc_Image_Position_And_Size(Net_Rendition const & nr,
		 Image const *image, int & width, int & height, Point & point);
GLOBAL_FUNCTION void HD_Choose_Font (Net_Rendition const & nr,
		 Font_Name const *possible_names,
		 char encoding,
		 bool complain);
GLOBAL_FUNCTION void HD_Clear_Software_Z_Buffer (Net_Rendition const & nr);
GLOBAL_FUNCTION void  HD_Compute_Hard_Clip (Net_Rendition & nr, Rectangle const *new_scope);
GLOBAL_FUNCTION void HD_Clip_DC_Face (
	Net_Rendition const & nr,
	int count,
	DC_Point const * points,
	RGBAS32 const *color,
	int flags);
GLOBAL_FUNCTION void HD_Clip_DC_Polydot (
	Net_Rendition const & nr,
	int count,
	DC_Point const * points,
	RGBAS32 const *rgb32_colors,
	int flags);
GLOBAL_FUNCTION void HD_Clip_DC_Polyline (
	Net_Rendition const & nr,
	int count,
	DC_Point const * points,
	int flags,
	RGBAS32 const *rgb32_colors = null,
	RGBA const *rgba_colors = null,
	Plane const * planes = null,
	DPlane const * dplanes = null,
	Parameter const *vertex_params = null,
	int param_width = 0,
	Parameter_Flags param_flags = 0);
GLOBAL_FUNCTION void HD_Clip_DC_Polytriangle (
	Net_Rendition const & nr,
	int count,
	DC_Point const * points,
	int flags,
	RGBAS32 const *rgb32_colors = null,
	RGBA const *rgba_colors = null,
	Plane const * planes = null,
	DPlane const * dplanes = null,
	Parameter const *vertex_params = null,
	int param_width = 0,
	Parameter_Flags param_flags = 0);
GLOBAL_FUNCTION void HD_Clip_DC_Text (
	Net_Rendition const & oldnr,
	DC_Point const * position,
	int count,
	unsigned short const *kstring);
GLOBAL_FUNCTION void HD_Clip_Poly (
	IntRectangle const *	clip_rec,
	int						n_in,
	DC_Point const *		in_points,
	int *					n_out,
	DC_Point *				out_points,
	bool					close);


GLOBAL_FUNCTION Plane HD_Find_Geometry_Plane (Geometry const *geometry);

GLOBAL_FUNCTION void HD_Compute_Screen_Plane_Facings (
	Matrix const &	matrix,
	int				count,
	Plane const *	planes,
	int *			directions,
	float *			dots=null);

GLOBAL_FUNCTION void HD_Compute_Screen_Plane_Facings(
	Matrix const &	matrix,
	int				count,
	DPlane const *	planes,
	int *			directions,
	float *			dots = null);

GLOBAL_FUNCTION void HD_Compute_Screen_Transform (Net_Rendition const & nr, Vector & scale, Vector & translate);

GLOBAL_FUNCTION int HD_Compute_Trimmed_Clip_Points (
	Transform_Rendition const &	tr,
	int							count,
	DC_Point const *			dc_points,
	float const *				ws,
	unsigned char *				clip_case);

GLOBAL_FUNCTION bool HD_Connect_Standard_Driver (
	Thread_Data *	thread_data,
	Actor *			actor,
	int				request,
	void (*action_getter) (Action_Table & at));
GLOBAL_FUNCTION Cutting_Plane_Instance* HD_Create_Hard_Clip_Cuts(Net_Rendition const & inr, IntRectangle const *clip_rec, int bias = 0);
GLOBAL_FUNCTION int HD_Cut_Hidden_By_Plane (Display_Context const * dc, Hidden *old_poly, Plane *plane);

GLOBAL_FUNCTION void HD_Defer_Tree (Net_Rendition const & nr, Segment const * seg, Include_Path const & path);
GLOBAL_FUNCTION void HD_Defer_Tree_Batch (Net_Rendition const & nr, Segment const * seg, Include_Path const & path, int batchnum, Geometry const * geometry = 0, int force_defer_batch=-1);
GLOBAL_FUNCTION int HD_Determine_Elliptical_Res (Net_Rendition const & nr, Generic_Arc const *curve);
GLOBAL_FUNCTION void HD_Determine_Fixed_Colors (Display_Context const *dc, int start, int *fixed, int *parts);
GLOBAL_FUNCTION void HD_Downwind_Camera (Net_Rendition & nr, Camera_Handle const & camera, bool record_camera = false, bool retain_wlimit = false);
GLOBAL_FUNCTION Material_Rendition HD_Downwind_Named_Material (Net_Rendition const & nr, Material_Rendition const & matr, Named_Material const * newmat);
GLOBAL_FUNCTION int  HD_Downwind_Color (Net_Rendition & nr, Color const *color, Seen_Flags maybes, Attribute_Lock const * lock);
GLOBAL_FUNCTION void  HD_Downwind_Color_Map (Net_Rendition & nr, Attribute const *attribute);
GLOBAL_FUNCTION void  HD_Downwind_Handedness (Net_Rendition & nr, Attribute const *attribute);
GLOBAL_FUNCTION int  HD_Downwind_Rendition (Net_Rendition & nr, Attribute const *att, Seen_Flags maybes, bool follow_list, Attribute_Lock const * filter=null);
GLOBAL_FUNCTION int  HD_Downwind_Text_Font (Net_Rendition & nr, Text_Font const *font);
GLOBAL_FUNCTION void  HD_Downwind_Window (Net_Rendition & nr, Attribute const *attribute);
GLOBAL_FUNCTION void  HD_Downwind_Window_Frame (Net_Rendition & nr, Attribute const *attribute);
GLOBAL_FUNCTION void HD_Resolve_Modelling_History(Net_Rendition & nr);
GLOBAL_FUNCTION void  HD_Prepend_Modelling_Matrix (
		 Net_Rendition & nr,
		 Matrix const & matrix);
GLOBAL_FUNCTION void HD_Draw_Defined_Font (Net_Rendition const & in_nr,
		 Font_Instance const *specific,
		 Point const *where,
		 int count,
		 unsigned short const *string,
		 Vector const *scale);
GLOBAL_FUNCTION void HD_Draw_Clipped_DC_Image (Net_Rendition const & nr,
		 DC_Point const *start,
		 DC_Point const *end,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Draw_Deferred_Trees (Display_Context *dc);

GLOBAL_FUNCTION void HD_Draw_Deferred_FBI_Lists (
	Net_Rendition const &	nr,
	Frame_Buffer_Info *		fbi,
	const char *			logging_note,
	bool					do_inplace = true,
	bool					do_spriting = false);


GLOBAL_FUNCTION bool HD_Draw_Alternate(Net_Rendition const & nr, Deferred_Draw_Tree * ddt = null);

GLOBAL_FUNCTION void HD_Draw_Freetype_Font (Net_Rendition const & nr,
		 Font_Instance const *specific,
		 Point const *where,
		 int count,
		 unsigned short const *string,
		 Vector const *scale);
GLOBAL_FUNCTION void HD_Draw_Glyphs (
		 Net_Rendition const &	in_nr,
		 Glyph const *			glyphs,
		 int					count,
		 DC_Point const *		points,
		 RGBAS32 const *		colors,
		 bool					single_color,
		 float const *			rotations,
		 float const *			size_fixups);
GLOBAL_FUNCTION Hidden * HD_Draw_Hidden_Item (Hidden *hidden, bool whole_list);
GLOBAL_FUNCTION void HD_Draw_Indexicated_3D_Polyedge (Net_Rendition const & inr, Polyedge const *ipe);
GLOBAL_FUNCTION void HD_Draw_Indexicated_3D_Tristrip (Net_Rendition const & inr, Tristrip const *its);
GLOBAL_FUNCTION void HD_Isoline_From_Tristrip (Net_Rendition const & inr, Tristrip const *its);
GLOBAL_FUNCTION void HD_Assemble_Isolines (
	Net_Rendition const & 			nr,
	Polyhedron *				polyhedron,
	unsigned short /* Composite_Face_Flags */	face_flags_to_be_used);
GLOBAL_FUNCTION int HD_Process_Isolines (
	Display_Context * dc,
	Geometry const * geometry,
	Bounding const & bounding,
	Polyedge * * linker,
	float tolerance);
GLOBAL_FUNCTION void HD_Draw_Lighted_Face (Net_Rendition const & nr,
		 int count,
		 DC_Point const *dcp,
		 Geometry const *geometry);
GLOBAL_FUNCTION void HD_Draw_Lighted_Polygon (Net_Rendition const & nr,
		 int count,
		 DC_Point const *dcp,
		 Geometry const *geometry);
GLOBAL_FUNCTION Tree_Process * HD_Prepare_Quickmoves_Tree (Net_Rendition & nr, char const * ident);
GLOBAL_FUNCTION void HD_Draw_Silhouette_Edges (Net_Rendition const & nr, Polyedge const *ipe);
GLOBAL_FUNCTION void HD_Draw_NonCulled_Edges (Net_Rendition const & nr, Polyedge const *ipe);
GLOBAL_FUNCTION void HD_Prepare_Soft_Z_Buffered_Tree (Net_Rendition & nr, Segment const * seg, Include_Path const & path);

GLOBAL_FUNCTION void HD_Draw_Trimmed_DC_Image (Net_Rendition const & nr,
	DC_Point const *		start,
	DC_Point const *		end,
	int						hoffset,
	char					format,
	int						row_bytes,
	void const * const *	rasters,
	int						pattern,
	Driver_Color const *	contrast_color,
	Image const *			db_image);
GLOBAL_FUNCTION void HD_Elaborate_Selection (Net_Rendition const & nr,
	Geometry const *		item,
	Key						item_key,
	Key						original_key,
	int						offset1,
	int						offset2,
	int						offset3,
	float					zw,
	float					proximity,
	Type_Flags				typeflag,
	Selection_Extras *		extras);
GLOBAL_FUNCTION bool HD_Exit_Update (Display_Context const *dc);
GLOBAL_FUNCTION bool HD_Check_Exit_Update (
	Net_Rendition const &	nr,	
	const char *			file, 
	int						line,
	intptr_t				ffdebug);
GLOBAL_FUNCTION void HD_Interrupt_Update (Display_Context const *dc, bool do_not_draw, bool recycle);
GLOBAL_FUNCTION void HD_Figure_Shading (Net_Rendition const & nr, Geometry const *geometry, RGBA *total);
GLOBAL_FUNCTION void HD_Find_One_Font (Display_Context const *dc);
GLOBAL_FUNCTION void HD_Find_Basic_Fonts (Display_Context const *dc);
GLOBAL_FUNCTION void HD_Find_All_Fonts (Display_Context const *dc);
GLOBAL_FUNCTION void HD_Find_Cutting_Planes (Net_Rendition & nr, Segment const *seg);
GLOBAL_FUNCTION Display_Context * HD_Find_DC_From_WID (intptr_t wid);
GLOBAL_FUNCTION int HD_Find_Dither_Pattern (Net_Rendition const & nr,
		 RGBAS32 const *target,
		 unsigned short *icomposites,
		 int *patterns);
GLOBAL_FUNCTION Font_Instance const * HD_Find_Fill_In_Font (Net_Rendition const & in_nr, Font_Instance const * in_font, unsigned short in_kar, unsigned short * out_kar);
GLOBAL_FUNCTION void HD_Find_Font_Height (Display_Context const *dc, Font_Instance const *const_instance);
GLOBAL_FUNCTION void HD_Find_Freetype_Fonts (Display_Context const *dc);
GLOBAL_FUNCTION bool HD_Find_One_Freetype_Font (Display_Context const *dc, char const *in_name);
GLOBAL_FUNCTION Glyph HD_Find_Glyph(Glyph_Rendition const & gr, const char * name_string, Name_Const const * name, bool error);

GLOBAL_FUNCTION Line_Style HD_Find_Solid_Line_Style(Net_Rendition const & nr);
GLOBAL_FUNCTION Line_Style HD_Find_Line_Style(Net_Rendition const & nr, const char * name);

GLOBAL_FUNCTION void HD_Find_Lights (Net_Rendition & nr, Segment const *seg);
GLOBAL_FUNCTION void HD_Finish_Priority_Inclusion (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Force_Font_Load (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Force_Partial_Refresh (Display_Context const *dc, int left, int right, int bottom, int top);
GLOBAL_FUNCTION void HD_Force_Refresh (Display_Context const *dc);
GLOBAL_FUNCTION void HD_Free_Colors (Display_Context *dc);
GLOBAL_FUNCTION void HD_Free_Defined_Font_Family (Display_Context *dc, Font_Family *family);
GLOBAL_FUNCTION void HD_Free_Fonts (Display_Context *dc);
GLOBAL_FUNCTION void HD_Free_Frame_Buffer_Info (Net_Rendition const & nr, Frame_Buffer_Info const *fbi);
GLOBAL_FUNCTION void HD_Free_Hidden_List (Display_Context const * dc, Hidden *hidden);
GLOBAL_FUNCTION void HD_Free_Hidden_Payload (Display_Context const * dc, Hidden_Payload *payload);
GLOBAL_FUNCTION void HD_Free_Material_Rendition (Material_Rendition const *matr);
GLOBAL_FUNCTION void HD_Free_Window_Rendition (Window_Rendition const *wr);
GLOBAL_FUNCTION bool HD_Gather_Device_Info (Display_Context const *dc, Gather_Device_Info * gdi);
GLOBAL_FUNCTION bool HD_Show_Window_Info (Display_Context const * dc, Show_Window_Info * swi);
GLOBAL_FUNCTION bool HD_Gather_Font_Info (Display_Context *dc, Gather_Font_Info *gfi);
GLOBAL_FUNCTION bool HD_Gather_Font_Names (Display_Context const *dc, Gather_Font_Names *gfn);
GLOBAL_FUNCTION void HD_Gather_Freetype_Text (Net_Rendition const & nr,
	Font_Instance const *	specific,
	size_t					count,
	unsigned short const *	string,
	Vector const *			scale,
	char					encoding,
	Gather_Font_Info *		gfi);
GLOBAL_FUNCTION bool HD_Gather_Screen_Extent (Display_Context *dc, Gather_Screen_Extent *gse);
GLOBAL_FUNCTION bool HD_Gather_Capping_Geometry (Display_Context * dc, Gather_Capping_Geometry * gcg);
GLOBAL_FUNCTION bool HD_Gather_Text_Extent (Display_Context *dc, Gather_Font_Info *gfi);
GLOBAL_FUNCTION bool HD_Gather_Text_Outline (Display_Context *dc, Gather_Font_Info *gfi);
GLOBAL_FUNCTION bool HD_Gather_Window_Extent (Display_Context *dc, Gather_Window_Extent *gwe);
GLOBAL_FUNCTION int * HD_Generate_DC_Screen_Facings (Net_Rendition const & nr, int count, Plane const * planes, bool object_space);
GLOBAL_FUNCTION int * HD_Generate_DC_Screen_Facings(Net_Rendition const & nr, int count, DPlane const * planes, bool object_space);
GLOBAL_FUNCTION void HD_Get_Actions (Action_Table & at, void (*driver_getter) (Action_Table & at));
GLOBAL_FUNCTION void HD_Get_Bit_Patterns (Display_Context *dc, bool reverse);
GLOBAL_FUNCTION void HD_Get_Bit_Patterns_8 (Display_Context *dc, bool reverse);
GLOBAL_FUNCTION void HD_Get_Colors (Display_Context *dc);
GLOBAL_FUNCTION void HD_Get_Current_Info (Display_Context *dc);
GLOBAL_FUNCTION void HD_Get_Gamma_Table (Display_Context *dc, bool nonlinear);
GLOBAL_FUNCTION void HD_Get_Physical_Info (Display_Context *dc);
GLOBAL_FUNCTION void HD_HL_DC_Colorized_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBAS32 const * color, bool single);
GLOBAL_FUNCTION void HD_HL_DC_Colorized_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBAS32 const * color, bool single);
GLOBAL_FUNCTION void HD_HL_DC_Colorized_Polymarker (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBAS32 const * color, bool single, float const * rotations, float const *size_fixups);
GLOBAL_FUNCTION void HD_HL_DC_Gouraud_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBAS32 const * colors);
GLOBAL_FUNCTION void HD_HL_DC_Gouraud_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBAS32 const * colors);
GLOBAL_FUNCTION void HD_HL_3D_Image (Net_Rendition const & nr, Image const * image);
GLOBAL_FUNCTION void HD_HL_DC_Image (Net_Rendition const & nr,
		 DC_Point const * start,
		 DC_Point const * end,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const * rasters,
		 int pattern,
		 Driver_Color const * contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_HL_DC_Polymarker (
		 Net_Rendition const & nr,
		 int count,
		 DC_Point const * point,
		 float const * rotations,
		 float const *size_fixups);
GLOBAL_FUNCTION void HD_HL_DC_Polydot(
		 Net_Rendition const & nr, 
		 int count, 
		 DC_Point const * point);
GLOBAL_FUNCTION void HD_HL_DC_Colorized_Polydot(
		 Net_Rendition const & nr, 
		 int count, 
		 DC_Point const * point,
		 RGBAS32 const * clr,
		 bool	single);
GLOBAL_FUNCTION void HD_HL_DC_Polyline (Net_Rendition const & nr, int count, DC_Point const * points);
GLOBAL_FUNCTION void HD_HL_DC_Polytriangle (Net_Rendition const & nr, int count, DC_Point const * points);
GLOBAL_FUNCTION void HD_HL_DC_Phong_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBA const * colors,
		 Plane const * planes,
		 DPlane const * dplanes);
GLOBAL_FUNCTION void HD_HL_DC_Textured_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBA const * colors,
		 Plane const * planes,
		 DPlane const * dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_HL_DC_Phong_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBA const * colors,
		 Plane const * planes,
		 DPlane const * dplanes);
GLOBAL_FUNCTION void HD_HL_DC_Textured_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const * points,
		 RGBA const * colors,
		 Plane const * planes,
		 DPlane const * dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_HL_DC_Text (Net_Rendition const & nr, DC_Point const *where, int count, unsigned short const *kstring);
GLOBAL_FUNCTION void HD_HL_Plain_Text (Net_Rendition const & nr,
		 Text const * dbtext,
		 Point const * point,
		 int count,
		 unsigned short const * kp,
		 Text_Karacter_Info const * ki);
GLOBAL_FUNCTION void HD_HL_DC_Text_Rasters (
	Net_Rendition const &		nr,
	DC_Point const *			start,
	DC_Point const *			end,
	int							row_bytes,
	RGBAS32 const *				rasters);
GLOBAL_FUNCTION void HD_HL_See_Whats_Hidden (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Hide_DC_Colorized_Face (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *color);
GLOBAL_FUNCTION void HD_Hide_DC_Colorized_Polyline (Net_Rendition const & nr, int count, DC_Point const *dcp, RGBAS32 const *color, bool single);
GLOBAL_FUNCTION void HD_Hide_DC_Colorized_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *color, bool single);
GLOBAL_FUNCTION void HD_Hide_DC_Face (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Hide_DC_Gouraud_Polyline (Net_Rendition const & nr, int count, DC_Point const *dcp, RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Hide_DC_Gouraud_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Hide_DC_Image (Net_Rendition const & nr,
		 DC_Point const *start,
		 DC_Point const *end,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Hide_DC_Polydot (Net_Rendition const & nr, int count, DC_Point const *dcp);
GLOBAL_FUNCTION void HD_Hide_DC_Colorized_Polydot (Net_Rendition const & nr, int count, DC_Point const *dcp, RGBAS32 const *color, bool single);
GLOBAL_FUNCTION void HD_Hide_DC_Polymarker (Net_Rendition const & nr, int count, DC_Point const * points,
											float const * rotations, float const * size_fixups);
GLOBAL_FUNCTION void HD_Hide_DC_Colorized_Polymarker (Net_Rendition const & nr, int count, DC_Point const * points,
													  RGBAS32 const * colors, bool single_color,
													  float const * rotations, float const * size_fixups);
GLOBAL_FUNCTION void HD_Hide_DC_Polygon (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Hide_DC_Polyline (Net_Rendition const & nr, int count, DC_Point const *dcp);
GLOBAL_FUNCTION void HD_Hide_DC_Polytriangle (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Hide_DC_Phong_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const *dcp,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes);
GLOBAL_FUNCTION void HD_Hide_DC_Phong_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes);
GLOBAL_FUNCTION void HD_Hide_DC_Textured_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const *dcp,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Hide_DC_Textured_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Hide_DC_Stenciled_Quad (Net_Rendition const & nr,
		 DC_Point const *points,
		 Stencil_Bitmap const *stencil);
GLOBAL_FUNCTION void HD_Hide_DC_Text (Net_Rendition const & nr, DC_Point const *where, int count, unsigned short const *kstring);
GLOBAL_FUNCTION void HD_Hide_Plain_Text (
		 Net_Rendition const & nr,
		 Text const * dbtext,
		 Point const * position,
		 int count,
		 unsigned short const * kp,
		 Text_Karacter_Info const * ki);
GLOBAL_FUNCTION void HD_Image_To_Bit_Rasters (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *image_rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Image_To_Gray8_Rasters (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *image_rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Image_To_Mapped16_Rasters (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *image_rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Image_To_Mapped8_Rasters (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *image_rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Image_To_RGB32_Rasters (Net_Rendition const & nr,
		 DC_Point const *istart,
		 DC_Point const *iend,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *image_rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION Frame_Buffer_Info * HD_Init_Frame_Buffer_Info (Net_Rendition const & nr, Segment const * seg, Include_Path const & path);
GLOBAL_FUNCTION void HD_Initialize_HL_Data (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Insert_In_Hidden_Tree (Hidden *item);
GLOBAL_FUNCTION void HD_Kill_Driver (Display_Context const *dc);
GLOBAL_FUNCTION void HD_Draw_Nada (Display_Context const * dc);
GLOBAL_FUNCTION bool HD_Load_Font (Display_Context const *dc, Font_Instance const *const_instance);
GLOBAL_FUNCTION bool HD_Unload_Oldest_Font (Display_Context *dc, int font_type);
GLOBAL_FUNCTION void * HD_Load_Defined_Font (Display_Context const *dc, char const *name, Font_Instance *font);
GLOBAL_FUNCTION void HD_Get_Freetype_Font_Bounding (Font_Instance const * instance, SimpleCuboid & font_bbox);
GLOBAL_FUNCTION void * HD_Load_Freetype_Font (Display_Context const *dc, char const *name, Font_Instance *instance);
GLOBAL_FUNCTION void HD_Get_Freetype_Font_Family_Name(Display_Context const * dc, char const * name, char * family_name);
GLOBAL_FUNCTION bool HD_Mark_Font_As_Aliased (Display_Context const *dc, char const *name, char const *real_name_string, long index);
GLOBAL_FUNCTION bool HD_Mark_Font_As_Defined (Display_Context const *dc, char const *name, Font const *font);
GLOBAL_FUNCTION bool HD_Mark_Font_As_Freetype (Display_Context const *dc, char const *name);
GLOBAL_FUNCTION bool HD_Mark_Font_As_Generic (Display_Context const *dc, char const *name, int generic);
GLOBAL_FUNCTION bool HD_Mark_Font_As_System_Default (Display_Context const *dc, char const *name, float Aheight);
GLOBAL_FUNCTION bool HD_Mark_Font_As_Transformable (Display_Context const *dc, char const *name, int transformable);
GLOBAL_FUNCTION bool HD_Measure_Characters (Net_Rendition const & nr, size_t count, unsigned short const *kp, char const encoding, float *size_out);
GLOBAL_FUNCTION bool HD_Measure_Freetype_Char (Net_Rendition const & nr, Font_Instance const *font, unsigned short kar, float *size_out, bool *missing, void ** per_char_data);
GLOBAL_FUNCTION bool HD_Measure_Defined_Char (Net_Rendition const & nr, Font_Instance const *font, unsigned short kar, float *size_out, bool *any_missing, void ** per_char_data);
GLOBAL_FUNCTION bool HD_Minimum_Update (Display_Context *dc);
GLOBAL_FUNCTION float HD_NURBS_Point_Invert (NURBS_Curve const *curve, Point const *point);
GLOBAL_FUNCTION bool HD_No_Driver (Thread_Data * thread_data, Actor const *actor, int request, char const *driver_name);
GLOBAL_FUNCTION bool HD_No_Such_Keyboard (Display_Context const *dc, int *button, int *status);
GLOBAL_FUNCTION bool HD_No_Such_Locater (Display_Context const *dc, int *button, int *x, int *y);

GLOBAL_FUNCTION void HD_Painters_Bias_Z (Hidden *hidden, bool backwards);
GLOBAL_FUNCTION bool HD_Point_Within_Cutting_Planes (Transform_Rendition const & tr, DC_Point const & point);
GLOBAL_FUNCTION void HD_Preprocess_Mapped_Colors(Net_Rendition & nr);
GLOBAL_FUNCTION void HD_Queue_Pixel_Location_Event (Display_Context const *dc, int status, int ix, int iy);
GLOBAL_FUNCTION void HD_Rasterize_DC_Spans_C08_U (Net_Rendition const & nr,
		 Span_Appearance const & sr,
		 Span const *span,
		 int num_spans,
		 int bottom);
GLOBAL_FUNCTION void HD_Rasterize_DC_Spans_C24_G (Net_Rendition const & nr,
		 Span_Appearance const & sr,
		 Span const *span,
		 int num_spans,
		 int bottom);
GLOBAL_FUNCTION void HD_Rasterize_DC_Spans_C24_R (Net_Rendition const & nr,
		 Span_Appearance const & sr,
		 Span const *span,
		 int num_spans,
		 int bottom,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Rasterize_DC_Spans_C24_U (Net_Rendition const & nr,
		 Span_Appearance const & sr,
		 Span const *span,
		 int num_spans,
		 int bottom);
GLOBAL_FUNCTION void HD_Record_Driver_Options (Display_Context *dc, Attribute const *attribute);
GLOBAL_FUNCTION void HD_Record_Font (Display_Context const *dc,
		 char const *family_name_string,
		 char const *specific_name_string,
		 float Aheight,
		 float vspace);
GLOBAL_FUNCTION void HD_Record_Font_With_Encoding (Display_Context const *dc,
		 char const *family_name_string,
		 char const *specific_name_string,
		 char encoding,
		 float Aheight,
		 float vspace);
GLOBAL_FUNCTION Font_Instance const * HD_Record_Scaleable_Instance (
		Display_Context const *dc,
		Font_Instance const	*specific,
		float Aheight,
		float vspace);
GLOBAL_FUNCTION void HD_Apply_Screen_Transform (Net_Rendition const & nr, int override_projection = -1);
GLOBAL_FUNCTION void HD_Remove_Hidden_Followers (Hidden *thing, Hidden *start);
GLOBAL_FUNCTION void HD_Report_Null_Color_Map (char const * polyhedron, Color_Map const * color_map);
GLOBAL_FUNCTION void HD_Resize_Subscreen (Display_Context const *const_dc,
		 bool actual,
		 int new_left,
		 int new_right,
		 int new_bottom,
		 int new_top);

GLOBAL_FUNCTION void HD_SP_Hide_DC_Colorized_Line (Net_Rendition const & nr,
		 DC_Point const * points,
		 RGBAS32 const * color);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Colorized_Trian (Net_Rendition const & nr,
		 DC_Point const * points,
		 RGBAS32 const * color);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Gouraud_Line (Net_Rendition const & nr,
		 DC_Point const * points,
		 RGBAS32 const * colors);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Gouraud_Trian (Net_Rendition const & nr,
		 DC_Point const * points,
		 RGBAS32 const * colors);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Image (Net_Rendition const & nr,
		 DC_Point const * start,
		 DC_Point const * end,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const * rasters,
		 int pattern,
		 Driver_Color const * contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Line (Net_Rendition const & nr, DC_Point const * points);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Marker (Net_Rendition const & nr, DC_Point const * point);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Reshaded_Line (Net_Rendition const & nr,
		 DC_Point const * points,
		 RGB const * colors,
		 Plane const * planes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Reshaded_Trian (Net_Rendition const & nr,
		 DC_Point const * points,
		 RGB const * colors,
		 Plane const * planes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Text (Net_Rendition const & nr, DC_Point const * where, int count, unsigned short const * kstring);
GLOBAL_FUNCTION void HD_SP_Hide_DC_Triangle (Net_Rendition const & nr, DC_Point const * points);
GLOBAL_FUNCTION void HD_SP_Hide_Plain_Text (Net_Rendition const & nr,
		 Text const * dbtext,
		 Point const * point,
		 int count,
		 unsigned short const * kp,
		 Text_Karacter_Info const * ki);
GLOBAL_FUNCTION void HD_See_Whats_Hidden (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Check_Capping (Net_Rendition const & nr);
GLOBAL_FUNCTION bool HD_Select_Circle_By_Polyline (Net_Rendition const & nr, DC_Point const & center, float radius);
GLOBAL_FUNCTION void HD_Select_Dot (Net_Rendition const & nr,
	Geometry const *	thing,
	Key					item_key,
	Key					original_key,
	Type_Flags			type,
	Point const *		whereabouts);
GLOBAL_FUNCTION void HD_Select_Dot (Net_Rendition const & nr,
	Geometry const *	thing,
	Key					item_key,
	Key					original_key,
	Type_Flags			type,
	DPoint const *		whereabouts);
GLOBAL_FUNCTION void HD_Select_Ellipse (Net_Rendition const & nr, Ellipse const *ellipse, bool face_test);
GLOBAL_FUNCTION void HD_Select_Elliptical_Arc (Net_Rendition const & nr, Elliptical_Arc const *arc);
GLOBAL_FUNCTION void HD_Select_Grid (Net_Rendition const & nr, Grid const *grid);
GLOBAL_FUNCTION void HD_Select_Image (Net_Rendition const & nr, Image const *image);
GLOBAL_FUNCTION void HD_Select_Inner_Window (Subsegment *subsegment, Window_Tree *parent);
GLOBAL_FUNCTION void HD_Select_Marker (Net_Rendition const & nr, Marker const *marker);
GLOBAL_FUNCTION void HD_Select_Multimarker (Net_Rendition const & nr, Multimarker const * mm);
GLOBAL_FUNCTION void HD_Select_NURBS_Curve (Net_Rendition const & nr, NURBS_Curve *curve);
GLOBAL_FUNCTION void HD_Select_Cylinder (Net_Rendition const & nr, Cylinder const *cylinder);
GLOBAL_FUNCTION void HD_Select_PolyCylinder (Net_Rendition const & nr, PolyCylinder const *polycylinder);
GLOBAL_FUNCTION void HD_Select_Sphere (Net_Rendition const & nr, Sphere const *sphere);
GLOBAL_FUNCTION void HD_Select_NURBS_Surface (Net_Rendition const & nr, NURBS_Surface const *surf);
GLOBAL_FUNCTION bool HD_Select_Point_By_Polygon (Net_Rendition const & nr, DC_Point const & center);
GLOBAL_FUNCTION bool HD_Select_Point_List (Net_Rendition const & nr,
		 Geometry const *geometry,
		 int icount,
		 Point const *ipl,
		 bool closed,
		 bool face_test,
		 int *offset1,
		 int *offset2,
		 float *prox,
		 float *zw);
GLOBAL_FUNCTION void HD_Select_Polyglin (Net_Rendition const & nr, Geometry const *geometry, bool face_test);
GLOBAL_FUNCTION void HD_Select_Polyhedron (Net_Rendition const & nr, Polyhedron const *polyhedron, void const * source = null);
GLOBAL_FUNCTION void HD_Select_Text (Net_Rendition const & nr, Text const *text);
GLOBAL_FUNCTION void HD_Selection_Render (Net_Rendition const & nr, Geometry const *geo, bool single);
GLOBAL_FUNCTION void HD_Selection_Walk_Tree (Net_Rendition const & inr, Segment const *seg, Include_Path const & path);
GLOBAL_FUNCTION void HD_Set_DC_Cutting_Planes (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Set_Z_Buffering_Transform (Net_Rendition const & nr,
		 bool z_transforms,
		 bool z_buffering,
		 float z_max);
GLOBAL_FUNCTION bool HD_Setup_Frame_Buffer (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Setup_Screen_Door (Net_Rendition & nr);
GLOBAL_FUNCTION void HD_Span_DC_Colorized_Polyline (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *in_color,
		 bool single);
GLOBAL_FUNCTION void HD_Span_DC_Colorized_Polytris (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *in_color,
		 bool single);
GLOBAL_FUNCTION void HD_Span_DC_Gouraud_Polyline (Net_Rendition const & nr, int count, DC_Point const *points, RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Span_DC_Gouraud_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Span_DC_Polyline (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Span_DC_Rectangle (Net_Rendition const & nr, int left, int right, int bottom, int top);
GLOBAL_FUNCTION void HD_Span_DC_Phong_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes);
GLOBAL_FUNCTION void HD_Span_DC_Textured_Polyline (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Span_DC_Phong_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes);
GLOBAL_FUNCTION void HD_Span_DC_Textured_Polytris (Net_Rendition const & nr, int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Span_DC_Polytriangle (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION RGBA HD_Standard_Compute_Shading (Net_Rendition const & nr,
		 Light_Rendition const & lir,
		 Material_Rendition const & matr,
		 Point const *location,
		 RGB const *color,
		 Plane const *plane,
		 Vector const *aligner = null,
		 float const * params = null,
		 int param_width = 0,
		 int param_flags = 0,
		 RGBA *specular_result = null);
GLOBAL_FUNCTION RGBA HD_Standard_Compute_Shading (Net_Rendition const & nr,
		 Light_Rendition const & lir,
		 Material_Rendition const & matr,
		 DPoint const *location,
		 RGB const *color,
		 DPlane const *plane,
		 Vector const *aligner = null,
		 float const * params = null,
		 int param_width = 0,
		 int param_flags = 0,
		 RGBA *specular_result = null);
GLOBAL_FUNCTION void HD_Activate_Texture_Image(
		Net_Rendition const & nr,
		Texture *txr,
		unsigned int needed_flags,
		int min_size_x, int max_size_x,
		int min_size_y, int max_size_y);
GLOBAL_FUNCTION void HD_Set_Image_Alpha(Image *image, unsigned char alpha);
GLOBAL_FUNCTION RGBA HD_Standard_Compute_Texture (Net_Rendition const & nr, Texture const *texture,
		 Point const *location, Plane const *plane,
		 float const * params, int param_width, int param_flags,
		 int usage);
GLOBAL_FUNCTION RGBA HD_Standard_Compute_Texture (Net_Rendition const & nr, Texture const *texture,
		 DPoint const *location, DPlane const *plane,
		 float const * params, int param_width, int param_flags,
		 int usage);
GLOBAL_FUNCTION bool HD_Standard_Name_Font(Net_Rendition const & nr, Font_Name const *font_name_in, Font_Name *font_name_out);
GLOBAL_FUNCTION void HD_Std_3D_Contour (Net_Rendition const & nr, Contour const *contour);
GLOBAL_FUNCTION void HD_Std_3D_Cylinder (Net_Rendition const & nr, Cylinder const *cylinder);
GLOBAL_FUNCTION void HD_Std_3D_PolyCylinder (Net_Rendition const & nr, PolyCylinder const *polycylinder);
GLOBAL_FUNCTION void HD_Std_3D_Sphere (Net_Rendition const & nr, Sphere const *sphere);
GLOBAL_FUNCTION void HD_Std_3D_Ellipse (Net_Rendition const & nr, Ellipse const *ellipse);
GLOBAL_FUNCTION void HD_Std_3D_Grid (Net_Rendition const & const_nr, Grid const *grid);
GLOBAL_FUNCTION void HD_Std_3D_Image (Net_Rendition const & nr, Image const *image);
GLOBAL_FUNCTION void HD_Std_3D_Light (Net_Rendition const & nr, Light const *light);
GLOBAL_FUNCTION void HD_Std_3D_Marker (Net_Rendition const & nr, Marker const *marker);
GLOBAL_FUNCTION void HD_Std_3D_Multimarker (Net_Rendition const & nr, Multimarker const * multimarker);
GLOBAL_FUNCTION void HD_Std_3D_NURBS_Curve (Net_Rendition const & nr, NURBS_Curve const *curve);
GLOBAL_FUNCTION void HD_Std_3D_Polyedge (Net_Rendition const & nr, Polyedge const *pe);
GLOBAL_FUNCTION void HD_Std_3D_Polyedge_From_Tristrip (Net_Rendition const & nr, Polyedge const * pe, Tristrip const * ts);
GLOBAL_FUNCTION void HD_Std_3D_Polygon (Net_Rendition const & nr, Polygon const *polygon);
GLOBAL_FUNCTION void HD_Std_3D_Polyhedron (Net_Rendition const & nr, Polyhedron const * polyhedron);
GLOBAL_FUNCTION void HD_Std_3D_Polyline (Net_Rendition const & nr, Polyline const *pl);
GLOBAL_FUNCTION void HD_Std_3D_Infinite_Line (Net_Rendition const & nr, Polyline const *pl);
GLOBAL_FUNCTION void HD_Std_3D_Polymarker (Net_Rendition const & inr, Polymarker const *pm);
GLOBAL_FUNCTION void HD_Std_3D_Text (Net_Rendition const & inr, Text const *text);
GLOBAL_FUNCTION void HD_Std_3D_Tristrip (Net_Rendition const & nr, Tristrip const *ts);
GLOBAL_FUNCTION void HD_Std_3D_NURBS_Surface (Net_Rendition const & nr, NURBS_Surface const *surface);
GLOBAL_FUNCTION void HD_Std_DC_Ellipse (Net_Rendition const & nr,
		 DC_Point const *where,
		 DC_Point const *radii);
GLOBAL_FUNCTION void HD_Std_DC_Face (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Std_DC_Image (Net_Rendition const & nr,
		 DC_Point const *start,
		 DC_Point const *end,
		 int hoffset,
		 char format,
		 int row_bytes,
		 void const * const *image_rasters,
		 int pattern,
		 Driver_Color const *contrast_color,
		 Image const *db_image);
GLOBAL_FUNCTION void HD_Std_DC_Polydot (Net_Rendition const & nr, int count, DC_Point const *where);
GLOBAL_FUNCTION void HD_Std_DC_Colorized_Polydot (Net_Rendition const & nr, int count, DC_Point const *where, RGBAS32 const *in_color, bool single);
GLOBAL_FUNCTION void HD_Std_DC_Polymarker (
		 Net_Rendition const & nr,
		 int count,
		 DC_Point const *where,
		 float const *rotations,
		 float const *size_fixups);
GLOBAL_FUNCTION void HD_Std_DC_Polygon (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Std_DC_Polyline (Net_Rendition const & oldnr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Std_DC_Rectangle (Net_Rendition const & nr, int left, int right, int bottom, int top);
GLOBAL_FUNCTION void HD_Std_DC_Text (Net_Rendition const & oldnr, DC_Point const *position, int count, unsigned short const *kstring);
GLOBAL_FUNCTION void HD_Std_DC_Polytriangle (Net_Rendition const & oldnr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Standard_Draw_Inter_Text (Net_Rendition const & nr, Text_Info const *ti);


GLOBAL_FUNCTION void HD_Draw_Quickmoved_Geometry(
	Net_Rendition const &			nr,
	Deferred_Item_Vector const &	defered);

GLOBAL_FUNCTION void HD_Process_Quickmoved_Geometry(
	Display_Context *			dc,
	Deferred_Item_Vector &		deferred);

GLOBAL_FUNCTION void HD_Draw_Deferred_3D_Geometry(
	Deferred_Item_Vector &	deferred,
	bool					disable_z_writes,
	int						layer = -1);


GLOBAL_FUNCTION Frame_Buffer_Info * HD_Save_FBI_Buffers(
	Net_Rendition const & 	nr,
	Segment const *			seg,
	Include_Path const &	path,
	bool					save_color,
	bool					save_z,
	const char *			logging_note);

GLOBAL_FUNCTION bool HD_Undraw_FBI_Buffers(
	Net_Rendition &			nr,
	Frame_Buffer_Info *		fbi,
	const char *			logging_note);

GLOBAL_FUNCTION void HD_Standard_Draw_Plain_Text (
	Net_Rendition const &	nr,
	Text const *			dbtext,
	Point const *			position,
	int						count,
	unsigned short const *	kp,
	Text_Karacter_Info const *	ki);
GLOBAL_FUNCTION void HD_Standard_Draw_Font (Net_Rendition const & nr,
	Font_Instance const *	specific,
	Point const *			where,
	int						count,
	unsigned short const *	string,
	Vector const *			scale);

GLOBAL_FUNCTION void HD_Standard_Draw_Tree (Net_Rendition const & inr, Segment const *seg, Include_Path const & path);
GLOBAL_FUNCTION void HD_Standard_Draw_Window (Net_Rendition const & nr, IntRectangle const *extent);
GLOBAL_FUNCTION void HD_Standard_Draw_Window_Frame (
	Net_Rendition const &	nr,
	IntRectangle const *	extent,
	IntRectangle *			frame,
	bool					calculate_only);
GLOBAL_FUNCTION bool HD_Standard_Driver (Thread_Data * thread_data, Actor *actor, int request, void * request_info);
GLOBAL_FUNCTION Geometry const * HD_Standard_Render (Net_Rendition const & nr, Geometry const *geometry, uint32_t mask, bool single);
GLOBAL_FUNCTION Geometry const * HD_Quickmoves_Render (Net_Rendition const & nr, Geometry const *geometry, uint32_t mask, bool single);
GLOBAL_FUNCTION Geometry const * HD_Process_Geometry_Reference (Net_Rendition const & nr, Geometry_Reference const * gr);
GLOBAL_FUNCTION void HD_Standard_Resize_Outer_Window (Display_Context const *indc);
GLOBAL_FUNCTION bool HD_Standard_Select (Display_Context *dc, Selection_Event *se);

GLOBAL_FUNCTION bool HD_Standard_Update (Display_Context *dc, Activity update_activity);
GLOBAL_FUNCTION void HD_Start_Priority_Inclusion (Net_Rendition const & nr, int32_t priority);
GLOBAL_FUNCTION void HD_Std_Create_Frame_Buffer (Net_Rendition const & nr,
	int					width,
	int					height,
	char				format,
	Image **			image,
	void **				driver_specific);
GLOBAL_FUNCTION void HD_Std_Destroy_Frame_Buffer (Net_Rendition const & nr, Image *image, void *driver_specific);
GLOBAL_FUNCTION void HD_Std_Display_Frame_Buffer (Net_Rendition const & nr,
	DC_Point const *	start,
	DC_Point const *	end,
	Image const *		image,
	int					hoffset,
	int					voffset,
	void const *		driver_specific);
GLOBAL_FUNCTION void HD_Std_3D_Elliptical_Arc (Net_Rendition const & nr, Elliptical_Arc const *arc);
GLOBAL_FUNCTION void HD_Std_3D_Stenciled_Quad (Net_Rendition const & inr,
		 Point const *points,
		 Stencil_Bitmap const *stencil);
GLOBAL_FUNCTION void HD_Std_DC_Colorized_Face (Net_Rendition const & nr,
		 int count,
		 DC_Point const *dcp,
		 RGBAS32 const *color);

GLOBAL_FUNCTION void HD_Std_DC_Colorized_Polyline (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *color,
		 bool single);
GLOBAL_FUNCTION void HD_Std_DC_Colorized_Polymarker (Net_Rendition const & nr,
		 int count,
		 DC_Point const *where,
		 RGBAS32 const *color,
		 bool single, float const * rotations,
		 float const *size_fixups);
GLOBAL_FUNCTION void HD_Std_DC_Colorized_Polytris (Net_Rendition const & oldnr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *color,
		 bool single);
GLOBAL_FUNCTION void HD_Std_DC_Elliptical_Arc (Net_Rendition const & nr,
		 DC_Point const *center,
		 DC_Point const *radius,
		 float start,
		 float end);
GLOBAL_FUNCTION void HD_Std_DC_Gouraud_Polyline (Net_Rendition const & oldnr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Std_DC_Gouraud_Polytris (Net_Rendition const & oldnr,
		 int count,
		 DC_Point const *points,
		 RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Std_DC_Outline_Ellipse (Net_Rendition const & nr,
		 DC_Point const *where,
		 DC_Point const *radii);
GLOBAL_FUNCTION void HD_Std_DC_Phong_Polyline (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes);
GLOBAL_FUNCTION void HD_Std_DC_Phong_Polytris (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes);
GLOBAL_FUNCTION void HD_Std_DC_Textured_Polyline (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Std_DC_Textured_Polytris (Net_Rendition const & nr,
		 int count,
		 DC_Point const *points,
		 RGBA const *colors,
		 Plane const *planes,
		 DPlane const *dplanes,
		 Parameter const *vertex_params,
		 int param_width,
		 Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Std_DC_Stenciled_Quad (
	Net_Rendition const &	nr,
	DC_Point const *		points,
	Stencil_Bitmap const *	stencil);
GLOBAL_FUNCTION void HD_Std_Retitle_Outer_Window (Display_Context const *indc);

GLOBAL_FUNCTION int HD_Transform_And_Test_Simple_Sphere (
	Net_Rendition const &	nr,
	int						count,
	SimpleSphere const *	spheres,
	short					culling,
	Rectangle const &		outer,
	IntRectangle const &	inner,
	Test *					results,
	float					extent_limit = 0,
	float *					actual_extents = null,
	float *					max_culled_extent = null,
	float					hard_extent_limit = 0);
GLOBAL_FUNCTION int HD_Transform_And_Test_Simple_Sphere (
	Net_Rendition const &	nr,
	int						count,
	DSimpleSphere const *	spheres,
	short					culling,
	Rectangle const &		outer,
	IntRectangle const &	inner,
	Test *					results,
	float					extent_limit = 0,
	float *					actual_extents = null,
	float *					max_culled_extent = null,
	float					hard_extent_limit = 0);

GLOBAL_FUNCTION int HD_Transform_And_Test_Simple_Cuboid (
	Net_Rendition const &	nr,
	SimpleCuboid const & 	cuboid,
	short					culling,
	Rectangle const &		outer,
	IntRectangle const &	inner,
	Test &					results,
	float					extent_limit = 0,
	float *					actual_extents = null);

GLOBAL_FUNCTION Test HD_Transform_And_Test_Bounding (
	Net_Rendition const &	nr,
	Bounding const &		bounding,
	short					culling,
	Rectangle const &		outer,
	IntRectangle const &	inner,
	float					extent_limit = 0,
	float *					actual_extent = null);

GLOBAL_FUNCTION int HD_Transform_And_Test_Simple_Cuboid (
	Net_Rendition const &	nr,
	SimpleCuboid const & 	cuboid,
	Rectangle const &		outer,
	IntRectangle const &	inner,
	Test &					results,
	float					extent_limit = 0,
	float *					actual_extents = null);

GLOBAL_FUNCTION Test HD_Transform_And_Test_Bounding (
	Net_Rendition const &	nr,
	Bounding const &		bounding,
	Rectangle const &		outer,
	IntRectangle const &	inner,
	float					extent_limit = 0,
	float *					actual_extent = null);

GLOBAL_FUNCTION int HD_Transform_Not_At_All (
	Transform_Rendition const &	tr,
	int							point_count,
	Point const *				in,
	Rectangle const *			clip,
	DC_Point *					out,
	float *						ws,
	unsigned char *				clip_case);
GLOBAL_FUNCTION int HD_Transform_And_Test_Points (
	Transform_Rendition const &	tr,
	int							point_count,
	Point const *				in,
	Rectangle const *			clip,
	DC_Point *					out,
	float *						ws,
	unsigned char *				clip_case,
	bool						transform=true);
GLOBAL_FUNCTION int HD_Transform_And_Test_Points (
	Transform_Rendition const &	tr,
	int							point_count,
	DPoint const *				in,
	Rectangle const *			clip,
	DC_Point *					out,
	float *						ws,
	unsigned char *				clip_case,
	bool						transform=true);
GLOBAL_FUNCTION void HD_Standard_Unload_All_Fonts (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Unload_All_Fonts (Display_Context *dc);
GLOBAL_FUNCTION void HD_Unload_Defined_Font (Display_Context const *dc, Font_Instance *instance);
GLOBAL_FUNCTION void HD_Unload_Freetype_Font (Display_Context const *dc, Font_Instance *instance);
GLOBAL_FUNCTION void HD_Upwind_Driver_Options (Display_Context *dc);
GLOBAL_FUNCTION void HD_Validate_Clip_Half_Planes (Convex_Clip_Region *cr, bool normals_point_in = false);
GLOBAL_FUNCTION void HD_Validate_Clipping_Rendition (Transform_Rendition & tr, Clipping_Rendition & cr, Clip_DC_Flags & flags);
GLOBAL_FUNCTION void HD_Validate_Clipping_Rendition (Transform_Rendition & tr, Clipping_Rendition & cr);
GLOBAL_FUNCTION void HD_Validate_Object_To_Proj (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Validate_Object_To_View (Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_Validate_World_To_Screen (Net_Rendition const & nr);

GLOBAL_FUNCTION float HD_Compute_Generic_Size (
		Net_Rendition const &	nr,
		double					value,
		Generic_Size_Units		units,
		float					nominal_size,
		bool					full_transforms,
		bool *					success,
		bool					errors);
GLOBAL_FUNCTION float HD_Compute_Line_Style_Max_Displacement (
		Net_Rendition const &	nr,
		Line_Rendition const &	lr);

GLOBAL_FUNCTION void HD_HL_3D_Polyhedron (
		Net_Rendition const & nr,
		Polyhedron const * phedron);

GLOBAL_FUNCTION void  HD_Fixup_Attributes (
		Net_Rendition & nr,
		Attribute_Lock const * lock,
		int flags);

GLOBAL_FUNCTION void HD_Generate_Cut_Geometry (
		Net_Rendition const & 	   nr,
		Segment const *seg,
		Geometry const *geometry);
GLOBAL_FUNCTION void HD_Process_Cut_Geometry (
		Net_Rendition const & 	   nr,
		Segment const *			seg,
		Include_Path const & path,
		bool generate,
		bool clean);
GLOBAL_FUNCTION Cut_Geometry_Pieces * HD_Prepare_Cut_Geometry_Pieces (
	Net_Rendition const &			nr,
	Cutting_Plane_Instance const *	cpi);
GLOBAL_FUNCTION void HD_Delete_Cut_Geometry_Pieces (
		Display_Context *		dc);
GLOBAL_FUNCTION void HD_Shadow_Update (
		Net_Rendition const & nr,
		Segment const * seg,
		Include_Path const & path,
		Simple_Shadow * shadow);
GLOBAL_FUNCTION void HD_Shadow_Map_Update (
		Net_Rendition const & nr,
		Segment const * seg,
		Include_Path const & path);

GLOBAL_FUNCTION void HD_SZB_DC_Polytriangle (
	Net_Rendition const & 	nr,
	int						count,
	DC_Point const *		points);
GLOBAL_FUNCTION void HD_SZB_DC_Colorized_Polytris (
	Net_Rendition const & 	nr,
	int						count,
	DC_Point const *		points,
	RGBAS32 const *			color,
	bool					single);
GLOBAL_FUNCTION void HD_SZB_DC_Gouraud_Polytris (
	Net_Rendition const & 	nr,
	int						count,
	DC_Point const *		points,
	RGBAS32 const *			colors);
GLOBAL_FUNCTION void HD_SZB_DC_Phong_Polytris (
	Net_Rendition const &	nr,
	int						count,
	DC_Point const *		points,
	RGBA const *			colors,
	Plane const *			planes,
	DPlane const *			dplanes);
GLOBAL_FUNCTION void HD_SZB_DC_Textured_Polytris (
	Net_Rendition const &	nr,
	int						count,
	DC_Point const *		points,
	RGBA const *			colors,
	Plane const *			planes,
	DPlane const *			dplanes,
	float const *			params,
	int32_t					param_width,
	int32_t					param_flags);

GLOBAL_FUNCTION void HD_SZB_DC_Polyline (
	Net_Rendition const & 	nr,
	int						count,
	DC_Point const *		points);
GLOBAL_FUNCTION void HD_SZB_DC_Colorized_Polyline (
	Net_Rendition const & 	nr,
	int						count,
	DC_Point const *		points,
	RGBAS32 const *			color,
	bool					single);
GLOBAL_FUNCTION void HD_SZB_DC_Gouraud_Polyline (
	Net_Rendition const & 	nr,
	int						count,
	DC_Point const *		points,
	RGBAS32 const *			colors);

GLOBAL_FUNCTION void HD_SZB_DC_Rectangle (
	Net_Rendition const & 		nr,
	int left, int right, int bottom, int top);

GLOBAL_FUNCTION void HD_SZB_Process_Triangle (
	Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_SZB_Process_Line (
	Net_Rendition const & nr);
GLOBAL_FUNCTION void HD_SZB_Process_Rectangle (
	Net_Rendition const & nr);


GLOBAL_FUNCTION void  HD_Find_Local_Cutting_Planes (
	Net_Rendition & nr, Geometry const * geometry);

GLOBAL_FUNCTION void HD_Select_Sphere_Polyhedron (
	Net_Rendition const & nr,  Sphere const *sphere);

GLOBAL_FUNCTION void HD_Vanillify_Actions (
	Display_Context  const *	dc,
	Drawing_Mode				pending,
	Action_Table &				actions);

GLOBAL_FUNCTION void HD_Free_HL_Data (HL_Data *data);

GLOBAL_FUNCTION void HD_Unwrap_Polytriangle (Net_Rendition const & oldnr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Unwrap_Colorized_Polytriangle (Net_Rendition const & oldnr, int count, DC_Point const *points, RGBAS32 const *color, bool single);
GLOBAL_FUNCTION void HD_Unwrap_Gouraud_Polytriangle (Net_Rendition const & oldnr, int count, DC_Point const *points, RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Unwrap_Phong_Polytriangle (Net_Rendition const & oldnr, int count, DC_Point const *points,
													  RGBA const * colors, Plane const * planes, DPlane const * dplanes);
GLOBAL_FUNCTION void HD_Unwrap_Reshaded_Polytriangle (Net_Rendition const & oldnr, int count, DC_Point const *points,
		 RGBA const * colors, Plane const * planes, DPlane const * dplanes,
		 Parameter const *vertex_params, int param_width, Parameter_Flags param_flags);
GLOBAL_FUNCTION void HD_Unwrap_Polyline (Net_Rendition const & oldnr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Unwrap_Colorized_Polyline (Net_Rendition const & oldnr, int count, DC_Point const *points, RGBAS32 const *color, bool single);
GLOBAL_FUNCTION void HD_Unwrap_Gouraud_Polyline (Net_Rendition const & oldnr, int count, DC_Point const *points, RGBAS32 const *colors);
GLOBAL_FUNCTION void HD_Unwrap_Phong_Polyline (Net_Rendition const & oldnr, int count, DC_Point const *points,
												  RGBA const * colors, Plane const * planes, DPlane const * dplanes);
GLOBAL_FUNCTION void HD_Unwrap_Reshaded_Polyline (Net_Rendition const & oldnr, int count, DC_Point const *points,
		 RGBA const * colors, Plane const * planes, DPlane const * dplanes,
		 Parameter const *vertex_params, int param_width, Parameter_Flags param_flags);

GLOBAL_FUNCTION void HD_Unwrap_HLR_Polymarker (Net_Rendition const & oldnr, int count, DC_Point const *points, float const * rotations, float const *size_fixups);
GLOBAL_FUNCTION void HD_Unwrap_HLR_Hidden_Polymarker (Net_Rendition const & oldnr, int count, DC_Point const *points, float const * rotations, float const *size_fixups);
GLOBAL_FUNCTION void HD_Unwrap_Polymarker (Net_Rendition const & oldnr, int count, DC_Point const *points, float const * rotations, float const *size_fixups);

GLOBAL_FUNCTION void HD_Unwrap_Colorized_Polymarker (Net_Rendition const & oldnr, int count, DC_Point const *points, RGBAS32 const *color, bool single, float const * rotations, float const *size_fixups);
GLOBAL_FUNCTION void HD_Compute_Shadow_Resolution (Net_Rendition const & inr, Segment const * seg, Simple_Shadow * shadow);
GLOBAL_FUNCTION void HD_Reposition_Shadow (Net_Rendition const & inr, Segment const * seg, Include_Path const & path, Simple_Shadow * shadow);
GLOBAL_FUNCTION void HD_Flush_Hidden_Item_Cache (Display_Context *dc);
GLOBAL_FUNCTION void HD_Free_Hidden_Item_Cache (Display_Context *dc);
GLOBAL_FUNCTION bool HD_Clean_Texture_Image_Cache (Net_Rendition const & inr, Texture const * texture, bool retain_active_image);
GLOBAL_FUNCTION void HD_Hide_3D_Tristrip(
		Net_Rendition const & nr,
		Net_Rendition const & back_nr,
		Tristrip const *ts,
		int flags,
		DC_Point* dcp);
GLOBAL_FUNCTION void HD_Hide_3D_Polymarker (
	Net_Rendition const & 		inr,
	Polymarker const *			pm,
	int							flags,
	DC_Point *					dcp) ;

GLOBAL_FUNCTION void HD_Cache_From_Hidden_Tristrip (Hidden_Tristrip *hidden_tristrip, float next_z);
GLOBAL_FUNCTION void HD_Free_Hidden_Tristrip (Hidden_Tristrip *hidden_tristrip);
GLOBAL_FUNCTION void HD_Free_Hidden_Polymarker (Hidden_Polymarker * hidden_polymarker);

GLOBAL_FUNCTION Cutting_Plane_Set * HD_Copy_Cutting_Plane_Sets (Cutting_Plane_Set const * oldcps, Memory_Pool * pool);
GLOBAL_FUNCTION void HD_Free_Cutting_Plane_Sets (Cutting_Plane_Set const *cps);
GLOBAL_FUNCTION void HD_Validate_Cutting_Plane_Sets (Cutting_Plane_Rendition const & cpr);
GLOBAL_FUNCTION void HD_Validate_Transparent_Cutting_Sets (Cutting_Plane_Rendition const & cpr);
GLOBAL_FUNCTION void* HD_Find_X11_Display(char const * const name);
GLOBAL_FUNCTION void HD_Free_X11_Display(void const * display);
GLOBAL_FUNCTION bool HD_Regex_Check_Display (char const * name);
GLOBAL_FUNCTION void HD_Generate_Elliptical_Bezier (
		Net_Rendition const & nr,
		Point const *center,
		Vector const *basis1,
		Vector const *basis2,
		float start,
		float end,
		int *count,
		Point *buffer);
GLOBAL_FUNCTION void HD_Generate_DC_Elliptical_Bezier (
		Net_Rendition const	& nr,
		Generic_Arc const *curve,
		int *count,
		DC_Point *buffer);

GLOBAL_FUNCTION void HD_Std_Depth_Peeling (Net_Rendition const & nr);

GLOBAL_FUNCTION void HD_Isolate_Tristrip_Data (Display_Context const * dc, Tristrip const * ts);
GLOBAL_FUNCTION void HD_Isolate_Polyedge_Data (Display_Context const * dc, Polyedge const * pe);
GLOBAL_FUNCTION void HD_Isolate_Polymarker_Data (Display_Context const * dc, Polymarker const * pm);

GLOBAL_FUNCTION void HD_Select_DC_Polyline (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Select_DC_Polygon (Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION void HD_Select_DC_Polytriangle (Net_Rendition const & nr, int count, DC_Point const *points);


GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Polyline (Net_Rendition const & nr, Polyline const *polyline);
GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Polymarker (Net_Rendition const & nr, Polymarker const *pm);
GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Marker (Net_Rendition const & nr, Marker const *marker);
GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Tristrip (Net_Rendition const & nr, Tristrip const *tristrip);
GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Ellipse (Net_Rendition const & nr, Ellipse const *ellipse);
GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Polygon (Net_Rendition const & nr, Polygon const *polygon);
GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Polyedge (Net_Rendition const & nr, Polyedge const *polyedge);

GLOBAL_FUNCTION void HD_Defer_Antialiased_3D_Polyline (Net_Rendition const & nr, Polyline const *polyline);
GLOBAL_FUNCTION void HD_Defer_Antialiased_3D_Polygon (Net_Rendition const & nr, Polygon const *polygon);
GLOBAL_FUNCTION void HD_Defer_Antialiased_3D_Polyedge (Net_Rendition const & nr, Polyedge const *polyedge);

GLOBAL_FUNCTION void HD_Maybe_Defer_Trans_3D_Polyedge (Net_Rendition const & nr, Polyedge const *polyedge);
GLOBAL_FUNCTION void HD_Maybe_Defer_Trans_3D_Polymarker(Net_Rendition const & nr, Polymarker const *pm);


GLOBAL_FUNCTION unsigned short HD_Map_Freetype_Char_Index(
	Font_Instance const	*specific,
	unsigned short			kar);

GLOBAL_FUNCTION Stencil_Bitmap * HD_Make_Freetype_Stencil(
	Net_Rendition const & nr,
	Font_Instance const	*specific,
	unsigned short			mapped_index,
	int						bitmap_height,
	int						bolding_level,
	float					xrslant,
	float					xrrotation,
	float					xrwidth_scale,
	bool					antialias);


GLOBAL_FUNCTION unsigned short HD_Standard_Map_Char_Index(
	Font_Instance const	*specific,
	unsigned short	kar);

GLOBAL_FUNCTION void HD_Draw_Stenciled_Font (
	Net_Rendition const & nr,
	Font_Instance const	*specific,
	Point const *		where,
	int						count,
	unsigned short const *	string,
	Vector const *		scale);

GLOBAL_FUNCTION void HD_Draw_Dead_Font (
	Net_Rendition const & nr,
	Font_Instance const	*specific,
	Point const *		where,
	int						count,
	unsigned short const *	string,
	Vector const *		scale);

GLOBAL_FUNCTION void HD_Standard_Size_Font(Net_Rendition const & nr, Font_Instance *specific);
GLOBAL_FUNCTION bool HD_Standard_Will_Use_Stencil(Net_Rendition const & nr, Font_Instance const *specific);
GLOBAL_FUNCTION bool HD_Freetype_Will_Use_Stencil(Net_Rendition const & nr, Font_Instance const *specific);

GLOBAL_FUNCTION void HD_Visit_Inner_Activity(Net_Rendition const & nr, Segment const * seg, Include_Path const & path, Activity forced_activity);
GLOBAL_FUNCTION void HD_Reset_Tree_State (Net_Rendition & nr);

GLOBAL_FUNCTION void HD_Handle_FBI_Pending_Window (Net_Rendition const & nr);

GLOBAL_FUNCTION void HD_Adjust_WLimit (Net_Rendition & nr, Segment const * seg, Include_Path const & path);

GLOBAL_FUNCTION int * HD_Get_Screen_Facings (Net_Rendition const & nr, Tristrip const *ts, int face_count);

GLOBAL_FUNCTION bool HD_Prepare_Geometry_Transparency (Net_Rendition & nr, char const * ident);

GLOBAL_FUNCTION unsigned int HD_Geometry_To_Action_Mask (Geometry const *g);

/* for IM cleanup */
GLOBAL_FUNCTION void HD_Free_Convex_Clip_Region (Convex_Clip_Region const *victim);
GLOBAL_FUNCTION void HD_Free_Complex_Clip_Region (Complex_Clip_Region const *victim);
GLOBAL_FUNCTION Convex_Clip_Region * HD_New_Convex_Clip_Region(Net_Rendition const & nr, int count, DC_Point const *points);
GLOBAL_FUNCTION Complex_Clip_Region * HD_New_Complex_Clip_Region(Net_Rendition const & nr, int loops, int const * lengths, int count, DC_Point const *points);
GLOBAL_FUNCTION Complex_Clip_Region * HD_New_Complex_Clip_Region(Net_Rendition const & nr, int count, DC_Point const *points);

GLOBAL_FUNCTION void HD_Add_Cutting_Plane (Net_Rendition & nr, Plane const *plane,
										   int id = -1, Cutting_Plane const * cp = 0, int offset = 0);
GLOBAL_FUNCTION void HD_Remove_Cutting_Planes(Net_Rendition & nr);

GLOBAL_FUNCTION void HD_Draw_Window(Net_Rendition const & nr, IntRectangle const *extent);

GLOBAL_FUNCTION void HD_Free_User_Face_Pattern(User_Face_Pattern const *up);

GLOBAL_FUNCTION float HD_Compute_Character_Width(Net_Rendition const & nr, Text_Info *ti, size_t index);

GLOBAL_FUNCTION Point const * HD_Compute_Character_Position(
	Net_Rendition const & nr, Text_Info *ti, size_t index);

GLOBAL_FUNCTION Point const * HD_Compute_Transformed_Text_Position(
	Net_Rendition const & nr, Text_Info *ti, Point const * point);

GLOBAL_FUNCTION void HD_Markers_To_Polymarker (Net_Rendition const & inr,  int count, Point * points, DPoint * dpoints);


GLOBAL_FUNCTION Driver_Action HD_Linux_Load_Dynamic_Driver(char const *driver_name);
GLOBAL_FUNCTION Driver_Action HD_OSX_Load_Dynamic_Driver (char const *driver_name);
GLOBAL_FUNCTION Driver_Action HD_Win32_Load_Dynamic_Driver (char const *driver_name);

GLOBAL_FUNCTION void HD_Purge_Display_Lists(Actor const * actor, Base_Driver_Display_List_Callback purge_check, void *extra);
GLOBAL_FUNCTION void HD_Queue_Destroy_List (Driver_DL *bddl, bool need_locking);

GLOBAL_FUNCTION float HD_Compute_Displacement_Scale (Net_Rendition const &nr, float db_displacement);


GLOBAL_FUNCTION Net_Rendition HD_New_Local_Edge_Rendition (
	Polyhedron	 *			polyhedron,
	Net_Rendition const & 			const_nr,
	Net_Rendition const & 			original_nr,
	One_Edge_Attributes const *		attr);
GLOBAL_FUNCTION  void HD_Replace_Line_Style(Line_Rendition & lr, Line_Style const & new_line_style, bool flush_glyphs = true);

GLOBAL_FUNCTION void HD_Setup_Line_Marker_Xform (
	Net_Rendition const &		inr,
	Polyline const *			pl,
	DC_Point *			dcp,
	Marker const *				mk);
GLOBAL_FUNCTION void HD_Takedown_Line_Marker_Xform (Net_Rendition const & inr);

GLOBAL_FUNCTION void HD_Update_Log(Display_Context const * dc, const char * message, const char* file, intptr_t val);

GLOBAL_FUNCTION void HD_Tint_Driver_Color (Net_Rendition const & nr, Driver_Color & dcolor);

// GLOBAL_FUNCTION bool HD_Sample_Selection (
// 	Display_Context *		dc,
// 	Selection_Event *		se);
// GLOBAL_FUNCTION bool HD_Highlight_Buffered_Selection (
// 	Display_Context *		dc,
// 	Selection_Event *		se);

GLOBAL_FUNCTION void HD_Defer_Peeled_Trans_3D_Text (
	Net_Rendition const &	nr,
	Text const *			text);

GLOBAL_FUNCTION void HD_Determine_Locks (
	Attribute_Lock &		locks,
	Attribute const *		attribute);

GLOBAL_FUNCTION void HD_New_Window (
	Net_Rendition &			nr,
	Segment const *			seg,
	Include_Path const &	path,
	Deferred_Draw_Tree *	ddt);

GLOBAL_FUNCTION void HD_Randomize_Index_Array( 
	Display_Context const *	dc, 
	int *					randomized_indices, 
	int						count);

GLOBAL_FUNCTION void HD_Randomize_Index_Array( 
	Display_Context const *	dc, 
	unsigned short *		randomized_indices, 
	unsigned short			count);

GLOBAL_FUNCTION void HD_Apply_Vertex_Displacement(Net_Rendition & nr);

enum HH_Tree_Only {
	HH_Tree_Only_Do_Not_Care=0,
	HH_Tree_Only_True,
	HH_Tree_Only_False,
};

GLOBAL_FUNCTION bool HD_Handle_Highlight (
	Net_Rendition &				nr,
	Segment const *				seg,
	Include_Path const &		path,
	bool						store,
	HH_Tree_Only				tree_only);

GLOBAL_FUNCTION bool HD_Handle_Complex_Highlight(
	Net_Rendition &				nr,
	Segment const *				seg,
	Include_Path const &		in_path,
	Geometry_Reference const *	gr,
	Include const *				inc);

GLOBAL_FUNCTION void HD_Apply_Highlight (
	Net_Rendition &		nr,
	Highlight const &	highlight,
	Seen_Flags			maybes = S_ALL_BITS);


GLOBAL_FUNCTION void HD_Generate_Quickmoved_Activity(Thread_Data *	thread_data);

GLOBAL_FUNCTION void HD_Condition_Value (
	int							code,
	void const *				passthru,
	float &						value,
	bool &						flag);

GLOBAL_FUNCTION void HD_Store_HWVisSel_Item(Net_Rendition const & nr, Geometry const * geometry, DirectRGB * out_geomid, bool needs_inutile = false);


#endif /* HD_PROTO_DEFINED */


