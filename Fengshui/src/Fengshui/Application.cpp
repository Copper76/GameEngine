#include "fspch.h"
#include "Application.h"
#include "Fengshui/Example/Snake.h"
#include "Fengshui/Events/Event.h"
#include "Fengshui/Events/ApplicationEvent.h"
#include "Fengshui/Logging/Log.h"

namespace Fengshui
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());
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
}
