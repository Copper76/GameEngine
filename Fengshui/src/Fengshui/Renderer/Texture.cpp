#include "fspch.h"
#include "Texture.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLTexture.h"

namespace Fengshui
{
	Ref<Texture> Texture::Create(uint32_t width, uint32_t height)
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
			return std::make_shared<OpenGLTexture>(width, height);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}

	Ref<Texture> Texture::Create(const std::string& filePath)
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
			return std::make_shared<OpenGLTexture>(filePath);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}

	Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
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
			return std::make_shared<OpenGLTexture2D>(width, height);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}

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