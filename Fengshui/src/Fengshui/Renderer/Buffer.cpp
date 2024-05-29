#include "fspch.h"
#include "Fengshui/Renderer/Buffer.h"
#include "Fengshui/Platform/OpenGL/OpenGLBuffer.h"
#include "Fengshui/Renderer/Renderer.h"

namespace Fengshui
{
	void VertexBuffer::Bind() const
	{

	}

	void VertexBuffer::Unbind() const
	{

	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, float* vertices)
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
			return std::make_shared<OpenGLVertexBuffer>(size, vertices);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}

	void IndexBuffer::Bind() const
	{

	}

	void IndexBuffer::Unbind() const
	{

	}

	Ref<IndexBuffer> IndexBuffer::Create(uint32_t count, uint32_t* indices)
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
			return std::make_shared<OpenGLIndexBuffer>(count, indices);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}
}