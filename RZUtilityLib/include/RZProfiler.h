#pragma once

namespace rczEngine
{
	enum PROFILE_EVENTS
	{
		PROF_GFX,
		PROF_GAME,
		PROF_MAX
	};

#define RZ_PROFILING

	class Profiler
	{
	public:
		RZ_EXP ~Profiler() noexcept { Destroy(); };

		RZ_EXP Profiler() noexcept {};						   // constructor
		RZ_EXP Profiler(const Profiler&) = delete;             // copy constructor
		RZ_EXP Profiler& operator=(const Profiler&) = delete;  // copy assignment
		RZ_EXP Profiler(Profiler&&) = delete;                  // move constructor
		RZ_EXP Profiler& operator=(Profiler&&) = delete;       // move assignment

		RZ_EXP static void Start();
		RZ_EXP static Profiler* Pointer() noexcept;
		RZ_EXP static void ShutDown();

		RZ_EXP void Destroy() noexcept;

		RZ_EXP void StartProfiler() noexcept;
		RZ_EXP void Reset() noexcept;
		RZ_EXP void SaveResults(const String& filePath = "autoProfileLog ") noexcept;

		RZ_EXP void NewFrameStart() noexcept;

		RZ_EXP void AddTime(const String& event, PROFILE_EVENTS eventType) noexcept;
		RZ_EXP void AddTime(const String& event, long double time, PROFILE_EVENTS eventType) noexcept;

	private:
		static Profiler*& _Instance() noexcept;

		Map<String, ProfilerEvent> m_GfxEvents;
		Map<String, ProfilerEvent> m_GameEvents;
		double m_AverageFrameTime = 0.0f;

		Timer m_Time;
		Timer m_FrameTime;
	};
}