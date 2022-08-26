#include "TerrainGenerator.h"
#include "World.h"

namespace Game {
	Vivium::Noise::Cellular TerrainGenerator::m_BiomeNoise;

	std::map<float, Biome::ID> TerrainGenerator::m_NoiseToBiomeMap = {
		{0.5f, Biome::ID::FOREST},
		{0.8f, Biome::ID::DESERT},
		{1.0f, Biome::ID::RIVER}
	};

	Biome::ID TerrainGenerator::m_GetBiomeID(int x, int y)
	{
		float noise = m_BiomeNoise.Get(x, y);

		for (const auto& [value, id] : m_NoiseToBiomeMap) {
			if (noise <= value) { return id; }
		}

		LogWarn("Couldn't get biome id");

		return Biome::ID::FOREST;
	}

	void TerrainGenerator::m_Init(unsigned int seed)
	{
		m_BiomeNoise = Vivium::Noise::Cellular(seed, 1.0f, BIOME_WAVELENGTH);
	}

	Biome::ID TerrainGenerator::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures)
	{
		Biome::ID id = m_GetBiomeID(x, y);

		const Biome::IBiome* ibiome_ptr = Biome::GetBiome(id);
		
		switch (id) {
		case Biome::ID::FOREST: BIOME_CAST_TO(Biome::ID::FOREST, ibiome_ptr)->GenerateAt(x, y, tile, world, structures); break;
		case Biome::ID::DESERT: BIOME_CAST_TO(Biome::ID::DESERT, ibiome_ptr)->GenerateAt(x, y, tile, world, structures); break;
		case Biome::ID::PLAIN:	BIOME_CAST_TO(Biome::ID::PLAIN, ibiome_ptr)->GenerateAt(x, y, tile, world, structures); break;
		case Biome::ID::RIVER:	BIOME_CAST_TO(Biome::ID::RIVER, ibiome_ptr)->GenerateAt(x, y, tile, world, structures); break;
		default:
			LogWarn("Got invalid biome id: {}", (uint8_t)id); break;
		}

		return id;
	}
}
