#include "CraftingInventory.h"

namespace Game {
	Vivium::Shader* CraftingInventory::m_Shader = nullptr;

	Vivium::Vector2<float> CraftingInventory::m_GetRenderingPosition(int delta)
	{
		float delta_spacing = delta * s_YSpacing;
		return Vivium::Vector2<float>(inventory_pos->x, inventory_pos->y + delta_spacing);
	}

	void CraftingInventory::m_UpdateCraftables(Inventory* player_inv)
	{
		m_Craftables = Recipe::GetCraftableRecipes(player_inv);
	}

	void CraftingInventory::m_UpdateSelectedSlot(Inventory* player_inv)
	{
		Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();

		// TODO: copied from rendering items
		for (int delta = -s_SlotsEitherSide; delta <= s_SlotsEitherSide; delta++) {
			int index = delta + m_SelectedItemSlot;

			if (index < 0 || index >= m_Craftables.size()) {
				continue;
			}
			else {
				Vivium::Vector2<float> draw_pos = m_GetRenderingPosition(delta);

				Vivium::Math::AABB slot_aabb = Vivium::Math::AABB(draw_pos.x, draw_pos.y, s_SlotSize, s_SlotSize);

				// If we clicked lmb
				if (Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1) == Vivium::Input::State::RELEASE) {
					// If this is the slot we clicked on
					if (slot_aabb.Contains(cursor_pos)) {
						// We clicked on the slot we already selected, indicating we want to craft the item
						if (delta == 0) {
							Recipe::ID recipe_id = m_Craftables.at(index);
							// TODO: for now only crafting 1
							// TODO: shift click to craft max?
							std::vector<Item> results = Recipe::CraftMaxFromInventory(recipe_id, player_inv, 1);

							player_inv->AddItems(results);
						}
						else {
							m_TransitionDelta = m_SelectedItemSlot - index;
							m_TransitionTimeRemaining = std::abs(m_TransitionDelta) * s_TransitionTime;
							m_SelectedItemSlot = index;
						}
					}
				}
			}
		}
	}

	void CraftingInventory::Init()
	{
		m_Shader = new Vivium::Shader("crafting_slot_vertex", "crafting_slot_frag");
	}

	void CraftingInventory::Terminate()
	{
		delete m_Shader;
	}

	CraftingInventory::CraftingInventory()
		: Inventory(Inventory::ID::CRAFTING)
	{
		m_Timer.Start();
	}

	void CraftingInventory::Update(Inventory* player_inv)
	{
		if (m_TransitionTimeRemaining > 0.0f) {
			float elapsed = m_Timer.GetElapsed();
			m_TransitionTimeRemaining = std::max(0.0f, m_TransitionTimeRemaining - elapsed);
		}

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

		Vivium::Batch slot_batch(s_TotalSlots, &slot_layout);
		Vivium::Batch item_batch(s_TotalSlots, &slot_layout);

		std::array<float, 8> normal_slot_coords = TextureManager::game_atlas->GetCoordsArray({2, 2});
		std::array<float, 8> selected_slot_coords = TextureManager::game_atlas->GetCoordsArray({ 2, 3 });

		for (int delta = -s_SlotsEitherSide; delta <= s_SlotsEitherSide; delta++) {
			int index = delta + m_SelectedItemSlot;

			if (index < 0 || index >= m_Craftables.size()) {
				continue;
			}
			else {
				float alpha = s_AlphaLevels[std::abs(delta)];

				Vivium::Vector2<float> draw_pos = m_GetRenderingPosition(delta);

				if (delta == 0) {
					slot_batch.Submit(draw_pos, s_SelectedSlotSize, selected_slot_coords[0], selected_slot_coords[2], selected_slot_coords[1], selected_slot_coords[5], &alpha, 1U);
				}
				else {
					slot_batch.Submit(draw_pos, s_SlotSize, normal_slot_coords[0], normal_slot_coords[2], normal_slot_coords[1], normal_slot_coords[5], &alpha, 1U);
				}

				// Get item to render from results of recipe
				Recipe::ID recipe_id = m_Craftables.at(index);
				Recipe recipe = Recipe::GetRecipe(recipe_id);
				Item first_item = recipe.results.at(0);
				
				std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray(Item::GetAltasIndex(first_item.id));

				item_batch.Submit(draw_pos, s_SlotSize * s_ItemScale, coords[0], coords[2], coords[1], coords[5], &alpha, 1U);
			}
		}

		auto slot_results = slot_batch.End();
		auto item_results = item_batch.End();

		if (slot_results.count > 0) {
			Vivium::Renderer::Submit(slot_results.vertex_buffer.get(), slot_results.index_buffer.get(), m_Shader, TextureManager::game_atlas->GetAtlas().get());
		}

		if (item_results.count > 0) {
			Vivium::Renderer::Submit(item_results.vertex_buffer.get(), item_results.index_buffer.get(), m_Shader, TextureManager::game_atlas->GetAtlas().get());
		}
	}
}
