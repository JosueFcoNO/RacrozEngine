#pragma once

namespace rczEngine
{
	enum { PROFILER_CAPTURED_FRAMES = 1200 };

	class RZ_EXP ProfilerEvent
	{
	public:
		void AddTimeEntry(long double time) noexcept;
		void SaveResults(const String& loggerFile, Logger& logger) noexcept;

	private:
		uint16 m_CurrentEntry = 0;
		long double m_EventTimeLine[PROFILER_CAPTURED_FRAMES];

		long double m_LargestTime = 0.0f;
		long double m_ShortestTime = std::numeric_limits<double>::max();
		long double m_AverageTime = 0.0f;
	};
}