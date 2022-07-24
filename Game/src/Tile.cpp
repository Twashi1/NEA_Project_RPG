#include "Tile.h"

std::array<Tile::Properties, (uint16_t)Tile::ID::MAX> Tile::m_Properties = {
	Tile::Properties("void", false),
	Tile::Properties("ground", false),
	Tile::Properties("grass", false),
	Tile::Properties("tree", true)
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

Tile::Tile() { ids.emplace_back(Tile::ID::VOID); }

Tile::Tile(const Tile::ID& id) { ids.emplace_back(id); }

Tile::Tile(const std::vector<Tile::ID>& ids) : ids(ids) {}

Tile::Tile(const Tile& other) : ids(other.ids) {}

Tile::Properties::Properties(const char* name, bool isPhysical)
	: name(name), isPhysical(isPhysical) {}