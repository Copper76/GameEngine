#include "fspch.h"
#include "OpenGLFramebuffer.h"

#include <glad/glad.h>

namespace Fengshui
{
	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpec& spec)
		: m_Spec(spec)
	{
		Renew();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);
	}

	void OpenGLFramebuffer::Renew()
	{
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColourAttachment);
		glBindTexture(GL_TEXTURE_2D, m_ColourAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Spec.Width, m_Spec.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		//glTextureStorage2D //storage might work better?
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColourAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Spec.Width, m_Spec.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		//glTexStorage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Spec.Width, m_Spec.Height);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

		FS_ENGINE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Frambuffer is not complete");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	}
	void OpenGLFramebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}