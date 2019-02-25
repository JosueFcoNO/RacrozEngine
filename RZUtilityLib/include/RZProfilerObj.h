#pragma once

namespace rczEngine
{
	class ProfilerObj
	{
	public:
		RZ_EXP ProfilerObj(const String& event, PROFILE_EVENTS eventType) noexcept :
			m_EventID(event), 
			m_EventType(eventType)
		{
			m_InnerTimer.StartTimer();
		};

		RZ_EXP ~ProfilerObj() noexcept
		{
			RegisterTime();
		};

		RZ_EXP FORCEINLINE void RegisterTime() noexcept
		{
			Profiler::Pointer()->AddTime(m_EventID, m_InnerTimer.GetFrameTime(), m_EventType);
		};

	private:
		Timer m_InnerTimer;
		String m_EventID;
		PROFILE_EVENTS m_EventType;
	};
};