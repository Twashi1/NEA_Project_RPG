#include "Region.h"

namespace Game {
    Region::Region(Region&& other) noexcept
        : tiles(std::move(other.tiles)), biomes(std::move(other.biomes)), npcs(std::move(other.npcs))
    {}

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
        
        // Write npcs
        s.Write(npcs.size());

        for (std::shared_ptr<NPC> npc : npcs) {
            // TODO: this would write a NPC type object, not whatever the derived class is
            //s.Write(*npc);
        }
    }

    void Region::Read(Vivium::Serialiser& s)
    {
        s.Read(&tiles);
        s.Read(&biomes);

        // Read npcs
        std::size_t size;

        s.Read(&size);
        npcs.reserve(size);

        for (std::size_t i = 0; i < size; i++) {
            // TODO: need to allocate memory for a class we don't know the type of, need to store type of derived as well?
            //Ref(NPC) new_npc = MakeRef(NPC);
            //s.Read(new_npc.get());

            //npcs.push_back(new_npc);
        }
    }
}
