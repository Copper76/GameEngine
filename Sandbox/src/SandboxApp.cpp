#include <Fengshui.h>

//External includes
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Fengshui/Platform/OpenGL/OpenGLShader.h"

class ExampleLayer : public Fengshui::Layer
{
public:
	//ExampleLayer() : Layer("Example"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), m_CameraPos(0.0f), m_SquareTransform(0.0f)
	ExampleLayer() : Layer("Example"), m_Camera(0, -1.0f, 1.0f, -1.0f, 1.0f), m_CameraPos(1), m_SquareTransform(2)
	{
		m_VertexArray.reset(Fengshui::VertexArray::Create());

		float vertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};

		Fengshui::Ref<Fengshui::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Fengshui::VertexBuffer::Create(sizeof(vertices), vertices));

		{
			Fengshui::BufferLayout layout({
				{Fengshui::ShaderDataType::Vec3, "a_Position"},
				{Fengshui::ShaderDataType::Vec2, "a_TexCoord"},
				});

			vertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		Fengshui::Ref<Fengshui::IndexBuffer> indexBuffer;
		indexBuffer.reset(Fengshui::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Fengshui::VertexArray::Create());

		float square_Vertices[4 * 3] =
		{
			-0.7f, -0.7f, 0.0f,
			0.7f, -0.7f, 0.0f,
			0.7f, 0.7f, 0.0f,
			-0.7f, 0.7f, 0.0f
		};

		Fengshui::Ref<Fengshui::VertexBuffer> squareVB;
		squareVB.reset(Fengshui::VertexBuffer::Create(sizeof(square_Vertices), square_Vertices));

		{
			Fengshui::BufferLayout layout({
				{Fengshui::ShaderDataType::Vec3, "a_Position"},
				});

			squareVB->SetLayout(layout);
		}

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t square_Indices[6] = { 0, 1, 2, 2, 3, 0 };
		Fengshui::Ref<Fengshui::IndexBuffer> squareIB;
		squareIB.reset(Fengshui::IndexBuffer::Create(sizeof(square_Indices) / sizeof(uint32_t), square_Indices));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 450 core

			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			void main()
			{
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
				
			}
		)";

		std::string fragmentSource = R"(
			#version 450 core

			layout(location = 0) out vec4 colour;

			uniform vec4 u_Colour;

			void main()
			{
				colour = u_Colour;
			}
		)";

		m_Shader.reset(Fengshui::Shader::Create(vertexSource, fragmentSource));

		std::string texVertexSource = R"(
			#version 450 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string texFragmentSource = R"(
			#version 450 core

			layout(location = 0) out vec4 colour;

			in vec2 v_TexCoord;

			uniform vec4 u_Colour;

			void main()
			{
				colour = vec4(v_TexCoord, 0.0, 1.0);
			}
		)";

		m_TexShader.reset(Fengshui::Shader::Create(texVertexSource, texFragmentSource));

		Fengshui::RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });

		m_SquareTransform.Scale = { 0.1f, 0.1f, 0.1f };
		//m_Scene = std::make_unique<Fengshui::Scene>();
	}

	void OnUpdate(float dt) override
	{
		//FS_TRACE("Delta Time: {0}s", deltaTime);

		//Clear the screen
		Fengshui::RenderCommand::Clear();

		//m_Scene->OnUpdate(dt);

		//m_Scene->GetCameraComponent().SetPosition(m_CameraPos);
		m_Camera.SetPosition(m_CameraPos.Position);

		//Render cycle
		//Fengshui::Renderer::BeginScene(m_Scene->GetCameraComponent());
		Fengshui::Renderer::BeginScene(m_Camera);

		std::dynamic_pointer_cast<Fengshui::OpenGLShader>(m_TexShader)->Bind();

		std::dynamic_pointer_cast<Fengshui::OpenGLShader>(m_Shader)->Bind();
		std::dynamic_pointer_cast<Fengshui::OpenGLShader>(m_Shader)->UploadUniformVec4("u_Colour", m_SquareColour);

		for (int i = -2; i < 3; i++)
		{
			for (int j = -2; j < 3; j++)
			{
				glm::vec3 pos(i * 0.3f, j * 0.3f, 0.0f);
				pos += m_SquareTransform.Position;
				glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), pos), glm::length(m_SquareTransform.Rotation), glm::length(m_SquareTransform.Rotation) == 0 ? glm::vec3(1.0) : glm::normalize(m_SquareTransform.Rotation)), m_SquareTransform.Scale);
				Fengshui::Renderer::Submit(m_Shader, m_SquareVA, transform);
			}
		}


		Fengshui::Renderer::Submit(m_TexShader, m_VertexArray);

		Fengshui::Renderer::EndScene();

		//Input handling
		if (Fengshui::Input::IsKeyPressed(FS_KEY_W))
		{
			m_CameraPos.Position.y += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_S))
		{
			m_CameraPos.Position.y -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_A))
		{
			m_CameraPos.Position.x -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_D))
		{
			m_CameraPos.Position.x += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_Q))
		{
			//m_Scene->GetCameraComponent().SetRotation(m_Scene->GetCameraComponent().GetRotation() + cameraMoveSpeed * dt);
			m_Camera.SetRotation(m_Camera.GetRotation() + cameraMoveSpeed * dt);
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_E))
		{
			//m_Scene->GetCameraComponent().SetRotation(m_Scene->GetCameraComponent().GetRotation() - cameraMoveSpeed * dt);
			m_Camera.SetRotation(m_Camera.GetRotation() - cameraMoveSpeed * dt);
		}

		if (Fengshui::Input::IsKeyPressed(FS_KEY_I))
		{
			m_SquareTransform.Position.y += cameraMoveSpeed * dt;
			//m_SquareTransform.Rotation.z += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_K))
		{
			m_SquareTransform.Position.y -= cameraMoveSpeed * dt;
			//m_SquareTransform.Rotation.z -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_J))
		{
			m_SquareTransform.Position.x -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_L))
		{
			m_SquareTransform.Position.x += cameraMoveSpeed * dt;
		}
	}

	void OnAttach() override
	{

	}

	void OnDetach() override
	{

	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Colour Picker");
		ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_SquareColour));
		ImGui::End();
	}

	void OnEvent(Fengshui::Event& e) override
	{

	}

private:
	Fengshui::Ref<Fengshui::Shader> m_Shader, m_TexShader;
	Fengshui::Ref<Fengshui::VertexArray>  m_VertexArray;

	Fengshui::Ref<Fengshui::VertexArray> m_SquareVA;

	Fengshui::TransformComponent m_CameraPos;
	//glm::vec3 m_CameraPos;
	float cameraMoveSpeed = 5.0f;

	Fengshui::TransformComponent m_SquareTransform;

	//std::unique_ptr<Fengshui::Scene> m_Scene;
	Fengshui::CameraComponent m_Camera;

	glm::vec4 m_SquareColour = { 0.2f, 0.3f, 0.7f, 1.0f };
};

class SandboxApp : public Fengshui::Application
{
public:
	SandboxApp() : Application()
	{
		ExampleLayer* layer = new ExampleLayer();
		PushLayer(layer);
	}

	~SandboxApp()
	{

	}
};

Fengshui::Application* Fengshui::CreateApplication()
{
	return new SandboxApp();
}