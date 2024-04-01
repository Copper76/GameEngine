#include "fspch.h"
#include "Fengshui/Core/Input.h"

#include "Fengshui/Core/Application.h"

#include "GLFW/glfw3.h"

namespace Fengshui
{

	bool Input::IsKeyPressed(int keyCode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetKey(window, keyCode);

		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		auto state = glfwGetMouseButton(window, button);

		return state == GLFW_PRESS;
	}


	std::pair<float, float> Input::GetMousePos()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		return { (float)xpos, (float)ypos };
	}

	float Input::GetMouseX()
	{
		auto[xPos, yPos] = GetMousePos();

		return xPos;
	}

	float Input::GetMouseY()
	{
		auto [xPos, yPos] = GetMousePos();

		return yPos;
	}
}
