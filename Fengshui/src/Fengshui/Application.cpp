#include "fspch.h"
#include "Application.h"
#include "Fengshui/Logging/Log.h"
#include "Fengshui/Renderer/Renderer.h"

#include "Fengshui/KeyCode.h"

namespace Fengshui
{
	Application* Application::s_Instance = nullptr;

	Application::Application() : m_Camera(-1.0f, 1.0f, -1.0f, 1.0f)
	{
		FS_ENGINE_ASSERT(!s_Instance, "Applicaiton already running!");
		s_Instance = this;
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(FS_BIND_EVENT_FN(Application::OnEvent));

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
		//PushLayer(new ExampleLayer());

		m_VertexArray.reset(VertexArray::Create());

		float vertices[3 * 10] =
		{
			-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;
		vertexBuffer.reset(VertexBuffer::Create(sizeof(vertices), vertices));

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				{ShaderDataType::Vec4, "a_Color"},
				{ShaderDataType::Vec3, "a_Normal"},
				});

			vertexBuffer->SetLayout(layout);
		}

		m_VertexArray->AddVertexBuffer(vertexBuffer);

		unsigned int indices[3] = { 0,1,2 };
		std::shared_ptr<IndexBuffer> indexBuffer;
		indexBuffer.reset(IndexBuffer::Create(sizeof(indices)/sizeof(uint32_t), indices));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(VertexArray::Create());

		float square_Vertices[4 * 7] =
		{
			-0.7f, -0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
			0.7f, -0.7f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			0.7f, 0.7f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f,
			-0.7f, 0.7f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		};

		std::shared_ptr<VertexBuffer> squareVB;
		squareVB.reset(VertexBuffer::Create(sizeof(square_Vertices), square_Vertices));

		{
			BufferLayout layout({
				{ShaderDataType::Vec3, "a_Position"},
				{ShaderDataType::Vec4, "a_Color"},

				});

			squareVB->SetLayout(layout);
		}

		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t square_Indices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<IndexBuffer> squareIB;
		squareIB.reset(IndexBuffer::Create(sizeof(square_Indices) / sizeof(uint32_t), square_Indices));

		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSource = R"(
			#version 450 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
				
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

		m_Shader = std::make_unique<Shader>(vertexSource, fragmentSource);
		RenderCommand::SetClearColour({ 0.2f, 0.2f, 0.2f, 1 });

		m_Camera.SetPosition({ 0.3f,0.0f,0.0f });
		//m_Camera.SetRotation(90.0f);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		FS_ENGINE_WARN("Initialized Log!")
		FS_INFO("Client works as well")

		while (m_Running)
		{
			//Clear the screen
			RenderCommand::Clear();

			//Render cycle
			Renderer::BeginScene(m_Camera);

			Renderer::Submit(m_Shader, "u_ViewProjectionMatrix", m_SquareVA);

			Renderer::Submit(m_Shader, "u_ViewProjectionMatrix", m_VertexArray);

			Renderer::EndScene();

			//Update layers
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnUpdate();
				}
			}

			//Update GUI layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			//Update window with poll events
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e)
	{
		//FS_TRACE(e)

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(FS_BIND_EVENT_FN(Application::OnWindowClose));
		eventDispatcher.Dispatch<KeyPressedEvent>(FS_BIND_EVENT_FN(Application::OnKeyPressed));

		
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

	bool Application::OnKeyPressed(KeyPressedEvent& e)
	{
		if (e.GetKeyCode() == FS_KEY_W)
		{
			glm::vec3 offset = { 0.0f, 0.1f, 0.0f };
			m_Camera.SetPosition(m_Camera.GetPosition() + offset);
			return true;
		}

		if (e.GetKeyCode() == FS_KEY_S)
		{
			glm::vec3 offset = { 0.0f, -0.1f, 0.0f };
			m_Camera.SetPosition(m_Camera.GetPosition() + offset);
			return true;
		}

		if (e.GetKeyCode() == FS_KEY_A)
		{
			glm::vec3 offset = { -0.1f, 0.0f, 0.0f };
			m_Camera.SetPosition(m_Camera.GetPosition() + offset);
			return true;
		}

		if (e.GetKeyCode() == FS_KEY_D)
		{
			glm::vec3 offset = { 0.1f, 0.0f, 0.0f };
			m_Camera.SetPosition(m_Camera.GetPosition() + offset);
			return true;
		}
		if (e.GetKeyCode() == FS_KEY_Q)
		{
			m_Camera.SetRotation(m_Camera.GetRotation() - 1.0f);
			return true;
		}

		if (e.GetKeyCode() == FS_KEY_E)
		{
			m_Camera.SetRotation(m_Camera.GetRotation() + 1.0f);
			return true;
		}

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
