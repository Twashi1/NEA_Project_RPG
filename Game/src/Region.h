#pragma once

#include "Tile.h"

namespace Game {
	struct Region {
		static constexpr unsigned int LENGTH = 64;
		static constexpr unsigned int SIZE = LENGTH * LENGTH;
		static constexpr unsigned int MEMORY_SIZE = SIZE * sizeof(Tile);

		std::shared_ptr<Tile[]> tiles; // 1D array of tile ids

		Region();
		Region(const Region& other);
		~Region();

		Tile& Index(int x, int y);
		Tile& Index(const Vivium::Vector2<int>& pos);
		const Tile& CIndex(int x, int y);
		const Tile& CIndex(const Vivium::Vector2<int>& pos);
	};
}