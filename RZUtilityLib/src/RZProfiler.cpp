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

	void Profiler::SaveResults(const String& filePath) noexcept
	{
		try
		{
			const String LoggerFile(filePath);
			const auto logger = Logger::PointerOrCreate();

			String eventStr("Event: ");

			///Start the profiler Log
			logger->StartLog(LoggerFile);
			logger->LogMessageToFileLog(LoggerFile, "+++PROFILER LOG+++", eLogMsgType::Error);

			///Log every Game Event
			logger->LogMessageToFileLog(LoggerFile, "+Game Events+", eLogMsgType::Error);

			for (auto event : m_GameEvents)
			{
				eventStr = "Event: " + event.first;
				logger->LogMessageToFileLog(LoggerFile, eventStr, eLogMsgType::Warning);
				event.second.SaveResults(LoggerFile, *logger);
			}

			///Log every Gfx Event
			logger->LogMessageToFileLog(LoggerFile, "+Gfx Events+", eLogMsgType::Error);
			for (auto event : m_GfxEvents)
			{
				eventStr = "Event: " + event.first;
				logger->LogMessageToFileLog(LoggerFile, eventStr, eLogMsgType::Warning);
				event.second.SaveResults(LoggerFile, *logger);
			}

			logger->CloseLog(LoggerFile);
		}
		catch (...)
		{
			Logger::Pointer()->Log("Save Results Failed", eLogMsgType::CriticalError);
		}
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
		try
		{
			switch (eventType)
			{
			case PROF_GFX:
				m_GfxEvents.at(event).AddTimeEntry(time);
				break;
			case PROF_GAME:
				m_GameEvents.at(event).AddTimeEntry(time);
				break;
			default:
				break;
			}
		}
		catch (...)
		{
			Logger::Pointer()->Log("AddTime failed", eLogMsgType::Error);
		}
	}

}