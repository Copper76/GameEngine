#include "fspch.h"

#include "VertexArray.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLVertexArray.h"

namespace Fengshui
{
	Ref<VertexArray> VertexArray::Create()
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
			return std::make_shared<OpenGLVertexArray>();
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}

	/**
	void VertexArrayLibrary::Add(const Ref<VertexArray> vertexArray, const std::string& name)
	{
		FS_ENGINE_ASSERT(!Exist(name), "Shader already exists");
		m_VertexArrays[name] = vertexArray;
	}

	Ref<VertexArray> VertexArrayLibrary::Get(const std::string& name)
	{
		FS_ENGINE_ASSERT(Exist(name), "Vertex Array not found");
		if (!Exist(name))
		{
			return nullptr;
		}
		return m_VertexArrays[name];
	}

	bool VertexArrayLibrary::Exist(const std::string& name)
	{
		return m_VertexArrays.find(name) != m_VertexArrays.end();
	}
	**/
}