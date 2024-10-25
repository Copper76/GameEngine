#pragma once

#include "Fengshui/Audio/AudioListener.h"

namespace Fengshui
{
	class OpenALAudioListener : public AudioListener
	{
		virtual void UpdateLocation(glm::vec3 position) override;
		virtual void UpdateOrientation(glm::vec3 forward, glm::vec3 up) override;
	};
}