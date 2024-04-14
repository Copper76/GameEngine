#pragma once
#include "Fengshui/Core/Scene.h"
#include "PropertyPanel.h"

namespace Fengshui
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel();

		void OnImGuiRender();

		void DrawEntityNode(EntityID entity);

		inline void ResetSelection() { m_SelectedEntity = 0; }

	private:
		Ref<Scene> m_Context;
		Ref<PropertyPanel> m_PropertyPanel;

		EntityID m_SelectedEntity = 0;
	};
}