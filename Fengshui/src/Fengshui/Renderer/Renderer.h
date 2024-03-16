#pragma once

#include "Fengshui/Renderer/RendererAPI.h"
#include "Fengshui/Renderer/RenderCommand.h"
#include "Fengshui/ECS/Components/CameraComponent.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/VertexArray.h"

namespace Fengshui
{

	class Renderer
	{
	public:

		static void Init();

		static void BeginScene(Ref<CameraComponent> camera);
		static void EndScene();
		
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		inline static ShaderLibrary* GetShaderLib() { return m_ShaderLib; }

		//inline static VertexArrayLibrary* GetVertexArrayLib() { return m_VertexArrLib; }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static SceneData* m_SceneData;

		static ShaderLibrary* m_ShaderLib;
		//static VertexArrayLibrary* m_VertexArrLib;
	};
}