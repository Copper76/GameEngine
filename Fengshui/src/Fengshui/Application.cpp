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
		m_Running = true;
		while (m_Running)
		{
			m_Window->OnUpdate();
		}
		WindowResizeEvent e(1280, 720);
		//FS_TRACE(e);
		//SnakeGame::Snake();
	}

	void Application::OnEvent(Event& e)
	{
		std::cout << e.ToString() << std::endl;

		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));

	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
