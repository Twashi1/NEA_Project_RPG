#include "Region.h"

Region::Region()
{
    // Allocate memory for tiles array
    tiles = new Tile[Region::SIZE];
}

Region::~Region() {}

Tile& Region::Index(int x, int y)
{
    // Check x, y within bounds
    if (x >= Region::LENGTH || y >= Region::LENGTH) Log(std::format("Invalid index [{}, {}]", x, y), LOG::INFO);
    // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
    return tiles[x + (y * Region::LENGTH)];
}

Tile& Region::Index(const Vector2<int>& pos)
{
    // Check x, y within bounds
    if (pos.x >= Region::LENGTH || pos.y >= Region::LENGTH) Log(std::format("Invalid index {}", to_string(pos)), LOG::INFO);
    // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
    return tiles[pos.x + (pos.y * Region::LENGTH)];
}
