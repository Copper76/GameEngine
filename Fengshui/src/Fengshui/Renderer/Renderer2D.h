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

		static void BeginScene(CameraComponent* scene);
		static void EndScene();

		static void Flush();

		//Primitives

		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation = 0.0f, const float tilingFactor = 1.0f, const Ref<Texture2D>& texture = nullptr, const glm::vec2* texCoords = nullptr,  const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation = 0.0f, const float tilingFactor = 1.0f, const Ref<Texture2D>& texture = nullptr, const glm::vec2* texCoords = nullptr, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });

	private:
		static void PrepareNextBatch();
	};
}