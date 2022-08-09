#pragma once

#include "Tile.h"

struct Region {
	static constexpr unsigned int LENGTH = 64;
	static constexpr unsigned int SIZE = LENGTH * LENGTH;
	static constexpr unsigned int MEMORY_SIZE = SIZE * sizeof(Tile);

	Tile* tiles; // 1D array of tile ids

	Region();
	~Region();

	Tile& Index(int x, int y);
	Tile& Index(const Vivium::Vector2<int>& pos);
};