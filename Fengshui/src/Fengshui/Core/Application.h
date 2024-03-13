#pragma once

#include "Fengshui/Core/Core.h"
#include "Fengshui/Events/Event.h"
#include "Fengshui/IMGUI/ImGuiLayer.h"
#include "Fengshui/Layers/LayerStack.h"
#include "Fengshui/Events/ApplicationEvent.h"

#include "Fengshui/IMGUI/ImGuiLayer.h"

#include "Fengshui/Core/Window.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/Buffer.h"
#include "Fengshui/Renderer/VertexArray.h"

#include "Fengshui/Renderer/Camera.h"

#include "Fengshui/Core/Time.h"

#include "Fengshui/Core/Scene.h"

namespace Fengshui
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline static Application& GetInstance() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		Time m_Time;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

