#include "Tile.h"

std::array<Tile::Properties, Tile::TOTAL_TILES> Tile::m_Properties = {
	Tile::Properties("void", false, 0),
	Tile::Properties("ground", false, 1),
	Tile::Properties("grass", false, 2),
	Tile::Properties("tree", true, 2)
};

Tile::Properties Tile::GetProperties(const Tile::ID& id)
{
	return Tile::m_properties[uint16_t(id)];
}

const char* Tile::GetName(const Tile::ID& id)
{
	return Tile::m_properties[uint16_t(id)].name;
}

bool Tile::GetIsPhysical(const Tile::ID& id)
{
	return Tile::m_properties[uint16_t(id)].isPhysical;
}

int Tile::GetZ(const Tile::ID& id)
{
	return Tile::m_properties[uint16_t(id)].z;
}

Tile::Tile() { ids.emplace_back(Tile::ID::VOID); }

Tile::Tile(const Tile::ID& id) { ids.emplace_back(id); }

Tile::Tile(const std::vector<Tile::ID>& ids) : ids(ids) {}

Tile::Tile(const Tile& other) : ids(other.ids) {}

Tile::Properties::Properties(const char* name, bool isPhysical, int z)
	: name(name), isPhysical(isPhysical), z(z) {}
