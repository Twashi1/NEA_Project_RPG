#include "Region.h"

Region::Region()
{
    // Allocate memory for tiles array
    tiles = new Tile[SIZE];
}

Region::~Region()
{
    // Free tiles memory
    delete[] tiles;
}

inline Tile& Region::Index(int x, int y)
{
    // Check x, y within bounds
    if (x >= Region::LENGTH || y >= Region::LENGTH) Log(std::format("Invalid index [{}, {}]", to_string(x), to_string(y)), Utils::ERROR::INFO);
    // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
    return tiles[x + (y * Region::LENGTH)];
}
