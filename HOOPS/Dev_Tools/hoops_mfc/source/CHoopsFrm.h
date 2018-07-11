// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// CHoopsFrm.h : interface of the CHoopsFrame class, derived from CMDIFrameWnd(MDI)/CFrameWnd(SDI)
// Manages palette creation and sharing per HOOPS MDI requirements

#include "HMFCGlobals.h"

#ifndef hoops_frame
#define hoops_frame

#ifdef HOOPS_MFC_SDI
#define CBaseHoopsFrameWnd CFrameWnd
#else
#define CBaseHoopsFrameWnd CMDIFrameWnd
#endif

class CFullScreenHandler {
public:
    CFullScreenHandler();
    ~CFullScreenHandler();

    void Maximize(CFrameWnd* pFrame, CWnd* pView);
    void Restore(CFrameWnd* pFrame);
    BOOL InFullScreenMode() { return !m_rcRestore.IsRectEmpty(); }
    CSize GetMaxSize(CWnd* pView);

protected:
    CRect m_rcRestore;
	CRect m_MaxSize;
	DWORD m_styleRestore;
	bool m_maximized;
};

//! The CHoopsFrame class provides a HOOPS-specific implementation of the CMDIFrameWnd(MDI)/CFrameWnd(SDI) object.
/*!
	CHoopsFrame encapsulates palette sharing/management when using HOOPS in conjuction with MFC.

	This is the base class from which all custom HOOPS-based MFC/MDI CMDIFrameWnd/CFrameWnd objects should be derived.
*/
class HMFC_API CHoopsFrame : public CBaseHoopsFrameWnd
{
// Attributes

protected:

    CPalette    *m_pPalette; /*!< Pointer to the palette which is shared by all the views */

    BOOL        m_bSharePalette;  /*!< Indicates whether the palette should be shared by all the views */

	/*!
	  Docks the give toolbar to the left-side of the reference toolbar
	  \param Bar	- The toolbar to be docked
	  \param LeftOf - The reference toolbar 
	*/
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);

public:
	CHoopsFrame();

#ifdef HOOPS_MFC_SDI
	DECLARE_DYNCREATE(CHoopsFrame)  //!< macro def for runtime creation
#else
	DECLARE_DYNAMIC(CHoopsFrame);   //!< macro def for runtime creation
#endif


    /*! 
	  Custom constructor
	  \param share Indicates whether a single palette should be shared by all the views
	*/
    CHoopsFrame(BOOL share); 


// Operations
public:
	/*! \return A pointer to the shared palette */
    CPalette    *GetSharedPalette() {return m_pPalette;};

	/*! \return Whether or not the palette is shared */
    BOOL        IsPaletteShared() {return m_bSharePalette;};

	/*! Sets whether the palette should be shared */
    void        SetPaletteShared(BOOL share);


 
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoopsFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);  //!< overloaded, but currently does nothing
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHoopsFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnViewFullScreen();

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;  //!< used by SetPaneText
	CToolBar    m_wndToolBar;    //!< currently unused

// Generated message map functions
protected:
	//{{AFX_MSG(CHoopsFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);  //!< overloaded for custom creation
	afx_msg BOOL OnQueryNewPalette();					//!< custom palette handling 
	afx_msg void OnPaletteChanged(CWnd* pFocusWnd);		//!< custom palette handling 
	afx_msg void OnMove(int x, int y);			//!< overloaded to support hardware-accelerated dual-monitor systems
	afx_msg virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pResult);	//!< overloaded to handle suspend messages
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpmmi);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif
/////////////////////////////////////////////////////////////////////////////
