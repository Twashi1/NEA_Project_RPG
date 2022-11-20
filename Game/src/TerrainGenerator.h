#pragma once

#include "Core.h"
#include "Biome.h"

namespace Game {
	class World;

	/// <summary>
	/// Generate tile, biome, and npcs for a given x, y coordinate
	/// Dictates world biomes
	/// </summary>
	class TerrainGenerator {
	private:
		static constexpr int BIOME_WAVELENGTH = 64;

		static Vivium::Noise::Cellular m_BiomeNoise;
		static std::map<float, Biome::ID> m_NoiseToBiomeMap;

		/// <summary>
		/// Get biome ID for a given coordinate
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <returns></returns>
		static Biome::ID m_GetBiomeID(int x, int y);

		/// <summary>
		/// Initialise biome generator given a seed
		/// </summary>
		/// <param name="seed"></param>
		static void m_Init(unsigned int seed);

	public:
		/// <summary>
		/// Generate tile, structures, etc. for the given x, y coordinate
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="tile"></param>
		/// <param name="region"></param>
		/// <param name="world"></param>
		/// <param name="structures"></param>
		/// <returns></returns>
		static Biome::ID GenerateAt(int x, int y, Tile& tile, Region& region, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures);

		friend World;
	};
}
