#include "Core.h"

namespace Vivium {
	uint32_t Flag::m_Flags = 0;

	void Flag::Set(uint32_t flag)
	{
		m_Flags |= flag;
	}

	void Flag::Toggle(uint32_t flag)
	{
		m_Flags ^= flag;
	}

	bool Flag::Test(uint32_t flag)
	{
		return m_Flags & flag;
	}

	float InverseSquareRoot(float number)
	{
		long i;
		float x2, y;
		const float threehalfs = 1.5F;

		x2 = number * 0.5F;
		y = number;
		i = *(long*)&y;
		i = 0x5f3759df - (i >> 1);
		y = *(float*)&i;
		y = y * (threehalfs - (x2 * y * y));

		return y;
	}
}
