#pragma once

#include "Core.h"
#include "Biome.h"

namespace Game {
	class World;

	class TerrainGenerator {
	private:
		static constexpr int BIOME_WAVELENGTH = 64;

		static Vivium::Noise::Cellular m_BiomeNoise;
		static std::map<float, Biome::ID> m_NoiseToBiomeMap;

		static Biome::ID m_GetBiomeID(int x, int y);

		static void m_Init(unsigned int seed);

	public:
		static Biome::ID GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures);

		friend World;
	};
}
