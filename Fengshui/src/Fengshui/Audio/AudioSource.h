#pragma once

#include "Fengshui/Audio/AudioBuffer.h"

namespace Fengshui
{
	struct AudioSetting
	{
		bool IsLoop = false;
		bool PlayOnStart = true;
		bool Is3D = false;
	};

	class AudioSource
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetSettings(AudioSetting settings) = 0;

		virtual void Play(bool restart = true) = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Unpause() = 0;

		virtual inline Ref<AudioBuffer> GetBuffer() = 0;

		virtual const inline Ref<AudioBuffer> GetBuffer() const = 0;
	};
}