#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "RenderComponent.h"

namespace Fengshui
{
	RenderComponent2D::RenderComponent2D()
	{
		m_VertexArray = VertexArray::Create();
	}

	void RenderComponent2D::OnUpdate(Ref<TransformComponent> transform)
	{
		auto shader = Fengshui::Renderer::GetShaderLib()->Get(m_ShaderName);
		if (shader)
		{
			shader->Bind();
		}

		if (m_Texture)
		{
			m_Texture->Bind();
		}
		Fengshui::Renderer::Submit(shader, m_VertexArray, transform->GetTransform());
	}

}