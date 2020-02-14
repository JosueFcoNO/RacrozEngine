#pragma once

namespace rczEngine
{
	class ProfilerObj
	{
	public:
		RZ_EXP ProfilerObj(const String&& event) :
			m_EventID(event)
		{
			m_InnerTimer.StartTimer();
		};

		ProfilerObj() = delete;
		ProfilerObj(const ProfilerObj& other) = delete;
		ProfilerObj& operator=(const ProfilerObj& other) = delete;
		ProfilerObj(const ProfilerObj&& other) = delete;
		ProfilerObj& operator=(const ProfilerObj&& other) = delete;


		RZ_EXP ~ProfilerObj() noexcept
		{
			RegisterTime();
		};

		RZ_EXP FORCEINLINE void RegisterTime() noexcept
		{
			Profiler::Pointer()->AddTime(std::move(m_EventID), m_InnerTimer.GetFrameTime());
		};

	private:
		Timer m_InnerTimer;
		String m_EventID;
	};
};