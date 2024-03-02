#include "fspch.h"
#include "WindowsWindow.h"

namespace Fengshui
{
	static bool s_GLFWInitialised = false;

	Window* Window::Create(const WindowInfo& info)
	{
		return new WindowsWindow(info);
	}

	WindowsWindow::WindowsWindow(const WindowInfo& info)
	{
		Init(info);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowInfo& info)
	{
		m_Data.title = info.Title;
		m_Data.Width = info.Width;
		m_Data.Height = info.Height;

		//FS_ENGINE_INFO("Createing WIndow {0} ({1}, {2})", info.Title, info.Width, info.Height);

		if (!s_GLFWInitialised)
		{
			int success = glfwInit();

			//FS_ENGINE_ASSERT(success, "Could not initialise GLFW")

			s_GLFWInitialised = true;
		}

		m_Window = glfwCreateWindow((int)info.Width, (int)info.Height, info.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
		{
			glfwSwapInterval(1);
		}
		else
		{
			glfwSwapInterval(0);
		}
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
}