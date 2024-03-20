#include "fspch.h"
#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/Renderer/Renderer.h"
#include "RenderComponent2D.h"

namespace Fengshui
{
	RenderComponent2D::RenderComponent2D(Ref<Texture2D> texture)
		:m_Texture(texture)
	{

	}

	void RenderComponent2D::OnUpdate(Ref<TransformComponent2D> transform)
	{
		if (m_Texture)
		{
			m_Texture->Bind();
			//Renderer2D::DrawTextureQuad(transform);
		}
		else
		{
			Renderer2D::DrawQuad(transform, m_Colour);
		}
	}
}