#include <RZUtilityPCH.h>

namespace rczEngine
{
	Profiler*& Profiler::_Instance()
	{
		static Profiler* instance = nullptr;
		return instance;
	}

	void Profiler::Start()
	{
		(_Instance()) = new Profiler;
	}

	Profiler* Profiler::Pointer()
	{
		return _Instance();
	}

	void Profiler::ShutDown()
	{
		delete _Instance();
	}

	void Profiler::Destroy()
	{
		m_GfxEvents.clear();
		m_GameEvents.clear();
		SaveResults();
	}

	void Profiler::StartProfiler()
	{
		m_Time.StartTimer();
		m_FrameTime.StartTimer();
	}

	void Profiler::Reset()
	{
		m_Time.StartTimer();
		m_GameEvents.clear();
		m_GfxEvents.clear();
	}

	void Profiler::SaveResults()
	{
		String LoggerFile("ProfilerLog.html");
		auto logger = Logger::Pointer();
		
		///Start the profiler Log
		logger->StartLog(LoggerFile.c_str());
		logger->LogMessage(LoggerFile.c_str(), "+++PROFILER LOG+++", Logger::e_ERROR);

		///Log every Game Event
		logger->LogMessage(LoggerFile.c_str(), "+Game Events+", Logger::e_ERROR);
		for (auto it = m_GameEvents.begin(); it != m_GameEvents.end(); ++it)
		{
			logger->LogMessage(LoggerFile.c_str(), String("+Event: "+(it->first)));
			it->second.SaveResults(LoggerFile.c_str(), m_AverageFrameTime);
		}

		///Log every Gfx Event
		logger->LogMessage(LoggerFile.c_str(), "+Gfx Events+", Logger::e_ERROR);
		for (auto it = m_GfxEvents.begin(); it != m_GfxEvents.end(); ++it)
		{
			logger->LogMessage(LoggerFile.c_str(), String("+Event: " + (it->first)));
			it->second.SaveResults(LoggerFile.c_str(), m_AverageFrameTime);
		}

		logger->CloseLog(LoggerFile.c_str());
	}

	void Profiler::NewFrameStart()
	{
		m_AverageFrameTime = (m_AverageFrameTime + m_FrameTime.GetFrameTime())/2.0f;
	}

	void Profiler::AddTime(String event, PROFILE_EVENTS eventType)
	{
		AddTime(event, m_Time.GetFrameTime(), eventType);
	}

	void Profiler::AddTime(String event, double time, PROFILE_EVENTS eventType)
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