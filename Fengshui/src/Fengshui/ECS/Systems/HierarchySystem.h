#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class HierarchySystem : public System
	{
	public:
		void Destroy(EntityID entity);
		void SetParent(EntityID entity, EntityID parent);

	private:
		void LinkChild(EntityID entity, EntityID parent);
	};
}