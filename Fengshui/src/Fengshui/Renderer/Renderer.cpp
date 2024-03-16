#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

namespace Fengshui
{
	Renderer::SceneData* Renderer::m_SceneData = new  Renderer::SceneData;

	ShaderLibrary* Renderer::m_ShaderLib = new ShaderLibrary;

	//VertexArrayLibrary* Renderer::m_VertexArrLib = new VertexArrayLibrary;

	void Renderer::Init()
	{
		RenderCommand::Init();
	}

	void Renderer::BeginScene(Ref<CameraComponent> camera)
	{
		m_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->SetMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}