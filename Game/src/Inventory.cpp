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
		) // CRAFTING ^^^
	};

	const Vivium::Vector2<float> Inventory::s_ItemCountOffsets[3] = {
		// Center
		Vivium::Vector2<float>(0.0f, 0.0f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f,
		// Left
		Vivium::Vector2<float>(-0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f,
		// Right
		Vivium::Vector2<float>(0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f
	};

	// Max offsets used for bounds of each item (used when picking up/re-organising items)
	// NOTE: if updating item offsets, must update these to be relevant
	const float Inventory::s_MaxOffsetWidth = s_ItemCountOffsets[2].x;
	const float Inventory::s_MaxOffsetHeight = s_ItemCountOffsets[2].y;

	Inventory::Slot Inventory::m_GetNextOpenSlot()
	{
		// Iterate the valid part of the inventory data
		for (slot_base_t i = m_InventoryData.start_index(); i < m_InventoryData.end_index(); i++) {
			// If empty slot
			if (m_InventoryData[i].id == Item::ID::VOID) {
				// Return that index
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

		// Maximum amount of background sprites we're rendering
		std::size_t max_count = inven_properties.inventory_size;

		// Construct batch for storing vertex data
		Vivium::Batch batch(max_count, &layout);

		// TODO: precompute
		// Default inventory background
		// TODO: better name
		std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray({ 2, 2 });
		// Selected slot inventory background
		std::array<float, 8> selected_coords = TextureManager::game_atlas->GetCoordsArray({ 2, 3 });

		// Iterate range given
		for (slot_base_t slot = (slot_base_t)start_slot; slot < (slot_base_t)start_slot + length; slot++) {
			// Get coord offset to render background at
			Vivium::Vector2<float> offset = inven_properties.slot_coords.at((Slot)slot);

			// X draw coordinate
			float dx = inventory_pos->x + offset.x * m_InventorySpriteScale;
			// Y draw coordinate
			float dy = inventory_pos->y + (float)Vivium::Application::height - (offset.y * m_InventorySpriteScale);

			// Size we're using for the inventory slots
			float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_BGScale;

			// Very hacky way to get the correct texture coordinates
			// TODO: use same system in CraftingInventory
			// Default to the normal slot background
			float* coords_ptr = &coords[0];

			// If we're on the selected slot
			if (slot == (slot_base_t)m_SelectedSlot) {
				// Switch to selected slot coordinates
				coords_ptr = &selected_coords[0];
				// And add a little to the size
				tile_scale += 2.0f; // TODO: Magic number
			}

			batch.Submit({ dx, dy }, tile_scale * 2.0f, coords_ptr);
		}

		Vivium::Batch::RenderData result = batch.End();

		if (result) {
			Vivium::Renderer::Submit(result.vertex_buffer.get(), result.index_buffer.get(), m_InventoryShader, TextureManager::game_atlas->GetAtlas().get());
		}
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

		// Constructing batches for item and text
		Vivium::Batch item_batch(max_count, &item_layout);
		Vivium::Batch text_batch(max_count, &text_layout);

		// Iterate region of inventory defined by parameters
		for (slot_base_t slot = (slot_base_t)start_slot; slot < (slot_base_t)start_slot + length; slot++) {
			Item& item = m_InventoryData.at(slot);

			// Get coord offset to render item at
			Vivium::Vector2<float> offset = inven_properties.slot_coords.at((Slot)slot);

			// Draw coordinate x
			float dx = inventory_pos->x + offset.x * m_InventorySpriteScale;
			// Draw coordinate y
			float dy = inventory_pos->y + (float)Vivium::Application::height - (offset.y * m_InventorySpriteScale);

			// Size of sprite to draw
			float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_ItemScale;

			// Update the item (checking for selection, recipe crafting, etc.)
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
			// Get item in cursor slot
			Item& cursor_item = m_InventoryData.at((uint8_t)Slot::CURSOR_0);

			// If not empty, draw the item
			if (cursor_item.id != Item::ID::VOID) {
				// Get draw coordinates
				Vivium::Vector2<float> draw_coords = Vivium::Input::GetCursorPos();

				// Size of sprite
				float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_ItemScale;

				// Render the cursor item
				m_RenderItem(&item_batch, cursor_item, draw_coords, tile_scale * 2.0f);

				// If stackable, render multiple copies
				if (Item::GetIsStackable(cursor_item.id)) {
					// Get rendering pos
					Vivium::Vector2<float> rendering_pos(draw_coords);
					rendering_pos += Vivium::Vector2<float>(1.0f, -4.0f) * m_InventorySpriteScale;

					// Get item count as text
					std::string item_count_string = std::to_string(cursor_item.count);

					// Submit text for displaying item count to our batch
					m_TextObject->SetText(item_count_string);
					m_TextObject->SetPos(rendering_pos);
					text_batch.Submit(m_TextObject);
				}
			}
		}

		Vivium::Batch::RenderData item_batch_data = item_batch.End();
		Vivium::Batch::RenderData text_batch_data = text_batch.End();

		if (item_batch_data) {
			Vivium::Renderer::Submit(item_batch_data.vertex_buffer.get(), item_batch_data.index_buffer.get(), m_ItemShader, TextureManager::game_atlas->GetAtlas().get());
		}

		if (text_batch_data) {
			Vivium::Renderer::Submit(text_batch_data.vertex_buffer.get(), text_batch_data.index_buffer.get(), m_TextShader, m_TextObject->GetTexture().get());
		}
	}

	void Inventory::m_RenderItem(Vivium::Batch* batch, const Item& item, const Vivium::Vector2<float>& pos, const float& size)
	{
		// Get texture coordinates for the item
		// TODO: precompute
		std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray(Item::GetAtlasIndex(item.id));
		
		// Hack for getting the amount of times we should draw this sprite
		int start_index = std::min(item.count, (uint16_t)3) - 1;

		// If we're not meant to display multiple
		if (!Item::GetDisplayMultiple(item.id)) {
			// Render once
			batch->Submit(pos, size, &coords[0]);
		}
		else {
			// Draw a maximum of 3 copies of the item
			for (int i = start_index; i >= 0; i--) {
				// Get item offset
				Vivium::Vector2<float> item_offset = s_ItemCountOffsets[i];

				// Calculate draw coords
				float dx = pos.x + item_offset.x;
				float dy = pos.y + item_offset.y;

				// Render as many copies as designated
				batch->Submit({ dx, dy }, size, &coords[0]);
			}
		}
	}

	void Inventory::m_UpdateItem(float x, float y, float width, float height, const Slot& item_slot)
	{
		// Create AABB for checking if we're hovering over the slot
		Vivium::Math::AABB item_rect(x, y, width, height);
		Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();

		// Get button states
		Vivium::Input::State lmb_state = Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1);
		Vivium::Input::State rmb_state = Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_2);

		// TODO: weird behaviour sometimes with clicks
		// TODO: some sort of pending system for splitting items
		// We hovered this slot
		if (item_rect.Contains(cursor_pos)) {
			// Show as selected slot
			m_SelectedSlot = item_slot;

			// Pressed lmb, pickup the item into the cursor if empty
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
			// If released LMB
			else if (lmb_state == Vivium::Input::State::RELEASE) {
				// Get cursor item
				Item& cursor_item = m_InventoryData.at((slot_base_t)Slot::CURSOR_0);

				// If cursor item is not empty
				if (cursor_item.id != Item::ID::VOID) {
					// Get item that we just released over
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

			// TODO: this is the part that needs some sort of pending system
			// Spreading items if holding down rmb
//#define EXPERIMENTAL_ITEM_SPREAD
#ifdef EXPERIMENTAL_ITEM_SPREAD
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
#endif
		}
	}

	void Inventory::m_UpdateFloorItems(const Vivium::Vector2<float>& player_pos, World* world)
	{
		// Get player position as tile
		Vivium::Vector2<float> player_scale_pos = player_pos / World::PIXEL_SCALE;
		// Get region index
		Vivium::Vector2<int> region_pos = world->GetRegionIndex(Vivium::Vector2<int>(player_scale_pos.floor()));

		bool pickedUpItem = false;
		
		// Iterate regions
		for (int region_y = region_pos.y - 1; region_y <= region_pos.y + 1; region_y++) {
			for (int region_x = region_pos.x - 1; region_x <= region_pos.x + 1; region_x++) {
				// Get floor items for the given region
				std::vector<FloorItem>* floor_items = world->GetFloorItems(Vivium::Vector2<int>(region_x, region_y));

				// If no floor items, just go next region
				if (floor_items == nullptr) { continue; }

				// Iterate floor items
				// NOTE: using while loop since we might delete some items during iteration
				auto it = floor_items->begin();
				while (it != floor_items->end()) {
					// Get position of item
					Vivium::Vector2<float> item_pos = it->m_Quad->GetCenter();

					// Calculate distance to player
					float sqr_distance = item_pos.sqr_distance(player_pos);

					// Within pickup range
					if (sqr_distance <= FloorItem::PICKUP_RANGE * FloorItem::PICKUP_RANGE) {
						pickedUpItem = true;
						
						// Add item to inventory
						int remainder = AddItem(it->GetItemData());

						// If entire stack was picked up
						if (remainder == 0) {
							// Erase from floor items
							it = floor_items->erase(it);
						}
						else {
							// Remove amount picked up from floor item
							it->m_ItemData.count = remainder;
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

		// Play a sound if we picked up an item
		if (pickedUpItem) {
			Vivium::Application::StartSound("pickup_item.wav");
		}
	}

	Inventory::Properties Inventory::GetProperties(const Inventory::ID& id)
	{
		// Cast id to integer and index properties
		return m_Properties[(id_base_t)id];
	}

	Vivium::Shader* Inventory::m_InventoryShader = nullptr;
	Vivium::Shader* Inventory::m_TextShader = nullptr;
	Vivium::Shader* Inventory::m_ItemShader = nullptr;
	Vivium::Text*	Inventory::m_TextObject = nullptr;

	void Inventory::Init()
	{
		m_InventoryShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
		
		// Create text shader
		m_TextShader = new Vivium::Shader("text_vertex", "text_frag");
		m_TextShader->Bind();
		// Change colour
		m_TextShader->SetUniform3f("u_TextColor", Vivium::RGBColor::WHITE);

		m_ItemShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
		m_TextObject = new Vivium::Text("", {0, 0}, Vivium::Text::Alignment::LEFT, 0.30f);
	}

	void Inventory::Terminate()
	{
		delete m_InventoryShader;
		delete m_TextObject;
		delete m_TextShader;
		delete m_ItemShader;
	}

	bool Inventory::IsFull() const
	{
		// Iterate inventory
		for (slot_base_t i = m_InventoryData.start_index(); i < m_InventoryData.end_index(); i++) {
			// If we encounter an empty slot skip next
			if (m_InventoryData.at((uint16_t)i).id == Item::ID::VOID) {
				return false;
			}
		}

		// If we reached the end of the loop, no open slots
		return true;
	}

// This is 0 to also indicate the stack is now empty, contradicts boolean for true/false
#define INVENTORY_ADD_SUCCESSFUL 0

	int Inventory::AddItem(const Item& item)
	{
		bool itemIsStackable = Item::GetIsStackable(item.id);
		// Will store amount of the given item we have left to add to the inventory
		int count_to_add = item.count;

		// If our item is stackable, search the inventory first to look if we already have a place to stack the item
		// If non-stackable we must add to an empty slot
		if (itemIsStackable) {
			for (Item& inventory_item : m_InventoryData) {
				// If our items have the same id
				if (item.id == inventory_item.id) {
					// If the sum of our counts is less than the stack limit
					if (count_to_add + inventory_item.count <= Item::STACK_LIMIT) {
						// Stack the items together
						inventory_item.count += count_to_add;

						// Update item count
						m_ChangeItemCount(item.id, count_to_add);

						// Successful in adding the item
						return INVENTORY_ADD_SUCCESSFUL;
					}
					// Above stack limit
					else {
						// So add as much as we can
						// Calculate space we have available in that stack
						int available_space = Item::STACK_LIMIT - inventory_item.count;
						// Remove from count to add
						count_to_add -= available_space;
						// And set inventory item to the stack limit, since we added everything we can
						inventory_item.count = Item::STACK_LIMIT;

						// Update item count
						m_ChangeItemCount(item.id, available_space);
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
			return INVENTORY_ADD_SUCCESSFUL;
		}

		// If we reached the end of the loop, we haven't found anywhere to stack the item completely, so return the remaining count of the item
		return count_to_add;
	}

	const Item& Inventory::GetItem(const Slot& slot)
	{
		return m_InventoryData.at((slot_base_t)slot);
	}

	// TODO: same thing?
	void Inventory::Update(const Slot& start_slot, slot_base_t length, const Vivium::Vector2<float>& player_pos, World* world)
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

		// Iterate inventory
		for (slot_base_t i = 0; i < length; i++) {
			// Add to items list
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
		: m_InventoryData({}), m_InventoryID(ID::MAX), inventory_pos(std::make_shared<Vivium::Vector2<float>>(0.0f, 0.0f))
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
		// Get index in item counts map for that id
		auto it = m_ItemCounts.find(id);

		// If index didn't exist
		if (it == m_ItemCounts.end()) {
			// Can't subtract from an item we have none of
			if (change < 0) { LogError("Attempted to subtract item count when item didn't exist"); }
			else {
				// Insert new entry into item counts
				m_ItemCounts.insert({ id, change });
			}
		}
		else {
			// Add the change
			it->second += change;

			// Clamp to 0 (although some crazy error must've happened if we go negative)
			if (it->second < 0) {
				LogError("Subtracted too much of item count for {}", Item::GetName(id));

				it->second = 0;
			}
		}
	}

	void Inventory::SetItems(const std::vector<Item>& items, const Slot& slot)
	{
		// Iterate items
		for (uint8_t i = 0; i < items.size(); i++) {
			// **Replace** items in inventory with items in list given
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
		: m_InventoryID(inventory_id), m_InventoryData(inventory_id), inventory_pos(std::make_shared<Vivium::Vector2<float>>(0.0f, 0.0f))
	{}

	Inventory::~Inventory() {}

	Inventory::Data& Inventory::GetData()
	{
		return m_InventoryData;
	}

	Inventory::Slot Inventory::GetSelectedSlot() const
	{
		return m_SelectedSlot;
	}

	void Inventory::SetSelectedSlot(const Slot& slot)
	{
		m_SelectedSlot = slot;
	}

	std::vector<int> Inventory::AddItems(const std::vector<Item>& items)
	{
		if (items.empty()) return {};

		std::size_t items_size = items.size();
		// Allocate space for items that couldn't be added
		std::vector<int> return_values(items_size);

		// Iterate items and add each
		for (std::size_t i = 0; i < items_size; i++) {
			return_values[i] = AddItem(items[i]);
		}

		// Return un-added items
		return return_values;
	}

	void Inventory::Write(Vivium::Serialiser& s) const
	{
		// TODO: this was implemented, change
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
