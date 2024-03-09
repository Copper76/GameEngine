#pragma once

#include "Fengshui/Renderer/RendererAPI.h"

namespace Fengshui
{
	class OpenGLRenderer : public RendererAPI
	{
	public:
		virtual void SetClearColour(const glm::vec4& color) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray) override;
	};
}