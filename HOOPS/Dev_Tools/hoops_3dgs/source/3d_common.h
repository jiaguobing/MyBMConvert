// Copyright (c) Tech Soft 3D
//
// The information contained herein is confidential and proprietary to Tech Soft 3D, Inc.,
// and considered a trade secret as defined under civil and criminal statutes.
// Tech Soft 3D, Inc. shall pursue its civil and criminal remedies in the event of
// unauthorized use or misappropriation of its trade secrets.  Use of this information
// by anyone other than authorized employees of Tech Soft 3D, Inc. is granted only under
// a written non-disclosure agreement, expressly prescribing the scope and manner of such use.

#ifndef H3DCOMMON_H
#define H3DCOMMON_H

#include "windows_wrapper.h"	// wrapped <windows.h>, must come first :(

#include "hoops.h"
#include "hc_proto.h"
#include "hi_proto.h"
#include "database.h"
#include "driver.h"
#include "phedron.h"
#include "please.h"
#include "select.h"

// #define DEBUG_HW_VIS_SEL

namespace {
#include "h3d_hlsl.h"
}

#ifndef NUTCRACKER
#ifdef WINDOWS_SYSTEM
#include "msw.h"
#endif
#endif

Begin_HOOPS_Namespace

/*****************************************************************************
*****************************************************************************
					Category: General constants
*****************************************************************************
*****************************************************************************/
#define	H3D_Bless_Constant					4096
#define H3D_Display_List_SEGDL_MINIMUM		32
#define H3D_Display_List_GEODL_MINIMUM		32
#define H3D_Display_List_PREFERRED_SIZE		5000			//pointcount
#define H3D_Display_List_MAX_SIZE			0xffff			//pointcount & primitive count
#define H3D_Hardware_Transform_Cutoff		256
#define H3D_Executions_Per_Interrupt_Check	128

// Size of the tiling jitter texture in X and Y. Smaller means faster (more chance that the
// texels will be in the cache) but the tiling is more visible.
#define SHADOW_JITTER_SIZE 64
#define SHADOW_BORDER_TEXELS 8

// Scaling for depth effects Z-range. Used to compress the range into [0, 1]
#define DEPTH_WRITE_RANGE 1000.0f

/*****************************************************************************
*****************************************************************************
					Category: General macros and enums
*****************************************************************************
*****************************************************************************/
#ifdef HOOPS_DEBUG_BUILD
#define DEBUG 1
#define H3D_SHADER_DEBUG
#endif

#include <sys/stat.h>

#ifndef memeq
#  define memeq(a,b,siz) !memcmp(a,b,siz)
#endif

//#define PURE_ABSTRACT
#ifndef PURE_ABSTRACT

void HOOPS_API abstract_error();
#define ABSTRACT_ERROR abstract_error()
#endif

#define UNREFERENCED_FUNCTION(func) SEMI_PROTECT(if (func != null) {})

#define H3DD(dc) ((H3DData *)((dc)->data2))


#define H_SAFE_DELETE(p)			{ if (p) { delete (p);	   (p)=null; } }
#define H_SAFE_RELEASE(p)			{ if (p) { (p)->Release(); (p)=null; } }

INLINE static bool LINE_STYLE_PUNT(Line_Style const & line_style) {
															// no accurate 16-bit representation 
	bool const punt = !ANYBIT(line_style->flags, LSF_SOLID) && (!ANYBIT(line_style->flags, LSF_BITS16|LSF_BITS32) || ANYBIT(line_style->flags, LSF_COMPLEX_ANYBITS));

	return punt;
}

/*
 * Make it explicit when we're using DC or 3D mode - a boolean is easy to misinterpret without context.
 */
enum DrawPrimitiveMode
{
	DP_DC, // DC primitives
	DP_3D, // 3D primitives
};

/*
 * Supported marker glyphs
 */
enum H3DGlyph
{
	H3D_GLYPH_DOT,
	H3D_GLYPH_FILLED_CIRCLE,
	H3D_GLYPH_FILLED_SQUARE,
	H3D_GLYPH_X_MARK,
	H3D_GLYPH_PLUS_MARK,
	H3D_GLYPH_SPHERE,

	// How many glyphs are supported.
	H3D_GLYPH_UNSUPPORTED,

	// For identifying glyphs we can't render
	H3D_GLYPH_COUNT = H3D_GLYPH_UNSUPPORTED
};

#define TEX_USAGE_ENVIRONMENT					1
#define TEX_USAGE_FACE_INDEX_INTERPOLATION		2
#define TEX_USAGE_EDGE_INDEX_INTERPOLATION		3
#define TEX_USAGE_3D_IMAGE						4
#define TEX_USAGE_FACE_PATTERN					5
#define TEX_USAGE_LINE_PATTERN					6
#define TEX_USAGE_LINE_TOOLS					7
#define TEX_USAGE_BUMP							8
#define TEX_USAGE_MARKER						9
#define TEX_USAGE_MARKER_INDEX_INTERPOLATION	10

struct H3DRect {

	long left;
	long top;
	long right;
	long bottom;

	H3DRect() {

	}

	H3DRect(long left, long top, long right, long bottom) : left(left), top(top), right(right), bottom(bottom) {

	}

	bool operator == (H3DRect const & that) const {
		return left == that.left && top == that.top && right == that.right && bottom == that.bottom;
	}

	bool operator != (H3DRect const & that) const {
		return !(*this == that);
	}
};

typedef enum H3D_Driver_Shader_Type_e {
	H3D_Driver_Shader_Type_DX9,
	H3D_Driver_Shader_Type_OGL2,
	H3D_Driver_Shader_Type_DX11,
} H3D_Driver_Shader_Type;

enum H3D_Color_Format
{
	H3D_COLOR_RGBA,
	H3D_COLOR_ARGB,
	H3D_COLOR_ABGR,
};

typedef unsigned int H3D_Color;
#define H3D_PACK_BYTES(a,b,c,d) ((H3D_Color)((((a)&0xff)<<24)|(((b)&0xff)<<16)|(((c)&0xff)<<8)|((d)&0xff)))

INLINE H3D_Color pack_H3D_Color_ARGB(RGBA32 const & c) 
{
	H3D_Color color = H3D_PACK_BYTES(c.a,c.r,c.g,c.b);
	return color;
}

INLINE H3D_Color pack_H3D_Color_ARGB(RGBAS32 const & c) 
{
	H3D_Color color = H3D_PACK_BYTES(c.a,c.r,c.g,c.b);
	return color;
}

INLINE H3D_Color pack_H3D_Color_ABGR(RGBA32 const & c) 
{
	H3D_Color color = H3D_PACK_BYTES(c.a,c.b,c.g,c.r);
	return color;
}

INLINE H3D_Color pack_H3D_Color_ABGR(RGBAS32 const & c) 
{
	H3D_Color color = H3D_PACK_BYTES(c.a,c.b,c.g,c.r);
	return color;
}

INLINE H3D_Color pack_H3D_Color_RGBA(RGBA32 const & c) 
{
	H3D_Color color = H3D_PACK_BYTES(c.r,c.g,c.b,c.a);
	return color;
}

INLINE H3D_Color pack_H3D_Color_RGBA(RGBAS32 const & c) 
{
	H3D_Color color = H3D_PACK_BYTES(c.r,c.g,c.b,c.a);
	return color;
}

INLINE H3D_Color h3d_pack_color(H3D_Color_Format format, RGBA32 const & c)
{
	if (format == H3D_COLOR_ARGB)
		return pack_H3D_Color_ARGB(c);
	else if (format == H3D_COLOR_ABGR)
		return pack_H3D_Color_ABGR(c);
	return pack_H3D_Color_RGBA(c);
}

INLINE H3D_Color h3d_pack_color(H3D_Color_Format format, RGBAS32 const & c)
{
	if (format == H3D_COLOR_ARGB)
		return pack_H3D_Color_ARGB(c);
	else if (format == H3D_COLOR_ABGR)
		return pack_H3D_Color_ABGR(c);
	return pack_H3D_Color_RGBA(c);
}


/*****************************************************************************
*****************************************************************************
					Category: Driver Specific Preprocessor
*****************************************************************************
*****************************************************************************/
#define H3D_SURFACE_ID				H3DRenderTarget *
#define H3D_TEXTURE_ID				H3DRenderTarget *


/*****************************************************************************
*****************************************************************************
						Category: Forward Declarations
*****************************************************************************
*****************************************************************************/
class DL_Bin;
class H3DActions;
class H3DCubeTexture;
class H3DTexture3D;
class H3DTexture;
struct H3DData;
class H3DIndexBufferCache;
class H3DIndexedBufferObject;
class H3DPost;
class H3DRenderTarget;
class H3DShaderID;
class H3DShader;
class H3DShaderHASH;
class H3DVertexBuffer;
class H3DVertexBufferCache;
class H3DGenericVertexBufferCache;
struct Selection_Key_Helper;

struct QuickMovesRegion
{	
	H3DRenderTarget *surface;
	H3DRenderTarget *texture;

	struct QuickMovesRegion * next;
};

/*****************************************************************************
*****************************************************************************
						Category: H3DGPUTimer Class

This class times GPU draw events.  Needs to be subclassed per driver.
*****************************************************************************
*****************************************************************************/
class H3DGPUTimer : public CMO_Pooled
{
public:
	H3DGPUTimer(H3DData* data) :
		m_h3ddata(data),
		m_status(STATUS_SOFTWARE),
		m_first_read(true),
		m_software_start(0),
		m_software_end(0)
	{
	}

	// Clean up
	virtual ~H3DGPUTimer() {}

	// Set the skip flag to ignore the next time
	void Skip()
	{
		m_first_read = true;
	}

	// Read time from last bracket. Ideally call *just before* the bracket to avoid pipeline stalls.
	virtual size_t GetElapsedMilliseconds() = 0;

	// Begin a timing bracket
	virtual void Begin() = 0;
	virtual void End() = 0;

protected:
	H3DData* m_h3ddata;

	enum Status
	{
		STATUS_SOFTWARE,
		STATUS_IDLE,
		STATUS_BEGUN,
		STATUS_ENDED
	};

	Status m_status;

	// Set to skip first time read, which seems to cost more than it should.
	bool m_first_read;

	// Software mode times
	Time m_software_start;
	Time m_software_end;
};


/*****************************************************************************
*****************************************************************************
						Category: H3DVertexFormat Class

This class is a wrapper around a vertex format description.  Needs to be
subclassed per driver.
*****************************************************************************
*****************************************************************************/

#define H3DVF								unsigned int

#define H3DVF_POSITION						0x0001
#define H3DVF_NORMAL						0x0002
#define H3DVF_DIFFUSE						0x0004
#define H3DVF_SPECULAR						0x0008
#define H3DVF_TEXCOUNT_1					0x0010
#define H3DVF_TEXCOUNT_2					0x0020
#define H3DVF_TEXCOUNT_3					0x0030
#define H3DVF_TEXCOUNT_4					0x0040
#define H3DVF_TEXCOUNT_5					0x0050
#define H3DVF_TEXCOUNT_6					0x0060
#define H3DVF_TEXCOUNT_7					0x0070
#define H3DVF_TEXCOUNT_8					0x0080
#define H3DVF_DOUBLE_POSITION				0x0100
#define H3DVF_TEXCOUNT_MASK					0x00f0
#define H3DVF_TEXCOUNT_SHIFT				4
#define H3DVF_TEXCOUNT_LOOKUP(vf)			((vf & H3DVF_TEXCOUNT_MASK) >> H3DVF_TEXCOUNT_SHIFT)

#define H3DVF_TEXCOORD_SIZE(i, size)		((size-1) << (i * 2 + 16))
#define H3DVF_TEXCOORD_SIZE_MASK(i)			(0x03 << (i * 2 + 16))
#define H3DVF_TEXCOORD_SIZE_LOOKUP(vf, i)	(((vf >> (i * 2 + 16)) & (0x3)) + 1)




/* A few compact definitions */
#define H3DVF_PT			(H3DVF_POSITION)
#define H3DVF_PT_CLR		(H3DVF_POSITION | H3DVF_DIFFUSE)
#define H3DVF_PT_TEX		(H3DVF_POSITION | H3DVF_TEXCOUNT_1 | H3DVF_TEXCOORD_SIZE(0,3))
#define H3DVF_PT_NML_TEX	(H3DVF_PT_TEX | H3DVF_NORMAL)
#define H3DVF_PT_NML_TEX2	(H3DVF_PT_NML_TEX | H3DVF_TEXCOUNT_2 | H3DVF_TEXCOORD_SIZE(1,3))


class HOOPS_API H3DVertexFormat : public CMO
{
protected:
	H3DVF m_bits;
	bool m_interleaved;
	mutable unsigned char m_size;
	unsigned char m_stream_count;

	void set_bit(int bit, bool state) {
		if (state)
			m_bits |= bit;
		else
			m_bits &= ~bit;
	};
public:
	H3DVertexFormat(H3DVF h3dvf)
	{
		reset();
		set_vf(h3dvf);
	};

	H3DVertexFormat() { reset(); };
	~H3DVertexFormat(){};

	void reset()
	{
		m_bits = 0;
		m_size = 0;
		m_interleaved = false;
	}

	void set_vf(H3DVF h3dvf)
	{
		m_bits |= h3dvf;
	};

	void set_interleaved(bool state) {m_interleaved = state;};
	void set_position(bool state = true) {set_bit(H3DVF_POSITION, state);};
	void set_double_position(bool state = true) {set_bit(H3DVF_DOUBLE_POSITION, state);};
	void set_normals(bool state = true) {set_bit(H3DVF_NORMAL, state);};
	void set_diffuse(bool state = true) {set_bit(H3DVF_DIFFUSE, state);};
	void set_specular(bool state = true) {set_bit(H3DVF_SPECULAR, state);};
	void set_max_tex_unit(int num) {
		int current_tex_count = get_tex_count();
		if (num+1 > current_tex_count) {
			set_tex_count(num+1);
		}
	};
	void incr_tex_count() {m_bits += H3DVF_TEXCOUNT_1;};
	void set_tex_count(int num) {
		m_bits &= ~H3DVF_TEXCOUNT_MASK;			// clear out existing bits
		m_bits |= (H3DVF_TEXCOUNT_1 * num);		// add in the requested texcount
	};
	void set_tex_size(int num, int size) {m_bits &= ~H3DVF_TEXCOORD_SIZE_MASK(num); m_bits |= H3DVF_TEXCOORD_SIZE(num, size);};

	H3DVF get_vf() const {return m_bits;};
	bool get_interleaved() const {return m_interleaved;};
	bool get_position() const {return ANYBIT(m_bits, H3DVF_POSITION);};
	bool get_double_position() const { return ANYBIT(m_bits, H3DVF_DOUBLE_POSITION); };
	bool get_normals() const {return ANYBIT(m_bits, H3DVF_NORMAL);};
	bool get_diffuse() const {return ANYBIT(m_bits, H3DVF_DIFFUSE);};
	bool get_specular() const {return ANYBIT(m_bits, H3DVF_SPECULAR);};
	bool has_attributes() const {return ANYBIT (m_bits, ~(H3DVF_POSITION|H3DVF_NORMAL));};
	unsigned char get_tex_count() const {return H3DVF_TEXCOUNT_LOOKUP(m_bits);};
	unsigned char get_tex_size(int num) const {return H3DVF_TEXCOORD_SIZE_LOOKUP(m_bits, num);};

	unsigned char get_stream_count()
	{
		if (m_interleaved)
			return 1;

		m_stream_count = 0;
		if (get_position()) m_stream_count++;
		if (get_double_position()) m_stream_count++;
		if (get_normals()) m_stream_count++;
		if (get_diffuse()) m_stream_count++;
		if (get_specular()) m_stream_count++;
		m_stream_count += get_tex_count();
		return m_stream_count;
	}
	
	void get_stream_strides(unsigned int *strides)
	{
		if (m_interleaved) {
			strides[0] = 0;
			if (get_position())
				strides[0] += 3;

			if (get_double_position())
				strides[0] += 6;

			if (get_normals())
				strides[0] += 3;

			if (get_diffuse())
				strides[0] += 1;

			if (get_specular())
				strides[0] += 1;

			for (int i = 0; i < get_tex_count(); i++) {
				strides[0] += get_tex_size(i);
			}
		}
		else {
			int cur_index = 0;
			if (get_position())
				strides[cur_index++] = 3;

			if (get_double_position())
				strides[cur_index++] = 6;

			if (get_normals())
				strides[cur_index++] = 3;

			if (get_diffuse())
				strides[cur_index++] = 1;

			if (get_specular())
				strides[cur_index++] = 1;

			for (int i = 0; i < get_tex_count(); i++) {
				strides[cur_index++] = get_tex_size(i);
			}
		}
	}

	int get_size(bool as_bytes = true) const
	{
		int stride_multiplier = 1;

		if (as_bytes)
			stride_multiplier = sizeof(float);

		m_size = 0;

		if (get_position())				m_size += 3;
		if (get_double_position())		m_size += 6;
		if (get_normals())				m_size += 3;
		if (get_diffuse())				m_size += 1;
		if (get_specular())				m_size += 1;

		for ( int i = 0; i < get_tex_count(); ++i )
			m_size += get_tex_size(i);

		return m_size * stride_multiplier;
	};

	bool operator == (H3DVertexFormat const &right) { 
		return (m_bits == right.m_bits) && (m_interleaved == right.m_interleaved); 
	};
	bool operator != (H3DVertexFormat const &right) { 
		return (m_bits != right.m_bits) || (m_interleaved != right.m_interleaved);
	};
};


/*****************************************************************************
*****************************************************************************
					Category: H3DVertexBuffer Class

This class is a wrapper around a generic float vertex buffer.  It allows a
vertex position, normals, etc. to be set in a more intuitive manner.
*****************************************************************************
*****************************************************************************/
#define H3DVB_COLOR_COUNT		2
#define H3DVB_TEXTURE_COUNT		8
class HOOPS_API H3DVertexBuffer : public CMO
{
private:

	float **pbuf;
	float **pbuf_start;
	int *pbuf_allocated;
	unsigned int *pbuf_strides;

	H3DVertexFormat vf;
	char v_index;
	char n_index;
	char c_index[H3DVB_COLOR_COUNT];
	char t_index[H3DVB_TEXTURE_COUNT];

	char v_offset;
	char n_offset;
	char c_offset[H3DVB_COLOR_COUNT];
	char t_offset[H3DVB_TEXTURE_COUNT];


	bool owns_buffer;
	int buffer_size;
	char stream_count;
	char allocated_stream_count;

public:
	H3DVertexBuffer(float **in_pbuf, H3DVertexFormat in_vf, int vertices_to_allocate = 0)
	{
		ClearMembers();
		Init(in_pbuf, &in_vf, vertices_to_allocate);
	}
	H3DVertexBuffer()
	{
		ClearMembers();
	};

	~H3DVertexBuffer()
	{
		Cleanup();
	};
	void Cleanup()
	{
		if (owns_buffer) {
			for (int i = 0; i < allocated_stream_count; i++) {
				FREE_ARRAY (pbuf_start[i], pbuf_allocated[i], float);
			}
			FREE_ARRAY(pbuf_allocated, allocated_stream_count, int);
		}

		if (pbuf)
			FREE_ARRAY(pbuf, allocated_stream_count, float*);
		if (pbuf_start)
			FREE_ARRAY(pbuf_start, allocated_stream_count, float*);
		if (pbuf_strides)
			FREE_ARRAY(pbuf_strides, allocated_stream_count, unsigned int);

		pbuf = null;
		pbuf_start = null;
		pbuf_strides = null;
		pbuf_allocated = 0;
		allocated_stream_count = 0;
	};

	H3DVertexBuffer(const H3DVertexBuffer &pv)
	{
		*this = pv;

		if (owns_buffer) {
			int lstream_count = vf.get_stream_count();
			ALLOC_ARRAY(pbuf, lstream_count, float*);
			ALLOC_ARRAY(pbuf_start, lstream_count, float*);
			ALLOC_ARRAY(pbuf_allocated, lstream_count, int);
			ALLOC_ARRAY(pbuf_strides, lstream_count,  unsigned int);

			COPY_MEMORY(pv.pbuf_strides, lstream_count * sizeof(char), pbuf_strides);

			for (int i = 0; i < lstream_count; i++) {
				pbuf_allocated[i] = pbuf_strides[i] * buffer_size;
				ALLOC_ARRAY (pbuf_start[i], pbuf_allocated[i], float);
				COPY_MEMORY (pv.pbuf_start[i], 3 * buffer_size * sizeof(float), pbuf_start[i]);
				pbuf[i] = pbuf_start[i];
			}
		}
	}

	INLINE void ClearMembers() {
		v_index = -1;
		n_index = -1;
		SET_MEMORY(c_index, sizeof(c_index), -1);
		SET_MEMORY(t_index, sizeof(t_index), -1);
		pbuf = null;
		pbuf_start = null;
		pbuf_strides = null;
		pbuf_allocated = null;	
		stream_count = 0;
		allocated_stream_count = 0;
		vf.reset();
		owns_buffer = 0;
		buffer_size = 0;
	}

	void Init(float **in_pbuf, H3DVertexFormat *in_vf, int vertices_to_allocate = 0)
	{
		// Ensure we don't leak uvst_offsets or buffer memory.
		ASSERT(pbuf_start == null || !owns_buffer);

		vf = *in_vf;
		owns_buffer = (vertices_to_allocate > 0) ? true : false;

		// cache stream's indices and offsets
		if (vf.get_interleaved()) {
			char offset = 0;

			if (vf.get_double_position()) {
				v_offset = offset;
				offset += 6;
			}
			else {
				v_offset = offset;
				offset += 3;
			}

			if (vf.get_normals()) {
				n_offset = offset;
				offset+=3;
			}
			if (vf.get_diffuse()) {
				c_offset[0] = offset; 
				offset+=1;
			}
			if (vf.get_specular()) {
				c_offset[1] = offset; 
				offset+=1;
			}
			for (int i = 0; i < vf.get_tex_count(); i++) {
				t_offset[i] = offset; 
				offset+=vf.get_tex_size(i);
			}

			v_index = 0;
			if (vf.get_normals())		n_index = 0;
			if (vf.get_diffuse())		c_index[0] = 0;
			if (vf.get_specular())		c_index[1] = 0;
			for (int i = 0; i < vf.get_tex_count(); i++) {
				t_index[i] = 0;
			}
		}
		else {
			v_offset = 0;
			if (vf.get_normals())		n_offset = 0;
			if (vf.get_diffuse())		c_offset[0] = 0;
			if (vf.get_specular())		c_offset[1] = 0;
			for (int i = 0; i < vf.get_tex_count(); i++) {
				t_offset[i] = 0;
			}

			char index = 0;
			v_index = index++;
			if (vf.get_normals())		n_index = index++;
			if (vf.get_diffuse())		c_index[0] = index++;
			if (vf.get_specular())		c_index[1] = index++;
			for (int i = 0; i < vf.get_tex_count(); i++) {
				t_index[i] = index++;
			}
		}

		stream_count = vf.get_stream_count();
		buffer_size = vertices_to_allocate;

		// Resize buffers
		if (stream_count > allocated_stream_count) {
			if (pbuf) 			FREE_ARRAY(pbuf, allocated_stream_count, float*);
			if (pbuf_start) 	FREE_ARRAY(pbuf_start, allocated_stream_count, float*);
			if (pbuf_strides)	FREE_ARRAY(pbuf_strides, allocated_stream_count,  unsigned int);

			allocated_stream_count = stream_count;
			ALLOC_ARRAY(pbuf, allocated_stream_count, float*);
			ALLOC_ARRAY(pbuf_start, allocated_stream_count, float*);
			ALLOC_ARRAY(pbuf_strides, allocated_stream_count, unsigned int);
		}

		// cache stream's strides
		vf.get_stream_strides(pbuf_strides);

		// copy pointers over (or allocate if requested)
		if (owns_buffer) {
			ALLOC_ARRAY(pbuf_allocated, stream_count, int);
			for (int i = 0; i < stream_count; i++) {
				pbuf_allocated[i] = buffer_size*pbuf_strides[i];
				ALLOC_ARRAY(pbuf_start[i], pbuf_allocated[i], float);
			}
		}
		else {
			COPY_MEMORY(in_pbuf, stream_count * sizeof(float*), pbuf_start);
		}

		reset();
	}

	// utility functions
	INLINE void reset() {
		COPY_MEMORY(pbuf_start, stream_count * sizeof(float*), pbuf);
	};
	INLINE void resize(int new_size) {
		int copy_size = Min(buffer_size, new_size) * sizeof(float);

		// Create temporary buffer array
		float **pbuf_new;
		ALLOC_ARRAY(pbuf_new, allocated_stream_count, float*);		
	
		for (int i = 0; i < allocated_stream_count; i++) {
			// Allocate new buffers
			int const & stride = pbuf_strides[i];
			ALLOC_ARRAY(pbuf_new[i], new_size * stride, float);

			// Copy data over
			COPY_MEMORY(pbuf_start[i], copy_size * stride, pbuf_new[i]);

			// Update offset pointers
			pbuf[i] = pbuf_new[i] + (pbuf[i] - pbuf_start[i]);

			// Free old buffer
			FREE_ARRAY(pbuf_start[i], buffer_size * stride, float);

			// Copy new buffer pointer over
			pbuf_start[i] = pbuf_new[i];
		}

		// Free temporary buffer array
		FREE_ARRAY(pbuf_new, allocated_stream_count, float*);
		buffer_size = new_size;
	}

	INLINE void copy_forward_n_times(int n_times) {
		for (int i = 0; i < stream_count; i++) {
			int const & stride = pbuf_strides[i];
			size_t const bytes_to_copy = stride*sizeof(float);
			for (int j = 1; j <= n_times; j++) {
				memcpy(pbuf[i] + j*stride, pbuf[i], bytes_to_copy);
			}
		}
	}

	INLINE void copy_to(int dest_index, int num_points = 1) {
		for (int i = 0; i < stream_count; i++) {
			int const & stride = pbuf_strides[i];
			COPY_MEMORY(pbuf[i], num_points*stride*sizeof(float), pbuf[i]+dest_index*stride);
		}
	}

	INLINE void copy_to(H3DVertexBuffer *dest, int num_points = 1) const {
		for (int i = 0; i < stream_count; i++) {
			int const & stride = pbuf_strides[i];
			COPY_MEMORY(pbuf[i], num_points*stride*sizeof(float), dest->pbuf[i]);
		}
	}

	INLINE void copy_range_to(H3DVertexBuffer *dest, unsigned int start_index, int num_points = 1) const {
		for (int i = 0; i < stream_count; i++) {
			unsigned int const & stride = pbuf_strides[i];
			float const * src = &pbuf[i][start_index*stride];
			COPY_MEMORY(src, num_points*stride*sizeof(float), dest->pbuf[i]);
		}
	}

	INLINE void copy_from_start_to(H3DVertexBuffer *dest, int num_points = 1) const {
		for (int i = 0; i < stream_count; i++) {
			int const & stride = pbuf_strides[i];
			COPY_MEMORY(pbuf_start[i], num_points*stride*sizeof(float), dest->pbuf_start[i]);
		}
	}

	INLINE void *get_vb_ptr() {return pbuf[v_index];}
	INLINE void *get_nb_ptr() { ASSERT(!vf.get_interleaved()); return pbuf[n_index]; }
	INLINE void *get_cb_ptr(int index) { ASSERT(!vf.get_interleaved()); return pbuf[c_index[index]]; }
	INLINE void *get_tb_ptr(int index) { ASSERT(!vf.get_interleaved()); return pbuf[t_index[index]]; }

	INLINE int get_buffer_size() {return buffer_size;};
	INLINE int get_points_stuffed() {return (int)(pbuf[0] - pbuf_start[0]) / pbuf_strides[0];};
	INLINE H3DVertexFormat & get_vf() {return vf;};
	INLINE int get_stride(int i) {return pbuf_strides[i];};
	INLINE float *get_index_ptr(int stream, int i) {return pbuf_start[stream] + i * pbuf_strides[stream];};

	// get() functions
	INLINE float get_x() {return pbuf[v_index][v_offset+0];}
	INLINE float get_y() {return pbuf[v_index][v_offset+1];}
	INLINE float get_z() {return pbuf[v_index][v_offset+2];}

	INLINE float get_nx() {return pbuf[n_index][n_offset+0];}
	INLINE float get_ny() {return pbuf[n_index][n_offset+1];}
	INLINE float get_nz() {return pbuf[n_index][n_offset+2];}

	INLINE float get_u(int texunit = 0) {
		if (texunit < vf.get_tex_count())
			return pbuf[t_index[texunit]][t_offset[texunit]+0];
		return -1;
	}
	INLINE float get_v(int texunit = 0) {
		if (texunit < vf.get_tex_count())
			return pbuf[t_index[texunit]][t_offset[texunit]+1];
		return -1;
	}
	INLINE float get_s(int texunit = 0) {
		if (texunit < vf.get_tex_count())
			return pbuf[t_index[texunit]][t_offset[texunit]+2];
		return -1;
	}
	INLINE float get_t(int texunit = 0) {
		if (texunit < vf.get_tex_count())
			return pbuf[t_index[texunit]][t_offset[texunit]+3];
		return -1;
	}

	// set() functions
	INLINE void xyz(Point const & p) {
		memcpy(&pbuf[v_index][v_offset], &p, sizeof(float)*3);
	}

	INLINE void xyz(DPoint const & p) {
		memcpy(&pbuf[v_index][v_offset], &p, sizeof(double)* 3);
	}

	INLINE void xyz(float ix, float iy, float iz) {
		memcpy(&pbuf[v_index][v_offset+0], &ix, sizeof(float));
		memcpy(&pbuf[v_index][v_offset+1], &iy, sizeof(float));
		memcpy(&pbuf[v_index][v_offset+2], &iz, sizeof(float));
	}
	INLINE void x(float ix) {
		memcpy(&pbuf[v_index][v_offset+0], &ix, sizeof(float));
	}
	INLINE void y(float iy) {
		memcpy(&pbuf[v_index][v_offset+1], &iy, sizeof(float));
	}
	INLINE void z(float iz) {
		memcpy(&pbuf[v_index][v_offset+2], &iz, sizeof(float));
	}

	INLINE void nxyz(Vector const & v) {
		memcpy(&pbuf[n_index][n_offset], &v, sizeof(float)*3);
	};
	INLINE void nxyz(float inx, float iny, float inz) {
		memcpy(&pbuf[n_index][n_offset+0], &inx, sizeof(float));
		memcpy(&pbuf[n_index][n_offset+1], &iny, sizeof(float));
		memcpy(&pbuf[n_index][n_offset+2], &inz, sizeof(float));
	}
	INLINE void nx(float inx) {
		memcpy(&pbuf[n_index][n_offset+0], &inx, sizeof(float));
	}
	INLINE void ny(float iny) {
		memcpy(&pbuf[n_index][n_offset+1], &iny, sizeof(float));
	}
	INLINE void nz(float inz) {
		memcpy(&pbuf[n_index][n_offset+2], &inz, sizeof(float));
	}

	INLINE void color(H3D_Color const in_color) {
		COPY_MEMORY(&in_color, sizeof(H3D_Color), &pbuf[c_index[0]][c_offset[0]]);
	}

	// Selections should already be packed as colour
	INLINE void selection(H3D_Color const key) {
		COPY_MEMORY(&key, sizeof(H3D_Color), &pbuf[c_index[1]][c_offset[1]]);
	}

	INLINE void uvst(Plane const *uvst, int texunit = 0) {u(uvst->a,texunit); v(uvst->b,texunit); s(uvst->c,texunit); t(uvst->d,texunit);}
	INLINE void uvst(float iu, float iv, float is, float it, int texunit = 0) {u(iu,texunit); v(iv,texunit); s(is,texunit); t(it,texunit);}
	INLINE void uvs(float iu, float iv, float is, int texunit = 0) {u(iu,texunit); v(iv,texunit); s(is,texunit);}
	INLINE void uvs(Point const & iuvs, int texunit = 0) {u(iuvs.x,texunit); v(iuvs.y,texunit); s(iuvs.z,texunit);}
	INLINE void uvs(Vector const & iuvs, int texunit = 0) {u(iuvs.x,texunit); v(iuvs.y,texunit); s(iuvs.z,texunit);}
	INLINE void uv(float iu, float iv, int texunit = 0) {u(iu,texunit); v(iv,texunit); }
	INLINE void u(float iu, int texunit = 0) {
		if (texunit >= 0 && texunit < vf.get_tex_count() && vf.get_tex_size(texunit) > 0)
			pbuf[t_index[texunit]][t_offset[texunit]+0] = iu;
	}
	INLINE void v(float iv, int texunit = 0) {
		if (texunit >= 0 && texunit < vf.get_tex_count() && vf.get_tex_size(texunit) > 1)
			pbuf[t_index[texunit]][t_offset[texunit]+1] = iv;
	}
	INLINE void s(float is, int texunit = 0) {
		if (texunit >= 0 && texunit < vf.get_tex_count() && vf.get_tex_size(texunit) > 2)
			pbuf[t_index[texunit]][t_offset[texunit]+2] = is;
	}
	INLINE void t(float it, int texunit = 0) {
		if (texunit >= 0 && texunit < vf.get_tex_count() && vf.get_tex_size(texunit) > 3)
			pbuf[t_index[texunit]][t_offset[texunit]+3] = it;
	}

	INLINE void uv_n_times(float const * uv, size_t n, int texunit = 0) {
		if(texunit >= 0 && texunit < vf.get_tex_count() && vf.get_tex_size(texunit) > 1) {
			float * p = &pbuf[t_index[texunit]][t_offset[texunit]];
			memcpy(p, uv, sizeof(float) * 2 * n);
		}
	}

	INLINE H3DVertexBuffer const & incr(int const steps = 1) {
		//ASSERT(this != &m_h3ddata->pv_shared_indexed);	//pv_shared_indexed must not be incremented!!

		for (int i = 0; i < stream_count; i++) {
			pbuf[i] += steps * pbuf_strides[i];
		}
		return *this;
	}

	INLINE int const length() const
	{
		return (int)(pbuf[0] - pbuf_start[0]) / pbuf_strides[0];
	}
};

/*****************************************************************************
*****************************************************************************
						Category: Driver structures
*****************************************************************************
*****************************************************************************/
#define H3D_SOFTWARE_XFORM_BIN_SIZE			256				//max size to allow for bin IM software xforms

enum H3D_Display_List_Type {
	H3D_DL_NONE = 0,
	H3D_DL_TEXTURE = 1,
	H3D_DL_SHADOW_MAP = 2,
	H3D_DL_GEOMETRY = 3,
	H3D_DL_SEGMENT_POLYLINE = 4,
	H3D_DL_SEGMENT_POLYGON = 5,
	H3D_DL_SEGMENT_TREE = 6,
	H3D_DL_SELECTION_MAP = 7,
	H3D_DL_HIGHLIGHT_MAP = 8,
};


typedef std::vector<H3DIndexedBufferObject *, POOL_Allocator<H3DIndexedBufferObject *> > H3DIndexedBufferObject_List;
typedef std::vector<DL_Bin *, POOL_Allocator<DL_Bin *> > DL_Bin_Vector;

typedef VHash<int, DL_Bin_Vector *> BinHash;

template <typename T>
bool HOOPS_API get_display_list (
	Display_Context const *	dc,
	Display_List **			linker,
	Display_List *&			dl,
	T *&					odl,
	unsigned int			type);

template <typename T>
INLINE bool get_display_list(
	Net_Rendition const &	nr,
	Display_List **			linker,
	Display_List *&			dl,
	T *&					odl,
	unsigned int			type)
{
	return get_display_list(nr->display_context, linker, dl, odl, type);
}

class Texture_H3DDL : public Driver_DL {
public:

	H3DTexture *	id;
	H3DTexture *	env_ids;

	int				vram_usage;	// approximate amount of memory this object consumes
	bool			default_pool;
};

class Geometry_H3DDL : public Driver_DL {
public:
	
	Destructible<H3DIndexedBufferObject_List>::unique_ptr	ibolist;

	SimpleSphere					total_bounding;
	SimpleSphere *					boundings;
	int								bounding_count;

	int count_ibo_geometry() const;
	mutable int						ibo_geometry_count;
	
	H3DVertexFormat					vf;

	unsigned int					rendo_flags;
	Strip_Flags						special_flags;
	Strip_Flags2					special_flags2;
	unsigned int					attributes_flags;
	int								geometry_bits;
	float							hard_edge_dihedral;
	int32_t							hash_key;
	int32_t							index; //for GPU selection
	int								line_style_id;

	struct {
		int total;
		int strips;
		int point_count;
	}	stats;

	Param_Source					tex_param_source;
	Param_Source					tex_param_source_back;
};

typedef std::vector<Geometry_H3DDL *, CMO_Allocator<Geometry_H3DDL *> > Geometry_H3DDL_Vector;

class Shadow_Map_H3DDL : public Driver_DL {
public:

	H3DTexture *	id;
	int				resolution;
	float			transform[16];
};

class Segment_Polyline_H3DDL : public Driver_DL {
public:

	Destructible<Driver_DL_List>::unique_ptr	pedl_list; // Poly Edge DLs

	int					budget; 
	int					continued_budget;
	float				max_deviation;
	float				max_angle;
	float				max_length;
	struct {
		int total;
		int strips;
		int point_count;
	}	stats;

	bool				contains_curves;
	bool				view_dependent;
	int					line_style_id;
};


class Segment_Polygon_H3DDL : public Driver_DL {
public:

	Destructible<Driver_DL_List>::unique_ptr	tsdl_list; // Triangle Strip DLs
	Destructible<Driver_DL_List>::unique_ptr	pedl_list; // Poly Edge DLs
	Destructible<Driver_DL_List>::unique_ptr	fedl_list; // Fake Ellipse DLs

	int					budget;
	int					continued_budget;
	float				max_deviation;
	float				max_angle;
	float				max_length;

	bool				contains_ellipses;
	bool				view_dependent;
};

// DISPLAY LIST CLEANUP TASK
bool HOOPS_API free_some_display_lists(H3DData *h3ddata, size_t bytes);

void HOOPS_API release_display_list(Driver_DL *bddl);

void HOOPS_API flush_display_lists(Display_Context const * dc);

void HOOPS_API abort_display_lists(H3DData *h3ddata);

class Segment_H3DDL : public Driver_DL {
public:
	enum SDL_Flags : unsigned char {
		NONE					= 0x00,
		Blessed_Geometry		= 0x01,
		Blessed_Segment			= 0x02,
		Has_Explicit_Visible	= 0x04,
		Local_IBOs				= 0x10,
	};

	Destructible<Geometry_H3DDL_Vector>::unique_ptr	tristrips;
	Destructible<Geometry_H3DDL_Vector>::unique_ptr	vertices;
	Destructible<Geometry_H3DDL_Vector>::unique_ptr	markers;

	Destructible<Driver_DL_List>::unique_ptr	polyedges_list;
	Destructible<Driver_DL_List>::unique_ptr	polycylinder_axes_list;	// polyedges
	Destructible<Driver_DL_List>::unique_ptr	isolines_list;			// polyedges
	Destructible<Driver_DL_List>::unique_ptr	polylines_list;
	Destructible<Driver_DL_List>::unique_ptr	polygons_list;

	Type_Flags			visibility;
	Type_Flags			explicit_visibility;  //elements that snuck in by virtue of local visibilities
	Rendo_Flags			interpolation_options;

	SDL_Flags			sdlflags;

	void AppendTristrips(Geometry_H3DDL *godl) {
		if (tristrips == null)
			tristrips = Construct<Geometry_H3DDL_Vector>();
		ASSERT(tristrips->size() < 0x7fff);
		if (ANYBIT(sdlflags, Local_IBOs))
			godl->special_flags2 |= DL2_LOCAL_IBOS;
		tristrips->push_back(godl);
	}
	void AppendVertices(Geometry_H3DDL *godl) {
		if (vertices == null)
			vertices = Construct<Geometry_H3DDL_Vector>();
		ASSERT(vertices->size() < 0x7fff);
		if (ANYBIT(sdlflags, Local_IBOs))
			godl->special_flags2 |= DL2_LOCAL_IBOS;

		vertices->push_back(godl);
	}
	void AppendMarkers(Geometry_H3DDL *godl) {
		if (markers == null)
			markers = Construct<Geometry_H3DDL_Vector>();
		ASSERT(markers->size() < 0x7fff);
		if (ANYBIT(sdlflags, Local_IBOs))
			godl->special_flags2 |= DL2_LOCAL_IBOS;

		markers->push_back(godl);
	}

	void ReleaseTristrips() {
		if (tristrips != null) {
			for (auto & x : *tristrips)
				release_display_list(x);
			tristrips = null;
		}
	}
	void ReleaseVertices() {
		if (vertices != null) {
			for (auto & x : *vertices)
				release_display_list(x);
			vertices = null;
		}
	}
	void ReleaseMarkers() {
		if (markers != null) {
			for (auto & x : *markers)
				release_display_list(x);
			markers = null;
		}
	}

	void Execute (Net_Rendition const &nr, bool arrays_only);
	void ConvertToLocal();

private:
	void ExecuteTristrips(Net_Rendition const &nr, bool draw_implicit);
	void ExecuteVertices(Net_Rendition const &nr, bool draw_implicit);
	void ExecuteMarkers(Net_Rendition const &nr, bool draw_implicit);

};


template<> struct _bit_ops_type < Segment_H3DDL::SDL_Flags > {
	typedef Segment_H3DDL::SDL_Flags type;
};


class Selection_Map_H3DDL : public Driver_DL {
public:

	H3DRenderTarget *	hi;
	H3DRenderTarget *	lo;
	int					width;
	int					height;
};


class Highlight_Map_H3DDL : public Driver_DL {
public:

	H3DTexture *	bitmap;
	H3DTexture *	table;
};


enum H3D_Vertex_Buffer_Style
{
	H3D_Invalid = 0,
	H3D_Shared_Vertices = 1,			// vertices shared between strips
	H3D_Partially_Shared_Vertices = 2,	// vertices shared within, but not between, strips.  e.g. flat lighting, but we're able to turn off interpolation
	H3D_Unshared_Vertices = 3			// vertices not shared.  necessary when there are conflicting requirements for shade model, e.g. flat-lit but textured
};


RGB const *color_normal_to_rgb (Color_Normal const *color, int mask);

INLINE RGB const * COLOR2RGB(Color const * clr, Color_Object bit) {

	if (clr != null && ANYBIT(clr->objects, bit))
		return color_normal_to_rgb((Color_Normal const *)clr->colors,bit);

	return null;
}


static INLINE void tint_color(
	Net_Rendition const &	nr,
	RGB const *&			gref_color,
	RGB &					tinted_rgb)
{
	if (gref_color != null) {
		Tinting const &		tint = nr->geometry_rendition->diffuse_tint;
		if (ANYBIT(tint.options, Tint_ACTIVE)) {
			tinted_rgb = HI_Apply_Color_Effects(*gref_color, tint);
			gref_color = &tinted_rgb;
		}
	}
}
static INLINE void item_mm(
	Deferred_Item const &	item,
	Matrix &				gref_matrix)
{
	if (item.mm)
		gref_matrix = item.mm->matrix;
	else
		gref_matrix = null;
}
static INLINE void item_color_and_mm(
	Net_Rendition const &	nr,
	Deferred_Item const &	item,
	Color_Object			bit,
	Matrix &				gref_matrix,
	RGB const *&			gref_color,
	RGB &					tinted_rgb)
{
	item_mm(item, gref_matrix);
	gref_color = COLOR2RGB(item.color, bit);
	tint_color(nr, gref_color, tinted_rgb);
}

static INLINE float MAKE_VERTEX_PRIORITY(int32_t priority, bool displacement) {

	static const float PRIORITY_HSRA_FACE_DISPLACEMENT = 0.5f;

	float value = 0.5f + 12.5e-8f * ((displacement ? PRIORITY_HSRA_FACE_DISPLACEMENT : 0.0f) - static_cast<float>(priority));

	return value;
}


struct DL_Collector : public CMO_Pooled
{
	DL_Collector(Memory_Pool * memory_pool) : memory_pool(memory_pool),
		tristrips(memory_pool), polyedges(memory_pool), polycylinder_axes(memory_pool), isolines(memory_pool),
		polylines(memory_pool), polygons(memory_pool), polymarkers(memory_pool), markers(memory_pool) {}

	~DL_Collector() {
		;
	}

	Memory_Pool *			memory_pool;
	DL_Collector *			next;
	Net_Rendition			nr;

	Deferred_Item_Vector	tristrips;
	Deferred_Item_Vector	polyedges;
	Deferred_Item_Vector	polycylinder_axes;
	Deferred_Item_Vector	isolines;
	Deferred_Item_Vector	polylines;			//lines, curves and arcs
	Deferred_Item_Vector	polygons;			//polygons, circles, ellipses
	Deferred_Item_Vector	polymarkers;
	Deferred_Item_Vector	markers;

	Action_Mask				mask;
	Action_Mask				suppress;
	Action_Mask				original_mask;  /* Action_Mask_WHATEVER bits, where 1==handled, 0==not handled */

	bool					punted_gref;
	bool					skip_collection;
	bool					gref_attributes;
	bool					compile_only;
};

struct RenderTargets
{
	enum { MAX_TARGETS = 4 };
	H3DRenderTarget *	targets[MAX_TARGETS];
	H3DRenderTarget *	depth_stencil;

	RenderTargets() : depth_stencil(0)
	{
		ZERO_ARRAY(targets, countof(targets), H3DRenderTarget*);
	}
};

/*****************************************************************************
*****************************************************************************
						Category: H3DData Class
*****************************************************************************
*****************************************************************************/
#define MAX_TEXTURE_UNIT					7				//highest texture unit to use
#define H3D_MAX_TEXTURES					8
#define H3D_MAX_SHADOW_MAPS					4
#define H3D_MAX_CUTTING_PLANES				15


typedef int DC_Type;
#define DCT_UNDEFINED		((DC_Type)0)
#define DCT_OUR_WINDOW		((DC_Type)1)
#define DCT_THEIR_WINDOW	((DC_Type)2)
#define DCT_IMAGE			((DC_Type)3)

/* All the different enumerations that can go into ensure functions */
typedef enum _H3DBLEND {
	H3DBLEND_ZERO               = 1,
	H3DBLEND_ONE                = 2,
	H3DBLEND_SRCCOLOR           = 3,
	H3DBLEND_INVSRCCOLOR        = 4,
	H3DBLEND_SRCALPHA           = 5,
	H3DBLEND_INVSRCALPHA        = 6,
	H3DBLEND_DESTALPHA          = 7,
	H3DBLEND_INVDESTALPHA       = 8,
	H3DBLEND_DESTCOLOR          = 9,
	H3DBLEND_INVDESTCOLOR       = 10,
	H3DBLEND_SRCALPHASAT        = 11,
	H3DBLEND_BOTHSRCALPHA       = 12,
	H3DBLEND_BOTHINVSRCALPHA    = 13,
	H3DBLEND_BLENDFACTOR        = 14, /* Only supported if H3DPBLENDCAPS_BLENDFACTOR is on */
	H3DBLEND_INVBLENDFACTOR     = 15, /* Only supported if H3DPBLENDCAPS_BLENDFACTOR is on */
	H3DBLEND_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
} H3DBLEND;

typedef enum _H3DCLEAR {
	H3DCLEAR_TARGET				= 1,
	H3DCLEAR_ZBUFFER			= 2,
	H3DCLEAR_STENCIL			= 3,
} H3DCLEAR;

typedef enum _H3DCMPFUNC {
	H3DCMP_NEVER                = 1,
	H3DCMP_LESS                 = 2,
	H3DCMP_EQUAL                = 3,
	H3DCMP_LESSEQUAL            = 4,
	H3DCMP_GREATER              = 5,
	H3DCMP_NOTEQUAL             = 6,
	H3DCMP_GREATEREQUAL         = 7,
	H3DCMP_ALWAYS               = 8,
	H3DCMP_FORCE_DWORD          = 0x7fffffff, /* force 32-bit size enum */
} H3DCMPFUNC;

typedef enum _H3DCULL {
	H3DCULL_NONE                = 1,
	H3DCULL_CW                  = 2,
	H3DCULL_CCW                 = 3,
	H3DCULL_FORCE_DWORD         = 0x7fffffff, /* force 32-bit size enum */
} H3DCULL;

typedef enum _H3DPRIMITIVETYPE {
	H3DPT_POINTLIST             = 1,
	H3DPT_LINELIST              = 2,
	H3DPT_LINESTRIP             = 3,
	H3DPT_TRIANGLELIST          = 4,
	H3DPT_TRIANGLESTRIP         = 5,
	H3DPT_TRIANGLEFAN           = 6,
	H3DPT_LINELIST_ADJ			= 7,
	H3DPT_LINESTRIP_ADJ			= 8,
	H3DPT_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
} H3DPRIMITIVETYPE;

typedef enum _H3DSHADEMODE {
	H3DSHADE_FLAT               = 1,
	H3DSHADE_GOURAUD            = 2,
	H3DSHADE_PHONG              = 3,
	H3DSHADE_FORCE_DWORD        = 0x7fffffff, /* force 32-bit size enum */
} H3DSHADEMODE;

typedef enum _H3DSTENCILOP {
	H3DSTENCILOP_KEEP           = 1,
	H3DSTENCILOP_ZERO           = 2,
	H3DSTENCILOP_REPLACE        = 3,
	H3DSTENCILOP_INCRSAT        = 4,
	H3DSTENCILOP_DECRSAT        = 5,
	H3DSTENCILOP_INVERT         = 6,
	H3DSTENCILOP_INCR           = 7,
	H3DSTENCILOP_DECR           = 8,
	H3DSTENCILOP_FORCE_DWORD    = 0x7fffffff, /* force 32-bit size enum */
} H3DSTENCILOP;

typedef enum _H3DTEXTUREADDRESS {
	H3DTADDRESS_WRAP            = 1,
	H3DTADDRESS_MIRROR          = 2,
	H3DTADDRESS_CLAMP           = 3,
	H3DTADDRESS_BORDER          = 4,
	H3DTADDRESS_MIRRORONCE      = 5,
	H3DTADDRESS_FORCE_DWORD     = 0x7fffffff, /* force 32-bit size enum */
} H3DTEXTUREADDRESS;

// TODO: do this for interpolation filter enums too
INLINE static H3DTEXTUREADDRESS Tiling_Style_To_H3DTEXTUREADDRESS(Tiling_Style tiling_style) {

	switch(tiling_style) {
		case HK_TS_REPEAT: return H3DTADDRESS_WRAP;
		case HK_TS_CLAMP: return H3DTADDRESS_CLAMP;
		case HK_TS_MIRROR: return H3DTADDRESS_MIRROR;
		default: return H3DTADDRESS_WRAP;
	}
}

typedef enum _H3DTEXTUREFILTERTYPE
{
	H3DTEXF_NONE				= 0,    // filtering disabled (valid for mip filter only)
	H3DTEXF_POINT				= 1,    // nearest
	H3DTEXF_LINEAR				= 2,    // linear interpolation
	H3DTEXF_ANISOTROPIC			= 3,    // anisotropic
	H3DTEXF_PYRAMIDALQUAD		= 6,    // 4-sample tent
	H3DTEXF_GAUSSIANQUAD		= 7,    // 4-sample gaussian
	H3DTEXF_FORCE_DWORD			= 0x7fffffff,   // force 32-bit size enum
} H3DTEXTUREFILTERTYPE;

typedef enum _H3DFORMAT
{
	H3DFMT_UNKNOWN              =  0,

	H3DFMT_R8G8B8               = 20,
	H3DFMT_A8R8G8B8             = 21,
	H3DFMT_X8R8G8B8             = 22,
	H3DFMT_R5G6B5               = 23,
	H3DFMT_X1R5G5B5             = 24,
	H3DFMT_A1R5G5B5             = 25,
	H3DFMT_A4R4G4B4             = 26,
	H3DFMT_R3G3B2               = 27,
	H3DFMT_A8                   = 28,
	H3DFMT_A8R3G3B2             = 29,
	H3DFMT_X4R4G4B4             = 30,
	H3DFMT_A2B10G10R10          = 31,
	H3DFMT_A8B8G8R8             = 32,
	H3DFMT_X8B8G8R8             = 33,
	H3DFMT_G16R16               = 34,
	H3DFMT_A2R10G10B10          = 35,
	H3DFMT_A16B16G16R16         = 36,

	H3DFMT_A8P8                 = 40,
	H3DFMT_P8                   = 41,
	H3DFMT_L8                   = 50,
	H3DFMT_A8L8                 = 51,
	H3DFMT_A4L4                 = 52,

	H3DFMT_V8U8                 = 60,
	H3DFMT_L6V5U5               = 61,
	H3DFMT_X8L8V8U8             = 62,
	H3DFMT_Q8W8V8U8             = 63,
	H3DFMT_V16U16               = 64,
	H3DFMT_A2W10V10U10          = 67,

#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0, ch1, ch2, ch3)                              \
		((unsigned long)(unsigned char)(ch0) | ((unsigned long)(unsigned char)(ch1) << 8) |       \
		((unsigned long)(unsigned char)(ch2) << 16) | ((unsigned long)(unsigned char)(ch3) << 24 ))
#endif /* defined(MAKEFOURCC) */
	H3DFMT_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
	H3DFMT_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
	H3DFMT_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
	H3DFMT_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
	H3DFMT_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),

	H3DFMT_D16_LOCKABLE         = 70,
	H3DFMT_D32                  = 71,
	H3DFMT_D15S1                = 73,
	H3DFMT_D24S8                = 75,
	H3DFMT_D24X8                = 77,
	H3DFMT_D24X4S4              = 79,
	H3DFMT_D16                  = 80,

	H3DFMT_D32F_LOCKABLE        = 82,
	H3DFMT_D24FS8               = 83,
	H3DFMT_L16                  = 81,

	H3DFMT_VERTEXDATA           =100,
	H3DFMT_INDEX16              =101,
	H3DFMT_INDEX32              =102,

	H3DFMT_Q16W16V16U16         =110,

	// s10e5 formats (16-bits per channel)
	H3DFMT_R16F                 = 111,
	H3DFMT_G16R16F              = 112,
	H3DFMT_A16B16G16R16F        = 113,

	// IEEE s23e8 formats (32-bits per channel)
	H3DFMT_R32F                 = 114,
	H3DFMT_G32R32F              = 115,
	H3DFMT_A32B32G32R32F        = 116,

	H3DFMT_CxV8U8               = 117,

	// Integer formats
	H3DFMT_R32_UINT				= 118,

	H3DFMT_FORCE_DWORD          =0x7fffffff
} H3DFORMAT;

//translate from upper level version of image format enum to the H3D version
INLINE H3DFORMAT h3dimageformat(Image_Format fmt)
{
	switch (fmt) {
		case Image_LUMINANCE8: 
			return H3DFMT_L8;
		case Image_RGB24: 
			return H3DFMT_R8G8B8;
		default:
			return H3DFMT_A8R8G8B8;
	}
}


enum H3DFACE
{
   H3DFACE_POSITIVE_X 			= 0,
   H3DFACE_NEGATIVE_X 			= 1,
   H3DFACE_POSITIVE_Y 			= 2,
   H3DFACE_NEGATIVE_Y 			= 3,
   H3DFACE_POSITIVE_Z 			= 4,
   H3DFACE_NEGATIVE_Z 			= 5,

   H3DFACE_COUNT				= 6,
};

#define	H3DTEXUSAGE_DEFAULT			0x0000
#define	H3DTEXUSAGE_RENDERTARGET	0x0001
#define	H3DTEXUSAGE_DEPTHSTENCIL	0x0002
#define	H3DTEXUSAGE_AUTOGENMIPMAP	0x0004
#define H3DTEXUSAGE_COMPUTE			0x0008
#define H3DTEXUSAGE_DYNAMIC			0x0010



/*
 * Options for clearing
 */
#define H3DCLEAR_TARGET				0x0001  /* Clear target surface */
#define H3DCLEAR_ZBUFFER			0x0002  /* Clear target z buffer */
#define H3DCLEAR_STENCIL			0x0004  /* Clear stencil planes */

/*
 * Color mask bits
 */
enum H3DMASK
{
	// One bit per channel
	H3DMASK_R = (1<<0),
	H3DMASK_G = (1<<1),
	H3DMASK_B = (1<<2),
	H3DMASK_A = (1<<3),

	// Shorthand combinations
	H3DMASK_RGB = H3DMASK_R|H3DMASK_G|H3DMASK_B,
	H3DMASK_RGBA = H3DMASK_RGB|H3DMASK_A,
};


struct H3DViewport {
	unsigned long       X;
	unsigned long       Y;				/* Viewport Top left */
	unsigned long       Width;
	unsigned long       Height;			/* Viewport Dimensions */
	float				MinZ;			/* Min/max of clip Volume */
	float				MaxZ;
};


class H3DTexture;
typedef VHash<intptr_t, H3DTexture *> H3DTexture_Hash;

struct Glyph_Texture
{
	H3DTexture *	texture;
	Glyph_Texture *	next;
	uint32_t		size;
	int32_t			glyph_key;
};

class H3DGPUTimer;

enum AntialiasMode
{
	UNKNOWN_ANTIALIASING,
	NO_ANTIALIASING,
	MULTISAMPLE_ANTIALIASING,
	SUPERSAMPLE_ANTIALIASING,
};

struct ReflectionPlaneConstants
{
	Plane	plane;
	float	scale;
	float	bias;
};

struct PriorityInclude {	
	H3DTexture *		include_target;
	H3DRenderTarget *	include_depth;
	RenderTargets		saved_targets;
	int32_t				priority;
};

#include "3d_common_shader.h"

/* shadow of device state */
struct H3DCache
{
	H3DViewport viewport;
	void *current_buffer_object;
	float z_range[2];
	bool zbuffering;
	bool stencilling;
	bool alphatesting;
	int alpha_ref;
	H3DCMPFUNC alpha_func;
	H3DCMPFUNC stencil_func;
	unsigned long stencil_ref;
	unsigned long stencil_mask;
	unsigned long stencil_write_mask;
	H3DSTENCILOP stencil_fail;
	H3DSTENCILOP stencil_zfail;
	H3DSTENCILOP stencil_pass;
	bool scissoring;
	bool transparency_on;
	H3DBLEND src_blend;
	H3DBLEND dest_blend;
	bool color_mask;
	bool color_mask_locked;
	H3DMASK channel_mask;
	bool depth_mask;
	bool depth_test_reversed;
	H3DRect scissor_rect;
	H3DCULL cull_mode;
	H3DShader *Shader;
	bool antialias;
	bool fog_on;
	bool point_sprites_enabled;

	int texture_wrap_s[H3D_MAX_TEXTURES];
	int texture_wrap_t[H3D_MAX_TEXTURES];
	int texture_mag_filter[H3D_MAX_TEXTURES];
	int texture_min_filter[H3D_MAX_TEXTURES];
	int texture_mip_filter[H3D_MAX_TEXTURES];
	int texture_target[H3D_MAX_TEXTURES];
	H3DTexture *texture[H3D_MAX_TEXTURES];

	int colormap_width;

	H3DSHADEMODE shade_mode;
	H3DTexture *texture_indexed;	
	
	H3DVertexFormat	vf;
};


typedef unrolled_list<H3DVertexBufferCache *, POOL_Allocator<H3DVertexBufferCache *> > H3DVertexBufferCache_List;
typedef unrolled_list<PriorityInclude *, POOL_Allocator<PriorityInclude *> > PriorityInclude_List;

struct H3DData {

	H3DRenderTarget *	render_target;			// Current render targets
	H3DRenderTarget *	depth_stencil;
	H3DTexture *	postprocessed_image;

	// used by display list cleanup tasks
	volatile int		H3D_DL_Delete_Task_count;
	Mutex				H3D_DL_Delete_Task_mutex;
	volatile bool		euthanize_data;
	bool				need_dl_delete_task;

	bool				device_objects_inited;	// device object state
	bool				device_objects_restored;// device object state
	bool				has_scene_began;		// state variable to avoid multiple BeginScene calls before an EndScene
	bool				shutting_down;

	Image *	output_image;							// valid iff we're an image driver

#ifndef NUTCRACKER
#ifdef WINDOWS_SYSTEM
	HWND hWND;
	HDC hDC;
	HDC hInfoDC;
	WNDPROC saved_window_proc;
	HPALETTE ghPalette, ghpalOld;
	HANDLE timer_handle;
#endif
#endif
	DC_Type dc_type;

	/*
	* Dimensions
	*/
	H3DRect current_window_rect;
	H3DRect master_rectangle;
	Int_XY old_size;
	Int_XY old_position;
	Int_XY original_aspect;

	int bottom_border;
	int right_border;
	int left_border;
	int top_border;

	bool use_yfudge;
	int yfudge;
	float zfudge;
	int y_screen_fudge;
	int mouse_x;
	int mouse_y;

	bool is_software;
	bool window_minimized;
	bool window_maximized;

	int	ssaa_width_scale;		// SSAA scales, apply to viewport.
	int	ssaa_height_scale;
	bool ssaa_active;			// Is an SSAA rendertarget set currently?
	bool ssaa_in_current_dl;
	bool ssaa_draw_wide_lines;		// if ssaa active and no MRT, then must draw as triangles

	AntialiasMode antialias_mode;			// Current antialiasing mode.
	int antialias_samples;		// How many samples (if AA is enabled)
	int requested_antialias_samples; // How many samples were asked for?

	// Set to prevent the driver resizing when a long operation is interrupted by the resize event.
	int	resize_delay_count;

	// Set by the driver to indicate a resize event was missed during a long operation (see above).
	bool resize_pending;

	int polymarker_batch_size;

	H3DCache *cache;

	struct {
		int nr_incarnation;
		int geo_bits;
		int flags;
		int type;
		H3DShaderID id;
	} shader_id_cache;

	Log_Font_Item *local_font_list;

	int mouse_buttons;
	bool mouse_was_queued;
	bool suppress_check_events;
	bool pending_resize;

	char *revision_string;
	bool double_buffering;
	Display_Context const *dc;

#define INVALID_INCARNATION				(Incarnation) -1
#define INDC(incarn)					(-(incarn))
#define IN3D(incarn)					(incarn)
	Incarnation tr_incarnation;
	Incarnation tr_incarnation_dc;
	Incarnation tr_incarnation_3d;
	int			   tr_is_dc;
	Incarnation geom_incarnation;
	Incarnation light_incarnation;
	Incarnation material_incarnation;
	Incarnation cutting_plane_incarnation;
	Incarnation fog_incarnation;
	Incarnation mask_region_incarnation;
	Incarnation clip_region_incarnation;

	/*
	* Advanced Rendering Structs
	*/
#define DEPTH_PEELING_MAX_LAYERS 16
	struct {
		bool			single_layer;
		bool			mode;
		bool			rts; /* is render to stencil under way */
		int				flags;
		H3DTexture *	peel_z_texture;
		int				w;
		int				h;
		H3DFORMAT		z_format;
		bool			use_mrt;
		H3DTexture *	z_textures[DEPTH_PEELING_MAX_LAYERS];
		H3DTexture *	rgba_textures[DEPTH_PEELING_MAX_LAYERS];			
	} depth_peeling;

	struct {
		bool must_pack_rgba;
		bool mode;
		int	 enabled;

		// For depth measurement only.
		H3DTexture *depth_measure_pattern;
		H3D_SURFACE_ID depth_measure_target;

		// For SSAO
		H3DTexture *jitter;
		int ssao_shader;
		H3DGPUTimer *ssao_timer;

		// For all effects.
		H3DTexture *depth_texture;

		// For depth of field, we need to reconstruct the real-world depths so we need the camera
		Camera_Handle camera;
		unsigned int camera_update_generation;

		float measured_zmin, measured_zmax;
	} depth_effects;

	struct {
		H3DTexture *	line_texture;
		bool			mode;
	} ssaa_line_target;

	// Reprojection images
	struct
	{
		H3DTexture *saved_depth;
		H3DTexture *saved_color;

		float saved_transform[16];

	} reprojection;

	struct {
		H3DRenderTarget *	depth_buffer;		// depth buffer for the shadow map
		H3DRenderTarget *	dummy_buffer;		// color buffer (not used)

		H3DTexture *		jitter;		// Per-pixel jitter in some form.


		int32_t				slice_index;
		int32_t				resolution;
		int32_t				resolution_fallback;
		int32_t				samples;

		bool				mode;
		bool				view_dependent;

	} shadow_maps;

	struct {
		
		H3DRenderTarget *	depth_buffer;		// depth buffer for the selection map
		Highlight_Map *		current_hm;
		int					width;
		int					height;
		bool				has_lo;			// Do we have lo bits as well as hi bits?
		bool				mode;

	} selection_maps;

	struct OIT {
		bool				mode;
		bool				skip;			// skip OIT collection (for clip regions)

		int					width;
		int					height;

		uint32_t			max_fragments;
	} oit;

	struct {
		RenderTargets		saved_targets;

		H3DTexture *		selection_target;
		H3DRenderTarget *	selection_depth;

		bool				mode;
	} hw_vis_sel;

	struct {
		bool				mode;
		int32_t				resolution;

	} simple_shadows;

	struct {
		bool				mode;
		int32_t				resolution;

	} glyph;

	struct {
		H3DTexture *				bump_map;
		ReflectionPlaneConstants	constants;
		int32_t						resolution;
		bool						mode;

	} reflection_plane;

	/*
	* Face and line patterns
	*/
#define NUM_FACE_PATTERNS		(FP_WINDOW_TRANSPARENT + 1)
	H3DTexture *			face_pattern_texture[NUM_FACE_PATTERNS];

	H3DTexture *			face_stipple_user_texture;
	int						face_stipple;

	H3DTexture *			lpatterns;
	Destructible<intptr_t_Lookup_Hash>::unique_ptr	lpatternhash;
	int						lpattern_count;

#define H3D_LARGEST_PRECOMPUTED_CIRCLE  20
#define H3D_LINE_TOOLS_SIZE				256
	struct {	
		H3DTexture *	texture;
		unsigned char *	data;
		int				data_size;
		float			circle_subimage_starts[H3D_LARGEST_PRECOMPUTED_CIRCLE+1];
		bool			valid;
	} line_tools;

	struct {		
		H3DTexture *	circle_texture;
		bool			valid;
	} glyphs;

	// these matrices cache calculation results, but have no effect on rendering
	// until they are copied into the shaders' constant tables
	float matrix_dc[16];
	float matrix_model_3d_single[16];
	double matrix_model_3d_double[16];
	Matrix_Single matrix_modelview_3d_single;
	Matrix_Double matrix_modelview_3d_double;
	Matrix_Single matrix_projection_3d_single;
	Matrix_Double matrix_projection_3d_double;
	Matrix_Single matrix_world_to_eye_3d_single;
	Matrix_Double matrix_world_to_eye_3d_double;
	bool matrix_texture_touched[H3D_MAX_TEXTURES];
	float matrix_texture_styled[H3D_MAX_TEXTURES][16];
	float general_displacement;
	float face_displacement_dc;
	float face_displacement_3d;
	//bool net_modelling_is_identity;
	Matrix_Single matrix_modelview_3d_save_single;
	Matrix_Double matrix_modelview_3d_save_double;
	float matrix_model_3d_save_single[16];
	double matrix_model_3d_save_double[16];
	float znear;
	float zfar;

	H3DShaderHASH *						ShaderHash;

	Destructible<H3DVertexBufferCache_List>::unique_ptr	vb_cache[256];

	H3DTexture_Hash *					colormap_hash;
	H3DTexture_Hash *					grayscale_colormap_hash;
	Glyph_Texture *						glyph_textures;
	Destructible<PriorityInclude_List>::unique_ptr	priority_include_list;
	H3DIndexBufferCache *				ib_cache;
	H3DVertexBuffer						pv_shared;				//for frequent use
	H3DVertexBuffer						pv_shared_indexed;		//for use with the operator[]

	H3DIndexBufferCache *				ib_generic;
	H3DGenericVertexBufferCache	*		vb_generic;

	char *	vs_scratch;
	char *	ps_scratch;
	char *	gs_scratch;
	char	cache_directory[2048];	//for precompiled shaders

#define FREE_VB_TEMP(h3ddata) SEMI_PROTECT( \
	if (h3ddata->vb_temp) \
		FREE_ARRAY(h3ddata->vb_temp, h3ddata->vb_temp_allocated, float); \
	h3ddata->vb_temp_allocated = 0; \
	h3ddata->vb_temp = null; \
) \

#define FREE_IB_TEMP(h3ddata) SEMI_PROTECT( \
	if (h3ddata->ib_temp) \
		FREE_ARRAY(h3ddata->ib_temp, h3ddata->ib_temp_allocated, short); \
	h3ddata->ib_temp_allocated = 0; \
	h3ddata->ib_temp = null; \
) \

#define ENSURE_VB_TEMP(h3ddata, out_vb_temp, size) SEMI_PROTECT( \
	 if (size > h3ddata->vb_temp_allocated) { \
		if (h3ddata->vb_temp) \
			FREE_ARRAY(h3ddata->vb_temp, h3ddata->vb_temp_allocated, float); \
		h3ddata->vb_temp_allocated = size; \
		ALLOC_ARRAY_CACHED(h3ddata->vb_temp, h3ddata->vb_temp_allocated, float); \
	} \
	out_vb_temp = h3ddata->vb_temp; \
)

#define ENSURE_IB_TEMP(h3ddata, out_ib_temp, size) SEMI_PROTECT( \
	if (size > h3ddata->ib_temp_allocated) { \
		if (h3ddata->ib_temp) \
			FREE_ARRAY(h3ddata->ib_temp, h3ddata->ib_temp_allocated, short); \
		h3ddata->ib_temp_allocated = size; \
		ALLOC_ARRAY_CACHED(h3ddata->ib_temp, h3ddata->ib_temp_allocated, short); \
	} \
	out_ib_temp = h3ddata->ib_temp; \
)

	float *	vb_temp;
	int							vb_temp_allocated;

	short *	ib_temp;
	int							ib_temp_allocated;

	Driver_DL *					current_display_list;

	/*
	* Segment-level display lists
	*/
	int32_t						collect_geometry;
	int32_t						save_collect_geometry;
	DL_Collector *				dl_collector;
	DL_Collector *				save_collect_list;
	uint32_t					save_action_mask;
	bool						ref_matrix_negative_3x3;
	int							display_list_limit;
	BinHash	*					binhash;

	bool						was_iconic;

	bool						right_handed_world;
	bool						right_handed_nmm;

	short *						short_pointmap;
	int							short_pointmap_allocated;

	int *						int_pointmap;
	int							int_pointmap_allocated;

#define ENSURE_TANGENTS_ARRAY(h3ddata, size) SEMI_PROTECT(\
	if (h3ddata->tangents == null || h3ddata->tangents_allocated < size) { \
		if (h3ddata->tangents) {\
			FREE_ARRAY(h3ddata->tangents, h3ddata->tangents_allocated, Plane); \
			FREE_ARRAY(h3ddata->bitangents, h3ddata->tangents_allocated, Vector); \
		}\
		h3ddata->tangents_allocated = size; \
		ALLOC_ARRAY(h3ddata->tangents, h3ddata->tangents_allocated, Plane); \
		ALLOC_ARRAY(h3ddata->bitangents, h3ddata->tangents_allocated, Vector); \
	} \
)
	Vector *					bitangents;
	Plane *						tangents;
	int							tangents_allocated;

	int							cull_results_scratch_length;
	Test *						cull_results_scratch;

	char						shader_handle[4096];			//scratch space for shader handles

	bool						culling;
	bool						continuous_mode;
	bool						priority_hsra;

	int							max_cutting_planes;
	bool						can_scissor;
	int							zbuffer_planes;
	unsigned long				zbuffer_max;

	bool						can_instance;

	bool						two_sided_lighting;
	int							flat_face_offset;		//used for flat shade model / no interpolation (dx9 uses first vertex of a triangle, ogl uses last)
	int							flat_line_offset;		//used for flat shade model / no interpolation (dx9 uses first vertex of a triangle, ogl uses last)
	Driver_Config *				card_entry;			//XBIT stuff
	int							debug;
	bool						debug_shaders;

	// user shaders
	intptr_t			user_vertex_shader_prev;
	intptr_t			user_pixel_shader_prev;
	intptr_t			user_vertex_shader;
	intptr_t			user_pixel_shader;

	// Store some generic capabilities here
	struct {
		float					max_point_sprite_size;	// How big can the hardware draw point sprites?
		bool 					has_depth_texture;		//hardware shadow maps
		bool 					has_r32f_texture;		//32-bit floating point textures
		int		 				mrt_count;				//multiple render target count

		unsigned int			max_vertex_index;
		unsigned int			max_primitive_count;

		unsigned int			max_texture_width;
		unsigned int			max_texture_height;
		ShaderModel				shader_model;
		bool					tex_pow2;				// true if texture dimensions must be powers of 2
		bool					shader_explicit_interpolation;
		bool					has_geometry_shader;
		bool					has_compute_shader;
		bool					scissored_clear;		// true if clear respects scissor state
		bool					has_derivatives;
	} caps;

	bool						supports_texture_dxt1;
	bool						supports_texture_dxt3;
	bool						supports_texture_dxt5;
	bool						supports_texture_rgb24;	
	bool						supports_texture_luminance8;

	bool						bad_flat_shading;
	bool						point_sprites_bash_all;
	bool						bad_point_sprites;
	bool						bad_packed_depth_stencil;

	HEC							error_type;
	H3D_Color_Format			rgba_format;
	H3D_Driver_Shader_Type		driver_shader_type;
	int							driver_specific_flip;

	H3DActions *	h3d_actions;

	H3DPost *	post;

}; //end struct H3DData

// Call after renderer creation.
bool HOOPS_API InitialiseH3DData(H3DData *h3ddata);

// Call before renderer destruction.
void HOOPS_API CleanupH3DData(H3DData *h3ddata);


/*
 * DISPLAY LIST RESOURCE MANAGEMENT
 */
#ifdef _WIN64
INLINE bool DISPLAY_LIST_RESOURCES_AVAILABLE(Display_Context const *, size_t request_size)
{
	return HOOPS::WORLD->display_list_vram_usage + HOOPS::WORLD->texture_vram_usage + request_size < HOOPS::WORLD->vram_usage_limit;
}	  
#else
//max for vram plus system memory before we stop compiling display lists
static const size_t DISPLAY_LIST_SYSTEM_MEMORY_LIMIT = 1024 * 1024 * (1024 + 512);
INLINE bool DISPLAY_LIST_RESOURCES_AVAILABLE(Display_Context const * dc, size_t request_size)
{
	return HOOPS::WORLD->display_list_vram_usage + HOOPS::WORLD->texture_vram_usage + request_size < HOOPS::WORLD->vram_usage_limit &&
		HOOPS::WORLD->display_list_vram_usage + dc->starting_dl_memory < DISPLAY_LIST_SYSTEM_MEMORY_LIMIT;
}
#endif


class HOOPS_API DelayedResize
{
public:
	DelayedResize(Display_Context const * dc);
	~DelayedResize();

private:
	Display_Context const *m_dc;
};

#define H3D_LONG_OPERATION(dc) DelayedResize delayed_resize(dc)








/*****************************************************************************
*****************************************************************************
					Category: H3DTexture Class

This class is a wrapper around a driver specific texture ID.
*****************************************************************************
*****************************************************************************/
#define INVALID_CACHE						(0x5c5c5c5c)
#define INVALID_H3D_TEXTURE					(H3DTexture*)(-1)
#define INVALID_GEOMETRY_CACHE				(void*)(-1)

class H3DRenderTarget : public CMO
{
public:
	H3DData *m_h3ddata;
	int m_width;
	int m_height;
	int m_msaa;
	H3DFORMAT m_format;
	unsigned int m_usage;

public:
	H3DRenderTarget(H3DData *h3ddata,
		unsigned int width,
		unsigned int height,
		unsigned int usage,
		H3DFORMAT format,
		unsigned int msaa)
	{		
		m_h3ddata = h3ddata;
		m_width = width;
		m_height = height;
		m_usage = usage;
		m_format = format;
		m_msaa = msaa;
	};
	virtual ~H3DRenderTarget(){};

	virtual bool IsTexture() {return false;}
	int Width() const { return m_width; }
	int Height() const { return m_height; }
};

void HOOPS_API fill_face(H3DData *h3ddata, Texture const * texture, unsigned char *pDst, int pitch);


class H3DTexture : public H3DRenderTarget
{
public:
	H3DTexture **	texcache;

	uint32_t		generation;
	int				m_bytes_per_pixel;

	H3DTEXTUREFILTERTYPE	m_min_filter;
	H3DTEXTUREFILTERTYPE	m_mag_filter;
	H3DTEXTUREFILTERTYPE	m_mip_filter;
	H3DTEXTUREADDRESS		m_texture_wrap_s;
	H3DTEXTUREADDRESS		m_texture_wrap_t;

public:
	H3DTexture(H3DData *h3ddata,
		unsigned int width,
		unsigned int height,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :
		H3DRenderTarget(h3ddata, width, height, usage, format, 0)
	{
		texcache = null;
		generation = HOOPS::WORLD->update_generation;
		UNREFERENCED(levels);
	};
	virtual ~H3DTexture()
	{
		if (texcache)
			*texcache = null;
		texcache = null;
	};
	virtual bool LockBytes(int bytes, unsigned char ** data){
		UNREFERENCED(bytes);
		UNREFERENCED(data);
		return false;
	};
	virtual bool Lock(H3DRect *h3drect, unsigned char ** data, int *pitch = null) = 0;
	virtual void Unlock() = 0;
	virtual void SetData(int level, void const * data, int width, int height, int stride) 
	{ 
		UNREFERENCED(level);
		UNREFERENCED(data);
		UNREFERENCED(width);
		UNREFERENCED(height);
		UNREFERENCED(stride);
		ASSERT(0); 
	}
	virtual void GenerateMipMaps() = 0;
	virtual bool IsTexture() {return true;}
	virtual bool IsCubeTexture() {return false;}
	virtual bool IsTexture3D() {return false;}
	int BytesPerPixel() const {return m_bytes_per_pixel;}

	virtual bool CompressTextureFromMemory(Texture * texture) {
		UNREFERENCED(texture);
		return false;
	}
	virtual bool LoadTextureFromMemory(Texture *	texture) {
		UNREFERENCED(texture);
		return false;
	}
};

class H3DCubeTexture : public H3DTexture
{
public:
	H3DCubeTexture(H3DData *h3ddata,
		unsigned int size,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :
		H3DTexture(h3ddata, size, size, levels, usage, format)
	{
	};
	virtual ~H3DCubeTexture(){};
	virtual bool Lock(H3DFACE face, H3DRect *h3drect, unsigned char ** data, int *pitch = null) = 0;
	virtual void Unlock(H3DFACE face) = 0;
	virtual bool IsCubeTexture() {return true;}
	virtual bool IsTexture3D() {return false;}

	// Redirect normal locks to first face (or we could ignore them?)
	virtual bool Lock(H3DRect *h3drect, unsigned char **data, int *pitch = null)
		{
		return Lock(H3DFACE_POSITIVE_X, h3drect, data, pitch);
	}
	virtual void Unlock()
	{
		Unlock(H3DFACE_POSITIVE_X);
	}
};


class H3DTexture3D : public H3DTexture
{
protected:
	unsigned int m_depth;

public:
	H3DTexture3D(H3DData *h3ddata,
		unsigned int w,
		unsigned int h,
		unsigned int depth,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format) :
		H3DTexture(h3ddata, w, h, levels, usage, format)
	{
		m_depth = depth;
	};
	virtual ~H3DTexture3D(){};
	virtual bool Lock(unsigned char **data, int *rowpitch, int *slicepitch) = 0;
	virtual void Unlock() = 0;
	virtual bool IsCubeTexture() {return false;}
	virtual bool IsTexture3D() {return true;}
	int Depth() const {return m_depth;}

	//we should never be attempting to lock a subrect of a 3d texture
	virtual bool Lock(H3DRect *h3drect, unsigned char **data, int *pitch = null) {
		UNREFERENCED (h3drect);
		UNREFERENCED (data);
		UNREFERENCED (pitch);
		ASSERT (0);
		return false;
	}

};


/*****************************************************************************
*****************************************************************************
					Category: ENSURE macros
*****************************************************************************
*****************************************************************************/



/*
 * VIEWPORT
 */
 template <typename T>
 INLINE void INVALIDATE_VIEWPORT(T * h3ddata) {
	ZERO_STRUCT (&h3ddata->cache->viewport, H3DViewport);
	h3ddata->tr_incarnation = INVALID_INCARNATION;
 }

template <typename T>
INLINE void FORCE_VIEWPORT(T * h3ddata, unsigned long left, unsigned long top, unsigned long width, unsigned long height) {
	H3DViewport *hvp = &h3ddata->cache->viewport;
	hvp->X = left;
	hvp->Y = top;
	hvp->Width = width;
	hvp->Height = height;
	hvp->MinZ = h3ddata->cache->z_range[0];
	hvp->MaxZ = h3ddata->cache->z_range[1];
	h3ddata->h3d_actions->SetViewport(hvp);
 }


template <typename T>
INLINE void ENSURE_VIEWPORT(T * h3ddata, unsigned long left, unsigned long top, unsigned long width, unsigned long height) {
	if (left != h3ddata->cache->viewport.X || top != h3ddata->cache->viewport.Y ||
		width != h3ddata->cache->viewport.Width || height != h3ddata->cache->viewport.Height ||
		h3ddata->cache->z_range[0] != h3ddata->cache->viewport.MinZ ||
		h3ddata->cache->z_range[1] != h3ddata->cache->viewport.MaxZ) {
		FORCE_VIEWPORT(h3ddata, left, top, width, height);
	}
}

#define FORCE_ALPHA_TEST(h3ddata, val) (h3ddata)->h3d_actions->force_alphatesting(val);
#define ENSURE_ALPHA_TEST(h3ddata, val) SEMI_PROTECT(\
	if ((h3ddata)->cache->alphatesting != (val)) { \
		FORCE_ALPHA_TEST(h3ddata, (val)); \
	} \
)


#define FORCE_ALPHA_FUNC(h3ddata, func, ref) (h3ddata)->h3d_actions->force_alphafunc(func,ref);
#define ENSURE_ALPHA_FUNC(h3ddata, func, ref) SEMI_PROTECT(\
	if ((h3ddata)->cache->alpha_ref != (ref) ||  \
		(h3ddata)->cache->alpha_func != (func)) { \
		FORCE_ALPHA_FUNC(h3ddata, (func), (ref)); \
	} \
)

#define FORCE_ANTI_ALIAS(h3ddata,mode) h3ddata->h3d_actions->force_anti_alias(mode);
#define ENSURE_ANTI_ALIAS(h3ddata,mode) SEMI_PROTECT( \
	if (h3ddata->cache->antialias != mode) \
		FORCE_ANTI_ALIAS(h3ddata, mode); \
)


template <typename T>
INLINE void FORCE_COLOR_MASK(T * h3ddata, bool mode, bool lock_it=false) {
	h3ddata->h3d_actions->force_color_mask(mode, lock_it);
}

template <typename T>
INLINE void ENSURE_COLOR_MASK(T * h3ddata, bool mode) {
	if (h3ddata->cache->color_mask != mode)
		FORCE_COLOR_MASK(h3ddata,mode);
}

#define FORCE_CHANNEL_MASK(h3ddata,mask) (h3ddata)->h3d_actions->force_channel_mask((mask));
#define ENSURE_CHANNEL_MASK(h3ddata,mask) SEMI_PROTECT( \
	if ((h3ddata)->cache->channel_mask != (mask)) \
		FORCE_CHANNEL_MASK(h3ddata,(mask)); \
)

#define FORCE_CULLING(h3ddata,mode) h3ddata->h3d_actions->force_culling(mode);
#define ENSURE_CULLING(h3ddata,mode) SEMI_PROTECT( \
	if (h3ddata->cache->cull_mode != mode) \
		FORCE_CULLING(h3ddata, mode); \
)


#define FORCE_DEPTH_MASK(h3ddata,mode) (h3ddata)->h3d_actions->force_depth_mask(mode);
#define ENSURE_DEPTH_MASK(h3ddata,mode) SEMI_PROTECT( \
	if ((h3ddata)->cache->depth_mask != mode) \
		FORCE_DEPTH_MASK((h3ddata), mode); \
)

#define FORCE_DEPTH_RANGE_SET(h3ddata,zmin,zmax) h3ddata->h3d_actions->force_depth_range_set(zmin,zmax);
#define ENSURE_DEPTH_RANGE_SET(h3ddata,zmin,zmax) SEMI_PROTECT(\
	if ((zmin != h3ddata->cache->z_range[0]) || (zmax != h3ddata->cache->z_range[1])) { \
		FORCE_DEPTH_RANGE_SET(h3ddata, zmin, zmax); \
	} \
)

/*
 * ATMOSPHERIC ATTENUATION
 */
#define FORCE_FOG(h3ddata, nr, mode) h3ddata->h3d_actions->force_fog(nr, mode);
#define ENSURE_FOG(h3ddata, nr, mode) SEMI_PROTECT(\
	if (h3ddata->cache->fog_on != mode) {\
		FORCE_FOG(h3ddata, nr, mode); \
	} \
)

#define FORCE_SHADE_MODE(h3ddata,mode) (h3ddata)->h3d_actions->force_shade_mode(mode);
#define ENSURE_SHADE_MODE(h3ddata,mode) SEMI_PROTECT( \
	if ((h3ddata)->cache->shade_mode != mode) \
		FORCE_SHADE_MODE(h3ddata, mode); \
)


#define FORCE_SCISSOR(h3ddata,mode) (h3ddata)->h3d_actions->force_scissor(mode);
#define ENSURE_SCISSOR(h3ddata,mode) SEMI_PROTECT(\
	if ((h3ddata)->cache->scissoring != mode) \
		FORCE_SCISSOR(h3ddata,mode); \
)


#define INVALIDATE_SCISSOR_SET(h3ddata) SEMI_PROTECT(\
	h3ddata->cache->scissor_rect.left		= INVALID_CACHE; \
	h3ddata->cache->scissor_rect.top			= INVALID_CACHE; \
	h3ddata->cache->scissor_rect.right		= INVALID_CACHE; \
	h3ddata->cache->scissor_rect.bottom		= INVALID_CACHE; \
)

#define FORCE_STENCILLING(h3ddata, mode) (h3ddata)->h3d_actions->force_stencilling(mode);
#define ENSURE_STENCILLING(h3ddata, mode) SEMI_PROTECT(\
	if ((h3ddata)->cache->stencilling != mode) { \
		FORCE_STENCILLING(h3ddata, mode); \
	} \
)

#define FORCE_TEXTURE(h3ddata,unit,texture) (h3ddata)->h3d_actions->force_texture(unit,texture);
#define ENSURE_TEXTURE(h3ddata,unit,in_texture) SEMI_PROTECT( \
	if ((h3ddata)->cache->texture[unit] != in_texture) \
		FORCE_TEXTURE((h3ddata), unit, in_texture); \
)
#define ENSURE_TEXTURE_UNSET(h3ddata, unit) SEMI_PROTECT(\
	ENSURE_TEXTURE(h3ddata, unit, null); \
)

#define FORCE_TRANSPARENCY(h3ddata,mode) (h3ddata)->h3d_actions->force_transparency(mode);
#define ENSURE_TRANSPARENCY(h3ddata,mode) SEMI_PROTECT( \
	if ((h3ddata)->cache->transparency_on != mode) \
		FORCE_TRANSPARENCY(h3ddata,mode); \
)

#define FORCE_ZBUFFERING(h3ddata,mode) (h3ddata)->h3d_actions->force_zbuffering(mode);

template <typename T>
INLINE static void ENSURE_ZBUFFERING(T * h3ddata, bool mode)
{
	if ((h3ddata)->cache->zbuffering != mode)
		FORCE_ZBUFFERING(h3ddata, mode);
}

#define FORCE_POINT_SPRITES(h3ddata, enable, min_size, max_size) (h3ddata)->h3d_actions->force_point_sprites((enable), (min_size), (max_size));
#define ENSURE_POINT_SPRITES(h3ddata, enable, min_size, max_size) SEMI_PROTECT( \
		FORCE_POINT_SPRITES(h3ddata, enable, min_size, max_size); \
)

INLINE static void INVALIDATE_TEXTURE_SETTING_CACHE(H3DData * h3ddata) {
	for (int _unit_ = 0; _unit_ < H3D_MAX_TEXTURES; ++_unit_) { 
		h3ddata->cache->texture_wrap_s[_unit_] = INVALID_CACHE;
		h3ddata->cache->texture_wrap_t[_unit_] = INVALID_CACHE;
		h3ddata->cache->texture_mag_filter[_unit_] = INVALID_CACHE;
		h3ddata->cache->texture_min_filter[_unit_] = INVALID_CACHE; 
		h3ddata->cache->texture_mip_filter[_unit_] = INVALID_CACHE;
	}
}




/*
 * STENCIL
 */
#define STENCIL_BIT (0x0080)
#define FORCE_STENCIL_MASK(h3ddata, val) h3ddata->h3d_actions->force_stencil_mask(val);
#define ENSURE_STENCIL_MASK(h3ddata, val) SEMI_PROTECT(\
	if (h3ddata->cache->stencil_mask != val) { \
		FORCE_STENCIL_MASK(h3ddata, val);	\
	} \
)

#define FORCE_STENCIL_WRITE_MASK(h3ddata, val) h3ddata->h3d_actions->force_stencil_write_mask(val);
#define ENSURE_STENCIL_WRITE_MASK(h3ddata, val) SEMI_PROTECT(\
	if (h3ddata->cache->stencil_write_mask != val) { \
		FORCE_STENCIL_WRITE_MASK(h3ddata, val);	\
	} \
)

#define FORCE_STENCIL_REF(h3ddata, val) h3ddata->h3d_actions->force_stencil_ref(val);
#define ENSURE_STENCIL_REF(h3ddata, val) SEMI_PROTECT(\
	if (h3ddata->cache->stencil_ref != val) { \
		FORCE_STENCIL_REF(h3ddata, val);	\
	} \
)

#define FORCE_STENCIL_FUNC(h3ddata, val) h3ddata->h3d_actions->force_stencil_func(val);
#define ENSURE_STENCIL_FUNC(h3ddata, val) SEMI_PROTECT(\
	if (h3ddata->cache->stencil_func != val) { \
		FORCE_STENCIL_FUNC(h3ddata, val);	\
	} \
)


#define FORCE_STENCIL_OP(h3ddata, fail, zfail, pass) h3ddata->h3d_actions->force_stencil_op(fail, zfail, pass);
#define ENSURE_STENCIL_OP(h3ddata, fail, zfail, pass) SEMI_PROTECT(\
	if (h3ddata->cache->stencil_fail != fail ||		\
		h3ddata->cache->stencil_zfail != zfail ||	\
		h3ddata->cache->stencil_pass != pass) { \
		FORCE_STENCIL_OP(h3ddata, fail, zfail, pass);	\
	} \
)

#define SET_STENCIL_FUNC(h3ddata, cmp, ref, mask, writemask) h3ddata->h3d_actions->set_stencil_func(cmp, ref, mask, writemask);
/*
#define SET_STENCIL_FUNC(h3ddata, cmp, ref, mask, writemask) SEMI_PROTECT(\
	ENSURE_STENCIL_FUNC(h3ddata, cmp); \
	ENSURE_STENCIL_REF(h3ddata, ref); \
	ENSURE_STENCIL_MASK(h3ddata, mask); \
	ENSURE_STENCIL_WRITE_MASK(h3ddata, writemask); \
)
*/

#define SET_STENCIL_OP(h3ddata, fail, zfail, pass) ENSURE_STENCIL_OP(h3ddata, fail, zfail, pass)


/*
 * BLEND
 */
#define FORCE_BLEND(h3ddata, src, dest) (h3ddata)->h3d_actions->force_blend(src, dest);
#define ENSURE_BLEND(h3ddata, src, dest) SEMI_PROTECT(\
	if ((h3ddata)->cache->src_blend != src || (h3ddata)->cache->dest_blend != dest) \
		FORCE_BLEND(h3ddata, src, dest); \
)

/*
 * Texture Sampler State
 */
#define ENSURE_SAMPLER_STATE(h3ddata, unit, filter, mip_filter, wrap) (h3ddata)->h3d_actions->force_sampler_state(unit, filter, mip_filter, wrap)

/*****************************************************************************
*****************************************************************************
						Category: Bin Class
*****************************************************************************
*****************************************************************************/

/* Display list "bin packing" */
class DL_Bin : public CMO_Pooled
{
public:

	DL_Bin(Memory_Pool * memory_pool) : items(memory_pool), owned_items(memory_pool) {
		nr = null;
		godl = null;
		flags = DL_NONE;
		sdlflags = Segment_H3DDL::NONE;
		stats.total = 0;
		stats.strips = 0;
		stats.point_count = 0;
	};

	~DL_Bin() {
		nr = null;
		for (size_t i = 0; i < owned_items.size(); ++i) {
			Deferred_Item * x = Alter(owned_items[i]); // kludge
			Destruct<Deferred_Item>(x);
		}
	};


	Net_Rendition				nr;			//the geometry in a bin should all have the same net rendition
	Deferred_Item_Ptr_Vector	items;
	Deferred_Item_Ptr_Vector	owned_items;// items owned by this bin
	Geometry_H3DDL *			godl;		//output; the resulting display list
	H3DShaderID					id;
	Strip_Flags					flags;		//any special flags
	Segment_H3DDL::SDL_Flags	sdlflags;

	struct {
		int total;
		int strips;
		int point_count;
	}	stats;
};



/*****************************************************************************
*****************************************************************************
					Category: H3DIndexBuffer Class

This class is a wrapper around a generic index buffer.  It transparently
handles both short and int based indexed buffers.
*****************************************************************************
*****************************************************************************/

class H3DIndexBuffer: public CMO
{
public:
	H3DIndexBuffer():m_ibptr(null), m_ibptr_start(null), m_stride(1){}

	H3DIndexBuffer(short const stride, void * const ib):
	m_ibptr((short*)ib), m_ibptr_start((short*)ib), m_stride(stride)
	{
	}


	H3DIndexBuffer(H3DFORMAT const format, void * const ib)
	{
		Init(format, ib);
	}

	~H3DIndexBuffer(){}

	void Init(H3DFORMAT const format, void * const ib)
	{
		m_ibptr = m_ibptr_start = (short *) ib;

		if (format == H3DFMT_INDEX16)
			m_stride = 1;
		else if (format == H3DFMT_INDEX32)
			m_stride = 2;
		else
			HE_PROCEDURAL_ERROR("Index buffer format must be H3DFMT_INDEX16 or H3DFMT_INDEX32.");
	}

	void Init(short const stride, void * const ib)
	{
		m_ibptr = m_ibptr_start = (short *) ib;
		m_stride = stride;
	}

	void CopyBuffer(H3DIndexBuffer const & source, int num_vertices)
	{
		short *dest = m_ibptr;
		short *src = source.m_ibptr;

		if (source.m_stride == m_stride)
			COPY_MEMORY(src, num_vertices * m_stride * sizeof(short), dest);
		else if (source.m_stride == 2 && m_stride == 1) {
			/* This case is usually triggered when we copy from a 32bit mesh to a 16bit indexbufferobject.
			 * This might happen because meshes have a strange restriction that it can't have more than
			 * 2^16 indices when it's declared as 16bit, whereas 16bit indexbufferobjects can.  In this case,
			 * all of the mesh's indices would be 16bits or less (we wouldn't have a 16bit IBO otherwise),
			 * so we can safely copy the points over.
			 */

			for (int i = 0; i < num_vertices; i++) {
				*dest = (short) *((int *) src);
				dest += m_stride;
				src += source.m_stride;
			}
		}
		else {
			for (int i = 0; i < num_vertices; i++) {
				*((int *) dest) = *src;
				dest += m_stride;
				src += source.m_stride;
			}
		}
	}

	// Operators
	int const operator*() const {
		if (m_stride == 2)
			return *((int *)m_ibptr);
		else
			return *m_ibptr;
	}

	bool const operator==(H3DIndexBuffer const &right) const {
		return (this->m_ibptr == right.m_ibptr);
	}
	bool const operator!=(H3DIndexBuffer const &right) const {
		return (this->m_ibptr != right.m_ibptr);
	}

	H3DIndexBuffer const & operator=(int const number) {
		if (m_stride == 2)
			*((int *) m_ibptr) = number;
		else
			*m_ibptr = (short) number;
		return *this;
	}
	H3DIndexBuffer const & operator++() {
		m_ibptr += m_stride;
		return *this;
	}
	H3DIndexBuffer const & operator--() {
		m_ibptr -= m_stride;
		return *this;
	}
	H3DIndexBuffer const & operator+=(int const steps) {
		m_ibptr += steps * m_stride;
		return *this;
	}
	H3DIndexBuffer const & operator-=(int const steps) {
		m_ibptr -= steps * m_stride;
		return *this;
	}
	H3DIndexBuffer const operator+(int const steps) const {
		H3DIndexBuffer itr;
		itr.m_ibptr = m_ibptr + steps * m_stride;
		itr.m_stride = m_stride;
		return itr;
	}
	H3DIndexBuffer operator-(int const steps) const {
		return *this + (-steps);
	}
	H3DIndexBuffer operator[](int const index) {
		return H3DIndexBuffer(m_stride, (void*)(m_ibptr + index*m_stride));
	}

	int const operator-(H3DIndexBuffer const &right) const {
		return m_stride ? static_cast<int>((this->m_ibptr - right.m_ibptr) / m_stride) : 0;
	}

	short *get_ptr() {
		return m_ibptr;
	}

	short *get_start_ptr() {
		return m_ibptr_start;
	}

	int const length() const {
		return static_cast<int>((m_ibptr - m_ibptr_start) / m_stride);
	}

private:
	short *m_ibptr, *m_ibptr_start;
	short m_stride;
};


/*****************************************************************************
*****************************************************************************
							Category: Buffers
*****************************************************************************
*****************************************************************************/


INLINE bool CULLED(Test const & cull_result)
{
	if (cull_result == Test_DISJOINT || ANYBIT(cull_result, Test_EXTENT|Test_VOLUME))
		return true;
	return false;
}


#define H3DIBO_LOCAL_BUFFERS				0x0001
#define H3DIBO_CS_LINE_PAT					0x0002


class H3DIndexedBufferObject : public CMO
{
public:
	enum Flags {
		Create_Error		= 0x1,
		VertexComplete		= 0x2,
		IndexComplete		= 0x4,
		TPP					= 0x8
	};

	unsigned int *		m_strides;
	int *				m_geom_offsets;
	void *				m_components; //tristrips, polyedges or polymarkers, or geometry.  Interpretation depends on Flags

	H3DVertexFormat		m_VF;
	H3DFORMAT			m_format;
	H3DPRIMITIVETYPE	m_type; 

	unsigned int		m_point_count;
	unsigned int		m_primitive_count;
	
	int					m_geom_first; // this is an offset into the original geometry, only used for indexing into culling list. cull_results[m_geom_first + i] corresponds to m_geom_offsets[i].
	int					m_geom_count;

	int32_t				m_flags;
	int32_t				m_index; //for GPU accelerated selection
	char				m_stream_count;

	H3DIndexedBufferObject(int index = -1)
	{
		m_point_count = 0;
		m_geom_offsets = null;
		m_components = null;
		m_geom_count = 0;
		m_index = index;
	}
	virtual ~H3DIndexedBufferObject()
	{
		if (m_components) {
			if (ANYBIT (m_flags, H3DIndexedBufferObject::VertexComplete)) {
				if (ANYBIT (m_flags, H3DIndexedBufferObject::TPP))
					FREE_ARRAY(m_components, m_geom_count, Strip_Common const *);
				else
					FREE_ARRAY(m_components, m_geom_count, Geometry const *); //-V523
			}
			else {
				ASSERT(0); //todo
			}
		}

		if (m_geom_offsets)
			FREE_ARRAY(m_geom_offsets, m_geom_count, int);
	}

	//called once per Geom
	virtual bool CreateVertexBuffer (H3DData * h3ddata, unsigned int point_count, H3DVertexFormat VF) = 0;

	virtual bool CreateIndexBuffer (H3DData * h3ddata, H3DPRIMITIVETYPE type, unsigned int length, unsigned int count) = 0;

	//only needed if we have a) implicit connectivity and b) something other than points
	void SetPrimitiveType (H3DPRIMITIVETYPE type) { m_type = type; }

	virtual bool LockIndexBuffer(unsigned int OffsetToLock, unsigned int SizeToLock, H3DIndexBuffer *ib, unsigned int Flags) = 0;
	virtual bool LockVertexBuffer(unsigned int OffsetToLock, unsigned int SizeToLock, H3DVertexBuffer *pv, unsigned int Flags) = 0;
	virtual void UnlockIndexBuffer() {
		ASSERT (!ANYBIT (m_flags, H3DIndexedBufferObject::IndexComplete)); //should be called only once per ibo
		m_flags |= H3DIndexedBufferObject::IndexComplete;
	}

	virtual void UnlockVertexBuffer() {
		ASSERT (!ANYBIT (m_flags, H3DIndexedBufferObject::VertexComplete)); //should be called only once per ibo
		m_flags |= H3DIndexedBufferObject::VertexComplete;
		if (m_components) {
			void_ptr_List *	list = (void_ptr_List *)m_components;
			m_geom_count = (int)list->size();
			if (m_geom_count > 0) {
				void const **buf;
				ALLOC_ARRAY (buf, m_geom_count, void const *);
				for (int i = 0 ; i < m_geom_count ; i++) {
					buf[i] = list->front();
					list->pop_front();
				}
				m_components = (void *)buf;
			}
			else
				m_components = null;

			Destruct<void_ptr_List>(list);
		}
	};
	virtual void Draw(H3DData * h3ddata, Test * cull_results = null, int instances = 1) const = 0;
	virtual void Draw_Decimated(H3DData * h3ddata, float decimation = 1.0f) const = 0;	
	virtual void Draw_CSLinePattern(H3DData * h3ddata, float u_scale, float v_scale, float const * matMVP, float const * matScreen, Test * cull_results = null, int instances = 1) = 0;
	virtual void Draw_CSEdgePattern(H3DData * h3ddata, float u_scale, float v_scale, float const * matMVP, float const * matScreen, Test * cull_results = null, int instances = 1) = 0;

	virtual void Draw_Point_Sprites(H3DData * h3ddata, float decimation = 1.0f)
	{
		Draw_Decimated(h3ddata, decimation);
	}

	int GetPointCount () const { return m_point_count; }

	void PopulateVertexBuffer(H3DVertexBuffer * vertices, int count)
	{
		H3DVertexBuffer pv;
		LockVertexBuffer(0, 0, &pv, 0);
		vertices->reset();
		vertices->copy_to(&pv, count);
		UnlockVertexBuffer();
	}

	void PopulateIndexBuffer(short * indices, int size)
	{
		H3DIndexBuffer pi;
		LockIndexBuffer(0, 0, &pi, 0);
		COPY_MEMORY(indices, size, pi.get_start_ptr());
		UnlockIndexBuffer();
	}

	void AppendStripCommon (Display_Context const * dc, Tristrip const * ts)
	{
		m_flags |= H3DIndexedBufferObject::TPP;
		AppendComponent (dc, (void const *)ts);
	}

	void AppendStripCommon(Display_Context const * dc, Polyedge const * pe)
	{
		m_flags |= H3DIndexedBufferObject::TPP;
		AppendComponent(dc, (void const *)pe);
	}

	void AppendStripCommon(Display_Context const * dc, Polymarker const * pm)
	{
		m_flags |= H3DIndexedBufferObject::TPP;
		AppendComponent(dc, (void const *)pm);
	}

	void AppendGeometry (Display_Context const * dc, Geometry const *g)
	{
		ASSERT (!ANYBIT (m_flags, H3DIndexedBufferObject::TPP));
		AppendComponent (dc, (void const *)g);
	}

	bool DecipherPrimitiveIndex(uint32_t primitive_index, Geometry const **g, Strip_Common const **tpp, int *fid)
	{
		int i = 0;
		if (m_geom_count > 1) {
			for (; i < m_geom_count ; i++)
				if (m_geom_offsets[i] <= (int)primitive_index)
					break;
			*fid = (int)primitive_index - m_geom_offsets[i];  // <---xxx: not sure about this
		}
		else
			*fid = (int)primitive_index;

		//this function can't be called until after vertices are populated (since that's 
		//where components is converted from list to array form)
		ASSERT (ANYBIT (m_flags, H3DIndexedBufferObject::VertexComplete));
		if (ANYBIT (m_flags, H3DIndexedBufferObject::TPP)) {
			//polyhedron-like object
			Strip_Common const **tpp_array = (Strip_Common const **)m_components;
			*tpp = tpp_array[i];
			*g = tpp_array[i]->parent_geom;
		}
		else {
			//non-polyhedron (e.g. polygon)
			Geometry const **geo_array = (Geometry const **)m_components;
			*tpp = null;
			*g = geo_array[i];
		}
		return true;
	}

	bool GetCreateStatus() const {return !ANYBIT(m_flags, H3DIndexedBufferObject::Create_Error); }

	virtual bool IsLocal() const { return false; }
	virtual bool IsVRAM() const { return true; }
	virtual void ChangeToVRAM() { }
	virtual void ChangeToLocal() { }

private:

	void AppendComponent (Display_Context const * dc, void const * thing) {
		if (!m_components)
			m_components = Construct<void_ptr_List>(dc->memory_pool()).release();
		void_ptr_List *	list = (void_ptr_List *)m_components;
		list->push_back (thing);
	}
};


class SwitchableIBO : public H3DIndexedBufferObject
{
public:

	enum BufferType
	{
		VRAM,
		Local,
	};

	H3DData * m_h3ddata;
	unsigned char * m_indices;
	float ** m_vertex_data;

	unsigned int m_index_size;
	unsigned int m_primitive_count;
	BufferType m_buffer_type;

	SwitchableIBO(int index, unsigned int flags) : H3DIndexedBufferObject(index),
		m_h3ddata(null),
		m_indices(null),
		m_vertex_data(null),
		m_index_size(0),
		m_primitive_count(0),
		m_buffer_type(ANYBIT(flags, H3DIBO_LOCAL_BUFFERS) ? Local : VRAM)
	{

	}

	~SwitchableIBO()
	{
		if (m_vertex_data) {

			for (int i = 0; i < m_stream_count; ++i) {
				if (m_vertex_data[i])
					FREE_ARRAY(m_vertex_data[i], m_point_count * m_strides[i], float);
			}

			FREE_ARRAY(m_vertex_data, m_stream_count, float*);
		}

		if (m_indices)
			FREE_ARRAY(m_indices, m_indices, unsigned char);

		FREE_ARRAY(m_strides, m_stream_count, unsigned int);
	}

	//called once per Geom
	bool CreateVertexBuffer(H3DData *h3ddata, unsigned int point_count, H3DVertexFormat VF)
	{
		m_h3ddata = h3ddata;

		ASSERT(point_count > 0);

		m_point_count = point_count;
		m_stream_count = VF.get_stream_count();
		m_VF = VF;

		ALLOC_ARRAY(m_strides, m_stream_count, unsigned int);
		VF.get_stream_strides(m_strides);

		AllocateLocalVertexArrays();

		return true;
	}

	//called ts->strips times per Geom
	bool CreateIndexBuffer(H3DData *h3ddata, H3DPRIMITIVETYPE type, unsigned int length, unsigned int count)
	{
		// No point creating an index buffer if the vertex buffer already failed
		if (!GetCreateStatus())
			return false;

		m_h3ddata = h3ddata;

		if (m_point_count <= 0x0000ffff && count <= 0x0000ffff)
			m_format = H3DFMT_INDEX16;
		else
			m_format = H3DFMT_INDEX32;

		m_type = type;
		m_index_size = length * (m_format == H3DFMT_INDEX16 ? sizeof(short) : sizeof(int));
		m_primitive_count = count;

		AllocateLocalIndexArray();

		return true;
	}

	bool LockIndexBuffer(unsigned int OffsetToLock, unsigned int SizeToLock, H3DIndexBuffer * ib, unsigned int Flags)
	{
		UNREFERENCED(OffsetToLock);
		UNREFERENCED(SizeToLock);
		UNREFERENCED(Flags);

		ib->Init(m_format, m_indices);

		return true;
	}

	bool LockVertexBuffer(unsigned int OffsetToLock, unsigned int SizeToLock, H3DVertexBuffer *pv, unsigned int Flags)
	{
		UNREFERENCED(OffsetToLock);
		UNREFERENCED(SizeToLock);
		UNREFERENCED(Flags);

		pv->Init(m_vertex_data, &m_VF);

		return true;
	}

	void UnlockIndexBuffer()
	{
		H3DIndexedBufferObject::UnlockIndexBuffer();

		if (m_buffer_type == VRAM) {
			CopyLocalIndexArrayToVRAM();

			if (CanDownloadFromVRAM())
				ReleaseLocalIndexArray();
		}
	}

	void UnlockVertexBuffer()
	{
		H3DIndexedBufferObject::UnlockVertexBuffer();

		if (m_buffer_type == VRAM) {
			CopyLocalVertexArraysToVRAM();

			if (CanDownloadFromVRAM())
				ReleaseLocalVertexArrays();
		}
	}

	void AllocateLocalVertexArrays()
	{
		ASSERT(m_vertex_data == null);

		ALLOC_ARRAY(m_vertex_data, m_stream_count, float *);

		for (int i = 0; i < m_stream_count; ++i) {
			size_t const stream_size = m_point_count * m_strides[i];
			ALLOC_ARRAY(m_vertex_data[i], stream_size, float);
		}
	}

	void ReleaseLocalVertexArrays()
	{
		for (int i = 0; i < m_stream_count; ++i) {
			size_t const stream_size = m_point_count * m_strides[i];
			FREE_ARRAY(m_vertex_data[i], stream_size, float);
		}

		FREE_ARRAY(m_vertex_data, m_stream_count, float *);
		m_vertex_data = null;
	}

	void AllocateLocalIndexArray()
	{
		ASSERT(m_indices == null);

		if (m_index_size)
			ALLOC_ARRAY(m_indices, m_index_size, unsigned char);
	}

	void ReleaseLocalIndexArray()
	{
		FREE_ARRAY(m_indices, m_index_size, unsigned char);
		m_indices = null;
	}

	bool IsLocal() const
	{
		return m_buffer_type == Local;
	}

	bool IsVRAM() const
	{
		return m_buffer_type == VRAM;
	}

	void ChangeToVRAM()
	{
		ASSERT(m_buffer_type == Local);
		m_buffer_type = VRAM;

		// Nothing has been allocated yet so don't allocate hardware buffers. 
		if (m_vertex_data == null)
			return;

		CopyLocalVertexArraysToVRAM();
		
		if (CanDownloadFromVRAM())
			ReleaseLocalVertexArrays();

		CopyLocalIndexArrayToVRAM();

		if (CanDownloadFromVRAM())
			ReleaseLocalIndexArray();
	}

	void ChangeToLocal()
	{
		ASSERT(m_buffer_type == VRAM);
		m_buffer_type = Local;

		AllocateLocalVertexArrays();
		DownloadVertexArraysFromVRAM();

		AllocateLocalIndexArray();
		DownloadIndexArrayFromVRAM();
	}

	virtual void CopyLocalVertexArraysToVRAM() = 0;
	virtual void CopyLocalIndexArrayToVRAM() = 0;
	virtual void DownloadVertexArraysFromVRAM() = 0;
	virtual void DownloadIndexArrayFromVRAM() = 0;

	virtual bool CanDownloadFromVRAM() const
	{
		return true;
	}
};

class H3DVertexBufferCache : public CMO
{
public:
	H3DVertexBufferCache(){}
	virtual ~H3DVertexBufferCache(){}

	virtual bool CreateVertexBuffer(H3DData *h3ddata, unsigned int max_points, H3DVertexFormat VF) = 0;
	virtual bool Reset() = 0;

	virtual bool Discard() = 0;
	virtual bool PushDataToBuffer(Matrix matrix = null, int length = 0) = 0;
	virtual bool Lock(unsigned int count, H3DVertexBuffer *pv) = 0;
	virtual bool Unlock(Matrix matrix = null) = 0;
	virtual bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1) = 0;
	virtual bool DrawRepeatable(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1) = 0;
	virtual void DrawComplete() = 0;

	H3DVertexFormat			m_VF;
	unsigned int *			m_strides;
	char					m_stream_count;
	unsigned int			m_nMaxPoints;
	unsigned int			m_nCurVertexOffset;
	unsigned int			m_nNextVertexOffset;

	unsigned int			m_locked_vertex_size;
	unsigned int			m_push_data_start;
	float *	temp_vb;
	float *	temp_nb;
	float **	m_vertex_data;
};


class H3DIndexBufferCache : public CMO
{
protected:
	H3DVertexBufferCache *	m_pVertexBufferCache;
	H3DGenericVertexBufferCache	*m_pGenericVertexBufferCache;

public:
	H3DIndexBufferCache(){};
	virtual ~H3DIndexBufferCache(){};

	virtual bool CreateIndexBuffer(H3DData *h3ddata) = 0;
	virtual bool Lock(int count, short **ppbData) = 0;
	virtual bool Unlock() = 0;
	virtual bool Draw(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, unsigned int VertexCount, unsigned int PrimitiveCount) = 0;

	virtual void DrawComplete() = 0;

	void SetVertexBufferCache (H3DVertexBufferCache *vbcache)
	{
		m_pGenericVertexBufferCache = null;
		m_pVertexBufferCache = vbcache;
	}
	void SetGenericVertexBufferCache (H3DGenericVertexBufferCache *vbcache)
	{
		m_pVertexBufferCache = null;
		m_pGenericVertexBufferCache = vbcache;
	}
};

#define VB_CACHE_MAX_SIZE	16384
#define IB_CACHE_MAX_SIZE	128000

template <typename T>
INLINE void ENSURE_VB_CACHE_VF(T * h3ddata, H3DVertexFormat vf, H3DVertexBufferCache *& vbcache)
{
	unsigned int vf_bits = vf.get_vf();

	vbcache = null;

	auto &	vbcache_list = h3ddata->vb_cache[vf_bits & 0xFF];

	if (vbcache_list != null) {
		for (auto it = vbcache_list->cbegin(), end = vbcache_list->cend(); it != end; ++it) {
			if ((*it)->m_VF == vf_bits) {
				vbcache = *it;
				break;
			}
		}
	}
	else {
		h3ddata->vb_cache[vf_bits & 0xFF] = Construct<H3DVertexBufferCache_List>(h3ddata->dc->memory_pool());
	}

	if (vbcache == null) {
		vbcache = h3ddata->h3d_actions->CreateVertexBufferCache();
		vbcache->CreateVertexBuffer(h3ddata, VB_CACHE_MAX_SIZE, vf_bits);
		vbcache_list->push_back(vbcache);
	}
}


template <typename T>
INLINE void ENSURE_IB_CACHE(T * h3ddata, H3DVertexBufferCache *& vb)
{
	if (h3ddata->ib_cache == null) {
		h3ddata->ib_cache = h3ddata->h3d_actions->CreateIndexBufferCache();
		h3ddata->ib_cache->CreateIndexBuffer(h3ddata);
	}
	h3ddata->ib_cache->SetVertexBufferCache(vb);
}



class H3DGenericVertexBufferCache : public CMO
{
public:
	H3DGenericVertexBufferCache(H3DData *h3ddata) :
		m_nMaxPoints(0xffff),
		m_curr_offset(0),
		m_next_offset(0),
		m_h3ddata(h3ddata)
	{
		m_strides[0] = 3;			// position
		m_strides[1] = 3;			// normals
		m_strides[2] = 1;			// color
		m_strides[3] = 1;			// selection
		for (int i = 4; i < 12; i++)
			m_strides[i] = 4;		// texcoords
	}
	virtual ~H3DGenericVertexBufferCache(){}
	virtual bool Lock(H3DVertexFormat const &vf, unsigned int count, H3DVertexBuffer *pv) = 0;
	virtual bool Unlock() = 0;
	virtual bool Draw(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount) = 0;
	virtual bool DrawRepeatable(H3DData *h3ddata , H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount) = 0;
	virtual void DrawComplete() = 0;

	H3DVertexFormat			m_VF;
	H3DData *	m_h3ddata;

	unsigned int			m_nMaxPoints;
	unsigned int			m_curr_offset;
	unsigned int			m_next_offset;
	float *	m_vertex_data[12];
	unsigned int			m_strides[12];
};


/*****************************************************************************
*****************************************************************************
						Category: H3DConstantTable class
*****************************************************************************
*****************************************************************************/
class H3DConstantTable : public CMO_Pooled
{
protected:
	H3DData *m_h3ddata;

public:
	H3DConstantTable(H3DData *h3ddata)
	{
		m_h3ddata = h3ddata;
	};
	virtual ~H3DConstantTable() {};

	virtual void SetFloat(const char *name, const float value)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(name);
		UNREFERENCED(value);
	}
	virtual void SetInt(const char *name, const int value)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(name);
		UNREFERENCED(value);
	}
	virtual void SetFloatArray(const char *handle, const float *values, int count)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(handle);
		UNREFERENCED(values);
		UNREFERENCED(count);
	};
	virtual void SetMatrix(const char *name, const float *matrix)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(name);
		UNREFERENCED(matrix);
	}
	virtual void SetVector(const char *name, const float *vector, const int vector_width = 4)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(name);
		UNREFERENCED(vector);
		UNREFERENCED(vector_width);
	};
	virtual void SetVectorArray(const char *name, const float *vectors, const int num_vectors, const int vector_width = 4)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(name);
		UNREFERENCED(vectors);
		UNREFERENCED(vector_width);
		UNREFERENCED(num_vectors);
	};
	virtual unsigned long GetSamplerIndex(const char *name)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(name);
		return 0;
	};
};

/*****************************************************************************
*****************************************************************************
						Category: H3DPost class
*****************************************************************************
*****************************************************************************/
#define SHADER_FLAG_NONE			0x0000
#define SHADER_FLAG_TAPS_4			0x0001
#define SHADER_FLAG_TAPS_8			0x0002
#define SHADER_FLAG_TAPS_12			0x0004
#define SHADER_FLAG_TAPS_16			0x0008
#define SHADER_FLAG_TEXCOORD_2		0x0010		//uses second texcoord input
#define SHADER_FLAG_DEPTH_OUTPUT	0x0020
#define SHADER_FLAG_MEDIUM_QUALITY	0x0040
#define SHADER_FLAG_HIGH_QUALITY	0x0080
#define SHADER_FLAG_NEED_SWIZZLE	0x0100
#define SHADER_FLAG_DIVIDE_ALPHA	0x0200

// Shader name/content pair
struct HOOPS_API ShaderName
{
	char const *	string;
	char const *	filename;
	int				flags;				//bitfield flags to indicate #defines to put in shaders
	
#ifdef H3D_SHADER_DEBUG
	int				size;
#define SHADER_FLAG_NEEDS_CLEANUP 0x80000000
	void load();
	~ShaderName() { 
		if (ANYBIT(flags, SHADER_FLAG_NEEDS_CLEANUP))
			FREE_ARRAY(string, size, char);
	}
#else
	INLINE void load() {}
#endif
};

// Define a shader name
#ifdef H3D_SHADER_DEBUG
#define SHADER_NAME(name,flags) { name, #name, flags, 0 }
#else
#define SHADER_NAME(name,flags) { name,  #name, flags}
#endif

// Available depth effects.
enum
{
	FAST_AMBIENT_OCCLUSION		= (1<<0),
	FAST_SILHOUETTES			= (1<<1),
	FAST_EXTERNAL_SILHOUETTES	= (1<<2),
	DEPTH_MEASUREMENT			= (1<<3),
	REPROJECTION				= (1<<4),
	DEPTH_OF_FIELD				= (1<<5),
	DEPTH_CAPTURE				= (1<<6),
	EYE_DOME_LIGHTING			= (1 << 7)
};

class HOOPS_API H3DPost : public CMO_Pooled {
public:
	H3DPost() :
		m_head(null),
		m_h3ddata(null),
		m_vb(null),
		m_2d_vertex_constant_table(null),
		m_3d_vertex_constant_table(null)

	{
		memset( m_constant_tables, 0, sizeof(m_constant_tables) );
	}
	virtual ~H3DPost() 
	{
		H_SAFE_DELETE(m_vb);
	}

	virtual bool Begin() {
		ABSTRACT_ERROR;
		return false;
	};
	virtual void End() {
		ABSTRACT_ERROR;
	};

	// Which post shaders are available?
	enum Vertex_Shader
	{
		VS_2D,
		VS_3D,
		NUM_VERTEX_SHADERS,
	};

	enum Shader
	{
		// General-purpose
		SH_COPY,
		SH_FILTER_4,
		SH_FILTER_8,
		SH_FILTER_12,
		SH_FILTER_16,
		SH_ADD_8,
		SH_DEPTH_FILTER_8,

		// Special-purpose
		SH_APPLY_REFLECTION,
		SH_AMBIENT_OCCLUSION_LOW,
		SH_AMBIENT_OCCLUSION_MEDIUM,
		SH_AMBIENT_OCCLUSION_HIGH,
		SH_AMBIENT_OCCLUSION_FILTER,
		SH_SILHOUETTE,
		SH_EXTERNAL_SILHOUETTE,
		SH_CAPTURE_DEPTH,
		SH_CAPTURE_BLOOM,
		SH_REPROJECT,
		SH_DEPTH_OF_FIELD,
		SH_DEPTH_TO_FOCUS,
		SH_FOCUS_FILTER_8,
		SH_MASK_BLOOM,
		SH_CLEAR_ALPHA,
		SH_FATTEN_PIXELS,
		SH_IMAGE_BLOCK,
		SH_IMAGE_BLOCK_BGRA,
		SH_EYE_DOME_SHADE,
		SH_EYE_DOME_MIX,
		SH_EYE_DOME_BLUR,
		SH_DIVIDE_ALPHA,

		NUM_SHADERS,
	};

	virtual H3DConstantTable *GetConstants( Vertex_Shader const which_vs, Shader const which )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(which_vs);
		UNREFERENCED(which);
		return null;
	}

	enum ShaderQuality
	{
		SQ_LOW,
		SQ_MEDIUM,
		SQ_HIGH,

		NUM_QUALITY_LEVELS,
	};

	virtual ShaderQuality GetQuality( Shader const which )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(which);
		return SQ_LOW;
	}

	void OnResetDevice();

	// Get a temporary render-target texture buffer from the cache
	H3DTexture *GetWorkTexture(const unsigned long width, const unsigned long height, const H3DFORMAT format, unsigned int usage = H3DTEXUSAGE_RENDERTARGET);

	// Get a temporary render-target buffer from the cache
	H3DRenderTarget *GetWorkSurface(const unsigned long width, const unsigned long height, const H3DFORMAT format, bool msaa);

	// Indicate that client has finished with a temporary buffer
	void ReleaseWorkTexture(H3DTexture **texture);

	// Indicate that client has finished with a temporary buffer
	void ReleaseWorkSurface(H3DRenderTarget ** surface);

	virtual void Copy(H3DRenderTarget *dest, H3DTexture *src, RGBA const * modulation_color = null, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(src);
		UNREFERENCED(modulation_color);
		UNREFERENCED(filter);
	}

	virtual void Copy(H3DTexture *dest, H3DTexture *src, RGBA const * modulation_color = null, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(src);
		UNREFERENCED(modulation_color);
		UNREFERENCED(filter);
	}

	virtual void Filter(H3DTexture *dest, H3DTexture *src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(src);
		UNREFERENCED(num_taps);
		UNREFERENCED(taps);
		UNREFERENCED(filter);
	}

	virtual void Filter(H3DRenderTarget *dest, H3DTexture *src, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT)
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(src);
		UNREFERENCED(num_taps);
		UNREFERENCED(taps);
		UNREFERENCED(filter);
	}

	virtual void DepthFilter( H3DTexture * dest, H3DTexture * src, H3DTexture *depth, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(src);
		UNREFERENCED(num_taps);
		UNREFERENCED(taps);
		UNREFERENCED(filter);
		UNREFERENCED(depth);
	}

	virtual void CustomFilter( H3DTexture * dest, H3DTexture * src, Shader which, int num_taps, Vector const *taps, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(src);
		UNREFERENCED(num_taps);
		UNREFERENCED(taps);
		UNREFERENCED(filter);
		UNREFERENCED(which);
	}

	// Add the specified textures together on the target
	virtual void Add( H3DRenderTarget * dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = null, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(srcs);
		UNREFERENCED(num_srcs);
		UNREFERENCED(modulation_color);
		UNREFERENCED(filter);
	}

	// Filter the specified texture to the target
	virtual void Add( H3DTexture * dest, H3DTexture **srcs, int num_srcs, RGBA const *modulation_color = null, H3DTEXTUREFILTERTYPE filter = H3DTEXF_POINT )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(srcs);
		UNREFERENCED(num_srcs);
		UNREFERENCED(modulation_color);
		UNREFERENCED(filter);
	}


	//
	// Compounds
	//

	// Blur a texture using a 7x7 Gaussian kernel
	void GaussianBlur7( H3DTexture *texture, int iterations = 1 );

	// Blur a texture using a 7x7 box kernel
	void BoxBlur7( H3DTexture *texture, int iterations = 1 );

	// Blur a texture using a 3x3 Gaussian kernel
	void GaussianBlur3( H3DTexture *texture, int iterations = 1 );

	// Blur a texture using a 3x3 box kernel
	void BoxBlur3( H3DTexture *texture, int iterations = 1 );

	// Blur a texture using a star-shaped kernel
	void StarBlur( H3DTexture *texture, int iterations = 1 );

	void DivideAlpha(H3DTexture * source, H3DTexture * dest);

	virtual void Apply( H3DRenderTarget *dest, Shader const which, float depth = 0.0f, float const *matrix = null, H3DTexture *offset_texture = null) {
		ABSTRACT_ERROR;
		UNREFERENCED(dest);
		UNREFERENCED(which);
		UNREFERENCED(depth);
		UNREFERENCED(matrix);
		UNREFERENCED(offset_texture);
	};

	virtual H3DConstantTable * ActivateShader(Shader which, float depth = 0.0f, float const *matrix = null, H3DTexture *offset_texture = null) {
		ABSTRACT_ERROR;
		UNREFERENCED(which);
		UNREFERENCED(depth);
		UNREFERENCED(matrix);
		UNREFERENCED(offset_texture);
		return null;
	};

	// Helpers
	void SetTexelOffset( H3DTexture* src );			// Apply texel offset to constants based on given texture

	virtual void OnReloadShaders() {
		ABSTRACT_ERROR;
		return;
	};

	virtual void Draw(H3DPost::Vertex_Shader, H3DPost::Shader)
	{
		m_vb->Draw(m_h3ddata);
	}

	void CreateVertexBuffer();	

protected:
	struct Buffer
	{
		bool active;										// Is someone using this buffer?
		H3DTexture *	texture;							// The rendertarget texture
		H3DRenderTarget *surface;							// The surface (for depth buffers)
		Buffer *next;										// Next buffer in the chain
	};
	Buffer *m_head;											// List of temporary buffers we've allocated

	H3DData *m_h3ddata;
	H3DVertexFormat m_VF;
	H3DIndexedBufferObject *m_vb;

	H3DConstantTable *m_2d_vertex_constant_table;
	H3DConstantTable *m_3d_vertex_constant_table;
	H3DConstantTable *m_constant_tables[NUM_VERTEX_SHADERS][NUM_SHADERS];		// Constant tables for binding the pixel shaders.
};


/*****************************************************************************
*****************************************************************************
					Category: Some helper functions
*****************************************************************************
*****************************************************************************/
template <typename F> void H3DMatrixPerspectiveLH(F * matrix, F w, F h, F zn, F zf, int driver_specific_flip)
{
	SET_MEMORY(matrix, 16*sizeof(F), 0);
	matrix[0] = 2 * zn / w;
	matrix[5] = 2 * zn / h;
	matrix[10] = driver_specific_flip * zf / (zf - zn);
	matrix[11] = (F) driver_specific_flip;
	matrix[14] = (zn * zf) / (zn - zf);
}

template <typename F> void H3DMatrixOrthoLH(F * matrix, F w, F h, F zn, F zf, int driver_specific_flip)
{
	SET_MEMORY(matrix, 16*sizeof(F), 0);
	matrix[0] = 2 / w;
	matrix[5] = 2 / h;
	matrix[10] = driver_specific_flip / (zf - zn);
	matrix[14] = zn / (zn - zf);
	matrix[15] = 1;
}

template <typename F> void H3DMatrixOrthoOffCenterRH(F * matrix, F l, F r, F b, F t, F zn, F zf)
{
	Matrix_44::Init_Identity(matrix);

	matrix[0] = (F)2.0 / (r - l);
	matrix[5] = (F)2.0 / (t - b);
	matrix[10] = (F)1.0 / (zn - zf);
	matrix[12] = -(F)1.0 - (F)2.0 *l / (r - l);
	matrix[13] = (F)1.0 + (F)2.0 * t / (b - t);
	matrix[14] = zn / (zn -zf);
}

/**
 * HELPER: Align to a power-of-2
 */
INLINE size_t align(size_t a, size_t alignment) {
	return (a + (alignment-1)) & ~(alignment-1);
}

/*****************************************************************************
*****************************************************************************
					Category: H3DActions Class

This class contains pointers to driver specific helper functions.
*****************************************************************************
*****************************************************************************/
class HOOPS_API H3DActions : public CMO_Pooled
{
protected:
	H3DData *m_h3ddata;

public:
	H3DActions(){};
	H3DActions(H3DData *h3ddata) {m_h3ddata = h3ddata;};


public:

// SIL: Maintain one list of actions and switch between modes here
#ifdef PURE_ABSTRACT
#define H3DACTION(method) virtual method = 0
#else
#define H3DACTION(method) virtual method
#endif

	H3DACTION(Int_XY GetWindowSize()) const
	{
		ABSTRACT_ERROR;
		return Int_XY();
	}

	H3DACTION(void DepthPeeling(Net_Rendition const &));	

	H3DACTION(void Present(Display_Context const *))
	{
		ABSTRACT_ERROR;
	}

	H3DACTION(void InitPicture(Net_Rendition const &, bool flush = true))
	{
		UNREFERENCED(flush);
		ABSTRACT_ERROR;
	}

	H3DACTION(void FinishPicture(Net_Rendition const &, bool swap_buffers = true, bool flush = true))
	{
		UNREFERENCED(swap_buffers);
		UNREFERENCED(flush);
		ABSTRACT_ERROR;
	}

	H3DACTION(bool Snapshot(Net_Rendition const &, int, int, int, void *))
	{
		ABSTRACT_ERROR;		
		return false;
	};

	H3DACTION( void draw_textured_rectangle(H3DTexture *	texture,
											int					x,
											int					y,
											int					width,
											int					height,
											float				u1 = 0,
											float				v1 = 0,
											float				u2 = 1,
											float				v2 = 1))
	{
		ABSTRACT_ERROR;		
		UNREFERENCED(texture);
		UNREFERENCED(x);
		UNREFERENCED(y);
		UNREFERENCED(width);
		UNREFERENCED(height);
		UNREFERENCED(u1);
		UNREFERENCED(v1);
		UNREFERENCED(u2);
		UNREFERENCED(v2);
	}

	H3DACTION( H3DGenericVertexBufferCache* CreateGenericVertexBufferCache() ) {ABSTRACT_ERROR; return null;};
	H3DACTION( H3DVertexBufferCache* CreateVertexBufferCache() ) {ABSTRACT_ERROR; return null;};
	H3DACTION( H3DIndexBufferCache* CreateIndexBufferCache() ) {ABSTRACT_ERROR; return null;};
	H3DACTION( H3DIndexedBufferObject* CreateIndexedBufferObject(int index, unsigned int flags) ) 
	{
		ABSTRACT_ERROR; UNREFERENCED(index); 
		ABSTRACT_ERROR; UNREFERENCED(flags);
		return null;
	};

	H3DACTION( bool CreateTexture(
		unsigned int width,
		unsigned int height,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format,
		H3DTexture **id) )
	{
		ABSTRACT_ERROR;		
		UNREFERENCED(width);
		UNREFERENCED(height);
		UNREFERENCED(levels);
		UNREFERENCED(usage);
		UNREFERENCED(format);
		UNREFERENCED(id);		
		return false;
	};

	H3DACTION( bool CreateCubeTexture(
		unsigned int size,
		unsigned int levels,
		unsigned int usage,
		H3DCubeTexture **id) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(size);
		UNREFERENCED(levels);
		UNREFERENCED(usage);
		UNREFERENCED(id);
		return false;
	};

	H3DACTION( bool CreateTexture3D(
		unsigned int w,
		unsigned int h,
		unsigned int depth,
		unsigned int levels,
		unsigned int usage,
		H3DFORMAT format,
		H3DTexture3D **id) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(w);
		UNREFERENCED(h);
		UNREFERENCED(depth);
		UNREFERENCED(levels);
		UNREFERENCED(usage);
		UNREFERENCED(format);
		UNREFERENCED(id);
		return false;
	};

	H3DACTION( bool CreateRenderTarget(
		unsigned int width,
		unsigned int height,
		unsigned int usage,
		H3DFORMAT z_format,
		unsigned int msaa,
		H3DRenderTarget **id))
	{
		ABSTRACT_ERROR;
		UNREFERENCED(width);
		UNREFERENCED(height);
		UNREFERENCED(usage);
		UNREFERENCED(z_format);
		UNREFERENCED(msaa);
		UNREFERENCED(id);
		return false;
	};

	H3DACTION( void SetTexture(int unit, H3DTexture *id) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(unit);
		UNREFERENCED(id);
	}

	H3DACTION( void SetViewport(H3DViewport const *hvp) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(hvp);
	}

	H3DACTION( H3DShader* CreateShader(const H3DShaderID &id, size_t sizeof_h3dshader) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(id);
		UNREFERENCED(sizeof_h3dshader);
		return null;
	}

	H3DACTION( void Clear(int flags, int color, float z_value, int stencil_value, bool simple_clear = false) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(flags);
		UNREFERENCED(color);
		UNREFERENCED(z_value);
		UNREFERENCED(stencil_value);

		// this arg is used to force only a call to clear function, without the supporting code around for depth effects
		UNREFERENCED(simple_clear);
	}

	H3DACTION( void Begin_Trace(wchar_t const *string) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(string);
	}
	H3DACTION( void End_Trace(wchar_t const *string) ) //-V524
	{
		ABSTRACT_ERROR;
		UNREFERENCED(string);
	}
	H3DACTION( void Dump_State() )
	{
		ABSTRACT_ERROR;
	}

	H3DACTION ( void CreateDLDeleteTask(Display_Context const *dc, Driver_DL * dl_cleanup_list) )
	{
		UNREFERENCED(dc);
		UNREFERENCED(dl_cleanup_list);
		ABSTRACT_ERROR;
	}

	H3DACTION( bool begin_scene() )
	{
		return false;
	}

	H3DACTION( void end_scene() )
	{
	}

	H3DACTION( void handle_size_change(Display_Context * dc) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dc);
	}

	H3DACTION( bool bind_texture(Net_Rendition const & nr, Texture * texture, int usage, H3DTexture * id, int texture_unit) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(nr);
		UNREFERENCED(texture);
		UNREFERENCED(usage);
		UNREFERENCED(id);
		UNREFERENCED(texture_unit);
		return false;
	}

	H3DACTION( void create_region (Net_Rendition const &  nr,
			IntRectangle const * area, void **image, void **z) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(nr);
		UNREFERENCED(area);
		UNREFERENCED(image);
		UNREFERENCED(z);
	}

	H3DACTION( void save_region (Net_Rendition const &  nr,
			IntRectangle const * area, void * image, void * z) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(nr);
		UNREFERENCED(area);
		UNREFERENCED(image);
		UNREFERENCED(z);
	}

	H3DACTION(void restore_region(Net_Rendition const &  nr, IntRectangle const * area, void * image, void * z));
	
	H3DACTION( void destroy_region (Display_Context * dc,
			void * image, void * z) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(dc);
		UNREFERENCED(image);
		UNREFERENCED(z);
	}

	H3DACTION( void save_targets( RenderTargets *saved ) )	{
		ABSTRACT_ERROR;
		UNREFERENCED(saved);
	}
	H3DACTION( void restore_targets( RenderTargets *saved,  bool release = true) )	{
		ABSTRACT_ERROR;
		UNREFERENCED(saved);
		UNREFERENCED(release);
	}

	H3DACTION( void set_target (
		int					index,
		H3DRenderTarget *	target)) {
		ABSTRACT_ERROR;
		UNREFERENCED(index);
		UNREFERENCED(target);
	}

	H3DACTION( void set_targets (
		H3DRenderTarget *	target0,
		H3DRenderTarget *	zbuffer = null,
		H3DRenderTarget *	target1 = null,
		H3DRenderTarget *	target2 = null,
		H3DRenderTarget *	target3 = null)) {
		ABSTRACT_ERROR;
		UNREFERENCED(target0);
		UNREFERENCED(zbuffer);
		UNREFERENCED(target1);
		UNREFERENCED(target2);
		UNREFERENCED(target3);
	}

	H3DACTION( bool test_targets(
		H3DRenderTarget *	target0,
		H3DRenderTarget *	zbuffer = null,
		H3DRenderTarget *	target1 = null,
		H3DRenderTarget *	target2 = null,
		H3DRenderTarget *	target3 = null)) {
		ABSTRACT_ERROR;
		UNREFERENCED(target0);
		UNREFERENCED(zbuffer);
		UNREFERENCED(target1);
		UNREFERENCED(target2);
		UNREFERENCED(target3);
		return false;
	};

	H3DACTION( bool get_pixels(
		H3DRenderTarget *	source,
		int					x,
		int					y,
		int					width,
		int					height,
		uint32_t	*pixels)) {
		UNREFERENCED(source);
		UNREFERENCED(x);
		UNREFERENCED(y);
		UNREFERENCED(width);
		UNREFERENCED(height);
		UNREFERENCED(pixels);
		return false;
	}

	H3DACTION ( void stretch_rect (
	   H3DRenderTarget *	in_src,
	   H3DRenderTarget *	in_dest,
	   H3DTEXTUREFILTERTYPE	filter = H3DTEXF_POINT,
	   bool					clear_alpha = false,
	   bool					flip = false,
	   bool					discard = false,
	   H3DRect const *	source_rect = null,
	   H3DRect const * dest_rect = null))
	{
		UNREFERENCED(in_src);
		UNREFERENCED(in_dest);
		UNREFERENCED(filter);
		UNREFERENCED(clear_alpha);
		UNREFERENCED(flip);
		UNREFERENCED(discard);
		UNREFERENCED(source_rect);
		UNREFERENCED(dest_rect);
		return;
	}

	H3DACTION( void set_depth_bias (float depth_bias, float slope_bias) ) {
		ABSTRACT_ERROR;
		UNREFERENCED(depth_bias);
		UNREFERENCED(slope_bias);
	}

	H3DACTION( void force_alphatesting (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_alphafunc (H3DCMPFUNC func, int ref) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(ref);
		UNREFERENCED(func);
	}

	H3DACTION( void force_anti_alias (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_blend (H3DBLEND src, H3DBLEND dest) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(src);
		UNREFERENCED(dest);
	}
	H3DACTION( void force_color_mask (bool mode, bool lock_it=false) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
		UNREFERENCED(lock_it);
	}
	H3DACTION( void force_channel_mask (H3DMASK mask) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mask);
	}
	H3DACTION( void force_culling (H3DCULL mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_depth_mask (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_depth_range_set (float zmin, float zmax) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(zmin);
		UNREFERENCED(zmax);
	}
	H3DACTION( void force_fog (Net_Rendition const & nr, bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(nr);
		UNREFERENCED(mode);
	}
	H3DACTION( void force_sampler_state (
		int texture_unit,
		H3DTEXTUREFILTERTYPE filter,
		H3DTEXTUREFILTERTYPE mip_filter,
		H3DTEXTUREADDRESS wrap) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(texture_unit);
		UNREFERENCED(filter);
		UNREFERENCED(mip_filter);
		UNREFERENCED(wrap);
	}
	H3DACTION( void force_scissor (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_scissor_set(int in_left, int in_bottom, int in_right, int in_top) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(in_left);
		UNREFERENCED(in_bottom);
		UNREFERENCED(in_right);
		UNREFERENCED(in_top);
	}
	H3DACTION( void force_shade_mode (H3DSHADEMODE mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_stencilling (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_texture (int unit, H3DTexture *texture) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(unit);
		UNREFERENCED(texture);
	}
	H3DACTION( void ensure_texture (int unit, H3DTexture *texture) ) //-V524
	{
		ABSTRACT_ERROR;
		UNREFERENCED(unit);
		UNREFERENCED(texture);
	}
	H3DACTION( void force_stencil_func(H3DCMPFUNC cmp) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(cmp);
	}
	H3DACTION( void force_stencil_mask(int val) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(val);
	}
	H3DACTION( void force_stencil_ref(int val) ) //-V524
	{
		ABSTRACT_ERROR;
		UNREFERENCED(val);
	}
	H3DACTION( void force_stencil_write_mask(unsigned int val) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(val);
	}
	H3DACTION( void force_stencil_op(H3DSTENCILOP fail, H3DSTENCILOP zfail, H3DSTENCILOP pass) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(fail);
		UNREFERENCED(zfail);
		UNREFERENCED(pass);
	}
	H3DACTION( void set_stencil_func(H3DCMPFUNC cmp, unsigned long ref, unsigned long mask, unsigned long writemask) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(cmp);
		UNREFERENCED(ref);
		UNREFERENCED(mask);
		UNREFERENCED(writemask);
	}
	H3DACTION( void force_transparency (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_zbuffering (bool mode) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(mode);
	}
	H3DACTION( void force_point_sprites (bool enable, float min_size, float max_size) )
	{
		ABSTRACT_ERROR;
		UNREFERENCED(enable);
		UNREFERENCED(min_size);
		UNREFERENCED(max_size);
	}	

	H3DACTION( void IM_Show_Modelview_Matrix(Net_Rendition const & nr, float * matrix) )
	{
		Transform_Rendition const &	tr = nr->transform_rendition;
		Camera_Handle const &		camera = tr->camera;
		float tempmatrix[4][4];

		Matrix_Single	campos = camera->positioning_matrix;
		campos->Copy(tempmatrix, false);

		/* do the handedness change and the camera xlate (0 to -5) */
		tempmatrix[0][2] *= m_h3ddata->driver_specific_flip;
		tempmatrix[1][2] *= m_h3ddata->driver_specific_flip;
		tempmatrix[2][2] *= m_h3ddata->driver_specific_flip;
		tempmatrix[3][2] *= m_h3ddata->driver_specific_flip;

		if (camera->projection == Camera::Projection_PERSPECTIVE)
			tempmatrix[3][2] += m_h3ddata->driver_specific_flip * camera->single_data->eye_distance;

		//m_h3ddata->net_modelling_is_identity = tr->net_modelling_matrix->Is_Identity();

		if (!tr->net_modelling_matrix->Is_Identity()) {
			Matrix_Single	nmm = tr->net_modelling_matrix;
			Matrix_44::Compute_Product (nmm->Elements(), tempmatrix, matrix);
		}
		else {
			/* use world_to_eye matrix instead of modelview and avoid copying */
			Matrix_44::Copy (tempmatrix, matrix);
		}
	}

	H3DACTION( void IM_Show_Projection_Matrix(Net_Rendition const & nr, float * matrix) )
	{
		Transform_Rendition const &	tr = nr->transform_rendition;
		Window_Rendition const &	wr = nr->window_rendition;
		Camera_Handle const &		camera = tr->camera;

		if (!matrix)
			return;

		float	xscale = 1.0f;
		float	yscale = 1.0f;

		Rectangle const &	screen_range = tr->screen_range;
		float	window_aspect = wr->aspect * screen_range.Height() / screen_range.Width();

		if (camera->projection != Camera::Projection_STRETCHED) {
			if (window_aspect <= camera->aspect)
				xscale = camera->aspect / window_aspect;
			else
				yscale = window_aspect / camera->aspect;
		}


		float wlimit = tr->wlimit;
		if (wlimit < 1.0e-3f)
			wlimit = 1.0e-3f;

		if (ANYBIT (tr->flags, TR_RIGHT_HANDED_WORLD))
			xscale = -xscale;

		if (camera->projection == Camera::Projection_PERSPECTIVE) {
			float zn = wlimit * camera->single_data->eye_distance;
	
			// Look 'far into the distance'
			float	zf = camera->single_data->eye_distance + (camera->single_data->eye_distance)*1000.0f;

			H3DMatrixPerspectiveLH(
				matrix,
				(camera->single_data->field_width*xscale)*wlimit,
				(camera->single_data->field_height*yscale)*wlimit,
				zn, zf,
				m_h3ddata->driver_specific_flip
			);

			m_h3ddata->zfar = zf;
			m_h3ddata->znear = zn;
		}
		else {
			/* stretched and orthographic */
			float zn = -camera->single_data->eye_distance;
			float zf = camera->single_data->eye_distance;

			/* For shadow maps, we know the frustum bounds the scene perfectly, so
			   don't expand it or we will lose depth resolution. */
			if ( !m_h3ddata->shadow_maps.mode ) {
				zn *= tr->wlimit_inverse;
				zf *= tr->wlimit_inverse;
			}
			else {
				/* Force right-handed light eye space here to avoid dealing with it in shadowmap setup. */
				xscale = yscale = 1;
			}

			m_h3ddata->zfar = zf;
			m_h3ddata->znear = zn;

			H3DMatrixOrthoLH(
				matrix,
				camera->single_data->field_width*xscale,
				camera->single_data->field_height*yscale,
				zn, zf,
				m_h3ddata->driver_specific_flip);
		}
	}

	H3DACTION( void * IM_Get_Surface(Net_Rendition const &) )
	{
		ABSTRACT_ERROR;
		return null;
	}
	
	H3DACTION( void IM_Draw_Textured_Background(void *, float const *, IntRectangle const *))
	{
		ABSTRACT_ERROR;
	}	

	virtual H3DTexture * ensure_gpu_res_texture(Net_Rendition const & nr);

	// Must match version in intermediate.cpp
	struct Image_Block
	{		
		void const * image_block;
		int width;
		int height;		
		int rowbytes;
		Image_Format format;		
	};

	void DrawImageBlock(Net_Rendition const & nr, Point2D const * in_start, HOOPS::Image_Format color_format, H3DTexture * color, H3DTexture * depth)
	{
		UNREFERENCED(color_format);		// unreferenced on non-windows

		m_h3ddata->cache->color_mask_locked = false;
		ENSURE_COLOR_MASK (m_h3ddata, true);
		ENSURE_DEPTH_MASK (m_h3ddata, true);
		ENSURE_ZBUFFERING (m_h3ddata, true);
		ENSURE_CULLING (m_h3ddata, H3DCULL_NONE);
		ENSURE_SCISSOR (m_h3ddata, false);
		ENSURE_TRANSPARENCY(m_h3ddata, true);
		ENSURE_ALPHA_TEST(m_h3ddata, false);				
				
		Point2D start = *in_start;

		start.x *= m_h3ddata->ssaa_width_scale;
		start.y *= m_h3ddata->ssaa_height_scale;

		// this will include ssaa scale
		int const dst_w = m_h3ddata->render_target->Width();
		int const dst_h = m_h3ddata->render_target->Height();
		
		ENSURE_DEPTH_RANGE_SET(m_h3ddata, 0.0f, 1.0f);
		ENSURE_VIEWPORT(m_h3ddata, 0, 0, dst_w / m_h3ddata->ssaa_width_scale, dst_h / m_h3ddata->ssaa_height_scale);

		// These could be collapsed into one matrix but I'm leaving it like this for clarity (and this is not a performance critical path). 
		// We're mapping a quad with coords from -1,1 onto a rectangle specified in pixels.
			
		float trans1[16];
		Matrix_44::Init_Translation(1.0f, 1.0, 0.0f, trans1);

		float scale1[16];
		Matrix_44::Init_Scale(0.5f, 0.5, 0.0f, scale1);

		float trans2[16];
		Matrix_44::Init_Translation(start.x, dst_h - start.y, 0.0f, trans2);	

		float scale2[16];
		Matrix_44::Init_Scale((float)color->Width() * m_h3ddata->ssaa_width_scale, (float)color->Height() * m_h3ddata->ssaa_height_scale, 0.0f, scale2);

		float proj[16];
		H3DMatrixOrthoOffCenterRH(proj, 0.0f, (float)dst_w, 0.0f, (float)dst_h, 0.0f, 1.0f);

		float matrix[16];
		Matrix_44::Compute_Product(trans1, scale1, matrix);
		Matrix_44::Compute_Product(matrix, scale2, matrix);
		Matrix_44::Compute_Product(matrix, trans2, matrix);
		Matrix_44::Compute_Product(matrix, proj, matrix);

		if (m_h3ddata->driver_shader_type == H3D_Driver_Shader_Type_DX9 || m_h3ddata->driver_shader_type == H3D_Driver_Shader_Type_DX11)
			Matrix_44::Compute_Transpose(matrix, matrix);
		
		H3DPost::Shader shader = H3DPost::SH_IMAGE_BLOCK;
#ifdef _MSC_VER
		if (color_format == Image_RGBAS32)
			shader = H3DPost::SH_IMAGE_BLOCK_BGRA;
#endif

		H3DConstantTable * constants = m_h3ddata->post->ActivateShader(shader, 0.0f, matrix, null);
		if (constants)
		{	
			unsigned long const color_sampler = constants->GetSamplerIndex("Color");
			ENSURE_TEXTURE(m_h3ddata, color_sampler, color);
			ENSURE_SAMPLER_STATE(m_h3ddata, color_sampler, H3DTEXF_POINT, H3DTEXF_NONE, H3DTADDRESS_CLAMP);

			unsigned long const depth_sampler = constants->GetSamplerIndex("Depth");
			ENSURE_TEXTURE(m_h3ddata, depth_sampler, depth);
			ENSURE_SAMPLER_STATE(m_h3ddata, depth_sampler, H3DTEXF_POINT, H3DTEXF_NONE, H3DTADDRESS_CLAMP);

			Transform_Rendition const & tr = nr->transform_rendition;
			constants->SetVector("DepthRange", tr->z_range, 2);

			m_h3ddata->post->Draw(H3DPost::VS_3D, shader);

			ENSURE_TEXTURE(m_h3ddata, color_sampler, null);
			ENSURE_TEXTURE(m_h3ddata, depth_sampler, null);
		}

		if (m_h3ddata->cache->Shader)
			m_h3ddata->cache->Shader->Activate();
	}

	H3DACTION(bool IM_Draw_Image_Block_Z(Net_Rendition const & nr, Image_Block const * c, Image_Block const * z, Point2D const * start))
	{
		Image_Block const * color_params = static_cast<Image_Block const*>(c);
		Image_Block const * z_params = static_cast<Image_Block const *>(z);

		H3DTexture * color = m_h3ddata->post->GetWorkTexture(color_params->width, color_params->height, H3DFMT_A8R8G8B8, H3DTEXUSAGE_DYNAMIC);
		H3DTexture * depth = m_h3ddata->post->GetWorkTexture(z_params->width, z_params->height, H3DFMT_R32F, H3DTEXUSAGE_DYNAMIC);

		color->SetData(0, color_params->image_block, color_params->width, color_params->height, color_params->rowbytes);
		depth->SetData(0, z_params->image_block, z_params->width, z_params->height, z_params->rowbytes);

		DrawImageBlock(nr, start, color_params->format, color, depth);

		m_h3ddata->post->ReleaseWorkTexture(&color);
		m_h3ddata->post->ReleaseWorkTexture(&depth);

		return true;		
	}

#undef H3DACTION

	void destroy_glyph_texture_cache(H3DData *h3ddata)
	{
		while (h3ddata->glyph_textures) {
			// Unlink
			Glyph_Texture *remove = h3ddata->glyph_textures;
			h3ddata->glyph_textures = h3ddata->glyph_textures->next;

			// Clean up
			H_SAFE_DELETE(remove->texture);
			FREE(remove, Glyph_Texture);
		}
	}

	void common_device_cleanup(Display_Context * dc)
	{
		H3DData *h3ddata = H3DD(dc);
	
		/* Force re-set-up of antialiasing mode */
		h3ddata->antialias_mode = UNKNOWN_ANTIALIASING;
		h3ddata->antialias_samples = -1;
		h3ddata->requested_antialias_samples = -1;

		// Clean up caches
		for (int i = 0; i < 256; ++i) {
			if (h3ddata->vb_cache[i] != null) {
				for (auto it = h3ddata->vb_cache[i]->cbegin(), end = h3ddata->vb_cache[i]->cend(); it != end; ++it)
					delete *it;
				//delete h3ddata->vb_cache[i];
				h3ddata->vb_cache[i] = null;
			}
		}

		if (h3ddata->colormap_hash) {
			h3ddata->colormap_hash->DeleteItems();
			H_SAFE_DELETE(h3ddata->colormap_hash);
		}

		destroy_glyph_texture_cache(h3ddata);

		H_SAFE_DELETE(h3ddata->ShaderHash);
		if (h3ddata->cache)
			h3ddata->cache->Shader = null;

		if (h3ddata->grayscale_colormap_hash) {
			h3ddata->grayscale_colormap_hash->DeleteItems();
			H_SAFE_DELETE(h3ddata->grayscale_colormap_hash);
		}

		H_SAFE_DELETE(h3ddata->binhash);
		H_SAFE_DELETE(h3ddata->ib_cache);
		H_SAFE_DELETE(h3ddata->vb_generic);
		H_SAFE_DELETE(h3ddata->ib_generic);

		if (h3ddata->short_pointmap)
			FREE_ARRAY(h3ddata->short_pointmap, h3ddata->short_pointmap_allocated, short);
		h3ddata->short_pointmap = null;
		h3ddata->short_pointmap_allocated = 0;

		if (h3ddata->int_pointmap_allocated)
			FREE_ARRAY(h3ddata->int_pointmap, h3ddata->int_pointmap_allocated, int);
		h3ddata->int_pointmap = null;
		h3ddata->int_pointmap_allocated = 0;

		if (h3ddata->tangents) {
			FREE_ARRAY(h3ddata->tangents, h3ddata->tangents_allocated, Plane);
			FREE_ARRAY(h3ddata->bitangents, h3ddata->tangents_allocated, Vector);
		}
		h3ddata->tangents = null;
		h3ddata->bitangents = null;
		h3ddata->tangents_allocated = 0;

		if (h3ddata->cull_results_scratch) 
			FREE_ARRAY (h3ddata->cull_results_scratch, h3ddata->cull_results_scratch_length, Test);
		h3ddata->cull_results_scratch = null;
		h3ddata->cull_results_scratch_length = 0;

		H_SAFE_DELETE(h3ddata->line_tools.texture);
		h3ddata->line_tools.valid = false;

		H_SAFE_DELETE(h3ddata->glyphs.circle_texture);
		h3ddata->glyphs.valid = false;

		if (h3ddata->line_tools.data) {
			FREE_ARRAY(h3ddata->line_tools.data, 4*h3ddata->line_tools.data_size, unsigned char);
			h3ddata->line_tools.data = null;
			h3ddata->line_tools.data_size = 0;
		}

		for (int i = 0; i < NUM_FACE_PATTERNS; i++)
			H_SAFE_DELETE(h3ddata->face_pattern_texture[i]);

		h3ddata->face_stipple = 0;
		H_SAFE_DELETE(h3ddata->face_stipple_user_texture);
		

		H_SAFE_DELETE(h3ddata->lpatterns);

		h3ddata->lpatternhash = null;
		h3ddata->lpattern_count = 0;

		H_SAFE_DELETE(h3ddata->shadow_maps.depth_buffer);
		H_SAFE_DELETE(h3ddata->shadow_maps.dummy_buffer);
		H_SAFE_DELETE(h3ddata->shadow_maps.jitter);
		h3ddata->shadow_maps.resolution = 0;

		H_SAFE_DELETE(h3ddata->selection_maps.depth_buffer);
		h3ddata->selection_maps.width = 0;
		h3ddata->selection_maps.height = 0;

		H_SAFE_DELETE(h3ddata->depth_effects.depth_measure_target);
		H_SAFE_DELETE(h3ddata->depth_effects.depth_measure_pattern);

		/* Clean up any temporaries we allocated */
		for (int i = 0 ; i < DEPTH_PEELING_MAX_LAYERS ; i++) {
			H_SAFE_DELETE(h3ddata->depth_peeling.z_textures[i]);
			H_SAFE_DELETE(h3ddata->depth_peeling.rgba_textures[i]);
		}

		if (h3ddata->priority_include_list) {
			auto & list = h3ddata->priority_include_list;
			for (auto it = list->cbegin(), end = list->cend(); it != end; ++it) {
				PriorityInclude *	item = *it;
				H_SAFE_DELETE(item->include_target);
				H_SAFE_DELETE(item->include_depth);
				FREE(item, PriorityInclude);
			}

			//delete h3ddata->priority_include_list;
			h3ddata->priority_include_list = null;
		}
	}


	void SetDepthTexture(H3DRenderTarget *target)
	{
		H3DData *h3ddata = m_h3ddata;
		if (h3ddata->ssaa_line_target.mode) {
			set_target(2, target);
		}
		else
			set_target(1, target);
	}

	void set_all_targets (H3DRenderTarget *target0, H3DRenderTarget *zbuffer = null)
	{
		// Intelligently pick what rendertargets go where
		H3DData *h3ddata = m_h3ddata;
		H3DRenderTarget *target1 = null;

		// See if we have the ssaa line target
		if (h3ddata->ssaa_line_target.mode) {
			target1 = h3ddata->ssaa_line_target.line_texture;
		}

		set_targets(target0, zbuffer, target1, null, null);
	}	
};

#define FORCE_SCISSOR_SET(h3ddata,in_left,in_bottom,in_right,in_top) \
	(h3ddata)->h3d_actions->force_scissor_set(in_left,in_bottom,in_right,in_top);

INLINE void ENSURE_SCISSOR_SET(H3DData * h3ddata, int in_left, int in_bottom, int in_right, int in_top)
{
	if (in_left != h3ddata->cache->scissor_rect.left || 
		in_bottom != h3ddata->cache->scissor_rect.bottom || 
		in_right != h3ddata->cache->scissor_rect.right || 
		in_top != h3ddata->cache->scissor_rect.top) { 
			FORCE_SCISSOR_SET(h3ddata,in_left,in_bottom,in_right,in_top); 
	}
}


/*****************************************************************************
*****************************************************************************
					Category: Debug trace support

*****************************************************************************
*****************************************************************************/

#ifdef DEBUG

// Helper: Trace on scope entry and exit. Use the H3D_TRACE macro below.
class AutoTrace
{
public:
	AutoTrace( H3DData *h3ddata, wchar_t const *label ) : m_h3ddata(h3ddata), m_label(label)
	{
		if ( m_h3ddata && m_h3ddata->h3d_actions )
			m_h3ddata->h3d_actions->Begin_Trace(label);
	}

	~AutoTrace()
	{
		if ( m_h3ddata && m_h3ddata->h3d_actions )
			m_h3ddata->h3d_actions->End_Trace(m_label);
	}

private:

	H3DData *m_h3ddata;
	const wchar_t *m_label;
};


#define H3D_TRACE(h3ddata, string) AutoTrace trace##__LINE__((h3ddata), (L##string));
#else
#define H3D_TRACE(h3ddata, string) UNREFERENCED(h3ddata)
#endif

/*****************************************************************************
*****************************************************************************
						Category: concave_tri
*****************************************************************************
*****************************************************************************/

struct concave_tri
{
	H3DData *h3ddata;
	bool doing_dc;
	union {
		DC_Point const *dc;
		Point const *threed;
	} points;
	union {
		H3DVertexBuffer *vb_dc;				//hd3d_pt_clr*	vb_dc;
		H3DVertexBuffer *vb_threed;			//hd3d_pt*		vb_threed;
	};
	float normal[3];
	Driver_Color color;
	int triangle_count;
	int buffer_triangle_count;
	void (*draw_triangle) (void * info,
						   int32_t convex_triangulation,
						   Point const * p1,
						   Point const * p2,
						   Point const * p3,
						   int32_t v1,
						   int32_t v2,
						   int32_t v3,
						   Intersection const * i1,
						   Intersection const * i2,
						   Intersection const * i3);
};

/*****************************************************************************
*****************************************************************************
						Category: Geometry Contents
*****************************************************************************
*****************************************************************************/

//geometry content bits
#define	GEO_VERTEX_INDICES			0x00000001
#define	GEO_FACE_INDICES			0x00000002
#define	GEO_VERTEX_COLORS			0x00000004
#define	GEO_VERTEX_RGBAS			0x00000008
#define	GEO_VERTEX_FINDICES			0x00000010
#define	GEO_VERTEX_NORMALS			0x00000020
#define	GEO_VERTEX_PARAMS			0x00000040
#define	GEO_VERTEX_PARAM_WIDTH		0x00000080
#define	GEO_FACE_COLORS				0x00000100
#define	GEO_FACE_NORMALS			0x00000200
#define	GEO_INDEXED_TRISTRIP		0x00000400
#define	GEO_EDGE_NORMALS			0x00000800
#define	GEO_EDGE_COLORS				0x00001000
#define	GEO_VERTEX_DOUBLE_POSITIONS	0x00002000
#define GEO_CURVES					0x00004000


/*****************************************************************************
*****************************************************************************
						Category: Texture Usage methods
*****************************************************************************
*****************************************************************************/
#define TU_SPECIAL						-1		//used only to offset the usage calculations
#define TU_INVALID						0
#define TU_DIFFUSE_TEXTURE				1
#define TU_SPECULAR_TEXTURE				2
#define TU_ENVIRONMENT_TEXTURE			3
#define TU_SHADOW_MAP_TEXTURE			4
#define TU_CUTTING_PLANE				5
#define TU_FACE_PATTERN					6
#define TU_DEPTH_PEELING_TEXTURE		7
#define TU_INDEX_TEXTURE				8
#define TU_SHADOW_MAP_JITTER			9
#define TU_PER_PIXEL_EYE_POSITION		10
#define TU_BUMP_TEXTURE					11
#define TU_TANGENT_COORD				12
#define TU_MARKER						13
#define TU_GOOCH						14
#define TU_LINE_PATTERN					15		//for line patterns
#define TU_WIDE_LINE_NEXT_POINT			16
#define TU_WIDE_LINE_OFFSET				17
#define TU_MITERED_LINE					18
#define TU_LINE_TOOLS					19		//for round or AA lines
#define TU_PRIORITY_HSRA				20

#define TU_BACK_DIFFUSE_TEXTURE			21
#define TU_MIRROR_TEXTURE				22
#define TU_TRANSMISSION_TEXTURE			23
#define TU_EMISSION_TEXTURE				24

#define TU_SELECTION_BITMAP				25
#define TU_SELECTION_TABLE				26

#define TU_GS_ELLIPTICAL_ARC			26

#define TU_COUNT						27

#define TU_GET_UNIT_USAGE			0xffff

struct Tex_Usage_Counts {
	int	total;
	bool diffuse_shared_coords;
	char index_texture;
	char line_texture;
	char mitered_line;
	char wide_line;
	char line_tools;
	char diffuse;
	char back_diffuse;
	char specular;
	char bump;
	char mirror;
	char transmission;
	char emission;
	char environment;
	char shadowmap;
	char cuttingplane;
	char facepattern;
	char depthpeeling;
	char shadowmapjitter;
	char perpixeleyeposition;
	char marker;
	char gooch;
	char priority_hsra;
	char tangent;
	char selection_bitmap;
	char selection_table;
	char gsellipticalarc;

	int calculate_total() const {
		int new_total = 0;
		new_total += index_texture;
		new_total += line_texture;
		new_total += mitered_line;
		new_total += wide_line;
		new_total += line_tools;
		new_total += diffuse;
		new_total += back_diffuse;
		new_total += specular;
		new_total += bump;
		new_total += mirror;
		new_total += transmission;
		new_total += emission;
		new_total += environment;
		new_total += shadowmap;
		new_total += cuttingplane;
		new_total += facepattern;
		new_total += depthpeeling;
		new_total += shadowmapjitter;
		new_total += perpixeleyeposition;
		new_total += marker;
		new_total += gooch;
		new_total += priority_hsra;
		new_total += tangent;
		new_total += selection_bitmap;
		new_total += selection_table;
		new_total += gsellipticalarc;
		return new_total;
	}
};


class TristripRenderControl {
  public:
	Cutting_Plane_Set const *	cut_set;
	Tex_Usage_Counts			tex_counts;
	H3DVertexFormat				vf;
	H3D_Vertex_Buffer_Style		buffer_style;
	unsigned int				shader_flags;
	Strip_Flags					special_flags;
	Strip_Flags2				special_flags2;
	bool						is_transparent;
	H3DCULL						front_cull;
	H3DCULL						back_cull;
	int							sampler_unit;

	TristripRenderControl() {
		SET_MEMORY (this, sizeof(TristripRenderControl), 0);
	}
	~TristripRenderControl() {
		//nothing to do.  we don't own any of our pointers
	}
};

/* this table should be just 520 bytes, so it shouldn't be much of a size issue */
static const unsigned char transparency_stipple_patterns[65][8] = {
	{0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
	{0x0, 0x80, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0},
	{0x0, 0x80, 0x0, 0x0, 0x0, 0x8, 0x0, 0x0},
	{0x0, 0x80, 0x0, 0x0, 0x0, 0x88, 0x0, 0x0},
	{0x0, 0x88, 0x0, 0x0, 0x0, 0x88, 0x0, 0x0},
	{0x0, 0x88, 0x0, 0x20, 0x0, 0x88, 0x0, 0x0},
	{0x0, 0x88, 0x0, 0x20, 0x0, 0x88, 0x0, 0x2},
	{0x0, 0x88, 0x0, 0x20, 0x0, 0x88, 0x0, 0x22},
	{0x0, 0x88, 0x0, 0x22, 0x0, 0x88, 0x0, 0x22},
	{0x0, 0x88, 0x0, 0xa2, 0x0, 0x88, 0x0, 0x22},
	{0x0, 0x88, 0x0, 0xa2, 0x0, 0x88, 0x0, 0x2a},
	{0x0, 0x88, 0x0, 0xa2, 0x0, 0x88, 0x0, 0xaa},
	{0x0, 0x88, 0x0, 0xaa, 0x0, 0x88, 0x0, 0xaa},
	{0x0, 0xa8, 0x0, 0xaa, 0x0, 0x88, 0x0, 0xaa},
	{0x0, 0xa8, 0x0, 0xaa, 0x0, 0x8a, 0x0, 0xaa},
	{0x0, 0xa8, 0x0, 0xaa, 0x0, 0xaa, 0x0, 0xaa},
	{0x0, 0xaa, 0x0, 0xaa, 0x0, 0xaa, 0x0, 0xaa},
	{0x40, 0xaa, 0x0, 0xaa, 0x0, 0xaa, 0x0, 0xaa},
	{0x40, 0xaa, 0x0, 0xaa, 0x4, 0xaa, 0x0, 0xaa},
	{0x40, 0xaa, 0x0, 0xaa, 0x44, 0xaa, 0x0, 0xaa},
	{0x44, 0xaa, 0x0, 0xaa, 0x44, 0xaa, 0x0, 0xaa},
	{0x44, 0xaa, 0x10, 0xaa, 0x44, 0xaa, 0x0, 0xaa},
	{0x44, 0xaa, 0x10, 0xaa, 0x44, 0xaa, 0x1, 0xaa},
	{0x44, 0xaa, 0x10, 0xaa, 0x44, 0xaa, 0x11, 0xaa},
	{0x44, 0xaa, 0x11, 0xaa, 0x44, 0xaa, 0x11, 0xaa},
	{0x44, 0xaa, 0x51, 0xaa, 0x44, 0xaa, 0x11, 0xaa},
	{0x44, 0xaa, 0x51, 0xaa, 0x44, 0xaa, 0x15, 0xaa},
	{0x44, 0xaa, 0x51, 0xaa, 0x44, 0xaa, 0x55, 0xaa},
	{0x44, 0xaa, 0x55, 0xaa, 0x44, 0xaa, 0x55, 0xaa},
	{0x54, 0xaa, 0x55, 0xaa, 0x44, 0xaa, 0x55, 0xaa},
	{0x54, 0xaa, 0x55, 0xaa, 0x45, 0xaa, 0x55, 0xaa},
	{0x54, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa},
	{0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa},
	{0xd5, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa},
	{0xd5, 0xaa, 0x55, 0xaa, 0x5d, 0xaa, 0x55, 0xaa},
	{0xd5, 0xaa, 0x55, 0xaa, 0xdd, 0xaa, 0x55, 0xaa},
	{0xdd, 0xaa, 0x55, 0xaa, 0xdd, 0xaa, 0x55, 0xaa},
	{0xdd, 0xaa, 0x75, 0xaa, 0xdd, 0xaa, 0x55, 0xaa},
	{0xdd, 0xaa, 0x75, 0xaa, 0xdd, 0xaa, 0x57, 0xaa},
	{0xdd, 0xaa, 0x75, 0xaa, 0xdd, 0xaa, 0x77, 0xaa},
	{0xdd, 0xaa, 0x77, 0xaa, 0xdd, 0xaa, 0x77, 0xaa},
	{0xdd, 0xaa, 0xf7, 0xaa, 0xdd, 0xaa, 0x77, 0xaa},
	{0xdd, 0xaa, 0xf7, 0xaa, 0xdd, 0xaa, 0x7f, 0xaa},
	{0xdd, 0xaa, 0xf7, 0xaa, 0xdd, 0xaa, 0xff, 0xaa},
	{0xdd, 0xaa, 0xff, 0xaa, 0xdd, 0xaa, 0xff, 0xaa},
	{0xfd, 0xaa, 0xff, 0xaa, 0xdd, 0xaa, 0xff, 0xaa},
	{0xfd, 0xaa, 0xff, 0xaa, 0xdf, 0xaa, 0xff, 0xaa},
	{0xfd, 0xaa, 0xff, 0xaa, 0xff, 0xaa, 0xff, 0xaa},
	{0xff, 0xaa, 0xff, 0xaa, 0xff, 0xaa, 0xff, 0xaa},
	{0xff, 0xea, 0xff, 0xaa, 0xff, 0xaa, 0xff, 0xaa},
	{0xff, 0xea, 0xff, 0xaa, 0xff, 0xae, 0xff, 0xaa},
	{0xff, 0xea, 0xff, 0xaa, 0xff, 0xee, 0xff, 0xaa},
	{0xff, 0xee, 0xff, 0xaa, 0xff, 0xee, 0xff, 0xaa},
	{0xff, 0xee, 0xff, 0xba, 0xff, 0xee, 0xff, 0xaa},
	{0xff, 0xee, 0xff, 0xba, 0xff, 0xee, 0xff, 0xab},
	{0xff, 0xee, 0xff, 0xba, 0xff, 0xee, 0xff, 0xbb},
	{0xff, 0xee, 0xff, 0xbb, 0xff, 0xee, 0xff, 0xbb},
	{0xff, 0xee, 0xff, 0xfb, 0xff, 0xee, 0xff, 0xbb},
	{0xff, 0xee, 0xff, 0xfb, 0xff, 0xee, 0xff, 0xbf},
	{0xff, 0xee, 0xff, 0xfb, 0xff, 0xee, 0xff, 0xff},
	{0xff, 0xee, 0xff, 0xff, 0xff, 0xee, 0xff, 0xff},
	{0xff, 0xfe, 0xff, 0xff, 0xff, 0xee, 0xff, 0xff},
	{0xff, 0xfe, 0xff, 0xff, 0xff, 0xef, 0xff, 0xff},
	{0xff, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff},
	{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}
};


/*****************************************************************************
*****************************************************************************
						Category: Misc Helper Functions
*****************************************************************************
*****************************************************************************/
int HOOPS_API prim_to_indices(H3DPRIMITIVETYPE type, int primitive_count);

INLINE bool face_contours_enabled (Misc_Rendition const &mr);

template <typename F>
bool transform_plane_world_to_eye(Net_Rendition const &nr, Plane_3D<F> const *in, Plane *out);

INLINE static void pack_rgba(float value, float *result)
{
	static const float RADIX = 255.0f;
	static const float shift[4] = { 1.0f, RADIX, RADIX*RADIX, RADIX*RADIX*RADIX };
	static const float mask[4] = { 1.0f / RADIX, 1.0f / RADIX, 1.0f / RADIX, 0.0 };

	float packed_rgba[4] = { value * shift[0], value * shift[1], value * shift[2], value * shift[3] };
	packed_rgba[0] = packed_rgba[0] - floor(packed_rgba[0]);
	packed_rgba[1] = packed_rgba[1] - floor(packed_rgba[1]);
	packed_rgba[2] = packed_rgba[2] - floor(packed_rgba[2]);
	packed_rgba[3] = packed_rgba[3] - floor(packed_rgba[3]);

	result[0] = packed_rgba[0] - mask[0] * packed_rgba[1]; //-V525
	result[1] = packed_rgba[1] - mask[1] * packed_rgba[2];
	result[2] = packed_rgba[2] - mask[2] * packed_rgba[3];
	result[3] = packed_rgba[3] - mask[3] * packed_rgba[3];
}

INLINE static float unpack_rgba(float *packed)
{
	static const float RADIX = 255.0f;
	static const float shift[] = { 1.0f, 1.0f / RADIX, 1.0f / (RADIX*RADIX), 1.0f / (RADIX*RADIX*RADIX) };
	float dot = packed[0] * shift[0] + packed[1] * shift[1] + packed[2] * shift[2] + packed[3] * shift[3];
	return dot;
}

/**
 * HELPER
 *
 * Pick between opaque and transparent cutting planes, check against driver limits, and decide
 * whether or not this primitive should be punted.
 *
 * If the function returns false, punt the primitive (usually one of the HD_Clip_xxx functions).
 * If the function returns true, use the cut_set in the IO parameter - this could be null if
 * there are no planes to use.
 */
bool get_cutting_plane_set(
		Net_Rendition const & nr,
		Cutting_Plane_Set const *& cut_set);


INLINE void invalidate_h3dcache (H3DData * h3ddata)
{
	if (h3ddata->cache)
		SET_MEMORY (h3ddata->cache, sizeof(H3DCache), (unsigned char)(INVALID_CACHE & 0xff));
}

/*
 * HELPER
 * Add primitive and count to appropriate DC stats.
 */
void add_primitives_to_stats (
		H3DData const *h3ddata,
		int count,
		int Mode,
		H3DPRIMITIVETYPE Type,
		int degenerate_triangles);

/*
 * HELPER
 * Handle the little drawing loop for cutting planes with vertex buffers.
 * Saves repeating the same ten lines of code all over.
 */
void draw_primitives(
		H3DData *h3ddata,
		Net_Rendition const & nr,
		H3DVertexBufferCache *vertex_buffer,
		Cutting_Plane_Set const *cut_set,
		int prim_count,
		int Mode,
		H3DPRIMITIVETYPE Type,
		int degenerate_triangles = 0);

/* Handle the little drawing loop for cutting planes with index buffers.
 * Saves repeating the same ten lines of code all over.
 */
void draw_indexed_primitives(
		H3DData *dx9data,
		Net_Rendition const & nr,
		H3DIndexBufferCache *index_buffer,
		Cutting_Plane_Set const *cut_set,
		int vertex_count,
		int prim_count,
		int Mode,
		H3DPRIMITIVETYPE Type,
		int degenerate_triangles = 0,
		bool two_sided_coloring = false);

H3DTexture *create_jitter (H3DData *h3ddata, int w, int h);

void HOOPS_API apply_ssao(H3DData *h3ddata, H3DRenderTarget *target, H3DTexture *depth_texture, bool opaque, bool scaled);
void HOOPS_API filter_ssao(H3DData *h3ddata, H3DTexture *ping, H3DTexture *pong);
void HOOPS_API choose_depth_effects(H3DData * h3ddata);
bool HOOPS_API init_depth_effects( H3DData *h3ddata, int update_slice );
void HOOPS_API finish_depth_effects(Net_Rendition const &nr, H3DTexture * dest);
void HOOPS_API postprocess_picture(Net_Rendition const &nr);
bool HOOPS_API init_ssaa_lines(H3DData *h3ddata);
void HOOPS_API finish_ssaa_lines(H3DData *h3ddata);
void HOOPS_API start_inclusion(Net_Rendition const & nr, int32_t priority);
void HOOPS_API finish_inclusion(Net_Rendition const & nr);


void set_pe_vertex_data (
	Net_Rendition const &		nr,
	Polyedge const *			pe,
	Line_Rendition const &		er,
	H3DVertexBuffer	*			vb,
	float						current_screen_length,
	int							i_pt,
	int							i_edge,
	Matrix const &				matrix,
	RGB const *					gref_color,
	Tex_Usage_Counts const &	tex_counts);

/* ensures that our D3D state has the correct settings for
 *	a) two sided lighting,
 *	b) cull face
 */
void set_handedness (
	H3DData *					h3ddata,
	Transform_Rendition const &	tr,
	bool						backface_pass);


/* Apply shader settings to the specified shader.*/
void apply_shader_settings(
	H3DData *				h3ddata,
	Net_Rendition const &	nr,
	H3DShader *				shader);

void push_modelling_matrix (
	H3DData *		h3ddata,
	Matrix const &	mat,
	H3DShader *		shader);

void pop_modelling_matrix (
	H3DData *		h3ddata,
	H3DShader *		shader);


/* Compare textures to see if they can share coordinates */
bool textures_can_share_coords (Net_Rendition const & nr, Texture const * txr1, Texture const * txr2);

const char HOOPS_API * get_shader_directory();

int nearest_texture_size (H3DData * h3ddata, int in_size);

/*****************************************************************************
*****************************************************************************
						Category: Textures
*****************************************************************************
*****************************************************************************/
H3DTexture * define_texture (
	H3DData *				h3ddata,
	Net_Rendition const &	nr,
	Texture *				texture,
	int						usage);


/*****************************************************************************
*****************************************************************************
					Category: Triangulation Helpers
*****************************************************************************
*****************************************************************************/
/*
 * HELPER
 */
void callback_collect_triangles (
	void *					vp,
	int						convex_triangulation,
	Point const *			p1,
	Point const *			p2,
	Point const *			p3,
	int						s1,
	int						s2,
	int						s3,
	Intersection const *	i1,
	Intersection const *	i2,
	Intersection const *	i3);


/* TODO: Fix assumption is wrong for many cases
 * Note: this was written with the assumption that triangulation is expensive and that
 * it is preferable to do many color changes versus doing multiple triangulations.	This
 * assumption may be invalid.  SAVE triangles for patterned stuff!
 */
void draw_concave_face (Net_Rendition const & nr, int count, struct concave_tri const * ct);

/* given a clip region defined in dc, draw everywhere else */
void everything_but_region (Net_Rendition const & nr,int count, struct concave_tri * ct);

void draw_concave_polygon (Net_Rendition const & nr, Polygon const * polygon);


/*****************************************************************************
*****************************************************************************
					Category: Set Xforms
*****************************************************************************
*****************************************************************************/
void set_dc_xform (Net_Rendition const & nr);
void compute_3d_xform_extent(Net_Rendition const & nr, IntRectangle & out_extent, bool & out_viewport_fix);
void set_3d_xform(Net_Rendition const & nr, IntRectangle const extent, bool const viewport_fix);
void set_3d_xform_viewport(Net_Rendition const & nr, IntRectangle const & extent, bool const viewport_fix);

INLINE void ENSURE_DC_XFORM(H3DData * h3ddata, Net_Rendition const & nr, H3DShader * shader)
{
	Transform_Rendition const &	tr = nr->transform_rendition;
	if (h3ddata->tr_incarnation != tr->incarnation || h3ddata->tr_is_dc == 0) {
		set_dc_xform (nr);
		h3ddata->tr_incarnation = tr->incarnation;
		h3ddata->tr_is_dc = 1;
	}
	shader->EnsureDCTransform(nr);
	if (h3ddata->depth_peeling.mode) {
		shader->EnsureDepthPeeling (h3ddata->depth_peeling.peel_z_texture, h3ddata);
	}
}

/*
 * Transformations
 * computes transformation matrices.  Some state goes on the device (e.g.
 * culling modes, viewport).  Other state goes in the shaders' constant tables
 */
INLINE void ENSURE_3D_XFORM(H3DData * h3ddata, Net_Rendition const & nr, H3DShader * shader)
{
	IntRectangle			extent;
	bool					viewport_fix = false;
	compute_3d_xform_extent(nr, extent, viewport_fix);

	Transform_Rendition const &	tr = nr->transform_rendition;
	if (h3ddata->tr_incarnation != tr->incarnation || h3ddata->tr_is_dc == 1) {
		set_3d_xform(nr, extent, viewport_fix);
		h3ddata->tr_incarnation = tr->incarnation;
		h3ddata->tr_is_dc = 0;
	}

	set_3d_xform_viewport(nr, extent, viewport_fix);

	if (shader) {
		shader->Ensure3DTransform(nr);

		if (h3ddata->depth_peeling.mode) {
			shader->EnsureDepthPeeling(h3ddata->depth_peeling.peel_z_texture, h3ddata);
		}
	}
}


/*****************************************************************************
*****************************************************************************
					Category: Post functions
*****************************************************************************
*****************************************************************************/
void HOOPS_API apply_post_effects( H3DData *h3ddata, H3DTexture *scene, H3DRenderTarget *backbuffer );

/*****************************************************************************
*****************************************************************************
					Category: Display Lists
*****************************************************************************
*****************************************************************************/
void HOOPS_API execute_display_list (
		Net_Rendition const &	nr,
		Display_List const *	dl);

void HOOPS_API check_display_list(
		Net_Rendition const &	nr,
		Display_List **			dll,
		uint32_t *				mask,
		uint32_t *				viz_added);

bool HOOPS_API begin_display_list(
		Net_Rendition const &	nr,
		Display_List **			dll,
		uint32_t *				mask,
		bool					compile_only);

void HOOPS_API end_display_list(
		Net_Rendition const &	nr,
		Display_List **			dll,
		bool					compile_only);

bool check_segment_display_list_valid (
		Net_Rendition const &	nr,
		Display_List const *	dl,
		Segment_H3DDL const *	segdl,
		Type_Flags *			viz,
		Type_Flags *			viz_added,
		Type_Flags *			viz_removed);

void mark_tristrip_display_list (
		Net_Rendition const &	nr,
		Tristrip const *		ts,
		RGB const *				gref_color,
		H3DVertexFormat const &	vf,
		Geometry_H3DDL *		godl);

bool check_tristrip_display_list_valid (
		Net_Rendition const &	nr,
		Tristrip const *		ts,
		Display_List const *	dl,
		Geometry_H3DDL const *	godl);

bool check_polyedge_display_list_valid (
		Net_Rendition const &	nr,
		Polyedge const *		pe,
		Display_List const *	dl,
		Geometry_H3DDL const *	godl,
		bool					want_wide_lines,
		bool					check_dihedral = false);

bool check_polymarker_display_list_valid (
		Net_Rendition const &	nr,
		Type_Flags				type_flags,
		Display_List const *	dl,
		Geometry_H3DDL const *	godl);

void execute_polygon_face_display_list (
		Net_Rendition const &	nr,
		Geometry_H3DDL *		odl);

void execute_polyedge_display_list (
		Net_Rendition const &	nr,
		Driver_DL *				bddl,
		Line_Rendition const &	er);

void execute_polyedge_display_list_lists (
		Net_Rendition const &	nr,
		Segment_H3DDL *			segdl,
		bool					draw_explicit,
		bool					draw_implicit);

void execute_marker_display_list (
		Net_Rendition const &	nr,
		Geometry_H3DDL *		odl);

void execute_polygon_display_list_list (
		Net_Rendition const &	nr,
		Driver_DL_List const &	odl_list,
		Type_Flags				visibility);

void execute_polyline_display_list_list (
		Net_Rendition const &	nr,
		Driver_DL_List const &	odl_list);


void prepare_tristrip(
		Net_Rendition const &	nr,
		Tristrip const *		ts,
		Geometry_H3DDL const *	godl,
		RGB const *				gref_color,
		TristripRenderControl &	rc);

/*****************************************************************************
*****************************************************************************
						Category: Actions
*****************************************************************************
*****************************************************************************/
void HOOPS_API draw_dc_rgb32_rasters (Net_Rendition const &  nr,
	   DC_Point const * start,
	   DC_Point const * end,
	   int row_bytes, RGBAS32 const * raster);

void HOOPS_API draw_dc_rectangle (
		Net_Rendition const & nr,
		int left,
		int right,
		int bottom,
		int top);

void HOOPS_API draw_dc_gouraud_polyline (
		Net_Rendition const &  nr,
		int count_in,
		DC_Point const *points_in,
		RGBAS32 const *colors_in);
void HOOPS_API draw_dc_polyline (
		Net_Rendition const &  nr,
		int count_in,
		DC_Point const *points_in);

void HOOPS_API draw_dc_polytriangle_helper (
		Net_Rendition const &  nr,
		int count,
		DC_Point const * points,
		RGBAS32 const * colors,
		const int color_mode);
void HOOPS_API draw_dc_colorized_polytriangle (
		Net_Rendition const &  nr,
		int count,
		DC_Point const * points,
		RGBAS32 const * colors,
		bool single);
void HOOPS_API draw_dc_polytriangle (
		Net_Rendition const &  nr,
		int count,
		DC_Point const * points);
void HOOPS_API draw_dc_gouraud_polytriangle (
		Net_Rendition const & nr,
		int	count,
		DC_Point const *points,
		RGBAS32 const *colors);
void HOOPS_API draw_dc_textured_polytriangle (
		Net_Rendition const &  nr,
		int count,
		DC_Point const * points,
		RGBA const * color,
		Plane const * planes,
		DPlane const * dplanes,
		Parameter const * params,
		int32_t param_width,
		Parameter_Flags param_flags);

void HOOPS_API draw_dc_colorized_polydot (
		Net_Rendition const &  nr,
		int count,
		DC_Point const * points,
		RGBAS32 const * colors,
		bool single);
void HOOPS_API draw_dc_polydot (
		Net_Rendition const &  nr,
		int count,
		DC_Point const * points);


void HOOPS_API draw_indexed_polyedge (
		Net_Rendition const &  nr,
		Polyedge const *pe);

void HOOPS_API draw_3d_marker (
		Net_Rendition const &  nr,
		Marker const *m);

void HOOPS_API draw_3d_polymarker (
		Net_Rendition const &  nr,
		Polymarker const *pm);

void HOOPS_API really_draw_3d_polymarker (
		Net_Rendition const &  nr,
		Polymarker const *pm,
		Marker_Rendition const & kr,
		DL_Bin *bin);

void HOOPS_API draw_3d_text (
		Net_Rendition const &  nr,
		Text const *text);

void HOOPS_API draw_3d_grid (
		Net_Rendition const &  nr,
		Grid const *grid);

void HOOPS_API draw_3d_image(
		Net_Rendition const &  nr,
		Image const *image);

void HOOPS_API really_draw_3d_polyedge_im (
		Net_Rendition const &  nr,
		Polyedge const *pe,
		DL_Bin *bin,
		Line_Rendition const &er,
		H3D_Vertex_Buffer_Style buffer_style,
		H3DVertexFormat vf);

bool punt_3d_polyedge (
		Net_Rendition const &nr,
		Line_Rendition const &er,
		bool transparency_punting,
		float weight);

void really_draw_3d_polyedge (
		Net_Rendition const &  nr,
		Polyedge const *pe,
		Line_Rendition const &er,
		DL_Bin *bin);

void HOOPS_API draw_3d_polyedge (
		Net_Rendition const &  nr,
		Polyedge const *pe);

void draw_3d_gs_elliptical_arc (
		Net_Rendition const &nr,
		Polyedge const *pe, 
		DL_Bin *bin,
		Cutting_Plane_Set const *cut_set);

void HOOPS_API draw_3d_elliptical_arc (
		Net_Rendition const & nr,
		Elliptical_Arc const *arc);

void HOOPS_API draw_3d_polyline (
		Net_Rendition const &  nr,
		Polyline const *p);

bool punt_3d_tristrip(
		Net_Rendition const & nr,
		Strip_Flags special_flags,
		bool is_transparent);

void really_draw_indexed_tristrip (
		Net_Rendition const &  nr,
		Tristrip const * ts,
		DL_Bin * bin);

void HOOPS_API draw_indexed_tristrip (
		Net_Rendition const &  nr,
		Tristrip const * ts);

void HOOPS_API really_draw_indexed_polyedge (
		Net_Rendition const &  nr,
		Polyedge const * pe,
		DL_Bin * bin);

void HOOPS_API draw_indexed_polyedge(
		Net_Rendition const &  nr,
		Polyedge const * pe);

void HOOPS_API draw_3d_tristrip_im_strips(
	Net_Rendition const &			nr,
	Tristrip const *				ts,
	DL_Bin *			bin,
	TristripRenderControl const &	rc);

void draw_3d_tristrip_im_lists(
	Net_Rendition const &			nr,
	Tristrip const *				ts,
	DL_Bin *			bin,
	TristripRenderControl const &	rc);

INLINE void draw_3d_tristrip_im (
	Net_Rendition const &	nr,
	Tristrip const *		ts,
	DL_Bin *	bin,
	TristripRenderControl &	rc);

void HOOPS_API really_draw_3d_tristrip (
		Net_Rendition const &  nr,
		Tristrip const * ts,
		DL_Bin * bin);

void HOOPS_API draw_3d_tristrip (
		Net_Rendition const &  nr,
		Tristrip const * ts);

void HOOPS_API draw_3d_polygon (
		Net_Rendition const & inr,
		Polygon const *polygon);

void HOOPS_API draw_3d_ellipse (
		Net_Rendition const & nr,
		Ellipse const *e);

void draw_3d_ellipse_helper (
		Net_Rendition const & nr,
		Ellipse const *e);

void HOOPS_API shadow_map_draw_tree (
		Net_Rendition const & nr,
		Segment const * start,
		Include_Path const & path) ;

void HOOPS_API simple_shadow_draw_tree (
		Net_Rendition const & inr,
		Segment const * start,
		Include_Path const & path) ;

void HOOPS_API reflection_plane_draw_tree (
		Net_Rendition const & nr,
		Segment const * start,
		Include_Path const & path,
		float opacity,
		float radius);

/*****************************************************************************
*****************************************************************************
						Category: Uncategorized
*****************************************************************************
*****************************************************************************/

//curve_metadata here is a little container so that we can link together
// HD_Determine_Elliptical_Res computation results.  Otherwise we'd have to
// call it once before allocating and again to generate the points -- and
// it can get to be kind of expensive.
struct curve_metadata {
	Geometry const *	g;
	Point *				points;
	DPoint *			dpoints;
	RGB const *			gref_edge_color;
	int					needed; //upper bound for allocation
	int					count;  //actual count
	DB_Type				type;
};

struct ellipse_metadata : public curve_metadata {
	RGB const *	gref_face_color;
	Plane		plane;
};

typedef unrolled_list<curve_metadata *, POOL_Allocator<curve_metadata *> > curve_metadata_List;
typedef unrolled_list<ellipse_metadata *, POOL_Allocator<ellipse_metadata *> > ellipse_metadata_List;

Polyedge *lines_to_polyedges(
	Net_Rendition const & nr,
	bool needs_culling,
	Geometry const *start_g,
	bool *contains_curves,
	bool single,
	RGB const *gref_line_color,
	Display_List * dl = null);

void HOOPS_API clear_z_buffer (
		Net_Rendition const &	nr,
		IntRectangle const &	area);

void render_3d_lines (
		Net_Rendition const &  nr,
		Geometry const * start_g,
		Action_Mask mask,
		bool single);

Geometry HOOPS_API const * segment_render (
		Net_Rendition const &  nr,
		Geometry const * start_g,
		Action_Mask mask,
		bool single);

void HOOPS_API create_region (Net_Rendition const &  nr,
			IntRectangle const * area, void **image, void **z);

void HOOPS_API save_region (Net_Rendition const &  nr,
		IntRectangle const * area, void * image, void * z);

void HOOPS_API restore_region (Net_Rendition const &  nr,
		IntRectangle const * area, void * image, void * z);

void HOOPS_API destroy_region (Net_Rendition const &  nr,
		void * image, void * z);

void HOOPS_API downgrade_segdl(Display_Context const * dc, Display_List * dl);

void HOOPS_API draw_window(Net_Rendition const & nr, IntRectangle const * extent);

int get_tristrip_contents(Tristrip const *ts, bool has_gref_color);
int get_polyedge_contents(Polyedge const *pe, bool has_gref_color);
int get_polymarker_contents(Polymarker const *pm, bool has_gref_color);

int get_tristrip_point_count(Tristrip const *ts);


H3D_Vertex_Buffer_Style figure_pe_vertex_buffer_style (
	Polyedge const *pe,
	DL_Bin *bin,
	Strip_Flags *special_flags_out);

Plane * create_tangents(H3DData * h3ddata, Tristrip const * ts);


/* To use this function, you *must* populate the Tex_Usage_Counts struct with
 * texture counts.  Then pass it in, and the function will calculate which
 * TEXCOORD should be used for the specified texture.
 *
 * Normally, this function returns the texture unit associated with a given texture type;
 * for instance, TU_DIFFUSE_TEXTURE might return unit 0, meaning TEXCOORD0.  However, when
 * TU_GET_UNIT_USAGE is queried, it's a reverse lookup; for a given TEXCOORD, it returns
 * the associated type (such as TU_DIFFUSE_TEXTURE).
 */
void GetTextureUnitUsage(
	int							usage,
	int							number,
	Tex_Usage_Counts const &	counts,
	int *						vin,
	int *						vout,
	int *						sampler);

/* attempts to allocate h3ddata->short_pointmap to the requested size, and returns it if successful */
static INLINE short *request_short_pointmap(H3DData *h3ddata, int requested_size)
{
	if (h3ddata->short_pointmap == null || h3ddata->short_pointmap_allocated < requested_size) { 
		short *pointmap;
		int size = (int) (requested_size * 1.5f);

		TRY_ALLOC_ARRAY(pointmap, size, short); 
		if (!pointmap) {
			size = requested_size;
			TRY_ALLOC_ARRAY(pointmap, size, short); 
			if (!pointmap)
				return null;
		}
		if (h3ddata->short_pointmap)
			FREE_ARRAY(h3ddata->short_pointmap, h3ddata->short_pointmap_allocated, short);
		h3ddata->short_pointmap_allocated = size;
		h3ddata->short_pointmap = pointmap;
	} 
	return h3ddata->short_pointmap;
}

/* attempts to allocate h3ddata->int_pointmap to the requested size, and returns it if successful */
static INLINE int *request_int_pointmap(H3DData *h3ddata, int requested_size)
{
	if (h3ddata->int_pointmap == null || h3ddata->int_pointmap_allocated < requested_size) { 
		int *pointmap;
		int size = (int) (requested_size * 1.5f);

		TRY_ALLOC_ARRAY(pointmap, size, int); 
		if (!pointmap) {
			size = requested_size;
			TRY_ALLOC_ARRAY(pointmap, size, int); 
			if (!pointmap)
				return null;
		}
		if (h3ddata->int_pointmap)
			FREE_ARRAY(h3ddata->int_pointmap, h3ddata->int_pointmap_allocated, int);
		h3ddata->int_pointmap_allocated = size;
		h3ddata->int_pointmap = pointmap;
	} 
	return h3ddata->int_pointmap;
}


enum MarkerFlags
{
	// Source VB should contain quads (default points)
	MARKER_GEOMETRY_QUADS		= (1<<1),
	MARKER_GEOMETRY_LINES		= (1<<2),

	// Vertex shader should displace the marker corners (default centre)
	MARKER_TRANSFORM_CORNERS	= (1<<3),

	// How to draw the marker - one is always set.
	MARKER_DRAW_POINTS			= (1<<4),
	MARKER_DRAW_QUADS			= (1<<5),
	MARKER_DRAW_X				= (1<<6),
	MARKER_DRAW_PLUS			= (1<<7),

	// Special flags
	MARKER_POINT_SPRITES		= (1<<8),
	MARKER_CIRCLE				= (1<<9),
	MARKER_WORLDSPACE			= (1<<10),
	MARKER_CLAMP_SIZE			= (1<<11),
	MARKER_ACCURATE				= (1<<12),
	MARKER_TEXTURE				= (1<<13),
	MARKER_MATERIAL_GS			= (1<<14),
	MARKER_SPHERE				= (1 << 15),

	// Send this marker back to upper level
	MARKER_PUNT					= 0,
};



void HOOPS_API * im_utility(
	Net_Rendition const &	nr,
	long					request,
	void *					pointer,
	intptr_t				v1,
	intptr_t				v2,
	intptr_t				v3,
	intptr_t				v4,
	float					f1,
	float					f2,
	float					f3,
	float					f4);

#if 0
// Layout of key is GEOMETRY:FACE with variable split based on the containing segment.
// This rules structure defines the split and provides methods to encode and decode keys.
struct HOOPS_API Selection_Key_Rules
{
	// Geometry types. Not the same as Type_Flags!
	enum Geometry_Type
	{
		Face =			0x0,
		Edge =			0x1,
		Marker =		0x2,
		Isoline =		0x3,

		Type_Count =	4,
		Type_Bits =		2,
		Invalid_Type =	-1
	};

	enum { 
		TYPE_SHIFT = 32-Type_Bits 
	};

	// Map between database type and rules type (one only)
	static Geometry_Type type_from_typeflags( Type_Flags type ) {
		switch ( type )
		{
			case T_FACES :		return Face;
			case T_EDGES :		return Edge;
			case T_MARKERS :
			case T_VERTICES :	return Marker;
			case T_ISOLINES :	return Isoline;
			default : 
				ASSERT(0);
		}

		return Invalid_Type;
	}

	// Map between database type flags and a bitfield we use to type-check the highlight map.
	static uint32_t bitfield_from_typeflags( Type_Flags types ) {
		return 
			  ((1 << Selection_Key_Rules::Face) *	 ((types & T_FACES) != 0))
			| ((1 << Selection_Key_Rules::Edge) *	 ((types & T_EDGES) != 0)) 
			| ((1 << Selection_Key_Rules::Marker) *	 ((types & (T_MARKERS|T_VERTICES)) != 0)) 
			| ((1 << Selection_Key_Rules::Isoline) * ((types & T_ISOLINES) != 0));
	}

	// How many bits used for storing geometry index?
	uint32_t m_geom_bits;	

	// How many bits used for storing sub-geometry index?
	uint32_t m_subgeom_bits;

	// How many bits used for storing face index? (Could both be zero for geometry outside the DB)
	uint32_t m_prim_bits;

	// For masking off faces
	uint32_t prim_mask() const		{ return (1<<m_prim_bits) - 1; }
	uint32_t prim_shift() const		{ return 0; }

	// For masking off sub-geometry
	uint32_t subgeom_mask() const		{ return (1<<m_subgeom_bits)-1; }
	uint32_t subgeom_shift() const	{ return m_prim_bits; }

	// For masking off geometry
	uint32_t geom_mask() const		{ return (1<<m_geom_bits)-1; }
	uint32_t geom_shift() const		{ return m_subgeom_bits + m_prim_bits; }

	// For masking off type
	uint32_t type_mask() const		{ return (1<<Type_Bits)-1; }
	uint32_t type_shift() const		{ return TYPE_SHIFT; }

	void init(Segment const *segment);

	Selection_Key_Rules::Geometry_Type get_type(uint32_t key) const
	{
		uint32_t raw = (key >> TYPE_SHIFT) & type_mask();
		return (Selection_Key_Rules::Geometry_Type)raw;
	}

	uint32_t get_geometry_index(uint32_t key) const
	{
		return geom_mask() & (key >> geom_shift());
	}

	uint32_t get_subgeometry_index(uint32_t key) const
	{
		return subgeom_mask() & (key >> subgeom_shift());
	}


	uint32_t get_prim_index(uint32_t key) const
	{
		return prim_mask() & (key >> prim_shift());
	}
};

// Extents
struct Selection_Key_Helper : public Selection_Key_Rules
{
	uint32_t m_path_index;	// For high bits of key
	uint32_t m_geom_base;	// Pre-shifted geom index

	void init(Display_Context const *dc, void const *thing, Geometry_Type type);
	void init(Display_Context const *dc, Geometry_H3DDL const *godl, H3DIndexedBufferObject const *ibo, Geometry_Type type);	

	INLINE void init(Display_Context const *dc, Tristrip const *ts)		{ init(dc, ts, Face); }
	INLINE void init(Display_Context const *dc, Polyedge const *pe)		{ init(dc, pe, Edge); }
	INLINE void init(Display_Context const *dc, Polymarker const *pm)	{ init(dc, pm, Marker); }
};
#endif

//int HOOPS_API map_id_to_prims(Selection_ID const *id, int& subgeom, int* prims, int max_prims);
void HOOPS_API image_driver_finish_picture(Net_Rendition const & nr);

#ifdef _MSC_VER
void HOOPS_API reload_shaders(H3DData * h3ddata);
void HOOPS_API delete_mod_times();

HOOPS_API VOID CALLBACK WaitOrTimerCallback(PVOID lpParameter, BOOLEAN TimerOrWaitFired);

static const int MESSAGE_PUMP_TIMER_INTERVAL = 1000;

#endif

void HOOPS_API hwvs_init_render(Net_Rendition const & nr);
void HOOPS_API hwvs_finish_render(Net_Rendition const & nr);

End_HOOPS_Namespace

#endif // H3DCOMMON_H


