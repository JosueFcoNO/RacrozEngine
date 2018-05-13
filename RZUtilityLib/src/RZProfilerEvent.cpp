#include <RZUtilityPCH.h>

namespace rczEngine
{
	void ProfilerEvent::AddTimeEntry(double time)
	{
		///Add the time to the array.
		m_EventTimeLine[m_CurrentEntry] = time;

		///Move m_CurrentEntry to the next element and wrap it with module.
		m_CurrentEntry = (m_CurrentEntry + 1) % PROFILER_CAPTURED_FRAMES;

		///Check if it is the shortest or the longest time captured.
		if (time < m_ShortestTime) m_ShortestTime = time;
		else if (time > m_LargestTime) m_LargestTime = time;

		///Compute the new Average time using the new time entry.
		m_AverageTime = (m_AverageTime + time) / 2.0f;
	}

	void ProfilerEvent::SaveResults(const char * loggerFile)
	{
		auto logg = Logger::Pointer();

		logg->LogMessage(loggerFile, "Average Time: ", (float)m_AverageTime*1000.0f);

		//logg->LogMessage(loggerFile, "Largest Time: ", (float)m_LargestTime);
		//logg->LogMessage(loggerFile, "Shortest Time: ", (float)m_ShortestTime);
	}
};