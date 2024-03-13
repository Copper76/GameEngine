#include "fspch.h"
#include "Texture.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLTexture.h"

namespace Fengshui
{
	Ref<Texture2D> Texture2D::Create(const std::string& filePath, Ref<Shader> shader)
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
			return std::make_shared<OpenGLTexture2D>(filePath, shader);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}
}