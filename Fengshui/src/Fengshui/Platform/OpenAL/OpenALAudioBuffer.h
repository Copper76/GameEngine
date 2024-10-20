#pragma once
#include "Fengshui/Audio/AudioBuffer.h"

namespace Fengshui
{
	class OpenALAudioBuffer : public AudioBuffer
	{
	public:
		OpenALAudioBuffer(const char* filePath);

		~OpenALAudioBuffer();
	public:
		virtual void Bind(const char* filePath) override;

		virtual void UnBind() override;

		inline const uint32_t GetID() const { return m_AudioID; }
	private:
		uint32_t m_AudioID;
		AudioData m_AudioData;
	};
}