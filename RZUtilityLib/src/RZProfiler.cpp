#include <RZUtilityPCH.h>

namespace rczEngine
{
	Profiler*& Profiler::_Instance() noexcept
	{
		static Profiler* instance = nullptr;
		return instance;
	}

	void Profiler::Start()
	{
		(_Instance()) = new Profiler;
	}

	Profiler* Profiler::Pointer() noexcept
	{
		return _Instance();
	}

	void Profiler::ShutDown() noexcept
	{
		delete _Instance();
	}

	void Profiler::Destroy() noexcept
	{
		try
		{
			SaveResults();
			m_ProfilerEvents.clear();
		}
		catch (...)
		{
			std::abort();
		}
	}

	void Profiler::StartProfiler() noexcept
	{
		m_Time.StartTimer();
		m_FrameTime.StartTimer();
	}

	void Profiler::Reset() noexcept
	{
		m_Time.StartTimer();
		m_GameEvents.clear();
		m_ProfilerEvents.clear();
	}

	void Profiler::SaveResults(const String& filePath)
	{
		const String LoggerFile(filePath);
		const auto logger = Logger::PointerOrCreate();
		
		String eventStr("Event: ");

		///Start the profiler Log
		logger->StartLog(LoggerFile);
		logger->LogMessageToFileLog(LoggerFile, "+++PROFILER LOG+++", eLogMsgType::Error);

		///Log every Event
		logger->LogMessageToFileLog(LoggerFile, "+Events+", eLogMsgType::Error);

		for (auto event : m_ProfilerEvents)
		{
			eventStr = "Event: " + event.first;
			logger->LogMessageToFileLog(LoggerFile, eventStr, eLogMsgType::Warning);
			event.second.SaveResults(LoggerFile, *logger);
		}

		logger->CloseLog(LoggerFile);
	}

	void Profiler::NewFrameStart() noexcept
	{
		m_AverageFrameTime = (m_AverageFrameTime + m_FrameTime.GetFrameTime())/2.0f;
	}

	void Profiler::AddTime(const String&& event) noexcept
	{
		AddTime(std::move(event), m_Time.GetFrameTime());
	}

	void Profiler::AddTime(const String&& event, long double time) noexcept
	{
		try
		{
			if (m_ProfilerEvents.find(event) == m_ProfilerEvents.end())
			{
				m_ProfilerEvents[event] = ProfilerEvent();
			}

			m_ProfilerEvents.at(event).AddTimeEntry(time);
		}
		catch (...)
		{
			std::abort();
		}
	}

}