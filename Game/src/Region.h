#pragma once

#include "Tile.h"
#include "Biome.h"

namespace Game {
	struct Region {
		static constexpr std::size_t LENGTH = 64;
		static constexpr std::size_t SIZE = LENGTH * LENGTH;
		static constexpr std::size_t TILES_MEM_SIZE = SIZE * sizeof(Tile);
		static constexpr std::size_t BIOMES_MEM_SIZE = SIZE * sizeof(Biome::ID);

		std::array<Tile, SIZE> tiles; // 1D array of tile ids
		std::array<Biome::ID, SIZE> biomes; // 1D array of the biome each tile is in

		Region();
		Region(const Region& other) = delete;
		Region(Region&& other) noexcept;
		~Region();

		Tile& Index(int x, int y);
		Tile& Index(const Vivium::Vector2<int>& pos);
		Biome::ID& IndexBiome(int x, int y);
		Biome::ID& IndexBiome(const Vivium::Vector2<int>& pos);
	};
}