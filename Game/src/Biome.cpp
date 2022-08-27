#include "Biome.h"
#include "World.h"

namespace Game {
	std::array<Biome::IBiome*, (uint8_t)Biome::ID::MAX> Biome::m_Biomes = {
		nullptr,	// VOID
		nullptr,	// PLAIN
		nullptr,	// FOREST
		nullptr,	// DESERT
		nullptr		// RIVER
	};

	const std::map<float, Tile::ID> Biome::ForestBiome::m_HeightToTileMap = {
		{0.1f,		Tile::ID::WATER},	// below 0.1 is water
		{0.2f,		Tile::ID::SAND},	// 0.2 to 0.1 is sand
		{0.3f,		Tile::ID::GROUND},	// 0.3 to 0.2 is ground
		{1.0f,		Tile::ID::GRASS}	// 1.0 to 0.3 is grass
	};

	const std::map<float, Tile::ID> Biome::DesertBiome::m_HeightToTileMap = {
		{1.0f,		Tile::ID::SAND}
	};

	const std::map<float, Tile::ID> Biome::RiverBiome::m_HeightToTileMap = {
		{0.7f,		Tile::ID::WATER},
		{0.8f,		Tile::ID::SAND},
		{0.9f,		Tile::ID::GROUND},
		{1.0f,		Tile::ID::GRASS}
	};

	Vivium::Noise::Interpolated Biome::IBiome::m_HeightNoise;

	Vivium::Noise::White Biome::ForestBiome::m_TreeNoise;
	Vivium::Noise::White Biome::ForestBiome::m_VegitationNoise; // TODO: Switch to worley noise
	Vivium::Noise::White Biome::ForestBiome::m_OreNoise;
	Vivium::Noise::White Biome::ForestBiome::m_DebrisNoise;

	Vivium::Noise::White Biome::DesertBiome::m_CactusNoise;
	Vivium::Noise::White Biome::DesertBiome::m_VegitationNoise; // TODO: Switch to worley noise
	Vivium::Noise::White Biome::DesertBiome::m_OreNoise;
	Vivium::Noise::White Biome::DesertBiome::m_DebrisNoise;

	// TODO: if they have the same key they will replace each other!
	const std::unordered_map<float, Structure::ID> Biome::ForestBiome::m_TreeWeights = {
		{100.0f,	Structure::ID::VOID},
		{10.0f,		Structure::ID::TREE}
	};

	const std::unordered_map<float, Tile::ID> Biome::ForestBiome::m_VegitationWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::BUSH},
		{3.0f,		Tile::ID::BUSH_FRUIT}
	};

	const std::unordered_map<float, Tile::ID> Biome::ForestBiome::m_OreWeights = {
		{100.0f,	Tile::ID::VOID},
		{1.01f,		Tile::ID::AMETHYST_NODE},
		{1.02f,		Tile::ID::EMERALD_NODE},
		{1.03f,		Tile::ID::RUBY_NODE},
		{1.04f,		Tile::ID::SAPPHIRE_NODE},
		{1.05f,		Tile::ID::TOPAZ_NODE},
	};

	const std::unordered_map<float, Tile::ID> Biome::ForestBiome::m_DebrisWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::MOSSY_DEBRIS},
		{3.0f,		Tile::ID::ROCKY_DEBRIS}
	};

	// TODO: if they have the same key they will replace each other!
	const std::unordered_map<float, Structure::ID> Biome::DesertBiome::m_CactusWeights = {
		{100.0f,	Structure::ID::VOID},
		{10.0f,		Structure::ID::CACTUS_TALL}
	};

	const std::unordered_map<float, Tile::ID> Biome::DesertBiome::m_VegitationWeights = {
		{10.0f,		Tile::ID::VOID},
		{1.0f,		Tile::ID::CACTUS_FRUIT},
		{2.0f,		Tile::ID::CACTUS_SMALL}
	};

	const std::unordered_map<float, Tile::ID> Biome::DesertBiome::m_OreWeights = {
		{100.0f,	Tile::ID::VOID},
		{3.01f,		Tile::ID::AMETHYST_NODE},
		{3.02f,		Tile::ID::EMERALD_NODE},
		{3.03f,		Tile::ID::RUBY_NODE},
		{3.04f,		Tile::ID::SAPPHIRE_NODE},
		{3.05f,		Tile::ID::TOPAZ_NODE},
	};

	const std::unordered_map<float, Tile::ID> Biome::DesertBiome::m_DebrisWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::ROCKY_DEBRIS}
	};


	Biome::ForestBiome::ForestBiome(unsigned int seed)
		: IBiome::IBiome(seed)
	{
		m_HeightNoise.m_Seed = seed;
		m_TreeNoise.m_Seed = seed;
		m_VegitationNoise.m_Seed = seed;
		m_OreNoise.m_Seed = seed;
		m_HeightNoise.wavelength = 6;
	}

	void Biome::ForestBiome::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		float noise = m_HeightNoise.Get(x, y);
		tile.bot = m_GetTileFromHeightMap(m_HeightToTileMap, noise);

		// NOTE: When worley noise is implemented, this will make sense (i think)
		// TODO: in future add some tag to a tile to determine if it can have vegitation above it
		if (m_TreeNoise.Get(x, y) > 0.5f && tile.bot == Tile::ID::GRASS) {
			Structure::ID chosen_structure = m_GetRandomWeightedChoice(m_TreeWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (chosen_structure != Structure::ID::VOID) {
				structures[{x, y}] = chosen_structure;
			}
		}

		if (m_VegitationNoise.Get(x, y) > 0.5f && tile.bot == Tile::ID::GRASS) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_VegitationWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.top = selected_tile;
		}

		if (m_OreNoise.Get(x, y) > 0.5f && tile.bot != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_OreWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.top = selected_tile;
		}

		if (m_DebrisNoise.Get(x, y) > 0.5f && tile.bot != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_DebrisWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (!(selected_tile == Tile::ID::MOSSY_DEBRIS && tile.bot != Tile::ID::GRASS))
			{
				if (selected_tile != Tile::ID::VOID) tile.top = selected_tile;
			}
		}
	}

	Tile::ID Biome::m_GetTileFromHeightMap(const std::map<float, Tile::ID>& height_map, float height)
	{
		for (auto& [max_height, tile_id] : height_map) {
			if (height <= max_height) { return tile_id; }
		}

		LogWarn("Bad biome height map? Height value {} was larger than maximum of table", height);

		return Tile::ID::VOID;
	}

	void Biome::Init(unsigned int seed)
	{
		m_Biomes = {
			nullptr,										// VOID
			(Biome::IBiome*)new Biome::ForestBiome(seed),	// PLAIN
			(Biome::IBiome*)new Biome::ForestBiome(seed),	// FOREST
			(Biome::IBiome*)new Biome::DesertBiome(seed),	// DESERT
			(Biome::IBiome*)new Biome::RiverBiome(seed)		// RIVER
		};
	}

#define DELETE_BIOME_PTR(id) if (m_Biomes[(uint8_t)id] != nullptr) { delete (BiomeType<Biome::ID::FOREST>::type*)m_Biomes[(uint8_t)id]; }

	void Biome::Terminate()
	{
		// DELETE_BIOME_PTR(Biome::ID::VOID);
		DELETE_BIOME_PTR(Biome::ID::PLAIN);
		DELETE_BIOME_PTR(Biome::ID::FOREST);
		DELETE_BIOME_PTR(Biome::ID::DESERT);
		DELETE_BIOME_PTR(Biome::ID::RIVER);
		// DELETE_BIOME_PTR(Biome::ID::MAX);
	}

	const Biome::IBiome* Biome::GetBiome(const ID& id)
	{
		return m_Biomes[(uint8_t)id];
	}

	Biome::DesertBiome::DesertBiome(unsigned int seed)
		: IBiome::IBiome(seed)
	{
		m_HeightNoise.m_Seed = seed;
		m_CactusNoise.m_Seed = seed;
		m_VegitationNoise.m_Seed = seed;
		m_OreNoise.m_Seed = seed;
		m_HeightNoise.wavelength = 4;
	}

	void Biome::DesertBiome::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		tile.bot = m_GetTileFromHeightMap(m_HeightToTileMap, m_HeightNoise.Get(x, y));

		// NOTE: When worley noise is implemented, this will make sense (i think)
		// TODO: in future add some tag to a tile to determine if it can have vegitation above it
		if (m_CactusNoise.Get(x, y) > 0.5f && tile.bot == Tile::ID::SAND) {
			Structure::ID chosen_structure = m_GetRandomWeightedChoice(m_CactusWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (chosen_structure != Structure::ID::VOID) {
				structures[{x, y}] = chosen_structure;
			}
		}

		if (m_VegitationNoise.Get(x, y) > 0.5f && tile.bot == Tile::ID::SAND) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_VegitationWeights, Vivium::Noise::Hashf(m_Seed, x, y));
			
			if (selected_tile != Tile::ID::VOID) tile.top = selected_tile;
		}

		if (m_OreNoise.Get(x, y) > 0.5f && tile.bot != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_OreWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.top = selected_tile;
		}

		if (m_DebrisNoise.Get(x, y) > 0.5f && tile.bot != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_DebrisWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.top = selected_tile;
		}
	}

	Biome::RiverBiome::RiverBiome(unsigned int seed)
		: IBiome::IBiome(seed)
	{
		m_HeightNoise.m_Seed = seed;
		m_HeightNoise.wavelength = 4;
	}

	void Biome::RiverBiome::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		tile.bot = m_GetTileFromHeightMap(m_HeightToTileMap, m_HeightNoise.Get(x, y));
	}

	Biome::IBiome::IBiome(unsigned int seed)
		: m_Seed(seed)
	{}
}
