#pragma once

#include "Fengshui/Core/Core.h"
#include "Fengshui/Core/MouseButtonCode.h"
#include "Fengshui/Core/KeyCode.h"

namespace Fengshui
{
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