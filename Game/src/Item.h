#pragma once

#include "TextureManager.h"

namespace Game {
	class Item : Vivium::StreamablePOD {
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

		// Global properties per item
		struct Properties : Vivium::Streamable {
		public:
			std::string name;
			bool isStackable;
			bool displayMultiple;
			bool isHandEquipable;
			Vivium::Vector2<int> atlas_index;

			Properties(std::string name, bool isStackable, bool displayMultiple, bool isHandEquipable, Vivium::Vector2<int> atlas_index);

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		struct DropData : Vivium::Streamable {
		public:
			Item::ID id;
			uint16_t min_count;
			uint16_t max_count;

			DropData(const Item::ID& id, const uint16_t& min_count, const uint16_t& max_count);
			DropData(const Item::ID& id, const uint16_t& count);

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		// Stored per tile, maps weights to list of potential drops
		struct DropTable : Vivium::Streamable {
		private:
			unsigned int m_Sum = 0;

		public:
			typedef std::unordered_map<float, std::vector<DropData>> DropTableMap_t;

			DropTableMap_t drop_table;

			DropTable();
			DropTable(const DropTableMap_t& drop_table);

			std::vector<Item> GetRandomDrop();

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		static Properties GetProperties(const Item::ID& id);
		static std::string GetName(const Item::ID& id);
		static bool GetIsStackable(const Item::ID& id);
		static bool GetDisplayMultiple(const Item::ID& id);
		static bool GetIsHandEquipable(const Item::ID& id);
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
		static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
	};

	class Inventory;

	class FloorItem : Vivium::Streamable {
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

		static void Init();
		static void Terminate();

		const Item& GetItemData() const;
		const std::shared_ptr<Vivium::Quad> GetQuad() const;

		void Update();
		
		FloorItem();
		FloorItem(const FloorItem& other);
		FloorItem(const Item& item_data, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float> dim);

		bool CheckDespawned();

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;

		friend std::ostream& operator<<(std::ostream& os, const FloorItem& object) {
			os << "{" << object.m_ItemData << ", at " << object.m_Quad->GetCenter() << "}";

			return os;
		}

		friend Inventory;
	};
}

namespace std {
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

