#include "Tile.h"

std::array<Tile::Properties, Tile::TOTAL_TILES> Tile::m_Properties = {
	Tile::Properties("void", false, 0),
	Tile::Properties("ground", false, 1),
	Tile::Properties("grass", false, 2),
	Tile::Properties("tree", true, 2)
};

Tile::Properties Tile::GetProperties(const Tile::ID& id)
{
	return Tile::m_Properties[uint16_t(id)];
}

const char* Tile::GetName(const Tile::ID& id)
{
	return Tile::m_Properties[uint16_t(id)].name;
}

bool Tile::GetIsPhysical(const Tile::ID& id)
{
	return Tile::m_Properties[uint16_t(id)].isPhysical;
}

int Tile::GetZ(const Tile::ID& id)
{
	return Tile::m_Properties[uint16_t(id)].z;
}

Tile::Tile() { ids.emplace_back(Tile::ID::VOID); }

Tile::Tile(const Tile::ID& id) { ids.emplace_back(id); }

Tile::Tile(const std::vector<Tile::ID>& ids) : ids(ids) {}

Tile::Tile(const Tile& other) : ids(other.ids) {}

Tile::Properties::Properties(const char* name, bool isPhysical, int z)
	: name(name), isPhysical(isPhysical), z(z) {}

template <> void Serialise(Serialiser& s, const Tile& data)
{
	// Serialise amount of ids in tile
	Serialise<uint8_t>(s, (uint8_t)data.ids.size());

	// Serialise each tile in ids
	for (const Tile::ID& id : data.ids) {
		Serialise<uint16_t>(s, (uint16_t)id);
	}
}

template <> void Deserialise(const Serialiser& s, Tile* memory)
{
	// Get amount of Tile::IDs
	uint8_t size;
	Deserialise<uint8_t>(s, &size);

	// Create vector to store Tile::IDs
	std::vector<Tile::ID> ids;
	ids.reserve(size);

	// Deserialise each tile
	for (int i = 0; i < size; i++) {
		// Deserialise tile id
		uint16_t tile_id;
		Deserialise<uint16_t>(s, &tile_id);

		// Push tile id into ids
		ids.push_back((Tile::ID)tile_id);
	}

	*memory = Tile(ids);
}

template <> void Serialise(Serialiser& s, Tile* data, const uint32_t& length)
{
	// Write length to file
	Serialise<uint32_t>(s, length);

	// Serialise each tile
	for (int i = 0; i < length; i++) {
		Serialise<Tile>(s, data[i]);
	}
}

template <> void DeserialiseArray(const Serialiser& s, Tile* memory)
{
	// Get length of array
	uint32_t length;
	Deserialise<uint32_t>(s, &length);

	for (uint32_t i = 0; i < length; i++) {
		// Deserialise each object
		Deserialise<Tile> (s, &(memory[i]));
	}
}

template <> void Serialise(Serialiser& s, Tile::ID* data, const uint32_t& length)
{
	// Write length to file
	Serialise<uint32_t>(s, length);

	// Serialise each tile
	for (int i = 0; i < length; i++) {
		Serialise<uint16_t>(s, *(uint16_t*)&data[i]);
	}
}
template <> void DeserialiseArray(const Serialiser& s, Tile::ID* memory)
{
	// Get length of array
	uint32_t length;
	Deserialise<uint32_t>(s, &length);

	for (uint32_t i = 0; i < length; i++) {
		// Deserialise each object
		Deserialise<uint16_t>(s, (uint16_t*)&(memory[i]));
	}
}