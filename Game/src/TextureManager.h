#pragma once

#include "Core.h"

namespace Game {
	class TextureManager {
	public:
		static Ref(Vivium::TextureAtlas) game_atlas;

		static void Init();
	};
}
