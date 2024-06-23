#pragma once
#include "Fengshui/ECS/ECS.h"
#include "Fengshui/ECS/Systems/RenderSystem.h"

namespace Fengshui
{
	class LightSystem : public System
	{
	public:
		void OnUpdate(Ref<RenderSystem> renderSystem);
	private:
		bool m_NoLightLastFrame = true;//A condition to reduce the number of resets for normal
	};
}

