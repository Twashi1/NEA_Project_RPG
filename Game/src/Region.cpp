#include "Region.h"

namespace Game {
    Region::Region()
    {
        // Allocate memory for tiles array
        tiles = new Tile[Region::SIZE];
    }

    Region::~Region() {}

    Tile& Region::Index(int x, int y)
    {
        // Check x, y within bounds
        if (x >= Region::LENGTH || y >= Region::LENGTH) LogInfo("Invalid index [{}, {}]", x, y);
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[x + (y * Region::LENGTH)];
    }

    Tile& Region::Index(const Vivium::Vector2<int>& pos)
    {
        // Check x, y within bounds
        if (pos.x >= Region::LENGTH || pos.y >= Region::LENGTH) LogInfo("Invalid index {}", pos);
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[pos.x + (pos.y * Region::LENGTH)];
    }
}
