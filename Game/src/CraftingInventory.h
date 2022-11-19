#pragma once

#include "Inventory.h"
#include "Recipe.h"

namespace Game {
	/// <summary>
	/// Specialisation of Inventory for displaying possible crafting recipes, and allowing user
	/// to select from, and craft these recipes
	/// </summary>
	class CraftingInventory : Inventory {
	private:
		// TODO: some sort of transition when switching slots
		static constexpr float s_YSpacing = 56.0f;
		// Size of slot in pixels
		static constexpr float s_SlotSize = 48.0f;
		// Size of the selected slot in pixels
		static constexpr float s_SelectedSlotSize = 54.0f;
		// Scale for item shown in crafting inventory
		static constexpr float s_ItemScale = 0.7f;

		// TODO: implement
		static constexpr float s_TransitionTime = 0.5f;

		static constexpr int s_SlotsEitherSide = 5;
		static constexpr int s_TotalSlots = s_SlotsEitherSide * 2 + 1;

		// TODO: maybe luminosity or intensity
		static constexpr float s_AlphaLevels[s_SlotsEitherSide + 1] = { 1.0f, 0.80f, 0.70f, 0.60f, 0.50f, 0.40f };

		static Vivium::Shader* m_Shader;

		using Inventory::m_InventoryData;
		using Inventory::m_InventoryID;

		// List of recipes we can craft with the items in the inventory
		std::vector<Recipe::ID> m_Craftables;
		// The current selected recipe index
		int m_SelectedItemSlot = NULL;
		// Transition time between moving crafting recipe (NOT IMPLEMENTED)
		float m_TransitionTimeRemaining = 0.0f;
		int m_TransitionDelta = 0;

		Vivium::Timer m_Timer;

		Vivium::Vector2<float> m_GetRenderingPosition(int delta);
		void m_UpdateCraftables(Inventory* player_inv);
		void m_UpdateSelectedSlot(Inventory* player_inv);

	public:
		using Inventory::inventory_pos;
		using Inventory::GetItem;
		using Inventory::GetData;
		using Inventory::GetItems;
		using Inventory::SetItems;

		/// <summary>
		/// Init crafting shader
		/// </summary>
		static void Init();
		/// <summary>
		/// Deallocate crafting shader
		/// </summary>
		static void Terminate();

		/// <summary>
		/// Update available recipes to craft and the selected item
		/// </summary>
		/// <param name="player_inv"></param>
		void Update(Inventory* player_inv);
		/// <summary>
		/// Render crafting recipe selection GUI
		/// </summary>
		void Render();

		CraftingInventory();
	};
}
