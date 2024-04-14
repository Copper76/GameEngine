#pragma once
#include "Fengshui/Core/Scene.h"

namespace Fengshui
{
	class PropertyPanel
	{
	public:
		PropertyPanel() = default;

		void OnImGuiRender(EntityID entity);

	private:
		void DrawComponents(EntityID entity);
	};
}