#pragma once

class GLEWActions : public HOGLActions
{
public:

	// we always have at least ssaa
	virtual bool CanAntiAlias() const
	{
		return true;
	}

	virtual int MaxMsaaSamples() const
	{
		HOGLData * ogldata = m_ogldata;

		GL_CHECK_ERROR();

		if (GLEW_EXT_framebuffer_multisample || GLEW_ARB_framebuffer_object)
		{
			GLint samples = 0;
			glGetIntegerv(GL_MAX_SAMPLES, &samples);

			// Older intel cards that support ARB_framebuffer_object but not EXT_framebuffer_multisample will sometimes incorrectly emit an error here from querying MAX_SAMPLES.
			// We'll suppress the error in this one case and take it to mean msaa is not in fact supported.
			GLenum const error = glGetError();
			ASSERT(error == GL_NO_ERROR || error == GL_INVALID_ENUM);
			if (error == GL_INVALID_ENUM)
			{
				ASSERT(GLEW_EXT_framebuffer_multisample == 0);
				samples = 0;
			}

			return samples;
		}		

		return 0;
	}

protected:

	GLEWActions(HOGLData * ogldata) : HOGLActions(ogldata)
	{

	}
};