#include "fspch.h"
#include "Renderer2D.h"
#include "Fengshui/Renderer/RenderCommand.h"

namespace Fengshui
{
	struct Renderer2DConfig
	{
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> flatColourShader;
	};

	static Renderer2DConfig* s_Data;

	void Renderer2D::Init()
	{
		//s_Data.reset(new Renderer2DConfig);
		s_Data = new Renderer2DConfig();

		s_Data->quadVertexArray = VertexArray::Create();

		float vertices[3 * 4] =
		{
			-0.5f, -0.5f, 0.0f,
			0.5f, -0.5f, 0.0f,
			0.5f, 0.5f, 0.0f,
			-0.5f, 0.5f, 0.0f,
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices);

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				});

			vertexBuffer->SetLayout(layout);
		}

		s_Data->quadVertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);

		s_Data->quadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->flatColourShader = Shader::Create("Assets/Shaders/FlatColourShader.glsl");
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(Ref<Scene> scene)
	{
		s_Data->flatColourShader->Bind();
		s_Data->flatColourShader->SetMat4("u_ViewProjectionMatrix", scene->GetCameraComponent()->GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const glm::vec4& colour)
	{
		s_Data->flatColourShader->Bind();
		s_Data->flatColourShader->SetVec4("u_Colour", colour);
		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		s_Data->flatColourShader->SetMat4("u_Transform",transform);

		s_Data->quadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);
	}

	void Renderer2D::DrawQuad(Ref<TransformComponent2D> transform, const glm::vec4& colour)
	{
		s_Data->flatColourShader->Bind();
		s_Data->flatColourShader->SetVec4("u_Colour", colour);
		s_Data->flatColourShader->SetMat4("u_Transform", transform->GetTransform());

		s_Data->quadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);
	}
}
