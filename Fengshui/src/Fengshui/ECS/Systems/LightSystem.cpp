#include "fspch.h"
#include "LightSystem.h"
#include "Fengshui/ECS/GeneralManager.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	void LightSystem::OnUpdate(Ref<RenderSystem> renderSystem)
	{
		if (m_Entities.empty())
		{
			if (m_NoLightLastFrame)
			{
				return;
			}
			else
			{
				for (EntityID entity : renderSystem->GetRenderedObjects())
				{
					auto& renderData = GeneralManager::GetComponent<Render>(entity);
					renderData.Normal = nullptr;
				}
				m_NoLightLastFrame = true;
			}
		}
		else
		{
			for (EntityID entity : renderSystem->GetRenderedObjects())
			{
				const Transform& transformData = TransformSystem::GetWorldTransform(entity);
				auto& renderData = GeneralManager::GetComponent<Render>(entity);

				int numVertex = 0;
				std::vector<glm::vec3> vertices;

				switch (renderData.Shape->GetType())
				{
				case ShapeType::SHAPE_CUBE:
					numVertex = 24;
					vertices = renderData.Shape->GetVertexCoords();
					break;
				case ShapeType::SHAPE_CONVEX:
					numVertex = ((RenderShapeConvex*)renderData.Shape)->GetVertexNum();
					vertices = renderData.Shape->GetVertexCoords();
					break;
				case ShapeType::SHAPE_SPHERE:
					numVertex = ((RenderShapeSphere*)renderData.Shape)->GetVertexNum();
					vertices = renderData.Shape->GetVertexCoords();
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
					const Transform& lightTransform = TransformSystem::GetWorldTransform(lightID);
					auto& light = GeneralManager::GetComponent<Light>(lightID);
					switch (light.Type)
					{
					case LightType::GlobalLight:
						for (int i = 0; i < numVertex; i++)
						{
							renderData.Normal[i] *= std::max(glm::dot(glm::normalize((transformData.Rotation * vertices[i]) * transformData.Scale), -light.Direction), 0.0f) * light.Colour;
						}
						break;
					case LightType::PointLight:
						{
						glm::vec3 lightPos = (lightTransform.Rotation * lightTransform.Position) * lightTransform.Scale;
						for (int i = 0; i < numVertex; i++)
						{
							glm::vec3 lightDirection = glm::normalize(lightPos - ((transformData.Rotation * (transformData.Position + vertices[i])) * transformData.Scale));
							renderData.Normal[i] *= std::max(glm::dot(glm::normalize((transformData.Rotation * vertices[i]) * transformData.Scale), lightDirection), 0.0f) * light.Colour;
						}
						}
						break;
					}
				}
			}
			m_NoLightLastFrame = false;
		}
	}
}