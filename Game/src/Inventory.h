#pragma once

#include "World.h"

namespace Game {
	class Recipe;

	class Inventory : Vivium::Streamable {
	/// <summary>
	/// Handles storing of items, moving items inside inventory, and visual GUI for items
	/// </summary>
	public:
		enum class Slot : uint8_t {
			/// <summary>
			/// For indexing the inventory in a slightly more human-readable way
			/// </summary>
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
			/// <summary>
			/// Type of inventory (dictates size, slot indices used, etc.)
			/// </summary>
			SMALL,
			LARGE,  // TODO: implement
			SMALL_WITH_HOTBAR,
			CRAFTING,
			MAX
		};

		using slot_base_t = std::underlying_type<Slot>::type;
		using id_base_t = std::underlying_type<ID>::type;

		class Data : Vivium::Streamable {
		/// <summary>
		/// ADT for encapsulating inventory data so it can be indexed with the Slot enum
		/// </summary>
		private:
			// Actual item data
			std::array<Item, (slot_base_t)Slot::MAX> m_Data;
			// Start index for iteration (different inventories may start from different slots)
			slot_base_t m_StartIndex;
			// Size of inventory (how many items it can store)
			slot_base_t m_Size;

		public:
			// https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
			struct Iterator {
			/// <summary>
			/// Enables for each loops, and all the STD functions that operate on iterators
			/// </summary>
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
			// NOTE: following 2 are not requires for a container but useful for this specific implementation
			inline slot_base_t start_index() const { return m_StartIndex; }
			inline slot_base_t end_index() const { return m_StartIndex + m_Size; }

			// Indexing with both base integral type for enum and using Slot enum itself
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

			// Serialiser methods
			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

	protected:
		// TODO: needs more cleanup
		// TODO: tile scale should reduce if theres more items
		static constexpr float s_BGScale = 0.4f;
		static constexpr float s_ItemScale = 0.3f;
		// For some items, having multiple of them in your inventory should display multiple copies
		// because I thought it'd look kinda nice, so this stores the set offset values for the extra
		// copies of the texture that should be displayed
		static const Vivium::Vector2<float> s_ItemCountOffsets[3];
		// Max offsets used for bounds of each item (used when picking up/re-organising items)
		static const float s_MaxOffsetWidth;
		static const float s_MaxOffsetHeight;

		struct Properties {
			/// <summary>
			/// Per inventory type properties
			/// </summary>
			Slot start_slot;
			uint8_t inventory_size;
			std::unordered_map<Slot, Vivium::Vector2<float>> slot_coords;

			Properties(
				const Slot& start_slot, const uint8_t& inventory_size,
				const std::unordered_map<Slot, Vivium::Vector2<float>>& slot_coords
			);
		};

		// Store properties for each inventory type
		static const std::array<Properties, (uint8_t)ID::MAX> m_Properties;

		Data m_InventoryData; // NOTE: Small inventory largest slot is INV_26, large inventory is INV_53
		ID m_InventoryID;

		// Storing item counts for each item to speed up checking if a recipe has its required ingredients
		typedef std::unordered_map<Item::ID, int> ItemCountMap_t;
		ItemCountMap_t m_ItemCounts;
		Slot m_SelectedSlot = Slot::INVALID;

		static Vivium::Shader* m_InventoryShader;
		static Vivium::Shader* m_TextShader;
		static Vivium::Shader* m_ItemShader;
		static Vivium::Text* m_TextObject;

		// TODO: bad (make it have a set ratio compared to window size
		static constexpr float m_InventorySpriteScale = 8.0f;
		
		/// <summary>
		/// Gets the next free inventory slot for when items need to be added, and can't stack anywhere else
		/// </summary>
		/// <returns></returns>
		Slot m_GetNextOpenSlot();
		
		/// <summary>
		/// Render the inventory background (the red/gold squares where the items are displayed on top of)
		/// (for the given range)
		/// </summary>
		/// <param name="start_slot"></param>
		/// <param name="length"></param>
		void m_RenderBackground(const Slot& start_slot, uint8_t length);
		/// <summary>
		/// Render the items stored in the inventory
		/// (for the given range)
		/// </summary>
		/// <param name="start_slot"></param>
		/// <param name="length"></param>
		void m_RenderItems(const Slot& start_slot, uint8_t length);
		/// <summary>
		/// Render an individual item by adding to a batch
		/// </summary>
		/// <param name="batch"></param>
		/// <param name="item"></param>
		/// <param name="pos"></param>
		/// <param name="size"></param>
		void m_RenderItem(
			Vivium::Batch* batch,
			const Item& item,
			const Vivium::Vector2<float>& pos,
			const float& size
		);

		/// <summary>
		/// Check if an item in the inventory has been clicked on/moved, and perform appropriate operations
		/// e.g. Moving item from one slot to another, or switching items
		/// </summary>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="item_slot"></param>
		void m_UpdateItem(float x, float y, float width, float height, const Slot& item_slot);
		/// <summary>
		/// Check all floor items nearby, and if close enough, delete from world and add to inventory
		/// </summary>
		/// <param name="player_pos"></param>
		/// <param name="world"></param>
		void m_UpdateFloorItems(const Vivium::Vector2<float>& player_pos, World* world);

		/// <summary>
		/// Update the item counts by iterating entire inventory
		/// </summary>
		void m_UpdateItemCounts();
		/// <summary>
		/// Change the item count for a specific item
		/// </summary>
		/// <param name="id"></param>
		/// <param name="change"></param>
		void m_ChangeItemCount(const Item::ID& id, int change);

	public:
		/// <summary>
		/// Get properties for a specific inventory by ID
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static Properties GetProperties(const Inventory::ID& id);

		/// <summary>
		/// Initialise inventory shaders
		/// </summary>
		static void Init();
		/// <summary>
		/// Delete inventory shaders
		/// </summary>
		static void Terminate();

		// Position of inventory on screen (is shared ptr so can be centered by Vivium panel)
		std::shared_ptr<Vivium::Vector2<float>> inventory_pos;
		
		// Checks if an inventory has no empty slots
		// NOTE: a full inventory does not necessarily mean there is no space for a specific item, since items can stack
		bool IsFull() const;

		int AddItem(const Item& item);
		std::vector<int> AddItems(const std::vector<Item>& items);
		// TODO: this should be const with respect to class?
		const Item& GetItem(const Slot& slot);
		// TODO: this should definitely be private
		Data& GetData();

		// Get slot enum for the selected slot
		Slot GetSelectedSlot() const;
		// Change what the selected slot is
		void SetSelectedSlot(const Slot& slot);

		// Get the item count for a specific item
		int GetItemCount(const Item::ID& id) const;

		/// <summary>
		/// Get all items in a range
		/// </summary>
		/// <param name="start_slot"></param>
		/// <param name="length"></param>
		/// <returns></returns>
		std::vector<Item> GetItems(const Slot& start_slot, uint8_t length);
		/// <summary>
		/// Set items in a range
		/// </summary>
		/// <param name="items"></param>
		/// <param name="slot"></param>
		void SetItems(const std::vector<Item>& items, const Slot& slot);

		/// <summary>
		/// Update a specific range of the inventory
		/// </summary>
		/// <param name="start_slot"></param>
		/// <param name="length"></param>
		/// <param name="player_pos"></param>
		/// <param name="world"></param>
		void Update(const Slot& start_slot, uint8_t length, const Vivium::Vector2<float>& player_pos, World* world);
		/// <summary>
		/// Update entire inventory (getting the actual range from the inventory properties)
		/// </summary>
		/// <param name="player_pos"></param>
		/// <param name="world"></param>
		void Update(const Vivium::Vector2<float>& player_pos, World* world);
		/// <summary>
		/// Render specific range of inventory
		/// </summary>
		/// <param name="start_slot"></param>
		/// <param name="length"></param>
		void Render(const Slot& start_slot, uint8_t length);
		/// <summary>
		/// Render entire inventory (getting the actual range from the inventory properties)
		/// </summary>
		void Render();

		Inventory();
		Inventory(const ID& inventory_id);
		~Inventory();

		// Serialiser methods
		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;

		friend Recipe;
	};
}

namespace std {
	// Debug for printing inventory
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
