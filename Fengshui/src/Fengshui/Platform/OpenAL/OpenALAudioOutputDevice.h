#pragma once

#include "Fengshui/Audio/AudioOutputDevice.h"
#include <AL/alc.h>

namespace Fengshui
{
	class OpenALAudioOutputDevice : public AudioOutputDevice
	{
	public:
		virtual void Create(const char* deviceName) override;

		virtual const std::string GetDeviceName() override;

		virtual void Destroy() override;
	private:
		ALCdevice* m_Device;
		ALCcontext* m_Context;
	};
}

