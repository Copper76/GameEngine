#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Fengshui
{
	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		/**
		for (std::string name : GeneralManager::GetActiveScene()->GetAllEntityNames())
		{
			ImGui::Text("%s", name.c_str());
		}
		**/

		for (EntityID entity : GeneralManager::GetActiveScene()->GetAllEntities())
		{
			DrawEntityNode(entity);
		}
		ImGui::End();
	}

	void SceneHierarchyPanel::DrawEntityNode(EntityID entity)
	{
		auto& tag = GeneralManager::GetComponent<Tag>(entity).Name;
		ImGuiTreeNodeFlags flags = ((m_SelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)entity,flags, tag.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectedEntity = entity;
		}

		if (opened)
		{
			ImGui::TreePop();
		}
	}
}
