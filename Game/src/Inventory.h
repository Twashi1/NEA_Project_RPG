#pragma once

#include "World.h"

namespace Game {
	class Recipe;

	class Inventory : Vivium::IStreamable {
	public:
		enum class Slot : uint8_t {
			INVALID,
			INV_0,  INV_1,  INV_2,  INV_3,  INV_4,  INV_5,  INV_6,  INV_7,  INV_8,  INV_9,
			INV_10, INV_11, INV_12, INV_13, INV_14, INV_15, INV_16, INV_17, INV_18, INV_19,
			INV_20, INV_21, INV_22, INV_23, INV_24, INV_25, INV_26, INV_27, INV_28, INV_29,
			INV_30, INV_31, INV_32, INV_33, INV_34, INV_35, INV_36, INV_37, INV_38, INV_39,
			INV_40, INV_41, INV_42, INV_43, INV_44, INV_45, INV_46, INV_47, INV_48, INV_49,
			INV_50, INV_51, INV_52, INV_53,
			CRAFT_0, CRAFT_1, CRAFT_2, CRAFT_3, CRAFT_4, CRAFT_5, CRAFT_6, CRAFT_7, CRAFT_8,
			CURSOR_0, MAX
		};

		enum class ID : uint8_t {
			SMALL,
			LARGE,  // TODO: implement
			SMALL_WITH_HOTBAR,
			CRAFTING,
			MAX
		};

		using slot_base_t = std::underlying_type<Slot>::type;
		using id_base_t = std::underlying_type<ID>::type;

		class Data : Vivium::IStreamable {
		private:
			std::array<Item, (slot_base_t)Slot::MAX> m_Data;
			slot_base_t m_StartIndex;
			slot_base_t m_Size;

		public:
			// https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
			struct Iterator {
			public:
				using iterator_category = std::forward_iterator_tag;
				using difference_type = std::ptrdiff_t;
				using value_type = Item;
				using pointer = Item*;
				using reference = Item&;

			private:
				pointer m_ptr;

			public:
				Iterator(pointer ptr) : m_ptr(ptr) {}

				reference operator*() const { return *m_ptr; }
				pointer operator->() { return m_ptr; }

				Iterator& operator++() { m_ptr++; return *this; }
				Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }

				friend bool operator==(const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; }
				friend bool operator!=(const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; }
			};

			Data();
			Data(const ID& id);
			Data(const Data& other);
			Data(Data&& other) noexcept;

			inline slot_base_t size() const { return m_Size; }
			inline slot_base_t start_index() const { return m_StartIndex; }
			inline slot_base_t end_index() const { return m_StartIndex + m_Size; }

			inline Item& operator[](const slot_base_t& index) { return m_Data[index]; }
			inline Item& operator[](const Slot& index) { return m_Data[(slot_base_t)index]; }
			inline Item& at(const slot_base_t& index) { return m_Data.at(index); }
			inline Item& at(const Slot& index) { return m_Data.at((slot_base_t)index); }
			inline const Item& operator[](const slot_base_t& index) const { return m_Data[index]; }
			inline const Item& operator[](const Slot& index) const { return m_Data[(slot_base_t)index]; }
			inline const Item& at(const slot_base_t& index) const { return m_Data.at(index); }
			inline const Item& at(const Slot& index) const { return m_Data.at((slot_base_t)index); }

			Iterator begin();
			Iterator end();

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

	protected:
		// TODO: needs more cleanup
		// TODO: tile scale should reduce if theres more items
		static constexpr float s_BGScale = 0.4f;
		static constexpr float s_ItemScale = 0.3f;
		static const Vivium::Vector2<float> s_ItemCountOffsets[3];
		// Max offsets used for bounds of each item (used when picking up/re-organising items)
		static const float s_MaxOffsetWidth;
		static const float s_MaxOffsetHeight;

		struct Properties {
			Slot start_slot;
			uint8_t inventory_size;
			std::unordered_map<Slot, Vivium::Vector2<float>> slot_coords;

			Properties(
				const Slot& start_slot, const uint8_t& inventory_size,
				const std::unordered_map<Slot, Vivium::Vector2<float>>& slot_coords
			);
		};

		static const std::array<Properties, (uint8_t)ID::MAX> m_Properties;

		Data m_InventoryData; // Small inventory largest slot is INV_26, large inventory is INV_53
		ID m_InventoryID;
		typedef std::unordered_map<Item::ID, int> ItemCountMap_t;
		ItemCountMap_t m_ItemCounts;
		Slot m_SelectedSlot = Slot::INVALID;

		static Vivium::Shader* m_InventoryShader;
		static Vivium::Shader* m_TextShader;
		static Vivium::Shader* m_ItemShader;
		static Vivium::Text* m_TextObject;

		static constexpr float m_InventorySpriteScale = 8.0f;

		Slot m_GetNextOpenSlot();
		
		void m_RenderBackground(const Slot& start_slot, uint8_t length);
		void m_RenderItems(const Slot& start_slot, uint8_t length);
		void m_RenderItem(
			Vivium::Batch* batch,
			const Item& item,
			const Vivium::Vector2<float>& pos,
			const float& size
		);

		void m_UpdateItem(float x, float y, float width, float height, const Slot& item_slot);
		void m_UpdateFloorItems(const Vivium::Vector2<float>& player_pos, World* world);

		void m_UpdateItemCounts();
		void m_ChangeItemCount(const Item::ID& id, int change);

	public:
		static Properties GetProperties(const Inventory::ID& id);

		static void Init();
		static void Terminate();

		Ref(Vivium::Vector2<float>) inventory_pos;

		bool IsFull() const;

		int AddItem(const Item& item);
		std::vector<int> AddItems(const std::vector<Item>& items);
		const Item& GetItem(const Slot& slot);
		Data& GetData();

		Slot GetSelectedSlot() const;
		void SetSelectedSlot(const Slot& slot);

		int GetItemCount(const Item::ID& id) const;

		std::vector<Item> GetItems(const Slot& start_slot, uint8_t length);
		void SetItems(const std::vector<Item>& items, const Slot& slot);

		void Update(const Slot& start_slot, uint8_t length, const Vivium::Vector2<float>& player_pos, World* world);
		void Update(const Vivium::Vector2<float>& player_pos, World* world);
		void Render(const Slot& start_slot, uint8_t length);
		void Render();

		Inventory();
		Inventory(const ID& inventory_id);
		~Inventory();

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;

		friend Recipe;
	};
}

namespace std {
	template <>
	struct formatter<Game::Inventory> : formatter<string> {
		auto format(Game::Inventory inven, format_context& ctx) {
			std::stringstream ss;
			ss << "{\n";
			
			Game::Inventory::Data& data = inven.GetData();

			for (Game::Inventory::slot_base_t i = data.start_index(); i < data.end_index(); i++) {
				ss << "    " << data[i] << "\n";
			}

			ss << "}\n";

			return formatter<string>::format(
				ss.str(), ctx);
		}
	};
}
