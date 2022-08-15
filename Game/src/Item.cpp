#include "Item.h"
#include "Inventory.h"

namespace Game {
	std::array<Item::Properties, (uint16_t)Item::ID::MAX> Item::m_Properties = {
		Item::Properties("void",				false,	Vivium::Vector2<int>(INT_MAX)),
		Item::Properties("amethyst crystal",	true,	Vivium::Vector2<int>(1, 0)),
		Item::Properties("emerald crystal",		true,	Vivium::Vector2<int>(1, 1)),
		Item::Properties("ruby crystal",		true,	Vivium::Vector2<int>(1, 2)),
		Item::Properties("sapphire crystal",	true,	Vivium::Vector2<int>(1, 3)),
		Item::Properties("topaz crystal",		true,	Vivium::Vector2<int>(1, 4)),
		Item::Properties("log", 				true,	Vivium::Vector2<int>(0, 5))
	};

	Item::DropTable::DropTable()
		: drop_table({{1.0f, Item::DropData(Item::ID::VOID, 0, 0)}}) // Certain chance of getting nothing
	{
		m_Sum = 1.0f;
	}

	Item::DropTable::DropTable(const DropTableMap_t& drop_table)
		: drop_table(drop_table)
	{
		for (const auto& it : drop_table) {
			m_Sum += it.first; // Sum up keys to get total weight
		}
	}

	Item Item::DropTable::GetRandomDrop()
	{
		// https://stackoverflow.com/questions/1761626/weighted-random-numbers
		// TODO: utils/math function for weighted sum?
		float value = Vivium::Random::GetFloat(0.0f, m_Sum);

		for (const auto& [weight, drop_data] : drop_table) {
			if (value < weight) {
				// If id is void, return no item
				if (drop_data.id == Item::ID::VOID) return Item(Item::ID::VOID, 0);

				// Get count of object
				uint16_t count = Vivium::Random::GetInt(drop_data.min_count, drop_data.max_count);

				// Return item
				return Item(drop_data.id, count);
			}

			value -= weight;
		}

		LogError("GetRandomDrop couldn't return an id, bad DropTable?");

		return Item(Item::ID::VOID, 0);
	}

	void Item::DropTable::Write(Vivium::Serialiser& s) const
	{
		// TODO
	}

	void Item::DropTable::Read(Vivium::Serialiser& s)
	{
		// TODO
	}

	Item::Properties::Properties(std::string name, bool isStackable, Vivium::Vector2<int> atlas_index)
		: name(name), isStackable(isStackable), atlas_index(atlas_index)
	{}

	void Item::Properties::Write(Vivium::Serialiser& s) const
	{
		s.Write(name);
		s.Write(isStackable);
		s.Write(atlas_index);
	}

	void Item::Properties::Read(Vivium::Serialiser& s)
	{
		s.Read(&name);
		s.Read(&isStackable);
		s.Read(&atlas_index);
	}
	
	Item::Properties Item::GetProperties(const Item::ID& id)
	{
		return m_Properties[(uint16_t)id];
	}

	std::string Item::GetName(const Item::ID& id)
	{
		return m_Properties[(uint16_t)id].name;
	}

	bool Item::GetIsStackable(const Item::ID& id)
	{
		return m_Properties[(uint16_t)id].isStackable;
	}

	Vivium::Vector2<int> Item::GetAltasIndex(const Item::ID& id)
	{
		return m_Properties[(uint16_t)id].atlas_index;
	}

	Item::Item(const Item::ID& id, const uint16_t& count)
		: id(id), count(count)
	{}

	Item::Item()
		: id(Item::ID::VOID), count(0)
	{}

	Item::Item(const Item& other)
		: id(other.id), count(other.count)
	{}

	Item::Item(Item&& other) noexcept
		: id(std::move(other.id)), count(std::move(other.count))
	{}

	Item& Item::operator=(const Item & other)
	{
		id = other.id;
		count = other.count;

		return *this;
	}

	Vivium::Shader* FloorItem::floor_shader = nullptr;

	void FloorItem::Init()
	{
		floor_shader = new Vivium::Shader("floor_item_vertex", "floor_item_frag");
	}

	void FloorItem::Terminate()
	{
		delete floor_shader;
	}

	const Item& FloorItem::GetItemData() const
	{
		return m_ItemData;
	}

	const Ref(Vivium::Quad) FloorItem::GetQuad() const
	{
		return m_Quad;
	}

	void FloorItem::Update()
	{
		float elapsed = m_Timer.GetElapsed();

		velocity += acceleration * elapsed;
		m_Quad->SetCenter(m_Quad->GetCenter() + velocity * elapsed);

		if (!m_InitialMotionEnded) {
			m_RemainingMovingTime -= elapsed;

			if (m_RemainingMovingTime <= 0.0f) {
				// Reset acceleration and velocity
				acceleration = 0.0f;
				velocity = 0.0f;

				m_InitialMotionEnded = true;
			}
		}
	}

	FloorItem::FloorItem(const FloorItem& other)
		: m_ItemData(other.m_ItemData), m_Quad(other.m_Quad),
		acceleration(other.acceleration), velocity(other.velocity),
		m_Timer(other.m_Timer),
		m_RemainingMovingTime(other.m_RemainingMovingTime),
		m_InitialMotionEnded(other.m_InitialMotionEnded)
	{}

	FloorItem::FloorItem(const Item& item_data, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float> dim)
		: m_ItemData(item_data)
	{
		m_Quad = MakeRef(Vivium::Quad, pos, dim, Vivium::Random::GetFloat(0.0f, 2.0f * Vivium::Math::PI));
		velocity = Vivium::Random::GetVector2f(100.0f);
		m_RemainingMovingTime = Vivium::Random::GetFloat(0.50f, 0.70f);
		acceleration = -velocity / m_RemainingMovingTime;

		m_Timer.Start();
	}

	bool FloorItem::CheckDespawned()
	{
		return false;
	}

	Item::DropData::DropData(const Item::ID& id, const uint16_t& min_count, const uint16_t& max_count)
		: id(id), min_count(min_count), max_count(max_count)
	{}

	Item::DropData::DropData(const Item::ID& id, const uint16_t & count)
		: id(id), min_count(count), max_count(count)
	{}

	void Item::DropData::Write(Vivium::Serialiser & s) const
	{
		// TODO
	}

	void Item::DropData::Read(Vivium::Serialiser& s)
	{
		// TODO
	}
}
