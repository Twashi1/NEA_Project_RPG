#pragma once

#include <Vivium.h>

namespace Game {
	class Item {
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

		// TODO: should take in a count as well (maybe just map to Item class)
		struct DropTable : Vivium::IStreamable {
		private:
			unsigned int m_Sum = 0;

		public:
			typedef std::unordered_map<float, DropData> DropTableMap_t;

			DropTableMap_t drop_table;

			DropTable();
			DropTable(const DropTableMap_t& drop_table);

			Item GetRandomDrop();

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;
		};

		static Properties GetProperties(const Item::ID& id);
		static std::string GetName(const Item::ID& id);
		static bool GetIsStackable(const Item::ID& id);
		static Vivium::Vector2<int> GetAltasIndex(const Item::ID& id);

		Item::ID id;
		uint16_t count;

		Item(const Item::ID& id, const uint16_t& count);

	private:
		static std::array<Properties, (uint16_t)ID::MAX> m_Properties;
	};

	class FloorItem {
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
		static Vivium::Shader* floor_shader;

		static void Init();
		static void Terminate();

		const Item& GetItemData() const;
		const Ref(Vivium::Quad) GetQuad() const;

		void Update();

		FloorItem(const FloorItem& other);
		FloorItem(const Item& item_data, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float> dim);

		bool CheckDespawned();
	};
}
