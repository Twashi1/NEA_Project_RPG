#include "Recipe.h"

namespace Game {
	const std::array<Recipe, (uint16_t)Recipe::ID::MAX> Recipe::m_Recipes = {
		Recipe({{Item::ID::AMETHYST_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::AMETHYST_PICKAXE, 1}}	), // AMETHYST_PICKAXE
		Recipe({{Item::ID::EMERALD_CRYSTAL, 3},		{Item::ID::LOG, 2}}, {{Item::ID::EMERALD_PICKAXE, 1}}	), // EMERALD_PICKAXE
		Recipe({{Item::ID::RUBY_CRYSTAL, 3},		{Item::ID::LOG, 2}}, {{Item::ID::RUBY_PICKAXE, 1}}		), // RUBY_PICKAXE
		Recipe({{Item::ID::SAPPHIRE_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::SAPPHIRE_PICKAXE, 1}}	), // SAPPHIRE_PICKAXE
		Recipe({{Item::ID::TOPAZ_CRYSTAL, 3},		{Item::ID::LOG, 2}}, {{Item::ID::TOPAZ_PICKAXE, 1}}		)  // TOPAZ_PICKAXE
	};

	Recipe::Recipe(const std::initializer_list<Item>& ingredients, const std::initializer_list<Item>& results)
		: ingredients(ingredients), results(results)
	{
	}

	uint16_t Recipe::CraftFromInventory(Inventory& inventory, uint16_t count)
	{
		// TODO
		return NULL;
	}

	Recipe Recipe::GetRecipe(const ID& id)
	{
		return m_Recipes.at((uint16_t)id);
	}
}
