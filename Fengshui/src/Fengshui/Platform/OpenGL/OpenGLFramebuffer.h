#pragma once

#include "Fengshui/Renderer/FrameBuffer.h"

namespace Fengshui
{
	class OpenGLFramebuffer : public Framebuffer
	{
	public:
		OpenGLFramebuffer(const FramebufferSpec& spec);
		virtual ~OpenGLFramebuffer();

		void Renew();

		virtual void Bind() override;
		virtual void Unbind() override;

		virtual void Resize(uint32_t width, uint32_t height) override;

		virtual uint32_t GetColourAttachmentRendererID() override { return m_ColourAttachment; };

		virtual FramebufferSpec& GetSpec() override { return m_Spec; }
		virtual const  FramebufferSpec& GetSpec() const override { return m_Spec; }

	private:
		uint32_t m_RendererID;
		uint32_t m_ColourAttachment;
		uint32_t m_DepthAttachment;
		FramebufferSpec m_Spec;
	};
}
