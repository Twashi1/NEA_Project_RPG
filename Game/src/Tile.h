#pragma once

#include "Item.h"

namespace Game {
	/// <summary>
	/// A single tile in the world, holding a foreground and a background
	/// 
	/// Also holds properties information for each tile
	/// </summary>
	class Tile : public Vivium::StreamablePOD {
	public:
		static constexpr float SPRITE_SIZE = 16.0f;

		enum class ID : uint16_t {
			VOID,
			GROUND,
			GRASS,
			TREE_0,
			TREE_1,
			SAND,
			WATER,
			BUSH,
			BUSH_FRUIT,
			AMETHYST_NODE,
			EMERALD_NODE,
			RUBY_NODE,
			SAPPHIRE_NODE,
			TOPAZ_NODE,
			CACTUS_SMALL,
			CACTUS_TALL0,
			CACTUS_TALL1,
			CACTUS_FRUIT,
			MOSSY_DEBRIS,
			ROCKY_DEBRIS,
			PINK_FLOWER,
			BLUE_FLOWER,
			MAX
		};

		/// <summary>
		/// Properties for each tile, such as name, index for the texture, if the tile is physical, etc.
		/// </summary>
		struct Properties : Vivium::Streamable {
			std::string name;						// Pretty name
			bool isPhysical;						// If tile is something the player can collide with
			bool isMineable;						// If player can mine the item
			bool isPlaceable;						// If its a tile that can be placed
			float mining_time;						// Amount of time it takes to mine this tile
			Vivium::Vector2<int> atlas_index;		// Coordinate of sprite in atlas
			float scale;							// Scale of tile when drawn
			Tool::Type tool_type;					// Tool type for this item
			Item::DropTable drop_data;				// Drop table for item

			Properties(std::string name, bool isPhysical, bool isMineable, bool isPlaceable, float mining_time, Vivium::Vector2<int> atlas_data, float scale, Tool::Type type, Item::DropTable drop_data);

			// Serialiser methods: unused, but maybe in future? (once I have some sort of text serialisation)
			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		// Getters for the properties
		static Properties GetProperties(const Tile::ID& id);
		static std::string GetName(const Tile::ID& id);
		static bool GetIsPhysical(const Tile::ID& id);
		static bool GetIsMineable(const Tile::ID& id);
		static float GetMiningTime(const Tile::ID& id);
		static float GetMiningTime(const Tile::ID& id, const Item::ID& tool);
		static Vivium::Vector2<int> GetAtlasIndex(const Tile::ID& id);
		static float GetScale(const Tile::ID& id);
		static Tool::Type GetToolType(const Tile::ID& id);
		static Item::DropTable GetDropData(const Tile::ID& id);

		// Actual data for each tile, each tile stores a background and a foreground
		Tile::ID background;
		Tile::ID foreground;

		// Copies only non-void tiles
		void CopyRealTiles(const Tile& other);
		bool CompareRealTiles(const Tile& other);

		/// <summary>
		/// For getting the background/foreground tile by an index instead (0->background, 1->foreground)
		/// </summary>
		/// <param name="os"></param>
		/// <param name="id"></param>
		/// <returns></returns>
		Tile::ID& Index(int x);

		/// <summary>
		/// Get the highest tile that is not void
		/// </summary>
		Tile::ID GetHighestRealTile() const;

		Tile();
		Tile(const Tile::ID& background, const Tile::ID& foreground = Tile::ID::VOID);
		Tile(const Tile& other);

	private:
		// TODO: Load properties data from file instead (implement text serialisation)
		static std::array<Properties, (uint16_t)ID::MAX> m_Properties;

	public:
		friend std::ostream& operator<<(std::ostream& os, const Tile::ID& id) {
			os << Game::Tile::GetName(id);

			return os;
		}
	};
}

namespace std {
	template <>
	struct formatter<Game::Tile::ID> : formatter<string> {
		auto format(Game::Tile::ID id, format_context& ctx) {
			return formatter<string>::format(
				Game::Tile::GetName(id), ctx);
		}
	};
}
