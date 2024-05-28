#pragma once

#include "Fengshui/Renderer/RendererAPI.h"
#include "Fengshui/Renderer/RenderCommand.h"
#include "Fengshui/Core/Scene.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/VertexArray.h"

namespace Fengshui
{

	class Renderer
	{
	public:

		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void BeginScene(CameraComponent* camera);
		static void EndScene();

		static void Flush();

		static void DrawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const float tilingFactor = 1.0f, const Ref<Texture>& texture = nullptr, const glm::vec2* texCoords = nullptr, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawCube(const glm::mat4 transform, const float tilingFactor = 1.0f, const Ref<Texture>& texture = nullptr, const glm::vec2* texCoords = nullptr, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });

		static void DrawConvex(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const std::vector<glm::vec3>& vertexCoords, const std::vector<Triangle>& tris, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });
		static void DrawConvex(const glm::mat4 transform, const std::vector<glm::vec3>& vertexCoords, const std::vector<Triangle>& tris, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });

	private:
		static void PrepareNextBatch();
	};
}