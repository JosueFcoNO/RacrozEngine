#pragma once

namespace rczEngine
{
	///A high res timer class.
	class RZ_EXP Timer
	{
	public:
		///Starts the timer.
		void StartTimer() noexcept;
		///Returns the time since the timer was started.
		double GetTime() noexcept;
		///Returns the time since the last GetFrameTime.
		double GetFrameTime() noexcept;

	private:
		double m_CountsPerSecond = 0.0;
		uint64 m_CounterStart = 0;

		uint64 m_FrameTimeOld = 0;
		double m_FrameTime;
	};
}