#include "Tile.h"

// TODO: load object data from serialised file
std::array<Tile::Properties, (uint16_t)Tile::ID::MAX> Tile::m_Properties = {
	Tile::Properties("void", false),
	Tile::Properties("ground", false),
	Tile::Properties("grass", false),
	Tile::Properties("tree", true),
	Tile::Properties("sand", false),
	Tile::Properties("water", false)
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

Tile::Tile()
	: base(Tile::ID::VOID), mid(Tile::ID::VOID), top(Tile::ID::VOID)
{}

Tile::Tile(const Tile::ID& base, const Tile::ID& mid, const Tile::ID& top)
	: base(base), mid(mid), top(top)
{}

Tile::Tile(const Tile & other)
	: base(other.base), mid(other.mid), top(other.top)
{}

Tile::Properties::Properties(const char* name, bool isPhysical)
	: name(name), isPhysical(isPhysical) {}