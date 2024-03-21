#include "fspch.h"
#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/Renderer/Renderer.h"
#include "RenderComponent2D.h"

namespace Fengshui
{
	RenderComponent2D::RenderComponent2D(Ref<Texture2D> texture, RenderShape shape)
		:m_Texture(texture), m_Shape(shape)
	{

	}

	void RenderComponent2D::OnUpdate(Ref<TransformComponent2D> transform)
	{
		switch (m_Shape)
		{
		case RenderShape::Quad:
			Renderer2D::DrawQuad(transform, m_Texture, m_Colour);
			return;
		default:
			FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
		}
	}
}