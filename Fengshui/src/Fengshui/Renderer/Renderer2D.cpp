#include "fspch.h"
#include "Renderer2D.h"
#include "Fengshui/Renderer/RenderCommand.h"

namespace Fengshui
{
	struct Renderer2DConfig
	{
		Ref<VertexArray> quadVertexArray;
		Ref<Shader> standardShader;
		Ref<Texture2D> defaultTexture;
	};

	static Renderer2DConfig* s_Data;

	void Renderer2D::Init()
	{
		//s_Data.reset(new Renderer2DConfig);
		s_Data = new Renderer2DConfig();

		s_Data->quadVertexArray = VertexArray::Create();

		float vertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};

		Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(sizeof(vertices), vertices);

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				{ShaderDataType::Vec2, "a_TexCoord"},
				});

			vertexBuffer->SetLayout(layout);
		}

		s_Data->quadVertexArray->AddVertexBuffer(vertexBuffer);


		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);

		s_Data->quadVertexArray->SetIndexBuffer(indexBuffer);

		s_Data->standardShader = Shader::Create("Assets/Shaders/StandardShader2D.glsl");
		s_Data->standardShader->Bind();
		s_Data->standardShader->SetInt("u_Texture", 0);

		s_Data->defaultTexture = Texture2D::Create(1, 1);
		uint32_t whieTextureData = 0xffffffff;
		s_Data->defaultTexture->SetData(&whieTextureData, sizeof(uint32_t));
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::BeginScene(Ref<Scene> scene)
	{
		s_Data->standardShader->Bind();
		s_Data->standardShader->SetMat4("u_ViewProjectionMatrix", scene->GetCameraComponent()->GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& colour)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, rotation, texture, colour);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const float rotation, const Ref<Texture2D>& texture, const glm::vec4& colour)
	{
		if (texture)
		{
			texture->Bind(0);
		}
		else
		{
			s_Data->defaultTexture->Bind(0);
		}

		s_Data->standardShader->SetVec4("u_Colour", colour);

		glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), position), glm::radians(rotation), { 0.0f, 0.0f, 1.0f }), { size.x, size.y, 1.0f });
		s_Data->standardShader->SetMat4("u_Transform", transform);

		s_Data->quadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);
	}

	void Renderer2D::DrawQuad(Ref<TransformComponent2D> transform, const Ref<Texture2D>& texture, const glm::vec4& colour)
	{
		if (texture)
		{
			texture->Bind(0);
		}
		else
		{
			s_Data->defaultTexture->Bind(0);
		}

		s_Data->standardShader->SetVec4("u_Colour", colour);
		s_Data->standardShader->SetMat4("u_Transform", transform->GetTransform());

		s_Data->quadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->quadVertexArray);
	}
}
