#pragma once

#include "Tile.h"

namespace Game {
	class World;

	/// <summary>
	/// A world structure, a multi-tile naturally generated building/thing
	/// </summary>
	class Structure {
	public:
		enum class ID : uint16_t {
			VOID,
			TREE,
			CACTUS_TALL
		};

		typedef std::unordered_map<Vivium::Vector2<int>, Tile> TileMap_t;

	private:
		static const std::vector<TileMap_t>* m_Tilemaps; // On heap

		/// <summary>
		/// Check if a tile of the structure is overlapping a (physical) world tile
		/// </summary>
		/// <param name="structure_tile"></param>
		/// <param name="world_tile"></param>
		/// <returns></returns>
		static bool m_CheckOverlapTile(const Tile& structure_tile, const Tile& world_tile);

	public:
		/// <summary>
		/// Get relative tilemap given a structure id
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static const TileMap_t& GetTilemap(const Structure::ID& id); // TODO: probably useless
		/// <summary>
		/// Get a tilemap given a structure id centered at a world position
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="id"></param>
		/// <returns></returns>
		static TileMap_t GetTilemap(const Vivium::Vector2<int>& pos, const Structure::ID& id);
		/// <summary>
		/// Place a tile at a location, returning if it was successful or not
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="id"></param>
		/// <param name="world"></param>
		/// <returns></returns>
		static bool Place(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world);
		/// <summary>
		/// Destroy a structure at a position
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="id"></param>
		/// <param name="world"></param>
		static void Delete(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world);

		/// <summary>
		/// Get pretty name of structure
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static const char* GetName(const Structure::ID& id);

		/// <summary>
		/// Get id of structure given one of the tiles in the structure
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static Structure::ID GetStructureID(const Tile::ID& id);
		/// <summary>
		/// Check if tile is part of a structure
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static bool IsStructure(const Tile::ID& id);

		/// <summary>
		/// Get offset of a tile from the center of a structure
		/// </summary>
		/// <param name="id"></param>
		/// <param name="structure_id"></param>
		/// <returns></returns>
		static Vivium::Vector2<int> GetTileOffset(const Tile::ID& id, const Structure::ID& structure_id);
		/// <summary>
		/// Check if a given position is within the bounds of the structure
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="structure_pos"></param>
		/// <param name="id"></param>
		/// <returns></returns>
		static bool IsWithinStructureBounds(const Vivium::Vector2<int>& pos, const Vivium::Vector2<int>& structure_pos, const Structure::ID& id);

		/// <summary>
		/// Given a id of a tile in the structure, derive structure id, and delete structure
		/// </summary>
		/// <param name="pos"></param>
		/// <param name="id"></param>
		/// <param name="world"></param>
		static void Delete(const Vivium::Vector2<int>& pos, const Tile::ID& id, World* world);

		friend std::ostream& operator<<(std::ostream& os, const Structure::ID& id) {
			os << Game::Structure::GetName(id);

			return os;
		}
	};
}

namespace std {
	template <>
	struct formatter<Game::Structure::ID> : formatter<string> {
		auto format(Game::Structure::ID id, format_context& ctx) {
			return formatter<string>::format(
				Game::Structure::GetName(id), ctx);
		}
	};
}
