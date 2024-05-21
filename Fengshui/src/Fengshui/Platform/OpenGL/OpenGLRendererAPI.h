#pragma once

#include "Fengshui/Renderer/RendererAPI.h"

namespace Fengshui
{
	class OpenGLRenderer : public RendererAPI
	{
	public:
		virtual void Init() override;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

		virtual void SetClearColour(const glm::vec4& color) override;
		virtual void SetClearDepth(const float depth) override;
		virtual void Clear() override;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount) override;
	};
}