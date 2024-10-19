#pragma once

#include "Fengshui/Audio/AudioAPI.h"

namespace Fengshui
{
	class OpenALAudioAPI : public AudioAPI
	{
		virtual void Init() override;

		virtual void Shutdown() override;

		//Queries
		virtual const std::vector<const char*> GetAvailableOutputDevices() override;

		virtual const char* GetDefaultOutputDevice() override;

		virtual void GetAvailableOutputDevices(std::vector<const char*>& allDevices, const char* defaultDevice) override;

		virtual const std::vector<const char*> GetAvailableInputDevices() override;

		virtual const char* GetDefaultInputDevice() override;

		virtual void GetAvailableInputDevices(std::vector<const char*>& allDevices, const char* defaultDevice) override;

		//Instance handling
		virtual void CreateOutputDevice(const char* deviceName) override;

		virtual void CreateInputDevice(const char* deviceName) override;
	};
}