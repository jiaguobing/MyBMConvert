// -------------------------------------------------------------------------------------------------------------------
// 1. Edit or copy and edit FILE _VSxxxx_HOOPS_yyyy.bat':
// 		- set HOOPS_VER=
// 		- set HOOPS_COMP=
// 		- set HOOPS_INSTALL_DIR=
// 		- set VS_EXE=
// 		- set SLN_NAME=
// 2. compile solution
// 3. copy correct .lib file from directory "lib\HOOPS_VER\..." into to your solution infrastructure
// 4. copy correct SegmentBrowserDll.dll file from directory "bin\HOOPS_VER\..." into to your application directory
// 
// Note
//	- The project settings assumes that HOOPS is installed using (default) directory structure provided by TechSoft
//	 
// -------------------------------------------------------------------------------------------------------------------
 

//
// To call the dialog in the application:
// 
// #ifdef _DEBUG
// 	#pragma comment(lib, "SegmentBrowserDlld.lib")
// #else
// 	#pragma comment(lib, "SegmentBrowserDll.lib")
// #endif
// 
// 
// #include "SegmentBrowserDll.h"
// 
// 
// 
// void SomeClass::SomeMethod() 
// {
//		// m_pHView: instance of HBaseView
//		// this:	 instance of CView
// 		SegmentBrowserDlg(m_pHView, this);
// }

//
// Exported function
//
_declspec(dllimport) void SegmentBrowserDlg(HBaseView *pHView, CView *pMFCView = NULL);