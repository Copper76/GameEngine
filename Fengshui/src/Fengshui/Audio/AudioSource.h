#pragma once

namespace Fengshui
{
	struct AudioSetting
	{

	};

	class AudioSource
	{
	public:
		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void SetSetting(AudioSetting setting) = 0;
		virtual AudioSetting& GetSetting() = 0;

		virtual void Play() = 0;
	};
}