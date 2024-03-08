#pragma once

#include "Fengshui/Core.h"
#include "Fengshui/Events/Event.h"
#include "Fengshui/IMGUI/ImGuiLayer.h"
#include "Fengshui/Layers/LayerStack.h"
#include "Fengshui/Events/ApplicationEvent.h"

#include "Fengshui/IMGUI/ImGuiLayer.h"

#include "Fengshui/Window.h"
#include "Fengshui/Renderer/Shader.h"

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

		unsigned int m_VertexArray, m_VertexBuffer, m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

