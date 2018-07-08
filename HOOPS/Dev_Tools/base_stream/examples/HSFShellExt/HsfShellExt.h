
#pragma once

#include "ShellExt.h"

class BStreamFileToolkit;
class TK_My_Thumbnail;

class ATL_NO_VTABLE CHsfShellExt :
    public CComObjectRootEx<CComSingleThreadModel>,
    public CComCoClass<CHsfShellExt, &CLSID_HsfShellExt>,
    public IPersistFile,
	public IExtractImage
{
public:
    CHsfShellExt() : m_tk(0), m_thumbnail(0) { }
	~CHsfShellExt();

    BEGIN_COM_MAP(CHsfShellExt)
        COM_INTERFACE_ENTRY(IPersistFile)
		COM_INTERFACE_ENTRY(IExtractImage)
    END_COM_MAP()

    DECLARE_REGISTRY_RESOURCEID(IDR_HSFICONSHLEXT)

// Begin IPersistFile Interface Methods
    STDMETHODIMP GetClassID( CLSID* ) { return E_NOTIMPL; }
    STDMETHODIMP Save( LPCOLESTR, BOOL ) { return E_NOTIMPL; }
    STDMETHODIMP SaveCompleted( LPCOLESTR ) { return E_NOTIMPL; }
    STDMETHODIMP GetCurFile( LPOLESTR* ) { return E_NOTIMPL; }
    STDMETHODIMP IsDirty();
    STDMETHODIMP Load( LPCOLESTR wszFile, DWORD );
// End IPersistFile Interface Methods


// Begin IExtractImage Interface Methods
	STDMETHODIMP GetLocation(
		LPWSTR pszPathBuffer,
		DWORD cchMax,
		DWORD *pdwPriority,
		const SIZE *prgSize,
		DWORD dwRecClrDepth,
		DWORD *pdwFlags
	);
	STDMETHODIMP Extract( HBITMAP * phBmpImage );
// End IExtractImage Interface Methods



protected:
	bool ExtractImageFromHSF(TCHAR const * filename);
	STDMETHODIMP ExtractThumbnail(TCHAR const * filename, HBITMAP* phicon);

	TCHAR     m_szFilename[MAX_PATH];   // Full path to the file in question.

	BStreamFileToolkit * m_tk;
	TK_My_Thumbnail * m_thumbnail;
	SIZE m_requestedSize;
};

