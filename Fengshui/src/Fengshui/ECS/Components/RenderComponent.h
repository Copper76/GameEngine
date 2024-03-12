#pragma once

#include "Fengshui/ECS/Components/BaseComponent.h"
#include "Fengshui/Renderer/VertexArray.h"

namespace Fengshui
{
	class RenderComponent : public Component
	{
	public:
		RenderComponent();

		virtual ~RenderComponent() = default;

		COMPONENT_CLASS_TYPE(ComponentRender);

		inline void Bind() const { m_VertexArray->Bind(); };
		inline void Unbind() const { m_VertexArray->Unbind(); };

		inline void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) { m_VertexArray->AddVertexBuffer(vertexBuffer); };
		inline void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) { m_VertexArray->SetIndexBuffer(indexBuffer); };

		inline const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const { m_VertexArray->GetVertexBuffers(); };
		inline const Ref<IndexBuffer>& GetIndexBuffer() const { m_VertexArray->GetIndexBuffer(); };

		inline const Ref<VertexArray> Get() const { return m_VertexArray; }

	private:
		Ref<VertexArray> m_VertexArray;
	};
}

