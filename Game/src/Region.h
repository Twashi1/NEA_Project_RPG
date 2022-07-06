#pragma once

#include "Tile.h"

struct Region {
	static constexpr unsigned int LENGTH = 64;
	static constexpr unsigned int SIZE = LENGTH * LENGTH;

	Tile::ID* tiles; // 1D array of tile ids

	Region();
	~Region();

	Tile::ID& Index(int x, int y);
};