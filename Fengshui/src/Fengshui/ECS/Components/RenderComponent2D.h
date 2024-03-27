#pragma once
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/ECS/Components/TransformComponent2D.h"
#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"

#include "glm/glm.hpp"

namespace Fengshui
{
	enum class RenderShape
	{
		Custom, Quad,
	};
	class RenderComponent2D : public Component
	{
	public:
		RenderComponent2D(Ref<Texture2D> texture = nullptr, RenderShape shape = RenderShape::Quad);

		virtual ~RenderComponent2D() = default;

		void OnUpdate(Ref<TransformComponent2D> transform);

		COMPONENT_CLASS_TYPE(ComponentRender2D);

		inline void SetTexture(Ref<Texture2D> texture) { m_Texture = texture; }

		inline void SetColour(glm::vec4 colour) { m_Colour = colour; }

		inline void SetTilingFactor(float factor) { m_TilingFactor = factor; }

		inline const Ref<Texture2D> GetTexture() const { return m_Texture; }

	private:
		Ref<Texture2D> m_Texture;
		glm::vec4 m_Colour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_TilingFactor = 1.0f;
		RenderShape m_Shape;
	};
}