#pragma once

#include "Fengshui/ECS/ECS.h"
namespace Fengshui
{
	class AudioListenSystem : public System
	{
	public:
		void OnUpdate(const float dt);

		void UpdateAudioListenerTransform();
	};
}

