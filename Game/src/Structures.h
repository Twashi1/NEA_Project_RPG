#pragma once

#include "Tile.h"

// world.CreateStructure(pos, Structure::ID::TREE)
// -> for (offset, tile : Structure::GetTilemap(id))

class World;

class Structure {
public:
	enum class ID : uint16_t {
		TREE
	};

	typedef std::unordered_map<Vivium::Vector2<int>, Tile> TileMap_t;

private:
	static const std::vector<TileMap_t>* m_Tilemaps; // On heap

	static bool m_CheckOverlapTile(const Tile& structure_tile, const Tile& world_tile);

public:
	static const TileMap_t& GetTilemap(const Structure::ID& id); // TODO: probably useless
	static TileMap_t GetTilemap(const Vivium::Vector2<int>& pos, const Structure::ID& id);
	static void Place(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world);
};
