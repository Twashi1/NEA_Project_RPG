#include "Inventory.h"

namespace Game {
	const std::array<Inventory::Properties, (uint8_t)Inventory::ID::MAX> Inventory::m_Properties{
		Inventory::Properties(
			Inventory::Slot::INV_0, 27, {0, 0}, {3, 1}, {114.0f, 46.0f},
			{
				{Slot::INV_0,  {18.3f + 9.8f * 0, 15.4f + 8.0f * 0}},
				{Slot::INV_1,  {18.3f + 9.8f * 1, 15.4f + 8.0f * 0}},
				{Slot::INV_2,  {18.3f + 9.8f * 2, 15.4f + 8.0f * 0}},
				{Slot::INV_3,  {18.3f + 9.8f * 3, 15.4f + 8.0f * 0}},
				{Slot::INV_4,  {18.3f + 9.8f * 4, 15.4f + 8.0f * 0}},
				{Slot::INV_5,  {18.3f + 9.8f * 5, 15.4f + 8.0f * 0}},
				{Slot::INV_6,  {18.3f + 9.8f * 6, 15.4f + 8.0f * 0}},
				{Slot::INV_7,  {18.3f + 9.8f * 7, 15.4f + 8.0f * 0}},
				{Slot::INV_8,  {18.3f + 9.8f * 8, 15.4f + 8.0f * 0}}, // TODO: other tex coords, also relative
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
		), // SMALL
		Inventory::Properties(
			Inventory::Slot::INV_0, 54, {0, 2}, {3, 4}, {114.0f, 90.0f},
			{
				{Slot::INV_0, {20.0f, 20.0f}}
			}
		), // LARGE
		// TODO: LARGE
		Inventory::Properties(
			Inventory::Slot::HOTBAR_0, 9, INT_MAX, INT_MAX, 0.0f,
			{}
		)  // TODO: HOTBAR
	};

	const Vivium::Vector2<float> Inventory::s_ItemCountOffsets[3] = {
			Vivium::Vector2<float>(0.0f, 0.0f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f,
			Vivium::Vector2<float>(-0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f,
			Vivium::Vector2<float>(0.3f, 0.3f) * World::PIXEL_SCALE * 0.5f * s_ItemScale * m_InventorySpriteScale * 0.25f
	};

	// Max offsets used for bounds of each item (used when picking up/re-organising items)
	const float Inventory::s_MaxOffsetWidth = s_ItemCountOffsets[2].x;
	const float Inventory::s_MaxOffsetHeight = s_ItemCountOffsets[2].y;

	void Inventory::m_RenderItems()
	{
		// Get properties of the inventory type we have
		Properties inven_properties = GetProperties(m_InventoryID);

		unsigned int max_count = inven_properties.inventory_size + 1; // Adding one for cursor

		// Allocate memory for vertex data and indices
		float* item_vertex_data = new float[16 * max_count * 3]; std::size_t item_vertex_index = 0;
		uint16_t* item_indices = new uint16_t[6 * max_count * 3]; std::size_t item_indices_index = 0;

		float* text_vertex_data = new float[16 * max_count * 3]; std::size_t text_vertex_index = 0;
		uint16_t* text_indices = new uint16_t[6 * max_count * 3]; std::size_t text_indices_index = 0;

		// Counters for amount of items/strings we're rendering
		uint16_t item_draw_count = 0;
		uint16_t text_draw_count = 0;

		// TODO: make InventoryData_t iterable
		for (uint8_t slot = (uint8_t)inven_properties.start_slot; slot < (uint8_t)inven_properties.start_slot + inven_properties.inventory_size; slot++) {
			Item& item = m_InventoryData.at(slot);

			// TODO: draw multiple copies of texture for multiple items
			// TODO: draw text to represent count
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
				item,
				Vivium::Vector2<float>(dx, dy),
				tile_scale,
				item_vertex_data, item_vertex_index,
				item_indices, item_indices_index
			);

			// Increase draw count
			item_draw_count += std::min(item.count, (uint16_t)3);

			// Now we need to render the text
			// Get rendering pos
			Vivium::Vector2<float> rendering_pos(dx, dy);
			// Little offset so it draws to the bottom right of the item
			rendering_pos += Vivium::Vector2<float>(1.0f, -4.0f) * m_InventorySpriteScale;

			// Get item count as text
			std::string item_count_string = std::to_string(item.count);

			// Iterate through characters in text
			for (char c : item_count_string) {
				Vivium::Font::Character ch = Vivium::Text::GetDefaultFont()->character_map.at(c);

				float x = rendering_pos.x + ch.bearing.x * m_TextObject->scale;
				float y = rendering_pos.y - (ch.size.y - ch.bearing.y) * m_TextObject->scale;
				float w = ch.size.x * m_TextObject->scale;
				float h = ch.size.y * m_TextObject->scale;

				m_RenderItemCount(c, { x, y }, { w, h }, text_vertex_data, text_vertex_index, text_indices, text_indices_index);

				text_draw_count++;
				rendering_pos.x += (ch.advance >> 6) * m_TextObject->scale * m_InventorySpriteScale / 8.0f * 0.45f;
			}
		}

		// Render cursor
		{
			Item& cursor_item = m_InventoryData.at((uint8_t)Slot::CURSOR_0);

			if (cursor_item.id != Item::ID::VOID) {
				Vivium::Vector2<float> draw_coords = Vivium::Input::GetCursorPos();

				float tile_scale = World::PIXEL_SCALE * 0.5f * m_InventorySpriteScale * 0.25f * s_ItemScale;

				// TODO: pass item draw count to function
				// Draw the item
				m_RenderItem(
					cursor_item,
					draw_coords,
					tile_scale,
					item_vertex_data, item_vertex_index,
					item_indices, item_indices_index
				);

				// Increase draw count
				item_draw_count += std::min(cursor_item.count, (uint16_t)3);

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

					m_RenderItemCount(c, { x, y }, { w, h }, text_vertex_data, text_vertex_index, text_indices, text_indices_index);

					text_draw_count++;
					rendering_pos.x += (ch.advance >> 6) * m_TextObject->scale * m_InventorySpriteScale / 8.0f * 0.45f;
				}
			}
 		}

		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		static const Vivium::BufferLayout text_layout = {
			Vivium::GLSLDataType::VEC4 // position xy, tex coords zw
		};

		if (item_draw_count > 0) {
			Vivium::VertexBuffer vb(item_vertex_data, item_vertex_index + 1, layout);
			Vivium::IndexBuffer ib(item_indices, item_indices_index + 1);

			Vivium::Renderer::Submit(&vb, &ib, m_ItemShader, World::m_ItemsAtlas->GetAtlas().get());
		}

		if (text_draw_count > 0) {
			Vivium::VertexBuffer vb(text_vertex_data, text_vertex_index + 1, text_layout);
			Vivium::IndexBuffer ib(text_indices, text_indices_index + 1);

			Vivium::Renderer::Submit(&vb, &ib, m_TextShader, m_TextFontTexture.get());
		}

		delete[] item_vertex_data;
		delete[] item_indices;

		delete[] text_vertex_data;
		delete[] text_indices;
	}

	void Inventory::m_RenderItem(const Item& item, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& halfsize, float* vertex_data, std::size_t& vertex_data_index, uint16_t* vertex_indices, std::size_t& vertex_indices_index)
	{
		// Draw a maximum of 3 copies of the item
		for (int i = std::min(item.count, (uint16_t)3) - 1; i >= 0; i--) {
			Vivium::Vector2<float> item_offset = s_ItemCountOffsets[i];

			std::array<float, 4>& faces = World::m_ItemsTextureCoords[(uint16_t)item.id];

			// Calculate draw coords
			float dx = pos.x + item_offset.x;
			float dy = pos.y + item_offset.y;

			// Bottom left
			vertex_data[vertex_data_index++] = dx - halfsize.x;
			vertex_data[vertex_data_index++] = dy - halfsize.y;
			vertex_data[vertex_data_index++] = faces[0];
			vertex_data[vertex_data_index++] = faces[1];
			// Bottom right
			vertex_data[vertex_data_index++] = dx + halfsize.x;
			vertex_data[vertex_data_index++] = dy - halfsize.y;
			vertex_data[vertex_data_index++] = faces[2];
			vertex_data[vertex_data_index++] = faces[1];
			// Top right
			vertex_data[vertex_data_index++] = dx + halfsize.x;
			vertex_data[vertex_data_index++] = dy + halfsize.y;
			vertex_data[vertex_data_index++] = faces[2];
			vertex_data[vertex_data_index++] = faces[3];
			// Top left
			vertex_data[vertex_data_index++] = dx - halfsize.x;
			vertex_data[vertex_data_index++] = dy + halfsize.y;
			vertex_data[vertex_data_index++] = faces[0];
			vertex_data[vertex_data_index++] = faces[3];

			int stride = vertex_indices_index / 6 * 4;

			vertex_indices[vertex_indices_index++] = 0 + stride;
			vertex_indices[vertex_indices_index++] = 1 + stride;
			vertex_indices[vertex_indices_index++] = 2 + stride;
			vertex_indices[vertex_indices_index++] = 2 + stride;
			vertex_indices[vertex_indices_index++] = 3 + stride;
			vertex_indices[vertex_indices_index++] = 0 + stride;
		}
	}

	void Inventory::m_RenderItemCount(char c, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& size, float* vertex_data, std::size_t& vertex_data_index, uint16_t* vertex_indices, std::size_t& vertex_indices_index)
	{
		// TODO: precalc this
		// Bottom left
		// Bottom right
		// Top right
		// Top left
		std::array<float, 8> tex_coords = m_TextFontAtlas->GetCoordsArray(c);

		// Bottom left
		vertex_data[vertex_data_index++] = pos.x;
		vertex_data[vertex_data_index++] = pos.y;
		vertex_data[vertex_data_index++] = tex_coords[0];
		vertex_data[vertex_data_index++] = tex_coords[5];
		// Bottom left
		vertex_data[vertex_data_index++] = pos.x + size.x;
		vertex_data[vertex_data_index++] = pos.y;
		vertex_data[vertex_data_index++] = tex_coords[2];
		vertex_data[vertex_data_index++] = tex_coords[7];
		// Bottom left
		vertex_data[vertex_data_index++] = pos.x + size.x;
		vertex_data[vertex_data_index++] = pos.y + size.y;
		vertex_data[vertex_data_index++] = tex_coords[4];
		vertex_data[vertex_data_index++] = tex_coords[1];
		// Bottom left
		vertex_data[vertex_data_index++] = pos.x;
		vertex_data[vertex_data_index++] = pos.y + size.y;
		vertex_data[vertex_data_index++] = tex_coords[6];
		vertex_data[vertex_data_index++] = tex_coords[3];

		int stride = vertex_indices_index / 6 * 4;

		vertex_indices[vertex_indices_index++] = 0 + stride;
		vertex_indices[vertex_indices_index++] = 1 + stride;
		vertex_indices[vertex_indices_index++] = 2 + stride;
		vertex_indices[vertex_indices_index++] = 2 + stride;
		vertex_indices[vertex_indices_index++] = 3 + stride;
		vertex_indices[vertex_indices_index++] = 0 + stride;
	}

	void Inventory::m_UpdateItem(float x, float y, float width, float height, const Slot& item_slot) // TODO: take item ref instead?
	{
		Vivium::Math::AABB item_rect(x, y, width, height);
		Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();

		Vivium::Input::State lmb_state = Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1);

		// TODO: not perfect
		// We have clicked on this item
		if (item_rect.Contains(cursor_pos)) {
			if (lmb_state == Vivium::Input::State::PRESS) {
				// Get cursor item
				Item& cursor_item = m_InventoryData.at((uint8_t)Slot::CURSOR_0);

				// If we have nothing in the cursor slot
				if (cursor_item.id == Item::ID::VOID) {
					// Move our item to that cursor slot
					Item& our_item = m_InventoryData.at((uint8_t)item_slot);
					cursor_item.id = our_item.id;
					cursor_item.count = our_item.count;

					// Delete our item
					our_item.id = Item::ID::VOID;
					our_item.count = 0;
				}
			}
			else if (lmb_state == Vivium::Input::State::RELEASE) {
				// Get cursor item
				Item& cursor_item = m_InventoryData.at((uint8_t)Slot::CURSOR_0);

				// If cursor item is NOT nothing
				if (cursor_item.id != Item::ID::VOID) {
					Item& our_item = m_InventoryData.at((uint8_t)item_slot);

					// Swap item in cursor with item we're updating
					std::swap(cursor_item, our_item);
				}
			}
		}
	}

	Inventory::Properties Inventory::GetProperties(const Inventory::ID& id)
	{
		return m_Properties[(uint8_t)id];
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
		m_TextObject = new Vivium::Text("", {0, 0}, 0.6f); // TODO: pretty ugly, later use font or something

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
