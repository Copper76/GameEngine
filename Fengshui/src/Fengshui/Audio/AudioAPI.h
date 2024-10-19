#pragma once

#include "AudioOutputDevice.h"
#include "AudioInputDevice.h"

namespace Fengshui
{
	class AudioAPI
	{
	public:
		enum class API
		{
			None = 0, OpenAL = 1,
		};

	public:
		virtual ~AudioAPI() = default;

		virtual void Init() = 0;

		virtual void Shutdown() = 0;

		//Queries
		virtual const std::vector<const char*> GetAvailableOutputDevices() = 0;

		virtual const char* GetDefaultOutputDevice() = 0;

		virtual void GetAvailableOutputDevices(std::vector<const char*>& allDevices, const char* defaultDevice) = 0;

		virtual const std::vector<const char*> GetAvailableInputDevices() = 0;

		virtual const char* GetDefaultInputDevice() = 0;

		virtual void GetAvailableInputDevices(std::vector<const char*>& allDevices, const char* defaultDevice) = 0;

		//Instance handling
		virtual void CreateInputDevice(const char* deviceName = nullptr) = 0;

		virtual void CreateOutputDevice(const char* deviceName = nullptr) = 0;

		inline Ref<AudioOutputDevice> GetOutputDevice() { return s_AudioOutputDevice; }

		inline const std::string GetOutputDeviceName() { return s_AudioOutputDevice->GetDeviceName(); }

		inline Ref<AudioInputDevice> GetInputDevice() { return s_AudioInputDevice; }

		inline const std::string GetInputDeviceName() { return s_AudioInputDevice->GetDeviceName(); }

		inline static API GetAPI() { return s_API; }

	protected:
		Ref<AudioOutputDevice> s_AudioOutputDevice;
		Ref<AudioInputDevice> s_AudioInputDevice;

	private:
		static API s_API;
	};
}

