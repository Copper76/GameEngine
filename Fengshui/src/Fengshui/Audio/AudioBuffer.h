#pragma once

#include "AudioLoader.h"

namespace Fengshui
{
	class AudioBuffer
	{
	public:
		virtual void Bind(const char* filePath) = 0;

		virtual void UnBind() = 0;

		virtual std::string GetFileName() = 0;
	};
}