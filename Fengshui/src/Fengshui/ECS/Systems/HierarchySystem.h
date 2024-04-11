#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class HierarchySystem : public System
	{
	public:
		void Destroy(EntityID entity);

	private:
		void DestroyInternal(EntityID entity);
	};
}