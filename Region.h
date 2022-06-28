#pragma once
#include "Tile.h"
#include "Texture.h"
#include <memory>

struct Region {
	static const unsigned int LENGTH = 128;
	static const unsigned int AREA = LENGTH * LENGTH;
	Tile::ID tiles[AREA];

	Tile::ID GetTile(unsigned int x, unsigned int y);
	void SetTile(unsigned int x, unsigned int y, Tile::ID id);

	Texture ToTexture();
};