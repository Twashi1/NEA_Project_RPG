#include "Inventory.h"

namespace Game {
	const std::array<Inventory::Properties, (uint8_t)Inventory::ID::MAX> Inventory::m_Properties{
		Inventory::Properties(
			Inventory::Slot::INV_0, 27, {0, 0}, {3, 1}, {114.0f, 46.0f},
			{
				{Slot::INV_0,  {18.3f,  15.4f}},
				{Slot::INV_1,  {18.3f + 9.8f,  15.4f}},
				{Slot::INV_2,  {18.3f + 9.8f * 2,  15.4f}},
				{Slot::INV_3,  {18.3f + 9.8f * 3,  15.4f}},
				{Slot::INV_4,  {18.3f + 9.8f * 4, 15.4f}},
				{Slot::INV_5,  {18.3f + 9.8f * 5, 15.4f}},
				{Slot::INV_6,  {18.3f + 9.8f * 6, 15.4f}},
				{Slot::INV_7,  {18.3f + 9.8f * 7, 15.4f}},
				{Slot::INV_8,  {18.3f + 9.8f * 8, 15.4f}}, // TODO: other tex coords, also relative
				{Slot::INV_9,  {10.0f,  40.0f}},
				{Slot::INV_10, {20.0f,  40.0f}},
				{Slot::INV_11, {30.0f,  40.0f}},
				{Slot::INV_12, {40.0f,  40.0f}},
				{Slot::INV_13, {50.0f, 40.0f}},
				{Slot::INV_14, {60.0f, 40.0f}},
				{Slot::INV_15, {70.0f, 40.0f}},
				{Slot::INV_16, {80.0f, 40.0f}},
				{Slot::INV_17, {90.0f, 40.0f}},
				{Slot::INV_18, {10.0f,  60.0f}},
				{Slot::INV_19, {20.0f,  60.0f}},
				{Slot::INV_20, {30.0f,  60.0f}},
				{Slot::INV_21, {40.0f,  60.0f}},
				{Slot::INV_22, {50.0f, 60.0f}},
				{Slot::INV_23, {60.0f, 60.0f}},
				{Slot::INV_24, {70.0f, 60.0f}},
				{Slot::INV_25, {80.0f, 60.0f}},
				{Slot::INV_26, {90.0f, 60.0f}}
			}
		), // SMALL
		Inventory::Properties(
			Inventory::Slot::INV_0, 54, {0, 2}, {3, 4}, {114.0f, 90.0f},
			{
				{Slot::INV_0, {20.0f, 20.0f}}
			}
		), // LARGE
		Inventory::Properties(
			Inventory::Slot::HOTBAR_0, 9, INT_MAX, INT_MAX, 0.0f,
			{}
		)  // TODO: HOTBAR
	};

	void Inventory::m_RenderItems()
	{
		static const Vivium::Vector2<float> itemCountOffsets[3] = {
			Vivium::Vector2<float>( 0.0f, 0.0f) * World::PIXEL_SCALE * 0.5f,
			Vivium::Vector2<float>(-0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f,
			Vivium::Vector2<float>( 0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f
		};

		Properties inven_properties = GetProperties(m_InventoryID);

		std::vector<float> coords;
		std::vector<unsigned short> indexCoords;

		unsigned short count = 0;
		unsigned int max_count = inven_properties.inventory_size;

		coords.reserve(16 * max_count * 3);
		indexCoords.reserve(6 * max_count * 3);

		// TODO: make InventoryData_t iterable
		for (uint8_t slot = (uint8_t)inven_properties.start_slot; slot < (uint8_t)inven_properties.start_slot + inven_properties.inventory_size; slot++) {
			Item& item = m_InventoryData.at(slot);

			// If item is void, go to next item
			if (item.id == Item::ID::VOID) { continue; }

			// TODO: draw multiple copies of texture for multiple items
			// TODO: draw text to represent count
			// Get coord offset to render item at
			Vivium::Vector2<float> offset = inven_properties.slot_coords.at((Slot)slot);

			for (int i = std::min(item.count, (uint16_t)3) - 1; i >= 0; i--) {
				Vivium::Vector2<float> itemOffset = itemCountOffsets[i];

				std::array<float, 4>& faces = World::m_ItemsTextureCoords[(uint16_t)item.id];

				float dx = m_InventoryQuad->Left() + (offset.x * m_InventorySpriteScale);
				float dy = m_InventoryQuad->Top() - (offset.y * m_InventorySpriteScale); // TODO: inverse?

				// TODO: base on sprite scale
				float tile_scale = World::PIXEL_SCALE * 0.5f - 5.5f;

				std::array<float, 16> this_coords =
				{
					dx - tile_scale + itemOffset.x, dy - tile_scale + itemOffset.y, faces[0], faces[1], // bottom left
					dx + tile_scale + itemOffset.x, dy - tile_scale + itemOffset.y, faces[2], faces[1], // bottom right
					dx + tile_scale + itemOffset.x, dy + tile_scale + itemOffset.y, faces[2], faces[3], // top right
					dx - tile_scale + itemOffset.x, dy + tile_scale + itemOffset.y, faces[0], faces[3]  // top left
				};

				int stride = count * 4;

				std::array<unsigned short, 6> this_indices =
				{
					0 + stride,
					1 + stride,
					2 + stride,
					2 + stride,
					3 + stride,
					0 + stride,
				};


				indexCoords.insert(indexCoords.end(), this_indices.begin(), this_indices.end());
				coords.insert(coords.end(), this_coords.begin(), this_coords.end());

				count++;
			}
		}

		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		if (count > 0) {
			Vivium::VertexBuffer vb(coords, layout);
			Vivium::IndexBuffer ib(indexCoords);

			// TODO: use different shader
			Vivium::Renderer::Submit(&vb, &ib, m_InventoryShader, World::m_ItemsAtlas->GetAtlas().get());
		}
	}

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

	void Inventory::Update(const Vivium::Vector2<float>& player_pos, World& world)
	{
		Vivium::Vector2<float> player_scale_pos = player_pos / World::PIXEL_SCALE;
		Vivium::Vector2<int> region_pos = world.GetRegionIndex(Vivium::Vector2<int>((player_scale_pos).floor()));

		int count = 0;

		for (int region_y = region_pos.y - 1; region_y <= region_pos.y + 1; region_y++) {
			for (int region_x = region_pos.x - 1; region_x <= region_pos.x + 1; region_x++) {
				std::vector<FloorItem>* floor_items = world.GetFloorItems(Vivium::Vector2<int>(region_x, region_y));

				// If no floor items, just go next
				if (floor_items == nullptr) { continue; }

				auto it = floor_items->begin();
				while (it != floor_items->end()) {

					Vivium::Vector2<float> item_pos = it->m_Quad->GetCenter();

					float sqr_distance = item_pos.sqr_distance(player_pos);

					// Within pickup range
					if (sqr_distance <= FloorItem::PICKUP_RANGE * FloorItem::PICKUP_RANGE) {
						// Add item to inventory
						if (AddItem(it->GetItemData())) {
							// Item was added to inventory, so erase the floor item
							it = floor_items->erase(it);
						}
						else {
							// Just increment iterator
							++it;
						}
					}
					else {
						// Within pull/magnet range
						if (sqr_distance <= FloorItem::MAGNET_RANGE * FloorItem::MAGNET_RANGE) {
							// Get direction vector from floor item to player
							Vivium::Vector2<float> direction = (player_pos - item_pos).normalise();
							// Set acceleration towards us
							it->acceleration = direction * FloorItem::MAGNET_SPEED;
							it->m_InitialMotionEnded = false;
							it->m_RemainingMovingTime = -1.0f;

							it->Update();
						}
						// TODO: acceleration/velocity with floor items is weird, maybe some automatic friction system
						// Not within pull/magnet range, so remove any acceleration/velocity
						else {
							if (it->m_RemainingMovingTime <= -1.0f) {
								// Reset all movement
								it->acceleration = 0.0f;
								it->velocity = 0.0f;

								it->m_InitialMotionEnded = true;
								it->m_RemainingMovingTime = 0.0f;
							}

							it->Update();
						}

						++it;
					}
				}
			}
		}
	}

	void Inventory::Render()
	{
		Vivium::Renderer::Submit(m_InventoryQuad, m_InventoryShader, m_InventoryAtlas->GetAtlas().get());

		m_RenderItems();
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

	Inventory::Properties::Properties(
		const Slot& start_slot, const uint8_t& inventory_size,
		const Vivium::Vector2<int>& top_left_index, const Vivium::Vector2<int>& bottom_right_index,
		const Vivium::Vector2<float>& sprite_size,
		const std::unordered_map<Slot, Vivium::Vector2<float>>& slot_coords
	)
		: start_slot(start_slot), inventory_size(inventory_size),
		top_left_index(top_left_index), bottom_right_index(bottom_right_index),
		sprite_size(sprite_size), slot_coords(slot_coords)
	{}
}