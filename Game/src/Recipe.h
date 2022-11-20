#pragma once

#include "Item.h"
#include "Inventory.h"

namespace Game {
	/// <summary>
	/// Class to hold ingredients and results for a given crafting recipe
	/// Also calculates the amount that can be crafted for a given recipe, and all recipes available given the items
	/// in an inventory
	/// Static class
	/// </summary>
	class Recipe {
	public:
		enum class ID : uint16_t {
			AMETHYST_PICKAXE, // NOTE: must be first for GetCraftableRecipes
			EMERALD_PICKAXE,
			RUBY_PICKAXE,
			SAPPHIRE_PICKAXE,
			TOPAZ_PICKAXE,
			AMETHYST_SWORD,
			EMERALD_SWORD,
			RUBY_SWORD,
			SAPPHIRE_SWORD,
			TOPAZ_SWORD,
			AMETHYST_WAND,
			EMERALD_WAND,
			RUBY_WAND,
			SAPPHIRE_WAND,
			TOPAZ_WAND,
			MAX
		};

		std::vector<Item> ingredients;
		std::vector<Item> results;

		// TODO: can't this be a const ref? m_Recipes wouldn't go out of scope
		/// <summary>
		/// Get recipe object for a given id
		/// </summary>
		static Recipe GetRecipe(const ID& id);
		
		// TODO: move constructor might be useful?
		Recipe(const Recipe& other) = default;
		Recipe(const std::initializer_list<Item>& ingredients, const std::initializer_list<Item>& results);

		/// <summary>
		/// Get maximum amount craftable given a recipe and an inventory
		/// </summary>
		static uint16_t GetMaxCraftable(const Recipe::ID& id, Inventory* inventory);
		/// <summary>
		/// Actually craft a certain amount from an inventory
		/// </summary>
		static std::vector<Item> CraftMaxFromInventory(const Recipe::ID& id, Inventory* inventory, uint16_t count); // TODO: should be static

		/// <summary>
		/// Get all craftable recipes given an inventory
		/// </summary>
		static std::vector<Recipe::ID> GetCraftableRecipes(Inventory* inventory);

	private:
		// Stores recipes
		static const std::array<Recipe, (uint16_t)ID::MAX> m_Recipes;
	};
}
