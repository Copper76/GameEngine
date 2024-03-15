#include <Fengshui.h>

//External includes
#include <imgui.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Fengshui::Layer
{
public:
	//ExampleLayer() : Layer("Example"), m_Camera(0, -1.0f, 1.0f, -1.0f, 1.0f), m_CameraPos(1), m_SquareTransform(2)
	ExampleLayer() : Layer("Example"), m_CameraPos(), m_SquareTransform()
	{
		m_Scene = Fengshui::Scene::Init();

		m_BigSquare = Fengshui::GameEntity(m_Scene);
		//m_VertexArray = Fengshui::RenderComponent();
		Fengshui::Ref<Fengshui::RenderComponent2D> renderComp = m_BigSquare.AddComponent<Fengshui::RenderComponent2D>();

		m_BigSquare.AddComponent<Fengshui::TransformComponent>();

		float vertices[5 * 4] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		};

		Fengshui::Ref<Fengshui::VertexBuffer> vertexBuffer = Fengshui::VertexBuffer::Create(sizeof(vertices), vertices);

		{
			Fengshui::BufferLayout layout({
				{Fengshui::ShaderDataType::Vec3, "a_Position"},
				{Fengshui::ShaderDataType::Vec2, "a_TexCoord"},
				});

			vertexBuffer->SetLayout(layout);
		}

		renderComp->AddVertexBuffer(vertexBuffer);

		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		Fengshui::Ref<Fengshui::IndexBuffer> indexBuffer = Fengshui::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);

		renderComp->SetIndexBuffer(indexBuffer);

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

		Fengshui::Ref<Fengshui::Shader> shader = Fengshui::Shader::Create(vertexSource, fragmentSource);
		shader->SetVec4("u_Colour", m_SquareColour);
		Fengshui::Renderer::GetShaderLib()->Add(shader);

		for (int i = -2; i < 3; i++)
		{
			for (int j = -2; j < 3; j++)
			{
				square = Fengshui::GameEntity(m_Scene);

				squareTrans = square.AddComponent<Fengshui::TransformComponent>();
				squareTrans->Position += glm::vec3(i * 0.3f, j * 0.3f, 0.0f);
				squareTrans->Scale = { 0.1f, 0.1f, 0.1f };

				m_SquareVA = square.AddComponent<Fengshui::RenderComponent2D>();

				FS_ENGINE_ASSERT(m_SquareVA == square.GetComponent<Fengshui::RenderComponent2D>(), "Not added");

				float square_Vertices[4 * 3] =
				{
					-0.7f, -0.7f, 0.0f,
					0.7f, -0.7f, 0.0f,
					0.7f, 0.7f, 0.0f,
					-0.7f, 0.7f, 0.0f
				};

				Fengshui::Ref<Fengshui::VertexBuffer> squareVB = Fengshui::VertexBuffer::Create(sizeof(square_Vertices), square_Vertices);

				{
					Fengshui::BufferLayout layout({
						{Fengshui::ShaderDataType::Vec3, "a_Position"},
						});

					squareVB->SetLayout(layout);
				}

				m_SquareVA->AddVertexBuffer(squareVB);

				uint32_t square_Indices[6] = { 0, 1, 2, 2, 3, 0 };
				Fengshui::Ref<Fengshui::IndexBuffer> squareIB = (Fengshui::IndexBuffer::Create(sizeof(square_Indices) / sizeof(uint32_t), square_Indices));

				m_SquareVA->SetIndexBuffer(squareIB);

				m_SquareVA->SetShaderName("Shader");
				m_Squares.emplace_back(square);
			}
		}

		//m_Shader = Fengshui::Shader::Create(vertexSource, fragmentSource);
		//m_Shader = Fengshui::Shader::Create("Assets/Shaders/FlatColourShader.glsl");

		//m_TexShader = Fengshui::Shader::Create(texVertexSource, texFragmentSource);
		//m_TexShader = Fengshui::Shader::Create("Assets/Shaders/TextureShader.glsl");
		auto textureShader = Fengshui::Renderer::GetShaderLib()->Load("Assets/Shaders/TextureShader.glsl");

		renderComp->SetShaderName("TextureShader");

		Fengshui::Ref<Fengshui::Texture2D> m_Texture = Fengshui::Texture2D::Create("Assets/Textures/Checkerboard.png", textureShader);
		//m_Texture = Fengshui::Texture2D::Create("Assets/Textures/ChernoLogo.png", m_TexShader);
		//m_Texture = Fengshui::Texture2D::Create("Assets/Textures/ChernoLogo.png", textureShader);

		renderComp->SetTexture(m_Texture);

		//m_TexShader->Bind();
		textureShader->Bind();
		//m_TexShader->SetInt("u_Texture", 0);
		textureShader->SetInt("u_Texture", 0);

		Fengshui::RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });

		m_SquareTransform.Scale = { 0.1f, 0.1f, 0.1f };
		//m_Scene = std::make_unique<Fengshui::Scene>();
	}

	void OnUpdate(float dt) override
	{

		//Clear the screen
		Fengshui::RenderCommand::Clear();

		auto shader = Fengshui::Renderer::GetShaderLib()->Get("Shader");
		shader->SetVec4("u_Colour", m_SquareColour);

		//m_Scene->OnUpdate(dt);

		m_Scene->GetCameraComponent()->SetPosition(m_CameraPos.Position);
		//m_Camera.SetPosition(m_CameraPos.Position);
		m_Scene->OnUpdate(dt);

		/**
		//Render cycle
		Fengshui::Renderer::BeginScene(*m_Scene->GetCameraComponent());
		//Fengshui::Renderer::BeginScene(m_Camera);

		
		//m_TexShader->Bind();
		auto textureShader = Fengshui::Renderer::GetShaderLib()->Get("TextureShader");
		textureShader->Bind();

		m_Texture->Bind();
		//Fengshui::Renderer::Submit(m_TexShader, m_VertexArray);
		//Fengshui::Renderer::Submit(textureShader, m_VertexArray->Get());
		Fengshui::Renderer::Submit(textureShader, m_BigSquare.GetComponent<Fengshui::RenderComponent2D>()->Get(), m_BigSquare.GetComponent<Fengshui::TransformComponent>()->GetTransform());
		
		//m_Shader->Bind();
		auto shader = Fengshui::Renderer::GetShaderLib()->Get("Shader");
		shader->Bind();
		//m_Shader->SetVec4("u_Colour", m_SquareColour);
		shader->SetVec4("u_Colour", m_SquareColour);

		for (int i = -2; i < 3; i++)
		{
			for (int j = -2; j < 3; j++)
			{
				glm::vec3 pos(i * 0.3f, j * 0.3f, 0.0f);
				pos += m_SquareTransform.Position;
				glm::mat4 transform = glm::scale(glm::rotate(glm::translate(glm::mat4(1.0f), pos), glm::length(m_SquareTransform.Rotation), glm::length(m_SquareTransform.Rotation) == 0 ? glm::vec3(1.0) : glm::normalize(m_SquareTransform.Rotation)), m_SquareTransform.Scale);
				//Fengshui::Renderer::Submit(m_Shader, m_SquareVA, transform);
				Fengshui::Renderer::Submit(shader, m_SquareVA.Get(), transform);
			}
		}

		Fengshui::Renderer::EndScene();
		**/
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
			m_Scene->GetCameraComponent()->SetRotation(m_Scene->GetCameraComponent()->GetRotation() + cameraMoveSpeed * dt);
			//m_Camera.SetRotation(m_Camera.GetRotation() + cameraMoveSpeed * dt);
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_E))
		{
			m_Scene->GetCameraComponent()->SetRotation(m_Scene->GetCameraComponent()->GetRotation() - cameraMoveSpeed * dt);
			//m_Camera.SetRotation(m_Camera.GetRotation() - cameraMoveSpeed * dt);
		}

		if (Fengshui::Input::IsKeyPressed(FS_KEY_I))
		{
			m_SquareTransform.Position.y += cameraMoveSpeed * dt;
			m_BigSquare.GetComponent<Fengshui::TransformComponent>()->Position.y += cameraMoveSpeed * dt;
			//m_SquareTransform.Rotation.z += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_K))
		{
			m_SquareTransform.Position.y -= cameraMoveSpeed * dt;
			m_BigSquare.GetComponent<Fengshui::TransformComponent>()->Position.y -= cameraMoveSpeed * dt;
			//m_SquareTransform.Rotation.z -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_J))
		{
			m_SquareTransform.Position.x -= cameraMoveSpeed * dt;
			m_BigSquare.GetComponent<Fengshui::TransformComponent>()->Position.x -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_L))
		{
			m_SquareTransform.Position.x += cameraMoveSpeed * dt;
			m_BigSquare.GetComponent<Fengshui::TransformComponent>()->Position.x += cameraMoveSpeed * dt;
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
	//Fengshui::Ref<Fengshui::Texture2D> m_Texture;

	Fengshui::GameEntity m_BigSquare;

	//Fengshui::RenderComponent m_VertexArray, m_SquareVA;
	std::vector<Fengshui::GameEntity> m_Squares;

	Fengshui::TransformComponent m_CameraPos;
	//glm::vec3 m_CameraPos;
	float cameraMoveSpeed = 5.0f;

	Fengshui::TransformComponent m_SquareTransform;

	Fengshui::Ref<Fengshui::Scene> m_Scene;
	//Fengshui::CameraComponent m_Camera;

	Fengshui::GameEntity square;
	Fengshui::Ref<Fengshui::TransformComponent> squareTrans;
	Fengshui::Ref<Fengshui::RenderComponent2D> m_SquareVA;

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