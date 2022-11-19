#pragma once

#include "Core.h"
#include "Tile.h"
#include "Structures.h"

namespace Game {
	class World;
	struct Region;

	// TODO: rename river biome to lake?
	// TODO: better blending between biomes
	/// <summary>
	/// Static class to store Generator instances
	/// </summary>
	class Biome {
	public:
		enum class ID : uint8_t {
			VOID,	// Invalid
			PLAIN,  // Biome consisting of sparsely populated trees with green vegitation and a grass base, and normal distribution of ores
			FOREST, // Biome with lots of trees, green vegitation, and a grass base, and normal distribution of ores
			DESERT, // Biome with cacti, a sand base, and densley populated with ores
			RIVER,  // Biome mostly of water (TODO: this generated more as lakes than a biome divider, TODO: spawn slimes here)
			MAX		// For iteration
		};
	
	private:
		/// <summary>
		/// Get pretty name given a biome id
		/// </summary>
		/// <param name="id">ID of biome to get pretty name for</param>
		/// <returns></returns>
		static const char* m_GetBiomeName(const Biome::ID& id);

	public:
		/// <summary>
		/// Abstract class for biome generators
		/// </summary>
		class Generator {
		protected:
			// Interpolated 2D noise - used to determine the tile for background (e.g. higher tiles grass, then lower tiles dirt)
			static Vivium::Noise::Interpolated m_HeightNoise;
			unsigned int m_Seed; // TODO: don't need to store seed?

		public:
			Generator(unsigned int seed);

			// For a given x, y coordinate, generate a tile, and possible generate a structure
			virtual void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const = 0;
			// Check with noise value whether to spawn an NPC at a specific tile
			virtual void SpawnNPCs(int x, int y, Region& region, World* world) const = 0;
		};

		/// <summary>
		/// Biome generator for forests: biomes with grass and high density of trees
		/// </summary>
		class Forest : Generator {
		private:
			using Generator::m_HeightNoise;

			static Vivium::Noise::White m_TreeNoise;
			static Vivium::Noise::White m_VegitationNoise;	// TODO: Switch to worley noise
			static Vivium::Noise::White m_OreNoise;			// TODO: Switch to worley noise
			static Vivium::Noise::White m_DebrisNoise;
			static Vivium::Noise::Cellular m_AnimalNoise;

			// Corresponds a height value to a tile
			// TODO: some way to inherit this
			static const std::map<float, Tile::ID> m_HeightToTileMap;

			static const std::unordered_map<float, Structure::ID> m_TreeWeights;
			// TODO: Tile or Tile::ID?
			static const std::unordered_map<float, Tile::ID> m_VegitationWeights;
			static const std::unordered_map<float, Tile::ID> m_OreWeights;
			static const std::unordered_map<float, Tile::ID> m_DebrisWeights;
			
		public:
			// TODO: handled by world rn, but maybe biomes should deal with this
			static constexpr float LEAF_PARTICLE_FREQUENCY = 0.3f; // Every second, release a particle

			Forest(unsigned int seed);
			
			void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const override;
			void SpawnNPCs(int x, int y, Region& region, World* world) const override;
		};

		/// <summary>
		/// Biome with sand, cacti, and high density of ores
		/// </summary>
		class Desert : Generator {
		private:
			using Generator::m_HeightNoise;

			static Vivium::Noise::White m_CactusNoise;
			static Vivium::Noise::White m_VegitationNoise;
			static Vivium::Noise::White m_OreNoise;
			static Vivium::Noise::White m_DebrisNoise;
			static Vivium::Noise::Cellular m_AnimalNoise;

			static const std::map<float, Tile::ID> m_HeightToTileMap;

			static const std::unordered_map<float, Structure::ID> m_CactusWeights;
			static const std::unordered_map<float, Tile::ID> m_VegitationWeights;
			static const std::unordered_map<float, Tile::ID> m_OreWeights;
			static const std::unordered_map<float, Tile::ID> m_DebrisWeights;

		public:
			Desert(unsigned int seed);

			void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const override;
			void SpawnNPCs(int x, int y, Region& region, World* world) const override;
		};

		/// <summary>
		/// Biome of mostly water
		/// </summary>
		class Lake : Generator {
			using Generator::m_HeightNoise;

			static const std::map<float, Tile::ID> m_HeightToTileMap;

		public:
			Lake(unsigned int seed);
			
			void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const override;
			void SpawnNPCs(int x, int y, Region& region, World* world) const override;
		};

	private:
		/// <summary>
		/// Get tile from the biome gen's height map given a height value
		/// </summary>
		/// <param name="height_map"></param>
		/// <param name="height"></param>
		/// <returns></returns>
		static Tile::ID m_GetTileFromHeightMap(const std::map<float, Tile::ID>& height_map, float height);

		template <typename T>
		static T m_GetRandomWeightedChoice(const std::unordered_map<float, T>& weight_map, float value)
		{
			float sum = 0.0f;

			for (const auto& it : weight_map) {
				sum += it.first;
			}

			float random = value * sum;

			for (const auto& [weight, object] : weight_map) {
				if (random < weight) {
					return object;
				}
				
				random -= weight;
			}

			// Weird logic error occured
			LogWarn("Couldn't get random weighted choice");

			return T();
		}

		/// <summary>
		/// Stores pointers to biome generates (allocated on heap)
		/// Indexed by casting ID enum to integral type
		/// </summary>
		static std::array<Generator*, (uint8_t)ID::MAX> m_Biomes;

	public:
		/// <summary>
		/// Construct biome generators
		/// </summary>
		/// <param name="seed"></param>
		static void Init(unsigned int seed);
		/// <summary>
		/// Deallocate biome generators
		/// </summary>
		static void Terminate();

		/// <summary>
		/// Get biome generator by id
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static const Generator* GetBiome(const ID& id);
	
		/// <summary>
		/// Helper to get type of biome by id (although its run-time only)
		/// </summary>
		/// <typeparam name="id"></typeparam>
		template <ID id>
		struct BiomeType { using type = Generator; };

		template<> struct BiomeType<ID::PLAIN>	{ using type = Forest; };
		template<> struct BiomeType<ID::FOREST> { using type = Forest; };
		template<> struct BiomeType<ID::DESERT> { using type = Desert; };
		template<> struct BiomeType<ID::RIVER>	{ using type = Lake;	};
		
		friend World;
	};
}

#define BIOME_CAST_TO(id, ptr) ((Biome::BiomeType<id>::type*)ptr)
