#include "Inventory.h"

namespace Game {
	const std::array<Inventory::Properties, (Inventory::id_base_t)Inventory::ID::MAX> Inventory::m_Properties{
		Inventory::Properties(
			// SMALL VVV
			Inventory::Slot::INV_0, 27, {0, 0}, {3, 1}, {4.0f * 32.0f, 2.0f * 32.0f},
			{
				// TODO: fix coords
				{Slot::INV_0,  {18.3f + 9.8f * 0, 15.4f + 8.0f * 0}},
				{Slot::INV_1,  {18.3f + 9.8f * 1, 15.4f + 8.0f * 0}},
				{Slot::INV_2,  {18.3f + 9.8f * 2, 15.4f + 8.0f * 0}},
				{Slot::INV_3,  {18.3f + 9.8f * 3, 15.4f + 8.0f * 0}},
				{Slot::INV_4,  {18.3f + 9.8f * 4, 15.4f + 8.0f * 0}},
				{Slot::INV_5,  {18.3f + 9.8f * 5, 15.4f + 8.0f * 0}},
				{Slot::INV_6,  {18.3f + 9.8f * 6, 15.4f + 8.0f * 0}},
				{Slot::INV_7,  {18.3f + 9.8f * 7, 15.4f + 8.0f * 0}},
				{Slot::INV_8,  {18.3f + 9.8f * 8, 15.4f + 8.0f * 0}},
				{Slot::INV_9,  {18.3f + 9.8f * 0, 15.4f + 8.0f * 1}},
				{Slot::INV_10, {18.3f + 9.8f * 1, 15.4f + 8.0f * 1}},
				{Slot::INV_11, {18.3f + 9.8f * 2, 15.4f + 8.0f * 1}},
				{Slot::INV_12, {18.3f + 9.8f * 3, 15.4f + 8.0f * 1}},
				{Slot::INV_13, {18.3f + 9.8f * 4, 15.4f + 8.0f * 1}},
				{Slot::INV_14, {18.3f + 9.8f * 5, 15.4f + 8.0f * 1}},
				{Slot::INV_15, {18.3f + 9.8f * 6, 15.4f + 8.0f * 1}},
				{Slot::INV_16, {18.3f + 9.8f * 7, 15.4f + 8.0f * 1}},
				{Slot::INV_17, {18.3f + 9.8f * 8, 15.4f + 8.0f * 1}},
				{Slot::INV_18, {18.3f + 9.8f * 0, 15.4f + 8.0f * 2}},
				{Slot::INV_19, {18.3f + 9.8f * 1, 15.4f + 8.0f * 2}},
				{Slot::INV_20, {18.3f + 9.8f * 2, 15.4f + 8.0f * 2}},
				{Slot::INV_21, {18.3f + 9.8f * 3, 15.4f + 8.0f * 2}},
				{Slot::INV_22, {18.3f + 9.8f * 4, 15.4f + 8.0f * 2}},
				{Slot::INV_23, {18.3f + 9.8f * 5, 15.4f + 8.0f * 2}},
				{Slot::INV_24, {18.3f + 9.8f * 6, 15.4f + 8.0f * 2}},
				{Slot::INV_25, {18.3f + 9.8f * 7, 15.4f + 8.0f * 2}},
				{Slot::INV_26, {18.3f + 9.8f * 8, 15.4f + 8.0f * 2}}
			}
		), // SMALL ^^^ LARGE VVV
		Inventory::Properties(
			Inventory::Slot::INV_0, 54, {0, 2}, {3, 4}, {4.0f * 32.0f, 3.0f * 32.0f},
			{
				{Slot::INV_0, {20.0f, 20.0f}}
			}
		), // LARGE ^^^ HOTBAR VVV
		Inventory::Properties(
			Inventory::Slot::INV_0, 9, {4, 3}, {7, 3}, {4.0f * 32.0f, 1 * 32.0f},
			{
				{Slot::INV_0, {20.5f + 11.0f * 0, 16.5f + 0.0f * 0}},
				{Slot::INV_1, {20.5f + 11.0f * 1, 16.5f + 0.0f * 0}},
				{Slot::INV_2, {20.5f + 11.0f * 2, 16.5f + 0.0f * 0}},
				{Slot::INV_3, {20.5f + 11.0f * 3, 16.5f + 0.0f * 0}},
				{Slot::INV_4, {20.5f + 11.0f * 4, 16.5f + 0.0f * 0}},
				{Slot::INV_5, {20.5f + 11.0f * 5, 16.5f + 0.0f * 0}},
				{Slot::INV_6, {20.5f + 11.0f * 6, 16.5f + 0.0f * 0}},
				{Slot::INV_7, {20.5f + 11.0f * 7, 16.5f + 0.0f * 0}},
				{Slot::INV_8, {20.5f + 11.0f * 8, 16.5f + 0.0f * 0}}
			}
		),  // HOTBAR ^^^ SMALL_WITH_HOTBAR VVV
		Inventory::Properties(
			Inventory::Slot::INV_0, 36, {4, 0}, {7, 1}, {4 * 32.0f, 2 * 32.0f},
			{
				{Slot::INV_0,  {21.5f + 11.0f * 0, 18.5f + 11.0f * 3}},
				{Slot::INV_1,  {21.5f + 11.0f * 1, 18.5f + 11.0f * 3}},
				{Slot::INV_2,  {21.5f + 11.0f * 2, 18.5f + 11.0f * 3}},
				{Slot::INV_3,  {21.5f + 11.0f * 3, 18.5f + 11.0f * 3}},
				{Slot::INV_4,  {21.5f + 11.0f * 4, 18.5f + 11.0f * 3}},
				{Slot::INV_5,  {21.5f + 11.0f * 5, 18.5f + 11.0f * 3}},
				{Slot::INV_6,  {21.5f + 11.0f * 6, 18.5f + 11.0f * 3}},
				{Slot::INV_7,  {21.5f + 11.0f * 7, 18.5f + 11.0f * 3}},
				{Slot::INV_8,  {21.5f + 11.0f * 8, 18.5f + 11.0f * 3}},
				{Slot::INV_9,  {21.5f + 11.0f * 0, 14.0f + 11.0f * 1}},
				{Slot::INV_10, {21.5f + 11.0f * 1, 14.0f + 11.0f * 1}},
				{Slot::INV_11, {21.5f + 11.0f * 2, 14.0f + 11.0f * 1}},
				{Slot::INV_12, {21.5f + 11.0f * 3, 14.0f + 11.0f * 1}},
				{Slot::INV_13, {21.5f + 11.0f * 4, 14.0f + 11.0f * 1}},
				{Slot::INV_14, {21.5f + 11.0f * 5, 14.0f + 11.0f * 1}},
				{Slot::INV_15, {21.5f + 11.0f * 6, 14.0f + 11.0f * 1}},
				{Slot::INV_16, {21.5f + 11.0f * 7, 14.0f + 11.0f * 1}},
				{Slot::INV_17, {21.5f + 11.0f * 8, 14.0f + 11.0f * 1}},
				{Slot::INV_18, {21.5f + 11.0f * 0, 14.0f + 11.0f * 2}},
				{Slot::INV_19, {21.5f + 11.0f * 1, 14.0f + 11.0f * 2}},
				{Slot::INV_20, {21.5f + 11.0f * 2, 14.0f + 11.0f * 2}},
				{Slot::INV_21, {21.5f + 11.0f * 3, 14.0f + 11.0f * 2}},
				{Slot::INV_22, {21.5f + 11.0f * 4, 14.0f + 11.0f * 2}},
				{Slot::INV_23, {21.5f + 11.0f * 5, 14.0f + 11.0f * 2}},
				{Slot::INV_24, {21.5f + 11.0f * 6, 14.0f + 11.0f * 2}},
				{Slot::INV_25, {21.5f + 11.0f * 7, 14.0f + 11.0f * 2}},
				{Slot::INV_26, {21.5f + 11.0f * 8, 14.0f + 11.0f * 2}},
				{Slot::INV_27, {21.5f + 11.0f * 0, 14.0f + 11.0f * 0}},
				{Slot::INV_28, {21.5f + 11.0f * 1, 14.0f + 11.0f * 0}},
				{Slot::INV_29, {21.5f + 11.0f * 2, 14.0f + 11.0f * 0}},
				{Slot::INV_30, {21.5f + 11.0f * 3, 14.0f + 11.0f * 0}},
				{Slot::INV_31, {21.5f + 11.0f * 4, 14.0f + 11.0f * 0}},
				{Slot::INV_32, {21.5f + 11.0f * 5, 14.0f + 11.0f * 0}},
				{Slot::INV_33, {21.5f + 11.0f * 6, 14.0f + 11.0f * 0}},
				{Slot::INV_34, {21.5f + 11.0f * 7, 14.0f + 11.0f * 0}},
				{Slot::INV_35, {21.5f + 11.0f * 8, 14.0f + 11.0f * 0}},
			 }
		) // SMALL_WITH_HOTBAR ^^^
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

	void Inventory::m_RenderItems()
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

		// TODO: make InventoryData_t iterable
		for (slot_base_t slot = m_InventoryData.start_index(); slot < m_InventoryData.end_index(); slot++) {
			Item& item = m_InventoryData.at(slot);

			// Get coord offset to render item at
			Vivium::Vector2<float> offset = inven_properties.slot_coords.at((Slot)slot);

			float dx = m_InventoryQuad->Left() + (offset.x * m_InventorySpriteScale);
			float dy = m_InventoryQuad->Top() - (offset.y * m_InventorySpriteScale); // TODO: - sign here is weird

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
				rendering_pos += Vivium::Vector2<float>(3.5f - (item_count_string.length() * 1.5f), -3.0f) * m_InventorySpriteScale;

				// Iterate through characters in text
				for (char c : item_count_string) {
					Vivium::Font::Character ch = Vivium::Text::GetDefaultFont()->character_map.at(c);

					float x = rendering_pos.x + ch.bearing.x * m_TextObject->scale;
					float y = rendering_pos.y - (ch.size.y - ch.bearing.y) * m_TextObject->scale;
					float w = ch.size.x * m_TextObject->scale;
					float h = ch.size.y * m_TextObject->scale;

					m_RenderItemCount(&text_batch, c, { x, y }, { w, h });

					rendering_pos.x += (ch.advance >> 6) * m_TextObject->scale * m_InventorySpriteScale / 8.0f;
				}
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

					// Iterate through characters in text
					for (char c : item_count_string) {
						// TODO: precalc
						Vivium::Font::Character ch = Vivium::Text::GetDefaultFont()->character_map.at(c);

						float x = rendering_pos.x + ch.bearing.x * m_TextObject->scale;
						float y = rendering_pos.y - (ch.size.y - ch.bearing.y) * m_TextObject->scale;
						float w = ch.size.x * m_TextObject->scale;
						float h = ch.size.y * m_TextObject->scale;

						m_RenderItemCount(&text_batch, c, { x, y }, { w, h });

						rendering_pos.x += (ch.advance >> 6) * m_TextObject->scale * m_InventorySpriteScale / 8.0f;
					}
				}
			}
 		}

		Vivium::Batch::BatchData item_batch_data = item_batch.End();
		Vivium::Batch::BatchData text_batch_data = text_batch.End();

		if (item_batch_data.count > 0) {
			Vivium::Renderer::Submit(item_batch_data.vertex_buffer.get(), item_batch_data.index_buffer.get(), m_ItemShader, World::m_ItemsAtlas->GetAtlas().get());
		}

		if (text_batch_data.count > 0) {
			Vivium::Renderer::Submit(text_batch_data.vertex_buffer.get(), text_batch_data.index_buffer.get(), m_TextShader, m_TextFontTexture.get());
		}
	}

	void Inventory::m_RenderItem(Vivium::Batch* batch, const Item& item, const Vivium::Vector2<float>& pos, const float& size)
	{
		std::array<float, 4>& faces = World::m_ItemsTextureCoords[(uint16_t)item.id];

		// Draw a maximum of 3 copies of the item
		for (int i = std::min(item.count, (uint16_t)3) - 1; i >= 0; i--) {
			Vivium::Vector2<float> item_offset = s_ItemCountOffsets[i];

			// Calculate draw coords
			float dx = pos.x + item_offset.x;
			float dy = pos.y + item_offset.y;

			batch->Submit({ dx, dy }, size, faces[0], faces[2], faces[1], faces[3]);
		}
	}

	void Inventory::m_RenderItemCount(Vivium::Batch* batch, char c, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& size)
	{
		// TODO: cleanup
		Vivium::Font::Character ch = Vivium::Text::GetDefaultFont()->character_map.at(c);
		// TODO: precalc this
		// Bottom left
		// Bottom right
		// Top right
		// Top left
		std::array<float, 8> tex_coords = m_TextFontAtlas->GetCoordsArray(c);

		float left = tex_coords[0];
		float top = tex_coords[1];
		float right = left + ((float)ch.size.x / (float)m_TextFontAtlas->GetAtlas()->GetWidth());
		float bottom = top + ((float)ch.size.y / (float)m_TextFontAtlas->GetAtlas()->GetHeight());

		batch->Submit(pos.x, pos.x + size.x, pos.y, pos.y + size.y, left, right, bottom, top);
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

	Inventory::Properties Inventory::GetProperties(const Inventory::ID& id)
	{
		return m_Properties[(id_base_t)id];
	}

	Vivium::Shader* Inventory::m_InventoryShader = nullptr;
	Vivium::Shader* Inventory::m_TextShader = nullptr;
	Vivium::Shader* Inventory::m_ItemShader = nullptr;
	Vivium::TextureAtlas* Inventory::m_InventoryAtlas = nullptr;
	Vivium::Text* Inventory::m_TextObject = nullptr;
	Ref(Vivium::Texture) Inventory::m_TextFontTexture = nullptr;
	Vivium::TextureAtlas* Inventory::m_TextFontAtlas = nullptr;

	void Inventory::Init()
	{
		m_InventoryAtlas = new Vivium::TextureAtlas("inventory.png", { 32, 32 });
		m_InventoryShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
		
		m_TextShader = new Vivium::Shader("text_vertex", "text_frag");
		m_TextShader->Bind();
		m_TextShader->SetUniform3f("u_TextColor", Vivium::RGBColor::WHITE);

		m_ItemShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
		m_TextObject = new Vivium::Text("", {0, 0}, Vivium::Text::Alignment::LEFT, 0.35f); // TODO: pretty ugly, later use font or something

		m_TextFontTexture = MakeRef(Vivium::Texture, Vivium::Text::GetDefaultFont().get());
		m_TextFontAtlas = new Vivium::TextureAtlas(m_TextFontTexture, { 64, 64 });
	}

	void Inventory::Terminate()
	{
		delete m_InventoryAtlas;
		delete m_InventoryShader;
		delete m_TextShader;
		delete m_ItemShader;
		delete m_TextFontAtlas;
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

			// Return that item was added to inventory
			return true;
		}

		// If we reached the end of the loop, we haven't found anywhere to stack the item, so return that we weren't successful
		return false;
	}

	void Inventory::SetPos(const Vivium::Vector2<float>& new_pos)
	{
		m_InventoryQuad->SetCenter(new_pos);
	}

	const Item& Inventory::GetItem(const Slot& slot)
	{
		return m_InventoryData.at((uint8_t)slot);
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

	std::vector<Item> Inventory::GetItems(const Slot& start_slot, slot_base_t length)
	{
		std::vector<Item> items(length);

		for (slot_base_t i = 0; i < length; i++) {
			items[i] = m_InventoryData.at((slot_base_t)start_slot + i);
		}

		return items;
	}

	void Inventory::Render()
	{
		Vivium::Renderer::Submit(m_InventoryQuad, m_InventoryShader, m_InventoryAtlas->GetAtlas().get());

		m_RenderItems();
	}

	Inventory::Inventory()
		: m_InventoryData({}), m_InventoryID(ID::MAX), m_InventoryQuad(nullptr)
	{}

	void Inventory::SetItems(const std::vector<Item>& items, const Slot& slot)
	{
		for (uint8_t i = 0; i < items.size(); i++) {
			m_InventoryData.at((uint8_t)slot + i) = items.at(i);
		}
	}

	Inventory::Inventory(const ID& inventory_id)
		: m_InventoryID(inventory_id), m_InventoryData(inventory_id)
	{
		Properties properties = Inventory::GetProperties(inventory_id);
		m_InventoryQuad = new Vivium::Quad(Vivium::Vector2<float>(0.0f), properties.sprite_size * m_InventorySpriteScale);

		m_InventoryAtlas->Set(m_InventoryQuad, properties.top_left_index, properties.bottom_right_index);
	}

	Inventory::~Inventory()
	{
		delete m_InventoryQuad;
	}

	Inventory::Data& Inventory::GetData()
	{
		return m_InventoryData;
	}

	std::vector<bool> Inventory::AddItems(const std::vector<Item>& items)
	{
		std::size_t items_size = items.size();
		std::vector<bool> return_values(items_size);

		for (std::size_t i = 0; i < items_size; i++) {
			return_values[i] = AddItem(items[i]);
		}

		return return_values;
	}

	void Inventory::Write(Vivium::Serialiser& s) const
	{
		// Write inventory type
		s.Write((id_base_t)m_InventoryID);
		// Write all items
		s.Write(m_InventoryData);
	}

	void Inventory::Read(Vivium::Serialiser& s)
	{
		// Read in inventory type
		s.Read((id_base_t*)&m_InventoryID);
		// Read all items
		s.Read(&m_InventoryData);

		// Init quad and the atlas coords for quad
		Properties properties = Inventory::GetProperties(m_InventoryID);
		m_InventoryQuad = new Vivium::Quad(Vivium::Vector2<float>(0.0f), properties.sprite_size * m_InventorySpriteScale);

		m_InventoryAtlas->Set(m_InventoryQuad, properties.top_left_index, properties.bottom_right_index);
	}

	Inventory::Properties::Properties(
		const Slot& start_slot, const slot_base_t& inventory_size,
		const Vivium::Vector2<int>& top_left_index, const Vivium::Vector2<int>& bottom_right_index,
		const Vivium::Vector2<float>& sprite_size,
		const std::unordered_map<Slot, Vivium::Vector2<float>>& slot_coords
	)
		: start_slot(start_slot), inventory_size(inventory_size),
		top_left_index(top_left_index), bottom_right_index(bottom_right_index),
		sprite_size(sprite_size), slot_coords(slot_coords)
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
