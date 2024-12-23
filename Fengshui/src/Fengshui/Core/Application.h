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

#include "Fengshui/Core/Time.h"

#include "Fengshui/Core/Scene.h"

namespace Fengshui
{
	/*
	* Application class which will launch and maintain the application and application window
	*/
	class Application
	{
	public:
		Application(const std::string& name = "Fengshui Engine");
		virtual ~Application();

		void Run();
		
		void Close();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);

		inline void StartGame() { m_IsPlaying = true; }

		inline void EndGame() 
		{ 
			m_IsPlaying = false; 
			m_Paused = false;
		}

		inline bool IsPlaying() { return m_IsPlaying; }

		inline void PauseGame(bool paused) { m_Paused = paused; }
		inline void TogglePause() { m_Paused = !m_Paused; }
		inline bool IsPaused() { return m_Paused; }

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		inline static Application& GetInstance() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);


		void UpdateFunction();
		void FixedUpdateFunction();
		void RenderFunction();

	private:
		std::atomic<bool> m_Paused = false;
		std::atomic<bool> m_IsPlaying = false;
		std::atomic<bool> m_Running = true;
		bool m_Minimised = false;

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		LayerStack m_LayerStack;
		Time m_Time;

	private:
		static Application* s_Instance;
	};

	Application* CreateApplication();
}

