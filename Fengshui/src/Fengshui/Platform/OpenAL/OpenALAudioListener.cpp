#include "fspch.h"
#include "OpenALAudioListener.h"

#include <AL/al.h>

namespace Fengshui
{
	void OpenALAudioListener::UpdateLocation(glm::vec3 position)
	{
		alListener3f(AL_POSITION, position.x, position.y, position.z);
	}

	void OpenALAudioListener::UpdateOrientation(glm::vec3 forward, glm::vec3 up)
	{
		float orientation[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
		alListenerfv(AL_ORIENTATION, orientation);
	}
}