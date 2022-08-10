#pragma once

#include <Vivium.h>

namespace Game {
	class Item {
	public:
		enum class ID : uint16_t {
			VOID,
			AMETHYST_CRYSTAL,
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
		private:
			unsigned int m_Sum = 0;

		public:
			typedef std::unordered_map<float, Item::ID> DropTable_t;

			DropTable_t drop_table;

			DropData();
			DropData(const DropTable_t& drop_table);

			Item::ID GetRandomDrop();

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
		// TODO: might need to go for the nuclear solution and just implement velocity and acceleration
		Ref(Vivium::Body) m_Body;

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
