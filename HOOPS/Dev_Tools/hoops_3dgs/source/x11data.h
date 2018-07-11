// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef X11DATA_DEFINED

/*		Moved multibuffering headers from x11_driver.c to x11data.h.
 *
 *		Multibuffering is causing the X11 server to crash if the
 *		HOOPS program is interrupted or aborted on some systems.
 *		The problem has been reproduced in an stand alone pure X
 *		program and has been reported to appropriate vendors.
 *		Until we receive a patch for the server crash we will use
 *		Pixmap double buffering (where appropriate).
 *		To enable Pixmap double buffering just define
 *						  #		  define				DUMMY_MBUF
 *		for the system below (if not already present).
 */

#define Window X_Window
#define Font X_Font
#define Drawable X_Drawable
#define Depth X_Depth
#define Time X_Time

#ifdef NUTCRACKER
# define XK_MISCELLANY
# define XK_LATIN1
# include <keysymdef.h>
#endif

#ifdef SPACEBALL
#include <X11/extensions/XInput.h>
#endif

#ifdef FAMOTIK
#		include <X11/XJplib.h>
#endif /* FAMOTIK */

#ifdef LINUX_SYSTEM
#		define	X11R5
#		define NeedFunctionPrototypes 1
#		include <X11/Xlib.h>
#		include <X11/X.h>
#		include <X11/keysym.h>
#		include <X11/Xutil.h>
#		include <X11/extensions/multibuf.h>
#endif /* LINUX_SYSTEM */

#ifdef OSX_SYSTEM
#		define	X11R5
#		define NeedFunctionPrototypes 1
#		include <X11/Xlib.h>
#		include <X11/X.h>
#		include <X11/keysym.h>
#		include <X11/Xutil.h>
#		include <X11/extensions/multibuf.h>
#endif /* LINUX_SYSTEM */

#ifdef NUTCRACKER
# include <Xlib.h>
# include <driver.h>
# include <Xutil.h>
#ifdef EXCEED_BUILD
# include <multibuf.h>
#else
#define Multibuffer int
#endif
#endif /* NUTCRACKER */


/*
 *		Defines used to tell us which X11 Server we are using.
 */
#define NOT_X11			0
#define UNKNOWN_X11		1

/*
 *		Defines used to tell us which Hoops Driver we are using.
 */
#define X11_ONLY		0
#define OPENGL_ON_X11	1


#ifdef FAMOTIK
typedef struct xftab {
	struct xftab *	next;
	long			id;
	char *			name;
} Xftab;
#endif

#define RENDERER_STRING_LENGTH 200

typedef struct {
	/* Hoops Driver in use */
	int					driver_type;

	/* X11 Server communication */
	Display *			display;
	Screen *			screen;
	Visual *			visual;
	/* X11 Server vendor */
	int					server_type;

	/* needed to find out hardware capability */
	char				renderer[RENDERER_STRING_LENGTH];

	bool				can_overlay;
	Window				overlay_window;


	/* back link for error handling */
	Display_Context *	dc;

	/* windows and sizes */
	Window				window;
	Window				parent_window;
	Window				root_window;
	char				title[4096];
	int					window_x;
	int					window_y;
	int					window_border_bias_x;
	int					window_border_bias_y;
	unsigned int		window_width;
	unsigned int		window_height;

	/* graphics contexts and incarnations */
	GC					line_gc;
	GC					face_gc;
	GC					text_gc;
	GC					image_gc;
	GC					area_gc;
	GC					bitmap_gc;
	GC					blit_gc;
	GC					colorized_face_gc;

	Incarnation			last_line_rendition;
	Incarnation			last_face_rendition;
	Incarnation			last_text_rendition;
	Incarnation			last_blit_rendition;

	Incarnation			last_line_xform_rendition;
	Incarnation			last_face_xform_rendition;
	Incarnation			last_text_xform_rendition;
	Incarnation			last_image_xform_rendition;
	Incarnation			last_blit_xform_rendition;
	Incarnation			last_colorized_face_xform_rendition;

	/* physical function pointers */
	void (*get_x11_physical_info)(Display_Context *dc);
	void (*create_x11_window)(Display_Context const *dc);
	void (*destroy_x11_window)(Display_Context const *dc);
	void (*hide_x11_window)(Display_Context const *dc);

	/* font function pointers */
	void (*record_x11_fonts)(Display_Context *dc);
	void (*record_basic_fonts)(Display_Context *dc);

	bool (*record_one_font)(Display_Context *dc,char const *name);
	char x11_font_resolution[5];

	/* new rendition function pointers, set based on visual type */
	bool (*new_face_rendition)(Net_Rendition const & nr, Driver_Color const *face_color);
	bool (*colorized_face_rendition)(Net_Rendition const & nr, Driver_Color const *face_color);

	/* We PROTO the next routine because Ultrix cannot handle the typedef */
	void (*set_color)(Net_Rendition const & nr, XGCValues *xgcv, Driver_Color const *fg, Driver_Color const *bg);

	/* double-buffering variables */
	Drawable			drawable;
	bool				can_multi_buffer;
	bool				multi_buffering;
	bool				generic;
	bool				pixmap_double_buffering;

	Multibuffer			multi_buffers[2];
	Pixmap				double_buffer;
	int					mbuf_num;
	int					db_left_clip;
	int					db_right_clip;
	int					db_bottom_clip;
	int					db_top_clip;
	long				poly_count;
	XPoint *			poly_points;

	/* color allocation variables */
	unsigned long *		colormap_map;
	Colormap			cmap;
	Colormap			default_cmap;

	/* # of planes (per color, if direct) */
	int					bits_per_color;
	bool				mapped_colors;
	int					num_mapped_colors;

	/* # bitplanes of this window */
	int					depth;

		/* stipples */
	Pixmap				stipples[TOTAL_BIT_PATTERNS];

	/* transformed text */
	Pixmap				bitmap;
	int					bitmap_size;
	char *				xformed;

	/* image array data */
#define BIT_RASTER_SWAPPED		0x01
#define BIT_RASTER_COMPLEMENT	0x02
	int					bit_raster_type;

#define RGBA_RASTER_RGBA		0
#define RGBA_RASTER_BGRA		1
#define RGBA_RASTER_ABGR		2
#define RGBA_RASTER_GRAB		3
#define RGBA_RASTER_ARGB		4
#define RGBA_RASTER_GBAR		5
	int					rgb32_raster_type;
	/* n x 1 image */
	XImage *			raster_image;
										/* n x m image */
	XImage *			block_image;
	RGBAS32 *			block;
	int					block_size;
										/* for DirectColor rgb's */
	bool				blue_first;
	unsigned char		mapped8_raster[NOMINAL_RASTER_LENGTH];
	unsigned char *		mapped8_block;
	int					mapped8_block_size;
	unsigned short		mapped16_raster[NOMINAL_RASTER_LENGTH];
	unsigned short *	mapped16_block;
	int					mapped16_block_size;

#define MAX_HEIGHT_CACHE 2
#define MAX_WIDTH_CACHE 512
	XImage *			block_image_cache[MAX_HEIGHT_CACHE][MAX_WIDTH_CACHE];

	/* mouse handling */
	int					yfudge;
	int					mouse_x;
	int					mouse_y;
	int					mouse_buttons;
	bool				mouse_was_queued;
	int					mouse_modifier;

	/* resizing hints */
	bool				stretching_was_on;
	bool				resizing_was_on;

	/* allow init_update to resize things */
	bool				first_update;

	/* flag for resize event confusion */
	bool				resize_in_progress;

	/* flag to indicate when free_outer_window() was called for a window */
	bool				marked_for_delete;

		/* flag to trim debug output */
	bool				check_since_finish;

		/* data for us to use to figure out compose sequences. */
#define NO_KEY			(0)
#define COMPOSING_KEY	(-1)
	int					first_key;
	bool				num_lock;

#ifdef FAMOTIK
	Xftab *			start;
	bool			henkan;
	unsigned long	j_id;
	unsigned long	values;
#endif /* FAMOTIK */

	/*OPENGL STUFF*/
	/* use generic holder for context (avoids many includes) */
	void *			context;

	/* needed for glXCreateContext */
	XVisualInfo *	visualinfo;

	/* on SoftOpenGL, we need this to set this to DOUBLEBUFFER */
	bool			first;

	/* OpenGL driver will figure out the list but X11 driver*/
	/* will use it to find the best visual.*/
	int *			attribute_list;

	/*this is here to move glx calls out of the x11 driver*/
	void (*opengl_get_best_visual)(Display_Context const *dc);

	/* avoid recursing through selections/check_events */
	bool			checking_events;
	/*END OPENGL STUFF*/

	/* info needed to convert between our color and TrueColor visual color */
	unsigned int	red_left_shift;
	int				red_right_shift;
	unsigned int	green_left_shift;
	int				green_right_shift;
	unsigned int	blue_left_shift;
	int				blue_right_shift;
} X11Data;


/*
 *		This structure stands between core Hoops and X11 fonts, so that
 *		the composite drivers on top of X11 can store more info about fonts.
 */
typedef struct	composite_font_struct {
	XFontStruct *	xfont;						/* pointer to X11 font	*/

	/* Font character bounding box parameters */
	int				lbearing;				/* left-most extent from origin */
	int				rbearing;				/* right-most extent from origin */
	int				ascent;				/* highest extent above origin */
	int				descent;				/* lowest extent below origin */

	void *			composite;				/* if NOT null, the font is not an X11 font */
	/* the data is driver dependent */
	int *			xgl_widths;
} Composite_Font;

/*
 * to keep track of events on a per-window basis
 */

struct X11_Event_Info {
	X11_Event_Info *	next;
	Display_Context *	dc;
	X11Data *			x11data;
	bool				move_pending;
	bool				move_back_pending;
	bool				resize_pending;
	bool				expose_pending;
	IntRectangle		expose;
	int					resize_x;
	int					resize_y;
	int					resize_width;
	int					resize_height;
};


#undef Window 
#undef Font
#undef Drawable
#undef Depth
#undef Time 



#define X11DATA_DEFINED

#endif /* ifndef X11DATA_DEFINED */
