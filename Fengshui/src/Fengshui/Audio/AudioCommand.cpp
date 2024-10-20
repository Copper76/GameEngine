#include "fspch.h"
#include "Fengshui/Audio/AudioCommand.h"

#include "Fengshui/Platform/OpenAL/OpenALAudioAPI.h"

namespace Fengshui
{
	Scope<AudioAPI> AudioCommand::s_AudioAPI = MakeScope<OpenALAudioAPI>();
}