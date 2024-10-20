#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "Fengshui/Renderer/VertexArray.h"

namespace Fengshui
{
	class RendererAPI
	{
	public:
		enum class API
		{
			None = 0, OpenGL = 1,
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;

		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

		virtual void SetClearColour(const glm::vec4& colour) = 0;
		virtual void SetClearDepth(const float depth) = 0;
		virtual void Clear() = 0;

		virtual void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount, bool isWireFrame) = 0;

		inline static API GetAPI() { return s_API; }

	private:
		static API s_API;
	};
}