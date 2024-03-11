#include "fspch.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Fengshui
{
	Shader* Shader::Create(const std::string& vertexSource, const std::string& fragmentSource)
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
				return new OpenGLShader(vertexSource, fragmentSource);
			}
			default:
			{
				FS_ENGINE_ASSERT(false, "Unknown render API");
				return nullptr;
			}
		}
	}
}