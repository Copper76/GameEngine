#include "fspch.h"
#include "RenderSystem2D.h"

#include "Fengshui/Renderer/Renderer2D.h"

namespace Fengshui
{
	void RenderSystem2D::OnRender()
	{
		glm::vec2 coords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		for (auto const& entity : m_Entities)
		{
			auto& tag = GeneralManager::GetComponent<Tag>(entity);
			auto& transformData = GeneralManager::GetComponent<Transform2D>(entity);
			auto& renderData = GeneralManager::GetComponent<Render2D>(entity);

			glm::vec2* texCoords = renderData.TexCoords;
			if (texCoords == nullptr)
			{
				texCoords = coords;
			}

			switch (renderData.Shape)
			{
			case RenderShape::Quad:
				Renderer2D::DrawQuad(transformData.Position, transformData.Scale, transformData.Rotation, renderData.TilingFactor, renderData.Texture, texCoords, renderData.Colour);
				break;
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
			}
		}
	}
}
