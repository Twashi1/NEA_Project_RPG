#pragma once

#include "Core.h"

namespace Game {
	// TODO: streamable
	class Tool {
	public:
		enum class Type {
			VOID,
			PICKAXE,
			MAX
		};

		struct Properties {
			Type type = Type::VOID;
			float scale = 1.0f;

			Properties(const Type& type, float scale);
			Properties() = default;
		};
	};
}
