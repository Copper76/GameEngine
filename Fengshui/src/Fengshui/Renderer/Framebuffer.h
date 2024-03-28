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
		virtual void Bind() = 0;
		virtual void Unbind()  = 0;

		virtual uint32_t GetColourAttachmentRendererID() = 0;

		virtual FramebufferSpec& GetSpec() = 0;
		virtual const FramebufferSpec& GetSpec() const = 0;

		static Ref<Framebuffer> Create(const FramebufferSpec& spec);

	};
}
