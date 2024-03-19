#pragma once
#include "Fengshui/Renderer/RenderCommand.h"
#include "Fengshui/Core/Scene.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/VertexArray.h"

namespace Fengshui
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(Ref<Scene> scene);
		static void EndScene();

		//Primitives
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& colour);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& colour);

		static void Renderer2D::DrawQuad(Ref<TransformComponent2D> transform, const glm::vec4& colour = {1.0f, 1.0f, 1.0f, 1.0f});
	};
}