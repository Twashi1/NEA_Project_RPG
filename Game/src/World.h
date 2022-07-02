#pragma once

#include <Engine.h>

class World {
	Noise::Interpolated m_noise_gen;

public:
	unsigned int seed;

	World(unsigned int seed);
};