#pragma once

#include "Fengshui/Audio/AudioSource.h"
#include "Fengshui/Platform/OpenAL/OpenALAudioBuffer.h"

namespace Fengshui
{
	class OpenALAudioSource : public AudioSource, public std::enable_shared_from_this<OpenALAudioSource>
	{
	public:
		OpenALAudioSource(char* filePath);

		OpenALAudioSource(Ref<OpenALAudioBuffer> buffer);

		~OpenALAudioSource();
	public:
		virtual void Bind() override;

		virtual void UnBind() override;

		virtual void SetSettings(AudioSetting settings) override;

		virtual void Play(bool restart) override;

		virtual void Stop() override;

		virtual void Pause() override;

		virtual void Unpause() override;

	public:
		void SetBuffer(Ref<OpenALAudioBuffer> buffer);

		virtual inline Ref<AudioBuffer> GetBuffer() override { return m_Buffer; }

		virtual inline const Ref<AudioBuffer> GetBuffer() const override { return m_Buffer; }

		inline Ref<OpenALAudioBuffer> GetOpenALBuffer() { return m_Buffer; }
		inline const Ref<OpenALAudioBuffer> GetOpenALBuffer() const { return m_Buffer; }

	private:
		uint32_t m_AudioSourceID = 0;

		Ref<OpenALAudioBuffer> m_Buffer;
	};

}