#pragma once

#include "Tile.h"

namespace Game {
	class World;

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

		static bool m_CheckOverlapTile(const Tile& structure_tile, const Tile& world_tile);

	public:
		static const TileMap_t& GetTilemap(const Structure::ID& id); // TODO: probably useless
		static TileMap_t GetTilemap(const Vivium::Vector2<int>& pos, const Structure::ID& id);
		static bool Place(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world);
		static void Delete(const Vivium::Vector2<int>& pos, const Structure::ID& id, World* world);

		static const char* GetName(const Structure::ID& id);

		static Structure::ID GetStructureID(const Tile::ID& id);
		static bool IsStructure(const Tile::ID& id);

		static Vivium::Vector2<int> GetTileOffset(const Tile::ID& id, const Structure::ID& structure_id);
		static bool IsWithinStructureBounds(const Vivium::Vector2<int>& pos, const Vivium::Vector2<int>& structure_pos, const Structure::ID& id);

		// Delete structure from id of one of the tiles in structure
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
