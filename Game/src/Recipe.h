#pragma once

#include "Item.h"
#include "Inventory.h"

namespace Game {
	class Recipe {
	public:
		enum class ID : uint16_t {
			AMETHYST_PICKAXE,
			EMERALD_PICKAXE,
			RUBY_PICKAXE,
			SAPPHIRE_PICKAXE,
			TOPAZ_PICKAXE,
			MAX
		};

		std::vector<Item> ingredients;
		std::vector<Item> results;

		static Recipe GetRecipe(const ID& id);

		Recipe(const std::initializer_list<Item>& ingredients, const std::initializer_list<Item>& results);

		// Returns amount crafted
		uint16_t CraftFromInventory(Inventory& inventory, uint16_t count);

	private:
		static const std::array<Recipe, (uint16_t)ID::MAX> m_Recipes;
	};
}
