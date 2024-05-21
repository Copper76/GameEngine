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
			auto transformData = GeneralManager::GetComponent<Transform2D>(entity);
			auto renderData = GeneralManager::GetComponent<Render2D>(entity);
			auto hierarchyData = GeneralManager::GetComponent<Hierarchy>(entity);

			EntityID curr = entity;

			glm::mat4 transform = glm::identity<glm::mat4>();

			while (curr != 0)
			{
				//Allow trasnform change via 3d transform as well
				if (GeneralManager::HasComponent<Transform>(curr))
				{
					transform = GeneralManager::GetComponent<Transform>(curr).GetTransform() * transform;
				}
				else
				{
					if (GeneralManager::HasComponent<Transform2D>(curr))
					{
						transform = GeneralManager::GetComponent<Transform2D>(curr).GetTransform() * transform;
					}
				}
				hierarchyData = GeneralManager::GetComponent<Hierarchy>(curr);
				curr = hierarchyData.Parent;
			}

			glm::vec2* texCoords = renderData.TexCoords;
			if (texCoords == nullptr)
			{
				texCoords = coords;
			}

			switch (renderData.Shape)
			{
			case RenderShape2D::Quad:
				Renderer2D::DrawQuad(transform, renderData.TilingFactor, renderData.Texture, texCoords, renderData.Colour);
				break;
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
			}
		}
	}
}
