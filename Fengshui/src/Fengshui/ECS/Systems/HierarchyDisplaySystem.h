#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class HierarchyDisplaySystem : public System
	{
	public:
		std::vector<std::string> GetEntityTags();
		std::set<EntityID> GetEntityIDs();
	};
}