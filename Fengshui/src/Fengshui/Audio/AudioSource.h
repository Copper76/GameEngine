#pragma once

namespace Fengshui
{
	struct AudioSetting
	{
		bool IsLoop = false;
	};

	class AudioSource
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetSetting(AudioSetting setting) = 0;
		virtual AudioSetting& GetSetting() = 0;

		virtual void Play(bool restart = true) = 0;
		virtual void Stop() = 0;
		virtual void Pause() = 0;
		virtual void Unpause() = 0;
	};
}