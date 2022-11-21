#include "Biome.h"
#include "World.h"

namespace Game {
	// Where the biome generators are stored, created by Biome::Init
	std::array<Biome::Generator*, (uint8_t)Biome::ID::MAX> Biome::m_Biomes = {
		nullptr,	// VOID
		nullptr,	// PLAIN
		nullptr,	// FOREST
		nullptr,	// DESERT
		nullptr		// RIVER
	};

	const std::map<float, Tile::ID> Biome::Forest::m_HeightToTileMap = {
		{0.1f,		Tile::ID::WATER},	// below 0.1 is water
		{0.2f,		Tile::ID::SAND},	// 0.2 to 0.1 is sand
		{0.3f,		Tile::ID::GROUND},	// 0.3 to 0.2 is ground
		{1.0f,		Tile::ID::GRASS}	// 1.0 to 0.3 is grass
	};

	const std::map<float, Tile::ID> Biome::Desert::m_HeightToTileMap = {
		{1.0f,		Tile::ID::SAND} // All sand
	};

	const std::map<float, Tile::ID> Biome::Lake::m_HeightToTileMap = {
		{0.7f,		Tile::ID::WATER},   // below 0.7 is water
		{0.8f,		Tile::ID::SAND},    // below 0.8 is sand
		{0.9f,		Tile::ID::GROUND},  // below 0.9 is ground
		{1.0f,		Tile::ID::GRASS}    // below 1.0 is grass
	};

	Vivium::Noise::Interpolated Biome::Generator::m_HeightNoise;

	Vivium::Noise::White Biome::Forest::m_TreeNoise;
	Vivium::Noise::White Biome::Forest::m_VegitationNoise; // TODO: Switch to worley noise
	Vivium::Noise::White Biome::Forest::m_OreNoise;
	Vivium::Noise::White Biome::Forest::m_DebrisNoise;
	Vivium::Noise::Cellular Biome::Forest::m_AnimalNoise;

	Vivium::Noise::White Biome::Desert::m_CactusNoise;
	Vivium::Noise::White Biome::Desert::m_VegitationNoise; // TODO: Switch to worley noise
	Vivium::Noise::White Biome::Desert::m_OreNoise;
	Vivium::Noise::White Biome::Desert::m_DebrisNoise;
	Vivium::Noise::Cellular Biome::Desert::m_AnimalNoise;

	// TODO: if they have the same key they will replace each other!
	const std::unordered_map<float, Structure::ID> Biome::Forest::m_TreeWeights = {
		{100.0f,	Structure::ID::VOID},
		{10.0f,		Structure::ID::TREE}
	};

	const std::unordered_map<float, Tile::ID> Biome::Forest::m_VegitationWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::BUSH},
		{3.0f,		Tile::ID::BUSH_FRUIT}
	};

	const std::unordered_map<float, Tile::ID> Biome::Forest::m_OreWeights = {
		{100.0f,	Tile::ID::VOID},
		{1.01f,		Tile::ID::AMETHYST_NODE},
		{1.02f,		Tile::ID::EMERALD_NODE},
		{1.03f,		Tile::ID::RUBY_NODE},
		{1.04f,		Tile::ID::SAPPHIRE_NODE},
		{1.05f,		Tile::ID::TOPAZ_NODE},
	};

	const std::unordered_map<float, Tile::ID> Biome::Forest::m_DebrisWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::MOSSY_DEBRIS},
		{3.0f,		Tile::ID::ROCKY_DEBRIS}
	};

	// TODO: if they have the same key they will replace each other!
	const std::unordered_map<float, Structure::ID> Biome::Desert::m_CactusWeights = {
		{100.0f,	Structure::ID::VOID},
		{10.0f,		Structure::ID::CACTUS_TALL}
	};

	const std::unordered_map<float, Tile::ID> Biome::Desert::m_VegitationWeights = {
		{10.0f,		Tile::ID::VOID},
		{1.0f,		Tile::ID::CACTUS_FRUIT},
		{2.0f,		Tile::ID::CACTUS_SMALL}
	};

	const std::unordered_map<float, Tile::ID> Biome::Desert::m_OreWeights = {
		{100.0f,	Tile::ID::VOID},
		{3.01f,		Tile::ID::AMETHYST_NODE},
		{3.02f,		Tile::ID::EMERALD_NODE},
		{3.03f,		Tile::ID::RUBY_NODE},
		{3.04f,		Tile::ID::SAPPHIRE_NODE},
		{3.05f,		Tile::ID::TOPAZ_NODE},
	};

	const std::unordered_map<float, Tile::ID> Biome::Desert::m_DebrisWeights = {
		{100.0f,	Tile::ID::VOID},
		{5.0f,		Tile::ID::ROCKY_DEBRIS}
	};


	Biome::Forest::Forest(unsigned int seed)
		: Generator::Generator(seed)
	{
		// Init seeds for various generators
		m_HeightNoise.m_Seed = seed;
		m_TreeNoise.m_Seed = seed;
		m_VegitationNoise.m_Seed = seed;
		m_OreNoise.m_Seed = seed;
		m_AnimalNoise.m_Seed = seed;

		// Set wavelengths
		m_AnimalNoise.wavelength = 6;
		m_HeightNoise.wavelength = 6;
	}

	void Biome::Forest::SpawnNPCs(int x, int y, Region& region, World* world) const
	{
		// Get noise value for animals
		float noise = m_AnimalNoise.Get(x, y);

		// If value above a threshold, spawn an animal
		if (noise > 0.9f) {
			// TODO: disgusting constructor
#define GENERATE_NPC
#ifdef GENERATE_NPC
			region.npcs.emplace_back(
				NPC::ID::PIG,
				// TEMP values
				std::make_shared<Vivium::Body>(
					std::make_shared<Vivium::Quad>(Vivium::Vector2<float>(x, y) * World::PIXEL_SCALE, Vivium::Vector2<float>(World::PIXEL_SCALE)),
					true,
					1.0f,
					1.0f
					),
				NPC::BehaviourDataMap{
					{Behaviours::Behaviour::ID::WANDER,	std::make_shared<Behaviours::Wandering::Client>(Vivium::Vector2<int>(x, y))},
					{Behaviours::Behaviour::ID::IDLE,		std::make_shared<Behaviours::Idle::Client>()},
				}
			);
#endif
		}
	}

	void Biome::Forest::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		// Get height noise
		float noise = m_HeightNoise.Get(x, y);
		// Select background tile based on height noise and height map
		tile.background = m_GetTileFromHeightMap(m_HeightToTileMap, noise);

		// NOTE: When worley noise is implemented, this will make sense (i think)
		// TODO: in future add some tag to a tile to determine if it can have vegitation above it
		if (m_TreeNoise.Get(x, y) > 0.5f && tile.background == Tile::ID::GRASS) {
			// Get a random structure (either tree or none)
			Structure::ID chosen_structure = m_GetRandomWeightedChoice(m_TreeWeights, Vivium::Noise::Hashf(m_Seed, x, y));
			
			// Generate that structure
			if (chosen_structure != Structure::ID::VOID) {
				structures[{x, y}] = chosen_structure;
			}
		}

		// If vegitation noise sufficient and the background tile is grass
		if (m_VegitationNoise.Get(x, y) > 0.5f && tile.background == Tile::ID::GRASS) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_VegitationWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.foreground = selected_tile;
		}

		// If ore noise sufficient and the background tile isn't water
		if (m_OreNoise.Get(x, y) > 0.5f && tile.background != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_OreWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.foreground = selected_tile;
		}

		// If debris noise sufficient and the background tile isn't water
		if (m_DebrisNoise.Get(x, y) > 0.5f && tile.background != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_DebrisWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (!(selected_tile == Tile::ID::MOSSY_DEBRIS && tile.background != Tile::ID::GRASS))
			{
				if (selected_tile != Tile::ID::VOID) tile.foreground = selected_tile;
			}
		}
	}

	const char* Biome::m_GetBiomeName(const Biome::ID& id)
	{
		switch (id) {
		case ID::VOID:		return "Void";
		case ID::PLAIN:		return "Plains";
		case ID::FOREST:	return "Forest";
		case ID::DESERT:	return "Desert";
		case ID::RIVER:		return "River";
		default:			return "InvalidBiome";
		}
	}

	Tile::ID Biome::m_GetTileFromHeightMap(const std::map<float, Tile::ID>& height_map, float height)
	{
		// Iterate height map
		for (auto& [max_height, tile_id] : height_map) {
			// If the height is less than the height in the tile map select that tile id
			if (height <= max_height) { return tile_id; }
		}

		LogWarn("Bad biome height map? Height value {} was larger than maximum of table", height);

		return Tile::ID::VOID;
	}

	void Biome::Init(unsigned int seed)
	{
		m_Biomes = {
			nullptr,						// VOID
			(Generator*)new Forest(seed),	// PLAIN (TODO: plains biome)
			(Generator*)new Forest(seed),	// FOREST
			(Generator*)new Desert(seed),	// DESERT
			(Generator*)new Lake(seed)		// RIVER
		};
	}

	void Biome::Terminate()
	{
		// Iterate biome generators
		for (int i = 1; i < (int)Biome::ID::MAX; i++) {
			// Get the biome generator
			Generator* biome_gen = m_Biomes[i];

			// If the biome generator has been initialised
			if (biome_gen != nullptr) {
				delete biome_gen;
			}
		}
	}

	const Biome::Generator* Biome::GetBiome(const ID& id)
	{
		return m_Biomes[(uint8_t)id];
	}

	Biome::Desert::Desert(unsigned int seed)
		: Generator::Generator(seed)
	{
		m_HeightNoise.m_Seed = seed;
		m_CactusNoise.m_Seed = seed;
		m_VegitationNoise.m_Seed = seed;
		m_OreNoise.m_Seed = seed;
		m_AnimalNoise.m_Seed = seed;
		m_AnimalNoise.wavelength = 6; // TODO: decide wavelength
		m_HeightNoise.wavelength = 4;
	}

	void Biome::Desert::SpawnNPCs(int x, int y, Region& region, World* world) const
	{
		// Spawning animals
		float noise = m_AnimalNoise.Get(x, y);

		if (noise > 0.9f) {
#ifdef GENERATE_NPC
			// What a disgustingly long constructor
			// TODO: factory?
			region.npcs.emplace_back(
				NPC::ID::COW,
				// TEMP values
				std::make_shared<Vivium::Body>(
					std::make_shared<Vivium::Quad>(Vivium::Vector2<float>(x, y) * World::PIXEL_SCALE, Vivium::Vector2<float>(World::PIXEL_SCALE)),
					true,
					1.0f,
					1.0f
				),
				NPC::BehaviourDataMap {
					{Behaviours::Behaviour::ID::WANDER,	std::make_shared<Behaviours::Wandering::Client>(Vivium::Vector2<int>(x, y))},
					{Behaviours::Behaviour::ID::IDLE,		std::make_shared<Behaviours::Idle::Client>()},
				}
			);
#endif
		}
	}

	void Biome::Desert::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		tile.background = m_GetTileFromHeightMap(m_HeightToTileMap, m_HeightNoise.Get(x, y));

		// NOTE: When worley noise is implemented, this will make sense (i think)
		// TODO: in future add some tag to a tile to determine if it can have vegitation above it
		if (m_CactusNoise.Get(x, y) > 0.5f && tile.background == Tile::ID::SAND) {
			Structure::ID chosen_structure = m_GetRandomWeightedChoice(m_CactusWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (chosen_structure != Structure::ID::VOID) {
				structures[{x, y}] = chosen_structure;
			}
		}

		if (m_VegitationNoise.Get(x, y) > 0.5f && tile.background == Tile::ID::SAND) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_VegitationWeights, Vivium::Noise::Hashf(m_Seed, x, y));
			
			if (selected_tile != Tile::ID::VOID) tile.foreground = selected_tile;
		}

		if (m_OreNoise.Get(x, y) > 0.5f && tile.background != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_OreWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.foreground = selected_tile;
		}

		if (m_DebrisNoise.Get(x, y) > 0.5f && tile.background != Tile::ID::WATER) {
			Tile::ID selected_tile = m_GetRandomWeightedChoice(m_DebrisWeights, Vivium::Noise::Hashf(m_Seed, x, y));

			if (selected_tile != Tile::ID::VOID) tile.foreground = selected_tile;
		}
	}

	Vivium::Noise::Cellular Biome::Lake::m_AnimalNoise;

	Biome::Lake::Lake(unsigned int seed)
		: Generator::Generator(seed)
	{
		m_HeightNoise.m_Seed = seed;
		m_HeightNoise.wavelength = 4;
		m_AnimalNoise.m_Seed = seed;
		m_AnimalNoise.wavelength = 3;
	}

	void Biome::Lake::SpawnNPCs(int x, int y, Region& region, World* world) const
	{
		// Not checking if an obstacle since slimes spawn on water
		// if (world->GetIsObstacle({ x, y })) return;

		float noise = m_AnimalNoise.Get(x, y);

		if (noise > 0.99f) {
#ifdef GENERATE_NPC
			region.npcs.emplace_back(
				NPC::ID::SLIME,
				// TEMP values
				std::make_shared<Vivium::Body>(
					std::make_shared<Vivium::Quad>(Vivium::Vector2<float>(x, y) * World::PIXEL_SCALE, Vivium::Vector2<float>(World::PIXEL_SCALE)),
					true,
					1.0f,
					1.0f
					),
				NPC::BehaviourDataMap{
					{Behaviours::Behaviour::ID::SLIME_ATTACK,	std::make_shared<Behaviours::SlimeAttack::Client>()},
					{Behaviours::Behaviour::ID::HUNTING,		std::make_shared<Behaviours::Hunting::Client>(Vivium::Vector2<int>(x, y))},
					{Behaviours::Behaviour::ID::IDLE,			std::make_shared<Behaviours::Idle::Client>()},
				}
			);
#endif
		}
	}

	void Biome::Lake::GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const
	{
		tile.background = m_GetTileFromHeightMap(m_HeightToTileMap, m_HeightNoise.Get(x, y));
	}

	Biome::Generator::Generator(unsigned int seed)
		: m_Seed(seed)
	{}
}
