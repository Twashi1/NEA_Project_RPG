#include "Tile.h"

namespace Game {
	// TODO: load object data from file (text)
	std::array<Tile::Properties, (uint16_t)Tile::ID::MAX> Tile::m_Properties = {
		//				 NAME                 isPhysical   isMineable  isPlaceable  mining_time  atlas_index   scale   drop data
		Tile::Properties("void",				false,		false,		false,		0.0f,		 {0,  0},		1.0f, {}),
		Tile::Properties("ground",				false,		false,		false,		0.0f,		 {1,  0},		1.0f, {}),
		Tile::Properties("grass",				false,		false,		false,		0.0f,		 {2,  0},		1.0f, {}),
		Tile::Properties("tree_0",				true,		true,		false,		2.0f,		 {3,  1},		1.0f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::LOG, 2)}}})),
		Tile::Properties("tree_1",				false,		true,		false,		2.0f,		 {3,  0},		1.0f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::LOG, 2)}}})),
		Tile::Properties("sand",				false,		false,		false,		0.0f,		 {4,  0},		1.0f, {}),
		Tile::Properties("water",				false,		false,		false,		0.0f,		 {5,  0},		1.0f, {}),
		Tile::Properties("bush",				false,		true,		false,		1.0f,		 {6,  0},		0.8f, {}),
		Tile::Properties("bush_fruit",			false,		true,		false,		0.5f,		 {7,  0},		0.8f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::BERRIES, 5, 10)}}})),
		Tile::Properties("amethyst_node",		true,		true,		false,		1.0f,		 {8,  0},		0.8f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::AMETHYST_CRYSTAL, 10, 15)}}})),
		Tile::Properties("emerald_node",		true,		true,		false,		1.0f,		 {9,  0},		0.8f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::EMERALD_CRYSTAL, 100, 300)}}})),
		Tile::Properties("ruby_node",			true,		true,		false,		1.0f,		 {10, 0},		0.8f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::RUBY_CRYSTAL, 1, 3)}}})),
		Tile::Properties("sapphire_node",		true,		true,		false,		1.0f,		 {11, 0},		0.8f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::SAPPHIRE_CRYSTAL, 1, 3)}}})),
		Tile::Properties("topaz_node",			true,		true,		false,		1.0f,		 {12, 0},		0.8f, Item::DropTable({{1.0f, {Item::DropData(Item::ID::TOPAZ_CRYSTAL, 1, 3)}}})),
		Tile::Properties("cactus_small",		true,		true,		false,		1.0f,		 {13, 0},		1.0f, {}),
		Tile::Properties("cactus_tall_0",		true,		true,		false,		1.0f,		 {14, 1},		1.0f, {}),
		Tile::Properties("cactus_tall_1",		true,		true,		false,		1.0f,		 {14, 0},		1.0f, {}),
		Tile::Properties("cactus_fruit",		false,		true,		false,		0.5f,		 {15, 0},		0.8f, {}),
		Tile::Properties("mossy_debris",		false,		true,		false,		0.5f,		 {0,  1},		0.8f, {}),
		Tile::Properties("rocky_debris",		false,		true,		false,		0.5f,		 {0,  2},		0.8f, {}),
		Tile::Properties("pink_flower",			false,		true,		false,		0.5f,		 {1,  1},		0.5f, {}),
		Tile::Properties("blue_flower",			false,		true,		false,		0.5f,		 {1,  2},		0.5f, {})
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

	Vivium::Vector2<int> Tile::GetAtlasIndex(const Tile::ID& id)
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
		background = other.background == Tile::ID::VOID ? background : other.background;
		foreground = other.foreground == Tile::ID::VOID ? foreground : other.foreground;
	}

	bool Tile::CompareRealTiles(const Tile& other)
	{
		// TODO: implementation
		return false;
	}

	Tile::ID& Tile::Index(int x)
	{
		switch (x) {
		case 0: return background;
		case 1: return foreground;
		default: LogWarn("Invalid index"); return background;
		}
	}

	Tile::ID Tile::GetHighestRealTile() const
	{
		if (foreground != Tile::ID::VOID) return foreground;
		if (background != Tile::ID::VOID) return background;

		return Tile::ID::VOID;
	}

	Tile::Tile()
		: background(Tile::ID::VOID), foreground(Tile::ID::VOID)
	{}

	Tile::Tile(const Tile::ID& background, const Tile::ID& foreground)
		: background(background), foreground(foreground)
	{}

	Tile::Tile(const Tile& other)
		: background(other.background), foreground(other.foreground)
	{}

	Tile::Properties::Properties(
		std::string name,
		bool isPhysical,
		bool isMineable,
		bool isPlaceable,
		float mining_time,
		Vivium::Vector2<int> atlas_index,
		float scale,
		Item::DropTable drop_data
	)
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
