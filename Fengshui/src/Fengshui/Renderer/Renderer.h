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

		static void BeginScene(CameraComponent* scene);
		static void EndScene();

		static void Flush();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void DrawQuad(const glm::vec3& position, const glm::vec3& size, const glm::vec3 rotation, const float tilingFactor = 1.0f, const Ref<Texture>& texture = nullptr, const glm::vec2* texCoords = nullptr, const glm::vec4& colour = { 1.0f, 1.0f, 1.0f, 1.0f });

	private:
		static void PrepareNextBatch();

	//	inline static ShaderLibrary* GetShaderLib() { return m_ShaderLib; }

	//	inline static VertexArrayLibrary* GetVertexArrayLib() { return m_VertexArrLib; }

	//private:
	//	struct SceneData
	//	{
	//		glm::mat4 ViewProjectionMatrix;
	//	};

	//	static SceneData* m_SceneData;

	//	static ShaderLibrary* m_ShaderLib;
	};
}