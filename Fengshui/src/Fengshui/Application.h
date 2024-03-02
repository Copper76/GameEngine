#pragma once

#include "Fengshui/Core.h"
#include "Fengshui/Events/Event.h"
#include "Fengshui/Events/ApplicationEvent.h"

#include "Fengshui/Window.h"

namespace Fengshui
{
	class FENGSHUI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		bool m_Running = false;
	};

	Application* CreateApplication();
}

