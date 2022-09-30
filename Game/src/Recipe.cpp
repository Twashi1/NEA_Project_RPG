#include "Recipe.h"

namespace Game {
	const std::array<Recipe, (uint16_t)Recipe::ID::MAX> Recipe::m_Recipes = {
		Recipe({{Item::ID::AMETHYST_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::AMETHYST_PICKAXE, 1}}	), // AMETHYST_PICKAXE
		Recipe({{Item::ID::EMERALD_CRYSTAL,  3},	{Item::ID::LOG, 2}}, {{Item::ID::EMERALD_PICKAXE, 1}}	), // EMERALD_PICKAXE
		Recipe({{Item::ID::RUBY_CRYSTAL,     3},	{Item::ID::LOG, 2}}, {{Item::ID::RUBY_PICKAXE, 1}}		), // RUBY_PICKAXE
		Recipe({{Item::ID::SAPPHIRE_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::SAPPHIRE_PICKAXE, 1}}	), // SAPPHIRE_PICKAXE
		Recipe({{Item::ID::TOPAZ_CRYSTAL,    3},	{Item::ID::LOG, 2}}, {{Item::ID::TOPAZ_PICKAXE, 1}}		), // TOPAZ_PICKAXE
		Recipe({{Item::ID::AMETHYST_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::AMETHYST_SWORD, 1}}	), // AMETHYST_SWORD
		Recipe({{Item::ID::EMERALD_CRYSTAL,  3},	{Item::ID::LOG, 2}}, {{Item::ID::EMERALD_SWORD, 1}}		), // EMERALD_SWORD
		Recipe({{Item::ID::RUBY_CRYSTAL,     3},	{Item::ID::LOG, 2}}, {{Item::ID::RUBY_SWORD, 1}}		), // RUBY_SWORD
		Recipe({{Item::ID::SAPPHIRE_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::SAPPHIRE_SWORD, 1}}	), // SAPPHIRE_SWORD
		Recipe({{Item::ID::TOPAZ_CRYSTAL,    3},	{Item::ID::LOG, 2}}, {{Item::ID::TOPAZ_SWORD, 1}}		), // TOPAZ_SWORD
		Recipe({{Item::ID::AMETHYST_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::AMETHYST_WAND, 1}}		), // AMETHYST_WAND
		Recipe({{Item::ID::EMERALD_CRYSTAL,  3},	{Item::ID::LOG, 2}}, {{Item::ID::EMERALD_WAND, 1}}		), // EMERALD_WAND
		Recipe({{Item::ID::RUBY_CRYSTAL,     3},	{Item::ID::LOG, 2}}, {{Item::ID::RUBY_WAND, 1}}			), // RUBY_WAND
		Recipe({{Item::ID::SAPPHIRE_CRYSTAL, 3},	{Item::ID::LOG, 2}}, {{Item::ID::SAPPHIRE_WAND, 1}}		), // SAPPHIRE_WAND
		Recipe({{Item::ID::TOPAZ_CRYSTAL,    3},	{Item::ID::LOG, 2}}, {{Item::ID::TOPAZ_WAND, 1}}		)  // TOPAZ_WAND
	};

	Recipe::Recipe(const std::initializer_list<Item>& ingredients, const std::initializer_list<Item>& results)
		: ingredients(ingredients), results(results)
	{
	}

	uint16_t Recipe::GetMaxCraftable(const Recipe::ID& id, Inventory* inventory)
	{
		uint16_t max_craftable = 0xffffU;

		Recipe recipe = GetRecipe(id);

		for (auto ingredient : recipe.ingredients) {
			int count = inventory->GetItemCount(ingredient.id);
			// Max craftable if ingredient is limiting factor
			int ingredient_max = count / ingredient.count;

			if (ingredient_max < max_craftable) max_craftable = ingredient_max;
		}

		return max_craftable;
	}

	std::vector<Item> Recipe::CraftMaxFromInventory(const Recipe::ID& id, Inventory* inventory, uint16_t count)
	{
		if (inventory->IsFull()) { return {}; }

		Inventory::Data& data = inventory->GetData();

		Recipe recipe = GetRecipe(id);

		int max_craftable = Recipe::GetMaxCraftable(id, inventory);

		if (max_craftable == 0) {
			return {};
		}
		else {
			int to_craft = std::min(max_craftable, (int)count);

			for (Item& item : data) {
				if (item.id == Item::ID::VOID) { continue; }

				auto it = std::find_if(recipe.ingredients.begin(), recipe.ingredients.end(),
					[&item](const Item& ingredient) { return ingredient.id == item.id; });

				// Item is an ingredient
				if (it != recipe.ingredients.end()) {
					int amount_to_delete = std::min(inventory->GetItemCount(item.id), (int)item.count);
					amount_to_delete = std::min(amount_to_delete, to_craft * it->count);
					
					item.count -= amount_to_delete;
					inventory->m_ChangeItemCount(item.id, -amount_to_delete);

					// If no item is left, set id to void
					if (item.count <= 0) {
						item.id = Item::ID::VOID;
					}
				}
			}

			// Create results vector
			std::size_t results_size = recipe.results.size();
			std::vector<Item> crafted_items(results_size);

			for (std::size_t i = 0; i < results_size; i++) {
				crafted_items[i] = Item(recipe.results[i].id, recipe.results[i].count * to_craft);
			}

			// NOTE: probably not needed
			inventory->m_UpdateItemCounts();

			return crafted_items;
		}

		LogWarn("Weird logical error");

		// ...
		return {};
	}

	std::vector<Recipe::ID> Recipe::GetCraftableRecipes(Inventory* inventory)
	{
		std::vector<Recipe::ID> craftable_recipes;

		for (uint16_t i = 0; i < (uint16_t)Recipe::ID::MAX; i++) {
			int count = GetMaxCraftable((Recipe::ID)i, inventory);

			if (count > 0) craftable_recipes.push_back((Recipe::ID)i);
		}

		return craftable_recipes;
	}

	Recipe Recipe::GetRecipe(const ID& id)
	{
		return m_Recipes.at((uint16_t)id);
	}
}
