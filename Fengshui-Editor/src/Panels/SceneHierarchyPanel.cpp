#include "SceneHierarchyPanel.h"

#include <imgui.h>

namespace Fengshui
{
	SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& context)
	{
		SetContext(context);
	}

	void SceneHierarchyPanel::SetContext(const Ref<Scene>& context)
	{
		m_Context = context;
	}

	void SceneHierarchyPanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");
		for (std::string name : m_Context->GetAllEntityNames())
		{
			ImGui::Text("%s", name.c_str());
		}
		ImGui::End();
	}
}
