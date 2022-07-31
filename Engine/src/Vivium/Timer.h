#pragma once

#include "Core.h"

namespace Vivium {
	class VIVIUM_API Timer {
		static const long double m_NsToS; // Nanoseconds to seconds ratio
		double m_Time;

	public:
		static const std::chrono::system_clock::time_point compile_time;

		static double GetTime(); // Get time since program compiled
		static std::string GetTimeString();

		Timer();

		// Gets time since last GetElapsed or Start call
		double GetElapsed();
		// Start calculating elapsed time
		void Start();
	};
}
