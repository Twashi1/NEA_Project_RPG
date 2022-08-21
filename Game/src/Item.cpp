#include "Item.h"
#include "Inventory.h"

namespace Game {
	std::array<Item::Properties, (uint16_t)Item::ID::MAX> Item::m_Properties = {
		Item::Properties("Void",				false,	Vivium::Vector2<int>(INT_MAX)),

		Item::Properties("Amethyst Crystal",	true,	Vivium::Vector2<int>(1, 0)),
		Item::Properties("Emerald Crystal",		true,	Vivium::Vector2<int>(1, 1)),
		Item::Properties("Ruby Crystal",		true,	Vivium::Vector2<int>(1, 2)),
		Item::Properties("Sapphire Crystal",	true,	Vivium::Vector2<int>(1, 3)),
		Item::Properties("Topaz Crystal",		true,	Vivium::Vector2<int>(1, 4)),

		Item::Properties("Amethyst Pickaxe",	false,	Vivium::Vector2<int>(2, 0)),
		Item::Properties("Emerald Pickaxe",		false,	Vivium::Vector2<int>(2, 1)),
		Item::Properties("Ruby Pickaxe",		false,	Vivium::Vector2<int>(2, 2)),
		Item::Properties("Sapphire Pickaxe",	false,	Vivium::Vector2<int>(2, 3)),
		Item::Properties("Topaz Pickaxe",		false,	Vivium::Vector2<int>(2, 4)),

		Item::Properties("Log", 				true,	Vivium::Vector2<int>(0, 5))
	};

	Item::DropTable::DropTable()
		: drop_table({{1.0f, {}}}) // Certain chance of getting nothing
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

	std::vector<Item> Item::DropTable::GetRandomDrop()
	{
		// https://stackoverflow.com/questions/1761626/weighted-random-numbers
		// TODO: utils/math function for weighted sum?
		float value = Vivium::Random::GetFloat(0.0f, m_Sum);

		for (const auto& [weight, drop_data] : drop_table) {
			if (value < weight) {
				// If no drops in drop data, return empty vector
				if (drop_data.empty()) return {};

				// Otherwise, create drop list
				std::vector<Item> item_drops;
				item_drops.resize(drop_data.size());

				// Iterate over all drops in vector
				for (int i = 0; i < drop_data.size(); i++) {
					// Get count of object
					uint16_t count = Vivium::Random::GetInt(drop_data[i].min_count, drop_data[i].max_count);
					// Add item to item drops
					item_drops[i] = Item(drop_data[i].id, count);
				}

				return item_drops;
			}

			value -= weight;
		}

		LogError("GetRandomDrop couldn't return an id, bad DropTable?");

		return {};
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

	void Item::Write(Vivium::Serialiser& s) const
	{
		s.Write((uint16_t)id);
		s.Write(count);
	}

	void Item::Read(Vivium::Serialiser& s)
	{
		s.Read((uint16_t*)&id);
		s.Read(&count);
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

	// Leave everything uninitialised
	FloorItem::FloorItem() {}

	void FloorItem::Write(Vivium::Serialiser& s) const
	{
		s.Write(m_ItemData);
		s.Write(m_Quad->GetCenter());
		s.Write(m_Quad->GetDim());
		s.Write(m_Quad->GetAngle());
		s.Write(velocity);
		s.Write(acceleration);
		s.Write(m_RemainingMovingTime);
		s.Write(m_InitialMotionEnded);
	}

	void FloorItem::Read(Vivium::Serialiser& s)
	{
		s.Read(&m_ItemData);
		Vivium::Vector2<float> pos, dim;
		float angle;
		s.Read(&pos);
		s.Read(&dim);
		s.Read(&angle);

		m_Quad = MakeRef(Vivium::Quad, pos, dim, angle);

		s.Read(&velocity);
		s.Read(&acceleration);
		s.Read(&m_RemainingMovingTime);
		s.Read(&m_InitialMotionEnded);
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
