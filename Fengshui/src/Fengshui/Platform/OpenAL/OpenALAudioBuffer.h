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

		virtual std::string GetFileName() override;

		inline const uint32_t* GetBufferIDs() const { return m_BufferIDs; }
		inline const int GetBufferNum() const { return m_BufferNum; }
	private:
		uint32_t* m_BufferIDs;
		int m_BufferNum;
		AudioData m_AudioData;

		std::string m_FileName;
	};
}