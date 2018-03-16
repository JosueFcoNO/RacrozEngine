#include <RZUtilityPCH.h>
#include <Windows.h>

namespace rczEngine
{
	void Timer::StartTimer()
	{
		LARGE_INTEGER frequencyCount;
		QueryPerformanceFrequency(&frequencyCount);

		m_CountsPerSecond = double(frequencyCount.QuadPart);

		QueryPerformanceCounter(&frequencyCount);
		m_CounterStart = frequencyCount.QuadPart;
		m_FrameTimeOld = m_CounterStart;
	}

	double Timer::GetTime()
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return double(currentTime.QuadPart - m_CounterStart) / m_CountsPerSecond;
	}

	double Timer::GetFrameTime()
	{
		LARGE_INTEGER currentTime;
		__int64 tickCount;
		QueryPerformanceCounter(&currentTime);

		tickCount = currentTime.QuadPart - m_FrameTimeOld;
		m_FrameTimeOld = currentTime.QuadPart;

		if (tickCount <= 0)
			tickCount = 0;

		return double(tickCount) / m_CountsPerSecond;
	}
}