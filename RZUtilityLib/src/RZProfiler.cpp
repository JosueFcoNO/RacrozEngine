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

	void Profiler::ShutDown()
	{
		delete _Instance();
	}

	void Profiler::Destroy() noexcept
	{
		SaveResults();
		m_GfxEvents.clear();
		m_GameEvents.clear();
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
		m_GfxEvents.clear();
	}

	void Profiler::SaveResults(const String& filePath)
	{
		const String LoggerFile(filePath);
		const auto logger = Logger::PointerOrCreate();
		
		const String eventStr("Event: ");

		///Start the profiler Log
		logger->StartLog(LoggerFile);
		logger->LogMessageToFileLog(LoggerFile, "+++PROFILER LOG+++", eLogMsgType::Error);

		///Log every Game Event
		logger->LogMessageToFileLog(LoggerFile, "+Game Events+", eLogMsgType::Error);
		for (auto event : m_GameEvents)
		{
			logger->LogMessageToFileLog(LoggerFile, eventStr + event.first, eLogMsgType::Warning);
			event.second.SaveResults(LoggerFile, *logger);
		}

		///Log every Gfx Event
		logger->LogMessageToFileLog(LoggerFile, "+Gfx Events+", eLogMsgType::Error);
		for (auto event : m_GfxEvents)
		{
			logger->LogMessageToFileLog(LoggerFile, eventStr + event.first, eLogMsgType::Warning);
			event.second.SaveResults(LoggerFile, *logger);
		}

		logger->CloseLog(LoggerFile);
	}

	void Profiler::NewFrameStart() noexcept
	{
		m_AverageFrameTime = (m_AverageFrameTime + m_FrameTime.GetFrameTime())/2.0f;
	}

	void Profiler::AddTime(const String& event, PROFILE_EVENTS eventType) noexcept
	{
		AddTime(event, m_Time.GetFrameTime(), eventType);
	}

	void Profiler::AddTime(const String& event, long double time, PROFILE_EVENTS eventType) noexcept
	{
		switch (eventType)
		{
		case PROF_GFX:
				m_GfxEvents[event].AddTimeEntry(time);
			break;
		case PROF_GAME:
				m_GameEvents[event].AddTimeEntry(time);
			break;
		default:
			break;
		}
	}

}