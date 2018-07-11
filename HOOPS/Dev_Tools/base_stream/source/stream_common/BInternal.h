// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// This file defines some macros that are used internally within base_stream, but which we do
// not want to be visible to base_stream users for fear of name conflicts

#ifndef BSTREAM_INTERNALS
#define BSTREAM_INTERNALS

#include "BStreamMemory.h"

#ifndef null
#   define null         0
#endif
#ifndef nobreak
#   define nobreak      // fall through explicitly, instead of nothing which might be a forgotten "break"
#endif
#ifndef countof
#   define countof(x)   (int)(sizeof(x) / sizeof(x[0]))
#endif
#ifndef forever
#   define forever      while (1)
#endif
#ifndef until
#   define until(x)     while (!(x))
#endif


#define streq(a,b)      (!strcmp(a,b))
#define streqn(a,b,n)   (!strncmp(a,b,n))

#ifdef _MSC_VER	    // disable any totally useless messages
#   pragma warning (disable: 4127 4996)  
#endif

#ifndef POINTER_SIZED_INT
#if defined(WIN64) || defined(_M_X64) || defined(_WIN64)
#   define POINTER_SIZED_INT __int64
#   define POINTER_SIZED_UINT unsigned __int64
#else
#   define POINTER_SIZED_INT long
#   define POINTER_SIZED_UINT unsigned long
#endif
#endif

#define UL2VP(x) (void*)(POINTER_SIZED_UINT)x
#define SL2VP(x) (void*)(POINTER_SIZED_INT)x
#define VP2UL(x) (unsigned HLONG)(POINTER_SIZED_UINT)x
#define VP2SL(x) (HLONG)(POINTER_SIZED_INT)x


#ifndef UNREFERENCED
  #define UNREFERENCED(x) (void)(x)
#endif

#ifndef HPS_CORE_BUILD
#ifdef _DEBUG
	#ifndef _WINDEF_
		extern "C" __declspec(dllimport) void __stdcall OutputDebugStringA (char const * str);
		#define OutputDebugString OutputDebugStringA
	#endif

    #include <stdio.h>
    #include <stdarg.h>
    inline void dprintf (char const * format, ...) {
        char        buffer[1024];
        va_list     arguments;

        va_start (arguments, format);  
        vsprintf (buffer, format, arguments);
        va_end (arguments);  
        #ifdef _MSC_VER
#ifndef _WIN32_WCE
            OutputDebugStringA (buffer);
#endif
        #else
            fprintf (stderr, buffer);
        #endif
    }
#else
    inline void dprintf (char const * format, ...) {
        UNREFERENCED(format);
    }
#endif
#endif


	#include <wchar.h>	// wchar_t and standard wcs functions
	#include <stdlib.h>
	#include <locale.h>
	#include <string.h>

	//// simple copy from ASCII to UTF-16
	inline void wcsstrcpy (wchar_t * dp, char const * sp) {

#ifdef _MSC_VER
		int was_type = _configthreadlocale(0);
		if (was_type != _ENABLE_PER_THREAD_LOCALE)
			_configthreadlocale(_ENABLE_PER_THREAD_LOCALE);
#endif
		char saved_locale[4096] = {""};
		char const * internal_locale_buffer = setlocale(LC_ALL, 0);
		if (internal_locale_buffer != 0)
			strcpy(saved_locale, internal_locale_buffer);
		/* [#13488] Setting locale to "" sets the locale according to the
		 * environment.  Please do not set locale to "C", it is wrong.  The "C"
		 * or "Posix" locale will only work on ascii text. */
		setlocale(LC_ALL, "");

		mbstowcs(dp, sp, strlen(sp) +1);

		setlocale(LC_ALL, saved_locale);
#ifdef _MSC_VER
		if (was_type != _ENABLE_PER_THREAD_LOCALE)
			_configthreadlocale(was_type);
#endif
	}

	// utility wcschr2() that searches for first occurrence of either of 2 chars
	// sort of like min (wcsrchr(str,c1), wcsrchr(str,c2)) but not null unless both are
	inline wchar_t const * wcschr2 (wchar_t const * str, wchar_t const c1, wchar_t const c2) {
		do
			if (*str == c1 || *str == c2)
				return str;
		while (*str++ != 0);
		return null;
	}
	inline wchar_t * wcschr2 (wchar_t * str, wchar_t const c1, wchar_t const c2) {
		do
			if (*str == c1 || *str == c2)
				return str;
		while (*str++ != 0);
		return null;
	}
	// utility wcsrchr2() that searches for last occurrence of either of 2 chars
	// sort of like max (wcsrchr(str,c1), wcsrchr(str,c2))
	inline wchar_t const * wcsrchr2 (wchar_t const * str, wchar_t const c1, wchar_t const c2) {
		wchar_t const * p = null;
		do
			if (*str == c1 || *str == c2)
				p = str;
		while (*str++ != 0);
		return p;
	}
	inline wchar_t * wcsrchr2 (wchar_t * str, wchar_t const c1, wchar_t const c2) {
		wchar_t * p = null;
		do
			if (*str == c1 || *str == c2)
				p = str;
		while (*str++ != 0);
		return p;
	}

#endif  // BSTREAM_INTERNALS
