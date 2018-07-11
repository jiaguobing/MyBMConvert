// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#if !defined(__DX11DRIVE_H_DEFINED__) && defined(_MSC_VER)
#define __DX11DRIVE_H_DEFINED__

#include <D3D11_1.h>
#include <DXGI.h>
#include <D3Dcompiler.h>

#include "3d_common.h"

#include "d3d_image.h"

/*forward declarations*/
struct DX11Data;
class HDX11RenderStates;
class HDX11RenderTarget;
class HDX11Texture2D;
class HDX11ShaderConstants;

bool ensure_cs_line_pattern_resources(DX11Data *dx11data);
bool ensure_cs_edge_pattern_resources(DX11Data *dx11data);

#define HDX11_PER_OBJECT_PENALTY 5000
#define HDX11_DISPLAY_LIST_LIMIT 0x7FFFFFFF //8192 old number
#define HDX11_POLYMARKER_BATCH_SIZE 4092

#define HDX11_OUT_OF_MEMORY(hr) (hr == E_OUTOFMEMORY || ((hr&0xffff) == ERROR_NOT_ENOUGH_MEMORY))


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

#define XBIT_NONE								0L
#define XBIT_FORCE_SM3							1L
#define XBIT_SLOW_PIXEL_OIT						2L

struct DX11SharedData;
struct DX11Cache;
class RingBuffer;

struct DX11Data : H3DData
{
	//HD3DEnumeration		*enumeration;			// available D3D settings: adapters, devices, modes, etc
	//HD3DSettings			*settings;				// current D3D settings: adapter, device, mode, formats, etc

	ID3D11Device			*pDevice;				// The D3D rendering device (allocated on sd)
	ID3D11DeviceContext		*pContext;				// The D3D device immediate context (allocated on sd)
	ID3DUserDefinedAnnotation * pTrace;

	DXGI_SWAP_CHAIN_DESC	swap_chain_desc;		// swap chain description (presentation parameters)
	IDXGISwapChain			*pSwapChain;			// The swap chain
	DX11SharedData			*sd;					// Pointer to shared data

	//spriting
	QuickMovesRegion *image_regions;
	QuickMovesRegion *depth_regions;


	// Support for get_pixels
	DXGI_FORMAT get_pixels_format;
	int get_pixels_width;
	int get_pixels_height;
	ID3D11Texture2D *get_pixels_staging_buffer;

	DX11Cache				*dx11cache;
	HDX11RenderStates		*pRenderStates;
	//cache the current rendertargets
	ID3D11RenderTargetView	*current_render_targets[RenderTargets::MAX_TARGETS];
	ID3D11DepthStencilView	*current_depth_stencil;	// depthstencil

	HDX11RenderTarget		*backbuffer;

	bool					debug_shadow_map;

	D3D11_INPUT_ELEMENT_DESC	vl_scratch[15];		// scratch space for creating vertex layouts

	struct OITData
	{
		ID3D11Buffer *vertices;
		ID3D11InputLayout *layout;

		ID3D11VertexShader *vertex_shader;
		ID3D11PixelShader *pixel_shader;

		ID3D11Buffer *fragments;
		ID3D11Buffer *heads;

		ID3D11UnorderedAccessView *fragments_uav;
		ID3D11UnorderedAccessView *heads_uav;

		ID3D11ShaderResourceView *fragments_srv;
		ID3D11ShaderResourceView *heads_srv;

		ID3D11Buffer *constants;

	} oit_data;

	struct HighlightMaps
	{
		ID3D11ComputeShader *add_shader;
		ID3D11ComputeShader *remove_shader;
		ID3D11ComputeShader *direct_add_shader;
		ID3D11ComputeShader *direct_remove_shader;

		ID3D11Buffer *constants;

	} highlight_maps;

	struct LinePatternCS
	{
		ID3D11ComputeShader *clip_shader;
		ID3D11ComputeShader *compute_lengths_shader;
		ID3D11ComputeShader *sum_shader;

		ID3D11Buffer *constants;
	} linepat_cs;

	struct EdgePatternCS
	{
		ID3D11ComputeShader *clip_shader;
		ID3D11ComputeShader *compute_lengths_shader;

		ID3D11Buffer *constants;
	} edgepat_cs;

	struct DepthBlit
	{
		struct Vertex
		{
			Point2D Position;
			Point2D TexCoord;
		};

		ID3D11VertexShader	*vertex_shader;
		ID3D11PixelShader	*pixel_shader_msaa;
		ID3D11PixelShader	*pixel_shader;
		ID3D11Buffer		*vertices;
		ID3D11InputLayout	*layout;

		ID3D11Buffer *constants;
	} depth_blit;

	struct LocalIBOBuffers
	{
		RingBuffer * verts;
		RingBuffer * indices;
	} local_ibo_buffers;

	HRESULT hr;
};

static const char * get_vs_profile(ShaderModel sm)
{
	switch (sm)
	{
		case SHADER_MODEL_5: return "vs_5_0";
		case SHADER_MODEL_4: return "vs_4_0";
		case SHADER_MODEL_3: return "vs_4_0";
		default: return "vs_4_0_level_9_1";
	}
}

static const char * get_ps_profile(ShaderModel sm)
{
	switch (sm)
	{
		case SHADER_MODEL_5: return "ps_5_0";
		case SHADER_MODEL_4: return "ps_4_0";
		case SHADER_MODEL_3: return "ps_4_0";
		default: return "ps_4_0_level_9_1";
	}
}

static const char * get_gs_profile(ShaderModel sm)
{
	switch (sm)
	{
		case SHADER_MODEL_5: return "gs_5_0";
		case SHADER_MODEL_4: return "gs_4_0";
		case SHADER_MODEL_3: return "gs_4_0";
		default: return null;
	}
}

// BITWISE components of the D3D11_FILTER enumeration
#define D3D11_FILTER_MIP_LINEAR		0x0001
#define D3D11_FILTER_MAG_LINEAR		0x0004
#define D3D11_FILTER_MIN_LINEAR		0x0010
#define D3D11_FILTER_ANISOTROPY		0x0055
#define D3D11_FILTER_COMPARISON		0x0080

struct DX11Cache
{
	H3DPRIMITIVETYPE	primitive_type;
	unsigned int		current_vb_offset[12];
	ID3D11Buffer		*current_vb[12];
	ID3D11Buffer		*current_ib;

	float				point_sprites_min;
	float				point_sprites_max;
};

struct DX11SharedData
{
	int						ref_count;
	ID3D11Device			*pDevice;				// The D3D rendering device
	ID3D11DeviceContext		*pContext;				// The D3D device immediate context	
	IDXGIFactory1			*pFactory;				// The DXGI factory	

	D3D_FEATURE_LEVEL		feature_level;			// The D3D feature level (11.0, 10.1, or 10.0)

	void					*cache;					// shared h3dcache
	DX11Cache				dx11cache;
	HDX11RenderStates		*pRenderStates;

	Mutex					mutex;
};


class HDX11RenderStates : public CMO
{
	ID3D11Device * pDevice;
	ID3D11DeviceContext * pContext;
	H3DCache * cache;

public:

	HDX11RenderStates(DX11SharedData * sd) : pDevice(sd->pDevice), pContext(sd->pContext), cache((H3DCache*)sd->cache)
	{
		for (int i = 0; i < H3D_MAX_TEXTURES; i++) {
			sampler_desc[i].Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampler_desc[i].AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampler_desc[i].AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampler_desc[i].AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
			sampler_desc[i].MinLOD = -FLT_MAX;
			sampler_desc[i].MaxLOD = FLT_MAX;
			sampler_desc[i].MipLODBias = 0.0f;
			sampler_desc[i].MaxAnisotropy = 16;
			sampler_desc[i].ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampler_desc[i].BorderColor[0] = 0;
			sampler_desc[i].BorderColor[1] = 0;
			sampler_desc[i].BorderColor[2] = 0;
			sampler_desc[i].BorderColor[3] = 0;
		}

		rasterizer_desc.FillMode = D3D11_FILL_SOLID;
		rasterizer_desc.CullMode = D3D11_CULL_NONE;
		rasterizer_desc.FrontCounterClockwise = false;
		rasterizer_desc.DepthBias = 0;
		rasterizer_desc.SlopeScaledDepthBias = 0.0f;
		rasterizer_desc.DepthBiasClamp = 0.0f;
		rasterizer_desc.DepthClipEnable = true;
		rasterizer_desc.ScissorEnable = false;
		rasterizer_desc.MultisampleEnable = false;
		rasterizer_desc.AntialiasedLineEnable = false;

		blend_desc.AlphaToCoverageEnable = false;
		blend_desc.IndependentBlendEnable = TRUE;
		blend_desc.RenderTarget[0].BlendEnable = false;
		blend_desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blend_desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blend_desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		depth_stencil_desc.DepthEnable = true;
		depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS;
		depth_stencil_desc.StencilEnable = false;
		depth_stencil_desc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
		depth_stencil_desc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
		depth_stencil_desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depth_stencil_desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depth_stencil_desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

		viewport_desc.MinDepth = 0;
		viewport_desc.MaxDepth = 1;
	}
	~HDX11RenderStates()
	{
	}

	void ApplyBlendStates()
	{
		if (blend_state_dirty) {
			ID3D11BlendState *blend_state = null;
			pDevice->CreateBlendState(&blend_desc, &blend_state);

			float blend_factor[] = { 1, 1, 1, 1 };
			pContext->OMSetBlendState(blend_state, blend_factor, 0xffffffff);
			if (blend_state) blend_state->Release();
		}
		blend_state_dirty = false;
	}
	void ApplySamplerStates()
	{
		if (sampler_state_dirty) {
			ID3D11SamplerState *sampler_state[H3D_MAX_TEXTURES];

			for (int i = 0; i < H3D_MAX_TEXTURES; i++) {
				pDevice->CreateSamplerState(&sampler_desc[i], &sampler_state[i]);
			}
			pContext->PSSetSamplers(0, H3D_MAX_TEXTURES, &sampler_state[0]);

			for (int i = 0; i < H3D_MAX_TEXTURES; i++) {
				H_SAFE_RELEASE(sampler_state[i]);
			}
		}
		sampler_state_dirty = false;
	}
	void ApplyDepthStencilStates()
	{
		if (depth_stencil_state_dirty) {
			ID3D11DepthStencilState *depth_stencil_state;
			pDevice->CreateDepthStencilState(&depth_stencil_desc, &depth_stencil_state);
			pContext->OMSetDepthStencilState(depth_stencil_state, cache->stencil_ref);
			H_SAFE_RELEASE(depth_stencil_state);
		}
		depth_stencil_state_dirty = false;
	}
	void ApplyRasterizerStates()
	{
		if (rasterizer_state_dirty) {
			ID3D11RasterizerState *rasterizer_state;
			pDevice->CreateRasterizerState(&rasterizer_desc, &rasterizer_state);
			pContext->RSSetState(rasterizer_state);
			H_SAFE_RELEASE(rasterizer_state);
		}
		rasterizer_state_dirty = false;
	}

	void ApplyRenderStates()
	{
		ApplyBlendStates();
		ApplySamplerStates();
		ApplyDepthStencilStates();
		ApplyRasterizerStates();
	}

	// Render States Scratch Space
	D3D11_DEPTH_STENCIL_DESC	depth_stencil_desc;
	D3D11_BLEND_DESC			blend_desc;
	D3D11_SAMPLER_DESC			sampler_desc[H3D_MAX_TEXTURES];
	D3D11_RASTERIZER_DESC		rasterizer_desc;
	D3D11_VIEWPORT				viewport_desc;
	bool						depth_stencil_state_dirty;
	bool						blend_state_dirty;
	bool						sampler_state_dirty;
	bool						rasterizer_state_dirty;
};


static D3D11_TEXTURE_ADDRESS_MODE dx11textureaddress(H3DTEXTUREADDRESS type);
static D3D11_FILTER dx11filter(H3DTEXTUREFILTERTYPE min, H3DTEXTUREFILTERTYPE mag, H3DTEXTUREFILTERTYPE mip);

/*
 * ENSURE MACROS
 */
INLINE static void FORCE_TEXTURE_WRAP_S(DX11Data * dx11data, int unit, H3DTEXTUREADDRESS mode) {
	dx11data->pRenderStates->sampler_desc[unit].AddressU = dx11textureaddress(mode);
	dx11data->cache->texture_wrap_s[unit] = mode;
	dx11data->pRenderStates->sampler_state_dirty = true;
}

#define ENSURE_TEXTURE_WRAP_S(dx11data, unit, mode) SEMI_PROTECT(\
	if ((dx11data)->cache->texture_wrap_s[unit] != mode) \
		FORCE_TEXTURE_WRAP_S((dx11data), unit, mode); \
)

INLINE static void FORCE_TEXTURE_WRAP_T(DX11Data * dx11data, int unit, H3DTEXTUREADDRESS mode) {
	dx11data->pRenderStates->sampler_desc[unit].AddressV = dx11textureaddress(mode);
	dx11data->cache->texture_wrap_t[unit] = mode;
	dx11data->pRenderStates->sampler_state_dirty = true;
}

#define ENSURE_TEXTURE_WRAP_T(dx11data, unit, mode) SEMI_PROTECT(\
	if ((dx11data)->cache->texture_wrap_t[unit] != mode) \
		FORCE_TEXTURE_WRAP_T((dx11data), unit, mode); \
)

INLINE static void FORCE_TEXTURE_MIN_FILTER(DX11Data * dx11data, int unit, H3DTEXTUREFILTERTYPE mode)
{	
	auto & cache = dx11data->cache;
	cache->texture_min_filter[unit] = mode;

	D3D11_SAMPLER_DESC & sampler_desc = dx11data->pRenderStates->sampler_desc[unit];
	sampler_desc.Filter = dx11filter((H3DTEXTUREFILTERTYPE)cache->texture_min_filter[unit], (H3DTEXTUREFILTERTYPE)cache->texture_mag_filter[unit], (H3DTEXTUREFILTERTYPE)cache->texture_mip_filter[unit]);

	dx11data->pRenderStates->sampler_state_dirty = true;
}

#define ENSURE_TEXTURE_MIN_FILTER(dx11data, unit, mode) SEMI_PROTECT(\
	if ((dx11data)->cache->texture_min_filter[unit] != mode) \
		FORCE_TEXTURE_MIN_FILTER((dx11data), unit, mode); \
)

INLINE static void FORCE_TEXTURE_MAG_FILTER(DX11Data * dx11data, int unit, H3DTEXTUREFILTERTYPE mode)
{
	auto & cache = dx11data->cache;
	cache->texture_mag_filter[unit] = mode;

	D3D11_SAMPLER_DESC & sampler_desc = dx11data->pRenderStates->sampler_desc[unit];
	sampler_desc.Filter = dx11filter((H3DTEXTUREFILTERTYPE)cache->texture_min_filter[unit], (H3DTEXTUREFILTERTYPE)cache->texture_mag_filter[unit], (H3DTEXTUREFILTERTYPE)cache->texture_mip_filter[unit]);

	dx11data->pRenderStates->sampler_state_dirty = true;
}

#define ENSURE_TEXTURE_MAG_FILTER(dx11data, unit, mode) SEMI_PROTECT(\
	if ((dx11data)->cache->texture_mag_filter[unit] != mode) \
		FORCE_TEXTURE_MAG_FILTER((dx11data), unit, mode); \
)

INLINE static void FORCE_TEXTURE_MIP_FILTER(DX11Data * dx11data, int unit, H3DTEXTUREFILTERTYPE mode)
{
	auto & cache = dx11data->cache;
	cache->texture_mip_filter[unit] = mode;

	D3D11_SAMPLER_DESC & sampler_desc = dx11data->pRenderStates->sampler_desc[unit];
	sampler_desc.Filter = dx11filter((H3DTEXTUREFILTERTYPE)cache->texture_min_filter[unit], (H3DTEXTUREFILTERTYPE)cache->texture_mag_filter[unit], (H3DTEXTUREFILTERTYPE)cache->texture_mip_filter[unit]);
	
	if (mode == H3DTEXF_LINEAR) {		
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	}
	else if (mode == H3DTEXF_POINT) {		
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	}
	else {		
		sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;
	}	
	
	dx11data->pRenderStates->sampler_state_dirty = true; 
}

#define ENSURE_TEXTURE_MIP_FILTER(dx11data, unit, mode) SEMI_PROTECT(\
	if ((dx11data)->cache->texture_mip_filter[unit] != mode) \
		FORCE_TEXTURE_MIP_FILTER((dx11data), unit, mode); \
)


#define FORCE_PRIMITIVE_TOPOLOGY(dx11data, mode) SEMI_PROTECT(\
	dx11data->pContext->IASetPrimitiveTopology(dx11type(mode)); \
	dx11data->dx11cache->primitive_type = mode; \
)
#define ENSURE_PRIMITIVE_TOPOLOGY(dx11data, mode) SEMI_PROTECT(\
	if (dx11data->dx11cache->primitive_type != mode) \
		FORCE_PRIMITIVE_TOPOLOGY(dx11data, mode); \
)


#define ENSURE_STREAM_SOURCE(dx11data,streamindex,vb,stride,offset) SEMI_PROTECT(\
	if (vb != dx11data->dx11cache->current_vb[streamindex] || \
		offset != dx11data->dx11cache->current_vb_offset[streamindex]) { \
		dx11data->pContext->IASetVertexBuffers (streamindex, 1, &vb, &stride, &offset); \
		dx11data->dx11cache->current_vb[streamindex] = vb; \
		dx11data->dx11cache->current_vb_offset[streamindex] = offset; \
	} \
)

/*****************************************************************************
*****************************************************************************
							Enumeration Conversions
*****************************************************************************
*****************************************************************************/
INLINE static D3D11_BLEND dx11blend(H3DBLEND type)
{
	switch (type) {
		case H3DBLEND_ZERO:				return D3D11_BLEND_ZERO;
		case H3DBLEND_ONE:				return D3D11_BLEND_ONE;
		case H3DBLEND_SRCCOLOR:         return D3D11_BLEND_SRC_COLOR;
		case H3DBLEND_INVSRCCOLOR:      return D3D11_BLEND_INV_SRC_COLOR;
		case H3DBLEND_SRCALPHA:         return D3D11_BLEND_SRC_ALPHA;
		case H3DBLEND_INVSRCALPHA:      return D3D11_BLEND_INV_SRC_ALPHA;
		case H3DBLEND_DESTALPHA:        return D3D11_BLEND_DEST_ALPHA;
		case H3DBLEND_INVDESTALPHA:     return D3D11_BLEND_INV_DEST_ALPHA;
		case H3DBLEND_DESTCOLOR:        return D3D11_BLEND_DEST_COLOR;
		case H3DBLEND_INVDESTCOLOR:     return D3D11_BLEND_INV_DEST_COLOR;
		case H3DBLEND_SRCALPHASAT:      return D3D11_BLEND_SRC_ALPHA_SAT;
		case H3DBLEND_BLENDFACTOR:      return D3D11_BLEND_BLEND_FACTOR;
		case H3DBLEND_INVBLENDFACTOR:   return D3D11_BLEND_INV_BLEND_FACTOR;
		default:
			HE_PROCEDURAL_ERROR("Unhandled blend type mapping.");
			return D3D11_BLEND_ZERO;
	}
}

INLINE static D3D11_COLOR_WRITE_ENABLE dx11colormask(int colormask)
{
	int mask = 0;

	mask |= ANYBIT(colormask, H3DMASK_R) ? D3D11_COLOR_WRITE_ENABLE_RED : 0;
	mask |= ANYBIT(colormask, H3DMASK_G) ? D3D11_COLOR_WRITE_ENABLE_GREEN : 0;
	mask |= ANYBIT(colormask, H3DMASK_B) ? D3D11_COLOR_WRITE_ENABLE_BLUE : 0;
	mask |= ANYBIT(colormask, H3DMASK_A) ? D3D11_COLOR_WRITE_ENABLE_ALPHA : 0;
	return (D3D11_COLOR_WRITE_ENABLE) mask;
}

INLINE static D3D11_COMPARISON_FUNC dx11cmpfunc(H3DCMPFUNC type)
{
	switch (type) {
		case H3DCMP_NEVER:			return D3D11_COMPARISON_NEVER;
		case H3DCMP_LESS:			return D3D11_COMPARISON_LESS;
		case H3DCMP_EQUAL:			return D3D11_COMPARISON_EQUAL;
		case H3DCMP_LESSEQUAL:		return D3D11_COMPARISON_LESS_EQUAL;
		case H3DCMP_GREATER:		return D3D11_COMPARISON_GREATER;
		case H3DCMP_NOTEQUAL:		return D3D11_COMPARISON_NOT_EQUAL;
		case H3DCMP_GREATEREQUAL:	return D3D11_COMPARISON_GREATER_EQUAL;
		case H3DCMP_ALWAYS:			return D3D11_COMPARISON_ALWAYS;
		default:
			HE_PROCEDURAL_ERROR("Unhandled comparison function mapping.");
			return D3D11_COMPARISON_NEVER;
	}
}

INLINE static D3D11_CULL_MODE dx11cullmode(H3DCULL type)
{
	switch (type) {
		case H3DCULL_NONE:			return D3D11_CULL_NONE;
		case H3DCULL_CW:			return D3D11_CULL_FRONT;
		case H3DCULL_CCW:			return D3D11_CULL_BACK;
		default:
			HE_PROCEDURAL_ERROR("Unhandled cull mode function mapping.");
			return D3D11_CULL_NONE;
	}
}

INLINE static DXGI_FORMAT dx11format(H3DFORMAT type)
{
	switch (type) {
		case H3DFMT_A8R8G8B8:		return DXGI_FORMAT_R8G8B8A8_UNORM;
		case H3DFMT_DXT1:			return DXGI_FORMAT_BC1_UNORM;
		case H3DFMT_DXT3:			return DXGI_FORMAT_BC2_UNORM;
		case H3DFMT_DXT5:			return DXGI_FORMAT_BC3_UNORM;
		case H3DFMT_D24S8:			return DXGI_FORMAT_D24_UNORM_S8_UINT;
		//case H3DFMT_L8:				return DXGI_FORMAT_R8_UNORM;
		case H3DFMT_L8:				return DXGI_FORMAT_R8G8B8A8_UNORM;
		case H3DFMT_R8G8B8:			return DXGI_FORMAT_R8G8B8A8_UNORM;
		case H3DFMT_R32F:			return DXGI_FORMAT_R32_FLOAT;
		case H3DFMT_INDEX16:		return DXGI_FORMAT_R16_UINT;
		case H3DFMT_INDEX32:		return DXGI_FORMAT_R32_UINT;
		case H3DFMT_R32_UINT:		return DXGI_FORMAT_R32_UINT;
		case H3DFMT_A8B8G8R8:		return DXGI_FORMAT_B8G8R8A8_UNORM;
		case H3DFMT_X8R8G8B8:		return DXGI_FORMAT_B8G8R8X8_UNORM;
		default:
			HE_PROCEDURAL_ERROR("Unhandled format function mapping.");
			return DXGI_FORMAT_R8G8B8A8_UNORM;
	}
}

INLINE static D3D11_STENCIL_OP dx11stencilop(H3DSTENCILOP type)
{
	switch (type) {
		case H3DSTENCILOP_KEEP:			return D3D11_STENCIL_OP_KEEP;
		case H3DSTENCILOP_ZERO:			return D3D11_STENCIL_OP_ZERO;
		case H3DSTENCILOP_REPLACE:		return D3D11_STENCIL_OP_REPLACE;
		case H3DSTENCILOP_INCRSAT:		return D3D11_STENCIL_OP_INCR_SAT;
		case H3DSTENCILOP_DECRSAT:		return D3D11_STENCIL_OP_DECR_SAT;
		case H3DSTENCILOP_INVERT:		return D3D11_STENCIL_OP_INVERT;
		case H3DSTENCILOP_INCR:			return D3D11_STENCIL_OP_INCR;
		case H3DSTENCILOP_DECR:			return D3D11_STENCIL_OP_DECR;
		default:
			HE_PROCEDURAL_ERROR("Unhandled stencil op mapping.");
			return D3D11_STENCIL_OP_KEEP;
	}
}

INLINE static D3D11_TEXTURE_ADDRESS_MODE dx11textureaddress(H3DTEXTUREADDRESS type)
{
	switch (type) {
		case H3DTADDRESS_WRAP:			return D3D11_TEXTURE_ADDRESS_WRAP;
		case H3DTADDRESS_MIRROR:		return D3D11_TEXTURE_ADDRESS_MIRROR;
		case H3DTADDRESS_CLAMP:			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case H3DTADDRESS_BORDER:		return D3D11_TEXTURE_ADDRESS_BORDER;
		case H3DTADDRESS_MIRRORONCE:	return D3D11_TEXTURE_ADDRESS_MIRROR_ONCE;
		default:
			HE_PROCEDURAL_ERROR("Unhandled shade mode mapping.");
			return D3D11_TEXTURE_ADDRESS_WRAP;
	}
}


INLINE static D3D11_FILTER_TYPE dx11texturefilter(H3DTEXTUREFILTERTYPE type)
{
	switch (type) {
		case H3DTEXF_NONE:				return D3D11_FILTER_TYPE_POINT;
		case H3DTEXF_POINT:				return D3D11_FILTER_TYPE_POINT;
		case H3DTEXF_LINEAR:			return D3D11_FILTER_TYPE_LINEAR;
		default:
			HE_PROCEDURAL_ERROR("Unhandled texture filter mapping.");
			return D3D11_FILTER_TYPE_POINT;
	}
}

INLINE static D3D11_PRIMITIVE_TOPOLOGY dx11type(H3DPRIMITIVETYPE type)
{
	switch (type) {
		case H3DPT_POINTLIST:			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
		case H3DPT_LINELIST:			return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
		case H3DPT_LINESTRIP:			return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
		case H3DPT_TRIANGLELIST:		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		case H3DPT_TRIANGLESTRIP:		return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
		case H3DPT_LINELIST_ADJ:		return D3D11_PRIMITIVE_TOPOLOGY_LINELIST_ADJ;
		case H3DPT_LINESTRIP_ADJ:		return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ;
		default:
			HE_PROCEDURAL_ERROR("Unhandled primitive topology type mapping.");
			return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
	}
}

INLINE static D3D11_FILTER dx11filter(H3DTEXTUREFILTERTYPE min, H3DTEXTUREFILTERTYPE mag, H3DTEXTUREFILTERTYPE mip)
{
	int dx11filter = 0;

	if (min == H3DTEXF_LINEAR)
		dx11filter |= D3D11_FILTER_MIN_LINEAR;
	if (mag == H3DTEXF_LINEAR)
		dx11filter |= D3D11_FILTER_MAG_LINEAR;
	if (mip == H3DTEXF_LINEAR)
		dx11filter |= D3D11_FILTER_MIP_LINEAR;

	if (min == H3DTEXF_ANISOTROPIC || mag == H3DTEXF_ANISOTROPIC || mip == H3DTEXF_ANISOTROPIC)
		dx11filter |= D3D11_FILTER_ANISOTROPIC;

	return (D3D11_FILTER) dx11filter;
}

INLINE static D3D11_VIEWPORT dx11viewport(H3DViewport const *hvp)
{
	D3D11_VIEWPORT vp;
	vp.TopLeftX = (float) hvp->X;
	vp.TopLeftY = (float) hvp->Y;
	vp.Width = (float) hvp->Width;
	vp.Height = (float) hvp->Height;
	vp.MinDepth = (float) hvp->MinZ;
	vp.MaxDepth = (float) hvp->MaxZ;
	return vp;
}

/*****************************************************************************
*****************************************************************************
							DX11Data
*****************************************************************************
*****************************************************************************/

void set_targets(DX11Data& dx11data, ID3D11RenderTargetView *target0, 
	   ID3D11DepthStencilView *zbuffer = null, 
	   ID3D11RenderTargetView *target1 = null, 
	   ID3D11RenderTargetView *target2 = null, 
	   ID3D11RenderTargetView *target3 = null);
void set_targets(DX11Data& dx11data, 
	   H3DRenderTarget *target0, 
	   H3DRenderTarget *zbuffer = null, 
	   H3DRenderTarget *target1 = null, 
	   H3DRenderTarget *target2 = null, 
	   H3DRenderTarget *target3 = null);

#define DX11D(dc) ((DX11Data *)((dc)->data2))
#define DX11NRD(nr) (DX11D((nr)->display_context))

local void invalidate_cache (DX11Data *dx11data)
{
	if (dx11data->cache)
		SET_MEMORY (dx11data->cache, sizeof(H3DCache), 0x5c);
	if (dx11data->dx11cache)
		SET_MEMORY (dx11data->dx11cache, sizeof(DX11Cache), 0x5c);
}



/*****************************************************************************
*****************************************************************************
							HDX11Shader Class
*****************************************************************************
*****************************************************************************/
ID3D11InputLayout * CreateInputLayoutFromVF(DX11Data *dx11data, H3DVertexFormat VF);

local int shader_datatype_size(D3D10_SHADER_VARIABLE_CLASS type) {
	switch(type) {
		case D3D10_SVC_SCALAR: return 4;
		case D3D10_SVC_VECTOR: return 16;
		case D3D10_SVC_MATRIX_ROWS: return 64;
		default: ASSERT(0);		// TODO: handle other cases
	}
	return 0;
}

class HDX11ShaderConstants
{
private:
	DX11Data *m_dx11data;
	float **m_buffer;
	bool *m_buffer_touched[SHC_END];

	ID3D11Buffer **m_pConstants;
	bool m_pConstantsTouched[15];		// max number of cbuffers

	ID3D11ShaderReflection *m_shader_reflection;
	D3D11_SHADER_DESC m_shader_desc;

public:
	HDX11ShaderConstants(DX11Data *dx11data, ID3D10Blob *shader_blob) : m_dx11data(dx11data)
	{
		for (int i = 0; i < 15; i++)
			m_pConstantsTouched[i] = false;

		// Get shader reflection
		D3DReflect(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void **)&m_shader_reflection);
		m_shader_reflection->GetDesc(&m_shader_desc);

		// Create constant buffers (and scratch spaces)
		m_buffer = new float*[m_shader_desc.ConstantBuffers];
		m_pConstants = new ID3D11Buffer*[m_shader_desc.ConstantBuffers];
		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) 
		{
			ID3D11ShaderReflectionConstantBuffer * cbuf = m_shader_reflection->GetConstantBufferByIndex(i);

			D3D11_SHADER_BUFFER_DESC cdesc;
			cbuf->GetDesc(&cdesc);
	
			// Create constant buffer scratch space
			m_buffer[i] = new float[cdesc.Size / 4];
			SET_MEMORY(m_buffer[i], cdesc.Size, 1);

			// Create constant buffer
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			// Align to 16b to satisfy constant buffer size requirements.
			bd.ByteWidth = (cdesc.Size + 15) & ~15;
			dx11data->pDevice->CreateBuffer(&bd, null, &m_pConstants[i]);			
		}
	}
	~HDX11ShaderConstants()
	{
		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			H_SAFE_RELEASE(m_pConstants[i]);
			delete [] m_buffer[i];
		}
		delete [] m_pConstants;
		delete [] m_buffer;
	}


	void SetConstantInt(H3D_Shader_Handle *cache, int shc, int data)
	{
		int *constant = (int*) cache[shc];
		if (constant) {
			if (EQUAL_MEMORY(constant, sizeof(int), &data))
				return;
			*constant = data;
			*m_buffer_touched[shc] = true;
		}
	}

	void SetConstantUInt(H3D_Shader_Handle *cache, int shc, unsigned int data)
	{
		unsigned int *constant = (unsigned int*) cache[shc];
		if (constant) {
			if (EQUAL_MEMORY(constant, sizeof(unsigned int), &data))
				return;
			*constant = data;
			*m_buffer_touched[shc] = true;
		}
	}

	void SetConstantFloat(H3D_Shader_Handle *cache, int shc, float data)
	{
		float *constant = (float*) cache[shc];
		if (constant) {
			if (EQUAL_MEMORY(constant, sizeof(float), &data))
				return;
			*constant = data;
			*m_buffer_touched[shc] = true;
		}
	}

	void SetConstantArray(H3D_Shader_Handle *cache, int shc, void const *data, int datatype, int num = 1)
	{
		float *constant = (float*) cache[shc];

		if (constant) {
			// Some datatypes need special handling, for example, a float3 or float3x3 is actually treated as
			// a float4 or float4x4 in the constant buffer
			if (datatype == H3D_Matrix && num == 9) {
				for (int i = 0; i < 3; i++) {
					if (!EQUAL_MEMORY((float*)data + i*3, 3*sizeof(float), constant + i*4)) {
						COPY_MEMORY((float*)data + i*3, 3*sizeof(float), constant + i*4);
						*m_buffer_touched[shc] = true;
					}
				}
			}
			else {
				if (EQUAL_MEMORY(data, num*sizeof(float), constant))
					return;
				COPY_MEMORY(data, num*sizeof(float), constant);
				*m_buffer_touched[shc] = true;
			}
		}
	}

	void SetConstantVector(H3D_Shader_Handle *cache, int shc, float const * data, int width)
	{
		float *constant = (float*) cache[shc];
		if (constant) {
			if (EQUAL_MEMORY(data, sizeof(float)*width, constant))
				return;
			COPY_MEMORY(data, sizeof(float)*width, constant);
			*m_buffer_touched[shc] = true;
		}
	}
	void SetConstantVectorArray(H3D_Shader_Handle *cache, int shc, float const *data, int num_vectors, int width)
	{
		float *constant = (float*) cache[shc];
		if (constant) {
			for (int i = 0; i < num_vectors; i++) {
				if (!EQUAL_MEMORY(data+i*width, sizeof(float)*width, constant+i*4)) {
					COPY_MEMORY(data+i*width, sizeof(float)*width, constant+i*4);
					*m_buffer_touched[shc] = true;
				}
			}
		}
	}

	void SetUncachedConstantVector(H3D_Shader_Handle handle, float const *data, int width)
	{
		if (!handle)
			return;
		float *constant = (float*) handle;
		if (constant)
			COPY_MEMORY(data, sizeof(float)*width, constant);
	}
	void SetUncachedConstantFloat(H3D_Shader_Handle handle, float data)
	{
		if (!handle)
			return;
		float *constant = (float*) handle;
		COPY_MEMORY(&data, sizeof(float), constant);
	}

	void SetUncachedConstantInt(H3D_Shader_Handle handle, int data)
	{
		if (!handle)
			return;
		int *constant = (int*) handle;
		COPY_MEMORY(&data, sizeof(int), constant);
	}

	H3D_Shader_Handle GetConstantByName(int shc, char const * parent, char const * constant)
	{
		ID3D11ShaderReflectionConstantBuffer *cbuf;
		char scratch[1024];
		HRESULT hr = S_OK;

		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			cbuf = m_shader_reflection->GetConstantBufferByIndex(i);

			D3D11_SHADER_VARIABLE_DESC vdesc;

			if (parent) {
				int parent_struct_size = 0;
				int array_index = -1;
				strcpy(scratch, parent); 
				parent = &scratch[0];
				char *array_index_str = strrchr(scratch, '[');
				if (array_index_str) {
					// We have an array, so isolate the index from the string
					*array_index_str = 0;
					array_index_str++;

					char *array_end = strrchr(array_index_str, ']');
					*array_end = 0;
					array_index = atoi(array_index_str);
				}

				D3D11_SHADER_VARIABLE_DESC parent_desc;
				ID3D11ShaderReflectionVariable *parent_var = cbuf->GetVariableByName(parent);
				hr = parent_var->GetDesc(&parent_desc);

				D3D11_SHADER_TYPE_DESC parent_type_desc;
				ID3D11ShaderReflectionType *parent_type = parent_var->GetType();
				hr = parent_type->GetDesc(&parent_type_desc);

				D3D11_SHADER_TYPE_DESC member_type_desc;
				ID3D11ShaderReflectionType *member_type = parent_var->GetType()->GetMemberTypeByName(constant);
				hr = member_type->GetDesc(&member_type_desc);

				if (array_index > 0) {
					// Calculate the parent struct size (by querying the last item in the struct)
					D3D11_SHADER_TYPE_DESC last_member_type_desc;
					ID3D11ShaderReflectionType *last_member_type = parent_var->GetType()->GetMemberTypeByIndex(parent_type_desc.Members-1);
					hr = last_member_type->GetDesc(&last_member_type_desc);

					int last_member_size = shader_datatype_size(last_member_type_desc.Class);
					parent_struct_size = last_member_type_desc.Offset + last_member_size;
					parent_desc.StartOffset += parent_struct_size*array_index;

					if (array_index >= (int)parent_type_desc.Elements)
						hr = E_FAIL;
				}

				if (SUCCEEDED(hr)) {
					int variable_offset = (parent_desc.StartOffset + member_type_desc.Offset) / 4;
					if (shc >= 0)
						m_buffer_touched[shc] = &m_pConstantsTouched[i];
					return (H3D_Shader_Handle) &m_buffer[i][variable_offset];
				}
			}
			else {
				int array_offset = 0;

				strcpy(scratch, constant); 
				constant = &scratch[0];
				int array_index = -1;
				char *array_index_str = strrchr(scratch, '[');
				if (array_index_str) {
					// We have an array, so isolate the index from the string
					*array_index_str = 0;
					array_index_str++;

					char *array_end = strrchr(array_index_str, ']');
					*array_end = 0;
					array_index = atoi(array_index_str);
				}

				ID3D11ShaderReflectionVariable *var = cbuf->GetVariableByName(constant);
				hr = var->GetDesc(&vdesc);

				D3D11_SHADER_TYPE_DESC constant_type_desc;
				ID3D11ShaderReflectionType *constant_type = var->GetType();
				hr = constant_type->GetDesc(&constant_type_desc);

				if (array_index > 0) {
					array_offset = array_index * shader_datatype_size(constant_type_desc.Class);
				}
				
				if (SUCCEEDED(hr)) {
					if (shc >= 0)
						m_buffer_touched[shc] = &m_pConstantsTouched[i];
					return (H3D_Shader_Handle) &m_buffer[i][(vdesc.StartOffset + array_offset) / 4];
				}
			}
		}
		return 0;
	}

	void ApplyConstants(H3D_Constant_Table table)
	{
		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			if (m_pConstantsTouched[i]) {
				m_dx11data->pContext->UpdateSubresource(m_pConstants[i], 0, null, m_buffer[i], 0, 0);
				m_pConstantsTouched[i] = false;
			}
		}

		if (!m_shader_desc.ConstantBuffers)
			return;

		switch(table) {
			case H3D_VSConstant_Table:
				m_dx11data->pContext->VSSetConstantBuffers(0, m_shader_desc.ConstantBuffers, m_pConstants);
				break;

			case H3D_PSConstant_Table:
				m_dx11data->pContext->PSSetConstantBuffers(0, m_shader_desc.ConstantBuffers, m_pConstants);
				break;

			case H3D_GSConstant_Table:
				m_dx11data->pContext->GSSetConstantBuffers(0, m_shader_desc.ConstantBuffers, m_pConstants);
				break;
		}
	}

	unsigned long GetSamplerIndex(const char *name) 
	{
		D3D11_SHADER_INPUT_BIND_DESC desc;
		HRESULT hr = S_OK;

		m_shader_reflection->GetResourceBindingDescByName(name, &desc);
		if (SUCCEEDED(hr)) {
			ASSERT(desc.Type == D3D10_SIT_SAMPLER);
			return desc.BindPoint;
		}
		return 0;
	};
};

class HDX11Shader : public H3DShader
{
protected:
	char const * get_shader_extension() const {return "hlsl";};
	HDX11ShaderConstants * get_shader_constants(H3D_Constant_Table table)
	{
		switch (table) {
			case H3D_VSConstant_Table: return m_pVSConstants;
			case H3D_PSConstant_Table: return m_pPSConstants;
			case H3D_GSConstant_Table: return m_pGSConstants;
		}
		return m_pVSConstants;
	}

public:
	ID3D11Device *m_pDevice;
	ID3D11DeviceContext *m_pContext;

	ID3D11VertexShader *m_pVS;
	ID3D11PixelShader *m_pPS;
	ID3D11GeometryShader *m_pGS;

	HDX11ShaderConstants *m_pVSConstants;
	HDX11ShaderConstants *m_pPSConstants;
	HDX11ShaderConstants *m_pGSConstants;

	ID3D10Blob *m_pVSBlob;
	ID3D10Blob *m_pPSBlob;
	ID3D10Blob *m_pGSBlob;

	ID3D11InputLayout *m_VL;
	ID3D11InputLayout *m_VL_interleaved;
	H3DVertexFormat m_VL_format;
	H3DVertexFormat m_VL_interleaved_format;
	
	HDX11Shader (const H3DShaderID& id);
	~HDX11Shader ();

	bool Create (H3DData *h3ddata, const char *material_shader_source = null);
	bool Activate (void);

	char *EmitTexture(char *ptr, HTextureDimension dimension, char const *name, int number, int sampler, HTexelType texel_type);

	void EnsureDepthPeeling (H3DTexture *texture, H3DData *h3ddata);
	void EnsureTextures(bool back_face = false);

	void EnsureInputLayout(H3DVertexFormat VF) {
		if (VF.get_interleaved()) {
			if (!m_VL_interleaved || m_VL_interleaved_format != VF) {
				H_SAFE_RELEASE(m_VL_interleaved);
				m_VL_interleaved = CreateInputLayoutFromVF((DX11Data*)m_h3ddata, VF);
				m_VL_interleaved_format = VF;
			}
			m_pContext->IASetInputLayout(m_VL_interleaved);
		}
		else {
			if (!m_VL || m_VL_format != VF) {
				H_SAFE_RELEASE(m_VL);
				m_VL = CreateInputLayoutFromVF((DX11Data*)m_h3ddata, VF);
				m_VL_format = VF;
			}
			m_pContext->IASetInputLayout(m_VL);
		}
		m_h3ddata->cache->vf = VF;
	}

	void SetConstantInt(H3D_Constant_Table table, int shc, int data)
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		constants->SetConstantInt(cache, shc, data);
	}

	void SetConstantUInt(H3D_Constant_Table table, int shc, unsigned int data)
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		constants->SetConstantUInt(cache, shc, data);
	}

	void SetConstantFloat(H3D_Constant_Table table, int shc, float data)
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		constants->SetConstantFloat(cache, shc, data);
	}

	void SetConstantArray(H3D_Constant_Table table, int shc, void const *data, int datatype, int num = 1)
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		constants->SetConstantArray(cache, shc, data, datatype, num);
	}

	void SetConstantVector(H3D_Constant_Table table, int shc, float const *data, int width)
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		constants->SetConstantVector(cache, shc, data, width);
	}
	void SetConstantVectorArray(H3D_Constant_Table table, int shc, float const *data, int num_vectors, int width) 
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		constants->SetConstantVectorArray(cache, shc, data, num_vectors, width);
	}

	H3D_Shader_Handle GetConstantByName(H3D_Constant_Table table, int shc, char const * parent, char const * constant)
	{
		HDX11ShaderConstants *constants = get_shader_constants(table);
		return constants->GetConstantByName(shc, parent, constant);
	}

	void ApplyConstants()
	{
		//Make sure alpha test parameters are setup (must be implemented in the shader starting dx10+)
		SET_CONSTANT_INT(H3D_PSConstant_Table, SHC_ALPHA_TEST, null, "AlphaTest", m_h3ddata->cache->alphatesting ? 1 : 0);

		m_pVSConstants->ApplyConstants(H3D_VSConstant_Table);
		m_pPSConstants->ApplyConstants(H3D_PSConstant_Table);

		if (m_id.HasGeometryShader())
			m_pGSConstants->ApplyConstants(H3D_GSConstant_Table);
	}

	void ApplyShaderStates(H3DData *h3ddata, H3DVertexFormat VF)
	{
		UNREFERENCED(h3ddata);
		ApplyConstants();
		EnsureInputLayout(VF);
	}
};


/*****************************************************************************
*****************************************************************************
						Vertex/Index Buffers
*****************************************************************************
*****************************************************************************/
ID3D11InputLayout * CreateInputLayoutFromVF(DX11Data *dx11data, H3DVertexFormat VF)
{
	HRESULT hr = S_OK;
	ID3D11InputLayout *result;
	bool interleaved = VF.get_interleaved();
	int slot = 0;
	int stream = 0;
	if (VF.get_position()) {
		dx11data->vl_scratch[slot].SemanticName = "SV_POSITION";
		dx11data->vl_scratch[slot].SemanticIndex = 0;
		dx11data->vl_scratch[slot].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		dx11data->vl_scratch[slot].InputSlot = stream;
		dx11data->vl_scratch[slot].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dx11data->vl_scratch[slot].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		dx11data->vl_scratch[slot].InstanceDataStepRate = 0;
		if (!interleaved)
			stream++;
		slot++;
	}
	if (VF.get_normals()) {
		dx11data->vl_scratch[slot].SemanticName = "NORMAL";
		dx11data->vl_scratch[slot].SemanticIndex = 0;
		dx11data->vl_scratch[slot].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		dx11data->vl_scratch[slot].InputSlot = stream;
		dx11data->vl_scratch[slot].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dx11data->vl_scratch[slot].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		dx11data->vl_scratch[slot].InstanceDataStepRate = 0;
		if (!interleaved)
			stream++;
		slot++;
	}
	if (VF.get_diffuse()) {
		dx11data->vl_scratch[slot].SemanticName = "COLOR";
		dx11data->vl_scratch[slot].SemanticIndex = 0;
		dx11data->vl_scratch[slot].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dx11data->vl_scratch[slot].InputSlot = stream;
		dx11data->vl_scratch[slot].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dx11data->vl_scratch[slot].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		dx11data->vl_scratch[slot].InstanceDataStepRate = 0;
		if (!interleaved)
			stream++;
		slot++;
	}
	if (VF.get_specular()) {
		dx11data->vl_scratch[slot].SemanticName = "COLOR";
		dx11data->vl_scratch[slot].SemanticIndex = 1;
		dx11data->vl_scratch[slot].Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		dx11data->vl_scratch[slot].InputSlot = stream;
		dx11data->vl_scratch[slot].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dx11data->vl_scratch[slot].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		dx11data->vl_scratch[slot].InstanceDataStepRate = 0;
		if (!interleaved)
			stream++;
		slot++;
	}
	for (int i = 0; i < VF.get_tex_count(); i++) {
		DXGI_FORMAT format = DXGI_FORMAT_R32_FLOAT;
		switch(VF.get_tex_size(i)) {
			case 2: format = DXGI_FORMAT_R32G32_FLOAT; break;
			case 3: format = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case 4: format = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
		}
		dx11data->vl_scratch[slot].SemanticName = "TEXCOORD";
		dx11data->vl_scratch[slot].SemanticIndex = i;
		dx11data->vl_scratch[slot].Format = format;
		dx11data->vl_scratch[slot].InputSlot = stream;
		dx11data->vl_scratch[slot].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		dx11data->vl_scratch[slot].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		dx11data->vl_scratch[slot].InstanceDataStepRate = 0;
		if (!interleaved)
			stream++;
		slot++;
	}

	ID3D10Blob * blob = static_cast<HDX11Shader const *>(dx11data->cache->Shader)->m_pVSBlob;
	hr = dx11data->pDevice->CreateInputLayout(dx11data->vl_scratch, slot, blob->GetBufferPointer(), blob->GetBufferSize(), &result);
	return result;
}


// this is the buffer object data structure which caches the vertex and index buffers
// Stores an entire Tristrip or Polyedge
local void flush_device(H3DData *h3ddata);

class RingBuffer : public CMO_Pooled
{
public:

	RingBuffer(DX11Data * dx11data, uint32_t size_in_bytes) : dx11data(dx11data), buffer(null), buffer_size(0), next_offset(0), locked(false)
	{
		Resize(size_in_bytes);
	}

	// Returns a pointer to a location where 'size_in_bytes' bytes can be written. 
	// 'out_offset' is the offset (in bytes) at which to bind the buffer with D3D.
	void * Lock(uint32_t num_bytes, uint32_t & out_offset, ID3D11Buffer * & out_buffer)
	{
		ASSERT(!locked);

		if (num_bytes > buffer_size) {
			Resize(2 * num_bytes);
		}

		ASSERT(num_bytes <= buffer_size);

		D3D11_MAPPED_SUBRESOURCE mapped;
		HRESULT hr = S_OK;

		if (next_offset + num_bytes < buffer_size)	{
			hr = dx11data->pContext->Map(buffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped);
		}

		else {
			hr = dx11data->pContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
			next_offset = 0;
		}

		ASSERT(SUCCEEDED(hr));

		void * ptr = static_cast<unsigned char *>(mapped.pData) + next_offset;

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
		dx11data->pContext->Unmap(buffer, 0);
		locked = false;
	}

	~RingBuffer()
	{
		buffer->Release();
	}

private:

	void Resize(uint32_t new_size_in_bytes)
	{
		ASSERT(!locked);

		if (buffer != null)
			buffer->Release();

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = new_size_in_bytes;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_DYNAMIC;

		HRESULT hr = dx11data->pDevice->CreateBuffer(&desc, null, &buffer);
		ASSERT(SUCCEEDED(hr));

		HOOPS::WORLD->display_list_vram_usage -= buffer_size;
		HOOPS::WORLD->display_list_vram_usage += new_size_in_bytes;

		buffer_size = new_size_in_bytes;
		next_offset = 0;
	}

	DX11Data * dx11data;
	ID3D11Buffer * buffer;
	uint32_t buffer_size;
	uint32_t next_offset;
	bool locked;
};

class HDX11IndexedBufferObject : public SwitchableIBO
{
public:	

	std::vector<ID3D11Buffer*, CMO_Allocator<ID3D11Buffer*>> m_vtx_buff;
	std::vector<ID3D11ShaderResourceView *, CMO_Allocator<ID3D11ShaderResourceView*>> m_vtx_srvs;
	ID3D11Buffer *					m_idx_buff;

	bool							m_using_linepat_cs;

	ID3D11ShaderResourceView *		m_cs_vtx_srv;
	ID3D11UnorderedAccessView *		m_cs_lengths_uav;

	ID3D11Buffer *					m_cs_clip_vb;
	ID3D11ShaderResourceView *		m_cs_clip_srv;
	ID3D11UnorderedAccessView *		m_cs_clip_uav;

	HDX11IndexedBufferObject(int index, unsigned int flags) : SwitchableIBO(index, flags),		
		m_idx_buff(null),
		m_using_linepat_cs(ANYBIT(flags, H3DIBO_CS_LINE_PAT)),
		m_cs_vtx_srv(null),
		m_cs_lengths_uav(null),
		m_cs_clip_vb(null),
		m_cs_clip_srv(null),
		m_cs_clip_uav(null)
	{

	}

	~HDX11IndexedBufferObject()
	{
		if (m_vtx_buff.size() > 0) {

			for (auto v : m_vtx_buff)
				v->Release();			

			HOOPS::WORLD->display_list_vram_usage -= m_VF.get_size() * m_point_count;
			HOOPS::WORLD->display_list_vram_usage -= m_index_size;
		}

		if (m_idx_buff)
			m_idx_buff->Release();

		for (auto srv : m_vtx_srvs) {
			srv->Release();
		}

		H_SAFE_RELEASE(m_cs_vtx_srv);
		H_SAFE_RELEASE(m_cs_lengths_uav);
		H_SAFE_RELEASE(m_cs_clip_vb);
		H_SAFE_RELEASE(m_cs_clip_srv);
		H_SAFE_RELEASE(m_cs_clip_uav);
	}	

	bool IsMarkers() const
	{
		return m_type == 0;
	}

	virtual void CopyLocalVertexArraysToVRAM()
	{
		ASSERT(m_buffer_type == VRAM);
		ASSERT(m_vtx_buff.size() == 0);

		auto dx11data = (DX11Data*)m_h3ddata;
		m_vtx_buff.resize(m_stream_count);
		for (int i = 0; i < m_stream_count; ++i) {

			D3D11_BUFFER_DESC desc;
			desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			desc.ByteWidth = 0;
			desc.CPUAccessFlags = 0;
			desc.MiscFlags = 0;
			desc.StructureByteStride = 0;
			desc.Usage = D3D11_USAGE_IMMUTABLE;

			if (m_using_linepat_cs) {
				desc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
				desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS;
				desc.Usage = D3D11_USAGE_DEFAULT;
			}

			else if (IsMarkers()) {
				desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
				desc.MiscFlags |= D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
				desc.StructureByteStride = m_strides[i] * sizeof(float);
				desc.Usage = D3D11_USAGE_DEFAULT;
			}

			uint32_t const stream_size = m_point_count * m_strides[i];
			desc.ByteWidth = stream_size * sizeof(float);

			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem = m_vertex_data[i];
			data.SysMemPitch = 0;
			data.SysMemSlicePitch = 0;

			HRESULT hr = dx11data->pDevice->CreateBuffer(&desc, &data, &m_vtx_buff[i]);
			ASSERT(SUCCEEDED(hr));
		}

		if (m_using_linepat_cs) {

			// vb/srv/uav for clipped lines
			{
				D3D11_BUFFER_DESC bufDesc = {};
				m_vtx_buff[0]->GetDesc(&bufDesc);
				
				HRESULT hr = dx11data->pDevice->CreateBuffer(&bufDesc, null, &m_cs_clip_vb);
				ASSERT(SUCCEEDED(hr));

				{
					D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
					desc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
					desc.BufferEx.FirstElement	= 0;
					desc.Format					= DXGI_FORMAT_R32_TYPELESS;
					desc.BufferEx.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;
					desc.BufferEx.NumElements	= bufDesc.ByteWidth / 4;
					HRESULT hr = dx11data->pDevice->CreateShaderResourceView(m_cs_clip_vb, &desc, &m_cs_clip_srv);
					ASSERT(SUCCEEDED(hr));
				}

				{
					D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
					desc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
					desc.Buffer.FirstElement	= 0;
					desc.Format					= DXGI_FORMAT_R32_TYPELESS;
					desc.Buffer.Flags			= D3D11_BUFFER_UAV_FLAG_RAW;
					desc.Buffer.NumElements		= bufDesc.ByteWidth / 4;
					hr = dx11data->pDevice->CreateUnorderedAccessView(m_cs_clip_vb, &desc, &m_cs_clip_uav);
					ASSERT(SUCCEEDED(hr));
				}
			}

			// SRV for vertices
			{
				D3D11_BUFFER_DESC bufDesc = {};
				m_vtx_buff[0]->GetDesc(&bufDesc);

				D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
				desc.ViewDimension			= D3D11_SRV_DIMENSION_BUFFEREX;
				desc.BufferEx.FirstElement	= 0;
				desc.Format					= DXGI_FORMAT_R32_TYPELESS;
				desc.BufferEx.Flags			= D3D11_BUFFEREX_SRV_FLAG_RAW;
				desc.BufferEx.NumElements	= bufDesc.ByteWidth / 4;
				HRESULT hr = dx11data->pDevice->CreateShaderResourceView(m_vtx_buff[0], &desc, &m_cs_vtx_srv);
				ASSERT(SUCCEEDED(hr));
			}

			// UAV for lengths
			{
				D3D11_BUFFER_DESC bufDesc = {};
				m_vtx_buff[m_stream_count-1]->GetDesc(&bufDesc);

				D3D11_UNORDERED_ACCESS_VIEW_DESC desc = {};
				desc.ViewDimension			= D3D11_UAV_DIMENSION_BUFFER;
				desc.Buffer.FirstElement	= 0;
				desc.Format					= DXGI_FORMAT_R32_TYPELESS;
				desc.Buffer.Flags			= D3D11_BUFFER_UAV_FLAG_RAW;
				desc.Buffer.NumElements		= bufDesc.ByteWidth / 4;
				HRESULT hr = dx11data->pDevice->CreateUnorderedAccessView(m_vtx_buff[m_stream_count-1], &desc, &m_cs_lengths_uav);
				ASSERT(SUCCEEDED(hr));
			}
		}

		HOOPS::WORLD->display_list_vram_usage += m_VF.get_size() * m_point_count;		
	}

	virtual void CopyLocalIndexArrayToVRAM()
	{
		ASSERT(m_buffer_type == VRAM);
		ASSERT(m_idx_buff == null);

		if (m_indices == null)
			return;

		auto dx11data = (DX11Data*)m_h3ddata;

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		desc.ByteWidth = m_index_size;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_IMMUTABLE;

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = m_indices;
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT hr = dx11data->pDevice->CreateBuffer(&desc, &data, &m_idx_buff);
		ASSERT(SUCCEEDED(hr));

		HOOPS::WORLD->display_list_vram_usage += m_index_size;
	}

	virtual void DownloadVertexArraysFromVRAM()
	{
		// No hardware buffers have been allocated yet so nothing to download.
		if (m_vtx_buff.size() == 0)
			return;

		auto dx11data = (DX11Data*)m_h3ddata;

		ID3D11Device * const device = dx11data->pDevice;
		ID3D11DeviceContext * const ctx = dx11data->pContext;

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = 0;
		desc.ByteWidth = 0;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_STAGING;

		// Download vertex arrays from VRAM and populate our local arrays
		{
			for (int i = 0; i < m_stream_count; ++i) {

				uint32_t const stream_size = m_point_count * m_strides[i] * sizeof(float);
				desc.ByteWidth = stream_size;

				ID3D11Buffer * staging = null;
				HRESULT hr = device->CreateBuffer(&desc, null, &staging);
				ASSERT(SUCCEEDED(hr));

				ctx->CopyResource(staging, m_vtx_buff[i]);

				// now we can throw away old VRAM buffer
				m_vtx_buff[i]->Release();

				D3D11_MAPPED_SUBRESOURCE mapped;
				hr = ctx->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
				{
					ASSERT(SUCCEEDED(hr));
					COPY_MEMORY(mapped.pData, stream_size, m_vertex_data[i]);
					ctx->Unmap(staging, 0);
				}

				staging->Release();
			}

			m_vtx_buff.clear();
			
			for (auto srv : m_vtx_srvs)
				srv->Release();
			m_vtx_srvs.clear();

			HOOPS::WORLD->display_list_vram_usage -= m_VF.get_size() * m_point_count;
		}

	}

	virtual void DownloadIndexArrayFromVRAM()
	{
		// Download indices
		if (m_idx_buff == null)
			return;

		auto dx11data = (DX11Data*)m_h3ddata;

		ID3D11Device * const device = dx11data->pDevice;
		ID3D11DeviceContext * const ctx = dx11data->pContext;

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = 0;
		desc.ByteWidth = m_index_size;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;
		desc.Usage = D3D11_USAGE_STAGING;

		ID3D11Buffer * staging = null;
		HRESULT hr = device->CreateBuffer(&desc, null, &staging);
		ASSERT(SUCCEEDED(hr));

		ctx->CopyResource(staging, m_idx_buff);

		m_idx_buff->Release();
		m_idx_buff = null;

		D3D11_MAPPED_SUBRESOURCE mapped;
		{
			hr = ctx->Map(staging, 0, D3D11_MAP_READ, 0, &mapped);
			ASSERT(SUCCEEDED(hr));
			COPY_MEMORY(mapped.pData, m_index_size, m_indices);
			ctx->Unmap(staging, 0);
		}

		staging->Release();

		HOOPS::WORLD->display_list_vram_usage -= m_index_size;
	}

	void EnsurePointSprites()
	{
		auto dx11data = (DX11Data*)m_h3ddata;

		if (m_idx_buff == null) {

			ASSERT(m_indices == null);
			m_index_size = m_point_count * 6 * sizeof(unsigned short);
			m_format = H3DFMT_INDEX16;
			ALLOC_ARRAY(m_indices, m_index_size, unsigned char);

			auto ptr = reinterpret_cast<unsigned short *>(m_indices);

			for (unsigned short i = 0; i < m_point_count; ++i) {
				ptr[6 * i + 0] = 4 * i + 0;
				ptr[6 * i + 1] = 4 * i + 1;
				ptr[6 * i + 2] = 4 * i + 2;

				ptr[6 * i + 3] = 4 * i + 1;
				ptr[6 * i + 4] = 4 * i + 3;
				ptr[6 * i + 5] = 4 * i + 2;
			}

			CopyLocalIndexArrayToVRAM();
			ReleaseLocalIndexArray();
		}
		

		if (m_vtx_srvs.size() == 0) {

			m_vtx_srvs.resize(m_stream_count);
			for (int i = 0; i < m_stream_count; ++i) {
				D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
				desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
				desc.BufferEx.FirstElement = 0;
				desc.Format = DXGI_FORMAT_UNKNOWN;
				desc.BufferEx.Flags = 0;
				desc.BufferEx.NumElements = m_point_count;
				HRESULT hr = dx11data->pDevice->CreateShaderResourceView(m_vtx_buff[i], &desc, &m_vtx_srvs[i]);
				ASSERT(SUCCEEDED(hr));
			}
		}		
	}

	void EnsureBufferPointers(DX11Data * dx11data, bool point_sprites = false) const
	{		
		if (m_buffer_type == Local) {

			if (!dx11data->local_ibo_buffers.verts) {
				dx11data->local_ibo_buffers.verts = POOL_NEW(dx11data->dc->memory_pool(), RingBuffer)(dx11data, 2 * H3D_Display_List_MAX_SIZE * (12 + 12 + 4));
			}

			if (!dx11data->local_ibo_buffers.indices) {
				dx11data->local_ibo_buffers.indices = POOL_NEW(dx11data->dc->memory_pool(), RingBuffer)(dx11data, 2 * H3D_Display_List_MAX_SIZE * sizeof(unsigned short));
			}

			{
				uint32_t strides[16];
				ID3D11Buffer * buffer = null;
				uint32_t offset = 0;
				// TODO: do we need to align each stream's start offset to a multiple of the stride?
				auto ptr = static_cast<unsigned char *>(dx11data->local_ibo_buffers.verts->Lock(m_point_count * m_VF.get_size(true), offset, buffer));

				for (int i = 0; i < m_stream_count; ++i) {

					strides[i] = m_strides[i] * sizeof(float);
					uint32_t const stream_size = strides[i] * m_point_count;

					COPY_MEMORY(m_vertex_data[i], stream_size, ptr);
					ENSURE_STREAM_SOURCE(dx11data, i, buffer, strides[i], offset);
					ptr += stream_size;
					offset += stream_size;
				}

				dx11data->local_ibo_buffers.verts->Unlock();
			}

			if (m_index_size) {
				ID3D11Buffer * buffer = null;
				uint32_t offset = 0;
				auto ptr = dx11data->local_ibo_buffers.indices->Lock(m_index_size, offset, buffer);
				COPY_MEMORY(m_indices, m_index_size, ptr);
				dx11data->pContext->IASetIndexBuffer(buffer, dx11format(m_format), offset);
				dx11data->local_ibo_buffers.indices->Unlock();
			}
		}

		else {

			if (point_sprites) {

				ASSERT(!m_VF.get_interleaved());

				ID3D11Buffer * const zero = null;
				uint32_t const offset = 0;

				for (int i = 0; i < m_stream_count; ++i) {					
					uint32_t const stride = m_strides[i] * sizeof(float);
					ENSURE_STREAM_SOURCE(dx11data, i, zero, stride, offset);					
				}

				// These binding locations have to match dx11_vertex.hlsl

				dx11data->pContext->VSSetShaderResources(0, 1, &m_vtx_srvs[0]); // positions				

				int i = 1;
				if (m_VF.get_normals()) {
					ASSERT(m_strides[i] == 3);
					dx11data->pContext->VSSetShaderResources(1, 1, &m_vtx_srvs[i]); 
					++i;
				}

				if (m_VF.get_diffuse()) {
					ASSERT(m_strides[i] == 1);
					dx11data->pContext->VSSetShaderResources(2, 1, &m_vtx_srvs[i]);
					++i;
				}
			}

			else {
				unsigned int stride;
				unsigned int offset = 0;
				int start_i = 0;

				if (m_using_linepat_cs) {
					stride = m_strides[0] * sizeof(float);
					ENSURE_STREAM_SOURCE(dx11data, 0, m_cs_clip_vb, stride, offset);
					start_i++;
				}

				for (int i = start_i; i < m_stream_count; ++i) {
					stride = m_strides[i] * sizeof(float);
					ENSURE_STREAM_SOURCE(dx11data, i, m_vtx_buff[i], stride, offset);
				}
			}

			if (m_idx_buff)
				dx11data->pContext->IASetIndexBuffer(m_idx_buff, dx11format(m_format), 0);
		}
	}

	void Draw(H3DData * h3ddata, Test * cull_results = null, int instances = 1) const
	{
		DX11Data *			dx11data = (DX11Data *)h3ddata;
		Display_Context *	dc = (Display_Context *)dx11data->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		if (dx11data->cache->Shader)
			static_cast<HDX11Shader *>(dx11data->cache->Shader)->ApplyShaderStates(dx11data, m_VF);

		dx11data->pRenderStates->ApplyRenderStates();

		EnsureBufferPointers(dx11data);

		if (cull_results == null || m_index_size == 0 || m_geom_offsets == null) {

			if (cull_results != null && CULLED(cull_results[m_geom_first]))
				return;

			if (m_index_size != 0) {
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

				ENSURE_PRIMITIVE_TOPOLOGY(dx11data, m_type);
				dx11data->pContext->DrawIndexed(prim_to_indices(m_type, m_primitive_count), 0, 0);
			}
			else {
				H3DPRIMITIVETYPE type = H3DPT_POINTLIST;
				if (m_type)
					type = m_type;

				ENSURE_PRIMITIVE_TOPOLOGY(dx11data, type);
				if (instances > 1)
					dx11data->pContext->DrawInstanced(m_point_count, instances, 0, 0);
				else
					dx11data->pContext->Draw(m_point_count, 0);
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
			ENSURE_PRIMITIVE_TOPOLOGY(dx11data, m_type);

			int start = 0;
			int last = 0;

			for (; start < m_geom_count;) {

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

					dx11data->pContext->DrawIndexed(prim_to_indices(m_type, primitive_count), m_geom_offsets[start], 0);
				}

				start = last;
			}
		}
	}

	void Draw_Decimated(H3DData *h3ddata, float decimation = 1.0f) const
	{
		DX11Data *dx11data = (DX11Data *)h3ddata;
		Display_Context *	dc = (Display_Context *)dx11data->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		static_cast<HDX11Shader *>(dx11data->cache->Shader)->ApplyShaderStates(dx11data, m_VF);
		dx11data->pRenderStates->ApplyRenderStates();

		EnsureBufferPointers(dx11data);

		if (m_index_size) {
			ENSURE_PRIMITIVE_TOPOLOGY(dx11data, m_type);
			dx11data->pContext->DrawIndexed((int)(prim_to_indices(m_type, m_primitive_count) * decimation), 0, 0);
		}

		else {
			ENSURE_PRIMITIVE_TOPOLOGY(dx11data, H3DPT_POINTLIST);
			dx11data->pContext->Draw((int)(m_point_count * decimation), 0);
		}
	}

	void Draw_Point_Sprites(H3DData * h3ddata, float decimation = 1.0f)
	{
		DX11Data *dx11data = (DX11Data *)h3ddata;
		Display_Context *	dc = (Display_Context *)dx11data->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		static_cast<HDX11Shader *>(dx11data->cache->Shader)->ApplyShaderStates(dx11data, m_VF);
		dx11data->pRenderStates->ApplyRenderStates();		

		EnsurePointSprites();
		EnsureBufferPointers(dx11data, true);

		ENSURE_PRIMITIVE_TOPOLOGY(dx11data, H3DPT_TRIANGLELIST);
		dx11data->pContext->DrawIndexed(6 * (int)(m_point_count * decimation), 0, 0);
	}

	void Draw_CSLinePattern(H3DData * h3ddata, float u_scale, float v_scale, float const * matMVP, float const * matScreen, Test * cull_results = null, int instances = 1)
	{
		DX11Data *			dx11data = (DX11Data *)h3ddata;
		auto &				linepat = dx11data->linepat_cs;
		ID3D11DeviceContext * context = dx11data->pContext;
		Display_Context *	dc = (Display_Context *)dx11data->dc;

		if (IsLocal())
			DC_STATS(mem_dl)++;

		if (!ensure_cs_line_pattern_resources(dx11data))
			return;

		// Manually sync this with N in shader source.
		int const GROUP_W = 256;
		int const DISPATCH_SIZE = (int)align(m_point_count / 2, GROUP_W) / GROUP_W;
		ASSERT(DISPATCH_SIZE < 1024);

		// Make sure vbs aren't bound
		{
			ID3D11Buffer *null_buffs[] = { null, null, null };
			UINT zero_uints[] = { 0, 0, 0 };
			dx11data->pContext->IASetVertexBuffers(0, countof(null_buffs), null_buffs, zero_uints, zero_uints );
			memset(&dx11data->dx11cache->current_vb[0], 0, sizeof(ID3D11Buffer *) * countof(null_buffs));			
		}

		// clip pass
		{
			// Extract frustum planes: left, right, bottom, top, near, far
			Plane planes[6];
			{
				float col1[] = { matMVP[0], matMVP[4], matMVP[8], matMVP[12] };
				float col2[] = { matMVP[1], matMVP[5], matMVP[9], matMVP[13] };
				float col3[] = { matMVP[2], matMVP[6], matMVP[10], matMVP[14] };
				float col4[] = { matMVP[3], matMVP[7], matMVP[11], matMVP[15] };
				planes[0] = Plane(Vector(col4[0] + col1[0], col4[1] + col1[1], col4[2] + col1[2]), col4[3] + col1[3]);
				planes[1] = Plane(Vector(col4[0] - col1[0], col4[1] - col1[1], col4[2] - col1[2]), col4[3] - col1[3]);
				planes[2] = Plane(Vector(col4[0] + col2[0], col4[1] + col2[1], col4[2] + col2[2]), col4[3] + col2[3]);
				planes[3] = Plane(Vector(col4[0] - col2[0], col4[1] - col2[1], col4[2] - col2[2]), col4[3] - col2[3]);
				planes[4] = Plane(Vector(col3[0], col3[1], col3[2]), col3[3]);
				planes[5] = Plane(Vector(col4[0] - col3[0], col4[1] - col3[1], col4[2] - col3[2]), col4[3] - col3[3]);
				for (int i = 0; i < 6; i++)
					planes[i].Normalize();
			}

			context->UpdateSubresource(linepat.constants, 0, nullptr, planes, 0, 0);

			ID3D11Buffer *cbuffs[] = { linepat.constants };
			context->CSSetConstantBuffers(0, countof(cbuffs), cbuffs);

			ID3D11ShaderResourceView *srvs[] = { m_cs_vtx_srv };
			context->CSSetShaderResources(0, countof(srvs), srvs);

			ID3D11UnorderedAccessView *uavs[] = { m_cs_clip_uav };
			context->CSSetUnorderedAccessViews(0, countof(uavs), uavs, null);

			context->CSSetShader(linepat.clip_shader, null, 0);
			context->Dispatch(DISPATCH_SIZE, 1, 1);
		}

		// compute lengths pass
		{
			float		tmp[18];
			memcpy(tmp, matScreen, sizeof(float)*16);
			tmp[16] = u_scale;
			tmp[17] = v_scale;
			context->UpdateSubresource(linepat.constants, 0, nullptr, tmp, 0, 0);

			ID3D11ShaderResourceView *null_srvs[] = { null };
			context->CSSetShaderResources(0, countof(null_srvs), null_srvs);

			ID3D11UnorderedAccessView *null_uavs[] = { null };
			context->CSSetUnorderedAccessViews(0, countof(null_uavs), null_uavs, null);

			ID3D11ShaderResourceView *srvs[] = { m_cs_clip_srv };
			context->CSSetShaderResources(0, countof(srvs), srvs);

			ID3D11UnorderedAccessView *uavs[] = { m_cs_lengths_uav };
			context->CSSetUnorderedAccessViews(0, countof(uavs), uavs, null);

			context->CSSetShader(linepat.compute_lengths_shader, null, 0);
			context->Dispatch(DISPATCH_SIZE, 1, 1);
		}

		// group sum pass
		if (DISPATCH_SIZE > 1)
		{
			ID3D11ShaderResourceView *null_srvs[] = { null };
			context->CSSetShaderResources(0, countof(null_srvs), null_srvs);

			context->CSSetShader(linepat.sum_shader, null, 0);
			context->Dispatch(DISPATCH_SIZE, 1, 1);
		}

		// cleanup
		{
			context->CSSetShader(null, null, 0);
			ID3D11ShaderResourceView *null_srvs[] = { null };
			context->CSSetShaderResources(0, countof(null_srvs), null_srvs);
			ID3D11UnorderedAccessView *null_uavs[] = { null };
			context->CSSetUnorderedAccessViews(0, countof(null_uavs), null_uavs, null);
			ID3D11Buffer *null_cbs[] = { null };
			context->CSSetConstantBuffers(0, countof(null_cbs), null_cbs);
		}

		Draw(h3ddata, cull_results, instances);
	}

	void Draw_CSEdgePattern(H3DData * h3ddata, float u_scale, float v_scale, float const * matMVP, float const * matScreen, Test * cull_results = null, int instances = 1)
	{
		DX11Data *			dx11data = (DX11Data *)h3ddata;
		auto &				edgepat = dx11data->edgepat_cs;
		ID3D11DeviceContext * context = dx11data->pContext;

		if (!ensure_cs_edge_pattern_resources(dx11data))
			return;

		// Manually sync this with N in shader source.
		int const GROUP_W = 256;
		int const DISPATCH_SIZE = (int)align(m_point_count / 2, GROUP_W) / GROUP_W;
		ASSERT(DISPATCH_SIZE < 1024);

		// Make sure vbs aren't bound
		{
			ID3D11Buffer *null_buffs[] = { null, null, null };
			UINT zero_uints[] = { 0, 0, 0 };
			dx11data->pContext->IASetVertexBuffers(0, countof(null_buffs), null_buffs, zero_uints, zero_uints);
			memset(&dx11data->dx11cache->current_vb[0], 0, sizeof(ID3D11Buffer *) * countof(null_buffs));
		}

		// clip pass
		{
			// Extract frustum planes: left, right, bottom, top, near, far
			Plane planes[6];
			{
				float col1[] = { matMVP[0], matMVP[4], matMVP[8], matMVP[12] };
				float col2[] = { matMVP[1], matMVP[5], matMVP[9], matMVP[13] };
				float col3[] = { matMVP[2], matMVP[6], matMVP[10], matMVP[14] };
				float col4[] = { matMVP[3], matMVP[7], matMVP[11], matMVP[15] };
				planes[0] = Plane(Vector(col4[0] + col1[0], col4[1] + col1[1], col4[2] + col1[2]), col4[3] + col1[3]);
				planes[1] = Plane(Vector(col4[0] - col1[0], col4[1] - col1[1], col4[2] - col1[2]), col4[3] - col1[3]);
				planes[2] = Plane(Vector(col4[0] + col2[0], col4[1] + col2[1], col4[2] + col2[2]), col4[3] + col2[3]);
				planes[3] = Plane(Vector(col4[0] - col2[0], col4[1] - col2[1], col4[2] - col2[2]), col4[3] - col2[3]);
				planes[4] = Plane(Vector(col3[0], col3[1], col3[2]), col3[3]);
				planes[5] = Plane(Vector(col4[0] - col3[0], col4[1] - col3[1], col4[2] - col3[2]), col4[3] - col3[3]);
				for (int i = 0; i < 6; i++)
					planes[i].Normalize();
			}

			context->UpdateSubresource(edgepat.constants, 0, nullptr, planes, 0, 0);

			ID3D11Buffer *cbuffs[] = { edgepat.constants };
			context->CSSetConstantBuffers(0, countof(cbuffs), cbuffs);

			ID3D11ShaderResourceView *srvs[] = { m_cs_vtx_srv };
			context->CSSetShaderResources(0, countof(srvs), srvs);

			ID3D11UnorderedAccessView *uavs[] = { m_cs_clip_uav };
			context->CSSetUnorderedAccessViews(0, countof(uavs), uavs, null);

			context->CSSetShader(edgepat.clip_shader, null, 0);
			context->Dispatch(DISPATCH_SIZE, 1, 1);
		}

		// compute lengths pass
		{
			float		tmp[18];
			memcpy(tmp, matScreen, sizeof(float)*16);
			tmp[16] = u_scale;
			tmp[17] = v_scale;
			context->UpdateSubresource(edgepat.constants, 0, nullptr, tmp, 0, 0);

			ID3D11ShaderResourceView *null_srvs[] = { null };
			context->CSSetShaderResources(0, countof(null_srvs), null_srvs);

			ID3D11UnorderedAccessView *null_uavs[] = { null };
			context->CSSetUnorderedAccessViews(0, countof(null_uavs), null_uavs, null);

			ID3D11ShaderResourceView *srvs[] = { m_cs_clip_srv };
			context->CSSetShaderResources(0, countof(srvs), srvs);

			ID3D11UnorderedAccessView *uavs[] = { m_cs_lengths_uav };
			context->CSSetUnorderedAccessViews(0, countof(uavs), uavs, null);

			context->CSSetShader(edgepat.compute_lengths_shader, null, 0);
			context->Dispatch(DISPATCH_SIZE, 1, 1);
		}

		// cleanup
		{
			context->CSSetShader(null, null, 0);
			ID3D11ShaderResourceView *null_srvs[] = { null };
			context->CSSetShaderResources(0, countof(null_srvs), null_srvs);
			ID3D11UnorderedAccessView *null_uavs[] = { null };
			context->CSSetUnorderedAccessViews(0, countof(null_uavs), null_uavs, null);
			ID3D11Buffer *null_cbs[] = { null };
			context->CSSetConstantBuffers(0, countof(null_cbs), null_cbs);
		}

		Draw(h3ddata, cull_results, instances);
	}

};

// this is the global vertex buffer cache, being used for view dependent geometry
class HDX11VertexBufferCache : public H3DVertexBufferCache
{
public:
	HDX11VertexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage += HDX11_PER_OBJECT_PENALTY;
		m_pBuffer = null;
		m_nMaxPoints	= 0;
		m_nNextVertexOffset = 0;
		m_nCurVertexOffset = 0;
	}

	~HDX11VertexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage -= HDX11_PER_OBJECT_PENALTY;

		if (temp_vb)
			FREE_ARRAY(temp_vb, m_nMaxPoints*m_strides[0], float);
		if (temp_nb)
			FREE_ARRAY(temp_nb, m_nMaxPoints*m_strides[1], float);

		for (int i = 0; i < m_stream_count; i++) {
			H_SAFE_RELEASE(m_pBuffer[i]);
		}
		FREE_ARRAY(m_pBuffer, m_stream_count, ID3D11Buffer*);
		FREE_ARRAY(m_strides, m_stream_count, unsigned int);
		FREE_ARRAY(m_vertex_data, m_stream_count, float*);

		HOOPS::WORLD->display_list_vram_usage -= (m_nMaxPoints * m_VF.get_size());
	}

	bool CreateVertexBuffer(H3DData *h3ddata, UINT max_points, H3DVertexFormat VF)
	{
		if (m_pBuffer) {
			for (int i = 0; i < m_stream_count; i++)
				H_SAFE_RELEASE(m_pBuffer[i]);
			FREE_ARRAY(m_pBuffer, m_stream_count, ID3D11Buffer*);
			FREE_ARRAY(m_strides, m_stream_count, unsigned int);
			FREE_ARRAY(m_vertex_data, m_stream_count, float*);
		}

		DX11Data *dx11data = (DX11Data *) h3ddata;

		m_pContext			= dx11data->pContext;
		m_nMaxPoints		= max_points;
		m_VF				= VF;
		m_stream_count		= VF.get_stream_count();
		ALLOC_ARRAY(m_strides, m_stream_count, unsigned int);
		VF.get_stream_strides(m_strides);

		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		ALLOC_ARRAY(m_vertex_data, m_stream_count, float*);
		ALLOC_ARRAY(m_pBuffer, m_stream_count, ID3D11Buffer*);
		for (int i = 0; i < m_stream_count; i++) {
			bd.ByteWidth = m_nMaxPoints*m_strides[i]*sizeof(float);
			dx11data->pDevice->CreateBuffer(&bd, null, &m_pBuffer[i]);
		}

		if (m_VF.get_position()) {
			ALLOC_ARRAY_CACHED(temp_vb, m_nMaxPoints*m_strides[0], float);
		}
		if (m_VF.get_normals()) {
			ALLOC_ARRAY_CACHED(temp_nb, m_nMaxPoints*m_strides[1], float);
		}

		HOOPS::WORLD->display_list_vram_usage += (m_nMaxPoints * m_VF.get_size());
		return true;
	}

	bool Reset()
	{
		for (int i = 0; i < m_stream_count; i++) {
			m_pBuffer[i]->Release();
		}
		FREE_ARRAY(m_pBuffer, m_stream_count, ID3D11Buffer*);
		FREE_ARRAY(m_strides, m_stream_count, unsigned int);

		HOOPS::WORLD->display_list_vram_usage -= (m_nMaxPoints * m_VF.get_size());

		m_nMaxPoints	= 0;
		m_nNextVertexOffset = 0;
		m_nCurVertexOffset = 0;
		m_push_data_start = 0;
		m_stream_count = 0;
		return true;
	}

	bool Discard()
	{
		m_push_data_start = 0;
		m_nNextVertexOffset = 0;
		m_nCurVertexOffset = 0;
		D3D11_MAPPED_SUBRESOURCE * pData = null;

		for (int i = 0; i < m_stream_count; i++) {
			m_pContext->Map(m_pBuffer[i], 0, D3D11_MAP_WRITE_DISCARD, 0, pData);
			m_pContext->Unmap(m_pBuffer[i], 0);
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

		if (matrix != null) {
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
		D3D11_MAP map_type = D3D11_MAP_WRITE_NO_OVERWRITE;

		// Check to see if the entire vertex buffer has been used up yet.
		if (m_nNextVertexOffset > (m_nMaxPoints - count))
		{
			// No space remains. Start over from the beginning 
			//		 of the vertex buffer.
			map_type = D3D11_MAP_WRITE_DISCARD;
			m_nCurVertexOffset = 0;
			m_nNextVertexOffset = 0;
		}

		// Lock the vertex buffer.
		HRESULT hr;
		for (int i = 0; i < m_stream_count; i++) {
			if (FAILED(hr = m_pContext->Map(m_pBuffer[i], 0, map_type, 0, &m_MappedResource)))
				return false;
			else
				m_vertex_data[i] = (float *)m_MappedResource.pData + (m_nCurVertexOffset * m_strides[i]);
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
			m_pContext->Unmap(m_pBuffer[i], 0);
		}
		return result;
	}

	int StartVertex()
	{
		return  m_nCurVertexOffset;
	}

	void EnsureVertexDeclaration(H3DData *h3ddata)
	{
		((HDX11Shader*)h3ddata->cache->Shader)->EnsureInputLayout(m_VF);
	}
	void EnsureStreamSources(H3DData *h3ddata)
	{
		DX11Data *dx11data = (DX11Data *)h3ddata;
		unsigned int offsets[16];
		unsigned int strides[16];
		for (int i = 0; i < m_stream_count; i++) {
			strides[i] = m_strides[i] * sizeof(float);
			offsets[i] = strides[i] * m_nCurVertexOffset;
			ENSURE_STREAM_SOURCE(dx11data, i, m_pBuffer[i], strides[i], offsets[i]);
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
		// Render the primitives.
		DX11Data *dx11data = (DX11Data *) h3ddata;
		EnsureStreamSources(dx11data);
		EnsureVertexDeclaration(dx11data);

		HDX11Shader *shader = (HDX11Shader*) dx11data->cache->Shader;
		shader->ApplyConstants();

		dx11data->pRenderStates->ApplyRenderStates();
		ENSURE_PRIMITIVE_TOPOLOGY(dx11data, PrimitiveType);

		if (instances > 1)
			m_pContext->DrawInstanced(prim_to_indices(PrimitiveType, PrimitiveCount), instances, 0, 0);
		else
			m_pContext->Draw(prim_to_indices(PrimitiveType, PrimitiveCount), 0);
		return true;
	}

	void DrawComplete()
	{
		m_nCurVertexOffset = m_nNextVertexOffset;
	}

	ID3D11Buffer **m_pBuffer;
	ID3D11DeviceContext *m_pContext;
	D3D11_MAPPED_SUBRESOURCE m_MappedResource;

};


// this is the global vertex buffer cache, being used for view dependent geometry
class HDX11GenericVertexBufferCache : public H3DGenericVertexBufferCache
{
public:
	HDX11GenericVertexBufferCache(H3DData *h3ddata) :
		H3DGenericVertexBufferCache(h3ddata)
	{
		DX11Data *dx11data = (DX11Data*)h3ddata;
		m_pContext = dx11data->pContext;
		HOOPS::WORLD->display_list_vram_usage += HDX11_PER_OBJECT_PENALTY;
	}

	~HDX11GenericVertexBufferCache()
	{
		for (int i = 0; i < 12; i++) {
			H_SAFE_RELEASE(m_pBuffer[i]);
		}
		HOOPS::WORLD->display_list_vram_usage -= HDX11_PER_OBJECT_PENALTY;
	}

	bool Lock(H3DVertexFormat const &vf, unsigned int count, H3DVertexBuffer *pv)
	{
		DX11Data *dx11data = (DX11Data*) m_h3ddata;
		
		float *m_locked_data[12];
		m_VF = vf;

		// No overwrite will be used if the vertices can fit into the space remaining in the vertex buffer.
		D3D11_MAP map_type = D3D11_MAP_WRITE_NO_OVERWRITE;
		if (m_next_offset > (m_nMaxPoints - count))	{
			map_type = D3D11_MAP_WRITE_DISCARD;
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

		D3D11_MAPPED_SUBRESOURCE m_MappedResource;
		D3D11_BUFFER_DESC bd;
		bd.Usage = D3D11_USAGE_DYNAMIC;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		bd.MiscFlags = 0;

		int stream = 0;
		for (i = 0; i < 12; i++) {
			if (ANYBIT(m_has_stream, 1 << i)) {
				unsigned int stride = m_strides[i] * sizeof(float);
				if (!m_pBuffer[i]) {
					bd.ByteWidth = m_nMaxPoints*stride;
					dx11data->pDevice->CreateBuffer(&bd, null, &m_pBuffer[i]);
				}
				m_pContext->Map(m_pBuffer[i], 0, map_type, 0, &m_MappedResource);
				m_vertex_data[i] = (float*)m_MappedResource.pData;
				m_locked_data[stream] = m_vertex_data[i] + m_next_offset * m_strides[i];
				stream++;
			}
			else {
				// Make sure we discard all buffers, not just the active one; not doing so can lead to erratic behavior
				if (map_type == D3D11_MAP_WRITE_DISCARD && m_pBuffer[i]) {
					m_pContext->Map(m_pBuffer[i], 0, map_type, 0, &m_MappedResource);
					m_pContext->Unmap(m_pBuffer[i], 0);
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
		for (int i = 0; i < 12; i++) {
			if (ANYBIT(m_has_stream, 1 << i))
				m_pContext->Unmap(m_pBuffer[i], 0);
		}
		return result;
	}

	int StartVertex()
	{
		return m_curr_offset;
	}

	void EnsureVertexDeclaration(H3DData *h3ddata)
	{
		((HDX11Shader*)h3ddata->cache->Shader)->EnsureInputLayout(m_VF);
	}
	void EnsureStreamSources(H3DData *h3ddata)
	{
		DX11Data *dx11data = (DX11Data*)h3ddata;
		int i = 0;
		int stream = 0;

		ID3D11Buffer *buffers[12];
		unsigned int strides[12];
		unsigned int offsets[12];

		for (i = 0; i < 4; i++) {
			if (ANYBIT(m_has_stream, 1 << i)) {
				strides[stream] = m_strides[i]*sizeof(float);
				offsets[stream] = m_curr_offset*strides[stream];
				buffers[stream] = m_pBuffer[i];
				dx11data->dx11cache->current_vb[stream] = buffers[stream];
				stream++;
			}
		}
		for (i = 0; i < m_VF.get_tex_count(); i++) {
			if (ANYBIT(m_has_stream, 1 << (i+4))) {
				strides[stream] = m_VF.get_tex_size(i)*sizeof(float);
				offsets[stream] = m_curr_offset*m_strides[i+4]*sizeof(float);
				buffers[stream] = m_pBuffer[i+4];
				dx11data->dx11cache->current_vb[stream] = buffers[stream];
				stream++;
			}
		}
		dx11data->pContext->IASetVertexBuffers(0, stream, buffers, strides, offsets);
	}

	bool Draw(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount)
	{
		// Render the primitives.
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount)
	{
		// Render the primitives.
		DX11Data *dx11data = (DX11Data *) h3ddata;
		EnsureStreamSources(dx11data);
		EnsureVertexDeclaration(dx11data);

		HDX11Shader *shader = (HDX11Shader*) dx11data->cache->Shader;
		shader->ApplyConstants();

		dx11data->pRenderStates->ApplyRenderStates();
		ENSURE_PRIMITIVE_TOPOLOGY(dx11data, PrimitiveType);
		m_pContext->Draw(prim_to_indices(PrimitiveType, PrimitiveCount), 0);
		return true;
	}

	void DrawComplete()
	{
		m_curr_offset = m_next_offset;
	}

	int m_has_stream;	// bitfield
	ID3D11Buffer *m_pBuffer[12];
	ID3D11DeviceContext *m_pContext;
};


// an index buffer, used in conjunction with a HDX11VertexBufferCache class. Can use any HDX11VertexBufferCache
class HDX11IndexBufferCache : public H3DIndexBufferCache
{
public:
	HDX11IndexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage += HDX11_PER_OBJECT_PENALTY;
		m_pIndexBuffer = 0;
		m_nNextIndexData = 0;
		m_nCurIndexData = 0;
	}

	~HDX11IndexBufferCache()
	{
		HOOPS::WORLD->display_list_vram_usage -= HDX11_PER_OBJECT_PENALTY;
		HOOPS::WORLD->display_list_vram_usage -= (IB_CACHE_MAX_SIZE * sizeof(short));
		H_SAFE_RELEASE(m_pIndexBuffer);
	}

	bool CreateIndexBuffer(H3DData *h3ddata)
	{
		HRESULT hr = S_OK;
		DX11Data *dx11data = (DX11Data *) h3ddata;
		m_pContext = dx11data->pContext;

		ASSERT(VB_CACHE_MAX_SIZE < 0x0000ffff);	//index buffer cache does not support 32 bit indices
		if (m_pIndexBuffer == NULL) {
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			bd.MiscFlags = 0;
			bd.ByteWidth = IB_CACHE_MAX_SIZE*sizeof(short);
			dx11data->pDevice->CreateBuffer(&bd, null, &m_pIndexBuffer);
		}

		HOOPS::WORLD->display_list_vram_usage += (IB_CACHE_MAX_SIZE * sizeof(short));
		return SUCCEEDED(hr);
	}

	bool Lock(int count, short **ppbData)
	{
		HRESULT hr;
		int sizetolock = count * sizeof(short);
		D3D11_MAPPED_SUBRESOURCE mapped_subresource;

		if (m_nNextIndexData > (IB_CACHE_MAX_SIZE * sizeof(short) - sizetolock)) {
			m_nCurIndexData = m_nNextIndexData = 0;
			hr = m_pContext->Map(m_pIndexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_subresource);
		}
		else {
			hr = m_pContext->Map(m_pIndexBuffer, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &mapped_subresource);
		}
		*ppbData = (short*) mapped_subresource.pData + (m_nCurIndexData / sizeof(short));
		ASSERT (ppbData != null);
		m_nNextIndexData += sizetolock;

		return SUCCEEDED(hr);
	}

	bool Unlock(VOID)
	{
		m_pContext->Unmap(m_pIndexBuffer, 0);
		return true;
	}

	bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, 
		UINT VertexCount,
		UINT PrimitiveCount)
	{
		UNREFERENCED(VertexCount);

		HDX11VertexBufferCache *VertexBufferCache = (HDX11VertexBufferCache*)m_pVertexBufferCache;
		HDX11GenericVertexBufferCache *GenericVertexBufferCache = (HDX11GenericVertexBufferCache*)m_pGenericVertexBufferCache;

		DX11Data *dx11data = (DX11Data *) h3ddata;
		// Render the primitives.
		if (VertexBufferCache) {
			VertexBufferCache->EnsureVertexDeclaration(dx11data);
			VertexBufferCache->EnsureStreamSources(dx11data);
		}
		else {
			GenericVertexBufferCache->EnsureVertexDeclaration(dx11data);
			GenericVertexBufferCache->EnsureStreamSources(dx11data);
		}

		HDX11Shader *shader = (HDX11Shader*) dx11data->cache->Shader;
		shader->ApplyConstants();

		dx11data->pRenderStates->ApplyRenderStates();
		m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R16_UINT, m_nCurIndexData);
		ENSURE_PRIMITIVE_TOPOLOGY(dx11data, PrimitiveType);
		
		m_pContext->DrawIndexed(prim_to_indices(PrimitiveType, PrimitiveCount), 0, 0);
		return true;
	}

	void DrawComplete()
	{
		m_nCurIndexData = m_nNextIndexData;

		if (m_pVertexBufferCache)
			m_pVertexBufferCache->DrawComplete();
		else
			m_pGenericVertexBufferCache->DrawComplete();
	}

	ID3D11Buffer			*m_pIndexBuffer;
	UINT					m_nCurIndexData;
	UINT					m_nNextIndexData;

	ID3D11DeviceContext		*m_pContext;
};

/*****************************************************************************
*****************************************************************************
							HDX11Texture Class
*****************************************************************************
*****************************************************************************/
class HDX11RenderTarget : public H3DRenderTarget
{
private:
	ID3D11Texture2D *m_id;
	ID3D11View *m_view;	// base class of ID3D11RenderTargetView/ID3D11DepthStencilView
	ID3D11ShaderResourceView *m_srv;
public:
	HRESULT hr;

public:

	HDX11RenderTarget(H3DData *h3ddata, ID3D11DepthStencilView *surface) :
		H3DRenderTarget(h3ddata, 0, 0, 0, H3DFMT_A8R8G8B8, 0)
	{
		D3D11_TEXTURE2D_DESC desc;
		surface->GetResource((ID3D11Resource **)&m_id);
		m_id->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
		m_view = surface;
		m_msaa = desc.SampleDesc.Count;
	}

	HDX11RenderTarget(H3DData *h3ddata, ID3D11RenderTargetView *surface) :
		H3DRenderTarget(h3ddata, 0, 0, 0, H3DFMT_A8R8G8B8, 0)
	{
		D3D11_TEXTURE2D_DESC desc;
		surface->GetResource((ID3D11Resource **)&m_id);
		m_id->GetDesc(&desc);

		m_width = desc.Width;
		m_height = desc.Height;
		m_view = surface;
		m_msaa = desc.SampleDesc.Count;
	}

	HDX11RenderTarget(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int usage,		// H3DTEXUSAGE
		H3DFORMAT format,
		int msaa = 0) :
		H3DRenderTarget(h3ddata, width, height, usage, format, msaa),
		m_srv(null)
	{
		DX11Data *dx11data = (DX11Data *) h3ddata;
		unsigned int msaa_levels = 0;

		if (msaa) {
			dx11data->pDevice->CheckMultisampleQualityLevels(dx11format(format), msaa, &msaa_levels);

			// Disable CSAA when sample count is 4; allow with 8, but limit to 8xQ
			if (msaa == 4)
				msaa_levels = 0;
			else if (msaa == 8)
				msaa_levels = Min(9U, msaa_levels);
		}

		msaa = Max(1, msaa);
		if (msaa_levels > 0)
			msaa_levels--;

		D3D11_TEXTURE2D_DESC texdesc;
		texdesc.Width = width;
		texdesc.Height = height;
		texdesc.MipLevels = 1;
		texdesc.ArraySize = 1;
		texdesc.Format = dx11format(format);
		texdesc.SampleDesc.Count = msaa;
		texdesc.SampleDesc.Quality = msaa_levels;
		texdesc.Usage = D3D11_USAGE_DEFAULT;
		texdesc.CPUAccessFlags = 0;
		texdesc.MiscFlags = 0;
		
		if (usage == H3DTEXUSAGE_RENDERTARGET) {
			texdesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
			hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id);
			hr = dx11data->pDevice->CreateRenderTargetView(m_id, 0, (ID3D11RenderTargetView **)&m_view);
			hr = dx11data->pDevice->CreateShaderResourceView(m_id, 0, &m_srv);
		}
		else if (usage == H3DTEXUSAGE_DEPTHSTENCIL) {
			texdesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
			texdesc.Format = DXGI_FORMAT_R24G8_TYPELESS;

			D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
			ZERO_STRUCT(&dsv_desc, D3D11_DEPTH_STENCIL_VIEW_DESC);
			dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			if (msaa > 1)
				dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

			hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id);
			hr = dx11data->pDevice->CreateDepthStencilView(m_id, &dsv_desc, (ID3D11DepthStencilView **)&m_view);
		}
		else
			ASSERT(0);
	}

	~HDX11RenderTarget()
	{
		H_SAFE_RELEASE(m_id);
		H_SAFE_RELEASE(m_view);
		H_SAFE_RELEASE(m_srv);
	}

	ID3D11View *GetView() {return m_view;}
	ID3D11ShaderResourceView *GetShaderResourceView() {return m_srv;}
	ID3D11Texture2D *GetTexture() {return m_id;}
};


class HDX11Texture2D : public H3DTexture
{
private:
	ID3D11Texture2D *m_id;
	ID3D11Texture2D *m_id_staging;
	ID3D11ShaderResourceView *m_sr_view;
	ID3D11RenderTargetView *m_rt_view;
	ID3D11DepthStencilView *m_ds_view;
	ID3D11UnorderedAccessView *m_ua_view;
	D3D11_MAPPED_SUBRESOURCE m_mapped_data;
	
	D3D11_BOX m_locked_rect;
	bool m_use_locked_rect;

public:
	HRESULT hr;

public:
	HDX11Texture2D(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format, 
		bool shared = false) :
		H3DTexture(h3ddata, width, height, levels, usage, format)
	{
		DX11Data *dx11data = (DX11Data *) h3ddata;
		m_h3ddata = h3ddata;
		m_bytes_per_pixel = 4;

		if (m_format == H3DFMT_DXT1 || m_format == H3DFMT_DXT3 || m_format == H3DFMT_DXT5)
			levels = 1;

		D3D11_TEXTURE2D_DESC texdesc;
		texdesc.Width = width;
		texdesc.Height = height;
		texdesc.MipLevels = levels;
		texdesc.ArraySize = 1;
		texdesc.Format = dx11format(format);
		texdesc.SampleDesc.Count = 1;
		texdesc.SampleDesc.Quality = 0;
		texdesc.CPUAccessFlags = 0;
		texdesc.MiscFlags = 0;
		texdesc.Usage = D3D11_USAGE_DEFAULT;
		texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		if (shared) {
			texdesc.MiscFlags |= D3D11_RESOURCE_MISC_SHARED;			
		}

		if (texdesc.Format == DXGI_FORMAT_R8G8B8A8_UNORM)
			m_format = H3DFMT_A8R8G8B8;

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) {
			texdesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}
		if (ANYBIT(usage, H3DTEXUSAGE_DEPTHSTENCIL)) {
			texdesc.BindFlags |= D3D11_BIND_DEPTH_STENCIL;
		}
		if (ANYBIT(usage, H3DTEXUSAGE_COMPUTE)) {
			texdesc.BindFlags |= D3D11_BIND_UNORDERED_ACCESS;
		}
		if (ANYBIT(usage, H3DTEXUSAGE_DYNAMIC)) {
			texdesc.Usage = D3D11_USAGE_DYNAMIC;
			texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}		

		// auto mipmaps can only be used on textures that are both rendertargets and shader resources
		if (ANYBIT(usage, H3DTEXUSAGE_AUTOGENMIPMAP)) {
			texdesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			texdesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		switch (m_format)
		{
			case H3DFMT_L8:
				m_bytes_per_pixel = 1;
				break;
			case H3DFMT_R8G8B8:
				if (dx11data->supports_texture_rgb24)
					m_bytes_per_pixel = 3;
				break;
			case H3DFMT_DXT1:
				m_bytes_per_pixel = -1;
				texdesc.BindFlags &= ~D3D11_BIND_RENDER_TARGET;
				texdesc.MiscFlags &= ~D3D11_RESOURCE_MISC_GENERATE_MIPS;
				break;
			case H3DFMT_DXT3:
				m_bytes_per_pixel = -1;
				break;
			case H3DFMT_DXT5:
				m_bytes_per_pixel = -1;
				break;
		}

		hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id);

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) {
			hr = dx11data->pDevice->CreateRenderTargetView(m_id, 0, (ID3D11RenderTargetView**)&m_rt_view);
		}
		if (ANYBIT(usage, H3DTEXUSAGE_DEPTHSTENCIL)) {
			hr = dx11data->pDevice->CreateDepthStencilView(m_id, 0, (ID3D11DepthStencilView**)&m_ds_view);
		}
		if (ANYBIT(usage, H3DTEXUSAGE_COMPUTE)) {
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavd;
			uavd.Format = DXGI_FORMAT_R32_UINT;
			uavd.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			uavd.Texture2D.MipSlice = 0;
			hr = dx11data->pDevice->CreateUnorderedAccessView(m_id, &uavd, &m_ua_view);
		}
		hr = dx11data->pDevice->CreateShaderResourceView(m_id, 0, (ID3D11ShaderResourceView**)&m_sr_view);
	}

	~HDX11Texture2D()
	{
		H_SAFE_RELEASE(m_rt_view);
		H_SAFE_RELEASE(m_ds_view);
		H_SAFE_RELEASE(m_sr_view);
		H_SAFE_RELEASE(m_ua_view);
		H_SAFE_RELEASE(m_id);
		H_SAFE_RELEASE(m_id_staging);
	};

	bool LockBytes(int bytes, unsigned char ** data)
	{
		UNREFERENCED(bytes);
		HRESULT hr;
		DX11Data *dx11data = (DX11Data *) m_h3ddata;
		m_use_locked_rect = false;

		// create a staging resource
		D3D11_TEXTURE2D_DESC texdesc;
		m_id->GetDesc(&texdesc);
		texdesc.Usage = D3D11_USAGE_STAGING;
		texdesc.BindFlags = 0;
		texdesc.MiscFlags = 0;
		texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id_staging);
		hr = dx11data->pContext->Map(m_id_staging, 0, D3D11_MAP_WRITE, 0, &m_mapped_data);
		
		*data = (unsigned char *) m_mapped_data.pData;
		return SUCCEEDED(hr);
	}

	bool Lock(H3DRect *h3drect, unsigned char ** data, int * pitch = null)
	{
		DX11Data *dx11data = (DX11Data *) m_h3ddata;

		// create a staging resource
		D3D11_TEXTURE2D_DESC texdesc;
		m_id->GetDesc(&texdesc);
		texdesc.Usage = D3D11_USAGE_STAGING;
		texdesc.BindFlags = 0;
		texdesc.MiscFlags = 0;		
		texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (h3drect) {
			texdesc.Width = h3drect->right - h3drect->left;
			texdesc.Height = h3drect->bottom - h3drect->top;

			m_locked_rect.left = h3drect->left;
			m_locked_rect.right = h3drect->right;
			m_locked_rect.top = h3drect->top;
			m_locked_rect.bottom = h3drect->bottom;
			m_locked_rect.front = m_locked_rect.back = 0;
			m_use_locked_rect = true;
		}
		else
			m_use_locked_rect = false;

		hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id_staging);
		hr = dx11data->pContext->Map(m_id_staging, 0, D3D11_MAP_WRITE, 0, &m_mapped_data);
		
		*data = (unsigned char *) m_mapped_data.pData;
		if (pitch)
			*pitch = m_mapped_data.RowPitch;
		return SUCCEEDED(hr);
	}

	void Unlock()
	{
		DX11Data *dx11data = (DX11Data *) m_h3ddata;

		dx11data->pContext->Unmap(m_id_staging, 0);

		if (m_use_locked_rect)
			dx11data->pContext->CopySubresourceRegion(m_id, 0, m_locked_rect.left, m_locked_rect.top, m_locked_rect.front,
				m_id_staging, 0, null);
		else
			dx11data->pContext->CopyResource(m_id, m_id_staging);

		ZERO_STRUCT(&m_mapped_data, D3D11_MAPPED_SUBRESOURCE);
		H_SAFE_RELEASE(m_id_staging);
	}

	void SetData(int level, void const * data, int width, int height, int stride)
	{
		DX11Data *dx11data = (DX11Data*) m_h3ddata;
		UNREFERENCED(width);

		ASSERT(ANYBIT(m_usage, H3DTEXUSAGE_DYNAMIC));

		D3D11_MAPPED_SUBRESOURCE mapped;
		dx11data->pContext->Map(m_id, level, D3D11_MAP_WRITE_DISCARD, 0, &mapped);
		{
			if (mapped.RowPitch == static_cast<UINT>(stride)) {
				memcpy(mapped.pData, data, stride * height);
			}
			else {
				unsigned char const * src_ptr = static_cast<unsigned char const *>(data);
				unsigned char * dst_ptr = static_cast<unsigned char *>(mapped.pData);

				for (int y = 0 ; y < height ; ++y) {
					memcpy(dst_ptr, src_ptr, stride);
					dst_ptr += mapped.RowPitch;
					src_ptr += stride;
				}
			}
		} dx11data->pContext->Unmap(m_id, 0);
	}

	void GenerateMipMaps()
	{
		DX11Data *dx11data = (DX11Data*) m_h3ddata;

		if (m_format == H3DFMT_DXT1 || m_format == H3DFMT_DXT3 || m_format == H3DFMT_DXT5) {
			// The following block works, but doesn't match DX9 (it doesn't generate mips for compressed textures)
#if 0
			// This only works for Direct3D 10.1+ (search MSDN for Block Compression (Direct3D 10))
			DXGI_FORMAT tex_mips_format = DXGI_FORMAT_R16G16B16A16_UINT;
			if (m_format == H3DFMT_DXT3 || m_format == H3DFMT_DXT5)
				tex_mips_format = DXGI_FORMAT_R32G32B32A32_UINT;

			ID3D11Texture2D *tex_mips;
			ID3D11ShaderResourceView *tex_mips_view;
			D3D11_TEXTURE2D_DESC texdesc;
			m_id->GetDesc(&texdesc);
			texdesc.Format = tex_mips_format;
			texdesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
			texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
			dx11data->pDevice->CreateTexture2D(&texdesc, null, &tex_mips);

			D3D11_MAPPED_SUBRESOURCE data;
			dx11data->pContext->CopySubresourceRegion(tex_mips, 0,0,0,0, m_id, 0,null);
			dx11data->pDevice->CreateShaderResourceView(tex_mips, null, &tex_mips_view);
			dx11data->pContext->GenerateMips(tex_mips_view);

			// For some reason, we need to set a box of 1/4 the size of the actual dimension to get the
			// copy to work
			D3D11_BOX box;
			box.left = 0;
			box.right = 32;
			box.top = 0;
			box.bottom = 32;
			box.back = 1;
			box.front = 0;
			dx11data->pContext->CopySubresourceRegion(m_id, 1,0,0,0, tex_mips, 1,&box);
#endif
		}
		else
			dx11data->pContext->GenerateMips(m_sr_view);
	}

	bool CompressTextureFromMemory(Texture *	texture)
	{
		return LoadTextureFromMemory(texture);
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
		return SUCCEEDED(hr);
	}


	/* HDX11Texture specific methods */
	/********************************/
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_rt_view;
	}
	ID3D11DepthStencilView* GetDepthStencilView()
	{
		return m_ds_view;
	}
	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return m_sr_view;
	}
	ID3D11UnorderedAccessView *GetUnorderedAccessView()
	{
		return m_ua_view;
	}

	ID3D11Texture2D *GetTexture() {return m_id;}
};


class HDX11CubeTexture : public H3DCubeTexture
{
private:
	ID3D11Texture2D *m_id;
	ID3D11Texture2D *m_id_staging;
	ID3D11ShaderResourceView *m_sr_view;
	ID3D11RenderTargetView *m_rt_view;
	D3D11_MAPPED_SUBRESOURCE m_mapped_data;
	
	D3D11_BOX m_locked_rect;
	bool m_use_locked_rect;

public:
	HRESULT hr;

public:
	HDX11CubeTexture(H3DData *h3ddata, 
		unsigned int size,
		unsigned int levels,
		unsigned int usage) :
		H3DCubeTexture(h3ddata, size, levels, usage, H3DFMT_A8R8G8B8)
	{
		DX11Data *dx11data = (DX11Data *) h3ddata;
		m_h3ddata = h3ddata;
		m_bytes_per_pixel = 4;

		D3D11_TEXTURE2D_DESC texdesc;
		texdesc.Width = size;
		texdesc.Height = size;
		texdesc.MipLevels = levels;
		texdesc.ArraySize = 6;
		texdesc.Format = dx11format(H3DFMT_A8R8G8B8);
		texdesc.SampleDesc.Count = 1;
		texdesc.SampleDesc.Quality = 0;
		texdesc.CPUAccessFlags = 0;
		texdesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
		texdesc.Usage = D3D11_USAGE_DEFAULT;
		texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) {
			texdesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		}

		// auto mipmaps can only be used on textures that are both rendertargets and shader resources
		if (ANYBIT(usage, H3DTEXUSAGE_AUTOGENMIPMAP)) {
			texdesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
			texdesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id);

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET)) {
			hr = dx11data->pDevice->CreateRenderTargetView(m_id, 0, (ID3D11RenderTargetView**)&m_rt_view);
		}
		hr = dx11data->pDevice->CreateShaderResourceView(m_id, 0, (ID3D11ShaderResourceView**)&m_sr_view);
	}

	~HDX11CubeTexture()
	{
		H_SAFE_RELEASE(m_rt_view);
		H_SAFE_RELEASE(m_sr_view);
		H_SAFE_RELEASE(m_id);
		H_SAFE_RELEASE(m_id_staging);
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
		UNREFERENCED(face);
		DX11Data *dx11data = (DX11Data *) m_h3ddata;

		// create a staging resource
		D3D11_TEXTURE2D_DESC texdesc;
		m_id->GetDesc(&texdesc);
		texdesc.Usage = D3D11_USAGE_STAGING;
		texdesc.BindFlags = 0;
		texdesc.MiscFlags = 0;
		texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		if (h3drect) {
			texdesc.Width = h3drect->right - h3drect->left;
			texdesc.Height = h3drect->bottom - h3drect->top;

			m_locked_rect.left = h3drect->left;
			m_locked_rect.right = h3drect->right;
			m_locked_rect.top = h3drect->top;
			m_locked_rect.bottom = h3drect->bottom;
			m_locked_rect.front = m_locked_rect.back = 0;
			m_use_locked_rect = true;
		}
		else
			m_use_locked_rect = false;

		hr = dx11data->pDevice->CreateTexture2D(&texdesc, 0, &m_id_staging);
		hr = dx11data->pContext->Map(m_id_staging, 0, D3D11_MAP_WRITE, 0, &m_mapped_data);
		
		*data = (unsigned char *) m_mapped_data.pData;
		if (pitch)
			*pitch = m_mapped_data.RowPitch;
		return SUCCEEDED(hr);
	}

	void Unlock(H3DFACE face)
	{
		DX11Data *dx11data = (DX11Data *) m_h3ddata;
		dx11data->pContext->Unmap(m_id_staging, 0);

		D3D11_TEXTURE2D_DESC texdesc;
		m_id->GetDesc(&texdesc);
		int face_index = D3D11CalcSubresource(0,face,texdesc.MipLevels);

		if (m_use_locked_rect)
			dx11data->pContext->CopySubresourceRegion(m_id, face_index,
				m_locked_rect.left, m_locked_rect.top, m_locked_rect.front,
				m_id_staging, 0, null);
		else
			dx11data->pContext->CopySubresourceRegion(m_id, face_index,
				0,0,0,
				m_id_staging, 0, null);

		ZERO_STRUCT(&m_mapped_data, D3D11_MAPPED_SUBRESOURCE);
		H_SAFE_RELEASE(m_id_staging);
	}

	void GenerateMipMaps()
	{
		DX11Data *dx11data = (DX11Data*) m_h3ddata;
		dx11data->pContext->GenerateMips(m_sr_view);
	}

	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_rt_view;
	}
	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return m_sr_view;
	}

	ID3D11Texture2D *GetTexture() {return m_id;}
};


class HDX11Texture3D : public H3DTexture3D
{
private:
	ID3D11Texture3D *m_id;
	ID3D11Texture3D *m_id_staging;
	ID3D11ShaderResourceView *m_view;
	D3D11_MAPPED_SUBRESOURCE m_mapped_data;

public:
	HRESULT hr;

public:
	HDX11Texture3D(H3DData *h3ddata, 
		unsigned int width, 
		unsigned int height,
		unsigned int depth,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :
		H3DTexture3D(h3ddata, width, height, depth, levels, usage, format)
	{
		DX11Data *dx11data = (DX11Data *) h3ddata;
		m_h3ddata = h3ddata;

		D3D11_TEXTURE3D_DESC texdesc;
		texdesc.Width = width;
		texdesc.Height = height;
		texdesc.Depth = depth;
		texdesc.MipLevels = levels;
		texdesc.Format = dx11format(format);
		texdesc.CPUAccessFlags = 0;
		texdesc.MiscFlags = 0;

		if (ANYBIT(usage, H3DTEXUSAGE_RENDERTARGET | H3DTEXUSAGE_DEPTHSTENCIL)) {
			texdesc.Usage = D3D11_USAGE_DEFAULT;
			texdesc.BindFlags = D3D11_BIND_RENDER_TARGET;
		}
		else {
			texdesc.Usage = D3D11_USAGE_DEFAULT;
			texdesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
			if (ANYBIT(usage, H3DTEXUSAGE_AUTOGENMIPMAP))
				texdesc.MiscFlags |= D3D11_RESOURCE_MISC_GENERATE_MIPS;
		}

		switch (format)
		{
			case H3DFMT_L8:
				m_bytes_per_pixel = 1;
				break;
			case H3DFMT_R8G8B8:
				if (dx11data->supports_texture_rgb24) {
					m_bytes_per_pixel = 3;
					break;
				}
			case H3DFMT_DXT1:
				m_bytes_per_pixel = -1;
				break;
			case H3DFMT_DXT3:
				m_bytes_per_pixel = -1;
				break;
			case H3DFMT_DXT5:
				m_bytes_per_pixel = -1;
				break;
		}

		hr = dx11data->pDevice->CreateTexture3D(&texdesc, 0, &m_id);
		hr = dx11data->pDevice->CreateShaderResourceView(m_id, 0, &m_view);
	}

	~HDX11Texture3D()
	{
		H_SAFE_RELEASE(m_view);
		H_SAFE_RELEASE(m_id);
		H_SAFE_RELEASE(m_id_staging);
	};

	bool Lock(int bytes, unsigned char ** data)
	{
		UNREFERENCED(bytes);
		HRESULT hr;
		DX11Data *dx11data = (DX11Data *) m_h3ddata;

		// create a staging resource
		D3D11_TEXTURE3D_DESC texdesc;
		m_id->GetDesc(&texdesc);
		texdesc.Usage = D3D11_USAGE_STAGING;
		texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = dx11data->pDevice->CreateTexture3D(&texdesc, 0, &m_id_staging);
		hr = dx11data->pContext->Map(m_id_staging, 0, D3D11_MAP_WRITE, 0, &m_mapped_data);
		
		*data = (unsigned char *) m_mapped_data.pData;
		return SUCCEEDED(hr);
	}

	bool Lock(unsigned char ** data, int * pitch, int *slicepitch)
	{
		DX11Data *dx11data = (DX11Data *) m_h3ddata;

		// create a staging resource
		D3D11_TEXTURE3D_DESC texdesc;
		m_id->GetDesc(&texdesc);
		texdesc.Usage = D3D11_USAGE_STAGING;
		texdesc.BindFlags = 0;
		texdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		hr = dx11data->pDevice->CreateTexture3D(&texdesc, 0, &m_id_staging);
		hr = dx11data->pContext->Map(m_id_staging, 0, D3D11_MAP_WRITE, 0, &m_mapped_data);
		
		*data = (unsigned char *) m_mapped_data.pData;
		if (pitch)
			*pitch = m_mapped_data.RowPitch;
		if (slicepitch)
			*slicepitch = m_mapped_data.DepthPitch;
		return SUCCEEDED(hr);
	}

	void Unlock()
	{
		DX11Data *dx11data = (DX11Data *) m_h3ddata;

		dx11data->pContext->Unmap(m_id_staging, 0);
		dx11data->pContext->CopyResource(m_id, m_id_staging);

		ZERO_STRUCT(&m_mapped_data, D3D11_MAPPED_SUBRESOURCE);
		H_SAFE_RELEASE(m_id_staging);
	}

	void GenerateMipMaps()
	{
		DX11Data *dx11data = (DX11Data*) m_h3ddata;
		dx11data->pContext->GenerateMips(m_view);
	}

	/* HDX11Texture specific methods */
	/********************************/
	ID3D11ShaderResourceView* GetShaderResourceView()
	{
		return m_view;
	}

	ID3D11Texture3D *GetTexture() {return m_id;}
};

/******************************************************************************
*****************************************************************************
						HDX11Actions Class
*****************************************************************************
*****************************************************************************/
class HDX11Actions : public H3DActions
{
private:
	DX11Data *m_dx11data;	

// Turn on debugger output
// #define DX11_TRACE_TO_DEBUGGER

	void Trace(wchar_t const *event, wchar_t const *label) {
#ifdef DX11_TRACE_TO_DEBUGGER
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

	D3DImageHelper d3d_image_helper;

	HDX11Actions()
	{

	};

	HDX11Actions(DX11Data *dx11data) : H3DActions(dx11data), m_dx11data(dx11data) 
	{

	};

	~HDX11Actions()
	{

	};

	Int_XY GetWindowSize() const {
		Int_XY size;
		size.x = m_dx11data->swap_chain_desc.BufferDesc.Width; 
		size.y = m_dx11data->swap_chain_desc.BufferDesc.Height;
		return size;
	}

	void Begin_Trace(wchar_t const *label) {
		D3DPERF_BeginEvent(0, label);
		if (m_dx11data->pTrace) {
			m_dx11data->pTrace->BeginEvent(label);
		}

		Trace( L"Enter", label );
	};
	void End_Trace(wchar_t const *label) {
		Trace( L"Exit", label );
		
		if (m_dx11data->pTrace) {
			m_dx11data->pTrace->EndEvent();
		}
		D3DPERF_EndEvent();
	};

	H3DGenericVertexBufferCache *CreateGenericVertexBufferCache() {return NEW(HDX11GenericVertexBufferCache)(m_h3ddata);}
	H3DVertexBufferCache *CreateVertexBufferCache() {return NEW(HDX11VertexBufferCache)();}
	H3DIndexBufferCache *CreateIndexBufferCache() {return NEW(HDX11IndexBufferCache)();}
	H3DIndexedBufferObject* CreateIndexedBufferObject(int index, unsigned int flags) { return NEW(HDX11IndexedBufferObject)(index, flags); }

	void Clear(int flags, int color, float z_value, int stencil_value, bool simple_clear)
	{
		int clearflags = 0;
		ENSURE_CHANNEL_MASK(m_dx11data, H3DMASK_RGBA);
		float unpacked_color[] = {
			(color & 0x000000ff) / 255.0f,
			((color & 0x0000ff00) >> 8) / 255.0f,
			((color & 0x00ff0000) >> 16) / 255.0f, 
			((color & 0xff000000) >> 24) / 255.0f
		};

		// Clear the regular targets
		ID3D11RenderTargetView *render_target_view[3];
		ID3D11DepthStencilView *depth_stencil_view;
		m_dx11data->pContext->OMGetRenderTargets(3, render_target_view, &depth_stencil_view);
		if (ANYBIT(flags, H3DCLEAR_TARGET)) {
			m_dx11data->pContext->ClearRenderTargetView(render_target_view[0],unpacked_color);
		}
		if (ANYBIT(flags, H3DCLEAR_ZBUFFER | H3DCLEAR_STENCIL)) {
			if (ANYBIT(flags, H3DCLEAR_ZBUFFER))
				clearflags |= D3D11_CLEAR_DEPTH;
			if (ANYBIT(flags, H3DCLEAR_STENCIL))
				clearflags |= D3D11_CLEAR_STENCIL;
			m_dx11data->pContext->ClearDepthStencilView(depth_stencil_view, clearflags, z_value, (char) stencil_value);
		}

		if (simple_clear)
			goto cleanup;

		// Clear depth texture if necessary
		if (m_dx11data->depth_effects.mode && !m_dx11data->shadow_maps.mode) {
			// If the depth buffer was cleared, clear the depth texture to a matching color value.
			if (ANYBIT(flags, H3DCLEAR_ZBUFFER)) {

				// If we're clearing to the far plane, make sure we don't hit it.				
				if (z_value == 1.0f)
					z_value = 254.0f/255.0f;

				float depth_color[4] = {z_value, z_value, z_value, z_value};
							
				if (m_dx11data->depth_effects.must_pack_rgba) {
					pack_rgba(z_value, depth_color);
				}
				
				int const depth_location = m_dx11data->ssaa_line_target.mode ? 2 : 1;
				m_dx11data->pContext->ClearRenderTargetView(render_target_view[depth_location], depth_color);
			}
		}

cleanup:
		H_SAFE_RELEASE(render_target_view[0]);
		H_SAFE_RELEASE(render_target_view[1]);
		H_SAFE_RELEASE(render_target_view[2]);
		H_SAFE_RELEASE(depth_stencil_view);
	}


	void SetViewport(H3DViewport const *hvp)
	{
		D3D11_VIEWPORT vp = dx11viewport(hvp);
		if ( m_dx11data->ssaa_active ) {
			vp.TopLeftX *= m_dx11data->ssaa_width_scale;
			vp.TopLeftY *= m_dx11data->ssaa_height_scale;
			vp.Width *= m_dx11data->ssaa_width_scale;
			vp.Height *= m_dx11data->ssaa_height_scale;
		}
		m_dx11data->pContext->RSSetViewports(1, &vp);
	}

	void set_depth_bias (float depth_bias, float slope_bias) {
		m_dx11data->pRenderStates->rasterizer_desc.DepthBias				= (int) depth_bias;
		m_dx11data->pRenderStates->rasterizer_desc.SlopeScaledDepthBias	= slope_bias;
	}

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

	bool bind_texture(const Net_Rendition & nr, Texture * texture, int usage, H3DTexture * id, int texture_unit);


	/********************/
	/* Texture Creation */
	/********************/
	bool CreateTexture(unsigned int width, unsigned int height, unsigned int levels, unsigned int usage, H3DFORMAT format, H3DTexture **id);
	bool CreateCubeTexture(unsigned int size, unsigned int levels, unsigned int usage, H3DCubeTexture **id);
	bool CreateTexture3D(unsigned int width, unsigned int height, unsigned int depth, unsigned int levels, unsigned int usage, H3DFORMAT format, H3DTexture3D **id);
	bool CreateRenderTarget(unsigned int width, unsigned int height, unsigned int usage, H3DFORMAT z_format, unsigned int msaa, H3DRenderTarget **id);
	void SetTexture(int unit, H3DTexture *id)
	{
		ENSURE_TEXTURE(m_dx11data, unit, id);
	}


	/*******************/
	/* Shader Creation */
	/*******************/
	H3DShader *CreateShader(const H3DShaderID &id, size_t size) 
	{
		ASSERT(size == sizeof(H3DShader));
		return NEW(HDX11Shader)(id);
	};

	/******************/
	/* Region actions */
	/******************/
	void create_region (Net_Rendition const &  nr,
			IntRectangle const * area, void **image, void **z);

	void save_region (Net_Rendition const &  nr,
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
		mode = mode && m_dx11data->render_target->m_msaa > 1;
		m_dx11data->cache->antialias = mode;
		m_dx11data->pRenderStates->rasterizer_desc.MultisampleEnable = mode;			
		m_dx11data->pRenderStates->rasterizer_state_dirty = true;	
	}

	void force_blend(H3DBLEND src, H3DBLEND dest)
	{
		m_dx11data->cache->src_blend = src;
		m_dx11data->cache->dest_blend = dest;

		m_dx11data->pRenderStates->blend_desc.RenderTarget[0].SrcBlend = dx11blend(src);
		m_dx11data->pRenderStates->blend_desc.RenderTarget[0].SrcBlendAlpha = dx11blend(src);
		m_dx11data->pRenderStates->blend_desc.RenderTarget[0].DestBlend = dx11blend(dest);
		m_dx11data->pRenderStates->blend_desc.RenderTarget[0].DestBlendAlpha = dx11blend(dest);

		if (ANYBIT(m_h3ddata->dc->options.bits, Drivo_WINDOW_OPACITY)) {
			m_dx11data->pRenderStates->blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			m_dx11data->pRenderStates->blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		}

		m_dx11data->pRenderStates->blend_state_dirty = true;
	}

	void force_color_mask(bool mode, bool lock_it=false)
	{
		if (!m_dx11data->cache->color_mask_locked) {
			m_dx11data->cache->color_mask = mode;
			m_dx11data->cache->color_mask_locked = lock_it;
			UINT8 rtMask = mode ? (UINT8)dx11colormask(m_dx11data->cache->channel_mask) : 0;
			m_dx11data->pRenderStates->blend_desc.RenderTarget[0].RenderTargetWriteMask = rtMask;
			if (m_dx11data->ssaa_line_target.mode)
				m_dx11data->pRenderStates->blend_desc.RenderTarget[1].RenderTargetWriteMask = rtMask;
			m_dx11data->pRenderStates->blend_state_dirty = true;
		}
		else if (mode && m_dx11data->cache->color_mask) {
			UINT8 rtMask = (UINT8)dx11colormask(m_dx11data->cache->channel_mask);
			m_dx11data->pRenderStates->blend_desc.RenderTarget[0].RenderTargetWriteMask = rtMask;
			if (m_dx11data->ssaa_line_target.mode)
				m_dx11data->pRenderStates->blend_desc.RenderTarget[1].RenderTargetWriteMask = rtMask;
			m_dx11data->pRenderStates->blend_state_dirty = true;
		}
	}

	void force_channel_mask(H3DMASK mask)
	{
		m_dx11data->cache->channel_mask = mask;
		force_color_mask(m_dx11data->cache->color_mask);
	}

	void force_culling(H3DCULL mode)
	{
		m_dx11data->cache->cull_mode = mode;

		if (mode == H3DCULL_NONE)
			m_dx11data->culling = false;
		else
			m_dx11data->culling = true;
		m_dx11data->pRenderStates->rasterizer_desc.CullMode = dx11cullmode(mode);
		m_dx11data->pRenderStates->rasterizer_state_dirty = true;
	}

	void force_depth_mask(bool mode)
	{
		m_dx11data->cache->depth_mask = mode;
		if (mode)
			m_dx11data->pRenderStates->depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		else
			m_dx11data->pRenderStates->depth_stencil_desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;

		bool const depth_peeling = m_dx11data->depth_peeling.mode && m_dx11data->depth_peeling.use_mrt;

		if (m_dx11data->depth_effects.mode || depth_peeling) {
			int const depth_location = m_dx11data->ssaa_line_target.mode ? 2 : 1;
			m_dx11data->pRenderStates->blend_desc.RenderTarget[depth_location].RenderTargetWriteMask = mode ? D3D11_COLOR_WRITE_ENABLE_ALL : 0;
			m_dx11data->pRenderStates->blend_state_dirty = true;
		}
	}

	void force_depth_range_set(float zmin, float zmax)
	{
		unsigned int num_viewports = 1;
		m_dx11data->pContext->RSGetViewports(&num_viewports, &m_dx11data->pRenderStates->viewport_desc);
		m_dx11data->pRenderStates->viewport_desc.MinDepth = m_dx11data->cache->z_range[0] = zmin;
		m_dx11data->pRenderStates->viewport_desc.MaxDepth = m_dx11data->cache->z_range[1] = zmax;
		m_dx11data->pContext->RSSetViewports(1, &m_dx11data->pRenderStates->viewport_desc);
	}

	void force_fog (Net_Rendition const & nr, bool mode)
	{
		UNREFERENCED(nr);
		m_dx11data->cache->fog_on = mode; // keep this from getting call constantly
	}

	void force_point_sprites (bool enable, float min_size, float max_size)
	{
		m_dx11data->cache->point_sprites_enabled = enable;
		m_dx11data->dx11cache->point_sprites_min = min_size;
		m_dx11data->dx11cache->point_sprites_max = max_size;
	}

	void force_sampler_state (
		int texture_unit, 
		H3DTEXTUREFILTERTYPE filter, 
		H3DTEXTUREFILTERTYPE mip_filter, 
		H3DTEXTUREADDRESS wrap)
	{
		ENSURE_TEXTURE_MIN_FILTER(m_dx11data, texture_unit, filter);
		ENSURE_TEXTURE_MAG_FILTER(m_dx11data, texture_unit, filter);
		ENSURE_TEXTURE_MIP_FILTER(m_dx11data, texture_unit, mip_filter);
		ENSURE_TEXTURE_WRAP_S(m_dx11data, texture_unit, wrap);
		ENSURE_TEXTURE_WRAP_T(m_dx11data, texture_unit, wrap);
	}

	void force_scissor(bool mode)
	{
		INVALIDATE_SCISSOR_SET(m_dx11data);
		m_dx11data->cache->scissoring = mode;

		m_dx11data->pRenderStates->rasterizer_desc.ScissorEnable = mode;
		m_dx11data->pRenderStates->rasterizer_state_dirty = true;
	}

	void force_scissor_set(int in_left, int in_bottom, int in_right, int in_top)
	{
		D3D11_RECT scr_rect;
		scr_rect.left = in_left;
		scr_rect.top = m_dx11data->yfudge - in_top;
		scr_rect.right = in_right + 1;
		scr_rect.bottom = m_dx11data->yfudge - in_bottom + 1;

		if ( m_dx11data->ssaa_active ) {
			scr_rect.left *= m_dx11data->ssaa_width_scale;
			scr_rect.right *= m_dx11data->ssaa_width_scale;
			scr_rect.top *= m_dx11data->ssaa_height_scale;
			scr_rect.bottom *= m_dx11data->ssaa_height_scale;
		}

		m_dx11data->pContext->RSSetScissorRects(1, &scr_rect);

		m_h3ddata->cache->scissor_rect.left = in_left;
		m_h3ddata->cache->scissor_rect.bottom = in_bottom; 
		m_h3ddata->cache->scissor_rect.right = in_right; 
		m_h3ddata->cache->scissor_rect.top = in_top; 
	}

	void force_shade_mode(H3DSHADEMODE mode)
	{
		// Implemented in shader
		m_dx11data->cache->shade_mode = mode;
	}

	void force_alphatesting(bool mode)
	{
		// Implemented in shader
		m_dx11data->cache->alphatesting = mode;
		return;
	}

	void force_alphafunc(H3DCMPFUNC func, int ref)
	{
		// Implemented in shader
		m_dx11data->cache->alpha_func = func;
		m_dx11data->cache->alpha_ref = ref;
		return;
	}

	void force_stencilling(bool mode)
	{
		m_dx11data->cache->stencilling = mode;
		m_dx11data->pRenderStates->depth_stencil_desc.StencilEnable = mode;
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}

	void force_stencil_func(H3DCMPFUNC cmp)
	{
		m_dx11data->cache->stencil_func = cmp;
		m_dx11data->pRenderStates->depth_stencil_desc.FrontFace.StencilFunc = dx11cmpfunc(cmp);
		m_dx11data->pRenderStates->depth_stencil_desc.BackFace.StencilFunc = dx11cmpfunc(cmp);
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}

	void force_stencil_mask(int val)
	{
		m_dx11data->cache->stencil_mask = val;
		m_dx11data->pRenderStates->depth_stencil_desc.StencilReadMask = (char)val;
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}

	void force_stencil_ref(int val)
	{
		// passed into pContext->OMSetDepthStencilState method
		m_dx11data->cache->stencil_ref = val;
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}

	void force_stencil_write_mask(unsigned int val)
	{
		m_dx11data->cache->stencil_write_mask = val;
		m_dx11data->pRenderStates->depth_stencil_desc.StencilWriteMask = (char)val;
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}

	void force_stencil_op(H3DSTENCILOP fail, H3DSTENCILOP zfail, H3DSTENCILOP pass)
	{
		m_dx11data->cache->stencil_fail = fail;
		m_dx11data->cache->stencil_zfail = zfail;
		m_dx11data->cache->stencil_pass = pass;

		D3D11_DEPTH_STENCIL_DESC & depth_stencil_desc = m_dx11data->pRenderStates->depth_stencil_desc;

		depth_stencil_desc.FrontFace.StencilFailOp = dx11stencilop(fail);
		depth_stencil_desc.FrontFace.StencilDepthFailOp = dx11stencilop(zfail);
		depth_stencil_desc.FrontFace.StencilPassOp = dx11stencilop(pass);

		depth_stencil_desc.BackFace.StencilFailOp = dx11stencilop(fail);
		depth_stencil_desc.BackFace.StencilDepthFailOp = dx11stencilop(zfail);
		depth_stencil_desc.BackFace.StencilPassOp = dx11stencilop(pass);
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}

	void set_stencil_func(H3DCMPFUNC cmp, unsigned long ref, unsigned long mask, unsigned long writemask)
	{
		if (cmp != m_dx11data->cache->stencil_func)
			force_stencil_func(cmp);
		if (ref != m_dx11data->cache->stencil_ref)
			force_stencil_ref(ref);
		if (mask != m_dx11data->cache->stencil_mask)
			force_stencil_mask(mask);
		if (writemask != m_dx11data->cache->stencil_write_mask)
			force_stencil_write_mask(writemask);
	}

	void force_texture (int unit, H3DTexture *texture)
	{
		ASSERT(unit < H3D_MAX_TEXTURES);
		m_dx11data->cache->texture[unit] = texture;
		
		if ( texture ) {
			texture->texcache = &m_dx11data->cache->texture[unit];

			// SIL: Different downcasts required for 2D and cube textures.
			if ( texture->IsCubeTexture() ) {
				HDX11CubeTexture *dx11_texture = static_cast<HDX11CubeTexture*>(texture);
				ID3D11ShaderResourceView *view = dx11_texture->GetShaderResourceView();
				m_dx11data->pContext->PSSetShaderResources(unit, 1, &view);
			}
			else if ( texture->IsTexture3D() ) {
				HDX11Texture3D *dx11_texture = static_cast<HDX11Texture3D*>(texture);
				ID3D11ShaderResourceView *view = dx11_texture->GetShaderResourceView();
				m_dx11data->pContext->PSSetShaderResources(unit, 1, &view);
			}
			else {
				HDX11Texture2D *dx11_texture = static_cast<HDX11Texture2D*>(texture);
				ID3D11ShaderResourceView *view = dx11_texture->GetShaderResourceView();
				m_dx11data->pContext->PSSetShaderResources(unit, 1, &view);
			}
		}
		else {
			ID3D11ShaderResourceView *unset = null;
			m_dx11data->pContext->PSSetShaderResources(unit, 1, &unset);
		}
		m_dx11data->dc->stats->bind_texture++;
	}
	void ensure_texture (int unit, H3DTexture *texture)
	{
		if (m_dx11data->cache->texture[unit] != texture)
			FORCE_TEXTURE(m_dx11data, unit, texture);
	}

	void force_transparency(bool mode)
	{
		m_dx11data->cache->transparency_on = mode;
		m_dx11data->pRenderStates->blend_desc.RenderTarget[0].BlendEnable = mode;
		
		if (ANYBIT(m_h3ddata->dc->options.bits, Drivo_WINDOW_OPACITY)) {
			m_dx11data->pRenderStates->blend_desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
			m_dx11data->pRenderStates->blend_desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		}

		m_dx11data->pRenderStates->blend_state_dirty = true;
	}

	void force_zbuffering(bool mode)
	{
		m_dx11data->cache->zbuffering = mode;

		if (!mode)
			m_dx11data->pRenderStates->depth_stencil_desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		else {
			if (m_dx11data->cache->depth_test_reversed)
				m_dx11data->pRenderStates->depth_stencil_desc.DepthFunc = D3D11_COMPARISON_GREATER;
			else
				m_dx11data->pRenderStates->depth_stencil_desc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		}
		m_dx11data->pRenderStates->depth_stencil_state_dirty = true;
	}


	void draw_textured_rectangle(H3DTexture * texture, int x, int y, int width, int height, float u1, float	v1, float u2, float	v2);

	bool Snapshot(Net_Rendition const & nr, int w, int h, int bpp, void * data);

	void * IM_Get_Surface(Net_Rendition const & nr)
	{
		if (m_dx11data->dc_type == DCT_IMAGE && ANYBIT (m_dx11data->dc->options.bits, Drivo_GPU_RESIDENT)) {
			H3DTexture * texture = ensure_gpu_res_texture(nr);
			return d3d_image_helper.Get_DX9Ex_Shared_Surface(m_dx11data->hWND, static_cast<HDX11Texture2D *>(texture)->GetTexture());			
		}

		return ((HDX11RenderTarget*)m_dx11data->render_target)->GetTexture();
	}

	virtual H3DTexture * ensure_gpu_res_texture(Net_Rendition const & nr) {

		if (ANYBIT(m_h3ddata->dc->options.bits, Drivo_WINDOW_OPACITY))
			return H3DActions::ensure_gpu_res_texture(nr);

		Display_List *dl = null;
		Texture_H3DDL *	todl = null;

		m_h3ddata->output_image->flags |= IF_DL_ONLY;
		bool const list_new = get_display_list(nr, &m_h3ddata->output_image->display_lists, dl, todl, H3D_DL_TEXTURE);		
		if (list_new) {

			int const w = m_h3ddata->output_image->width;
			int const h = m_h3ddata->output_image->height;			

			todl->id = NEW(HDX11Texture2D)(m_h3ddata, w, h, 1, H3DTEXUSAGE_RENDERTARGET, H3DFMT_X8R8G8B8, true);
			
			todl->peer->owner = (Anything*)m_h3ddata->output_image;
		}

		return todl->id;

	}	
};

#endif // __DX11DRIVE_H_DEFINED__


