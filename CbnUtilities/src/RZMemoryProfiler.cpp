#include "CbnUtilitiesPCH.h"

namespace rczEngine
{
	MemoryProfiler*& MemoryProfiler::_Instance() noexcept
	{
		static MemoryProfiler* instance = nullptr;
		return instance;
	}

	void MemoryProfiler::Start()
	{
		(_Instance()) = new MemoryProfiler;
	}

	MemoryProfiler* MemoryProfiler::Pointer() noexcept
	{
		return _Instance();
	}

	void MemoryProfiler::ShutDown() noexcept
	{
		delete _Instance();
	}

	void MemoryProfiler::Destroy() noexcept
	{
		try
		{
			SaveResults();
			m_MemoryProfilerEvents.clear();
		}
		catch (...)
		{
			std::abort();
		}
	}

	void MemoryProfiler::SaveResults(const String& filePath)
	{
		const String LoggerFile(filePath);
		const auto logger = Logger::PointerOrCreate();

		String eventStr("Event: ");

		///Start the profiler Log
		logger->StartLog(LoggerFile);
		logger->LogMessageToFileLog(LoggerFile, "+++PROFILER LOG+++", eLogMsgType::Error);

		///Log every Event
		logger->LogMessageToFileLog(LoggerFile, "+Events+", eLogMsgType::Error);

		for (auto event : m_MemoryProfilerEvents)
		{
			eventStr = "Event: " + event.first;
			logger->LogMessageToFileLog(LoggerFile, eventStr, eLogMsgType::Warning);
			event.second.SaveResults(LoggerFile, *logger);
		}

		logger->CloseLog(LoggerFile);
	}

	void MemoryProfiler::AddMemory(const String&& event) noexcept
	{
		try
		{
			
		}
		catch (...)
		{
			std::abort();
		}
	}


}