// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef __DX9DRIVE_H_DEFINED__
#define __DX9DRIVE_H_DEFINED__

#include "windows_wrapper.h"	// wrapped <windows.h>, must come first :(


#include <stdio.h>
#include <stdlib.h>

#include "database.h"
#include "driver.h"
#include "phedron.h"
#include "please.h"

#include "d3dutil.h"

/*forward declarations*/
class HD3DEnumeration;
class HD3DSettings;
class HDX9VertexBufferCache;
class HDX9IndexBufferCache;
class HDX9Shader;
class HDX9Texture2D;
class HDX9Post;
class HDX9RenderTarget;

#include "msw.h"

#pragma warning(disable : 4710)	// The given function was selected for inline expansion, but the compiler did not perform the inlining.

#define HDX9_PER_OBJECT_PENALTY 5000
#define HDX9_DISPLAY_LIST_LIMIT 0x7FFFFFFF //8192 old number
#define HDX9_POLYMARKER_BATCH_SIZE 4092

#define HDX9_OUT_OF_MEMORY(hr) (hr == E_OUTOFMEMORY || ((hr&0xffff) == ERROR_NOT_ENOUGH_MEMORY))

// D3DPOOL_MANAGED or D3DPOOL_DEFAULT, 
#define HDX9_MANAGED_POOL
#ifdef HDX9_MANAGED_POOL
#	define HDX9_POOL_TYPE					D3DPOOL_MANAGED
#else
#	define HDX9_POOL_TYPE					D3DPOOL_DEFAULT
#endif

D3DPRIMITIVETYPE d3dprimitive(H3DPRIMITIVETYPE type)
{
	switch (type) {
		case H3DPT_POINTLIST:		return D3DPT_POINTLIST;
		case H3DPT_LINELIST:		return D3DPT_LINELIST;
		case H3DPT_LINESTRIP:    	return D3DPT_LINESTRIP;
		case H3DPT_TRIANGLELIST: 	return D3DPT_TRIANGLELIST;
		case H3DPT_TRIANGLESTRIP:	return D3DPT_TRIANGLESTRIP;
		case H3DPT_TRIANGLEFAN:  	return D3DPT_TRIANGLEFAN;
		case H3DPT_FORCE_DWORD:  	return D3DPT_FORCE_DWORD; 
		default:
			HE_PROCEDURAL_ERROR("Unhandled primitive type mapping.");
			return D3DPT_POINTLIST;
	}
}

D3DBLEND d3dblend(H3DBLEND type)
{
	switch (type) {
		case H3DBLEND_ZERO:				return D3DBLEND_ZERO;
		case H3DBLEND_ONE:				return D3DBLEND_ONE;
		case H3DBLEND_SRCCOLOR:         return D3DBLEND_SRCCOLOR;
		case H3DBLEND_INVSRCCOLOR:      return D3DBLEND_INVSRCCOLOR;
		case H3DBLEND_SRCALPHA:         return D3DBLEND_SRCALPHA;
		case H3DBLEND_INVSRCALPHA:      return D3DBLEND_INVSRCALPHA;
		case H3DBLEND_DESTALPHA:        return D3DBLEND_DESTALPHA;
		case H3DBLEND_INVDESTALPHA:     return D3DBLEND_INVDESTALPHA;
		case H3DBLEND_DESTCOLOR:        return D3DBLEND_DESTCOLOR;
		case H3DBLEND_INVDESTCOLOR:     return D3DBLEND_INVDESTCOLOR;
		case H3DBLEND_SRCALPHASAT:      return D3DBLEND_SRCALPHASAT;
		case H3DBLEND_BOTHSRCALPHA:     return D3DBLEND_BOTHSRCALPHA;
		case H3DBLEND_BOTHINVSRCALPHA:  return D3DBLEND_BOTHINVSRCALPHA;
		case H3DBLEND_BLENDFACTOR:      return D3DBLEND_BLENDFACTOR;
		case H3DBLEND_INVBLENDFACTOR:   return D3DBLEND_INVBLENDFACTOR;
		/* H3D9Ex only -- */
#if !defined(H3D_DISABLE_9EX)
		//case H3DBLEND_SRCCOLOR2:        return D3DBLEND_SRCCOLOR2;
		//case H3DBLEND_INVSRCCOLOR2:     return D3DBLEND_INVSRCCOLOR2;
#endif // !H3D_DISABLE_9EX
		/* -- H3D9Ex only */
		case H3DBLEND_FORCE_DWORD:      return D3DBLEND_FORCE_DWORD;
		default:
			HE_PROCEDURAL_ERROR("Unhandled blend type mapping.");
			return D3DBLEND_ZERO;
	}
}



DWORD d3dclear(int flags)
{
	DWORD clear_flags = 0;
	if (ANYBIT(flags, H3DCLEAR_TARGET))
		clear_flags |= D3DCLEAR_TARGET;
	if (ANYBIT(flags, H3DCLEAR_ZBUFFER))
		clear_flags |= D3DCLEAR_ZBUFFER;
	if (ANYBIT(flags, H3DCLEAR_STENCIL))
		clear_flags |= D3DCLEAR_STENCIL;
	return clear_flags;
}

D3DCMPFUNC d3dcmpfunc(H3DCMPFUNC func)
{
	switch (func) {
		case H3DCMP_NEVER:			return D3DCMP_NEVER;
		case H3DCMP_LESS:			return D3DCMP_LESS;
		case H3DCMP_EQUAL:			return D3DCMP_EQUAL;
		case H3DCMP_LESSEQUAL:		return D3DCMP_LESSEQUAL;
		case H3DCMP_GREATER:		return D3DCMP_GREATER;
		case H3DCMP_NOTEQUAL:		return D3DCMP_NOTEQUAL;
		case H3DCMP_GREATEREQUAL:	return D3DCMP_GREATEREQUAL;
		case H3DCMP_ALWAYS:			return D3DCMP_ALWAYS;
		default:
			HE_PROCEDURAL_ERROR("Unhandled comparison function mapping.");
			return D3DCMP_ALWAYS;
	}
}

D3DSHADEMODE d3dshademode(H3DSHADEMODE type)
{
	switch (type) {
		case H3DSHADE_FLAT:			return D3DSHADE_FLAT;
		case H3DSHADE_GOURAUD:		return D3DSHADE_GOURAUD;
		case H3DSHADE_PHONG:		return D3DSHADE_PHONG;
		case H3DSHADE_FORCE_DWORD:	return D3DSHADE_FORCE_DWORD;
		default:
			HE_PROCEDURAL_ERROR("Unhandled shade mode mapping.");
			return D3DSHADE_FLAT;
	}
}

D3DTEXTUREADDRESS d3dtextureaddress(H3DTEXTUREADDRESS type)
{
	switch (type) {
		case H3DTADDRESS_WRAP:			return D3DTADDRESS_WRAP;
		case H3DTADDRESS_MIRROR:		return D3DTADDRESS_MIRROR;
		case H3DTADDRESS_CLAMP:			return D3DTADDRESS_CLAMP;
		case H3DTADDRESS_BORDER:		return D3DTADDRESS_BORDER;
		case H3DTADDRESS_MIRRORONCE:	return D3DTADDRESS_MIRRORONCE;
		default:
			HE_PROCEDURAL_ERROR("Unhandled shade mode mapping.");
			return D3DTADDRESS_WRAP;
	}
}

D3DTEXTUREFILTERTYPE d3dtexturefilter(H3DTEXTUREFILTERTYPE type)
{
	switch (type) {
		case H3DTEXF_NONE:				return D3DTEXF_NONE;
		case H3DTEXF_POINT:				return D3DTEXF_POINT;
		case H3DTEXF_LINEAR:			return D3DTEXF_LINEAR;
		case H3DTEXF_ANISOTROPIC:		return D3DTEXF_ANISOTROPIC;
		case H3DTEXF_PYRAMIDALQUAD:		return D3DTEXF_PYRAMIDALQUAD;
		case H3DTEXF_GAUSSIANQUAD:		return D3DTEXF_GAUSSIANQUAD;
		default:
			HE_PROCEDURAL_ERROR("Unhandled shade mode mapping.");
			return D3DTEXF_NONE;
	}
}

D3DCULL d3dcull(H3DCULL type)
{
	switch (type) {
		case H3DCULL_NONE:			return D3DCULL_NONE;
		case H3DCULL_CW:			return D3DCULL_CW;
		case H3DCULL_CCW:			return D3DCULL_CCW;
		case H3DCULL_FORCE_DWORD:	return D3DCULL_FORCE_DWORD;
		default:
			HE_PROCEDURAL_ERROR("Unhandled cull mode mapping.");
			return D3DCULL_NONE;
	}
}

D3DFORMAT d3dformat(H3DFORMAT type)
{
	switch (type) {
		case H3DFMT_D24S8:			return D3DFMT_D24S8;
		case H3DFMT_A8R8G8B8:		return D3DFMT_A8R8G8B8;
		case H3DFMT_R32F:			return D3DFMT_R32F;
		case H3DFMT_INDEX16:		return D3DFMT_INDEX16;
		case H3DFMT_INDEX32:		return D3DFMT_INDEX32;
		default:
			HE_PROCEDURAL_ERROR("Unhandled format mapping.");
			return D3DFMT_INDEX32;
	}
}

D3DVIEWPORT9 d3dviewport(H3DViewport const *hvp)
{
	D3DVIEWPORT9 vp;
	vp.X = hvp->X;
	vp.Y = hvp->Y;
	vp.Width = hvp->Width;
	vp.Height = hvp->Height;
	vp.MinZ = hvp->MinZ;
	vp.MaxZ = hvp->MaxZ;
	return vp;
}

H3DViewport h3dviewport(D3DVIEWPORT9 const *vp)
{
	H3DViewport hvp;
	hvp.X = vp->X;
	hvp.Y = vp->Y;
	hvp.Width = vp->Width;
	hvp.Height = vp->Height;
	hvp.MinZ = vp->MinZ;
	hvp.MaxZ = vp->MaxZ;
	return hvp;
}
/*****************************************************************************
*****************************************************************************
					H3DVertexFormat helper functions
*****************************************************************************
*****************************************************************************/

struct DX9Data;
void set_targets(DX9Data& dx9data, IDirect3DSurface9 *target0, IDirect3DSurface9 *zbuffer = null, IDirect3DSurface9 *target1 = null, IDirect3DSurface9 *target2 = null, IDirect3DSurface9 *target3 = null );

class VertexRingBuffer;
class IndexRingBuffer;

struct DX9Data : H3DData
{
	float					shader_value_cache[SHC_END][16];
	Define_Bitset(SHC_END) SHC_BITS;
	SHC_BITS				shader_value_cache_valid;

	DX9Data *	next;
	DX9Data **	backlink;

	HD3DEnumeration *		enumeration;			// available D3D settings: adapters, devices, modes, etc
	HD3DSettings *			settings;				// current D3D settings: adapter, device, mode, formats, etc
	D3DPRESENT_PARAMETERS	presentation_params;	// presentation parameters for the device to be created 
	LPDIRECT3DDEVICE9		pDevice;				// The D3D rendering device
	LPDIRECT3DSWAPCHAIN9	pSwapChain;				// The swap chain

	D3D_System_Data *	sd;

	// Reference count for init_/finish_picture
	int						in_picture_count;

	//spriting
	QuickMovesRegion *image_regions;
	QuickMovesRegion *depth_regions;


	// Support for get_pixels
	D3DFORMAT get_pixels_format;
	int get_pixels_width;
	int get_pixels_height;
	LPDIRECT3DSURFACE9 get_pixels_default_buffer;
	LPDIRECT3DSURFACE9 get_pixels_system_buffer;

	//cache the current vertex/index buffer
	unsigned int *	current_vb_offset;
	LPDIRECT3DVERTEXBUFFER9 *current_vb;
	LPDIRECT3DINDEXBUFFER9	*current_ib;

	//cache the current rendertargets
	struct {
		IDirect3DSurface9 *	rt0;
		IDirect3DSurface9 *	rt1;
		IDirect3DSurface9 *	rt2;
		IDirect3DSurface9 *	rt3;
		IDirect3DSurface9 *	ds;	// depthstencil
	} current_render_targets;

	D3DCAPS9				capabilities;			//Caps for the device
	D3DFORMAT				depth_texture_format;
	bool					debug_shadow_map;
	bool					supports_texture_xrgb;

	D3DVERTEXELEMENT9		vd_scratch[15];		// scratch space for creating vertex declarations

	struct LocalIBOBuffers
	{
		VertexRingBuffer * verts;
		IndexRingBuffer * indices;
	} local_ibo_buffers;

	HRESULT hr;
};

#define DX9D(dc) ((DX9Data *)((dc)->data2))
#define DX9NRD(nr) (DX9D((nr)->display_context))



#ifndef DX9DATA_ONLY


#define Debug_NO_TWO_SIDED_LIGHTING				0x00000001
#define Debug_ENABLE_NVPERFHUD					0x00000002
#define Debug_NO_DEPTH_COPY						0x00000004
#define Debug_UNUSED4							0x00000008
#define Debug_UNUSED5							0x00000010
#define Debug_UNUSED6							0x00000020
#define Debug_NO_WINDOWS_HOOK					0x00000040
#define Debug_UNUSED8							0x00000080
#define Debug_UNUSED9							0x00000100
#define Debug_UNUSED10							0x00000200
#define Debug_UNUSED11							0x00000400
#define Debug_UNUSED12							0x00000800
#define Debug_UNUSED13							0x00001000
#define Debug_UNUSED14							0x00002000
#define DEBUG_STARTUP_CLEAR_BLACK				0x00004000
#define Debug_UNUSED16							0x00008000
#define Debug_UNUSED17							0x00010000
#define Debug_UNUSED18							0x00020000
#define Debug_UNUSED19							0x00040000
#define Debug_UNUSED20							0x00080000
#define Debug_USE_WINDOW_IS_IMAGE				0x00100000
#define Debug_UNUSED22							0x00200000
#define Debug_UNUSED23							0x00400000
#define Debug_UNUSED24							0x00800000
#define Debug_FORCE_SOFTWARE					0x01000000
#define Debug_FORCE_SSAA						0x02000000
#define Debug_UNUSED27							0x04000000
#define Debug_UNUSED28							0x08000000
#define Debug_UNUSED29							0x10000000
#define Debug_UNUSED30							0x20000000
#define Debug_UNUSED31							0x40000000
#define Debug_UNUSED32							0x80000000

/* XBIT STUFF */
#define XBIT_NONE							0L
#define XBIT_NO_INDEXED_PRIMITIVES			1L
#define XBIT_BAD_MSAA_SPRITING				2L
#define XBIT_BAD_DEPTH_PEELING				3L
#define XBIT_POINT_SPRITES_BASH_ALL			4L


#define D3DXMatrix_2_floats(d3d_mat_in, floats_out)		\
	floats_out[0][0] = d3d_mat_in._11;					\
	floats_out[0][1] = d3d_mat_in._12;					\
	floats_out[0][2] = d3d_mat_in._13;					\
	floats_out[0][3] = d3d_mat_in._14;					\
	\
	floats_out[1][0] = d3d_mat_in._21;					\
	floats_out[1][1] = d3d_mat_in._22;					\
	floats_out[1][2] = d3d_mat_in._23;					\
	floats_out[1][3] = d3d_mat_in._24;					\
	\
	floats_out[2][0] = d3d_mat_in._31;					\
	floats_out[2][1] = d3d_mat_in._32;					\
	floats_out[2][2] = d3d_mat_in._33;					\
	floats_out[2][3] = d3d_mat_in._34;					\
	\
	floats_out[3][0] = d3d_mat_in._41;					\
	floats_out[3][1] = d3d_mat_in._42;					\
	floats_out[3][2] = d3d_mat_in._43;					\
	floats_out[3][3] = d3d_mat_in._44;			


/*
 * TEXTURE STATES
 */
#define FORCE_TEXTURE_WRAP_S(dx9data, unit, mode) SEMI_PROTECT(\
	(dx9data)->cache->texture_wrap_s[unit] = mode; \
	(dx9data)->pDevice->SetSamplerState(unit, D3DSAMP_ADDRESSU, mode); \
)

#define ENSURE_TEXTURE_WRAP_S(dx9data, unit, mode) SEMI_PROTECT(\
	if ((dx9data)->cache->texture_wrap_s[unit] != mode) \
		FORCE_TEXTURE_WRAP_S((dx9data), unit, mode); \
)

#define FORCE_TEXTURE_WRAP_T(dx9data, unit, mode) SEMI_PROTECT(\
	(dx9data)->cache->texture_wrap_t[unit] = mode; \
	(dx9data)->pDevice->SetSamplerState(unit, D3DSAMP_ADDRESSV, mode); \
)

#define ENSURE_TEXTURE_WRAP_T(dx9data, unit, mode) SEMI_PROTECT(\
	if ((dx9data)->cache->texture_wrap_t[unit] != mode) \
		FORCE_TEXTURE_WRAP_T((dx9data), unit, mode); \
)

#define FORCE_TEXTURE_MIN_FILTER(dx9data, unit, mode) SEMI_PROTECT(\
	(dx9data)->cache->texture_min_filter[unit] = mode; \
	(dx9data)->pDevice->SetSamplerState(unit, D3DSAMP_MINFILTER, mode); \
)

#define ENSURE_TEXTURE_MIN_FILTER(dx9data, unit, mode) SEMI_PROTECT(\
	if ((dx9data)->cache->texture_min_filter[unit] != mode) \
		FORCE_TEXTURE_MIN_FILTER((dx9data), unit, mode); \
)

#define FORCE_TEXTURE_MAG_FILTER(dx9data, unit, mode) SEMI_PROTECT(\
	(dx9data)->cache->texture_mag_filter[unit] = mode; \
	(dx9data)->pDevice->SetSamplerState(unit, D3DSAMP_MAGFILTER, mode); \
)

#define ENSURE_TEXTURE_MAG_FILTER(dx9data, unit, mode) SEMI_PROTECT(\
	if ((dx9data)->cache->texture_mag_filter[unit] != mode) \
		FORCE_TEXTURE_MAG_FILTER((dx9data), unit, mode); \
)

#define FORCE_TEXTURE_MIP_FILTER(dx9data, unit, mode) SEMI_PROTECT(\
	(dx9data)->cache->texture_mip_filter[unit] = mode; \
	(dx9data)->pDevice->SetSamplerState(unit, D3DSAMP_MIPFILTER, mode); \
)

#define ENSURE_TEXTURE_MIP_FILTER(dx9data, unit, mode) SEMI_PROTECT(\
	if ((dx9data)->cache->texture_mip_filter[unit] != mode) \
		FORCE_TEXTURE_MIP_FILTER((dx9data), unit, mode); \
)

// Stream source
#define ENSURE_STREAM_SOURCE(dx9data,streamindex,vb,stride,offset) SEMI_PROTECT(\
	if (vb != dx9data->current_vb[streamindex] || \
		offset != dx9data->current_vb_offset[streamindex]) { \
		dx9data->pDevice->SetStreamSource (streamindex, vb, offset, stride); \
		dx9data->current_vb[streamindex] = vb; \
		dx9data->current_vb_offset[streamindex] = offset; \
	} \
)




/*****************************************************************************
*****************************************************************************
						DX9 Device Tasks
*****************************************************************************
*****************************************************************************/
HRESULT restore_device_objects (Display_Context * dc);
static HRESULT invalidate_device_objects(Display_Context * dc);
local void invalidate_cache (D3D_System_Data *d3d_sd)
{
	if (d3d_sd->cache)
		SET_MEMORY (d3d_sd->cache, sizeof(H3DCache), 0x5c);

	for (int i = 0; i < countof(d3d_sd->current_vb); i++) {
		d3d_sd->current_vb[i] = null;
		d3d_sd->current_vb_offset[i] = 0;
	}
	d3d_sd->current_ib = null;
}


class DX9CreateDeviceTask : public Task 
{
public:
	DX9CreateDeviceTask(
		D3D_System_Data *d3d_sd,
		UINT Adapter,
		D3DDEVTYPE DeviceType,
		HWND hFocusWindow,
		DWORD BehaviorFlags) :
			d3d_sd(d3d_sd),
			Adapter(Adapter),
			DeviceType(DeviceType),
			hFocusWindow(hFocusWindow),
			BehaviorFlags(BehaviorFlags)
	{
		d3d_sd->device_task_queue->Add_Task(this);
	}
	virtual ~DX9CreateDeviceTask() {;};

	virtual void Do_Task(bool exiting)
	{
		UNREFERENCED(exiting);
		D3DPRESENT_PARAMETERS main_pp;
		ZeroMemory( &main_pp, sizeof(main_pp) );

		main_pp.Windowed = true;
		main_pp.SwapEffect = D3DSWAPEFFECT_COPY;
		main_pp.BackBufferFormat = D3DFMT_A8R8G8B8;
		main_pp.BackBufferWidth = 1;
		main_pp.BackBufferHeight = 1;

		ASSERT(d3d_sd->pDevice == NULL);

		d3d_sd->pD3D->CreateDevice(
				Adapter, 
				DeviceType,
				hFocusWindow, 
				BehaviorFlags, 
				&main_pp,
				&d3d_sd->pDevice);

		d3d_sd->device_task_semaphore->Release();
		delete this;
	}

	virtual Task_Type Type() {return Task_Type_DX9_Device_Create_Task;};

private:
	D3D_System_Data *d3d_sd;
	UINT Adapter;
	D3DDEVTYPE DeviceType;
	HWND hFocusWindow;
	DWORD BehaviorFlags;
};

class DX9DeleteDeviceTask : public Task 
{
public:

	DX9DeleteDeviceTask(D3D_System_Data *d3d_sd) : d3d_sd(d3d_sd)
	{
		d3d_sd->device_task_queue->Add_Task(this);
	};
	virtual ~DX9DeleteDeviceTask() {;};

	virtual void Do_Task(bool exiting)
	{
		UNREFERENCED(exiting);

		if (d3d_sd->pDevice)
		{
			ULONG new_count = d3d_sd->pDevice->Release();
			ASSERT(new_count == 0);
			UNREFERENCED(new_count);
			d3d_sd->pDevice = NULL;
		}

		d3d_sd->device_task_semaphore->Release();
	};

	virtual Task_Type Type() {return Task_Type_DX9_Device_Delete_Task;};

private:

	D3D_System_Data *d3d_sd;
};


class DX9ResetDeviceTask : public Task 
{
public:
	DX9ResetDeviceTask(D3D_System_Data * d3d_sd, Display_Context * dc): d3d_sd(d3d_sd), dc(dc)
	{
		d3d_sd->device_task_queue->Add_Task(this);
	}
	virtual ~DX9ResetDeviceTask() {;};

	virtual void Do_Task(bool exiting)
	{
		UNREFERENCED(exiting);

		HThreadID thread_id = THREAD_ID();
		{
			Mutexer mutexer(HOOPS::WORLD->thread_mutex);
			auto it = HOOPS::WORLD->internal_threads->find(thread_id);
			ASSERT(it != HOOPS::WORLD->internal_threads->end());
			it->second = dc->user_thread_data;
		}

		DX9Data *dx9data;
		HRESULT hr;

		D3DPRESENT_PARAMETERS main_pp;
		ZeroMemory( &main_pp, sizeof(main_pp) );

		main_pp.Windowed = true;
		main_pp.SwapEffect = D3DSWAPEFFECT_COPY;
		main_pp.BackBufferFormat = D3DFMT_A8R8G8B8;
		main_pp.BackBufferWidth = 1;
		main_pp.BackBufferHeight = 1;

		invalidate_cache(d3d_sd);

		// Release all vidmem objects
		dx9data = d3d_sd->dx9data_list;
		while (dx9data) {
			if (dx9data->device_objects_restored) {
				dx9data->device_objects_restored = false;
				invalidate_device_objects((Display_Context*)dx9data->dc);
				H_SAFE_RELEASE(dx9data->pSwapChain);
			}
			dx9data = dx9data->next;
		}
		

		while (FAILED(hr = d3d_sd->pDevice->Reset(&main_pp))) {
			if (hr == D3DERR_DEVICELOST) {
				while ((hr = d3d_sd->pDevice->TestCooperativeLevel()) == D3DERR_DEVICELOST)
					;	//do nothing
				if (hr == D3DERR_DRIVERINTERNALERROR) {
					ASSERT(0);
				}
			}
			else if (hr == D3DERR_INVALIDCALL) {
				ASSERT(0);
			}
			else {
				ASSERT(0);
			}
		}

		// Initialize the app's device-dependent objects
		dx9data = d3d_sd->dx9data_list;
		while (dx9data) {
			// guard for partially created driver instances (that have been through start_device but not get_outer_window yet)
			if (dx9data->pDevice) {
				hr = restore_device_objects((Display_Context*)dx9data->dc);
				if (FAILED(hr)) {
					invalidate_device_objects((Display_Context*)dx9data->dc);
				}
			}
			dx9data->device_objects_restored = true;
			dx9data = dx9data->next;
		}

		{
			Mutexer mutexer(HOOPS::WORLD->thread_mutex);
			auto it = HOOPS::WORLD->internal_threads->find(thread_id);
			ASSERT(it != HOOPS::WORLD->internal_threads->end());
			it->second = (Thread_Data *)1;
		}

		d3d_sd->device_task_semaphore->Release();
	}

	virtual Task_Type Type() {return Task_Type_DX9_Device_Create_Task;};

private:
	D3D_System_Data *	d3d_sd;
	Display_Context *	dc;
};


local void destroy_h3d_data(H3DData *h3ddata);
class HDX9_DL_Delete_Task : public Task
{
public:

	HDX9_DL_Delete_Task(Display_Context const * dc, Driver_DL * dl_cleanup_list) {

		h3ddata = H3DD (dc);
		World::Lock(h3ddata->H3D_DL_Delete_Task_mutex);
		h3ddata->H3D_DL_Delete_Task_count++;
		World::Unlock(h3ddata->H3D_DL_Delete_Task_mutex);

		dl_list = dl_cleanup_list;

		Actor const *	actor = dc->actor;
		my_generation = actor->please_generation;

		dc->actor->Add_Task(this);
	};

	virtual ~HDX9_DL_Delete_Task() {

		World::Lock(h3ddata->H3D_DL_Delete_Task_mutex);
		h3ddata->H3D_DL_Delete_Task_count--;
		if (h3ddata->euthanize_data &&
			h3ddata->H3D_DL_Delete_Task_count == 0) {
				destroy_h3d_data(h3ddata);
		}
		else
			World::Unlock(h3ddata->H3D_DL_Delete_Task_mutex);

	};

	virtual Task_Type Type() { return HOOPS::Task_Type_DL_Delete_Task; }

	virtual void Do_Task(bool exiting) {

		Actor const *	actor = h3ddata->dc->actor;

		size_t	total_used = HOOPS::WORLD->display_list_vram_usage + HOOPS::WORLD->texture_vram_usage;
		bool vram_limited = ((total_used * 2) > HOOPS::WORLD->vram_usage_limit);

		if (exiting || vram_limited || ANYBIT(actor->dflags, D_DEAD | D_SHUTDOWN)) {

			Driver_DL * bddl=null;

			while ((bddl = dl_list) != null) {
				dl_list = bddl->next;
				release_display_list (bddl);
			}
		}
		else {

			Driver_DL * bddl=null;

			while ((bddl = dl_list) != null) {
				dl_list = bddl->next;
				release_display_list (bddl);
				if (my_generation != actor->please_generation)
					break;
			}
		}

		if (dl_list != null) {
			my_generation = actor->please_generation;
			actor->Add_Task(this);
		} else
			delete this;
	};

protected:
	Driver_DL *	dl_list;
	H3DData *	h3ddata;
	uint32_t	my_generation;
};


/*****************************************************************************
*****************************************************************************
						HDX9Shader Class
*****************************************************************************
*****************************************************************************/
LPDIRECT3DVERTEXDECLARATION9 CreateVertexDeclarationFromVF(DX9Data *dx9data, H3DVertexFormat VF);

class HDX9Shader : public H3DShader
{
protected:
	char const * get_shader_extension() const {return "hlsl";};

public:	
	IDirect3DDevice9 *m_pDevice;
	IDirect3DVertexShader9 *m_pVS;
	IDirect3DPixelShader9 *m_pPS;
	HD3DXConstantTable *m_pVSConstantTable;
	HD3DXConstantTable *m_pPSConstantTable;
	
	HDX9Shader (const H3DShaderID& id);
	~HDX9Shader ();

	bool Create (H3DData *h3ddata, const char *material_shader_source = null);
	bool Activate (void);

	char *EmitTexture(char *ptr, HTextureDimension dimension, char const *name, int number, int sampler, HTexelType texel_type);

	void EnsureDepthPeeling (H3DTexture *texture, H3DData *h3ddata);
	void EnsureTextures(bool back_face = false);

	void SetConstantInt(H3D_Constant_Table table, int shc, int data)
	{
		DX9Data * const dx9data = static_cast<DX9Data*>(m_h3ddata);

		if (dx9data->shader_value_cache_valid.test(shc) && EQUAL_MEMORY(dx9data->shader_value_cache[shc], sizeof(int), &data))
			return;
		COPY_MEMORY(&data, sizeof(int), dx9data->shader_value_cache[shc]);
		dx9data->shader_value_cache_valid.set(shc);

		HD3DXConstantTable *dxtable = (table == H3D_PSConstant_Table) ? m_pPSConstantTable : m_pVSConstantTable;
		D3DXHANDLE dxhandle = (D3DXHANDLE) cache[shc];
		dxtable->SetInt(m_pDevice, dxhandle, data);
	}

	void SetConstantUInt(H3D_Constant_Table table, int shc, unsigned int data)
	{
		// DX9 does not support uint in hlsl
		ASSERT(0);
		UNREFERENCED(table);
		UNREFERENCED(shc);
		UNREFERENCED(data);
	}

	void SetConstantFloat(H3D_Constant_Table table, int shc, float data)
	{
		DX9Data * const dx9data = static_cast<DX9Data*>(m_h3ddata);

		if (dx9data->shader_value_cache_valid.test(shc) && data == dx9data->shader_value_cache[shc][0])
			return;
		dx9data->shader_value_cache[shc][0] = data;
		dx9data->shader_value_cache_valid.set(shc);

		HD3DXConstantTable *dxtable = (table == H3D_PSConstant_Table) ? m_pPSConstantTable : m_pVSConstantTable;
		D3DXHANDLE dxhandle = (D3DXHANDLE) cache[shc];
		dxtable->SetFloat(m_pDevice, dxhandle, data);
	}

	void SetConstantArray(H3D_Constant_Table table, int shc, void const *data, int datatype, int num = 1)
	{
		DX9Data * const dx9data = static_cast<DX9Data*>(m_h3ddata);

		int num_values = num;

		if (datatype == H3D_Vector)
			num_values = num * 4;

		if (num_values <= 16) {
			if (dx9data->shader_value_cache_valid.test(shc) && 
				EQUAL_MEMORY(dx9data->shader_value_cache[shc], num_values * sizeof(float), (float *) data))
				return;
			COPY_MEMORY(data, num_values * sizeof(float), dx9data->shader_value_cache[shc]);
			dx9data->shader_value_cache_valid.set(shc);
		}

		HD3DXConstantTable *dxtable = (table == H3D_PSConstant_Table) ? m_pPSConstantTable : m_pVSConstantTable;
		D3DXHANDLE dxhandle = (D3DXHANDLE) cache[shc];
		switch (datatype) {
			case H3D_Integer:
				dxtable->SetIntArray(m_pDevice, dxhandle, (const INT *) data, num);
				break;
			case H3D_Float:
				dxtable->SetFloatArray(m_pDevice, dxhandle, (const FLOAT *) data, num);
				break;
			case H3D_Vector:
				if (num == 1)
					dxtable->SetVector(m_pDevice, dxhandle, (const D3DXVECTOR4 *) data);
				else
					dxtable->SetVectorArray(m_pDevice, dxhandle, (const D3DXVECTOR4 *) data, num);
				break;
			case H3D_Matrix:
				if (num == 16)
					dxtable->SetMatrix(m_pDevice, dxhandle, (const D3DXMATRIX *) data);
				else
					dxtable->SetFloatArray(m_pDevice, dxhandle, (const FLOAT *) data, num);

				// dxtable->SetMatrixArray(m_pDevice, dxhandle, (const D3DXMATRIX *) data, num);	//not handled yet
				break;
			default:
				ASSERT(0);
				break;
		}
	}

	void SetConstantVector(H3D_Constant_Table table, int shc, float const * data, int width) {
		SetConstantVectorArray(table, shc, data, 1, width);
	}
	void SetConstantVectorArray(H3D_Constant_Table table, int shc, float const * data, int num_vectors, int width) {
		SetConstantArray(table, shc, data, H3D_Float, num_vectors * width);
	}

	H3D_Shader_Handle GetConstantByName(H3D_Constant_Table table, int shc, char const * parent, char const * constant)
	{
		UNREFERENCED(shc);
		HD3DXConstantTable *d3dtable = (table == H3D_PSConstant_Table) ? m_pPSConstantTable : m_pVSConstantTable;
		D3DXHANDLE parenthandle = 0;

		if (parent)
			parenthandle = d3dtable->GetConstantByName(0, parent);

		H3D_Shader_Handle	handle = (H3D_Shader_Handle) d3dtable->GetConstantByName(parenthandle, constant);

		if (handle == null)
			handle = INVALID_SHADER_HANDLE;

		return handle;
	}
};

/*****************************************************************************
*****************************************************************************
							Vertex/Index Buffers
*****************************************************************************
*****************************************************************************/

local void flush_device(H3DData *h3ddata);

template <typename T> class RingBuffer : public CMO_Pooled
{
public:

	RingBuffer(DX9Data * dx9data) : dx9data(dx9data), buffer(null), buffer_size(0), next_offset(0), locked(false)
	{

	}

	// Returns a pointer to a location where 'size_in_bytes' bytes can be written. 
	// 'out_offset' is the offset (in bytes) at which to bind the buffer with D3D.
	void * Lock(uint32_t num_bytes, uint32_t & out_offset, T * & out_buffer)
	{
		ASSERT(!locked);

		if (num_bytes > buffer_size) {
			Resize(2 * num_bytes);
		}

		ASSERT(num_bytes <= buffer_size);

		void * ptr = null;
		HRESULT hr = S_OK;

		if (next_offset + num_bytes < buffer_size)	{
			hr = buffer->Lock(next_offset, num_bytes, &ptr, D3DLOCK_NOOVERWRITE);
		}

		else {
			hr = buffer->Lock(0, num_bytes, &ptr, D3DLOCK_DISCARD);
			next_offset = 0;
		}

		ASSERT(SUCCEEDED(hr));

		out_offset = next_offset;
		out_buffer = buffer;

		next_offset += num_bytes;
		locked = true;

		// Why do this? I don't know the other vertex/index buffer cache stuff does.
		if (next_offset % 4 || next_offset % 3) {
			// Pad to next 16 byte alignment
			next_offset += (4 - (next_offset % 4));

			// Find next 16 byte alignment that's also 12 byte aligned
			while (next_offset % 3)
				next_offset += 4;
		}

		return ptr;
	}

	void Unlock()
	{
		ASSERT(locked);
		HRESULT hr = buffer->Unlock();
		ASSERT(SUCCEEDED(hr));
		locked = false;
	}

	~RingBuffer()
	{
		buffer->Release();
	}

	void Resize(uint32_t new_size_in_bytes)
	{
		ASSERT(!locked);

		if (buffer != null)
			buffer->Release();

		buffer = CreateBuffer(new_size_in_bytes);
		ASSERT(buffer != null);

		HOOPS::WORLD->display_list_vram_usage -= buffer_size;
		HOOPS::WORLD->display_list_vram_usage += new_size_in_bytes;

		buffer_size = new_size_in_bytes;
		next_offset = 0;
	}

protected:

	virtual T * CreateBuffer(uint32_t size_in_bytes) = 0;	

	DX9Data * dx9data;
	T * buffer;
	uint32_t buffer_size;
	uint32_t next_offset;
	bool locked;
};

class VertexRingBuffer : public RingBuffer <IDirect3DVertexBuffer9>
{
public:

	VertexRingBuffer(DX9Data * dx9data) : RingBuffer(dx9data)
	{

	}

protected:

	virtual IDirect3DVertexBuffer9 * CreateBuffer(uint32_t size_in_bytes)
	{
		IDirect3DVertexBuffer9 * buffer = null;

		HRESULT hr = dx9data->pDevice->CreateVertexBuffer(size_in_bytes, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &buffer, null);
		ASSERT(SUCCEEDED(hr));

		return buffer;
	}	
};

class IndexRingBuffer : public RingBuffer <IDirect3DIndexBuffer9>
{
public:

	IndexRingBuffer(DX9Data * dx9data) : RingBuffer(dx9data)
	{

	}

protected:

	virtual IDirect3DIndexBuffer9 * CreateBuffer(uint32_t size_in_bytes)
	{
		IDirect3DIndexBuffer9 * buffer = null;

		HRESULT hr = dx9data->pDevice->CreateIndexBuffer(size_in_bytes, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_DEFAULT, &buffer, null);
		ASSERT(SUCCEEDED(hr));

		return buffer;
	}
};

// this is the buffer object data structure which caches the vertex and index buffers
// Stores an entire Tristrip or Polyedge
class HDX9IndexedBufferObject : public SwitchableIBO
{
private:
	
	IDirect3DVertexDeclaration9 * m_VD;	
	std::vector<IDirect3DVertexBuffer9*, CMO_Allocator<IDirect3DVertexBuffer9*>> m_vtx_buff;
	IDirect3DIndexBuffer9 * m_idx_buff;

public:

	HDX9IndexedBufferObject(int index, unsigned int flags) : SwitchableIBO(index, flags), m_VD(null), m_idx_buff(null)
	{
		HOOPS::WORLD->display_list_vram_usage += HDX9_PER_OBJECT_PENALTY;			
	}

	~HDX9IndexedBufferObject()
	{
		H_SAFE_RELEASE(m_VD);

		HOOPS::WORLD->display_list_vram_usage -= HDX9_PER_OBJECT_PENALTY;	

		if (m_idx_buff != null) {
			m_idx_buff->Release();
			HOOPS::WORLD->display_list_vram_usage -= m_index_size;
		}

		if (m_vtx_buff.size() > 0) {

			for (int i = 0; i < m_stream_count; ++i)
				m_vtx_buff[i]->Release();

			HOOPS::WORLD->display_list_vram_usage -= m_VF.get_size() * m_point_count;
		}
	}

	virtual bool CanDownloadFromVRAM() const
	{
		return HDX9_POOL_TYPE == D3DPOOL_MANAGED;
	}

	virtual void CopyLocalVertexArraysToVRAM()
	{
		ASSERT(m_buffer_type == VRAM);
		ASSERT(m_vtx_buff.size() == 0);

		m_vtx_buff.resize(m_stream_count);

		DX9Data * dx9data = (DX9Data*)m_h3ddata;
		auto device = dx9data->pDevice;

		// DEFAULT pool must use write only.
		DWORD const usage = HDX9_POOL_TYPE == D3DPOOL_DEFAULT ? D3DUSAGE_WRITEONLY : 0;

		for (int i = 0; i < m_stream_count; ++i) {
			uint32_t const stream_size = m_point_count * m_strides[i] * sizeof(float);
			HRESULT hr = device->CreateVertexBuffer(stream_size, usage, 0, HDX9_POOL_TYPE, &m_vtx_buff[i], null);
			ASSERT(SUCCEEDED(hr));

			void * ptr = null;
			hr = m_vtx_buff[i]->Lock(0, stream_size, &ptr, 0);
			ASSERT(SUCCEEDED(hr));
			COPY_MEMORY(m_vertex_data[i], stream_size, ptr);
			hr = m_vtx_buff[i]->Unlock();
		}

		HOOPS::WORLD->display_list_vram_usage += m_VF.get_size() * m_point_count;
	}

	virtual void CopyLocalIndexArrayToVRAM()
	{
		ASSERT(m_buffer_type == VRAM);
		ASSERT(m_idx_buff == null);

		DX9Data * dx9data = (DX9Data*)m_h3ddata;
		auto device = dx9data->pDevice;

		DWORD const usage = HDX9_POOL_TYPE == D3DPOOL_DEFAULT ? D3DUSAGE_WRITEONLY : 0;

		HRESULT hr = device->CreateIndexBuffer(m_index_size, usage, d3dformat(m_format), HDX9_POOL_TYPE, &m_idx_buff, null);
		ASSERT(SUCCEEDED(hr));

		void * ptr = null;
		hr = m_idx_buff->Lock(0, m_index_size, &ptr, 0);
		ASSERT(SUCCEEDED(hr));
		COPY_MEMORY(m_indices, m_index_size, ptr);
		hr = m_idx_buff->Unlock();

		HOOPS::WORLD->display_list_vram_usage += m_index_size;
	}

	virtual void DownloadVertexArraysFromVRAM()
	{
		ASSERT(HDX9_POOL_TYPE == D3DPOOL_MANAGED);

		if (m_vtx_buff.size() == 0)
			return;

		for (int i = 0; i < m_stream_count; ++i) {
			uint32_t const stream_size = m_point_count * m_strides[i] * sizeof(float);
			
			void * ptr = null;
			HRESULT hr = m_vtx_buff[i]->Lock(0, stream_size, &ptr, D3DLOCK_READONLY);
			ASSERT(SUCCEEDED(hr));
			COPY_MEMORY(ptr, stream_size, m_vertex_data[i]);
			hr = m_vtx_buff[i]->Unlock();

			m_vtx_buff[i]->Release();
		}

		m_vtx_buff.clear();
		
		HOOPS::WORLD->display_list_vram_usage -= m_VF.get_size() * m_point_count;
	}

	virtual void DownloadIndexArrayFromVRAM()
	{
		ASSERT(HDX9_POOL_TYPE == D3DPOOL_MANAGED);

		if (m_idx_buff == null)
			return;

		void * ptr = null;
		HRESULT hr = m_idx_buff->Lock(0, m_index_size, &ptr, D3DLOCK_READONLY);
		ASSERT(SUCCEEDED(hr));
		COPY_MEMORY(ptr, m_index_size, m_indices);
		hr = m_idx_buff->Unlock();

		m_idx_buff->Release();
		m_idx_buff = null;

		HOOPS::WORLD->display_list_vram_usage -= m_index_size;
	}
	
	bool CreateVertexBuffer(H3DData *h3ddata, UINT point_count, H3DVertexFormat VF) 
	{
		DX9Data * dx9data = (DX9Data*)h3ddata;

		ASSERT(m_VD == null);
		m_VD = CreateVertexDeclarationFromVF(dx9data, VF);

		return SwitchableIBO::CreateVertexBuffer(h3ddata, point_count, VF);
	}

	void EnsureVertexDeclaration(H3DData * h3ddata) const
	{
		DX9Data * dx9data = (DX9Data *)h3ddata;
		if (dx9data->cache->vf != m_VF) {
			dx9data->pDevice->SetVertexDeclaration(m_VD);
			dx9data->cache->vf = m_VF;
		}
	}

	uint32_t EnsureBufferPointers() const
	{
		DX9Data * dx9data = (DX9Data *)m_h3ddata;

		uint32_t base_index = 0;

		if (m_buffer_type == SwitchableIBO::Local) {

			if (!dx9data->local_ibo_buffers.verts) {
				dx9data->local_ibo_buffers.verts = POOL_NEW(dx9data->dc->memory_pool(), VertexRingBuffer)(dx9data);
				dx9data->local_ibo_buffers.verts->Resize(2 * H3D_Display_List_MAX_SIZE * (12 + 12 + 4));
			}

			if (!dx9data->local_ibo_buffers.indices) {
				dx9data->local_ibo_buffers.indices = POOL_NEW(dx9data->dc->memory_pool(), IndexRingBuffer)(dx9data);
				dx9data->local_ibo_buffers.indices->Resize(2 * H3D_Display_List_MAX_SIZE * sizeof(unsigned short));
			}

			{
				uint32_t strides[16];
				IDirect3DVertexBuffer9 * buffer = null;
				uint32_t offset = 0;
				// TODO: do we need to align each stream's start offset to a multiple of the stride?
				auto ptr = static_cast<unsigned char *>(dx9data->local_ibo_buffers.verts->Lock(m_point_count * m_VF.get_size(true), offset, buffer));

				for (int i = 0; i < m_stream_count; ++i) {

					strides[i] = m_strides[i] * sizeof(float);
					uint32_t const stream_size = strides[i] * m_point_count;

					COPY_MEMORY(m_vertex_data[i], stream_size, ptr);
					ENSURE_STREAM_SOURCE(dx9data, i, buffer, strides[i], offset);
					ptr += stream_size;
					offset += stream_size;
				}

				dx9data->local_ibo_buffers.verts->Unlock();
			}

			if (m_index_size) {
				IDirect3DIndexBuffer9 * buffer = null;
				uint32_t offset = 0;
				auto ptr = dx9data->local_ibo_buffers.indices->Lock(m_index_size, offset, buffer);
				COPY_MEMORY(m_indices, m_index_size, ptr);
				dx9data->local_ibo_buffers.indices->Unlock();
				dx9data->pDevice->SetIndices(buffer);
				base_index = offset / sizeof(unsigned short);
			}
		}

		else {

			for (int i = 0; i < m_stream_count; i++)
				ENSURE_STREAM_SOURCE(dx9data, i, m_vtx_buff[i], m_strides[i] * sizeof(float), 0);

			if (m_index_size != 0)
				dx9data->pDevice->SetIndices(m_idx_buff);
		}

		return base_index;
	}

	void Draw(H3DData * h3ddata, Test * cull_results = null, int instances = 1) const
	{
		UNREFERENCED(instances);

		DX9Data *			dx9data = (DX9Data *)h3ddata;
		Display_Context *	dc = (Display_Context *)dx9data->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		EnsureVertexDeclaration(dx9data);

		uint32_t const base_index = EnsureBufferPointers();

		if (cull_results == null || m_index_size == 0 || m_geom_offsets == null) {

			if (m_index_size != 0) {

				if (cull_results != null && CULLED(cull_results[m_geom_first]))
					return;

				switch (m_type) {
					case H3DPT_TRIANGLELIST:
					case H3DPT_TRIANGLESTRIP:
						DC_STATS(dl_triangles_3d) += m_primitive_count;
						break;
					case H3DPT_LINELIST:
					case H3DPT_LINESTRIP:
						DC_STATS(dl_lines_3d) += m_primitive_count;
						break;
					case H3DPT_POINTLIST:
						DC_STATS(dl_vertices_3d) += m_primitive_count;
						break;
					default:
						ASSERT(0);
						break;
				}

				dx9data->pDevice->DrawIndexedPrimitive(d3dprimitive(m_type), 0, 0, m_point_count, base_index, m_primitive_count);
			}
			else {
				dx9data->pDevice->DrawPrimitive (d3dprimitive(m_type), 0, m_point_count);
			}
		}
		else {

			int total_primitive_count = 0;
			switch (m_type) {
				case H3DPT_TRIANGLELIST:
					total_primitive_count = m_primitive_count * 3;
					break;
				case H3DPT_TRIANGLESTRIP:
					total_primitive_count = m_primitive_count + 2;
					break;
				case H3DPT_LINELIST:
					total_primitive_count = m_primitive_count * 2;
					break;
				case H3DPT_LINESTRIP:
					total_primitive_count = m_primitive_count + 1;
					break;
				default:
					ASSERT(0);
					break;
			}

			int start = 0;
			int last = 0;

			for (;start < m_geom_count;) {

				while (CULLED(cull_results[start + m_geom_first])) {
					if (++start >= m_geom_count)
						return;
				}

				last = start;

				while (!CULLED(cull_results[last + m_geom_first])) {
					if (++last >= m_geom_count)
						break;
				}

				int len;

				if (last == m_geom_count)
					len = total_primitive_count - m_geom_offsets[start];
				else
					len = m_geom_offsets[last] - m_geom_offsets[start];

				if (len > 0) {
					int primitive_count = 0;
					switch (m_type) {
					case H3DPT_TRIANGLELIST:
						primitive_count = len / 3;
						DC_STATS(dl_triangles_3d) += primitive_count;
						break;
					case H3DPT_TRIANGLESTRIP:
						primitive_count = len - 2;
						DC_STATS(dl_triangles_3d) += primitive_count;
						break;
					case H3DPT_LINELIST:
						primitive_count = len / 2;
						DC_STATS(dl_lines_3d) += primitive_count;
						break;
					case H3DPT_LINESTRIP:
						primitive_count = len - 1;
						DC_STATS(dl_lines_3d) += primitive_count;
						break;
					default:
						ASSERT(0);
						break;
					}

					ASSERT((unsigned int)m_geom_offsets[start] + prim_to_indices(m_type, primitive_count) <= m_index_size / (m_format == H3DFMT_INDEX16 ? sizeof(short) : sizeof(int)));
					dx9data->pDevice->DrawIndexedPrimitive(d3dprimitive(m_type), 0, 0, m_point_count, base_index + m_geom_offsets[start], primitive_count);
				}

				start = last;
			}
		}
	}

	void Draw_CSLinePattern(H3DData * /*h3ddata*/, float /*u_scale*/, float /*v_scale*/, float const * /*matMVP*/, float const * /*matScreen*/, Test * /*cull_results*/, int /*instances*/)
	{
		ASSERT(0);
	}

	void Draw_CSEdgePattern(H3DData * /*h3ddata*/, float /*u_scale*/, float /*v_scale*/, float const * /*matMVP*/, float const * /*matScreen*/, Test * /*cull_results*/, int /*instances*/)
	{
		ASSERT(0);
	}

	void Draw_Decimated( H3DData * h3ddata, float decimation = 1.0f) const
	{
		DX9Data *	dx9data = (DX9Data *)h3ddata;
		Display_Context *	dc = (Display_Context *)dx9data->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		EnsureVertexDeclaration(dx9data);
		
		uint32_t const base_index = EnsureBufferPointers();

		if (m_index_size != 0) {
			int decimated_prim_count = (int) (m_primitive_count * decimation);
			if (decimation < 1) {
				// Assume that draw_decimated with indices is for + and x markers,
				// and with 2 primitives per marker, draw even number of primitives
				decimated_prim_count &= 0xffffffe;
			}

			if (decimated_prim_count > 0) {
				dx9data->pDevice->DrawIndexedPrimitive(d3dprimitive(m_type), 0, 0, m_point_count, base_index, decimated_prim_count);
			}
		}
		else {
			int const decimated = (int)(m_point_count * decimation);
			if (decimated > 0)
				dx9data->pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, decimated);
		}
	}
};


// this is the global vertex buffer cache, being used for view dependent geometry
LPDIRECT3DVERTEXDECLARATION9 CreateVertexDeclarationFromVF(DX9Data * dx9data, H3DVertexFormat VF)
{
	LPDIRECT3DVERTEXDECLARATION9 result;
	bool interleaved = VF.get_interleaved();
	int slot = 0;
	unsigned short stream = 0;
	unsigned short offset = 0;
	if (VF.get_position()) {
		dx9data->vd_scratch[slot].Stream = stream;
		dx9data->vd_scratch[slot].Offset = offset;
		dx9data->vd_scratch[slot].Type = D3DDECLTYPE_FLOAT3;
		dx9data->vd_scratch[slot].Method = D3DDECLMETHOD_DEFAULT;
		dx9data->vd_scratch[slot].Usage = D3DDECLUSAGE_POSITION;
		dx9data->vd_scratch[slot].UsageIndex = 0;
		if (interleaved)
			offset += 12;
		else
			stream++;
		slot++;
	}
	if (VF.get_normals()) {
		dx9data->vd_scratch[slot].Stream = stream;
		dx9data->vd_scratch[slot].Offset = offset;
		dx9data->vd_scratch[slot].Type = D3DDECLTYPE_FLOAT3;
		dx9data->vd_scratch[slot].Method = D3DDECLMETHOD_DEFAULT;
		dx9data->vd_scratch[slot].Usage = D3DDECLUSAGE_NORMAL;
		dx9data->vd_scratch[slot].UsageIndex = 0;
		if (interleaved)
			offset += 12;
		else
			stream++;
		slot++;
	}
	if (VF.get_diffuse()) {
		dx9data->vd_scratch[slot].Stream = stream;
		dx9data->vd_scratch[slot].Offset = offset;
		dx9data->vd_scratch[slot].Type = D3DDECLTYPE_D3DCOLOR;
		dx9data->vd_scratch[slot].Method = D3DDECLMETHOD_DEFAULT;
		dx9data->vd_scratch[slot].Usage = D3DDECLUSAGE_COLOR;
		dx9data->vd_scratch[slot].UsageIndex = 0;
		if (interleaved)
			offset += 4;
		else
			stream++;
		slot++;
	}
	if (VF.get_specular()) {
		dx9data->vd_scratch[slot].Stream = stream;
		dx9data->vd_scratch[slot].Offset = offset;
		dx9data->vd_scratch[slot].Type = D3DDECLTYPE_D3DCOLOR;
		dx9data->vd_scratch[slot].Method = D3DDECLMETHOD_DEFAULT;
		dx9data->vd_scratch[slot].Usage = D3DDECLUSAGE_COLOR;
		dx9data->vd_scratch[slot].UsageIndex = 1;
		if (interleaved)
			offset += 4;
		else
			stream++;
		slot++;
	}
	for (int i = 0; i < VF.get_tex_count(); i++) {
		BYTE type = D3DDECLTYPE_FLOAT1;
		switch(VF.get_tex_size(i)) {
			case 2: type = D3DDECLTYPE_FLOAT2; break;
			case 3: type = D3DDECLTYPE_FLOAT3; break;
			case 4: type = D3DDECLTYPE_FLOAT4; break;
		}
		dx9data->vd_scratch[slot].Stream = stream;
		dx9data->vd_scratch[slot].Offset = offset;
		dx9data->vd_scratch[slot].Type = type;
		dx9data->vd_scratch[slot].Method = D3DDECLMETHOD_DEFAULT;
		dx9data->vd_scratch[slot].Usage = D3DDECLUSAGE_TEXCOORD;
		dx9data->vd_scratch[slot].UsageIndex = (unsigned char) i;
		if (interleaved)
			offset += VF.get_tex_size(i) * sizeof(float);
		else
			stream++;
		slot++;
	}
	//D3DDECL_END()
	dx9data->vd_scratch[slot].Stream = 0xFF;
	dx9data->vd_scratch[slot].Offset = 0;
	dx9data->vd_scratch[slot].Type = D3DDECLTYPE_UNUSED;
	dx9data->vd_scratch[slot].Method = 0;
	dx9data->vd_scratch[slot].Usage = 0;
	dx9data->vd_scratch[slot].UsageIndex = 0;

	dx9data->pDevice->CreateVertexDeclaration(dx9data->vd_scratch, &result);
	return result;
}

class HDX9VertexBufferCache : public H3DVertexBufferCache
{
public:
	HDX9VertexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage += HDX9_PER_OBJECT_PENALTY;
		m_pBuffer = null;
		m_nMaxPoints	= 0;
		m_nNextVertexOffset = 0;
		m_nCurVertexOffset = 0;
	}

	~HDX9VertexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage -= HDX9_PER_OBJECT_PENALTY;

		DestroyContents();
	}

	bool CreateVertexBuffer(H3DData *h3ddata, UINT max_points, 
		H3DVertexFormat VF)
	{
		if (m_pBuffer)
			DestroyContents();

		DX9Data *dx9data = (DX9Data *) h3ddata;

		m_nMaxPoints		= max_points;
		m_VF				= VF;
		m_VD				= CreateVertexDeclarationFromVF(dx9data, m_VF);
		m_stream_count		= VF.get_stream_count();
		ALLOC_ARRAY(m_strides, m_stream_count, unsigned int);
		VF.get_stream_strides(m_strides);


		ALLOC_ARRAY(m_vertex_data, m_stream_count, float*);
		ALLOC_ARRAY(m_pBuffer, m_stream_count, IDirect3DVertexBuffer9*);
		for (int i = 0; i < m_stream_count; i++) {
			dx9data->pDevice->CreateVertexBuffer(m_nMaxPoints*m_strides[i]*sizeof(float), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,  
												0, D3DPOOL_DEFAULT, &m_pBuffer[i], NULL);
		}

		if (m_VF.get_position()) {
			ALLOC_ARRAY(temp_vb, m_nMaxPoints*m_strides[0], float);
		}
		if (m_VF.get_normals()) {
			ALLOC_ARRAY(temp_nb, m_nMaxPoints*m_strides[1], float);
		}

		HOOPS::WORLD->display_list_vram_usage += (m_nMaxPoints * m_VF.get_size());
		return true;
	}

	bool Reset()
	{
		DestroyContents();

		m_nMaxPoints	= 0;
		m_nNextVertexOffset = 0;
		m_nCurVertexOffset = 0;
		m_push_data_start = 0;
		return true;
	}

	bool Discard()
	{
		m_push_data_start = 0;
		m_nNextVertexOffset = 0;
		m_nCurVertexOffset = 0;
		void * pData;

		for (int i = 0; i < m_stream_count; i++) {
			m_pBuffer[i]->Lock((UINT)m_nNextVertexOffset * m_strides[i], 0, &pData, D3DLOCK_DISCARD);
			m_pBuffer[i]->Unlock();
		}
		return true;
	}

	bool PushDataToBuffer(Matrix matrix = null, int length = -1)
	{
		bool result = true;
		int vertices_to_copy = (length >= 0) ? length : m_locked_vertex_size;
		vertices_to_copy -= m_push_data_start;
		if (vertices_to_copy <= 0)
			return true;
		ASSERT (vertices_to_copy > 0);

		int offset = m_push_data_start*3;

		if (matrix) {
			matrix->Transform(vertices_to_copy, (Point*)(temp_vb+offset), (Point*)(m_vertex_data[0]+offset));
			if (m_VF.get_normals())
				matrix->adjoint()->Transform(vertices_to_copy, (Vector*)(temp_nb+offset), (Vector*)(m_vertex_data[1]+offset));
		}
		else {
			COPY_MEMORY(temp_vb+offset, vertices_to_copy*m_strides[0]*sizeof(float), m_vertex_data[0]+offset);
			if (m_VF.get_normals())
				COPY_MEMORY(temp_nb+offset, vertices_to_copy*m_strides[1]*sizeof(float), m_vertex_data[1]+offset);
		}

		m_push_data_start += vertices_to_copy;
		return result;
	}

	bool Lock(unsigned int count, H3DVertexBuffer *pv)
	{
		ASSERT(count <= m_nMaxPoints);
		m_push_data_start = 0;

		// No overwrite will be used if the vertices can fit into 
		//		 the space remaining in the vertex buffer.
		DWORD dwLockFlags = D3DLOCK_NOOVERWRITE;

		// Check to see if the entire vertex buffer has been used up yet.
		if (m_nNextVertexOffset > (m_nMaxPoints - count))
		{
			// No space remains. Start over from the beginning 
			//		 of the vertex buffer.
			dwLockFlags = D3DLOCK_DISCARD;
			m_nCurVertexOffset = 0;
			m_nNextVertexOffset = 0;
		}

		// Lock the vertex buffer.
		HRESULT hr;
		for (int i = 0; i < m_stream_count; i++) {
			int stride = m_strides[i] * sizeof(float);
			if (FAILED(hr = m_pBuffer[i]->Lock((UINT)m_nNextVertexOffset * stride, count * stride, 
				(void **) &m_vertex_data[i], dwLockFlags)))
				return false;
		}
		m_locked_vertex_size = count;

		// Initialize the vertexbuffer iterator with temp vertex/normal buffers
		float *vb_saved = null, *nb_saved = null;
		vb_saved = m_vertex_data[0];
		m_vertex_data[0] = temp_vb;
		if (m_VF.get_normals()) {
			nb_saved = m_vertex_data[1];
			m_vertex_data[1] = temp_nb;
		}
		pv->Init(m_vertex_data, &m_VF);
		m_vertex_data[0] = vb_saved;
		if (m_VF.get_normals()) {
			m_vertex_data[1] = nb_saved;
		}

		// Advance to the next position in the vertex buffer.
		m_nNextVertexOffset += count;

		// Find next common 12-byte and 16-byte alignment (least common multiple of 12 and 16 > m_nNextVertexdata)
		if (m_nNextVertexOffset % 4 || m_nNextVertexOffset % 3) {
			// Pad to next 16 byte alignment
			m_nNextVertexOffset += (4 - (m_nNextVertexOffset % 4));

			// Find next 16 byte alignment that's also 12 byte aligned
			while (m_nNextVertexOffset % 3)
				m_nNextVertexOffset += 4;
		}
		return true;
	}

	bool Unlock(Matrix matrix = null)
	{
		bool result = true;

		PushDataToBuffer(matrix);

		for (int i = 0; i < m_stream_count; i++) {
			result &= SUCCEEDED(m_pBuffer[i]->Unlock());
		}
		return result;
	}

	int StartVertex()
	{
		return m_nCurVertexOffset;
	}

	void EnsureVertexDeclaration(H3DData *h3ddata)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		if (dx9data->cache->vf != m_VF) {
			dx9data->pDevice->SetVertexDeclaration(m_VD);
			dx9data->cache->vf = m_VF;
		}
	}
	void EnsureStreamSources(H3DData *h3ddata)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		int stream = 0;

		for (int i = 0; i < m_stream_count; i++) {
			ENSURE_STREAM_SOURCE(dx9data, stream, m_pBuffer[i], m_strides[i]*sizeof(float), 0);
			stream++;
		}
	}

	bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, int instances = 1)
	{
		// Render the primitives.
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount, instances);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, int instances = 1)
	{
		UNREFERENCED(instances);

		// Render the primitives.
		DX9Data *dx9data = (DX9Data *) h3ddata;
		EnsureStreamSources(dx9data);
		EnsureVertexDeclaration(dx9data);
		HRESULT hr = dx9data->pDevice->DrawPrimitive(d3dprimitive(PrimitiveType), StartVertex(), PrimitiveCount);
		return !FAILED(hr);
	}

	void DrawComplete()
	{
		m_nCurVertexOffset = m_nNextVertexOffset;
	}

	void DestroyContents()
	{
		HOOPS::WORLD->display_list_vram_usage -= (m_nMaxPoints * m_VF.get_size());

		if (temp_vb)
			FREE_ARRAY(temp_vb, m_nMaxPoints*m_strides[0], float);
		if (temp_nb)
			FREE_ARRAY(temp_nb, m_nMaxPoints*m_strides[1], float);

		for (int i = 0; i < m_stream_count; i++) {
			H_SAFE_RELEASE(m_pBuffer[i]);
		}
		FREE_ARRAY(m_pBuffer, m_stream_count, IDirect3DVertexBuffer9*);
		FREE_ARRAY(m_strides, m_stream_count, unsigned int);
		FREE_ARRAY(m_vertex_data, m_stream_count, float*);

		H_SAFE_RELEASE(m_VD);		

		m_stream_count = 0;
	}

	IDirect3DVertexBuffer9 **	m_pBuffer;
	char					m_stream_count;

	IDirect3DVertexDeclaration9 *m_VD;
};


// this is the global vertex buffer cache, being used for view dependent geometry
class HDX9GenericVertexBufferCache : public H3DGenericVertexBufferCache
{
public:
	enum { MAX_STREAMS = 12 };

	HDX9GenericVertexBufferCache(H3DData *h3ddata) :
		H3DGenericVertexBufferCache(h3ddata),
		m_VD(null)
	{
		HOOPS::WORLD->display_list_vram_usage += HDX9_PER_OBJECT_PENALTY;
	}

	~HDX9GenericVertexBufferCache()
	{
		for (int i = 0; i < MAX_STREAMS; i++) {
			H_SAFE_RELEASE(m_pBuffer[i]);
		}
		H_SAFE_RELEASE(m_VD);
		HOOPS::WORLD->display_list_vram_usage -= HDX9_PER_OBJECT_PENALTY;
	}

	bool Lock(H3DVertexFormat const &vf, unsigned int count, H3DVertexBuffer *pv)
	{
		DX9Data *dx9data = (DX9Data*) m_h3ddata;

		float *m_locked_data[MAX_STREAMS];
		if (m_VF != vf || !m_VD) {
			m_VF = vf;
			H_SAFE_RELEASE(m_VD);
			m_VD = CreateVertexDeclarationFromVF(dx9data, vf);
		}

		// No overwrite will be used if the vertices can fit into the space remaining in the vertex buffer.
		int lockflags = D3DLOCK_NOOVERWRITE;
		if (m_next_offset > (m_nMaxPoints - count))	{
			lockflags = D3DLOCK_DISCARD;
			m_next_offset =  m_curr_offset = 0;
		}

		// Lock the vertex buffer.
		int i = 0;
		m_has_stream = 0;
		m_has_stream |= m_VF.get_position()		? 1 << i : 0;	i++;
		m_has_stream |= m_VF.get_normals()		? 1 << i : 0;	i++;
		m_has_stream |= m_VF.get_diffuse()		? 1 << i : 0;	i++;
		m_has_stream |= m_VF.get_specular()		? 1 << i : 0;	i++;
		for (int j = 0; j < m_VF.get_tex_count(); j++) {
			m_has_stream |= (1 << i);		i++;
		}

		int stream = 0;
		for (i = 0; i < MAX_STREAMS; i++) {
			if (ANYBIT(m_has_stream, 1 << i)) {
				int stride = m_strides[i] * sizeof(float);
				if (!m_pBuffer[i]) {
					dx9data->pDevice->CreateVertexBuffer(m_nMaxPoints*stride, D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
															0, D3DPOOL_DEFAULT, &m_pBuffer[i], NULL);
				}
				m_pBuffer[i]->Lock(m_next_offset*stride, count*stride, (void**)&m_vertex_data[i], lockflags);
				m_locked_data[stream] = m_vertex_data[i];
				stream++;
			}
			else {
				// Make sure we discard all buffers, not just the active one; not doing so can lead to erratic behavior
				if (lockflags == D3DLOCK_DISCARD && m_pBuffer[i]) {
					void *dummy;
					int stride = m_strides[i] * sizeof(float);
					m_pBuffer[i]->Lock(0, stride, &dummy, lockflags);
					m_pBuffer[i]->Unlock();
				}
			}
		}

		// TODO: ALLOC_ARRAY(m_vertex_data[i], m_nMaxPoints*m_strides[i], float);
		pv->Init(m_locked_data, &m_VF);
			
		// Advance to the next position in the vertex buffer.
		m_next_offset += count;
		return true;
	}

	bool Unlock()
	{
		bool result = true;
		for (int i = 0; i < MAX_STREAMS; i++) {
			if (ANYBIT(m_has_stream, 1 << i))
				m_pBuffer[i]->Unlock();
		}
		return result;
	}

	int StartVertex()
	{
		return m_curr_offset;
	}

	void EnsureVertexDeclaration(H3DData *h3ddata)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		if (dx9data->cache->vf != m_VF) {
			dx9data->pDevice->SetVertexDeclaration(m_VD);
			dx9data->cache->vf = m_VF;
		}
	}
	void EnsureStreamSources(H3DData *h3ddata)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		int i = 0;
		int stream = 0;
		for (i = 0; i < 4; i++) {
			if (ANYBIT(m_has_stream, 1 << i)) {
				unsigned int stride = m_strides[i]*sizeof(float);
				ENSURE_STREAM_SOURCE(dx9data, stream, m_pBuffer[i], stride, m_curr_offset*stride);
				stream++;
			}
		}
		for (i = 0; i < m_VF.get_tex_count(); i++) {
			if (ANYBIT(m_has_stream, 1 << (i+4))) {
				unsigned int stride = m_strides[i+4]*sizeof(float);
				unsigned int realstride = m_VF.get_tex_size(i)*sizeof(float);
				ENSURE_STREAM_SOURCE(dx9data, stream, m_pBuffer[i+4], realstride, m_curr_offset*stride);
				stream++;
			}
		}
	}

	bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount)
	{
		// Render the primitives.
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount)
	{
		// Render the primitives.
		DX9Data *dx9data = (DX9Data *) h3ddata;
		EnsureStreamSources(dx9data);
		EnsureVertexDeclaration(dx9data);
		HRESULT hr = dx9data->pDevice->DrawPrimitive(d3dprimitive(PrimitiveType), 0, PrimitiveCount);
		return !FAILED(hr);
	}

	void DrawComplete()
	{
		m_curr_offset = m_next_offset;
	}

	short m_has_stream;
	IDirect3DVertexBuffer9 *m_pBuffer[MAX_STREAMS];
	IDirect3DVertexDeclaration9 *m_VD;
};


// an index buffer, used in conjunction with a HDX9VertexBufferCache class. Can use any HDX9VertexBufferCache
class HDX9IndexBufferCache : public H3DIndexBufferCache
{
public:
	HDX9IndexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage += HDX9_PER_OBJECT_PENALTY;
		m_pIndexBuffer = 0;
		m_nNextIndexData = 0;
		m_nCurIndexData = 0;
	}

	~HDX9IndexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage -= HDX9_PER_OBJECT_PENALTY;
		HOOPS::WORLD->display_list_vram_usage -= (IB_CACHE_MAX_SIZE * sizeof(short));
		H_SAFE_RELEASE(m_pIndexBuffer);
	}

	bool CreateIndexBuffer(H3DData *h3ddata)
	{
		HRESULT hr = S_OK;
		DX9Data *dx9data = (DX9Data *) h3ddata;
		LPDIRECT3DDEVICE9 pDevice = dx9data->pDevice;

		ASSERT(VB_CACHE_MAX_SIZE < 0x0000ffff);	//index buffer cache does not support 32 bit indices
		if (m_pIndexBuffer == NULL)
			hr = pDevice->CreateIndexBuffer (
				IB_CACHE_MAX_SIZE * sizeof(short), 
				D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 
				D3DFMT_INDEX16,
				D3DPOOL_DEFAULT,
				&m_pIndexBuffer,
				NULL);

		HOOPS::WORLD->display_list_vram_usage += (IB_CACHE_MAX_SIZE * sizeof(short));
		return SUCCEEDED(hr);
	}

	bool Lock(int count, short **ppbData)
	{
		HRESULT hr;
		int sizetolock = count * sizeof(short);
		if (m_nNextIndexData > (IB_CACHE_MAX_SIZE * sizeof(short) - sizetolock)) {
			m_nCurIndexData = m_nNextIndexData = 0;
			hr = m_pIndexBuffer->Lock(0, sizetolock, (void **) ppbData, D3DLOCK_DISCARD);
		}
		else {
			hr = m_pIndexBuffer->Lock(m_nNextIndexData, sizetolock, (void **) ppbData, D3DLOCK_NOOVERWRITE);
		}
		ASSERT (ppbData != null);
		m_nNextIndexData += sizetolock;

		return SUCCEEDED(hr);
	}

	bool Unlock(VOID)
	{
		return SUCCEEDED(m_pIndexBuffer->Unlock());
	}

	bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, 
		UINT VertexCount,
		UINT PrimitiveCount)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;

		HDX9VertexBufferCache *VertexBufferCache = (HDX9VertexBufferCache*)m_pVertexBufferCache;
		HDX9GenericVertexBufferCache *GenericVertexBufferCache = (HDX9GenericVertexBufferCache*)m_pGenericVertexBufferCache;

		// Render the primitives.
		int start_vertex = 0;
		if (VertexBufferCache) {
			VertexBufferCache->EnsureVertexDeclaration(dx9data);
			VertexBufferCache->EnsureStreamSources(dx9data);
			start_vertex = VertexBufferCache->StartVertex();
		}
		else {
			GenericVertexBufferCache->EnsureVertexDeclaration(dx9data);
			GenericVertexBufferCache->EnsureStreamSources(dx9data);
			start_vertex = 0;
		}

		dx9data->pDevice->SetIndices (m_pIndexBuffer);		
		UINT const startIndex = m_nCurIndexData/sizeof(short);
		ASSERT(startIndex + prim_to_indices(PrimitiveType, PrimitiveCount) <= IB_CACHE_MAX_SIZE);
		HRESULT hr = dx9data->pDevice->DrawIndexedPrimitive(d3dprimitive(PrimitiveType), start_vertex, 0, VertexCount, startIndex, PrimitiveCount);
		return SUCCEEDED(hr);
	}

	void DrawComplete()
	{
		m_nCurIndexData = m_nNextIndexData;

		if (m_pVertexBufferCache)
			m_pVertexBufferCache->DrawComplete();
		else
			m_pGenericVertexBufferCache->DrawComplete();
	}

	IDirect3DIndexBuffer9	*m_pIndexBuffer;
	UINT					m_nCurIndexData;
	UINT					m_nNextIndexData;
};



/*****************************************************************************
*****************************************************************************
							HDX9Texture Class
*****************************************************************************
*****************************************************************************/
class HDX9RenderTarget : public H3DRenderTarget
{
private:
	LPDIRECT3DSURFACE9	m_id;

public:
	HRESULT hr;

public:

	HDX9RenderTarget(H3DData *h3ddata, IDirect3DSurface9 *surface) : 
		H3DRenderTarget(h3ddata, 0, 0, 0, H3DFMT_A8B8G8R8, 0)
	{
		D3DSURFACE_DESC desc;
		surface->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
		//m_format = desc.Format;
		m_msaa = desc.MultiSampleQuality;
		m_id = surface;
	}

	HDX9RenderTarget(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int usage,		// H3DTEXUSAGE
		H3DFORMAT format,
		int msaa = 0,
		bool discard = false) :
		H3DRenderTarget(h3ddata, width, height, usage, format, msaa)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		bool success = true;

		if (usage == H3DTEXUSAGE_RENDERTARGET) {
			success = success && SUCCEEDED( hr = dx9data->pDevice->CreateRenderTarget( 
				width, height, d3dformat(format), 
				(D3DMULTISAMPLE_TYPE) msaa, 0, FALSE, &m_id, null ) );
		}
		else if (usage == H3DTEXUSAGE_DEPTHSTENCIL) {
			success = success && SUCCEEDED( hr = dx9data->pDevice->CreateDepthStencilSurface( 
				width, height, d3dformat(format), 
				(D3DMULTISAMPLE_TYPE) msaa, 0, discard, &m_id, null ) );
		}
		else
			ASSERT(0);
	}

	~HDX9RenderTarget()
	{
		H_SAFE_RELEASE(m_id);
	}

	LPDIRECT3DSURFACE9 GetBuffer() {return m_id;}
};


class HDX9Texture2D : public H3DTexture
{
private:
	LPDIRECT3DSURFACE9 m_surface;
	LPDIRECT3DTEXTURE9 m_id;
	D3DLOCKED_RECT m_rect;

public:
	HRESULT hr;

public:
	HDX9Texture2D(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :
		H3DTexture(h3ddata, width, height, levels, usage, format)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		m_h3ddata = h3ddata;
		D3DFORMAT d3dformat = D3DFMT_A8R8G8B8;
		D3DPOOL pool = D3DPOOL_DEFAULT;
		unsigned int tex_usage = 0;
		m_bytes_per_pixel = 4;

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) 
			tex_usage = D3DUSAGE_RENDERTARGET;
		else if (ANYBIT(usage, H3DTEXUSAGE_DEPTHSTENCIL))
			tex_usage = D3DUSAGE_DEPTHSTENCIL;
		else if (ANYBIT(usage, H3DTEXUSAGE_DYNAMIC))
			tex_usage = D3DUSAGE_DYNAMIC;
		else
			pool = D3DPOOL_MANAGED;

		if (ANYBIT(usage, H3DTEXUSAGE_AUTOGENMIPMAP))
			tex_usage |= D3DUSAGE_AUTOGENMIPMAP;
		else			
			levels = 1;	// If we aren't generating mip levels, don't create them, since the driver doesn't fill in anything but the top level.
		
		switch (format)
		{
			case H3DFMT_L8:
				d3dformat = D3DFMT_L8;
				m_bytes_per_pixel = 1;
				break;
			case H3DFMT_R8G8B8: 
				if (dx9data->supports_texture_rgb24) {
					d3dformat = D3DFMT_R8G8B8;
					m_bytes_per_pixel = 3;
					break;
				}
				else if (dx9data->supports_texture_xrgb) {
					d3dformat = D3DFMT_X8R8G8B8;
					m_format = H3DFMT_X8R8G8B8;
					m_bytes_per_pixel = 4;
					break;
				}
			case H3DFMT_A8R8G8B8:
				d3dformat = D3DFMT_A8R8G8B8;
				m_format = H3DFMT_A8R8G8B8;
				break;
			case H3DFMT_R32F: {
				if (dx9data->caps.has_r32f_texture) {
					d3dformat = D3DFMT_R32F;
				}
				else {
					d3dformat = D3DFMT_A8R8G8B8;
					m_format = H3DFMT_A8R8G8B8;
				}			
			}break;

			case H3DFMT_X8R8G8B8: {
				d3dformat = D3DFMT_X8R8G8B8;
			}break;

			case H3DFMT_D24S8:
			case H3DFMT_D24X8:
				d3dformat = dx9data->depth_texture_format;
				break;

			case H3DFMT_DXT1:
				d3dformat = D3DFMT_DXT1;
				m_bytes_per_pixel = -1;
				break;
			case H3DFMT_DXT3:
				d3dformat = D3DFMT_DXT3;
				m_bytes_per_pixel = -1;
				break;
			case H3DFMT_DXT5:
				d3dformat = D3DFMT_DXT5;
				m_bytes_per_pixel = -1;
				break;

			default:
				HE_ERROR(HEC_DX9_DRIVER, HES_PROCEDURAL_ERROR, "Internal error: unhandled DX9 texture format!");
		}

		hr = dx9data->pDevice->CreateTexture(width, height, levels, tex_usage, d3dformat, pool, &m_id, NULL);

		if (SUCCEEDED(hr))
			m_id->GetSurfaceLevel(0, &m_surface);
	}

	~HDX9Texture2D()
	{
		H_SAFE_RELEASE(m_id);
		H_SAFE_RELEASE(m_surface);
	};

	bool LockBytes(int bytes, unsigned char ** data)
	{
		UNREFERENCED(bytes);
		HRESULT hr;
		hr = m_id->LockRect(0, &m_rect, null, 0);
		*data = (unsigned char *) m_rect.pBits;
		return SUCCEEDED(hr);
	}

	bool Lock(H3DRect *h3drect, unsigned char ** data, int * pitch = null)
	{
		HRESULT hr;
		RECT rect;
		if (h3drect)
		{
			rect.top = h3drect->top;
			rect.bottom = h3drect->bottom;
			rect.right = h3drect->right;
			rect.left = h3drect->left;
			hr = m_id->LockRect(0, &m_rect, &rect, 0);
		}
		else
			hr = m_id->LockRect(0, &m_rect, null, 0);
		*data = (unsigned char *) m_rect.pBits;
		if (pitch)
			*pitch = m_rect.Pitch;
		return SUCCEEDED(hr);
	}

	void Unlock()
	{
		m_id->UnlockRect(0);
		ZERO_STRUCT(&m_rect, D3DLOCKED_RECT);
	}

	void SetData(int level, void const * data, int width, int height, int stride)
	{
		UNREFERENCED(width);

		ASSERT(ANYBIT(m_usage, H3DTEXUSAGE_DYNAMIC));

		D3DLOCKED_RECT locked;
		HRESULT hr = m_id->LockRect(level, &locked, NULL, D3DLOCK_DISCARD);
		{
			ASSERT(SUCCEEDED(hr));

			if (locked.Pitch == stride) {
				memcpy(locked.pBits, data, stride * height);
			}
			else {
				unsigned char const * src_ptr = static_cast<unsigned char const *>(data);
				unsigned char * dst_ptr = static_cast<unsigned char *>(locked.pBits);

				for (int y = 0 ; y < height ; ++y) {
					memcpy(dst_ptr, src_ptr, stride);
					dst_ptr += locked.Pitch;
					src_ptr += stride;
				}
			}
			hr = m_id->UnlockRect(0);
		}
	}

	void GenerateMipMaps()
	{
		m_id->GenerateMipSubLevels();
	}

	bool CompressTextureFromMemory(Texture *	texture) {

#ifndef USE_D3DX
		UNREFERENCED(texture);
		return false;
#else
		LPDIRECT3DSURFACE9 surface;
		unsigned char *	image_block = (unsigned char *)texture->active_image->block;
		int		image_block_alloc_size = 0;
		bool	in_place = false;

		m_id->GetSurfaceLevel(0, &surface);

		RECT src_rect;
		src_rect.top = 0;
		src_rect.left = 0;
		src_rect.bottom = texture->active_image->height;
		src_rect.right = texture->active_image->width;

		D3DFORMAT format = D3DFMT_A8B8G8R8;
		if (texture->active_image->format == Image_RGB24) {
			format = D3DFMT_R8G8B8;

			// Since D3D doesn't have a D3DFMT_B8G8R8 format, we must swap the components manually
			image_block_alloc_size = texture->active_image->width*texture->active_image->height*texture->active_image->bytes_per_pixel;
			image_block = null;
			// activate (and maybe adjust) this test to skip allocation of very large temp blocks
			//if (image_block_alloc_size <= 16*1024*1024)
				TRY_ALLOC_ARRAY(image_block, image_block_alloc_size, unsigned char);

			if (image_block != null) {
				for (int i = 0; i < texture->active_image->width*texture->active_image->height; i++) {
					image_block[i*3+0] = ((unsigned char*) texture->active_image->block)[i*3+2];
					image_block[i*3+1] = ((unsigned char*) texture->active_image->block)[i*3+1];
					image_block[i*3+2] = ((unsigned char*) texture->active_image->block)[i*3+0];
				}
			}
			else {
				image_block = (unsigned char *)texture->active_image->block;
				image_block_alloc_size = 0;
				in_place = true;

				for (int i = 0; i < texture->active_image->width*texture->active_image->height; i++) {
					Swap (image_block[i*3+0], image_block[i*3+2]);
				}
			}
		}
		if (texture->active_image->format == Image_LUMINANCE8) {
			format = D3DFMT_L8;
		}
		
		HRESULT hr = D3DXLoadSurfaceFromMemory(surface, null, null, image_block, format, texture->active_image->row_bytes, null, &src_rect, D3DX_DEFAULT, 0x00000000);

		if (image_block_alloc_size) {
			ASSERT(image_block != texture->active_image->block);
			FREE_ARRAY(image_block, image_block_alloc_size, unsigned char);
		}
		else if (in_place) {
			for (int i = 0; i < texture->active_image->width*texture->active_image->height; i++) {
				Swap (image_block[i*3+0], image_block[i*3+2]);
			}
		}

		H_SAFE_RELEASE(surface);
		return SUCCEEDED(hr);;
#endif
	}

	bool LoadTextureFromMemory(Texture *	texture)
	{
		HRESULT hr;
		unsigned char *data;
		int pitch;

		if (SUCCEEDED(hr = Lock(null, &data, &pitch))) {
			fill_face(m_h3ddata, texture, data, pitch);
			Unlock();
		}
		return SUCCEEDED(hr);;
	}


	/* HDX9Texture specific methods */
	/********************************/
	HRESULT GetSurfaceLevel(unsigned int level, LPDIRECT3DSURFACE9 * surface)
	{
		return m_id->GetSurfaceLevel(level, surface);
	}

	LPDIRECT3DSURFACE9 GetSurface()
	{
		return m_surface;
	}

	HRESULT GetLevelDesc(unsigned int level, D3DSURFACE_DESC * desc)
	{
		return m_id->GetLevelDesc(level, desc);
	}

	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return m_id;
	}
};


class HDX9CubeTexture : public H3DCubeTexture
{
private:
	LPDIRECT3DCUBETEXTURE9 m_id;

public:
	HRESULT hr;

public:
	HDX9CubeTexture(H3DData *h3ddata, 
		unsigned int size, 
		unsigned int levels,
		unsigned int usage) :
		H3DCubeTexture(h3ddata, size, levels, usage, H3DFMT_A8R8G8B8)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		m_h3ddata = h3ddata;
		D3DFORMAT format = D3DFMT_A8R8G8B8;
		D3DPOOL pool = D3DPOOL_DEFAULT;
		unsigned int tex_usage = 0;

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) 
			tex_usage = D3DUSAGE_RENDERTARGET;
		else
			pool = D3DPOOL_MANAGED;

		if (ANYBIT(usage, H3DTEXUSAGE_AUTOGENMIPMAP))
			tex_usage |= D3DUSAGE_AUTOGENMIPMAP;
		else
			// If we aren't generating mip levels, don't create them, since the driver
			// doesn't fill in anything bar the top level.
			levels = 1;
		
		hr = dx9data->pDevice->CreateCubeTexture(size, levels, tex_usage,
			format, pool, &m_id, NULL);
	}

	~HDX9CubeTexture()
	{
		H_SAFE_RELEASE(m_id);
	};

	enum Face
	{
		POSITIVE_X,
		NEGATIVE_X,
		POSITIVE_Y,
		NEGATIVE_Y,
		POSITIVE_Z,
		NEGATIVE_Z,
	};

	bool Lock(H3DFACE face, H3DRect *h3drect, unsigned char ** data, int * pitch = null)
	{
		HRESULT hr;
		RECT rect;
		D3DLOCKED_RECT lr;
		if (h3drect)
		{
			rect.top = h3drect->top;
			rect.bottom = h3drect->bottom;
			rect.right = h3drect->right;
			rect.left = h3drect->left;
			hr = m_id->LockRect(static_cast<D3DCUBEMAP_FACES>(face), 0, &lr, &rect, 0);
		}
		else
			hr = m_id->LockRect(static_cast<D3DCUBEMAP_FACES>(face), 0, &lr, null, 0);
		*data = (unsigned char *)lr.pBits;
		if (pitch)
			*pitch = lr.Pitch;
		return SUCCEEDED(hr);
	}

	void Unlock(H3DFACE face)
	{
		m_id->UnlockRect(static_cast<D3DCUBEMAP_FACES>(face), 0);
	}

	void GenerateMipMaps()
	{
		m_id->GenerateMipSubLevels();
	}

	LPDIRECT3DCUBETEXTURE9 GetTexture()
	{
		return m_id;
	}
};



class HDX9Texture3D : public H3DTexture3D
{
private:
	LPDIRECT3DVOLUMETEXTURE9 m_id;

public:
	HRESULT hr;

public:
	HDX9Texture3D(H3DData *h3ddata, 
		unsigned int w, 
		unsigned int h,
		unsigned int depth,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :
		H3DTexture3D(h3ddata, w, h, depth, levels, usage, format)
	{
		DX9Data *dx9data = (DX9Data *) h3ddata;
		m_h3ddata = h3ddata;
		D3DFORMAT d3dformat = D3DFMT_UNKNOWN;
		D3DPOOL pool = D3DPOOL_DEFAULT;
		unsigned int tex_usage = 0;

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) 
			tex_usage = D3DUSAGE_RENDERTARGET;
		else
			pool = D3DPOOL_MANAGED;

		switch (format)
		{
			case H3DFMT_L8:
				d3dformat = D3DFMT_L8;
				break;
			case H3DFMT_R8G8B8:
				if (dx9data->supports_texture_rgb24) {
					d3dformat = D3DFMT_R8G8B8;
					break;
				}
			case H3DFMT_A8R8G8B8:
				d3dformat = D3DFMT_A8R8G8B8;
				break;

			case H3DFMT_R32F:
			case H3DFMT_D24S8:
			case H3DFMT_D24X8:
			case H3DFMT_DXT1:
			case H3DFMT_DXT3:
			case H3DFMT_DXT5:
			default:
				HE_ERROR(HEC_DX9_DRIVER, HES_PROCEDURAL_ERROR, "Internal error: unhandled DX9 texture format in HDX9Texture3D!");
				return;
		}

		if (ANYBIT(usage, H3DTEXUSAGE_AUTOGENMIPMAP))
			tex_usage |= D3DUSAGE_AUTOGENMIPMAP;
		else
			// If we aren't generating mip levels, don't create them, since the driver
			// doesn't fill in anything bar the top level.
			levels = 1;

#ifdef USE_D3DX
		//use d3dx as above instead of the following in order to get some free error checking
		hr = D3DXCreateVolumeTexture (dx9data->pDevice, w, h, depth, levels, tex_usage, d3dformat, pool, &m_id);
#else	
		hr = dx9data->pDevice->CreateVolumeTexture(w, h, depth, levels, tex_usage, d3dformat, pool, &m_id, NULL);
#endif
	}

	~HDX9Texture3D()
	{
		H_SAFE_RELEASE(m_id);
	};

	bool Lock(unsigned char ** data, int *rowpitch, int *slicepitch)
	{
		HRESULT hr;
		D3DLOCKED_BOX lb;

		hr = m_id->LockBox(0, &lb, null, 0);
		*rowpitch = lb.RowPitch;
		*slicepitch = lb.SlicePitch;
		*data = (unsigned char *)lb.pBits;
		return SUCCEEDED(hr);
	}

	void Unlock()
	{
		m_id->UnlockBox(0);
	}

	void GenerateMipMaps()
	{
		m_id->GenerateMipSubLevels();
	}

	LPDIRECT3DVOLUMETEXTURE9 GetTexture()
	{
		return m_id;
	}
};



/*****************************************************************************
*****************************************************************************
						HDX9Actions Class
*****************************************************************************
*****************************************************************************/
class HDX9Actions : public H3DActions
{
private:
	DX9Data *m_dx9data;

// Turn on debugger output
// #define DX9_TRACE_TO_DEBUGGER

	void Trace(wchar_t const *event, wchar_t const *label) {
#ifdef DX9_TRACE_TO_DEBUGGER
		DWORD const thread = GetCurrentThreadId();

		int const buffer_size = 256;
		wchar_t buffer[buffer_size];
		_snwprintf( buffer, buffer_size-1, L"[Thread %d] %s: %s\n", thread, event, label );
		buffer[buffer_size-1] = '\0';

		OutputDebugStringW( buffer );
#else
		UNREFERENCED(event);
		UNREFERENCED(label);
#endif
	}

public:
	HDX9Actions(){};
	HDX9Actions(DX9Data *dx9data) : 
		H3DActions(dx9data),
		m_dx9data(dx9data) {};
	~HDX9Actions(){};

	Int_XY GetWindowSize() const
	{
		Int_XY size;
		size.x = m_dx9data->presentation_params.BackBufferWidth;
		size.y = m_dx9data->presentation_params.BackBufferHeight;
		return size;
	}

	void Begin_Trace(wchar_t const *label) {
		D3DPERF_BeginEvent(0, label);
		Trace( L"Enter", label );
	};
	void End_Trace(wchar_t const *label) {
		Trace( L"Exit", label );
		D3DPERF_EndEvent();
	};

	void CreateDLDeleteTask(Display_Context const *dc, Driver_DL * dl_cleanup_list)
	{
		NEW(HDX9_DL_Delete_Task(dc, dl_cleanup_list));
	}

	H3DGenericVertexBufferCache *CreateGenericVertexBufferCache() {return NEW(HDX9GenericVertexBufferCache)(m_h3ddata);}
	H3DVertexBufferCache *CreateVertexBufferCache() {return NEW(HDX9VertexBufferCache)();}
	H3DIndexBufferCache *CreateIndexBufferCache() {return NEW(HDX9IndexBufferCache)();}
	H3DIndexedBufferObject* CreateIndexedBufferObject(int index, unsigned int flags) 
	{		
		return NEW(HDX9IndexedBufferObject)(index, flags);
	}

	void Clear(int flags, int color, float z_value, int stencil_value, bool simple_clear)
	{
		ENSURE_CHANNEL_MASK(m_dx9data, H3DMASK_RGBA);

		if (simple_clear) {
			m_dx9data->pDevice->Clear(0, NULL, d3dclear(flags), color, z_value, stencil_value);
			return;
		}

		// If we're using depth effects, don't clear the depth texture to the colour value.
		RenderTargets saved;
		if (m_dx9data->depth_effects.mode && !m_dx9data->shadow_maps.mode) {
			save_targets(&saved);
			set_targets(saved.targets[0], saved.depth_stencil);
		}

		// Clear the color target and Z/stencil normally via D3D.
		DWORD clearflags = d3dclear(flags);
		m_dx9data->pDevice->Clear(0, NULL, clearflags, color, z_value, stencil_value);

		// If we're using depth effects, we need to put back the old targets.
		if (m_dx9data->depth_effects.mode && !m_dx9data->shadow_maps.mode) {
			// If the depth buffer was cleared, clear the depth texture to a matching color value.
			if (clearflags & D3DCLEAR_ZBUFFER) {

				// If we're clearing to the far plane, make sure we don't hit it.
				if (z_value == 1.0f)
					z_value = 254.0f / 255.0f;

				int clearcolor = 0;
				
				if (m_dx9data->depth_effects.must_pack_rgba) {
					float packed[4];
					pack_rgba(z_value, packed);
					clearcolor = h3d_pack_color(m_dx9data->rgba_format, RGBA32(*((RGBA*)packed)));
				}

				else {
					clearcolor = D3DCOLOR_COLORVALUE(z_value, z_value, z_value, z_value);
				}

				if (m_dx9data->ssaa_line_target.mode)
					set_targets(saved.targets[2]);
				else
					set_targets(saved.targets[1]);
				m_dx9data->pDevice->Clear(0, NULL, D3DCLEAR_TARGET, clearcolor, 0, 0);
			}

			// Restore original targets.
			m_dx9data->h3d_actions->restore_targets(&saved);
		}
	}

	void SetViewport(H3DViewport const *hvp)
	{
		D3DVIEWPORT9 vp = d3dviewport(hvp);
		if ( m_dx9data->ssaa_active ) {
			vp.X *= m_dx9data->ssaa_width_scale;
			vp.Y *= m_dx9data->ssaa_height_scale;
			vp.Width *= m_dx9data->ssaa_width_scale;
			vp.Height *= m_dx9data->ssaa_height_scale;
		}

		// Prevent viewport from being outside render target surface
		D3DSURFACE_DESC desc;
		m_dx9data->current_render_targets.rt0->GetDesc(&desc);

		if (vp.X + vp.Width > desc.Width) {
			vp.Width = desc.Width - vp.X;
		}
		if (vp.Y + vp.Height > desc.Height) {
			vp.Height = desc.Height - vp.Y;
		}
		m_dx9data->pDevice->SetViewport(&vp);
	}

	void set_depth_bias (float depth_bias, float slope_bias) {
		m_dx9data->pDevice->SetRenderState (D3DRS_DEPTHBIAS, *(DWORD const *)&depth_bias);
		m_dx9data->pDevice->SetRenderState (D3DRS_SLOPESCALEDEPTHBIAS, *(DWORD const *)&slope_bias);
	}

	bool  begin_scene () {
		if ( !m_dx9data->has_scene_began )
		{
			m_dx9data->has_scene_began = true;
			m_dx9data->pDevice->BeginScene();
			return true;
		}

		return false;
	}

	void end_scene () {
		m_dx9data->pDevice->EndScene();
		m_dx9data->has_scene_began = false;
	}

	void handle_size_change(Display_Context * dc);

	void stretch_rect (
	   H3DRenderTarget *	in_src,
	   H3DRenderTarget *	in_dest,
	   H3DTEXTUREFILTERTYPE		filter,
	   bool						clear_alpha,
	   bool						flip,
	   bool						discard,
	   H3DRect const *			source_rect,
	   H3DRect const *			dest_rect);

	/************************/
	/* Define/Bind Textures */
	/************************/

	bool bind_texture(const Net_Rendition & nr, Texture * texture, int usage, H3DTexture *id, int texture_unit);


	/********************/
	/* Texture Creation */
	/********************/
	bool CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned int usage, H3DFORMAT format, H3DTexture **id);
	bool CreateCubeTexture(unsigned int size, unsigned int levels, unsigned int usage, H3DCubeTexture **id);
	bool CreateTexture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int levels, unsigned int usage, H3DFORMAT format, H3DTexture3D **id);
	bool CreateRenderTarget(unsigned int width, unsigned int height, unsigned int usage, H3DFORMAT z_format, unsigned int msaa, H3DRenderTarget **id);
	void SetTexture(int unit, H3DTexture *id)
	{
		ENSURE_TEXTURE(m_dx9data, unit, id);
	}


	/*******************/
	/* Shader Creation */
	/*******************/
	H3DShader *CreateShader(const H3DShaderID &id, size_t size) 
	{
		ASSERT(size == sizeof(H3DShader));
		return NEW(HDX9Shader)(id);
	};



	/******************/
	/* Region actions */
	/******************/
	void create_region (Net_Rendition const &  nr,
			IntRectangle const * area, void **image, void **z);

	void save_region (Net_Rendition const &  nr,
			IntRectangle const * area, void * image, void * z);

	void restore_region (Net_Rendition const &  nr,
			IntRectangle const * area, void * image, void * z);

	void destroy_region (Display_Context * dc,
			void * image, void * z);


	/********************/
	/* SaveRendertarget */
	/********************/
	void save_targets (RenderTargets *saved);
	void restore_targets (RenderTargets *saved, bool release = true);

	void set_target (int index, H3DRenderTarget *target);
	void set_targets (H3DRenderTarget *target0, H3DRenderTarget *zbuffer = null, H3DRenderTarget *target1 = null, H3DRenderTarget *target2 = null, H3DRenderTarget *target3 = null );

	bool test_targets (H3DRenderTarget *target0, H3DRenderTarget *zbuffer = null, H3DRenderTarget *target1 = null, H3DRenderTarget *target2 = null, H3DRenderTarget *target3 = null ) {
		UNREFERENCED(target0);
		UNREFERENCED(zbuffer);
		UNREFERENCED(target1);
		UNREFERENCED(target2);
		UNREFERENCED(target3);
		return true;
	};

	bool get_pixels( H3DRenderTarget *source, int x, int y, int width, int height, uint32_t *pixels);

	/************************/
	/* Device state "force" */
	/************************/

	void force_anti_alias(bool mode)
	{
		m_dx9data->cache->antialias = mode;
		m_dx9data->pDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, mode);
	}

	void force_blend(H3DBLEND src, H3DBLEND dest)
	{
		m_dx9data->cache->src_blend = src;
		m_dx9data->pDevice->SetRenderState(D3DRS_SRCBLEND, d3dblend(src));
		m_dx9data->cache->dest_blend = dest;
		m_dx9data->pDevice->SetRenderState(D3DRS_DESTBLEND, d3dblend(dest));
	}

	void force_color_mask(bool mode, bool lock_it=false)
	{
		if (!m_dx9data->cache->color_mask_locked) {
			m_dx9data->cache->color_mask = mode;
			m_dx9data->cache->color_mask_locked = lock_it;
			m_dx9data->pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, mode ? m_dx9data->cache->channel_mask : 0);
		}
		else if (mode && m_dx9data->cache->color_mask)
			m_dx9data->pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, m_dx9data->cache->channel_mask);
	}

	void force_channel_mask(H3DMASK mask)
	{
		m_dx9data->cache->channel_mask = mask;
		force_color_mask(m_dx9data->cache->color_mask);
	}

	void force_culling(H3DCULL mode)
	{
		if (mode == H3DCULL_NONE)
			m_dx9data->culling = false;
		else
			m_dx9data->culling = true;
		m_dx9data->cache->cull_mode = mode;
		m_dx9data->pDevice->SetRenderState(D3DRS_CULLMODE, d3dcull(mode));
	}

	void force_depth_mask(bool mode)
	{
		m_dx9data->cache->depth_mask = mode;
		if (mode)
			m_dx9data->pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_TRUE);
		else
			m_dx9data->pDevice->SetRenderState(D3DRS_ZWRITEENABLE, D3DZB_FALSE);

		D3DRENDERSTATETYPE depth_location = D3DRS_COLORWRITEENABLE1;
		if (m_dx9data->ssaa_line_target.mode)
			depth_location = D3DRS_COLORWRITEENABLE2;

		bool const depth_peeling = m_dx9data->depth_peeling.mode && m_dx9data->depth_peeling.use_mrt;				
		bool const depth_texture = (m_dx9data->depth_effects.mode || depth_peeling) && ANYBIT (m_dx9data->capabilities.PrimitiveMiscCaps, D3DPMISCCAPS_INDEPENDENTWRITEMASKS);

		if (depth_texture)
			m_dx9data->pDevice->SetRenderState(depth_location, mode ? 0xf : 0);
	}

	void force_depth_range_set(float zmin, float zmax)
	{
		H3DViewport vp = m_dx9data->cache->viewport;
		vp.MinZ = m_dx9data->cache->z_range[0] = zmin;
		vp.MaxZ = m_dx9data->cache->z_range[1] = zmax;
		SetViewport(&vp);
	}

	void force_fog (Net_Rendition const & nr, bool mode)
	{
		if (mode) {
			m_dx9data->cache->fog_on = true;
			m_dx9data->pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
			if (nr->visualization_rendition->incarnation != m_dx9data->fog_incarnation) {
				/*float fog_color[4];*/
				m_dx9data->fog_incarnation = nr->misc_rendition->incarnation;
				/* do not do pixel fog calculations */
				m_dx9data->pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
				m_dx9data->pDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
				DirectRGB const &	direct_rgb = nr->window_rendition->window_color.direct_rgb;
				m_dx9data->pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(direct_rgb.r, direct_rgb.g, direct_rgb.b));
			} \
		}
		else {
			m_dx9data->cache->fog_on = false;
			m_dx9data->pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		}
	}

	void force_sampler_state (
		int texture_unit, 
		H3DTEXTUREFILTERTYPE filter, 
		H3DTEXTUREFILTERTYPE mip_filter, 
		H3DTEXTUREADDRESS wrap)
	{
		ENSURE_TEXTURE_MIN_FILTER(m_dx9data, texture_unit, d3dtexturefilter(filter));
		ENSURE_TEXTURE_MAG_FILTER(m_dx9data, texture_unit, d3dtexturefilter(filter));
		ENSURE_TEXTURE_MIP_FILTER(m_dx9data, texture_unit, d3dtexturefilter(mip_filter));
		ENSURE_TEXTURE_WRAP_S(m_dx9data, texture_unit, d3dtextureaddress(wrap));
		ENSURE_TEXTURE_WRAP_T(m_dx9data, texture_unit, d3dtextureaddress(wrap));
	}

	void force_scissor(bool mode)
	{
		if (m_dx9data->can_scissor) {
			INVALIDATE_SCISSOR_SET(m_dx9data);
			m_dx9data->cache->scissoring = mode;
			if (mode)
				m_dx9data->pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
			else
				m_dx9data->pDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		}
	}

	void force_scissor_set(int in_left, int in_bottom, int in_right, int in_top)
	{
		RECT scr_rect;
		scr_rect.left = in_left;
		scr_rect.top = m_dx9data->yfudge - in_top;
		scr_rect.right = in_right + 1;
		scr_rect.bottom = m_dx9data->yfudge - in_bottom + 1;

		if ( m_dx9data->ssaa_active ) { \
			scr_rect.left *= m_dx9data->ssaa_width_scale;
			scr_rect.right *= m_dx9data->ssaa_width_scale;
			scr_rect.top *= m_dx9data->ssaa_height_scale;
			scr_rect.bottom *= m_dx9data->ssaa_height_scale;
		}

		// Prevent scissor rect from existing outside render target surface
		scr_rect.left = Min((int)scr_rect.left, m_dx9data->render_target->Width());
		scr_rect.right = Min((int)scr_rect.right, m_dx9data->render_target->Width());
		scr_rect.top = Min((int)scr_rect.top, m_dx9data->render_target->Height());
		scr_rect.bottom = Min((int)scr_rect.bottom, m_dx9data->render_target->Height());

		m_h3ddata->cache->scissor_rect.left = in_left;
		m_h3ddata->cache->scissor_rect.bottom = in_bottom; 
		m_h3ddata->cache->scissor_rect.right = in_right; 
		m_h3ddata->cache->scissor_rect.top = in_top; 

		m_dx9data->pDevice->SetScissorRect(&scr_rect);
	}

	void force_shade_mode(H3DSHADEMODE mode)
	{
		m_dx9data->cache->shade_mode = mode;
		m_dx9data->pDevice->SetRenderState(D3DRS_SHADEMODE, d3dshademode(mode));
	}

	void force_alphatesting(bool mode)
	{
		if (mode){
			m_dx9data->pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE); 
			force_alphafunc(H3DCMP_GREATEREQUAL, 0x01);
		}
		else
			m_dx9data->pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
		m_dx9data->cache->alphatesting = mode;
	}

	void force_alphafunc(H3DCMPFUNC func = H3DCMP_GREATEREQUAL, int ref = 0x01)
	{
		m_dx9data->pDevice->SetRenderState(D3DRS_ALPHAREF, (DWORD)ref);
		m_dx9data->pDevice->SetRenderState(D3DRS_ALPHAFUNC, d3dcmpfunc(func));
		m_dx9data->cache->alpha_ref = ref;
		m_dx9data->cache->alpha_func = func;
	}

	void force_stencilling(bool mode)
	{
		if (mode)
			m_dx9data->pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
		else
			m_dx9data->pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		m_dx9data->cache->stencilling = mode;

	}

	void force_stencil_func(H3DCMPFUNC cmp)
	{
		m_dx9data->cache->stencil_func = cmp;
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILFUNC, cmp);
	}

	void force_stencil_mask(int val)
	{
		m_dx9data->cache->stencil_mask = val;
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILMASK, val);
	}

	void force_stencil_ref(int val)
	{
		m_dx9data->cache->stencil_ref = val;
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILREF, val);
	}

	void force_stencil_write_mask(unsigned int val)
	{
		m_dx9data->cache->stencil_write_mask = val;
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILWRITEMASK, val);
	}

	void force_stencil_op(H3DSTENCILOP fail, H3DSTENCILOP zfail, H3DSTENCILOP pass)
	{
		m_dx9data->cache->stencil_fail = fail;
		m_dx9data->cache->stencil_zfail = zfail;
		m_dx9data->cache->stencil_pass = pass;
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILFAIL, fail);
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILZFAIL, zfail);
		m_dx9data->pDevice->SetRenderState(D3DRS_STENCILPASS, pass);
	}

	void set_stencil_func(H3DCMPFUNC cmp, unsigned long ref, unsigned long mask, unsigned long writemask)
	{
		if (cmp != m_dx9data->cache->stencil_func)
			force_stencil_func(cmp);
		if (ref != m_dx9data->cache->stencil_ref)
			force_stencil_ref(ref);
		if (mask != m_dx9data->cache->stencil_mask)
			force_stencil_mask(mask);
		if (writemask != m_dx9data->cache->stencil_write_mask)
			force_stencil_write_mask(writemask);
	}

	void force_texture (int unit, H3DTexture *texture)
	{
		ASSERT(unit < H3D_MAX_TEXTURES);
		m_dx9data->cache->texture[unit] = texture;
		
		if ( texture ) {
			texture->texcache = &m_dx9data->cache->texture[unit];

			// SIL: Different downcasts required for 2D and cube textures.
			if ( texture->IsCubeTexture() ) {
				HDX9CubeTexture *dx9_texture = static_cast<HDX9CubeTexture*>(texture);
				m_dx9data->pDevice->SetTexture(unit, dx9_texture->GetTexture());
			}
			else if ( texture->IsTexture3D() ) {
				HDX9Texture3D *dx9_texture = static_cast<HDX9Texture3D*>(texture);
				m_dx9data->pDevice->SetTexture(unit, dx9_texture->GetTexture());
			}
			else {
				HDX9Texture2D *dx9_texture = static_cast<HDX9Texture2D*>(texture);
				m_dx9data->pDevice->SetTexture(unit, dx9_texture->GetTexture());
			}
		}
		else
			m_dx9data->pDevice->SetTexture(unit, null);
		m_dx9data->dc->stats->bind_texture++;
	}
	void ensure_texture (int unit, H3DTexture *texture)
	{
		if (m_dx9data->cache->texture[unit] != texture)
			FORCE_TEXTURE(m_dx9data, unit, texture);
	}

	void force_transparency(bool mode)
	{	
		auto device = m_dx9data->pDevice;
		
		if (mode && ANYBIT(m_h3ddata->dc->options.bits, Drivo_WINDOW_OPACITY))
		{
			ANYBIT(m_dx9data->capabilities.PrimitiveMiscCaps, D3DPMISCCAPS_SEPARATEALPHABLEND);
			device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
			device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
			device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_INVSRCALPHA);
		}

		m_dx9data->cache->transparency_on = mode;
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, mode);
	}

	void force_zbuffering(bool mode)
	{
		if (!mode)
			m_dx9data->pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);
		else {
			if (m_dx9data->cache->depth_test_reversed)
				m_dx9data->pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_GREATER);
			else
				m_dx9data->pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		}
		m_dx9data->cache->zbuffering = mode;
	}

	void force_point_sprites(bool enable, float min_size, float max_size)
	{
		m_dx9data->cache->point_sprites_enabled = enable;

		m_dx9data->pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, enable);
		m_dx9data->pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, *reinterpret_cast<const DWORD*>(&min_size));
		m_dx9data->pDevice->SetRenderState(D3DRS_POINTSIZE_MAX, *reinterpret_cast<const DWORD*>(&max_size));
	}

	void draw_textured_rectangle(H3DTexture * texture, int	x, int	y, int width, int height, float u1, float v1, float u2, float v2);

	void * IM_Get_Surface(Net_Rendition const & nr)
	{
		if (m_dx9data->dc_type == DCT_IMAGE && ANYBIT (m_dx9data->dc->options.bits, Drivo_GPU_RESIDENT)) {
			
			H3DTexture * texture = ensure_gpu_res_texture(nr);
			return static_cast<HDX9Texture2D *>(texture)->GetSurface();
		}

		return ((HDX9RenderTarget*)m_dx9data->render_target)->GetBuffer();
	}

	bool Snapshot(Net_Rendition const & nr, int w, int h, int bpp, void * data);
	
}; // HDX9Actions


#endif // DX9DATA_ONLY

#pragma warning(default : 4710)	// The given function was selected for inline expansion, but the compiler did not perform the inlining.rw

#endif // __DX9DRIVE_H_DEFINED__


