// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// CHoopsApp.h : interface of the CHoopsApp class, derived from CWinApp
// Adds encapsulation of the main HOOPS database object

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#ifndef hoops_app
#define hoops_app

//#include "resource.h"      
#include "hc.h"
#include "HMFCGlobals.h"

/*! \def WM_DOREALIZE
	\brief user defined message
*/
#define WM_DOREALIZE    (WM_USER + 0)

/*! \def WMF
	\brief define for a 'regular' windows metafile
*/
#define WMF             0

/*! \def EMF
	\brief define for an 'enhanced' windows metafile
*/
#define EMF             1


class HDB;
class CTDriver;

/////////////////////////////////////////////////////////////////////////////
// CHoopsApp:
// See hoops.cpp for the implementation of this class
//

//! The CHoopsApp class provides a HOOPS-specific implementation of the CWinApp object.
/*!
	CHoopsApp contains a pointer to a corresponding HOOPS/MVO HDB object.
 
	This is the base class from which your custom HOOPS-based MFC/MDI CWinApp object should be derived.
*/
class HMFC_API CHoopsApp : public CWinApp
{

protected:
    
	/*! Pointer to the HOOPS/MVO HDB object associated with this instance of the application */
	HDB*	m_pHoopsDB;		
    CTDriver *m_pCTDriver;	//!< Pointer to the CTDriver timer service class

public:

	CHoopsApp();


	/*! \return A pointer to the HOOPS/MVO HDB object associated with this instance of the application */
	HDB*	GetHoopsDB() { return m_pHoopsDB; }

	/*! \return A pointer to the CTDriver object associated with this instance of the application */
	CTDriver* GetCTDriver() {return m_pCTDriver; }

   
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoopsApp)
	public:
	virtual BOOL InitInstance();  //!< overloaded init function
	virtual int ExitInstance();   //!< overloaded exit function
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHoopsApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif


/////////////////////////////////////////////////////////////////////////////
