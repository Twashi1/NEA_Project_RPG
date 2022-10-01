#include "TextureManager.h"
#include "Tile.h"

namespace Game {
	Ref(Vivium::TextureAtlas) TextureManager::game_atlas = nullptr;
	Ref(Vivium::TextureAtlas) TextureManager::particle_atlas = nullptr;

	void TextureManager::Init() {
		game_atlas = std::make_shared<Vivium::TextureAtlas>("game_atlas.png", Vivium::Vector2<int>(Tile::SPRITE_SIZE, Tile::SPRITE_SIZE));
		particle_atlas = std::make_shared<Vivium::TextureAtlas>("particles.png", Vivium::Vector2<int>(PARTICLE_SIZE, PARTICLE_SIZE));
	}
}
