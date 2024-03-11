#include "fspch.h"
#include "Fengshui/Renderer/Renderer.h"
#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

namespace Fengshui
{
	Renderer::SceneData* Renderer::m_SceneData = new  Renderer::SceneData;
	void Renderer::BeginScene(CameraComponent camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);
		std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}
}