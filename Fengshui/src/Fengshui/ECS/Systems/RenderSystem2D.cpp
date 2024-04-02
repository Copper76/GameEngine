#include "fspch.h"
#include "RenderSystem2D.h"

#include "Fengshui/Renderer/Renderer2D.h"

namespace Fengshui
{
	void RenderSystem2D::OnUpdate(float dt)
	{
		for (auto const& entity : m_Entities)
		{
			auto& transformData = GeneralManager::GetComponent<Transform2D>(entity);
			auto& renderData = GeneralManager::GetComponent<Render2D>(entity);
			switch (renderData.Shape)
			{
			case RenderShape::Quad:
				Renderer2D::DrawQuad({ transformData.Position.x, transformData.Position.y, transformData.Depth }, transformData.Scale, transformData.Rotation, renderData.TilingFactor, renderData.Texture, renderData.Colour);
				return;
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
			}
		}
	}
}
