#include "CraftingInventory.h"

namespace Game {
	Vivium::Shader* CraftingInventory::m_BGShader = nullptr;

	void CraftingInventory::m_UpdateCraftables(Inventory* player_inv)
	{
		m_Craftables = Recipe::GetCraftableRecipes(player_inv);
	}

	void CraftingInventory::m_UpdateSelectedSlot(Inventory* player_inv)
	{
		Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();

		// TODO: copied from rendering items
		for (int delta = -4; delta <= 4; delta++) {
			int index = delta + m_SelectedItemSlot;
			float y_offset = delta * s_YSpacing;

			if (index < 0 || index >= m_Craftables.size()) {
				continue;
			}
			else {
				Vivium::Vector2<float> draw_pos = { inventory_pos.x, inventory_pos.y + y_offset };

				Vivium::Math::AABB slot_aabb = Vivium::Math::AABB(draw_pos.x, draw_pos.y, s_SlotSize, s_SlotSize);

				// If we clicked lmb
				if (Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1) == Vivium::Input::State::RELEASE) {
					// If this is the slot we clicked on
					if (slot_aabb.Contains(cursor_pos)) {
						// We clicked on the slot we already selected, indicating we want to craft the item
						// TODO: move item into cursor
						if (delta == 0) {
							Recipe::ID recipe_id = m_Craftables.at(index);
							// TODO: for now only crafting 1
							std::vector<Item> results = Recipe::CraftMaxFromInventory(recipe_id, player_inv, 1);

							if (!results.empty()) {
								player_inv->AddItems(results);
							}
						}
						else {
							m_SelectedItemSlot = index;
						}
					}
				}
			}
		}
	}

	void CraftingInventory::Init()
	{
		m_BGShader = new Vivium::Shader("crafting_slot_vertex", "crafting_slot_frag");
	}

	void CraftingInventory::Terminate()
	{
		delete m_BGShader;
	}

	CraftingInventory::CraftingInventory()
		: Inventory(Inventory::ID::CRAFTING)
	{
	}

	void CraftingInventory::Update(Inventory* player_inv)
	{
		m_UpdateCraftables(player_inv);
		m_UpdateSelectedSlot(player_inv);
	}

	void CraftingInventory::Render()
	{
		static const Vivium::BufferLayout slot_layout = {
			Vivium::GLSLDataType::VEC2, // Position
			Vivium::GLSLDataType::VEC2, // Tex coords
			Vivium::GLSLDataType::FLOAT // Alpha
		};

		static const Vivium::BufferLayout item_layout = {
			Vivium::GLSLDataType::VEC2, // Position
			Vivium::GLSLDataType::VEC2  // Tex coords
		};

		// If we have no craftables, we can skip rendering
		if (m_Craftables.size() == 0) return;

		Vivium::Batch slot_batch(9, &slot_layout);
		Vivium::Batch item_batch(9, &item_layout);

		std::array<float, 8> tex_coords = TextureManager::game_atlas->GetCoordsArray({2, 2});

		for (int delta = -4; delta <= 4; delta++) {
			int index = delta + m_SelectedItemSlot;
			float y_offset = delta * s_YSpacing;

			if (index < 0 || index >= m_Craftables.size()) {
				continue;
			}
			else {
				float alpha = s_AlphaLevels[std::abs(delta)];
				Vivium::Vector2<float> draw_pos = { inventory_pos.x, inventory_pos.y + y_offset };

				slot_batch.Submit(draw_pos, s_SlotSize, tex_coords[0], tex_coords[2], tex_coords[1], tex_coords[5], &alpha, 1U);

				// Get item to render from results of recipe
				Recipe::ID recipe_id = m_Craftables.at(index);
				Recipe recipe = Recipe::GetRecipe(recipe_id);
				Item first_item = recipe.results.at(0);
				first_item.count = 1; // Because we don't want to render multiply copies, although this is a somewhat hacky solution
				m_RenderItem(&item_batch, first_item, draw_pos, s_SlotSize * s_ItemScale);
			}
		}

		auto slot_results = slot_batch.End();
		auto item_results = item_batch.End();

		if (slot_results.count > 0) {
			Vivium::Renderer::Submit(slot_results.vertex_buffer.get(), slot_results.index_buffer.get(), m_BGShader, TextureManager::game_atlas->GetAtlas().get());
		}

		if (item_results.count > 0) {
			Vivium::Renderer::Submit(item_results.vertex_buffer.get(), item_results.index_buffer.get(), m_ItemShader, TextureManager::game_atlas->GetAtlas().get());
		}
	}
}
