#pragma once
#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class HierarchySystem : public System
	{
	public:
		void Destroy(Entity entity);

	private:
		void DestroyInternal(Entity entity);
	};
}