#include "Item.h"

namespace Game {
	std::array<Item::Properties, (uint16_t)Item::ID::MAX> Item::m_Properties = {
		Item::Properties("void",				false,	Vivium::Vector2<int>(INT_MAX)),
		Item::Properties("amethyst crystal",	true,	Vivium::Vector2<int>(1, 0))
	};

	Item::DropData::DropData()
		: drop_table({{1.0f, Item::ID::VOID}}) // Certain chance of getting nothing
	{
		m_Sum = 1.0f;
	}

	Item::DropData::DropData(const DropTable_t& drop_table)
		: drop_table(drop_table)
	{
		for (const auto& it : drop_table) {
			m_Sum += it.first; // Sum up keys to get total weight
		}
	}

	Item::ID Item::DropData::GetRandomDrop()
	{
		// https://stackoverflow.com/questions/1761626/weighted-random-numbers
		// TODO: utils/math function for weighted sum?
		float value = Vivium::Random::GetFloat(0.0f, m_Sum);

		for (const auto& [weight, id] : drop_table) {
			if (value < weight) { return id; }

			value -= weight;
		}

		LogError("GetRandomDrop couldn't return an id, bad DropTable?");

		return Item::ID::VOID;
	}

	void Item::DropData::Write(Vivium::Serialiser& s) const
	{
		// TODO
	}

	void Item::DropData::Read(Vivium::Serialiser& s)
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
		if (!m_InitialMotionEnded) {
			float elapsed = m_Timer.GetElapsed();

			m_RemainingMovingTime -= elapsed;

			if (m_RemainingMovingTime <= 0.0f) {
				// Reset acceleration and velocity
				m_Body->acc = 0.0f;
				m_Body->vel = 0.0f;

				m_InitialMotionEnded = true;
			}
		}
	}

	FloorItem::FloorItem(const FloorItem& other)
		: m_ItemData(other.m_ItemData), m_Quad(other.m_Quad), m_Body(other.m_Body), m_Timer(other.m_Timer),
		m_RemainingMovingTime(other.m_RemainingMovingTime),
		m_InitialMotionEnded(other.m_InitialMotionEnded)
	{}

	FloorItem::FloorItem(const Item& item_data, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float> dim)
		: m_ItemData(item_data)
	{
		m_Quad = MakeRef(Vivium::Quad, pos, dim, Vivium::Random::GetFloat(0.0f, 2.0f * Vivium::Math::PI));
		m_Body = MakeRef(Vivium::Body, m_Quad, false, 1.0f, 1.0f);
		m_Body->vel = Vivium::Random::GetVector2f(500.0f);
		m_RemainingMovingTime = Vivium::Random::GetFloat(0.10f, 0.20f);
		m_Body->acc = -m_Body->vel / m_RemainingMovingTime;

		Vivium::Application::physics->Register(m_Body, 0);

		m_Timer.Start();
	}

	bool FloorItem::CheckDespawned()
	{
		return false;
	}
}
