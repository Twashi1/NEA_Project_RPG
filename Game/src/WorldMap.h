#pragma once

#include "Tile.h"

namespace Game {
	class World;

	class WorldMap {
	private:
		static std::array<Vivium::RGBColor, (uint16_t)Tile::ID::MAX> m_TileColors;
		static Vivium::Shader* m_MapShader;

		Vivium::Vector2<int> m_MapSize;

		std::size_t m_TextureSize = 0;
		uint8_t* m_PixelData = nullptr;

	public:
		static void Init();
		static void Terminate();

		WorldMap(const Vivium::Vector2<int>& map_size);
		~WorldMap();

		void GenerateFullMap(const Vivium::Vector2<float>& player_pos, World& world);
		void Render(const Vivium::Vector2<float>& pos);
	};
}
