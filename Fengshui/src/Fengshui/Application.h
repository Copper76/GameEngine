#pragma once

#include "Fengshui/Core.h"
#include "Fengshui/Window.h"

namespace Fengshui
{
	class FENGSHUI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = false;
	};

	Application* CreateApplication();
}

