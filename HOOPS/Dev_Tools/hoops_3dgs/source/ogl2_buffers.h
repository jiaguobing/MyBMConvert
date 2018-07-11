#pragma once

class RingBuffer : public CMO_Pooled
{
public:

	RingBuffer(HOGLData * ogldata, uint32_t size_in_bytes) : ogldata(ogldata), buffer(0), buffer_size(0), next_offset(0), locked(false)
	{
		EnsureSize(size_in_bytes);
	}

	// Returns a pointer to a location where 'size_in_bytes' bytes can be written. 
	// 'out_offset' is the offset (in bytes) at which to bind the buffer with OpenGL.
	void * Lock(uint32_t num_bytes, uint32_t & out_offset, GLuint & out_buffer)
	{
		ASSERT(!locked);

		if (num_bytes > buffer_size) {
			EnsureSize(2 * num_bytes);
		}

		ASSERT(num_bytes <= buffer_size);

		// may not actually be used as ARRAY_BUFFER by user
		ENSURE_ARRAY_BUFFER(ogldata, buffer);

		GLvoid * ptr = null;		

		if (next_offset + num_bytes > buffer_size)	{
			ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, num_bytes, GL_MAP_INVALIDATE_BUFFER_BIT | GL_MAP_WRITE_BIT);
			next_offset = 0;
		}

		else {
			ptr = glMapBufferRange(GL_ARRAY_BUFFER, next_offset, num_bytes, GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_WRITE_BIT);
		}

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
		ENSURE_ARRAY_BUFFER(ogldata, buffer);
		glUnmapBuffer(GL_ARRAY_BUFFER);
		locked = false;
	}

	void EnsureSize(uint32_t new_size_in_bytes)
	{
		if (buffer_size >= new_size_in_bytes)
			return;

		ASSERT(!locked);

		if (buffer == 0)
			glGenBuffers(1, &buffer);

		ENSURE_ARRAY_BUFFER(ogldata, buffer);

		glBufferData(GL_ARRAY_BUFFER, new_size_in_bytes, null, GL_DYNAMIC_DRAW);

		HOOPS::WORLD->display_list_vram_usage -= buffer_size;
		HOOPS::WORLD->display_list_vram_usage += new_size_in_bytes;

		buffer_size = new_size_in_bytes;
		next_offset = 0;
	}

	~RingBuffer()
	{
		if (buffer != 0) {

			if (ogldata->oglcache.current_array_buffer == buffer)
				ogldata->oglcache.current_array_buffer = 0;

			if (ogldata->oglcache.current_element_buffer == buffer)
				ogldata->oglcache.current_element_buffer = 0;

			glDeleteBuffers(1, &buffer);
		}
	}

private:

	HOGLData * ogldata;
	GLuint buffer;
	uint32_t buffer_size;
	uint32_t next_offset;
	bool locked;
};

// abstract interface so other code in driver can use DrawWithAttributes without knowing which VertexBufferCache implementation it has.
class IVertexBufferCache : public H3DVertexBufferCache
{
public:

	virtual bool DrawWithAttributes(H3DData *h3ddata, HOGLVertexAttributes const & attributes, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1) = 0;
};

class VertexBufferCacheRB : public IVertexBufferCache
{
public:

	VertexBufferCacheRB() : ogldata(null), buffer_id(0), buffer_offset(0), stride(0), buffer(null), m_locked(false)
	{
		m_stream_count = 0;
		m_vertex_data = null;
		m_nMaxPoints = 0;
	}

	~VertexBufferCacheRB()
	{
		FREE_ARRAY(m_strides, m_stream_count, unsigned int);

		HOOPS::WORLD->display_list_vram_usage -= HOGL_PER_OBJECT_PENALTY;

		delete buffer;
	}

	bool CreateVertexBuffer(H3DData *h3ddata, unsigned int max_points, H3DVertexFormat VF)
	{
		ogldata = (HOGLData*)h3ddata;

		m_nMaxPoints = max_points;
		m_VF = VF;
		m_stream_count = VF.get_stream_count();
		ALLOC_ARRAY(m_strides, m_stream_count, unsigned int);
		VF.get_stream_strides(m_strides);

		stride = VF.get_size(true);

		ALLOC_ARRAY(m_vertex_data, m_stream_count, float*);

		buffer = POOL_NEW(ogldata->dc->memory_pool(), RingBuffer)(ogldata, m_nMaxPoints * stride);

		return true;
	}

	bool Reset()
	{
		for (int i = 0; i < m_stream_count; ++i) {
			m_vertex_data[i] = null;
		}

		m_nMaxPoints = 0;
		return true;
	}

	bool Discard()
	{
		return true;
	}

	bool PushDataToBuffer(Matrix matrix = null, int length = -1)
	{
		int vertices_to_copy = length >= 0 ? length : m_locked_vertex_size;
		vertices_to_copy -= m_push_data_start;
		if (vertices_to_copy <= 0)
			return true;

		int offset = m_push_data_start * 3;
		if (matrix) {
			matrix->Transform(vertices_to_copy, (Point*)(m_vertex_data[0] + offset), (Point*)(m_vertex_data[0] + offset));
			if (m_VF.get_normals())
				matrix->Transform(vertices_to_copy, (Vector*)(m_vertex_data[1] + offset), (Vector*)(m_vertex_data[1] + offset));
		}

		m_push_data_start += vertices_to_copy;

		return true;
	}

	bool Lock(unsigned int count, H3DVertexBuffer *pv)
	{
		ASSERT(count <= m_nMaxPoints);
		ASSERT(!m_locked);

		m_locked = true;

		uint32_t const requested_size = count * stride;
		float * base_ptr = (float*)buffer->Lock(requested_size, buffer_offset, buffer_id);

		float * ptrs[16];
		for (int i = 0; i < m_stream_count; ++i) {
			m_vertex_data[i] = ptrs[i] = base_ptr;
			base_ptr += count * m_strides[i];
		}

		m_locked_vertex_size = count;
		m_push_data_start = 0;

		// Lock the vertex buffer.
		pv->Init(&ptrs[0], &m_VF);

		return true;
	}

	GLvoid const * StreamOffset(int stream_index) const
	{
		uint32_t offset = buffer_offset;

		// FIXME
		for (int i = 0; i < stream_index; ++i) {
			offset += m_locked_vertex_size * m_strides[i] * sizeof(float);
		}

		return static_cast<unsigned char const *>(0) + offset;
	}

	bool Unlock(Matrix matrix)
	{
		ASSERT(m_locked);

		PushDataToBuffer(matrix);

		buffer->Unlock();
		
		for (int i = 0; i < m_stream_count; ++i) {
			m_vertex_data[i] = null;
		}

		m_locked = false;

		return true;
	}

	bool DrawRepeatableWithAttributes(H3DData *, HOGLVertexAttributes const & attributes, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		GLenum const prim_type = oglprimitive(PrimitiveType);
		GLsizei const num_indices = prim_to_indices(PrimitiveType, PrimitiveCount);

		BEGIN_VERTEX_ATTRIBS(ogldata);
		{
			ENSURE_ARRAY_BUFFER(ogldata, buffer_id);

			int stream_index = 0;

			SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, m_VF.get_double_position() ? GL_DOUBLE : GL_FLOAT, GL_FALSE, StreamOffset(stream_index));
			stream_index++;
			if (m_VF.get_normals()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, StreamOffset(stream_index));
				stream_index++;
			}
			if (m_VF.get_diffuse()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, StreamOffset(stream_index));
				stream_index++;
			}
			if (m_VF.get_specular()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, StreamOffset(stream_index));
				stream_index++;
			}
			if (m_VF.get_tex_count() > 0) {
				for (int texcoord = 0; texcoord < m_VF.get_tex_count(); texcoord++) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], m_VF.get_tex_size(texcoord), GL_FLOAT, GL_FALSE, StreamOffset(stream_index));
					stream_index++;
				}
			}
		}
		END_VERTEX_ATTRIBS(ogldata);

		if (instances > 1)
			glDrawArraysInstanced(prim_type, 0, num_indices, instances);
		else
			glDrawArrays(prim_type, 0, num_indices);

		return true;
	}

	virtual bool DrawWithAttributes(H3DData *h3ddata, HOGLVertexAttributes const & attributes, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		bool result = DrawRepeatableWithAttributes(h3ddata, attributes, PrimitiveType, PrimitiveCount, instances);
		DrawComplete();
		return result;
	}

	bool Draw(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount, instances);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount, int instances = 1)
	{
		HOGLShader *shader = (HOGLShader *)h3ddata->cache->Shader;
		HOGLVertexAttributes const & attributes = shader->m_vertex_attributes;
		return DrawRepeatableWithAttributes(h3ddata, attributes, PrimitiveType, PrimitiveCount, instances);
	}

	void DrawComplete()
	{
		buffer_id = 0;
		buffer_offset = 0;
	}

	RingBuffer * buffer;
	HOGLData * ogldata;
	GLuint buffer_id;
	GLuint buffer_offset;
	uint32_t stride; // in bytes
	bool m_locked;
};

class GenericVertexBufferCacheRB : public H3DGenericVertexBufferCache
{
public:

	GenericVertexBufferCacheRB(H3DData *h3ddata) :
		H3DGenericVertexBufferCache(h3ddata), 
		ogldata((HOGLData*)h3ddata),
		buffer(null),
		buffer_id(0), buffer_offset(0), 
		locked_count(0)
	{

	}

	~GenericVertexBufferCacheRB()
	{
		delete buffer;
	}

	GLvoid const * StreamOffset(int stream_index) const
	{
		uint32_t offset = buffer_offset;

		// FIXME
		for (int i = 0; i < stream_index; ++i) {
			offset += locked_count * m_strides[i] * sizeof(float);
		}

		return static_cast<unsigned char const *>(0) + offset;
	}

	bool Lock(H3DVertexFormat const &vf, unsigned int count, H3DVertexBuffer *pv)
	{
		ASSERT(!m_locked);

		m_VF = vf;
		m_VF.get_stream_strides(m_strides);
		m_locked = true;

		ASSERT(m_VF == vf);

		uint32_t const stride = vf.get_size(true);
		uint32_t const new_size = m_nMaxPoints * stride; // how big the buffer should be to accommodate streams in VF
		uint32_t const requested_size = count * stride; // how many bytes caller wants right now

		if (buffer == null)
			buffer = POOL_NEW(ogldata->dc->memory_pool(), RingBuffer)(ogldata, new_size);
		else
			buffer->EnsureSize(new_size);
		
		float * base_ptr = (float*)buffer->Lock(requested_size, buffer_offset, buffer_id);
		
		int const stream_count = m_VF.get_stream_count();

		float * ptrs[12];
		ZERO_ARRAY(ptrs, 12, float *);
		for (int i = 0; i < stream_count ; ++i) {
			ptrs[i] = base_ptr;
			base_ptr += count * m_strides[i];			
		}

		locked_count = count;
		
		// Lock the vertex buffer.
		pv->Init(&ptrs[0], &m_VF);

		return true;
	}

	bool Unlock()
	{
		ASSERT(m_locked);
		m_locked = false;

		buffer->Unlock();

		return true;
	}

	int StartVertex()
	{
		return m_curr_offset;
	}

	bool Draw(H3DData *h3ddata, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount)
	{
		bool result = DrawRepeatable(h3ddata, PrimitiveType, PrimitiveCount);
		DrawComplete();
		return result;
	}

	bool DrawRepeatable(H3DData *, H3DPRIMITIVETYPE PrimitiveType, unsigned int PrimitiveCount)
	{
		GLenum const prim_type = oglprimitive(PrimitiveType);
		GLsizei const num_indices = prim_to_indices(PrimitiveType, PrimitiveCount);
		HOGLShader *shader = (HOGLShader *)ogldata->cache->Shader;

		HOGLVertexAttributes const & attributes = shader->m_vertex_attributes;

		ENSURE_ARRAY_BUFFER(ogldata, buffer_id);

		BEGIN_VERTEX_ATTRIBS(ogldata);
		{
			int stream_index = 0;

			if (m_VF.get_position()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_FLOAT, GL_FALSE, StreamOffset(stream_index++));
			}
			if (m_VF.get_double_position()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_DOUBLE, GL_FALSE, StreamOffset(stream_index++));
			}
			if (m_VF.get_normals()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, StreamOffset(stream_index++));
			}
			if (m_VF.get_diffuse()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, StreamOffset(stream_index++));
			}
			if (m_VF.get_specular()) {
				SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, StreamOffset(stream_index++));
			}
			if (m_VF.get_tex_count() > 0) {
				for (int texcoord = 0; texcoord < m_VF.get_tex_count(); ++texcoord) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], m_VF.get_tex_size(texcoord), GL_FLOAT, GL_FALSE, StreamOffset(stream_index++));
				}
			}
		}
		END_VERTEX_ATTRIBS(ogldata);

		glDrawArrays(prim_type, 0, num_indices);

		return GL_GET_ERROR() == GL_NO_ERROR;
	}

	void DrawComplete()
	{
		buffer_id = 0;
		buffer_offset = 0;
	}

	RingBuffer * buffer;
	GLuint buffer_id;
	uint32_t buffer_offset;
	uint32_t locked_count;
	HOGLData * ogldata;
	bool m_locked;
};

class IndexBufferCacheRB : public H3DIndexBufferCache
{
public:
	IndexBufferCacheRB() : buffer(null), buffer_id(0), buffer_offset(0), m_locked(false)
	{
		m_pVertexBufferCache = null;
	}

	~IndexBufferCacheRB()
	{
		delete buffer;
	}

	bool CreateIndexBuffer(H3DData *h3ddata)
	{
		ogldata = (HOGLData *)h3ddata;		

		GLsizei const size = IB_CACHE_MAX_SIZE * sizeof(GLushort);
		buffer = buffer = POOL_NEW(ogldata->dc->memory_pool(), RingBuffer)(ogldata, size);

		return true;
	}

	bool Lock(int count, short ** out_ptr)
	{
		ASSERT(!m_locked);

		*out_ptr = (short*)buffer->Lock(count * sizeof(short), buffer_offset, buffer_id);

		m_locked = true;

		return true;
	}

	bool Unlock()
	{
		ASSERT(m_locked);

		m_locked = false;

		buffer->Unlock();

		return true;
	}

	bool Draw(H3DData *, H3DPRIMITIVETYPE PrimitiveType, unsigned int VertexCount, unsigned int PrimitiveCount)
	{
		ENSURE_ELEMENT_BUFFER(ogldata, buffer_id);

		HOGLShader * shader = (HOGLShader *)ogldata->cache->Shader;
		HOGLVertexAttributes const & attributes = shader->m_vertex_attributes;

		if (m_pVertexBufferCache) {
			H3DVertexFormat const * vf = &m_pVertexBufferCache->m_VF;
			VertexBufferCacheRB * vb = (VertexBufferCacheRB*)m_pVertexBufferCache;

			ENSURE_ARRAY_BUFFER(ogldata, vb->buffer_id);

			BEGIN_VERTEX_ATTRIBS(ogldata);
			{
				int stream_index = 0;
				if (vf->get_position()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_FLOAT, GL_FALSE, vb->StreamOffset(stream_index));
					stream_index++;
				}
				else if (vf->get_double_position()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_DOUBLE, GL_FALSE, vb->StreamOffset(stream_index));
					stream_index++;
				}
				if (vf->get_normals()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, vb->StreamOffset(stream_index));
					stream_index++;
				}
				if (vf->get_diffuse()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, vb->StreamOffset(stream_index));
					stream_index++;
				}
				if (vf->get_specular()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, vb->StreamOffset(stream_index));
					stream_index++;
				}
				for (int texcoord = 0; texcoord < vf->get_tex_count(); texcoord++) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], vf->get_tex_size(texcoord), GL_FLOAT, GL_FALSE, vb->StreamOffset(stream_index));
					stream_index++;
				}
			}
			END_VERTEX_ATTRIBS(ogldata);
		}
		else {

			auto *vb = (GenericVertexBufferCacheRB *)m_pGenericVertexBufferCache;
			H3DVertexFormat *vf = &m_pGenericVertexBufferCache->m_VF;

			ENSURE_ARRAY_BUFFER(ogldata, vb->buffer_id);

			BEGIN_VERTEX_ATTRIBS(ogldata);
			{
				int stream_index = 0;
				if (vf->get_position()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_FLOAT, GL_FALSE, vb->StreamOffset(stream_index++));
				}
				else if (vf->get_double_position()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.position, 3, GL_DOUBLE, GL_FALSE, vb->StreamOffset(stream_index++));
				}
				if (vf->get_normals()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.normal, 3, GL_FLOAT, GL_FALSE, vb->StreamOffset(stream_index++));
				}
				if (vf->get_diffuse()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.color, 4, GL_UNSIGNED_BYTE, GL_TRUE, vb->StreamOffset(stream_index++));
				}
				if (vf->get_specular()) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.secondary_color, 4, GL_UNSIGNED_BYTE, GL_TRUE, vb->StreamOffset(stream_index++));
				}
				for (int texcoord = 0; texcoord < vf->get_tex_count(); ++texcoord) {
					SET_VERTEX_ATTRIB_POINTER(ogldata, attributes.texcoords[texcoord], vf->get_tex_size(texcoord), GL_FLOAT, GL_FALSE, vb->StreamOffset(stream_index++));
				}
			}
			END_VERTEX_ATTRIBS(ogldata);
		}
		UNREFERENCED(VertexCount);

		glDrawRangeElements(oglprimitive(PrimitiveType), 0, VertexCount, prim_to_indices(PrimitiveType, PrimitiveCount), GL_UNSIGNED_SHORT, (GLvoid const *)(intptr_t)buffer_offset);

		GL_CHECK_ERROR();

		return true;
	}

	void DrawComplete()
	{
		if (m_pVertexBufferCache)
			m_pVertexBufferCache->DrawComplete();
		else
			m_pGenericVertexBufferCache->DrawComplete();

		buffer_id = 0;
		buffer_offset = 0;
	}

	RingBuffer * buffer;
	HOGLData *		ogldata;
	GLuint			buffer_id;
	uint32_t		buffer_offset;
	bool			m_locked;
};
