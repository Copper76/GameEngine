#include <Fengshui.h>
#include "Sandbox2D.h"

#include <Fengshui/Core/EntryPoint.h>

//External includes
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Fengshui::Layer
{
public:
	//ExampleLayer() : Layer("Example"), m_Camera(0, -1.0f, 1.0f, -1.0f, 1.0f), m_CameraPos(1), m_SquareTransform(2)
	ExampleLayer() : Layer("Example")
	{
		
	}

	void OnUpdate(float dt) override
	{

		auto shader = m_Squares[0]->GetComponent<Fengshui::RenderComponent>()->GetShader();
		shader->SetVec4("u_Colour", m_SquareColour);

		auto sceneCam = m_Scene->GetCameraComponent();

		auto sceneCamPos = sceneCam->GetPosition();
		auto sceneCamRot = sceneCam->GetRotation();

		if (Fengshui::Input::IsKeyPressed(FS_KEY_I))
		{
			m_BigSquare->GetComponent<Fengshui::TransformComponent>()->Position.y += cameraMoveSpeed * dt;
			//m_SquareTransform.Rotation.z += cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_K))
		{
			m_BigSquare->GetComponent<Fengshui::TransformComponent>()->Position.y -= cameraMoveSpeed * dt;
			//m_SquareTransform.Rotation.z -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_J))
		{
			m_BigSquare->GetComponent<Fengshui::TransformComponent>()->Position.x -= cameraMoveSpeed * dt;
		}
		if (Fengshui::Input::IsKeyPressed(FS_KEY_L))
		{
			m_BigSquare->GetComponent<Fengshui::TransformComponent>()->Position.x += cameraMoveSpeed * dt;
		}

		m_Scene->OnUpdate(dt);
	}

	void OnAttach() override
	{
		//Setup and loading

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

		unsigned int indices[6] = { 0, 1, 2, 2, 3, 0 };
		Fengshui::Ref<Fengshui::IndexBuffer> indexBuffer = Fengshui::IndexBuffer::Create(sizeof(indices) / sizeof(uint32_t), indices);

		auto textureShader = Fengshui::Renderer::GetShaderLib()->Load("Assets/Shaders/TextureShader.glsl");

		Fengshui::Ref<Fengshui::Texture> m_Texture = Fengshui::Texture::Create("Assets/Textures/Checkerboard.png", textureShader);

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);

		auto shader = Fengshui::Renderer::GetShaderLib()->Load("Shader", "Assets/Shaders/FlatColourShader.glsl");
		shader->SetVec4("u_Colour", m_SquareColour);

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

		uint32_t square_Indices[6] = { 0, 1, 2, 2, 3, 0 };
		Fengshui::Ref<Fengshui::IndexBuffer> squareIB = (Fengshui::IndexBuffer::Create(sizeof(square_Indices) / sizeof(uint32_t), square_Indices));

		//Scene initialise
		m_Scene = Fengshui::Scene::Init();

		//Big Square
		m_BigSquare = Fengshui::GameEntity::Create(m_Scene);

		m_BigSquare->AddComponent<Fengshui::TransformComponent>();

		m_BigSquare->AddComponent<Fengshui::RenderComponent>(vertexBuffer, indexBuffer, textureShader, m_Texture);

		//Small Squares

		for (int i = -2; i < 3; i++)
		{
			for (int j = -2; j < 3; j++)
			{
				Fengshui::Ref<Fengshui::GameEntity> square = Fengshui::GameEntity::Create(m_Scene, m_BigSquare->GetComponent<Fengshui::HierarchyComponent>());

				Fengshui::Ref<Fengshui::TransformComponent> squareTrans = square->AddComponent<Fengshui::TransformComponent>();
				squareTrans->Position += glm::vec3(i * 0.3f, j * 0.3f, 0.0f);
				squareTrans->Scale = { 0.1f, 0.1f, 0.1f };

				square->AddComponent<Fengshui::RenderComponent>(squareVB, squareIB, shader);
				m_Squares.emplace_back(square);
			}
		}

		//Testing code
		//m_Scene->RemoveEntity(1);

		m_BigSquare->RemoveComponent<Fengshui::TransformComponent>();
		m_BigSquare->AddComponent<Fengshui::TransformComponent>();
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
		m_Scene->OnEvent(e);
	}

private:
	Fengshui::Ref < Fengshui::GameEntity> m_BigSquare;

	//Fengshui::RenderComponent m_VertexArray, m_SquareVA;
	std::vector<Fengshui::Ref<Fengshui::GameEntity>> m_Squares;

	float cameraMoveSpeed = 5.0f;

	Fengshui::Ref<Fengshui::Scene> m_Scene;
	//Fengshui::CameraComponent m_Camera;

	glm::vec4 m_SquareColour = { 0.2f, 0.3f, 0.7f, 1.0f };
};

class SandboxApp : public Fengshui::Application
{
public:
	SandboxApp() : Application()
	{
		//PushLayer(new ExampleLayer());
		PushLayer(new Sandbox2D());
	}

	~SandboxApp()
	{

	}
};

Fengshui::Application* Fengshui::CreateApplication()
{
	return new SandboxApp();
}