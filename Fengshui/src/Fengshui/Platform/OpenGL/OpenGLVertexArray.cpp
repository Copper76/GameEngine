#include "fspch.h"

#include "Fengshui/Platform/OpenGL/OpenGLVertexArray.h"
#include "glad/glad.h"

namespace Fengshui
{

	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:   return GL_FLOAT;
			case ShaderDataType::Vec2:    return GL_FLOAT;
			case ShaderDataType::Vec3:    return GL_FLOAT;
			case ShaderDataType::Vec4:    return GL_FLOAT;
			case ShaderDataType::Int:     return GL_INT;
			case ShaderDataType::IntVec2: return GL_INT;
			case ShaderDataType::IntVec3: return GL_INT;
			case ShaderDataType::IntVec4: return GL_INT;
			case ShaderDataType::Mat3:    return GL_FLOAT;
			case ShaderDataType::Mat4:    return GL_FLOAT;
			case ShaderDataType::Bool:    return GL_BOOL;
			default:
			{
				FS_ENGINE_ASSERT(false, "Unknown Shader Data Type");
				return 0;
			}
		}
	}

	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);
	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RendererID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer)
	{
		glBindVertexArray(m_RendererID);
		vertexBuffer->Bind();

		FS_ENGINE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex buffer layout cannot be empty");

		uint32_t index = 0;
		auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, element.GetComponentCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)element.Offset);
			index++;
		}
		m_VertexBuffers.push_back(vertexBuffer);
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}