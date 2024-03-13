#include "fspch.h"
#include "Fengshui/Core/Core.h"
#include "Fengshui/IMGUI/ImGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include "Fengshui/Core/Application.h"

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
		/**
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
		/**
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
		**/
		
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
		//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

		ImGui::StyleColorsDark();
		//ImGui::StyleColorsClassic();

		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		Application& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::Begin()
	{

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		
	}

	void ImGuiLayer::OnImGuiRender()
	{
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}
}