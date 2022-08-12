#include "Tile.h"

namespace Game {
	// TODO: load object data from file (text)
	std::array<Tile::Properties, (uint16_t)Tile::ID::MAX> Tile::m_Properties = {
		Tile::Properties("void",				false,	false,	false, 0.0f, {0,  0}, 1.0f, {}),
		Tile::Properties("ground",				false,	false,	false, 0.0f, {1,  0}, 1.0f, {}),
		Tile::Properties("grass",				false,	false,	false, 0.0f, {2,  0}, 1.0f, {}),
		Tile::Properties("tree_0",				true,	true,	false, 2.0f, {3,  1}, 1.0f, Item::DropTable({{1.0f, Item::DropData(Item::ID::LOG, 2)}})),
		Tile::Properties("tree_1",				false,	true,	false, 2.0f, {3,  0}, 1.0f, Item::DropTable({{1.0f, Item::DropData(Item::ID::LOG, 2)}})),
		Tile::Properties("sand",				false,	false,	false, 0.0f, {4,  0}, 1.0f, {}),
		Tile::Properties("water",				false,	false,	false, 0.0f, {5,  0}, 1.0f, {}),
		Tile::Properties("bush",				true,	true,	false, 1.0f, {6,  0}, 0.8f, {}), // TODO drop table
		Tile::Properties("bush_fruit",			true,	true,	false, 0.5f, {7,  0}, 0.8f, {}), // TODO drop table
		Tile::Properties("amethyst_node",		true,	true,	false, 1.0f, {8,  0}, 0.8f, Item::DropTable({{1.0f, Item::DropData(Item::ID::AMETHYST_CRYSTAL, 10, 15)}})),
		Tile::Properties("emerald_node",		true,	true,	false, 1.0f, {9,  0}, 0.8f, Item::DropTable({{1.0f, Item::DropData(Item::ID::EMERALD_CRYSTAL, 1, 3)}})),
		Tile::Properties("ruby_node",			true,	true,	false, 1.0f, {10, 0}, 0.8f, Item::DropTable({{1.0f, Item::DropData(Item::ID::RUBY_CRYSTAL, 1, 3)}})),
		Tile::Properties("sapphire_node",		true,	true,	false, 1.0f, {11, 0}, 0.8f, Item::DropTable({{1.0f, Item::DropData(Item::ID::SAPPHIRE_CRYSTAL, 1, 3)}})),
		Tile::Properties("topaz_node",			true,	true,	false, 1.0f, {12, 0}, 0.8f, Item::DropTable({{1.0f, Item::DropData(Item::ID::TOPAZ_CRYSTAL, 1, 3)}}))
	};

	Tile::Properties Tile::GetProperties(const Tile::ID& id)
	{
		return Tile::m_Properties[uint16_t(id)];
	}

	std::string Tile::GetName(const Tile::ID& id)
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

	Vivium::Vector2<int> Tile::GetAltasIndex(const Tile::ID& id)
	{
		return Tile::m_Properties[uint16_t(id)].atlas_index;
	}

	float Tile::GetScale(const Tile::ID& id)
	{
		return Tile::m_Properties[uint16_t(id)].scale;
	}

	Item::DropTable Tile::GetDropData(const Tile::ID& id)
	{
		return Tile::m_Properties[(uint16_t)id].drop_data;
	}

	void Tile::CopyRealTiles(const Tile& other)
	{
		base = other.base == Tile::ID::VOID ? base : other.base;
		mid = other.mid == Tile::ID::VOID ? mid : other.mid;
		top = other.top == Tile::ID::VOID ? top : other.top;
	}

	bool Tile::CompareRealTiles(const Tile& other)
	{
		// TODO: implementation
		return false;
	}

	Tile::Tile()
		: base(Tile::ID::VOID), mid(Tile::ID::VOID), top(Tile::ID::VOID)
	{}

	Tile::Tile(const Tile::ID& base, const Tile::ID& mid, const Tile::ID& top)
		: base(base), mid(mid), top(top)
	{}

	Tile::Tile(const Tile& other)
		: base(other.base), mid(other.mid), top(other.top)
	{}

	Tile::Properties::Properties(std::string name, bool isPhysical, bool isMineable, bool isPlaceable, float mining_time, Vivium::Vector2<int> atlas_index, float scale, Item::DropTable drop_data)
		: name(name), isPhysical(isPhysical), isMineable(isMineable), isPlaceable(isPlaceable), mining_time(mining_time), atlas_index(atlas_index), scale(scale), drop_data(drop_data) {}

	void Tile::Properties::Write(Vivium::Serialiser& s) const
	{
		s.Write(name);
		s.Write(isPhysical);
		s.Write(isMineable);
		s.Write(mining_time);
		s.Write(atlas_index);
	}
	void Tile::Properties::Read(Vivium::Serialiser& s)
	{
		s.Read(&name);
		s.Read(&isPhysical);
		s.Read(&isMineable);
		s.Read(&mining_time);
		s.Read(&atlas_index);
	}
}
