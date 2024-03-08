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

	VertexBuffer* VertexBuffer::Create(uint32_t size, float* vertices)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None:
		{
			FS_ENGINE_ASSERT(false, "RenderAPI::None is not supported");
			return nullptr;
		}
		case RenderAPI::OpenGL:
		{
			return new OpenGLVertexBuffer(size, vertices);
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

	IndexBuffer* IndexBuffer::Create(uint32_t size, uint32_t* indices)
	{
		switch (Renderer::GetAPI())
		{
		case RenderAPI::None:
		{
			FS_ENGINE_ASSERT(false, "RenderAPI::None is not supported");
			return nullptr;
		}
		case RenderAPI::OpenGL:
		{
			return new OpenGLIndexBuffer(size, indices);
		}
		default:
		{
			FS_ENGINE_ASSERT(false, "Unknown render API");
			return nullptr;
		}
		}
	}
}