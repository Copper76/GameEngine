#pragma once

#include "Fengshui/Core/Core.h"
#include "Fengshui/Core/MouseButtonCode.h"
#include "Fengshui/Core/KeyCode.h"

namespace Fengshui
{
	//An input class for checking if a specific input is made or get the mouse position, implementation can be found in window class
	class Input
	{
	public:
		static bool IsKeyPressed(int keyCode);
		static bool IsMouseButtonPressed(int button);
		static float GetMouseX();
		static float GetMouseY();
		static std::pair<float, float> GetMousePos();
	};
}