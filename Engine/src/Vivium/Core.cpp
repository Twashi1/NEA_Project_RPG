#include "Core.h"

namespace Vivium {
	Flag::flag_t Flag::m_Flags = 0;

	void Flag::Set(flag_t flag, bool val)
	{
		if (val) {
			m_Flags |= flag;
		}
		else {
			m_Flags &= ~flag;
		}
	}

	void Flag::Toggle(flag_t flag)
	{
		m_Flags ^= flag;
	}

	bool Flag::Test(flag_t flag)
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
