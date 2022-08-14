#include "Region.h"

namespace Game {
    Region::Region()
    {
        // Allocate memory for tiles array
        tiles = std::make_shared_for_overwrite<Tile[]>(Region::SIZE);
    }

    Region::Region(const Region& other)
        : tiles(other.tiles)
    {}

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

    const Tile& Region::CIndex(int x, int y)
    {
        // Check x, y within bounds
        if (x >= Region::LENGTH || y >= Region::LENGTH) LogInfo("Invalid index [{}, {}]", x, y);
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[x + (y * Region::LENGTH)];
    }

    const Tile& Region::CIndex(const Vivium::Vector2<int>& pos)
    {
        // Check x, y within bounds
        if (pos.x >= Region::LENGTH || pos.y >= Region::LENGTH) LogInfo("Invalid index {}", pos);
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[pos.x + (pos.y * Region::LENGTH)];
    }
}
