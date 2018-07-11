// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HOOPS_DEFINED
#define HOOPS_DEFINED

#ifdef _MSC_VER
// #define _SX(x) #x
// #define _S(x) _SX(x)
// #pragma message("_MSC_FULL_VER: " _S(_MSC_FULL_VER))
# if _MSC_VER == 1700
#  if _MSC_FULL_VER < 170061030
	static_assert(false, "You must install Visual Studio 11 (2012) Update 4");
#  endif
# elif _MSC_VER == 1800
#  if _MSC_FULL_VER < 180021005
	static_assert(false, "You must install Visual Studio 12 (2013) Update 1");
#  endif
# else
#  pragma message ("warning: unsupported version of Visual Studio detected")
# endif
#endif

#if !defined(HPS_CORE_BUILD) && !defined(HOOPS_CORE_BUILD)
	static_assert(false, "One of these must be defined!");
# endif

#if defined(HPS_CORE_BUILD) && defined(HOOPS_CORE_BUILD)
	static_assert(false, "Only one of these can be defined!");
# endif

#ifdef _MSC_VER
#	pragma warning (disable: 4127)	// conditional expression is constant
#	pragma warning (disable: 4996)	// standard string functions considered unsafe

#	ifndef WINDOWS_SYSTEM
#		define	WINDOWS_SYSTEM
#	endif

#	define	HOOPS_BIGENDIAN			0
#	define	HOOPS_PLATFORM_ID		0x200
#	define	PATH_DELIMITER			'\\'
#	define	PATH_DELIMITER_TWO		':'

#	define	HC_CDECL	__cdecl
#	define	HPS_INLINE	__forceinline

#	ifndef HOOPS_DEBUG_BUILD
#		define NODEFAULT	default: __assume (0);
#	endif

#ifndef STATIC_APP
#	if !defined (HOOPS_STATIC_LIB)
#		if defined (HOOPS_WINDOWS_HDI_DRIVER)
#			define HOOPS_API __declspec(dllimport)
#		else
#			define HOOPS_API __declspec(dllexport)
#		endif
#	else
#		if defined(HPS_CORE_BUILD)
#			define HOOPS_API __declspec(dllexport)
#		endif
#	endif
#	define HOOPS_TEMPLATE_API __declspec(dllexport)
#endif

#	if defined (_M_X64)
#		define		HOOPS_64_BIT	1
#		ifndef WIN64_SYSTEM
#			define		WIN64_SYSTEM
#		endif
#		define		HOOPS_PLATFORM "Windows x64"
#	endif

#	ifdef _M_IX86
#		define		HOOPS_PLATFORM		"Windows x86"
#	endif

#	ifdef _WIN32_WCE
#		define		HOOPS_PLATFORM		"Windows CE"
#		define		HOOPS_DEFAULT_PICTURE	"msw/window0"
#	endif

#	ifndef HOOPS_PLATFORM
#		define		HOOPS_PLATFORM		"Windows Unknown"
#	endif

#	ifndef	HOOPS_DEFAULT_PICTURE
#		define		HOOPS_DEFAULT_PICTURE	"opengl/window0"
#	endif
#endif

#ifdef __linux__
#	ifndef LINUX_SYSTEM
#		define	LINUX_SYSTEM
#	endif
#	ifndef UNIX_SYSTEM
#		define	UNIX_SYSTEM
#	endif
#ifndef STATIC_APP
#	if !defined (HOOPS_STATIC_LIB)
#		ifndef HOOPS_WINDOWS_HDI_DRIVER
#			define HOOPS_API __attribute__ ((visibility ("default")))
#		endif
#	else
#		if defined(HPS_CORE_BUILD)
#			define HOOPS_API __attribute__ ((visibility ("default")))
#		endif
#	endif
#	define HOOPS_TEMPLATE_API __attribute__ ((visibility ("default")))
#endif
#	define		HOOPS_BIGENDIAN			0
#	define		HOOPS_PLATFORM			"Linux"
#	define		HOOPS_PLATFORM_ID		0x400
#	define		HOOPS_DEFAULT_PICTURE	H_FORMAT_TEXT("x11/%s", getenv("DISPLAY") ? getenv("DISPLAY") : ":0.0")
#	ifdef _LP64
#		define	HOOPS_64_BIT	1
#	endif
#endif

#ifdef __APPLE_CC__
#	ifndef OSX_SYSTEM
#		define	OSX_SYSTEM
#	endif
#	ifndef UNIX_SYSTEM
#		define	UNIX_SYSTEM
#	endif
#	define		HOOPS_BIGENDIAN			0
#	define		HOOPS_PLATFORM			"OSX"
#	define		HOOPS_PLATFORM_ID		0x400
#	if (TARGET_OS_IPHONE)
#		define  HOOPS_DEFAULT_PICTURE	"opengl2/window"
#       ifdef __LP64__
#           define 	HOOPS_64_BIT	1
#       endif
#	else
#	    if defined (COCOA_OPENGL_DRIVER)
#		    define	HOOPS_DEFAULT_PICTURE	"opengl/window"
#		    define 	HOOPS_64_BIT	1
#	    else
#		   define	HOOPS_DEFAULT_PICTURE	"opengl/window"
#	    endif
#   endif
#endif

#ifndef HPS_INLINE
#	define HPS_INLINE	inline
#endif


#ifndef HOOPS_API
#	define HOOPS_API
#endif

#ifndef HOOPS_TEMPLATE_API
#	define HOOPS_TEMPLATE_API
#endif


#if defined(HPS_CORE_BUILD) || defined(HOOPS_CORE_BUILD)
# if !defined(SPROCKETS)
#	include "hps_utility_public.h"
#	include "hps_enums_public.h"
# endif
#endif

#define INLINE	HPS_INLINE
#define GLOBAL_FUNCTION		extern HOOPS_API
#define GLOBAL_DATA			extern HOOPS_API
#define UNSPECIFIED_ARGUMENTS ...
#define UNREFERENCED(param) ((void)(param))

#ifndef HC_CDECL
#	define	HC_CDECL
#endif

#ifdef HOOPS_DEBUG_BUILD
#	ifdef HPS_CORE_BUILD
#		define  H_ASSERT_HPS(_X_) ASSERT(_X_)
#	else
#		define  H_ASSERT_HPS(_X_) DO_NOTHING(_X_)
#	endif
#else
#	define H_ASSERT_HPS(_X_) DO_NOTHING(_X_)
#endif

#ifndef HC_INTERFACE
#	ifndef HOOPS_CORE_BUILD
#define HC_INTERFACE	static
#	else
#define HC_INTERFACE	extern "C" HOOPS_API
#	endif
#else
#	ifdef _MSC_VER
#	pragma warning (disable: 4505)	// unreferenced local function has been removed
#	endif
#endif


/*****************************************************************************/
/*				Determine components to enable/disable						 */

#ifdef DISABLE_MAXIMUM
#	define		DISABLE_DOUBLE_PRECISION
#	define		DISABLE_METAFILE
#	define		DISABLE_PHONG
#	define		DISABLE_TEXTURING
#	define		DISABLE_SORTING_HSR
#	define		DISABLE_DRAW_3D
#	define		DISABLE_RASTERIZER
#	define		DISABLE_DATABASE_EDIT
#	define		DISABLE_GEOMETRY_EDIT
#	define		DISABLE_GEOMETRY_SHOW
#	define		DISABLE_GENERATE_SHELL
#	define		DISABLE_SEARCH
#	define		DISABLE_SHOW
#	define		DISABLE_UNSET
#	define		DISABLE_GEOMETRY_ATTRIBUTES
#	define		DISABLE_LOD_GENERATION
#	define		DISABLE_CODE_GENERATION
#	define		DISABLE_DEFINED_FONTS
#	define		DISABLE_EVENTS
#	define		DISABLE_COLOR_NAMES
#	define		DISABLE_COLOR_MAPS
#	define		DISABLE_INSTANCING
#	define		DISABLE_POLY_SELECTION
#	define		DISABLE_COMPUTE
#	define		DISABLE_SELECTION
#	define		DISABLE_BIT_RASTERS
#	define		DISABLE_GRAY8_RASTERS
#	define		DISABLE_MAPPED16_RASTERS
#	define		DISABLE_MAPPED8_RASTERS
#	define		DISABLE_STYLE_SEGMENT
#	define		DISABLE_CALLBACKS
#	define		DISABLE_NURBS
#	define		DISABLE_CUT_GEOMETRY
#	define		DISABLE_SIMPLE_SHADOW
#	define		DISABLE_UNICODE_OPTIONS
#	define		DISABLE_ERROR_WARNING_INFO
#	define		DISABLE_NON_MANIFOLD_OPTIMIZATION
#	define		DISABLE_NON_RGB_IMAGES
#	define		DISABLE_EXTENDED_TEXTURE
#	define		DISABLE_SOFTWARE_TEXTURE
#	define		DISABLE_AIR_OPTIONS
#endif

#ifdef HPS_CORE_BUILD
#   define DISABLE_SHOW
#   define DISABLE_GEOMETRY_SHOW
#   define DISABLE_COLOR_NAMES
#   define DISABLE_HCD
#	define DISABLE_AIR_OPTIONS
#	define DISABLE_ENVIRONMENT
#	define DISABLE_METAFILE
#	define DISABLE_BOOLEAN_COMPUTE
#	define DISABLE_CODE_GENERATION
#endif

#ifdef TEST_RIG_1
#	define DISABLE_NON_MANIFOLD_OPTIMIZATION
#	define DISABLE_NON_RGB_IMAGES
#	define DISABLE_EXTENDED_TEXTURE
#	define DISABLE_SOFTWARE_TEXTURE
#	define DISABLE_AIR_OPTIONS
#	define DISABLE_INTERNAL_FONTS
#	define DISABLE_PRIORITY
#	define DISABLE_CODE_GENERATION
#	define DISABLE_BIT_RASTERS
#	define DISABLE_GRAY8_RASTERS
#	define DISABLE_MAPPED16_RASTERS
#	define DISABLE_MAPPED8_RASTERS
#	define DISABLE_PAINTERS
#ifndef _DEBUG
#	define DISABLE_ERROR_WARNING_INFO
#	define DISABLE_METAFILE
#endif
#	define DISABLE_PHONG
#	define DISABLE_LOD_GENERATION
#	define DISABLE_CONTOURING
#	define DISABLE_NURBS_SURFACES
#	define DISABLE_EUC_JIS_UNICODE_MAPPING
#	define DISABLE_RASTERIZER
#	define DISABLE_BOOLEAN_COMPUTE
#   define SMALL_OGL_TRISTRIP
#   define DISABLE_HCD
//#   define DISABLE_JPEG
//#   define DISABLE_PNG
/*
# define DISABLE_STYLE_SEGMENT
# define DISABLE_COLOR_NAMES
# define DISABLE_SORTING_HSR
# define DISABLE_DEFINED_FONTS
# define DISABLE_INSTANCING
#	define DISABLE_GEOMETRY_EDIT
*/
#endif

#ifdef DISABLE_SHOW
#	define		DISABLE_QSHOW
#	define		DISABLE_PSHOW
#endif

#ifdef DISABLE_NURBS
#	define		DISABLE_NURBS_CURVES
#	define		DISABLE_NURBS_SURFACES
#endif

#ifdef DISABLE_GEOMETRY_ATTRIBUTES
#	define		DISABLE_GEOMETRY_COLORS
#	define		DISABLE_GEOMETRY_NORMALS
#	define		DISABLE_GEOMETRY_FACE
#	define		DISABLE_GEOMETRY_EDGE
#	define		DISABLE_GEOMETRY_MARKER
#	define		DISABLE_GEOMETRY_MISC	/* ie, visibility */
#endif

#ifdef DISABLE_GEOMETRY_COLORS
#	define		DISABLE_CONTOURING
#endif


#ifdef DISABLE_SORTING_HSR
#	define		DISABLE_PAINTERS
#	define		DISABLE_HIDDEN_LINE
#	define		DISABLE_PRIORITY
#endif

#ifdef DISABLE_RASTERIZER
#	define		DISABLE_SFB
#	define		DISABLE_SZB
#endif

#ifdef DISABLE_DEFINED_FONTS
#	define		DISABLE_INTERNAL_FONTS
#endif

#ifdef DISABLE_SELECTION
#	define		DISABLE_SELECTION_BY_SHELL
#endif

/* force compiler complaint on obviously non-viable combinations */
#ifdef DISABLE_COLOR_MAPS
#	ifndef DISABLE_CONTOURING
		static_assert(false, "CONTOURING requires COLOR_MAPS!");
#	endif
#endif

#ifdef DISABLE_DRAW_3D
#	ifndef DISABLE_SORTING_HSR
		static_assert(false, "SORTING_HSR requires DRAW_3D!");
#	endif
#	ifndef DISABLE_PAINTERS
		static_assert(false, "PAINTERS requires DRAW_3D!");
#	endif
#	ifndef DISABLE_HIDDEN_LINE
		static_assert(false, "HIDDEN_LINE requires DRAW_3D!");
#	endif
#	ifndef DISABLE_PRIORITY
		static_assert(false, "PRIORITY requires DRAW_3D!");
#	endif
#	ifndef DISABLE_SZB
		static_assert(false, "SZB requires DRAW_3D!");
#	endif
#	ifndef DISABLE_SFB
		static_assert(false, "SFB requires DRAW_3D!");
#	endif
#	ifndef DISABLE_PHONG
		static_assert(false, "PHONG requires DRAW_3D!");
#	endif
#	ifndef DISABLE_SELECTION
		static_assert(false, "SELECTION requires DRAW_3D!");
#	endif
#endif


#ifdef DISABLE_PHONG
#	ifdef DISABLE_TEXTURING
#		define DISABLE_RESHADED /* common software code paths */
#	endif
#endif

// include these public C headers
#include "hversion.h"
#include "patterns.h"

#if defined(HPS_CORE_BUILD)
#include "hpserror_mapping.h"
#else
#include "hpserror.h"
#endif

/*****************************************************************************/

// basic error levels
enum Error_Severity {
	Error_INFO		= 0x00,
	Error_WARNING	= 0x01,
	Error_ERROR		= 0x02,
	Error_FATAL		= 0x03,
	Error_INTERNAL	= 0x04,
};

//#define DISABLE_ERROR_WARNING_INFO
#ifndef DISABLE_ERROR_WARNING_INFO

#	define	HE_INFO(_cat_,_spec_,...)		HI_Basic_Error (null, _cat_,_spec_, Error_INFO, __VA_ARGS__)
#	define	HE_WARNING(_cat_,_spec_,...)	HI_Basic_Error (null, _cat_,_spec_, Error_WARNING, __VA_ARGS__)
#	define	HE_ERROR(_cat_,_spec_,...)		HI_Basic_Error (null, _cat_,_spec_, Error_ERROR, __VA_ARGS__)
#	define	HE_FATAL(_cat_,_spec_,...)		HI_Basic_Error (null, _cat_,_spec_, Error_FATAL, __VA_ARGS__)

#	define	HE_DATA_ERROR(_message_)		HI_Basic_Error (null, HEC_INTERNAL_ERROR, HES_DATA_ERROR, Error_INTERNAL, _message_)
#	define	HE_PROCEDURAL_ERROR(_message_)	HI_Basic_Error (null, HEC_INTERNAL_ERROR, HES_PROCEDURAL_ERROR, Error_INTERNAL, _message_)
#	define	HE_CODE_DISABLED(_part_)		HI_Basic_Error (null, HEC_UNDEFINED, HES_UNDEFINED, Error_ERROR, HOOPS::READ_ONLY->disabled_message, _part_)

#else
#	define	HE_INFO(_cat_,_spec_,...)		UNREFERENCED(_cat_), UNREFERENCED(_spec_)
#	define	HE_WARNING(_cat_,_spec_,...)	UNREFERENCED(_cat_), UNREFERENCED(_spec_)
#	define	HE_ERROR(_cat_,_spec_,...)		UNREFERENCED(_cat_), UNREFERENCED(_spec_)
#	define	HE_FATAL(_cat_,_spec_,...)		UNREFERENCED(_cat_), UNREFERENCED(_spec_)

#	define	HE_DATA_ERROR(_message_)		UNREFERENCED(_message_)
#	define	HE_PROCEDURAL_ERROR(_message_)	UNREFERENCED(_message_)
#	define	HE_CODE_DISABLED(_part_)		UNREFERENCED(_part_)

#endif



#include	<cstddef>
#include	<stdint.h>
#include	<cstring>

//#define CPP11_THREADS 1

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4530)	// C++ exception handler used, but unwind semantics are not enabled.
#endif

#include	<type_traits>
#include	<tuple>
#include	<functional>
#include	<memory>

#if defined(CPP11_THREADS)
#include	<thread>
#endif

#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#define dprintf system_dprintf
#include	<cstdio>
#undef dprintf

#ifdef HOOPS_DEBUG_BUILD
#	include	<cstdarg>

#	ifdef _MSC_VER
		extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA (char const * lpOutputString);
#		define OutputDebugString OutputDebugStringA

		inline void dprintf(char const * format, ...) {
			char		buffer[1024] = {0};
			va_list		arguments;

			va_start(arguments, format);
			vsnprintf(buffer, 1023, format, arguments);
			va_end(arguments);
			OutputDebugString(buffer);
		}
#	elif TARGET_OS_ANDROID==1
#		include <android/log.h>
#		define dprintf(...)		((void)__android_log_print(ANDROID_LOG_DEBUG, "HOOPS", __VA_ARGS__))
#	elif TARGET_OS_IPHONE==1
#		define dprintf(...)		printf(__VA_ARGS__)
#	else
#		define dprintf(...)		do { \
			fprintf(stdout, __VA_ARGS__); \
			fflush(stdout); } while (0)
#	endif
#else
#	define dprintf(...)			((void)(__VA_ARGS__))
#endif

#ifndef NODEFAULT
#define NODEFAULT default: ASSERT(0); break;
#endif

#ifndef nullroutine
#	define		nullroutine		null
#endif

#ifndef HOOPS_MEMORY_ALIGNMENT
#	define		HOOPS_MEMORY_ALIGNMENT		4 // align 16 (1<<4)
#endif

#ifndef HOOPS_PLATFORM_ID
#	define		HOOPS_PLATFORM_ID			0x800	/* must be a source build */
#endif

#ifndef PATH_DELIMITER
#	define		PATH_DELIMITER			'/'
#endif
#ifndef PATH_DELIMITER_TWO
#	define		PATH_DELIMITER_TWO		'\0'
#endif

#ifndef HERTZ
#	define		HERTZ			1000.0f				/* ticks per second */
#endif

#ifndef HOOPS_DEFAULT_PICTURE
	static_assert(false, "Default for ?Picture not filled in -- missing define or system not recognized!");
#endif

#define H_COMPILE_TIME_ASSERT(_expr_) static_assert((_expr_), "something is wrong here")



#define globaldata		extern
#define local			static	/* local data should be 'const' too! */

template<typename T, size_t n>
char (& extract_array_size(const T (&)[n]) )[n]; // count of only for array types

#ifndef countof
#	define countof(_x_)	sizeof(extract_array_size(_x_))
#endif


template <typename T, typename U, typename V>
INLINE size_t _offsetof(T const* t, U V::* a) {
	return (char const*)t - (char const*)&(t->*a) >= 0 ?
		(char const*)t - (char const*)&(t->*a) : (char const*)&(t->*a) - (char const*)t;
}

#ifndef nobreak
#	define nobreak		/* (fall through -- instead of break) */
#endif

/* do any required definitions not specified by the system block above */

#ifndef HOOPS_UTILITY_PRIVATE_API
#	define HOOPS_UTILITY_PRIVATE_API HOOPS_API
#endif

#ifndef HOOPS_UTILITY_PRIVATE_API_DEBUG_BUILD
#	ifdef HOOPS_DEBUG_BUILD
#	define HOOPS_UTILITY_PRIVATE_API_DEBUG_BUILD
#	endif
#endif

#define GLOBAL_UTILITY_FUNCTION	extern HOOPS_UTILITY_PRIVATE_API


#define DO_NOTHING(x)	do if (0 && (x)) {UNREFERENCED(x);} while (0)

#define TOASCII(c)		(c)

#define SPRINTF_BUFFER_SIZE		16530	/* (number of characters) */

#define MAX_C_RETURN_STRING_LENGTH		(1L<<20)	/* 1MB */

#define MAX_SHORT				(short)(((unsigned short)~0L)>>1)
#define MAX_INT					(int)(((unsigned int)~0L)>>1)
#define MAX_USHORT				((unsigned short)~0L)


#define EQUAL_KNAMES(a,b)						\
	((a).length == (b).length && ((a).length == 0 || \
	EQUAL_MEMORY ((a).ktext, (a).length*sizeof(Karacter), (b).ktext)))



#define SIGNED_CHARTOINT(c)		(((int)(char)-1 == -1) ? \
	(int)(char)(c) : \
	(UNSIGNED_CHARTOINT(c) & 0x80) ? \
	UNSIGNED_CHARTOINT(c) - 256 : \
	UNSIGNED_CHARTOINT(c))

#define UNSIGNED_CHARTOINT(c)	(((int)(unsigned char)0xFF == 0xFF) ? \
	(int)(unsigned char)(c) : (0xFF & (int)(c)))

#define UNSIGNED_CHARTOLONG(c)	(((long)(unsigned char)0xFF == 0xFF) ? \
	(long)(unsigned char)(c) : (0xFF & (long)(c)))

#define HM_TOLOWER(c)	HOOPS::READ_ONLY->to_lower[UNSIGNED_CHARTOINT(c)]

#define SEMI_PROTECT(x) do {x} while (0)	/* (';' missing on purpose) */



#define Begin_HOOPS_Namespace	namespace HOOPS {
#define End_HOOPS_Namespace		}

#include "tags.h"

/*****************************************************************************/

// CPU SPECIFIC

#ifdef _MSC_VER

#	ifdef HOOPS_UTILITY_PRIVATE_API_DEBUG_BUILD
#		undef ASSERT
#		define FORCED_ASSERT(x) ((!(x))?__debugbreak():1)
#		define ASSERT(x) ((!(x))?__debugbreak():1)
#		ifdef NDEBUG
		static_assert(false, "NDEBUG should not be defined!");
#		endif
#	else
#		undef ASSERT
#		define FORCED_ASSERT(x) ((!(x))?__debugbreak():1)
#		define ASSERT(x) DO_NOTHING(x)
#	endif

#	ifndef ALIGN_16
#		define ALIGN_16 __declspec(align(16))
#	endif

#	define	USE_MSVC_INTRINSIC	1
#	define	USE_XMM_INTRINSICS	1

extern "C" void __cdecl _mm_prefetch(char const*_A, int _Sel);
#pragma intrinsic (_mm_prefetch)

extern "C" void __cdecl _mm_pause(void);
#pragma intrinsic (_mm_pause)

extern "C" void __cdecl _mm_mfence(void);
#pragma intrinsic (_mm_mfence)

extern "C" long __cdecl _InterlockedIncrement(long volatile *Addend);
extern "C" long __cdecl _InterlockedDecrement(long volatile *Subtrahend);
extern "C" long __cdecl _InterlockedExchange(long volatile *Target, long Value);
extern "C" long __cdecl _InterlockedExchangeAdd(long volatile *Target, long Value);
#pragma intrinsic (_InterlockedIncrement, _InterlockedDecrement, _InterlockedExchange, _InterlockedExchangeAdd)

extern "C" long __cdecl _InterlockedCompareExchange (long volatile *Destination, long Exchange, long Comparand); // XP
extern "C" __int64 __cdecl _InterlockedCompareExchange64 (__int64 volatile *Destination, __int64 Exchange, __int64 Comparand); // vista
#pragma intrinsic (_InterlockedCompareExchange, _InterlockedCompareExchange64)

#ifdef _M_X64
extern "C" void * __cdecl _InterlockedCompareExchangePointer(void * volatile * Destination, void * Exchange, void * Comparand); // XP
extern "C" unsigned char __cdecl _InterlockedCompareExchange128(__int64 volatile * Destination, __int64 ExchangeHigh, __int64 ExchangeLow, __int64 * Comparand);
#pragma intrinsic (_InterlockedCompareExchangePointer, _InterlockedCompareExchange128)

extern "C" void __cdecl __faststorefence(void);
#pragma intrinsic (__faststorefence)
#endif

#else

#   ifdef HOOPS_UTILITY_PRIVATE_API_DEBUG_BUILD
#       undef ASSERT
#       define FORCED_ASSERT(x) do {if(!(x))__builtin_trap();}while(0)
#       define ASSERT(x) do {if(!(x))__builtin_trap();}while(0)
#	else
#		undef ASSERT
#		define FORCED_ASSERT(x) do {if(!(x))__builtin_trap();}while(0)
#		define ASSERT(x) DO_NOTHING(x)
#	endif

# if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 2)

#	define	USE_GCC_INTRINSICS	1

# if !TARGET_OS_IPHONE && !TARGET_OS_ANDROID
#	define	USE_XMM_INTRINSICS	1
# endif

#	ifndef ALIGN_16
#		define ALIGN_16 __attribute__ ((aligned (16)))
#	endif

# endif

#endif

#   if (defined(__arm__) || defined(__arm64__))
#       define USE_NEON
#   endif

#	ifndef INLINE
#		define	INLINE	inline
#	endif

#	ifndef ALIGN_16
#		define	ALIGN_16
#	endif

# if !defined(__APPLE__) && defined(__GNUC__) && (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 8))
#	define OLD_GCC
#	define OVERRIDE 
# else
#	define OVERRIDE override
# endif

Begin_HOOPS_Namespace

INLINE void	Prefetch (void const * item)
{
#if defined (USE_MSVC_INTRINSIC)
	_mm_prefetch ((char *)item, 1);
#elif defined (USE_GCC_INTRINSICS)
	__builtin_prefetch (item);
#else
	UNREFERENCED(item);
#endif
}

INLINE void	ThreadPause ()
{
#if defined (USE_MSVC_INTRINSIC)
	_mm_pause();
#elif defined (USE_GCC_INTRINSICS)
	//__builtin_ia32_pause();
#else
	UNREFERENCED(item);
#endif
}

INLINE void	ReadWriteBarrier ()
{
#if defined (USE_MSVC_INTRINSIC)
#	ifdef _M_X64
	__faststorefence();
#	else
	_mm_mfence();
#endif
#elif defined (USE_GCC_INTRINSICS)
	__sync_synchronize();
#else
	UNREFERENCED(item);
#endif
}


#if !defined(USE_MSVC_INTRINSIC) && !defined(USE_GCC_INTRINSICS)
GLOBAL_UTILITY_FUNCTION	bool	locked_CAS (uint32_t const * destination, uint32_t newval, uint32_t oldval);
GLOBAL_UTILITY_FUNCTION	bool	locked_CAS (intptr_t const * destination, intptr_t newval, intptr_t oldval);
GLOBAL_UTILITY_FUNCTION	int		locked_decrement (uint32_t const * value);
GLOBAL_UTILITY_FUNCTION	int		locked_increment (uint32_t const * value);
GLOBAL_UTILITY_FUNCTION int32_t locked_add (int32_t const * value, int32_t count);
GLOBAL_UTILITY_FUNCTION	int		locked_test_and_set (uint32_t * value);
GLOBAL_UTILITY_FUNCTION	void	locked_clear (uint32_t * value);
#else

// not really const, but avoids extra casts on caller
INLINE	bool	locked_CAS (uint32_t const * destination, uint32_t newval, uint32_t oldval) {
	bool	result;

#	if defined(USE_MSVC_INTRINSIC)
	result = (_InterlockedCompareExchange ((long volatile *)destination, (long)newval, (long)oldval) == (long)oldval);
#	elif defined(USE_GCC_INTRINSICS)
	result = (__sync_val_compare_and_swap ((uint32_t volatile *)destination, oldval, newval) == oldval);
#	else
	ASSERT(0);
#	endif

	return result;
}

// not really const, but avoids extra casts on caller
INLINE	bool	locked_CAS (intptr_t const * destination, intptr_t newval, intptr_t oldval) {
	bool	result;

#	if defined(USE_MSVC_INTRINSIC)
#		ifdef _M_X64
	result = (_InterlockedCompareExchangePointer ((void * volatile *)destination, (void *)newval, (void *)oldval) == (void *)oldval);
#		else
	result = (_InterlockedCompareExchange ((long volatile *)destination, (long)newval, (long)oldval) == (long)oldval);
#		endif
#	elif defined(USE_GCC_INTRINSICS)
	result = (__sync_val_compare_and_swap ((intptr_t volatile *)destination, oldval, newval) == oldval);
#	else
	ASSERT(0);
#	endif

	return result;
}

#if 0
#if defined(_M_IX86) || defined(_LP64)
// not really const, but avoids extra casts on caller
INLINE	bool	locked_CAS_128 (intptr_t const * destination, intptr_t * newval, intptr_t * oldval) { 
	bool	result;

#	if defined(USE_MSVC_INTRINSIC)
	result = (_InterlockedCompareExchange128 ((__int64 volatile *)destination, (__int64)newval[1], (__int64)newval[0], (__int64 *)oldval) == 1);
#	elif defined(USE_GCC_INTRINSICS)
	result = (__sync_val_compare_and_swap ((__int128 volatile *)destination, *((__int128*)oldval), *((__int128*)newval)) == *(__int128*)oldval);
#	else
	ASSERT(0);
#	endif

	return result;
}
#endif
#endif


// not really const, but avoids extra casts on caller
INLINE	int		locked_decrement (uint32_t const * value) {
	int		result;

#	if defined(USE_MSVC_INTRINSIC)
	result = _InterlockedDecrement ((long*)value);
#	elif defined(USE_GCC_INTRINSICS)
	result = __sync_fetch_and_sub ((uint32_t volatile *)value, 1) - 1;
#	else
	ASSERT(0);
#	endif

	return result;
}

// not really const, but avoids extra casts on caller
INLINE	int		locked_increment (uint32_t const * value) {
	int		result;

#	if defined(USE_MSVC_INTRINSIC)
	result = _InterlockedIncrement ((long*)value);
#	elif defined(USE_GCC_INTRINSICS)
	result = __sync_fetch_and_add ((uint32_t volatile *)value, 1) + 1;
#	else
	ASSERT(0);
#	endif

	return result;
}

// not really const, but avoids extra casts on caller
INLINE int32_t locked_add (int32_t const * value, int32_t count) {
	int32_t		result;

#	if defined(USE_MSVC_INTRINSIC)
	result = _InterlockedExchangeAdd ((long*)value, (long)count);
#	elif defined(USE_GCC_INTRINSICS)
	result = __sync_fetch_and_add ((int32_t volatile *)value, count);
#	else
	ASSERT(0);
#	endif

	return result;
}


INLINE	int		locked_test_and_set (uint32_t * value) {
	int		result;

#	if defined(USE_MSVC_INTRINSIC)
	result = _InterlockedExchange ((long *)value, 1);
#	elif defined(USE_GCC_INTRINSICS)
	result =  __sync_lock_test_and_set (value, 1);
#	else
	ASSERT(0);
#	endif

	return result;
}

INLINE	void	locked_clear (uint32_t * value) {
#	if defined(USE_MSVC_INTRINSIC)
	_InterlockedExchange ((long *)value, 0);
#	elif defined(USE_GCC_INTRINSICS)
	__sync_lock_release (value);
#	else
	ASSERT(0);
#	endif
}

#endif

// C++ 11
typedef std::nullptr_t Null_Object;
#	define null nullptr

template <size_t N, size_t base = 2>
struct log_
{
	enum { value = 1 + log_<N / base, base>::value };
};

template <size_t base>
struct log_<1, base>
{
	enum { value = 0 };
};

template <size_t base>
struct log_<0, base>
{
	enum { value = 0 };
};


template <typename T>
struct Hasher {
	inline size_t operator()(const T & key) const{
		size_t hash = (size_t)key;
		hash = (hash ^ 61) ^ (hash >> 16);
		hash = hash + (hash << 3);
		hash = hash ^ (hash >> 4);
		hash = hash * 0x27d4eb2d;
		hash = hash ^ (hash >> 15);
		return hash;
	}
};


template <typename T>
struct HashKeyAdapter {
	inline size_t operator()(const T & item) const{
		return item.hash_key();
	}
};


// starting 1:05 http://channel9.msdn.com/Shows/Going+Deep/C-and-Beyond-2012-Andrei-Alexandrescu-Systematic-Error-Handling-in-C
template< typename Fun >
class ScopeGuard
{
	Fun f_;
	bool active_;

public:
	INLINE explicit ScopeGuard(Fun && _f) : f_(std::forward<Fun>(_f)), active_(true) {}

	INLINE ~ScopeGuard()
	{
		if (active_)
			f_();
	}

	INLINE ScopeGuard(ScopeGuard && that) : f_(std::move(that.f_)), active_(that.active_)
	{
		that.active_ = false;
	}

	INLINE void dismiss() { active_ = false; }

private:
	ScopeGuard(Fun const &);
	ScopeGuard();
	ScopeGuard const & operator=(Fun const &);
};

template<typename Fun>
INLINE ScopeGuard<Fun> MakeScopeGuard(Fun && _f)
{
	return ScopeGuard<Fun>(std::forward<Fun>(_f));
}


/*****************************************************************************/

template<typename T> struct _bit_ops_type {};

template<typename T> INLINE typename _bit_ops_type<T>::type operator~ (T a) { return (T)~(uint32_t)a; }
template<typename T> INLINE typename _bit_ops_type<T>::type operator| (T a, T b) { return (T)((uint32_t)a | (uint32_t)b); }
template<typename T> INLINE typename _bit_ops_type<T>::type operator& (T a, T b) { return (T)((uint32_t)a & (uint32_t)b); }
template<typename T> INLINE typename _bit_ops_type<T>::type operator^ (T a, T b) { return (T)((uint32_t)a ^ (uint32_t)b); }
template<typename T> INLINE typename _bit_ops_type<T>::type& operator|= (T& a, T b) { return (T&)((uint32_t&)a |= (uint32_t)b); }
template<typename T> INLINE typename _bit_ops_type<T>::type& operator&= (T& a, T b) { return (T&)((uint32_t&)a &= (uint32_t)b); }
template<typename T> INLINE typename _bit_ops_type<T>::type& operator^= (T& a, T b) { return (T&)((uint32_t&)a ^= (uint32_t)b); }

template <typename T, typename U>
INLINE bool ANYBIT(T v, U mask) {
	return ((v & mask) != 0);
}

template <typename T, typename U>
INLINE bool ALLBITS(T v, U mask) {
	return ((v & mask) == mask);
}

template <typename T>
INLINE T LOWBIT(T v) {
	typedef typename std::make_signed<T>::type signed_T;
	return (T)((signed_T)v & -((signed_T)v));
}

INLINE size_t BIT_COUNT(unsigned char byte) {

	static const char bits_per_byte[] = {
		0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
		4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
	};

	return bits_per_byte[byte];
}

template <typename T>
INLINE size_t BIT_COUNT(T v) {

	size_t num_bits = 0;

	unsigned char data[sizeof(T)];
	memcpy(data, &v, sizeof(T));

	for (size_t i=0; i<sizeof(T); i++)
		num_bits += BIT_COUNT(data[i]);

	return num_bits;
}

template <typename T>
INLINE size_t TRAILING_ZEROS(T v) {
	typedef typename std::make_unsigned<T>::type unsigned_T;
	unsigned_T ulb = (unsigned_T)LOWBIT(v);
	ulb -= 1;
	return BIT_COUNT(ulb);
}

INLINE size_t TRAILING_ZEROS(uint32_t v) {
	static const int	deBruijn_decode[32] = {
		0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8, 
		31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
	};

	return deBruijn_decode[(LOWBIT(v) * 0x077CB531U) >> 27];
}

INLINE size_t TRAILING_ZEROS(int32_t v) {
	return TRAILING_ZEROS((uint32_t)v);
}

INLINE size_t TRAILING_ZEROS(uint64_t v) {

	uint32_t data[2];
	memcpy(data, &v, sizeof(uint64_t));

	size_t num_bits = TRAILING_ZEROS(data[0]);

	if (num_bits == 32)
		num_bits += TRAILING_ZEROS(data[1]);

	return num_bits;
}

INLINE size_t TRAILING_ZEROS(int64_t v) {
	return TRAILING_ZEROS((uint64_t)v);
}

#define ZERO_STRUCT(_dst_, _type_)			SET_MEMORY (_dst_, sizeof(_type_), 0)
#define ZERO_ARRAY(_dst_, _cnt_, _type_)	SET_MEMORY (_dst_, (_cnt_) * sizeof(_type_), 0)

INLINE void SET_MEMORY (void *dest, size_t size, int val)
{
	//if the caller passes in an int with stuff in the high bytes that doesn't match the low bytes, it's probably not going 
	//to do what they expect.  They should be using the new SET_MEMORY32 instead
	ASSERT (!ANYBIT (val, 0xffffff00) || 
		((((val & (int)0xff000000)>>24)& 0xff) == (val & 0xff) && 
		((val & 0x00ff0000)>>16) == (val & 0xff) && 
		((val & 0x0000ff00)>>8) == (val & 0xff))
		);
	memset(dest, val, size);
}

INLINE void SET_MEMORY32 (void *dest, size_t size, unsigned int val)
{
	int *ptr = (int *)dest;
	ASSERT (!ANYBIT (size, 0x3));

	size = size >> 2;
	int *end = ptr + size;
	if (size < 4) {
		while (ptr < end) {
			*ptr++ = val;
		}
	}
	else {
		int *start = ptr;
		//seed the first 4
		ptr[0] = ptr[1] = ptr[2] = ptr[3] = val;
		intptr_t batch = 4;
		ptr += 4;
		//grow batch exponentially
		while (ptr + batch < end) {
			//COPY_MEMORY isn't defined yet
			memcpy (ptr, start, batch*sizeof(int));
			ptr += batch;
			if ((ptr + (batch << 1)) <= end)
				batch = batch << 1;
		}
		//we're guaranteed to be at least at the half way point by now
		if (ptr < end) {
			batch = end - ptr;
			memcpy (ptr, start, batch*sizeof(int));
			ptr += batch;
		}
	}
}

INLINE void SET_MEMORY32 (void *dest, size_t size, float val)
{
	union
	{
		float f;
		unsigned int ui;
	} uval;

	uval.f = val;
	SET_MEMORY32 (dest, size, uval.ui);
}

INLINE void SET_MEMORY32 (void *dest, size_t size, int val)
{
	SET_MEMORY32 (dest, size, *(unsigned int *)(&val));
}

/*****************************************************************************/


typedef	HPS::Point	DC_Point;	// different name is just a reminder of the coordinate space

using HPS::Point_3D;
using HPS::Point;
using HPS::DPoint;

using HPS::Point_2D;
using HPS::Point2D;
using HPS::DPoint2D;

using HPS::Vector_3D;
using HPS::Vector;
using HPS::DVector;

using HPS::Vector_2D;
using HPS::Vector2D;
using HPS::DVector2D;

using HPS::Plane_3D;
using HPS::Plane;
using HPS::DPlane;

using HPS::Plane_2D;
using HPS::Plane2D;
using HPS::DPlane2D;

using HPS::Cuboid_3D;
using HPS::SimpleCuboid;
using HPS::DSimpleCuboid;

using HPS::Sphere_3D;
using HPS::SimpleSphere;
using HPS::DSimpleSphere;

using HPS::IntRectangle;
using HPS::Rectangle;

typedef HPS::RGBColor		RGB;
typedef HPS::RGBAColor		RGBA;
typedef HPS::RGBAS32Color	RGBAS32;
typedef HPS::RGBA32Color	RGBA32;
typedef HPS::RGB24Color		RGB24;
typedef HPS::DirectRGBColor	DirectRGB;

using HPS::Float;
using HPS::Float_Traits;

using HPS::Alter;
using HPS::Swap;
using HPS::Min;
using HPS::Max;
using HPS::Sign;
using HPS::Compare;
using HPS::Clamp;
using HPS::Abs;
using HPS::Floor;
using HPS::Ceiling;

using HPS::PI;
using HPS::Degrees_To_Radians;
using HPS::Radians_To_Degrees;
using HPS::SinCos;
using HPS::Sin;
using HPS::Cos;
using HPS::ACos;
using HPS::ATan2;


using HPS::Is_Abnormal;

typedef intptr_t	Key;

#if defined(CPP11_THREADS)
class HOOPS_UTILITY_PRIVATE_API HThreadID {
private:
	char _data[sizeof(std::thread::id)];
public:
	HThreadID(int x = 0) {
		ASSERT(x == 0);
		memset(_data, 0, sizeof(_data));
	}
	HThreadID(HThreadID const & that) {
		memcpy(_data, that._data, sizeof(_data));
	}
	HThreadID(std::thread::id const & it) {
		memcpy(_data, &it, sizeof(_data));
	}
	INLINE bool operator == (HThreadID const & that) const {
		return (*((std::thread::id const *)&_data[0]) == *((std::thread::id const *)&that._data[0]));
	}
	INLINE bool operator != (HThreadID const & that) const {return !(that == *this);}

	INLINE int32_t hash_key() const {
		std::hash<std::thread::id> hash_fn;
		return (int32_t)hash_fn(*((std::thread::id const *)&_data[0]));
	}
};

struct HThreadIDHasher {
	size_t operator()(const HThreadID & it) const{
		return (size_t)it.hash_key();
	}
};

#else
typedef intptr_t	HThreadID;

struct HThreadIDHasher {
	size_t operator()(const HThreadID & it) const{
		Hasher<intptr_t> hasher;
		return hasher(it);
	}
};
#endif

template <typename T> class CMO_Allocator;

typedef double			Time;

typedef unsigned short	Color_Index;
typedef float			Color_Float_Index;

typedef short			Light_Flags;
typedef char			Spot_Light_Size_Units;
typedef int32_t			Heuristics_Flags;
typedef uint32_t		Activity;
typedef int				Button;
typedef int32_t			Parameter_Flags;
typedef char			Generic_Size_Units;

typedef int32_t			Locater_Actions;	 /* defined further in "please.h" */

typedef uint32_t		Utility;			/* used for database objects */
typedef uint32_t		Reference_Count;	/* for anything else */


#ifndef NO_SHARING
#  define IS_SHARED(ref)  ((ref)->refcount > 1)
#else
#  define IS_SHARED(ref)  true
#endif

template<typename T>
struct Null_Deleter
{
	void operator()(T * item) const
	{
		UNREFERENCED(item);
	}
};

template <typename T>
struct Init_Ptr {
	typedef std::unique_ptr<T, Null_Deleter<T>> unique_ptr;
};

// this odd setup is so init_priority can be used
// it only works for classes not bare pointers

struct World;
struct Eternal_World;
struct World_Read_Only;

GLOBAL_DATA Init_Ptr<World>::unique_ptr				WORLD;
GLOBAL_DATA Init_Ptr<Eternal_World>::unique_ptr		ETERNAL_WORLD;
GLOBAL_DATA Init_Ptr<World_Read_Only>::unique_ptr	READ_ONLY;

// GLOBAL_FUNCTION C FUNCTION TYPES
extern "C" {
	typedef	void * (HC_CDECL * Malloc_Action) (size_t);
	typedef	void (HC_CDECL * Free_Action) (void *);
	
	typedef void (HC_CDECL * Void_Routine) (UNSPECIFIED_ARGUMENTS);
	typedef int (HC_CDECL * Int_Routine) (UNSPECIFIED_ARGUMENTS);

	typedef	void (HC_CDECL * Event_Checker_Action) (void);

	typedef	void (HC_CDECL * Database_Key_Action) (Key);

	typedef	void (HC_CDECL * Delete_Database_Key_Action) (Key, int);

	// Drivers need non-mangled names (for dynamic loaders)
	typedef bool (HC_CDECL * Driver_Action) (Thread_Data * thread_data, Actor *driver, int request, void * request_info);

#ifndef HPS_CORE_BUILD
	typedef void (HC_CDECL * Exit_Handler_Routine) (void);

	typedef void (HC_CDECL * Error_Handler_Routine)
		(int category, int specific, int severity,
		int msgc, char const * const * msgv,
		int routinec, char const * const * routinev);
#endif

	typedef bool icu_unicode_entrypoint (
		Net_Rendition & nr,
		Text const *  text,
		Text_Info *   ti);
	

	// Dynamic loaders themselves
	typedef Driver_Action (HC_CDECL * Load_Dynamic_Driver_Action)(char const *driver_name);


	typedef void (HC_CDECL * Segment_Iterator_Action) (
		Thread_Data *		thread_data,
		Segment *			seg,
		void *				info);

	typedef Key (HC_CDECL * Output_Parse_Action) (
		Thread_Data *		thread_data,
		Segment *			segment,
		Segment *			owner,
		Subsegment **		backlink,
		Subsegment *		pred,
		Name_Const const &	name,
		bool				check_owner,
		bool				exists,
		void *				info);

	typedef bool (HC_CDECL * Base_Driver_Display_List_Callback) (Driver_DL *bddl, void *extra);
}

struct Memory_Pool;
struct Eternal_World {

	int							system_flags;
	int							code_file_number;

	Malloc_Action				malloc;
	Free_Action					free;
	Malloc_Action				system_malloc;
	Free_Action					system_free;

	void *						license_info;

	Memory_Pool *				memory_pool;
	bool						using_raw_memory;

	size_t						vram_size;
	char						graphics_card_name[1024];
	volatile bool				graphics_card_query_complete;
};

GLOBAL_DATA Eternal_World ETERNAL_WORLD_DATA;


struct World_Read_Only {
	unsigned short				dither_thresholds[8][8];

	int							incr_mod_4[4];
	int							incr_mod_8[8];
	int							incr_mod_16[16];
	short						power2[16];

	unsigned char				reversed_byte[256];

	char						to_lower[256];
	float						uniform_rand[1024];

	char						index_of_highbit[256];

	char const *				disabled_message;

	float						sixty_four_ones[64];

	char const *				power_chain[32];
};

#define Error_Control_OFF		0x0000
#define Error_Control_ON		0x0001
#define Error_Control_ONCE		0x0002

#define Error_Control_INTERNAL	0x0100

typedef int						Error_Mask;

struct Error_Control {
	Error_Control *		next;
	HEC					category;
	HES					specific;
	int					setting;	// provided by user
	int					state;		// setting, except once -> off after use
};

End_HOOPS_Namespace
using namespace HOOPS;

// GLOBALS that are not in the HOOPS name space but using it
#define TIME(sec)		((HOOPS::Time)(HERTZ*(sec)))  /* Seconds to ticks */
#define POLL_INTERVAL	TIME(1.0/100.0)

GLOBAL_FUNCTION Time HI_What_Time (void);

GLOBAL_FUNCTION void HI_Set_Thread_Name(HThreadID id, char const * name);

GLOBAL_FUNCTION bool HI_Do_Update (
	Thread_Data *		thread_data,
	double				limit,
	bool				do_isolated);

GLOBAL_FUNCTION void HI_Define_Constant_Options (
	Constant_Option_Type const *	constant_table,
	int								constant_table_size,
	Option_Hash_Table *				hash_table);


GLOBAL_FUNCTION void HI_Basic_Error (
	Thread_Data *	thread_data,
	HEC				category,
	HES				specific,
	Error_Severity	severity,
	char const *	buf1,
	char const *	buf2 = 0,
	char const *	buf3 = 0);


GLOBAL_FUNCTION Key HI_Key_To_Internal (Thread_Data * thread_data, Key key, Segment const * seg);

GLOBAL_FUNCTION void HI_Start_Hoops (char const *function_name, bool minimal);

GLOBAL_FUNCTION void HI_Dump_Code (char const *string);

GLOBAL_FUNCTION void HI_Chain_Code_Files (void);

GLOBAL_FUNCTION char * HI_Sprintf4 (
	Thread_Data *	thread_data,
	char *			buf,
	char const *	spec,
	int				int1,
	int				int2,
	void const *	ptr1,
	void const *	ptr2);


GLOBAL_FUNCTION Thread_Data * HI_Set_Name (const char * function_name);

GLOBAL_FUNCTION bool HI_Really_Find_Our_Open (Thread_Data * thread_data);

Begin_HOOPS_Namespace

typedef HM_Mutex *	Mutex;
typedef HM_RWLock *	RWLock;
typedef HM_Semaphore * Semaphore;
typedef HM_Thread * Thread;

template <size_t MAX_BIT, size_t UNIQUE_ID, typename storage_type = size_t>
class Bitset {
private:

	static const size_t max_bit = MAX_BIT;
	static const size_t storage_size = 8 * sizeof(storage_type);
	static const size_t n_words = (MAX_BIT + storage_size-1) / storage_size;

	static const storage_type one_bit = static_cast<storage_type>(1);
	static const storage_type no_bits = static_cast<storage_type>(0);

	static const storage_type upper_mask = (max_bit % storage_size) != 0 ? (one_bit << (max_bit % storage_size)) - 1 : ~no_bits;

	storage_type	bits[n_words];

public:

	Bitset () {
		for (size_t i=0; i<n_words; ++i)
			bits[i] = no_bits;
	}

	Bitset (Bitset const & that) {
		for (size_t i=0; i<n_words; i++)
			bits[i] = that.bits[i];
	}

	explicit Bitset (size_t index) {
		for (size_t i=0; i<n_words; ++i)
			bits[i] = 0;
		size_t	bank = index / storage_size;
		size_t	offset = index & (storage_size-1); //index % storage_size;
		bits[bank] = one_bit << offset;
	}

	INLINE Bitset & operator= (Bitset const & that) {
		for (size_t i=0; i<n_words; i++)
			bits[i] = that.bits[i];
		return *this;
	}

	INLINE size_t size() const {
		return max_bit;
	}

	INLINE bool any () const {
		for (size_t i=0; i<n_words; i++)
			if (bits[i] != 0)
				return true;
		return false;
	}

	INLINE bool none () const {
		return !any();
	}

	INLINE size_t count () const {

		size_t num_bits = 0;

		for (size_t i=0; i<n_words; i++)
			if (bits[i] != 0)
				num_bits += BIT_COUNT(bits[i]);

		return num_bits;
	}

	INLINE Bitset & flip () {
		for (size_t i=0; i<n_words; i++)
			bits[i] = ~bits[i];
		bits[n_words-1] &= upper_mask;
		return *this;
	}
	INLINE Bitset & flip (size_t index) {
		size_t	bank = index / storage_size;
		size_t	offset = index & (storage_size-1); //index % storage_size;
		bits[bank] ^= one_bit<<offset;
		return *this;
	}

	INLINE Bitset & reset () {
		for (size_t i=0; i<n_words; ++i)
			bits[i] = no_bits;
		return *this;
	}
	INLINE Bitset & reset (size_t index) {
		size_t	bank = index / storage_size;
		size_t	offset = index & (storage_size-1); //index % storage_size;
		bits[bank] &= ~(one_bit<<offset);
		return *this;
	}

	INLINE Bitset & set () {
		for (size_t i=0; i<n_words; ++i)
			bits[i] = ~no_bits;
		return *this;
	}
	INLINE Bitset & set (size_t index, bool turn_on) {
		if (turn_on)
			set(index);
		else
			reset(index);
		return *this;
	}

	INLINE Bitset & set (size_t index) {
		size_t	bank = index / storage_size;
		size_t	offset = index & (storage_size-1); //index % storage_size;
		storage_type the_bit = one_bit << offset;
		bits[bank] |= the_bit;
		return *this;
	}

	INLINE bool test (size_t index) const {
		size_t	bank = index / storage_size;
		size_t	offset = index & (storage_size-1); //index % storage_size;
		return (bits[bank] & (one_bit << offset)) != 0;
	}

	INLINE Bitset & operator<<= (size_t num_positions_to_shift) {
		if(num_positions_to_shift == 0)
			return *this;

		if(num_positions_to_shift >= max_bit)
			return reset();

		size_t num_leftover_bits = num_positions_to_shift % storage_size;
		size_t src_word_index    = n_words - (num_positions_to_shift / storage_size);
		size_t dest_word_index   = n_words;

		if(num_leftover_bits != 0) {
			storage_type lower_mask = (one_bit << num_leftover_bits) - 1;

			while(src_word_index > 0) {
				storage_type upper_part     = bits[src_word_index - 1] << num_leftover_bits;
				storage_type value_to_shift = upper_part;

				if(--src_word_index > 0) {
					storage_type lower_part = bits[src_word_index - 1] >> (storage_size - num_leftover_bits);

					value_to_shift |= lower_part & lower_mask;
				}

				bits[dest_word_index - 1] = value_to_shift;
				--dest_word_index;
			}
		} else {
			while(src_word_index > 0) {
				bits[dest_word_index - 1] = bits[src_word_index - 1];
				--dest_word_index;
				--src_word_index;
			}
		}

		while(dest_word_index > 0) {
			bits[dest_word_index - 1] = 0;
			--dest_word_index;
		}

		bits[n_words-1] &= upper_mask;

		return *this;
	}

	INLINE Bitset & operator>>= (size_t num_positions_to_shift) {
		if(num_positions_to_shift == 0)
			return *this;

		if(num_positions_to_shift >= max_bit)
			return reset();

		size_t num_leftover_bits = num_positions_to_shift % storage_size;
		size_t src_word_index    = num_positions_to_shift / storage_size;
		size_t dest_word_index   = 0;

		if(num_leftover_bits != 0) {
			storage_type lower_mask = (one_bit << (storage_size - num_leftover_bits)) - 1;

			while(src_word_index < n_words) {
				storage_type lower_part     = bits[src_word_index] >> num_leftover_bits;
				storage_type value_to_shift = lower_part & lower_mask;

				if(++src_word_index < n_words) {
					storage_type upper_part = bits[src_word_index] << (storage_size - num_leftover_bits);

					value_to_shift |= upper_part;
				}

				bits[dest_word_index++] = value_to_shift;
			}
		} else {
			while(src_word_index < n_words) {
				bits[dest_word_index++] = bits[src_word_index++];
			}
		}

		while(dest_word_index < n_words) {
			bits[dest_word_index++] = 0;
		}

		return *this;
	}

	INLINE operator const bool () const {
		for (size_t i=0; i<n_words; i++)
			if (bits[i] != 0)
				return true;
		return false;
	}

	INLINE bool operator== (Bitset const & that) const {
		for (size_t i=0; i<n_words; i++)
			if (bits[i] != that.bits[i])
				return false;
		return true;
	}

	INLINE bool operator!= (Bitset const & that) const {
		return !(*this == that);
	}

	INLINE Bitset operator~ () const {
		Bitset	temp;
		for (size_t i=0; i<n_words; i++)
			temp.bits[i] = ~bits[i];
		temp.bits[n_words-1] &= upper_mask;
		return temp;
	}

	INLINE Bitset operator|= (Bitset const & that)  {
		for (size_t i=0; i<n_words; i++)
			bits[i] |= that.bits[i];
		return *this;
	}
	INLINE Bitset operator&= (Bitset const & that)  {
		for (size_t i=0; i<n_words; i++)
			bits[i] &= that.bits[i];
		return *this;
	}
	INLINE Bitset operator^= (Bitset const & that)  {
		for (size_t i=0; i<n_words; i++)
			bits[i] ^= that.bits[i];
		return *this;
	}

	INLINE bool any (Bitset const & that) const {
		Bitset	temp;
		for (size_t i=0; i<n_words; i++)
			if ((bits[i] & that.bits[i]) != 0)
				return true;
		return false;
	}
	INLINE bool all (Bitset const & that) const {
		Bitset	temp;
		for (size_t i=0; i<n_words; i++)
			if ((bits[i] & that.bits[i]) != that.bits[i])
				return false;
		return true;
	}

	INLINE Bitset lowbit () const {
		Bitset	temp;
		for (size_t i=0; i<n_words; i++) 
			if ((temp.bits[i] = LOWBIT(bits[i])) != 0)
				break;
		return temp;
	}

	INLINE size_t trailing_zeros() const {

		size_t num_bits = 0;

		for (size_t i=0; i<n_words; i++) {
			if (bits[i] != 0) {
				num_bits += TRAILING_ZEROS(bits[i]);
				break;
			}
			num_bits += storage_size;
		}

		return (num_bits < max_bit)?num_bits:max_bit;
	}
};

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> operator<<(const Bitset<N,U,T>& bset, size_t num_positions_to_shift)
{
	Bitset<N,U,T> result(bset);
	result <<= num_positions_to_shift;
	return result;
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> operator>>(const Bitset<N,U,T>& bset, size_t num_positions_to_shift)
{
	Bitset<N,U,T> result(bset);
	result >>= num_positions_to_shift;
	return result;
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> operator| (const Bitset<N,U,T>& left_bset, const Bitset<N,U,T>& right_bset)
{
	Bitset<N,U,T> result(left_bset);
	return result |= right_bset;
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> operator& (const Bitset<N,U,T>& left_bset, const Bitset<N,U,T>& right_bset)
{
	Bitset<N,U,T> result(left_bset);
	return result &= right_bset;
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> operator^ (const Bitset<N,U,T>& left_bset, const Bitset<N,U,T>& right_bset)
{
	Bitset<N,U,T> result(left_bset);
	return result ^= right_bset;
}

#define Define_Bitset(_bit_count_) typedef Bitset<(_bit_count_), __LINE__>

template <size_t N, size_t U, typename T>
INLINE bool ANYBIT(Bitset<N,U,T> const & v, Bitset<N,U,T> const & mask) {
	return v.any(mask);
}

template <size_t N, size_t U, typename T>
INLINE bool ALLBITS(Bitset<N,U,T> const & v, Bitset<N,U,T> const & mask) {
	return v.all(mask);
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> LOWBIT(Bitset<N,U,T> const & v) {
	return v.lowbit();
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> BIT_COUNT(Bitset<N,U,T> const & v) {
	return v.count();
}

template <size_t N, size_t U, typename T>
INLINE Bitset<N,U,T> TRAILING_ZEROS(Bitset<N,U,T> const & v) {
	return v.trailing_zeros();
}


/*****************************************************************************/

#include "hoops_memory.h"

#define ALLOCATE_MEMORY(p,t) (p = (t *)HOOPS::HUI_System_Alloc (sizeof(t)))
#define ALLOCATE_MEMORY_ARRAY(p,c,t)	(p = (t *)HOOPS::HUI_System_Alloc ((c)*sizeof(t)))
#define FREE_MEMORY(p,t) HOOPS::HUI_System_Free (p)

/* basic memory copying & setting */

#define COPY_MEMORY(_src_, _siz_, _dst_)	memcpy(_dst_, _src_, _siz_)
#define COPY_ARRAY(_src_, _cnt_, _type_, _dst_)	COPY_MEMORY (_src_, (_cnt_) * sizeof(_type_), _dst_)

#define CLONE_ARRAY(_src_, _cnt_, _type_, _dst_) do { \
	void* _tmp_src_ = (void*)_src_; \
	if (_cnt_ <= 0 || _src_ == null) { \
	_dst_ = null; \
	break; \
	} \
	ALLOC_ARRAY(_dst_, _cnt_, _type_); \
	COPY_ARRAY(_tmp_src_, _cnt_, _type_, _dst_); \
} while (0)

#define POOL_CLONE_ARRAY(_src_, _cnt_, _type_, _dst_, _pool_) do { \
	void* _tmp_src_ = (void*)_src_; \
	if (_cnt_ <= 0 || _src_ == null) { \
	_dst_ = null; \
	break; \
	} \
	POOL_ALLOC_ARRAY(_dst_, _cnt_, _type_, _pool_); \
	COPY_ARRAY(_tmp_src_, _cnt_, _type_, _dst_); \
} while (0)

#define CLONE_ARRAY_CACHED(_src_, _cnt_, _type_, _dst_) do { \
	void* _tmp_src_ = (void*)_src_; \
	if (_cnt_ <= 0 || _src_ == null) { \
	_dst_ = null; \
	break; \
	} \
	ALLOC_ARRAY_CACHED(_dst_, _cnt_, _type_); \
	COPY_ARRAY(_tmp_src_, _cnt_, _type_, _dst_); \
} while (0)

#define POOL_CLONE_ARRAY_CACHED(_src_, _cnt_, _type_, _dst_, _pool_) do { \
	void* _tmp_src_ = (void*)_src_; \
	if (_cnt_ <= 0 || _src_ == null) { \
	_dst_ = null; \
	break; \
	} \
	POOL_ALLOC_ARRAY_CACHED(_dst_, _cnt_, _type_, _pool_); \
	COPY_ARRAY(_tmp_src_, _cnt_, _type_, _dst_); \
} while (0)


/*****************************************************************************/

#include "locking.h"

/*****************************************************************************/

INLINE uint32_t Nearest_Power_Of_2(uint32_t v) {
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	v += (v == 0);
	return v;
}


End_HOOPS_Namespace

/* Cast away constness! This is not thread-safe. */
#define REALLY_INCR_UTILITY(ref) (++*(Reference_Count *)&((ref)->refcount))
#define REALLY_DECR_UTILITY(ref) (--*(Reference_Count *)&((ref)->refcount))
#define ZERO_UTILITY(ref) ((ref)->refcount = 0)

#if defined (DEBUG_UTILITY)
#	 define INCR_UTILITY(ref) \
	(dprintf ("+%p %d->%d %s %d\n", (void*)(ref), (ref)->refcount, (ref)->refcount+1, __FILE__, __LINE__),\
	REALLY_INCR_UTILITY(ref))
#	 define DECR_UTILITY(ref) \
	(dprintf ("-%p %d->%d %s %d\n", (void*)(ref), (ref)->refcount, (ref)->refcount-1, __FILE__, __LINE__),\
	REALLY_DECR_UTILITY(ref))
#	 define INIT_UTILITY(ref) (((ref)->refcount = 0), INCR_UTILITY (ref))

#elif defined (VALIDATE_MEMORY)
#	define INCR_UTILITY(ref) ((((ref)->refcount)==0x55555555) ? (HE_DATA_ERROR("detected INCR_UTILITY after free")),1 : REALLY_INCR_UTILITY(ref))
#	define DECR_UTILITY(ref) ((((ref)->refcount)==0x55555555) ? (HE_DATA_ERROR("detected DECR_UTILITY after free")),1 : REALLY_DECR_UTILITY(ref))
#	define INIT_UTILITY(ref) ((ref)->refcount = 1)
#else
#	define INCR_UTILITY(ref) REALLY_INCR_UTILITY(ref)
#	define DECR_UTILITY(ref) REALLY_DECR_UTILITY(ref)
#	define INIT_UTILITY(ref) ((ref)->refcount = 1)
#endif


#include "adt.h"


Begin_HOOPS_Namespace

typedef unrolled_list<HThreadHandle, CMO_Allocator<HThreadHandle> > Delayed_Thread_List;

typedef std::vector<Named_Style, POOL_Allocator<Named_Style> > Named_Style_Vector_Pooled;

typedef std::vector<Segment const *, CMO_Allocator<Segment const *> > Segment_Vector;
typedef std::vector<Segment const *, POOL_Allocator<Segment const *> > Segment_Vector_Pooled;
typedef std::vector<char, POOL_Allocator<char> > char_Vector_Pooled;


typedef std::vector<User_Options const *, POOL_Allocator<User_Options const *> > User_Options_Vector_Pooled;
typedef std::vector<Clip_Region const *, POOL_Allocator<Clip_Region const *> > Clip_Region_Vector_Pooled;

typedef std::vector<Texture const *, CMO_Allocator<Texture const *> > Texture_Vector;
typedef std::vector<Texture_Defs const *, CMO_Allocator<Texture_Defs const *> > Texture_Defs_Vector;


typedef std::vector<int, CMO_Allocator<int> > int_Vector;
typedef std::vector<Point, CMO_Allocator<Point> > Point_Vector;
typedef std::vector<DPoint, CMO_Allocator<DPoint> > DPoint_Vector;
typedef std::vector<intptr_t, CMO_Allocator<intptr_t> > intptr_t_Vector;

//typedef VHash<HThreadID, Thread_Data *> HThreadID_TSD_Hash;
typedef Unordered<HThreadID, Thread_Data *, HThreadIDHasher,
	std::equal_to<HThreadID>, CMO_Allocator<std::pair<const HThreadID, Thread_Data *> > > HThreadID_TSD_Hash;

typedef Banked_Array<int, CMO_Allocator<int> > int_Banked_Array; // XXX cached vs no cached
typedef Banked_Array<int, POOL_Allocator<int> > int_Banked_Array_Cached;
typedef Banked_Array<float, POOL_Allocator<float> > float_Banked_Array_Cached;

typedef std::vector<Geometry *, CMO_Allocator<Geometry *> > Geometry_Vector;
typedef std::vector<Geometry_Reference *, CMO_Allocator<Geometry_Reference *> > Geometry_Reference_Vector;
typedef std::set<Geometry_Reference *, std::less<Geometry_Reference *>, CMO_Allocator<Geometry_Reference *> > Geometry_Reference_Set;


typedef std::vector<DC_Point, POOL_Allocator<DC_Point> > DC_Point_Vector_Pooled;
typedef std::vector<RGBAS32, POOL_Allocator<RGBAS32> > RGBAS32_Vector_Pooled;

class Geometry_Reference_Lookup_Item : public CMO {
public:

	Geometry_Reference_Lookup_Item() : grs(null) {};

	~Geometry_Reference_Lookup_Item() {
	};

	inline Geometry_Reference_Vector::iterator begin() {
		if (grs != null)
			to_vec();
		return grv.begin();
	}
	inline Geometry_Reference_Vector::iterator end() {
		if (grs != null)
			to_vec();
		return grv.end();
	}

	inline Geometry_Reference_Vector::const_iterator cbegin() {
		if (grs != null)
			to_vec();
		return grv.cbegin();
	}
	inline Geometry_Reference_Vector::const_iterator cend() {
		if (grs != null)
			to_vec();
		return grv.cend();
	}

	INLINE size_t size() {
		if (grs != null)
			return grs->size();
		return grv.size();
	}

	INLINE bool empty() {
		if (grs != null)
			return grs->empty();
		return grv.empty();
	}

	bool remove(Geometry_Reference * gr) {

		if (grv.size() > 32)
			to_set();

		if (grs != null) {

			auto it = grs->find(gr);

			if (it != grs->end()) {
				if (*it != gr)
					return false;
				grs->erase(it);
				return true;
			}

		}
		else {

			auto it = std::find(grv.begin(), grv.end(), gr);
		
			if (it != grv.end()) {
				if (*it != gr)
					return false;
				grv.erase(it);
				return true;
			}
		}

		return false;
	}

	INLINE void insert(Geometry_Reference * gr) {
		if (grs != null)
			grs->insert(gr);
		else
			grv.push_back(gr);
	}

private:

	void to_vec() {
		ASSERT(grs != null);
		ASSERT(grv.size() == 0);
		grv.reserve(grs->size());
		std::copy(grs->begin(), grs->end(), std::back_inserter(grv));
		grs = null;
	}

	void to_set() {
		ASSERT(grs == null);
		ASSERT(grv.size() > 0);
		grs = Construct<Geometry_Reference_Set>();
		std::copy(grv.begin(), grv.end(), std::inserter(*grs, grs->end()));
		grv.clear();
	}

	Geometry_Reference_Vector							grv;
	Destructible<Geometry_Reference_Set>::unique_ptr	grs;
};

typedef VHash<void const *, Geometry_Reference_Lookup_Item *> Geometry_Reference_Lookup;

typedef VList<int *> Int_Ptr_List;
typedef VList<Int_Ptr_List *> Int_Ptr_List_List;

typedef unrolled_list<Polygon_Collector *, POOL_Allocator<Polygon_Collector *> > Polygon_Collector_List;

typedef unrolled_list<Segment const *, POOL_Allocator<Segment const *> > Segment_List_Pooled;
typedef unrolled_list<Segment const *, CMO_Allocator<Segment const *> > Segment_List;


typedef unrolled_list<Shell *, POOL_Allocator<Shell *> > Shell_List;
typedef unrolled_list<Intersecting_PSeg *, POOL_Allocator<Intersecting_PSeg *>, 256> Intersecting_PSeg_List;

typedef unrolled_list<void const *, POOL_Allocator<void const *> > void_ptr_List;

typedef unrolled_list<Conditions const *, POOL_Allocator<Conditions const *> > Conditions_List;
typedef unrolled_list<Geometry const *, POOL_Allocator<Geometry const *> > Geometry_List;


typedef std::vector<Name, CMO_Allocator<Name> > Name_Vector;

typedef Unordered<Name, intptr_t, NameHasher,
	std::equal_to<const Name>, POOL_Allocator<std::pair<const Name, intptr_t> >> intptr_t_Lookup_Hash;


typedef Unordered<Name, Named_Style_Tracker_Node *, NameHasher,
	std::equal_to<const Name>, POOL_Allocator<std::pair<const Name, Named_Style_Tracker_Node*> > > Named_Style_Tracker;

struct Segment_Count_Node {
	Segment_Count_Node() : segment(null), count() {};
	Segment_Count_Node(Segment const * segment, size_t count) : segment(segment), count(count) {};
	Segment const * segment;
	size_t			count;
};
typedef std::vector<Segment_Count_Node, CMO_Allocator<Segment_Count_Node> > Segment_Count_Node_Vector;

typedef Unordered<Name, Segment_Count_Node_Vector, NameHasher,
	std::equal_to<const Name>, POOL_Allocator<std::pair<const Name, Segment_Count_Node_Vector> > > Texture_Tracker;

typedef Unordered<Name, Named_Material *, NameHasher,
	std::equal_to<const Name>, CMO_Allocator<std::pair<const Name, Named_Material*> > > Named_Material_Lookup;


typedef Trivial_LRU<Segment const *> Segment_LRU;

#define ITEM_SEEN_SET_SIZE		128
typedef VXSet<void const *, Hasher<void const *>, std::equal_to<void const *>, POOL_Allocator<void const *> > Item_Seen_Set;

typedef VHash<Segment const *, int> Segment_Int_Hash;

typedef VXMap<Segment const *, int, Hasher<Segment const *>,
	std::equal_to<Segment const *>, POOL_Allocator<std::pair<Segment const * const, int> > > Segment_Int_Map;

typedef VXSet<Geometry const *, Hasher<Geometry const *>, std::equal_to<Geometry const *>, POOL_Allocator<Geometry const *> > Geometry_Set;

typedef VXSet<Segment const *, Hasher<Segment const *>, std::equal_to<Segment const *>, POOL_Allocator<Segment const *> > Segment_Set;

typedef VXMap<intptr_t, intptr_t, Hasher<intptr_t>,
	std::equal_to<intptr_t>, POOL_Allocator<std::pair<const intptr_t, intptr_t> > > intptr_t_Map;

typedef VXMap<intptr_t, size_t, Hasher<intptr_t>,
	std::equal_to<intptr_t>, POOL_Allocator<std::pair<const intptr_t, size_t> > > intptr_size_t_Map;

typedef VXMap<Key, Highlight_Geometry *, Hasher<Key>,
	std::equal_to<Key>, POOL_Allocator<std::pair<Key const, Highlight_Geometry *> > > Geometry_Highlight_Map;

typedef VXMap<Key, Geometry_Highlight *, Hasher<Key>,
	std::equal_to<Key>, POOL_Allocator<std::pair<Key const, Geometry_Highlight *> > > Geometry_Highlights;

typedef VHash<intptr_t, intptr_t> intptr_t_Hash;
typedef VList<intptr_t> intptr_t_List;

typedef VHash<Deferred_Draw_Tree *, intptr_t> Deferred_Draw_Tree_Hash;

typedef Unordered<uint32_t, void *, Hasher<uint32_t>,
	std::equal_to<uint32_t>, CMO_Allocator<std::pair<const uint32_t, void *> > > By_Index_Hash;

typedef std::vector<unsigned char, CMO_Allocator<unsigned char> > By_Index_Data;

typedef Unordered<intptr_t, By_Index_Data *, Hasher<intptr_t>,
	std::equal_to<intptr_t>, CMO_Allocator<std::pair<const intptr_t, By_Index_Data *> > > By_Index_Data_Hash;

typedef VXMap<Segment const *, Highlight_Segment *, Hasher<Segment const *>,
	std::equal_to<Segment const *>, POOL_Allocator<std::pair<Segment const * const, Highlight_Segment *> > > Segment_Highlight_Map;


typedef std::vector<Highlight_Segment const *, POOL_Allocator<Highlight_Segment const *> > Highlight_Segment_Vector;
typedef unrolled_list<Highlight_Segment const *, CMO_Allocator<Highlight_Segment const *> > Highlight_Segment_List;
typedef unrolled_list<Quickmoved_Segment const *, CMO_Allocator<Quickmoved_Segment const *> > Quickmoved_Segment_List;

typedef std::vector<Display_List *, POOL_Allocator<Display_List *> > Display_List_Vector;

typedef VHash<Segment const *, Mimic_Info *> Mimic_Info_Hash;

typedef std::vector<uint32_t, CMO_Allocator<uint32_t> > uint32_t_Vector;
typedef std::vector<uint32_t, POOL_Allocator<uint32_t> > uint32_t_Vector_Pooled;

typedef std::vector<Pending_Activity, CMO_Allocator<Pending_Activity> > Activity_Vector;

typedef std::vector<Image const *, CMO_Allocator<Image const *> > Image_Vector;


/*****************************************************************************/

// most string operations can be handled using builtins 
GLOBAL_UTILITY_FUNCTION char const * HUI_Last_Substring_In_String (char const *substr, char const *string);
GLOBAL_UTILITY_FUNCTION char const * HUI_CI_Last_Substring_In_String (char const *substr, char const *string);
GLOBAL_UTILITY_FUNCTION char const * HUI_CI_First_Substring_In_String (char const *substr, char const *string);
GLOBAL_UTILITY_FUNCTION bool HUI_CI_Equal_Strings (char const *a, char const *b);
GLOBAL_UTILITY_FUNCTION bool HUI_CI_Equal_Strings_To_Max (char const *a, int max, char const *b);

GLOBAL_UTILITY_FUNCTION bool HUI_Wild_Equal_Strings (char const *a, char const *p);
GLOBAL_UTILITY_FUNCTION bool HUI_CI_Wild_Equal_Strings (char const *a, char const *p);
GLOBAL_UTILITY_FUNCTION bool HUI_CI_Wild_Equal_Strings_To_Max (char const *a, size_t max, char const *p);

// maybe allow use of builtins like stricmp()/strcasecmp() and "n" versions,
// but need to see if they have incompatible "locale" issues.

#ifndef STRING_CASE_INSENSITIVE_COMPARE
#	define STRING_CASE_INSENSITIVE_COMPARE			!HUI_CI_Equal_Strings
#endif
#ifndef STRING_CASE_INSENSITIVE_LIMITED
#	define _CI_EQUAL_LIMITED_REORDER_(_a_,_b_,_m_)	HUI_CI_Equal_Strings_To_Max(_a_,_m_,_b_)
#	define STRING_CASE_INSENSITIVE_LIMITED			!_CI_EQUAL_LIMITED_REORDER_
#endif

#define STRING_LENGTH(_str_)					strlen(_str_)
#define COMPARE_STRINGS(_s1_,_s2_)				strcmp(_s1_,_s2_)
#define COMPARE_STRINGS_LIMITED(_s1_,_m_,_s2_)	strncmp(_s1_,_s2_,_m_)
#define EQUAL_STRINGS(_s1_,_s2_)				(!strcmp(_s1_,_s2_))
#define EQUAL_STRINGS_LIMITED(_s1_,_m_,_s2_)	(!strncmp(_s1_,_s2_,_m_))
#define CI_EQUAL_STRINGS(_s1_,_s2_)				(!STRING_CASE_INSENSITIVE_COMPARE(_s1_,_s2_))
#define CI_EQUAL_STRINGS_LIMITED(_s1_,_s2_,_m_) (!STRING_CASE_INSENSITIVE_LIMITED(_s1_,_s2_,_m_))
/* memcmp() like strncmp(), but doesn't need checks for null termination */
#define EQUAL_MEMORY(_s1_,_m_,_s2_)				(!memcmp(_s1_,_s2_,_m_))
#define COMPARE_MEMORY(_s1_,_m_,_s2_)			memcmp(_s1_,_s2_,_m_)

#define COPY_STRING(_in_,_out_)					strcpy(_out_,_in_)
#define COPY_STRING_LIMITED(_in_,_m_,_out_)		((_in_==0)?_out_[0]='\0':(strncpy(_out_,_in_,_m_), _out_[_m_ - 1] = '\0'))
#define APPEND_STRING(_in_,_out_)				strcat(_out_,_in_)
#define APPEND_STRING_LIMITED(_in_,_m_,_out_)	strncat(_out_,_in_,_m_)

#define FIRST_CHAR_IN_STRING(_chr_,_str_)		strchr(_str_,_chr_)
#define LAST_CHAR_IN_STRING(_chr_,_str_)		strrchr(_str_,_chr_)
#define FIRST_SUBSTRING(_needle_,_haystack_)	strstr(_haystack_,_needle_)
#define LAST_SUBSTRING(_sub_,_str_)				HUI_Last_Substring_In_String(_sub_,_str_)
#define CI_FIRST_SUBSTRING(_needle_,_haystack_)	HUI_CI_First_Substring_In_String(_needle_,_haystack_)
#define CI_LAST_SUBSTRING(_sub_,_str_)			HUI_CI_Last_Substring_In_String(_sub_,_str_)


// COUNTED POINTERS

class HOOPS_UTILITY_PRIVATE_API Counted_Object : public CMO {
private:
	mutable int				utility;

protected:
	Counted_Object () : utility (0) {}
	Counted_Object (Counted_Object const &) : utility (0) {}

public:
	INLINE void initialize() const	{ utility = 1; }
	INLINE void	retain () const		{ ++utility; }
	INLINE bool	release () const	{ 
		if (--utility == 0) { 
			delete this; 
			return true;
		}
		return false;
	}

	INLINE bool	shared () const		{ return utility > 1; }

	INLINE bool	unused () const		{ return utility == 0; }

private:
	Counted_Object const &	operator= (Counted_Object const & x);		// no assignment, only copy
};

INLINE bool utility_valid (uint32_t const * value) {
#ifdef VALIDATE_MEMORY
	if (*value == 0x55555555) {
		HE_DATA_ERROR("detected modify after free");
		return false;
	}
#else
	UNREFERENCED(value);
#endif
	return true;
}

class HOOPS_UTILITY_PRIVATE_API Counted_Object_Interlocked : public CMO {
protected:
	mutable uint32_t		utility;

protected:
	Counted_Object_Interlocked () : utility (0) {}
	Counted_Object_Interlocked (Counted_Object_Interlocked const &) : utility (0) {}

public:
	INLINE void initialize() const	{ utility = 1; }
	INLINE void	retain () const {
		if (utility_valid (&utility)) {
			locked_increment (&utility);
		}
	}
	INLINE bool	release () const {
		if (utility_valid (&utility)) {
			if (locked_decrement (&utility) == 0)
			{
				delete this;
				return true;
			}
		}
		return false;
	}

	// will release only if doing so will not cause a delete
	INLINE bool	release_weak () const {
		if (utility_valid (&utility)) {
			while (1) {
				uint32_t current_utility = utility;
				if (current_utility > 1) {
					uint32_t future_utility = current_utility - 1;
					if (locked_CAS(&utility, future_utility, current_utility))
						return true;
				}
				else
					break;
			}
		}
		return false;
	}

	INLINE bool	shared () const		{ return utility > 1; }

	INLINE uint32_t uses () const		{ return utility; }

	INLINE bool	unused () const		{ return utility == 0; }

private:
	void operator= (Counted_Object_Interlocked const &);		// no assignment, only copy
};

class HOOPS_API Counted_Object_Pooled : public CMO_Pooled_Copyable {
private:
	mutable int				utility;

protected:
	Counted_Object_Pooled (Memory_Pool * pool) : CMO_Pooled_Copyable(pool), utility (0) {}
	Counted_Object_Pooled (Counted_Object_Pooled const & x) : CMO_Pooled_Copyable(x), utility (0) {}

public:
	INLINE void initialize() const	{ utility = 1; }
	INLINE void	retain () const		{ ++utility; }
	INLINE void	release () const	{ if (--utility == 0) delete this; }

	INLINE bool	shared () const		{ return utility > 1; }


private:
	Counted_Object_Pooled ();
	Counted_Object_Pooled const &	operator= (Counted_Object_Pooled const & x);		// no assignment, only copy
};


template <typename T>
class Counted_Pointer {
protected:
	T *				counted_item;

public:	//protected:	// public for now so we can let go of things that are in FREEd (rather than destructed) structs
	INLINE void	retain () const		{ if (counted_item != 0) counted_item->retain(); }
	INLINE void	release () const	{ if (counted_item != 0) counted_item->release(); }
	INLINE void initialize() const	{ counted_item->initialize(); }

public:
	Counted_Pointer () : counted_item (0) {};
	Counted_Pointer (Counted_Pointer const & that) : counted_item (that.counted_item) { retain(); }
	Counted_Pointer(Counted_Pointer && that) : counted_item(that.counted_item) { that.counted_item = 0; }
	Counted_Pointer (Null_Object const &) : counted_item (0) {}

	~Counted_Pointer () { release(); }

	INLINE Counted_Pointer const &	operator= (Counted_Pointer const & that) {
		if (that.counted_item != counted_item) {
			T * item = that.counted_item;
			that.retain();
			release();
			counted_item = item;
		}
		return *this;
	}
	INLINE Counted_Pointer const &	operator= (Counted_Pointer && that) {
		release();
		counted_item = that.counted_item;
		that.counted_item = 0;
		return *this;
	}
	INLINE Counted_Pointer const &	operator= (Null_Object const &) {
		release();
		counted_item = 0;
		return *this;
	}

	INLINE bool items_equal(Counted_Pointer const & that) const { 
		if (counted_item == that.counted_item)
			return true;

		if (counted_item == null || that.counted_item == null)
			return false;

		return *counted_item == *that.counted_item; 
	}

	INLINE bool operator== (Counted_Pointer const & that) const { return counted_item == that.counted_item; }
	INLINE bool operator!= (Counted_Pointer const & that) const { return counted_item != that.counted_item; }
	INLINE bool operator== (Null_Object const &) const { return counted_item == 0; }
	INLINE bool operator!= (Null_Object const &) const { return counted_item != 0; }

	INLINE bool operator! () const { return counted_item == 0; }
	INLINE operator bool () const { return counted_item != 0; }

	static Counted_Pointer Create () {
		Counted_Pointer		temp;
		typedef T Counted_Pointer_T;
		temp.counted_item = NEW(Counted_Pointer_T)();
		temp.initialize();
		return temp;
	}

	INLINE Counted_Pointer & Modify () {
		if (counted_item->shared()) {
			typedef T Counted_Pointer_T;
			T *	temp = NEW(Counted_Pointer_T)(*counted_item);
			release();
			counted_item = temp;
			retain();
		}
		return *this;
	}
	INLINE Counted_Pointer Copy () const {
		Counted_Pointer		temp;
		typedef T Counted_Pointer_T;
		temp.counted_item = NEW(Counted_Pointer_T)(*counted_item);
		temp->retain();
		return temp;
	}


	INLINE void	Prefetch () const {
		HOOPS::Prefetch(counted_item);
	}

	// so we can access data elements directly as in C rather than wrapping everything in set/show functions
	INLINE T const *	operator-> () const		{ return counted_item; }
	INLINE T *	operator-> ()		{ return counted_item; }
};


template <typename T>
class Counted_Pointer_Pooled : public Counted_Pointer<T> {
	typedef		Counted_Pointer<T>		CP;		// shorter name so code is readable

private:
	static Counted_Pointer_Pooled Create ();

public:
	Counted_Pointer_Pooled () : CP () {};	// needed because the next one is needed...
	// these don't inherit as needed due to the effective type of the result
	Counted_Pointer_Pooled (Null_Object const &) : CP () {}
	Counted_Pointer_Pooled const &	operator= (Null_Object const &) {
		CP::release();
		CP::counted_item = 0;
		return *this;
	}

	static Counted_Pointer_Pooled	Create (Memory_Pool * pool) {
		Counted_Pointer_Pooled		temp;
		typedef T Counted_Pointer_Pooled_T;
		temp.counted_item = POOL_NEW(pool, Counted_Pointer_Pooled_T) (pool);
		temp.initialize();
		return temp;
	}
	INLINE Counted_Pointer_Pooled & Modify () {
		if (CP::counted_item->shared()) {
			typedef T Counted_Pointer_Pooled_T;
			T *	temp = POOL_NEW(CP::counted_item->memory_pool, Counted_Pointer_Pooled_T)(*CP::counted_item);
			CP::release();
			CP::counted_item = temp;
			CP::retain();
		}
		return *this;
	}
	INLINE Counted_Pointer_Pooled Copy () const {
		Counted_Pointer_Pooled		temp;
		typedef T Counted_Pointer_Pooled_T;
		temp.counted_item = POOL_NEW(CP::counted_item->memory_pool, Counted_Pointer_Pooled_T)(*CP::counted_item);
		temp->retain();
		return temp;
	}
};


// MATH OPS
#ifndef DISABLE_ERROR_WARNING_INFO
#	define Sqrt_ERROR_CHECK if (a < (T)-1e-20f) HE_PROCEDURAL_ERROR("Attempted square root of a negative number");
#else
#	define Sqrt_ERROR_CHECK
#endif

template <typename T>
INLINE T Sqrt (T a) {
	if (a > 0)
		return sqrt (a);
	Sqrt_ERROR_CHECK
		return 0;
}

template <typename T>
INLINE T Pow (T base, T power) {

	if (power == 0.0)
		return 1.0;

	if (power == 1.0)
		return base;

	if (base == 0.0) {
		if (power < 0.0)
			return std::numeric_limits<T>::max();
		else
			return 0.0;
	}

	int	ipower = (int)power;
	if (power == (float)ipower &&
		ipower > 0 &&
		ipower < static_cast<int>(countof(HOOPS::READ_ONLY->power_chain))) {

			char const *	chain = HOOPS::READ_ONLY->power_chain[ipower];

			T	accumulator[10];
			T *	acc = &accumulator[1];

			*acc = base;
			while (*chain) {
				*++acc = accumulator[(int)chain[0]] * accumulator[(int)chain[1]];
				chain += 3; // skips accumulator indices, and whitespace separator
			}
			return *acc;
	}
	else
		return (T)pow (base, power);
}


typedef unsigned char Matrix_Contents;

template <typename F>
class HOOPS_API Matrix_4x4 {
private:
	static F const	identity[16];
protected:

public:
	typedef	F		MR[4];		// Matrix_Row
	F				m[4][4];

	static INLINE void Init (MR const * in_44, MR * out_44, bool perspective=false) {
		COPY_ARRAY(in_44, 16, F, out_44);

		if (!perspective) {
			out_44[0][3] = 0;
			out_44[1][3] = 0;
			out_44[2][3] = 0;
			out_44[3][3] = 1;
		}

		if (out_44[3][3] != 1 && out_44[3][3] != 0) {
			F		recip = (F)1 / out_44[3][3];

			out_44[0][0] *= recip;
			out_44[0][1] *= recip;
			out_44[0][2] *= recip;
			out_44[0][3] *= recip;
			out_44[1][0] *= recip;
			out_44[1][1] *= recip;
			out_44[1][2] *= recip;
			out_44[1][3] *= recip;
			out_44[2][0] *= recip;
			out_44[2][1] *= recip;
			out_44[2][2] *= recip;
			out_44[2][3] *= recip;
			out_44[3][0] *= recip;
			out_44[3][1] *= recip;
			out_44[3][2] *= recip;
			out_44[3][3] *= recip;
		}
	}

#	define Matrix_Contents_NONE			(Matrix_Contents)0x00
#	define Matrix_Contents_ROTATION		(Matrix_Contents)0x01
#	define Matrix_Contents_SCALE		(Matrix_Contents)0x02
#	define Matrix_Contents_TRANSLATION	(Matrix_Contents)0x04
#	define Matrix_Contents_PERSPECTIVE	(Matrix_Contents)0x08
#	define Matrix_Contents_ANYTHING		(Matrix_Contents)0x0F

	static Matrix_Contents Contents(MR const * m, F & symmetric_scale, Matrix_Contents test_contents = Matrix_Contents_ANYTHING);

	static INLINE void Copy(F const * in_44, F * out_44, bool perspective=true) {
		COPY_ARRAY(in_44, 16, F, out_44);
		if (!perspective) {
			MR * out = (MR *)out_44;
			out[0][3] = 0;
			out[1][3] = 0;
			out[2][3] = 0;
			out[3][3] = 1;
		}
	}
	static INLINE void Copy(MR const * in, MR * out, bool perspective=true) {Copy((F const *)in, (F *) out, perspective);}
	static INLINE void Copy(F const * in_44, MR * out, bool perspective=true) {Copy(in_44, (F *) out, perspective);}
	static INLINE void Copy(MR const * in, F * out_44, bool perspective=true) {Copy((F const *)in, out_44, perspective);}

	static bool Verify_Identity(F const * in_44) {
		if (in_44 == null)
			return false;
		for (int i = 0; i < 16; i++)
			if (identity[i] != in_44[i])
				return false;
		return true;
	}
	static INLINE bool Verify_Identity(MR const * in) {return Verify_Identity((F const *)in);}

	static INLINE void Init_Identity(MR *	out) {
#if 1
		COPY_ARRAY(identity, 16, F, out);
#else
		out[0][0] = 1;
		out[0][1] = 0;
		out[0][2] = 0;
		out[0][3] = 0;
		out[1][0] = 0;
		out[1][1] = 1;
		out[1][2] = 0;
		out[1][3] = 0;
		out[2][0] = 0;
		out[2][1] = 0;
		out[2][2] = 1;
		out[2][3] = 0;
		out[3][0] = 0;
		out[3][1] = 0;
		out[3][2] = 0;
		out[3][3] = 1;
#endif
	}
	static INLINE void Init_Identity(F * out_44) {Init_Identity((MR *)out_44);}

	static INLINE F Determinant3x3 (MR const * in) {
		return (in[0][0] * (in[1][1]*in[2][2] - in[2][1]*in[1][2]) -
				in[1][0] * (in[0][1]*in[2][2] - in[2][1]*in[0][2]) +
				in[2][0] * (in[0][1]*in[1][2] - in[1][1]*in[0][2]));
	}
	static INLINE F Determinant3x3(F const * in_44) {return Determinant3x3((MR const *)in_44);}

	static INLINE F Determinant (MR const *	a) {
		F		a20_31_21_30 = a[2][0]*a[3][1] - a[2][1]*a[3][0],
				a20_32_22_30 = a[2][0]*a[3][2] - a[2][2]*a[3][0],
				a20_33_23_30 = a[2][0]*a[3][3] - a[2][3]*a[3][0],
				a21_32_22_31 = a[2][1]*a[3][2] - a[2][2]*a[3][1],
				a21_33_23_31 = a[2][1]*a[3][3] - a[2][3]*a[3][1],
				a22_33_23_32 = a[2][2]*a[3][3] - a[2][3]*a[3][2];
		F		p0 = a[1][1]*a22_33_23_32 - a[1][2]*a21_33_23_31 + a[1][3]*a21_32_22_31,
				p1 = a[1][0]*a22_33_23_32 - a[1][2]*a20_33_23_30 + a[1][3]*a20_32_22_30,
				p2 = a[1][0]*a21_33_23_31 - a[1][1]*a20_33_23_30 + a[1][3]*a20_31_21_30,
				p3 = a[1][0]*a21_32_22_31 - a[1][1]*a20_32_22_30 + a[1][2]*a20_31_21_30;
		return	a[0][0]*p0 - a[0][1]*p1 + a[0][2]*p2 - a[0][3]*p3;
	}
	static INLINE F Determinant(F const * in_44) {return Determinant((MR const *)in_44);}

	static bool Compute_Inverse(MR const * in, MR * out);
	static INLINE bool Compute_Inverse(MR const * in, F * out_44) {return Compute_Inverse(in, (MR *)out_44);}
	static INLINE bool Compute_Inverse(F const * in_44, F * out_44) {return Compute_Inverse((MR const *)in_44, (MR *)out_44);}
	static INLINE bool Compute_Inverse(F const * in_44, MR * out) {return Compute_Inverse((MR const *)in_44, out);}

	static void Compute_Adjoint(MR const * in, MR *	out);
	static INLINE void Compute_Adjoint(MR const * in, F * out_44) {Compute_Adjoint(in, (MR *)out_44);}
	static INLINE void Compute_Adjoint(F const * in_44, MR * out) {Compute_Adjoint((MR const *)in_44, out);}
	static INLINE void Compute_Adjoint(F const * in_44, F * out_44) {Compute_Adjoint((MR const *)in_44, (MR *)out_44);}

	static void Compute_Transpose(MR const * in, MR *	out);
	static INLINE void Compute_Transpose(MR const * in, F * out_44) {Compute_Transpose(in, (MR *)out_44);}
	static INLINE void Compute_Transpose(F const * in_44, MR * out) {Compute_Transpose((MR const *)in_44, out);}
	static INLINE void Compute_Transpose(F const * in_44, F * out_44) {Compute_Transpose((MR const *)in_44, (MR *)out_44);}

	static void Init_Rotation(F const & x, F const & y, F const & z, MR * out);
	static INLINE void Init_Rotation(F const & x, F const & y, F const & z, F * out_44) {Init_Rotation(x, y, z, (MR *)out_44);}

	static INLINE void Init_Scale(F const & x, F const & y, F const & z, MR * m) {
		m[0][0] = x;		m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
		m[1][0] = 0;		m[1][1] = y;		m[1][2] = 0;		m[1][3] = 0;
		m[2][0] = 0;		m[2][1] = 0;		m[2][2] = z;		m[2][3] = 0;
		m[3][0] = 0;		m[3][1] = 0;		m[3][2] = 0;		m[3][3] = 1;
	};
	static INLINE void Init_Scale(F const & x, F const & y, F const & z, F * out_44) {Init_Scale(x, y, z, (MR *)out_44);}

	static void Init_Offaxis_Rotation (Vector const & in_axis, float const & angle, MR * out);
	static INLINE void Init_Offaxis_Rotation (Vector const & in_axis, float const & angle, F * out_44)
	{Init_Offaxis_Rotation(in_axis, angle, (MR *)out_44);}


	static INLINE void Init_Translation(F const & x, F const & y, F const & z, MR * m) {
		m[0][0] = 1;		m[0][1] = 0;		m[0][2] = 0;		m[0][3] = 0;
		m[1][0] = 0;		m[1][1] = 1;		m[1][2] = 0;		m[1][3] = 0;
		m[2][0] = 0;		m[2][1] = 0;		m[2][2] = 1;		m[2][3] = 0;
		m[3][0] = x;		m[3][1] = y;		m[3][2] = z;		m[3][3] = 1;
	}
	static INLINE void Init_Translation(F const & x, F const & y, F const & z, F * out_44)
	{Init_Translation(x, y, z, (MR *)out_44);}

	static INLINE void Init_Column_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3,
		Vector_3D<F> const & translate, MR * m) {
			m[0][0] = v1.x;
			m[1][0] = v1.y;
			m[2][0] = v1.z;
			m[3][0] = translate.x;

			m[0][1] = v2.x;
			m[1][1] = v2.y;
			m[2][1] = v2.z;
			m[3][1] = translate.y;

			m[0][2] = v3.x;
			m[1][2] = v3.y;
			m[2][2] = v3.z;
			m[3][2] = translate.z;

			m[0][3] = 0;
			m[1][3] = 0;
			m[2][3] = 0;
			m[3][3] = 1;
	}
	static INLINE void Init_Column_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3, MR * m)
	{Init_Column_Basis (v1, v2, v3, Vector_3D<F>::Zero(), m);}

	static INLINE void Init_Row_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3,
		Vector_3D<F> const & translate, MR * m) {
			m[0][0] = v1.x;
			m[0][1] = v1.y;
			m[0][2] = v1.z;
			m[0][3] = 0;

			m[1][0] = v2.x;
			m[1][1] = v2.y;
			m[1][2] = v2.z;
			m[1][3] = 0;

			m[2][0] = v3.x;
			m[2][1] = v3.y;
			m[2][2] = v3.z;
			m[2][3] = 0;

			m[3][0] = translate.x;
			m[3][1] = translate.y;
			m[3][2] = translate.z;
			m[3][3] = 1;
	}
	static INLINE void Init_Row_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3, MR * m)
	{Init_Row_Basis (v1, v2, v3, Vector_3D<F>::Zero(), m);}

	static bool Normalize(MR const * in_44, MR * out_44) {
		F	 det = Determinant(in_44);
		if (det != 0) {
			F	invdet = F(1) / det;

			out_44[0][0] = in_44[0][0] * invdet;
			out_44[0][1] = in_44[0][1] * invdet;
			out_44[0][2] = in_44[0][2] * invdet;
			out_44[0][3] = in_44[0][3] * invdet;

			out_44[1][0] = in_44[1][0] * invdet;
			out_44[1][1] = in_44[1][1] * invdet;
			out_44[1][2] = in_44[1][2] * invdet;
			out_44[1][3] = in_44[1][3] * invdet;

			out_44[2][0] = in_44[2][0] * invdet;
			out_44[2][1] = in_44[2][1] * invdet;
			out_44[2][2] = in_44[2][2] * invdet;
			out_44[2][3] = in_44[2][3] * invdet;

			out_44[3][0] = in_44[3][0] * invdet;
			out_44[3][1] = in_44[3][1] * invdet;
			out_44[3][2] = in_44[3][2] * invdet;
			out_44[3][3] = in_44[3][3] * invdet;
			return true;
		}
		return false;
	}
	static bool Normalize(F const * in_44, F * out_44) {return Normalize((MR const *)in_44, (MR *)out_44);}

	static void Compute_Product(MR const * in_left, MR const * in_right, MR * out_result, bool perspective=true);
	static INLINE void Compute_Product(MR const * in_left, MR const * in_right, F * out_result_44, bool perspective=true) 
		{Compute_Product(in_left, in_right, (MR *)out_result_44, perspective);}
	static INLINE void Compute_Product(MR const * in_left, F const * in_right_44, MR * out_result, bool perspective=true) 
		{Compute_Product(in_left, (MR const *)in_right_44, out_result, perspective);}
	static INLINE void Compute_Product(MR const * in_left, F const * in_right_44, F * out_result_44, bool perspective=true) 
		{Compute_Product(in_left, (MR const *)in_right_44, (MR *)out_result_44, perspective);}
	static INLINE void Compute_Product(F const * in_left, MR const * in_right, MR * out_result_44, bool perspective=true) 
		{Compute_Product((MR const *)in_left, in_right, out_result_44, perspective);}
	static INLINE void Compute_Product(F const * in_left, MR const * in_right, F * out_result_44, bool perspective=true) 
		{Compute_Product((MR const *)in_left, in_right, (MR *)out_result_44, perspective);}
	static INLINE void Compute_Product(F const * in_left, F const * in_right_44, MR * out_result, bool perspective=true) 
		{Compute_Product((MR const *)in_left, (MR const *)in_right_44, out_result, perspective);}
	static INLINE void Compute_Product(F const * in_left_44, F const * in_right_44, F * out_result_44, bool perspective=true) 
		{Compute_Product((MR const *)in_left_44, (MR const *)in_right_44, (MR *)out_result_44, perspective);}


	static void Transform_Points (MR const * transform, size_t count, Point const * input, Point * output);
	static INLINE void Transform_Points (F const * transform, size_t count, Point const * input, Point * output)
		{Transform_Points ((MR const *)transform, count, input, output);}

	static void Transform_Points (MR const * transform, size_t count, Point const * input, Point * output, float wlimit, float * ws=0);
	static INLINE void Transform_Points (F const * transform, size_t count, Point const * input, Point * output, float wlimit, float * ws=0)
		{Transform_Points ((MR const *)transform, count, input, output, wlimit, ws);}


	static void Transform_Points (MR const * transform, size_t count, DPoint const * input, DPoint * output);
	static INLINE void Transform_Points (F const * transform, size_t count, DPoint const * input, DPoint * output)
		{Transform_Points ((MR const *)transform, count, input, output);}

	static void Transform_Points (MR const * transform, size_t count, DPoint const * input, DPoint * output, float wlimit, float * ws=0);
	static INLINE void Transform_Points (F const * transform, size_t count, DPoint const * input, DPoint * output, float wlimit, float * ws=0)
		{Transform_Points ((MR const *)transform, count, input, output, wlimit, ws);}


	static void Transform_Points (MR const * transform, size_t count, DPoint const * input, Point * output);
	static INLINE void Transform_Points (F const * transform, size_t count, DPoint const * input, Point * output)
		{Transform_Points ((MR const *)transform, count, input, output);}

	static void Transform_Points (MR const * transform, size_t count, DPoint const * input, Point * output, float wlimit, float * ws=0);
	static INLINE void Transform_Points (F const * transform, size_t count, DPoint const * input, Point * output, float wlimit, float * ws=0)
		{Transform_Points ((MR const *)transform, count, input, output, wlimit, ws);}


	static void Transform_Points (MR const * transform, size_t count, Point const * input, DPoint * output);
	static INLINE void Transform_Points (F const * transform, size_t count, Point const * input, DPoint * output)
		{Transform_Points ((MR const *)transform, count, input, output);}


	INLINE float Transform_X(Point const & point) const {
		return float(m[0][0]*point.x + m[1][0]*point.y + m[2][0]*point.z + m[3][0]);
	}
	INLINE float Transform_Y(Point const & point) const {
		return float(m[0][1]*point.x + m[1][1]*point.y + m[2][1]*point.z + m[3][1]);
	}
	INLINE float Transform_Z(Point const & point) const {
		return float(m[0][2]*point.x + m[1][2]*point.y + m[2][2]*point.z + m[3][2]);
	}
	INLINE float Transform_W(Point const & point, bool perspective=true) const {
		if (perspective)
			return float(m[0][3]*point.x + m[1][3]*point.y + m[2][3]*point.z + m[3][3]);
		return 1;
	}

	INLINE double Transform_X(DPoint const & point) const {
		return m[0][0]*point.x + m[1][0]*point.y + m[2][0]*point.z + m[3][0];
	}
	INLINE double Transform_Y(DPoint const & point) const {
		return m[0][1]*point.x + m[1][1]*point.y + m[2][1]*point.z + m[3][1];
	}
	INLINE double Transform_Z(DPoint const & point) const {
		return m[0][2]*point.x + m[1][2]*point.y + m[2][2]*point.z + m[3][2];
	}
	INLINE double Transform_W(DPoint const & point, bool perspective=true) const {
		if (perspective)
			return m[0][3]*point.x + m[1][3]*point.y + m[2][3]*point.z + m[3][3];
		return 1;
	}

	INLINE float Transform_C(Plane const & plane) const {
		return	float(m[2][0] * plane.a + m[2][1] * plane.b + m[2][2] * plane.c + m[2][3] * plane.d);
	}

	INLINE double Transform_C(DPlane const & plane) const {
		return	(m[2][0] * plane.a + m[2][1] * plane.b + m[2][2] * plane.c + m[2][3] * plane.d);
	}

	Point Transform(Point const & point, float const & point_w, float & ws, bool perspective=true) const {
		F		w = 1, inv_w = 1;

		if (perspective) {
			w = m[0][3]*point.x + m[1][3]*point.y + m[2][3]*point.z + m[3][3]*point_w;
			inv_w = 1 / w;
		}

		float		x = (float)((m[0][0]*point.x + m[1][0]*point.y + m[2][0]*point.z + m[3][0]*point_w) * inv_w);
		float		y = (float)((m[0][1]*point.x + m[1][1]*point.y + m[2][1]*point.z + m[3][1]*point_w) * inv_w);
		float		z = (float)((m[0][2]*point.x + m[1][2]*point.y + m[2][2]*point.z + m[3][2]*point_w) * inv_w);

		ws = (float)w;
		return Point(x, y, z);
	}

	Point Transform(Point const & point, float & w) const {
		return Transform(point, 1, w);
	}

	Point Transform(Point const & point) const {
		float	w;
		return Transform(point, 1, w);
	}

	DPoint Transform(DPoint const & point, float const & point_w, float & ws, bool perspective=true) const {
		F		w = 1, inv_w = 1;

		if (perspective) {
			w = F(m[0][3]*point.x + m[1][3]*point.y + m[2][3]*point.z + m[3][3]*point_w);
			inv_w = 1 / w;
		}

		F		x = F(m[0][0]*point.x + m[1][0]*point.y + m[2][0]*point.z + m[3][0]*point_w);
		F		y = F(m[0][1]*point.x + m[1][1]*point.y + m[2][1]*point.z + m[3][1]*point_w);
		F		z = F(m[0][2]*point.x + m[1][2]*point.y + m[2][2]*point.z + m[3][2]*point_w);

		ws = (float)w;
		return DPoint(x * inv_w, y * inv_w, z * inv_w);
	}

	DPoint Transform(DPoint const & point, float & w) const {
		return Transform(point, 1, w);
	}

	DPoint Transform(DPoint const & point) const {
		float	w;
		return Transform(point, 1, w);
	}

	Vector TransformToVector(Plane const & plane, bool normalize) const {
		Vector	v;
		v.x = float(plane.a*m[0][0] + plane.b*m[0][1] + plane.c*m[0][2] + plane.d*m[0][3]);
		v.y = float(plane.a*m[1][0] + plane.b*m[1][1] + plane.c*m[1][2] + plane.d*m[1][3]);
		v.z = float(plane.a*m[2][0] + plane.b*m[2][1] + plane.c*m[2][2] + plane.d*m[2][3]);
		if (normalize)
			v.Normalize();
		return v;
	}

	DVector TransformToVector(DPlane const & plane, bool normalize) const {
		DVector	v;
		v.x = float(plane.a*m[0][0] + plane.b*m[0][1] + plane.c*m[0][2] + plane.d*m[0][3]);
		v.y = float(plane.a*m[1][0] + plane.b*m[1][1] + plane.c*m[1][2] + plane.d*m[1][3]);
		v.z = float(plane.a*m[2][0] + plane.b*m[2][1] + plane.c*m[2][2] + plane.d*m[2][3]);
		if (normalize)
			v.Normalize();
		return v;
	}

	Plane Transform(Plane const & plane, bool normalize) const {
		Plane	new_plane;
		/* "Planes transform as Minv . P" - Newman & Sproull */
		new_plane.a = float(m[0][0] * plane.a + m[0][1] * plane.b + m[0][2] * plane.c + m[0][3] * plane.d);
		new_plane.b = float(m[1][0] * plane.a + m[1][1] * plane.b + m[1][2] * plane.c + m[1][3] * plane.d);
		new_plane.c = float(m[2][0] * plane.a + m[2][1] * plane.b + m[2][2] * plane.c + m[2][3] * plane.d);
		new_plane.d = float(m[3][0] * plane.a + m[3][1] * plane.b + m[3][2] * plane.c + m[3][3] * plane.d);

		if (normalize)
			new_plane.Normalize();

		return new_plane;
	}

	DPlane Transform(DPlane const & plane, bool normalize) const {
		DPlane	new_plane;
		/* "Planes transform as Minv . P" - Newman & Sproull */
		new_plane.a = m[0][0] * plane.a + m[0][1] * plane.b + m[0][2] * plane.c + m[0][3] * plane.d;
		new_plane.b = m[1][0] * plane.a + m[1][1] * plane.b + m[1][2] * plane.c + m[1][3] * plane.d;
		new_plane.c = m[2][0] * plane.a + m[2][1] * plane.b + m[2][2] * plane.c + m[2][3] * plane.d;
		new_plane.d = m[3][0] * plane.a + m[3][1] * plane.b + m[3][2] * plane.c + m[3][3] * plane.d;

		if (normalize)
			new_plane.Normalize();

		return new_plane;
	}

	Vector Transform(Vector const & vector, bool normalize) const {
		Vector	new_vector;
		new_vector.x = float(m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z);
		new_vector.y = float(m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z);
		new_vector.z = float(m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z);

		if (normalize)
			new_vector.Normalize();

		return new_vector;
	}
	DVector Transform(DVector const & vector, bool normalize) const {
		DVector	new_vector;
		new_vector.x = (m[0][0] * vector.x + m[0][1] * vector.y + m[0][2] * vector.z);
		new_vector.y = (m[1][0] * vector.x + m[1][1] * vector.y + m[1][2] * vector.z);
		new_vector.z = (m[2][0] * vector.x + m[2][1] * vector.y + m[2][2] * vector.z);

		if (normalize)
			new_vector.Normalize();

		return new_vector;
	}

	Vector Transform(Vector2D const & vector, bool normalize) const {
		Vector	new_vector;
		new_vector.x = float(m[0][0] * vector.x + m[0][1] * vector.y);
		new_vector.y = float(m[1][0] * vector.x + m[1][1] * vector.y);
		new_vector.z = float(m[2][0] * vector.x + m[2][1] * vector.y);

		if (normalize)
			new_vector.Normalize();

		return new_vector;
	}
	DVector Transform(DVector2D const & vector, bool normalize) const {
		DVector	new_vector;
		new_vector.x = (m[0][0] * vector.x + m[0][1] * vector.y);
		new_vector.y = (m[1][0] * vector.x + m[1][1] * vector.y);
		new_vector.z = (m[2][0] * vector.x + m[2][1] * vector.y);

		if (normalize)
			new_vector.Normalize();

		return new_vector;
	}

};

template <>
void Matrix_4x4<float>::Transform_Points(
	MR const *          matrix,
	size_t				count,
	Point const *       input,
	Point *             output);

template <>
void Matrix_4x4<double>::Transform_Points(
	MR const *          matrix,
	size_t				count,
	Point const *       input,
	Point *             output);

template <>
void Matrix_4x4<float>::Transform_Points(
	MR const *  		matrix,
	size_t				count,
	Point const *       input,
	Point *             output,
	float               wlimit,
	float *             ws);

template <>
void Matrix_4x4<double>::Transform_Points(
	MR const *  		matrix,
	size_t				count,
	Point const *       input,
	Point *             output,
	float               wlimit,
	float *             ws);


typedef	Matrix_4x4<float>	Matrix_44;
typedef	Matrix_4x4<double>	Matrix_44D;

// utility class lets you access memory as if it were defined as a 4x4 array
template <typename F>
class Matrix_Access {
private:
	F				elements[4];
	Matrix_Access();
public:
	F const &		operator[] (int index) const	{ return elements[index]; }
	F &				operator[] (int index)			{ return elements[index]; }
};


End_HOOPS_Namespace






//
#ifdef HOOPS_CC_NONE
#	undef HOOPS_CC_NONE
#endif
#define HOOPS_CC_NONE				0
#define HOOPS_CC_OBJECT				1
#define HOOPS_CC_WORLD				2
#define HOOPS_CC_VIEWPOINT			3
#define HOOPS_CC_PROJECTION			4
#define HOOPS_CC_SCREEN_RANGE		5
#define HOOPS_CC_LOCAL_WINDOW		6
#define HOOPS_CC_LOCAL_PIXELS		7
#define HOOPS_CC_OUTER_WINDOW		8
#define HOOPS_CC_OUTER_PIXELS		9

#define HOOPS_CC_CAMERA		HOOPS_CC_WORLD


Begin_HOOPS_Namespace

/*****************************************************************************/

#define ALIAS_HASH_SIZE					128
#define FONTS_HASH_SIZE					 64
#define TEXTURE_NAME_HASH_SIZE			 32
#define COLOR_NAME_HASH_SIZE			128
#define KEY_HASH_SIZE					512


struct Option_Hash_Table {
	Option_Type **					table;
	Constant_Option_Type const *	source;
	int								size;
	int								count;
};

enum Hash_Types {
	CALLBACK_OPTIONS_HASH,
	CAMERA_PROJECTION_HASH,
	COLOR_HASH,
	COLOR_CHANNEL_HASH,
	CONDITIONS_HASH,
	DRIVER_OPTIONS_HASH,
	HEURISTICS_HASH,
	METAFILE_HASH,
	RENDERING_OPTIONS_HASH,
	SELECTABILITY_HASH,
	TEXT_FONT_HASH,
	TEXTURE_OPTION_HASH,
	USER_OPTIONS_HASH,
	USER_KOPTIONS_HASH,
	VISIBILITY_HASH,
	WINDOW_FRAME_HASH,

	AREA_LIGHT_HASH,
	CHARACTER_ATTRIBUTE_OPTIONS_HASH,
	CURVE_OPTION_HASH,
	CYLINDER_CAP_OPTION_HASH,
	IMAGE_HASH,
	SPOT_LIGHT_HASH,
	TEXT_ENCODING_HASH,
	TEXT_REGION_HASH,
	GEOMETRY_OPTION_HASH,
	GEOMETRY_POINTER_OPTION_HASH,
	LINE_PATTERN_EXPLICIT_HASH,

	DRIVER_CONFIG_HASH,
	FONT_OPTIONS_HASH,
	FONT_SEARCH_HASH,
	SHOW_FONT_HASH,
	SYSTEM_INFO_HASH,
	SYSTEM_OPTIONS_HASH,

	BOUNDING_FILTER_HASH,
	CONTENTS_SEARCH_HASH,
	CONTENTS_KSEARCH_HASH,
	MINIMIZE_OPTIONS_HASH,
	OPTIMIZE_TREE_HASH,
	UPDATE_CONTROL_OPTIONS_HASH,
	DECIMATE_OPTION_HASH,

	BOOLEAN_OPTIONS_HASH,
	DISTANCE_SELECTION_OPTIONS_HASH,
	GENERATE_SHELL_HASH,
	POINTS_INSIDE_SHELL_HASH,
	SHELL_SELECTION_OPTIONS_HASH,
	SWEPT_SHELL_OPTIONS_HASH,
	MEMORY_PROFILE_HASH,
	POINTS_SHELL_DISTANCE_HASH,
	CLIP_REGION_HASH,
	CONDITIONAL_ACTION_HASH,
	HIGHLIGHT_HASH,
	UNDEFINE_HIGHLIGHT_HASH,

	GENERATE_EDGE_HASH,
	PRECISION_HASH,

	ATTRIBUTE_LOCKING_HASH,

	MAX_OPTION_HASH_TABLES
};


struct Int_XY {
	Int_XY():x(0),y(0) {};
	INLINE bool operator == (Int_XY const & that) const {
		return (x == that.x && y == that.y);
	}
	int		x, y;
};

struct Int_Point {
	int		x, y ,z;
};


typedef float Parameter;

#define COPY_PARAMETER(src, dest, width) \
		COPY_ARRAY((src), (width), Parameter, (dest))

#define COPY_N_PARAMETERS(src, dest, width, count) \
		COPY_ARRAY((src), (count)*(width), Parameter, (dest))

#define GET_PARAMETER(vp, offset, out, width) do { \
		COPY_PARAMETER(&(vp)[(offset)*(width)], (out), width); \
	} while (0)

#define INIT_PARAMETER(param,width) \
		ZERO_ARRAY((param), (width), Parameter)

#define INIT_N_PARAMETERS(param,count,width) \
		ZERO_ARRAY((param), (count)*(width), Parameter)


typedef unsigned char Test;

enum Test_Bits : Test {
	Test_DISJOINT				= 0x00,
	Test_INSCRIBED				= 0x01,
	Test_INTERSECTING			= 0x02,
	Test_EXTENT					= 0x04,
	Test_VOLUME					= 0x08,
	Test_INSCRIBED_VOLUME		= (Test_INSCRIBED | Test_VOLUME),
	Test_INTERSECTING_VOLUME	= (Test_INTERSECTING | Test_VOLUME),
	Test_INSCRIBED_EXTENT		= (Test_INSCRIBED | Test_EXTENT),
	Test_INTERSECTING_EXTENT	= (Test_INTERSECTING | Test_EXTENT),
};

typedef unsigned char Bounding_Type;
typedef int Bounding_Flags;


typedef uint32_t	HFSB_Flags;

enum HFSB_Flags_Bits : HFSB_Flags {
	HFSB_IGNORE_VISIBILITY		= 0x0001,
	HFSB_DETAILED				= 0x0002,
	HFSB_SKIP_SHADOW_POINTS		= 0x0004,
	HFSB_CRUDE					= 0x0008,
	HFSB_OBEY_EXCLUDE			= 0x0010,
	HFSB_EXCLUDE_CAMERA			= 0x0020,
	HFSB_WANT_EXCLUDE			= 0x0040,
	HFSB_RESPECT_LOCKING		= 0x0080,
	HFSB_IGNORE_CLIP_REGION		= 0x0100,
	HFSB_MINIMAL				= 0x0200,
	HFSB_POINTS_REQUIRED		= 0x0400,
	HFSB_RETAIN_DOUBLE			= 0x0800,
	HFSB_IGNORE_ARC_LIMITS		= 0x1000,
};

#define DECIPHER_POLY_VALUES(_count_, _bump_, _natural_) do {						\
			if (_count_ > 0) {	_count_ -= (_natural_ - 1); _bump_ = 1;			}	\
			else {				_count_ /= -(_natural_);	_bump_ = _natural_; }	\
		} while (0)

class Name_Const;

class HOOPS_API Name {
public:
	enum Create_Mode {
		None,
		Canonize,
		CanonizeRemoveQuote,
		CanonizeRetainQuote,
	};

	Name() : _text(0), _length(0), _key(0), _lower_sum(0) {};

	Name(Null_Object const &) : _text(0), _length(0), _key(0), _lower_sum(0) {};

	~Name();
	Name(Name const & that);

	// Move constructor.
	Name(Name && other) {
		_text = other._text;
		_length = other._length;
		_key = other._key;
		_lower_sum = other._lower_sum;
		other._text = 0;
		other._length = 0;
		other._key = 0;
		other._lower_sum = 0;
	}

	// Move assignment operator.
	Name & operator=(Name && other);

	Name(Memory_Pool * pool, Name_Const const & that, Create_Mode create_mode=None);
	Name(Memory_Pool * pool, const char * in, Create_Mode create_mode=None);

	explicit Name(const char * in, Create_Mode create_mode=None);
	Name(Name_Const const & that, Create_Mode create_mode=None);

	Name(char const * that, size_t length);

	Name const &	operator= (Name const & that);
	Name const &	operator= (Name_Const const & that);
	Name const &	operator= (const char * in);

	bool operator == (Name const & that) const;
	bool operator != (Name const & that) const;

	bool operator == (Name_Const const & that) const;
	bool operator != (Name_Const const & that) const;

	INLINE bool operator== (Null_Object const &) const { return _text == 0; }
	INLINE bool operator!= (Null_Object const &) const { return _text != 0; }

	INLINE Name const &	operator= (Null_Object const &) {
		*this = Name();
		return *this;
	}

	INLINE operator Name_Const const & () const {return *((Name_Const*)this);}
	INLINE operator Name_Const const & () {return *((Name_Const*)this);}
	INLINE operator Name_Const const * () const {return ((Name_Const*)this);}
	INLINE operator Name_Const const * () {return ((Name_Const*)this);}

	INLINE bool CI_Equal(Name const & that) const;
	INLINE bool CI_Equal(Name_Const const & that) const;

	INLINE void to_chars(char * out, size_t out_n) const {
		size_t n = Min(out_n-1, (size_t)length());
		memcpy(out, text(), n);
		out[n] = '\0';
	}

	INLINE bool empty() const { return (_length == 0); }
	INLINE size_t length() const {return _length;}
	INLINE const char * text() const {
		return _text;
	}

	INLINE int32_t lower_sum() const;
	INLINE int32_t hash_key() const;

private:
	char *			_text;
	size_t			_length;
	mutable int32_t	_key;
	mutable int32_t	_lower_sum;

	void	init(Name_Const const & that, Name::Create_Mode create_mode, Memory_Pool * pool);
	void	canonize(char const * s, char *& text, size_t & length, Memory_Pool * pool);
	void	canonize_quoted(char const * s, char *& text, size_t & length, bool retain_quotes, Memory_Pool * pool);
};

template <typename T, typename U>
INLINE void simple_string_to_hash (T const * string, U & hash) {
	/* djb2 */
	hash = 5381;
	if (string != null) {
		int			c;
		while ((c = (int)*string++) != 0)
			hash = ((hash << 5) + hash) ^ c;
	}
}

struct NameHasher {
	size_t operator()(const Name & name) const{
		return (size_t)name.hash_key();
	}
};

#define HASH_KNAME(_n_, _h_)	simple_string_to_hash ((_n_).ktext, _h_)



class Callback_Name_Data : public Counted_Object_Interlocked {

public:

	Void_Routine		address;
	Name				name;
	bool				deleted;

	Callback_Name_Data() : Counted_Object_Interlocked(), address(null), name(), deleted(false) {}
	~Callback_Name_Data() {}

	INLINE bool operator == (Callback_Name_Data const & that) const {

		if (name != that.name)
			return false;
		if (address != that.address)
			return false;
		if (deleted || that.deleted)
			return false;

		return true;
	}

	INLINE bool operator!= (Callback_Name_Data const & that) const	{ return  !(*this == that); }

private:
	Callback_Name_Data(Callback_Name_Data const &);
	Callback_Name_Data const & operator= (Callback_Name_Data const & that);
};

typedef Counted_Pointer<Callback_Name_Data>	Callback_Name;


typedef std::vector<Callback_Name, POOL_Allocator<Callback_Name> > Callback_Name_Vector_Pooled;
typedef std::unordered_map<Name, Callback_Name, NameHasher, std::equal_to<Name>, CMO_Allocator<std::pair<const Name, Callback_Name> > > Callback_Name_Map_Pooled;

/* integer/pointer pair used for User_Option_By_Index */

struct Indexed_User_Option {
	void *	value;
	long	index;	
};


typedef int32_t	Material_Components;
typedef uint32_t	Color_Object;

typedef char		Text_Encoding;

typedef uint32_t	Rendo_Flags;
typedef int32_t		Rendo_Debug;

typedef int32_t		Technology_Flags;

typedef int32_t		Quantization_Flags;
typedef int32_t		HSR_Algorithm;
typedef int32_t		Buffer_Flags;
typedef int32_t		Hidden_Line_Flags;
typedef int32_t		Isoline_Contour_Flags;
typedef int32_t		LOD_Flags;
typedef int32_t		GOOCH_Flags;
typedef int32_t		NURBS_Curve_Flags;
typedef int32_t		NURBS_Surface_Flags;
typedef int32_t		Tessellation_Flags;
typedef int32_t		Cut_Geometry_Flags;
typedef int32_t		Simple_Shadow_Flags;
typedef int32_t		Shadow_Map_Flags;
typedef int32_t		Simple_Reflection_Flags;
typedef int32_t		Color_Depth;
typedef int32_t		Fast_Ambient_Occlusion_Flags;
typedef int32_t		Fast_Silhouette_Flags;
typedef int32_t		Bloom_Flags;
typedef int32_t		Eye_Dome_Lighting_Flags;
typedef int32_t		Depth_Of_Field_Flags;

/* you must keep these types in sync with those in the HIC header */
typedef int32_t			Depth;
typedef char					Image_Format;
#define Image_NONE				(Image_Format)0
#define Image_MAPPED_8			(Image_Format)1
#define Image_MAPPED_16			(Image_Format)2
#define Image_RGB24				(Image_Format)3
#define Image_ARGB32			(Image_Format)4
#define Image_RGBA32			(Image_Format)5
#define Image_DEPTH_FLOAT32		(Image_Format)6
/* internal framebuffer format */
#define Image_RGBAS32			(Image_Format)7
/* other internal formats */
#define Image_DEPTH16			(Image_Format)8
#define Image_STENCIL			(Image_Format)9
#define Image_DEPTH32			(Image_Format)10
#define Image_DXT1				(Image_Format)11
#define Image_DXT3				(Image_Format)12
#define Image_DXT5				(Image_Format)13
#define Image_PNG				(Image_Format)14
#define Image_JPEG				(Image_Format)15
#define Image_TARGA				(Image_Format)16
#define Image_LUMINANCE8		(Image_Format)17


/* Image source types */
#define Image_Source_COPY								0
#define Image_Source_BY_REF								1
#define Image_Source_DRIVER_CREATED_FRAME_BUFFER		2
#define Image_Source_SYSTEM_CREATED_FRAME_BUFFER		3
#define Image_Source_IMAGE								4
#define Image_Source_NONE								5

typedef int				Polygon_Rank;
#define Polygon_SIMPLE					(Polygon_Rank)0x00
#define Polygon_CONCAVE					(Polygon_Rank)0x01
#define Polygon_SELF_INTERSECTING		(Polygon_Rank)0x02



typedef uint32_t	Seg_Flags;

typedef unsigned short			Database_Flags;

#define DB_DELETED				(Database_Flags)0x0001	// for anything
#define DB_OUT_OF_ORDER			(Database_Flags)0x0002	// for geometry, include, segment
#define DB_CONTAINER			(Database_Flags)0x0004	// geometry container type
#define DB_NEEDS_FIXUP			(Database_Flags)0x0008	// geometry edits that need final processing
#define DB_BOUNDING				(Database_Flags)0x0010	// segments & geometry
#define DB_NAUGHTY_BIT			(Database_Flags)0x0020	// segments & geometry
#define DB_DOUBLE_PRECISION		(Database_Flags)0x0040	// double precision object
// next two overlap
#define DB_HAS_GEO_REFS			(Database_Flags)0x0080	// for segments tracking geo refs
#define DB_HAS_HIGHLIGHTS		(Database_Flags)0x0080	// for geometry tracking highlights

#define DB_TYPE_SPECIFIC(v)		(Database_Flags)(1 << (7+(v)))	/* currently top 8 reserved bits numbered 1-8 */
#define DB_TEMPORARY			DB_TYPE_SPECIFIC(8) // temp geometry (type 8 shared with static tree processing -- no conflict)


#define XBIT_SIZE   2

#define XBIT_BIT(item)		(item%32)
#define XBIT_OFFSET(item)	(item/32)

#define XBIT_TEST(a,b) (((a)[XBIT_OFFSET(b)] & (1<<XBIT_BIT(b))) != 0)
#define XBIT_APPEND(a,b) (((a)[XBIT_OFFSET(b)] |= (1<<XBIT_BIT(b))))

typedef int32_t Driver_Config_Flags;

class Name_List;

#define HTDF_NONE		(Driver_Config_Flags)0x00000000
#define HTDF_SOFTWARE	(Driver_Config_Flags)0x00000001

struct Driver_Config {
	Name				driver;
	Name				id;
	Name				version;
	Name				original_config_string;

	Driver_Config_Flags	flags;

	int32_t				xbits[XBIT_SIZE];

	Name_List *			config;

	int					tristrips_min;
	int					dl_tristrips_min;
	int					flat_tristrips_min;
	int					flat_dl_tristrips_min;

	Driver_Config *		next;
};


struct Selection_Event_CD {
	DPoint	dhit_location;
	Point	hit_location;
};


struct XBits {
	const char *	options;
	int32_t	bit;
};

#define OPTIMIZED_MAPPING_HASH_SIZE	256

struct Optimized_Mapping {
	Optimized_Mapping *	forward;
	Optimized_Mapping *	backward;
	struct {
		Key				owner;
		Key				key;
		int				region;
	} original, optimized;
};

struct Optimized_Mapping_Table {
	Optimized_Mapping_Table *	next;
	Segment const *	segment;
	Optimized_Mapping *	forward[OPTIMIZED_MAPPING_HASH_SIZE];
	Optimized_Mapping *	backward[OPTIMIZED_MAPPING_HASH_SIZE];
};

struct Key_Hash {
	Key_List **	table;
	int32_t		size;
	int32_t		entries;
};


typedef	void (HC_CDECL *System_Cleanup_Hook_Action)(void);

typedef Attribute * (HC_CDECL * Attribute_Acquisitor_Action)
(Thread_Data * thread_data, Anything * item, Attribute const * old_attr, Attribute * new_attr);


typedef bool (HC_CDECL * UnSet_Attribute_Action)
(Thread_Data * thread_data, Anything * item, Attribute * old_attr, Attribute const * unset_proto);

struct Key_Entry {
	union {
		Anything *		item;			// normal usage
		Key_Entry *		next_empty;		// to chain empty entries
	};
	Key				key;
	int				offset;				// -1 on empties
};

#define Sysop_DS_NO_UPDATE_CONTROL			0x00000001
#define Sysop_DS_NO_GEOMETRY_BOUNDING		0x00000002




struct Definition_Set {
	Segment	const *					texture_root;
	Alias **						alias_hash_table;
	Font **							defined_fonts_hash_table;
	Color_Name **					color_names_hash_table;
	Color_Def *						color_defs;	
	Destructible<Callback_Name_Map_Pooled>::unique_ptr	callback_names_hash_table;
	Destructible<Named_Material_Lookup>::unique_ptr		named_material_lookup;
	Embedaphore						named_material_lookup_benaphore;
	Key_Hash						global_keys;
	Name							metafile_root;
	int32_t							flags;
};


struct Thread_Data;



enum Type_Flags : uint32_t {

	T_NONE						= 0x00000000,
	T_ALL_BITS					= 0xFFFFFFFF,

	T_FACES						= 0x00000001,
	T_EDGES						= 0x00000002,
	T_LINES						= 0x00000004,
	T_MARKERS					= 0x00000008,
	T_VERTICES					= 0x00000010,
	T_IMAGES					= 0x00000020,
	T_TEXT						= 0x00000040,
	T_STRING_CURSORS			= 0x00000080,
	T_WINDOWS					= 0x00000100,
	T_CUTTING_PLANES			= 0x00000200,
	T_SHARED = (T_FACES | T_EDGES | T_LINES | T_MARKERS | T_VERTICES | T_IMAGES | T_TEXT | T_STRING_CURSORS | T_WINDOWS | T_CUTTING_PLANES),

	T_INTERIOR_SILHOUETTE_EDGES	= 0x00010000,
	T_PERIMETER_EDGES			= 0x00020000,
	T_ADJACENT_EDGES			= 0x00040000,
	T_MESH_QUAD_EDGES			= 0x00080000,
	T_HARD_EDGES				= 0x00100000,
	T_NONCULLED_EDGES			= 0x00200000,

	T_LIGHTS_FOR_FACES			= 0x00400000,
	T_LIGHTS_FOR_EDGES			= 0x00800000,
	T_LIGHTS_FOR_MARKERS		= 0x01000000,

	T_ISOLINES					= 0x02000000,

	T_SHADOW_RECEIVING			= 0x04000000,
	T_SHADOW_CASTING			= 0x08000000,
	T_SHADOW_EMITTING			= 0x10000000,
	T_ANY_SHADOW = (T_SHADOW_RECEIVING | T_SHADOW_CASTING | T_SHADOW_EMITTING),

	T_CUT_FACES					= 0x20000000,
	T_CUT_EDGES					= 0x40000000,
	T_ANY_CUT_GEOMETRY = (T_CUT_FACES | T_CUT_EDGES),

	// used for visibility parsing and bounding
	T_SPECIAL					= 0x80000000,

	T_GENERIC_EDGES = T_EDGES,
	T_ANY_POLYGON_EDGE = (T_GENERIC_EDGES | T_PERIMETER_EDGES | T_ADJACENT_EDGES),
	T_ANY_GRID_EDGE = T_ANY_POLYGON_EDGE,
	T_ANY_SHELL_EDGE = (T_ANY_POLYGON_EDGE | T_HARD_EDGES | T_INTERIOR_SILHOUETTE_EDGES | T_NONCULLED_EDGES),
	T_ANY_MESH_EDGE = (T_ANY_SHELL_EDGE | T_MESH_QUAD_EDGES),
	T_ANY_SPHERE_EDGE = (T_GENERIC_EDGES | T_INTERIOR_SILHOUETTE_EDGES | T_MESH_QUAD_EDGES | T_NONCULLED_EDGES),
	T_ANY_VIEW_INDEPENDENT_EDGE = (T_EDGES | T_MESH_QUAD_EDGES | T_HARD_EDGES | T_PERIMETER_EDGES | T_ADJACENT_EDGES),
	T_ANY_EDGE = T_ANY_MESH_EDGE,

	T_ANY_LIGHTS = (T_LIGHTS_FOR_FACES | T_LIGHTS_FOR_EDGES | T_LIGHTS_FOR_MARKERS),

#ifndef HPS_CORE_BUILD
	T_ANY_DRAWABLE_GEOMETRY = (T_FACES | T_ANY_EDGE | T_LINES | T_MARKERS | T_VERTICES | T_IMAGES | T_TEXT | T_STRING_CURSORS | T_ANY_LIGHTS),
#else
	T_ANY_DRAWABLE_GEOMETRY = (T_FACES | T_ANY_EDGE | T_LINES | T_MARKERS | T_VERTICES | T_TEXT | T_ANY_LIGHTS),
#endif

	T_ANY_SELECTABLE_GEOMETRY = T_ANY_DRAWABLE_GEOMETRY,
	T_ANY_COLORED_GEOMETRY = T_ANY_DRAWABLE_GEOMETRY,
	T_ANY_SELECTABILITY_STUFF = (T_ANY_SELECTABLE_GEOMETRY | T_WINDOWS | T_ISOLINES),
	T_ANY_GEOMETRY = (T_ANY_DRAWABLE_GEOMETRY | T_CUTTING_PLANES),
	T_ANY_REDRAWABLE_STUFF = (T_ANY_GEOMETRY | T_WINDOWS),
	T_ANY_VISIBILITY_STUFF = (T_ANY_GEOMETRY | T_WINDOWS | T_ANY_CUT_GEOMETRY | T_ANY_SHADOW),
};

template<> struct _bit_ops_type < Type_Flags > {
	typedef Type_Flags type;
};


enum Seen_Flags : uint32_t {

	S_NONE						= 0x00000000,
	S_ALL_BITS					= 0xFFFFFFFF,

	S_FACES						= 0x00000001,
	S_EDGES						= 0x00000002,
	S_LINES						= 0x00000004,
	S_MARKERS					= 0x00000008,
	S_VERTICES					= 0x00000010,
	S_IMAGES					= 0x00000020,
	S_TEXT						= 0x00000040,
	S_STRING_CURSORS			= 0x00000080,
	S_WINDOWS					= 0x00000100,
	S_CUTTING_PLANES			= 0x00000200,
	S_SHARED = (S_FACES | S_EDGES | S_LINES | S_MARKERS | S_VERTICES | S_IMAGES | S_TEXT | S_STRING_CURSORS | S_WINDOWS | S_CUTTING_PLANES),

	S_CAMERA_BELOW				= 0x00001000,
	S_GENERIC_VISIBLE			= 0x00002000,
	S_GLYPHS_VISIBLE			= 0x00004000,
	S_LINES_VISIBLE				= 0x00008000,
	S_WINDOWS_VISIBLE			= 0x00010000,
	S_GLYPH_SIZE				= 0x00020000,
	S_ODD_3D					= 0x00040000,
	S_CONDITIONALS				= 0x00080000,
	S_DITHER_INDUCING_GEOMETRY	= 0x00100000,
	S_GEOMETRY_SELECTABLE		= 0x00200000,
	S_WINDOWS_SELECTABLE		= 0x00400000,
	S_WINDOW_PATTERNS			= 0x00800000,
	S_TRANSPARENCY_SEEN			= 0x01000000,
	S_SHADOW_SEEN				= 0x02000000,
	S_LIGHT_SEEN				= 0x04000000,
	S_CUTS_SEEN					= 0x08000000,
	S_POPULATED					= 0x10000000,
	S_LOCKING					= 0x20000000,
	S_CALLBACKS					= 0x40000000,
	S_SEEN_ANTIQUATED			= 0x80000000,

	S_BLOCKED_BY_WINDOWS = (S_TRANSPARENCY_SEEN | S_SHADOW_SEEN | S_LIGHT_SEEN | S_CUTS_SEEN | S_SEEN_ANTIQUATED),

	S_BLOCKED_BY_QUICKMOVES = (S_TRANSPARENCY_SEEN | S_SEEN_ANTIQUATED),

	S_GEOMETRY_VISIBLE = (S_GENERIC_VISIBLE | S_GLYPHS_VISIBLE | S_LINES_VISIBLE),

	S_SEEN_THROUGH_STYLE = (S_ODD_3D | S_WINDOWS_SELECTABLE | S_WINDOWS_VISIBLE | S_GEOMETRY_SELECTABLE |
							S_GEOMETRY_VISIBLE | S_WINDOW_PATTERNS | S_CAMERA_BELOW | S_DITHER_INDUCING_GEOMETRY | S_CALLBACKS |
							S_CONDITIONALS | S_TRANSPARENCY_SEEN | S_SHADOW_SEEN | S_SEEN_ANTIQUATED),

	S_SEEN_THROUGH_REFERENCE = (S_GEOMETRY_SELECTABLE | S_GEOMETRY_VISIBLE | S_DITHER_INDUCING_GEOMETRY |
								S_CONDITIONALS | S_LIGHT_SEEN | S_CUTS_SEEN | S_SEEN_ANTIQUATED | S_SHARED),
};

template<> struct _bit_ops_type < Seen_Flags > {
	typedef Seen_Flags type;
};


// MUST MATCH TABLE IN SHOW_KEY_TYPE! AND HIC HEADERS & intermediate.cpp's compile-time checking block!


enum DB_Type : unsigned char {
	STYLE					= 0, /* special lockable */

	CONDITIONS				= 1, FIRST_ATTRIBUTE = CONDITIONS,
	TEXTURE_DEFS			= 2,
	GLYPH_DEFS				= 3,
	LINE_STYLE_DEFS			= 4,
	NAMED_STYLE_DEFS		= 5,

	// 'Lockables' need to match HIC_Attribute types
	RENDERING_OPTIONS		= 6, FIRST_LOCKABLE = RENDERING_OPTIONS,
	COLOR_MAP				= 7, // must be before other colors
	SELECTABILITY			= 8,
	VISIBILITY				= 9,
	COLOR					= 10,
	WINDOW_FRAME			= 11, // must be before WINDOW
	WINDOW_PATTERN			= 12,
	WINDOW					= 13, // Not lockable, but keep as early as reasonable
	CAMERA					= 14,
	HEURISTICS				= 15,
	MODELLING_MATRIX		= 16, // through here commonly hunted during processing, so keep early
	IM_CALLBACK				= 17,
	CLIP_REGION				= 18,
	ACTOR					= 19,
	DRIVER_OPTIONS			= 20,
	EDGE_PATTERN			= 21,
	EDGE_WEIGHT				= 22,
	FACE_PATTERN			= 23,
	HANDEDNESS				= 24,
	LINE_PATTERN			= 25,
	LINE_WEIGHT				= 26,
	MARKER_SIZE				= 27,
	MARKER_SYMBOL			= 28,
	TEXT_ALIGNMENT			= 29,
	TEXT_FONT				= 30,
	TEXT_PATH				= 31,
	TEXT_SPACING			= 32,
	USER_OPTIONS			= 33,
	USER_VALUE				= 34,
	TEXTURE_MATRIX			= 35, LAST_LOCKABLE = TEXTURE_MATRIX,
	// 'Lockables' need to match HIC_Attribute types

	UNUSED_1				= 36,
	STREAMING_MODE			= 37, LAST_ATTRIBUTE = STREAMING_MODE,

	// cutting planes and lights first to make preprocessing steps quicker
	CUTTING_PLANE			= 38, FIRST_GEOMETRY = CUTTING_PLANE,
	LIGHT					= 39,
	// complex surface types
	SHELL					= 40,
	MESH					= 41,
	GRID					= 42,
	NURBS_SURFACE			= 43,
	CYLINDER				= 44,
	POLYCYLINDER			= 45,
	SPHERE					= 46,
	// simple polygonal types
	POLYGON					= 47,
	CIRCLE					= 48,
	CIRCULAR_CHORD			= 49,
	CIRCULAR_WEDGE			= 50,
	ELLIPSE					= 51,
	// simple polyline types
	LINE					= 52,
	MULTILINE				= 53,
	POLYLINE				= 54,
	NURBS_CURVE				= 55,
	CIRCULAR_ARC			= 56,
	ELLIPTICAL_ARC			= 57,
	INFINITE_LINE			= 58,
	// miscellaneous
	IMAGE					= 59,
	MARKER					= 60,
	MULTIMARKER				= 61,
	TEXT					= 62,
	STRING_CURSOR			= 63,
	GEOMETRY_REFERENCE		= 64, LAST_GEOMETRY = GEOMETRY_REFERENCE,

	NOTHING					= 65,

	INCLUDE					= 66, FIRST_SUBSEG = INCLUDE,
	SEGMENT					= 67, LAST_SUBSEG = SEGMENT,


	// the rest of these can be in any order...
	LOD_NODE				= 68, FIRST_MISC = LOD_NODE,
	OPEN_ITEM				= 69,
	HOOPS_WORLD				= 70,
	ALIAS					= 71,
	COLOR_DEF				= 72,
	ERROR_HANDLER			= 73,
	EXIT_HANDLER			= 74,
	BUTTON_EVENT			= 75, FIRST_EVENT = BUTTON_EVENT,
	LOCATION_EVENT			= 76,
	SELECTION_EVENT			= 77,
	SPECIAL_EVENT			= 78,
	STRING_EVENT			= 79,
	WAKEUP_EVENT			= 80, LAST_EVENT = WAKEUP_EVENT,
	HOOPS_FONT				= 81,
	TEXTURE					= 82,
	WAKEUP					= 83,
	OPTION_TYPE				= 84,
	UV_TRIM_POLY			= 85,
	UV_TRIM_CURVE			= 86,
	UV_TRIM_COLLECTION		= 87,
	DISPLAY_LIST			= 88,
	CONTOUR					= 89,
	STATIC_MODEL			= 90, LAST_MISC = STATIC_MODEL,


	Light_DISTANT, FIRST_XTYPE = Light_DISTANT,
	Light_POINT,
	Light_SPOT,
	Light_AREA,

	// xtype choice of camera manipulation
	Camera_SET_ALL, // set whole camera
	// modify a component
	Camera_SET_POSITION,
	Camera_SET_TARGET,
	Camera_SET_UP_VECTOR,
	Camera_SET_FIELD,
	Camera_SET_PROJECTION,
	// near limit is always handled separately
	Camera_SET_NEAR_LIMIT,
	// convenience operations
	Camera_SET_DOLLY,
	Camera_SET_ORBIT,
	Camera_SET_PAN,
	Camera_SET_ROLL,
	Camera_SET_ZOOM, LAST_XTYPE = Camera_SET_ZOOM, LAST_TYPE = LAST_XTYPE,

	OPTIMIZE_ID, // only use by optimize
	BAD_TYPE,
};

template <typename Base_Type, typename Owner_Type>
struct HOOPS_API Minimum_Header : public CMO {
public:

	Minimum_Header<Base_Type, Owner_Type>() 
		: next(null), backlink(&next), owner(null), utility(0), type(BAD_TYPE), xtype(NOTHING), dbflags(0)
	{
		;
	}

	Minimum_Header<Base_Type, Owner_Type>(Minimum_Header<Base_Type, Owner_Type> const & that)
		: next(null), backlink(&next), owner(null), utility(0), type(that.type), xtype(that.xtype), dbflags(that.dbflags)
	{
		;
	}

	Base_Type *				next;
	Base_Type **			backlink;
	Owner_Type *			owner;
	Utility					utility;
	DB_Type					type;
	DB_Type					xtype;
	Database_Flags			dbflags;
};


struct World : public Minimum_Header<World, void *> {

	INLINE local void Lock(Mutex m);
	INLINE local void Unlock(Mutex m);


	INLINE local bool Read();
	INLINE local bool TryRead();
	INLINE local void ShareRead(HThreadID thread_id);

	// returns true if lock acquired directly, was already write locked, or safely upgraded from a read lock
	INLINE local bool Write();

	// Try does not acquire (wait) if it is not safe
	INLINE local bool TryWrite();

	INLINE local void Release();

	INLINE local int CheckState();

	Time						time_stamp;

	enum Flags : uint32_t {
		STARTED						= 0x00000001,
		STARTING					= 0x00000002,
		RESETING					= 0x00000004,
		AFTER_MAIN					= 0x00000008,
		CALLED_EXIT					= 0x00000010,
		EXIT_HANDLING				= 0x00000020,
		ERROR_HANDLER_EVER_DEFINED	= 0x00000040,
		SKIP_EXTENDED_FONT_SEARCH	= 0x00000080,
		NO_AUTO_STREAM				= 0x00000100,
		WARN_ONCE_PICTURE_ALIAS		= 0x00000200,
		PRINT_VERSION_OK_ON_START	= 0x00000400,
		MINIMAL_STARTUP				= 0x00000800,
		KNOWN_WRITE_LOCK_UPGRADE	= 0x00001000,
		CHECK_WRITER				= 0x00002000,
		CODE_BEGINNING_OF_LINE		= 0x00004000,
		DELAYED_THREADS_LAUNCHED	= 0x00100000,
		NAMED_STYLES_DIRTY			= 0x01000000,
		UPDATE_EVENTS_NEEDED		= 0x10000000,
		UPDATE_NEEDED				= 0x20000000,
	};

	uint32_t					flags;


#	define		SYS_DEBUG_PRINT_VERSION						0x00000001
#	define		SYS_DEBUG_SUPPRESS_ERROR_DIALOG				0x00000002
#	define		SYS_DEBUG_DISABLE_HARDWARE_VISUAL_SEL		0x00000004
#	define		SYS_DEBUG_SZB_FORCE_GAP_FILL				0x00000008
#	define		SYS_DEBUG_DISABLE_TRISTRIP_QUADS			0x00000100
#	define		SYS_DEBUG_FALSE_COLOR_TRISTRIPS				0x00000200
#	define		SYS_DEBUG_SHOW_TRISTRIP_STATISTICS			0x00000400
#	define		SYS_DEBUG_DISABLE_NON_MANIFOLD_OPT			0x00000800
#	define		SYS_DEBUG_TRISTRIP_HEX_TRIANGLE				0x00001000
#	define		SYS_DEBUG_OLD_NURBS_SURFACE_TESSELLATION	0x00002000
#	define		SYS_DEBUG_SUBSTRIP_LARGE_FACES				0x00004000
#	define		SYS_DEBUG_DISABLE_FAST_SEGMENT_LOOKUP		0x00008000
#	define		SYS_DEBUG_QUICK_MEM_LOGGING					0x00010000
#	define		SYS_DEBUG_SUPPRESS_WATERMARK				0x00020000
#	define		SYS_DEBUG_BIG_STATS							0x00040000
#	define		SYS_DEBUG_DETAIL_SHELL_HANDEDNESS			0x00080000

	uint32_t					debug;

#	define		Sysop_CONTINUOUS_UPDATE				0x00000001
#	define		Sysop_THREADING						0x00000002
#	define		Sysop_GENERATE_CODE					0x00000004
#	define		Sysop_CONSISTENT_NORMALS			0x00000008
#	define		Sysop_CONSERVE_MEMORY				0x00000010
#	define		Sysop_SHELL_RESTRIP_ON_EDIT			0x00000020
#	define		Sysop_OLD_STYLE_PATHNAMES			0x00000040
#	define		Sysop_TRISTRIP_PER_REGION			0x00000080
#	define		Sysop_LEGACY_TEXT_EXTENT_X			0x00000100
#	define		Sysop_LEGACY_MESH_NUMBERING			0x00000200
#	define		Sysop_NEAR_LIMIT_UPDATE_PROMOTION	0x00000400
#	define		Sysop_IGNORE_ENVIRONMENT			0x00000800
#	define		Sysop_DISABLE_AMBIENT_MATERIAL		0x00001000
#	define		Sysop_SUPPRESS_DRIVER_STATS			0x00002000
#	define		Sysop_MOM_AND_APPLE_PIE				0x10000000

	uint32_t					system_flags;

#	define		Conserve_NONE					0x00000000
#	define		Conserve_SHELL_FACE_LIST		0x00000001
#	define		Conserve_SCREEN_PLANE_FACINGS	0x00000002
#	define		Conserve_LIGHTWEIGHT_CIRCLES	0x00000004
#	define		Conserve_POLYEDGES				0x00000008
#	define		Conserve_EVERYTHING				0x0000000F
	uint32_t					conserve_flags;

	Thread_Data *				main_thread;
	Thread_Data *				recent_thread;

	Definition_Set *			definition_sets;

	Task_Queue *				task_queue;

	Memory_Pool *				memory_pool_list;

	Destructible<Delayed_Thread_List>::unique_ptr	delayed_threads;
	Destructible<HThreadID_TSD_Hash>::unique_ptr	internal_threads;
	Destructible<Segment_List>::unique_ptr			commited_proxies;
	Destructible<Named_Style_Tracker>::unique_ptr	named_style_tracker;
	Destructible<Segment_List>::unique_ptr			removed_named_style_segments;
	Destructible<Segment_List>::unique_ptr			named_style_defs_segments;

	Destructible<Name_Vector>::unique_ptr			dirty_image_names;
	Destructible<Texture_Tracker>::unique_ptr		texture_tracker;

	Segment_LRU *				seg_path_lookups;

	Geometry_Reference_Lookup *	geo_ref_lookup;

	Actor *						actor_list;
	Segment *					root;

	System_Data *				system_data;


	struct {
		size_t						allocated_bytes;
		long						oversized_allocs;
		long						raw_free;
		long						memory_acquired;
		long						memory_allocate;

		long						seg_bounding_count;
		long						geo_bounding_count;

		long						activate_texture;
		long						failed_texture;

		long						proxy_segments;
		long						populated_segments;
		long						depopulated_segments;
	}	stats;

	size_t						soft_memory_limit_bytes;

	size_t						populate_memory_limit;
	size_t						original_populate_memory_limit;
	size_t						populate_memory_used;

	size_t						original_memory_workspace_size;
	size_t						memory_workspace_size;

	char const *				ff_routine;
	char const *				lff_routine;
	intptr_t					ffdebug;
	intptr_t					lffdebug;
	char						ff_debug_note[32];

	Key_Hash					local_keys;

	struct {
		Key_Entry *		table;
		Key_Entry *		empty;		// chain emptied elements below the top
		int				count;
		int				allocated;
	}	lite_keys;

	struct {
		Option_Type **	lookup;
		int				count;
		int				allocated;
		int				max_index;
	}	conditions;

#ifndef HPS_CORE_BUILD
	Error_Handler *				error_handler_list;
	Exit_Handler *				exit_handler_list;
	Error_Handler *				error_handler_save;
#endif

	unsigned char *				dither_patterns8;
	unsigned short *			dither_patterns16;
	float const *				sinlookup72;


	Wakeup *					wakeup_list;
	void *						system_time_data;

	Pending_Special *			pending_special_events;

	Destructible<Activity_Vector>::unique_ptr	pending_activity;
	
	Driver_Config *				driver_config_data;

	Destructible<Display_List_Vector>::unique_ptr	display_list_heap;  //sorted by usefulness

	Driver_DL *					master_display_lists;  //sorted by time stamp

	Image *						texture_fallback_image;

	Optimized_Mapping_Table *	optimized_mapping_tables;

	Destructible<Segment_Highlight_Map>::unique_ptr		all_segment_highlights;

	Destructible<Segment_Vector>::unique_ptr			modified_highlight_segments;

	Destructible<Segment_Highlight_Map>::unique_ptr		overlay_segment_highlights; // doesn't own the contents
	Destructible<Segment_Highlight_Map>::unique_ptr		spriting_segment_highlights; // doesn't own the contents
	Destructible<Segment_Highlight_Map>::unique_ptr		inplace_segment_highlights; // doesn't own the contents

	Destructible<Quickmoved_Segment_List>::unique_ptr	quickmoved_direct;

	Destructible<Geometry_Vector>::unique_ptr			fixup_geometries;

	// global_font_data for font processors
	void *						global_font_data_freetype;
	void *						font_stencil_cache;
	void *						icu_handle;
	icu_unicode_entrypoint *	icu_unicode_layout;
	void *						kanji_data;
	Name						system_default_font_directory;


	Name_List *					font_directory;
	Name_List *					metafile_directory;
	void *						code_file;
	int							expire_hours;

	Driver_Action				default_3d_driver;
	uint32_t					default_hps_3ddriver_type;

	int							definition_set_count;

	int							hoops_3df_version;

	int32_t						next_priority;

	int							next_user_option_id;

	int							timer_enables;

	Option_Hash_Table			option_hash_tables[MAX_OPTION_HASH_TABLES];


	Callback_Name				event_checker_name;
	Event_Checker_Action		event_checker;

	Callback_Name				deleted_key_callback_name;
	Delete_Database_Key_Action	deleted_key_callback;

	Callback_Name				populate_key_callback_name;
	Database_Key_Action			populate_key_callback;

	Callback_Name				depopulated_key_callback_name;
	Database_Key_Action			depopulated_key_callback;

	Callback_Name				stale_proxy_key_callback_name;
	Database_Key_Action			stale_proxy_key_callback;

	System_Cleanup_Hook_Action	system_cleanup_hook;
	void *	_glBindBufferARB;
	void *	_glBufferDataARB;
	void *	_glDeleteBuffersARB;

	Load_Dynamic_Driver_Action	dynamic_driver_loader;


	uint32_t					highlight_generation;

	uint32_t					extent_generation;

	uint32_t					update_generation;

	uint32_t					global_id; // for shader def's, etc...


	struct Invalidate_Segdl {
		Segment_Int_Hash *		seen_bits;
		uint32_t				generation;
		Spinlock				spinlock;
	} invalidate_segdl;

	struct Clean_Segdl {
		Destructible<Segment_Set>::unique_ptr	seen_segments;
		uint32_t				generation;
	} clean_segdl;

	/* system options... */
	Error_Mask					error_mask;
	Error_Control *				error_control[4];	// info, warning, error, fatal
	int							C_return_string_length;
	Name						application;
	Name						code_generation_directory;
	Name						driver_config_file;
	Text_Encoding				encoding;

	int							message_limit;
	int							messages_given;

	int32_t						random;
	int32_t						rand48_seed;

	/* move the rest, if any, up when convenient... */


	int							string_history_limit;

	float						original_dpi_scale_factor;
	float						dpi_scale_factor;

	int							code_file_number;
	int							code_lines;
	int							code_line_limit;
	int							code_indent_level;

	Mutex						memory_pool_mutex;
	Mutex						atomic_mutex;
	Mutex						proxy_mutex;
	Mutex						timer_mutex;
	Mutex						bounding_mutex;

	Mutex						intrinsic_mutex; // used if intrinsic locks are missing
	Mutex						thread_mutex;
	Mutex						low_thread_mutex;
	Mutex						segment_lookup_mutex;
	Mutex						hash_mutex;
	Mutex						code_generation_mutex;
	Mutex						driver_mutex;		// only for below standard driver
	Mutex						actor_list_mutex;	// only for above standard driver
	Mutex						font_instance_mutex;
	Mutex						display_list_mutex;
	Mutex						splay_mutex;
	Mutex						wakeup_mutex;

	RWLock						database_lock;

#define Processor_NONE			0x00000000
#define Processor_SSE			0x00000001
#define Processor_SSE2			0x00000002
#define Processor_SSE3			0x00000004
#define Processor_SSSE3			0x00000008
#define Processor_SSE41			0x00000010
#define Processor_SSE42			0x00000020
#define Processor_SSE4a			0x00000040
#define Processor_AVX			0x00000080
#define Processor_FMA3			0x00000100
#define Processor_FMA4			0x00000200
#define Processor_XOP			0x00000400

	int							processor_capabilities;
	int							original_processor_capabilities;
	int							processor_override;

#define MAX_VRAM ((size_t)(~0UL))
	size_t						display_list_vram_usage;
	size_t						texture_vram_usage;
	size_t						vram_usage_limit;


	unsigned int				transparent_image_count;

	size_t						handedness_errors_generated;

// "WLIMIT" defines how close to the eye plane something can come without
// being clipped. (Camera target is at W=1.0, camera position is at W=0.0,
// WLIMIT is in between.) The closer to zero it is, the more accurate the
// X and Y but the more wild the Z/W.
#define DEFAULT_WLIMIT			-0.001f
	float						camera_near_limit;

};



class Name_Const {
friend class Name;
public:

	Name_Const() : _text(0), _length(0), _key(0), _lower_sum(0) {};

	INLINE Name_Const (char const * s, size_t l) : _text(s), _length(l), _key(0), _lower_sum(0) {};

	INLINE Name_Const (char const * s) {
		_text = s;
		if (_text != null)
			_length = strlen(_text);
		else
			_length = 0;
		_key = 0;
		_lower_sum = 0;
	}

	Name_Const(Null_Object const &) : _text(0), _length(0), _key(0), _lower_sum(0) {};

	Name_Const(Name_Const const & that) : _text(that._text), _length(that._length), _key(that._key), _lower_sum(that._lower_sum) {};

	~Name_Const()
	{

	}

	Name_Const const &	operator= (Name_Const const & that) {
		_text = that._text;
		_length = that._length;
		_key = that._key;
		_lower_sum = that._lower_sum;
		return *this;
	}


	bool operator == (Name_Const const & that) const;
	bool operator != (Name_Const const & that) const {return !(that == *this);}

	INLINE operator Name const & () const {return *((Name*)this);}

	bool operator < (Name_Const const & that) const {
		// compares only the text 

		char const *	a = _text;
		char const *	b = that._text;
		while (*a != '\0') {
			const int achar = UNSIGNED_CHARTOINT(HM_TOLOWER(*a));
			const int bchar = UNSIGNED_CHARTOINT(HM_TOLOWER(*b));
			if (achar < bchar) {
				return true;
			}
			else if(achar > bchar){
				return false;
			}
			else {
				++a;
				++b;
				if(*b == '\0') return false;
			}
		}
		return true;
	};

	INLINE bool CI_Equal(Name const & that) const {return that.CI_Equal(*this);}
	INLINE bool CI_Equal(Name_Const const & that) const;

	INLINE void to_chars(char * out, size_t out_n) const {
		size_t n = Min(out_n-1, (size_t)length());
		memcpy(out, text(), n);
		out[n] = '\0';
	}

	INLINE bool empty() const { return (_length == 0); }
	INLINE size_t length() const {return _length;}
	INLINE const char * text() const {return _text;}

	INLINE int32_t hash_key() const;
	INLINE int32_t lower_sum() const;

private:
	char const * 	_text;
	size_t			_length;
	mutable	int32_t	_key;
	mutable	int32_t	_lower_sum;
};

// Name_Const builder for string literals
template <size_t N>
Name_Const NC(char const (&s)[N]) {
	return Name_Const(s, N-1);
}

/* "Karacters" are extended 16-bit (or more) characters. If the upper
 * byte(s) are all zeros, the character code is Ascii, in the low
 * bytes. If the upper isn't all zeros, then it's something else,
 * usually UTF-16 Unicode, but maybe Kanji or extended ISO fonts.
 */

typedef unsigned short	Karacter;

struct KName {
	Karacter *	ktext;
	size_t		length;
	int32_t		dummy1; // _key;
	int32_t		dummy2; // _lower_sum;
};


INLINE void INITKNAME(KName & name)
{
	name.length = 0;
	name.ktext = 0;
}

INLINE void KNAMEFREE(KName & name)
{
	if (name.length != 0) {
		FREE_ARRAY(name.ktext, name.length+1, Karacter);
		INITKNAME(name);
	}
}



#ifdef HPS_CORE_BUILD
void World::Lock(Mutex m) {
	m->Lock();
}

void World::Unlock(Mutex m) {
	m->Unlock();
}

bool World::Read() {
	return	HOOPS::WORLD->database_lock->Read();
}

bool World::TryRead() {
	return	HOOPS::WORLD->database_lock->TryRead();
}

void World::ShareRead(HThreadID thread_id) {
	return	HOOPS::WORLD->database_lock->ShareRead(thread_id);
}

bool World::Write() {
	return HOOPS::WORLD->database_lock->Write();
}

bool World::TryWrite() {
	return HOOPS::WORLD->database_lock->TryWrite();
}

void World::Release() {
	HOOPS::WORLD->database_lock->Release();
}

#else

void World::Lock(Mutex m) {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		m->Lock();
}

void World::Unlock(Mutex m) {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		m->Unlock();
}

bool World::Read() {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		return HOOPS::WORLD->database_lock->Read();
	return true;
}

bool World::TryRead() {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		return HOOPS::WORLD->database_lock->TryRead();
	return true;
}

void World::ShareRead(HThreadID thread_id) {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		HOOPS::WORLD->database_lock->ShareRead(thread_id);
}

// returns true if lock acquired directly, was already write locked, or safely upgraded from a read lock
bool World::Write() {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		return HOOPS::WORLD->database_lock->Write();
	return true;
}

// Try does not acquire (wait) if it is not safe
bool World::TryWrite() {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		return HOOPS::WORLD->database_lock->TryWrite();
	return true;
}

void World::Release() {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		HOOPS::WORLD->database_lock->Release();
}

#endif

// common as threading must be ignored during startup
int World::CheckState() {
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING))
		return HOOPS::WORLD->database_lock->CheckState();

	return (HM_LOCK_STATE_READ|HM_LOCK_STATE_WRITE);
}


class HOOPS_UTILITY_PRIVATE_API Mutexer {
private:
	Mutex const &	mutex;

public:
	INLINE Mutexer (Mutex const & m) : mutex(m) {
		World::Lock(mutex);
	}

	INLINE ~Mutexer () {
		World::Unlock(mutex);
	}
private:
	// don't really want or need these, except to shut up the compiler
	Mutexer (Mutexer const &);
	Mutexer const & operator= (Mutexer const &);
};


#ifdef HOOPS_DEBUG_BUILD
#	define H_ASSERT_READ_DATABASE(_td_) \
	do { \
		UNREFERENCED(_td_); \
		if (!ANYBIT(HOOPS::World::CheckState(), HM_LOCK_STATE_READ|HM_LOCK_STATE_WRITE)) \
		ASSERT(0); \
	} while (0)


#	define H_ASSERT_WRITE_DATABASE(_td_) \
	do { \
		if (ANYBIT ((_td_)->flags, Thread_Data::PROXIE_WRITER)) { \
			if (!ANYBIT(HOOPS::World::CheckState(), HM_LOCK_STATE_READ|HM_LOCK_STATE_WRITE)) \
				ASSERT(0); \
		} \
		else if (ANYBIT(HOOPS::WORLD->flags, World::CHECK_WRITER) && !ANYBIT((_td_)->flags, Thread_Data::ALLOW_WRITE)) \
			ASSERT(0); \
		else if (!ANYBIT(HOOPS::World::CheckState(), HM_LOCK_STATE_WRITE)) \
			ASSERT(0); \
	} while (0)

#else
#	define H_ASSERT_READ_DATABASE(_td_) DO_NOTHING(_td_)
#	define H_ASSERT_WRITE_DATABASE(_td_) DO_NOTHING(_td_)
#endif


struct Thread_Data {

	enum Thread_Flags : uint32_t {
		NO_UPDATE_CONTROL			= 0x00000001,
		PROXIE_WRITER				= 0x00000002,
		SUPPRESS_STATIC_ACTIVITY	= 0x00000004,
		ALLOW_UNSAFE_THREADING		= 0x00000008,
		ALLOW_WRITE					= 0x00000010,
		CONSERVE_SHELL_FACE_LIST	= 0x00000020,
	};

	Time						update_start_time;
	Time						update_end_time;
	Time						update_limit_time;

	Thread_Data *				next;
	Memory_Pool *				memory_pool;

	HThreadID					thread_id;
	Semaphore					semaphore;

	/* context-sensitive stuff */
	Open_Item *					open_list;

	char const *				current_routine;
	char const *				current_routine_stack[20]; /* (20 is large!) */
	char const **				current_routine_sp;

	Search_Header *				segment_search_stack;
	Search_Header *				contents_search_stack;
	Search_Header *				alias_search_stack;
	Search_Header *				callback_search_stack;
	Search_Header *				color_search_stack;
	Search_Header *				open_search_stack;
	Search_Header *				font_search_stack;
	Search_Header *				texture_search_stack;
	Search_Header *				shader_search_stack;
	Search_Header *				glyph_search_stack;
	Search_Header *				named_style_search_stack;
	Search_Header *				line_style_search_stack;
	Search_Header *				error_handler_search_stack;
	Search_Header *				exit_handler_search_stack;

	Event *						event_queue;
	Event *						newest_event;
	Event *						current_event;
	Selection_Event_CD *		cd_event;
	Selection_Event *			selection_buffer;

	void *						shell_selection_cache[2]; // float, double

	void *						decimate_shell_caches;

	void *						intersect_polyline_cache;
	void *						point_inside_shell_cache;

	Definition_Set *			def_set;
	int							definition_set_id;

	int							error_severity;
	int							error_category;
	int							error_specific;

	uint32_t					flags;

	/* thread-specific update info */
#define UP_SOMEWHERE_ELSE				0		/* assumed starting state */
#define UP_IN_PROGRESS					1
#define UP_IN_PROGRESS_AND_PENDING		2
#define UP_INTERRUPTED					3

	int							update_status;
	int							update_end_time_check;
	volatile bool				timer_expired;

	Name_Const					name_const_tmp;
	char_Vector_Pooled			sprintf_buffer;
	Segment_Vector_Pooled		segment_buffer;

	static Thread_Data * Create(HThreadID thread_id);
	static void Destroy(Thread_Data * thread_data);

	static Thread_Data * Find(HThreadID	current_thread, bool internal_lookup_allowed);

	static void Forget(HThreadID current_thread);
	static void Clean(Thread_Data * thread_data);

	INLINE bool Find_Open()
	{
#ifdef HPS_CORE_BUILD
		H_ASSERT_HPS(0);
		return false;
#else
		return HI_Really_Find_Our_Open(this);
#endif
	}

};


/* global fixed data (such as it is) */


#ifdef DISABLE_CODE_GENERATION

#	ifdef HPS_CORE_BUILD
#	define CODE_GENERATION(x_code_x) error // should not be in hps
#	else
#	define CODE_GENERATION(x_code_x) do {} while (0)
#	endif

#else
#	define CODE_GENERATION(x_code_x)										\
	do if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_GENERATE_CODE)) {		\
		Thread_Data *_thread_data_;											\
		FIND_USER_THREAD_DATA (_thread_data_);								\
		if (_thread_data_->current_routine_sp == _thread_data_->current_routine_stack) {	\
			Mutexer mutexer(HOOPS::WORLD->code_generation_mutex);			\
			{ x_code_x }													\
			if (HOOPS::WORLD->code_lines > HOOPS::WORLD->code_line_limit)	\
				HI_Chain_Code_Files ();										\
		}																	\
	} while (0)
#endif

#define UNIRAND()		(HOOPS::READ_ONLY->uniform_rand[++HOOPS::WORLD->rand48_seed & 0x3FF])
#define RANDOM()		(HOOPS::WORLD->random *= (int)69069, ++HOOPS::WORLD->random)
#define URANDOM()		((unsigned int)RANDOM())

#define POINTER_SIZED_HIGHBIT	(intptr_t)((uintptr_t)1 << (int)(8 * sizeof (intptr_t) - 1))
#define POINTER_SIZED_NEXTBIT	(intptr_t)((uintptr_t)1 << (int)(8 * sizeof (intptr_t) - 2))

INLINE intptr_t V2I (void *x) {
	return (intptr_t)x;
}
INLINE void *I2V (intptr_t x) {
	return (void *)x;
}


/*** assumes a pointer can map into a negative long ***/
#define ERROR_KEY				(-1L)

template <typename T>
INLINE Key POINTER_TO_KEY(T * p) {
	return (Key)((((uintptr_t)(p))>>2) | POINTER_SIZED_HIGHBIT);
}


INLINE Anything * Key_To_Pointer (
	Thread_Data *		thread_data,
	Key					key,
	int *				offset,
	Segment const *		seg = null)
{
	H_ASSERT_READ_DATABASE(thread_data);

	if (offset != null)
		*offset = 0;

	if (key > ERROR_KEY)
		key = HI_Key_To_Internal (thread_data, key, seg);
	if (key == ERROR_KEY)
		return null;
	if (ANYBIT (key, POINTER_SIZED_NEXTBIT)) {
		intptr_t		index = ~key;

		if (index >= HOOPS::WORLD->lite_keys.count || HOOPS::WORLD->lite_keys.table[index].offset < 0) //-V807
			return null;
		if (offset != null)
			*offset = HOOPS::WORLD->lite_keys.table[index].offset;
		return HOOPS::WORLD->lite_keys.table[index].item;
	}

	key <<= 2;

	if (((intptr_t)key & ((1<<HOOPS_MEMORY_ALIGNMENT) - 1)) != 0) {
		ASSERT(0); // bad pointer
		return null;
	}

	return (Anything *)key;
}

INLINE Anything * Key_To_Pointer (
	Thread_Data *		thread_data,
	Key					key,
	Segment const *		seg)
{
	return Key_To_Pointer (thread_data, key, null, seg);
}

INLINE Anything * Key_To_Pointer (
	Thread_Data *		thread_data,
	Key					key)
{
	return Key_To_Pointer (thread_data, key, null, null);
}





INLINE void FIND_USER_THREAD_DATA(Thread_Data *& thread_data) {
	HThreadID current_thread;
	thread_data = HOOPS::WORLD->recent_thread;
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING) &&
		(thread_data->thread_id != (current_thread = THREAD_ID())))
		thread_data = Thread_Data::Find(current_thread, false);
}

INLINE void FIND_INTERNAL_THREAD_DATA(Thread_Data *& thread_data) {
	HThreadID current_thread;
	thread_data = HOOPS::WORLD->recent_thread;
	if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_THREADING) &&
		(thread_data->thread_id != (current_thread = THREAD_ID())))
		thread_data = Thread_Data::Find(current_thread, true);
}


INLINE void TD_SETNAME(Thread_Data * thread_data, const char * routine) {
	if (routine == null)
		thread_data->current_routine = "(none)";
	else {
		thread_data->current_routine = routine;
		if (thread_data->current_routine_sp == thread_data->current_routine_stack) {
			thread_data->error_severity = -1;
			thread_data->error_category = -1;
			thread_data->error_specific = -1;
		}
	}
}

INLINE void PUSHNAME(Thread_Data * thread_data) {
	*thread_data->current_routine_sp++ = thread_data->current_routine;
}

INLINE void POPNAME(Thread_Data * thread_data) {
	thread_data->current_routine = *(thread_data->current_routine_sp-1);
	*--thread_data->current_routine_sp = 0;
	ASSERT(thread_data->current_routine_sp >= thread_data->current_routine_stack); // POP less please
}


//#define CONTEXT_LOGGING

class Context {
#ifdef CONTEXT_LOGGING
private:
	struct Context_Update_Log {
		HThreadID				thread_id;
		const char *			name;
		Time					time;
	};

	static const int N_LOG_MESSAGES=1024;
	static Context_Update_Log log[N_LOG_MESSAGES];
	static uint32_t	log_index;
#endif

private:
	Thread_Data *	thread_data;
#ifdef HOOPS_DEBUG_BUILD
	int lock_status;
	int final_status;
#endif

public:
	Context (const char * name) {
		thread_data = HI_Set_Name(name);
		ASSERT(thread_data != null);
		record_lock_status();
		log_message(thread_data, name);
	}

	Context (Thread_Data *	startup_thread_data) {
		thread_data = startup_thread_data;
		ASSERT(thread_data != null);
		record_lock_status();
	}

	~Context () {
#ifdef HOOPS_DEBUG_BUILD
		final_status = World::CheckState();
		ASSERT((lock_status & ~(HM_LOCK_STATE_OTHER_SHARED|HM_LOCK_STATE_OTHER_READ|HM_LOCK_STATE_OTHER_WRITE)) ==
			   (final_status & ~(HM_LOCK_STATE_OTHER_SHARED|HM_LOCK_STATE_OTHER_READ|HM_LOCK_STATE_OTHER_WRITE)));
#endif
		thread_data->flags &= ~Thread_Data::PROXIE_WRITER;

		if (ANYBIT (HOOPS::WORLD->system_flags, Sysop_CONTINUOUS_UPDATE) &&
			ANYBIT (HOOPS::WORLD->flags, World::UPDATE_NEEDED)) {
			PUSHNAME(thread_data);
			TD_SETNAME (HOOPS::WORLD->main_thread, "Continuous Update (internal)");
			HI_Do_Update (thread_data, -1.0, false);
			POPNAME(thread_data);
		}

		TD_SETNAME (thread_data, null);
	}

	INLINE void record_lock_status() {
#ifdef HOOPS_DEBUG_BUILD
		final_status = 0;
		lock_status = World::CheckState();
#endif
	}

	static INLINE void log_message(Thread_Data * thread_data, const char * message) {
#ifdef CONTEXT_LOGGING
		int index = locked_increment(&log_index);
		Context_Update_Log & the_log = log[index%N_LOG_MESSAGES];
		the_log.thread_id = thread_data->thread_id;
		the_log.name = message;
		the_log.time = HOOPS::WORLD->time_stamp;
#else
		UNREFERENCED(thread_data);
		UNREFERENCED(message);
#endif
	}

	INLINE operator Thread_Data * () {return thread_data;}

	// so we can access data elements directly as in C rather than wrapping everything in set/show functions
	INLINE Thread_Data const *	operator-> () const		{ return thread_data; }
	INLINE Thread_Data *	operator-> ()		{ return thread_data; }

private:
	Context ();
	Context (Context const &);
	void operator= (Context const &);
};



// Sprintf single arguments
#define Sprintf_D(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,a,0,null,null)			//	%d, %x, %o	(all "short" int)
#define Sprintf_X(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)&a,null)		//	%X (pointer)
#define Sprintf_K(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)&a,null)		//	%k, %K (@key or keylo,keyhi)
#define Sprintf_F(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)&a,null)		//	%f (float), %F (double)
#define Sprintf_S(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,null)		//	%s, %S (quotes added)
#define Sprintf_N(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,null)		//	%n, %N (quoted if special)
#define Sprintf_P(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,null)		//	%p (segment path)
// special quoted formats (note: %qn has different idea of special than %N)
#define Sprintf_QS(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,null)		//	%qs (string, quoted if special)
#define Sprintf_QN(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,null)		//	%qn (name, quoted if special)
// single argument decimal values, of long or pointer size
#define Sprintf_LD(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)&a,null)		//	%D (long)
#define Sprintf_LP(buf,spec,a)	 	HI_Sprintf4(null,buf,spec,0,0,(void*)&a,null)		//	%P (pointer or equivalent)
// 2 arguments, common combinations of simple forms
#define Sprintf_DD(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,a,b,null,null)
#define Sprintf_DF(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,a,0,(void*)&b,null)
#define Sprintf_FF(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)&a,(void*)&b)
#define Sprintf_SS(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_SD(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,b,0,(void*)a,null)
#define Sprintf_SF(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)&b)
#define Sprintf_SK(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)&b)
#define Sprintf_SN(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_SP(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_NN(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_NP(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_ND(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,b,0,(void*)a,null)
#define Sprintf_NS(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_PP(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_PS(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_PN(buf,spec,a,b) 	HI_Sprintf4(null,buf,spec,0,0,(void*)a,(void*)b)
#define Sprintf_DX(buf,spec,a,b)	HI_Sprintf4(null,buf,spec,a,0,(void*)&b,null)

// same as above, with explicit thread data
#define TD_Sprintf_D(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,a,0,null,null)
#define TD_Sprintf_X(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)&a,null)
#define TD_Sprintf_K(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)&a,null)
#define TD_Sprintf_F(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)&a,null)
#define TD_Sprintf_S(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,null)
#define TD_Sprintf_N(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,null)
#define TD_Sprintf_P(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,null)
#define TD_Sprintf_QS(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,null)
#define TD_Sprintf_QN(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,null)
#define TD_Sprintf_LD(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)&a,null)
#define TD_Sprintf_LP(_td_,buf,spec,a)		HI_Sprintf4(_td_,buf,spec,0,0,(void*)&a,null)
#define TD_Sprintf_DD(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,a,b,null,null)
#define TD_Sprintf_DF(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,a,0,(void*)&b,null)
#define TD_Sprintf_FF(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)&a,(void*)&b)
#define TD_Sprintf_SS(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_SD(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,b,0,(void*)a,null)
#define TD_Sprintf_SF(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)&b)
#define TD_Sprintf_SN(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_SP(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_SX(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)&b)
#define TD_Sprintf_NN(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_NP(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_ND(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,b,0,(void*)a,null)
#define TD_Sprintf_NS(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_PP(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_PS(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_PN(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,0,0,(void*)a,(void*)b)
#define TD_Sprintf_DX(_td_,buf,spec,a,b)	HI_Sprintf4(_td_,buf,spec,a,0,(void*)&b,null)



#define OPTIONS_HASH(index)		(&HOOPS::WORLD->option_hash_tables[index])

/* the following would normally be in database.h, but put here
 * for the drivers.
 */
/* MUST MATCH TABLE IN Show_Text_Font! */
typedef int		Generic_Font_Name;
typedef char	Font_Transform,
				Font_Size_Units;

#define GFN_NONE				(Generic_Font_Name)0 /* for use in drivers */
#define GFN_SPECIFIED			(Generic_Font_Name)1
#define GFN_SYSTEM_DEFAULT		(Generic_Font_Name)2
#define GFN_SANS_SERIF			(Generic_Font_Name)3
#define GFN_TYPEWRITER			(Generic_Font_Name)4
#define GFN_ROMAN				(Generic_Font_Name)5
#define GFN_STROKED				(Generic_Font_Name)6
#define GFN_SANS_SERIF_BOLD		(Generic_Font_Name)7
#define GFN_TYPEWRITER_BOLD		(Generic_Font_Name)8
#define GFN_ROMAN_BOLD			(Generic_Font_Name)9
#define GFN_LAST_GFN			GFN_ROMAN_BOLD


#define Encoding_INVALID		(Text_Encoding)0

#define Encoding_ISO_Latin_One	(Text_Encoding)1
#define Encoding_ISO_Latin		(Text_Encoding)2
#define Encoding_JEC			(Text_Encoding)3		/* Shift-JIS */
#define Encoding_EUC			(Text_Encoding)4
#define Encoding_Raw_16			(Text_Encoding)5
#define Encoding_Unicode		(Text_Encoding)6
#define Encoding_UTF8			(Text_Encoding)7
#define Encoding_UTF16			(Text_Encoding)8
#define Encoding_UTF32			(Text_Encoding)9
#define Encoding_WCS			(Text_Encoding)10
#define Encoding_MBS			(Text_Encoding)11
#define Encoding_ISO_10646		(Text_Encoding)12

#define MAX_ENCODING			(Text_Encoding)12

#define Encoding_Mapping_NONE	(int)0
#define Encoding_Mapping_DIRECT (int)1
#define Encoding_Mapping_MAPPED (int)2

template <typename F>
struct Intersection_3D {
	struct {
		Point_3D<F> const *	start;
		Point_3D<F> const *	end;
		int					start_index;
		int					end_index;
		float				fraction;
	}	lines[2];
};
typedef Intersection_3D<float>		Intersection;
typedef Intersection_3D<double>		DIntersection;


#ifndef FAMOTIK
#	define	Encoding_DEFAULT	Encoding_ISO_Latin_One
#else
#	define	Encoding_DEFAULT	Encoding_JEC
#endif


class Name_List : public CMO {
public:

	Name_List *	next;
	Name		name;

	Name_List() :  next(null), name() {};

	Name_List(Name_List const & that) {

		if (that.next != null)
			next = NEW(Name_List)(*that.next);

		name = that.name;
	};

	~Name_List() {
		delete next;
	}

	Name_List & operator= (Name_List const & that) {
		if (that.next != null)
			next = NEW(Name_List)(*that.next);

		name = that.name;
		return *this;
	};
};

class Counted_Name_List : public CMO {

public:

	Counted_Name_List *	next;
	Name				name;
	int					count;

	Counted_Name_List() :  next(null), name(), count(0) {};

	Counted_Name_List(Counted_Name_List const & that) {

		if (that.next != null)
			next = NEW(Counted_Name_List)(*that.next);

		name = that.name;
	};

	~Counted_Name_List() {
		delete next;
	}

	Counted_Name_List & operator= (Counted_Name_List const & that) {
		if (that.next != null)
			next = NEW(Counted_Name_List)(*that.next);

		name = that.name;
		return *this;
	};
};


class Rendition_Base : public Counted_Object_Pooled {
public:
	Display_Context const *	display_context;
	int						incarnation;

protected:
	HOOPS_API Rendition_Base (Display_Context const *);
	HOOPS_API Rendition_Base (Rendition_Base const &);


private:
	Rendition_Base();		// no default
	Rendition_Base const & operator= (Rendition_Base const & that);
};


// Note: osx compiler can't seem to resolve anything that Rendition_Pointer inherits from Counted_Pointer
// without explicitly qualifying everything
template <typename T>
class Rendition_Pointer : public Counted_Pointer<T> {
	typedef		Counted_Pointer<T>		CP;		// shorter name so code is readable

private:
	static Rendition_Pointer Create ();

public:
	Rendition_Pointer () : CP () {};	// needed because the next one is needed...
	// these don't inherit as needed due to the effective type of the result
	Rendition_Pointer (Null_Object const &) : CP () {}
	Rendition_Pointer const &	operator= (Null_Object const &) {
		CP::release();
		CP::counted_item = 0;
		return *this;
	}

	INLINE Rendition_Pointer Copy () const {
		Rendition_Pointer		temp;
		temp.counted_item = POOL_NEW(CP::counted_item->memory_pool, T)(*CP::counted_item);
		temp->retain();
		return temp;
	}

	static Rendition_Pointer	Create (Display_Context const * dc);

	INLINE Rendition_Pointer &			Modify (int offset = 0);
	INLINE Rendition_Pointer &			Renew ();
};





INLINE void set_float_ones(size_t count, float * ws);

typedef	unsigned char	Matrix_Data_Flags;
#	define Matrix_Data_Flags_NONE			(Matrix_Data_Flags)0x00
#	define Matrix_Data_Flags_NEG3X3			(Matrix_Data_Flags)0x01
#	define Matrix_Data_Flags_NEG3X3_VALID	(Matrix_Data_Flags)0x02

//#	define Matrix_Data_Flags		(Matrix_Data_Flags_NEG3X3|Matrix_Data_Flags_NEG3X3_VALID)

typedef	unsigned short	Matrix_Overrides;


class Matrix_Base;
template <typename F> class Matrix_Data;


class Matrix : public Counted_Pointer<Matrix_Base> {
	typedef		Counted_Pointer<Matrix_Base>		CP;
	template <typename F> friend class Matrix_Data;
	public:
		Matrix () : CP () {};	// needed because the next one is needed...
		// these don't inherit as needed due to the effective type of the result
		Matrix (Null_Object const &) : CP () {}
		INLINE Matrix const &	operator= (Null_Object const &) {
			CP::release();
			CP::counted_item = 0;
			return *this;
		}

		INLINE Matrix Copy () const;
		INLINE Matrix & Modify ();


		INLINE Matrix operator * (Matrix const & that) const;

		INLINE bool Equals (Matrix const & that, int tol=32) const;

		INLINE bool Equals3x3 (Matrix const & that, int tol=32) const;

	private:
		Matrix_Base * const &	counted () const	{ return CP::counted_item; }
		Matrix_Base * &			counted ()			{ return CP::counted_item; }
		static Matrix Create();	// only create a specific derived form
};


class Matrix_Base : public Counted_Object_Interlocked {
	friend class Matrix;

	protected:
		mutable	Matrix_Contents		contents;
		mutable	Matrix_Data_Flags	data_flags;

	protected:
		Matrix_Base() : contents (0), data_flags (0) {}

		virtual void		validate_contents(Matrix_Contents test_contents = Matrix_Contents_ANYTHING) = 0;
		virtual void		validate_determinant3x3_negative() const = 0;
		virtual void		ASSERT_IDENTITY() const = 0;

	public:
		virtual Matrix_Base *	Clone () const = 0;
		virtual int				Precision() const = 0;

		INLINE bool Contains(int desired_contents, int rejected_contents = Matrix_Contents_NONE) const {
			return (ANYBIT(contents, desired_contents) &&
					!ANYBIT(contents, rejected_contents));
		}

		bool Is_Identity() const {
#ifdef HOOPS_DEBUG_BUILD
			ASSERT_IDENTITY();
#endif
			return !ANYBIT (contents, Matrix_Contents_ANYTHING);
		}
		
		bool Is_Perspective() const {
			return ANYBIT(contents, Matrix_Contents_PERSPECTIVE);
		}

		bool Is_Determinant3x3_Negative() const {
			if (!ANYBIT (data_flags, Matrix_Data_Flags_NEG3X3_VALID))
				validate_determinant3x3_negative();
			return ANYBIT (data_flags, Matrix_Data_Flags_NEG3X3);
		}

		virtual int			hash_key() const = 0;
		virtual bool 		Is_3D() const = 0;
		virtual bool 		Is_SINGULAR() const = 0;
		virtual bool 		Is_Symmetric() const = 0;
		

		virtual Matrix_4x4<float> const &	ElementsFloat() const = 0;
		virtual Matrix_4x4<double> const &	ElementsDouble() const = 0;

		virtual double		Determinant3x3() const = 0;
		virtual double		Determinant() const = 0;

		virtual	double		Symmetric_Scale() const = 0;
		virtual	double		Max_Scale() const = 0;


		virtual Matrix const & adjoint() const = 0;
		virtual void Normalize3x3() = 0;
		//virtual void OverrideTranslate(Vector_3D<F> const & translate) = 0;

		virtual void Scale_And_Translate (Vector const & scale, Vector const &	translate) = 0;



		virtual Point Transform(Point const & point, float const & point_w, float & w) const = 0;
		virtual Point Transform(Point const & point, float & w) const = 0;
		virtual Point Transform(Point const & point) const = 0;
		virtual void Transform(size_t count, Point const * in_points, Point * out_points,
							   float wlimit = -1.0f, float * ws = null) const = 0;

		virtual DPoint Transform(DPoint const & point, float const & point_w, float & w) const = 0;
		virtual DPoint Transform(DPoint const & point, float & w) const = 0;
		virtual DPoint Transform(DPoint const & point) const = 0;
		virtual void Transform(size_t count, DPoint const * in_points, DPoint * out_points,
							   float wlimit = -1.0f, float * ws = null) const = 0;

		virtual void Transform(size_t count, DPoint const * in_points, Point * out_points,
							   float wlimit = -1.0f, float * ws = null) const = 0;

		virtual float Transform_X(Point const & point) const = 0;
		virtual float Transform_Y(Point const & point) const = 0;
		virtual float Transform_Z(Point const & point) const = 0;
		virtual float Transform_W(Point const & point) const = 0;
		virtual double Transform_X(DPoint const & point) const = 0;
		virtual double Transform_Y(DPoint const & point) const = 0;
		virtual double Transform_Z(DPoint const & point) const = 0;
		virtual double Transform_W(DPoint const & point) const = 0;

		virtual Vector Transform(Vector const & vector, bool normalize = true) const = 0;
		virtual DVector Transform(DVector const & vector, bool normalize = true) const = 0;
		virtual void Transform(size_t count, Vector const * in_vector, Vector * out_vector, bool normalize=true) const = 0;
		virtual void Transform(size_t count, DVector const * in_vector, DVector * out_vector, bool normalize=true) const = 0;

		virtual Plane Transform(Plane const & plane) const = 0;
		virtual DPlane Transform(DPlane const & plane) const = 0;
		virtual void Transform(size_t count, Plane const * in_plane, Plane * out_plane) const = 0;
		virtual void Transform(size_t count, DPlane const * in_plane, DPlane * out_plane) const = 0;

		virtual float Transform_C(Plane const & plane) const = 0;
		virtual double Transform_C(DPlane const & plane) const = 0;


		virtual Vector TransformToVector(Plane const & plane) const = 0;

		virtual Plane TransformToPlane(Vector const & normal, Point const & point) const = 0;
		virtual DPlane TransformToPlane(Vector const & normal, DPoint const & point) const = 0;

		virtual void TransformToPlanes(size_t count, Point const * points, Vector const * normals, Plane * planes) const = 0;
		virtual void TransformToPlanes(size_t count, DPoint const * points, Vector const * normals, DPlane * planes) const = 0;
//
		virtual void TransformToPlanes(size_t count, DPoint const * points, Vector const * normals, Plane * planes) const = 0;


		virtual	void Transform_Parameter(size_t count, Parameter const * in_param, Parameter * out_param, int width) const = 0;

		virtual Cuboid_3D<float> Transform(Cuboid_3D<float> const & cuboid) const = 0;
		virtual Sphere_3D<float> Transform(Sphere_3D<float> const & sphere) const = 0;
		virtual Cuboid_3D<double> Transform(Cuboid_3D<double> const & cuboid) const = 0;
		virtual Sphere_3D<double> Transform(Sphere_3D<double> const & sphere) const = 0;
};

Matrix Matrix::Copy () const {
	Matrix				temp;
	temp.counted_item = CP::counted_item->Clone();
	temp.initialize();
	return temp;
}
Matrix & Matrix::Modify () {
	if (CP::counted_item->shared()) {
		Matrix_Base *	temp = CP::counted_item->Clone();
		release();
		CP::counted_item = temp;
		retain();
	}
	return *this;
}


template <typename F>
class Matrix_Pointer : public Matrix {

	public:
		typedef	Matrix_Data<F>	Matrix_Data_Type;

		Matrix_Pointer () : Matrix () {};	// needed because the next one is needed...
		// these don't inherit as needed due to the effective type of the result
		Matrix_Pointer (Null_Object const &) : Matrix () {}
		INLINE Matrix_Pointer const &	operator= (Null_Object const &) {
			Matrix::release();
			Matrix::counted_item = 0;
			return *this;
		}

		INLINE Matrix_Pointer Copy () const;
		INLINE Matrix_Pointer (Matrix const & base_that);
		INLINE Matrix_Pointer(Matrix && base_that);

		INLINE Matrix_Pointer const &	operator= (Matrix const & base_that);
		INLINE Matrix_Pointer const &	operator= (Matrix && base_that);


		static Matrix_Pointer Create () {
			Matrix_Pointer	temp;
			temp.counted_item = NEW (Matrix_Data_Type) ();
			temp.initialize();
			return temp;
		}

		INLINE Matrix_Data_Type const *	operator-> () const		{ return (Matrix_Data_Type const *)counted_item; }
		INLINE Matrix_Data_Type *		operator-> ()			{ return (Matrix_Data_Type *)counted_item; }
};


template <typename F>
class Matrix_Data : public Matrix_Base, public Matrix_4x4<F> {
	typedef	typename Float_Traits<F>::Alternative	D;
	friend class Matrix_Pointer<F>;
	friend class Matrix_Data<D>;
	friend class Matrix;
	private:
		typedef	Matrix_Pointer<F>		Pointer;
		typedef	Matrix_Pointer<D>		DPointer;

		typedef Matrix_4x4<F>			M44;

		mutable	Pointer		adjoint_;
		mutable	DPointer	other_;


		Matrix_Data() : Matrix_Base () {	Init_Identity();	}
		//Matrix_Data (Matrix_Data const & that);
		Matrix_Data (Matrix const & that);

		void invalidate_derived() {
			adjoint_ = null;
			other_ = null;
		}

		void validate_contents(Matrix_Contents test_contents = Matrix_Contents_ANYTHING) {
			contents &= ~test_contents;
			contents |= M44::Contents(M44::m, symmetric_scale, test_contents);
		};

		void validate_determinant3x3_negative() const {
			if (!ANYBIT (data_flags, Matrix_Data_Flags_NEG3X3_VALID)) {
				double		det = Determinant3x3();

				data_flags |= Matrix_Data_Flags_NEG3X3_VALID;

				if (det < 0)
					data_flags |= Matrix_Data_Flags_NEG3X3;
				else
					data_flags &= ~Matrix_Data_Flags_NEG3X3;
			}

			ASSERT((M44::Determinant3x3(M44::m) < 0) == ANYBIT (data_flags, Matrix_Data_Flags_NEG3X3));
		};

	public:
		typedef	F		MR[4];		// Matrix_Row
		F				symmetric_scale;

		Matrix_Data *	Clone () const;
		int				Precision() const	{ return Float_Traits<F>::Type; }

		Matrix_4x4<float> const &	ElementsFloat() const {
			if (Float_Traits<float>::Type == Float_Traits<F>::Type)
				return (Matrix_4x4<float> const &)*this;
			else
				return Other()->ElementsFloat();
		}

		Matrix_4x4<double> const &	ElementsDouble() const {
			if (Float_Traits<double>::Type == Float_Traits<F>::Type)
				return (Matrix_4x4<double> const &)*this;
			else
				return Other()->ElementsDouble();
		}

		INLINE Pointer const & adjoint() const {
			if (adjoint_ == null) {
				adjoint_ = Pointer::Create ();

				if (!Is_Identity()) {
					M44::Compute_Adjoint (M44::m, adjoint_->m);
					adjoint_->validate_contents(contents);
				}
			}

			return adjoint_;
		}
		DPointer		Other () const {
			if (other_ == null) {
				other_ = DPointer::Create ();

				for (int i=0; i<4; ++i)
					for (int j=0; j<4; ++j)
						other_->m[i][j] = (D)M44::m[i][j];
				other_->symmetric_scale = (D)symmetric_scale;
				other_->Orthogonalize();
				other_->validate_contents();
				//other_->contents = contents;
			}

			return other_;
		}



		INLINE bool operator == (Matrix_Data const & that) const {
			if (Precision() != that.Precision())
				return false;

			if (contents != that.contents)
				return false;

			if (symmetric_scale != that.symmetric_scale)
				return false;

			if (!EQUAL_MEMORY (M44::m, 16 * sizeof(F), that.m))
				return false;

			return true;
		}
		INLINE bool operator != (Matrix_Data const & that) const {
			return !operator== (that);
		}

		INLINE F const * Elements() const {
			return &M44::m[0][0];
		}

		INLINE bool Equals(Matrix_Data const & that, int tol=32) const {
			F const *	m1 = (F const *)M44::m;
			F const *	m2 = (F const *)that.m;
			for (int i=0; i<16; i++) {
				if (!Float::Equals(m1[i], m2[i], tol))
					return false;
			}
			return true;
		}

		INLINE bool Equals3x3(Matrix_Data const & that, int tol=32) const {
			F const *	m1 = (F const *)M44::m;
			F const *	m2 = (F const *)that.m;
			for (int i=0; i<12; i++) {
				if (!Float::Equals(m1[i], m2[i], tol))
					return false;
			}
			return true;
		}

		int hash_key() const {
			int		value = contents;
			for (int i=0; i<4; i++) {
				for (int j=0; j<3; j++) {
					uint32_t v[2];
					memcpy(&v, &M44::m[i][j], sizeof(F));
					for (size_t k = 0; k < sizeof(F)/sizeof(uint32_t); ++k)
						value += (v[k] & 0x0000FFFF) ^ (v[k] >> 16);
				}
			}

			return value;
		}

		INLINE Vector_3D<F> Row_Vector(int n) const {
			return Vector_3D<F>(M44::m[n][0], M44::m[n][1], M44::m[n][2]);
		}

		INLINE void Normalize3x3() {
			Vector_3D<F>	v;
			memcpy(&v, &M44::m[0][0], sizeof(Vector_3D<F>));
			v.Normalize();
			memcpy(&M44::m[0][0], &v, sizeof(Vector_3D<F>));

			memcpy(&v, &M44::m[1][0], sizeof(Vector_3D<F>));
			v.Normalize();
			memcpy(&M44::m[1][0], &v, sizeof(Vector_3D<F>));

			memcpy(&v, &M44::m[2][0], sizeof(Vector_3D<F>));
			v.Normalize();
			memcpy(&M44::m[2][0], &v, sizeof(Vector_3D<F>));
		}



		INLINE Pointer Create_Invert_Scale_Matrix() const {
			F 		xscale = (F)Row_Vector(0).Length();
			F 		yscale = (F)Row_Vector(1).Length();
			F 		zscale = (F)Row_Vector(2).Length();

			if (xscale == 0)
				xscale = 1;
			if (yscale == 0)
				yscale = 1;
			if (zscale == 0)
				zscale = 1;

			Pointer		matrix = Pointer::Create();
			matrix->Init_Scale(1 / xscale, 1 / yscale, 1 / zscale);
			return matrix;
		}


		bool Orthogonalize() {
			if (symmetric_scale == 0)
				return false; // can't do it

			//Re-normalize each row vector in the 3x3
			F	norm = M44::m[0][0] * M44::m[0][0] + M44::m[0][1] * M44::m[0][1] + M44::m[0][2] * M44::m[0][2];

			if (norm != 1) {
				norm = symmetric_scale / Sqrt (norm);
				M44::m[0][0] *= norm;
				M44::m[0][1] *= norm;
				M44::m[0][2] *= norm;
			}

			norm = M44::m[1][0] * M44::m[1][0] + M44::m[1][1] * M44::m[1][1] + M44::m[1][2] * M44::m[1][2];
			if (norm != 1.0f) {
				norm = symmetric_scale / Sqrt (norm);
				M44::m[1][0] *= norm;
				M44::m[1][1] *= norm;
				M44::m[1][2] *= norm;
			}

			norm = M44::m[2][0] * M44::m[2][0] + M44::m[2][1] * M44::m[2][1] + M44::m[2][2] * M44::m[2][2];
			if (norm != 1.0f) {
				norm = symmetric_scale / Sqrt (norm);
				M44::m[2][0] *= norm;
				M44::m[2][1] *= norm;
				M44::m[2][2] *= norm;
			}

			return true;
		}



		double Symmetric_Scale() const {
			return symmetric_scale;
		};

		double Max_Scale() const {
			if (Is_Symmetric()) {
#ifdef HOOPS_DEBUG_BUILD
				F	test_length = (F)Max(Row_Vector(0).Length(), Row_Vector(1).Length(), Row_Vector(2).Length());
				ASSERT(Float::Equals(symmetric_scale, test_length));
#endif
				return symmetric_scale;
			}

			double length_squared = Max(Row_Vector(0).LengthSquared(), Row_Vector(1).LengthSquared(), Row_Vector(2).LengthSquared());

			return Sqrt(length_squared);
		};

		bool Is_3D() const {
			return (M44::m[0][2] != 0 || M44::m[1][2] != 0 || M44::m[3][2] != 0);
		};

		bool Is_SINGULAR() const {
			return (M44::m[3][3] == 0);
		}

		bool Is_Symmetric() const {
			return (symmetric_scale != 0);
		};

		bool Is_Abnormal () const {
			return Is_Abnormal(16, &M44::m[0][0]);
		}

		void ASSERT_IDENTITY() const {
			ASSERT(M44::Verify_Identity(M44::m) == !ANYBIT (contents, Matrix_Contents_ANYTHING));
		};

		double Determinant3x3() const {
			return M44::Determinant3x3(M44::m);
		};
		double Determinant() const {
			return M44::Determinant(M44::m);
		};

		INLINE bool Is_Rotation() const {
#			ifdef HOOPS_DEBUG_BUILD
				ASSERT (ANYBIT(contents, Matrix_Contents_ROTATION) ==
						(M44::m[0][1] != 0 || M44::m[0][2] != 0 || M44::m[1][0] != 0 || M44::m[1][2] != 0 || M44::m[2][0] != 0 || M44::m[2][1] != 0));
#			endif
			return ANYBIT(contents, Matrix_Contents_ROTATION);
		}




		INLINE void Init_Identity() {
			M44::Init_Identity(M44::m);
			symmetric_scale = 1;
			contents = Matrix_Contents_NONE;
			data_flags = Matrix_Data_Flags_NONE;
			invalidate_derived();
		};

		void Init (MR const * in_44, bool perspective=false) {
			M44::Init(in_44, M44::m, perspective);
			invalidate_derived();
			validate_contents();
		}
		INLINE void Init (F const *	in_44, bool perspective=false) {Init((MR const *)in_44, perspective);};

		void Init_Scale(F const & x, F const & y, F const & z) {
			M44::Init_Scale(x, y, z, M44::m);

			symmetric_scale = 0.0f;
			invalidate_derived();
			contents = Matrix_Contents_NONE;
			data_flags = Matrix_Data_Flags_NONE;

			if ((z == y || -z == y) && (z == x || -z == x)) {
				if (z < 0)
					symmetric_scale = -z;
				else
					symmetric_scale = z;

				if (z != 1 || x != 1 || y != 1)
					contents = Matrix_Contents_SCALE;
			}
			else
				contents = Matrix_Contents_SCALE;
		}
		INLINE void Init_Scale(F const & scale) {Init_Scale(scale, scale, scale);};

		void Init_Rotation(F const & x, F const & y, F const & z) {
			M44::Init_Rotation (x, y, z, M44::m);
			// Orthogonalization is already necessary because we only approximate the trig functions.
			Orthogonalize();

			symmetric_scale = 1;
			invalidate_derived();

			data_flags = Matrix_Data_Flags_NONE;
			validate_contents(Matrix_Contents_ROTATION);
		}

		INLINE void Init_Translation(F const & x, F const & y, F const & z) {
			M44::Init_Translation(x, y, z, M44::m);

			symmetric_scale = 1;
			invalidate_derived();
			contents = Matrix_Contents_NONE;
			data_flags = Matrix_Data_Flags_NONE;

			if (z != 0 || x != 0 || y != 0)
				contents = Matrix_Contents_TRANSLATION;
		}
		INLINE void Init_Translation(Vector_3D<F> const & v) {Init_Translation(v.x, v.y, v.z);};

		INLINE void Init_Reflection(Plane_3D<F> const & plane) {
			Init_Identity();

			M44::m[0][0] = 1 - 2 * plane.a * plane.a;
			M44::m[1][1] = 1 - 2 * plane.b * plane.b;
			M44::m[2][2] = 1 - 2 * plane.c * plane.c;

			M44::m[0][1] = M44::m[1][0] = -2 * plane.a * plane.b;
			M44::m[0][2] = M44::m[2][0] = -2 * plane.a * plane.c;
			M44::m[1][2] = M44::m[2][1] = -2 * plane.b * plane.c;

			M44::m[3][0] = -2 * plane.a * plane.d;
			M44::m[3][1] = -2 * plane.b * plane.d;
			M44::m[3][2] = -2 * plane.c * plane.d;

			contents = Matrix_Contents_ROTATION | Matrix_Contents_SCALE;
			if (plane.d != 0)
				contents |= Matrix_Contents_TRANSLATION;
			data_flags = Matrix_Data_Flags_NONE;
		}


		INLINE void Translate(Vector_3D<F> const & translate) {
			if (translate.z != 0 || translate.x != 0 || translate.y != 0) {
				M44::m[3][0] += translate.x;
				M44::m[3][1] += translate.y;
				M44::m[3][2] += translate.z;
				invalidate_derived();
				validate_contents(Matrix_Contents_TRANSLATION);
			}
		};

		INLINE void OverrideTranslate(Vector_3D<F> const & translate) {
			M44::m[3][0] = translate.x;
			M44::m[3][1] = translate.y;
			M44::m[3][2] = translate.z;
			invalidate_derived();
			validate_contents(Matrix_Contents_TRANSLATION);
		};

		INLINE void Init_Column_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3) {
			M44::Init_Column_Basis(v1, v2, v3, M44::m);
			invalidate_derived();
			validate_contents();
		}

		INLINE void Init_Column_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3,
										Vector_3D<F> const & translate) {
			M44::Init_Column_Basis(v1, v2, v3, translate, M44::m);
			invalidate_derived();
			validate_contents();
		}

		INLINE void Init_Row_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3) {
			M44::Init_Row_Basis(v1, v2, v3, M44::m);
			invalidate_derived();
			validate_contents();
		}

		INLINE void Init_Row_Basis(Vector_3D<F> const & v1, Vector_3D<F> const & v2, Vector_3D<F> const & v3,
										Vector_3D<F> const & translate) {
			M44::Init_Row_Basis(v1, v2, v3, translate, M44::m);
			invalidate_derived();
			validate_contents();
		}

		INLINE void Scale (F const & scale) {;
			M44::m[0][0] *= scale;
			M44::m[0][1] *= scale;
			M44::m[0][2] *= scale;
			M44::m[1][0] *= scale;
			M44::m[1][1] *= scale;
			M44::m[1][2] *= scale;
			M44::m[2][0] *= scale;
			M44::m[2][1] *= scale;
			M44::m[2][2] *= scale;
			M44::m[3][0] *= scale;
			M44::m[3][1] *= scale;
			M44::m[3][2] *= scale;

			invalidate_derived();
			contents |= Matrix_Contents_SCALE;
		}

		INLINE void LocalScale (F const & scale) {
			M44::m[0][0] *= scale;
			M44::m[1][0] *= scale;
			M44::m[2][0] *= scale;
			M44::m[3][0] *= scale;
			M44::m[0][1] *= scale;
			M44::m[1][1] *= scale;
			M44::m[2][1] *= scale;
			M44::m[3][1] *= scale;

			invalidate_derived();
			contents |= Matrix_Contents_SCALE;
		}

		void Scale_And_Translate (
			Vector const &	scale,
			Vector const &	translate) {

			if (ANYBIT(data_flags, Matrix_Data_Flags_NEG3X3_VALID)) {
				if (scale.x < 0)
					data_flags ^= Matrix_Data_Flags_NEG3X3;
				if (scale.y < 0)
					data_flags ^= Matrix_Data_Flags_NEG3X3;
				if (scale.z < 0)
					data_flags ^= Matrix_Data_Flags_NEG3X3;
			}

			symmetric_scale = 0;

			invalidate_derived();
			contents |= Matrix_Contents_TRANSLATION|Matrix_Contents_SCALE;

			M44::m[0][0] = M44::m[0][0] * scale.x + M44::m[0][3] * translate.x;
			M44::m[0][1] = M44::m[0][1] * scale.y + M44::m[0][3] * translate.y;
			M44::m[0][2] = M44::m[0][2] * scale.z + M44::m[0][3] * translate.z;

			M44::m[1][0] = M44::m[1][0] * scale.x + M44::m[1][3] * translate.x;
			M44::m[1][1] = M44::m[1][1] * scale.y + M44::m[1][3] * translate.y;
			M44::m[1][2] = M44::m[1][2] * scale.z + M44::m[1][3] * translate.z;

			M44::m[2][0] = M44::m[2][0] * scale.x + M44::m[2][3] * translate.x;
			M44::m[2][1] = M44::m[2][1] * scale.y + M44::m[2][3] * translate.y;
			M44::m[2][2] = M44::m[2][2] * scale.z + M44::m[2][3] * translate.z;

			M44::m[3][0] = M44::m[3][0] * scale.x + M44::m[3][3] * translate.x;
			M44::m[3][1] = M44::m[3][1] * scale.y + M44::m[3][3] * translate.y;
			M44::m[3][2] = M44::m[3][2] * scale.z + M44::m[3][3] * translate.z;
		}

		INLINE void Init_Offaxis_Rotation (Vector const & axis, float const & angle) {
			M44::Init_Offaxis_Rotation (axis, angle, M44::m);
			//Orthogonalization is already necessary because we only approximate the trig functions.
			Orthogonalize();

			symmetric_scale = 1;
			invalidate_derived();

			validate_contents(Matrix_Contents_ROTATION);
			data_flags = Matrix_Data_Flags_NONE;
		}




		INLINE Matrix_Data operator * (Matrix_Data const & right) const {
			Matrix_Data const &		left = *this;
			Matrix_Data				result;

			M44::Compute_Product (left.m, right.m, result.m, ANYBIT (left.contents | right.contents, Matrix_Contents_PERSPECTIVE));
			result.validate_contents();
			result.Orthogonalize();

			return result;
		}

		Point_3D<F> Transform(Point_3D<F> const & point, float const & point_w, float & ws) const {
			return M44::Transform(point, point_w, ws);
		}
		Point_3D<F> Transform(Point_3D<F> const & point, float & w) const {
			if (Is_Identity()) {
				w = 1;
				return point;
			}
			return M44::Transform(point, 1, w);
		}
		Point_3D<F> Transform(Point_3D<F> const & point) const {
			if (Is_Identity())
				return point;
			float	w;
			return M44::Transform(point, 1, w);
		}

		Point_3D<D> Transform(Point_3D<D> const & point, float const & point_w, float & w) const {
			return Other()->Transform (point, point_w, w);
		}
		Point_3D<D> Transform(Point_3D<D> const & point, float & w) const {
			return Other()->Transform (point, w);
		}
		Point_3D<D> Transform(Point_3D<D> const & point) const {
			return Other()->Transform (point);
		}


		void Transform(size_t count, Point_3D<F> const * in_points, Point_3D<F> * out_points, float wlimit = -1.0f, float * ws = null) const {
			ASSERT(ANYBIT(contents, Matrix_Contents_PERSPECTIVE) != (M44::m[0][3] == 0 && M44::m[1][3] == 0 && M44::m[2][3] == 0 && M44::m[3][3] == 1));

			if (!ANYBIT(contents, Matrix_Contents_PERSPECTIVE)) {
				if (ws != null)
					set_float_ones(count, ws);

				if (Is_Identity()) {
					if (in_points != out_points)
						COPY_ARRAY (in_points, count, Point_3D<F>, out_points);
				}
				else
					M44::Transform_Points(M44::m, count, in_points, out_points);
			}
			else {
				if (wlimit == -1.0f) {
					wlimit = 0.001f;
					//ASSERT(0); // this should probably not happen
				}
				else {
					//ASSERT(ws != null);
				}
				M44::Transform_Points(M44::m, count, in_points, out_points, wlimit, ws);
			}
		}
		void Transform(size_t count, Point_3D<D> const * in_points, Point_3D<D> * out_points, float wlimit = -1.0f, float * ws = null) const {
			Other()->Transform (count, in_points, out_points, wlimit, ws);
		}


		void Transform(size_t count, DPoint const * in_points, Point * out_points, float wlimit = -1.0f, float * ws=null) const {
#ifndef DISABLE_DOUBLE_PRECISION
			if (wlimit == -1.0f) {
				wlimit = 0.001f;
				//ASSERT(0); // this should probably not happen
			}
			else {
				//ASSERT(ws != null);
			}
			M44::Transform_Points(M44::m, count, in_points, out_points, wlimit, ws);
#else
			UNREFERENCED (count);	UNREFERENCED (in_points);	UNREFERENCED (out_points);
			UNREFERENCED (wlimit);	UNREFERENCED (ws);
#endif
		}


		void Transform(size_t count, Point const * in_points, DPoint * out_points) const {
#ifndef DISABLE_DOUBLE_PRECISION
			M44::Transform_Points(M44::m, count, in_points, out_points);
#else
			UNREFERENCED (count);	UNREFERENCED (in_points);	UNREFERENCED (out_points);
#endif
		}



		float Transform_X(Point const & point) const {
			return M44::Transform_X(point);
		}
		float Transform_Y(Point const & point) const {
			return M44::Transform_Y(point);
		}
		float Transform_Z(Point const & point) const {
			return M44::Transform_Z(point);
		}
		float Transform_W(Point const & point) const {
			return M44::Transform_W(point, ANYBIT(contents, Matrix_Contents_PERSPECTIVE));
		}

		double Transform_X(DPoint const & point) const {
			return M44::Transform_X(point);
		}
		double Transform_Y(DPoint const & point) const {
			return M44::Transform_Y(point);
		}
		double Transform_Z(DPoint const & point) const {
			return M44::Transform_Z(point);
		}
		double Transform_W(DPoint const & point) const {
			return M44::Transform_W(point, ANYBIT(contents, Matrix_Contents_PERSPECTIVE));
		}


		Vector Transform(Vector const & vector, bool normalize=true) const {
			if (Is_Identity())
				return vector;
			return M44::Transform(vector, normalize && ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}
		DVector Transform(DVector const & vector, bool normalize=true) const {
			if (Is_Identity())
				return vector;
			return M44::Transform(vector, normalize && ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}
		// You will probably need use the adjoint()
		void Transform(size_t count, Vector const * in_vector, Vector * out_vector, bool normalize=true) const {
			if (Is_Identity()) {
				if (in_vector != out_vector)
					COPY_ARRAY (in_vector, count, Vector, out_vector);
				return;
			}
			normalize = normalize && ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);
			for (size_t i = 0; i < count; ++i)
				*out_vector++ = M44::Transform(*in_vector++, normalize);
		}
		void Transform(size_t count, DVector const * in_vector, DVector * out_vector, bool normalize=true) const {
			if (Is_Identity()) {
				if (in_vector != out_vector)
					COPY_ARRAY (in_vector, count, DVector, out_vector);
				return;
			}
			normalize = normalize && ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);
			for (size_t i = 0; i < count; ++i)
				*out_vector++ = M44::Transform(*in_vector++, normalize);
		}

		// You will probably need use the adjoint()
		void Transform(size_t count, Vector2D const * in_vector, Vector2D * out_vector, bool normalize=true) const {
			if (Is_Identity()) {
				if (in_vector != out_vector)
					COPY_ARRAY (in_vector, count, Vector2D, out_vector);
				return;
			}
			normalize = normalize && ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);
			for (size_t i = 0; i < count; ++i)
				*out_vector++ = Vector2D(M44::Transform(*in_vector++, normalize));
		}
		void Transform(size_t count, DVector2D const * in_vector, DVector2D * out_vector, bool normalize=true) const {
			if (Is_Identity()) {
				if (in_vector != out_vector)
					COPY_ARRAY (in_vector, count, DVector2D, out_vector);
				return;
			}
			normalize = normalize && ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);
			for (size_t i = 0; i < count; ++i)
				*out_vector++ = DVector2D(M44::Transform(*in_vector++, normalize));
		}

		// You will probably need use the adjoint()
		Plane Transform(Plane const & plane) const {
			if (Is_Identity())
				return plane;
			return M44::Transform(plane, ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}
		DPlane Transform(DPlane const & plane) const {
			if (Is_Identity())
				return plane;
			return M44::Transform(plane, ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}
		// You will probably need use the adjoint()
		void Transform(size_t count, Plane const * in_plane, Plane * out_plane) const {
			if (Is_Identity()) {
				if (in_plane != out_plane)
					COPY_ARRAY (in_plane, count, Plane, out_plane);
				return;
			}

			bool normalize = ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);

			for (size_t i = 0; i < count; ++i)
				*out_plane++ =  M44::Transform(*in_plane++, normalize);
		}
		void Transform(size_t count, DPlane const * in_plane, DPlane * out_plane) const {
			if (Is_Identity()) {
				if (in_plane != out_plane)
					COPY_ARRAY (in_plane, count, DPlane, out_plane);
				return;
			}

			bool normalize = ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);

			for (size_t i = 0; i < count; ++i)
				*out_plane++ = M44::Transform(*in_plane++, normalize);
		}
		void Transform(size_t count, Plane const * in_plane, DPlane * out_plane) const {
			if (Is_Identity()) {
				for (size_t i=0; i<count; ++i)
					*out_plane++ = DPlane (*in_plane++);
				return;
			}

			bool normalize = ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION);

			for (size_t i = 0; i < count; ++i)
				*out_plane++ = M44::Transform(DPlane(*in_plane++), normalize);
		}

		float Transform_C(Plane const & plane) const {
			return M44::Transform_C(plane);
		}

		double Transform_C(DPlane const & plane) const {
			return M44::Transform_C(plane);
		}

		// You will probably need use the adjoint()
		Plane TransformToPlane(Vector const & normal, Point const & point) const {
			Plane	plane (normal, point);
			if (Is_Identity())
				return plane;
			return M44::Transform(plane, ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}
		DPlane TransformToPlane(Vector const & normal, DPoint const & point) const {
			DPlane	plane (DVector(normal), point);
			if (Is_Identity())
				return plane;
			return M44::Transform(plane, ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}

		// You will probably need use the adjoint()
		void TransformToPlanes(size_t count, Point const * points, Vector const * normals, Plane * planes) const {
			if (Is_Identity()) {
				for (size_t i = 0; i < count; ++i)
					planes[i] = Plane(normals[i], points[i]);
			}
			else {
				for (size_t i = 0; i < count; ++i) {
					Plane p(normals[i], points[i]);
					planes[i] = M44::Transform(p, true);
				}
			}
		}
		void TransformToPlanes(size_t count, DPoint const * points, Vector const * normals, DPlane * planes) const {
#ifndef DISABLE_DOUBLE_PRECISION
			if (Is_Identity()) {
				for (size_t i = 0; i < count; ++i)
					planes[i] = DPlane(DVector(normals[i]), points[i]);
			}
			else {
				for (size_t i = 0; i < count; ++i) {
					DPlane	p(DVector(normals[i]), points[i]);
					planes[i] = M44::Transform(p, true);
				}
			}
#else
			UNREFERENCED (count);	UNREFERENCED (points);	UNREFERENCED (normals);	UNREFERENCED (planes);
#endif
		}
//TODO: this one can probably go away once driver-side "world planes" work is done
		void TransformToPlanes(size_t count, DPoint const * points, Vector const * normals, Plane * planes) const {
#ifndef DISABLE_DOUBLE_PRECISION
			if (Is_Identity()) {
				for (size_t i = 0; i < count; ++i)
					planes[i] = Plane(normals[i], Point(points[i]));
			}
			else {
				for (size_t i = 0; i < count; ++i) {
					Plane p(normals[i], Point(points[i]));
					planes[i] = M44::Transform(p, true);
				}
			}
#else
			UNREFERENCED (count);	UNREFERENCED (points);	UNREFERENCED (normals);	UNREFERENCED (planes);
#endif
		}

		void Transform_Parameter(size_t count, Parameter const * in_param, Parameter * out_param, int width) const {
			// XXX this need reform
			ASSERT(width==3);

			if (Is_Identity()) {
				if (out_param != in_param)
					memcpy(out_param, in_param, count*width*sizeof(Parameter));
				return;
			}

			Parameter	p[3];

			p[0] = in_param[0];
			p[1] = in_param[1];
			p[2] = in_param[2];

			out_param[0] = float(M44::m[0][0]*p[0] + M44::m[1][0]*p[1] + M44::m[2][0]*p[2] + M44::m[3][0]);
			out_param[1] = float(M44::m[0][1]*p[0] + M44::m[1][1]*p[1] + M44::m[2][1]*p[2] + M44::m[3][1]);
			out_param[2] = float(M44::m[0][2]*p[0] + M44::m[1][2]*p[1] + M44::m[2][2]*p[2] + M44::m[3][2]);

			if (Is_Perspective()) {
				float inv_w	= 1.0f / float(M44::m[0][3]*p[0] + M44::m[1][3]*p[1] + M44::m[2][3]*p[2] + M44::m[3][3]);
				out_param[0] *= inv_w;
				out_param[1] *= inv_w;
				out_param[2] *= inv_w;
			}
		}

		Vector TransformToVector(Plane const & plane) const {
			return M44::TransformToVector(plane, ANYBIT(contents, Matrix_Contents_SCALE|Matrix_Contents_TRANSLATION));
		}

		Cuboid_3D<float> Transform(Cuboid_3D<float> const & cuboid) const {
			if (Is_Identity())
				return cuboid;

			if (Is_Perspective()) {
				Point_3D<float>	in[8], out[8];
				cuboid.Generate_Cuboid_Points(in);
				Transform(8, in, out);
				return Cuboid_3D<float>(8, out);
			}

			if (Is_Rotation()) {
				Cuboid_3D<double> result;

				result.min = result.max = Point_3D<double>(M44::m[3][0], M44::m[3][1], M44::m[3][2]);

				for (size_t i=0; i<3; ++i) {
					for (size_t j=0; j<3; ++j) {
						double	v1 = cuboid.min[i] * M44::m[i][j];
						double	v2 = cuboid.max[i] * M44::m[i][j];

						if (v1 < v2) {
							result.min[j] += v1;
							result.max[j] += v2;
						}
						else {
							result.min[j] += v2;
							result.max[j] += v1;
						}
					} 
				} 

				Vector_3D<double>	diag = result.max - result.min;
				double				extra = 5.0e-6 * diag.Length();
				result.Expand (extra);
				return Cuboid_3D<float>(result);
			}

			Point_3D<float>	p[2];

			p[0] = M44::Transform(cuboid.min);
			p[1] = M44::Transform(cuboid.max);

			return Cuboid_3D<float>(2, p);
		}
		Sphere_3D<float> Transform(Sphere_3D<float> const & sphere) const {
			if (Is_Identity())
				return sphere;

			Point_3D<float>		new_center = M44::Transform(sphere.center);
			float				new_radius = float(Max_Scale() * sphere.radius);

			return Sphere_3D<float>(new_center, new_radius);
		}
		Cuboid_3D<double> Transform(Cuboid_3D<double> const & cuboid) const {

			if (Is_Identity())
				return cuboid;

			if (Is_Perspective()) {
				Point_3D<double>	in[8], out[8];
				cuboid.Generate_Cuboid_Points(in);
				Transform(8, in, out);
				return Cuboid_3D<double>(8, out);
			}

			if (Is_Rotation()) {

				Cuboid_3D<double> result;

				result.min = result.max = Point_3D<double>(M44::m[3][0], M44::m[3][1], M44::m[3][2]);

				for (size_t i=0; i<3; ++i) {

					for (size_t j=0; j<3; ++j) {

						double v1 = cuboid.min[i] * M44::m[i][j];
						double v2 = cuboid.max[i] * M44::m[i][j];

						if (v1 < v2) {
							result.min[j] += v1;
							result.max[j] += v2;
						}
						else {
							result.min[j] += v2;
							result.max[j] += v1;
						}
					} 
				} 

				return result;
			}

			Point_3D<double>	p[2];

			p[0] = M44::Transform(cuboid.min);
			p[1] = M44::Transform(cuboid.max);

			return Cuboid_3D<double>(2, p);
		}
		Sphere_3D<double> Transform(Sphere_3D<double> const & sphere) const {
			if (Is_Identity())
				return sphere;

			Point_3D<double>	new_center = M44::Transform(sphere.center);
			double				new_radius = Max_Scale() * sphere.radius;

			return Sphere_3D<double>(new_center, new_radius);
		}




		INLINE void Copy(F * out_44, bool perspective = true) const {
			M44::Copy (M44::m, out_44, perspective);
		}

		INLINE void Copy(MR * out_44, bool perspective = true) const {
			M44::Copy (M44::m, out_44, perspective);
		}

		INLINE void Copy(MR & out_44, bool perspective = true) const {
			M44::Copy (M44::m, &out_44, perspective);
		}
};


template <typename F>
Matrix_Data<F>::Matrix_Data (Matrix const & that) : Matrix_Base () {
	if (that == null)
		Init_Identity();
	else if (that->Precision() == Float_Traits<F>::Type)
		ASSERT(0);//roughly *this = *(Matrix_Data<F> const *)that.counted();	// should never need this
	else {
		Matrix_Data<D> const *	other = (Matrix_Data<D> const *)that.counted();
		for (int i=0; i<4; ++i)
			for (int j=0; j<4; ++j)
				M44::m[i][j] = (F)other->m[i][j];
		symmetric_scale = (F)other->symmetric_scale;
		contents = other->contents;
	}
}

template <typename F>
Matrix_Data<F> * Matrix_Data<F>::Clone () const {
	typedef Matrix_Data<F>	Matrix_Data_Type;
	return NEW (Matrix_Data_Type) (*this);
}



// Caution: that->Precision() may optimize away based on the type F
template <typename F>
Matrix_Pointer<F>::Matrix_Pointer (Matrix const & base_that) {
	Matrix_Pointer const &	that = (Matrix_Pointer const &)base_that;

	if (that.counted_item == 0)
		Matrix::counted_item = 0;
	else if (base_that->Precision() == Float_Traits<F>::Type)
		Matrix::counted_item = (Matrix_Data_Type *)that.counted_item;
	else
		Matrix::counted_item = NEW (Matrix_Data_Type) (that);
	Matrix::retain();
}

template <typename F>
Matrix_Pointer<F>::Matrix_Pointer(Matrix && base_that) {
	Matrix_Pointer &&	that = (Matrix_Pointer &&)base_that;

	if (that.counted_item == 0)
		Matrix::counted_item = 0;
	else if (base_that->Precision() == Float_Traits<F>::Type) {
		Matrix::counted_item = (Matrix_Data_Type *)that.counted_item;
		that.counted_item = 0;
	}
	else {
		Matrix::counted_item = NEW(Matrix_Data_Type) (that);
		Matrix::retain();
	}
}

template <typename F>
Matrix_Pointer<F> const &	Matrix_Pointer<F>::operator= (Matrix const & base_that) {
	Matrix_Pointer const &	that = (Matrix_Pointer const &)base_that;

	if (Matrix::counted_item != that.counted_item) {
		Matrix_Base *		old = Matrix::counted_item;
		if (that.counted_item == 0)
			Matrix::counted_item = 0;
		else if (base_that->Precision() == Float_Traits<F>::Type)
			Matrix::counted_item = (Matrix_Data_Type *)that.counted_item;
		else
			Matrix::counted_item = NEW (Matrix_Data_Type) (that);
		Matrix::retain();
		if (old != 0)
			old->release();
	}
	return *this;
}

template <typename F>
Matrix_Pointer<F> const &	Matrix_Pointer<F>::operator= (Matrix && base_that) {
	Matrix_Pointer &&	that = (Matrix_Pointer  &&)base_that;

	Matrix_Base *		old = Matrix::counted_item;
	if (old != 0)
		old->release();

	if (that.counted_item == 0)
		Matrix::counted_item = 0;
	else if (base_that->Precision() == Float_Traits<F>::Type) {
		Matrix::counted_item = (Matrix_Data_Type *)that.counted_item;
		that.counted_item = 0;
	}
	else {
		Matrix::counted_item = NEW(Matrix_Data_Type) (that);
		Matrix::retain();
	}

	return *this;
}

template <typename F>
Matrix_Pointer<F> Matrix_Pointer<F>::Copy () const {
	Matrix_Pointer		temp;
	temp.counted_item = NEW (Matrix_Data_Type) ((Matrix_Data_Type const &)*Matrix::counted_item);
	temp.initialize();
	return temp;
}



INLINE bool Matrix::Equals (Matrix const & that, int tol) const {
	if ((*this)->Precision() != that->Precision())
		return false;
	if (that->Precision() == Float_Traits<float>::Type)
		return ((Matrix_Data<float> const *)counted_item)->Equals(*(Matrix_Data<float> const *)that.counted_item, tol);
	else
		return ((Matrix_Data<double> const *)counted_item)->Equals(*(Matrix_Data<double> const *)that.counted_item, tol);
}

INLINE bool Matrix::Equals3x3 (Matrix const & that, int tol) const {
	if ((*this)->Precision() != that->Precision())
		return false;
	if (that->Precision() == Float_Traits<float>::Type)
		return ((Matrix_Data<float> const *)counted_item)->Equals3x3(*(Matrix_Data<float> const *)that.counted_item, tol);
	else
		return ((Matrix_Data<double> const *)counted_item)->Equals3x3(*(Matrix_Data<double> const *)that.counted_item, tol);
}



typedef Matrix_Pointer<float>		Matrix_Single;
typedef Matrix_Pointer<double>		Matrix_Double;


INLINE Matrix Matrix::operator * (Matrix const &	that) const {
	if (counted_item->Precision() == Float_Traits<float>::Type && that->Precision() == Float_Traits<float>::Type) {
		Matrix_Single			result = Matrix_Single::Create();
		Matrix_Single const &	left = (Matrix_Single const &)*this;
		Matrix_Single const &	right = (Matrix_Single const &)that;

		Matrix_4x4<float>::Compute_Product (left->m, right->m, result->m,
											ANYBIT (left->contents | right->contents, Matrix_Contents_PERSPECTIVE));
		result->validate_contents();
		result->Orthogonalize();

		return result;
	}
	else {
		Matrix_Double			result = Matrix_Double::Create();
		Matrix_Double const	left = *this;
		Matrix_Double const	right = that;

		Matrix_4x4<double>::Compute_Product (left->m, right->m, result->m,
											 ANYBIT (left->contents | right->contents, Matrix_Contents_PERSPECTIVE));
		result->validate_contents();
		result->Orthogonalize();

		return result;
	}
}







class Timed {
public:
	Time	start_time;
	Time	end_time;

	Timed() {
		start_time = HI_What_Time();
	}

	Time Total() {
		end_time = HI_What_Time();
		return (end_time - start_time) / HERTZ;
	}
};


typedef int	Open_Type;

typedef int32_t Window_Frame_Flags;

struct Display_List_Cache_Internal;
typedef	Counted_Pointer_Pooled<Display_List_Cache_Internal>	Display_List_Cache;

End_HOOPS_Namespace
using namespace HOOPS;


INLINE Name::~Name() {
	if (_length != 0) {
		FREE_ARRAY(_text, _length+1, char);
		_length = 0;
		_text = null;
	}
}

INLINE Name::Name(Name const & that) {
	if (that._length == 0) {
		_length = 0;
		_text = null;
	}
	else {
		_length = that._length;
		ALLOC_ARRAY(_text, _length+1, char);
		COPY_ARRAY(that._text, _length, char, _text);
		_text[_length] = '\0';
	}

	_key = 0;
	_lower_sum = 0;
}

INLINE Name::Name(Name_Const const & that, Name::Create_Mode create_mode) {
	init(that, create_mode, DEFAULT_MEMORY_POOL());
}

INLINE Name::Name(const char * in, Create_Mode create_mode) {
	init(Name_Const(in), create_mode, DEFAULT_MEMORY_POOL());
}

INLINE Name::Name(Memory_Pool * pool, Name_Const const & that, Create_Mode create_mode) {
	init(that, create_mode, pool);
}

INLINE Name::Name(Memory_Pool * pool, const char * in, Create_Mode create_mode) {
	init(Name_Const(in), create_mode, pool);
}


INLINE Name::Name(char const * in, size_t length) {
	if (in == null || length == 0) {
		_length = 0;
		_text = null;
	}
	else {
		_length = length;
		ALLOC_ARRAY (_text, _length+1, char);
		COPY_ARRAY (in, _length, char, _text);
		_text[_length] = '\0';
	}
	_key = 0;
	_lower_sum = 0;
}

INLINE Name_Const const & Concatenate(Thread_Data * thread_data, Name_Const const & a, Name_Const const & b) {

	auto & buffer = thread_data->sprintf_buffer;

	ASSERT(buffer.empty() || (a.text() != &buffer[0] && b.text() != &buffer[0]));
	if (a.length() == 0)
		return b;
	else if (b.length() == 0)
		return a;

	size_t new_size = a.length() + b.length();

	if (new_size + 1 > buffer.size()) {
		buffer.clear();
		buffer.resize(new_size + 1);
	}

	char *	n = &buffer[0];
	COPY_ARRAY(a.text(), a.length(), char, n);
	COPY_ARRAY(b.text(), b.length(), char, &n[a.length()]);
	n[new_size] = '\0';

	thread_data->name_const_tmp = Name_Const(n, new_size);
	return thread_data->name_const_tmp;
}

INLINE Name_Const const & Concatenate(Name_Const const & a, Name_Const const & b) {
	Thread_Data * thread_data;
	FIND_INTERNAL_THREAD_DATA (thread_data);
	return Concatenate(thread_data, a, b);
}


INLINE int Compare (
	Name const &	a,
	Name const &	b) {
	int				cmp;

	if (a.length() < b.length()) {
		if (a.length() == 0)
			return -1;
		if ((cmp = COMPARE_STRINGS_LIMITED (a.text(), a.length(), b.text())) != 0)
			return cmp;

		return -1;
	}
	else if (a.length() > b.length()) {
		if (b.length() == 0)
			return 1;
		if ((cmp = COMPARE_STRINGS_LIMITED (a.text(), b.length(), b.text())) != 0)
			return cmp;

		return 1;
	}

	return COMPARE_STRINGS_LIMITED (a.text(), a.length(), b.text());
}



INLINE Name const &	Name::operator= (Name_Const const & that) {
	if (that.text() != _text) {
		if (_length != 0) {
			FREE_ARRAY(_text, _length+1, char);
		}
		if (that._length == 0) {
			_length = 0;
			_text = null;
			_key = 0;
			_lower_sum = 0;
		}
		else {
			_length = that._length;
			ALLOC_ARRAY(_text, _length+1, char);
			COPY_ARRAY(that._text, _length, char, _text);
			_text[_length] = '\0';
			_key = that._key;
			_lower_sum = that._lower_sum;
		}
	}

	return *this;
}

INLINE Name const &	Name::operator= (Name const & that) {
	return *this = Name_Const(that);
}

INLINE Name const &	Name::operator= (const char * in) {
	return *this = Name_Const(in);
}

INLINE Name & Name::operator=(Name && other) {

	if (other.text() != _text) {
		if (_length != 0)
			FREE_ARRAY(_text, _length+1, char);

		_text = other._text;
		_length = other._length;
		_key = other._key;
		_lower_sum = other._lower_sum;
		other._text = 0;
		other._length = 0;
		other._key = 0;
		other._lower_sum = 0;
	}
	return *this;
}

INLINE bool Name::operator == (Name const & that) const {
	return (_length == that._length) &&
			((_length == 0) || (hash_key() == that.hash_key() && EQUAL_MEMORY (_text, _length, that._text)));
}

INLINE bool Name::operator == (Name_Const const & that) const {
	return (_length == that._length) &&
			((_length == 0) || (hash_key() == that.hash_key() && EQUAL_MEMORY (_text, _length, that._text)));
}

INLINE bool Name::operator != (Name const & that) const {return !(that == *this);}
INLINE bool Name::operator != (Name_Const const & that) const {return !(that == *this);}

INLINE bool Name_Const::operator == (Name_Const const & that) const {
	return (_length == that._length) &&
		((_length == 0) || (hash_key() == that.hash_key() && EQUAL_MEMORY (_text, _length, that._text)));
}


INLINE int32_t Name::lower_sum() const {
	if (_lower_sum == 0) {
		char const *	cp = text();
		for (size_t i=0; i<length(); i++)
			_lower_sum += HM_TOLOWER (*cp++);
	}
	return _lower_sum;
}

INLINE int32_t Name::hash_key() const {
	if (_key == 0) {
		/* sdbm */
		for (size_t i=0; i<_length; i++)
			_key = _text[i] + (_key << 6) + (_key << 16) - _key;
	}
	return _key;
}

INLINE int32_t Name_Const::lower_sum() const {
	if (_lower_sum == 0) {
		char const *	cp = text();
		for (size_t i=0; i<length(); i++)
			_lower_sum += HM_TOLOWER (*cp++);
	}
	return _lower_sum;
}

INLINE int32_t Name_Const::hash_key() const {
	if (_key == 0) {
		/* sdbm */
		for (size_t i=0; i<_length; i++)
			_key = _text[i] + (_key << 6) + (_key << 16) - _key;
	}
	return _key;
}

INLINE bool Name::CI_Equal(Name const & that) const {
	if (lower_sum() != that.lower_sum())
		return false;
	return CI_EQUAL_STRINGS (text(), that.text());
}

INLINE bool Name::CI_Equal(Name_Const const & that) const {
	if (lower_sum() != that.lower_sum())
		return false;
	return CI_EQUAL_STRINGS (text(), that.text());
}

INLINE bool Name_Const::CI_Equal(Name_Const const & that) const {
	if (lower_sum() != that.lower_sum())
		return false;
	return CI_EQUAL_STRINGS (text(), that.text());
}

INLINE int INDEX_OF_HIGHBIT (unsigned int input) {
	if (input & 0xffff0000) {
		if (input & 0xff000000)
			return 24 + HOOPS::READ_ONLY->index_of_highbit[input>>24];
		else
			return 16 + HOOPS::READ_ONLY->index_of_highbit[input>>16];
	}
	else {
		if (input & 0xff00)
			return 8 + HOOPS::READ_ONLY->index_of_highbit[input>>8];
		else
			return HOOPS::READ_ONLY->index_of_highbit[input];
	}
}


#include "task_queue.h"

Begin_HOOPS_Namespace


INLINE void set_float_ones(size_t count, float * ws) {
	float const *	ws_end = ws + count;
	int batch = 64;

	do {
		if (ws_end - ws < batch)
			batch = (int)(ws_end - ws);
		COPY_ARRAY (HOOPS::READ_ONLY->sixty_four_ones, (int)batch, float, ws);
		ws += batch;
	} while (ws < ws_end);
}


namespace Direction {
	enum {
		MATCH=-1,
		LEFT=0,
		RIGHT=1,
		BOTTOM=2,
		TOP=3,
		HITHER=4,
		YON=5
	};
}


struct Optimize_Control {
	Optimize_Control(Thread_Data * _thread_data, char const * who);

	~Optimize_Control();

	Thread_Data *			thread_data;
	Memory_Pool *			memory_pool;
	Mimic_Info_Hash *		mimic_info;
	Destructible<Item_Seen_Set>::unique_ptr	seen_items;

	int		flags;
	int		id;
	int		conversion;	// precision

	int		geometry_count_split_limit;
	int		segment_count_split_limit;
	int		point_count_split_limit;

	int		instancing_point_count_min_cutoff;

	int		unroll_include_point_total_cutoff;
	int		unroll_include_point_min_cutoff;

	int		condense_seg_count_cutoff;
	int		way_too_many_attributes;

	// user specified organization ordering, if any
	int		organization_phases;
	int		organization[64];
	char	tmpname[4096];
};

#define Optimize_EXPAND_INCLUDES			0x00000001
#define Optimize_COLLAPSE_MATRICES			0x00000002
#define Optimize_REORGANIZE					0x00000004
#define Optimize_UNNECESSARY_ATTRIBUTES		0x00000008
#define Optimize_MERGE_SHELLS				0x00000010
#define Optimize_GENERATE_MAPPING			0x00000020
#define Optimize_LOCALIZE_USER_DATA			0x00000040
#define Optimize_DISCARD_USER_DATA			0x00000080
#define Optimize_PROCESS_INCLUDES			0x00000100
#define Optimize_INSTANCE_SHELLS			0x00000200
#define Optimize_REMOVE_DUPLICATE_SHELLS	0x00000400
#define Optimize_PRESERVE_USER_DATA			0x00000800
#define Optimize_DISCARD_INVISIBLE			0x00001000
#define Optimize_SPLIT_DEFER_REFS			0x00002000
#define Optimize_MODEL_TYPE_LMV				0x00004000
#define Optimize_EXTRACT_ATTRIBUTES			0x00008000
#define Optimize_MERGE_USER_DATA			0x00010000
#define Optimize_MSN						0x00020000
#define Optimize_EXPAND_USE_GEO_REFS		0x00040000
#define Optimize_REORGANIZE_ATTRIBUTES		0x00080000
#define Optimize_SEGMENT_NAMES_DISCARD		0x00100000
#define Optimize_SEGMENT_NAMES_RECORD		0x00200000
#define Optimize_SEGMENT_NAMES_SEQUENCE		0x00400000
#define Optimize_LOCALIZE_MATRICES			0x00800000
#define Optimize_CONVERT_PRECISION			0x01000000
#define Optimize_MERGE_CONVERT_MESHES		0x02000000
#define Optimize_INSTANCE_POINT_MIN_CUTOFF	0x04000000
#define Optimize_SEGMENT_NAMES				(Optimize_SEGMENT_NAMES_DISCARD|Optimize_SEGMENT_NAMES_RECORD|Optimize_SEGMENT_NAMES_SEQUENCE)

class Version {
public:
	Version (int major, int minor)
		: major_(major)
		, minor_(minor)
	{}
	static Version const & invalid () {
		static const Version v(-1, -1);
		return v;
	}
	bool operator== (Version const & other) const {
		return major_ == other.major_ && minor_ == other.minor_;
	}
	bool operator< (Version const & other) const {
		if (major_ < other.major_)
			return true;
		if (major_ == other.major_)
			return minor_ < other.minor_;
		return false;
	}
	bool operator!= (Version const & other) const {
		return !(*this == other);
	}
	bool operator<= (Version const & other) const {
		return *this < other || *this == other;
	}
	bool operator>= (Version const & other) const {
		return !(other < *this);
	}
	bool operator> (Version const & other) const {
		return other < *this;
	}
	int major () const {
		return major_;
	}
	int minor () const {
		return minor_;
	}
private:
	int major_;
	int minor_;
};


struct License_Error {
	enum Type {
		No_Error,
		A_License_Already_Exists,
		Could_Not_Decode,
		Expired,
	};
};


struct Product_Error {
	enum Type {
		No_Error,
		No_License_Set,
		Product_Not_Licensed,
		Incompatable_Version,
	};
};


struct Product {
	enum Flags {
		None			= 0,
		Visualize		= (1<<0),
		Visualize_3df	= (1<<1) | Visualize,
		Visualize_Hps	= (1<<2) | Visualize,
		Exchange		= (1<<3),
		Publish			= (1<<4),
	};
};

struct Visualize_Feature {
	enum Flags {
		None		= 0,
		Mobile		= (1<<0),
	};
};





End_HOOPS_Namespace


#endif

