#pragma once

namespace Fengshui
{
	struct AudioData
	{
		uint32_t SampleRate;
		uint32_t Channels;
		uint64_t NumSamples;
		uint8_t BitsPerSample;
	};

	class AudioLoader
	{
	public:
		static short* Load(const char* filePath, AudioData& data);
	};
}