#include "fspch.h"
#include "OpenALAudioInputDevice.h"

#include <alc/alcmain.h>

namespace Fengshui
{
	void OpenALAudioInputDevice::Create(const char* deviceName, const bool startCapture)
	{
		if (m_Device)
		{
			if (m_Device->DeviceName == deviceName)
			{
				FS_WARN("Device already created");
				return;
			}

			Destroy();
		}

		m_Device = alcCaptureOpenDevice(deviceName, 44100, AL_FORMAT_MONO16, 1024);

		FS_ASSERT(m_Device, "Failed to initialise OpenAL input device");

		if (startCapture)
		{
			StartCapture();
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

	void OpenALAudioInputDevice::StartCapture()
	{
		FS_ASSERT(m_Device->DeviceName != "", "No valid device exists");

		alcCaptureStart(m_Device);
	}
	void OpenALAudioInputDevice::StopCapture()
	{
		FS_ASSERT(m_Device->DeviceName != "", "No valid device exists");

		alcCaptureStop(m_Device);
	}
}