// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef __DX11POST_H_DEFINED__
#define __DX11POST_H_DEFINED__

/*forward declarations*/
class HDX11Post;
struct DX11Data;

// Only to be used with HDX11Post
class HDX11ConstantTable : public H3DConstantTable
{
private:
	float **m_buffer;

	ID3D11Buffer **m_pConstants;

	ID3D11ShaderReflection *m_shader_reflection;
	D3D11_SHADER_DESC m_shader_desc;

public:
	HDX11ConstantTable(H3DData *h3ddata, ID3D10Blob *shader_blob):
		H3DConstantTable(h3ddata)
	{
		HRESULT hr;
		ID3D11ShaderReflectionConstantBuffer *cbuf;
		D3D11_SHADER_BUFFER_DESC cdesc;
		DX11Data *dx11data = (DX11Data *)h3ddata;

		// Get shader reflection
		D3DReflect(shader_blob->GetBufferPointer(), shader_blob->GetBufferSize(),
			IID_ID3D11ShaderReflection, (void **)&m_shader_reflection);
		m_shader_reflection->GetDesc(&m_shader_desc);

		// Create constant buffers (and scratch spaces)
		m_buffer = new float*[m_shader_desc.ConstantBuffers];
		m_pConstants = new ID3D11Buffer*[m_shader_desc.ConstantBuffers];
		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			cbuf = m_shader_reflection->GetConstantBufferByIndex(i);
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
			bd.ByteWidth = cdesc.Size;
			hr = dx11data->pDevice->CreateBuffer(&bd, null, &m_pConstants[i]);
		}
	};
	~HDX11ConstantTable()
	{
		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			H_SAFE_RELEASE(m_pConstants[i]);
			delete [] m_buffer[i];
		}
		delete [] m_pConstants;
		delete [] m_buffer;
	}


	void SetInt(const char *name, const int data)
	{
		int *constant = (int*) GetConstantByName("", name);
		if (constant)
			*constant = data;
	}

	void SetFloat(const char *name, const float data)
	{
		float *constant = (float*) GetConstantByName("", name);
		if (constant)
			*constant = data;
	}

	void SetMatrix(const char *name, const float *matrix)
	{
		SetFloatArray(name, matrix, H3D_Matrix, 16);
	}

	void SetFloatArray(const char *name, void const *data, const int datatype, int num = 1)
	{
		float *constant = (float*) GetConstantByName("", name);
		if (constant) {
			// Some datatypes need special handling, for example, a float3 or float3x3 is actually treated as
			// a float4 or float4x4 in the constant buffer
			if (datatype == H3D_Matrix && num == 9) {
				for (int i = 0; i < 3; i++) {
					COPY_MEMORY((float*)data + i*3, 3*sizeof(float), constant + i*4);
				}
			}
			else {
				COPY_MEMORY(data, num*sizeof(float), constant);
			}
		}
	}

	void SetVector(const char *name, const float *data, const int width)
	{
		float *constant = (float*) GetConstantByName("", name);
		if (constant)
			COPY_MEMORY(data, sizeof(float)*width, constant);
	}
	void SetVectorArray(const char *name, const float *data, const int num_vectors, const int width)
	{
		float *constant = (float*) GetConstantByName("", name);
		if (constant) {
			for (int i = 0; i < num_vectors; i++) {
				COPY_MEMORY(data+i*width, sizeof(float)*width, constant+i*4);
			}
		}
	}

	H3D_Shader_Handle GetConstantByName(char const * parent, char const * constant)
	{
		ASSERT(strlen(parent) == 0); // not handled yet
		ID3D11ShaderReflectionConstantBuffer *cbuf;
		char scratch[1024];
		HRESULT hr = S_OK;

		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			cbuf = m_shader_reflection->GetConstantBufferByIndex(i);

			D3D11_SHADER_VARIABLE_DESC vdesc;
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
				switch(constant_type_desc.Class) {
					case D3D10_SVC_SCALAR: array_offset = 4; break;
					case D3D10_SVC_VECTOR: array_offset = 16; break;
					default: ASSERT(0);
				}
				array_offset *= array_index;
			}
			
			if (SUCCEEDED(hr)) {
				return (H3D_Shader_Handle) &m_buffer[i][(vdesc.StartOffset + array_offset) / 4];
			}
		}
		return 0;
	}

	void ApplyConstants(H3D_Constant_Table table)
	{
		DX11Data *dx11data = (DX11Data *)m_h3ddata;
		for (unsigned int i = 0; i < m_shader_desc.ConstantBuffers; i++) {
			dx11data->pContext->UpdateSubresource(m_pConstants[i], 0, null, m_buffer[i], 0, 0);
		}

		switch(table) {
			case H3D_VSConstant_Table:
				dx11data->pContext->VSSetConstantBuffers(0, m_shader_desc.ConstantBuffers, m_pConstants);
				break;

			case H3D_PSConstant_Table:
				dx11data->pContext->PSSetConstantBuffers(0, m_shader_desc.ConstantBuffers, m_pConstants);
				break;

			case H3D_GSConstant_Table:
				dx11data->pContext->GSSetConstantBuffers(0, m_shader_desc.ConstantBuffers, m_pConstants);
				break;
		}
	}

	unsigned long GetSamplerIndex(const char *name) 
	{
		D3D11_SHADER_INPUT_BIND_DESC desc;
		HRESULT hr = m_shader_reflection->GetResourceBindingDescByName(name, &desc);
		if (SUCCEEDED(hr)) {
			ASSERT(desc.Type == D3D10_SIT_SAMPLER);
			return desc.BindPoint;
		}
		return 0;
	};
};

class HDX11Post : public H3DPost
{
public:

	HDX11Post( DX11Data& data );
	~HDX11Post();

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
	void Filter( ID3D11RenderTargetView *dest, H3DTexture * src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT );

	// Filter the specified texture to the target
	void Filter( H3DTexture * dest, H3DTexture * src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ID3D11RenderTargetView *dest_surface = ((HDX11Texture2D *) dest)->GetRenderTargetView();
		if (dest_surface)
			Filter( dest_surface, src, num_taps, taps, filter);
	}

	// Filter the specified texture to the target
	void DepthFilter( ID3D11RenderTargetView *dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT );

	// Filter the specified texture to the target (depth aware)
	void DepthFilter( H3DTexture * dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ID3D11RenderTargetView *dest_surface = ((HDX11Texture2D *) dest)->GetRenderTargetView();
		if (dest_surface)
			DepthFilter( dest_surface, src, depth, num_taps, taps, filter);
	}

	// Filter the specified texture to the target (custom shader)
	void CustomFilter( ID3D11RenderTargetView *dest, H3DTexture * src, Shader which, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT );

	// Filter the specified texture to the target (custom shader)
	void CustomFilter( H3DTexture * dest, H3DTexture * src, Shader which, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ID3D11RenderTargetView *dest_surface = ((HDX11Texture2D *) dest)->GetRenderTargetView();
		if (dest_surface)
			CustomFilter( dest_surface, src, which, num_taps, taps, filter);
	}

	// Add the specified textures together on the target
	void Add( ID3D11RenderTargetView *dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT );

	// Filter the specified texture to the target
	void Add( H3DTexture * dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ID3D11RenderTargetView *dest_surface = ((HDX11Texture2D *) dest)->GetRenderTargetView();
		if (dest_surface)
			Add( dest_surface, srcs, num_srcs, modulation_color, filter);
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
	void Apply( ID3D11RenderTargetView *dest, Shader const which, float depth = 0.0f, float const *matrix = NULL, H3DTexture *offset_texture = NULL  );

	// Apply the specified shader to the target
	void Apply( H3DRenderTarget *dest, Shader const which, float depth = 0.0f, float const *matrix = NULL, H3DTexture *offset_texture = NULL  )
	{
		if (dest) {
			if ( dest->IsTexture()) {
				H3DTexture *texture = static_cast<H3DTexture*>(dest);
				if ( !texture->IsCubeTexture() ) {
					ID3D11RenderTargetView *surface = static_cast<HDX11Texture2D*>(dest)->GetRenderTargetView();
					Apply( surface, which, depth, matrix, offset_texture );
				}
			}
			else {
				Apply( (ID3D11RenderTargetView*)static_cast<HDX11RenderTarget*>(dest)->GetView(), which, depth, matrix, offset_texture );
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

	virtual void Draw(H3DPost::Vertex_Shader which_vs, H3DPost::Shader which_ps)
	{
		// Set geometry		

		// Unset any previously bound geometry shader
		m_context->GSSetShader(NULL, NULL, 0);
		m_context->IASetInputLayout(m_format);
		m_h3ddata->cache->vf = H3DVertexFormat();				

		// Draw...
		if (which_vs == VS_3D)
			static_cast<HDX11ConstantTable*>(m_3d_vertex_constant_table)->ApplyConstants(H3D_VSConstant_Table);
		else
			static_cast<HDX11ConstantTable*>(m_2d_vertex_constant_table)->ApplyConstants(H3D_VSConstant_Table);
		static_cast<HDX11ConstantTable*>(m_constant_tables[0][which_ps])->ApplyConstants(H3D_PSConstant_Table);		

		m_dx11data->pRenderStates->ApplyRenderStates();
		H3DPost::Draw(which_vs, which_ps);		
	}

private:

	// Avoid C4512
	HDX11Post& operator=( const HDX11Post& other )			{ UNREFERENCED(other); ASSERT(0); return *this; }

	DX11Data *m_dx11data;									// DX11 driver state
	ID3D11Device *m_device;									// D3D device
	ID3D11DeviceContext *m_context;							// D3D device context	
	ID3D11InputLayout *m_format;							// Format descriptor for these vertices
	ID3D11VertexShader *m_2d_vertex_shader;					// Common vertex shader
	ID3D11VertexShader *m_3d_vertex_shader;					// Common vertex shader for 3D
	ID3D11PixelShader *m_pixel_shaders[NUM_SHADERS];		// Different pixel shaders

	ID3D10Blob *m_2d_vs_blob;								// Blob of Common vertex shader
	ID3D10Blob *m_3d_vs_blob;								// Blob of Common vertex shader for 3D
	ID3D10Blob *m_ps_blobs[NUM_SHADERS];					// Blobs of pixel shaders

	ShaderQuality m_quality[NUM_SHADERS];					// Quality level in use for each shader.

	RenderTargets m_saved_targets;

	// Helpers
	void CreateVertexShaders();								// Create/compile vertex shaders
	void CreatePixelShader(Shader which);					// Create/compile pixel shader
	void DestroyShaders();									// Destroy all shaders
};

#endif // __DX11POST_H_DEFINED__


