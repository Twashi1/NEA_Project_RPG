#pragma once

#include "Core.h"
#include "Tile.h"
#include "Structures.h"

namespace Game {
	class World;

	class Biome {
	public:
		enum class ID : uint8_t {
			VOID,
			PLAIN,
			FOREST,
			DESERT,
			RIVER,
			MAX
		};
	
	private:
		static const char* m_GetBiomeName(const Biome::ID& id);

	public:
		class IBiome {
		protected:
			static Vivium::Noise::Interpolated m_HeightNoise;
			unsigned int m_Seed;

		public:
			IBiome(unsigned int sed);

			virtual void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const = 0;
		};

		class ForestBiome : IBiome {
		private:
			using IBiome::m_HeightNoise;

			static Vivium::Noise::White m_TreeNoise;
			static Vivium::Noise::White m_VegitationNoise; // TODO: Switch to worley noise
			static Vivium::Noise::White m_OreNoise;		// TODO: Switch to worley noise
			static Vivium::Noise::White m_DebrisNoise;

			static const std::map<float, Tile::ID> m_HeightToTileMap;

			static const std::unordered_map<float, Structure::ID> m_TreeWeights;
			// TODO: Tile or Tile::ID?
			static const std::unordered_map<float, Tile::ID> m_VegitationWeights;
			static const std::unordered_map<float, Tile::ID> m_OreWeights;
			static const std::unordered_map<float, Tile::ID> m_DebrisWeights;
			
		public:
			// TODO: handled by world rn, but maybe biomes should deal with this
			static constexpr float LEAF_PARTICLE_FREQUENCY = 0.3f; // Every second, release a particle

			ForestBiome(unsigned int seed);
			
			void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const override;
		};

		class DesertBiome : IBiome {
		private:
			using IBiome::m_HeightNoise;

			static Vivium::Noise::White m_CactusNoise;
			static Vivium::Noise::White m_VegitationNoise;
			static Vivium::Noise::White m_OreNoise;
			static Vivium::Noise::White m_DebrisNoise;

			static const std::map<float, Tile::ID> m_HeightToTileMap;

			static const std::unordered_map<float, Structure::ID> m_CactusWeights;
			static const std::unordered_map<float, Tile::ID> m_VegitationWeights;
			static const std::unordered_map<float, Tile::ID> m_OreWeights;
			static const std::unordered_map<float, Tile::ID> m_DebrisWeights;

		public:
			DesertBiome(unsigned int seed);

			void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const override;
		};

		class RiverBiome : IBiome {
			using IBiome::m_HeightNoise;

			static const std::map<float, Tile::ID> m_HeightToTileMap;

		public:
			RiverBiome(unsigned int seed);
			
			void GenerateAt(int x, int y, Tile& tile, World* world, std::unordered_map<Vivium::Vector2<int>, Structure::ID>& structures) const override;
		};

	private:
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

		static std::array<IBiome*, (uint8_t)ID::MAX> m_Biomes;

	public:
		static void Init(unsigned int seed);
		static void Terminate();

		static const IBiome* GetBiome(const ID& id);

		template <ID id>
		struct BiomeType { using type = IBiome; };

		template<> struct BiomeType<ID::PLAIN>	{ using type = ForestBiome; };
		template<> struct BiomeType<ID::FOREST> { using type = ForestBiome; };
		template<> struct BiomeType<ID::DESERT> { using type = DesertBiome; };
		template<> struct BiomeType<ID::RIVER>	{ using type = RiverBiome;	};
		
		friend World;
	};
}

#define BIOME_CAST_TO(id, ptr) ((Biome::BiomeType<id>::type*)ptr)
