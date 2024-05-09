#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class HierarchySystem : public System
	{
	public:
		void Destroy(EntityID entity);
		void SetParent(EntityID entity, EntityID parent);
		void AddChild(EntityID entity, EntityID child);

	private:
		void LinkChild(EntityID entity, EntityID parent);
	};
}