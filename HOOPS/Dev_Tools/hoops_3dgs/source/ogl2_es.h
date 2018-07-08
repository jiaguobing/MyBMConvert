#pragma once

class GLESActions : public HOGLActions
{
public:

	GLESActions(HOGLData * ogldata) : HOGLActions(ogldata)
	{

	}

	void Dump_State()
	{
		dump_state();  //dumps absolutely everything that can be queried with glGet
	}

	void force_anti_alias(bool mode)
	{
		m_ogldata->cache->antialias = mode;

		GL_CHECK_ERROR();
	}

	void force_alphatesting(bool mode)
	{
		m_ogldata->cache->alphatesting = mode;

		GL_CHECK_ERROR();
	}

	void force_alphafunc(H3DCMPFUNC func, int ref)
	{		
		m_ogldata->cache->alpha_ref = ref;
		m_ogldata->cache->alpha_func = func;

		GL_CHECK_ERROR();
	}
    
    void create_region_helper(QuickMovesRegion *image_region, QuickMovesRegion *depth_region)
    {
        int const width = m_ogldata->render_target->Width();
        int const height = m_ogldata->render_target->Height();
        
        if (image_region) {

            ASSERT(image_region->surface == null);
            image_region->surface = NEW(HOGLTexture2D)(m_ogldata, width, height, 1, H3DTEXUSAGE_RENDERTARGET, H3DFMT_A8R8G8B8);
            
            if (image_region->surface) {
                unlink(m_ogldata->image_regions, image_region);
                image_region->next = m_ogldata->image_regions;
                m_ogldata->image_regions = image_region;
            }
            
        }

    	if (depth_region && m_ogldata->feature_level == OGL2_ES3) {
            
    		ASSERT(width == m_ogldata->depth_stencil->m_width && height == m_ogldata->depth_stencil->m_height);

    		ASSERT(depth_region->surface == null);
    		depth_region->surface = NEW(HOGLTexture2D)(m_ogldata, width, height, 1, H3DTEXUSAGE_DEPTHSTENCIL,  m_ogldata->depth_stencil->m_format);

    		// Assume no depth texture
    		depth_region->texture = null;

    		if (depth_region->surface) {
    			unlink(m_ogldata->depth_regions, depth_region);
    			depth_region->next = m_ogldata->depth_regions;
    			m_ogldata->depth_regions = depth_region;
    		}
    	}
    }

	void DiscardFramebuffer(GLenum framebuffer_binding, int discards_count, GLenum const * discards)
	{
		if (m_ogldata->feature_level == OGL2_ES3) {				
			glInvalidateFramebuffer(framebuffer_binding, discards_count, discards);
		}

		else if (m_ogldata->feature_level == OGL2_ES2 && glDiscardFramebufferEXT != null) {
			glDiscardFramebufferEXT(framebuffer_binding, discards_count, discards);
		}		

		GL_CHECK_ERROR();
	}

};

static void es_draw_window(Net_Rendition const & nr, IntRectangle const * extent)
{
	HOGLData *ogldata = OGLD(nr->display_context);

	IntRectangle const & outer_actual_extent = nr->display_context->outer_actual_extent;

	if (*extent == outer_actual_extent) {

		ENSURE_DEPTH_MASK(ogldata, true);
		ENSURE_STENCIL_MASK(ogldata, 0xff);
		ENSURE_CHANNEL_MASK(ogldata, H3DMASK_RGBA);
		ENSURE_COLOR_MASK(ogldata, true);
		ENSURE_SCISSOR(ogldata, false);
		ENSURE_VIEWPORT(ogldata, 0, 0, ogldata->render_target->Width(), ogldata->render_target->Height());

		glClearDepthf(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		GL_CHECK_ERROR();
	}

    draw_window(nr, extent);
}
