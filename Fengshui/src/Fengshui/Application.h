#pragma once

#include "Fengshui/Core.h"
#include "Fengshui/Events/Event.h"
#include "Fengshui/IMGUI/ImGuiLayer.h"
#include "Fengshui/Layers/LayerStack.h"
#include "Fengshui/Events/ApplicationEvent.h"

#include "Fengshui/IMGUI/ImGuiLayer.h"

#include "Fengshui/Window.h"

#include "Fengshui/Renderer/Shader.h"
#include "Fengshui/Renderer/Buffer.h"
#include "Fengshui/Renderer/VertexArray.h"

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

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray>  m_VertexArray;

		std::shared_ptr<VertexArray> m_SquareVA;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

