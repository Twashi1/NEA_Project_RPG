#pragma once

#include "TextureManager.h"
#include "ToolData.h"

namespace Game {
	class Item : Vivium::StreamablePOD {
	/// <summary>
	/// Data representation for an item, stores an ID and a count
	/// </summary>
	public:
		enum class ID : uint16_t {
			VOID,
			/* Crystals */
			AMETHYST_CRYSTAL,
			EMERALD_CRYSTAL,
			RUBY_CRYSTAL,
			SAPPHIRE_CRYSTAL,
			TOPAZ_CRYSTAL,
			/* Pickaxes */
			AMETHYST_PICKAXE,
			EMERALD_PICKAXE,
			RUBY_PICKAXE,
			SAPPHIRE_PICKAXE,
			TOPAZ_PICKAXE,
			/* Swords */
			AMETHYST_SWORD,
			EMERALD_SWORD,
			RUBY_SWORD,
			SAPPHIRE_SWORD,
			TOPAZ_SWORD,
			/* Wands */
			AMETHYST_WAND,
			EMERALD_WAND,
			RUBY_WAND,
			SAPPHIRE_WAND,
			TOPAZ_WAND,
			/* Misc */
			LOG,
			BERRIES,
			MAX
		};

		struct Properties : Vivium::Streamable {
		/// <summary>
		/// Properties for each item id
		/// </summary>
		public:
			std::string name;					// Pretty name
			bool isStackable;					// Should an item stack 999, or 1 per slot
			bool displayMultiple;				// Should an item display multiple copies of itself if there are >1 of it
			bool isHandEquipable;				// Is an item equipable in the player's hand
			Vivium::Vector2<int> atlas_index;   // Atlas index for the item's sprite
			Tool::Properties tool_props;		// Tool properties (if applicable)

			Properties(std::string name, bool isStackable, bool displayMultiple, bool isHandEquipable, Vivium::Vector2<int> atlas_index, Tool::Properties tool_props);

			// Serialiser methods
			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		struct DropData : Vivium::Streamable {
		/// <summary>
		/// Drop data for a given item - for block breaking
		/// </summary>
		public:
			Item::ID id;
			uint16_t min_count;
			uint16_t max_count;

			DropData(const Item::ID& id, const uint16_t& min_count, const uint16_t& max_count);
			DropData(const Item::ID& id, const uint16_t& count);

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		// TODO: should be other way round, list of drop data to weight
		struct DropTable : Vivium::Streamable {
		/// <summary>
		/// Potential drops for each tile, mapping a weight to each of the potential drops
		/// </summary>
		private:
			unsigned int m_Sum = 0;

		public:
			typedef std::unordered_map<float, std::vector<DropData>> DropTableMap_t;

			DropTableMap_t drop_table;

			DropTable();
			DropTable(const DropTableMap_t& drop_table);

			// Get a random drop (or list of drops) from this drop table
			std::vector<Item> GetRandomDrop();

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		// Get for properties
		static Properties GetProperties(const Item::ID& id);
		static std::string GetName(const Item::ID& id);
		static bool GetIsStackable(const Item::ID& id);
		static bool GetDisplayMultiple(const Item::ID& id);
		static bool GetIsHandEquipable(const Item::ID& id);
		static Tool::Properties GetToolProperties(const Item::ID& id);
		static Vivium::Vector2<int> GetAtlasIndex(const Item::ID& id);

		static constexpr uint16_t STACK_LIMIT = 999;
		Item::ID id;
		uint16_t count;

		Item();
		Item(Item&& other) noexcept;
		Item(const Item& other);
		Item(const Item::ID& id, const uint16_t& count);

		Item& operator=(const Item& other);

		friend std::ostream& operator<<(std::ostream& os, const Item::ID& id) {
			os << Item::GetName(id);
			return os;
		}

		friend std::ostream& operator<<(std::ostream& os, const Item& item) {
			os << "{" << Item::GetName(item.id) << ", " << item.count << "}";
			return os;
		}

	private:
		// Stores properties for each item id
		static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
	};

	class Inventory;

	class FloorItem : Vivium::Streamable {
	/// <summary>
	/// Represents an item that is inside the game world, resting on the ground, waiting to be picked up
	/// </summary>
	private:
		Item m_ItemData;
		std::shared_ptr<Vivium::Quad> m_Quad;
		// TODO: naming conventions
		// TODO: use body
		Vivium::Vector2<float> velocity;
		Vivium::Vector2<float> acceleration;

		Vivium::Timer m_Timer; // For physics only!!
		float m_RemainingMovingTime;
		bool m_InitialMotionEnded = false;

	public:
		static constexpr float DESPAWN_TIME = 600.0f; // TODO: implement despawning
		static constexpr float PICKUP_RANGE = 100.0f;
		static constexpr float MAGNET_RANGE = 230.0f;
		static constexpr float MAGNET_SPEED = 50000.0f;
		static Vivium::Shader* floor_shader;

		/// <summary>
		/// Create floor shader
		/// </summary>
		static void Init();
		/// <summary>
		/// Delete floor shader
		/// </summary>
		static void Terminate();

		/// <summary>
		/// Get the pure data representation of the item (just id and count)
		/// </summary>
		/// <returns></returns>
		const Item& GetItemData() const;
		const std::shared_ptr<Vivium::Quad> GetQuad() const;

		/// <summary>
		/// Update motion of item
		/// </summary>
		void Update();
		
		FloorItem();
		FloorItem(const FloorItem& other);
		FloorItem(const Item& item_data, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float> dim);

		// TODO: implement
		bool CheckDespawned();

		// Serialiser method
		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;

		// TODO: unused? preferring formatter now
		friend std::ostream& operator<<(std::ostream& os, const FloorItem& object) {
			os << "{" << object.m_ItemData << ", at " << object.m_Quad->GetCenter() << "}";

			return os;
		}

		friend Inventory;
	};
}

namespace std {
	// For printing
	template <>
	struct formatter<Game::Item::ID> : formatter<string> {
		auto format(Game::Item::ID id, format_context& ctx) {
			return formatter<string>::format(
				Game::Item::GetName(id), ctx);
		}
	};

	template <>
	struct formatter<Game::Item> : formatter<string> {
		auto format(Game::Item item, format_context& ctx) {
			return formatter<string>::format(
				std::format("{{{}, {}}}", item.id, item.count), ctx);
		}
	};

	template <>
	struct formatter<Game::FloorItem> : formatter<string> {
		auto format(Game::FloorItem item, format_context& ctx) {
			return formatter<string>::format(
				std::format("{{{}, {}}}", item.GetItemData(), item.GetQuad()->GetCenter()), ctx);
		}
	};
}

