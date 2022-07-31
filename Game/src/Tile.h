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

	std::vector<ID> ids; // List of IDs of tiles that should display at the same coordinate

	// Explicit conversion to id
	explicit operator Tile::ID() const { return ids[0]; }

	Tile();
	Tile(const Tile::ID& id);
	Tile(const std::vector<Tile::ID>& ids);
	Tile(const Tile& other);

private:
	// TODO: Load properties data from file instead?
	static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
};