#include "fspch.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

#include <glm/gtc/type_ptr.hpp>

namespace Fengshui
{
	Ref<Shader> Shader::Create(const std::string& vertexSource, const std::string& fragmentSource, const std::string& name)
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
				return std::make_shared<OpenGLShader>(vertexSource, fragmentSource, name);
			}
			default:
			{
				FS_ENGINE_ASSERT(false, "Unknown render API");
				return nullptr;
			}
		}
	}

	Ref<Shader> Shader::Create(const std::string& shaderFilePath)
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
			return std::make_shared<OpenGLShader>(shaderFilePath);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}

	void ShaderLibrary::Add(const std::string& name, const Ref<Shader> shader)
	{
		FS_ENGINE_ASSERT(!Exist(name), "Shader already exists");
		m_Shaders[name] = shader;
	}

	void ShaderLibrary::Add(const Ref<Shader> shader)
	{
		std::string name = shader->GetName();
		FS_ENGINE_ASSERT(!Exist(name), "Shader already exists");
		m_Shaders[name] = shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Load(const std::string& name, const std::string& filePath)
	{
		auto shader = Shader::Create(filePath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> ShaderLibrary::Get(const std::string& name)
	{
		FS_ENGINE_ASSERT(Exist(name), "Shader not found");
		if (!Exist(name))
		{
			return nullptr;
		}
		return m_Shaders[name];
	}

	bool ShaderLibrary::Exist(const std::string& name)
	{
		return m_Shaders.find(name) != m_Shaders.end();
	}
}