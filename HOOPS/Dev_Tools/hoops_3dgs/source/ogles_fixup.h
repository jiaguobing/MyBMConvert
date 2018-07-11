// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

//#include "oglext.h"

static int throw_assert () {
    ASSERT(0);
    return 0;
}

#ifndef GL_EXT_texture_rg
#define GL_RED_EXT                                              0x1903
#define GL_RG_EXT                                               0x8227
#define GL_R8_EXT                                               0x8229
#define GL_RG8_EXT                                              0x822B
#endif

//#define GLEW_EXT_packed_depth_stencil		GL_OES_packed_depth_stencil
//#define GLEW_EXT_texture_filter_anisotropic	GL_EXT_texture_filter_anisotropic
#define GLEW_EXT_timer_query				0
#define GLEW_EXT_texture_compression_s3tc	0
#define GLEW_EXT_framebuffer_blit			(ogldata->feature_level == OGL2_ES3)
#define GLEW_EXT_texture_integer            0
#define GLEW_ARB_texture_float              0//GL_OES_texture_float
#define GLEW_ARB_shader_atomic_counters		0
#define GLEW_ARB_shader_image_load_store	0
#define GLEW_ARB_clear_buffer_object		0
#define GLEW_ARB_uniform_buffer_object		0
#define GLEW_ARB_gpu_shader_fp64			0
#define GLEW_ARB_vertex_attrib_64bit		0
#define GLEW_ARB_map_buffer_range			(ogldata->feature_level == OGL2_ES3)
#define GLEW_ARB_compute_shader				0
#define GLEW_ARB_shader_storage_buffer_object 0

#define GL_DOUBLE							0
typedef double GLdouble;

#define glUniformMatrix4dv(a, b, c, d)
#define glUniform3dv(a, b, c)
#define glVertexAttribLPointer(a, b, c, d, e)

#define GL_RGBA16F_ARB						throw_assert()
#define GL_RED_INTEGER_EXT                  throw_assert()
#define GL_RGBA32UI_EXT                     throw_assert()
#define GL_ALPHA_TEST						throw_assert()

//use the es equivalent for constants when available 

#define GL_DEPTH24_STENCIL8_EXT                 GL_DEPTH24_STENCIL8_OES
#define GL_UNSIGNED_INT_24_8_EXT                GL_UNSIGNED_INT_24_8_OES
#define GL_DEPTH_STENCIL_EXT                    GL_DEPTH_STENCIL_OES

#define GL_COMPRESSED_RGB_ARB                   0
#define GL_GEOMETRY_SHADER                      0
#define GL_LINES_ADJACENCY_EXT                  0x000A
#define GL_TRIANGLES_ADJACENCY_EXT              0x000C     
#define GL_TEXTURE_1D                           0

#ifndef GL_EXT_texture_compression_dxt1
#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT         0
#endif

#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT        0
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT        0
#define GL_MULTISAMPLE                          0

#define	GL_ATOMIC_COUNTER_BUFFER				throw_assert()
#define GL_R32UI								throw_assert()
#define GL_RGBA32UI								throw_assert()
#define GL_TEXTURE_BUFFER						throw_assert()
#define GL_READ_ONLY							throw_assert()
#define GL_WRITE_ONLY							throw_assert()
#define GL_READ_WRITE							throw_assert()
#define GL_UNIFORM_BUFFER						throw_assert()
#define GL_UNIFORM_BUFFER_BINDING				throw_assert()
#define GL_SHADER_STORAGE_BUFFER				throw_assert()
#define GL_SHADER_STORAGE_BUFFER_BINDING		throw_assert()
#define GL_COMPUTE_SHADER						throw_assert()



//throw asserts if we use a function for which there is no equivalent
#define glAlphaFunc(a,b) //throw_assert()
#define glBeginQuery(a,b) throw_assert()

#define glDeleteQueries(a,b) throw_assert()
#define glEndQuery(a) throw_assert()
#define glGetTexImage(a,b,c,d,e) throw_assert()
#define glGetTexLevelParameteriv(a,b,c,d) throw_assert()
#define glGenQueries(a,b) throw_assert()
#define glGetQueryObjectuiv(a,b,c) throw_assert()
#define glTexImage3D(a,b,c,d,e,f,g,h,i,j) throw_assert()
#define glTexSubImage3D(a,b,c,d,e,f,g,h,i,j,k) throw_assert()
#define glGenerateMipmapEXT glGenerateMipmap

//provide direct translation when possible, or in some cases silently do nothing.
//for the "do nothing" cases, it's our own responsibility to work around those limitations.
#define glDepthRange(a,b) glDepthRangef(a,b)
#define glClearDepth(a) glClearDepthf(a)
#define glShadeModel(a)					 //do nothing
#define glDrawArraysInstanced(a,b,c,d)//do nothing
#define glProgramParameteriEXT(a,b,c)    //do nothing

#define glTexBuffer(a, b, c)
#define glClearBufferData(a, b, c, d, e)
#define glBindImageTexture(a, b, c, d, e, f, g)
#define glBindBufferBase(a, b, c)
#define glUniform1ui glUniform1i

#define glDispatchCompute(a, b, c) throw_assert()


typedef void (GL_APIENTRY * PFNGLCOLORMASKIPROC) (GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
static PFNGLCOLORMASKIPROC glColorMaski = 0;

static void glDrawBuffer(GLenum buffer)
{
	glDrawBuffers(1, &buffer);
}

#define GLEWContext void

static void dump_floats (const char *label, GLenum which, int count) {
	GLfloat vals[64];
	ASSERT (count < 64);
	
	glGetFloatv (which, vals);
#ifdef DEBUG
	int err = glGetError();
	ASSERT (err == GL_NO_ERROR);
#endif
	
	printf ("%s:  ", label);
	for (int i = 0 ; i < count ; i++) {
		printf ("%3.2f", vals[i]);
		if (i < count - 1)
			printf (", ");
	}
	printf ("\n");
}

static void dump_ints (const char *label, GLenum which, int count) {
	GLint vals[64];
	ASSERT (count < 64);
	
	glGetIntegerv (which, vals);
#ifdef DEBUG
	int err = glGetError();
	ASSERT (err == GL_NO_ERROR);
#endif
	
	printf ("%s:  ", label);
	for (int i = 0 ; i < count ; i++) {
		printf ("0x%x", vals[i]);
		if (i < count - 1)
			printf (", ");
	}
	printf ("\n");
}

static void dump_bools (const char *label, GLenum which, int count) {
	GLboolean vals[64];
	ASSERT (count < 64);
	
	glGetBooleanv (which, vals);
#ifdef DEBUG
	int err = glGetError();
	ASSERT (err == GL_NO_ERROR);
#endif
	
	printf ("%s:  ", label);
	for (int i = 0 ; i < count ; i++) {
		if (vals[i])
			printf ("true");
		else
			printf ("false");
		if (i < count - 1)
			printf (", ");
	}
	printf ("\n");
}


static void dump_state()
{
	printf ("======================================================================\n");
    glGetError();
	dump_ints ("GL_ACTIVE_TEXTURE", GL_ACTIVE_TEXTURE, 1);
	dump_floats ("GL_ALIASED_LINE_WIDTH_RANGE", GL_ALIASED_LINE_WIDTH_RANGE, 2);
	dump_floats ("GL_ALIASED_POINT_SIZE_RANGE", GL_ALIASED_POINT_SIZE_RANGE, 2);
	dump_ints ("GL_ALPHA_BITS", GL_ALPHA_BITS, 1);
	dump_ints ("GL_ARRAY_BUFFER_BINDING", GL_ARRAY_BUFFER_BINDING, 1);
	dump_ints ("GL_BLEND", GL_BLEND, 1);
	dump_floats ("GL_BLEND_COLOR", GL_BLEND_COLOR, 4);
	dump_ints ("GL_BLEND_DST_ALPHA", GL_BLEND_DST_ALPHA, 1);
	dump_ints ("GL_BLEND_DST_RGB", GL_BLEND_DST_RGB, 1);
	dump_ints ("GL_BLEND_EQUATION_ALPHA", GL_BLEND_EQUATION_ALPHA, 1);
	dump_ints ("GL_BLEND_EQUATION_RGB", GL_BLEND_EQUATION_RGB, 1);
	dump_ints ("GL_BLEND_SRC_ALPHA", GL_BLEND_SRC_ALPHA, 1);
	dump_ints ("GL_BLEND_SRC_RGB", GL_BLEND_SRC_RGB, 1);
	dump_ints ("GL_BLUE_BITS", GL_BLUE_BITS, 1);
	dump_floats ("GL_COLOR_CLEAR_VALUE", GL_COLOR_CLEAR_VALUE, 4);
	dump_bools ("GL_COLOR_WRITEMASK", GL_COLOR_WRITEMASK, 4);
	int num_compressed_formats;
	glGetIntegerv (GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num_compressed_formats);
	dump_ints ("GL_COMPRESSED_TEXTURE_FORMATS", GL_COMPRESSED_TEXTURE_FORMATS, num_compressed_formats);
	dump_bools ("GL_CULL_FACE", GL_CULL_FACE, 1);
	dump_ints ("GL_CULL_FACE_MODE", GL_CULL_FACE_MODE, 1);
	dump_ints ("GL_CURRENT_PROGRAM", GL_CURRENT_PROGRAM, 1);
	dump_ints ("GL_DEPTH_BITS", GL_DEPTH_BITS, 1);
	dump_floats ("GL_DEPTH_CLEAR_VALUE", GL_DEPTH_CLEAR_VALUE, 1);
	dump_ints ("GL_DEPTH_FUNC", GL_DEPTH_FUNC, 1);
	dump_floats ("GL_DEPTH_RANGE", GL_DEPTH_RANGE, 2);
	dump_bools ("GL_DEPTH_TEST", GL_DEPTH_TEST, 1);
	dump_bools ("GL_DEPTH_WRITEMASK", GL_DEPTH_WRITEMASK, 1);
	dump_bools ("GL_DITHER", GL_DITHER, 1);
	dump_ints ("GL_ELEMENT_ARRAY_BUFFER_BINDING", GL_ELEMENT_ARRAY_BUFFER_BINDING, 1);
	dump_ints ("GL_FRAMEBUFFER_BINDING", GL_FRAMEBUFFER_BINDING, 1);
	dump_ints ("GL_FRONT_FACE", GL_FRONT_FACE, 1);
	dump_ints ("GL_GENERATE_MIPMAP_HINT", GL_GENERATE_MIPMAP_HINT, 1);
	dump_ints ("GL_GREEN_BITS", GL_GREEN_BITS, 1);
	//these throw INVALID_FRAMEBUFFER_OPERATION
	//dump_ints ("GL_IMPLEMENTATION_COLOR_READ_FORMAT", GL_IMPLEMENTATION_COLOR_READ_FORMAT, 1);
	//dump_ints ("GL_IMPLEMENTATION_COLOR_READ_TYPE", GL_IMPLEMENTATION_COLOR_READ_TYPE, 1);
	dump_floats ("GL_LINE_WIDTH", GL_LINE_WIDTH, 1);
	dump_ints ("GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, 1);
	dump_ints ("GL_MAX_CUBE_MAP_TEXTURE_SIZE", GL_MAX_CUBE_MAP_TEXTURE_SIZE, 1);
	dump_ints ("GL_MAX_FRAGMENT_UNIFORM_VECTORS", GL_MAX_FRAGMENT_UNIFORM_VECTORS, 1);
	dump_ints ("GL_MAX_RENDERBUFFER_SIZE", GL_MAX_RENDERBUFFER_SIZE, 1);
	dump_ints ("GL_MAX_TEXTURE_IMAGE_UNITS", GL_MAX_TEXTURE_IMAGE_UNITS, 1);
	dump_ints ("GL_MAX_TEXTURE_SIZE", GL_MAX_TEXTURE_SIZE, 1);
	dump_ints ("GL_MAX_VARYING_VECTORS", GL_MAX_VARYING_VECTORS, 1);
	dump_ints ("GL_MAX_VERTEX_ATTRIBS", GL_MAX_VERTEX_ATTRIBS, 1);
	dump_ints ("GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS", GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS, 1);
	dump_ints ("GL_MAX_VERTEX_UNIFORM_VECTORS", GL_MAX_VERTEX_UNIFORM_VECTORS, 1);
	dump_ints ("GL_MAX_VIEWPORT_DIMS", GL_MAX_VIEWPORT_DIMS, 2);
	dump_ints ("GL_NUM_COMPRESSED_TEXTURE_FORMATS", GL_NUM_COMPRESSED_TEXTURE_FORMATS, 1);
	dump_ints ("GL_NUM_SHADER_BINARY_FORMATS", GL_NUM_SHADER_BINARY_FORMATS, 1);
	dump_ints ("GL_PACK_ALIGNMENT", GL_PACK_ALIGNMENT, 1);
	dump_floats ("GL_POLYGON_OFFSET_FACTOR", GL_POLYGON_OFFSET_FACTOR, 1);
	dump_bools ("GL_POLYGON_OFFSET_FILL", GL_POLYGON_OFFSET_FILL, 1);
	dump_ints ("GL_POLYGON_OFFSET_UNITS", GL_POLYGON_OFFSET_UNITS, 1);
	dump_ints ("GL_RED_BITS", GL_RED_BITS, 1);
	dump_ints ("GL_RENDERBUFFER_BINDING", GL_RENDERBUFFER_BINDING, 1);
	dump_bools ("GL_SAMPLE_ALPHA_TO_COVERAGE", GL_SAMPLE_ALPHA_TO_COVERAGE, 1);
	dump_ints ("GL_SAMPLE_BUFFERS", GL_SAMPLE_BUFFERS, 1);
	dump_bools ("GL_SAMPLE_COVERAGE", GL_SAMPLE_COVERAGE, 1);
	dump_bools ("GL_SAMPLE_COVERAGE_INVERT", GL_SAMPLE_COVERAGE_INVERT, 1);
	dump_floats ("GL_SAMPLE_COVERAGE_VALUE", GL_SAMPLE_COVERAGE_VALUE, 1);
	dump_ints ("GL_SAMPLES", GL_SAMPLES, 1);
	dump_ints ("GL_SCISSOR_BOX", GL_SCISSOR_BOX, 4);
	dump_bools ("GL_SCISSOR_TEST", GL_SCISSOR_TEST, 1);
	int num_shader_binary_formats;
	glGetIntegerv (GL_NUM_SHADER_BINARY_FORMATS, &num_shader_binary_formats);
	dump_ints ("GL_SHADER_BINARY_FORMATS", GL_SHADER_BINARY_FORMATS, num_shader_binary_formats);
	dump_bools ("GL_SHADER_COMPILER", GL_SHADER_COMPILER, 1);
	dump_ints ("GL_STENCIL_BACK_FAIL", GL_STENCIL_BACK_FAIL, 1);
	dump_ints ("GL_STENCIL_BACK_FUNC", GL_STENCIL_BACK_FUNC, 1);
	dump_ints ("GL_STENCIL_BACK_PASS_DEPTH_FAIL", GL_STENCIL_BACK_PASS_DEPTH_FAIL, 1);
	dump_ints ("GL_STENCIL_BACK_PASS_DEPTH_PASS", GL_STENCIL_BACK_PASS_DEPTH_PASS, 1);
	dump_ints ("GL_STENCIL_BACK_REF", GL_STENCIL_BACK_REF, 1);
	dump_ints ("GL_STENCIL_BACK_VALUE_MASK", GL_STENCIL_BACK_VALUE_MASK, 1);
	dump_ints ("GL_STENCIL_BACK_WRITEMASK", GL_STENCIL_BACK_WRITEMASK, 1);
	dump_ints ("GL_STENCIL_BITS", GL_STENCIL_BITS, 1);
	dump_ints ("GL_STENCIL_CLEAR_VALUE", GL_STENCIL_CLEAR_VALUE, 1);
	dump_ints ("GL_STENCIL_FAIL", GL_STENCIL_FAIL, 1);
	dump_ints ("GL_STENCIL_FUNC", GL_STENCIL_FUNC, 1);
	dump_ints ("GL_STENCIL_PASS_DEPTH_FAIL", GL_STENCIL_PASS_DEPTH_FAIL, 1);
	dump_ints ("GL_STENCIL_PASS_DEPTH_PASS", GL_STENCIL_PASS_DEPTH_PASS, 1);
	dump_ints ("GL_STENCIL_REF", GL_STENCIL_REF, 1);
	dump_ints ("GL_STENCIL_TEST", GL_STENCIL_TEST, 1);
	dump_ints ("GL_STENCIL_VALUE_MASK", GL_STENCIL_VALUE_MASK, 1);
	dump_ints ("GL_STENCIL_WRITEMASK", GL_STENCIL_WRITEMASK, 1);
	dump_ints ("GL_SUBPIXEL_BITS", GL_SUBPIXEL_BITS, 1);
	dump_ints ("GL_TEXTURE_BINDING_2D", GL_TEXTURE_BINDING_2D, 1);
	dump_ints ("GL_TEXTURE_BINDING_CUBE_MAP", GL_TEXTURE_BINDING_CUBE_MAP, 1);
	dump_ints ("GL_UNPACK_ALIGNMENT", GL_UNPACK_ALIGNMENT, 1);
	dump_ints ("GL_VIEWPORT", GL_VIEWPORT, 4);

#if 0
//a few other bits of info that might in the future be worth dumping
glGetActiveAttrib, glGetActiveUniform, glGetAttachedShaders, glGetAttribLocation, glGetBufferParameteriv, glGetFramebufferAttachmentParameteriv, glGetProgramiv, glGetProgramInfoLog, glGetRenderbufferParameteriv, glGetShaderiv, glGetShaderInfoLog, glGetShaderSource, glGetTexParameter, glGetVertexAttrib, glGetVertexAttribPointerv
#endif


}

static void dump_framebuffers()
{
	dump_ints ("GL_FRAMEBUFFER_BINDING", GL_FRAMEBUFFER_BINDING, 1);
	GLint val;
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &val);
	printf ("color0 name: %d\n", val);	
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &val);
	printf ("color0 type: %d\n", val);
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1_EXT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &val);
	//printf ("color1 name: %d\n", val);
	//glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1_EXT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &val);
	//printf ("color1 type: %d\n", val);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &val);
	printf ("depth name: %d\n", val);
	glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME, &val);
	printf ("stencil name: %d\n", val);
}


