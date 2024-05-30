#include "fspch.h"
#include "RenderSystem.h"

#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/ECS/GeneralManager.h"

namespace Fengshui
{
	void RenderSystem::OnRender(Ref<TransformSystem> transformSystem)
	{
		glm::vec2 coords[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f}};
		for (auto const& entity : m_Entities)
		{
			auto renderData = GeneralManager::GetComponent<Render>(entity);

			glm::mat4 transform = transformSystem->GetWorldTransformMatrix(entity);

			glm::vec2* texCoords = renderData.TexCoords;

			switch (renderData.Shape->GetType())
			{
			case ShapeType::SHAPE_CUBE:
				if (texCoords == nullptr)
				{
					texCoords = coords;
				}
				Renderer::DrawCube(transform, renderData.TilingFactor, renderData.Texture, texCoords, renderData.Colour);
				break;
			case ShapeType::SHAPE_CONVEX:
				{
				RenderShapeConvex* shape = (RenderShapeConvex*)renderData.Shape;
				Renderer::DrawConvex(transform, shape->GetVertexCoords(), shape->GetTris(), renderData.Colour);
				}
				break;
			case ShapeType::SHAPE_SPHERE:
			{
				RenderShapeSphere* shape = (RenderShapeSphere*)renderData.Shape;
				//Renderer::DrawConvex(transform, shape->GetVertexCoords(), shape->GetTris(), renderData.Colour);
				break;
			}
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM RENDERING")
			}
		}
	}
}
