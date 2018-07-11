// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// CHoopsView.h : interface of the CHoopsView class, derived from CView
// Adds encapsulation of a HOOPS View Object, which corresponds to an
// instance of a HOOPS driver
//
// Also provides high-level support for copying the view to the clipboard, 
// printing and printpreview

#ifndef _C_HOOPS_VIEW_
#define _C_HOOPS_VIEW_


#include "HBaseView.h"
#include "HMFCGlobals.h"

class CHoopsDoc;
class GDIExportInformation;

// msw driver specific driver_option debug flags

/*!
    \def DEBUG_FORCE_FULL_COLOR
  Causes HOOPS/3dgs msw driver to output commands as if the device is full color
  Dithering will not occur - device will appear to have no palette and
  16777216 colors - this will look very bad on a VGA device and is
  mainly intended for metafiles for printing and sharing
*/
#define DEBUG_FORCE_FULL_COLOR				0x00000010

/*!
    \def DEBUG_NO_WINDOWS_HOOK
    Obselete.  
*/
#define DEBUG_NO_WINDOWS_HOOK				0x00000040 

/*!
    \def DEBUG_SFB_COPY_TO_CLIPBOARD
   Tells HOOPS/3dGS' msw driver that sfb is active and we are copying to a wmf.  
   Needed so that we know to limit the sfb image size and stretchblit
*/
#define DEBUG_SFB_COPY_TO_CLIPBOARD			0x00004000

/*!
    \def DEBUG_STARTUP_CLEAR_BLACK
   Tells HOOPS/3dGS' opengl driver to clear using black (instead of white) for
   the first update.
*/
#define DEBUG_STARTUP_CLEAR_BLACK			0x00004000

/*!
    \def DEBUG_NO_RASTERS_IN_PRINTING
    Obsolete
*/
#define DEBUG_NO_RASTERS_IN_PRINTING		0x00020000

/*!
    \def DEBUG_PRINTING_MODE
    Tells HOOPS/3dGS msw driver to enter printing mode  
*/
#define DEBUG_PRINTING_MODE					0x00040000

/*!
    \def DEBUG_CLIPBOARD_MODE
    Tells HOOPS/3dGS msw driver to enter clipboard mode  
*/
#define DEBUG_CLIPBOARD_MODE				0x00080000


/*!
    \def DEBUG_PRINT_NOT_FRAMEBUFFER_MODE
    Tell HOOPS/3dGS msw driver to use a single stretch blt to copy a software 
	framebuffer to the printer device 
*/
#define DEBUG_PRINT_NOT_FRAMEBUFFER_MODE	0x00020000


/*!
    \def DEBUG_PRINT_NOT_RASTER_REDUCTION
    Obsolete
*/
#define DEBUG_PRINT_NOT_RASTER_REDUCTION  	0x00200000

/*!
    \def DEBUG_FORCE_SOFTWARE
    Tells HOOPS/3dGS opengl driver to force software z-buffer
*/
#define DEBUG_FORCE_SOFTWARE				0x01000000

/*!
    \def DEBUG_NO_PAINTERS_PANELLING
    Used to tell HOOPS/3dGS not to perform panelling when using painter's algorithm
	(This setting is made be calling HOOPS/3dGS HC_Set_Rendering_Options subroutine
*/
#define DEBUG_NO_PAINTERS_PANELLING			0x00000008

/*!
    \def DEBUG_16BIT_ZBUFFER
    OpenGL specific debug flag forces 16-bit z-buffer
*/
#define DEBUG_16BIT_ZBUFFER					0x00200000


/*! \def WMF
	\brief define for a 'regular' windows metafile
*/
#define WMF             0

/*! \def EMF
	\brief define for an 'enhanced' windows metafile
*/
#define EMF             1


class DIDeviceInfo;
class DIDevice;

//! The CHoopsView class provides a HOOPS-specific implementation of the CView object.
/*!
	CHoopsView creates and manages the connection of a HOOPS/3dGS driver instance to a CView object, which includes
	properly updating the window during exposes/resizes.  It contains a pointer to a corresponding HOOPS/MVO HBaseView object.

	It includes built-in support for printing, print preview, and copying to cliboard of the HOOPS/3dGS scene, 
	and encapsulates palette management if HOOPS/3dGS is being used on 8-bit system with the MSW Driver. 

	This is the base class from which all custom HOOPS-based MFC/MDI CView objects should be derived.
*/
class HMFC_API CHoopsView : public CView
{

protected: // create from serialization only
	CHoopsView();

	DECLARE_DYNCREATE(CHoopsView);   //!< implements runtime object creation


    CPalette *		m_pViewPalette;  	/*!< Pointer to this view's unique palette. This will be null if we're in 24-bit mode, or if palettes are shared among all views. */

	/*! 
		If true, the scene is rendered to an offscreen bitmap and stretch-blitted to the printer or Windows metafile 
		during printing or copy to clipboard. The default is true.
	*/
    bool			m_bFastPrint;   

	/*!
		If true, we have initialized and are using a 3DMouse or Joystick using DirectInput
	*/
	bool			m_bUsingDI8Joystick;
	DIDeviceInfo *	di8_data;

    /*! 
		If true, the scene is rendered to a windows metafile and then played to the 
		printer. The default is false.
	*/
	bool			m_bMetaPrint;      
	
    /*! If true, the scene will render to the clipboard in truecolor mode. The default is true.*/
	bool			m_bClipboardTruecolor;

	/*! Type of metafile generated when m_bMetaPrint==true, 0=WMF amd 1=EMF.*/
    int				m_MetafileType;

	/*! Pointer to HBaseView MVO object associated with this CView object.  */
	HBaseView *		m_pHView;		

	/*! 
	  The key of the HOOPS driver instance segment associated with this view
	  Intended to be used as an ALTERNATIVE to MVO (and the HBaseView object referenced by m_pHView)
	*/
	HC_KEY			m_ViewKey;	
	
	/*! 
	  The key of the HOOPS segment representing the top of the scene hierarchy, typically a subsegment of the driver instance 
	  segment (m_ViewKey)   Intended to be used in conjuction with m_view_key as an ALTERNATIVE to MVO (and the HBaseView 
	  object referenced by m_pHView)
	*/
	HC_KEY			m_SceneKey;	

	/*! Maps the MFC button and Shift/Control state to abstracted HOOPS/MVO values */
	unsigned long MapFlags( unsigned long state ); 	
	
	/*! Maps the MFC key state to abstracted HOOPS/MVO values */
	unsigned long MapKeyFlags( BYTE * virtual_key_state ); 						

public:


	HRESULT UpdateDI8State( unsigned long devIndex );
	HRESULT CHoopsView::InitDI8( void );

	DIDevice *		dev_handle;
	DIDeviceInfo *	g_pDevice;
	int				g_nDevices;

	/*! 
		\return A Pointer to the CHoopsDoc object that being viewed by this CHoopsView object
	*/
	CHoopsDoc *	GetDocument();

	/*! \return The pointer to the HBaseView object associated with this CView object. */
	HBaseView *	GetHoopsView( void ) { return m_pHView; }

	/*! 
		\return The palette used for this view.  This will be null if we are in 24-bit mode.  If the palette is being shared 
				among all views, this will return the shared palette, otherwise it will return this view's unique palette
	*/
    CPalette *	GetPalette();


	/*! 
		Initialize this view's unique palette (meaning that we aren't sharing a palette among
		all views, which should be quite rare)  
	*/
    void        InitializePalette();

	// printing and metafile methods

	/*! 
		\return A Boolean indicating whether native GDI printing or a fast bitmap blit will be used for printing.
				The default is true.
	*/
    bool                GetFastPrint (void) {return m_bFastPrint;}

	/*! Sets whether native GDI printing or a fast bitmap blit will be used for printing. The default is true.*/
    void                SetFastPrint (bool fastprint) {m_bFastPrint = fastprint;}

	/*! 
		\return A Boolean indicating whether a Windows Enhanced Metafile will first be created and then played to 
		the printer when CHoopsView::OnPrint() is called. The default is false.
	*/
	bool                GetMetaPrint (void) {return m_bMetaPrint;}

	/*! Sets whether a Windows Enhanced Metafile containing the scene will first be created and then played to 
		the printer when CHoopsView::OnPrint() is called (rather than the scene going straight to the printer). The default is false.
	*/
    void                SetMetaPrint (bool metaprint) {m_bMetaPrint = metaprint;}

	/*! \return An integer indicating whether an emf or wmf will be for clipboard rendering.  The default is EMF. */
    int			        GetMetafileType (void) { return m_MetafileType; }

	/*! Sets whether an emf or wmf will be used for clipboard rendering.  The default is EMF. */
    void		        SetMetafileType (int emf_or_wmf) { m_MetafileType = emf_or_wmf; }

	/*! \return A Boolean indicating whether the first update has occurred. */
    bool				GetFirstUpdate(){return m_pHView->GetFirstUpdate();}

	/*! Sets whether the first view update has occurred.*/
    void				SetFirstUpdate(bool firstupdate){ UNREFERENCED(firstupdate); }

	/*! \return A Boolean indicating whether copying to clipboard uses mapped or true color. The default is true. */
	bool		        GetClipboardTruecolor (void) { return m_bClipboardTruecolor; }

	/*! Sets whether copying to clipboard uses mapped or true color.  The default is true. */
    void		        SetClipboardTruecolor (bool truecolor) { m_bClipboardTruecolor = truecolor; }


	void		SaveEMF(const char *filename, HOutputHandlerOptions * options = 0);  //!< saves scene to enhanced metafile


	/*! \return The key of the HOOPS segment denoting the top of the view hierarchy; usually the driver instance segment */
	HC_KEY		GetViewKey();

	/*! \return The key of the HOOPS segment denoting the top of the scene hierarchy, usually a subsegment of the view 
				segment key, and the location where the camera is set */
	HC_KEY		GetSceneKey();

	/*! Adjusts the relative size of the Axis window so that it always has the same size if the outer window is resized */
	void		AdjustAxisWindow();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoopsView)
	public:
	virtual void OnDraw(CDC* pDC);  //!< overridden but currently does nothing

	/*! We overload the PreCreateWindow method to modify the CWnd window class; necessary for OpenGL support */
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	/*! 
		Performs some HOOPS/MFC specific initialization: if HOOPS/MFC is being used in palette mode and each view has a unique
		palette, the palette is initialized.
	*/
	virtual void OnInitialUpdate();

	/*! We overload the Create method to modify the CWnd window style; necessary for OpenGL support */
#ifdef _UNICODE
	virtual BOOL Create(__wchar_t const * lpszClassName, __wchar_t const * lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual BOOL Create(unsigned short const * lpszClassName, unsigned short const *  lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
#else
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
#endif
	protected:

	//! overloaded but currently does nothing
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);

	//! overloaded but currently does nothing
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);

	//! overloaded but currently does nothing
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	/*!
		Prints the scene, or performs a print preview, according to the tye of Windows DC.  If m_bFastPrint is true, the scene will be 
		rendered to an offscreen bitmap and stretch-blitted to the printer. If false, the scene will be decomposed 
		into native GDI information (2D vector and/or raster data). 

		If m_bFastPrint is true, it might be desirable to set m_bMetaPrint to true as well. In some cases, text
		and other objects may not print correctly without turning on both of these switches.  See the section on
		GDI Driver Printing in the HOOPS/3dGS Platform and Device Guide for more details.
	*/ 
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);


#ifndef DOXYGEN_SHOULD_SKIP_THIS

	virtual void OnNcDestroy();

	virtual void PostNcDestroy();

#endif // DOXYGEN_SHOULD_SKIP_THIS


	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHoopsView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
		
	/*! 
		Called when display change happens.  This can be used to adjust when
		screen orientation changes or resolution changes. 
	*/
	virtual void OnDisplayChanged(int xres, int yres, int depth);
	/*! helper to get the current ClientRect into a platform-independent structure */
	void GetIntRectangle( HIntRectangle *rectangle );

	/*! does the actual work for copy to clipboard. */
	void EditCopy(GDIExportInformation * pInfo);


// Generated message map functions
protected:
	//{{AFX_MSG(CHoopsView)

	/*!  Overloaded CView method.  Instructs HOOPS to traverse the scene-graph and update the display */
	afx_msg virtual void OnPaint();

	afx_msg virtual void OnSize(UINT, int, int);

	/*!
		Renders the scene to a Windows Metafile (emf or wmf) and places it on the clipboard.  The type of 
		metafile generated is accessed through GetMetafileType and SetMetafileType.
	*/ 
	afx_msg virtual void OnEditCopy();

	/*! 
		Calls the OnKeyDown method of the view's current HOOPS/MVO HBaseOperator object.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	/*! 
		Calls the OnKeyUp method of the view's current HOOPS/MVO HBaseOperator object.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);

	/*! 
		Dispatches the OnLButtonDblClk event to all event listeners that are registered for the OnLButtonDblClk event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnLButtonDblClk(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnLButtonDown event to all event listeners that are registered for the OnLButtonDown event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnLButtonDown(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnLButtonUp event to all event listeners that are registered for the OnLButtonUp event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnLButtonUp(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnMouseMove event to all event listeners that are registered for the OnMouseMove event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnMouseMove(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnRButtonDblClk event to all event listeners that are registered for the OnRButtonDblClk event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnRButtonDblClk(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnRButtonDown event to all event listeners that are registered for the OnRButtonDown event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnRButtonDown(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnMButtonUp event to all event listeners that are registered for the OnMButtonUp event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnMButtonUp(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnMButtonDown event to all event listeners that are registered for the OnMButtonDown event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnMButtonDown(UINT nFlags, CPoint point);

	/*! 
		Dispatches the OnRButtonUp event to all event listeners that are registered for the OnRButtonUp event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual void OnRButtonUp(UINT nFlags, CPoint point);

	/*! \return Nonzero to halt further processing, or 0 to continue
				The default implementation sets the cursor to the standard cursor (IDC_ARROW)
				If you want your own cursor, override the function and do not call the base class.
	*/
	afx_msg virtual BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	/*! 
		Calls the OnTimer method of the view's current HOOPS/MVO HBaseOperator object.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
#if (_MSC_VER < 1400)
	afx_msg virtual void OnTimer(UINT nIDEvent);
#else
	afx_msg virtual void OnTimer(UINT_PTR nIDEvent);
#endif


	/*! 
		Callback for retrieving low-level key-presses
		\param key ASCII Value of key to query state for
		\param flags Status of Modifier keys (MVO_LEFT_SHIFT, MVO_RIGHT_SHIFT, MVO_SHIFT, MVO_CONTROL)
		\return True if specified key is pressed
	*/
	static bool GetKeyState(unsigned int key, int &flags);

 
	/*! 
		Dispatches the OnMouseWheel event to all event listeners that are registered for the OnMouseWheel event type.  The GUI specific flags are
		mapped to HOOPS/MVO abstracted flags use the CHoopsView::MapFlags method 
	*/
	afx_msg virtual BOOL OnMouseWheel( UINT nFlags, short zDelta, CPoint pt );

	/*! 
		Called when WM_DISPLAYCHANGE happens.  Calls OnDisplayChange();
	*/
	afx_msg virtual LRESULT OnAFXDisplayChange( WPARAM wparam, LPARAM lparam);

	
	/*!
		Called when WM_POWERBROADCAST is sent by CHoopsFrame, from a "resume suspend".  Calls HC_Control_Update
		to "reset device".
	*/
	afx_msg virtual BOOL OnWndMsg( UINT message, WPARAM wParam, LPARAM lParam, LRESULT *pResult);


	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  
inline CHoopsDoc* CHoopsView::GetDocument()
   { return (CHoopsDoc*)m_pDocument; }
#endif

#endif

/////////////////////////////////////////////////////////////////////////////
