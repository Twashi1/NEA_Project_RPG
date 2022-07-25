#pragma once

#include "Tile.h"

struct Region {
	static constexpr unsigned int LENGTH = 64;
	static constexpr unsigned int SIZE = LENGTH * LENGTH;

	Tile* tiles; // 1D array of tile ids

	Region();
	~Region();

	Tile& Index(int x, int y);
	Tile& Index(const CoolEngineName::Vector2<int>& pos);
};