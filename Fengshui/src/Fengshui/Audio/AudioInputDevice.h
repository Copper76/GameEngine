#pragma once

namespace Fengshui
{
	class AudioInputDevice
	{
	public:
		virtual void Create(const char* deviceName, const bool startCapture = false) = 0;

		virtual const std::string GetDeviceName() = 0;

		virtual void Destroy() = 0;
		//operations pending
	};
}
