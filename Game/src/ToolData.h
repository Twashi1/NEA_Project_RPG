#pragma once

#include "Core.h"

namespace Game {
	// TODO: streamable
	/// <summary>
	/// Just an inner class for the type and properties of each tool
	/// </summary>
	class Tool {
	public:
		enum class Type {
			VOID,
			PICKAXE,
			MAX
		};

		/// <summary>
		/// Store properties for each tool - the type it is good at breaking, and the speed scale
		/// </summary>
		struct Properties {
			Type type = Type::VOID;
			float scale = 1.0f; // 2 -> indicates mines 2x as fast

			Properties(const Type& type, float scale);
			Properties() = default;
		};
	};
}
