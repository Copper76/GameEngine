#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class RenderSystem : public System
	{
	public:
		void OnRender();

		inline std::set<EntityID> GetRenderedObjects() { return m_Entities; }
	};
}
