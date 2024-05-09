#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Fengshui
{
	SceneHierarchyPanel::SceneHierarchyPanel()
	{
		m_PropertyPanel = std::make_shared<PropertyPanel>();
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		auto& comp = GeneralManager::GetComponent<Hierarchy>(0);
		for (EntityID entity : comp.Children)
		{
			DrawEntityNode(entity);
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
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(EntityID entity)
	{
		auto& children = GeneralManager::GetComponent<Hierarchy>(entity).Children;
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
			for (EntityID child : children)
			{
				DrawEntityNode(child);
			}
			ImGui::TreePop();
		}
	}
}
