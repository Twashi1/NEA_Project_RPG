#include "Structures.h"

#include "World.h"

namespace Game {
	const std::vector<Structure::TileMap_t>* Structure::m_Tilemaps = new std::vector<Structure::TileMap_t>
	{
		// Tree
		TileMap_t{
			{{0, 0}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::TREE_0)},
			{{0, 1}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::TREE_1)}
		}
	};

	bool Structure::m_CheckOverlapTile(const Tile& structure_tile, const Tile& world_tile)
	{
		return !((world_tile.base != Tile::ID::VOID && structure_tile.base != Tile::ID::VOID) ||
			(world_tile.mid != Tile::ID::VOID && structure_tile.mid != Tile::ID::VOID) ||
			(world_tile.top != Tile::ID::VOID && structure_tile.top != Tile::ID::VOID));
	}

	const Structure::TileMap_t& Structure::GetTilemap(const Structure::ID& id)
	{
		// TODO: some validation
		return m_Tilemaps->at((uint16_t)id);
	}

	Structure::TileMap_t Structure::GetTilemap(const Vivium::Vector2<int>& pos, const Structure::ID& id)
	{
		// Get tilemap
		const TileMap_t& tilemap = m_Tilemaps->at((uint16_t)id);
	
		// Iterate tiles and offset them
		TileMap_t new_tilemap;

		for (auto& [offset, tile] : tilemap) {
			new_tilemap[offset + pos] = tile;
		}

		return new_tilemap;
	}

	void Structure::Place(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world)
	{
		TileMap_t tilemap = GetTilemap(pos, id);

		for (auto& [pos, tile] : tilemap) {
			Tile& world_tile = world->GetTile(pos);

			if (!m_CheckOverlapTile(tile, world_tile))
			{
				// Exit function since one tile was invalid
				return;
			}
		}

		for (auto& [pos, tile] : tilemap) {
			Tile& world_tile = world->GetTile(pos);

			// We already know no tiles will overlap at this point so
			world_tile.CopyRealTiles(tile);
		}
	}

	void Structure::Delete(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world)
	{
		TileMap_t tilemap = GetTilemap(pos, id);

		for (auto& [pos, tile] : tilemap) {
			Tile& world_tile = world->GetTile(pos);

			if (tile.base != Tile::ID::VOID) { world_tile.base = Tile::ID::VOID; }
			if (tile.mid != Tile::ID::VOID) { world_tile.mid = Tile::ID::VOID; }
			if (tile.top != Tile::ID::VOID) { world_tile.top = Tile::ID::VOID; }
		}
	}
}