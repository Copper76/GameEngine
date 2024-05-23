#include "fspch.h"
#include "RenderSystem.h"

#include "Fengshui/Renderer/Renderer.h"

namespace Fengshui
{
	void RenderSystem::OnRender(Ref<TransformSystem> transformSystem)
	{
		glm::vec2 coords[] = { {0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f},{0.0f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f}, {1.0f, 0.0f} };
		for (auto const& entity : m_Entities)
		{
			auto renderData = GeneralManager::GetComponent<Render>(entity);

			glm::mat4 transform = transformSystem->GetWorldTransformMatrix(entity);

			glm::vec2* texCoords = renderData.TexCoords;
			if (texCoords == nullptr)
			{
				texCoords = coords;
			}

			switch (renderData.Shape)
			{
			case RenderShape::Cube:
				Renderer::DrawCube(transform, renderData.TilingFactor, renderData.Texture, texCoords, renderData.Colour);
				break;
			default:
				FS_ENGINE_ASSERT(false, "NO SUPPORT FOR CUSTOM 2D RENDERING")
			}
		}
	}
}
