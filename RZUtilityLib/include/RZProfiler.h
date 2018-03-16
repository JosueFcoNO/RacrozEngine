#pragma once

namespace rczEngine
{
	enum PROFILE_EVENTS
	{
		PROF_GFX,
		PROF_GAME,
		PROF_MAX
	};

	class RZ_UTILITY_EXPORT Profiler
	{
	public:
		~Profiler() {
			Destroy();
		};

		static void Start();
		static Profiler* Pointer();
		static void ShutDown();

		void Destroy();

		void StartProfiler();
		void Reset();
		void SaveResults();

		void NewFrameStart();

		void AddTime(String event, PROFILE_EVENTS eventType);
		void AddTime(String event, double time, PROFILE_EVENTS eventType);

	private:
		static Profiler*& _Instance();

		Map<String, ProfilerEvent> m_GfxEvents;
		Map<String, ProfilerEvent> m_GameEvents;
		double m_AverageFrameTime = 0.0f;

		Timer m_Time;
		Timer m_FrameTime;
	};
}