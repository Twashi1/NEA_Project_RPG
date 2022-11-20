#pragma once

#include "Tile.h"

namespace Game {
	class World;

	/// <summary>
	/// Displays a world map in the top right corner of the screen
	/// </summary>
	class WorldMap {
	private:
		// Maps a color to each tile id
		static std::array<Vivium::RGBColor, (uint16_t)Tile::ID::MAX> m_TileColors;
		static Vivium::Shader* m_MapShader;

		// Size of the map - how many tiles it shows
		Vivium::Vector2<int> m_MapSize;

		std::size_t m_TextureSize = 0;
		uint8_t* m_PixelData = nullptr;

		/// <summary>
		/// Load the pixel data for a given region into the map
		/// </summary>
		/// <param name="region_index"></param>
		/// <param name="world"></param>
		/// <param name="left"></param>
		/// <param name="right"></param>
		/// <param name="bottom"></param>
		/// <param name="top"></param>
		void m_LoadPixelDataFromRegion(const Vivium::Vector2<int>& region_index, World& world, int left, int right, int bottom, int top);

	public:
		static void Init();
		static void Terminate();

		WorldMap(const Vivium::Vector2<int>& map_size);
		~WorldMap();

		// TODO: rename to Update
		/// <summary>
		/// Generate the full map given the player position
		/// </summary>
		/// <param name="player_pos"></param>
		/// <param name="world"></param>
		void GenerateFullMap(const Vivium::Vector2<float>& player_pos, World& world);
		/// <summary>
		/// Render the map
		/// </summary>
		/// <param name="pos"></param>
		void Render(const Vivium::Vector2<float>& pos);
	};
}
