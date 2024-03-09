#pragma once

#include "Fengshui/Renderer/RendererAPI.h"
#include "Fengshui/Renderer/RenderCommand.h"

namespace Fengshui
{

	class Renderer
	{
	public:
		static void BeginScene();
		static void EndScene();
		
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}