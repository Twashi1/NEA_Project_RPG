#include "Biome.h"
#include "World.h"

namespace Game {
	const std::map<float, Tile::ID> Biome::ForestBiome::m_HeightToTileMap = {
		{0.1f,		Tile::ID::WATER},	// below 0.1 is water
		{0.2f,		Tile::ID::SAND},	// 0.2 to 0.1 is sand
		{0.3f,		Tile::ID::GROUND},	// 0.3 to 0.2 is ground
		{1.0f,		Tile::ID::GRASS}	// 1.0 to 0.3 is grass
	};

	Vivium::Noise::Interpolated Biome::IBiome::m_HeightNoise;

	Vivium::Noise::White Biome::ForestBiome::m_TreeNoise;
	Vivium::Noise::White Biome::ForestBiome::m_VegitationNoise; // TODO: Switch to worley noise
	Vivium::Noise::White Biome::ForestBiome::m_OreNoise;

	const std::array<Biome::IBiome*, (uint8_t)Biome::ID::MAX> Biome::m_Biomes = {
		nullptr,									// VOID
		nullptr,									// PLAIN
		(Biome::IBiome*)new Biome::ForestBiome(),	// FOREST
		nullptr										// DESERT
	};

	// TODO: if they have the same key they will replace each other!
	const std::unordered_map<float, Structure::ID> Biome::ForestBiome::m_TreeWeights = {
		{100.0f,	Structure::ID::VOID},
		{10.0f,		Structure::ID::TREE}
	};

	const std::unordered_map<float, Tile::ID> Biome::ForestBiome::m_VegitationWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::BUSH},
		{1.0f,		Tile::ID::BUSH_FRUIT}
	};

	const std::unordered_map<float, Tile::ID> Biome::ForestBiome::m_OreWeights = {
		{100.0f,	Tile::ID::VOID},
		{1.01f,		Tile::ID::AMETHYST_NODE},
		{1.02f,		Tile::ID::EMERALD_NODE},
		{1.03f,		Tile::ID::RUBY_NODE},
		{1.04f,		Tile::ID::SAPPHIRE_NODE},
		{1.05f,		Tile::ID::TOPAZ_NODE},
	};


	Biome::ForestBiome::ForestBiome()
	{
		m_HeightNoise.wavelength = 6;
	}

	void Biome::ForestBiome::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		tile.bot = m_GetTileFromHeightMap(m_HeightToTileMap, m_HeightNoise.Get(x, y));

		// NOTE: When worley noise is implemented, this will make sense (i think)
		// TODO: in future add some tag to a tile to determine if it can have vegitation above it
		if (m_TreeNoise.Get(x, y) > 0.5f && tile.bot == Tile::ID::GRASS) {
			Structure::ID chosen_structure = m_GetRandomWeightedChoice(m_TreeWeights, Vivium::Random::GetFloat(0.0f, 1.0f));

			if (chosen_structure != Structure::ID::VOID) {
				structures[{x, y}] = chosen_structure;
			}
		}

		if (m_VegitationNoise.Get(x, y) > 0.5f && tile.bot == Tile::ID::GRASS) {
			tile.top = m_GetRandomWeightedChoice(m_VegitationWeights, Vivium::Random::GetFloat(0.0f, 1.0f));
		}

		if (m_OreNoise.Get(x, y) > 0.5f && tile.bot != Tile::ID::WATER) {
			tile.top = m_GetRandomWeightedChoice(m_OreWeights, Vivium::Random::GetFloat(0.0f, 1.0f));
		}
	}

	Tile::ID Biome::m_GetTileFromHeightMap(const std::map<float, Tile::ID>& height_map, float height)
	{
		for (auto& [max_height, tile_id] : height_map) {
			if (height <= max_height) { return tile_id; }
		}

		LogWarn("Bad height map? Height value {} was larger than maximum of table", height);

		return Tile::ID::VOID;
	}

	void Biome::Init()
	{
	}

#define DELETE_BIOME_PTR(id) if (m_Biomes[(uint8_t)id] != nullptr) { delete (BiomeType<Biome::ID::FOREST>::type*)m_Biomes[(uint8_t)id]; }

	void Biome::Terminate()
	{
		// DELETE_BIOME_PTR(Biome::ID::VOID);
		DELETE_BIOME_PTR(Biome::ID::PLAIN);
		DELETE_BIOME_PTR(Biome::ID::FOREST);
		DELETE_BIOME_PTR(Biome::ID::DESERT);
		// DELETE_BIOME_PTR(Biome::ID::MAX);
	}

	const Biome::IBiome* Biome::GetBiome(const ID& id)
	{
		return m_Biomes[(uint8_t)id];
	}
}
