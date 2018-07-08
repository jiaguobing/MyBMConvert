// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef OGL2_H_DEFINED
#define OGL2_H_DEFINED

#ifdef _MSC_VER
#	pragma warning(disable: 4505)	// unreferenced local function has been removed
#endif

//use vertex arrays instead of vbos for the vertex_buffer_cache/index_buffer_cache
#define VERTEX_ARRAY_BUFFER_CACHE
#define INDEX_ARRAY_BUFFER_CACHE

#define HOGL2_POLYMARKER_BATCH_SIZE 600

#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "driver.h"
#include "phedron.h"
#include "please.h"

#include "3d_common.h"

#include "collection_utils.h"

#ifdef OGL_ES
#define USE_GLSL_OPTIMIZER
#endif

#define	DEBUG_GLX_INDIRECT			0x00000002
// Debug_NO_WINDOWS_HOOK is provided so that users can tell us not to hook into
// the message queue for a window - this means that the user must process
// ALL messages and call Update_Display when they want hoops to draw
// into the provided window.
#define Debug_NO_DEPTH_COPY			0x00000004
#define	Debug_NO_WINDOWS_HOOK		0x00000040
#define	Debug_WGL_NO_SHARELISTS		0x00000100
#define Debug_USE_WINDOW_IS_IMAGE	0x00100000
#define Debug_16BIT_ZBUFFER			0x00200000
#define Debug_NO_OGL_OVERLAY_PLANES	0x00800000
#define Debug_FORCE_SOFTWARE		0x01000000
#define Debug_FORCE_SSAA			0x02000000

#define ACTION_NAME(a,b)

// XBIT STUFF

#define XBIT_NONE							0L
#define XBIT_BAD_MSAA_SPRITING				1L
#define XBIT_BAD_NON_POWER_OF_2_TEXTURES	2L
#define XBIT_TEXTURE_SIZE_OFF_BY_ONE		3L
#define XBIT_BAD_GLSL4_SAMPLER_CUBE			4L
#define XBIT_FORCE_GLSL2					5L
#define XBIT_BAD_BLIT_FILTER				6L
#define XBIT_BROKEN							7L
#define XBIT_NO_ANTIALIAS					8L
#define XBIT_AMD_BAD_DOUBLE_PRECISION		9L
#define XBIT_BAD_POINT_SPRITES				10L
#define XBIT_AMD_BAD_RGB24_TEXTURE			11L

#ifdef HOOPS_DEBUG_BUILD
static const char * gl_error_to_string(GLenum error) 
{
	switch(error) {
		case GL_NO_ERROR:				return "GL_NO_ERROR";
		case GL_INVALID_ENUM:			return "GL_INVALID_ENUM";
		case GL_INVALID_VALUE:			return "GL_INVALID_VALUE";
		case GL_INVALID_OPERATION:		return "GL_INVALID_OPERATION";
		//case GL_STACK_OVERFLOW:			return "GL_STACK_OVERFLOW";
		//case GL_STACK_UNDERFLOW:		return "GL_STACK_UNDERFLOW";
		case GL_OUT_OF_MEMORY:			return "GL_OUT_OF_MEMORY";
		//case GL_TABLE_TOO_LARGE:		return "GL_TABLE_TOO_LARGE";
		case GL_INVALID_FRAMEBUFFER_OPERATION: return "GL_INVALID_FRAMEBUFFER_OPERATION";
		default:						return "OpenGL Error!";
	}
}
#endif

static const char * framebuffer_status_to_string(GLenum status)
{
	switch (status) {
		case GL_FRAMEBUFFER_COMPLETE: return "GL_FRAMEBUFFER_COMPLETE";
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";			
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT: return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT
		case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS";
#endif
		   
#ifdef GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT
		case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT: return "GL_FRAMEBUFFER_INCOMPLETE_FORMATS";
#endif
			
#ifdef GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
#endif
			
#ifdef GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
#endif
			
#ifdef GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
#endif
			
		case GL_FRAMEBUFFER_UNSUPPORTED: return "GL_FRAMEBUFFER_UNSUPPORTED";		
		default: return null;
	}
}

/* Forward Declarations */
local bool start_device (Display_Context * dc);
local void stop_device (Display_Context * dc);

local void get_physical_info (Display_Context * dc);
local intptr_t get_outer_window (Display_Context const * const_dc);
local void get_current_info (Display_Context * dc);

local void finish_update (Display_Context const * dc);
local void init_update (Display_Context const * dc);

#if 0
local bool request_keyboard (Display_Context const * dc,
				  int * button, int * status);

local bool request_location (Display_Context const * dc,
				  int * button,
				  int * x, int * y);
#endif

local void init_picture (Net_Rendition const &  nr);
local void finish_picture (Net_Rendition const &  nr, bool swap_buffers);

local void common_free_outer_window(Display_Context const * dc);
local void free_outer_window (Display_Context const * dc);
local void retitle_outer_window (Display_Context const	*dc);
local void resize_outer_window (Display_Context const * dc);
local void identify_renderer (Display_Context * dc);

local void update_physical_info (Display_Context * dc);

local void flush_device(H3DData*);

#ifdef HOOPS_DEBUG_BUILD
#	define DEBUG_FLUSH()	glFlush()
#else
#	define DEBUG_FLUSH()	(void)0
#endif

struct HOGLData;
bool ensure_cs_line_pattern_resources(HOGLData *ogldata);
bool ensure_cs_edge_pattern_resources(HOGLData *ogldata);

/*****************************************************************************
*****************************************************************************
					Category: H3D -> OGL conversion functions
*****************************************************************************
*****************************************************************************/
//#define SHOW_CONVERSION_ERRORS

INLINE static GLenum oglprimitive(H3DPRIMITIVETYPE type)
{
	switch (type) {
		case H3DPT_POINTLIST:		return GL_POINTS;
		case H3DPT_LINELIST:		return GL_LINES;
		case H3DPT_LINESTRIP:    	return GL_LINE_STRIP;
		case H3DPT_TRIANGLELIST: 	return GL_TRIANGLES;
		case H3DPT_TRIANGLESTRIP:	return GL_TRIANGLE_STRIP;
		case H3DPT_TRIANGLEFAN:  	return GL_TRIANGLE_FAN;
		default:
#ifdef SHOW_CONVERSION_ERRORS
			HE_PROCEDURAL_ERROR("Unhandled primitive type mapping.");
#endif
			return GL_POINTS;
	}
}

INLINE static GLenum oglblend(H3DBLEND type)
{
	switch (type) {
		case H3DBLEND_ZERO:				return GL_ZERO;
		case H3DBLEND_ONE:				return GL_ONE;
		case H3DBLEND_SRCCOLOR:			return GL_SRC_COLOR;
		case H3DBLEND_INVSRCCOLOR:		return GL_ONE_MINUS_SRC_COLOR;
		case H3DBLEND_SRCALPHA:			return GL_SRC_ALPHA;
		case H3DBLEND_INVSRCALPHA:		return GL_ONE_MINUS_SRC_ALPHA;
		case H3DBLEND_DESTALPHA:		return GL_DST_ALPHA;
		case H3DBLEND_INVDESTALPHA:		return GL_ONE_MINUS_DST_ALPHA;
		case H3DBLEND_DESTCOLOR:		return GL_DST_COLOR;
		case H3DBLEND_INVDESTCOLOR:		return GL_ONE_MINUS_DST_COLOR;
		case H3DBLEND_SRCALPHASAT:		return GL_SRC_ALPHA_SATURATE;
#ifndef OGL_ES
		case H3DBLEND_BOTHSRCALPHA:		return GL_CONSTANT_ALPHA;
		case H3DBLEND_BOTHINVSRCALPHA:	return GL_ONE_MINUS_CONSTANT_ALPHA;
		case H3DBLEND_BLENDFACTOR:		return GL_CONSTANT_COLOR;
		case H3DBLEND_INVBLENDFACTOR:	return GL_ONE_MINUS_CONSTANT_COLOR;
#endif
		default:
#ifdef SHOW_CONVERSION_ERRORS
			HE_PROCEDURAL_ERROR("Unhandled blend type mapping.");
#endif
			return GL_ZERO;
	}
}

INLINE static GLbitfield oglclear(int flags)
{
	GLbitfield clear_flags = 0;
	if (ANYBIT(flags, H3DCLEAR_TARGET))
		clear_flags |= GL_COLOR_BUFFER_BIT;
	if (ANYBIT(flags, H3DCLEAR_ZBUFFER))
		clear_flags |= GL_DEPTH_BUFFER_BIT;
	if (ANYBIT(flags, H3DCLEAR_STENCIL))
		clear_flags |= GL_STENCIL_BUFFER_BIT;
	return clear_flags;
}

INLINE static GLenum oglcull(H3DCULL type)
{
	switch (type) {
		case H3DCULL_NONE:		return GL_FRONT_AND_BACK;
		case H3DCULL_CW:		return GL_BACK;
		case H3DCULL_CCW:		return GL_FRONT;
		default:
#ifdef SHOW_CONVERSION_ERRORS
			HE_PROCEDURAL_ERROR("Unhandled cull type mapping.");
#endif
			return GL_FRONT_AND_BACK;
	}
}


INLINE static GLenum oglshademode(H3DSHADEMODE type)
{
#ifndef OGL_ES
	switch (type) {
		case H3DSHADE_FLAT:		return GL_FLAT;
		case H3DSHADE_GOURAUD:	return GL_SMOOTH;
		case H3DSHADE_PHONG:	return GL_SMOOTH;
		default: HE_PROCEDURAL_ERROR("Unhandled shade mode mapping.");	return GL_SMOOTH;
	}
#else	
	return -1;		
#endif
}


INLINE static GLuint ogltextureaddress(H3DTEXTUREADDRESS type)
{
	switch (type) {
		case H3DTADDRESS_WRAP:			return GL_REPEAT;
		case H3DTADDRESS_CLAMP:			return GL_CLAMP_TO_EDGE;
		case H3DTADDRESS_MIRROR:		return GL_MIRRORED_REPEAT;
#ifndef OGL_ES
		case H3DTADDRESS_BORDER:		return GL_CLAMP_TO_BORDER;
		case H3DTADDRESS_MIRRORONCE:	return GL_MIRROR_CLAMP_EXT;
#endif
		default:
			HE_PROCEDURAL_ERROR("Unhandled texture mode.");
			return GL_REPEAT;
	}
}

INLINE static GLuint ogltexturefilter(H3DTEXTUREFILTERTYPE type)
{
	switch (type) {
		case H3DTEXF_NONE:				return GL_NEAREST;
		case H3DTEXF_POINT:				return GL_NEAREST;
		case H3DTEXF_LINEAR:			return GL_LINEAR;
		case H3DTEXF_ANISOTROPIC:		return GL_LINEAR_MIPMAP_NEAREST;
		case H3DTEXF_PYRAMIDALQUAD:		return GL_NEAREST;
		case H3DTEXF_GAUSSIANQUAD:		return GL_NEAREST;
		default:
			HE_PROCEDURAL_ERROR("Unhandled shade mode mapping.");
			return GL_NEAREST;
	}
}

INLINE static GLenum oglcmpfunc(H3DCMPFUNC type)
{
	switch (type) {
		case H3DCMP_NEVER :			return GL_NEVER;
		case H3DCMP_LESS :          return GL_LESS;
		case H3DCMP_EQUAL :         return GL_EQUAL;
		case H3DCMP_LESSEQUAL :     return GL_LEQUAL;
		case H3DCMP_GREATER :       return GL_GREATER;
		case H3DCMP_NOTEQUAL :      return GL_NOTEQUAL;
		case H3DCMP_GREATEREQUAL :  return GL_GEQUAL;
		case H3DCMP_ALWAYS :        return GL_ALWAYS;
		default:
#ifdef SHOW_CONVERSION_ERRORS
			HE_PROCEDURAL_ERROR("Unhandled comparison function mapping.");
#endif
			return GL_ALWAYS;
	}
}

INLINE static GLenum oglstencilop(H3DSTENCILOP type)
{
	switch (type) {
		case H3DSTENCILOP_KEEP :		return GL_KEEP;
		case H3DSTENCILOP_ZERO :		return GL_ZERO;
		case H3DSTENCILOP_REPLACE :		return GL_REPLACE;
		case H3DSTENCILOP_INCRSAT :		return GL_INCR;
		case H3DSTENCILOP_DECRSAT :		return GL_DECR;
		case H3DSTENCILOP_INVERT :		return GL_INVERT;
		case H3DSTENCILOP_INCR :		return GL_INCR_WRAP;
		case H3DSTENCILOP_DECR :		return GL_DECR_WRAP;
		default:
#ifdef SHOW_CONVERSION_ERRORS
			HE_PROCEDURAL_ERROR("Unhandled stencil op mapping.");
#endif
			return GL_KEEP;
	}
}

INLINE static GLenum oglcubemapface(H3DFACE face)
{
	switch (face) {
		case H3DFACE_POSITIVE_X :		return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
		case H3DFACE_NEGATIVE_X : 		return GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
		case H3DFACE_POSITIVE_Y : 		return GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
		case H3DFACE_NEGATIVE_Y : 		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
		case H3DFACE_POSITIVE_Z : 		return GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
		case H3DFACE_NEGATIVE_Z : 		return GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		default:
#ifdef SHOW_CONVERSION_ERRORS
			HE_PROCEDURAL_ERROR("Unhandled texture cube face mapping.");
#endif
			return GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	}
}

/*****************************************************************************
*****************************************************************************
					Category: Force/Ensure Macros
*****************************************************************************
*****************************************************************************/


#ifdef HOOPS_DEBUG_BUILD

#define GL_GET_ERROR() glGetError()

static void ASSERT_GL_INTEGER(GLenum param, GLuint cached)
{
	GLint check = 0; 
	glGetIntegerv(param, &check);
	ASSERT(static_cast<GLuint>(check) == cached);
}

static void GL_CHECK_ERROR()
{
	GLint error = GL_GET_ERROR();
	if (error != GL_NO_ERROR) {
		const char * error_string = gl_error_to_string(error);
		dprintf("%s\n",error_string);
		ASSERT(0);
	}
}

#else 
#define GL_GET_ERROR() GL_NO_ERROR
#define ASSERT_GL_INTEGER(binding, buffer) do { /* nothing */ } while(0)
#define GL_CHECK_ERROR()  do { /* nothing */ } while(0)
#endif

void FORCE_ACTIVE_TEXTURE(H3DData *h3ddata, int unit);
#define ENSURE_ACTIVE_TEXTURE(ogldata,unit) SEMI_PROTECT(\
	if ((int) unit != ((HOGLData*)ogldata)->oglcache.current_active_texture) \
		FORCE_ACTIVE_TEXTURE(ogldata,unit); \
)

static void FORCE_TEXTURE_MIP_FILTER(H3DData * ogldata, int unit, H3DTEXTUREFILTERTYPE mode);

#define FORCE_TEXTURE_WRAP_S(ogldata, texture_unit, mode)	SEMI_PROTECT(\
	ENSURE_ACTIVE_TEXTURE (ogldata, texture_unit); \
	((HOGLTexture2D *) ogldata->cache->texture[texture_unit])->m_texture_wrap_s = mode; \
	GLenum _target_ = ogldata->cache->texture_target[texture_unit]; \
	glTexParameteri (_target_, GL_TEXTURE_WRAP_S, ogltextureaddress(mode)); \
)

#define ENSURE_TEXTURE_WRAP_S(ogldata, texture_unit, mode)	SEMI_PROTECT(\
	if (((HOGLTexture2D *) ogldata->cache->texture[texture_unit])->m_texture_wrap_s != mode) \
		FORCE_TEXTURE_WRAP_S(ogldata, texture_unit, mode); \
)

#define FORCE_TEXTURE_WRAP_T(ogldata, texture_unit, mode)	SEMI_PROTECT(\
	ENSURE_ACTIVE_TEXTURE (ogldata, texture_unit); \
	((HOGLTexture2D *) ogldata->cache->texture[texture_unit])->m_texture_wrap_t = mode; \
	GLenum _target_ = ogldata->cache->texture_target[texture_unit]; \
	glTexParameteri (_target_, GL_TEXTURE_WRAP_T, ogltextureaddress(mode)); \
)

#define ENSURE_TEXTURE_WRAP_T(ogldata, texture_unit, mode)	SEMI_PROTECT(\
	if (((HOGLTexture2D *) ogldata->cache->texture[texture_unit])->m_texture_wrap_t != mode) \
		FORCE_TEXTURE_WRAP_T(ogldata, texture_unit, mode); \
)

#define ENSURE_TEXTURE_MIP_FILTER(ogldata, unit, mode) SEMI_PROTECT(\
	if (((HOGLTexture2D *) ogldata->cache->texture[unit])->m_mip_filter != mode) \
		FORCE_TEXTURE_MIP_FILTER(ogldata, unit, mode); \
)

#define FORCE_TEXTURE_MIN_FILTER(ogldata, unit, mode) SEMI_PROTECT(\
	ENSURE_ACTIVE_TEXTURE (ogldata, unit); \
	((HOGLTexture2D *) ogldata->cache->texture[unit])->m_min_filter = mode; \
	FORCE_TEXTURE_MIP_FILTER(ogldata, unit, ((HOGLTexture2D *) ogldata->cache->texture[unit])->m_mip_filter); \
)

#define ENSURE_TEXTURE_MIN_FILTER(ogldata, unit, mode) SEMI_PROTECT(\
	if (((HOGLTexture2D *) ogldata->cache->texture[unit])->m_min_filter != mode) \
		FORCE_TEXTURE_MIN_FILTER(ogldata, unit, mode); \
)

#define FORCE_TEXTURE_MAG_FILTER(ogldata, unit, mode) SEMI_PROTECT(\
	ENSURE_ACTIVE_TEXTURE (ogldata, unit); \
	((HOGLTexture2D *) ogldata->cache->texture[unit])->m_mag_filter = mode; \
	GLenum _target_ = ogldata->cache->texture_target[unit]; \
	glTexParameteri (_target_, GL_TEXTURE_MAG_FILTER, ogltexturefilter(mode)); \
)

#define ENSURE_TEXTURE_MAG_FILTER(ogldata, unit, mode) SEMI_PROTECT(\
	if (((HOGLTexture2D *) ogldata->cache->texture[unit])->m_mag_filter != mode) \
		FORCE_TEXTURE_MAG_FILTER(ogldata, unit, mode); \
)

/*****************************************************************************
*****************************************************************************
					Category: HOGLData
*****************************************************************************
*****************************************************************************/

enum OGL2_FeatureLevel
{
	OGL2_Desktop,
	OGL2_ES2,
	OGL2_ES3,
	OGL2_CoreProfile,
};

class HOGLFrameBufferObject;
class HOGLRenderTarget;
class HOGLTexture2D;
class HOGLSimpleShader;

struct HOGLData : H3DData
{
	GLEWContext *glewcontext;

#ifdef GLX_OPENGL2_DRIVER
	GLXEWContext *	glxewcontext;
	Display *	display;
	X_Window 			window;
	GLXContext 			context;
	XVisualInfo *	visualinfo;
	Screen *	screen;
	Visual *	visual;

	/* use to mung the X11 driver */
	Action_Table x11;

	struct {
		X_Window window;
		GLXContext context;
		Colormap colormap;
		int num_colors;
		long width;
		long height;
		int trans_pixel;
		Driver_Color *colors;
	} overlay;

#endif
	
#if defined(COCOA_OGL_DRIVER) || defined(COCOA_OGL2_DRIVER)
	NSOpenGLContext * context;
	NSView * view;
	NSWindow * window;
	CGDirectDisplayID display_id;
#endif

#ifdef WINDOWS_SYSTEM
	HGLRC hGLRC;
	WGLEWContext *wglewcontext;	
#endif

#if TARGET_OS_IPHONE
	UIView *uiview;
	EAGLContext *eagl_context;
	NSAutoreleasePool * ns_autorelease_pool;
	HOGLRenderTarget *ios_renderbuffer;
#elif TARGET_OS_ANDROID
	EGLNativeWindowType window;
	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	EGLConfig	config;
	JavaVM * jvm;
	jobject app_ctx;
	JNIEnv * jenv;
	int android_version;
#endif
	
	int window_width;
	int window_height;

	bool can_anti_alias;	
	int max_msaa_samples;
	int back_buffer_samples;
	uint32_t max_vertex_attribs;

	int max_line_weight;
	float max_anisotropy;

	int glsl_version;
	OGL2_FeatureLevel feature_level;

	bool debug_print_actions;
	bool debug_shader_dumped;

	//don't put bools in here -- we need to be able to mark stuff as invalid
	struct {
		
		GLuint current_program;

		GLuint read_fbo;
		GLuint draw_fbo;
		
		GLuint current_array_buffer;
		GLuint current_element_buffer;
		GLuint current_uniform_buffer;
		GLuint current_shader_storage_buffer;
		int current_active_texture;		

		unsigned int attrib_locations;
		unsigned int attrib_locations_mirror;
		
	} oglcache;

	struct OITData
	{
		HOGLSimpleShader * oit_compose;

		GLuint heads_texture;
		GLuint heads_buffer;

		GLuint data_buffer;
		GLuint data_texture;

		GLuint atomic_buffer;

	} oit_data;

	struct LinePatternCS
	{
		GLuint clip_shader;
		GLuint compute_lengths_shader;
		GLuint sum_shader;

		GLuint constants;
	} linepat_cs;

	struct EdgePatternCS
	{
		GLuint clip_shader;
		GLuint compute_lengths_shader;

		GLuint constants;
	} edgepat_cs;

	H3DTexture *point_splat_dummy_tex;

	HOGLFrameBufferObject *fbo;	

	//spriting
	QuickMovesRegion * image_regions;		
	QuickMovesRegion * depth_regions;

	bool occlusion_query_flush_required;	
};


INLINE static void FORCE_ARRAY_BUFFER(HOGLData * ogldata, GLuint buffer) {
	ogldata->oglcache.current_array_buffer = buffer;
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
}

INLINE static void ENSURE_ARRAY_BUFFER(HOGLData * ogldata, GLuint buffer) {
	if (buffer != ogldata->oglcache.current_array_buffer)
		FORCE_ARRAY_BUFFER(ogldata, buffer);
	ASSERT_GL_INTEGER(GL_ARRAY_BUFFER_BINDING, buffer);
}

INLINE static void FORCE_ELEMENT_BUFFER(HOGLData * ogldata, GLuint buffer) {
	ogldata->oglcache.current_element_buffer = buffer;
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}

INLINE static void ENSURE_ELEMENT_BUFFER(HOGLData * ogldata, GLuint buffer) {
	if (buffer != ogldata->oglcache.current_element_buffer)
		FORCE_ELEMENT_BUFFER(ogldata, buffer);
	ASSERT_GL_INTEGER(GL_ELEMENT_ARRAY_BUFFER_BINDING, buffer);
}

INLINE static void FORCE_SHADER_STORAGE_BUFFER(HOGLData * ogldata, GLuint buffer) {
	ogldata->oglcache.current_shader_storage_buffer = buffer;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buffer);
}

INLINE static void ENSURE_SHADER_STORAGE_BUFFER(HOGLData * ogldata, GLuint buffer) {
	if (buffer != ogldata->oglcache.current_shader_storage_buffer)
		FORCE_SHADER_STORAGE_BUFFER(ogldata, buffer);
	ASSERT_GL_INTEGER(GL_SHADER_STORAGE_BUFFER_BINDING, buffer);
}

INLINE static void FORCE_UNIFORM_BUFFER(HOGLData * ogldata, GLuint buffer) {
	ogldata->oglcache.current_uniform_buffer = buffer;
	glBindBuffer(GL_UNIFORM_BUFFER, buffer);
}

INLINE static void ENSURE_UNIFORM_BUFFER(HOGLData * ogldata, GLuint buffer) {
	if (buffer != ogldata->oglcache.current_uniform_buffer)
		FORCE_UNIFORM_BUFFER(ogldata, buffer);
	ASSERT_GL_INTEGER(GL_UNIFORM_BUFFER_BINDING, buffer);
}

INLINE static void FORCE_PROGRAM(HOGLData * ogldata, GLuint program) {
	ogldata->oglcache.current_program = program;
	glUseProgram(program);
}

INLINE static void ENSURE_PROGRAM(HOGLData * ogldata, GLuint program) {
	if (ogldata->oglcache.current_program != program)
		FORCE_PROGRAM(ogldata, program);
	ASSERT_GL_INTEGER(GL_CURRENT_PROGRAM, program);
}

INLINE static void FORCE_TEXTURE_MIP_FILTER(H3DData * h3ddata, int unit, H3DTEXTUREFILTERTYPE mode) {

	ENSURE_ACTIVE_TEXTURE(h3ddata, unit);
	h3ddata->cache->texture[unit]->m_mip_filter = mode;
	GLenum _target_ = h3ddata->cache->texture_target[unit];

	if (h3ddata->cache->texture[unit]->m_min_filter == H3DTEXF_POINT) {
		switch (mode) {
			case H3DTEXF_NONE: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST); break;
			case H3DTEXF_POINT: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); break;
			case H3DTEXF_LINEAR: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR); break;
			default: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_INVALID_ENUM); break;
		}
	}

	else if (h3ddata->cache->texture[unit]->m_min_filter == H3DTEXF_LINEAR) {
		switch (mode) {
			case H3DTEXF_NONE: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR); break;
			case H3DTEXF_POINT: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST); break;
			case H3DTEXF_LINEAR: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); break;
			default: glTexParameteri(_target_, GL_TEXTURE_MIN_FILTER, GL_INVALID_ENUM); break;
		}
	}

	GL_CHECK_ERROR();
}


INLINE static void ENSURE_READ_FRAME_BUFFER(HOGLData * ogldata, GLuint in_fbo) {
	if (ogldata->oglcache.read_fbo != in_fbo) {
		ogldata->oglcache.read_fbo = in_fbo;
		glBindFramebuffer(GL_READ_FRAMEBUFFER, in_fbo);
	}
	ASSERT_GL_INTEGER(GL_READ_FRAMEBUFFER_BINDING,in_fbo);
}

INLINE static void ENSURE_DRAW_FRAME_BUFFER(HOGLData * ogldata, GLuint in_fbo) {
	if (ogldata->oglcache.draw_fbo != in_fbo) {
		ogldata->oglcache.draw_fbo = in_fbo;
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, in_fbo);
	}
	ASSERT_GL_INTEGER(GL_DRAW_FRAMEBUFFER_BINDING,in_fbo);
}

// binding GL_FRAMEBUFFER is equivalent to binding both READ_FRAMEBUFFER and DRAW_FRAMEBUFFER
INLINE static void ENSURE_FRAME_BUFFER(HOGLData * ogldata, GLuint in_fbo) {
	if (ogldata->oglcache.read_fbo != in_fbo || ogldata->oglcache.draw_fbo != in_fbo) {
		ogldata->oglcache.read_fbo = in_fbo;
		ogldata->oglcache.draw_fbo = in_fbo;
		glBindFramebuffer(GL_FRAMEBUFFER, in_fbo);
	}
	ASSERT_GL_INTEGER(GL_FRAMEBUFFER_BINDING,in_fbo);
}

#ifdef HOOPS_DEBUG_BUILD
static void ASSERT_FRAMEBUFFER_COMPLETE(HOGLData * ogldata, GLenum target) {
	GLenum status = glCheckFramebufferStatus(target);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		const char * error_string = framebuffer_status_to_string(status);
		dprintf("%s\n",error_string);
		ASSERT(0);
	}
}

static void ASSERT_VERTEX_ATTRIB_ARRAY(HOGLData * ogldata, GLuint index, GLboolean enabled) {
	GLint value = 0;
	glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &value);
	ASSERT(value == enabled);
}

#else
#define ASSERT_FRAMEBUFFER_COMPLETE(ogldata, target) do { /* nothing */ } while(0)	
#define ASSERT_VERTEX_ATTRIB_ARRAY(ogldata_, index_, enabled_) do { /* nothing */ } while(0)
#endif

#define OGLD(dc) ((HOGLData *)((dc)->data2))
#define OGLNRD(nr) (OGLD((nr)->display_context))

#ifdef GLX_OPENGL2_DRIVER
#  define X11D(dc) ((X11Data *)((dc)->data))
#  define X11NRD(nr) (X11D(nr->display_context))
#endif

INLINE void SET_VERTEX_ATTRIB_POINTER(HOGLData *ogldata, GLint location, GLint size, GLenum type, GLboolean normalized, const GLvoid *pointer) 
{
	if (location >= 0) {
		if (type == GL_DOUBLE)
			glVertexAttribLPointer(location, size, GL_DOUBLE, 0, pointer);
		else
			glVertexAttribPointer(location,size,type,normalized,0,pointer);

		ogldata->oglcache.attrib_locations |= 0x1 << location;

		if (type == GL_DOUBLE && XBIT_TEST(ogldata->card_entry->xbits, XBIT_AMD_BAD_DOUBLE_PRECISION) && ogldata->oglcache.current_array_buffer != 0) {
			ogldata->oglcache.attrib_locations |= 0x1 << (location + 1);
		}
	}
}

INLINE void SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(HOGLData *ogldata, GLint location, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) 
{
	if (location >= 0) {
		if (type == GL_DOUBLE)
			glVertexAttribLPointer(location, size, type, stride, pointer);
		else
			glVertexAttribPointer(location,size,type,normalized,stride,pointer);

		ogldata->oglcache.attrib_locations |= 0x1 << location;

		if (type == GL_DOUBLE && XBIT_TEST(ogldata->card_entry->xbits, XBIT_AMD_BAD_DOUBLE_PRECISION) && ogldata->oglcache.current_array_buffer != 0) {
			ogldata->oglcache.attrib_locations |= 0x1 << (location + 1);
		}
	}
}

#define BEGIN_VERTEX_ATTRIBS(ogldata) (ogldata)->oglcache.attrib_locations = 0

INLINE void END_VERTEX_ATTRIBS(HOGLData *ogldata) 
{
	if (ogldata->oglcache.attrib_locations_mirror == INVALID_CACHE)
	{
		// last N 1s		
		uint32_t const last_n = (uint32_t)(((uint64_t)1 << ogldata->max_vertex_attribs) - 1);
		ogldata->oglcache.attrib_locations_mirror = ogldata->oglcache.attrib_locations ^ last_n;
	}

	int mask = 0x1; 
	uint32_t location = 0;
	while (ogldata->oglcache.attrib_locations != ogldata->oglcache.attrib_locations_mirror) { 
		if (ANYBIT (ogldata->oglcache.attrib_locations, mask)) { 
			if (!ANYBIT (ogldata->oglcache.attrib_locations_mirror, mask)) { 
				glEnableVertexAttribArray(location); 
				ogldata->oglcache.attrib_locations_mirror |= mask; 
			} 
			ASSERT_VERTEX_ATTRIB_ARRAY(ogldata, location, GL_TRUE);
		} 
		else { 
			if (ANYBIT (ogldata->oglcache.attrib_locations_mirror, mask)) { 
				glDisableVertexAttribArray(location); 
				ogldata->oglcache.attrib_locations_mirror &= ~mask; 
			} 
			ASSERT_VERTEX_ATTRIB_ARRAY(ogldata, location, GL_FALSE);
		}

		mask = mask << 1; 		
		location++;

		if (location >= ogldata->max_vertex_attribs)
			break;
	} 
}

/*****************************************************************************
*****************************************************************************
					Category: HOGLShader class
*****************************************************************************
*****************************************************************************/
#define glUniformfv(location, num, data) SEMI_PROTECT (\
	switch (num) { \
		case 1: glUniform1fv (location, 1, (const GLfloat *) data); break; \
		case 2: glUniform2fv (location, 1, (const GLfloat *) data); break; \
		case 3: glUniform3fv (location, 1, (const GLfloat *) data); break; \
		case 4: glUniform4fv (location, 1, (const GLfloat *) data); break; \
	} \
)

#define glUniformiv(location, num, data) SEMI_PROTECT (\
	switch (num) { \
		case 1: glUniform1iv (location, 1, (const GLint *) data); break; \
		case 2: glUniform2iv (location, 1, (const GLint *) data); break; \
		case 3: glUniform3iv (location, 1, (const GLint *) data); break; \
		case 4: glUniform4iv (location, 1, (const GLint *) data); break; \
	} \
)

// Stores vertex attribute locations used to bind a vertex buffer to a shader for drawing
class HOGLVertexAttributes
{
public:

	HOGLVertexAttributes() : 
		position(-1), 
		normal(-1),
		color(-1),
		secondary_color(-1),
		fog_coord(-1)		
	{
		memset(texcoords,0xFF,sizeof(texcoords));
	}

	HOGLVertexAttributes(HOGLData * ogldata, GLuint program)
	{
		GetAttribLocations(ogldata, program);
	}

	GLint position;
	GLint normal;
	GLint color;
	GLint secondary_color;
	GLint fog_coord;
	GLint texcoords[8];

	void GetAttribLocations(HOGLData * ogldata, GLuint program)
	{
		position = glGetAttribLocation(program, "_in_position");
		normal = glGetAttribLocation(program, "_in_normal");
		color = glGetAttribLocation(program, "_in_color");
		secondary_color = glGetAttribLocation(program, "_in_secondary_color");
		texcoords[0] = glGetAttribLocation(program, "_in_texcoord0");
		texcoords[1] = glGetAttribLocation(program, "_in_texcoord1");
		texcoords[2] = glGetAttribLocation(program, "_in_texcoord2");
		texcoords[3] = glGetAttribLocation(program, "_in_texcoord3");
		texcoords[4] = glGetAttribLocation(program, "_in_texcoord4");
		texcoords[5] = glGetAttribLocation(program, "_in_texcoord5");
		texcoords[6] = glGetAttribLocation(program, "_in_texcoord6");
		texcoords[7] = glGetAttribLocation(program, "_in_texcoord7");
		fog_coord = glGetAttribLocation(program, "_in_fogcoord");

		GL_CHECK_ERROR();
	}
};

// Helper class for GL compile functions. This should be the only place we call glCompileShader, glLinkProgram, etc.
class HOGLShaderCompiler
{
public:

	HOGLShaderCompiler(HOGLData * ogldata) : ogldata(ogldata)
	{

	}

	bool Compile(GLenum type, const char * source, GLuint & out_shader, bool report_error = true)
	{
		ASSERT(type == GL_VERTEX_SHADER || type == GL_FRAGMENT_SHADER || type == GL_GEOMETRY_SHADER || type == GL_COMPUTE_SHADER);

		GLuint shader = glCreateShader(type);

		glShaderSource(shader, 1, &source, 0);
		glCompileShader(shader);		

		GLint status = GL_FALSE;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		
		if (status == GL_TRUE)
		{
			out_shader = shader;			

#ifdef HOOPS_DEBUG_BUILD 
			// Look for warnings
			if (GetShaderLog(shader) > 0)
				dprintf("opengl2 shader compiler reports:\n%s",(char const*)log);			
#endif
			GL_CHECK_ERROR();

			return true;
		}

		else if (report_error)
		{	
			GetShaderLog(shader);
			
			HE_PROCEDURAL_ERROR(Sprintf_S (null, "opengl2 shader compile failed:\n%S", log));			

#ifdef _MSC_VER
			dprintf("opengl2 shader compile failed:\n%s",(char const*)log);
#endif
			out_shader = 0;
			glDeleteShader(shader);
			GL_CHECK_ERROR();			
		}

		return false;
	}

	GLuint CreateProgram(GLuint vs, GLuint ps, GLuint gs = 0)
	{
		GLuint program = glCreateProgram();

		glAttachShader(program, vs);
		glAttachShader(program, ps);
		if (gs)
		{		
			glAttachShader(program, gs);
		}

		GL_CHECK_ERROR();

		return program;
	}

	GLuint CreateProgram(GLuint cs)
	{
		GLuint program = glCreateProgram();

		glAttachShader(program, cs);

		GL_CHECK_ERROR();

		return program;
	}

	bool LinkProgram(GLuint program, bool cleanup_shaders = true, bool report_error = true)
	{
		glLinkProgram(program);

		GLint status = GL_FALSE;
		glGetProgramiv(program, GL_LINK_STATUS, &status);

		if (status == GL_TRUE)
		{
#ifdef	HOOPS_DEBUG_BUILD
			if (GetProgramLog(program) > 0)
				dprintf("opengl2 shader link reports:\n%s",(char const*)log);
#endif
		}

		else 
		{
			if (report_error)
			{	
				GetProgramLog(program);

#ifdef _MSC_VER
				dprintf("opengl2 shader link failed:\n%s",(char const*)log);
#endif
				HE_PROCEDURAL_ERROR(Sprintf_S (null, "opengl2 shader link failed:\n%S", log));			
			}

			if (cleanup_shaders)
			{		
				GLuint shaders[3] = {0,0,0};
				GLsizei num_shaders = 0;
				glGetAttachedShaders(program, 3, &num_shaders, shaders);

				for (int i = 0 ; i < num_shaders ; i++)
				{
					glDeleteShader(shaders[i]);
				}
			}

			glDeleteProgram(program);

			GL_CHECK_ERROR();

			return false;
		}

		GL_CHECK_ERROR();

		return true;
	}

private:

	GLsizei GetShaderLog(GLuint shader)
	{
		GLsizei len = 0;		
		glGetShaderInfoLog(shader, (GLsizei)log.size, &len, log);			
		return len;
	}

	GLsizei GetProgramLog(GLuint program)
	{
		GLsizei len = 0;		
		glGetProgramInfoLog(program, (GLsizei)log.size, &len, log);
		return len;
	}
	
	ErrBuffer log;
	HOGLData * ogldata;
};

class HOGLUniform : public CMO_Pooled
{
private:
	float cached_value[16];
public:

	HOGLUniform() : location(-1), value_valid(false)
	{
		ZERO_ARRAY(cached_value, 16, float);
	}

	INLINE bool TryUpdateValue(void const * new_value, uint32_t size_in_bytes)
	{
		ASSERT(size_in_bytes <= sizeof(cached_value));

		if (value_valid && EQUAL_MEMORY(cached_value, size_in_bytes, new_value))
		{
			return false;
		}
		
		else 
		{			
			COPY_MEMORY(new_value, size_in_bytes, cached_value); 
			value_valid = true;
			return true;
		}
	}

	GLint location;

private:
	bool value_valid;

};

typedef Unordered<Name, HOGLUniform *, NameHasher,
	std::equal_to<const Name>, POOL_Allocator<std::pair<const Name, HOGLUniform *> >> HOGLUniform_Hash;


class HOGLUniformCache : public CMO_Pooled
{
public:

	HOGLUniformCache(HOGLData * ogldata) : ogldata(ogldata), program(0), hash(ogldata->dc->memory_pool())
	{
		
	}

	~HOGLUniformCache()
	{
		for (auto it = hash.begin(), end = hash.end(); it != end; ++it)
			delete it->second;
	}

	void SetProgram(GLuint program) 
	{
		ASSERT(program);
		this->program = program;
	}

	HOGLUniform * GetUniform(Name_Const const & name) {

		ASSERT(program != 0);

		HOGLUniform * ui = 0;

		auto it = hash.find(name);

		if (it == hash.end()) {
			// This should be the *only* place we call glGetUniformLocation.
			ui = POOL_NEW(ogldata->dc->memory_pool(), HOGLUniform);
			ui->location = glGetUniformLocation(program, name.text());
			hash.insert(std::make_pair(Name(name), ui));
		}
		else
			ui = it->second;

		GL_CHECK_ERROR();

		return ui;
	}

	GLint GetUniformLocation(Name_Const const & name) {
		return GetUniform(name)->location;
	}

private:

	HOGLData *			ogldata;
	GLuint				program;
	HOGLUniform_Hash	hash;
};

class HOGLSimpleShader 
{
public:

	HOGLData * ogldata;
	GLuint program;	
	HOGLUniformCache uniforms;
	HOGLVertexAttributes vertex_attributes;

	HOGLSimpleShader(HOGLData * ogldata) : ogldata(ogldata), program(0), uniforms(ogldata)
	{

	}

	bool Init(const char * vs_source, const char * ps_source)
	{
		ASSERT(program == 0);

		HOGLShaderCompiler compiler(ogldata);
		
		GLuint vs = 0;
		compiler.Compile(GL_VERTEX_SHADER, vs_source, vs);

		GLuint ps = 0;
		compiler.Compile(GL_FRAGMENT_SHADER, ps_source, ps);

		program = compiler.CreateProgram(vs, ps);

		if (!compiler.LinkProgram(program))
		{
			program = 0;
			return false;
		}

		else 
		{
			uniforms.SetProgram(program);
		}

		return true;
	}

	void Destroy()
	{
		if (program)
		{		
			GLuint shaders[3] = {0,0,0};
			GLsizei num_shaders = 0;
			glGetAttachedShaders(program, 3, &num_shaders, shaders);

			for (int i = 0 ; i < num_shaders ; i++)
			{
				glDeleteShader(shaders[i]);
			}

			glDeleteProgram(program);

			program = 0;

			GL_CHECK_ERROR();
		}
	}

	~HOGLSimpleShader()
	{
		Destroy();
	}
};

#ifdef USE_GLSL_OPTIMIZER

#include "glsl_optimizer.h"

class GLSL_Optimizer
{
public:

	GLSL_Optimizer(OGL2_FeatureLevel feature_level) : ctx(null)
	{
		glslopt_target target;
		switch (feature_level)
		{
			case OGL2_Desktop: target = kGlslTargetOpenGL; break;
			case OGL2_ES2: target = kGlslTargetOpenGLES20; break;
			case OGL2_ES3: target = kGlslTargetOpenGLES30; break;
		}

		ctx = glslopt_initialize(target);
	}

	~GLSL_Optimizer()
	{
		glslopt_cleanup(ctx);
	}

	bool Optimize(GLenum type, const char * source, char * buffer)
	{
		glslopt_shader * shader = glslopt_optimize(ctx, type == GL_VERTEX_SHADER ? kGlslOptShaderVertex : kGlslOptShaderFragment, source, 0);

		bool status = glslopt_get_status (shader);

		if (status)
		{
			const char *optimized_vs_source = glslopt_get_output(shader);
			strcpy(buffer, optimized_vs_source);
		}

		else
		{
			const char *errorLog = glslopt_get_log(shader);
			dprintf("opengl2 shader optimizer failed:\n%s", errorLog);
		}

		glslopt_shader_delete (shader);

		return status;
	}

private:

	glslopt_ctx * ctx;

};

#endif

#include "ogl2_post.h"

class HOGLShader : public H3DShader
{
private:
	bool snorch_preoptimized_source (const char *name, char *source) const;		
protected:
	char const * get_shader_extension() const {return "glsl";};
	virtual void GetBuiltInMacros(Memory_Pool *	pool, intptr_t_Lookup_Hash * macros);
	OGL2_FeatureLevel GetEffectiveFeatureLevel() const
	{
		HOGLData * ogldata = (HOGLData *)m_h3ddata;

		if (ogldata->feature_level == OGL2_ES3 && m_id.IsTextureExternalOES())
			return OGL2_ES2;

		return ogldata->feature_level;
	}
public:
	GLint m_pProgram;
	GLuint m_pVS;
	GLuint m_pPS;
	GLuint m_pGS;
	HOGLVertexAttributes m_vertex_attributes;
	// this is another level of caching below the H3D_Shader_Handle_Type stuff. 
	// It simply ensures that we make the minimum necessary number of calls to glGetUniformLocation.
	HOGLUniformCache m_uniforms;

public:
	HOGLShader (HOGLData * ogldata, const H3DShaderID& id);
	~HOGLShader ();

	bool Create(H3DData *h3ddata, const char *material_shader_source = null);
	bool Activate();

	char *EmitTexture(char *ptr, HTextureDimension dimension, char const *name, int number, int sampler, HTexelType texel_type);

	void EnsureDepthPeeling(H3DTexture *texture, H3DData *h3ddata);
	void EnsureTextures (bool back_face = false);
	GLuint GetProgram() {return m_pProgram;};

	void SetConstantInt(H3D_Constant_Table, int shc, int data)
	{
		HOGLData *ogldata = (HOGLData *)m_h3ddata;
		UNREFERENCED(ogldata);

		HOGLUniform * ui = (HOGLUniform *)cache[shc];

		if (!ui->TryUpdateValue(&data, sizeof(int)))
			return;

		glUniform1i(ui->location, data);
	}

	void SetConstantUInt(H3D_Constant_Table, int shc, unsigned int data)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		UNREFERENCED(ogldata);

		HOGLUniform * ui = (HOGLUniform *)cache[shc];

		if (!ui->TryUpdateValue(&data, sizeof(unsigned int)))
			return;

		GLint glhandle = ui->location;
		#ifdef OGL_ES
		// I'm not sure this is legal.
		glUniform1i (glhandle, data);
		#else
		// glUniform1uiEXT may not be available on all drivers
		if (glUniform1uiEXT)
			glUniform1uiEXT(glhandle, data);
		else
			glUniform1i(glhandle, data);
		#endif
	}

	void SetConstantFloat(H3D_Constant_Table, int shc, float data)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		UNREFERENCED(ogldata);

		HOGLUniform * ui = (HOGLUniform *)cache[shc];

		if (!ui->TryUpdateValue(&data, sizeof(float)))
			return;

		glUniform1f (ui->location, data);
	}

	void SetConstantArray(H3D_Constant_Table, int shc, void const *data, int datatype, int num = 1)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		UNREFERENCED(ogldata);		

		int num_values = num;

		if (datatype == H3D_Vector)
			num_values = num * 4;

		else if (datatype == H3D_DMatrix || datatype == H3D_Double)
			num_values = num * 2; 

		HOGLUniform * ui = (HOGLUniform *)cache[shc];

		if (num_values <= 16) {
			if (!ui->TryUpdateValue(data, num_values * sizeof(float)))
				return;
		}

		GLint glhandle = ui->location;
		GLfloat const * value = (GLfloat const *) data;

		switch (datatype) {
			case H3D_Integer:
				glUniformiv (glhandle, num, (GLint const *) data);
				break;
			case H3D_Float:
				glUniformfv (glhandle, num, value);
				break;
			case H3D_DMatrix:
				glUniformMatrix4dv(glhandle, 1, GL_FALSE, (GLdouble const *)data);
				break;
			case H3D_Double:
				switch (num) {
					case 3: glUniform3dv(glhandle, 1, (GLdouble const *)data); 
						break;
					default: 
						ASSERT(0);
						break;
				}
				break;
			case H3D_Matrix:

				switch (num) {
					case 4:
						glUniformMatrix2fv (glhandle, 1, false, value);
						break;
					case 9:
						glUniformMatrix3fv (glhandle, 1, false, value);
						break;
					case 16:
						glUniformMatrix4fv (glhandle, 1, false, value);
						break;
					default:
						ASSERT(0);		//6,8,12 elements: not supported yet!
						break;
				}
				break;
			default:
				ASSERT(0);
				break;
		}
	
		GL_CHECK_ERROR();
	}

	void SetConstantVector(H3D_Constant_Table table, int shc, float const * data, int width) {
		SetConstantVectorArray(table, shc, data, 1, width);
	}
	void SetConstantVectorArray(H3D_Constant_Table, int shc, float const * data, int num_vectors, int width) {
	
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		UNREFERENCED(ogldata);		

		int num_values = num_vectors * width;

		HOGLUniform * ui = (HOGLUniform *)cache[shc];

		if (num_values <= 16) {
			if (!ui->TryUpdateValue(data, num_values*sizeof(float)))
				return;
		}

		GLint glhandle = ui->location;

		switch(width) {
			case 1: 
				glUniform1fv (glhandle, num_vectors, data);
				break;
			case 2: 
				glUniform2fv (glhandle, num_vectors, data);
				break;
			case 3: 
				glUniform3fv (glhandle, num_vectors, data);
				break;
			case 4: 
				glUniform4fv (glhandle, num_vectors, data);
				break;
		}

		GL_CHECK_ERROR();
	}

	H3D_Shader_Handle GetConstantByName(H3D_Constant_Table, int, char const * parent, char const * constant)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		UNREFERENCED(ogldata);

		HOGLUniform * ui = 0;

		if (parent) {
			char buf[1024];
			sprintf(buf, "%s.%s", parent, constant);
			ui = m_uniforms.GetUniform(buf);
		}
		else {
			ui = m_uniforms.GetUniform(constant);			
		}

		GL_CHECK_ERROR();

		H3D_Shader_Handle result = ui == null ? INVALID_SHADER_HANDLE : reinterpret_cast<H3D_Shader_Handle>(ui);		

		return result;
	}	

	char *AppendOGLHeader (
		const char *filename,
		const char *shader_string,
		char *ptr,
		char const *end,
		const char *glsl_version,
		ShaderType type);
};

#define HOGL_PER_OBJECT_PENALTY 128

#include "ogl2_buffers.h"

/*****************************************************************************
*****************************************************************************
				Category: HOGLVertexBufferCache Class
*****************************************************************************
*****************************************************************************/
// this is the buffer object data structure which caches the vertex and index buffers
// Stores an entire Tristrip or Polyedge
class HOGLIndexedBufferObject : public SwitchableIBO
{
private:

	GLuint m_index_buffer;
	std::vector<GLuint, CMO_Allocator<GLuint>> m_vertex_buffers;

	bool m_using_linepat_cs;
	GLuint m_cs_clip_vb;

public:
	HOGLIndexedBufferObject(int index, unsigned int flags) : SwitchableIBO(index, flags), 
		m_index_buffer(0),
		m_using_linepat_cs(ANYBIT(flags, H3DIBO_CS_LINE_PAT)),
		m_cs_clip_vb(0)
	{
		
	}
	~HOGLIndexedBufferObject()
	{
		HOGLData *ogldata = (HOGLData *)m_h3ddata;
		UNREFERENCED(ogldata);

		if (m_index_buffer != 0) {
			glDeleteBuffers(1, &m_index_buffer);
			HOOPS::WORLD->display_list_vram_usage -= m_index_size;
		}

		if (m_vertex_buffers.size() > 0) {
			glDeleteBuffers((GLsizei)m_vertex_buffers.size(), &m_vertex_buffers[0]);
			HOOPS::WORLD->display_list_vram_usage -= m_point_count * m_VF.get_size();
		}	

		if (m_cs_clip_vb != 0)
			glDeleteBuffers(1, &m_cs_clip_vb);

		FORCE_ELEMENT_BUFFER(ogldata, 0);
		FORCE_ARRAY_BUFFER(ogldata, 0);
	}

	virtual void CopyLocalVertexArraysToVRAM()
	{
		ASSERT(m_buffer_type == VRAM);
		ASSERT(m_vertex_buffers.size() == 0);

		HOGLData *ogldata = (HOGLData *)m_h3ddata;

		m_vertex_buffers.resize(m_stream_count);
		glGenBuffers(m_stream_count, &m_vertex_buffers[0]);
		for (int i = 0; i < m_stream_count; ++i) {
			ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[i]);
			glBufferData(GL_ARRAY_BUFFER, m_point_count * m_strides[i] * sizeof(float), m_vertex_data[i], GL_STATIC_DRAW);
		}

		if (m_using_linepat_cs) {
			glGenBuffers(1, &m_cs_clip_vb);
			ENSURE_SHADER_STORAGE_BUFFER(ogldata, m_cs_clip_vb);
			glBufferData(GL_SHADER_STORAGE_BUFFER, m_point_count * m_strides[0] * sizeof(float), null, GL_STATIC_DRAW);
		}

		HOOPS::WORLD->display_list_vram_usage += m_VF.get_size() * m_point_count;
	}

	virtual void CopyLocalIndexArrayToVRAM()
	{
		ASSERT(m_buffer_type == VRAM);
		ASSERT(m_index_buffer == 0);

		HOGLData *ogldata = (HOGLData *)m_h3ddata;

		glGenBuffers(1, &m_index_buffer);
		ENSURE_ELEMENT_BUFFER(ogldata, m_index_buffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_index_size, m_indices, GL_STATIC_DRAW);

		HOOPS::WORLD->display_list_vram_usage += m_index_size;
	}

	virtual void DownloadVertexArraysFromVRAM()
	{
		if (m_vertex_buffers.size() == 0)
			return;

		HOGLData *ogldata = (HOGLData *)m_h3ddata;

		for (int i = 0; i < m_stream_count; ++i) {
			ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[i]);
			size_t const stream_size = m_point_count * m_strides[i] * sizeof(float);
			auto ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, stream_size, GL_MAP_READ_BIT);
			COPY_MEMORY(ptr, stream_size, m_vertex_data[i]);
			glUnmapBuffer(GL_ARRAY_BUFFER);
		}		

		glDeleteBuffers((GLsizei)m_vertex_buffers.size(), &m_vertex_buffers[0]);
		m_vertex_buffers.clear();

		FORCE_ARRAY_BUFFER(ogldata, 0);

		HOOPS::WORLD->display_list_vram_usage -= m_VF.get_size() * m_point_count;
	}

	virtual void DownloadIndexArrayFromVRAM()
	{
		if (m_index_buffer == 0)
			return;

		HOGLData *ogldata = (HOGLData *)m_h3ddata;

		ENSURE_ELEMENT_BUFFER(ogldata, m_index_buffer);
		auto ptr = glMapBufferRange(GL_ELEMENT_ARRAY_BUFFER, 0, m_index_size, GL_MAP_READ_BIT);
		COPY_MEMORY(ptr, m_index_size, m_indices);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		glDeleteBuffers(1, &m_index_buffer);
		m_index_buffer = 0;

		FORCE_ELEMENT_BUFFER(ogldata, 0);

		HOOPS::WORLD->display_list_vram_usage -= m_index_size;
	}

	virtual bool CanDownloadFromVRAM() const
	{
		HOGLData * ogldata = (HOGLData *)m_h3ddata;

		if (ogldata->feature_level == OGL2_ES2)
			return false;

		return true;
	}	
	
	void EnsureBufferPointers(HOGLData * ogldata) const
	{
		HOGLShader * shader = (HOGLShader *) ogldata->cache->Shader;
		H3DVertexFormat const &	vf = m_VF;

		GLint program = ogldata->oglcache.current_program;
		HOGLVertexAttributes const & attributes = shader && program == shader->m_pProgram ? shader->m_vertex_attributes : HOGLVertexAttributes(ogldata, program);

		bool const interleaved = vf.get_interleaved();
		int const stride = interleaved ? vf.get_size() : 0;
		
		BEGIN_VERTEX_ATTRIBS(ogldata); 

		if (m_buffer_type == VRAM) {
			
			int stream_index = 0;
			intptr_t offset = 0;

			if (m_using_linepat_cs) {
				ENSURE_ARRAY_BUFFER(ogldata, m_cs_clip_vb);
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, vf.get_double_position() ? GL_DOUBLE : GL_FLOAT, GL_FALSE, stride, (GLvoid const *)offset);
				ASSERT(!interleaved);
				stream_index++;
			}
			else {
				ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[stream_index]);
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, vf.get_double_position() ? GL_DOUBLE : GL_FLOAT, GL_FALSE, stride, (GLvoid const *)offset);
				if (interleaved)
					offset += vf.get_double_position() ? 24 : 12;
				else
					stream_index++;
			}

			if (vf.get_normals()) {
				ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[stream_index]);
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid const *)offset);
				if (interleaved)
					offset += 12;
				else
					stream_index++;
			}
			if (vf.get_diffuse()) {
				ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[stream_index]);
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid const *)offset);
				if (interleaved)
					offset += 4;
				else
					stream_index++;
			}
			if (vf.get_specular()) {
				ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[stream_index]);
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLvoid const *)offset);
				if (interleaved)
					offset += 4;
				else
					stream_index++;
			}
			for (int texcoord = 0; texcoord < vf.get_tex_count(); texcoord++) {
				ENSURE_ARRAY_BUFFER(ogldata, m_vertex_buffers[stream_index]);
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], vf.get_tex_size(texcoord), GL_FLOAT, GL_FALSE, stride, (GLvoid const *)offset);
				if (interleaved)
					offset += vf.get_tex_size(texcoord) * sizeof(float);
				else
					stream_index++;
			}
		}

		// here we'll need to use the RingBuffer for storage because client vertex arrays are not allowed in Core Profile.
		else if (ogldata->feature_level == OGL2_CoreProfile) {
			ASSERT(0);
		}

		else {

			int stream_index = 0;
			intptr_t offset = 0;

			ENSURE_ARRAY_BUFFER(ogldata, 0);
			SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, vf.get_double_position() ? GL_DOUBLE : GL_FLOAT, GL_FALSE, stride, (GLubyte const *)m_vertex_data[stream_index] + offset);
			if (interleaved)
				offset += vf.get_double_position() ? 24 : 12;
			else
				stream_index++;

			if (vf.get_normals()) {				
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, stride, (GLubyte const *)m_vertex_data[stream_index] + offset);
				if (interleaved)
					offset += 12;
				else
					stream_index++;
			}
			if (vf.get_diffuse()) {				
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLubyte const *)m_vertex_data[stream_index] + offset);
				if (interleaved)
					offset += 4;
				else
					stream_index++;
			}
			if (vf.get_specular()) {				
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, stride, (GLubyte const *)m_vertex_data[stream_index] + offset);
				if (interleaved)
					offset += 4;
				else
					stream_index++;
			}
			for (int texcoord = 0; texcoord < vf.get_tex_count(); texcoord++) {				
				SET_INTERLEAVED_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], vf.get_tex_size(texcoord), GL_FLOAT, GL_FALSE, stride, (GLubyte const *)m_vertex_data[stream_index] + offset);
				if (interleaved)
					offset += vf.get_tex_size(texcoord) * sizeof(float);
				else
					stream_index++;
			}
		}

		END_VERTEX_ATTRIBS(ogldata);
	}

	void Draw(H3DData * h3ddata, Test * cull_results = null, int instances = 1) const
	{
		HOGLData *			ogldata = (HOGLData *) h3ddata;
		Display_Context *	dc = (Display_Context *)h3ddata->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;
		
		GLbyte const * index_ptr;

		if (m_buffer_type == VRAM) {
			ENSURE_ELEMENT_BUFFER(ogldata, m_index_buffer);
			index_ptr = null;
		}		
		
		else if (ogldata->feature_level == OGL2_CoreProfile) {
			ASSERT(0);
			index_ptr = null;
		}

		else {
			ENSURE_ELEMENT_BUFFER(ogldata, 0);
			index_ptr = (GLbyte const *)m_indices;
		}
		
		EnsureBufferPointers(ogldata);

		GLenum	index_type = m_format == H3DFMT_INDEX16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT;

		if (cull_results == null || !m_primitive_count || m_geom_offsets == null) {

			if (cull_results != null && CULLED(cull_results[m_geom_first]))
				return;

			if (m_primitive_count) {
				switch (m_type) {
					case H3DPT_TRIANGLELIST:
					case H3DPT_TRIANGLESTRIP:
						h3ddata->dc->stats->dl_triangles_3d += m_primitive_count;
						break;
					case H3DPT_LINELIST:
					case H3DPT_LINESTRIP:
						h3ddata->dc->stats->dl_lines_3d += m_primitive_count;
						break;
					case H3DPT_POINTLIST:
						h3ddata->dc->stats->dl_vertices_3d += m_primitive_count;
						break;
					default:
						ASSERT(0);
						break;
				}

				if (ogldata->feature_level == OGL2_ES2)
					glDrawElements(oglprimitive(m_type), prim_to_indices(m_type, m_primitive_count), index_type, index_ptr);
				else
					glDrawRangeElements(oglprimitive(m_type), 0, m_point_count - 1, prim_to_indices(m_type, m_primitive_count), index_type, index_ptr);
			}
			else {
				GLenum type = oglprimitive(m_type);
				if (instances > 1)
					glDrawArraysInstanced(type, 0, m_point_count, instances);
				else
					glDrawArrays(type,	0, m_point_count);
			}
		}
		else {

			int total_primitive_count = 0;
			switch (m_type) {
			case H3DPT_TRIANGLELIST:
				total_primitive_count = m_primitive_count * 3;
				break;
			case H3DPT_TRIANGLESTRIP:
				total_primitive_count = m_primitive_count + 2;
				break;
			case H3DPT_LINELIST:
				total_primitive_count = m_primitive_count * 2;
				break;
			case H3DPT_LINESTRIP:
				total_primitive_count = m_primitive_count + 1;
				break;
			default:
				ASSERT(0);
				break;
			}

			int	index_stride = (m_format == H3DFMT_INDEX32) ? 4 : 2;
			
			int start = 0;
			int last = 0;

			for (;start < m_geom_count;) {

				while (CULLED(cull_results[start + m_geom_first])) {
					if (++start >= m_geom_count)
						return;
				}

				last = start;

				while (!CULLED(cull_results[last + m_geom_first])) {
					if (++last >= m_geom_count)
						break;
				}

				int len;

				if (last == m_geom_count)
					len = total_primitive_count - m_geom_offsets[start];
				else
					len = m_geom_offsets[last] - m_geom_offsets[start];

				if (len > 0) {
					int primitive_count = 0;
					switch (m_type) {
					case H3DPT_TRIANGLELIST:
						primitive_count = len / 3;
						DC_STATS(dl_triangles_3d) += primitive_count;
						break;
					case H3DPT_TRIANGLESTRIP:
						primitive_count = len - 2;
						DC_STATS(dl_triangles_3d) += primitive_count;
						break;
					case H3DPT_LINELIST:
						primitive_count = len / 2;
						DC_STATS(dl_lines_3d) += primitive_count;
						break;
					case H3DPT_LINESTRIP:
						primitive_count = len - 1;
						DC_STATS(dl_lines_3d) += primitive_count;
						break;
					default:
						ASSERT(0);
						break;
					}
	
					glDrawElements(oglprimitive(m_type), prim_to_indices(m_type, primitive_count), index_type, index_ptr + m_geom_offsets[start] * index_stride);
				}

				start = last;
			}
		}

		GL_CHECK_ERROR();		
	}

	void Draw_Decimated(H3DData *h3ddata, float decimation = 1.0f) const
	{
		HOGLData *ogldata = (HOGLData *) h3ddata;
		Display_Context *	dc = (Display_Context *)h3ddata->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		UNREFERENCED(ogldata);

		GLbyte const * index_ptr;

		if (m_buffer_type == VRAM) {
			ENSURE_ELEMENT_BUFFER(ogldata, m_index_buffer);
			index_ptr = null;
		}

		else if (ogldata->feature_level == OGL2_CoreProfile) {
			index_ptr = null;
			ASSERT(0);
		}

		else {
			ENSURE_ELEMENT_BUFFER(ogldata, 0);
			index_ptr = (GLbyte const *)m_indices;
		}
		
		EnsureBufferPointers(ogldata);

		if (m_index_size != 0) {
			int decimated_prim_count = (int) (m_primitive_count * decimation);
			if (decimation < 1) {
				// Assume that draw_decimated with indices is for + and x markers,
				// and with 2 primitives per marker, draw even number of primitives
				decimated_prim_count &= 0xffffffe;
			}

			glDrawElements(oglprimitive(m_type), prim_to_indices(m_type, decimated_prim_count), m_format == H3DFMT_INDEX16 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, index_ptr);
		}
		else {
			glDrawArrays(oglprimitive(m_type), 0, (GLsizei)(m_point_count * decimation));
		}

		GL_CHECK_ERROR();
	}

	void Draw_CSLinePattern(H3DData * h3ddata, float u_scale, float v_scale, float const * matMVP, float const * matScreen, Test * cull_results, int instances)
	{
		HOGLData *ogldata = (HOGLData *)h3ddata;

		// Manually sync this with N in shader source.
		int const GROUP_W = 256;
		int const DISPATCH_SIZE = (int)align(m_point_count / 2, GROUP_W) / GROUP_W;
		ASSERT(DISPATCH_SIZE < 1024);

		if (!ensure_cs_line_pattern_resources(ogldata))
			return;

		GLuint cachedShader = ogldata->oglcache.current_program;

		// clip pass
		{
			// Extract frustum planes: left, right, bottom, top, near, far
			Plane planes[6];
			{
				float col1[] = { matMVP[0], matMVP[4], matMVP[8], matMVP[12] };
				float col2[] = { matMVP[1], matMVP[5], matMVP[9], matMVP[13] };
				float col3[] = { matMVP[2], matMVP[6], matMVP[10], matMVP[14] };
				float col4[] = { matMVP[3], matMVP[7], matMVP[11], matMVP[15] };
				planes[0] = Plane(Vector(col4[0] + col1[0], col4[1] + col1[1], col4[2] + col1[2]), col4[3] + col1[3]);
				planes[1] = Plane(Vector(col4[0] - col1[0], col4[1] - col1[1], col4[2] - col1[2]), col4[3] - col1[3]);
				planes[2] = Plane(Vector(col4[0] + col2[0], col4[1] + col2[1], col4[2] + col2[2]), col4[3] + col2[3]);
				planes[3] = Plane(Vector(col4[0] - col2[0], col4[1] - col2[1], col4[2] - col2[2]), col4[3] - col2[3]);
				planes[4] = Plane(Vector(col3[0], col3[1], col3[2]), col3[3]);
				planes[5] = Plane(Vector(col4[0] - col3[0], col4[1] - col3[1], col4[2] - col3[2]), col4[3] - col3[3]);
				for (int i = 0; i < 6; i++)
					planes[i].Normalize();
			}

			ENSURE_UNIFORM_BUFFER(ogldata, ogldata->linepat_cs.constants);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*4*6, planes);

			glBindBufferBase(GL_UNIFORM_BUFFER, 0, ogldata->linepat_cs.constants);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vertex_buffers[0]);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_cs_clip_vb);

			ENSURE_PROGRAM(ogldata, ogldata->linepat_cs.clip_shader);
			glDispatchCompute(DISPATCH_SIZE, 1, 1);
		}

 		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// compute lengths pass
		{
			float		tmp[18];
			memcpy(tmp, matScreen, sizeof(float)*16);
			tmp[16] = u_scale;
			tmp[17] = v_scale;
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*18, tmp);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_cs_clip_vb);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vertex_buffers[m_stream_count-1]);

			ENSURE_PROGRAM(ogldata, ogldata->linepat_cs.compute_lengths_shader);
			glDispatchCompute(DISPATCH_SIZE, 1, 1);
		}

		// group sum pass
		if (DISPATCH_SIZE > 1)
		{
			//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

			// Only need lengths buffer
			glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);

  			ENSURE_PROGRAM(ogldata, ogldata->linepat_cs.sum_shader);
  			glDispatchCompute(DISPATCH_SIZE, 1, 1);
		}
	
 		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);

		// Cleanup
 		FORCE_UNIFORM_BUFFER(ogldata, 0);
 		FORCE_SHADER_STORAGE_BUFFER(ogldata, 0);
 		FORCE_PROGRAM(ogldata, cachedShader);

		// Draw from clip buffer
		Draw(h3ddata, cull_results, instances);
	}

	void Draw_CSEdgePattern(H3DData * h3ddata, float u_scale, float v_scale, float const * matMVP, float const * matScreen, Test * cull_results, int instances)
	{
		HOGLData *ogldata = (HOGLData *)h3ddata;

		// Manually sync this with N in shader source.
		int const GROUP_W = 256;
		int const DISPATCH_SIZE = (int)align(m_point_count / 2, GROUP_W) / GROUP_W;
		ASSERT(DISPATCH_SIZE < 1024);

		if (!ensure_cs_edge_pattern_resources(ogldata))
			return;

		GLuint cachedShader = ogldata->oglcache.current_program;

		// clip pass
		{
			// Extract frustum planes: left, right, bottom, top, near, far
			Plane planes[6];
			{
				float col1[] = { matMVP[0], matMVP[4], matMVP[8], matMVP[12] };
				float col2[] = { matMVP[1], matMVP[5], matMVP[9], matMVP[13] };
				float col3[] = { matMVP[2], matMVP[6], matMVP[10], matMVP[14] };
				float col4[] = { matMVP[3], matMVP[7], matMVP[11], matMVP[15] };
				planes[0] = Plane(Vector(col4[0] + col1[0], col4[1] + col1[1], col4[2] + col1[2]), col4[3] + col1[3]);
				planes[1] = Plane(Vector(col4[0] - col1[0], col4[1] - col1[1], col4[2] - col1[2]), col4[3] - col1[3]);
				planes[2] = Plane(Vector(col4[0] + col2[0], col4[1] + col2[1], col4[2] + col2[2]), col4[3] + col2[3]);
				planes[3] = Plane(Vector(col4[0] - col2[0], col4[1] - col2[1], col4[2] - col2[2]), col4[3] - col2[3]);
				planes[4] = Plane(Vector(col3[0], col3[1], col3[2]), col3[3]);
				planes[5] = Plane(Vector(col4[0] - col3[0], col4[1] - col3[1], col4[2] - col3[2]), col4[3] - col3[3]);
				for (int i = 0; i < 6; i++)
					planes[i].Normalize();
			}

			ENSURE_UNIFORM_BUFFER(ogldata, ogldata->edgepat_cs.constants);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*4*6, planes);

			glBindBufferBase(GL_UNIFORM_BUFFER, 0, ogldata->edgepat_cs.constants);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_vertex_buffers[0]);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_cs_clip_vb);

			ENSURE_PROGRAM(ogldata, ogldata->edgepat_cs.clip_shader);
			glDispatchCompute(DISPATCH_SIZE, 1, 1);
		}

 		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// compute lengths pass
		{
			float		tmp[18];
			memcpy(tmp, matScreen, sizeof(float)*16);
			tmp[16] = u_scale;
			tmp[17] = v_scale;
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*18, tmp);

			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_cs_clip_vb);
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, m_vertex_buffers[m_stream_count-1]);

			ENSURE_PROGRAM(ogldata, ogldata->edgepat_cs.compute_lengths_shader);
			glDispatchCompute(DISPATCH_SIZE, 1, 1);
		}

 		//glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		glBindBufferBase(GL_UNIFORM_BUFFER, 0, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, 0);
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, 0);

		// Cleanup
 		FORCE_UNIFORM_BUFFER(ogldata, 0);
 		FORCE_SHADER_STORAGE_BUFFER(ogldata, 0);
 		FORCE_PROGRAM(ogldata, cachedShader);

		// Draw from clip buffer
		Draw(h3ddata, cull_results, instances);
	}
};


/*****************************************************************************
*****************************************************************************
				Category: HOGLVertexBufferCache Class
*****************************************************************************
*****************************************************************************/

// this is the global vertex buffer cache, being used for view dependent geometry
class HOGLVertexBufferCache : public IVertexBufferCache
{
public:
	HOGLVertexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage += HOGL_PER_OBJECT_PENALTY;
		m_stream_count = 0;
		m_vertex_data = null;
		m_nMaxPoints = 0;
	}

	~HOGLVertexBufferCache()
	{
		for (int i = 0; i < m_stream_count; i++)
			FREE_ARRAY(m_vertex_data[i], m_nMaxPoints*m_strides[i], float);
		FREE_ARRAY(m_vertex_data, m_stream_count, float*);
		FREE_ARRAY(m_strides, m_stream_count, unsigned int);

		HOOPS::WORLD->display_list_vram_usage -= HOGL_PER_OBJECT_PENALTY;
		HOOPS::WORLD->display_list_vram_usage -= (m_nMaxPoints * m_VF.get_size());
	}

	bool CreateVertexBuffer(H3DData *h3ddata, unsigned int max_points, 
		H3DVertexFormat VF)
	{
		HOGLData *ogldata = m_ogldata = (HOGLData *) h3ddata;
		UNREFERENCED(ogldata);

		m_nMaxPoints		= max_points;
		m_VF				= VF;
		m_stream_count		= VF.get_stream_count();
		ALLOC_ARRAY(m_strides, m_stream_count, unsigned int);
		VF.get_stream_strides(m_strides);

		ALLOC_ARRAY(m_vertex_data, m_stream_count, float*);
		for (int i = 0; i < m_stream_count; i++) {
			ALLOC_ARRAY(m_vertex_data[i], m_nMaxPoints*m_strides[i], float);
		}

		HOOPS::WORLD->display_list_vram_usage += (m_nMaxPoints * m_VF.get_size());
		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	bool Reset()
	{
		//TODO: H_SAFE_RELEASE(m_pVertexBuffer);
		HOOPS::WORLD->display_list_vram_usage -= (m_nMaxPoints * m_VF.get_size());

		for (int i = 0; i < m_stream_count; i++) {
			FREE_ARRAY(m_vertex_data, m_nMaxPoints*m_strides[i], float);
		}

		m_nMaxPoints	= 0;
		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	bool Discard()
	{
		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	bool PushDataToBuffer(Matrix matrix = null, int length = -1)
	{
		bool result = true;
		int vertices_to_copy = (length >= 0) ? length : m_locked_vertex_size;
		vertices_to_copy -= m_push_data_start;
		if (vertices_to_copy <= 0)
			return true;

		int offset = m_push_data_start*3;
		if (matrix) {
			matrix->Transform(vertices_to_copy, (Point*)(m_vertex_data[0]+offset), (Point*)(m_vertex_data[0]+offset));
			if (m_VF.get_normals())
				matrix->Transform(vertices_to_copy, (Vector*)(m_vertex_data[1]+offset), (Vector*)(m_vertex_data[1]+offset));
		}

		m_push_data_start += vertices_to_copy;
		return result;
	}

	bool Lock(unsigned int count, H3DVertexBuffer *pv)
	{
		ASSERT(count <= m_nMaxPoints);

		m_push_data_start = 0;
		m_locked = true;

		// Determine the size of data to be moved into the vertex buffer.
		unsigned int data_size = count * m_strides[0];
		m_locked_vertex_size = data_size;

		// Lock the vertex buffer.
		pv->Init(m_vertex_data, &m_VF);
		return true;
	}

	bool Unlock(Matrix matrix)
	{
		PushDataToBuffer(matrix);

		m_locked = false;

		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	int StartVertex() const
	{
		return m_nCurVertexOffset;
	}

	bool DrawRepeatableWithAttributes(H3DData *h3ddata , HOGLVertexAttributes const & attributes, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		GLenum const prim_type = oglprimitive(PrimitiveType);
		GLsizei const num_indices = prim_to_indices(PrimitiveType, PrimitiveCount);
		HOGLData *ogldata = m_ogldata = (HOGLData *) h3ddata;

		ENSURE_ARRAY_BUFFER(ogldata, 0);

		BEGIN_VERTEX_ATTRIBS(ogldata);
		{
			int stream_index = 0;
			SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, m_VF.get_double_position() ? GL_DOUBLE : GL_FLOAT, GL_FALSE, (char *) m_vertex_data[stream_index]);
			stream_index++;
			if (m_VF.get_normals()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, (char *) m_vertex_data[stream_index]);
				stream_index++;
			}
			if (m_VF.get_diffuse()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) m_vertex_data[stream_index]);
				stream_index++;
			}
			if (m_VF.get_specular()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) m_vertex_data[stream_index]);
				stream_index++;
			}
			if (m_VF.get_tex_count() > 0) {				
				for (int texcoord = 0; texcoord < m_VF.get_tex_count(); texcoord++) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], m_VF.get_tex_size(texcoord), GL_FLOAT, GL_FALSE, (char *) m_vertex_data[stream_index]);
					stream_index++;
				}
			}
		}
		END_VERTEX_ATTRIBS(ogldata);

		if (instances > 1)
			glDrawArraysInstanced(prim_type, 0, num_indices, instances);
		else
			glDrawArrays(prim_type, 0, num_indices);

		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	virtual bool DrawWithAttributes(H3DData *h3ddata, HOGLVertexAttributes const & attributes, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		bool result = DrawRepeatableWithAttributes(h3ddata, attributes, PrimitiveType, PrimitiveCount, instances);
		DrawComplete();
		return result;
	}

	bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount, instances);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		HOGLShader *shader = (HOGLShader *)h3ddata->cache->Shader;
		HOGLVertexAttributes const & attributes = shader->m_vertex_attributes;
		return DrawRepeatableWithAttributes(h3ddata, attributes, PrimitiveType, PrimitiveCount, instances);
	}

	void DrawComplete()
	{
		m_nCurVertexOffset = m_nNextVertexOffset;
	}

	HOGLData *m_ogldata;

	bool m_locked;
};



// this is the global vertex buffer cache, being used for view dependent geometry
class HOGLGenericVertexBufferCache : public H3DGenericVertexBufferCache
{
public:
	HOGLGenericVertexBufferCache(H3DData *h3ddata) :
		H3DGenericVertexBufferCache(h3ddata)
	{
		HOOPS::WORLD->display_list_vram_usage += HOGL_PER_OBJECT_PENALTY;
	}

	~HOGLGenericVertexBufferCache()
	{
		for (int i = 0; i < 12; i++) {
			if (m_vertex_data[i])
				FREE_ARRAY(m_vertex_data[i], m_nMaxPoints*m_strides[i], float);
		}

		HOOPS::WORLD->display_list_vram_usage -= HOGL_PER_OBJECT_PENALTY;
	}

	bool Lock(H3DVertexFormat const &vf, unsigned int count, H3DVertexBuffer *pv)
	{
		UNREFERENCED(count);
		float *m_locked_data[12];

		m_VF = vf;
		m_VF.get_stream_strides(m_strides);
		m_locked = true;

		// Lock the vertex buffer.
		int i = 0;
		bool has_stream[12];
		SET_MEMORY(has_stream, sizeof(has_stream), 0);
		has_stream[i++] = m_VF.get_position() || m_VF.get_double_position();
		has_stream[i++] = m_VF.get_normals();
		has_stream[i++] = m_VF.get_diffuse();
		has_stream[i++] = m_VF.get_specular();
		for (int j = 0; j < m_VF.get_tex_count(); j++)
			has_stream[i++] = true;

		int stream = 0;
		for (i = 0; i < 12; i++) {
			if (has_stream[i]) {
				if (!m_vertex_data[i])
					ALLOC_ARRAY(m_vertex_data[i], m_nMaxPoints*m_strides[i], float);
				m_locked_data[stream] = m_vertex_data[i];
				stream++;
			}
		}

		pv->Init(m_locked_data, &m_VF);
		return true;
	}

	bool Unlock()
	{
		m_locked = false;
		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	int StartVertex()
	{
		return m_curr_offset;
	}

	bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount)
	{
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount)
	{
		GLenum const prim_type = oglprimitive(PrimitiveType);
		GLsizei const num_indices = prim_to_indices(PrimitiveType, PrimitiveCount);
		HOGLData *ogldata = m_ogldata = (HOGLData *) h3ddata;
		HOGLShader *shader = (HOGLShader *)ogldata->cache->Shader;

		HOGLVertexAttributes const & attributes = shader->m_vertex_attributes;

		ENSURE_ARRAY_BUFFER(ogldata, 0);

		BEGIN_VERTEX_ATTRIBS(ogldata);
		{
			if (m_VF.get_position())
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_FLOAT, GL_FALSE, (char *) m_vertex_data[0]);
			if (m_VF.get_double_position())
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_DOUBLE, GL_FALSE, (char *)m_vertex_data[0]);
			if (m_VF.get_normals())
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, (char *) m_vertex_data[1] );
			if (m_VF.get_diffuse())
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) m_vertex_data[2]);
			if (m_VF.get_specular())
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) m_vertex_data[3]);
			if (m_VF.get_tex_count() > 0) {				
				for (int texcoord = 0; texcoord < m_VF.get_tex_count(); texcoord++) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], m_VF.get_tex_size(texcoord), GL_FLOAT, GL_FALSE, (char *) m_vertex_data[4 + texcoord]);
				}
			}
		}
		END_VERTEX_ATTRIBS(ogldata);

		glDrawArrays(prim_type, 0, num_indices);
		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	void DrawComplete()
	{
	}

	HOGLData *m_ogldata;

	bool m_locked;
};

/*****************************************************************************
*****************************************************************************
					Category: HOGLIndexBufferCache class
*****************************************************************************
*****************************************************************************/

class HOGLIndexBufferCache : public H3DIndexBufferCache
{
public:
	HOGLIndexBufferCache()
	{
		m_pIndexBuffer = 0;
		m_pVertexBufferCache = null;
		m_nCurIndexData = 0;
		m_nNextIndexData = 0;
		m_locked = false;
	}

	~HOGLIndexBufferCache()
	{
		HOGLData *ogldata = m_ogldata;
		UNREFERENCED(ogldata);

		if (m_data)
			FREE_ARRAY(m_data, IB_CACHE_MAX_SIZE, short);
	}

	bool CreateIndexBuffer(H3DData *h3ddata)
	{
		HOGLData *ogldata = m_ogldata = (HOGLData *) h3ddata;
		UNREFERENCED(ogldata);
		ALLOC_ARRAY(m_data, IB_CACHE_MAX_SIZE, short);
		return true;
	}


	bool Lock(int count, short ** ppbData)
	{
		if (m_locked)
			return false;

		m_locked_count = count;

		if (m_nCurIndexData + count >= IB_CACHE_MAX_SIZE) {
			m_nCurIndexData = 0;
		}

		m_nNextIndexData = m_nCurIndexData + count;

#ifdef _DEBUG
		m_data[m_nNextIndexData - 1] = -1;	// for debugging purposes; allows us to see where end of buffer is
#endif

		// Lock the vertex buffer.
		*ppbData = m_data + m_nCurIndexData;
		m_locked = true;
		
		return true;
	}

	bool Unlock()
	{
		HOGLData *ogldata = (HOGLData *) m_ogldata;
		UNREFERENCED(ogldata);

		if (!m_locked)
			return false;
		m_locked = false;
		return true;
	}

	bool Draw(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, unsigned int VertexCount, unsigned int PrimitiveCount)
	{
		UNREFERENCED(VertexCount);
		HOGLData *		ogldata = (HOGLData *) h3ddata;
		HOGLShader *	shader = (HOGLShader *) ogldata->cache->Shader;

		ENSURE_ARRAY_BUFFER(ogldata, 0);
		ENSURE_ELEMENT_BUFFER(ogldata, m_pIndexBuffer);

		HOGLVertexAttributes const & attributes = shader->m_vertex_attributes;

		if (m_pVertexBufferCache) {
			H3DVertexFormat const * vf = &m_pVertexBufferCache->m_VF;
			HOGLVertexBufferCache * vb = (HOGLVertexBufferCache*) m_pVertexBufferCache;			

			BEGIN_VERTEX_ATTRIBS(ogldata);
			{
				int stream_index = 0;
				if (vf->get_position()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_FLOAT, GL_FALSE, (char *)vb->m_vertex_data[stream_index]);
					stream_index++;
				}
				if (vf->get_double_position()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_DOUBLE, GL_FALSE, (char *)vb->m_vertex_data[stream_index]);
					stream_index++;
				}
				if (vf->get_normals()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, (char *) vb->m_vertex_data[stream_index]);
					stream_index++;
				}
				if (vf->get_diffuse()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) vb->m_vertex_data[stream_index]);
					stream_index++;
				}
				if (vf->get_specular()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) vb->m_vertex_data[stream_index]);
					stream_index++;
				}				
				for (int texcoord = 0; texcoord < vf->get_tex_count(); texcoord++) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], vf->get_tex_size(texcoord), GL_FLOAT, GL_FALSE, (char *) vb->m_vertex_data[stream_index]);
					stream_index++;
				}
			}
			END_VERTEX_ATTRIBS(ogldata);
		}
		else {
			HOGLGenericVertexBufferCache *vb = (HOGLGenericVertexBufferCache*) m_pGenericVertexBufferCache;	
			H3DVertexFormat *vf = &m_pGenericVertexBufferCache->m_VF;

			BEGIN_VERTEX_ATTRIBS(ogldata);
			{
				if (vf->get_position())
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_FLOAT, GL_FALSE, (char *) vb->m_vertex_data[0]);
				if (vf->get_double_position())
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_DOUBLE, GL_FALSE, (char *)vb->m_vertex_data[0]);
				if (vf->get_normals())
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, (char *) vb->m_vertex_data[1]);	
				if (vf->get_diffuse())
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) vb->m_vertex_data[2]);	
				if (vf->get_specular())
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, (char *) vb->m_vertex_data[3]);					
				for (int texcoord = 0; texcoord < vf->get_tex_count(); texcoord++) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], vf->get_tex_size(texcoord), GL_FLOAT, GL_FALSE, (char *) vb->m_vertex_data[4 + texcoord]);
				}
			}
			END_VERTEX_ATTRIBS(ogldata);
		}
		short const * index_ptr = m_data;
		
		if (ogldata->feature_level == OGL2_ES2) {
			glDrawElements(oglprimitive(PrimitiveType), prim_to_indices(PrimitiveType, PrimitiveCount), GL_UNSIGNED_SHORT, index_ptr);
		}
		
		else {
			glDrawRangeElements(oglprimitive(PrimitiveType), m_nCurIndexData, m_nCurIndexData + VertexCount, prim_to_indices(PrimitiveType, PrimitiveCount), GL_UNSIGNED_SHORT, index_ptr);
		}

		GL_CHECK_ERROR();
		
		return true;
	}

	void DrawComplete()
	{
		if (m_pVertexBufferCache)
			m_pVertexBufferCache->DrawComplete();
		else
			m_pGenericVertexBufferCache->DrawComplete();
	}

	short *			m_data;
	bool			m_locked;
	int				m_locked_count;

	HOGLData *		m_ogldata;
	GLuint			m_pIndexBuffer;
	unsigned int	m_nCurIndexData;
	unsigned int	m_nNextIndexData;
};

typedef std::vector<HOGLFrameBufferObject*, POOL_Allocator<HOGLFrameBufferObject*>> FboVector;

/*****************************************************************************
*****************************************************************************
							HOGLTexture Class
*****************************************************************************
*****************************************************************************/
class HOGLRenderTarget : public H3DRenderTarget
{
private:
	GLuint m_id;
	GLuint m_id2;			// used if we need both depth and stencil, but can't pack into a single target	
	FboVector m_parent_framebuffers;

public:
	HOGLRenderTarget(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int usage,		// H3DTEXUSAGE
		H3DFORMAT format,
		int msaa = 0, 
		GLuint id_in = -1) : H3DRenderTarget(h3ddata, width, height, usage, format, msaa), 
			m_id(0), m_id2(0), m_parent_framebuffers(h3ddata->dc->memory_pool())
	{
		ASSERT(width > 0);
		ASSERT(height > 0);
		
		HOGLData *ogldata = (HOGLData *) h3ddata;
		H3D_TRACE (ogldata, "HOGLRenderTarget");

		GLint gl_usage = 0, gl_usage2 = 0;
		msaa = Min(msaa, ogldata->max_msaa_samples);

		// create and allocate memory for buffer
		if (id_in == -1)
			glGenRenderbuffers(1, &m_id);
		else {
			m_id = id_in;
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
			return;
		}
		glBindRenderbuffer(GL_RENDERBUFFER, m_id);


		switch (usage) {
			case H3DTEXUSAGE_DEPTHSTENCIL:
				if (GLEW_EXT_packed_depth_stencil && !ogldata->bad_packed_depth_stencil) {
					gl_usage = GL_DEPTH24_STENCIL8_EXT;
				}
				else {
					glGenRenderbuffers(1, &m_id2);	

#ifdef OGL_ES
					if (GLEW_NV_depth_nonlinear)
						gl_usage = GL_DEPTH_COMPONENT16_NONLINEAR_NV;
					else
						gl_usage = GL_DEPTH_COMPONENT16;
					gl_usage2 = GL_STENCIL_INDEX8;
#else
					gl_usage = GL_DEPTH_COMPONENT;
					gl_usage2 = GL_STENCIL_INDEX;
#endif
				}
				break;
			default: {
				Name_List const *	name_list2;

				local const Name_Const	rgba16f("rgba16f");

				gl_usage = GL_RGBA;
				if ((name_list2 = h3ddata->dc->options.output_format2) != null) do {
					if (name_list2->name == rgba16f) {
						gl_usage = GL_RGBA16F_ARB;
						break;
					}
				} while ((name_list2 = name_list2->next) != null);
#ifdef OGL_ES
				gl_usage = GL_RGBA8_OES;
#endif				
				gl_usage2 = 0;
				break;
			}
		}	

		if (gl_usage && id_in == -1) {
			if (msaa > 1)
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, gl_usage, width, height);
			else
				glRenderbufferStorage(GL_RENDERBUFFER, gl_usage, width, height);

		}
		if (gl_usage2) {
			glBindRenderbuffer(GL_RENDERBUFFER, m_id2);
			if (msaa > 1)
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, msaa, gl_usage2, width, height);
			else
				glRenderbufferStorage(GL_RENDERBUFFER, gl_usage2, width, height);
			glBindRenderbuffer(GL_RENDERBUFFER, m_id);
		}
		
		GL_CHECK_ERROR();
	}

	~HOGLRenderTarget();	

	void OnAttachedToFrameBuffer(HOGLFrameBufferObject * framebuffer);	
	void OnDetachedFromFrameBuffer(HOGLFrameBufferObject * framebuffer);	

	GLuint GetBuffer() {return m_id;}
	GLuint GetBuffer2() {return m_id2;}
};


class HOGLTexture2D : public H3DTexture
{
private:
	GLuint m_id;
	GLuint m_pbo; // for dynamic usage case
	GLuint m_format, m_internal_format;
	bool m_locked;
	unsigned int m_level_locked;
	unsigned char *m_data;
	GLuint m_data_type;	

	unsigned int m_size_allocated;

	bool m_regenerate_mipmaps;
	bool m_texture_updated;
	bool m_compressed;
	int m_compressed_size;

	H3DRect m_locked_rect;
	bool m_sub_rect;
	
	FboVector m_parent_framebuffers;

public:
	HOGLTexture2D(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :	H3DTexture(h3ddata, width, height, levels, usage, format),
			m_id(0), m_pbo(0), m_parent_framebuffers(h3ddata->dc->memory_pool())
	{
		H3D_TRACE (h3ddata, "HOGLTexture2D::HOGLTexture2D");
		HOGLData *ogldata = (HOGLData*) h3ddata;
	
		UNREFERENCED(levels);		

		// OpenGL defaults
		m_min_filter = H3DTEXF_POINT;
		m_mag_filter = H3DTEXF_LINEAR;
		m_mip_filter = H3DTEXF_NONE;
		m_texture_wrap_s = H3DTADDRESS_WRAP;
		m_texture_wrap_t = H3DTADDRESS_WRAP;
		//format and internal_format are to be determined below
		m_format = 0; 
		m_internal_format = 0;
		m_data_type = GL_UNSIGNED_BYTE;

		m_h3ddata = h3ddata;
		m_data = null;
		m_locked = false;
		m_regenerate_mipmaps = false;
		m_texture_updated = false;
		glGenTextures (1, &m_id);

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET | H3DTEXUSAGE_DEPTHSTENCIL | H3DTEXUSAGE_DYNAMIC)) {
			GLint saved_texture = 0;

			glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);
			glBindTexture(GL_TEXTURE_2D, m_id);
			
			m_bytes_per_pixel = 4;

			switch (format) {
				case H3DFMT_DXT1:
				case H3DFMT_DXT3:
				case H3DFMT_DXT5:
					ASSERT(0); // these should never be used as render targets
					break;
				case H3DFMT_D24S8:
					m_internal_format = GL_DEPTH24_STENCIL8_EXT ;
					m_format = GL_DEPTH_STENCIL_EXT ;
					m_data_type = GL_UNSIGNED_INT_24_8_EXT ;
					break;
				case H3DFMT_L8:
					m_format = GL_LUMINANCE;
					m_internal_format = 1;
					m_bytes_per_pixel = 1;
					break;
				case H3DFMT_R8G8B8:
					m_format = GL_RGB;        
                    m_internal_format = ogldata->feature_level == OGL2_Desktop ? GL_RGB8 : GL_RGB;
					m_bytes_per_pixel = 3;
					break;
				case H3DFMT_R32F:				
					ASSERT(ogldata->feature_level != OGL2_ES2);
					m_internal_format = GL_R32F;			
					m_format = GL_RED;
					m_data_type = GL_FLOAT;				
					break;
				case H3DFMT_A8R8G8B8:
				default:
					m_format = GL_RGBA;					
					m_internal_format = ogldata->feature_level == OGL2_Desktop ? GL_RGBA8 : GL_RGBA;					
					break;
			}

			glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format, width, height, 0, m_format, m_data_type, null);

			GL_CHECK_ERROR();
			// Must use these settings to avoid having to gen mipmaps (which is very slow)
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			m_min_filter = H3DTEXF_POINT;
			m_mag_filter = H3DTEXF_POINT;
			m_mip_filter = H3DTEXF_NONE;
			m_texture_wrap_s = H3DTADDRESS_CLAMP;
			m_texture_wrap_t = H3DTADDRESS_CLAMP;
			glBindTexture(GL_TEXTURE_2D, saved_texture);		
		}
		else {
			GLint saved_texture = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);
			glBindTexture(GL_TEXTURE_2D, m_id);

			switch (format) {
				case H3DFMT_L8:
					m_format = GL_LUMINANCE;
					m_internal_format = 1;
					m_bytes_per_pixel = 1;
					break;
				case H3DFMT_R8G8B8:
					if (h3ddata->supports_texture_rgb24) {
						m_format = GL_RGB;
						m_internal_format = GL_RGB;
						m_bytes_per_pixel = 3;
						break;
					}
				case H3DFMT_A8R8G8B8:
					m_format = GL_RGBA;
					m_internal_format = GL_RGBA;
					m_bytes_per_pixel = 4;
					break;
				case H3DFMT_R32_UINT:
					ASSERT(GLEW_EXT_texture_integer);
					m_format = GL_RED_INTEGER_EXT;
					m_internal_format = GL_RGBA32UI_EXT;
					m_bytes_per_pixel = 4;
					m_data_type = GL_UNSIGNED_INT;
					break;
				case H3DFMT_DXT1:
					m_compressed = true;
					m_format = GL_COMPRESSED_RGB_ARB;
					m_internal_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
					m_bytes_per_pixel = -1; //unknown
					break;
				case H3DFMT_DXT3:
					m_compressed = true;
					m_format = GL_COMPRESSED_RGB_ARB;
					m_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
					m_bytes_per_pixel = -1; //unknown
					break;
				case H3DFMT_DXT5:
					m_compressed = true;
					m_format = GL_COMPRESSED_RGB_ARB;
					m_internal_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
					m_bytes_per_pixel = -1; //unknown
					break;
				case H3DFMT_D24S8:
					m_internal_format = GL_DEPTH24_STENCIL8_EXT ;
					m_format = GL_DEPTH_STENCIL_EXT ;
					m_data_type = GL_UNSIGNED_INT_24_8_EXT ;
					break;
				default:
					m_bytes_per_pixel = 4;
					ASSERT (0); //unhandled texture format
			}
			if (!m_compressed)
				glTexImage2D(GL_TEXTURE_2D, 0, m_internal_format, width, height, 0, m_format, m_data_type, null);
			// Must use these settings to avoid having to gen mipmaps (which is very slow)
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			m_min_filter = H3DTEXF_POINT;
			m_mag_filter = H3DTEXF_POINT;
			m_mip_filter = H3DTEXF_NONE;
			m_texture_wrap_s = H3DTADDRESS_CLAMP;
			m_texture_wrap_t = H3DTADDRESS_CLAMP;
			glBindTexture(GL_TEXTURE_2D, saved_texture);
		}

		GL_CHECK_ERROR();
	}

	~HOGLTexture2D();
	
	bool LockBytes(int bytes, unsigned char ** data)
	{
		m_size_allocated = bytes;
		ALLOC_ARRAY_CACHED(m_data, m_size_allocated, unsigned char);
		*data = m_data;
		return true;
	}

	bool Lock(H3DRect *h3drect, int level, unsigned char ** data, int * pitch = null)
	{
		HOGLData *ogldata = (HOGLData*) m_h3ddata;	
		H3D_TRACE (ogldata, "HOGLTexture2D::Lock");
		
		if (m_locked == true) {
			HE_ERROR(HEC_OPENGL2_DRIVER, HES_PROCEDURAL_ERROR, "Attempting to lock a locked texture.");
			return false;
		}
		m_locked = true;
		m_level_locked = level;
		m_texture_updated = true;
		
		// free any old data
		if (m_data) {
			FREE_ARRAY(m_data, m_size_allocated, unsigned char);
			m_data = null;
		}

		if (h3drect) {
			m_locked_rect = *h3drect;
			m_locked_rect.top = h3drect->bottom;
			m_locked_rect.bottom = h3drect->top;

			m_sub_rect = true;
			m_size_allocated = Abs((h3drect->right - h3drect->left) * (h3drect->bottom - h3drect->top) * m_bytes_per_pixel);
		}
		else {
			m_sub_rect = false;
			m_size_allocated = m_width * m_height * m_bytes_per_pixel;
		}

		ALLOC_ARRAY_CACHED(m_data, m_size_allocated, unsigned char);
		*data = m_data;
		
		if (pitch)
			*pitch = m_width * m_bytes_per_pixel;

		return true;
	}

	bool Lock(H3DRect *h3drect, unsigned char ** data, int * pitch = null)
	{
		return Lock(h3drect, 0, data, pitch);
	}

	void Unlock()
	{
		HOGLData *ogldata = (HOGLData*) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture2D::Unlock");
		GLint saved_texture = 0;

		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);
		glBindTexture(GL_TEXTURE_2D, m_id);
		
		if (m_compressed) {
			glCompressedTexImage2D (GL_TEXTURE_2D, 0, m_internal_format, m_width, m_height, 0, m_size_allocated, m_data);
		}
		else {
			/* If we're locking subrects, we must buffer data as soon as we unlock,
			 * in case we want to update multiple regions of the texture before binding
			 */
			if (m_sub_rect)
				glTexSubImage2D(GL_TEXTURE_2D, m_level_locked, 
									static_cast<GLint>(m_locked_rect.left), 
									static_cast<GLint>(m_locked_rect.bottom), 
									static_cast<GLint>(m_locked_rect.right - m_locked_rect.left),
									static_cast<GLint>(m_locked_rect.top - m_locked_rect.bottom),
									m_format, GL_UNSIGNED_BYTE, m_data);
			else
				glTexImage2D(GL_TEXTURE_2D, m_level_locked, m_internal_format, m_width, m_height, 0, m_format, m_data_type, m_data);
		}
		if (m_data) {
			FREE_ARRAY (m_data, m_size_allocated, unsigned char);
			m_data = NULL;
			m_size_allocated = 0;
		}

		m_locked = false;
		glBindTexture(GL_TEXTURE_2D, saved_texture);
	}

	void SetData(int level, void const * data, int width, int height, int stride)
	{
		HOGLData *ogldata = (HOGLData*) m_h3ddata;

		bool const use_pbo = true;

		GLint saved_texture = 0;
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);
		glBindTexture(GL_TEXTURE_2D, m_id);

		if (ANYBIT(m_usage, H3DTEXUSAGE_DYNAMIC) && use_pbo) {

			if (m_pbo == 0)
				glGenBuffers(1, &m_pbo);			

			size_t const size = height * stride;

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
			glBufferData(GL_PIXEL_UNPACK_BUFFER, size, null, GL_STREAM_DRAW);
			GLubyte * ptr = (GLubyte*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, size, GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);
			memcpy(ptr, data, size);
			glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);

			glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, m_format, m_data_type, null);

			glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

			GL_CHECK_ERROR();
		}

		else {
			glTexSubImage2D(GL_TEXTURE_2D, level, 0, 0, width, height, m_format, m_data_type, data);
		}

		glBindTexture(GL_TEXTURE_2D, saved_texture);
	}

	void GenerateMipMaps()
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture2D::GenerateMipMaps");

		GLint saved_texture;
		m_regenerate_mipmaps = true;

		ENSURE_ACTIVE_TEXTURE(ogldata, 0);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);		
		glBindTexture(GL_TEXTURE_2D, m_id);
		glGenerateMipmap( GL_TEXTURE_2D );
		glBindTexture(GL_TEXTURE_2D, saved_texture);
	}

	bool CompressTextureFromMemory(Texture *texture) {
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture2D::CompressTextureFromMemory");
				
		if (!ogldata->supports_texture_dxt1)
			return false;

		GLint saved_texture;
		GLuint format = GL_RGB, internal_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;

		ENSURE_ACTIVE_TEXTURE(ogldata, 0);
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);		
		glBindTexture(GL_TEXTURE_2D, m_id);

		switch (texture->active_image->format) {
			case Image_LUMINANCE8:
				format = GL_LUMINANCE;
				break;
			case Image_RGB24:
				format = GL_RGB;
				break;
			case Image_RGBA32:
				format = GL_RGBA;
				internal_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				break;
		}

		auto const w = texture->active_image->width;
		auto const h = texture->active_image->height;

		if (format == GL_RGB && !ogldata->supports_texture_rgb24) {
			unsigned char * padded = null;
			ALLOC_ARRAY(padded, 4 * w*h, unsigned char);
			fill_face(ogldata, texture, padded, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, padded);
			FREE_ARRAY(padded, 4 * w*h, unsigned char);
		}

		else
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, texture->active_image->block);

		glBindTexture(GL_TEXTURE_2D, saved_texture);
		return true;
	}

	bool LoadTextureFromMemory(Texture *texture) {
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture2D::LoadTextureFromMemory");

		GLint saved_texture;
		GLuint format = GL_RGB, internal_format = GL_RGB;

		ENSURE_ACTIVE_TEXTURE(ogldata, 0);		
		glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);
		glBindTexture(GL_TEXTURE_2D, m_id);

		switch (texture->active_image->format) {
			case Image_LUMINANCE8:
				format = GL_LUMINANCE;
				break;
			case Image_RGB24:
				format = GL_RGB;
				break;
			case Image_RGBA32:
				format = GL_RGBA;
				internal_format = GL_RGBA;
				break;
		}

		auto const w = texture->active_image->width;
		auto const h = texture->active_image->height;

		if (format == GL_RGB && !ogldata->supports_texture_rgb24) {
			unsigned char * padded = null;
			ALLOC_ARRAY(padded, 4 * w*h, unsigned char);
			fill_face(ogldata, texture, padded, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, padded);
			FREE_ARRAY(padded, 4 * w*h, unsigned char);
		}

		else {
			glTexImage2D(GL_TEXTURE_2D, 0, internal_format, w, h, 0, format, GL_UNSIGNED_BYTE, texture->active_image->block);
		}		

		glBindTexture(GL_TEXTURE_2D, saved_texture);
		return true;
	}

	/* HOGLTexture specific methods */
	/********************************/
	void BindTexture(int tex_unit)
	{
		HOGLData *ogldata = (HOGLData*) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture2D::BindTexture");

		glActiveTexture(GL_TEXTURE0 + tex_unit);		
		glBindTexture(GL_TEXTURE_2D, m_id);

		if (m_regenerate_mipmaps) {
			m_regenerate_mipmaps = false;
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}

	GLuint GetTexture()
	{
		return m_id;
	}

	void OnAttachedToFrameBuffer(HOGLFrameBufferObject * framebuffer);	
	void OnDetachedFromFrameBuffer(HOGLFrameBufferObject * framebuffer);	
};



class HOGLCubeTexture : public H3DCubeTexture
{
private:
	GLuint m_id;
	bool m_locked;
	H3DFACE m_face_locked;
	unsigned char *m_data;

	unsigned int m_size_allocated;

	bool m_regenerate_mipmaps;
	bool m_texture_updated;

	H3DRect m_locked_rect;
	bool m_sub_rect;

public:
	HOGLCubeTexture(H3DData *h3ddata, 
		unsigned int size, 
		unsigned int levels,
		unsigned int usage) :
		H3DCubeTexture(h3ddata, size, levels, usage, H3DFMT_A8R8G8B8)
	{
		GLint saved_texture = 0;
		UNREFERENCED(levels);
		UNREFERENCED(usage);		

		m_h3ddata = h3ddata;
		HOGLData * ogldata = (HOGLData*)h3ddata;

		m_data = null;
		m_locked = false;
		m_regenerate_mipmaps = false;
		m_texture_updated = false;
		m_bytes_per_pixel = 4;
		glGenTextures (1, &m_id);
	
		glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &saved_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

		for (int i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, ogldata->feature_level == OGL2_Desktop ? GL_RGBA8 : GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, saved_texture);
	}

	~HOGLCubeTexture()
	{
		if (m_data)
			FREE_ARRAY (m_data, m_size_allocated, unsigned char);
		glDeleteTextures (1, &m_id);
	};

	bool Lock(H3DFACE face, H3DRect *h3drect, unsigned char ** data, int * pitch = null)
	{
		if (m_locked == true) {
			HE_ERROR(HEC_OPENGL2_DRIVER, HES_PROCEDURAL_ERROR, "Attempting to lock a locked texture.");
			return false;
		}
		m_locked = true;
		m_face_locked = face;
		m_texture_updated = true;

		if (h3drect) {
			m_locked_rect = *h3drect;
			m_locked_rect.top = h3drect->bottom;
			m_locked_rect.bottom = h3drect->top;
			m_sub_rect = true;
		}
		else
			m_sub_rect = false;
		
		if (!m_data) {
			m_size_allocated = m_width * m_height * m_bytes_per_pixel;
			ALLOC_ARRAY_CACHED(m_data, m_size_allocated, unsigned char);
		}
		*data = m_data;
		
		if (pitch)
			*pitch = m_width * m_bytes_per_pixel;

		return true;
	}

	void Unlock(H3DFACE face)
	{
		GLint saved_texture = 0;
		ASSERT(m_face_locked == face);	// currently, can only lock and unlock one face at a time

		glGetIntegerv(GL_TEXTURE_BINDING_CUBE_MAP, &saved_texture);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
		
		/* If we're locking subrects, we must buffer data as soon as we unlock,
		 * in case we want to update multiple regions of the texture before binding
		 */
		if (m_sub_rect)
			glTexSubImage2D(oglcubemapface(face), 0, 
								static_cast<GLint>(m_locked_rect.left), 
								static_cast<GLint>(m_locked_rect.bottom), 
								static_cast<GLint>(m_locked_rect.right - m_locked_rect.left),
								static_cast<GLint>(m_locked_rect.top - m_locked_rect.bottom),
								GL_RGBA, GL_UNSIGNED_BYTE, m_data);
		else
			glTexSubImage2D(oglcubemapface(face), 0, 0,0,m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, m_data);

		m_locked = false;
		glBindTexture(GL_TEXTURE_CUBE_MAP, saved_texture);

		if (m_data) {
			FREE_ARRAY (m_data, m_size_allocated, unsigned char);
			m_data = null;
		}
	}

	void GenerateMipMaps()
	{
		m_regenerate_mipmaps = true;
	}

	GLuint GetTexture()
	{
		return m_id;
	}
};



class HOGLTexture3D : public H3DTexture3D
{
private:
	GLuint m_id;
	GLuint m_format, m_internal_format;
	bool m_locked;
	H3DFACE m_face_locked;
	unsigned char *m_data;

	unsigned int m_size_allocated;

	bool m_regenerate_mipmaps;
	bool m_texture_updated;

	H3DRect m_locked_rect;
	bool m_sub_rect;

public:
	HOGLTexture3D(H3DData *h3ddata, 
		unsigned int w, 
		unsigned int h, 
		unsigned int depth, 
		unsigned int levels,
		unsigned int usage, 
		H3DFORMAT format) :
		H3DTexture3D(h3ddata, w, h, depth, levels, usage, format)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture3D::HOGLTexture3D");

		GLint saved_texture = 0;
		UNREFERENCED(levels);
		UNREFERENCED(usage);

		m_h3ddata = h3ddata;
		m_data = null;
		m_locked = false;
		m_regenerate_mipmaps = false;
		m_texture_updated = false;
		m_bytes_per_pixel = 4;

		m_format = 0;
		m_internal_format = 0; 

		switch (format) {
			case H3DFMT_L8:
				m_format = GL_LUMINANCE;
				m_internal_format = 1;
				m_bytes_per_pixel = 1;
				break;
			case H3DFMT_R8G8B8:
				m_format = GL_RGB;
				m_internal_format = GL_RGB8;
				if (h3ddata->supports_texture_rgb24)
					m_bytes_per_pixel = 3;
				else
					m_bytes_per_pixel = 4;
				break;
			case H3DFMT_A8R8G8B8:
				m_format = GL_RGBA;
				m_internal_format = GL_RGBA8;
				m_bytes_per_pixel = 4;
				break;
			case H3DFMT_DXT1:
			case H3DFMT_DXT3:
			case H3DFMT_DXT5:
			default:
				HE_PROCEDURAL_ERROR("Unhandled texture type in opengl2 driver.");
		}

		glGenTextures (1, &m_id);
		glGetIntegerv(GL_TEXTURE_3D, &saved_texture);
		glBindTexture(GL_TEXTURE_3D, m_id);

		glTexImage3D(GL_TEXTURE_3D, 0, m_internal_format, w, h, depth, 0, m_format, GL_UNSIGNED_BYTE, null);

		glBindTexture(GL_TEXTURE_3D, saved_texture);
	}

	virtual ~HOGLTexture3D()
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture3D::~HOGLTexture3D");
		
		if (m_data)
			FREE_ARRAY (m_data, m_size_allocated, unsigned char);
		glDeleteTextures (1, &m_id);
	};

	bool Lock(unsigned char ** data, int *rowpitch, int *slicepitch)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture3D::Lock");
		
		if (m_locked == true) {
			HE_ERROR(HEC_OPENGL2_DRIVER, HES_PROCEDURAL_ERROR, "Attempting to lock a locked texture.");
			return false;
		}
		m_locked = true;
		m_texture_updated = true;
		*rowpitch = 0;
		*slicepitch = 0;
		
		if (!m_data) {
			m_size_allocated = m_width * m_height * m_depth * m_bytes_per_pixel;
			ALLOC_ARRAY_CACHED(m_data, m_size_allocated, unsigned char);
		}
		*data = m_data;
		
		return true;
	}

	void Unlock()
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture3D::Unlock");
		GLint saved_texture = 0;

		glGetIntegerv(GL_TEXTURE_BINDING_3D, &saved_texture);
		glBindTexture(GL_TEXTURE_3D, m_id);
		
		glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, m_width, m_height, m_depth, m_format, GL_UNSIGNED_BYTE, m_data);

		m_locked = false;
		glBindTexture(GL_TEXTURE_3D, saved_texture);

		if (m_data) {
			FREE_ARRAY (m_data, m_size_allocated, unsigned char);
			m_size_allocated = 0;
			m_data = null;
		}
	}

	void GenerateMipMaps()
	{
		m_regenerate_mipmaps = true;
	}


	/* HOGLTexture specific methods */
	/********************************/
	void BindTexture(int tex_unit)
	{
		HOGLData *ogldata = (HOGLData *) m_h3ddata;
		H3D_TRACE (ogldata, "HOGLTexture3D::BindTexture");

		glActiveTexture(GL_TEXTURE0 + tex_unit);		
		glBindTexture(GL_TEXTURE_3D, m_id);
	}

	GLuint GetTexture()
	{
		return m_id;
	}
};


/*****************************************************************************
*****************************************************************************
					Category: HOGLFrameBufferObject Class
*****************************************************************************
*****************************************************************************/
GLuint GetNativeTexture(H3DRenderTarget *texture) {
	if (texture && texture->IsTexture()) {
		H3DTexture *h3dtex = (H3DTexture *)texture;

		if (h3dtex->IsCubeTexture())
			return ((HOGLCubeTexture *) texture)->GetTexture();
		else if (h3dtex->IsTexture3D())
			return ((HOGLTexture3D *) texture)->GetTexture();
		else
			return ((HOGLTexture2D *) texture)->GetTexture();
	}
	else
		return 0;
}

GLenum GetNativeTarget(H3DRenderTarget *texture) {
	if (texture && texture->IsTexture()) {
		H3DTexture *h3dtex = (H3DTexture *)texture;

		if (h3dtex->IsCubeTexture())
			return GL_TEXTURE_CUBE_MAP;
		else if (h3dtex->IsTexture3D())
			return GL_TEXTURE_3D;
		else
			return GL_TEXTURE_2D;
	}
	else
		return 0;
}	

static const GLenum mrt[] = { 
	GL_COLOR_ATTACHMENT0, 
	GL_COLOR_ATTACHMENT1,
	GL_COLOR_ATTACHMENT2, 
	GL_COLOR_ATTACHMENT3
};

class HOGLFrameBufferObject : public CMO
{
private:

	HOGLData *			ogldata;

public:
	
	GLuint				m_fbo;	
	RenderTargets		m_attachments;	

	HOGLFrameBufferObject(HOGLData *ogldata) : ogldata(ogldata),	m_fbo(0) {
		glGenFramebuffers(1, &m_fbo);
	}

	~HOGLFrameBufferObject()
	{
		for (int i = 0 ; i < 4 ; i++) {
			if (m_attachments.targets[i] != null)
				RemoveAttachment(m_attachments.targets[i]);
		}

		if (m_attachments.depth_stencil != null)
			RemoveAttachment(m_attachments.depth_stencil);

		glDeleteFramebuffers(1, &m_fbo);
	}	

	void OnRenderTargetDetached(H3DRenderTarget * detachedRT)
	{
		// TODO: what to do with m_saved stuff?

		ENSURE_FRAME_BUFFER(ogldata, m_fbo);

		for (int i = 0 ; i < 4 ; i++) {
			if (m_attachments.targets[i] == detachedRT) {				
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, mrt[i], GL_RENDERBUFFER, 0);
				m_attachments.targets[i] = null;	
				GL_CHECK_ERROR();
				return;
			}
		}

		if (m_attachments.depth_stencil == detachedRT) {
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
			m_attachments.depth_stencil = null;
		}
		else  {
			ASSERT(0); // Apparently not attached to this framebuffer
		}

		GL_CHECK_ERROR();
	}

	bool BindFrameBuffer()
	{		
		ENSURE_FRAME_BUFFER(ogldata, m_fbo);		

		// enable multiple rendertargets
		int num_targets = 0;
		if (m_attachments.targets[0]) num_targets++;
		if (m_attachments.targets[1]) num_targets++;
		if (m_attachments.targets[2]) num_targets++;
		if (m_attachments.targets[3]) num_targets++;
		if (ogldata->feature_level != OGL2_ES2)
			glDrawBuffers(num_targets, mrt);

		ASSERT_FRAMEBUFFER_COMPLETE(ogldata, GL_FRAMEBUFFER);

		// If successful, we need to reset a few states as the code expects them to be reset
		int render_width = 0;
		int render_height = 0;

		if (m_attachments.targets[0]) {
			render_width = m_attachments.targets[0]->m_width;
			render_height = m_attachments.targets[0]->m_height;
		}
		else if (m_attachments.depth_stencil) {
			render_width = m_attachments.depth_stencil->m_width;
			render_height = m_attachments.depth_stencil->m_height;
		}

		if (ogldata->ssaa_active) {
			render_width /= ogldata->ssaa_width_scale;
			render_height /= ogldata->ssaa_height_scale;
		}

		ENSURE_VIEWPORT(ogldata, 0, 0, render_width, render_height);
		ENSURE_SCISSOR(ogldata, false);

		GL_CHECK_ERROR();
		
		return true;
	}

	void SetTarget(GLuint attachment_point, H3DRenderTarget *target)
	{
		ENSURE_FRAME_BUFFER(ogldata, m_fbo);

		switch(attachment_point) {
			case GL_COLOR_ATTACHMENT0:
				if (!TrySetRenderTarget(target, m_attachments.targets[0]))
					return;
				break;
			case GL_COLOR_ATTACHMENT1:
				if (!TrySetRenderTarget(target, m_attachments.targets[1]))
					return;
				break;
			case GL_COLOR_ATTACHMENT2:
				if (!TrySetRenderTarget(target, m_attachments.targets[2]))
					return;
				break;
			case GL_COLOR_ATTACHMENT3:
				if (!TrySetRenderTarget(target, m_attachments.targets[3]))
					return;
				break;
			case GL_DEPTH_STENCIL_EXT:
				if (!TrySetRenderTarget(target, m_attachments.depth_stencil))
					return;
				break;

			default:
				HE_ERROR(HEC_OPENGL2_DRIVER, HES_PROCEDURAL_ERROR, "Unrecognized attachment point!");
				return;
		}		

		if (target) {
			if (target->IsTexture()) {
#ifdef OGL_ES
				if (target->m_msaa > 1) {
					ASSERT(GLEW_IMG_multisampled_render_to_texture || GLEW_EXT_multisampled_render_to_texture);
					glFramebufferTexture2DMultisampleIMG(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GetNativeTexture(target), 0, target->m_msaa);
				}

				else if (attachment_point == GL_DEPTH_STENCIL_EXT) {
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GetNativeTexture(target), 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, GetNativeTexture(target), 0);
				}

				else {
					glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point, GL_TEXTURE_2D, GetNativeTexture(target), 0);
				}
#else
				if (attachment_point == GL_DEPTH_STENCIL_EXT) {
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, GetNativeTexture(target), 0);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_TEXTURE_2D, GetNativeTexture(target), 0);
				}

				else {
					glFramebufferTexture2D(GL_FRAMEBUFFER, attachment_point, GL_TEXTURE_2D, GetNativeTexture(target), 0);
				}
#endif

				GL_GET_ERROR();
			}
			else {
				GLuint buffer, buffer2;
				HOGLRenderTarget *hogl_rb = (HOGLRenderTarget *) target;

				// if we have buffer2, then we have separate depth and stencil buffers
				if (attachment_point == GL_DEPTH_STENCIL_EXT) {
					buffer = buffer2 = hogl_rb->GetBuffer();

					if (hogl_rb->GetBuffer2() != 0)
						buffer2 = hogl_rb->GetBuffer2();

					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);

					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, buffer);
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, buffer2);
				}
				else {
					buffer = hogl_rb->GetBuffer();
					glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_point, GL_RENDERBUFFER, buffer);
				}
			}
		}
		else {
			if (attachment_point == GL_DEPTH_STENCIL_EXT) {
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, 0);
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, 0);
			}
			else
				glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment_point, GL_RENDERBUFFER, 0);
		}
		
		GL_CHECK_ERROR();
	}

	void ClearTargets()
	{
		SetTarget(GL_DEPTH_STENCIL_EXT, null);
		
		int const num_targets = Min(ogldata->caps.mrt_count, 4);        
		for (int i = 0 ; i < num_targets ; i++) {
			SetTarget(mrt[i], null);
		}
	}

	void SaveTargets(RenderTargets * save)
	{
		ASSERT(save);

		*save = m_attachments;
	}

	void RestoreTargets(RenderTargets const * saved)
	{
		ASSERT(saved);

		SetTarget(GL_DEPTH_STENCIL_EXT, saved->depth_stencil);
		
		int const num_targets = Min(ogldata->caps.mrt_count, 4);        
		for (int i = 0 ; i < num_targets ; i++) {
			SetTarget(mrt[i], saved->targets[i]);
		}
		
		GL_CHECK_ERROR();
	}

#ifdef HOOPS_DEBUG_BUILD
	void DumpAttachments(GLenum binding)
	{
		GLint depth = 0;
		glGetFramebufferAttachmentParameteriv(binding, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &depth);

		GLint stencil = 0;
		glGetFramebufferAttachmentParameteriv(binding, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &stencil);

		GLint color = 0;
		glGetFramebufferAttachmentParameteriv(binding, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &color);

		GLint color_type = 0;
		glGetFramebufferAttachmentParameteriv(binding, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &color_type);

		const char * color_type_string = color_type == GL_TEXTURE ? "texture" : "renderbuffer";

		dprintf("color (%s)= %i, depth = %i, stencil = %i\n",color_type_string, color, depth, stencil);
	}
#endif

private:
	
	void AddAttachment(H3DRenderTarget * rt)
	{
		if (rt->IsTexture()) 
			static_cast<HOGLTexture2D*>(rt)->OnAttachedToFrameBuffer(this);
		else
			static_cast<HOGLRenderTarget*>(rt)->OnAttachedToFrameBuffer(this);
	}

	void RemoveAttachment(H3DRenderTarget * rt)
	{
		if (rt->IsTexture()) 
			static_cast<HOGLTexture2D*>(rt)->OnDetachedFromFrameBuffer(this);
		else
			static_cast<HOGLRenderTarget*>(rt)->OnDetachedFromFrameBuffer(this);
	}

	bool TrySetRenderTarget(H3DRenderTarget * target, H3DRenderTarget * & cache)
	{
		if (cache != target) {			

			if (cache != null)
				RemoveAttachment(cache);

			if (target != null)
				AddAttachment(target);

			// this needs to happen after all the above logic because we may enter OnRenderTargetDetached multiple times before getting down here
			cache = target;

			return true;
		}

		return false;
	}
};

HOGLRenderTarget::~HOGLRenderTarget()
{
	for (size_t i = 0 ; i < m_parent_framebuffers.size() ; i++)	
		m_parent_framebuffers[i]->OnRenderTargetDetached(this);

	HOGLData *ogldata = (HOGLData *) m_h3ddata;
	H3D_TRACE (ogldata, "~HOGLRenderTarget");

	if (m_id != 0) {
		glDeleteRenderbuffers(1, &m_id);
		m_id = 0;
	}
	if (m_id2 != 0) {
		glDeleteRenderbuffers(1, &m_id2);
		m_id2 = 0;
	}
}

HOGLTexture2D::~HOGLTexture2D()
{
	HOGLData *ogldata = (HOGLData *) m_h3ddata;

	for (size_t i = 0 ; i < m_parent_framebuffers.size() ; i++)	
		m_parent_framebuffers[i]->OnRenderTargetDetached(this);

	H3D_TRACE (m_h3ddata, "HOGLTexture2D::~HOGLTexture2D");
	if (m_data)
		FREE_ARRAY (m_data, m_size_allocated, unsigned char);
	glDeleteTextures (1, &m_id);

	if (m_pbo)
		glDeleteBuffers(1, &m_pbo);
};

void HOGLRenderTarget::OnAttachedToFrameBuffer(HOGLFrameBufferObject * framebuffer)
{
	if (!vector_utils::contains(m_parent_framebuffers, framebuffer))
		m_parent_framebuffers.push_back(framebuffer);
}

void HOGLRenderTarget::OnDetachedFromFrameBuffer(HOGLFrameBufferObject * framebuffer)
{	
	vector_utils::remove(m_parent_framebuffers, framebuffer);	
}

void HOGLTexture2D::OnAttachedToFrameBuffer(HOGLFrameBufferObject * framebuffer)
{	
	// the same framebuffer may be added to the list more than once if we're moving a target from one attachment point to another
	m_parent_framebuffers.push_back(framebuffer);	
}

void HOGLTexture2D::OnDetachedFromFrameBuffer(HOGLFrameBufferObject * framebuffer)
{	
	vector_utils::remove(m_parent_framebuffers, framebuffer);	
}

/*****************************************************************************
*****************************************************************************
					Category: HOGLActions Class
*****************************************************************************
*****************************************************************************/
class HOGLActions : public H3DActions
{
protected:	
		
	HOGLData *m_ogldata;

public:

	HOGLSimpleShader color_blit;
	HOGLSimpleShader depth_blit;	
	HOGLFrameBufferObject blit_source;
	HOGLFrameBufferObject blit_dest;	

	HOGLActions(HOGLData *ogldata) : H3DActions(ogldata), 
		m_ogldata(ogldata), 
		color_blit(ogldata), 
		depth_blit(ogldata), 		
		blit_source(ogldata), 
		blit_dest(ogldata)		
	{

	};

	~HOGLActions()
	{

	};

	Int_XY GetWindowSize() const
	{
		Int_XY size;
		size.x = m_ogldata->window_width;
		size.y = m_ogldata->window_height;
		return size;
	}

	// This means as far as we can tell by querying the hardware, should anti-aliasing be possible. This is distinct
	// from xbits that may turn off or modify anti-alias capabilities. The xbits are a manual override of the hardware's
	// reported capabilities.
	virtual bool CanAntiAlias() const
	{
		return true;
	}

	virtual bool CanMsaa() const
	{
		return false;
	}

	virtual int MaxMsaaSamples() const
	{
		return 0;
	}

	virtual void DiscardFramebuffer(GLenum, int, GLenum const *)
	{

	}

	void InitPicture(Net_Rendition const & nr, bool flush);
	void FinishPicture(Net_Rendition const & nr, bool swap_buffers, bool flush);
	bool SetAntiAliasMode(AntialiasMode mode, int requested_samples);

	void Begin_Trace(wchar_t const *string)
	{ 
		if (m_ogldata->debug_print_actions)
			dprintf ("+%ls\n", string);

		GL_CHECK_ERROR();
	}

	void End_Trace(wchar_t const *string)
	{ 
		if (m_ogldata->debug_print_actions)
			dprintf ("-%ls\n", string);
		
		GL_CHECK_ERROR();
	}

	void Dump_State() 
	{

	}
	
	H3DGenericVertexBufferCache * CreateGenericVertexBufferCache()
	{
		HOGLData * ogldata = m_ogldata;

		if (XBIT_TEST(m_ogldata->card_entry->xbits, XBIT_AMD_BAD_DOUBLE_PRECISION) && GLEW_ARB_map_buffer_range)
			return NEW(GenericVertexBufferCacheRB)(m_h3ddata);
		else
			return NEW(HOGLGenericVertexBufferCache)(m_h3ddata);
	}

	H3DVertexBufferCache * CreateVertexBufferCache()
	{
		HOGLData * ogldata = m_ogldata;

		if (XBIT_TEST(m_ogldata->card_entry->xbits, XBIT_AMD_BAD_DOUBLE_PRECISION) && GLEW_ARB_map_buffer_range)
			return NEW(VertexBufferCacheRB)();
		else
			return NEW(HOGLVertexBufferCache)();
	}

	H3DIndexBufferCache * CreateIndexBufferCache()
	{
		HOGLData * ogldata = m_ogldata;

		if (XBIT_TEST(m_ogldata->card_entry->xbits, XBIT_AMD_BAD_DOUBLE_PRECISION) && GLEW_ARB_map_buffer_range)
			return NEW(IndexBufferCacheRB)();
		else
			return NEW(HOGLIndexBufferCache)();
	}

	H3DIndexedBufferObject * CreateIndexedBufferObject(int index, unsigned int flags) 
	{ 
		return NEW(HOGLIndexedBufferObject)(index, flags);
	}

	void SetViewport(H3DViewport const *hvp) 
	{
		H3DViewport vp = *hvp;
		if ( m_ogldata->ssaa_active ) 
		{
			vp.X *= m_ogldata->ssaa_width_scale;
			vp.Y *= m_ogldata->ssaa_height_scale;
			vp.Width *= m_ogldata->ssaa_width_scale;
			vp.Height *= m_ogldata->ssaa_height_scale;
		}
		glViewport(static_cast<GLint>(vp.X), static_cast<GLint>(vp.Y), static_cast<GLint>(vp.Width), static_cast<GLint>(vp.Height));
		
		GL_CHECK_ERROR();
	}

	void Clear(int flags, int color, float z_value, int stencil_value, bool simple_clear)
	{
		H3DMASK const channel_mask_save = m_ogldata->cache->channel_mask;
		bool const depth_mask_save = m_ogldata->cache->depth_mask;
		ENSURE_CHANNEL_MASK(m_ogldata, H3DMASK_RGBA);
		ENSURE_DEPTH_MASK(m_ogldata, true);

		float const a = ((color & 0xff000000) >> 24) / 255.0f;
		float const b = ((color & 0xff0000) >> 16) / 255.0f;
		float const g = ((color & 0xff00) >> 8) / 255.0f;
		float const r = (color & 0xff) / 255.0f;
		GLbitfield const clearflags = oglclear(flags);

		if (simple_clear) {
			glClearColor(r, g, b, a);
			glClearStencil(stencil_value);
			glClearDepth(z_value);
			glClear(clearflags);
			ENSURE_CHANNEL_MASK(m_ogldata, channel_mask_save);
			ENSURE_DEPTH_MASK(m_ogldata, depth_mask_save);
			return;
		}

		// If we're using depth effects, don't clear the depth texture to the colour value.
		RenderTargets saved;
		if (m_ogldata->depth_effects.mode && !m_ogldata->shadow_maps.mode) {
			save_targets(&saved);
			set_targets(saved.targets[0], saved.depth_stencil);
		}

		// Clear the color target and Z/stencil normally via D3D.
		glClearColor(r, g, b, a);
		glClearStencil(stencil_value);
		glClearDepth(z_value);
		glClear(clearflags);

		// If we're using depth effects, we need to put back the old targets.
		if (m_ogldata->depth_effects.mode && !m_ogldata->shadow_maps.mode) {
			// If the depth buffer was cleared, clear the depth texture to a matching color value.
			if (flags & H3DCLEAR_ZBUFFER) {
				// If we're clearing to the far plane, make sure we don't hit it.
				if(z_value == 1.0f)
					z_value = 254.0f/255.0f;

				if (m_ogldata->ssaa_line_target.mode)
					set_targets(saved.targets[2]);
				else
					set_targets(saved.targets[1]);

				if (m_ogldata->depth_effects.must_pack_rgba) {
					float packed[4];
					pack_rgba(z_value, packed);
					glClearColor(packed[0], packed[1], packed[2], packed[3]);
				}
				
				else {
					glClearColor(z_value, z_value, z_value, z_value);
				}

				glClearStencil(0);
				glClearDepth(0);
				glClear(GL_COLOR_BUFFER_BIT);
			}

			// Restore original targets.
			m_ogldata->h3d_actions->restore_targets(&saved);
		}

		ENSURE_CHANNEL_MASK(m_ogldata, channel_mask_save);
		ENSURE_DEPTH_MASK(m_ogldata, depth_mask_save);

		GL_CHECK_ERROR();
	}

	void set_depth_bias (float depth_bias, float slope_bias) {
		glPolygonOffset(slope_bias, depth_bias);
	}

	void ensure_color_blit(float const * matrix, int sampler);
	void ensure_depth_blit(float const * matrix, int sampler);	

	void stretch_rect(
		H3DRenderTarget *	in_src,
		H3DRenderTarget *	in_dest,
		H3DTEXTUREFILTERTYPE		filter,
		bool						clear_alpha,
		bool						flip,
		bool						discard,
		H3DRect const *		source_rect,
		H3DRect const *		dest_rect);

	void stretch_rect_texture(
		H3DRenderTarget * in_src, 
		H3DRenderTarget * in_dest, 
		H3DTEXTUREFILTERTYPE filter = H3DTEXF_NONE, 
		bool flip = false, 
		H3DRect const * source_rect = null, 
		H3DRect const * dest_rect = null);

	/*******************/
	/* Shader Creation */
	/*******************/
	H3DShader *CreateShader(const H3DShaderID &id, size_t size) 
	{
		ASSERT(size == sizeof(H3DShader));
		return NEW(HOGLShader)(m_ogldata, id);
	};

	/********************/
	/* Texture Creation */
	/********************/

	bool bind_texture(const Net_Rendition & nr, Texture * texture, int usage, H3DTexture * id, int texture_unit);

	bool CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned int usage, H3DFORMAT format, H3DTexture **id)
	{
		*id = NEW(HOGLTexture2D)(m_ogldata, width, height, levels, usage, format);
		return true;
	}
	bool CreateCubeTexture(unsigned int size, unsigned int levels, unsigned int usage, H3DCubeTexture **id)
	{
		*id = NEW(HOGLCubeTexture)(m_ogldata,size, levels, usage);
		return true;
	}
	bool CreateTexture3D(unsigned int w, unsigned int h, unsigned int depth, unsigned int levels, unsigned int usage, H3DFORMAT format, H3DTexture3D **id)
	{
		*id = NEW(HOGLTexture3D)(m_ogldata, w, h, depth, levels, usage, format);
		return true;
	}
	bool CreateRenderTarget(unsigned int width, unsigned int height, unsigned int usage, H3DFORMAT format, unsigned int msaa, H3DRenderTarget **id)
	{
		*id = NEW(HOGLRenderTarget)(m_ogldata, width, height, usage, format, msaa);
		return true;
	}
	void SetTexture(int unit, H3DTexture *id)
	{	
		ENSURE_TEXTURE(m_h3ddata, unit, id);
	}

	/******************/
	/* Region actions */
	/******************/
	void create_region (Net_Rendition const &  nr, IntRectangle const * area, void **image, void **z);
	void save_region (Net_Rendition const &  nr, IntRectangle const * area, void * image, void * z);	
	void destroy_region (Display_Context * dc, void * image, void * z);
	virtual bool create_targets(int width, int height, unsigned int msaa);    
	virtual void create_region_helper(QuickMovesRegion *image_region, QuickMovesRegion *depth_region);

	/********************/
	/* SaveRendertarget */
	/********************/
	void save_targets (RenderTargets *saved);
	void restore_targets (RenderTargets *saved, bool release = true);

	void set_target (int index, H3DRenderTarget *target);
	void set_targets (H3DRenderTarget *	target0, H3DRenderTarget *	zbuffer = null, H3DRenderTarget *	target1 = null, H3DRenderTarget *	target2 = null, H3DRenderTarget *	target3 = null);

	bool test_targets (H3DRenderTarget *target0, H3DRenderTarget *zbuffer = null, H3DRenderTarget *target1 = null, H3DRenderTarget *target2 = null, H3DRenderTarget *target3 = null );

	bool get_pixels( H3DRenderTarget *source, int x, int y, int width, int height, uint32_t *pixels);

	/************************/
	/* Device state "force" */
	/************************/
	void force_alphatesting(bool mode)
	{
		if (mode){
			glEnable (GL_ALPHA_TEST);
			force_alphafunc(H3DCMP_GREATEREQUAL, 0x01);
		}
		else
			glDisable(GL_ALPHA_TEST);

		force_alphafunc(H3DCMP_GREATEREQUAL, 0x01);
		m_ogldata->cache->alphatesting = mode;
		
		GL_CHECK_ERROR();
	}

	void force_alphafunc(H3DCMPFUNC func = H3DCMP_GREATEREQUAL, int ref = 0x01)
	{
		glAlphaFunc (oglcmpfunc(func), ref / 255.0f);
		m_ogldata->cache->alpha_ref = ref;
		m_ogldata->cache->alpha_func = func;

		GL_CHECK_ERROR();
	}

	void force_anti_alias(bool mode)
	{
		if (mode)
			glEnable (GL_MULTISAMPLE);
		else
			glDisable (GL_MULTISAMPLE);

		m_ogldata->cache->antialias = mode;
		
		GL_CHECK_ERROR();
	}

	void force_blend(H3DBLEND src, H3DBLEND dest)
	{
		HOGLData * ogldata = m_ogldata;

		if (ANYBIT(m_h3ddata->dc->options.bits, Drivo_WINDOW_OPACITY))
			glBlendFuncSeparate(oglblend(src), oglblend(dest), GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		else		
			glBlendFunc(oglblend(src), oglblend(dest));		

		m_ogldata->cache->src_blend = src;
		m_ogldata->cache->dest_blend = dest;		
		
		GL_CHECK_ERROR();
	}

	void force_color_mask(bool mode, bool lock_it=false)
	{
		HOGLData *ogldata = m_ogldata;
		UNREFERENCED(ogldata);

		auto const cm = m_ogldata->cache->channel_mask;

		if (!m_ogldata->cache->color_mask_locked) {
			m_ogldata->cache->color_mask = mode;
			m_ogldata->cache->color_mask_locked = lock_it;

			// the color mask as far as upper-level is concerned really just means color writes to render target 0 (TODO: target 0 *AND* the ssaa lines target actually).
			// We don't want to, for instance, turn off color writes to the post process depth texture while doing the depth-only pass for zhlr,
			// but we can only control color writes per target with glColorMaski which is not always available. So for now we're using ColorMaski where we can,
			// and drawing the wrong picture in other cases.

			if (glColorMaski) {
				if (mode)
					glColorMaski(0, ANYBIT(cm, H3DMASK_R), ANYBIT(cm, H3DMASK_G), ANYBIT(cm, H3DMASK_B), ANYBIT(cm, H3DMASK_A));
				else
					glColorMaski(0, GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			}

			else {
				if (mode)
					glColorMask(ANYBIT(cm, H3DMASK_R), ANYBIT(cm, H3DMASK_G), ANYBIT(cm, H3DMASK_B), ANYBIT(cm, H3DMASK_A));
				else
					glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			}
			
			GL_CHECK_ERROR();
		}
		else if (mode && m_ogldata->cache->color_mask) {
			if (glColorMaski)
				glColorMaski(0, ANYBIT(cm, H3DMASK_R), ANYBIT(cm, H3DMASK_G), ANYBIT(cm, H3DMASK_B), ANYBIT(cm, H3DMASK_A));
			else
				glColorMask(ANYBIT(cm, H3DMASK_R), ANYBIT(cm, H3DMASK_G), ANYBIT(cm, H3DMASK_B), ANYBIT(cm, H3DMASK_A));
		}
	}

	void force_channel_mask(H3DMASK mode)
	{
		m_ogldata->cache->channel_mask = mode;
		force_color_mask(m_ogldata->cache->color_mask);
	}

	void force_depth_mask(bool mode)
	{
		HOGLData *ogldata = m_ogldata;
		UNREFERENCED(ogldata);

		m_ogldata->cache->depth_mask = mode;
		glDepthMask ((GLboolean)mode);

		bool const depth_peeling = m_ogldata->depth_peeling.mode && m_ogldata->depth_peeling.use_mrt;

		if (m_ogldata->depth_effects.mode || depth_peeling) {

			if (glColorMaski) {
				GLuint const depth_location = m_ogldata->ssaa_line_target.mode ? 2 : 1;
				GLboolean enable = mode ? GL_TRUE : GL_FALSE;
				glColorMaski(depth_location, enable, enable, enable, enable);
			}

			else {

				if (mode) {
					int targets = 0;
					while (m_ogldata->fbo->m_attachments.targets[targets])
						targets++;
					glDrawBuffers(targets, mrt);
				}
				else {
					glDrawBuffers(1, mrt);
				}
			}
		}

		GL_CHECK_ERROR();
	}

	void force_depth_range_set(float zmin, float zmax)
	{
		m_ogldata->cache->z_range[0] = zmin;
		m_ogldata->cache->z_range[1] = zmax;
		glDepthRange(zmin,zmax);
		GL_CHECK_ERROR();
	}

	void force_transparency(bool mode)
	{
		HOGLData * ogldata = m_ogldata;

		if (mode && ANYBIT(m_h3ddata->dc->options.bits, Drivo_WINDOW_OPACITY))
			glBlendFuncSeparate(oglblend(m_ogldata->cache->src_blend), oglblend(m_ogldata->cache->dest_blend), GL_ONE, GL_ONE_MINUS_SRC_ALPHA);		

		m_ogldata->cache->transparency_on = mode;

		if (mode)
			glEnable (GL_BLEND);
		else
			glDisable (GL_BLEND);
		GL_CHECK_ERROR();
	}

	void force_culling(H3DCULL mode)
	{
		bool const new_culling = mode != H3DCULL_NONE;
		
		if (m_ogldata->culling != new_culling) {
			
			if (new_culling) {
				glEnable(GL_CULL_FACE);
			}
			
			else {
				glDisable(GL_CULL_FACE);
			}
			
			m_ogldata->culling = new_culling;
		}
		
		m_ogldata->cache->cull_mode = mode;
		glCullFace (oglcull(mode));
		
		GL_CHECK_ERROR();
	}

	void force_fog (Net_Rendition const & nr, bool mode)
	{
		// no implementation because GLSL shaders must manually fog
		UNREFERENCED(nr);
		m_ogldata->cache->fog_on = mode; // keep this from getting call constantly
	}

	void force_point_sprites (bool enable, float min_size, float max_size)
	{
		HOGLData *ogldata = m_ogldata;
		
#ifndef OGL_ES

		ASSERT(glIsEnabled(GL_PROGRAM_POINT_SIZE));
		
		if (enable) {
			glEnable(GL_POINT_SPRITE);
			glPointParameteri(GL_POINT_SPRITE_COORD_ORIGIN, GL_LOWER_LEFT);
			glPointParameterf(GL_POINT_SIZE_MIN_EXT, min_size);
			glPointParameterf(GL_POINT_SIZE_MAX_EXT, max_size);
		}
		else {
			glDisable(GL_POINT_SPRITE);			
		}
#endif

		if (enable) {	
			
			ENSURE_ACTIVE_TEXTURE(ogldata, 0);			
			GLint saved_texture = 0;
			glGetIntegerv(GL_TEXTURE_BINDING_2D, &saved_texture);

			if (ogldata->point_splat_dummy_tex == null) {
				ogldata->h3d_actions->CreateTexture(1,1,1,0,H3DFMT_A8R8G8B8,&ogldata->point_splat_dummy_tex);

				glBindTexture(GL_TEXTURE_2D, GetNativeTexture(ogldata->point_splat_dummy_tex));
				ogldata->cache->texture[0] = ogldata->point_splat_dummy_tex;

				char const data[4] = {0, 0, 0, 0};
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				ogldata->dc->stats->load_texture++;

				if (saved_texture)
					glBindTexture(GL_TEXTURE_2D, saved_texture);
			}
			if (!saved_texture)
				ENSURE_TEXTURE(ogldata, 0, ogldata->point_splat_dummy_tex);
		}

		ogldata->cache->point_sprites_enabled = enable;
		
		GL_CHECK_ERROR();
	}

	void force_sampler_state (
		int texture_unit, 
		H3DTEXTUREFILTERTYPE filter, 
		H3DTEXTUREFILTERTYPE mip_filter, 
		H3DTEXTUREADDRESS wrap)
	{
		FORCE_TEXTURE_MIN_FILTER(m_ogldata, texture_unit, filter);
		FORCE_TEXTURE_MAG_FILTER(m_ogldata, texture_unit, filter);
		FORCE_TEXTURE_MIP_FILTER(m_ogldata, texture_unit, mip_filter);
		FORCE_TEXTURE_WRAP_S(m_ogldata, texture_unit, wrap);
		FORCE_TEXTURE_WRAP_T(m_ogldata, texture_unit, wrap);
		
		GL_CHECK_ERROR();
	}

	void force_scissor(bool mode)
	{
		if (m_ogldata->can_scissor) {
			INVALIDATE_SCISSOR_SET(m_ogldata);
			if (mode)
				glEnable(GL_SCISSOR_TEST);
			else
				glDisable(GL_SCISSOR_TEST);
			m_ogldata->cache->scissoring = mode;
		}
		
		GL_CHECK_ERROR();
	}

	void force_scissor_set(int in_left, int in_bottom, int in_right, int in_top)
	{
		if (m_ogldata->can_scissor) {
			int width = in_right - in_left + 1;
			int height = in_top - in_bottom + 1;
	
			if ( m_ogldata->ssaa_active ) {
				in_left *= m_ogldata->ssaa_width_scale;
				in_bottom *= m_ogldata->ssaa_height_scale;
				width *= m_ogldata->ssaa_width_scale;
				height *= m_ogldata->ssaa_height_scale;
			}
			glScissor (in_left, in_bottom, width, height);
		}

		m_h3ddata->cache->scissor_rect.left = in_left;
		m_h3ddata->cache->scissor_rect.bottom = in_bottom; 
		m_h3ddata->cache->scissor_rect.right = in_right; 
		m_h3ddata->cache->scissor_rect.top = in_top; 
		
		GL_CHECK_ERROR();
	}

	void force_shade_mode(H3DSHADEMODE mode)
	{
		// We don't have 'flat' keyword in shader prior to GLSL 130.
		if (m_ogldata->glsl_version <= 120)
			glShadeModel(oglshademode(mode));

		m_ogldata->cache->shade_mode = mode;		

		GL_CHECK_ERROR();
	}

	void force_stencilling(bool mode)
	{
		m_ogldata->cache->stencilling = mode;
		if (mode)
			glEnable (GL_STENCIL_TEST);
		else
			glDisable (GL_STENCIL_TEST);
		
		GL_CHECK_ERROR();
	}

	void force_stencil_func(H3DCMPFUNC val)
	{
		m_ogldata->cache->stencil_func = val;

		int stencil_ref;
		int stencil_mask;
		glGetIntegerv (GL_STENCIL_VALUE_MASK, (GLint*)(&stencil_mask));
		glGetIntegerv (GL_STENCIL_REF, (GLint*)(&stencil_ref));

		glStencilFunc (oglcmpfunc(val),stencil_ref,stencil_mask);
		
		GL_CHECK_ERROR();
	}

	void force_stencil_mask(int val)
	{
		m_ogldata->cache->stencil_mask = val;

		int stencil_func;
		int stencil_ref;
		glGetIntegerv (GL_STENCIL_FUNC, (GLint*)(&stencil_func));
		glGetIntegerv (GL_STENCIL_REF, (GLint*)(&stencil_ref));

		glStencilFunc (stencil_func, stencil_ref, val);
		
		GL_CHECK_ERROR();
	}

	void force_stencil_ref(int val)
	{
		m_ogldata->cache->stencil_ref = val;
		
		int stencil_func;
		int stencil_mask;
		glGetIntegerv (GL_STENCIL_FUNC, (GLint*)(&stencil_func));
		glGetIntegerv (GL_STENCIL_VALUE_MASK, (GLint*)(&stencil_mask));
		
		glStencilFunc (stencil_func, val, stencil_mask);
		
		GL_CHECK_ERROR();
	}

	void force_stencil_write_mask(unsigned int val)
	{
		m_ogldata->cache->stencil_write_mask = val;
		glStencilMask(val);
		
		GL_CHECK_ERROR();
	}

	void force_stencil_op(H3DSTENCILOP fail, H3DSTENCILOP zfail, H3DSTENCILOP pass)
	{
		m_ogldata->cache->stencil_fail = fail;
		m_ogldata->cache->stencil_zfail = zfail;
		m_ogldata->cache->stencil_pass = pass;
		glStencilOp(oglstencilop(fail),oglstencilop(zfail),oglstencilop(pass));
		
		GL_CHECK_ERROR();
	}

	void set_stencil_func(H3DCMPFUNC cmp, unsigned long ref, unsigned long mask, unsigned long writemask)
	{
		m_ogldata->cache->stencil_func = cmp;
		m_ogldata->cache->stencil_ref = ref;
		m_ogldata->cache->stencil_mask = mask;

		glStencilFunc(oglcmpfunc(cmp), ref, mask);
		if (writemask != m_ogldata->cache->stencil_write_mask)
			force_stencil_write_mask(writemask);
		
		GL_CHECK_ERROR();
	}

	void force_texture (int unit, H3DTexture *texture)
	{
		ASSERT(unit < H3D_MAX_TEXTURES);

		HOGLData *ogldata = m_ogldata;
		GLuint ogl_texture = texture ? GetNativeTexture(texture) : 0;
		GLenum ogl_target = texture ? GetNativeTarget(texture) : ogldata->cache->texture_target[unit];

		// ensure ogl_target is valid
		if (!(ogl_target == GL_TEXTURE_1D ||
			ogl_target == GL_TEXTURE_2D ||
			ogl_target == GL_TEXTURE_3D ||
			ogl_target == GL_TEXTURE_CUBE_MAP))
			ogl_target = GL_TEXTURE_2D;

		ENSURE_ACTIVE_TEXTURE(ogldata, unit);
		glBindTexture(ogl_target, ogl_texture);

		ogldata->cache->texture_target[unit] = ogl_target;
		ogldata->cache->texture[unit] = texture;
		ogldata->dc->stats->bind_texture++;

		if (texture)
			texture->texcache = &ogldata->cache->texture[unit];
		
		GL_CHECK_ERROR();
	}

	void ensure_texture (int unit, H3DTexture *texture)
	{
		if (m_ogldata->cache->texture[unit] != texture)
			force_texture(unit, texture);
	}

	void force_zbuffering(bool mode)
	{
		ASSERT(glIsEnabled(GL_DEPTH_TEST));

		m_ogldata->cache->zbuffering = mode;
		if (!mode)
			glDepthFunc(GL_ALWAYS);
		else {
			if (m_ogldata->cache->depth_test_reversed)
				glDepthFunc(GL_GREATER);
			else
				glDepthFunc(GL_LEQUAL);
		}

		GL_CHECK_ERROR();
	}	

	bool Snapshot(Net_Rendition const & nr, int w, int h, int bpp, void * data);

	void * IM_Get_Surface(Net_Rendition const & nr)
	{
		if (m_ogldata->dc_type == DCT_IMAGE && ANYBIT(m_ogldata->dc->options.bits, Drivo_GPU_RESIDENT)) {

			H3DTexture * texture = ensure_gpu_res_texture(nr);
			return (void*)(uintptr_t)static_cast<HOGLTexture2D *>(texture)->GetTexture();			
		}

		return null;
	}
	
	void draw_textured_rectangle(H3DTexture * texture, int x, int	y, int width, int height, float	u1, float v1, float u2, float v2);	

	void IM_Draw_Textured_Background(void * in_tex, float const * in_matrix, IntRectangle const * extent) 
	{
		HOGLData * ogldata = (HOGLData *) m_h3ddata;

		// Save device states
		bool const color_mask			= ogldata->cache->color_mask;
		bool const depth_mask			= ogldata->cache->depth_mask;
		bool const zbuffering			= ogldata->cache->zbuffering;
		H3DCULL const culling			= ogldata->cache->cull_mode;
		bool const scissor              = ogldata->cache->scissoring;

		ENSURE_COLOR_MASK (ogldata, true);
		ENSURE_DEPTH_MASK (ogldata, false);
		ENSURE_ZBUFFERING (ogldata, false);
		ENSURE_CULLING (m_h3ddata, H3DCULL_NONE);
		ENSURE_SCISSOR (m_h3ddata, false);
		ENSURE_VIEWPORT(m_h3ddata, 0, 0, ogldata->window_width, ogldata->window_height);
		ENSURE_TRANSPARENCY(m_h3ddata, false);        

		uintptr_t tex_tmp;
		memcpy(&tex_tmp, &in_tex, sizeof(uintptr_t));
		GLuint const tex = static_cast<GLuint>(tex_tmp);
		ASSERT(tex == tex_tmp);

		ASSERT(glIsTexture(tex) == GL_TRUE);
				
#if TARGET_OS_ANDROID==1
		GLenum const target = GL_TEXTURE_EXTERNAL_OES;
#else
		GLenum const target = GL_TEXTURE_2D;
#endif

		ENSURE_TEXTURE(m_h3ddata, 0, null);
		glBindTexture(target, tex);
		glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// This shouldn't be necessary because ogl sampler settings apply to the texture itself, not the texture unit.
		//INVALIDATE_TEXTURE_SETTING_CACHE(ogldata);

		float depth_range[2];
		glGetFloatv(GL_DEPTH_RANGE, depth_range);
		float maxz = depth_range[1];

		float matrix[16];
		SET_MEMORY(matrix, sizeof(matrix), 0);
		matrix[0] = (GLfloat) (2.0 / ogldata->window_width);
		matrix[5] = (GLfloat) (2.0 / ogldata->window_height);
		matrix[10] = (GLfloat) (1.0 / maxz);
		matrix[12] = (GLfloat) - 1;
		matrix[13] = (GLfloat) - 1;
		matrix[14] = (GLfloat) 0;
		matrix[15] = (GLfloat) 1;

		// Setup the shader 
		H3DShaderID id;
		H3DShader *shader;
		id.InitTexturedRectangle();

#if TARGET_OS_ANDROID == 1
		id.AddFlags(H3DID_TEXTURE_EXTERNAL_OES, 5);
#else
		id.AddFlags(H3DID_BGRA_TO_RGBA, 5);
#endif

		shader = m_h3ddata->ShaderHash->Lookup (id);
		if (!shader) {
			shader = ogldata->h3d_actions->CreateShader(id, sizeof(H3DShader));
			if (!shader->Create(m_h3ddata))
				return;
			m_h3ddata->ShaderHash->Insert (id, shader);
		}
		if (m_h3ddata->cache->Shader != shader) {
			shader->Activate();
			m_h3ddata->cache->Shader = shader;
		}

		shader->EnsureColor(RGBA::White());
		shader->EnsureTextures();
		shader->Force2DTransform((float *) &matrix[0]);        

		float const x = static_cast<float>(extent->left);
		float const y = static_cast<float>(extent->bottom);

		float const w = static_cast<float>(extent->right - extent->left);
		float const h = static_cast<float>(extent->top - extent->bottom);

		Point const corners[4] = {Point(x,y,0), Point(x,y+h,0), Point(x+w,y,0), Point(x+w,y+h, 0)};
		Point uvs[4] = {Point(0,0,0), Point(0,1,0), Point(1,0,0), Point(1,1,0)};

		if (in_matrix)
			Matrix_44::Transform_Points(in_matrix, 4, uvs, uvs);

		H3DVertexBuffer & pv = m_h3ddata->pv_shared;
		H3DVertexBufferCache *vb;

		// Fill in points and draw textured rectangle
		H3DVertexFormat vf(H3DVF_PT_TEX);
		ENSURE_VB_CACHE_VF(m_h3ddata, vf, vb);
		if (vb->Lock(4, &pv)) {
			for (int i = 0 ; i < 4 ; i++) {
				pv.xyz(corners[i]);
				pv.uvs(uvs[i]);
				pv.incr();
			}            
			vb->Unlock();
		}
		vb->Draw(m_h3ddata, H3DPT_TRIANGLESTRIP, 2);

		// Restore device states
		ENSURE_COLOR_MASK (ogldata, color_mask);
		ENSURE_DEPTH_MASK (ogldata, depth_mask);
		ENSURE_ZBUFFERING (ogldata, zbuffering);
		ENSURE_CULLING (m_h3ddata, culling);
		ENSURE_SCISSOR (m_h3ddata, scissor);
		ENSURE_VIEWPORT(m_h3ddata, 0, 0, ogldata->window_width, ogldata->window_height);

	}	
	
}; // HOGLActions

	  
#endif
/*OGL2_H_DEFINED*/
