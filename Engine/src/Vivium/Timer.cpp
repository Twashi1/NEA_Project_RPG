#include "Timer.h"

namespace Vivium {
	const long double Timer::m_NsToS = pow(10, -9);
	const std::chrono::system_clock::time_point Timer::compile_time = std::chrono::system_clock::now();

	double Timer::GetTime()
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - compile_time).count() * m_NsToS;
	}

	void Timer::Start()
	{
		m_Time = GetTime();
	}

	std::string Timer::GetTimeString()
	{
		return std::format("{:%H:%M:%OS}", std::chrono::system_clock::now());
	}

	Timer::Timer()
		: m_Time(GetTime())
	{}

	double Timer::GetElapsed()
	{
		// Get current time
		double now = Timer::GetTime();
		// Calculate difference
		double elapsed = now - m_Time;
		// Update our time
		m_Time = now;
		// Return elapsed time
		return elapsed;
	}
}