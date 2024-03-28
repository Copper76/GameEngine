#include "fspch.h"
#include "FrameBuffer.h"

#include "Fengshui/Renderer/Renderer.h"

#include "Fengshui/Platform/OpenGL/OpenGLFramebuffer.h"

namespace Fengshui
{
	Ref<Framebuffer> Framebuffer::Create(const FramebufferSpec& spec)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None:
		{
			FS_ENGINE_ASSERT(false, "RenderAPI::None is not supported");
			return nullptr;
		}
		case RendererAPI::API::OpenGL:
		{
			return std::make_shared<OpenGLFramebuffer>(spec);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
}
}