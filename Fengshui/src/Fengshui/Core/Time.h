#pragma once

namespace Fengshui
{
	class Time
	{
	public:
		Time(float time = 0.0f) : m_Time(time)
		{

		}

		float GetTime() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.0f; }

		float GetDeltaTime() const { return m_DeltaTime; }
		float GetDeltaTimeMilli() const { return m_DeltaTime * 1000.0f; }

		void UpdateTime(float time)
		{
			m_DeltaTime = time - m_Time;
			m_Time = time;
		}

	private:
		float m_Time;
		float m_DeltaTime = 0.0f;
	};
}