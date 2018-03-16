#pragma once

namespace rczEngine
{
	class RZ_UTILITY_EXPORT Timer
	{
	public:
		void StartTimer();
		double GetTime();
		double GetFrameTime();

	private:
		double m_CountsPerSecond = 0.0;
		uint64 m_CounterStart = 0;

		uint32 m_FrameCount = 0;
		uint32 m_Fps = 0;

		uint64 m_FrameTimeOld = 0;
		double m_FrameTime;
	};
}