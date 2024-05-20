#include "fspch.h"
#include "Application.h"
#include "Fengshui/Logging/Log.h"
#include "Fengshui/Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace Fengshui
{
	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name)
	{
		FS_ENGINE_ASSERT(!s_Instance, "Applicaiton already running!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowInfo()));

		m_Window->SetEventCallback(FS_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		std::thread m_UpdateThread(std::bind(&Application::UpdateFunction, this));
		std::thread m_FixedUpdateThread(std::bind(&Application::FixedUpdateFunction, this));
		
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			m_Time.UpdateTime(time);
			float dt = m_Time.GetDeltaTime();

			//Update Render
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnRender();
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			//Update window with poll events, involved in renderering so runs on main thread
			m_Window->OnUpdate();
		}

		
		//while (m_Running)
		//{
		//	float time = (float)glfwGetTime();
		//	m_Time.UpdateTime(time);
		//	float dt = m_Time.GetDeltaTime();

		//	if (!m_Minimised)
		//	{
		//		//Update layers
		//		for (Layer* layer : m_LayerStack)
		//		{
		//			if (layer->IsActive())
		//			{
		//				layer->OnUpdate(dt);
		//			}
		//		}
		//	}

		//	//Update window with poll events
		//	m_Window->OnUpdate();

		//	//Update Render
		//	m_ImGuiLayer->Begin();
		//	for (Layer* layer : m_LayerStack)
		//	{
		//		if (layer->IsActive())
		//		{
		//			layer->OnRender();
		//			layer->OnImGuiRender();
		//		}
		//	}
		//	m_ImGuiLayer->End();
		//}
		
		
		//std::thread m_RenderThread(std::bind(&Application::RenderFunction, this));

		m_UpdateThread.join();
		m_FixedUpdateThread.join();
		//m_RenderThread.join();
	}

	void Application::UpdateFunction()
	{
		while (m_Running)
		{
			float dt = m_Time.GetDeltaTime();

			if (!m_Minimised)
			{
				//Update layers
				for (Layer* layer : m_LayerStack)
				{
					if (layer->IsActive())
					{
						layer->OnUpdate(dt);
					}
				}
			}
		}
	}

	void Application::FixedUpdateFunction()
	{
		while (m_Running)
		{
			float dt = m_Time.GetDeltaTimeMicro();

			if (dt < 16000.0f)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(16000 - (int)dt));
				dt = 16000;
			}

			//Avoid the time step to be too big
			if (dt > 33000.0f)
			{
				dt = 33000.0f;
			}

			dt = dt * 0.001f * 0.001f;

			if (!m_Minimised)
			{
				//Update layers
				for (Layer* layer : m_LayerStack)
				{
					if (layer->IsActive())
					{
						layer->OnFixedUpdate(dt);
					}
				}
			}
		}
	}

	void Application::RenderFunction()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_Window->GetNativeWindow());
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			m_Time.UpdateTime(time);
			float dt = m_Time.GetDeltaTime();

			//Update GUI layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnRender();
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();
		}
	}

	void Application::Close()
	{
		m_Running = false;
	}

	void Application::OnEvent(Event& e)
	{
		//FS_TRACE(e)

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(FS_BIND_EVENT_FN(Application::OnWindowClose));
		eventDispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(Application::OnWindowResize));
		
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
		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimised = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		m_Minimised = false;
		return false;
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
