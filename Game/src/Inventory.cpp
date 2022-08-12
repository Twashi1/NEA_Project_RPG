#include "Inventory.h"

namespace Game {
	const std::array<Inventory::Properties, (uint8_t)Inventory::ID::MAX> Inventory::m_Properties{
		Inventory::Properties(Inventory::Slot::INV_0, 27, {0, 0}, {3, 1}, {114.0f, 46.0f}), // SMALL
		Inventory::Properties(Inventory::Slot::INV_0, 54, {0, 2}, {3, 4}, {114.0f, 90.0f}), // LARGE
		Inventory::Properties(Inventory::Slot::HOTBAR_0, 9, INT_MAX, INT_MAX, 0.0f)  // TODO: HOTBAR
	};

	Inventory::Properties Inventory::GetProperties(const Inventory::ID& id)
	{
		return m_Properties[(uint8_t)id];
	}

	Vivium::Shader* Inventory::m_InventoryShader;
	Vivium::TextureAtlas* Inventory::m_InventoryAtlas;

	void Inventory::Init()
	{
		m_InventoryAtlas = new Vivium::TextureAtlas("inventory.png", { 32, 32 });
		m_InventoryShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
	}

	void Inventory::Terminate()
	{
		delete m_InventoryAtlas;
		delete m_InventoryShader;
	}

	bool Inventory::AddItem(const Item& item)
	{
		// TODO: hotbar test
		bool itemIsStackable = Item::GetIsStackable(item.id);

		Properties properties = Inventory::GetProperties(m_InventoryID);

		for (uint8_t i = (uint8_t)properties.start_slot; i < (uint8_t)properties.start_slot + properties.inventory_size; i++) {
			Item& inventory_item = m_InventoryData[i];

			// If our items have the same id
			if (item.id == inventory_item.id) {
				// If both our current item is stackable, and their item is stackable
				if (itemIsStackable && Item::GetIsStackable(inventory_item.id)) {
					// If the sum of our counts is less than the stack limit
					if (item.count + inventory_item.count <= Item::STACK_LIMIT) {
						// Stack the items together
						inventory_item.count += item.count;

						// Successful in adding the item
						return true;
					}
				}
			}
			else {
				// If the inventory item is void (empty slot), replace it with our item
				if (inventory_item.id == Item::ID::VOID) {
					inventory_item = item;

					// Successful in adding item
					return true;
				}
			}
		}

		// If we reached the end of the loop, we haven't found anywhere to stack the item, so return that we weren't successful
		return false;
	}

	void Inventory::SetPos(const Vivium::Vector2<float>& new_pos)
	{
		m_InventoryQuad->SetCenter(new_pos);
	}

	Vivium::Vector2<float> Inventory::GetPos() const
	{
		return m_InventoryQuad->GetCenter();
	}

	void Inventory::Render()
	{
		Vivium::Renderer::Submit(m_InventoryQuad, m_InventoryShader, m_InventoryAtlas->GetAtlas().get());
	}

	Inventory::Inventory(const ID& inventory_id)
		: m_InventoryID(inventory_id)
	{
		Properties properties = Inventory::GetProperties(inventory_id);
		m_InventoryQuad = new Vivium::Quad(Vivium::Vector2<float>(0.0f), properties.sprite_size * m_InventorySpriteScale);

		m_InventoryAtlas->Set(m_InventoryQuad, properties.top_left_index, properties.bottom_right_index);
	}

	Inventory::~Inventory()
	{
		delete m_InventoryQuad;
	}

	Inventory::Properties::Properties(const Slot& start_slot, const uint8_t& inventory_size, const Vivium::Vector2<int>& top_left_index, const Vivium::Vector2<int>& bottom_right_index, const Vivium::Vector2<float>& sprite_size)
		: start_slot(start_slot), inventory_size(inventory_size),
		top_left_index(top_left_index), bottom_right_index(bottom_right_index),
		sprite_size(sprite_size)
	{}
}
