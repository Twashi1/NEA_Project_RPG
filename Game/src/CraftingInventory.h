#pragma once

#include "Inventory.h"
#include "Recipe.h"

namespace Game {
	class CraftingInventory : Inventory {
	private:
		// TODO: maybe luminosity or intensity
		static constexpr float s_AlphaLevels[5] = { 1.0f, 0.70f, 0.50f, 0.30f, 0.10f };
		static constexpr float s_YSpacing = 52.0f;
		static constexpr float s_SlotSize = 48.0f;
		static constexpr float s_ItemScale = 0.7f;

		static Vivium::Shader* m_BGShader;

		using Inventory::m_InventoryData;
		using Inventory::m_InventoryID;

		std::vector<Recipe::ID> m_Craftables;
		int m_SelectedItemSlot = NULL;

		void m_UpdateCraftables(Inventory* player_inv);
		void m_UpdateSelectedSlot(Inventory* player_inv);

		using Inventory::m_RenderItem;

	public:
		using Inventory::inventory_pos;
		using Inventory::GetItem;
		using Inventory::GetData;
		using Inventory::GetItems;
		using Inventory::SetItems;

		static void Init();
		static void Terminate();

		void Update(Inventory* player_inv);
		void Render();

		CraftingInventory();
	};
}
