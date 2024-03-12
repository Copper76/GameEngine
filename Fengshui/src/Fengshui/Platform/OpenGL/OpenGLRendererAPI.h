#pragma once

#include "Fengshui/Renderer/RendererAPI.h"

namespace Fengshui
{
	class OpenGLRenderer : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetClearColour(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray) override;
	};
}