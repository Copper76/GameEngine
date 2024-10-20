#pragma once

#include "Fengshui/Renderer/RendererAPI.h"

namespace Fengshui
{
	class RenderCommand
	{
	public:

		inline static void Init()
		{
			s_RendererAPI->Init();
		}

		inline static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_RendererAPI->SetViewport(x, y, width, height);
		}

		inline static void SetClearColour(const glm::vec4& colour)
		{
			s_RendererAPI->SetClearColour(colour);
		};

		inline static void SetClearDepth(const float depth)
		{
			s_RendererAPI->SetClearDepth(depth);
		};

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		};

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount  = 0, bool isWireFrame = false)
		{
			s_RendererAPI->DrawIndexed(vertexArray, indexCount, isWireFrame);
		}
	private:
		static Scope<RendererAPI> s_RendererAPI;
	};
}