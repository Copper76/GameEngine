#include "fspch.h"
#include "OpenALAudioInputDevice.h"

#include <alc/alcmain.h>

namespace Fengshui
{
	void OpenALAudioInputDevice::Create(const char* deviceName, const bool startCapture)
	{
		if (m_Device)
		{
			Destroy();
		}

		m_Device = alcCaptureOpenDevice(deviceName, 44100, AL_FORMAT_MONO16, 1024);

		FS_ASSERT(m_Device, "Failed to initialise OpenAL input device");

		if (startCapture)
		{
			alcCaptureStart(m_Device);
		}
	}

	const std::string OpenALAudioInputDevice::GetDeviceName()
	{
		FS_ASSERT(m_Device->DeviceName != "", "No valid device exists");
		return m_Device->DeviceName;
	}

	void OpenALAudioInputDevice::Destroy()
	{
		if (m_Device)
		{
			alcCaptureStop(m_Device);
			alcCaptureCloseDevice(m_Device);

			m_Device = nullptr;
		}
	}
}