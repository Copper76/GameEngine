#include "fspch.h"
#include "RenderSystem.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/ECS/GeneralManager.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	void RenderSystem::OnRender()
	{
		glm::vec2 coords[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
		for (auto const& entity : m_Entities)
		{
			auto renderData = GeneralManager::GetComponent<Render>(entity);

			//if (!renderData.IsActive)
			//{
			//	continue;
			//}

			glm::mat4 transform = TransformSystem::GetWorldTransformMatrix(entity);

			glm::vec2* texCoords = renderData.TexCoords;

			switch (renderData.Shape->GetType())
			{
			case ShapeType::SHAPE_CUBE:
				{
				if (texCoords == nullptr)
				{
					texCoords = coords;
				}
				RenderShapeCube* shape = (RenderShapeCube*)renderData.Shape;
				Renderer::DrawCube(transform, shape->GetVertexCoords(), renderData.TilingFactor, renderData.Texture, texCoords, renderData.Colour, renderData.Normal);
				break;
				}
			case ShapeType::SHAPE_CONVEX:
				{
				RenderShapeConvex* shape = (RenderShapeConvex*)renderData.Shape;
				Renderer::DrawConvex(transform, shape->GetVertexCoords(), shape->GetTris(), renderData.Colour, renderData.Normal);
				}
				break;
			case ShapeType::SHAPE_SPHERE:
			{
				RenderShapeSphere* shape = (RenderShapeSphere*)renderData.Shape;
				Renderer::DrawSphere(transform, shape->GetVertexCoords(), shape->GetDivisions(), renderData.TilingFactor, renderData.Texture, renderData.Colour, renderData.Normal);
				break;
			}
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM RENDERING")
			}
		}
	}
}
