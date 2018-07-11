
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
#include "HTManager.h"
#include "HMFCGlobals.h"

/*! 
    This class delivers timer ticks which are intended to be used by a platform/GUI independent timer service.
	
	HOOPS/MVO HTManager class provides such a service.
*/
class HMFC_API CTDriver : public CWnd
{
// Construction
public:
	//! custom constructor sets 10ms tick
	CTDriver( int milliseconds = 10 );  
	virtual ~CTDriver();

    void StartTimer();  //!< starts timer
    void KillTimer();   //!< kills timer

	virtual BOOL DestroyWindow();			//!< overloaded method which kills the WM_TIMER
   

protected:
	UINT    m_nTimer;        //!< indicates whether timer was set
    int     m_milliseconds;  //!< amount of time to tick

	int Init();  //!< sets up window

	BOOL PreCreateWindow(CREATESTRUCT& cs);	//!< overloaded method which sets WS_OVERLAPPEDWINDOW style
#if (_MSC_VER < 1400)
	afx_msg void OnTimer(UINT nIDEvent);	//!< timer callback
#else
	afx_msg void OnTimer(UINT_PTR nIDEvent);	//!< timer callback
#endif
	DECLARE_MESSAGE_MAP()

};