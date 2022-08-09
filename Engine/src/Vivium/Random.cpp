#include "Random.h"
#include "Application.h"

namespace Vivium {
	std::mt19937 Random::m_Generator;

	void Random::m_Init()
	{
		m_Generator = std::mt19937(
			// i actually remembered this you know, im proud of myself
			std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count()
		);
	}

	int Random::GetInt(int min, int max)
	{
		std::uniform_int_distribution<int> dist(min, max);

		return dist(m_Generator);
	}

	float Random::GetFloat(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);

		return dist(m_Generator);
	}

	Vector2<float> Random::GetVector2f(float min, float max)
	{
		std::uniform_real_distribution<float> dist(min, max);

		return Vector2<float>(dist(m_Generator), dist(m_Generator));
	}

}