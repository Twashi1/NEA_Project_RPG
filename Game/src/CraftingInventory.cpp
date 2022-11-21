#include "CraftingInventory.h"

namespace Game {
	Vivium::Shader* CraftingInventory::m_Shader = nullptr;

	Vivium::Vector2<float> CraftingInventory::m_GetRenderingPosition(int delta)
	{
		// Get calculate y spacing between elements
		float delta_spacing = delta * s_YSpacing;
		// Return position with the y spacing
		return Vivium::Vector2<float>(inventory_pos->x, inventory_pos->y + delta_spacing);
	}

	void CraftingInventory::m_UpdateCraftables(Inventory* player_inv)
	{
		// Get craftables for the new player inventory
		m_Craftables = Recipe::GetCraftableRecipes(player_inv);
	}

	void CraftingInventory::m_UpdateSelectedSlot(Inventory* player_inv)
	{
		Vivium::Vector2<float> cursor_pos = Vivium::Input::GetCursorPos();

		// TODO: copied from rendering items
		// Iterate deltas
		for (int delta = -s_SlotsEitherSide; delta <= s_SlotsEitherSide; delta++) {
			// Get item index from delta and selected slot
			int index = delta + m_SelectedItemSlot;

			// If index OOB, don't render
			if (index < 0 || index >= m_Craftables.size()) {
				continue;
			}
			else {
				// Calculate draw pos
				Vivium::Vector2<float> draw_pos = m_GetRenderingPosition(delta);

				// AABB for checking if cursor is clicking on slot
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
						// Otherwise we clicked on a different available recipe so switch to displaying that one
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
		// Updating transition time based on elapsed time
		if (m_TransitionTimeRemaining > 0.0f) {
			float elapsed = m_Timer.GetElapsed();
			// Keep at 0
			m_TransitionTimeRemaining = std::max(0.0f, m_TransitionTimeRemaining - elapsed);
		}

		m_UpdateCraftables(player_inv);
		m_UpdateSelectedSlot(player_inv);
	}

	void CraftingInventory::Render()
	{
		// VB layout for slot background
		static const Vivium::BufferLayout slot_layout = {
			Vivium::GLSLDataType::VEC2, // Position
			Vivium::GLSLDataType::VEC2, // Tex coords
			Vivium::GLSLDataType::FLOAT // Alpha
		};

		// VB layout for item sprite
		static const Vivium::BufferLayout item_layout = {
			Vivium::GLSLDataType::VEC2, // Position
			Vivium::GLSLDataType::VEC2  // Tex coords
		};

		// If we have no craftables, we can skip rendering
		if (m_Craftables.size() == 0) return;

		// Create batches for slots and items
		Vivium::Batch slot_batch(s_TotalSlots, &slot_layout);
		Vivium::Batch item_batch(s_TotalSlots, &slot_layout);

		// Get texture coordinates for the normal slot background, and for the slot background when selected
		// TODO: precomp
		std::array<float, 8> normal_slot_coords = TextureManager::game_atlas->GetCoordsArray({2, 2});
		std::array<float, 8> selected_slot_coords = TextureManager::game_atlas->GetCoordsArray({ 2, 3 });

		// Iterate delta
		for (int delta = -s_SlotsEitherSide; delta <= s_SlotsEitherSide; delta++) {
			// Get index for given delta
			int index = delta + m_SelectedItemSlot;

			// If index OOB don't render
			if (index < 0 || index >= m_Craftables.size()) {
				continue;
			}
			else {
				// Get alpha for the slot
				float alpha = s_AlphaLevels[std::abs(delta)];

				Vivium::Vector2<float> draw_pos = m_GetRenderingPosition(delta);

				// If selected slot
				if (delta == 0) {
					slot_batch.Submit(draw_pos, s_SelectedSlotSize, selected_slot_coords[0], selected_slot_coords[2], selected_slot_coords[1], selected_slot_coords[5], &alpha, 1U);
				}
				// If not selected slot
				else {
					slot_batch.Submit(draw_pos, s_SlotSize, normal_slot_coords[0], normal_slot_coords[2], normal_slot_coords[1], normal_slot_coords[5], &alpha, 1U);
				}

				// Get item to render from results of recipe
				Recipe::ID recipe_id = m_Craftables.at(index);
				// Get recipe data
				Recipe recipe = Recipe::GetRecipe(recipe_id);
				// Get first result of recipe (if recipe has multiple results, assume first is most important)
				Item first_item = recipe.results.at(0);
				
				// Get texture index for result of recipe
				std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray(Item::GetAtlasIndex(first_item.id));

				// Submit result of recipe texture to batch
				item_batch.Submit(draw_pos, s_SlotSize * s_ItemScale, coords[0], coords[2], coords[1], coords[5], &alpha, 1U);
			}
		}

		Vivium::Batch::RenderData slot_results = slot_batch.End();
		Vivium::Batch::RenderData item_results = item_batch.End();

		// If we have any slots to render
		if (slot_results) {
			Vivium::Renderer::Submit(slot_results.vertex_buffer.get(), slot_results.index_buffer.get(), m_Shader, TextureManager::game_atlas->GetAtlas().get());
		}

		// If we have any items to render
		if (item_results) {
			Vivium::Renderer::Submit(item_results.vertex_buffer.get(), item_results.index_buffer.get(), m_Shader, TextureManager::game_atlas->GetAtlas().get());
		}
	}
}
