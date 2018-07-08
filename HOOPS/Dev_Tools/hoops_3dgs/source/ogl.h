// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef OGL_H_DEFINED
#define OGL_H_DEFINED


#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "driver.h"
#include "phedron.h"
#include "please.h"

#include "oglext.h"

#if OSMESA_BUILD
#include "GL/osmesa.h"
#endif

#ifdef GLX_DRIVER
#define Window X_Window
#define Font X_Font
#define Drawable X_Drawable
#define Depth X_Depth
#define Time X_Time

# include <GL/glx.h>

#undef Window 
#undef Font
#undef Drawable
#undef Depth
#undef Time 

# include "x11data.h"

# ifdef LINUX_SYSTEM
#   define caddr_t char*
#   ifndef __cplusplus
	  typedef struct GLUquadric GLUquadric;
#   endif
# endif

# if defined(NUTCRACKER)
#ifndef EXCEED_BUILD
/* for machine missing pbuffers */
	  typedef Window GLXPbuffer;
	  typedef void *GLXFBConfig;
#endif
#define glXChooseFBConfig(x1,x2,x3,x4) null
#define glXGetVisualFromFBConfig(x1,x2) null
#define glXCreatePbuffer(x1,x2,x3) null 
#endif
#endif

#ifdef WGL_DRIVER
#	include "msw.h"
#endif

#ifdef OSX_SYSTEM

#define Point OSX_Point
#define Style OSX_Style
#define Marker OSX_Marker
#define Polygon OSX_Polygon
#define Button OSX_Button
#define Line OSX_Line

#ifdef AGL_DRIVER
# undef local
# undef OS_INLINE
# define OS_INLINE static __inline__ /* workaround for a problem in agl headers */
# include <AGL/agl.h>
# undef local
# define local static
# include <OpenGL/gl.h>
# include <OpenGL/glu.h>
# include <Carbon/Carbon.h>
#endif

#ifdef COCOA_OGL_DRIVER

# undef local

# include <ApplicationServices/ApplicationServices.h>
# include <CoreFoundation/CoreFoundation.h>
# include <CoreServices/CoreServices.h>


# define local static

# include <OpenGL/gl.h>
# include <OpenGL/glu.h>


#endif

#undef Point
#undef Style
#undef Marker
#undef Polygon 
#undef Button
#undef Line

#else
# include <GL/gl.h>
# include <GL/glu.h>
#endif

#ifndef AGLM
# define AGLM(x)
#endif


/* Debug_NO_TWO_SIDED_LIGHTING is a way of indicating that it is okay for the 
 * back face of objects to appear dark, even if they are not culled away.  This
 * will improve performance significantly on some systems. */
#define	Debug_NO_TWO_SIDED_LIGHTING	0x00000001
#define	DEBUG_GLX_INDIRECT			0x00000002
#define	Debug_OSX_UNDERLAY			0x00000004
#define	Debug_PUNT_BIG_TEXTURE		0x00000008
#define	Debug_UNUSED5_RETIREDUNTIL19 0x00000010  //do not reuse until HOOPS 19
#define	Debug_UNUSED6_RETIREDUNTIL19 0x00000020  //do not reuse until HOOPS 19
/* Debug_NO_WINDOWS_HOOK is provided so that users can tell us not to hook into
 * the message queue for a window - this means that the user must process
 * ALL messages and call Update_Display when they want hoops to draw
 * into the provided window is unused on unix*/
#define	Debug_NO_WINDOWS_HOOK		0x00000040
#define	Debug_USE_QGL_CONTEXT		0x00000080
#define	Debug_WGL_NO_SHARELISTS		0x00000100
#define	Debug_QMOVES_NO_ZDAMAGE		0x00000200
#define	Debug_ENVIRONMENT_MAP_FORCE_MULTIPASS 0x00000400
#define	Debug_UNUSED12				0x00000800
#define	DEBUG_MITRE_TO_DEFAULT		0x00001000
/*default clear is white*/
#define DEBUG_STARTUP_NO_CLEAR		0x00002000
#define	DEBUG_STARTUP_CLEAR_BLACK	0x00004000
#define DEBUG_FORCE_XOR_OVERLAY		0x00008000
#define DEBUG_NO_VERTEX_ARRAYS		0x00010000
#define Debug_FLUSH_INTERVAL		0x00020000
#define Debug_UNUSED19				0x00040000
#define Debug_UNUSED20				0x00080000
#define Debug_USE_WINDOW_IS_IMAGE	0x00100000
#define Debug_16BIT_ZBUFFER			0x00200000
#define Debug_HLR_ACTIVE			0x00400000
#define Debug_NO_OGL_OVERLAY_PLANES	0x00800000
#define Debug_FORCE_SOFTWARE		0x01000000
#define DEBUG_FORCE_REAL_OVERLAY	0x02000000
#define DEBUG_DISABLE_OCTANE2_HACK	0x04000000
#define DEBUG_OGL_STUB				0x08000000

//#define PRINT_ACTIONS 
#ifdef PRINT_ACTIONS
#  define TRACE(action_name) \
	OutputDebugString (action_name); \
	OutputDebugString ("\n"); 
#else
#  define TRACE(action_name) DO_NOTHING(action_name)
#endif
#define ACTION_NAME(action_name, is_draw) \
	TRACE (action_name); \
	if (is_draw && ANYBIT(ogldata->dc->options.debug, DEBUG_OGL_STUB)) \
		return;


/* XBIT STUFF */

#define XBIT_NONE							0L
#define XBIT_UNDRAW_OVERLAY_FRONT_AND_BACK	1L
#define XBIT_BAD_KTX						2L
#define XBIT_DOUBLE_BUFFER_SWAP_ONLY		3L
#define XBIT_BAD_VERTEX_ARRAYS				4L
#define XBIT_BAD_SCISSORING					5L
#define XBIT_EYE_SPACE_DEPTH				6L
#define XBIT_FORCE_DEFAULT_SOFTWARE			7L
#define XBIT_BAD_OVERLAY					8L
#define XBIT_BAD_STENCIL					9L
#define XBIT_BAD_TRANSPARENT_POINT			10L
#define XBIT_RESTORE_REGION_ZBUFFER_OFF		11L
#define XBIT_BAD_MULTILIGHT					12L
#define XBIT_BAD_DISPLAY_LIST               13L
#define XBIT_FORCE_OLD_CHOOSE_PIXEL_FORMAT  14L
#define XBIT_BAD_PIXEL_STORE                15L
#define XBIT_RASTERS_AS_VERTICES			16L
#define XBIT_ALLOW_DRAWPIXEL_RESTORE		17L
#define XBIT_DOUBLE_BUFFER_COPY_ONLY		18L
#define XBIT_LIES_ABOUT_SWAP				19L
#define XBIT_PIXEL_OPERATIONS_EXPENSIVE		20L /* ***DEPRECATED*** currently used only in direct-to-ogl HStencilProbe from mvo */
#define XBIT_BAD_PHONG						21L
#define XBIT_BAD_PATTERNED_LINES			22L
#define XBIT_BAD_PBUFFER					23L
#define XBIT_BAD_SPOTLIGHT_TEXTURE			24L
#define XBIT_4142_WORKAROUND				25L
#define XBIT_BAD_TWO_SIDED_LIGHTING			26L
#define XBIT_BAD_FLAT_VERTEX_ARRAYS			27L
#define XBIT_BAD_WGL_ARB_BUFFER				28L
#define XBIT_BAD_MSAA_WGL_ARB_BUFFER		29L
#define XBIT_WGL_ARB_BUFFER_1024_LIMIT		30L
#define XBIT_FLAT_TRISTRIP_MISMATCH			31L
#define XBIT_BAD_NONDL_VERTEX_ARRAYS		32L
#define XBIT_BAD_SINGLE_BUFFERING           33L
#define XBIT_BAD_ACCUMULATION_BUFFER		34L
#define XBIT_BAD_16BIT_ACCUMULATION_BUFFER	35L
#define XBIT_LINE_PATTERN_NEEDS_BLENDING	36L
#define XBIT_SLOW_MSAA_DEPTH_TEXTURE		37L
#define XBIT_BAD_MSAA_ACCUMULATION_BUFFER	38L
#define XBIT_BAD_VERTEX_BUFFER_OBJECT		39L
#define XBIT_BAD_ELEMENT_BUFFER				40L
#define XBIT_BAD_CUT_POINTS					41L
#define XBIT_BAD_FRAGMENT_PROGRAM			42L
#define XBIT_BAD_SEPARATE_SPECULAR			43L
#define XBIT_DELAYED_VERTEX_ARRAYS			44L
#define XBIT_BAD_TEXTURE_RECTANGLE			45L
#define XBIT_BAD_MULTIPLE_CLIP_PLANES		46L
#define XBIT_SLOW_PEELED_LINESMOOTH			47L
#define XBIT_BAD_NON_POWER_OF_2_TEXTURES	48L
#define XBIT_TEXTURE_SIZE_OFF_BY_ONE		49L
#define XBIT_AMD_BAD_RGB24_TEXTURE			50L
//#define XBIT_RESIZE_IF_WE_GET_HERE			64L // see XBIT_SIZE in hoops.h

/* local typedefs: */


/****************************
* Extension functions prototypes
****************************/

#ifdef WGL_DRIVER
	/* setup */
	typedef const char * (FAR __stdcall * wglGetExtensionsStringARBProc)(HDC hdc);
	typedef GLboolean (FAR __stdcall * wglChoosePixelFormatARBProc) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
	typedef GLboolean (FAR __stdcall * wglGetPixelFormatAttribivARBProc) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, int *piValues);
	typedef GLboolean (FAR __stdcall * wglGetPixelFormatAttribfvARBProc) (HDC hdc, int iPixelFormat, int iLayerPlane, UINT nAttributes, const int *piAttributes, float *pfValues);
	typedef GLboolean (FAR __stdcall * wglChoosePixelFormatARBProc) (HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);

	/* AMD */
	typedef UINT (FAR __stdcall * wglGetGPUIDsAMDProc)(UINT maxCount, UINT *ids);
	typedef INT (FAR __stdcall * wglGetGPUInfoAMDProc)(UINT id, INT property, GLenum dataType, UINT size, void *data);

	/* spriting */
	typedef HANDLE (FAR __stdcall * wglCreateBufferRegionARBProc) (HDC hDC, int iLayerPlane, UINT uType);
	typedef void (FAR __stdcall * wglDeleteBufferRegionARBProc) (HANDLE region);
	typedef void (FAR __stdcall * wglSaveBufferRegionARBProc) (HANDLE region, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (FAR __stdcall * wglRestoreBufferRegionARBProc) (HANDLE region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
	typedef GLboolean (FAR __stdcall * wglMakeContextCurrentARBProc) (HDC hDrawDC, HDC hReadDC, HGLRC hglrc);
	typedef HANDLE (FAR __stdcall * wglCreatePbufferARBProc) (HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int *piAttribList);
	typedef HDC (FAR __stdcall * wglGetPbufferDCARBProc) (HANDLE hPbuffer);
	typedef int (FAR __stdcall * wglReleasePbufferDCARBProc) (HANDLE hPbuffer, HDC hDC);
	typedef GLboolean (FAR __stdcall * wglDestroyPbufferARBProc) (HANDLE hPbuffer);
	typedef GLboolean (FAR __stdcall * wglQueryPbufferARBProc) (HANDLE hPbuffer);
	typedef GLuint (FAR __stdcall * glNewKTXBufferRegionProc) (GLenum type);
	typedef void (FAR __stdcall * glDeleteKTXBufferRegionProc) (GLuint region);
	typedef void (FAR __stdcall * glReadKTXBufferRegionProc) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (FAR __stdcall * glDrawKTXBufferRegionProc) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
	typedef GLuint (FAR __stdcall * glKTXBufferRegionEnabledProc) (void);
	/* multitexture */
	typedef void (FAR __stdcall * glActiveTextureARBProc) (GLenum format);
	typedef void (FAR __stdcall * glClientActiveTextureARBProc) (GLenum format);
	typedef void (FAR __stdcall * glMultiTexCoord2fvARBProc) (GLenum format, const GLfloat *);
	typedef void (FAR __stdcall * glMultiTexCoord3fvARBProc) (GLenum format, const GLfloat *);
	/* vertex/pixel shaders */
	typedef void (FAR __stdcall *glGenProgramsARBProc) (GLsizei n, GLuint *programs);
	typedef void (FAR __stdcall *glDeleteProgramsARBProc) (GLsizei n, const GLuint *programs);
	typedef void (FAR __stdcall *glProgramStringARBProc) (GLenum target, GLenum format, GLsizei len, const void *string);
	typedef void (FAR __stdcall *glBindProgramARBProc) (GLenum target, GLuint program);
	typedef void (FAR __stdcall *glProgramEnvParameter4fARBProc) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	/* ARB shader objects */
	typedef void (FAR __stdcall *glShaderSourceARBProc) (GLuint shader, GLsizei nstrings, const char **strings, const GLint *lengths);
	typedef void (FAR __stdcall *glCompileShaderARBProc) (GLuint shader);
	typedef void (FAR __stdcall *glGetInfoLogARBProc) (GLuint shader, GLsizei maxLength, GLsizei *length, char *buffer);
	typedef void (FAR __stdcall *glAttachObjectARBProc) (GLuint program, GLuint shader);
	typedef void (FAR __stdcall *glDetachObjectARBProc) (GLuint program, GLuint shader);
	typedef void (FAR __stdcall *glDeleteShaderARBProc) (GLuint shader);
	typedef void (FAR __stdcall *glLinkProgramARBProc) (GLuint program);
	typedef GLuint (FAR __stdcall *glCreateShaderObjectARBProc) (GLenum shader_type);
	typedef GLuint (FAR __stdcall *glCreateProgramObjectARBProc) (void);
	typedef void (FAR __stdcall *glDeleteProgramARBProc) (GLuint program);
	typedef void (FAR __stdcall *glUseProgramObjectARBProc) (GLuint program);
	typedef void (FAR __stdcall *glGetObjectParameterfvARBProc) (GLuint shader, GLenum pname, GLfloat *params);
	typedef void (FAR __stdcall *glGetObjectParameterivARBProc) (GLuint shader, GLenum pname, GLint *params);
	/* ARB occlusion query */
	typedef void (FAR __stdcall *glGenQueriesARBProc) (GLsizei n, GLuint *ids);
	typedef void (FAR __stdcall *glDeleteQueriesARBProc) (GLsizei n, const GLuint *ids);
	typedef void (FAR __stdcall *glBeginQueryARBProc) (GLenum target, GLuint id);
	typedef void (FAR __stdcall *glEndQueryARBProc) (GLenum target);
	typedef void (FAR __stdcall *glGetQueryivARBProc) (GLenum target, GLenum pname, GLint *params);
	typedef void (FAR __stdcall *glGetQueryObjectuivARBProc) (GLuint id, GLenum pname, GLuint *params);
	/* NV occlusion query */
	typedef void (FAR __stdcall *glBeginOcclusionQueryNVProc) (GLuint id);
	typedef void (FAR __stdcall *glEndOcclusionQueryNVProc) (void);
	typedef void (FAR __stdcall *glGetOcclusionQueryuivNVProc) (GLuint id, GLenum pname, GLuint *params);
	/* NV fence */
	typedef void (FAR __stdcall *glGenFencesNVProc)(GLsizei n, GLuint *fences);
	typedef void (FAR __stdcall *glDeleteFencesNVProc)(GLsizei n, const GLuint *fences);
	typedef void (FAR __stdcall *glSetFenceNVProc)(GLuint fence, GLenum condition);
	typedef GLboolean (FAR __stdcall *glTestFenceNVProc)(GLuint fence);
	typedef void (FAR __stdcall *glFinishFenceNVProc)(GLuint fence);
	typedef GLboolean (FAR __stdcall *glIsFenceNVProc)(GLuint fence);
	typedef void (FAR __stdcall *glGetFenceivNVProc)(GLuint fence, GLenum pname, GLint *params);
	/* ARB PointParameter */
	typedef void (FAR __stdcall *glPointParameterfARBProc)(GLenum pname, GLfloat param);
	typedef void (FAR __stdcall *glPointParameterfvARBProc)(GLenum pname, const GLfloat *param);
	/* ARB Texture Compression */
	typedef void (FAR __stdcall *glCompressedTexImage2DARBProc)(GLenum target, GLint level, 
				GLenum internalformat, GLsizei width, GLsizei height, 
				GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (FAR __stdcall *glPrimitiveRestartNVProc)(void);
	typedef void (FAR __stdcall *glPrimitiveRestartIndexNVProc)(GLuint index);
	/* ARB 3D textures */
	typedef void (FAR __stdcall *glTexImage3DProc)(
				GLenum target,  
				GLint level,  
				GLint internalFormat,  
				GLsizei width,  
				GLsizei height,  
				GLsizei depth,  
				GLint border,  
				GLenum format,  
				GLenum type,  
				const GLvoid *data); 
	typedef void (FAR __stdcall *glTexSubImage3DProc)(
				GLenum   target,  
				GLint level,  
				GLint xoffset,  
				GLint yoffset,  
				GLint zoffset,  
				GLsizei width,  
				GLsizei height,  
				GLsizei depth,  
				GLenum format,  
				GLenum type,  
				const GLvoid * data); 

	/* EXT_framebuffer_object */
	typedef GLboolean (FAR __stdcall *glIsRenderbufferEXTProc)(GLuint renderbuffer);
	
	typedef void (FAR __stdcall *glBindRenderbufferEXTProc)(GLenum target, GLuint renderbuffer);
	typedef void (FAR __stdcall *glDeleteRenderbuffersEXTProc)(GLsizei n, const GLuint *renderbuffers);
	typedef void (FAR __stdcall *glGenRenderbuffersEXTProc)(GLsizei n, GLuint *renderbuffers);
	typedef void (FAR __stdcall *glRenderbufferStorageEXTProc)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (FAR __stdcall *glGetRenderbufferParameterivEXTProc)(GLenum target, GLenum pname, GLint *params);

	typedef GLboolean (FAR __stdcall *glIsFramebufferEXTProc)(GLuint framebuffer);
	typedef void (FAR __stdcall *glBindFramebufferEXTProc)(GLenum target, GLuint framebuffer);
	typedef void (FAR __stdcall *glDeleteFramebuffersEXTProc)(GLsizei n, const GLuint *framebuffers);
	typedef void (FAR __stdcall *glGenFramebuffersEXTProc)(GLsizei n, GLuint *framebuffers);
	typedef GLenum (FAR __stdcall *glCheckFramebufferStatusEXTProc)(GLenum target);

	typedef void (FAR __stdcall *glFramebufferTexture1DEXTProc)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (FAR __stdcall *glFramebufferTexture2DEXTProc)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (FAR __stdcall *glFramebufferTexture3DEXTProc)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

	typedef void (FAR __stdcall *glFramebufferRenderbufferEXTProc)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	typedef void (FAR __stdcall *glGetFramebufferAttachmentParameterivEXTProc)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
	typedef void (FAR __stdcall *glGenerateMipmapEXTProc)(GLenum target);

	/* ARB Vertex Buffer Objects */
	typedef void (FAR __stdcall *glBindBufferARBProc)(GLenum target, GLuint buffer);
	typedef void (FAR __stdcall *glDeleteBuffersARBProc)(GLsizei n, const GLuint *buffers);
	typedef void (FAR __stdcall *glGenBuffersARBProc)(GLsizei n, GLuint *buffers);
	typedef GLboolean (FAR __stdcall *glIsBufferARBProc)(GLuint buffer);
	typedef void (FAR __stdcall *glBufferDataARBProc)(GLenum target, GLsizei size, const void *data, GLenum usage);
	typedef void (FAR __stdcall *glBufferSubDataARBProc)(GLenum target, GLint *offset, GLsizei *size, const void *data);
	typedef void (FAR __stdcall *glGetBufferSubDataARBProc)(GLenum target, GLint *offset, GLsizei *size, void *data);
	typedef void * (FAR __stdcall *glMapBufferARBProc)(GLenum target, GLenum access);
	typedef GLboolean (FAR __stdcall *glUnmapBufferARBProc)(GLenum target);
	typedef void (FAR __stdcall *glGetBufferParameterivARBProc)(GLenum target, GLenum pname, int *params);
	typedef void (FAR __stdcall *glGetBufferPointervARBProc)(GLenum target, GLenum pname, void **params);

	/* glDrawRangeElementsEXT */
	typedef void (FAR __stdcall *glDrawRangeElementsEXTProc)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

	/* glMultiDrawArraysEXT */
	typedef void (FAR __stdcall *glMultiDrawArraysEXTProc) (GLenum, GLint *, GLsizei *, GLsizei);
	typedef void (FAR __stdcall *glMultiDrawElementsEXTProc) (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);

#endif

#if defined(GLX_DRIVER) || defined(AGL_DRIVER) || defined(COCOA_OGL_DRIVER)
	/* spriting */
	typedef GLuint (*glNewKTXBufferRegionProc) (GLenum type);
	typedef void (*glDeleteKTXBufferRegionProc) (GLuint region);
	typedef void (*glReadKTXBufferRegionProc) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height);
	typedef void (*glDrawKTXBufferRegionProc) (GLuint region, GLint x, GLint y, GLsizei width, GLsizei height, GLint xDest, GLint yDest);
	typedef GLuint (*glKTXBufferRegionEnabledProc) (void);
	/* multitexture */
	typedef void (*glActiveTextureARBProc) (GLenum format);
	typedef void (*glClientActiveTextureARBProc) (GLenum format);
	typedef void (*glMultiTexCoord2fvARBProc) (GLenum format, const GLfloat *);
	typedef void (*glMultiTexCoord3fvARBProc) (GLenum format, const GLfloat *);
	/* vertex/pixel shaders */
	typedef void (*glGenProgramsARBProc) (GLsizei n, GLuint *programs);
	typedef void (*glDeleteProgramsARBProc) (GLsizei n, const GLuint *programs);
	typedef void (*glProgramStringARBProc) (GLenum target, GLenum format, GLsizei len, const void *string);
	typedef void (*glBindProgramARBProc) (GLenum target, GLuint program);
	typedef void (*glProgramEnvParameter4fARBProc) (GLenum target, GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
	/* ARB shader objects */
	typedef void (*glShaderSourceARBProc) (GLuint shader, GLsizei nstrings, const char **strings, const GLint *lengths);
	typedef void (*glCompileShaderARBProc) (GLuint shader);
	typedef void (*glGetInfoLogARBProc) (GLuint shader, GLsizei maxLength, GLsizei *length, char *buffer);
	typedef void (*glAttachObjectARBProc) (GLuint program, GLuint shader);
	typedef void (*glDetachObjectARBProc) (GLuint program, GLuint shader);
	typedef void (*glDeleteShaderARBProc) (GLuint shader);
	typedef void (*glLinkProgramARBProc) (GLuint program);
	typedef GLuint (*glCreateShaderObjectARBProc) (GLenum shader_type);
	typedef GLuint (*glCreateProgramObjectARBProc) (void);
	typedef void (*glDeleteProgramARBProc) (GLuint program);
	typedef void (*glUseProgramObjectARBProc) (GLuint program);
	typedef void (*glGetObjectParameterfvARBProc) (GLuint shader, GLenum pname, GLfloat *params);
	typedef void (*glGetObjectParameterivARBProc) (GLuint shader, GLenum pname, GLint *params);
	/* ARB occlusion query */
	typedef void (*glGenQueriesARBProc) (GLsizei n, GLuint *ids);
	typedef void (*glDeleteQueriesARBProc) (GLsizei n, const GLuint *ids);
	typedef void (*glBeginQueryARBProc) (GLenum target, GLuint id);
	typedef void (*glEndQueryARBProc) (GLenum target);
	typedef void (*glGetQueryivARBProc) (GLenum target, GLenum pname, GLint *params);
	typedef void (*glGetQueryObjectuivARBProc) (GLuint id, GLenum pname, GLuint *params);
	/* NV occlusion query */
	typedef void (*glGenOcclusionQueriesNVProc) (GLsizei n, GLuint *ids);
	typedef void (*glDeleteOcclusionQueriesNVProc) (GLsizei n, const GLuint *ids);
	typedef void (*glBeginOcclusionQueryNVProc) (GLuint id);
	typedef void (*glEndOcclusionQueryNVProc) (void);
	typedef void (*glGetOcclusionQueryuivNVProc) (GLuint id, GLenum pname, GLuint *params);
	/* NV fence */
	typedef void (*glGenFencesNVProc)(GLsizei n, GLuint *fences);
	typedef void (*glDeleteFencesNVProc)(GLsizei n, const GLuint *fences);
	typedef void (*glSetFenceNVProc)(GLuint fence, GLenum condition);
	typedef GLboolean (*glTestFenceNVProc)(GLuint fence);
	typedef void (*glFinishFenceNVProc)(GLuint fence);
	typedef GLboolean (*glIsFenceNVProc)(GLuint fence);
	typedef void (*glGetFenceivNVProc)(GLuint fence, GLenum pname, GLint *params);
	/* ARB PointParameter */
	typedef void (*glPointParameterfARBProc)(GLenum pname, GLfloat param);
	typedef void (*glPointParameterfvARBProc)(GLenum pname, const GLfloat *param);
	/* ARB Texture Compression */
	typedef void (*glCompressedTexImage2DARBProc)(GLenum target, GLint level, 
				GLenum internalformat, GLsizei width, GLsizei height, 
				GLint border, GLsizei imageSize, const GLvoid *data);
	typedef void (*glPrimitiveRestartNVProc)(void);
	typedef void (*glPrimitiveRestartIndexNVProc)(GLuint index);
	/* ARB 3d textures */
	typedef void (*glTexImage3DProc)(
				GLenum target,  
				GLint level,  
				GLint internalFormat,  
				GLsizei width,  
				GLsizei height,  
				GLsizei depth,  
				GLint border,  
				GLenum format,  
				GLenum type,  
				const GLvoid *data); 
	typedef void (*glTexSubImage3DProc)(
				GLenum   target,  
				GLint level,  
				GLint xoffset,  
				GLint yoffset,  
				GLint zoffset,  
				GLsizei width,  
				GLsizei height,  
				GLsizei depth,  
				GLenum format,  
				GLenum type,  
				const GLvoid * data); 

	/* EXT_framebuffer_object */
	typedef GLboolean (*glIsRenderbufferEXTProc)(GLuint renderbuffer);
	
	typedef void (*glBindRenderbufferEXTProc)(GLenum target, GLuint renderbuffer);
	typedef void (*glDeleteRenderbuffersEXTProc)(GLsizei n, const GLuint *renderbuffers);
	typedef void (*glGenRenderbuffersEXTProc)(GLsizei n, GLuint *renderbuffers);
	typedef void (*glRenderbufferStorageEXTProc)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
	typedef void (*glGetRenderbufferParameterivEXTProc)(GLenum target, GLenum pname, GLint *params);

	typedef GLboolean (*glIsFramebufferEXTProc)(GLuint framebuffer);
	typedef void (*glBindFramebufferEXTProc)(GLenum target, GLuint framebuffer);
	typedef void (*glDeleteFramebuffersEXTProc)(GLsizei n, const GLuint *framebuffers);
	typedef void (*glGenFramebuffersEXTProc)(GLsizei n, GLuint *framebuffers);
	typedef GLenum (*glCheckFramebufferStatusEXTProc)(GLenum target);

	typedef void (*glFramebufferTexture1DEXTProc)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (*glFramebufferTexture2DEXTProc)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
	typedef void (*glFramebufferTexture3DEXTProc)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level, GLint zoffset);

	typedef void (*glFramebufferRenderbufferEXTProc)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
	typedef void (*glGetFramebufferAttachmentParameterivEXTProc)(GLenum target, GLenum attachment, GLenum pname, GLint *params);
	typedef void (*glGenerateMipmapEXTProc)(GLenum target);

	/* ARB Vertex Buffer Objects */
	typedef void (*glBindBufferARBProc)(GLenum target, GLuint buffer);
	typedef void (*glDeleteBuffersARBProc)(GLsizei n, const GLuint *buffers);
	typedef void (*glGenBuffersARBProc)(GLsizei n, GLuint *buffers);
	typedef GLboolean (*glIsBufferARBProc)(GLuint buffer);
	typedef void (*glBufferDataARBProc)(GLenum target, GLsizei size, const void *data, GLenum usage);
	typedef void (*glBufferSubDataARBProc)(GLenum target, GLint *offset, GLsizei *size, const void *data);
	typedef void (*glGetBufferSubDataARBProc)(GLenum target, GLint *offset, GLsizei *size, void *data);
	typedef void * (*glMapBufferARBProc)(GLenum target, GLenum access);
	typedef GLboolean (*glUnmapBufferARBProc)(GLenum target);
	typedef void (*glGetBufferParameterivARBProc)(GLenum target, GLenum pname, int *params);
	typedef void (*glGetBufferPointervARBProc)(GLenum target, GLenum pname, void **params);

	/* glDrawRangeElementsEXT */
	typedef void (*glDrawRangeElementsEXTProc)(GLenum mode, GLuint start, GLuint end, GLsizei count, GLenum type, const GLvoid *indices);

	/* glMultiDrawArraysEXT */
	typedef void (*glMultiDrawArraysEXTProc) (GLenum, GLint *, GLsizei *, GLsizei);
	typedef void (*glMultiDrawElementsEXTProc) (GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);

# if (MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_3)
	typedef void *AGLPbuffer;
# endif

#endif


#define	Begin_HOOPS_OGL_Namespace	namespace HOGL {
#define	End_HOOPS_OGL_Namespace		}

Begin_HOOPS_OGL_Namespace

struct OGLlpattern {
	GLint factor;
	GLushort pattern;
};


#define OGL_MAX_PASSES 64
typedef int OGL_Render_Pass;
#define RC_PASS_DIFFUSE						(OGL_Render_Pass)(0x0001)
#define RC_PASS_DIFFUSE_FRONT				(OGL_Render_Pass)(0x0002)
#define RC_PASS_DIFFUSE_BACK				(OGL_Render_Pass)(0x0004)
#define RC_PASS_DIFFUSE_TEXTURE				(OGL_Render_Pass)(0x0008)
#define RC_PASS_DIFFUSE_MULTITEXTURE		(OGL_Render_Pass)(0x0010)
#define RC_PASS_SPECULAR_TEXTURE			(OGL_Render_Pass)(0x0020)
#define RC_PASS_ENVIRONMENT_TEXTURE			(OGL_Render_Pass)(0x0040)
#define RC_PASS_SPOT_LIGHT_TEXTURE			(OGL_Render_Pass)(0x0080)
#define RC_PASS_BUMP_TEXTURE				(OGL_Render_Pass)(0x0100)
#define RC_PASS_IMAGE						(OGL_Render_Pass)(0x0200)
#define RC_PASS_INDEXED						(OGL_Render_Pass)(0x0400)
															 
typedef int OGL_Render_Control_Flags;						 
#define RCF_PARAMETER_FIXUP			(OGL_Render_Control_Flags)(0x0001) 
#define RCF_TEXTURE_MODULATES_LIGHT	(OGL_Render_Control_Flags)(0x0002) 
#define RCF_SYNTHETIC_PARAMS		(OGL_Render_Control_Flags)(0x0004) 
#define RCF_IGNORE_TRANSFORM		(OGL_Render_Control_Flags)(0x0008) 
#define RCF_GEN_WORLD_PARAMS		(OGL_Render_Control_Flags)(0x0010)
#define RCF_GEN_ENVIRONMENT_PARAMS	(OGL_Render_Control_Flags)(0x0020) 
#define RCF_FRONT_PASS				(OGL_Render_Control_Flags)(0x0040) 
#define RCF_BACK_PASS				(OGL_Render_Control_Flags)(0x0080) 
#define RCF_STATUS_PARAMS			(OGL_Render_Control_Flags)(0x0100) 
#define RCF_BUMPMAP					(OGL_Render_Control_Flags)(0x0200) 
#define RCF_FAKE_TWO_SIDED_LIGHTING	(OGL_Render_Control_Flags)(0x0400) 
#define RCF_SUPPRESS_SPECULAR		(OGL_Render_Control_Flags)(0x0800) 
#define RCF_EXTRA_DISPLACEMENT		(OGL_Render_Control_Flags)(0x1000) 


struct Bumpmap_data {
	Vector *	vTangent;
	Vector *	vBiNormal;
	Vector *	vNormal;
	Point *		vTangentVertices;
	int			length;
};


struct OGL_Render_Control {
	unsigned int			n;
	unsigned int			pass;
	unsigned int			passes;
	GLuint					id[OGL_MAX_PASSES];
	
	Texture *				texture[OGL_MAX_PASSES];
	OGL_Render_Pass			usage[OGL_MAX_PASSES];
	RGB						material_color[OGL_MAX_PASSES];
	RGB						specular_material_color[OGL_MAX_PASSES];
	int						param_offset[OGL_MAX_PASSES];
	Parameter const *		params[OGL_MAX_PASSES];
	int32_t					param_width[OGL_MAX_PASSES];    
	int						flags[OGL_MAX_PASSES];
	int						param_count[OGL_MAX_PASSES];
	Light_Instance const *	light[OGL_MAX_PASSES];
	int						multipass[OGL_MAX_PASSES];
	Bumpmap_data			bm_parameters;
};



#define OGL_VBO_ARRAYS				0x0002 
#define OGL_VBO_STRIP_ARRAYS		0x0004

struct OGL_VBO{
	SimpleSphere const *	simple_sphere_boundings;

	int *					param_offsets;
	int *					vi_offsets_long;
	int *					lengths_long;
	unsigned short *		vi_long_buffer;
	int *					vi_offsets_short;
	unsigned short *		vi_short_buffer;

	int						count;			/* number of ts's or pe's */
	int						point_count;

	GLuint					points_vbo;
	GLuint					normals_vbo;
	GLuint					params_vbo;
	GLuint					colors_vbo;
	int						param_width;

	int						position_offset;
	int						normals_offset;
	
	int						colors_offset;
	int						stride;
	
	int						num_textures;

	GLuint					vi_vbo_long;   /* strips */
	int						strips_long;
	int						total_long;
	
	GLuint					vi_vbo_short;  /* triangles or lines */
	int						total_short;
	
	int						flags;

	unsigned short			index_size;
};


struct OGL_Polymarker_VBO{
	int				point_count;
	GLuint			vertexbuffer;	//interleaved point,normal,param,color
	unsigned int	vf;
	unsigned int	vram;
};

struct OGLData; //forward

#define OGLDL_NONE					0
#define OGLDL_TEXTURE				1
#define OGLDL_GEOMETRY				2
#define OGLDL_GEOMETRY_VBO			3
#define OGLDL_SEGMENT_POLYLINE		4
#define OGLDL_SEGMENT_POLYGON		5
#define OGLDL_SEGMENT_TREE			6
#define OGLDL_SHADOW_MAP			7
#define OGLDL_GEOMETRY_LIST			8
#define OGLDL_POLYMARKER_VBO_LIST	9

#define OGLDL_BLESSED_GEOMETRY			0x0001U
#define OGLDL_BLESSED_SEGMENT			0x0002U
#define OGLDL_SINGLE_INSTANCED			0x0004U /* indicates that gdl's can be promoted to segdl's for free */
#define OGL_SEGDL_HAS_EXPLICIT_VISIBLE	0x0008U

#define FLUSH_INTERVAL 100


struct OGL_Collector;
struct OGLDL_Bin;
class Geometry_OGLDL;


/* curve_metadata here is a little container so that we can link together 
 * HD_Determine_Elliptical_Res computation results.  Otherwise we'd have to 
 * call it once before allocating and again to generate the points -- and 
 * it can get to be kind of expensive. */
struct curve_metadata {
	Point *		points;
	RGB const *	gref_edge_color;
	int			needed; //upper bound for allocation
	int			count;  //actual count
	DB_Type		type;
};
struct ellipse_metadata : public curve_metadata {
	Plane		plane;
	RGB const *	gref_face_color;
};
typedef VList<curve_metadata *> curve_metadata_List;
typedef VList<ellipse_metadata *> ellipse_metadata_List;


#include "hi_proto.h"

#define OGLDL_DELETE_TASK

#ifdef TEST_RIG_1
//[10503] : Memory leak on file close/reopen.
#undef OGLDL_DELETE_TASK
#endif

#ifdef AUTODESK
#undef OGLDL_DELETE_TASK
#endif

#ifndef WGL_DRIVER
#undef OGLDL_DELETE_TASK
#endif

#ifdef OGLDL_DELETE_TASK
GLOBAL_FUNCTION void launch_OGLDL_Delete_Task(Display_Context const * dc, Driver_DL * dl_cleanup_list);
#endif

/*
 * DISPLAY LIST RESOURCE MANAGEMENT
 */
#ifdef _WIN64
#  define DL_RESOURCES_AVAILABLE(ogldata, request_size) \
	  (HOOPS::WORLD->display_list_vram_usage + HOOPS::WORLD->texture_vram_usage + (size_t)(request_size) < HOOPS::WORLD->vram_usage_limit)
#else
//max for vram plus system memory before we stop compiling display lists
#  define DL_SYSTEM_MEMORY_LIMIT ((size_t)1024*1024*(1024+512))
#  define DL_RESOURCES_AVAILABLE(ogldata, request_size) \
	  (HOOPS::WORLD->display_list_vram_usage + HOOPS::WORLD->texture_vram_usage + (size_t)(request_size) < HOOPS::WORLD->vram_usage_limit && \
	   HOOPS::WORLD->display_list_vram_usage + dc->starting_dl_memory < DL_SYSTEM_MEMORY_LIMIT)
#endif


#ifdef WGL_DRIVER
/* these are used by the REGION_ACTIONS */
typedef void * HPBUFFERARB;
#endif

// in order of preference
typedef enum {
	OGLR_INVALID = 0,
	OGLR_FIRST_METHOD = 1,
	OGLR_DEPTH_TEXTURE_RECT = 1,
	OGLR_DEPTH_TEXTURE_SQUARE = 2,
	OGLR_KTX_BUFFER_REGION = 3,
	OGLR_WGL_ARB_BUFFER = 4,
	OGLR_MAKE_CONTEXT_CURRENT = 5,
	OGLR_DRAW_PIXELS = 6,
	OGLR_LAST_METHOD = 6
} oglregion_type;

typedef struct {
	/* used for vanilla read/draw pixels case */
	void *buffer;
	int size;
	oglregion_type type;
	IntRectangle original_area;
	GLuint id;   
#ifdef WGL_DRIVER
	/* used for arb buffer region case */
	HANDLE handle;
	HPBUFFERARB pbuffer;
	HDC hDC;
	HGLRC hGLRC;
#endif
} oglregion;


#ifdef _DEBUG
#	define DEBUG_FLUSH()	glFlush(),glFinish()
#else
#	define DEBUG_FLUSH()	(void)0
#endif


#ifdef GLX_DRIVER

/* Structure format for SERVER_OVERLAY_VISUALS property. */
typedef struct _XOverlayVisual {
	unsigned long overlay_visual;
	unsigned long transparent_type;
#define OVERLAYS_None                   0x0
#define OVERLAYS_TransparentPixel       0x1
#define OVERLAYS_TransparentMask        0x2
	unsigned long value;
	unsigned long layer;
}
XOverlayVisual;

#endif


#ifdef WGL_DRIVER

/****************************
* Font handling data
****************************/

struct OGLFontData {
	float yscale;
	int fake_ysize;
	int real_ysize;
	int descent;
	int max_char_width;

#define OGL_FONT_DATA_BUFFER_SIZE (1<<8)
#define OGL_FONT_DATA_BUFFER_MASK (OGL_FONT_DATA_BUFFER_SIZE-1)

	Stencil_Bitmap * stencil[OGL_FONT_DATA_BUFFER_SIZE];

	LOGFONT log_font;
	HFONT hFont;

	float xrrotation;
	Stencil_Bitmap * rotated_stencil[OGL_FONT_DATA_BUFFER_SIZE];
	LOGFONT rotated_log_font;
	HFONT rotated_hFont;
};

#endif

typedef int DC_Type;
#define DCT_UNDEFINED		((DC_Type)0)
#define DCT_OUR_WINDOW		((DC_Type)1)
#define DCT_THEIR_WINDOW	((DC_Type)2)
#define DCT_IMAGE			((DC_Type)3)
#define DCT_PBUFFER_IMAGE	((DC_Type)4)
#define DCT_FBO_IMAGE		((DC_Type)5)
#define DCT_OSMESA_IMAGE	((DC_Type)6)




#define ENV_MAP_TEXTURE_COUNT 11
#define SPOT_LIGHT_TEXTURE_COUNT 20


/* INTERNAL ACTION TABLE */
 
typedef void (HC_CDECL *ogl_standard_action)(OGLData * ogldata, Net_Rendition const &  nr);
typedef void (HC_CDECL *ogl_turn_off_lights_action)(OGLData * ogldata);
typedef void (HC_CDECL *ogl_turn_on_lights_action)(OGLData * ogldata, Net_Rendition const &  nr, Light_Rendition const & lr);
typedef void (HC_CDECL *ogl_set_handedness_action)(OGLData * ogldata, Transform_Rendition const & tr, int flags);
typedef void (HC_CDECL *ogl_set_line_rendition_action)(OGLData * ogldata, Net_Rendition const &  nr, Line_Rendition const & lr);
typedef void (HC_CDECL *ogl_set_material_action)(OGLData * ogldata, Net_Rendition const &  nr, Material_Rendition const & matr, RGB const *use_color, RGB const *use_specular_color);
typedef void (HC_CDECL *ogl_set_indexed_material_action)(OGLData * ogldata, Net_Rendition const &  nr, Material_Rendition const & matr);
typedef GLuint (HC_CDECL *ogl_define_texture_action)(OGLData * ogldata, Net_Rendition const &  nr, Texture * texture, Material_Components usage, int texture_unit);
typedef bool (HC_CDECL *ogl_define_interpolation_texture_action)(OGLData * ogldata, Net_Rendition const &  nr);

struct ogl_actions{
	ogl_standard_action 					set_3d_xform;
	ogl_standard_action 					set_dc_xform;
	ogl_standard_action 					normal_z_buffer;
	ogl_standard_action 					displaced_z_buffer;
	ogl_turn_off_lights_action 				turn_off_lights;
	ogl_turn_on_lights_action 				turn_on_lights;
	ogl_set_handedness_action 				set_handedness;
	ogl_set_line_rendition_action			set_line_rendition;
	ogl_set_material_action					set_material;
	ogl_set_indexed_material_action			set_texture_indexed_material;
	ogl_define_texture_action				define_texture;
	ogl_define_interpolation_texture_action	define_interpolation_texture;
};
 

#ifndef DISABLE_OGL_ACTIONS

/*OGL_ACTIONS*/
 
/*INTERNAL ACTION MACROS*/
#define OGL_SET_3D_XFORM(ogldata, nr) \
	(*(ogldata)->actions.set_3d_xform)(ogldata, nr)

#define OGL_SET_DC_XFORM(ogldata, nr) \
	(*(ogldata)->actions.set_dc_xform)(ogldata, nr)

#define OGL_TURN_OFF_LIGHTS(ogldata) \
	(*(ogldata)->actions.turn_off_lights)(ogldata)

#define OGL_TURN_ON_LIGHTS(ogldata, nr, lr) \
	(*(ogldata)->actions.turn_on_lights)(ogldata, nr, lr)

#define OGL_SET_HANDEDNESS(ogldata, tr, flags) \
	(*(ogldata)->actions.set_handedness)(ogldata, tr, flags)

#define OGL_SET_LINE_RENDITION(ogldata, nr, lr) \
	(*(ogldata)->actions.set_line_rendition)(ogldata, nr, lr)

#define OGL_SET_MATERIAL(ogldata, nr, mr, clr, sclr) \
	(*(ogldata)->actions.set_material)(ogldata, nr, mr, clr, sclr)

#define OGL_SET_TEXTURE_INDEXED_MATERIAL(ogldata, nr, mr) \
	(*(ogldata)->actions.set_texture_indexed_material)(ogldata, nr, mr)
	
#define OGL_DEFINE_TEXTURE(ogldata, nr, texture, usage, texture_unit) \
	(*(ogldata)->actions.define_texture)(ogldata, nr, texture, usage, texture_unit)
	
#define OGL_DEFINE_INTERPOLATION_TEXTURE(ogldata, nr) \
	(*(ogldata)->actions.define_interpolation_texture)(ogldata, nr)
	
#else

/*DISABLE_OGL_ACTIONS*/
 
/*INTERNAL ACTION MACROS*/
#define OGL_SET_3D_XFORM(ogldata, nr) set_3d_xform(ogldata, nr);

#define OGL_SET_DC_XFORM(ogldata, nr) set_dc_xform(ogldata, nr);

#define OGL_TURN_OFF_LIGHTS(ogldata) turn_off_lights(ogldata);

#define OGL_TURN_ON_LIGHTS(ogldata, nr, lr) turn_on_lights(ogldata, nr, lr);

#define OGL_SET_HANDEDNESS(ogldata, tr, flags) set_handedness(ogldata, tr, flags);

#define OGL_SET_LINE_RENDITION(ogldata, nr, lr) set_line_rendition(ogldata, nr, lr);

#define OGL_SET_MATERIAL(ogldata, nr, mr, clr, sclr) set_material(ogldata, nr, mr, clr, sclr);

#define OGL_SET_TEXTURE_INDEXED_MATERIAL(ogldata, nr, mr) set_texture_indexed_material(ogldata, nr, mr);
	
#define OGL_DEFINE_TEXTURE(ogldata, nr) define_texture(ogldata, nr);
	
#define OGL_DEFINE_INTERPOLATION_TEXTURE(ogldata, nr) define_interpolation_texture(ogldata, nr);
	
#endif

/*DEFAULT INTERNAL ACTION MACROS*/

#define OGL_DEFAULT_SET_3D_XFORM(ogldata, nr) \
	(*(ogldata)->default_actions.set_3d_xform)(ogldata, nr);

#define OGL_DEFAULT_SET_DC_XFORM(ogldata, nr) \
	(*(ogldata)->default_actions.set_dc_xform)(ogldata, nr);

#define OGL_DEFAULT_TURN_OFF_LIGHTS(ogldata) \
	(*(ogldata)->default_actions.turn_off_lights)(ogldata);

#define OGL_DEFAULT_TURN_ON_LIGHTS(ogldata, nr, lr) \
	(*(ogldata)->default_actions.turn_on_lights)(ogldata, nr, lr);

#define OGL_DEFAULT_SET_HANDEDNESS(ogldata, tr, flags) \
	(*(ogldata)->default_actions.set_handedness)(ogldata, tr, flags);

#define OGL_DEFAULT_SET_LINE_RENDITION(ogldata, nr, lr) \
	(*(ogldata)->default_actions.set_line_rendition)(ogldata, nr, lr);

#define OGL_DEFAULT_SET_MATERIAL(ogldata, nr, mr, clr, sclr) \
	(*(ogldata)->default_actions.set_material)(ogldata, nr, mr, clr, sclr);

#define OGL_DEFAULT_SET_TEXTURE_INDEXED_MATERIAL(ogldata, nr, mr) \
	(*(ogldata)->default_actions.set_texture_indexed_material)(ogldata, nr, mr);
	
#define OGL_DEFAULT_DEFINE_TEXTURE(ogldata, nr) \
	(*(ogldata)->default_actions.define_texture)(ogldata, nr);
	
#define OGL_DEFAULT_DEFINE_INTERPOLATION_TEXTURE(ogldata, nr) \
	(*(ogldata)->default_actions.define_interpolation_texture)(ogldata, nr);


class OptimizerScratch;


#define LOCAL_MAX_CUTTING_PLANES	8
struct cached_cutting_plane {
	GLdouble flipped[4];
};

struct OGLData {

#ifdef GLX_DRIVER

	Display *		display;
	X_Window 		window;
	GLXContext 		context;
	XVisualInfo *	visualinfo;
	Screen *		screen;
	Visual *		visual;
	GLXPbuffer		pbuffer;  /* if we are rendering as a pbuffer image */

	/* use to mung the X11 driver */
	Action_Table x11;

	struct {
		X_Window		window;
		GLXContext		context;
		Colormap		colormap;
		int				num_colors;
		long			width;
		long			height;
		int				trans_pixel;
		Driver_Color *	colors;
	} overlay;

#endif

#ifdef WGL_DRIVER

	HWND hWND;

	HDC hDC; 
	HDC hInfoDC;
	HPBUFFERARB pbuffer; /* if we are rendering as a pbuffer image */

	/*for software hack*/
	bool use_memory_buffer;
	HDC hMemDC; 
	HBITMAP hBitmap;
	HBITMAP old_hBitmap;

	HGLRC hGLRC;
	HDC   prevDC;
	HGLRC prevGLRC;
	HGLRC overlayGLRC;
	WNDPROC saved_window_proc;
	HPALETTE ghPalette, ghpalOld;

	/*
	 * Dimensions
	 */
	RECT master_rectangle;
	Int_XY old_size;
	Int_XY old_position;
	Int_XY original_aspect;

	int bottom_border;
	int right_border;
	int left_border;
	int top_border;

	int yfudge;
	int y_screen_fudge;
	int mouse_x;
	int mouse_y;

	/***********************/
	/*FONT DATA */
	int font_transformability;

	/* Logical font list */
	Log_Font_Item *local_font_list;

	int mouse_buttons;
	bool mouse_was_queued;
	bool suppress_check_events;

	bool pending_resize;
#endif

#ifdef AGL_DRIVER

	WindowRef window;
	AGLPbuffer pbuffer;  /* replacement for "window" when drawing to pbuffers */

	/* x,y,w,h outer then inner*/
	int * use_clip_override;
	int last_clip_override[8];

	GDHandle device;
	GDHandle info_device;

	AGLContext curr_AGLContext;
	AGLContext prev_AGLContext;
	GrafPtr curr_GrafPtr;
	GrafPtr prev_GrafPtr;

	EventHandlerUPP window_EventHandlerUPP; 
	EventHandlerRef event_handler_ref;

	int yfudge;
	int y_screen_fudge;

	float window_aspect;

	int mouse_x;
	int mouse_y;

	int mouse_buttons;
	bool mouse_was_queued;

	UInt32 key_state;

	bool pending_resize;
#endif
	
#ifdef COCOA_OGL_DRIVER
	CocoaData * cocoaData;
	int yfudge;
	int y_screen_fudge;    
	NSOpenGLPixelBuffer *pbuffer;
	
	Int_XY old_size;
	Int_XY old_position;
	Int_XY original_aspect;
	
	int bottom_border;
	int right_border;
	int left_border;
	int top_border;
	
#endif

	/*DON"T PUT ANY PLATFORM SPECIFIC DATA BELOW HERE EXCEPT IN THE EXTENTIONS AREA*/
	DC_Type dc_type;
	char *revision_string;

	ogl_actions actions;
	ogl_actions default_actions;

	bool double_buffering;

	Image * output_image;
	void *output_image_scratch;
	int output_image_scratch_size;
	
	Display_Context const *dc;

	unsigned int flush_interval;
	GLint stencil_planes;
	GLint zbuffer_planes;
	int zbuffer_max;

	int max_line_weight;

	bool can_texture;
	bool can_stencil;
	bool can_anti_alias; 
	int anti_alias_samples; 
	bool can_phong;

	bool fog_on;
	bool transparency_on;
	bool line_smooth_on;

	int spriting_method;

#define VERTEX_INDICES_SCRATCH_SIZE 0xffff
	int *vertex_indices_scratch_array;
#define OGL_SCRATCH_SPACE_SIZE 4096 /* must be even */
	Point *points_scratch_space;
	Vector *normals_scratch_space;
	RGB *colors_scratch_space;
	float *params_scratch_space;

	int cull_results_scratch_length;
	Test *cull_results_scratch;

	OptimizerScratch *	optimizer_scratch;

	GLint max_lights;
	GLint alpha_bits;
	int lights_in_use;
#define LIGHTS_INVALID	-1
#define LIGHTS_OFF	0
#define LIGHTS_NORMAL	1
#define LIGHTS_PHONG	2
	int light_mode;
	bool is_spotlight[32];
	bool disable_spotlight_texture;
	bool cannot_spotlight_texture_condition;

	GLint max_cutting_planes;
	GLint max_texture_size;

#define OGL_POLYMARKER_BATCH_SIZE 600
	int	polymarker_batch_size;

	bool adjoint_Z_valid;
	float adjoint_Z[4];

	int n_cutting_planes_in_use;
	int n_cutting_planes_on;
	cached_cutting_plane cached_cutting_planes[LOCAL_MAX_CUTTING_PLANES];
	Incarnation cutting_transform_incarnation;

	bool net_modelling_is_identity;
	GLfloat world_to_eye[16];
	GLfloat modelview[4][4];	

#define INVALID_INCARNATION	(Incarnation) -1
#define INDC(incarn)		(-(incarn))
#define IN3D(incarn)		(incarn)
	Incarnation tr_incarnation;
	Incarnation geom_incarnation;
	Incarnation light_incarnation;
	Incarnation fog_incarnation;
	Incarnation clip_region_incarnation;
	Incarnation mask_region_incarnation;
	bool empty_clip_region;

	/* cache current OpenGL state */
#define INVALID_CACHE		(0xfeeeeeedU)
	struct {
		int texture_unit;
		bool colormaterial;
		GLenum colormaterialmode;
		int face_pattern;
		Line_Style	line_style;
		GLfloat line_weight;
		Driver_Color color;
		Driver_Color backcolor;
		Driver_Color window_color;

		/* zbuffer_modes: */
#define Z_OFF                   0
#define Z_NORMAL                1
#define Z_DISPLACED             2
#define Z_RAW			3
		int zbuffer_mode;
		float		z_max;
		float		z_range[2];
		float		total_displacement;
		bool		extra_displacement;

		GLenum shade_model;

		/* drawing overlay via XOR */
		bool xor_on;

		struct
		{
			RGBA a_d;		/* ambient and diffuse color of lighted objects */
			RGB specular;	/* specular reflectance of lighted objects */
			RGB emission;	/* emission objects */
			RGB ambient;	/* the global ambient illumination in a scene */
			GLfloat shininess;
		}
		material, back_material;

		bool local_viewer;	/* local or infinite viewers for the light model*/

		/* culling stuff */
		bool culling;
		GLenum cull_face;

#define SCISSOR_MODE_OFF 	0
#define SCISSOR_MODE_ON 	1
		/* whether scissor clipping is enabled */
		bool scissor_mode;	
		GLint scissor_x;		
		GLint scissor_y;
		GLsizei scissor_w;
		GLsizei scissor_h;

		GLint viewport_x;		
		GLint viewport_y;
		GLsizei viewport_w;
		GLsizei viewport_h;

		GLfloat pixel_zoom_x;
		GLfloat pixel_zoom_y;

		GLint pixel_unpack_alignment;

		/* whether auto normalize is enabled */
		bool normalize;	

		GLenum frontface;
		int two_sided;

		bool color_array;
		bool vertex_array;
		bool normal_array;
		bool param_array[8];
		int active_texture;

		GLuint	array_buffer;
		GLuint	element_buffer;

		/* note that we should never add caching for glTexParameteri (at least not
		 * here) because it is separate per-texture state, not per-texunit */
#define OGL_MAX_TEXTURE_UNITS 8
		GLint texture_wrap_s[OGL_MAX_TEXTURE_UNITS];
		GLint texture_wrap_t[OGL_MAX_TEXTURE_UNITS];
		GLint texture_env_mode[OGL_MAX_TEXTURE_UNITS];
		GLint texture_gen_mode[OGL_MAX_TEXTURE_UNITS];
		bool texture_gen[OGL_MAX_TEXTURE_UNITS];
		GLint texture_perspective_correction[OGL_MAX_TEXTURE_UNITS];
		GLenum textures_on[OGL_MAX_TEXTURE_UNITS];		//GL_TEXTURE_2D or GL_TEXTURE_3D if enabled
		GLuint texture_current[OGL_MAX_TEXTURE_UNITS];
		GLuint texture_indexed;
		GLuint texture_solidcolor;
		Named_Material* texture_indexed_mapped_materials;


		bool  stencil_on;

		int	stereo_mode;
		float	stereo_skew;
		GLenum	default_buffer_mode;
#define ANTI_ALIAS_OFF 0
#define ANTI_ALIAS_ON  1
		int anti_alias_mode;

		struct {
			GLuint texture_id;
			float gloss;
			Camera_Handle	camera_pointer;
		} phong;

		int depth_function;
		GLboolean depth_mask;
		GLboolean color_mask;
		bool color_mask_locked;
		GLboolean alpha_test;
		float cutoff;
		bool compiling_display_lists;
		bool compiling_geometry_display_lists;

		bool point_smooth;
		GLfloat point_size;

		OGL_Render_Control render_control;

		GLuint list_base;
		GLenum volume_hint;

		GLenum blendfunc_sfactor;
		GLenum blendfunc_dfactor;
		bool blend_on;

	} cache;

	bool use_vertex_arrays;

#define NUM_FACE_PATTERNS		(FP_WINDOW_TRANSPARENT + 1)
	/* 32bit x 32bit mask */
	GLubyte fpatterns[NUM_FACE_PATTERNS][128];	
	
	/* for transparency stippling and user-defined patterns */
	int stipple_incarnation;
	unsigned int stipple_pattern[32];

	/*XBIT STUFF */
	Driver_Config *card_entry;

	/* for rendering nurbs curves */
	GLUnurbsObj *nurbs;

	/* for short-circuiting parts of set_[dc,3d]_xform functions */
	bool dc_mode;
	bool dc_mode_zbuffering;
	Camera_Handle camera_pointer;
	float final_wlimit;
	float aspect;
	long extent_width;
	long extent_height;
	int window_width;
	int window_height;
	long dc_width;
	long dc_height;
	bool world_handedness;
	Incarnation modelling_matrix_incarnation;
	bool is_high_color;

	/*PUT ANY EXTENTIONS STUFF BELOW HERE INCLUDING PLATFORM SPECIFIC ONES*/

	/* true if the renderer has extention*/ 
	/* even if chosen pixel format/visual does not */
	bool supports_multisample; 
	bool supports_clip_volume_hint;
	bool supports_ARB_texture_mirrored_repeat;
	bool supports_ARB_texture_border_clamp;
	bool need_texture_border;
	bool separate_specular;
	bool supports_dot3;
	int version_major;
	int version_minor;
	int subversion_minor;

	bool supports_ATI_meminfo;
	bool supports_texture_filter_anisotropic;
	bool supports_buffer_region;
	bool supports_pbuffer;	
	bool supports_fbo_buffer;	
	bool supports_make_context_current; 
	bool supports_bgra;
	bool supports_display_lists;
	bool supports_texture_non_power_of_two;
	bool supports_texture_rectangle;
	bool supports_texture_compression_s3tc;
	bool supports_ARB_depth_texture;
	bool supports_ARB_shadow;

#ifdef WGL_DRIVER
	bool supports_wgl_arb_pixel_format;
	wglGetPixelFormatAttribivARBProc wglGetPixelFormatAttribivARB;
	wglChoosePixelFormatARBProc wglChoosePixelFormatARB;

	wglCreateBufferRegionARBProc wglCreateBufferRegionARB;
	wglDeleteBufferRegionARBProc wglDeleteBufferRegionARB;
	wglSaveBufferRegionARBProc wglSaveBufferRegionARB;
	wglRestoreBufferRegionARBProc wglRestoreBufferRegionARB;

	wglCreatePbufferARBProc wglCreatePbufferARB;
	wglGetPbufferDCARBProc wglGetPbufferDCARB;
	wglReleasePbufferDCARBProc wglReleasePbufferDCARB;
	wglDestroyPbufferARBProc wglDestroyPbufferARB;
	wglQueryPbufferARBProc wglQueryPbufferARB;

	wglMakeContextCurrentARBProc wglMakeContextCurrentARB;
#endif

	bool supports_ktx_buffer;
	glNewKTXBufferRegionProc glNewKTXBufferRegion;
	glDeleteKTXBufferRegionProc glDeleteKTXBufferRegion;
	glReadKTXBufferRegionProc glReadKTXBufferRegion;
	glDrawKTXBufferRegionProc glDrawKTXBufferRegion;

#ifdef glActiveTextureARB
#  undef glActiveTextureARB
#endif
#ifdef glGenProgramsARB
#  undef glGenProgramsARB
#endif
#ifdef glDeleteProgramsARB
#  undef glDeleteProgramsARB
#endif
#ifdef glProgramStringARB
#  undef glProgramStringARB
#endif
#ifdef glBindProgramARB
#  undef glBindProgramARB
#endif
#ifdef glShaderSourceARB
#  undef glShaderSourceARB
#endif
#ifdef glCompileShaderARB
#  undef glCompileShaderARB
#endif
#ifdef glGetInfoLogARB
#  undef glGetInfoLogARB
#endif
#ifdef glAttachObjectARB
#  undef glAttachObjectARB
#endif
#ifdef _glDetachObjectARB
#  undef _glDetachObjectARB
#endif
#ifdef glDeleteShaderARB
#  undef glDeleteShaderARB
#endif
#ifdef glLinkProgramARB
#  undef glLinkProgramARB
#endif
#ifdef glCreateShaderObjectARB
#  undef glCreateShaderObjectARB
#endif
#ifdef glDeleteProgramARB
#  undef glDeleteProgramARB
#endif
#ifdef glUseProgramObjectARB
#  undef glUseProgramObjectARB
#endif

#ifdef glGetObjectParameterfvARB
#  undef glGetObjectParameterfvARB
#endif
#ifdef glGetObjectParameterivARB
#  undef glGetObjectParameterivARB
#endif

	glActiveTextureARBProc glActiveTextureARB;
	glClientActiveTextureARBProc _glClientActiveTextureARB;
	glMultiTexCoord2fvARBProc glMultiTexCoord2fvARB;
	glMultiTexCoord3fvARBProc glMultiTexCoord3fvARB;
	GLint max_texture_units;

	bool supports_ARB_vertex_program;
	bool supports_ARB_fragment_program;
	bool supports_ARB_fragment_program_shadow;
	bool supports_texture_swizzle;
	glGenProgramsARBProc glGenProgramsARB;
	glDeleteProgramsARBProc glDeleteProgramsARB;
	glProgramStringARBProc glProgramStringARB;
	glBindProgramARBProc glBindProgramARB;

	bool supports_ARB_shader_objects;
	glShaderSourceARBProc glShaderSourceARB;
	glCompileShaderARBProc glCompileShaderARB;
	glAttachObjectARBProc glAttachObjectARB;
	glGetInfoLogARBProc glGetInfoLogARB;
	glDetachObjectARBProc _glDetachObjectARB;
	glDeleteShaderARBProc glDeleteShaderARB;
	glLinkProgramARBProc glLinkProgramARB;
	glCreateShaderObjectARBProc glCreateShaderObjectARB;
	glCreateProgramObjectARBProc glCreateProgramObjectARB;
	glDeleteProgramARBProc glDeleteProgramARB;
	glUseProgramObjectARBProc glUseProgramObjectARB;
	glGetObjectParameterfvARBProc glGetObjectParameterfvARB;
	glGetObjectParameterivARBProc glGetObjectParameterivARB;
	glProgramEnvParameter4fARBProc glProgramEnvParameter4fARB;

	/* occlusion query */
	glGenQueriesARBProc glGenQueries;
	glDeleteQueriesARBProc glDeleteQueries;
	glBeginQueryARBProc glBeginQueryARB;
	glEndQueryARBProc glEndQueryARB;
	glGetQueryivARBProc glGetQueryivARB;
	glGetQueryObjectuivARBProc glGetQueryObjectuiv;
	glBeginOcclusionQueryNVProc glBeginOcclusionQueryNV;
	glEndOcclusionQueryNVProc glEndOcclusionQueryNV;
	bool occlusion_query_flush_required;

	/* VBO */
	bool supports_vbo;
	glBindBufferARBProc _glBindBufferARB;
	glDeleteBuffersARBProc _glDeleteBuffersARB;			
	glGenBuffersARBProc _glGenBuffersARB;				
	glIsBufferARBProc _glIsBufferARB;					
	glBufferDataARBProc _glBufferDataARB;				
	glBufferSubDataARBProc _glBufferSubDataARB;			
	glGetBufferSubDataARBProc _glGetBufferSubDataARB;		
	glMapBufferARBProc _glMapBufferARB;				
	glUnmapBufferARBProc _glUnmapBufferARB;				
	glGetBufferParameterivARBProc _glGetBufferParameterivARB;		
	glGetBufferPointervARBProc _glGetBufferPointervARB;		

	/* glDrawRangeElements */
	glDrawRangeElementsEXTProc glDrawRangeElementsEXT;
	glMultiDrawElementsEXTProc glMultiDrawElementsEXT;
	glMultiDrawArraysEXTProc glMultiDrawArraysEXT;

	/* NV fence */
	bool supports_nvfence;
	glGenFencesNVProc glGenFencesNV;
	glDeleteFencesNVProc glDeleteFencesNV; 
	glSetFenceNVProc glSetFenceNV;
	glTestFenceNVProc glTestFenceNV;
	glFinishFenceNVProc glFinishFenceNV;
	glIsFenceNVProc glIsFenceNV;
	glGetFenceivNVProc glGetFenceivNV; 

	/* ARB PointParameter */
	bool supports_point_parameters;
	glPointParameterfARBProc _glPointParameterfARB;
	glPointParameterfvARBProc _glPointParameterfvARB;

	/* Primitive Restart */
#define PRIMITIVE_RESTART_INDEX 0xFFFFFFFF
	bool supports_NV_primitive_restart;
	glPrimitiveRestartNVProc _glPrimitiveRestartNV;
	glPrimitiveRestartIndexNVProc _glPrimitiveRestartIndexNV;

	/* ARB Texture Compression */
	bool supports_ARB_texture_compression;
	glCompressedTexImage2DARBProc glCompressedTexImage2DARB;

	/* ARB Texture Compression */
	bool supports_ARB_3d_texture;
	glTexImage3DProc glTexImage3D;
	glTexSubImage3DProc glTexSubImage3D;

	/* EXT_framebuffer_object */
	bool supports_EXT_framebuffer_object;
	bool supports_EXT_packed_depth_stencil;

	glIsRenderbufferEXTProc _glIsRenderbufferEXT;
	glBindRenderbufferEXTProc _glBindRenderbufferEXT;
	glDeleteRenderbuffersEXTProc _glDeleteRenderbuffersEXT;
	glGenRenderbuffersEXTProc _glGenRenderbuffersEXT;
	glRenderbufferStorageEXTProc _glRenderbufferStorageEXT;
	glGetRenderbufferParameterivEXTProc glGetRenderbufferParameterivEXT;

	glIsFramebufferEXTProc _glIsFramebufferEXT;
	glBindFramebufferEXTProc _glBindFramebufferEXT;
	glDeleteFramebuffersEXTProc _glDeleteFramebuffersEXT;
	glGenFramebuffersEXTProc _glGenFramebuffersEXT;
	glCheckFramebufferStatusEXTProc _glCheckFramebufferStatusEXT;

	glFramebufferTexture1DEXTProc _glFramebufferTexture1DEXT;
	glFramebufferTexture2DEXTProc _glFramebufferTexture2DEXT;
	glFramebufferTexture3DEXTProc _glFramebufferTexture3DEXT;

	glFramebufferRenderbufferEXTProc _glFramebufferRenderbufferEXT;
	glGetFramebufferAttachmentParameterivEXTProc glGetFramebufferAttachmentParameterivEXT;
	glGenerateMipmapEXTProc glGenerateMipmapEXT;

	GLuint fbo_fb;
	GLuint fbo_color_tex;
	GLuint fbo_depth_rb;
	GLuint fbo_stencil_rb;
	GLuint fbo_depth_stencil_rb;
	GLint fbo_max_color_attachments;
	GLint fbo_max_renderbuffer_size;

	int fbo_width;
	int fbo_height;

#if OSMESA_BUILD
	OSMesaContext	osmesa_ctx;
#endif
	void *			osmesa_buffer;
	int				osmesa_width;
	int				osmesa_height;

	/* used for drawing cylinders */
	GLUquadric *cyl_quad; 
	GLUquadric *wire_cyl_quad;
	/* used for drawing spheres */
	GLUquadric *sphere_quad; 
	GLUquadric *wire_sphere_quad;

	bool is_software;
	bool continuous_mode;
	bool no_transparency_punting;

	Texture *spot_light_textures[ SPOT_LIGHT_TEXTURE_COUNT + 1 ];
	bool disable_blending;

#define	DEPTH_PEELING_MAX_LAYERS	16
#define DEPTH_PEELING_Z_TEXTURES	1
	GLuint depth_peeling_z_textures[DEPTH_PEELING_Z_TEXTURES];
	GLuint depth_peeling_rgba_textures[DEPTH_PEELING_MAX_LAYERS+1];
	int depth_peeling_width;
	int depth_peeling_height;
	bool depth_peeling_mode;
	int depth_peeling_pass;
	GLuint depth_peel_fp;
	GLuint depth_peel_fp_texture;

	GLuint depth_blit_program_id;
	GLuint depth_blit_shader_id;
	GLuint depth_blit_square_program_id;
	GLuint depth_blit_square_shader_id;

	int32_t					collect_geometry;
	int32_t					save_collect_geometry;
	OGL_Collector *			collect_list;
	OGL_Collector *			save_collect_list;
	uint32_t				save_action_mask;
	bool					ref_matrix_neg3x3;

	bool region_buffer_in_use;

	bool synthetic_swap_buffer;
	oglregion *back_buffer_blt_image;
	int	back_buffer_blt_image_w;
	int	back_buffer_blt_image_h;

	int max_pointsize;
	bool eftc;
};


typedef VList<OGLData *> OGLData_List;



class Texture_OGLDL : public Driver_DL{
public:
	GLuint *		env_ids;
	GLuint			id;
	int				vram_usage;		/* approximate amount of memory this object consumes */
};

#define ENSURE_LINE_SMOOTH_OFF(ogldata) SEMI_PROTECT (\
	if (ogldata->line_smooth_on) { \
		ogldata->line_smooth_on = false; \
		glDisable (GL_LINE_SMOOTH); \
	} \
)

#define ENSURE_LINE_SMOOTH_ON(ogldata) SEMI_PROTECT (\
	if (!ogldata->line_smooth_on && \
		(!ogldata->depth_peeling_mode || !XBIT_TEST (ogldata->card_entry->xbits, XBIT_SLOW_PEELED_LINESMOOTH))) { \
		ogldata->line_smooth_on = true; \
		glEnable (GL_LINE_SMOOTH); \
	} \
)

#define ENSURE_LINE_SMOOTH(ogldata, set) SEMI_PROTECT (\
	if (set) \
		ENSURE_LINE_SMOOTH_ON(ogldata); \
	else \
		ENSURE_LINE_SMOOTH_OFF(ogldata); \
)

#define FORCE_BLEND_FUNC(ogldata, nsfactor, ndfactor) SEMI_PROTECT(\
	(ogldata)->cache.blendfunc_sfactor = nsfactor; \
	(ogldata)->cache.blendfunc_dfactor = ndfactor; \
	glBlendFunc(nsfactor, ndfactor); \
	)

#define ENSURE_BLEND_FUNC(ogldata, nsfactor, ndfactor) SEMI_PROTECT(\
	if ((ogldata)->cache.blendfunc_sfactor != nsfactor || \
	(ogldata)->cache.blendfunc_dfactor != ndfactor) { \
	FORCE_BLEND_FUNC(ogldata, nsfactor, ndfactor); \
} \
	)

#define FORCE_BLEND_OFF(ogldata)	SEMI_PROTECT(\
	ogldata->cache.blend_on = false; \
	glDisable (GL_BLEND); \
	)

#define ENSURE_BLEND_OFF(ogldata)	SEMI_PROTECT(\
	if (ogldata->cache.blend_on) \
	FORCE_BLEND_OFF (ogldata); \
	)

#define FORCE_BLEND_ON(ogldata)	SEMI_PROTECT(\
	ogldata->cache.blend_on = true; \
	glEnable (GL_BLEND); \
	)

#define ENSURE_BLEND_ON(ogldata)	SEMI_PROTECT(\
	if (!ogldata->cache.blend_on) \
	FORCE_BLEND_ON (ogldata); \
	)




#ifndef DISABLE_OGL_ANTI_ALIAS
# ifdef TEST_RIG_1
#   define FORCE_ANTI_ALIAS_OFF(ogldata)  SEMI_PROTECT (\
	  ogldata->cache.anti_alias_mode = ANTI_ALIAS_OFF;    \
	  if (ogldata->can_anti_alias)                       \
		  glDisable(MULTISAMPLE_ARB);                   \
	  else                                                \
		  ENSURE_LINE_SMOOTH_OFF(ogldata);                \
  )



#   define FORCE_ANTI_ALIAS_ON(ogldata)  SEMI_PROTECT (\
	  ogldata->cache.anti_alias_mode = ANTI_ALIAS_ON;     \
	  if (ogldata->can_anti_alias) {                     \
		  glEnable(MULTISAMPLE_ARB);                    \
	  } else {                                            \
		  ENSURE_LINE_SMOOTH_ON(ogldata);                \
		  ENSURE_BLEND_ON(ogldata);						\
	  }                                                   \
  )
# else
INLINE void FORCE_ANTI_ALIAS_OFF(OGLData * ogldata) {
	ogldata->cache.anti_alias_mode = ANTI_ALIAS_OFF;
	if (ogldata->can_anti_alias)
		glDisable(MULTISAMPLE_ARB);
}

INLINE void FORCE_ANTI_ALIAS_ON(OGLData * ogldata) {
	ogldata->cache.anti_alias_mode = ANTI_ALIAS_ON;
	if (ogldata->can_anti_alias)
		glEnable(MULTISAMPLE_ARB);
}

# endif
INLINE void ENSURE_ANTI_ALIAS(OGLData * ogldata, bool bool_mode) {
	
	int mode = bool_mode?ANTI_ALIAS_ON:ANTI_ALIAS_OFF;
	if (ogldata->cache.xor_on) {
		if (ogldata->cache.anti_alias_mode == ANTI_ALIAS_ON)
			FORCE_ANTI_ALIAS_OFF(ogldata);
	}
	else if (ogldata->cache.anti_alias_mode != mode) {
		if (mode)
			FORCE_ANTI_ALIAS_ON(ogldata);
		else
			FORCE_ANTI_ALIAS_OFF(ogldata);
	}
}
#else
# define FORCE_ANTI_ALIAS_OFF(ogldata,mode) 
# define FORCE_ANTI_ALIAS_ON(ogldata,mode) 
# define ENSURE_ANTI_ALIAS(ogldata,mode) 
#endif


INLINE void INVALIDATE_TEXTURE_SETTING_CACHE(OGLData * ogldata, int texture_unit)
{
	ogldata->cache.texture_wrap_s[texture_unit] = INVALID_CACHE;
	ogldata->cache.texture_wrap_t[texture_unit] = INVALID_CACHE;
}


INLINE void FORCE_TEXTURE_UNIT(OGLData * ogldata, int tu)
{
	ASSERT (tu >= 0 && tu <= 8);
	if (ogldata->glActiveTextureARB) {
		(*ogldata->glActiveTextureARB)(GL_TEXTURE0_ARB + tu); }
	ogldata->cache.texture_unit = tu;
}

INLINE void ENSURE_TEXTURE_UNIT(OGLData * ogldata, int tu)
{
	if (ogldata->cache.texture_unit != tu) {
		FORCE_TEXTURE_UNIT (ogldata, tu);
	}
}

INLINE void FORCE_TEXTURE_BIND(OGLData * ogldata, int texunit, GLenum textype, GLuint texid)
{
	ENSURE_TEXTURE_UNIT(ogldata, texunit);
	glBindTexture (textype, texid);
	ogldata->cache.texture_current[texunit] = texid;
	ogldata->dc->stats->bind_texture++;
	INVALIDATE_TEXTURE_SETTING_CACHE(ogldata, texunit);
}

INLINE void ENSURE_TEXTURE_BIND(OGLData * ogldata, int texunit, GLenum textype, GLuint texid)
{
	if (ogldata->cache.texture_current[texunit] != texid)
		FORCE_TEXTURE_BIND (ogldata, texunit, textype, texid);
}

INLINE void ENSURE_TEXTURES_OFF(OGLData * ogldata, int unit)
{
	ENSURE_TEXTURE_UNIT(ogldata, unit);
	if (ogldata->cache.textures_on[unit]) {
		GLenum target = ogldata->cache.textures_on[unit];
		glDisable (target);
		ogldata->cache.textures_on[unit] = 0;
		if (unit == 0 && ogldata->depth_peeling_mode && ogldata->depth_peeling_pass > 0)
			(*ogldata->glBindProgramARB) (GL_FRAGMENT_PROGRAM_ARB, ogldata->depth_peel_fp);
	}
}

INLINE void ENSURE_TEXTURES_ON(OGLData * ogldata, GLenum target, int unit)
{
	ENSURE_TEXTURE_UNIT(ogldata, unit);
	if (!ogldata->cache.textures_on[unit]) {
		glEnable (target);
		ogldata->cache.textures_on[unit] = target;
		if (unit == 0 && ogldata->depth_peeling_mode && ogldata->depth_peeling_pass > 0)
			(*ogldata->glBindProgramARB) (GL_FRAGMENT_PROGRAM_ARB, ogldata->depth_peel_fp_texture);
	}
}


/* direct lookup from upper stereo_mode to GL buffer selection */
static const GLenum stereo_back_buffers[]  = { GL_BACK,  GL_BACK_LEFT,  GL_BACK_RIGHT  };
static const GLenum stereo_front_buffers[] = { GL_FRONT, GL_FRONT_LEFT, GL_FRONT_RIGHT };

#define FORCE_STEREO(ogldata, mode) SEMI_PROTECT (\
	if (ogldata->dc_type == DCT_FBO_IMAGE) { \
		glDrawBuffer (GL_COLOR_ATTACHMENT0_EXT); \
	} \
	else { \
		if (ogldata->double_buffering) \
			glDrawBuffer (stereo_back_buffers[ogldata->cache.stereo_mode = mode]); \
		else \
			glDrawBuffer (stereo_front_buffers[ogldata->cache.stereo_mode = mode]); \
	} \
)

#define ENSURE_STEREO(ogldata, mode) SEMI_PROTECT (\
	if (ogldata->cache.stereo_mode != mode) 		\
		FORCE_STEREO(ogldata, mode);				\
)



#define EQ_RGB(c1, c2) ((c1).red == (c2).red && (c1).green == (c2).green && (c1).blue == (c2).blue)

#define FORCE_CULLING(ogldata, face)		SEMI_PROTECT(\
	ogldata->cache.cull_face = face; \
	glCullFace (face); \
	ogldata->cache.culling = true; \
	glEnable (GL_CULL_FACE); \
)

#define ENSURE_CULLING(ogldata, face)		SEMI_PROTECT(\
	if (ogldata->cache.cull_face != face) { \
		ogldata->cache.cull_face = face; \
		glCullFace (face); \
	} \
	if (!ogldata->cache.culling) { \
		ogldata->cache.culling = true; \
		glEnable (GL_CULL_FACE); \
	} \
)

#define FORCE_CULLING_OFF(ogldata) SEMI_PROTECT(\
	ogldata->cache.culling = false; \
	glDisable (GL_CULL_FACE); \
)

#define ENSURE_CULLING_OFF(ogldata) SEMI_PROTECT(\
	if (ogldata->cache.culling) { \
		FORCE_CULLING_OFF (ogldata); \
	} \
)

#define FORCE_SHADE_MODEL(ogldata, model) SEMI_PROTECT(\
	ogldata->cache.shade_model = model; \
	glShadeModel (model); \
)

#define ENSURE_SHADE_MODEL(ogldata, model) SEMI_PROTECT(\
	if (ogldata->cache.shade_model != model) \
		FORCE_SHADE_MODEL (ogldata, model); \
)



#define FORCE_LIGHTS_OFF(ogldata) SEMI_PROTECT(\
	OGL_TURN_OFF_LIGHTS(ogldata); \
	ogldata->light_mode = LIGHTS_OFF; \
)

#define FORCE_LIGHTS_ON(ogldata, nr, lr, mode) SEMI_PROTECT(\
	OGL_TURN_ON_LIGHTS(ogldata, nr, (lr)); \
	ogldata->light_mode = mode; \
	ogldata->light_incarnation = (lr)->incarnation; \
)

#define ENSURE_LIGHTS_OFF(ogldata) SEMI_PROTECT(\
	if (ogldata->light_mode != LIGHTS_OFF) \
		FORCE_LIGHTS_OFF(ogldata); \
)

INLINE void ENSURE_LIGHTS_ON(OGLData * ogldata, Net_Rendition const & nr, Light_Rendition const & lr)
{
	if (ogldata->light_mode != LIGHTS_NORMAL || 
		ogldata->light_incarnation != lr->incarnation ||
		ogldata->cache.material.ambient != nr->misc_rendition->ambient_up)
		FORCE_LIGHTS_ON(ogldata, nr, lr, LIGHTS_NORMAL);
}


INLINE void ENSURE_LIGHTS_PHONG(OGLData * ogldata, Net_Rendition const & nr, Light_Rendition const & lr)
{
	if (ogldata->light_mode != LIGHTS_PHONG ||
		ogldata->light_incarnation != lr->incarnation ||
		ogldata->cache.material.ambient != nr->misc_rendition->ambient_up)
		FORCE_LIGHTS_ON(ogldata, nr, lr, LIGHTS_PHONG);
}

#define FORCE_TWO_SIDED(ogldata, mode) SEMI_PROTECT(\
	ogldata->cache.two_sided = mode; \
	glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, mode); \
)

#define ENSURE_TWO_SIDED(ogldata, mode)	SEMI_PROTECT(\
	if (ogldata->cache.two_sided != mode) \
		FORCE_TWO_SIDED (ogldata, mode); \
)

#define FORCE_FRONT_FACE(ogldata, mode)	SEMI_PROTECT(\
	ogldata->cache.frontface = mode; \
	glFrontFace (mode); \
)

#define ENSURE_FRONT_FACE(ogldata, mode) SEMI_PROTECT(\
	if (ogldata->cache.frontface != mode) \
		FORCE_FRONT_FACE (ogldata, mode); \
)


INLINE void FORCE_WINDOW_COLOR(OGLData * ogldata, Driver_Color const & newcolor)
{
	ogldata->cache.window_color = newcolor;
	RGB	out = RGB(newcolor.direct_rgb);
	glClearColor (out.red, out.green, out.blue, 1.0f);
}

INLINE void ENSURE_WINDOW_COLOR(OGLData * ogldata, Driver_Color const & newcolor)
{
	if (ogldata->cache.window_color != newcolor ||
		ogldata->cache.window_color.invalid())
		FORCE_WINDOW_COLOR(ogldata, newcolor);
}


void set_z_buffer (OGLData * ogldata, Net_Rendition const & nr, int mode);

INLINE void ENSURE_ZBUFFER_OFF(OGLData * ogldata, Net_Rendition const & nr)
{
	if (ogldata->cache.zbuffer_mode != Z_OFF ||
		ogldata->cache.z_range[0] != 0 ||
		ogldata->cache.z_range[1] != 0) {
		set_z_buffer(ogldata, nr, Z_OFF);
	}
}


INLINE void ENSURE_ZBUFFER_RAW(OGLData * ogldata, Net_Rendition const & nr)
{
	if (ogldata->cache.zbuffer_mode != Z_RAW ||
		ogldata->cache.z_range[0] != 0 ||
		ogldata->cache.z_range[1] != 1) {
		set_z_buffer(ogldata, nr, Z_RAW);
	}
}

INLINE void ENSURE_ZBUFFER_NORMAL(OGLData * ogldata, Net_Rendition const & nr)
{
	Transform_Rendition const &	tr = nr->transform_rendition;
	if (ANYBIT(tr->flags, TR_Z_BUFFERING)) {
		if (ogldata->cache.zbuffer_mode != Z_NORMAL ||
			ogldata->cache.z_range[0] != tr->z_range[0] ||
			ogldata->cache.z_range[1] != tr->z_range[1] ||
			ogldata->cache.total_displacement != tr->general_displacement) {
			set_z_buffer(ogldata, nr, Z_NORMAL);
		}
	} else {
		ENSURE_ZBUFFER_OFF (ogldata, nr);
	}
}


INLINE void ENSURE_ZBUFFER_DISPLACED(OGLData * ogldata, Net_Rendition const & nr)
{
	Transform_Rendition const &	tr = nr->transform_rendition;
	if (ANYBIT(tr->flags, TR_Z_BUFFERING)) {
		if (ogldata->cache.zbuffer_mode != Z_DISPLACED ||
			ogldata->cache.z_range[0] != tr->z_range[0] ||
			ogldata->cache.z_range[1] != tr->z_range[1] ||
			ogldata->cache.total_displacement !=
				(tr->face_displacement + tr->general_displacement + 
					(ogldata->cache.extra_displacement?1.0f:0))) {
			set_z_buffer(ogldata, nr, Z_DISPLACED);
		}
	} else {
		ENSURE_ZBUFFER_OFF (ogldata, nr);
	}
}

#define ENSURE_ZBUFFER_NOT_DISPLACED(ogldata,nr)            SEMI_PROTECT( \
	if (ogldata->cache.total_displacement != 0.0f)			    \
		set_z_buffer(ogldata, nr, Z_RAW);			    \
)


#define FORCE_DEPTH_FUNCTION(ogldata,val) SEMI_PROTECT(\
	glDepthFunc(val); \
	ogldata->cache.depth_function = val; \
)

#define ENSURE_DEPTH_FUNCTION(ogldata,val) SEMI_PROTECT(\
	if (ogldata->cache.depth_function != val) \
		FORCE_DEPTH_FUNCTION(ogldata,val); \
)


INLINE void FORCE_DEPTH_MASK(OGLData * ogldata, GLboolean val) {
	ogldata->cache.depth_mask = val;
	glDepthMask (val);
}

INLINE void ENSURE_DEPTH_MASK(OGLData * ogldata, GLboolean val) {
	if (ogldata->cache.depth_mask != val)
		FORCE_DEPTH_MASK(ogldata,val);
}

INLINE void FORCE_ALPHA_TEST(OGLData * ogldata, GLboolean val, float cutoff)
{
	if (val){
		glAlphaFunc(GL_GREATER, cutoff);
		glEnable(GL_ALPHA_TEST);	
	}
	else
		glDisable(GL_ALPHA_TEST);

	ogldata->cache.cutoff = cutoff;
	ogldata->cache.alpha_test = val;
}


INLINE void ENSURE_ALPHA_TEST(OGLData * ogldata, GLboolean val, float opacity=-1.0f) {
	ASSERT(val==false || opacity != -1.0f);
	float cutoff = 0.01f;
	if (!ogldata->depth_peeling_mode) 
		cutoff = 0.5f * opacity;
	else
		val = true;
	if (ogldata->cache.alpha_test != val || 
		ogldata->cache.cutoff != cutoff ) {
		FORCE_ALPHA_TEST(ogldata, val, cutoff);
	}
}


INLINE void FORCE_COLOR_MASK(OGLData * ogldata, GLboolean val, bool lock_it=false) {
	if (!ogldata->cache.color_mask_locked) {
		if (!val)
			glColorMask (GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		else
			glColorMask (GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		ogldata->cache.color_mask = val;
		ogldata->cache.color_mask_locked = lock_it;
	}
}

INLINE void ENSURE_COLOR_MASK(OGLData * ogldata, GLboolean val) {
	if (!ogldata->cache.color_mask != !val)
		FORCE_COLOR_MASK(ogldata, val);
}

INLINE void FORCE_COLOR(OGLData * ogldata, Driver_Color const * newcolor, bool grayscale)
{
	ogldata->cache.color = *newcolor;

	if (grayscale) {
		unsigned char _gray_ = RGBAS32(newcolor->direct_rgb).Gray();
		if (newcolor->direct_rgb.a != 255)
			glColor4ub (_gray_, _gray_, _gray_ , (newcolor)->direct_rgb.a);
		else
			glColor3ub (_gray_, _gray_, _gray_);
	}
	else {
		if ((newcolor)->direct_rgb.a != 255)
			glColor4ub ((newcolor)->direct_rgb.r, (newcolor)->direct_rgb.g, (newcolor)->direct_rgb.b, (newcolor)->direct_rgb.a);
		else
			glColor3ub ((newcolor)->direct_rgb.r, (newcolor)->direct_rgb.g, (newcolor)->direct_rgb.b);
	}

}

INLINE void FORCE_COLOR(RGBA const & rgba, bool grayscale)
{
	if (grayscale) {
		float gray = rgba.Gray();
		glColor4f(gray, gray, gray, rgba.alpha);
	}
	else {
		glColor4f(rgba.red, rgba.green, rgba.blue, rgba.alpha);
	}
}

INLINE void FORCE_COLOR(RGBAS32 const & rgbas32, bool grayscale)
{
	if (grayscale) {
		unsigned char gray = rgbas32.Gray();
		glColor4ub(gray, gray, gray, rgbas32.a);
	}
	else {
		glColor4ub(rgbas32.r, rgbas32.g, rgbas32.b, rgbas32.a);
	}
}


INLINE void ENSURE_COLOR(OGLData * ogldata, Driver_Color const * newcolor, Net_Rendition const & nr)
{
	if (ogldata->cache.xor_on) {
		Driver_Color xor_color = *newcolor;
		Driver_Color const &	window_color = nr->window_rendition->window_color;
		xor_color.direct_rgb.r ^= window_color.direct_rgb.r;
		xor_color.direct_rgb.g ^= window_color.direct_rgb.g;
		xor_color.direct_rgb.b ^= window_color.direct_rgb.b;
		if (ogldata->cache.color.invalid() ||
			ogldata->cache.color != xor_color) {
			FORCE_COLOR(ogldata, &xor_color, false);
		}
	} else {
		if (ogldata->cache.color.invalid() ||
			ogldata->cache.color != *newcolor) {
			FORCE_COLOR(ogldata, newcolor, false);
		}
	} 
	ogldata->cache.material.a_d.red = -1.0f;
	ogldata->cache.material.a_d.green = -1.0f;
	ogldata->cache.material.a_d.blue = -1.0f;
	ogldata->cache.material.a_d.alpha = -1.0f;
}

INLINE void FORCE_LINE_PATTERN(OGLData * ogldata, Line_Style const & lstyle)
{
	ogldata->cache.line_style = lstyle;
	if (!ANYBIT(lstyle->flags, LSF_SOLID) && !ogldata->cache.xor_on) {
		glLineStipple ((GLint)lstyle->scale16, (GLushort)lstyle->bits16);
		glEnable (GL_LINE_STIPPLE);
		if (XBIT_TEST(ogldata->card_entry->xbits, XBIT_LINE_PATTERN_NEEDS_BLENDING))
			ENSURE_BLEND_ON (ogldata);
	} else
		glDisable (GL_LINE_STIPPLE);
}

INLINE void ENSURE_LINE_PATTERN(OGLData * ogldata, Line_Style const & lstyle)
{
	if (ogldata->cache.line_style != lstyle)
		FORCE_LINE_PATTERN(ogldata, lstyle);
}

#define FORCE_LINE_WEIGHT(ogldata, width) \
	glLineWidth ((GLfloat)((ogldata)->cache.line_weight = (GLfloat)(width)))

#define ENSURE_LINE_WEIGHT(ogldata, lwidth)	SEMI_PROTECT(\
	if ((ogldata)->cache.line_weight != (GLfloat)(lwidth)) { \
		FORCE_LINE_WEIGHT(ogldata, lwidth); \
	} \
)

#ifndef OGL_MAX_LINE_WEIGHT
#  ifdef TEST_RIG_1
#    define OGL_MAX_LINE_WEIGHT 3
#  else
#    define OGL_MAX_LINE_WEIGHT 2
#  endif
#endif


INLINE bool LINE_STYLE_PUNT(OGLData * ogldata, Line_Rendition const & lr, int weight)
{
	Line_Style const &	line_style = lr->line_style;
	return (!ANYBIT(line_style->flags, LSF_SOLID) &&
			(XBIT_TEST(ogldata->card_entry->xbits, XBIT_BAD_PATTERNED_LINES) || /* can't trust this card's glLineStipple */
			 !ANYBIT(line_style->flags, LSF_BITS16) || /* no accurate 16-bit representation */
			 ANYBIT(line_style->flags, LSF_COMPLEX_ANYBITS)) ||
			weight > OGL_MAX_LINE_WEIGHT && !ANYBIT (ogldata->dc->options.debug, DEBUG_MITRE_TO_DEFAULT) ||
			weight > ogldata->max_line_weight);
}


INLINE void ENSURE_FACE_PATTERN(OGLData * ogldata, int fpattern)
{
	if (fpattern == FP_USER_DEFINED) {
		ogldata->cache.face_pattern = INVALID_CACHE;
		glPolygonStipple((GLubyte const *)ogldata->stipple_pattern);
	}
	else if (ogldata->cache.face_pattern != fpattern) {
		ogldata->cache.face_pattern = fpattern;
		glPolygonStipple((GLubyte const *)ogldata->fpatterns[fpattern]);
	}
}

#define FORCE_MATERIAL_COLOR_ON(ogldata)		SEMI_PROTECT(\
	ogldata->cache.colormaterial = true; \
	glEnable (GL_COLOR_MATERIAL); \
)

#define ENSURE_MATERIAL_COLOR_ON(ogldata)	SEMI_PROTECT(\
	if (!ogldata->cache.colormaterial) { \
		FORCE_MATERIAL_COLOR_ON(ogldata); \
	} \
)

#define FORCE_MATERIAL_COLOR_OFF(ogldata)	SEMI_PROTECT(\
	ogldata->cache.colormaterial = false; \
	glDisable (GL_COLOR_MATERIAL); \
)

#define ENSURE_MATERIAL_COLOR_OFF(ogldata)	SEMI_PROTECT(\
	if (ogldata->cache.colormaterial) { \
		FORCE_MATERIAL_COLOR_OFF(ogldata); \
	} \
)

#define FORCE_MATERIAL_COLOR_MODE(ogldata,mode)	SEMI_PROTECT(\
	ogldata->cache.colormaterialmode = mode; \
	glColorMaterial(mode,GL_AMBIENT_AND_DIFFUSE); \
)

#define ENSURE_MATERIAL_COLOR_MODE(ogldata,mode)	SEMI_PROTECT(\
	if (ogldata->cache.colormaterialmode != mode) { \
		FORCE_MATERIAL_COLOR_MODE(ogldata,mode); \
	} \
)


INLINE void FORCE_CUTTING_OFF(OGLData * ogldata)
{
	for (int i = 0; i < ogldata->n_cutting_planes_on; ++i)
		glDisable (GL_CLIP_PLANE0 + i);

	ogldata->n_cutting_planes_on = 0;
}

INLINE void ENSURE_CUTTING_OFF(OGLData * ogldata)
{
	if (ogldata->n_cutting_planes_on > 0)
		FORCE_CUTTING_OFF(ogldata);
}

INLINE void FORCE_CUTTING_ON(OGLData * ogldata)
{
	ASSERT(ogldata->n_cutting_planes_on == 0); 
	for (int i = 0; i < ogldata->n_cutting_planes_in_use; ++i)
		glEnable (GL_CLIP_PLANE0 + i);

	ogldata->n_cutting_planes_on = ogldata->n_cutting_planes_in_use;
}

INLINE void ENSURE_CUTTING_ON(OGLData * ogldata)
{
	if (ogldata->n_cutting_planes_on != ogldata->n_cutting_planes_in_use)
		FORCE_CUTTING_ON(ogldata);
}


#define FORCE_XPARENCY_ON(ogldata)		SEMI_PROTECT(\
	ogldata->transparency_on = true; \
	ENSURE_BLEND_ON(ogldata); \
)

#define ENSURE_XPARENCY_ON(ogldata)		SEMI_PROTECT (\
	if (!ogldata->transparency_on) \
		FORCE_XPARENCY_ON(ogldata); \
)

#define FORCE_XPARENCY_OFF(ogldata)		SEMI_PROTECT(\
	ogldata->transparency_on = false; \
	ENSURE_BLEND_OFF(ogldata); \
)

#define ENSURE_XPARENCY_OFF(ogldata)		SEMI_PROTECT(\
	if (ogldata->transparency_on) \
		FORCE_XPARENCY_OFF(ogldata); \
)

#define ENSURE_TRANSPARENCY(ogldata, set)		SEMI_PROTECT(\
	if ((set) && !ogldata->disable_blending)	ENSURE_XPARENCY_ON(ogldata);  \
	else	ENSURE_XPARENCY_OFF(ogldata); \
)


#define ENSURE_FOG_OFF(ogldata, nr) SEMI_PROTECT (\
	if (ogldata->fog_on) { \
	ogldata->fog_on = false; \
	glDisable (GL_FOG); \
	} \
)

INLINE void ENSURE_FOG_ON(OGLData * ogldata, Net_Rendition const & nr)
{
	Visualization_Rendition const &	vr = nr->visualization_rendition;
	ogldata->fog_on = true;
	glEnable (GL_FOG);
	if (vr->incarnation != ogldata->fog_incarnation) {
		Transform_Rendition const &		tr = nr->transform_rendition;
		float fog_color[4];
		ogldata->fog_incarnation = vr->incarnation;
		glFogi (GL_FOG_MODE, GL_LINEAR);
		Driver_Color const &	window_color = nr->window_rendition->window_color;
		fog_color[0] = window_color.direct_rgb.r * (1.0f / 255.0f);
		fog_color[1] = window_color.direct_rgb.g * (1.0f / 255.0f);
		fog_color[2] = window_color.direct_rgb.b * (1.0f / 255.0f);
		fog_color[3] = RGBAS32::Opaque_Alpha();
		glFogfv (GL_FOG_COLOR, fog_color);
		/* do not do pixel fog calculations */
		glHint (GL_FOG_HINT, GL_DONT_CARE);

		Camera_Handle const &	camera = tr->camera;
		if (camera->projection == Camera::Projection_PERSPECTIVE) {
			glFogf (GL_FOG_START, vr->fog.hither);
			glFogf (GL_FOG_END, vr->fog.yon);
		}
		else {
			glFogf (GL_FOG_START, vr->fog.hither - camera->single_data->eye_distance);
			glFogf (GL_FOG_END, vr->fog.yon - camera->single_data->eye_distance);
		}
	}
}

#define INVALIDATE_SCISSOR_SET(ogldata)  SEMI_PROTECT(\
	ogldata->cache.scissor_x = INVALID_CACHE; \
	ogldata->cache.scissor_y = INVALID_CACHE; \
	ogldata->cache.scissor_w = INVALID_CACHE; \
	ogldata->cache.scissor_h = INVALID_CACHE; \
)

#define FORCE_SCISSOR_OFF(ogldata)	SEMI_PROTECT(\
	INVALIDATE_SCISSOR_SET(ogldata); \
	ogldata->cache.scissor_mode = SCISSOR_MODE_OFF; \
	glDisable (GL_SCISSOR_TEST); \
)

#define ENSURE_SCISSOR_OFF(ogldata)	SEMI_PROTECT(\
	if (ogldata->cache.scissor_mode != SCISSOR_MODE_OFF) \
	FORCE_SCISSOR_OFF (ogldata); \
)

#define FORCE_SCISSOR_ON(ogldata)	SEMI_PROTECT(\
	INVALIDATE_SCISSOR_SET(ogldata); \
	ogldata->cache.scissor_mode = SCISSOR_MODE_ON; \
	glEnable (GL_SCISSOR_TEST); \
)

#define ENSURE_SCISSOR_ON(ogldata)	SEMI_PROTECT(\
	if (ogldata->cache.scissor_mode != SCISSOR_MODE_ON) \
	FORCE_SCISSOR_ON (ogldata); \
)

#define ENSURE_SCISSOR_SET(ogldata, x, y, w, h)  SEMI_PROTECT(\
	if ((x != ogldata->cache.scissor_x) || (y != ogldata->cache.scissor_y) || \
	(w != ogldata->cache.scissor_w) || (h != ogldata->cache.scissor_h)) { \
	glScissor ((ogldata->cache.scissor_x = x), (ogldata->cache.scissor_y = y), \
		   (ogldata->cache.scissor_w = w), (ogldata->cache.scissor_h = h)); \
	} \
)

#define INVALIDATE_PIXEL_ZOOM(ogldata)  SEMI_PROTECT(\
	ogldata->cache.pixel_zoom_x = 10000000.0f; \
	ogldata->cache.pixel_zoom_y = 10000000.0f; \
)

#define ENSURE_PIXEL_ZOOM(ogldata, x, y)  SEMI_PROTECT(\
	if (((x) != ogldata->cache.pixel_zoom_x) || ((y) != ogldata->cache.pixel_zoom_y)) { \
	glPixelZoom ((ogldata->cache.pixel_zoom_x = (x)), (ogldata->cache.pixel_zoom_y = (y))); \
	} \
)

#define INVALIDATE_PIXEL_UNPACK_ALIGNMENT(ogldata)  SEMI_PROTECT(\
	ogldata->cache.pixel_unpack_alignment = INVALID_CACHE; \
)

#define ENSURE_PIXEL_UNPACK_ALIGNMENT(ogldata, a)  SEMI_PROTECT(\
	if (((a) != ogldata->cache.pixel_unpack_alignment)) { \
		ogldata->cache.pixel_unpack_alignment = (a); \
	glPixelStorei (GL_PACK_ALIGNMENT, (a)); \
	glPixelStorei (GL_UNPACK_ALIGNMENT, (a)); \
	} \
)



#define FORCE_STENCIL_OFF(ogldata)	SEMI_PROTECT(\
	ogldata->cache.stencil_on = false; \
	glDisable (GL_STENCIL_TEST); \
)

#define ENSURE_STENCIL_OFF(ogldata)	SEMI_PROTECT(\
	if (ogldata->cache.stencil_on) \
		FORCE_STENCIL_OFF (ogldata); \
)

#define FORCE_STENCIL_ON(ogldata)	SEMI_PROTECT(\
	ogldata->cache.stencil_on = true; \
	glEnable (GL_STENCIL_TEST); \
)

#define ENSURE_STENCIL_ON(ogldata)	SEMI_PROTECT(\
	if (ogldata->cache.stencil_on != SCISSOR_MODE_ON) \
		FORCE_STENCIL_ON (ogldata); \
)





#define INVALIDATE_VIEWPORT_SET(ogldata)  SEMI_PROTECT(\
	ogldata->cache.viewport_x = INVALID_CACHE; \
	ogldata->cache.viewport_y = INVALID_CACHE; \
	ogldata->cache.viewport_w = INVALID_CACHE; \
	ogldata->cache.viewport_h = INVALID_CACHE; \
)

#define ENSURE_VIEWPORT_SET(ogldata, x, y, w, h)  SEMI_PROTECT(\
	if ((x != ogldata->cache.viewport_x) || (y != ogldata->cache.viewport_y) || \
	(w != ogldata->cache.viewport_w) || (h != ogldata->cache.viewport_h)) { \
	glViewport((ogldata->cache.viewport_x = x), (ogldata->cache.viewport_y = y), \
		   (ogldata->cache.viewport_w = w), (ogldata->cache.viewport_h = h)); \
	} \
)


#define FORCE_NORMALIZE_OFF(ogldata)	SEMI_PROTECT( \
	ogldata->cache.normalize = false; \
	glDisable (GL_NORMALIZE); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_NORMALIZE_OFF(ogldata)	SEMI_PROTECT( \
	if (ogldata->cache.normalize) \
		FORCE_NORMALIZE_OFF (ogldata); \
)

#define FORCE_NORMALIZE_ON(ogldata)	SEMI_PROTECT( \
	ogldata->cache.normalize = true; \
	glEnable (GL_NORMALIZE); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_NORMALIZE_ON(ogldata)	SEMI_PROTECT( \
	if (!ogldata->cache.normalize) \
		FORCE_NORMALIZE_ON (ogldata); \
)



#define FORCE_COLOR_ARRAY_ON(ogldata) SEMI_PROTECT( \
	ogldata->cache.color_array = true; \
	glEnableClientState(GL_COLOR_ARRAY); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define FORCE_COLOR_ARRAY_OFF(ogldata) SEMI_PROTECT( \
	ogldata->cache.color_array = false; \
	glDisableClientState(GL_COLOR_ARRAY); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_COLOR_ARRAY_ON(ogldata) SEMI_PROTECT( \
	if (!ogldata->cache.color_array) \
		FORCE_COLOR_ARRAY_ON(ogldata); \
)

#define ENSURE_COLOR_ARRAY_OFF(ogldata) SEMI_PROTECT( \
	if (ogldata->cache.color_array) \
		FORCE_COLOR_ARRAY_OFF(ogldata); \
)


#define FORCE_VERTEX_ARRAY_ON(ogldata) SEMI_PROTECT( \
	ogldata->cache.vertex_array = true; \
	glEnableClientState(GL_VERTEX_ARRAY); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define FORCE_VERTEX_ARRAY_OFF(ogldata) SEMI_PROTECT( \
	ogldata->cache.vertex_array = false; \
	glDisableClientState(GL_VERTEX_ARRAY); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_VERTEX_ARRAY_ON(ogldata) SEMI_PROTECT( \
	if (!ogldata->cache.vertex_array) {\
		FORCE_VERTEX_ARRAY_ON(ogldata); \
	} \
)

#define ENSURE_VERTEX_ARRAY_OFF(ogldata) SEMI_PROTECT( \
	if (ogldata->cache.vertex_array) { \
		FORCE_VERTEX_ARRAY_OFF(ogldata); \
	} \
)


#define FORCE_NORMAL_ARRAY_ON(ogldata) SEMI_PROTECT( \
	ogldata->cache.normal_array = true; \
	glEnableClientState(GL_NORMAL_ARRAY); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define FORCE_NORMAL_ARRAY_OFF(ogldata) SEMI_PROTECT( \
	ogldata->cache.normal_array = false; \
	glDisableClientState(GL_NORMAL_ARRAY); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_NORMAL_ARRAY_ON(ogldata) SEMI_PROTECT( \
	if (!ogldata->cache.normal_array) \
		FORCE_NORMAL_ARRAY_ON(ogldata); \
)

#define ENSURE_NORMAL_ARRAY_OFF(ogldata) SEMI_PROTECT( \
	if (ogldata->cache.normal_array) \
		FORCE_NORMAL_ARRAY_OFF(ogldata); \
)

INLINE void FORCE_ACTIVE_TEXTURE(OGLData * ogldata, int texunit)
{
	ogldata->_glClientActiveTextureARB (GL_TEXTURE0 + texunit);
	ogldata->cache.active_texture = texunit;
}

INLINE void ENSURE_ACTIVE_TEXTURE(OGLData * ogldata, int texunit)
{
	if (ogldata->cache.active_texture != texunit)
		FORCE_ACTIVE_TEXTURE(ogldata,texunit);
}

INLINE void FORCE_TEXTURE_COORD_ARRAY(OGLData * ogldata, int unit, bool mode)
{
	ogldata->cache.param_array[unit] = mode;
	ENSURE_ACTIVE_TEXTURE (ogldata,unit);
	if (mode)
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	else
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


INLINE void ENSURE_TEXTURE_COORD_ARRAY(OGLData * ogldata, int unit, bool mode)
{
	if (mode != ogldata->cache.param_array[unit])
		FORCE_TEXTURE_COORD_ARRAY(ogldata,unit,mode);
	else if (mode == true)
		ENSURE_ACTIVE_TEXTURE (ogldata, unit); /* to avoid redundant setting */
}

INLINE void GL_BIND_ARRAY_BUFFER(OGLData * ogldata, GLuint buffer)
{
	if (buffer != ogldata->cache.array_buffer) {
		ogldata->cache.array_buffer = buffer;
		ogldata->_glBindBufferARB(GL_ARRAY_BUFFER_ARB, buffer);
	}
}

INLINE void GL_BIND_ELEMENT_BUFFER(OGLData * ogldata, GLuint buffer)
{
	if (buffer != ogldata->cache.element_buffer) {
		ogldata->cache.element_buffer = buffer;
		ogldata->_glBindBufferARB(GL_ELEMENT_BUFFER_ARB, buffer);
	}
}


#define FORCE_TEXTURE_WRAP_S(ogldata, target, mode, texture_unit)	SEMI_PROTECT( \
	ENSURE_TEXTURE_UNIT(ogldata, texture_unit); \
	ogldata->cache.texture_wrap_s[texture_unit] = mode; \
	glTexParameteri (target, GL_TEXTURE_WRAP_S, mode); \
)

#define ENSURE_TEXTURE_WRAP_S(ogldata, target, mode, texture_unit)	SEMI_PROTECT( \
	if (ogldata->cache.texture_wrap_s[texture_unit] != mode) \
		FORCE_TEXTURE_WRAP_S(ogldata, target, mode, texture_unit); \
)

#define FORCE_TEXTURE_WRAP_T(ogldata, target, mode, texture_unit)	SEMI_PROTECT( \
	ENSURE_TEXTURE_UNIT(ogldata, texture_unit); \
	ogldata->cache.texture_wrap_t[texture_unit] = mode; \
	glTexParameteri (target, GL_TEXTURE_WRAP_T, mode); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_TEXTURE_WRAP_T(ogldata, target, mode, texture_unit)	SEMI_PROTECT( \
	if (ogldata->cache.texture_wrap_t[texture_unit] != mode) \
		FORCE_TEXTURE_WRAP_T(ogldata, target, mode, texture_unit); \
)

#define FORCE_TEXTURE_ENV_MODE(ogldata, mode, texture_unit)	SEMI_PROTECT( \
	ENSURE_TEXTURE_UNIT(ogldata, texture_unit); \
	ogldata->cache.texture_env_mode[texture_unit] = mode; \
	glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, mode); \
	ASSERT (!ogldata->cache.compiling_display_lists); \
)

#define ENSURE_TEXTURE_ENV_MODE(ogldata, mode, texture_unit)	SEMI_PROTECT( \
	if (ogldata->cache.texture_env_mode[texture_unit] != mode) \
	FORCE_TEXTURE_ENV_MODE(ogldata, mode, texture_unit); \
)

#define FORCE_TEXTURE_PERSPECTIVE_CORRECTION_ON(ogldata, texture_unit)	SEMI_PROTECT(\
	ENSURE_TEXTURE_UNIT(ogldata, texture_unit); \
	ogldata->cache.texture_perspective_correction[texture_unit] = 1; \
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); \
)

#define ENSURE_TEXTURE_PERSPECTIVE_CORRECTION_ON(ogldata, texture_unit)	SEMI_PROTECT(\
	if (ogldata->cache.texture_perspective_correction[texture_unit] != 1) \
		FORCE_TEXTURE_PERSPECTIVE_CORRECTION_ON(ogldata, texture_unit); \
)

#define FORCE_TEXTURE_PERSPECTIVE_CORRECTION_OFF(ogldata, texture_unit)	SEMI_PROTECT(\
	ENSURE_TEXTURE_UNIT(ogldata, texture_unit); \
	ogldata->cache.texture_perspective_correction[texture_unit] = 0; \
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST); \
)

#define ENSURE_TEXTURE_PERSPECTIVE_CORRECTION_OFF(ogldata, texture_unit)	SEMI_PROTECT(\
	if (ogldata->cache.texture_perspective_correction[texture_unit] != 0) \
		FORCE_TEXTURE_PERSPECTIVE_CORRECTION_OFF(ogldata, texture_unit); \
)

#define FORCE_TEXTURE_GEN_OFF(ogldata, texture_unit)	SEMI_PROTECT(\
	ogldata->cache.texture_gen[texture_unit] = false; \
	glDisable (GL_TEXTURE_GEN_S); \
	glDisable (GL_TEXTURE_GEN_T); \
)

#define ENSURE_TEXTURE_GEN_OFF(ogldata, texture_unit)	SEMI_PROTECT(\
	if (ogldata->cache.texture_gen[texture_unit]) \
		FORCE_TEXTURE_GEN_OFF (ogldata, texture_unit); \
)

#define FORCE_TEXTURE_GEN_ON(ogldata, texture_unit)	SEMI_PROTECT(\
	ogldata->cache.texture_gen[texture_unit] = true; \
	glEnable (GL_TEXTURE_GEN_S); \
	glEnable (GL_TEXTURE_GEN_T); \
)

#define ENSURE_TEXTURE_GEN_ON(ogldata, texture_unit)	SEMI_PROTECT(\
	if (!ogldata->cache.texture_gen[texture_unit]) \
		FORCE_TEXTURE_GEN_ON (ogldata, texture_unit); \
)

#define FORCE_TEXTURE_GEN_MODE(ogldata, mode, texture_unit)	SEMI_PROTECT(\
	ENSURE_TEXTURE_UNIT(ogldata, texture_unit); \
	ogldata->cache.texture_gen_mode[texture_unit] = mode; \
	glTexGenf (GL_S, GL_TEXTURE_GEN_MODE, mode); \
	glTexGenf (GL_T, GL_TEXTURE_GEN_MODE, mode); \
)

#define ENSURE_TEXTURE_GEN_MODE(ogldata, mode, texture_unit)  SEMI_PROTECT(\
	if (ogldata->cache.texture_gen_mode[texture_unit] != mode) \
		FORCE_TEXTURE_GEN_MODE(ogldata, mode, texture_unit); \
)





#define INVALIDATE_COLOR_AND_LIGHT_A_D(ogldata) SEMI_PROTECT(\
	(ogldata)->cache.color = RGBAS32::Invalid(); \
	(ogldata)->cache.material.a_d.red = -1.0f; \
	(ogldata)->cache.material.a_d.green = -1.0f; \
	(ogldata)->cache.material.a_d.blue = -1.0f; \
	(ogldata)->cache.material.a_d.alpha = -1.0f; \
)


#define FORCE_LOCAL_VIEWER(ogldata, mode) SEMI_PROTECT(\
	(ogldata)->cache.local_viewer = mode;\
	glLightModeli (GL_LIGHT_MODEL_LOCAL_VIEWER, (mode?GL_TRUE:GL_FALSE)); \
)

#define ENSURE_LOCAL_VIEWER(ogldata, mode) SEMI_PROTECT(\
	if ((ogldata)->cache.local_viewer != mode) { \
		FORCE_LOCAL_VIEWER(ogldata, mode); \
	} \
)




#define ENSURE_DC_XFORM(ogldata, nr) SEMI_PROTECT(\
	if ((ogldata)->tr_incarnation != INDC ((nr)->transform_rendition->incarnation)) { \
		OGL_SET_DC_XFORM ((ogldata), (nr)); \
	} \
)


/* update the ref_matrix_negative_3x3 boolean, and make sure handedness stays 
 * in synch.  (ref_matrix_negative_3x3 tracks whether geometry has been put 
 * through a geometry reference with a negative scale matrix */
#define ENSURE_NEG3X3(ogldata, tr, val) SEMI_PROTECT(\
	if (val != ogldata->ref_matrix_neg3x3) { \
		ogldata->ref_matrix_neg3x3 = val; \
		OGL_SET_HANDEDNESS (ogldata, tr, 0); \
	} \
	else \
		ogldata->ref_matrix_neg3x3 = val; \
)


INLINE void ENSURE_3D_XFORM(OGLData * ogldata, Net_Rendition const & nr)
{
	Transform_Rendition const &	tr = nr->transform_rendition;
	if (ogldata->tr_incarnation != IN3D (tr->incarnation)) {
		OGL_SET_3D_XFORM (ogldata, nr);
	}
	else {
		Display_Context const *	dc = nr->display_context;
		bool neg3x3 = false;
		if (dc->reference_list &&
			dc->reference_list->modelling_matrix)
			neg3x3 = dc->reference_list->modelling_matrix->matrix->Is_Determinant3x3_Negative();
		ENSURE_NEG3X3 (ogldata, tr, neg3x3);
	}
}

INLINE void ENSURE_ADJOINT_Z_VALID(OGLData * ogldata, Net_Rendition const & nr)
{
	if (!ogldata->adjoint_Z_valid) {
		Matrix_Single		am = nr->transform_rendition->matrix->adjoint();
		Matrix_Access<float> const *	adjoint = (Matrix_Access<float> const *)am->Elements();
		ogldata->adjoint_Z[0] = adjoint[2][0];
		ogldata->adjoint_Z[1] = adjoint[2][1];
		ogldata->adjoint_Z[2] = adjoint[2][2];
		ogldata->adjoint_Z[3] = adjoint[2][3];
		ogldata->adjoint_Z_valid = true;
	} 
}

#define FORCE_LIST_BASE(ogldata, base)	SEMI_PROTECT(\
	ogldata->cache.list_base = base; \
	glListBase (base); \
)

#define ENSURE_LIST_BASE(ogldata, base)	SEMI_PROTECT(\
	if (ogldata->cache.list_base != base) \
	FORCE_LIST_BASE(ogldata, base); \
)

#define INVALIDATE_LIST_BASE(ogldata) SEMI_PROTECT(\
	ogldata->cache.list_base = INVALID_CACHE; \
)

#define ENSURE_CLIP_VOLUME_HINT(ogldata, hint) SEMI_PROTECT(\
	if (ogldata->supports_clip_volume_hint && ogldata->cache.volume_hint != hint) { \
		glHint(GL_CLIP_VOLUME_CLIPPING_HINT_EXT,(ogldata->cache.volume_hint=hint)); \
	} \
)


#define ENSURE_POINT_SIZE(ogldata,size) SEMI_PROTECT(\
	if (ogldata->cache.point_size != size) { \
		glPointSize(size); \
		ogldata->cache.point_size = size; \
	} \
)

#define ENSURE_POINT_SMOOTH(ogldata,onoff) SEMI_PROTECT(\
	if (ogldata->cache.point_smooth != onoff) { \
		if (onoff) \
			glEnable (GL_POINT_SMOOTH); \
		else \
			glDisable (GL_POINT_SMOOTH); \
		ogldata->cache.point_smooth = onoff; \
	} \
)

/*
 * HELPER
 */
INLINE void COLOR3(bool force_gray, RGB const & color)
{
	if (force_gray) {
		float cc[3];
		cc[0] = cc[1] = cc[2] = color.Gray();
		glColor3fv ((float const *)(cc));
	}
	else {
		glColor3fv ((float const *)&color);
	}
}



#ifdef OSX_SYSTEM
#define ARRAY_TRISTRIPS_MIN_COUNT 0
#else
#define ARRAY_TRISTRIPS_MIN_COUNT 20
#endif


#  define OGL_ALLOC_TEXTURE(id) do { id=0 ; glGenTextures (1, &id); }while (0)
#  define OGL_FREE_TEXTURE(id) do { glDeleteTextures (1, &id); id=0;}while (0)

/* on AGL_DRIVER or quartz driver, quartz data is on dc->data */
#define OGLD(dc) ((OGLData *)((dc)->data2))
#define OGLNRD(nr) (OGLD((nr)->display_context))

#ifdef GLX_DRIVER
#  define X11D(dc) ((X11Data *)((dc)->data))
#  define X11NRD(nr) (X11D(nr->display_context))
#endif



/* weak imports for run-time detection of features that first became available
 * after MacOS 10.2, and declaration of stubs for compiles on older OS versions */
#ifdef AGL_DRIVER
#  if (MAC_OS_X_VERSION_MAX_ALLOWED < MAC_OS_X_VERSION_10_3)
  typedef CGContextRef (*CGGLContextCreateProc) (void *glContext, CGSize size, CGColorSpaceRef colorspace);
  typedef GLboolean (*aglSetPBufferProc) (AGLContext, AGLPbuffer, GLint, GLint, GLint);
  typedef GLboolean (*aglCreatePBufferProc) (GLint, GLint, GLenum, GLenum, long, AGLPbuffer *);
  typedef GLboolean (*aglDestroyPBufferProc) (AGLPbuffer);
  extern CGGLContextCreateProc CGGLContextCreate;
  extern aglSetPBufferProc aglSetPBuffer;
  extern aglCreatePBufferProc aglCreatePBuffer;
  extern aglDestroyPBufferProc aglDestroyPBuffer;
#  else
  extern CGContextRef CGGLContextCreate (
	void *glContext, 
	CGSize size, 
	CGColorSpaceRef colorspace
)  __attribute__((weak_import));

  extern GLboolean aglSetPBuffer (
	AGLContext ctx, 
	AGLPbuffer pbuffer, 
	GLint face, 
	GLint level, 
	GLint screen 
)  __attribute__((weak_import));

  extern GLboolean aglCreatePBuffer (
	GLint width, 
	GLint height, 
	GLenum target, 
	GLenum internalFormat, 
	long max_level, 
	AGLPbuffer *pbuffer
)  __attribute__((weak_import));

  extern GLboolean aglDestroyPBuffer (
	AGLPbuffer pbuffer
)  __attribute__((weak_import));
#  endif
#endif


local void init_ogl_actions (OGLData * ogldata);

local void identify_renderer (Display_Context * dc);
local void get_extension_specific_actions (OGLData * ogldata);

local void clear_screen (Display_Context const * dc);

local void turn_off_lights (OGLData * ogldata);
local void turn_on_lights (OGLData * ogldata, Net_Rendition const &  nr, Light_Rendition const & lr);

local void set_dc_xform (OGLData * ogldata, Net_Rendition const &  nr);
local void set_3d_xform (OGLData * ogldata, Net_Rendition const &  nr);
local void set_handedness (OGLData * ogldata, Transform_Rendition const & tr, int flags);

local void set_line_rendition (OGLData * ogldata, Net_Rendition const &  nr, Line_Rendition const & lr);
local void set_material (OGLData * ogldata, Net_Rendition const &  nr, Material_Rendition const & matr, RGB const *use_color, RGB const *use_specular_color);
local void set_texture_indexed_material (OGLData * ogldata, Net_Rendition const &  nr, Material_Rendition const & matr);


local void toggle_xor(Net_Rendition const & nr, bool undrawing);


local void insert_colormap_texture_image  (Net_Rendition const &  nr, GLint width, int use_mipmaps);

local GLuint define_texture (
	OGLData * ogldata, 
	Net_Rendition const &  nr, 
	Texture *texture, 
	Material_Components usage,
	int texture_unit);
local bool bind_texture (
	OGLData *ogldata,
	Net_Rendition const &  nr, 
	Texture *texture, 
	OGL_Render_Control_Flags rcf,
	Material_Components usage,
	GLuint id,
	int texture_unit); 

local bool define_interpolation_texture (OGLData * ogldata, Net_Rendition const &  nr);


local void really_draw_3d_tristrip (Net_Rendition const & nr, Tristrip const *ts, OGLDL_Bin *bin);
local void really_draw_indexed_tristrip (Net_Rendition const & nr, Tristrip const *ts, OGLDL_Bin *bin);


local void really_draw_3d_polyedge (Net_Rendition const &  nr, Line_Rendition const & er, Polyedge const * pe, OGLDL_Bin *bin);


local bool collect_polygon_list (Net_Rendition const & nr, Polygon const * g, bool single);


local bool start_device (Display_Context * dc);
local void stop_device (Display_Context * dc);
local void init_picture (Net_Rendition const &  nr);
local void finish_picture (Net_Rendition const &  nr,
				bool swap_buffers);
local void finish_update (Display_Context const * dc);
local void init_update (Display_Context const * dc);
local intptr_t get_outer_window (Display_Context const *const_dc);
local void free_outer_window (Display_Context const *dc);
local void resize_outer_window (Display_Context const *dc);
local void get_current_info (Display_Context * dc);
local void get_physical_info (Display_Context * dc);

#if 0
local bool request_location (
	Display_Context const * dc,
	int * button,
	int * x, 
	int * y);
local bool request_keyboard (
	Display_Context const * dc,
	int * button, 
	int * status);
#endif
		
local bool common_make_fbo_current(OGLData * ogldata);
local bool common_setup_fbo(OGLData * ogldata, bool test_it_only);
local void common_cleanup_fbo(OGLData * ogldata);

template <typename T>
local bool get_display_list(Net_Rendition const & nr,
						Display_List **linker,
						Display_List *& dl,
						T *&odl,
						unsigned int type);

local void release_vbo (OGL_VBO *vbo); 
local void release_polymarker_vbo (OGL_Polymarker_VBO *vbo); 

local void define_cutting_planes (Net_Rendition const & nr, Cutting_Plane_Instance const *cp);
local bool punt_3d_tristrip (Net_Rendition const &nr, Strip_Flags special_flags, bool is_transparent);
local bool punt_3d_polyedge (Net_Rendition const &nr, Line_Rendition const &er, Polyedge const *pe, int weight);
local bool punt_3d_polymarker (Net_Rendition const &nr, Marker_Rendition const &kr, Polymarker const *pm, int *glyph_id_out);
local void kill_ts_display_list (Net_Rendition const & nr, Tristrip const *ts);
local void setup_passes (Net_Rendition const &		nr,
						OGL_Render_Control *		rc,
						Tristrip const *			ts);
local bool check_tristrip_display_list_valid (
						Net_Rendition const &		nr,
						Tristrip const *			ts,
						Display_List const *		dl,
						Geometry_OGLDL const *		godl);
local bool pre_pass(	Net_Rendition const &		nr,
						OGL_Render_Control *		rc,
						Tristrip const *			ts,
						bool						reverse = false);
local bool post_pass (	Net_Rendition const & nr,
						int32_t pw,
						Parameter_Flags pf,
						OGL_Render_Control *rc);

local int tristrip_vram_usage (Tristrip const *ts); 
local bool append_polyline_to_collector (
						Net_Rendition const &nr, 
						Geometry const *g,
						bool single);



void clear_z_buffer (Net_Rendition const &  nr, IntRectangle const & area);


void invalidate_cache (OGLData * ogldata);

void common_free_outer_window(Display_Context const * dc);

void common_finish_picture_begin(Net_Rendition const & nr);
void common_finish_picture_end(Net_Rendition const & nr);

void read_output_image_texture(Net_Rendition const & nr);

void create_region (Net_Rendition const &  nr, IntRectangle const * area, void **image, void **z);
void save_region (Net_Rendition const &  nr, IntRectangle const * area, void * image, void * z);
void restore_region (Net_Rendition const &  nr, IntRectangle const * area, void * image, void * z);
void destroy_region (Net_Rendition const &  nr, void * image, void * z);


void show_image_data (Display_Context const * dc, void *list);

void update_physical_info (Display_Context * dc);

void check_display_list(Net_Rendition const & nr, Display_List ** dll, uint32_t * mask_out, uint32_t * viz_out);


void setup_ogl (Display_Context * dc);
void init_ogldata (Display_Context * dc);


void draw_3d_nurbs_curve (Net_Rendition const &  nr, NURBS_Curve const * curve);

void draw_dc_polydot (Net_Rendition const &  nr, int count, DC_Point const * where);
void draw_dc_polyline (Net_Rendition const &  nr, int count, DC_Point const * points);
void draw_dc_rectangle (Net_Rendition const &  nr, int left, int right, int bottom, int top);
void draw_dc_polytriangle (Net_Rendition const &  nr, int count, DC_Point const * points);
void draw_dc_colorized_polytriangle (Net_Rendition const &  nr, int count, DC_Point const * points, RGBAS32 const * color, bool single);
void draw_dc_gouraud_polytriangle (Net_Rendition const &  nr, int count, DC_Point const * points, RGBAS32 const * colors);
void draw_dc_gouraud_polyline (Net_Rendition const &  nr, int count, DC_Point const *points, RGBAS32 const *colors);
void draw_dc_face (Net_Rendition const &  nr, int count, DC_Point const * points);
void draw_dc_colorized_face (Net_Rendition const &  nr, int count, DC_Point const * points, RGBAS32 const * color);
void draw_dc_textured_polytriangle (Net_Rendition const &  nr, int count, DC_Point const * points, RGBA const * color, Plane const * plane, DPlane const * dplane, Parameter const * vertex_params, int32_t param_width, Parameter_Flags param_flags);
void draw_dc_rgb32_rasters (Net_Rendition const &  nr, DC_Point const * start, DC_Point const * end, int row_bytes, RGBAS32 const * raster);
void draw_3d_image(Net_Rendition const & nr, Image const * image);
void draw_3d_polycylinder (Net_Rendition const & inr, PolyCylinder const * pc);
void draw_3d_cylinder (Net_Rendition const & nr, Cylinder const * cylinder);
void draw_3d_sphere (Net_Rendition const & nr, Sphere const * sphere);
void draw_indexed_polyedge (Net_Rendition const & nr, Polyedge const * pe);
void draw_3d_grid (Net_Rendition const & nr, Grid const * grid);

void draw_dc_colorized_polydot (Net_Rendition const & nr, int count, DC_Point const * points, RGBAS32 const * clr, bool single);
void draw_dc_colorized_polyline (Net_Rendition const & nr, int count, DC_Point const * points, RGBAS32 const * colors_in, bool single);

void draw_3d_tristrip (Net_Rendition const & nr, Tristrip const * ts);
void draw_indexed_tristrip (Net_Rendition const & nr, Tristrip const * ts);

void draw_3d_polyedge (Net_Rendition const &  nr, Polyedge const * pe);
void draw_3d_marker (Net_Rendition const &  nr, Marker const * m);
void draw_3d_polymarker (Net_Rendition const &  nr, Polymarker const * pm);

Geometry const * segment_render (Net_Rendition const & nr, Geometry const * g, Action_Mask mask, bool single);
void draw_3d_polyline (Net_Rendition const & nr, Polyline const * polyline);
void draw_3d_polygon (Net_Rendition const & nr, Polygon const * polygon);
void draw_3d_ellipse (Net_Rendition const & nr, Ellipse const * ellipse);

void draw_3d_polyedge_from_tristrip (Net_Rendition const & nr, Polyedge const * pe, Tristrip const * ts);

bool snapshot(Net_Rendition const & nr, int width, int height, unsigned char * data);

void * im_utility(
	Net_Rendition const &	nr,
	long					request,
	void *					pointer,
	intptr_t				v1,
	intptr_t				v2,
	intptr_t				v3,
	intptr_t				v4,
	float					f1,
	float					f2,
	float					f3,
	float					f4);

bool begin_display_list(
	Net_Rendition const &	nr,
	Display_List **			dll,
	uint32_t *				mask,
	bool					compile_only);

void end_display_list(
	Net_Rendition const &	nr,
	Display_List **			dll,
	bool					compile_only);

void execute_display_list (
	Net_Rendition const &	nr,
	Display_List const *	dl);

void flush_display_lists (Display_Context const *dc);

void release_display_list (Driver_DL *	bddl); 

extern XBits xbit_table[];

End_HOOPS_OGL_Namespace


#endif
/*OGL_H_DEFINED*/










