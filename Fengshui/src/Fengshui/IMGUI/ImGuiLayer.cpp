#include "fspch.h"
#include "Fengshui/Core.h"
#include "ImGuiLayer.h"
#include "Fengshui/Application.h"

#include "imgui.h"
#include "Fengshui/KeyCode.h"
#include "Fengshui/Platform/OpenGL/ImGuiOpenGLRenderer.h"

#include "glad/glad.h"

#include "GLFW/glfw3.h"

namespace Fengshui
{
	ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer", true)
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

		//Mapping from imgui_impl_glfw.cpp
		io.KeyMap[ImGuiKey_Tab] = FS_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = FS_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = FS_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = FS_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = FS_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = FS_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = FS_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = FS_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = FS_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = FS_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = FS_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = FS_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = FS_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = FS_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = FS_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = FS_KEY_A;
		io.KeyMap[ImGuiKey_C] = FS_KEY_C;
		io.KeyMap[ImGuiKey_V] = FS_KEY_V;
		io.KeyMap[ImGuiKey_X] = FS_KEY_X;
		io.KeyMap[ImGuiKey_Y] = FS_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = FS_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");
		//ImGui_ImplOpenGL3_Init();
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

		float time = (float)glfwGetTime();
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);
		m_Time = time;

		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);

		glClear(GL_COLOR_BUFFER_BIT);//Clear last render

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void ImGuiLayer::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<MouseButtonPressedEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
		dispatcher.Dispatch<MouseMovedEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
		dispatcher.Dispatch<MouseScrolledEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
		dispatcher.Dispatch<KeyPressedEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
		dispatcher.Dispatch<KeyReleasedEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
		dispatcher.Dispatch<KeyTypedEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
		dispatcher.Dispatch<WindowResizeEvent>(FS_BIND_EVENT_FN(ImGuiLayer::OnWindowResizeEvent));
	}

	bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = true;

		return false;//Non-consuming at the moment
	}
	bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseDown[e.GetMouseButton()] = false;

		return false;
	}
	bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos = ImVec2(e.GetX(), e.GetY());

		return false;
	}
	bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.MouseWheelH += e.GetXOffset();
		io.MouseWheel += e.GetYOffset();

		return false;
	}
	bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = true;

		io.KeyCtrl = io.KeysDown[FS_KEY_LEFT_CONTROL] || io.KeysDown[FS_KEY_RIGHT_CONTROL];
		io.KeyShift = io.KeysDown[FS_KEY_LEFT_SHIFT] || io.KeysDown[FS_KEY_RIGHT_SHIFT];
		io.KeyAlt = io.KeysDown[FS_KEY_LEFT_ALT] || io.KeysDown[FS_KEY_RIGHT_ALT];
		io.KeySuper = io.KeysDown[FS_KEY_LEFT_SUPER] || io.KeysDown[FS_KEY_RIGHT_SUPER];

		return false;
	}
	bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.KeysDown[e.GetKeyCode()] = false;

		return false;
	}
	bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		int keyCode = e.GetKeyCode();
		if (keyCode > 0 && keyCode < 0x10000)
		{
			io.AddInputCharacter((unsigned short) keyCode);
		}

		return false;
	}
	bool ImGuiLayer::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
		glViewport(0, 0, e.GetWidth(), e.GetHeight());

		return false;
	}
}