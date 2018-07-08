// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef __DX9POST_H_DEFINED__
#define __DX9POST_H_DEFINED__

/*forward declarations*/
class HDX9Post;
struct DX9Data;
struct IDirect3DDevice9;
struct IDirect3DVertexBuffer9;
struct IDirect3DVertexShader9;
struct IDirect3DPixelShader9;
//struct ID3DXConstantTable;

class HDX9ConstantTable : public H3DConstantTable
{
private:
	HD3DXConstantTable *m_table;

public:

	HDX9ConstantTable(H3DData *h3ddata, void *table): H3DConstantTable(h3ddata), m_table((HD3DXConstantTable*) table) 
	{

	};

	~HDX9ConstantTable() 
	{
		H_SAFE_RELEASE(m_table);
	};

	virtual void SetInt(const char *name, const int value)
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null, name);
		
		// TODO: Guard like this in all these functions?
		if (handle)
			m_table->SetInt(((DX9Data *)m_h3ddata)->pDevice, handle, value);
	}

	void SetFloat(const char *name, const float value)
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null,name);
		m_table->SetFloat(((DX9Data *)m_h3ddata)->pDevice, handle, value);
	}
	void SetFloatArray(const char *name, const float *values, int count)	
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null,name);
		m_table->SetFloatArray(((DX9Data *)m_h3ddata)->pDevice, handle, (const float *) values, count);
	}
	void SetMatrix(const char *name, const float *matrix)
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null,name);
		m_table->SetMatrix(((DX9Data *)m_h3ddata)->pDevice, handle, (const D3DXMATRIX *) matrix);
	}
	void SetVector(const char *name, const float *vector, const int vector_width = 4) 
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null,name);

		if (vector_width == 4)
			m_table->SetVector(((DX9Data *)m_h3ddata)->pDevice, handle, (const D3DXVECTOR4 *) vector);
		else
			m_table->SetFloatArray(((DX9Data *)m_h3ddata)->pDevice, handle, vector, vector_width);
	};
	void SetVectorArray(const char *name, const float *vectors, const int num_vectors, const int vector_width = 4) 
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null,name);

		if (vector_width == 4)
			m_table->SetVectorArray(((DX9Data *)m_h3ddata)->pDevice, handle, (const D3DXVECTOR4 *) vectors, num_vectors);
		else
			m_table->SetFloatArray(((DX9Data *)m_h3ddata)->pDevice, handle, vectors, num_vectors * vector_width);
	};
	unsigned long GetSamplerIndex(const char *name) 
	{
		D3DXHANDLE const handle = m_table->GetConstantByName(null,name);
		return m_table->GetSamplerIndex(handle);
	};
};

class HDX9Post : public H3DPost
{
public:

	HDX9Post( DX9Data& data );
	~HDX9Post();

	bool Begin();					// Start a post session
	void End();	// Finish a post session

	//
	// Drawing primitives
	//

	// Copy the specified texture to the target
	void Copy( H3DRenderTarget * dest, H3DTexture *src, RGBA const * modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT );

	// Copy the specified texture to the target
	void Copy( H3DTexture *dest, H3DTexture *src, RGBA const * modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		Copy( (H3DRenderTarget *)dest, src, modulation_color, filter );
	}

	// Filter the specified texture to the target.
	void Filter( IDirect3DSurface9& dest, H3DTexture * src, int num_taps, Vector const *taps, D3DTEXTUREFILTERTYPE filter = D3DTEXF_POINT );

	// Filter the specified texture to the target
	void Filter( H3DTexture * dest, H3DTexture * src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		IDirect3DSurface9 *dest_surface;
		((HDX9Texture2D *) dest)->GetSurfaceLevel(0, &dest_surface);
		if (dest_surface)
			Filter( *dest_surface, src, num_taps, taps, d3dtexturefilter(filter));

		H_SAFE_RELEASE(dest_surface);
	}

	// Filter the specified texture to the target
	void DepthFilter( IDirect3DSurface9& dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, D3DTEXTUREFILTERTYPE filter = D3DTEXF_POINT );

	// Filter the specified texture to the target (depth aware)
	void DepthFilter( H3DTexture * dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		IDirect3DSurface9 *dest_surface;
		((HDX9Texture2D *) dest)->GetSurfaceLevel(0, &dest_surface);
		if (dest_surface)
			DepthFilter( *dest_surface, src, depth, num_taps, taps, d3dtexturefilter(filter));

		H_SAFE_RELEASE(dest_surface);
	}

	// Filter the specified texture to the target (custom shader)
	void CustomFilter( IDirect3DSurface9& dest, H3DTexture * src, Shader which, int num_taps, Vector const *taps, D3DTEXTUREFILTERTYPE filter = D3DTEXF_POINT );

	// Filter the specified texture to the target (custom shader)
	void CustomFilter( H3DTexture * dest, H3DTexture * src, Shader which, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		IDirect3DSurface9 *dest_surface;
		((HDX9Texture2D *) dest)->GetSurfaceLevel(0, &dest_surface);
		if (dest_surface)
			CustomFilter( *dest_surface, src, which, num_taps, taps, d3dtexturefilter(filter));

		H_SAFE_RELEASE(dest_surface);
	}

	// Add the specified textures together on the target
	void Add( IDirect3DSurface9& dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = NULL, D3DTEXTUREFILTERTYPE filter = D3DTEXF_POINT );

	// Filter the specified texture to the target
	void Add( H3DTexture * dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		IDirect3DSurface9 *dest_surface;
		((HDX9Texture2D *) dest)->GetSurfaceLevel(0, &dest_surface);
		if (dest_surface)
			Add( *dest_surface, srcs, num_srcs, modulation_color, d3dtexturefilter(filter));

		H_SAFE_RELEASE(dest_surface);
	}

	// Access a shader in order to configure it
	INLINE H3DConstantTable *GetConstants( Vertex_Shader const which_vs, Shader const which )	
	{ 
		UNREFERENCED(which_vs); 
		if (!m_pixel_shaders[which]) 
			CreatePixelShader(which);
		return m_constant_tables[0][which]; 
	}

	// Access a shader's quality level
	INLINE ShaderQuality GetQuality( Shader const which )	{ return m_quality[which]; }

	// Activate a shader in order to render manually
	H3DConstantTable *ActivateShader( Shader which, float depth = 0.0f, float const *matrix = NULL, H3DTexture *offset_texture = NULL );

	// Apply the specified shader to the target
	void Apply( IDirect3DSurface9& dest, Shader const which, float depth = 0.0f, float const *matrix = NULL, H3DTexture *offset_texture = NULL  );

	// Apply the specified shader to the target
	void Apply( H3DRenderTarget *dest, Shader const which, float depth = 0.0f, float const *matrix = NULL, H3DTexture *offset_texture = NULL  )
	{
		if (dest) {
			if ( dest->IsTexture()) {
				H3DTexture *texture = static_cast<H3DTexture*>(dest);
				if ( !texture->IsCubeTexture() ) {
					IDirect3DSurface9 *surface = null;
					static_cast<HDX9Texture2D*>(texture)->GetSurfaceLevel(0, &surface);
					Apply(*surface, which, depth, matrix, offset_texture);
					H_SAFE_RELEASE(surface);
				}
			}
			else {
				Apply(*static_cast<HDX9RenderTarget*>(dest)->GetBuffer(), which, depth, matrix, offset_texture );
			}
		}
	}

	// Get a temporary render-target texture buffer from the cache
	H3DRenderTarget *GetWorkSurface( const DWORD width, const DWORD height, const H3DFORMAT format, bool msaa );

	// Reload shaders from disk
	void OnReloadShaders()
	{
		DestroyShaders();
		CreateVertexShaders();
	}

private:

	// Avoid C4512
	HDX9Post& operator=( const HDX9Post& other )			{ UNREFERENCED(other); ASSERT(0); return *this; }

	DX9Data& m_dx9data;										// DX9 driver state
	IDirect3DDevice9 &m_device;								// D3D device		
	IDirect3DVertexShader9 *m_2d_vertex_shader;				// Common vertex shader
	IDirect3DVertexShader9 *m_3d_vertex_shader;				// Common vertex shader for 3D
	IDirect3DPixelShader9 *m_pixel_shaders[NUM_SHADERS];	// Different pixel shaders
	ShaderQuality m_quality[NUM_SHADERS];					// Quality level in use for each shader.

	RenderTargets m_saved_targets;

	// Helpers
	void CreateVertexShaders();								// Create/compile vertex shaders
	void CreatePixelShader(Shader which);					// Create/compile pixel shader
	void DestroyShaders();									// Destroy all shaders
};

#endif // __DX9POST_H_DEFINED__


