#pragma once

#include "Core.h"

namespace Game {
	class TextureManager {
	public:
		static constexpr int PARTICLE_SIZE = 8;

		static Ref(Vivium::TextureAtlas) game_atlas;
		static Ref(Vivium::TextureAtlas) particle_atlas;

		static void Init();
	};
}
