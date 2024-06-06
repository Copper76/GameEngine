#include "fspch.h"
#include "LightSystem.h"
#include "Fengshui/ECS/GeneralManager.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	void LightSystem::OnUpdate(Ref<RenderSystem> renderSystem)
	{
		for (EntityID entity : renderSystem->GetRenderedObjects())
		{
			auto& renderData = GeneralManager::GetComponent<Render>(entity);

			if (m_Entities.empty())
			{
				renderData.Normal = nullptr;
				return;
			}

			int numVertex = 0;
			std::vector<glm::vec3> vertices;

			switch (renderData.Shape->GetType())
			{
			case ShapeType::SHAPE_CUBE:
				numVertex = 8;
				break;
			case ShapeType::SHAPE_CONVEX:
				numVertex = ((RenderShapeConvex*)renderData.Shape)->GetVertexNum();
				vertices = ((RenderShapeConvex*)renderData.Shape)->GetVertexCoords();
				break;
			case ShapeType::SHAPE_SPHERE:
				numVertex = ((RenderShapeSphere*)renderData.Shape)->GetVertexNum();
				vertices = ((RenderShapeSphere*)renderData.Shape)->GetVertexCoords();
				break;
			default:
				FS_ASSERT(false, "Unsupported shape type");
			}

			renderData.Normal = new glm::vec3[numVertex];
			for (int i = 0; i < numVertex; i++)
			{
				renderData.Normal[i] = glm::vec3(1.0f);
			}

			for (EntityID lightID : m_Entities)
			{
				auto& light = GeneralManager::GetComponent<GlobalLight>(lightID);
				light.Direction = glm::normalize(light.Direction);
				switch (renderData.Shape->GetType())
				{
				case ShapeType::SHAPE_SPHERE:
					for (int i = 0; i < numVertex; i++)
					{
						renderData.Normal[i] *= std::clamp(glm::dot(glm::normalize(vertices[i]), -light.Direction), 0.0f, 1.0f) * light.Colour;
					}
					break;
				}
			}
		}
	}
}