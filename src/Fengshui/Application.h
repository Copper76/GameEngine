#pragma once

#include "Core.h"

namespace Fengshui
{
	class FENGSHUI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	Application* CreateApplication();
}

