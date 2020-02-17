#pragma once

namespace rczEngine
{

	//#define RZ_PROFILING

	/// The profiler, can count the time between frames and functions using ProfileObjects.
	class MemoryProfiler
	{
	public:
		RZ_EXP ~MemoryProfiler() noexcept { Destroy(); };

		RZ_EXP MemoryProfiler() noexcept {};						   // constructor
		RZ_EXP MemoryProfiler(const MemoryProfiler&) = delete;             // copy constructor
		RZ_EXP MemoryProfiler& operator=(const MemoryProfiler&) = delete;  // copy assignment
		RZ_EXP MemoryProfiler(MemoryProfiler&&) = delete;                  // move constructor
		RZ_EXP MemoryProfiler& operator=(MemoryProfiler&&) = delete;       // move assignment

		RZ_EXP static void Start();
		RZ_EXP static MemoryProfiler* Pointer() noexcept;
		RZ_EXP static void ShutDown() noexcept;

		/// Destroy the profiler, clear everything.
		RZ_EXP void Destroy() noexcept;

		/// Saves the profiler Log to the file path given in the param.
		RZ_EXP void SaveResults(const String& filePath = "autoProfileLog ");

		RZ_EXP void AddMemory(const String && event) noexcept;

		RZ_EXP Vector<String> GetReport() noexcept { return Vector<String>(); };

	private:
		static MemoryProfiler*& _Instance() noexcept;

		Map<String, ProfilerEvent> m_MemoryProfilerEvents;

		Timer m_Time;
	};
}