// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

// CHoopsDoc.h : interface of the CHoopsDoc class, derived from CDocument
// Adds encapsulation of a HOOPS Model Object, which corresponds to a segment
// in the HOOPS Include Library

#ifndef hoops_doc
#define hoops_doc 


#include "HBaseModel.h"
#include "HMFCGlobals.h"

//! The CHoopsDoc class provides a HOOPS-specific implementation of the CDocument object.
/*!
	CHoopsDoc contains a pointer to a corresponding HOOPS/MVO HBaseModel object.
 
	This is the base class from which all custom HOOPS-based MFC/MDI CDocument objects should be derived.
*/
class HMFC_API CHoopsDoc : public CDocument
{
protected: // create from serialization only
	CHoopsDoc();
	DECLARE_DYNCREATE(CHoopsDoc);  //!< macro def for runtime creation

    
	HBaseModel*		m_pHoopsModel;  /*!< Pointer to the HOOPS/MVO HBaseModel object associated with this document */	
								

	/*! Key of the HOOPS Include Library segment associated with this document, intended to be used as an ALTERNATIVE 
	    to a HOOPS/MVO HBaseModel object (m_pHoopsModel) */
	HC_KEY			m_include_key;  

// Operations
public:

	/*! \return A pointer to the HBaseModel object associated with this Document */
	HBaseModel* GetHoopsModel( void ) { return m_pHoopsModel; }

	/*! \return The key of the HOOPS Include Library segment associated with this document, used as an alternative
	            to the HOOPS/MVO HBaseModel object */
	HC_KEY		GetIncludeKey( void) { return m_include_key; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoopsDoc)
	public:
	virtual BOOL OnNewDocument();    //!< overloaded, but does nothing (app-wizard generator overloaded it and we left it that way)
	virtual void Serialize(CArchive& ar);  //!< overloaded, but does nothing (app-wizard generator overloaded it and we left it that way)
#ifdef _UNICODE
	virtual BOOL OnOpenDocument(__wchar_t const * lpszPathName);  //!< overloaded, but does nothing (app-wizard generator overloaded it and we left it that way)
#ifdef H_USHORT_OVERLOAD
	virtual BOOL OnOpenDocument(unsigned short const * lpszPathName);  //!< overloaded, but does nothing (app-wizard generator overloaded it and we left it that way)
#endif
#else
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);  //!< overloaded, but does nothing (app-wizard generator overloaded it and we left it that way)
#endif
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CHoopsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CHoopsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif

/////////////////////////////////////////////////////////////////////////////
