#pragma once

#include "Tile.h"

struct Region {
	static constexpr unsigned int LENGTH = 256;
	static constexpr unsigned int SIZE = LENGTH * LENGTH; // sizeof(Tile) is 3 bytes, thus each Region is about 200KB

	Tile* tiles;

	Region();
	~Region();

	Tile& Index(int x, int y);
};