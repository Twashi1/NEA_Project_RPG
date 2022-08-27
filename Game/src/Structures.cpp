#include "Structures.h"

#include "World.h"

namespace Game {
	const std::vector<Structure::TileMap_t>* Structure::m_Tilemaps = new std::vector<Structure::TileMap_t>
	{
		// Void
		TileMap_t{
			{{0, 0}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::VOID)}
		},
		// Tree
		TileMap_t{
			{{0, 0}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::TREE_0)},
			{{0, 1}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::TREE_1)}
		},
		// Tall cactus
		TileMap_t{
			{{0, 0}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::CACTUS_TALL0)},
			{{0, 1}, Tile(Tile::ID::VOID, Tile::ID::VOID, Tile::ID::CACTUS_TALL1)}
		}
	};

	bool Structure::m_CheckOverlapTile(const Tile& structure_tile, const Tile& world_tile)
	{
		return !((world_tile.bot != Tile::ID::VOID && structure_tile.bot != Tile::ID::VOID) ||
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

	bool Structure::Place(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world)
	{
		TileMap_t tilemap = GetTilemap(pos, id);

		for (auto& [pos, tile] : tilemap) {
			// TODO: this is a bad solution since it means if a structure is placed on a region boundary it is cancelled, so larger structures will very
			//		often be cancelled, instead world should keep track of all structures, and when a region is generated, if it checks against the world's
			//		structure list, and will act appropriately if it contains a structure (performance risks for large worlds with lots of structures generated?)
			Tile* world_tile = world->GetLoadedTile(pos);

			// Tile was not loaded in the world
			if (world_tile == nullptr) return false;

			// Check structure doesn't overlap the current tile
			if (!m_CheckOverlapTile(tile, *world_tile))
			{
				// Exit function since one tile was invalid
				return false;
			}
		}

		for (auto& [pos, tile] : tilemap) {
			// No world tiles should be unloaded by this point
			Tile* world_tile = world->GetLoadedTile(pos);

			// No tiles overlap so we can just copy over the tile data
			world_tile->CopyRealTiles(tile);
		}

		return true;
	}

	void Structure::Delete(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world)
	{
		TileMap_t tilemap = GetTilemap(pos, id);

		for (auto& [pos, tile] : tilemap) {
			Tile& world_tile = world->GetTile(pos);

			if (tile.bot != Tile::ID::VOID) { world_tile.bot = Tile::ID::VOID; }
			if (tile.mid != Tile::ID::VOID) { world_tile.mid = Tile::ID::VOID; }
			if (tile.top != Tile::ID::VOID) { world_tile.top = Tile::ID::VOID; }
		}
	}

	const char* Structure::GetName(const Structure::ID& id)
	{
		switch (id) {
		case ID::VOID: return "Void";
		case ID::TREE: return "Tree";
		case ID::CACTUS_TALL: return "Cactus tall";
		default: return "Invalid structure";
		}
	}

	Structure::ID Structure::GetStructureID(const Tile::ID& id)
	{
		// TODO: tiles should store the structure id they're a part of
		switch (id) {
		case Tile::ID::TREE_0:
		case Tile::ID::TREE_1:
			return Structure::ID::TREE;
		case Tile::ID::CACTUS_TALL0:
		case Tile::ID::CACTUS_TALL1:
			return Structure::ID::CACTUS_TALL;
		default:
			LogWarn("No structure that contains tile: {}", id); break;
		}

		return Structure::ID::VOID;
	}

	bool Structure::IsStructure(const Tile::ID& id)
	{
		// TODO: tile should store if they're part of a structure
		switch (id) {
		case Tile::ID::TREE_0:
		case Tile::ID::TREE_1:
		case Tile::ID::CACTUS_TALL0:
		case Tile::ID::CACTUS_TALL1:
			return true;
		}

		return false;
	}

	Vivium::Vector2<int> Structure::GetTileOffset(const Tile::ID& id, const Structure::ID& structure_id)
	{
		// Get untransformed tilemap
		TileMap_t tilemap = GetTilemap(structure_id);


		for (auto& [offset, tile] : tilemap) {
			// TODO: temp, only works for top tiles
			if (tile.top == id && tile.top != Tile::ID::VOID) {
				return offset;
			}
		}

		// Couldn't find id in tilemap (Shouldn't ever reach here)
		LogError("Couldn't find matching tile in tilemap for structure id {}", structure_id);

		return Vivium::Vector2<int>(INT_MAX);
	}

	bool Structure::IsWithinStructureBounds(const Vivium::Vector2<int>& pos, const Vivium::Vector2<int>& structure_pos, const Structure::ID& id)
	{
		const TileMap_t& tilemap = GetTilemap(id);
		
		Vivium::Vector2<int> relative_offset = pos - structure_pos;

		for (const auto& [offset, tile] : tilemap) {
			if (relative_offset == offset) {
				return true;
			}
		}

		return false;
	}

	void Structure::Delete(const Vivium::Vector2<int>& pos, const Tile::ID& id, World* world)
	{
		Structure::ID structure_id = GetStructureID(id);

		if (structure_id == Structure::ID::VOID) return;

		// Get untransformed tilemap
		TileMap_t tilemap = GetTilemap(structure_id);

		// Search for offset of given tile in tilemap
		Vivium::Vector2<int> desired_offset = GetTileOffset(id, structure_id);

		Structure::Delete(pos - desired_offset, structure_id, world);
	}
}