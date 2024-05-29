#pragma once

#include "fspch.h"

#include "Fengshui/Core/Core.h"
#include "Fengshui/Events/Event.h"

namespace Fengshui
{
	/*
	* struct used to store the necessary information for instantiating a window
	*/
	struct WindowInfo
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;

		WindowInfo(const std::string& title = "Fengshui Engine",
			uint32_t width = 1280,
			uint32_t height = 720)
			: Title(title), Width(width), Height(height) {}
	};

	/*
	* The actual window that is created for the application to run, the implementation is found in Platform/Windows
	*/
	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Scope<Window> Create(const WindowInfo& info = WindowInfo());
	};
}