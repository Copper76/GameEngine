#include "fspch.h"
#include "Texture.h"

#include "Fengshui/Renderer/Renderer.h"

namespace Fengshui
{
	Ref<Texture2D> Texture2D::Create(const std::string& filePath)
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
			return std::make_shared<OpenGLTexture2D>(filePath);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}
}