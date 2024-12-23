#pragma once

namespace Fengshui
{
	struct FramebufferSpec
	{
		uint32_t Width = 0, Height = 0;
		bool SwapChainTarget = false;//if the render is for the screen
	};

	class Framebuffer
	{
	public:
		virtual ~Framebuffer() = default;
		virtual void Bind() = 0;
		virtual void Unbind()  = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0;

		virtual uint32_t GetColourAttachmentRendererID() = 0;

		virtual FramebufferSpec& GetSpec() = 0;
		virtual const FramebufferSpec& GetSpec() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);

	};
}
