#pragma once

#include "Fengshui/Audio/AudioInputDevice.h"
#include <AL/alc.h>

namespace Fengshui
{
	class OpenALAudioInputDevice : public AudioInputDevice
	{
	public:
		virtual void Create(const char* deviceName, const bool startCapture) override;

		virtual const std::string GetDeviceName() override;

		virtual void Destroy() override;
	private:
		ALCdevice* m_Device;
	};
}

