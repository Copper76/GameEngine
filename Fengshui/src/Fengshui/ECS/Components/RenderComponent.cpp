#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "RenderComponent.h"

namespace Fengshui
{
	RenderComponent::RenderComponent() : m_VertexArray(VertexArray::Create())
	{
		m_Shader = Renderer::GetShaderLib()->Get("Shader");
	}

	RenderComponent::RenderComponent(Ref<VertexArray> vertexArray, Ref<Shader> shader, Ref<Texture> texture)
		: m_VertexArray(vertexArray), m_Shader(shader), m_Texture(texture)
	{
		if (m_Shader == nullptr)
		{
			m_Shader = Renderer::GetShaderLib()->Get("Shader");
		}
	}

	RenderComponent::RenderComponent(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer, Ref<Shader> shader, Ref<Texture> texture)
		: m_VertexArray(VertexArray::Create()), m_Shader(shader), m_Texture(texture)
	{
		m_VertexArray->AddVertexBuffer(vertexBuffer);
		m_VertexArray->SetIndexBuffer(indexBuffer);

		if (m_Shader == nullptr)
		{
			m_Shader = Renderer::GetShaderLib()->Get("Shader");
		}
	}

	void RenderComponent::OnUpdate(Ref<TransformComponent> transform)
	{
		if (m_Shader)
		{
			m_Shader->Bind();
		}

		if (m_Texture)
		{
			m_Texture->Bind(0);
		}
		Renderer::Submit(m_Shader, m_VertexArray, transform->GetTransform());
	}
}