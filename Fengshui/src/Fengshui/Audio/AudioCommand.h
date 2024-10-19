#pragma once

#include "Fengshui/Audio/AudioAPI.h"

namespace Fengshui
{
	class AudioCommand
	{
	public:
		inline static void Init()
		{
			s_AudioAPI->Init();
		}

		inline static void Shutdown()
		{
			s_AudioAPI->Shutdown();
		}

		inline static std::vector<const char*> GetAvailableOutputDevices()
		{
			return s_AudioAPI->GetAvailableOutputDevices();
		}

		inline static const char* GetDefaultOutputDevice()
		{
			return s_AudioAPI->GetDefaultOutputDevice();
		}

		inline static void GetAvailableOutputDevices(std::vector<const char*>& allDevices, const char* defaultDevice)
		{
			s_AudioAPI->GetAvailableOutputDevices(allDevices, defaultDevice);
		}

		inline static std::vector<const char*> GetAvailableInputDevices()
		{
			return s_AudioAPI->GetAvailableInputDevices();
		}

		inline static const char* GetDefaultInputDevice()
		{
			return s_AudioAPI->GetDefaultInputDevice();
		}

		inline static void GetAvailableInputDevices(std::vector<const char*>& allDevices, const char* defaultDevice)
		{
			s_AudioAPI->GetAvailableInputDevices(allDevices, defaultDevice);
		}

		inline static void SetOutputDevice(const char* deviceName)
		{
			s_AudioAPI->CreateOutputDevice(deviceName);
		}

		inline static void SetOutputDeviceDefault()
		{
			s_AudioAPI->CreateOutputDevice();
		}
		
		inline static const std::string GetCurrentOutputDeviceName()
		{
			return s_AudioAPI->GetOutputDeviceName();
		}

		inline static void SetInputDevice(const char* deviceName)
		{
			s_AudioAPI->CreateInputDevice(deviceName);
		}

		inline static void SetInputDeviceDefault()
		{
			s_AudioAPI->CreateInputDevice();
		}

		inline static const std::string GetCurrentInputDeviceName()
		{
			return s_AudioAPI->GetInputDeviceName();
		}

	private:
		static Scope<AudioAPI> s_AudioAPI;
	};
}