#pragma once

#include "Fengshui/ECS/ECS.h"


namespace Fengshui
{
	class AudioPlayerSystem : public System
	{
	public:
		void OnUpdate(const float dt);

		void PlayOnStart();

		void StopAll();

		void SetSettings(EntityID entity, AudioSetting settings);

		void Play(EntityID entity, bool restart);

		void Stop(EntityID entity);

		void Pause(EntityID entity);

		void Unpause(EntityID entity);
	};
}