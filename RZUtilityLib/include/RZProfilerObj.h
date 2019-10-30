#pragma once

namespace rczEngine
{
	class ProfilerObj
	{
	public:
		RZ_EXP ProfilerObj(const String& event, PROFILE_EVENTS eventType) :
			m_EventID(event), 
			m_EventType(eventType)
		{
			m_InnerTimer.StartTimer();
		};

		RZ_EXP ~ProfilerObj() noexcept
		{
			RegisterTime();
		};

		RZ_EXP ProfilerObj() = delete;
		RZ_EXP ProfilerObj(const ProfilerObj& other) = delete;
		RZ_EXP ProfilerObj(const ProfilerObj&& other) = delete;
		RZ_EXP ProfilerObj& operator=(const ProfilerObj& rhs) = delete;
		RZ_EXP ProfilerObj& operator=(const ProfilerObj&& rhs) = delete;

		RZ_EXP FORCEINLINE void RegisterTime() noexcept
		{
			try
			{
				Profiler::Pointer()->AddTime(m_EventID, m_InnerTimer.GetFrameTime(), m_EventType);
			}
			catch (...)
			{
				Logger::Pointer()->Log("Profiler: RegisterTime Failed", eLogMsgType::Error);
			}
		};

	private:
		Timer m_InnerTimer;
		String m_EventID;
		PROFILE_EVENTS m_EventType;
	};
};