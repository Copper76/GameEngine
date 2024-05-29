#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Systems/TransformSystem.h"

namespace Fengshui
{
	class RenderSystem : public System
	{
	public:
		void OnRender(Ref<TransformSystem> transformSystem);
	};
}
