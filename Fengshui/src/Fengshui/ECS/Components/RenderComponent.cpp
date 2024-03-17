#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "RenderComponent.h"

namespace Fengshui
{
	RenderComponent2D::RenderComponent2D() : m_VertexArray(VertexArray::Create())
	{
		m_Shader = Renderer::GetShaderLib()->Get("Shader");
	}

	RenderComponent2D::RenderComponent2D(Ref<VertexArray> vertexArray, Ref<Shader> shader, Ref<Texture2D> texture)
		: m_VertexArray(vertexArray), m_Shader(shader), m_Texture(texture)
	{
		if (m_Shader == nullptr)
		{
			m_Shader = Renderer::GetShaderLib()->Get("Shader");
		}
	}

	RenderComponent2D::RenderComponent2D(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, Ref<Texture2D> texture)
		: m_VertexArray(VertexArray::Create()), m_Shader(shader), m_Texture(texture)
	{
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		if (m_Shader == nullptr)
		{
			m_Shader = Renderer::GetShaderLib()->Get("Shader");
		}
	}

	void RenderComponent2D::OnUpdate(Ref<TransformComponent> transform)
	{
		if (m_Shader)
		{
			m_Shader->Bind();
		}

		if (m_Texture)
		{
			m_Texture->Bind();
		}
		Fengshui::Renderer::Submit(m_Shader, m_VertexArray, transform->GetTransform());
	}

}