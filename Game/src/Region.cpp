#include "Region.h"

namespace Game {
    Region::Region()
    {
        // Allocate memory for tiles array
        // tiles = new Tile[Region::SIZE];
    }

    Region::Region(Region&& other) noexcept
        : tiles(std::move(other.tiles))
    {}

    Region::~Region()
    {
    }

    Tile& Region::Index(int x, int y)
    {
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[x + (y * Region::LENGTH)];
    }

    Tile& Region::Index(const Vivium::Vector2<int>& pos)
    {
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[pos.x + (pos.y * Region::LENGTH)];
    }

    const Tile& Region::CIndex(int x, int y)
    {
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[x + (y * Region::LENGTH)];
    }

    const Tile& Region::CIndex(const Vivium::Vector2<int>& pos)
    {
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[pos.x + (pos.y * Region::LENGTH)];
    }
}
