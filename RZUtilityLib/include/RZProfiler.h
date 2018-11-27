#pragma once

namespace rczEngine
{
	enum PROFILE_EVENTS
	{
		PROF_GFX,
		PROF_GAME,
		PROF_MAX
	};

	class Profiler
	{
	public:
		RZ_EXP ~Profiler() {
			Destroy();
		};

		RZ_EXP static void Start();
		RZ_EXP static Profiler* Pointer();
		RZ_EXP static void ShutDown();

		RZ_EXP void Destroy();

		RZ_EXP void StartProfiler();
		RZ_EXP void Reset();
		RZ_EXP void SaveResults();

		RZ_EXP void NewFrameStart();

		RZ_EXP void AddTime(String event, PROFILE_EVENTS eventType);
		RZ_EXP void AddTime(String event, double time, PROFILE_EVENTS eventType);

	private:
		static Profiler*& _Instance();

		Map<String, ProfilerEvent> m_GfxEvents;
		Map<String, ProfilerEvent> m_GameEvents;
		double m_AverageFrameTime = 0.0f;

		Timer m_Time;
		Timer m_FrameTime;
	};
}