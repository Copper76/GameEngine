#include "fspch.h"
#include "Application.h"
#include "Fengshui/Logging/Log.h"
#include "Fengshui/Input.h"
#include "Fengshui/Platform/OpenGL/OpenGLRendererAPI.h"
#include "Fengshui/Renderer/Renderer.h"

namespace Fengshui
{
	Application* Application::s_Instance = nullptr;

	Application::Application()
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

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
				
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
			Renderer::BeginScene();

			m_Shader->Bind();

			Renderer::Submit(m_SquareVA);

			Renderer::Submit(m_VertexArray);

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
