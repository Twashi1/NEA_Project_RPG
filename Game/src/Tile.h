#pragma once

#include <Vivium.h>

class Tile {
public:
	enum class ID : uint16_t {
		VOID,
		GROUND,
		GRASS,
		TREE,
		SAND,
		WATER,
		MAX
	};

	struct Properties {
		const char* name; // Tile::ID as string
		bool isPhysical;  // If tile is something the player can collide with

		Properties(const char* name, bool isPhysical);
	};

	static Properties GetProperties(const Tile::ID& id);
	static const char* GetName(const Tile::ID& id);
	static bool GetIsPhysical(const Tile::ID& id);

	Tile::ID base;
	Tile::ID mid;
	Tile::ID top;

	Tile();
	Tile(const Tile::ID& base, const Tile::ID& mid = Tile::ID::VOID, const Tile::ID& top = Tile::ID::VOID);
	Tile(const Tile& other);

private:
	// TODO: Load properties data from file instead?
	static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
};