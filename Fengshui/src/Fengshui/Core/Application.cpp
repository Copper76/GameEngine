#include "fspch.h"
#include "Application.h"
#include "Fengshui/Logging/Log.h"
#include "Fengshui/Renderer/Renderer.h"

#include <glfw/glfw3.h>

namespace Fengshui
{
	Application* Application::s_Instance = nullptr;

	//Instantiate the application
	Application::Application(const std::string& name)
	{
		FS_ENGINE_ASSERT(!s_Instance, "Applicaiton already running!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowInfo()));

		m_Window->SetEventCallback(FS_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer);
	}

	//A default destructor
	Application::~Application()
	{

	}

	//The main body of application which will be executed throughout the life cycle of the application
	void Application::Run()
	{
		//Individual threads for the two different kinds of updates, fixed update is called at with a minimum delta time
		std::thread m_UpdateThread(std::bind(&Application::UpdateFunction, this));
		std::thread m_FixedUpdateThread(std::bind(&Application::FixedUpdateFunction, this));
		
		//The render thread is run on the main thread since it reduces problems caused by switching context
		RenderFunction();

		//Join the other threads before terminating the application
		m_UpdateThread.join();
		m_FixedUpdateThread.join();
	}

	//Default update used to handle most non-physical updates such as position, scale, and data change
	void Application::UpdateFunction()
	{
		while (m_Running && !m_Minimised)
		{
			float dt = m_Time.GetDeltaTime();

			//Update each layer
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnUpdate(dt);
				}
			}
		}
	}

	//Fixed update used to handle most physical updates such as collision, position and rotation changes due to velocity
	void Application::FixedUpdateFunction()
	{
		while (m_Running && !m_Minimised)
		{
			float dt = m_Time.GetDeltaTimeMicro();

			//Sleep this thread until a minimum value so there is no weird behaviour introduced by short delta time
			if (dt < 16000.0f)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(16000 - (int)dt));
				dt = 16000;
			}

			//Avoid the time step being too big, which could cause objects to teleport through solid objects, though this is still an issue to be addressed in physics
			if (dt > 33000.0f)
			{
				dt = 33000.0f;
			}

			dt = dt * 0.001f * 0.001f * 0.01f;//something wrong with time scalinG

			//Update each layer
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnFixedUpdate(dt);
				}
			}
		}
	}

	//Render update used to handle most updates related to graphics
	void Application::RenderFunction()
	{
		glfwMakeContextCurrent((GLFWwindow*)m_Window->GetNativeWindow());
		while (m_Running)
		{
			float time = (float)glfwGetTime();
			m_Time.UpdateTime(time);
			float dt = m_Time.GetDeltaTime();

			//Update the GUI and graphics layer for each layer
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack)
			{
				if (layer->IsActive())
				{
					layer->OnRender();
					layer->OnImGuiRender();
				}
			}
			m_ImGuiLayer->End();

			//Update window with poll events, involved with glfw so runs on main thread
			m_Window->OnUpdate();
		}
	}

	//Toggling the m_Running to false so the while loop would terminate
	void Application::Close()
	{
		m_Running = false;
	}

	//Event callbacks which are called when an event is fired in window
	void Application::OnEvent(Event& e)
	{
		EventDispatcher eventDispatcher(e);
		eventDispatcher.Dispatch<WindowCloseEvent>(FS_BIND_EVENT_FN(Application::OnWindowClose));
		eventDispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(Application::OnWindowResize));
		
		//Propogate the event down the layer stack until it is handled by a layer
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
			{
				break;
			}
		}
	}

	//Called when the window is closed through event
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		Close();
		return true;
	}

	//Called when the window is resized
	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		//Don't bother resizing the window if the window is minimised
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimised = true;
			return false;
		}

		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());
		m_Minimised = false;
		return false;
	}

	//Add a layer to the bottom of the layerstack
	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
	}

	//Add a layer to the top of the layerstack
	void Application::PushOverlay(Layer* overlay)
	{
		m_LayerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}
}
