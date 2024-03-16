#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"
#include "Fengshui/ECS/Components/RenderComponent.h"
#include "Fengshui/ECS/Components/TransformComponent.h"
#include "Fengshui/Renderer/VertexArray.h"
#include "Fengshui/Renderer/Texture.h"

namespace Fengshui
{
	class RenderComponent2D : public Component
	{
	public:
		RenderComponent2D();

		virtual ~RenderComponent2D() = default;

		void OnUpdate(Ref<TransformComponent> transform);

		COMPONENT_CLASS_TYPE(ComponentRender);

		inline void Bind() const { m_VertexArray->Bind(); };
		inline void Unbind() const { m_VertexArray->Unbind(); };

		inline void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { m_VertexArray->AddVertexBuffer(vertexBuffer); };
		inline void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) { m_VertexArray->SetIndexBuffer(indexBuffer); };

		inline void SetTexture(Ref<Texture2D> texture) { m_Texture = texture; }
		inline void SetShaderName(std::string name) { m_ShaderName = name; }

		inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { m_VertexArray->GetVertexBuffers(); };
		inline const Ref<IndexBuffer>& GetIndexBuffer() const { m_VertexArray->GetIndexBuffer(); };

		inline const Ref<VertexArray> Get() const { return m_VertexArray; }

		inline const Ref<Texture2D> GetTexture() const { return m_Texture; }
		inline const std::string GetShaderName() const { return m_ShaderName;  }

	private:
		Ref<VertexArray> m_VertexArray;
		Ref<Texture2D> m_Texture;
		std::string m_ShaderName;
	};
}

