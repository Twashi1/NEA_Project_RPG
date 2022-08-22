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

	std::vector<Item> Recipe::CraftMaxFromInventory(Inventory& inventory, uint16_t count)
	{
		// TODO: Major cleanup needed
		struct _IngredientData {
			int count_needed = 0;
			int total_count = 0;
			int amount_craftable = 0;

			_IngredientData() = default;

			_IngredientData(int count_needed, int total_count, int amount_craftable)
				: count_needed(count_needed), total_count(total_count), amount_craftable(amount_craftable)
			{}
		};

		Inventory::Data& data = inventory.GetData();

		std::unordered_map<Item::ID, _IngredientData> ingredient_map;

		// Fill ingredient map
		for (Item& ingredient : ingredients) {
			ingredient_map.insert({ ingredient.id, _IngredientData(ingredient.count, 0, 0) });
		}

		int max_craftable = INT_MAX;

		// Iterate items in inventory
		for (Item& item : data) {
			if (item.id == Item::ID::VOID) { continue; }

			// https://stackoverflow.com/questions/42933943/how-to-use-lambda-for-stdfind-if
			auto it = std::find_if(ingredients.begin(), ingredients.end(),
				[&item](const Item& ingredient) { return ingredient.id == item.id; });

			// We found that item in our ingredients list
			if (it != ingredients.end()) {
				auto map_it = ingredient_map.find(it->id);

				// If it already exists in our ingredient map
				if (map_it != ingredient_map.end()) {
					// Change total count and amount craftable
					map_it->second.total_count += item.count;
					map_it->second.amount_craftable = map_it->second.total_count / map_it->second.count_needed;
				}
			}
		}

		// Calculate max craftable
		for (auto& [id, data] : ingredient_map) {
			max_craftable = max_craftable > data.amount_craftable ? data.amount_craftable : max_craftable;
		}

		if (max_craftable == 0) {
			return {};
		}
		else {
			int to_craft = std::min(max_craftable, (int)count);

			for (auto& [id, data] : ingredient_map) {
				data.total_count = data.count_needed * to_craft;
			}

			for (Item& item : data) {
				if (item.id == Item::ID::VOID) { continue; }

				auto it = std::find_if(ingredients.begin(), ingredients.end(),
					[&item](const Item& ingredient) { return ingredient.id == item.id; });

				// Item is an ingredient
				if (it != ingredients.end()) {
					int amount_to_delete = std::min(ingredient_map[item.id].total_count, (int)item.count);
					
					item.count -= amount_to_delete;
					ingredient_map[item.id].total_count -= amount_to_delete;

					// If no item is left, set id to void
					if (item.count <= 0) {
						item.id = Item::ID::VOID;
					}
				}
			}

			// Create results vector
			std::size_t results_size = results.size();
			std::vector<Item> crafted_items(results_size);

			for (std::size_t i = 0; i < results_size; i++) {
				crafted_items[i] = Item(results[i].id, results[i].count * to_craft);
			}

			return crafted_items;
		}

		LogWarn("Weird logical error");

		// ...
		return {};
	}

	Recipe Recipe::GetRecipe(const ID& id)
	{
		return m_Recipes.at((uint16_t)id);
	}
}
