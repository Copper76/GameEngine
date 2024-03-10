#include <Fengshui.h>

//External includes
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public Fengshui::Layer
{
public:
	//ExampleLayer() : Layer("Example"), m_Camera(-1.0f, 1.0f, -1.0f, 1.0f), m_CameraPos(0.0f), m_SquareTransform(0.0f)
	ExampleLayer() : Layer("Example"), m_SquareTransform(0.0f), m_CameraPos(0.0f), m_Camera(0)
	{
		m_VertexArray.reset(Fengshui::VertexArray::Create());

		float vertices[3 * 10] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		};

		std::shared_ptr<Fengshui::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Fengshui::VertexBuffer::Create(sizeof(vertices), vertices));

		{
			Fengshui::BufferLayout layout({
				{Fengshui::ShaderDataType::Vec3, "a_Position"},
				{Fengshui::ShaderDataType::Vec4, "a_Color"},
				{Fengshui::ShaderDataType::Vec3, "a_Normal"},
				});

			vertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<Fengshui::IndexBuffer> indexBuffer;
		indexBuffer.reset(Fengshui::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Fengshui::VertexArray::Create());

		float square_Vertices[4 * 7] =
		{
			-0.7f, -0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.7f, -0.7f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.7f, 0.7f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.7f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<Fengshui::VertexBuffer> squareVB;
		squareVB.reset(Fengshui::VertexBuffer::Create(sizeof(square_Vertices), square_Vertices));

		{
			Fengshui::BufferLayout layout({
				{Fengshui::ShaderDataType::Vec3, "a_Position"},
				{Fengshui::ShaderDataType::Vec4, "a_Color"},

				});

			squareVB->SetLayout(layout);
		}

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t square_Indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Fengshui::IndexBuffer> squareIB;
		squareIB.reset(Fengshui::IndexBuffer::Create(sizeof(square_Indices) / sizeof(uint32_t), square_Indices));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 450 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
				
			}
		)";

		std::string fragmentSource = R"(
			#version 450 core

			layout(location = 0) out vec4 colour;

			in vec3 v_Position;
			in vec4 v_Color;

			void main()
			{
				//colour = vec4((v_Position * 0.5 + 0.5), 1.0);
				colour = v_Color;
			}
		)";

		m_Shader = std::make_unique<Fengshui::Shader>(vertexSource, fragmentSource);
		Fengshui::RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });

		//m_Scene = std::make_unique<Fengshui::Scene>();
	}

	void OnUpdate(float dt) override
	{
		//FS_TRACE("Delta Time: {0}s", deltaTime);

		//Clear the screen
		Fengshui::RenderCommand::Clear();

		//m_Scene->OnUpdate(dt);

		//m_Scene->GetCameraComponent().SetPosition(m_CameraPos);
		m_Camera.SetPosition(m_CameraPos);

		//Render cycle
		//Fengshui::Renderer::BeginScene(m_Scene->GetCameraComponent());
		Fengshui::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int i = -2; i < 3; i++)
		{
			glm::vec3 pos(i * 0.3f, 0.0f, 0.0f);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
			Fengshui::Renderer::Submit(m_Shader, m_SquareVA, transform);
		}


		Fengshui::Renderer::Submit(m_Shader, m_VertexArray);

		Fengshui::Renderer::EndScene();

		//Input handling
		if (Fengshui::Input::IsKeyPressed(FS_KEY_W))
		{
			m_CameraPos.y += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_S))
		{
			m_CameraPos.y -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_A))
		{
			m_CameraPos.x -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_D))
		{
			m_CameraPos.x += cameraMoveSpeed * dt;
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
			m_SquareTransform.y += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_K))
		{
			m_SquareTransform.y -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_J))
		{
			m_SquareTransform.x -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_L))
		{
			m_SquareTransform.x += cameraMoveSpeed * dt;
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
		//ImGui::Begin("TEST");
		//ImGui::Text("Hello World");
		//ImGui::End();
	}

	void OnEvent(Fengshui::Event& e) override
	{

	}

private:
	std::shared_ptr<Fengshui::Shader> m_Shader;
	std::shared_ptr<Fengshui::VertexArray>  m_VertexArray;

	std::shared_ptr<Fengshui::VertexArray> m_SquareVA;

	glm::vec3 m_CameraPos;
	float cameraMoveSpeed = 5.0f;

	glm::vec3 m_SquareTransform;

	//std::unique_ptr<Fengshui::Scene> m_Scene;
	Fengshui::CameraComponent m_Camera;
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