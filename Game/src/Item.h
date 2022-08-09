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
	};

	struct ItemDropData : Vivium::IStreamable {
		typedef std::unordered_map<float, Item::ID> DropTable_t;

		DropTable_t drop_table;

		ItemDropData();
		ItemDropData(const DropTable_t& drop_table);

		void Write(Vivium::Serialiser& s) const override;
		void Read(Vivium::Serialiser& s) override;
	};
}
