#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Fengshui
{
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_PropertyPanel = MakeRef<PropertyPanel>();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		auto& comp = GeneralManager::GetComponent<Hierarchy>(0);
		for (EntityID e : comp.Children)
		{
			DrawEntityNode(e);
		}

		if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		{
			ResetSelection();
		}

		ImGui::End();


		ImGui::Begin("Properties");
		if (m_SelectedEntity != 0)
		{
			m_PropertyPanel->OnImGuiRender(m_SelectedEntity);

			if (ImGui::BeginCombo("##1", "Add Component"))
			{
				if (!GeneralManager::HasComponent<CameraComponent>(m_SelectedEntity))
				{
					if (ImGui::Selectable("Camera"))
					{
						GeneralManager::AddComponent<CameraComponent>(m_SelectedEntity, CameraComponent());
					}
				}
				bool mutex = GeneralManager::HasComponent<Render>(m_SelectedEntity) || GeneralManager::HasComponent<Render2D>(m_SelectedEntity);
				if (!mutex)
				{
					if (ImGui::Selectable("Render"))
					{
						GeneralManager::AddComponent<Render>(m_SelectedEntity, Render());
					}

					if (ImGui::Selectable("Render2D"))
					{
						GeneralManager::AddComponent<Render2D>(m_SelectedEntity, Render2D());
					}
				}

				if (!GeneralManager::HasComponent<Rigidbody>(m_SelectedEntity))
				{
					if (ImGui::Selectable("Rigidbody"))
					{
						GeneralManager::AddComponent<Rigidbody>(m_SelectedEntity, Rigidbody());
					}
				}

				if (!GeneralManager::HasComponent<Collider>(m_SelectedEntity))
				{
					if (ImGui::Selectable("Collider"))
					{
						GeneralManager::AddComponent<Collider>(m_SelectedEntity, Collider());
					}
				}

				mutex = GeneralManager::HasComponent<Transform>(m_SelectedEntity) || GeneralManager::HasComponent<Transform2D>(m_SelectedEntity);

				if (!mutex)
				{
					if (ImGui::Selectable("Transform"))
					{
						GeneralManager::AddComponent<Transform>(m_SelectedEntity, Transform());
					}

					if (ImGui::Selectable("Transform2D"))
					{
						GeneralManager::AddComponent<Transform2D>(m_SelectedEntity, Transform2D());
					}
				}

				ImGui::EndCombo();
			}
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(EntityID entity)
	{
		auto children = GeneralManager::GetComponent<Hierarchy>(entity).Children;
		auto& tag = GeneralManager::GetComponent<Tag>(entity).Name;
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0);
		if (children.empty())
		{
			flags |= ImGuiTreeNodeFlags_Leaf;
		}
		else
		{
			flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick;
		}
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity, flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (opened)
		{
			for (EntityID e : children)
			{
				DrawEntityNode(e);
			}
			ImGui::TreePop();
		}
	}
}
