#include "fspch.h"
#include "Application.h"
#include "Fengshui/Example/Snake.h"
#include "Fengshui/Logging/Log.h"

namespace Fengshui
{

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running)
		{
			/**
			for (Layer* layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
			**/
			if (m_Window)
			{
				m_Window->OnUpdate();
			}
		}
		//spdlog::drop_all();
		//SnakeGame::Snake();
	}

	void Application::OnEvent(Event& e)
	{
		//std::cout << e.ToString() << std::endl;

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

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
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
	}
}
