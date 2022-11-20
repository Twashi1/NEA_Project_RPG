#pragma once

#include "Core.h"

namespace Game {
	/// <summary>
	/// Stores game and particle texture atlases, and has initialise function for creating them
	/// </summary>
	class TextureManager {
	public:
		static constexpr int PARTICLE_SIZE = 8;

		static std::shared_ptr<Vivium::TextureAtlas> game_atlas;
		static std::shared_ptr<Vivium::TextureAtlas> particle_atlas;

		static void Init();
	};
}
