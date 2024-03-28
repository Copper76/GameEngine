#include "fspch.h"
#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/Renderer/Renderer.h"
#include "RenderComponent2D.h"

namespace Fengshui
{
	RenderComponent2D::RenderComponent2D(Ref<Texture2D> texture, RenderShape shape)
		: m_Texture(texture), m_TexCoords(nullptr), m_Shape(shape)
	{

	}

	RenderComponent2D::RenderComponent2D(Ref<SubTexture2D> texture, RenderShape shape)
		: m_Texture(texture->GetTexture()), m_TexCoords(texture->GetTexCoords()), m_Shape(shape)
	{

	}

	void RenderComponent2D::OnUpdate(Ref<TransformComponent2D> transform)
	{
		switch (m_Shape)
		{
		case RenderShape::Quad:
			Renderer2D::DrawQuad(transform, m_TilingFactor, m_Texture, m_TexCoords, m_Colour);
			return;
		default:
			FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
		}
	}
}