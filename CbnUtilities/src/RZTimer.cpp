#include "CbnUtilitiesPCH.h"
#include <Windows.h>

namespace rczEngine
{
	void Timer::StartTimer() noexcept
	{
		LARGE_INTEGER frequencyCount;
		QueryPerformanceFrequency(&frequencyCount);
		
		m_CountsPerSecond = gsl::narrow_cast<double>(frequencyCount.QuadPart);

		QueryPerformanceCounter(&frequencyCount);
		m_CounterStart = frequencyCount.QuadPart;
		m_FrameTimeOld = m_CounterStart;
	}

	double Timer::GetTime() noexcept
	{
		LARGE_INTEGER currentTime;
		QueryPerformanceCounter(&currentTime);
		return gsl::narrow_cast<double>(currentTime.QuadPart - m_CounterStart) / m_CountsPerSecond;
	}

	double Timer::GetFrameTime() noexcept
	{
		LARGE_INTEGER currentTime;
		__int64 tickCount;
		QueryPerformanceCounter(&currentTime);

		tickCount = currentTime.QuadPart - m_FrameTimeOld;
		m_FrameTimeOld = currentTime.QuadPart;

		if (tickCount <= 0)
			tickCount = 0;

		return gsl::narrow_cast<double>(tickCount) / m_CountsPerSecond;
	}
}