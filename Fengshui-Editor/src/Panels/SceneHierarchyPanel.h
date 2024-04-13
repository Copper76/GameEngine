#pragma once
#include "Fengshui/Core/Scene.h"

namespace Fengshui
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() {};

		void OnImGuiRender();

		void SceneHierarchyPanel::DrawEntityNode(EntityID name);

	private:
		Ref<Scene> m_Context;
		EntityID m_SelectedEntity;
	};
}