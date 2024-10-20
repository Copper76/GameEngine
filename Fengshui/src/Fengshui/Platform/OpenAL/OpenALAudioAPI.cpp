#include "fspch.h"
#include "OpenALAudioAPI.h"
#include "OpenALAudioOutputDevice.h"
#include "OpenALAudioInputDevice.h"
#include "OpenALAudioBufferManager.h"

#include <AL/alc.h>

namespace Fengshui
{
	void OpenALAudioAPI::Init()
	{
		s_AudioOutputDevice = MakeRef<OpenALAudioOutputDevice>();
		s_AudioInputDevice = MakeRef<OpenALAudioInputDevice>();

		s_AudioOutputDevice->Create(nullptr);
		s_AudioInputDevice->Create(nullptr);
	}

	void OpenALAudioAPI::Shutdown()
	{
		s_AudioOutputDevice->Destroy();
		s_AudioInputDevice->Destroy();
	}

	const std::vector<const char*> OpenALAudioAPI::GetAvailableOutputDevices()
	{
		std::vector<const char*> deviceNames;

		deviceNames.clear();

		const ALCchar* devices = alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER);

		const ALCchar* deviceName = devices;
		while (*deviceName != '\0') 
		{
			deviceNames.push_back(deviceName);
			deviceName += strlen(deviceName) + 1;
		}

		return deviceNames;
	}

	const char* OpenALAudioAPI::GetDefaultOutputDevice()
	{
		return alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	}

	void OpenALAudioAPI::GetAvailableOutputDevices(std::vector<const char*>& allDevices, const char* defaultDevice)
	{
		allDevices.clear();

		allDevices = GetAvailableOutputDevices();
		defaultDevice = GetDefaultOutputDevice();
	}

	const std::vector<const char*> OpenALAudioAPI::GetAvailableInputDevices()
	{
		std::vector<const char*> deviceNames;

		deviceNames.clear();

		const ALCchar* devices = alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER);

		const ALCchar* deviceName = devices;
		while (*deviceName != '\0')
		{
			deviceNames.push_back(deviceName);
			deviceName += strlen(deviceName) + 1;
		}

		return deviceNames;
	}

	const char* OpenALAudioAPI::GetDefaultInputDevice()
	{
		return alcGetString(NULL, ALC_CAPTURE_DEFAULT_DEVICE_SPECIFIER);
	}

	void OpenALAudioAPI::GetAvailableInputDevices(std::vector<const char*>& allDevices, const char* defaultDevice)
	{
		allDevices.clear();

		allDevices = GetAvailableInputDevices();
		defaultDevice = GetDefaultInputDevice();
	}

	void OpenALAudioAPI::CreateOutputDevice(const char* deviceName)
	{
		s_AudioOutputDevice->Create(deviceName);
	}

	void OpenALAudioAPI::CreateInputDevice(const char* deviceName)
	{
		s_AudioInputDevice->Create(deviceName);
	}

	Ref<AudioSource> OpenALAudioAPI::CreateAudioSource(char* filePath)
	{
		Ref<OpenALAudioSource> audioSource = MakeRef<OpenALAudioSource>(filePath);
		OpenALAudioBufferManager::AddSourceMapping(audioSource->GetBuffer(), audioSource);

		return audioSource;
	}
}