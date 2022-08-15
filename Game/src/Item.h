#pragma once

#include <Vivium.h>

namespace Game {
	class Item : Vivium::IStreamable {
	public:
		enum class ID : uint16_t {
			VOID,
			AMETHYST_CRYSTAL,
			EMERALD_CRYSTAL,
			RUBY_CRYSTAL,
			SAPPHIRE_CRYSTAL,
			TOPAZ_CRYSTAL,
			LOG,
			MAX
		};

		// Global properties per item
		struct Properties : Vivium::IStreamable {
		public:
			std::string name;
			bool isStackable;
			Vivium::Vector2<int> atlas_index;

			Properties(std::string name, bool isStackable, Vivium::Vector2<int> atlas_index);

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		struct DropData : Vivium::IStreamable {
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
		struct DropTable : Vivium::IStreamable {
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
		static Vivium::Vector2<int> GetAltasIndex(const Item::ID& id);

		static constexpr uint16_t STACK_LIMIT = 999;
		Item::ID id;
		uint16_t count;

		Item();
		Item(Item&& other) noexcept;
		Item(const Item& other);
		Item(const Item::ID& id, const uint16_t& count);

		Item& operator=(const Item& other);

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;

	private:
		static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
	};

	class Inventory;

	class FloorItem : Vivium::IStreamable {
	private:
		Item m_ItemData;
		Ref(Vivium::Quad) m_Quad;
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
		const Ref(Vivium::Quad) GetQuad() const;

		void Update();
		
		FloorItem();
		FloorItem(const FloorItem& other);
		FloorItem(const Item& item_data, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float> dim);

		bool CheckDespawned();

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;

		friend Inventory;
	};
}

namespace std {
	template <>
	struct formatter<Game::Item::ID> : formatter<string> {
		auto format(Game::Item::ID id, format_context& ctx) {
			std::string id_string;

			switch (id) {
			case Game::Item::ID::VOID:				id_string = "Void";				break;
			case Game::Item::ID::AMETHYST_CRYSTAL:	id_string = "Amethyst crystal"; break;
			case Game::Item::ID::EMERALD_CRYSTAL:	id_string = "Emerald crystal";	break;
			case Game::Item::ID::RUBY_CRYSTAL:		id_string = "Ruby crystal";		break;
			case Game::Item::ID::SAPPHIRE_CRYSTAL:	id_string = "Sapphire crystal"; break;
			case Game::Item::ID::TOPAZ_CRYSTAL:		id_string = "Topaz crystal";	break;
			case Game::Item::ID::LOG:				id_string = "Log";				break;
			default:								id_string = "InvalidID";		break;
			}

			return formatter<string>::format(
				id_string, ctx);
		}
	};

	template <>
	struct formatter<Game::Item> : formatter<string> {
		auto format(Game::Item item, format_context& ctx) {
			return formatter<string>::format(
				std::format("{{{}, {}}}", item.id, item.count), ctx);
		}
	};
}

