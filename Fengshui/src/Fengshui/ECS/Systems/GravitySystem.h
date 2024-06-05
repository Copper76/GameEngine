#pragma once

#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	class GravitySystem : public System
	{
	public:
		void OnUpdate(const float dt);
	};

}
