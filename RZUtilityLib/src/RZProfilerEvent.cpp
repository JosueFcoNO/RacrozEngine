#include <RZUtilityPCH.h>

namespace rczEngine
{
	void ProfilerEvent::AddTimeEntry(long double time) noexcept
	{
		///Add the time to the array.
		m_EventTimeLine[m_CurrentEntry] = time;

		///Move m_CurrentEntry to the next element and wrap it with module.
		m_CurrentEntry = (m_CurrentEntry + 1) % PROFILER_CAPTURED_FRAMES;

		///Check if it is the shortest or the longest time captured.
		if (time < m_ShortestTime) m_ShortestTime = time;
		else if (time > m_LargestTime) m_LargestTime = time;

		///Compute the new Average time using the new time entry.
		if (m_CurrentEntry > 1)
			m_AverageTime = (m_AverageTime + time) / 2.0f;
		else
			m_AverageTime = time;
	}

	void ProfilerEvent::SaveResults(const String& loggerFile, Logger& logger)
	{
		logger.LogMessageToFileLog(loggerFile, "Average Time: ", m_AverageTime * 10);
		logger.LogMessageToFileLog(loggerFile, "Largest Time: ", m_LargestTime * 10);
		logger.LogMessageToFileLog(loggerFile, "Shortest Time: ", m_ShortestTime * 10);
	}
};