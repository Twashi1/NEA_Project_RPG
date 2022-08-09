#pragma once

#include <Vivium.h>

class Tile {
public:
	struct AtlasData : Vivium::IStreamable {
		Vivium::Vector2<int> index;

		AtlasData();
		AtlasData(const Vivium::Vector2<int>& index);

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;
	};

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
		MAX
	};

	struct Properties : Vivium::IStreamable {
		const char* name;		// Tile::ID as string
		bool isPhysical;		// If tile is something the player can collide with
		bool isMineable;		// If player can mine the item
		float mining_time;		// Amount of time it takes to mine this tile
		AtlasData atlas_data;

		Properties(const char* name, bool isPhysical, bool isMineable, float mining_time, AtlasData atlas_data);

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;
	};

	static Properties GetProperties(const Tile::ID& id);
	static const char* GetName(const Tile::ID& id);
	static bool GetIsPhysical(const Tile::ID& id);
	static bool GetIsMineable(const Tile::ID& id);
	static float GetMiningTime(const Tile::ID& id);
	static AtlasData GetAltasData(const Tile::ID& id);

	Tile::ID base; // TODO: rename to bot
	Tile::ID mid;
	Tile::ID top;

	// Copies only non-void tiles
	void CopyRealTiles(const Tile& other);

	Tile();
	Tile(const Tile::ID& base, const Tile::ID& mid = Tile::ID::VOID, const Tile::ID& top = Tile::ID::VOID);
	Tile(const Tile& other);

private:
	// TODO: Load properties data from file instead?
	static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
};

namespace std {
	template <>
	struct formatter<Tile::ID> : formatter<string> {
		auto format(Tile::ID id, format_context& ctx) {
			return formatter<string>::format(
				Tile::GetName(id), ctx);
		}
	};
}
