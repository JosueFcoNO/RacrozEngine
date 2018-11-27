#pragma once

namespace rczEngine
{
	enum { PROFILER_CAPTURED_FRAMES = 1200 };

	class RZ_EXP ProfilerEvent
	{
	public:
		void AddTimeEntry(double time);
		void SaveResults(const char* loggerFile);

	private:
		uint16 m_CurrentEntry = 0;
		double m_EventTimeLine[PROFILER_CAPTURED_FRAMES];

		double m_LargestTime = 0.0f;
		double m_ShortestTime = std::numeric_limits<double>::max();
		double m_AverageTime = 0.0f;
	};
}