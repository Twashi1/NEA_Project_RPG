#pragma once

#include "Tile.h"
#include "Biome.h"
#include "NPC.h"

namespace Game {
	struct Region : Vivium::Streamable {
		static constexpr std::size_t LENGTH = 64;
		static constexpr float LENGTH_F = LENGTH;
		static constexpr std::size_t SIZE = LENGTH * LENGTH;
		static constexpr std::size_t TILES_MEM_SIZE = SIZE * sizeof(Tile);
		static constexpr std::size_t BIOMES_MEM_SIZE = SIZE * sizeof(Biome::ID);

		std::array<Tile, SIZE> tiles; // 1D array of tile ids
		std::array<Biome::ID, SIZE> biomes; // 1D array of the biome each tile is in

		// NPCs stored in the region they were spawned in, since their wander area is bounded by their home, this should be fine?
		std::vector<NPC> npcs;

		Region() = default;
		Region(const Region& other) = delete; // NOTE: could be available now
		Region(Region&& other) noexcept;
		~Region() = default;

		Tile& Index(int x, int y);
		Tile& Index(const Vivium::Vector2<int>& pos);
		Biome::ID& IndexBiome(int x, int y);
		Biome::ID& IndexBiome(const Vivium::Vector2<int>& pos);

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;
	};
}