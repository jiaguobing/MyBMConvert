// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//-----------------------------------------------------------------------------
// File: D3DUtil.h
//
// Desc: Helper functions and typing shortcuts for Direct3D programming.
//-----------------------------------------------------------------------------
#ifndef HD3DUTIL_H
#define HD3DUTIL_H
#ifdef _DEBUG
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif 
#include <D3D9.h>
#include "hoops.h"
struct DX9Data;

#include <D3Dcompiler.h>

//#define USE_D3DX

#ifdef USE_D3DX
#include <D3DX9Math.h>
#define HD3DXConstantTable ID3DXConstantTable
#else
typedef void * D3DXHANDLE;
typedef Plane D3DXVECTOR4;
typedef D3DMATRIX D3DXMATRIX;

#include "d3dx_constant_table.h"
#endif

typedef struct d3d_system_data 
{
	int						ref_count;				// reference count
	LPDIRECT3D9				pD3D;					// the main D3D object
	LPDIRECT3DDEVICE9		pDevice;				// the main D3D device
	DX9Data *				dx9data_list;			// a list of all dx9datas
	Task_Queue *			device_task_queue;		// task queue for creating/deleting device
	Semaphore				device_task_semaphore; // semaphore for the device task
	void *					cache;					// shared h3dcache
	Mutex					mutex;

	unsigned int			current_vb_offset[15];
	LPDIRECT3DVERTEXBUFFER9 current_vb[15];			// cache the current vertex/index buffer
	LPDIRECT3DINDEXBUFFER9	current_ib;
} D3D_System_Data;


//-----------------------------------------------------------------------------
// name: D3DUtil_InitMaterial()
// Desc: Initializes a D3DMATERIAL9 structure, setting the diffuse and ambient
//				 colors. It does not set emissive or specular colors.
//-----------------------------------------------------------------------------
VOID D3DUtil_InitMaterial(
	D3DMATERIAL9& mtrl, 
	FLOAT r=0.0f, 
	FLOAT g=0.0f,
	FLOAT b=0.0f, 
	FLOAT a=1.0f);


//-----------------------------------------------------------------------------
// File: DXUtil.h
//
// Desc: Helper functions and typing shortcuts for DirectX programming.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Miscellaneous helper functions
//-----------------------------------------------------------------------------
#define H_SAFE_DELETE(p)			 { if (p) { delete (p);	   (p)=null; } }
#define H_SAFE_RELEASE(p)			 { if (p) { (p)->Release(); (p)=null; } }




//-----------------------------------------------------------------------------
// name: ArrayListType
// Desc: Indicates how data should be stored in a CArrayList
//-----------------------------------------------------------------------------
enum ArrayListType
{
		AL_VALUE,				// entry data is copied into the list
		AL_REFERENCE,	// entry pointers are copied into the list
};


//-----------------------------------------------------------------------------
// name: CArrayList
// Desc: A growable array
//-----------------------------------------------------------------------------
class CArrayList
{
protected:
		ArrayListType m_ArrayListType;
		void* m_pData;
		UINT m_BytesPerEntry;
		UINT m_NumEntries;
		UINT m_NumEntriesAllocated;

public:
		CArrayList(ArrayListType Type, UINT BytesPerEntry = 0);
		~CArrayList(void);
		HRESULT Add(void* pEntry);
		void Remove(UINT Entry);
		void* GetPtr(UINT Entry);
		UINT Count(void) { return m_NumEntries; }
		bool Contains(void* pEntryData);
		void Clear(void) { m_NumEntries = 0; }
};


//-----------------------------------------------------------------------------
// File: D3DEnumeration.h
//
// Desc: Enumerates D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------

extern UINT ColorChannelBits(D3DFORMAT fmt);
extern UINT AlphaChannelBits(D3DFORMAT fmt);
extern UINT DepthBits(D3DFORMAT fmt);
extern UINT StencilBits(D3DFORMAT fmt);

//-----------------------------------------------------------------------------
// name: enum VertexProcessingType
// Desc: Enumeration of all possible D3D vertex processing types.
//-----------------------------------------------------------------------------
enum VertexProcessingType
{
		SOFTWARE_VP,
		MIXED_VP,
		HARDWARE_VP,
		PURE_HARDWARE_VP
};


//-----------------------------------------------------------------------------
// name: struct D3DAdapterInfo
// Desc: Info about a display adapter.
//-----------------------------------------------------------------------------
struct D3DAdapterInfo
{
		int AdapterOrdinal;
		D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
		CArrayList* pDisplayModeList; // List of D3DDISPLAYMODEs
		CArrayList* pDeviceInfoList; // List of D3DDeviceInfo pointers
		~D3DAdapterInfo(void);
};


//-----------------------------------------------------------------------------
// name: struct D3DDeviceInfo
// Desc: Info about a D3D device, including a list of D3DDeviceCombos (see below) 
//				 that work with the device.
//-----------------------------------------------------------------------------
struct D3DDeviceInfo
{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DCAPS9 Caps;
		CArrayList* pDeviceComboList; // List of D3DDeviceCombo pointers
		~D3DDeviceInfo(void);
};


//-----------------------------------------------------------------------------
// name: struct D3DDSMSConflict
// Desc: A depth/stencil buffer format that is incompatible with a
//				 multisample type.
//-----------------------------------------------------------------------------
struct D3DDSMSConflict
{
		D3DFORMAT DSFormat;
		D3DMULTISAMPLE_TYPE MSType;
};


//-----------------------------------------------------------------------------
// name: struct D3DDeviceCombo
// Desc: A combination of adapter format, back buffer format, and windowed/fullscreen 
//				 that is compatible with a particular D3D device (and the app).
//-----------------------------------------------------------------------------
struct D3DDeviceCombo
{
		int AdapterOrdinal;
		D3DDEVTYPE DevType;
		D3DFORMAT AdapterFormat;
		D3DFORMAT BackBufferFormat;
		bool IsWindowed;
		CArrayList* pDepthStencilFormatList; // List of D3DFORMATs
		CArrayList* pMultiSampleTypeList; // List of D3DMULTISAMPLE_TYPEs
		CArrayList* pMultiSampleQualityList; // List of DWORDs (number of quality 
																				 // levels for each multisample type)
		CArrayList* pDSMSConflictList; // List of D3DDSMSConflicts
		CArrayList* pVertexProcessingTypeList; // List of VertexProcessingTypes
		CArrayList* pPresentIntervalList; // List of D3DPRESENT_INTERVALs

		~D3DDeviceCombo(void);
};


typedef bool(* CONFIRMDEVICECALLBACK)(D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
																				   D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat);



//-----------------------------------------------------------------------------
// name: class HD3DEnumeration
// Desc: Enumerates available D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------
class HD3DEnumeration
{
private:
		IDirect3D9* m_pD3D;

private:
		HRESULT EnumerateDevices(D3DAdapterInfo* pAdapterInfo, CArrayList* pAdapterFormatList);
		HRESULT EnumerateDeviceCombos(D3DDeviceInfo* pDeviceInfo, CArrayList* pAdapterFormatList);
		void BuildDepthStencilFormatList(D3DDeviceCombo* pDeviceCombo);
		void BuildMultiSampleTypeList(D3DDeviceCombo* pDeviceCombo);
		void BuildDSMSConflictList(D3DDeviceCombo* pDeviceCombo);
		void BuildVertexProcessingTypeList(D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo);
		void BuildPresentIntervalList(D3DDeviceInfo* pDeviceInfo, D3DDeviceCombo* pDeviceCombo);

public:
		CArrayList* m_pAdapterInfoList;
		// The following variables can be used to limit what modes, formats, 
		// etc. are enumerated.	 Set them to the values you want before calling
		// Enumerate().
		CONFIRMDEVICECALLBACK ConfirmDeviceCallback;
		UINT AppMinFullscreenWidth;
		UINT AppMinFullscreenHeight;
		UINT AppMinColorChannelBits; // min color bits per channel in adapter format
		UINT AppMinAlphaChannelBits; // min alpha bits per pixel in back buffer format
		UINT AppMinDepthBits;
		UINT AppMinStencilBits;
		bool AppUsesDepthBuffer;
		bool AppUsesMixedVP; // whether app can take advantage of mixed vp mode
		bool AppRequiresWindowed;
		bool AppRequiresFullscreen;
		CArrayList* m_pAllowedAdapterFormatList; // list of D3DFORMATs

		HD3DEnumeration();
		~HD3DEnumeration();
		void SetD3D(IDirect3D9* pD3D) { m_pD3D = pD3D; }
		HRESULT Enumerate();
};

bool confirm_device_helper(
	D3DCAPS9* pCaps, 
	VertexProcessingType vertexProcessingType,
	D3DFORMAT adapterFormat, 
	D3DFORMAT backBufferFormat);

//-----------------------------------------------------------------------------
// File: D3DSettings.h
//
// Desc: Settings class and change-settings dialog class for the Direct3D 
//				 samples framework library.
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// name: class HD3DSettings
// Desc: Current D3D settings: adapter, device, mode, formats, etc.
//-----------------------------------------------------------------------------
class HD3DSettings 
{
public:
		bool IsWindowed;

		D3DAdapterInfo* pWindowed_AdapterInfo;
		D3DDeviceInfo* pWindowed_DeviceInfo;
		D3DDeviceCombo* pWindowed_DeviceCombo;

		D3DDISPLAYMODE Windowed_DisplayMode; // not changable by the user
		D3DFORMAT Windowed_DepthStencilBufferFormat;
		D3DMULTISAMPLE_TYPE Windowed_MultisampleType;
		DWORD Windowed_MultisampleQuality;
		VertexProcessingType Windowed_VertexProcessingType;
		UINT Windowed_PresentInterval;
		int Windowed_Width;
		int Windowed_Height;

		D3DAdapterInfo* pFullscreen_AdapterInfo;
		D3DDeviceInfo* pFullscreen_DeviceInfo;
		D3DDeviceCombo* pFullscreen_DeviceCombo;

		D3DDISPLAYMODE Fullscreen_DisplayMode; // changable by the user
		D3DFORMAT Fullscreen_DepthStencilBufferFormat;
		D3DMULTISAMPLE_TYPE Fullscreen_MultisampleType;
		DWORD Fullscreen_MultisampleQuality;
		VertexProcessingType Fullscreen_VertexProcessingType;
		UINT Fullscreen_PresentInterval;

		D3DAdapterInfo* PAdapterInfo() { return IsWindowed ? pWindowed_AdapterInfo : pFullscreen_AdapterInfo; }
		D3DDeviceInfo* PDeviceInfo() { return IsWindowed ? pWindowed_DeviceInfo : pFullscreen_DeviceInfo; }
		D3DDeviceCombo* PDeviceCombo() { return IsWindowed ? pWindowed_DeviceCombo : pFullscreen_DeviceCombo; }

		int AdapterOrdinal() { return PDeviceCombo()->AdapterOrdinal; }
		D3DDEVTYPE DevType() { return PDeviceCombo()->DevType; }
		D3DFORMAT BackBufferFormat() { return PDeviceCombo()->BackBufferFormat; }

		D3DDISPLAYMODE DisplayMode() { return IsWindowed ? Windowed_DisplayMode : Fullscreen_DisplayMode; }
		void SetDisplayMode(D3DDISPLAYMODE value) { if (IsWindowed) Windowed_DisplayMode = value; else Fullscreen_DisplayMode = value; }

		D3DFORMAT DepthStencilBufferFormat() { return IsWindowed ? Windowed_DepthStencilBufferFormat : Fullscreen_DepthStencilBufferFormat; }
		void SetDepthStencilBufferFormat(D3DFORMAT value) { if (IsWindowed) Windowed_DepthStencilBufferFormat = value; else Fullscreen_DepthStencilBufferFormat = value; }

		D3DMULTISAMPLE_TYPE MultisampleType() { return IsWindowed ? Windowed_MultisampleType : Fullscreen_MultisampleType; }
		void SetMultisampleType(D3DMULTISAMPLE_TYPE value) { if (IsWindowed) Windowed_MultisampleType = value; else Fullscreen_MultisampleType = value; }

		DWORD MultisampleQuality() { return IsWindowed ? Windowed_MultisampleQuality : Fullscreen_MultisampleQuality; }
		void SetMultisampleQuality(DWORD value) { if (IsWindowed) Windowed_MultisampleQuality = value; else Fullscreen_MultisampleQuality = value; }

		VertexProcessingType GetVertexProcessingType() { return IsWindowed ? Windowed_VertexProcessingType : Fullscreen_VertexProcessingType; }
		void SetVertexProcessingType(VertexProcessingType value) { if (IsWindowed) Windowed_VertexProcessingType = value; else Fullscreen_VertexProcessingType = value; }

		UINT PresentInterval() { return IsWindowed ? Windowed_PresentInterval : Fullscreen_PresentInterval; }
		void SetPresentInterval(UINT value) { if (IsWindowed) Windowed_PresentInterval = value; else Fullscreen_PresentInterval = value; }
};



#endif // HD3DUTIL_H
