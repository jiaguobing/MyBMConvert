// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef __OGL2_POST_H_DEFINED__
#define __OGL2_POST_H_DEFINED__


#include "3d_common.h"

/*forward declarations*/
class HOGLPost;
struct HOGLData;


class HOGLConstantTable : public H3DConstantTable
{
private:
	GLuint m_shader_program;
	HOGLUniformCache m_uniforms;

#define MAX_SAMPLER_INDEX 10
	H3D_Shader_Handle m_sampler_cache[MAX_SAMPLER_INDEX];	//arbitrary size for now

public:
	HOGLConstantTable(HOGLData * ogldata, GLuint table): H3DConstantTable(ogldata), m_shader_program(table), m_uniforms(ogldata)
	{
		SET_MEMORY(&m_sampler_cache[0], sizeof(m_sampler_cache), INITIAL_SHADER_HANDLE);
		m_uniforms.SetProgram(m_shader_program);
	};
	~HOGLConstantTable() {};

	void SetFloat(const char *name, const float value);
	void SetInt(const char *name, const int value);
	void SetFloatArray(const char *name, const float *values, int count);
	void SetMatrix(const char *name, const float *matrix);
	void SetVector(const char *name, const float *vector, const int vector_width = 4);
	void SetVectorArray(const char *name, const float *vectors, const int num_vectors, const int vector_width = 4);
	unsigned long GetSamplerIndex(const char *name);
};


class HOGLPost : public H3DPost
{
public:

	HOGLPost( H3DData *data );
	~HOGLPost();

	bool Begin();					// Start a post session
	void End();						// Finish a post session

	void Copy(H3DRenderTarget *dest, H3DTexture * src, RGBA const *modulation_color, H3DTEXTUREFILTERTYPE filter);

	void Copy(H3DTexture *dest, H3DTexture * src, RGBA const *modulation_color, H3DTEXTUREFILTERTYPE filter) {
		Copy((H3DRenderTarget *) dest, src, modulation_color, filter);
	}

	void Filter(H3DRenderTarget *dest, H3DTexture *src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT);

	void Filter(H3DTexture *dest, H3DTexture *src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT) {
		Filter((H3DRenderTarget *) dest, src, num_taps, taps, filter);
	}


	void CustomFilter(H3DRenderTarget *dest, H3DTexture *src, Shader which,int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT);

	void CustomFilter(H3DTexture *dest, H3DTexture *src, Shader which,int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT) {
		CustomFilter((H3DRenderTarget *) dest, src, which, num_taps, taps, filter);
	}

	void Add( H3DRenderTarget * dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT );

	void Add( H3DTexture * dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = NULL, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		Add((H3DRenderTarget *) dest, srcs, num_srcs, modulation_color, filter);
	}

	void DepthFilter(H3DRenderTarget * dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter);

	void DepthFilter( H3DTexture * dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		DepthFilter((H3DRenderTarget *) dest, src, depth, num_taps, taps, filter);
	}

	// Apply the specified shader to the target
	void Apply( H3DRenderTarget *dest, Shader const which, float depth = 0.0f, float const *matrix = null, H3DTexture *offset_texture = null  )
	{
		m_h3ddata->h3d_actions->set_targets(dest);
		if (ActivateShader(which, depth, matrix, offset_texture))
		{
			// Draw...
			m_vb->Draw(m_h3ddata);
		}
	}

	H3DConstantTable * ActivateShader(Shader which, float depth = 0.0f, float const *matrix = null, H3DTexture *offset_texture = null);

	H3DConstantTable* GetConstants( Vertex_Shader const which_vs, Shader const which );

	// Access a shader's quality level
	INLINE ShaderQuality GetQuality( Shader const which )	{ return m_quality[which]; }

	bool LinkShader(Vertex_Shader const vs, Shader const ps, bool quiet);

	// Reload shaders from disk
	void OnReloadShaders()
	{
		DestroyShaders();
		CreateVertexShaders();
	}

private:
	RenderTargets m_saved_targets;

	GLuint m_shader_programs[H3DPost::NUM_VERTEX_SHADERS][H3DPost::NUM_SHADERS];		// Shader programs (one for each vs/ps combo)
	GLuint m_2d_vertex_shader;								// Common vertex shader
	GLuint m_3d_vertex_shader;								// Common vertex shader for 3D
	GLuint m_pixel_shaders[H3DPost::NUM_SHADERS];			// Different pixel shaders
	ShaderQuality m_quality[NUM_SHADERS];					// Quality level in use for each shader.

	// Helpers
	void CreateVertexShaders();								// Create/compile vertex shaders
	void CreatePixelShader(Shader which);					// Create/compile pixel shader
	void DestroyShaders();									// Destroy all shaders
};

#endif // __OGL2_POST_H_DEFINED__
