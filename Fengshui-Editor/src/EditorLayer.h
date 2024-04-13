#pragma once
#include <Fengshui.h>
#include "Panels/SceneHierarchyPanel.h"

//External includes
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace Fengshui
{
	class EditorLayer : public Layer
	{
	public:
		EditorLayer();

		void OnUpdate(float dt) override;

		void OnAttach() override;
		void OnDetach() override;

		void OnImGuiRender() override
		{
			static bool dockspaceOpen = true;
			static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

			// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
			// because it would be confusing to have two docking targets within each others.
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

			// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
			// and handle the pass-thru hole, so we ask Begin() to not render a background.
			if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
				window_flags |= ImGuiWindowFlags_NoBackground;

			// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
			// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
			// all active windows docked into it will lose their parent and become undocked.
			// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
			// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
			ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

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
					if (ImGui::MenuItem("Exit", NULL)) Application::GetInstance().Close();

					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			ImGui::Begin("Settings");
			ImGui::ColorEdit4("Square Colour", glm::value_ptr(m_SquareColour));
			ImGui::DragFloat("Tiling Factor", &m_TilingFactor);
			ImGui::Text("Camera Pos: (%f, %f)", m_ActiveScene->GetCameraComponent().CameraPos.x , m_ActiveScene->GetCameraComponent().CameraPos.y);
			ImGui::Text("Square Name: %s", m_BigSquare->Name().c_str());
			if(ImGui::Checkbox("Scene Camera", &m_UsingSceneCamera))
			{
				m_SecondCamera->GetComponent<CameraComponent>().Primary = !m_UsingSceneCamera;
				m_ActiveScene->GetCameraComponent().Primary = m_UsingSceneCamera;
			}

			if (ImGui::Checkbox("Active Scene", &m_UsingSceneOne))
			{
				if (m_UsingSceneOne)
				{
					m_ActiveScene = m_Scene;
				}
				else
				{
					m_ActiveScene = m_OtherScene;
				}
				GeneralManager::SetActiveScene(m_ActiveScene);
				m_SceneHierarchyPanel->SetContext(m_ActiveScene);
			}
			ImGui::End();

			m_SceneHierarchyPanel->OnImGuiRender();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
			ImGui::Begin("ViewPort");
			m_ActiveScene->SetViewportFocused(ImGui::IsWindowFocused());
			Application::GetInstance().GetImGuiLayer()->SetBlockEvent(!ImGui::IsWindowHovered() || !ImGui::IsWindowFocused());
			ImVec2 viewportSize = ImGui::GetContentRegionAvail();
			if (m_ViewportSize != *(glm::vec2*)&viewportSize)
			{
				m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
				m_ViewportSize = { viewportSize.x, viewportSize.y };

				m_ActiveScene->ResizeBounds(viewportSize.x, viewportSize.y);
			}
			uint32_t textureID = m_Framebuffer->GetColourAttachmentRendererID();
			ImGui::Image((void*)textureID, viewportSize, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
			ImGui::End();
			ImGui::PopStyleVar();

			ImGui::End();
		}

		void OnEvent(Event& e) override
		{
			m_Scene->OnEvent(e);
		}

	private:
		Ref<Scene> m_ActiveScene;

		Ref<Scene> m_Scene;
		Ref<Scene> m_OtherScene;

		glm::vec4 m_SquareColour = { 1.0f, 1.0f, 1.0f, 1.0f };
		float m_TilingFactor = 1.0f;

		Ref<Entity> m_BigSquare;
		Ref<Entity> m_SecondCamera;
		std::vector< Ref<Entity>> m_BackgroundSquares;

		Ref<SceneHierarchyPanel> m_SceneHierarchyPanel;

		bool m_UsingSceneCamera = true;
		bool m_UsingSceneOne = true;

		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize;
	};
}