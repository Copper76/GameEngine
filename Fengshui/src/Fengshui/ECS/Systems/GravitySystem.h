#pragma once

#include "Fengshui/ECS/ECS.h"

namespace Fengshui
{
	class GravitySystem : public System
	{
	public:
		void OnUpdate(float dt);
	};

}
