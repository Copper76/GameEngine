#pragma once
#include "Fengshui/Core/Scene.h"

namespace Fengshui
{
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() {};

		SceneHierarchyPanel(const Ref<Scene>& context);

		void SetContext(const Ref<Scene>& context);

		void OnImGuiRender();

	private:
		Ref<Scene> m_Context;
	};
}