#pragma once

namespace Fengshui
{
	class AudioOutputDevice
	{
	public:
		virtual void Create(const char* deviceName) = 0;

		virtual const std::string GetDeviceName() = 0;

		virtual void Destroy() = 0;
		//operations pending
	};
}