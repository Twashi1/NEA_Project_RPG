#pragma once

#include <array>
#include <vector>

#include <Engine.h>

class Tile {
public:
	enum class ID : uint16_t {
		VOID,
		GROUND,
		GRASS,
		TREE,
		MAX
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

	// Explicit conversion to id
	explicit operator Tile::ID() const { return ids[0]; }

	Tile();
	Tile(const Tile::ID& id);
	Tile(const std::vector<Tile::ID>& ids);
	Tile(const Tile& other);

private:
	static std::array<Properties, TOTAL_TILES> m_Properties;
};

template <>
void Serialise<Tile>(Serialiser& s, const Tile& data);

template <>
void Deserialise<Tile>(const Serialiser& s, Tile* memory);

template <>
void Serialise<Tile>(Serialiser& s, Tile* data, const uint32_t& length);

template <>
void DeserialiseArray<Tile>(const Serialiser&, Tile*);

template <>
void Serialise<Tile::ID>(Serialiser& s, Tile::ID* data, const uint32_t& length);

template <>
void DeserialiseArray<Tile::ID>(const Serialiser&, Tile::ID*);