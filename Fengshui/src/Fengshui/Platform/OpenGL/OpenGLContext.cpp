#include "fspch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>

#include <glad/glad.h>

namespace Fengshui
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
	{
		FS_ENGINE_ASSERT(windowHandle, "Window doesn't exist");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FS_ENGINE_ASSERT(status, "Failed to initialise GLAD");

		FS_ENGINE_INFO("Device: {0}", reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
		FS_ENGINE_INFO("OpenGL Renderer: {0}", reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
