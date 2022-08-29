#include "TextureManager.h"

namespace Game {
	Ref(Vivium::TextureAtlas) TextureManager::game_atlas = nullptr;

	void TextureManager::Init() {
		game_atlas = MakeRef(Vivium::TextureAtlas, "game_atlas.png", Vivium::Vector2<int>(32, 32));
	}
}
