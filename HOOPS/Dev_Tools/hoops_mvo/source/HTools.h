// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef _HTOOLS_H
#define _HTOOLS_H

#include "utf_utils.h"

#ifdef H_PACK_8
#pragma pack(push)
#pragma pack(8)
#endif


#ifdef _MSC_VER

#ifdef _DEBUG
#include <crtdbg.h>
#endif


#ifndef WINDOWS_SYSTEM
#define WINDOWS_SYSTEM
#endif
#ifndef MVO_STATIC_LIB
	#ifdef MVO_EXPORTS
		#define MVO_API  __declspec (dllexport)
	#else
		#define MVO_API  __declspec (dllimport)
	#endif
#else
	#define MVO_API
#endif

#elif defined(LINUX_SYSTEM)
    #define MVO_API __attribute__ ((visibility ("default")))
#else
	#define MVO_API
#endif


#ifdef NEED_BOOL_TYPE
#undef NEED_BOOL_TYPE
# ifdef true
#   undef true
# endif
# ifdef false
#   undef false
# endif
    typedef int bool;

    const bool true = !0;
    const bool false = 0;
#endif


#ifndef MVO_POINTER_SIZED_INT
#ifdef POINTER_SIZED_INT
#   define MVO_POINTER_SIZED_INT POINTER_SIZED_INT
#   define MVO_POINTER_SIZED_UINT unsigned POINTER_SIZED_INT 
#else
#if defined(WIN64) || defined(_M_X64) || defined(_WIN64) || defined(_M_AMD64)
#   define MVO_POINTER_SIZED_INT __int64
#   define MVO_POINTER_SIZED_UINT unsigned __int64
#else
#   define MVO_POINTER_SIZED_INT long
#   define MVO_POINTER_SIZED_UINT unsigned long
#endif
#endif
#endif

#define MVO_V2I(x) ((MVO_POINTER_SIZED_INT)(x))
#define MVO_I2V(x) ((void *)(MVO_POINTER_SIZED_INT)(x))


#if defined (_DEBUG) && !defined (_WIN32_WCE)
#include <assert.h>
#define H_ASSERT(x)	assert(x)
#else
#define H_ASSERT(x)
#endif

#define MVO_BIGENDIAN 0

#ifdef WINDOWS_SYSTEM
#	ifdef _WIN32_WCE
#	define stricmp strcasecmp
#	define wcsicmp wcscasecmp
#	endif
#	define NO_CASE_CMP(x,y) _stricmp(x,y)
#	define WNO_CASE_CMP(x,y) wcsicmp(x,y)
#   define _tcseq(a,b)		(!_tcscmp(a,b))
#   define _tcsieq(a,b)		(!_tcsicmp(a,b))
#else
#	define NO_CASE_CMP(x,y) strcasecmp(x,y)
#	define WNO_CASE_CMP(x,y) wcscasecmp(x,y)
#endif

#define streq(a,b)      (!strcmp(a,b))
#define wcseq(a,b)		(!wcscmp(a,b))
#define strieq(a,b)     (!NO_CASE_CMP(a,b))
#define wcsieq(a,b)		(!WNO_CASE_CMP(a,b))

#define feq(a, b, eps)	((fabs((a)-(b)) < (eps)))

/* define MIN and MAX here rather than earlier in the file because
 * some system headers define their own (compatible) macros */
#ifndef MIN
#	define MIN(a, b)		((a) < (b) ? (a) : (b))
#endif
#ifndef MAX
#	define MAX(a, b)		((a) > (b) ? (a) : (b))
#endif

#ifndef H_M_PI
#	define H_M_PI (3.14159265358979323846264338327950288)
#endif
#undef H_ACOS //in case someone else already defined this
#undef H_ASIN
#define H_ACOS(x) (((x)<-1)?(H_M_PI):(((x)>1)?(0):(acos(x))))
#define H_ASIN(x) (((x)<-1)?(-H_M_PI/2):(((x)>1)?(H_M_PI/2):(asin(x))))

	

#ifndef H_UNICODE_STRLEN
#		define H_UNICODE_STRLEN(x) HUtility::strlen16(x)
#endif

#ifndef UNREFERENCED
#       define UNREFERENCED(param) ((void)(param))
#endif

#define H_SAFE_DELETE(any__pointer_)	\
	do {								\
		delete any__pointer_;			\
		any__pointer_ = 0;				\
	} while (0)

#define H_SAFE_DELETE_ARRAY(any__pointer_)	\
	do {									\
		delete[] any__pointer_;				\
		any__pointer_ = 0;					\
	} while (0)

#define H_SAFE_DELETE_VLIST(any__vlist_)	\
	do {									\
		delete_vlist(any__vlist_);			\
		any__vlist_ = 0;					\
	} while (0)

#define H_SAFE_DELETE_VLIST_AND_NODES(TYPE, LIST)					\
	do {															\
		if (LIST) {													\
			vlist_reset_cursor(LIST);								\
			while (1) {												\
				TYPE *temp_node;									\
				if (!(temp_node = (TYPE *)vlist_peek_cursor(LIST)))	\
					break;											\
				delete temp_node;									\
				vlist_advance_cursor(LIST);							\
			}														\
			delete_vlist(LIST);										\
			LIST = 0;												\
		}															\
	} while (0)


// The definition of HC_KEY is required by almost all files
// hence hc.h is included here. Ensure the htools.h is included
// first in any new files you add to MVO - Rajesh B (01-Oct-01)
#include "hc.h"

#include "HGlobals.h"

// HCLOCALE does whatever operation it is given in the standard "C" locale,
// rather than whatever had been defined previously.  This prevents things like
// sprintf("%f", 3.14f) from coming out as "3,14" when set to european locales (or
// those of most of the rest of the world, for that matter.

#ifdef _WIN32_WCE
#define HCLOCALE(x) x
#else
#include <locale.h>
#include <string.h>

#ifdef _MSC_VER
#define HCLOCALE(x) do { \
	int was_type = _configthreadlocale(0); \
	if (was_type != _ENABLE_PER_THREAD_LOCALE) \
		_configthreadlocale(_ENABLE_PER_THREAD_LOCALE); \
	char saved_locale[4096] = {""}; \
	char const * internal_locale_buffer = setlocale(LC_ALL, 0); \
	if (internal_locale_buffer != 0) \
		strncpy_s(saved_locale, internal_locale_buffer, sizeof(saved_locale)); \
	setlocale(LC_ALL, "C"); \
	x; \
	setlocale(LC_ALL, saved_locale); \
	if (was_type != _ENABLE_PER_THREAD_LOCALE) \
		_configthreadlocale(was_type); \
} while(0)
#else
#define HCLOCALE(x) do { \
	char saved_locale[4096] = {""}; \
	char const * internal_locale_buffer = setlocale(LC_ALL, 0); \
	if (internal_locale_buffer != 0) \
	strncpy(saved_locale, internal_locale_buffer, sizeof(saved_locale)); \
	setlocale(LC_ALL, "C"); \
	x; \
	setlocale(LC_ALL, saved_locale); \
} while(0)
#endif


#endif

#define H_EXTRA_POINTER_FORMAT HUtility::extra_pointer_format() 

#ifndef INVALID_KEY
#define INVALID_KEY (-1L)
#endif

#ifndef NO_RELATED_SELECTION_LIMIT
#define NO_RELATED_SELECTION_LIMIT (-1)
#endif

// define size for max float -- especially useful for limiting camera components that get squared
#define MVO_SQRT_MAX_FLOAT					1e15f

// define size for HNet message buffers
#define MVO_MESSAGE_BUFFER_SIZE						8192

// define size for holding segment pathnames
#define MVO_SEGMENT_PATHNAME_BUFFER			4096

// define size for general buffers on stack
#define MVO_BUFFER_SIZE						4096

// define size for small buffers on stack
#define MVO_SMALL_BUFFER_SIZE						256

#define MVO_BIG_BUFFER_SIZE					32768

#ifndef SWIG		//SWIG messes up on these
// Magic numbers which we use to hang pointers to various things in MVO from the HOOPS
// scene graph using HC_Set_User_Index
#define H_WHITE_BACKGROUND_TEXT_MAGIC   (('w'<<24) | ('b'<<16) | ('t'<<8) | ('m'))
#define H_DEBUG_ZBUFFER_MAGIC	    (('h'<<24) | ('d'<<16) | ('z'<<8) | ('b'))
#define H_DISABLE_SCALE_MAGIC	    (('h'<<24) | ('d'<<16) | ('s'<<8) | ('r'))
#define H_HBASEVIEW_POINTER_MAGIC   (('v'<<24) | ('p'<<16) | ('t'<<8) | ('r'))
#define H_HIGHLIGHT_KEY_MAGIC       (('h'<<24) | ('h'<<16) | ('l'<<8) | ('k'))
#define H_SOLID_BODY_MAGIC          (('b'<<24) | ('o'<<16) | ('d'<<8) | ('y'))
#define H_SUPPRESS_SCALING_MAGIC    (('s'<<24) | ('u'<<16) | ('p'<<8) | ('p'))
#define H_TEMP_SEGMENT_MAGIC        (('h'<<24) | ('t'<<16) | ('s'<<8) | ('m'))
#define HTC_OBJECT_ROTATE_MAGIC     (('h'<<24) | ('t'<<16) | ('o'<<8) | ('r'))
//#define H_MARKUP_POS_MAGIC			(('h'<<24) | ('m'<<16) | ('p'<<8) | ('m'))
#endif

#ifndef _WIN32_WCE
#include <assert.h>
#else  
#define assert(exp)    ((void)0)
#endif

#ifdef H_PACK_8
#pragma pack(pop)
#endif

#endif
