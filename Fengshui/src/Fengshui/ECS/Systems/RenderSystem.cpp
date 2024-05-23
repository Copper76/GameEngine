#include "fspch.h"
#include "RenderSystem.h"

#include "Fengshui/Renderer/Renderer.h"

namespace Fengshui
{
	void RenderSystem::OnRender(Ref<TransformSystem> transformSystem)
	{
		glm::vec2 coords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		for (auto const& entity : m_Entities)
		{
			//auto transformData = GeneralManager::GetComponent<Transform>(entity);
			auto renderData = GeneralManager::GetComponent<Render>(entity);
			//auto hierarchyData = GeneralManager::GetComponent<Hierarchy>(entity);

			/*EntityID curr = entity;

			glm::mat4 transform = glm::identity<glm::mat4>();

			while (curr != 0)
			{
				if (GeneralManager::HasComponent<Transform>(curr))
				{
					transform = GeneralManager::GetComponent<Transform>(curr).GetTransform() * transform;
				}
				hierarchyData = GeneralManager::GetComponent<Hierarchy>(curr);
				curr = hierarchyData.Parent;
			}*/

			glm::mat4 transform = transformSystem->GetWorldTransformMatrix(entity);

			switch (renderData.Shape)
			{
			case RenderShape::Cube:
				Renderer::DrawCube(transform, renderData.TilingFactor, renderData.Texture, renderData.TexCoords, renderData.Colour);
				break;
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
			}
		}
	}
}
