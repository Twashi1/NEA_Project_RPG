#pragma once

#include "World.h"

namespace Game {
	// TODO: move rendering information to here
	class Inventory {
	public:
		enum class Slot : uint8_t {
			INVALID,
			HOTBAR_0, HOTBAR_1, HOTBAR_2, HOTBAR_3, HOTBAR_4, HOTBAR_5, HOTBAR_6, HOTBAR_7, HOTBAR_8, HOTBAR_9,
			INV_0,  INV_1,  INV_2,  INV_3,  INV_4,  INV_5,  INV_6,  INV_7,  INV_8,  INV_9,
			INV_10, INV_11, INV_12, INV_13, INV_14, INV_15, INV_16, INV_17, INV_18, INV_19,
			INV_20, INV_21, INV_22, INV_23, INV_24, INV_25, INV_26, INV_27, INV_28, INV_29,
			INV_30, INV_31, INV_32, INV_33, INV_34, INV_35, INV_36, INV_37, INV_38, INV_39,
			INV_40, INV_41, INV_42, INV_43, INV_44, INV_45, INV_46, INV_47, INV_48, INV_49,
			INV_50, INV_51, INV_52, INV_53,
			CURSOR_0, MAX
		};

		enum class ID : uint8_t {
			SMALL,
			LARGE,
			HOTBAR, // TODO: Implement
			MAX
		};

	private:
		struct Properties {
			Slot start_slot;
			uint8_t inventory_size;
			Vivium::Vector2<int> top_left_index;
			Vivium::Vector2<int> bottom_right_index;
			Vivium::Vector2<float> sprite_size;
			std::unordered_map<Slot, Vivium::Vector2<float>> slot_coords;

			Properties(
				const Slot& start_slot, const uint8_t& inventory_size,
				const Vivium::Vector2<int>& top_left_index, const Vivium::Vector2<int>& bottom_right_index,
				const Vivium::Vector2<float>& sprite_size,
				const std::unordered_map<Slot, Vivium::Vector2<float>>& slot_coords
			);
		};

		static const std::array<Properties, (uint8_t)ID::MAX> m_Properties;

		typedef std::array<Item, (uint8_t)Slot::MAX> InventoryData_t;

		InventoryData_t m_InventoryData; // Small inventory largest slot is INV_26, large inventory is INV_53
		ID m_InventoryID;
		Vivium::Quad* m_InventoryQuad;
		static Vivium::Shader* m_InventoryShader;
		static Vivium::TextureAtlas* m_InventoryAtlas;

		static constexpr float m_InventorySpriteScale = 8.0f;
		
		void m_RenderItems();

	public:
		static Properties GetProperties(const Inventory::ID& id);

		static void Init();
		static void Terminate();

		bool AddItem(const Item& item);

		void SetPos(const Vivium::Vector2<float>& new_pos);
		Vivium::Vector2<float> GetPos() const;

		void Update(const Vivium::Vector2<float>& player_pos, World& world);
		void Render();

		Inventory(const ID& inventory_id);
		~Inventory();
	};
}