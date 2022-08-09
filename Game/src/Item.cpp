#include "Item.h"

namespace Game {
	ItemDropData::ItemDropData()
		: drop_table({{1.0f, Item::ID::VOID}}) // Certain chance of getting nothing
	{}

	ItemDropData::ItemDropData(const DropTable_t& drop_table)
		: drop_table(drop_table)
	{}

	void ItemDropData::Write(Vivium::Serialiser& s) const
	{
	}

	void ItemDropData::Read(Vivium::Serialiser& s)
	{
	}
}
