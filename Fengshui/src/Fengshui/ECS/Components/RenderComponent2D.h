#pragma once
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/ECS/Components/TransformComponent2D.h"
#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"

namespace Fengshui
{
	class RenderComponent2D : public Component
	{
	public:
		RenderComponent2D(Ref<Texture2D> texture = nullptr);

		virtual ~RenderComponent2D() = default;

		void OnUpdate(Ref<TransformComponent2D> transform);

		COMPONENT_CLASS_TYPE(ComponentRender2D);

		inline void SetTexture(Ref<Texture2D> texture) { m_Texture = texture; }

		inline const Ref<Texture2D> GetTexture() const { return m_Texture; }

	private:
		Ref<Texture2D> m_Texture;
	};
}