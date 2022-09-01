#include "Inventory.h"
#include "Recipe.h"

namespace Game {
	const std::array<Inventory::Properties, (Inventory::id_base_t)Inventory::ID::MAX> Inventory::m_Properties{
		Inventory::Properties(
			// SMALL vvv
			Inventory::Slot::INV_0, 27,
			{
				{Slot::INV_0,  {0.0f, 0.0f}},
				{Slot::INV_1,  {0.0f, 0.0f}},
				{Slot::INV_2,  {0.0f, 0.0f}},
				{Slot::INV_3,  {0.0f, 0.0f}},
				{Slot::INV_4,  {0.0f, 0.0f}},
				{Slot::INV_5,  {0.0f, 0.0f}},
				{Slot::INV_6,  {0.0f, 0.0f}},
				{Slot::INV_7,  {0.0f, 0.0f}},
				{Slot::INV_8,  {0.0f, 0.0f}},
				{Slot::INV_9,  {0.0f, 0.0f}},
				{Slot::INV_10, {0.0f, 0.0f}},
				{Slot::INV_11, {0.0f, 0.0f}},
				{Slot::INV_12, {0.0f, 0.0f}},
				{Slot::INV_13, {0.0f, 0.0f}},
				{Slot::INV_14, {0.0f, 0.0f}},
				{Slot::INV_15, {0.0f, 0.0f}},
				{Slot::INV_16, {0.0f, 0.0f}},
				{Slot::INV_17, {0.0f, 0.0f}},
				{Slot::INV_18, {0.0f, 0.0f}},
				{Slot::INV_19, {0.0f, 0.0f}},
				{Slot::INV_20, {0.0f, 0.0f}},
				{Slot::INV_21, {0.0f, 0.0f}},
				{Slot::INV_22, {0.0f, 0.0f}},
				{Slot::INV_23, {0.0f, 0.0f}},
				{Slot::INV_24, {0.0f, 0.0f}},
				{Slot::INV_25, {0.0f, 0.0f}},
				{Slot::INV_26, {0.0f, 0.0f}}
			}
		), // SMALL ^^^ LARGE vvv
		Inventory::Properties(
			Inventory::Slot::INV_0, 54,
			{
				{Slot::INV_0, {0.0f, 0.0f}}
			}
		), // LARGE ^^^ SMALL_WITH_HOTBAR vvv
		Inventory::Properties(
			Inventory::Slot::INV_0, 36,
			{
				{Slot::INV_0,  {7.0f * 0, 7.0f * 0}},
				{Slot::INV_1,  {7.0f * 1, 7.0f * 0}},
				{Slot::INV_2,  {7.0f * 2, 7.0f * 0}},
				{Slot::INV_3,  {7.0f * 3, 7.0f * 0}},
				{Slot::INV_4,  {7.0f * 4, 7.0f * 0}},
				{Slot::INV_5,  {7.0f * 5, 7.0f * 0}},
				{Slot::INV_6,  {7.0f * 6, 7.0f * 0}},
				{Slot::INV_7,  {7.0f * 7, 7.0f * 0}},
				{Slot::INV_8,  {7.0f * 8, 7.0f * 0}},
				{Slot::INV_9,  {7.0f * 0, 7.0f * 1}},
				{Slot::INV_10, {7.0f * 1, 7.0f * 1}},
				{Slot::INV_11, {7.0f * 2, 7.0f * 1}},
				{Slot::INV_12, {7.0f * 3, 7.0f * 1}},
				{Slot::INV_13, {7.0f * 4, 7.0f * 1}},
				{Slot::INV_14, {7.0f * 5, 7.0f * 1}},
				{Slot::INV_15, {7.0f * 6, 7.0f * 1}},
				{Slot::INV_16, {7.0f * 7, 7.0f * 1}},
				{Slot::INV_17, {7.0f * 8, 7.0f * 1}},
				{Slot::INV_18, {7.0f * 0, 7.0f * 2}},
				{Slot::INV_19, {7.0f * 1, 7.0f * 2}},
				{Slot::INV_20, {7.0f * 2, 7.0f * 2}},
				{Slot::INV_21, {7.0f * 3, 7.0f * 2}},
				{Slot::INV_22, {7.0f * 4, 7.0f * 2}},
				{Slot::INV_23, {7.0f * 5, 7.0f * 2}},
				{Slot::INV_24, {7.0f * 6, 7.0f * 2}},
				{Slot::INV_25, {7.0f * 7, 7.0f * 2}},
				{Slot::INV_26, {7.0f * 8, 7.0f * 2}},
				{Slot::INV_27, {7.0f * 0, 7.0f * 3}},
				{Slot::INV_28, {7.0f * 1, 7.0f * 3}},
				{Slot::INV_29, {7.0f * 2, 7.0f * 3}},
				{Slot::INV_30, {7.0f * 3, 7.0f * 3}},
				{Slot::INV_31, {7.0f * 4, 7.0f * 3}},
				{Slot::INV_32, {7.0f * 5, 7.0f * 3}},
				{Slot::INV_33, {7.0f * 6, 7.0f * 3}},
				{Slot::INV_34, {7.0f * 7, 7.0f * 3}},
				{Slot::INV_35, {7.0f * 8, 7.0f * 3}},
			 }
		), // SMALL_WITH_HOTBAR ^^^ CRAFTING vvv
		Inventory::Properties(
			Inventory::Slot::CRAFT_0, 9,
			{
				{Slot::CRAFT_0,  {7.0f, 7.0f * 0}},
				{Slot::CRAFT_1,  {7.0f, 7.0f * 1}},
				{Slot::CRAFT_2,  {7.0f, 7.0f * 2}},
				{Slot::CRAFT_3,  {7.0f, 7.0f * 3}},
				{Slot::CRAFT_4,  {7.0f, 7.0f * 4}},
				{Slot::CRAFT_5,  {7.0f, 7.0f * 5}},
				{Slot::CRAFT_6,  {7.0f, 7.0f * 6}},
				{Slot::CRAFT_7,  {7.0f, 7.0f * 7}},
				{Slot::CRAFT_8,  {7.0f, 7.0f * 8}}
			}
		)
	};

	const Vivium::Vector2<float> Inventory::s_ItemCountOffsets[3] = {
			Vivium::Vector2<float>(0.0f, 0.0f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f,
			Vivium::Vector2<float>(-0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f,
			Vivium::Vector2<float>(0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f
	};

	// Max offsets used for bounds of each item (used when picking up/re-organising items)
	const float Inventory::s_MaxOffsetWidth = s_ItemCountOffsets[2].x;
	const float Inventory::s_MaxOffsetHeight = s_ItemCountOffsets[2].y;

	Inventory::Slot Inventory::m_GetNextOpenSlot()
	{
		for (slot_base_t i = m_InventoryData.start_index(); i < m_InventoryData.end_index(); i++) {
			if (m_InventoryData[i].id == Item::ID::VOID) {
				return (Slot)i;
			}
		}

		// If we reached the end of the loop, no open slots
		return Slot::INVALID;
	}

	void Inventory::m_RenderBackground(const Slot& start_slot, uint8_t length)
	{
		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		// Get properties of the inventory type we have
		Properties inven_properties = GetProperties(m_InventoryID);

		std::size_t max_count = inven_properties.inventory_size;

		Vivium::Batch batch(max_count, &layout);

		// TODO: should be computed at compile time
		// NOTE: (2, 2) is index of the inventory background
		std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray({ 2, 2 });

		for (slot_base_t slot = (slot_base_t)start_slot; slot < (slot_base_t)start_slot + length; slot++) {
			// Get coord offset to render background at
			Vivium::Vector2<float> offset = inven_properties.slot_coords.at((Slot)slot);

			float dx = inventory_pos.x + offset.x * m_InventorySpriteScale;
			float dy = inventory_pos.y + (float)Vivium::Application::height - (offset.y * m_InventorySpriteScale);

			float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_BGScale;

			batch.Submit({ dx, dy }, tile_scale * 2.0f, &coords[0]);
		}

		auto result = batch.End();

		Vivium::Renderer::Submit(result.vertex_buffer.get(), result.index_buffer.get(), m_InventoryShader, TextureManager::game_atlas->GetAtlas().get());
	}

	void Inventory::m_RenderItems(const Slot& start_slot, uint8_t length)
	{
		static const Vivium::BufferLayout item_layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		static const Vivium::BufferLayout text_layout = {
			Vivium::GLSLDataType::VEC4 // position xy, tex coords zw
		};

		// Get properties of the inventory type we have
		Properties inven_properties = GetProperties(m_InventoryID);

		// Adding one for cursor, and multiplying by 3 since can draw 3 copies of each item, or 3 digits per text
		std::size_t max_count = (inven_properties.inventory_size + 1) * 3;

		Vivium::Batch item_batch(max_count, &item_layout);
		Vivium::Batch text_batch(max_count, &text_layout);

		for (slot_base_t slot = (slot_base_t)start_slot; slot < (slot_base_t)start_slot + length; slot++) {
			Item& item = m_InventoryData.at(slot);

			// Get coord offset to render item at
			Vivium::Vector2<float> offset = inven_properties.slot_coords.at((Slot)slot);

			float dx = inventory_pos.x + offset.x * m_InventorySpriteScale;
			float dy = inventory_pos.y + (float)Vivium::Application::height - (offset.y * m_InventorySpriteScale);

			float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_ItemScale;

			m_UpdateItem(dx, dy, (tile_scale + s_MaxOffsetWidth) * 2.0f, (tile_scale + s_MaxOffsetHeight) * 2.0f, (Slot)slot);

			// If item is void, go to next item
			if (item.id == Item::ID::VOID) { continue; }

			// Draw the item
			m_RenderItem(
				&item_batch,
				item,
				Vivium::Vector2<float>(dx, dy),
				tile_scale * 2.0f
			);

			// Only draw item count for non-stackable items
			if (Item::GetIsStackable(item.id)) {
				// Get item count as text
				std::string item_count_string = std::to_string(item.count);

				// Now we need to render the text
				// Get rendering pos
				Vivium::Vector2<float> rendering_pos(dx, dy);
				// Little offset so it draws to the bottom right of the item
				// TODO: change size of text based on digits
				rendering_pos += Vivium::Vector2<float>(3.5f - (item_count_string.length() * 1.5f), -3.0f) * m_InventorySpriteScale;

				// Submit text to our batch
				m_TextObject->SetText(item_count_string);
				m_TextObject->SetPos(rendering_pos);
				text_batch.Submit(m_TextObject);
			}
		}

		// Render item at cursor
		{
			Item& cursor_item = m_InventoryData.at((uint8_t)Slot::CURSOR_0);

			if (cursor_item.id != Item::ID::VOID) {
				Vivium::Vector2<float> draw_coords = Vivium::Input::GetCursorPos();

				float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_ItemScale;

				m_RenderItem(&item_batch, cursor_item, draw_coords, tile_scale * 2.0f);

				if (Item::GetIsStackable(cursor_item.id)) {
					// Get rendering pos
					Vivium::Vector2<float> rendering_pos(draw_coords);
					rendering_pos += Vivium::Vector2<float>(1.0f, -4.0f) * m_InventorySpriteScale;

					// Get item count as text
					std::string item_count_string = std::to_string(cursor_item.count);

					// Submit text to our batch
					m_TextObject->SetText(item_count_string);
					m_TextObject->SetPos(rendering_pos);
					text_batch.Submit(m_TextObject);
				}
			}
		}

		Vivium::Batch::BatchData item_batch_data = item_batch.End();
		Vivium::Batch::BatchData text_batch_data = text_batch.End();

		if (item_batch_data.count > 0) {
			Vivium::Renderer::Submit(item_batch_data.vertex_buffer.get(), item_batch_data.index_buffer.get(), m_ItemShader, TextureManager::game_atlas->GetAtlas().get());
		}

		if (text_batch_data.count > 0) {
			Vivium::Renderer::Submit(text_batch_data.vertex_buffer.get(), text_batch_data.index_buffer.get(), m_TextShader, m_TextObject->GetTexture().get());
		}
	}

	void Inventory::m_RenderItem(Vivium::Batch* batch, const Item& item, const Vivium::Vector2<float>& pos, const float& size)
	{
		std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray(Item::GetAltasIndex(item.id));
		
		int start_index = std::min(item.count, (uint16_t)3) - 1;

		// If we're not meant to display multiple
		if (!Item::GetDisplayMultiple(item.id)) {
			batch->Submit(pos, size, &coords[0]);
		}
		else {
			// Draw a maximum of 3 copies of the item
			for (int i = start_index; i >= 0; i--) {
				Vivium::Vector2<float> item_offset = s_ItemCountOffsets[i];

				// Calculate draw coords
				float dx = pos.x + item_offset.x;
				float dy = pos.y + item_offset.y;

				batch->Submit({ dx, dy }, size, &coords[0]);
			}
		}
	}

	void Inventory::m_UpdateItem(float x, float y, float width, float height, const Slot& item_slot) // TODO: take item ref instead?
	{
		Vivium::Math::AABB item_rect(x, y, width, height);
		Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();

		Vivium::Input::State lmb_state = Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1);
		Vivium::Input::State rmb_state = Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_2);

		// TODO: not perfect
		// TODO: some sort of pending system for splitting items
		// We have clicked on this item
		if (item_rect.Contains(cursor_pos)) {
			if (lmb_state == Vivium::Input::State::PRESS) {
				// Get cursor item
				Item& cursor_item = m_InventoryData.at((slot_base_t)Slot::CURSOR_0);

				// If we have nothing in the cursor slot
				if (cursor_item.id == Item::ID::VOID) {
					// Move our item to that cursor slot
					Item& our_item = m_InventoryData.at((slot_base_t)item_slot);
					cursor_item.id = our_item.id;
					cursor_item.count = our_item.count;

					// Delete our item
					our_item.id = Item::ID::VOID;
					our_item.count = 0;
				}
			}
			else if (lmb_state == Vivium::Input::State::RELEASE) {
				// Get cursor item
				Item& cursor_item = m_InventoryData.at((slot_base_t)Slot::CURSOR_0);

				// If cursor item is NOT nothing
				if (cursor_item.id != Item::ID::VOID) {
					Item& our_item = m_InventoryData.at((slot_base_t)item_slot);

					// If our item counts are the same and our counts are less than the stack limit, stack the items
					if (cursor_item.id == our_item.id && cursor_item.count + our_item.count <= Item::STACK_LIMIT) {
						our_item.count += cursor_item.count;

						// Delete cursor item
						cursor_item.id = Item::ID::VOID;
						cursor_item.count = 0;
					}
					// If our items are different, or the sum of our counts are above the stack limit, swap items
					else if (cursor_item.id != our_item.id) {
						std::swap(cursor_item, our_item);
					}
				}
			}

			// Spreading items if holding down rmb
			/*
			if (rmb_state == Vivium::Input::State::PRESS || rmb_state == Vivium::Input::State::HOLD) {
				// Get cursor item
				Item& cursor_item = m_InventoryData.at((uint8_t)Slot::CURSOR_0);

				// If cursor item is NOT nothing
				if (cursor_item.id != Item::ID::VOID) {
					Item& our_item = m_InventoryData.at((uint8_t)item_slot);

					// If our item ids are the same or it is an empty slot, and we can add one item to without exceeding the stack limit
					if ((cursor_item.id == our_item.id || our_item.id == Item::ID::VOID) && our_item.count + 1 <= Item::STACK_LIMIT) {
						++our_item.count;
						--cursor_item.count;

						our_item.id = cursor_item.id;

						if (cursor_item.count == 0) {
							cursor_item.id = Item::ID::VOID;
						}
					}
				}
			}
			*/
		}
	}

	void Inventory::m_UpdateFloorItems(const Vivium::Vector2<float>& player_pos, World* world)
	{
		Vivium::Vector2<float> player_scale_pos = player_pos / World::PIXEL_SCALE;
		Vivium::Vector2<int> region_pos = world->GetRegionIndex(Vivium::Vector2<int>((player_scale_pos).floor()));

		int count = 0;

		for (int region_y = region_pos.y - 1; region_y <= region_pos.y + 1; region_y++) {
			for (int region_x = region_pos.x - 1; region_x <= region_pos.x + 1; region_x++) {
				std::vector<FloorItem>* floor_items = world->GetFloorItems(Vivium::Vector2<int>(region_x, region_y));

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

	Inventory::Properties Inventory::GetProperties(const Inventory::ID& id)
	{
		return m_Properties[(id_base_t)id];
	}

	Vivium::Shader* Inventory::m_InventoryShader = nullptr;
	Vivium::Shader* Inventory::m_TextShader = nullptr;
	Vivium::Shader* Inventory::m_ItemShader = nullptr;
	Vivium::Text* Inventory::m_TextObject = nullptr;

	void Inventory::Init()
	{
		m_InventoryShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
		
		m_TextShader = new Vivium::Shader("text_vertex", "text_frag");
		m_TextShader->Bind();
		m_TextShader->SetUniform3f("u_TextColor", Vivium::RGBColor::WHITE);

		m_ItemShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
		m_TextObject = new Vivium::Text("", {0, 0}, Vivium::Text::Alignment::LEFT, 0.30f); // TODO: pretty ugly, later use font or something
	}

	void Inventory::Terminate()
	{
		delete m_InventoryShader;
		delete m_TextObject;
		delete m_TextShader;
		delete m_ItemShader;
	}

	bool Inventory::AddItem(const Item& item)
	{
		bool itemIsStackable = Item::GetIsStackable(item.id);

		// If our item is stackable, search the inventory first to look if we already have a place to stack the item
		if (itemIsStackable) {
			for (Item& inventory_item : m_InventoryData) {
				// If our items have the same id
				if (item.id == inventory_item.id) {
					// If the sum of our counts is less than the stack limit
					if (item.count + inventory_item.count <= Item::STACK_LIMIT) {
						// Stack the items together
						inventory_item.count += item.count;

						m_ChangeItemCount(item.id, item.count);

						// Successful in adding the item
						return true;
					}
				}
			}
		}

		// Couldn't find anywhere to stack the item if we're here, so get next open slot
		Slot open_slot = m_GetNextOpenSlot();

		// Will return invalid if there is no open slot
		if (open_slot != Slot::INVALID) {
			// Set the open slot to our item
			m_InventoryData[(slot_base_t)open_slot] = item;

			m_ChangeItemCount(item.id, item.count);

			// Return that item was added to inventory
			return true;
		}

		// If we reached the end of the loop, we haven't found anywhere to stack the item, so return that we weren't successful
		return false;
	}

	const Item& Inventory::GetItem(const Slot& slot)
	{
		return m_InventoryData.at((uint8_t)slot);
	}

	// TODO: same thing?
	void Inventory::Update(const Slot& start_slot, uint8_t length, const Vivium::Vector2<float>& player_pos, World* world)
	{
		m_UpdateFloorItems(player_pos, world);
	}

	void Inventory::Update(const Vivium::Vector2<float>& player_pos, World* world)
	{
		m_UpdateFloorItems(player_pos, world);
	}

	std::vector<Item> Inventory::GetItems(const Slot& start_slot, slot_base_t length)
	{
		std::vector<Item> items(length);

		for (slot_base_t i = 0; i < length; i++) {
			items[i] = m_InventoryData.at((slot_base_t)start_slot + i);
		}

		return items;
	}

	void Inventory::Render(const Slot& start_slot, uint8_t length)
	{
		m_RenderBackground(start_slot, length);
		m_RenderItems(start_slot, length);
	}

	void Inventory::Render()
	{
		Properties props = GetProperties(m_InventoryID);

		m_RenderBackground(props.start_slot, props.inventory_size);
		m_RenderItems(props.start_slot, props.inventory_size);
	}

	Inventory::Inventory()
		: m_InventoryData({}), m_InventoryID(ID::MAX)
	{}

	void Inventory::m_UpdateItemCounts()
	{
		m_ItemCounts.clear();

		// TODO: should be const ref but i think i haven't implemented that iterator yet
		for (Item& item : m_InventoryData) {
			m_ItemCounts.insert({ item.id, item.count });
		}
	}

	void Inventory::m_ChangeItemCount(const Item::ID& id, int change)
	{
		auto it = m_ItemCounts.find(id);

		if (it == m_ItemCounts.end()) {
			if (change < 0) { LogWarn("Attempted to subtract item count when item didn't exist"); }
			else {
				m_ItemCounts.insert({ id, change });
			}
		}
		else {
			it->second += change;

			// Clamp to 0 (although big error must've happened if we go negative)
			if (it->second < 0) {
				it->second = 0;
			}
		}
	}

	void Inventory::SetItems(const std::vector<Item>& items, const Slot& slot)
	{
		for (uint8_t i = 0; i < items.size(); i++) {
			m_InventoryData.at((uint8_t)slot + i) = items.at(i);
		}

		m_UpdateItemCounts();
	}

	int Inventory::GetItemCount(const Item::ID& id) const
	{
		auto it = m_ItemCounts.find(id);

		if (it == m_ItemCounts.end()) { return 0; }
		else { return it->second; }
	}

	Inventory::Inventory(const ID& inventory_id)
		: m_InventoryID(inventory_id), m_InventoryData(inventory_id)
	{}

	Inventory::~Inventory() {}

	Inventory::Data& Inventory::GetData()
	{
		return m_InventoryData;
	}

	std::vector<bool> Inventory::AddItems(const std::vector<Item>& items)
	{
		if (items.empty()) return {};

		std::size_t items_size = items.size();
		std::vector<bool> return_values(items_size);

		for (std::size_t i = 0; i < items_size; i++) {
			return_values[i] = AddItem(items[i]);
		}

		return return_values;
	}

	void Inventory::Write(Vivium::Serialiser& s) const
	{
		// TODO: serialiser could check if type is an enum class?
		typedef std::unordered_map<std::underlying_type<Item::ID>::type, int> counts_streamable_t;

		// Write inventory type
		s.Write((id_base_t)m_InventoryID);
		// Write all items
		s.Write(m_InventoryData);
		// Write item counts
		s.Write(*(counts_streamable_t*)&m_ItemCounts);
	}

	void Inventory::Read(Vivium::Serialiser& s)
	{
		typedef std::unordered_map<std::underlying_type<Item::ID>::type, int> counts_streamable_t;

		// Read in inventory type
		s.Read((id_base_t*)&m_InventoryID);
		// Read all items
		s.Read(&m_InventoryData);
		// Read item counts
		s.Read((counts_streamable_t*)&m_ItemCounts);
	}

	Inventory::Properties::Properties(
		const Slot& start_slot, const slot_base_t& inventory_size,
		const std::unordered_map<Slot, Vivium::Vector2<float>>& slot_coords
	)
		: start_slot(start_slot), inventory_size(inventory_size), slot_coords(slot_coords)
	{}

	Inventory::Data::Data()
		: m_Data(), m_Size(0), m_StartIndex(0)
	{}

	Inventory::Data::Data(const ID& id)
	{
		Inventory::Properties properties = GetProperties(id);

		m_StartIndex = (uint16_t)properties.start_slot;
		m_Size = properties.inventory_size;
	}

	Inventory::Data::Data(const Data& other)
		: m_Data(other.m_Data), m_Size(other.m_Size), m_StartIndex(other.m_StartIndex)
	{}

	Inventory::Data::Data(Data && other) noexcept
		: m_Data(std::move(other.m_Data)), m_Size(std::move(other.m_Size)), m_StartIndex(std::move(other.m_StartIndex))
	{}

	Inventory::Data::Iterator Inventory::Data::begin()
	{
		return Iterator(&m_Data[m_StartIndex]);
	}
	Inventory::Data::Iterator Inventory::Data::end()
	{
		return Iterator(&m_Data[m_StartIndex + m_Size]);
	}

	void Inventory::Data::Write(Vivium::Serialiser& s) const
	{
		s.Write(m_Data);
		s.Write(m_Size);
		s.Write(m_StartIndex);
	}

	void Inventory::Data::Read(Vivium::Serialiser& s)
	{
		s.Read(&m_Data);
		s.Read(&m_Size);
		s.Read(&m_StartIndex);
	}
}
