// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#pragma once

#define WINVER          0x0501
#define _WIN32_WINNT    0x0501
#define _ATL_APARTMENT_THREADED

// ATL
#include <atlbase.h>
extern CComModule _Module;
#include <atlcom.h>
#include <atlconv.h>

// Win32
#include <comdef.h>
#include <shlobj.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
