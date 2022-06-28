#pragma once
#include "Utilities.h"

#include <array>

class Tile {
public:
	enum class ID : uint16_t {
		VOID,
		GROUND,
		GRASS,
		TREE
	};

	struct Properties {
		const char* name; // Tile::ID as string
		bool isPhysical;  // If tile is something the player can collide with
		int z;			  // Determines what this should be drawn behind or in front of

		Properties(const char* name, bool isPhysical, int z);
	};

	static constexpr uint16_t TOTAL_TILES = 4; // Amount of different tiles which have IDs allocated

	static Properties GetProperties(const Tile::ID& id);
	static const char* GetName(const Tile::ID& id);
	static bool GetIsPhysical(const Tile::ID& id);
	static int GetZ(const Tile::ID& id);

	std::vector<ID> ids; // List of IDs of tiles that should display at the same coordinate

	// Implicit conversion to id
	operator Tile::ID() const { return ids[0]; }

	Tile();
	Tile(const Tile::ID& id);
	Tile(const std::vector<Tile::ID>& ids);
	Tile(const Tile& other);

private:
	static std::array<Properties, TOTAL_TILES> m_properties;
};