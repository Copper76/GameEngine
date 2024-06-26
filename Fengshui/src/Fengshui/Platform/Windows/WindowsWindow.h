#pragma once

#include "Fengshui/Core/Window.h"
#include "Fengshui/Renderer/RenderContext.h"

#include <GLFW/glfw3.h>

namespace Fengshui
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowInfo& info);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline uint32_t GetWidth() const override { return m_Data.Width; }
		inline uint32_t GetHeight() const override { return m_Data.Height;  }

		inline virtual void* GetNativeWindow() const { return m_Window; }

		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

	private:
		virtual void Init(const WindowInfo& info);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window; //May need to change to Window in the future
		RenderContext* m_Context;

		struct WindowData
		{
			std::string title;
			uint32_t Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};
}