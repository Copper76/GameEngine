#pragma once

#include "Fengshui/Renderer/RendererAPI.h"

namespace Fengshui
{
	class RenderCommand
	{
	public:

		inline static void SetClearColour(const glm::vec4& colour)
		{
			s_RendererAPI->SetClearColour(colour);
		};

		inline static void Clear()
		{
			s_RendererAPI->Clear();
		};

		inline static void DrawIndexed(const Ref<VertexArray>& vertexArray)
		{
			s_RendererAPI->DrawIndexed(vertexArray);
		}
	private:
		static RendererAPI* s_RendererAPI;
	};
}