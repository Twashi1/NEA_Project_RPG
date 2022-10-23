#include "Region.h"

namespace Game {
    Region::Region(Region&& other) noexcept
        : tiles(std::move(other.tiles)), biomes(std::move(other.biomes)), npcs(std::move(other.npcs))
    {
        LogTrace("Region moved!");
    }

    Tile& Region::Index(int x, int y)
    {
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[x + y * Region::LENGTH];
    }

    Tile& Region::Index(const Vivium::Vector2<int>& pos)
    {
        // Tiles is a 1D array for simplicity when serialising, but we index it as if it were a 2D array
        return tiles[pos.x + pos.y * Region::LENGTH];
    }

    Biome::ID& Region::IndexBiome(int x, int y)
    {
        return biomes[x + y * Region::LENGTH];
    }

    Biome::ID& Region::IndexBiome(const Vivium::Vector2<int>& pos)
    {
        return biomes[pos.x + pos.y * Region::LENGTH];
    }

    void Region::Write(Vivium::Serialiser& s) const
    {
        s.Write(tiles);
        s.Write(biomes);
        s.Write(npcs);
    }

    void Region::Read(Vivium::Serialiser& s)
    {
        s.Read(&tiles);
        s.Read(&biomes);
        s.Read(&npcs);
    }
}
