// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef HOOPS_WINDOWS_WRAPPER

#ifndef NUTCRACKER

#ifdef _MSC_VER		// only on windows
#	ifdef _WINDEF_	// but too late if windows.h has already been included
#		error	"windows_wrapper.h  needs to be included before other headers"
#	endif

#	define _WIN32_WINNT 0x05010300
#	define PSAPI_VERSION 1

#	include <wchar.h>

// Windows.h pollutes the global namespace

#define Ellipse INACCESSIBLE_Ellipse
#define Polyline INACCESSIBLE_Polyline
#define Polygon INACCESSIBLE_Polygon
#define Rectangle INACCESSIBLE_Rectangle


#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#	include <windows.h>

#	ifndef __in		// apparently new since VC6
#		define __in
#	endif

// now clean up and define the wrapper functions we provide which will redirect to the real ones

#undef	Ellipse
#undef	Polyline
#undef	Polygon
#undef	Rectangle

#	undef max
#	undef min

#	undef	RGB
#	define	GDI_RGB(r,g,b)          ((COLORREF)(((BYTE)(r)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(b))<<16)))

// these are wrapped up in win32_system.cpp
extern "C" {
	BOOL WINAPI GDI_Polygon(__in HDC hdc, CONST POINT *apt, __in int cpt);
	BOOL WINAPI GDI_Polyline(__in HDC hdc, CONST POINT *apt, __in int cpt);
	BOOL WINAPI GDI_Ellipse( __in HDC hdc, __in int left, __in int top,  __in int right, __in int bottom);
	BOOL WINAPI GDI_Rectangle(__in HDC hdc, __in int left, __in int top, __in int right, __in int bottom);
}

#	include <process.h>


#endif	// _MSC_VER
#endif  // NUTCRACKER

#define HOOPS_WINDOWS_WRAPPER
#endif

