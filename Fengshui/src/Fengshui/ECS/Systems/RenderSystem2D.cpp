#include "fspch.h"
#include "RenderSystem2D.h"

#include "Fengshui/Renderer/Renderer2D.h"
#include "Fengshui/ECS/GeneralManager.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	void RenderSystem2D::OnRender()
	{
		glm::vec2 coords[] = { {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} };
		for (auto const& entity : m_Entities)
		{
			auto renderData = GeneralManager::GetComponent<Render2D>(entity);

			glm::mat4 transform = TransformSystem::GetWorldTransformMatrix2D(entity);

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
