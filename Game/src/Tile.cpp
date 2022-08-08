#include "Tile.h"

// TODO: load object data from file (text)
std::array<Tile::Properties, (uint16_t)Tile::ID::MAX> Tile::m_Properties = {
	Tile::Properties("void", false, false, 0.0f),
	Tile::Properties("ground", false, false, 0.0f),
	Tile::Properties("grass", false, false, 0.0f),
	Tile::Properties("tree_bottom", true, true, 3.0f),
	Tile::Properties("tree_top", false, false, 0.0f),
	Tile::Properties("sand", false, false, 0.0f),
	Tile::Properties("water", false, false, 0.0f),
	Tile::Properties("bush", true, true, 1.0f),
	Tile::Properties("bush_fruit", true, true, 1.5f),
	Tile::Properties("amethyst_node", true, true, 3.0f)
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

bool Tile::GetIsMineable(const Tile::ID& id)
{
	return Tile::m_Properties[uint16_t(id)].isMineable;
}

float Tile::GetMiningTime(const Tile::ID& id)
{
	return Tile::m_Properties[uint16_t(id)].mining_time;
}

Tile::Tile()
	: base(Tile::ID::VOID), mid(Tile::ID::VOID), top(Tile::ID::VOID)
{}

Tile::Tile(const Tile::ID& base, const Tile::ID& mid, const Tile::ID& top)
	: base(base), mid(mid), top(top)
{}

Tile::Tile(const Tile & other)
	: base(other.base), mid(other.mid), top(other.top)
{}

Tile::Properties::Properties(const char* name, bool isPhysical, bool isMineable, float mining_time)
	: name(name), isPhysical(isPhysical), isMineable(isMineable), mining_time(mining_time) {}