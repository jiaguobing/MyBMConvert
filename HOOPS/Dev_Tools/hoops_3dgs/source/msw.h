// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef MSW_H_DEFINED
#include "driver.h"


struct HOGLData;
typedef VList<HOGLData *> HOGLData_List;

Begin_HOOPS_Namespace

struct W32_Font_Stenciled {
	float yscale;
	int Aheight;
	int real_ysize;
	int descent;
	int max_char_width;

	LOGFONT log_font;
	HFONT hFont;

	long original_lfWidth;

	float ratio;
	long height_fudge;

	int	alt_real_ysize;
	float alt_rotation;
	float alt_width;
	float alt_slant;
	LOGFONT alt_log_font; 
	HFONT alt_hFont;
	int alt_descent;
	int alt_max_char_width;
};


struct Windows_World {
	HINSTANCE	caller_instance;
	HINSTANCE	resource_instance;
};

GLOBAL_DATA Windows_World _windows_world;
#	define MY(a) (_windows_world.a)


struct Pen_User {
	Pen_User *	next_user;
	Display_Context	*dc;
};


struct Pen_Cache {
	Pen_Cache *	next_pen_cache;
	DWORD				pen_style;
	DWORD				pen_width;
	DWORD				pen_rgbColor;
	HPEN				pen_handle;
	Line_Style			line_style;
	Pen_User *	next_user;
};


struct Brush_User {
	Brush_User *	next_user;
	Display_Context const	*dc;
};


struct Brush_Cache {
	Brush_Cache *	next_brush_cache;
	short						brush_style;
	int							brush_pattern_index;
	struct {
		COLORREF				colorref_color;
		int32_t			whole_thing;
		Driver_Color		whichever_color;
	}							brush_fg_color;
	Driver_Color			brush_bg_color;
	int							brush_incarnation;
	HBRUSH						brush_handle;
	Brush_User *	next_user;
};


struct Log_Font_Item {
	Log_Font_Item *	next;
	char *			name;
	char *			specific;
	LOGFONT			log_font;

	float			aheight;
	float			vspace;
	float			ratio;
	int32_t			trans_flags;
	Text_Encoding	encoding;
};

struct Sized_Font {
	LOGFONT const *		base_font;
	void *				w32_font_steciled; /*W32_Font_Stenciled*/
	long				max_char_width;
	float				ratio;
};

struct Font_User {
	Font_User *				next_user;
	Display_Context const *	dc;
};


struct Font_Cache {
	Font_Cache *	next_font_cache;
	LOGFONT const *	base_font;
	Font_User *		next_user;
	HFONT			font_handle;	
	long			font_size;
	long			font_escapement;
	float			width_scale;

};


typedef enum {
	win32s		= 0,
	win95		= 1,
	win98		= 2,
	winCE		= 3,
	winNT		= 4,
	win2K		= 5,
	winXP		= 6,
	winVista	= 7,
	win7		= 8,
	win8		= 9
} OS_Types;





struct System_Data {
	OS_Types							os_type;

	char								ini_app_name[4096];
	char								ini_file_name[4096];
	HKEY								ini_key;

	/*
	 * Patterns that can be shared by all instances
	 * of the MSW driver.  The bit patterns are
	 * precious GDI resources.	The line patterns are
	 * not, but are included for convenience.
	 */
	HBITMAP *							bit_patterns;
	
	/*
	 * hooks are HC_INTERFACE and optional
	 */
	FARPROC								next_getmessage_hook;
	FARPROC								getmessage_inst;
	int									hook_count;

	HANDLE								timer_thread;
	DWORD								id_thread;
	volatile bool						euthanize_timer;

	FARPROC								printer_abort;
	BOOL								user_abort;
	HWND								handle_dialog_print;

	char								print_app_name[80];

	unsigned char						mapped8_remap_cube[8];

	/*
	 * We need to share brushes and pen among COMPATABLE
	 * instances - there are three catagories that exist
	 */
	int									screen_pen_count;
	int									screen_pens_in_use;
	Pen_Cache							screen_pen_cache;
	int									screen_brush_count;
	int									screen_brushes_in_use;
	Brush_Cache							screen_brush_cache;
	int									screen_font_count;
	int									screen_fonts_in_use;
	Font_Cache							screen_font_cache;


	BOOL								in_error_routine;

	bool								msw_hoops_class;
	bool								ogl_hoops_class;
	bool								ogl2_hoops_class;
	bool								d3d_hoops_class;
	bool								dx11_hoops_class;

	/* Direct3D global data */
	void *								d3d_shared_data;
	void *								dx11_shared_data;
	/* OpenGL global data */
	void *								ogl_context_list;
	HOGLData_List *						ogl2_context_list;

	void *								ogl_master_context;
	System_Cleanup_Hook_Action			ogl_master_context_cleanup_hook;

	Log_Font_Item *						font_list;
};

End_HOOPS_Namespace

#define MSW_H_DEFINED	1
#endif /* MSW_H_DEFINED */
