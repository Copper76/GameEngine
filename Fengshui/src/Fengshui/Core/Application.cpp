#include "fspch.h"
#include "Application.h"
#include "Fengshui/Logging/Log.h"
#include "Fengshui/Renderer/Renderer.h"

#include <glfw/glfw3.h>

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
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		//FS_ENGINE_WARN("Initialized Log!")
		//FS_INFO("Client works as well")

		while (m_Running)
		{
			float time = (float)glfwGetTime();
			m_Time.UpdateTime(time);
			float dt = m_Time.GetDeltaTime();

			//Update entities in scene

			//Update layers
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnUpdate(dt);
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