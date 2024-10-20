#include "fspch.h"
#include "OpenALAudioOutputDevice.h"

#include <alc/alcmain.h>

namespace Fengshui
{
	void OpenALAudioOutputDevice::Create(const char* deviceName)
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

		m_Device = alcOpenDevice(deviceName);

		FS_ASSERT(m_Device, "Failed to initialise OpenAL output device");

		m_Context = alcCreateContext(m_Device, nullptr); // what is the nullptr

		FS_ASSERT(m_Context, "Failed to initialise OpenAL output context");

		ALCboolean success = alcMakeContextCurrent(m_Context);

		FS_ASSERT(success, "Failed to make the context current");
	}

	const std::string OpenALAudioOutputDevice::GetDeviceName()
	{
		FS_ASSERT(m_Device->DeviceName != "", "No valid device exists");
		return m_Device->DeviceName;
	}

	void OpenALAudioOutputDevice::Destroy()
	{
		if (alcGetCurrentContext() == m_Context)
		{
			alcMakeContextCurrent(nullptr);
		}

		if (m_Context) 
		{
			alcDestroyContext(m_Context);
			m_Context = nullptr;
		}

		if (m_Device)
		{
			alcCloseDevice(m_Device);
			m_Device = nullptr;
		}
	}
}
