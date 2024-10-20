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

		virtual void SetSetting(AudioSetting setting) override;

		virtual AudioSetting& GetSetting() override;

		virtual void Play(bool restart) override;

		virtual void Stop() override;

		virtual void Pause() override;

		virtual void Unpause() override;

	public:
		void SetBuffer(Ref<OpenALAudioBuffer> buffer);

		inline Ref<OpenALAudioBuffer> GetBuffer() { return m_Buffer; }

		inline const Ref<OpenALAudioBuffer> GetBuffer() const { return m_Buffer; }

	private:
		uint32_t m_AudioSourceID = 0;

		Ref<OpenALAudioBuffer> m_Buffer;

		AudioSetting m_AudioSetting;
	};

}