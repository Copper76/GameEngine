#pragma once
#include "Fengshui.h"

//External includes
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

class Sandbox2D : public Fengshui::Layer
{
public:
	Sandbox2D();

	void OnUpdate(float dt) override;

	void OnAttach() override;
	void OnDetach() override;

	void OnImGuiRender() override
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Submit the DockSpace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Options"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen);
				if (ImGui::MenuItem("Exit", NULL)) Fengshui::Application::GetInstance().Close();
				ImGui::MenuItem("Padding", NULL, &opt_padding);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_SquareColour));
		ImGui::DragFloat("Tiling Factor", &m_TilingFactor);
		uint32_t textureID = m_Framebuffer->GetColourAttachmentRendererID();
		ImGui::Image((void*)textureID, ImVec2{256.0f, 256.0f});
		ImGui::End();

		ImGui::End();
	}

	void OnEvent(Fengshui::Event& e) override
	{
		m_Scene->OnEvent(e);
	}

private:
	Fengshui::Ref<Fengshui::Scene> m_Scene;
	glm::vec4 m_SquareColour = { 1.0f, 1.0f, 1.0f, 1.0f };
	float m_TilingFactor = 1.0f;

	Fengshui::Ref<Fengshui::GameEntity> m_BigSquare;

	Fengshui::Ref<Fengshui::Framebuffer> m_Framebuffer;
};