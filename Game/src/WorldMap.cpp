#include "WorldMap.h"
#include "World.h"

namespace Game {
	std::array<Vivium::RGBColor, (uint16_t)Tile::ID::MAX> WorldMap::m_TileColors{
		Vivium::RGBColor::BLACK,
		Vivium::RGBColor("#b67a48"), // GROUND
		Vivium::RGBColor("#44a318"), // GRASS
		Vivium::RGBColor("#c5941d"), // TREE_0
		Vivium::RGBColor("#c5941d"), // TREE_1
		Vivium::RGBColor("#c8a322"), // SAND
		Vivium::RGBColor("#5e86c5"), // WATER
		Vivium::RGBColor("#7aff66"), // BUSH
		Vivium::RGBColor("#7aff66"), // BUSH_FRUIT
		Vivium::RGBColor("#790098"), // AMETHYST_NODE
		Vivium::RGBColor("#1f9800"), // EMERALD_NODE
		Vivium::RGBColor("#ff411f"), // RUBY_NODE
		Vivium::RGBColor("#4cc0ff"), // SAPPHIRE_NODE
		Vivium::RGBColor("#fcff4c"), // TOPAZ_NODE,
		Vivium::RGBColor("#1e8a48"), // CACTUS_SMALL
		Vivium::RGBColor("#1e8a48"), // CACTUS_TALL0
		Vivium::RGBColor("#1e8a48"), // CACTUS_TALL1
		Vivium::RGBColor("#ff0400"), // CACTUS_SMALL
		Vivium::RGBColor::BLACK,	 // MOSSY_DEBRIS
		Vivium::RGBColor::BLACK,	 // ROCKY_DEBRIS
		Vivium::RGBColor::BLACK,	 // PINK_FLOWER
		Vivium::RGBColor::BLACK		 // BLUE_FLOWER
	};

	Vivium::Shader* WorldMap::m_MapShader = nullptr;

	void WorldMap::Init()
	{
		m_MapShader = new Vivium::Shader("static_texture_vertex", "texture_frag");
	}
	
	void WorldMap::Terminate()
	{
		delete m_MapShader;
	}

	WorldMap::WorldMap(const Vivium::Vector2<int>& map_size)
		: m_MapSize(map_size)
	{
		m_TextureSize = m_MapSize.x * m_MapSize.y * 3;
		m_PixelData = new uint8_t[m_TextureSize];
	}

	WorldMap::~WorldMap()
	{
		delete[] m_PixelData, m_TextureSize;
	}

	void WorldMap::m_LoadPixelDataFromRegion(const Vivium::Vector2<int>& region_index, World& world, int left, int right, int bottom, int top)
	{
		Region& region = world.m_LoadRegion(region_index);

		Vivium::Vector2<int> region_offset = region_index * Region::LENGTH;

		for (int y = 0; y < Region::LENGTH; y++) {
			int world_y = y + region_offset.y;

			// Check y position is within the bounds of what we want rendered
			if (world_y >= bottom && world_y < top) {
				for (int x = 0; x < Region::LENGTH; x++) {
					int world_x = x + region_offset.x;

					// Check x position is within the bounds of what we want rendered
					if (world_x >= left && world_x < right) {
						// Check if it is a border tile
						bool isBorderTile = (world_x == left) || (world_x == right - 1)
							|| (world_y == bottom) || (world_y == top - 1);

						// Get tile from index
						Tile& tile = region.Index(x, y);

						Vivium::RGBColor color = Vivium::RGBColor::BLACK;

						if (!isBorderTile) {
							for (int i = 2; i >= 0; i--) {
								Tile::ID& tile_id = tile.Index(i);

								if (tile_id != Tile::ID::VOID) {
									color = m_TileColors.at((uint16_t)tile_id);

									if (color != Vivium::RGBColor::BLACK) {
										break;
									}
								}
							}
						}

						uint8_t red		= static_cast<uint8_t>(color.r * 255.0f);
						uint8_t green	= static_cast<uint8_t>(color.g * 255.0f);
						uint8_t blue	= static_cast<uint8_t>(color.b * 255.0f);

						int x_index = world_x - left;
						int y_index = world_y - bottom;
						int map_pixel_index = x_index + (y_index * m_MapSize.x);
						int map_color_index = map_pixel_index * 3;

						m_PixelData[map_color_index] = red;
						m_PixelData[map_color_index + 1] = green;
						m_PixelData[map_color_index + 2] = blue;
					}
				}
			}
		}
	}
	
	void WorldMap::GenerateFullMap(const Vivium::Vector2<float>& player_pos, World& world)
	{
		Vivium::Vector2<int> player_tile_pos = player_pos / World::PIXEL_SCALE;

		int left = player_tile_pos.x - m_MapSize.x / 2;
		int right = player_tile_pos.x + m_MapSize.x / 2;
		int bottom = player_tile_pos.y - m_MapSize.y / 2;
		int top = player_tile_pos.y + m_MapSize.y / 2;

		float reg_scale = static_cast<float>(Region::LENGTH);
		int reg_left =		std::floor(left / reg_scale);
		int reg_right =		std::floor(right / reg_scale);
		int reg_bottom =	std::floor(bottom / reg_scale);
		int reg_top =		std::floor(top / reg_scale);

		// NOTE: Assuming we're max gonna have 4 regions within render distance of player
		// Load 4 regions for player
		Vivium::Vector2<int> tl(reg_left, reg_top);
		Vivium::Vector2<int> tr(reg_right, reg_top);
		Vivium::Vector2<int> bl(reg_left, reg_bottom);
		Vivium::Vector2<int> br(reg_right, reg_bottom);

		m_LoadPixelDataFromRegion(tl, world, left, right, bottom, top);
		m_LoadPixelDataFromRegion(tr, world, left, right, bottom, top);
		m_LoadPixelDataFromRegion(bl, world, left, right, bottom, top);
		m_LoadPixelDataFromRegion(br, world, left, right, bottom, top);
	}

	void WorldMap::Render(const Vivium::Vector2<float>& pos)
	{
		// Create texture from texture data
		Vivium::Texture map_texture(m_PixelData, m_MapSize.x, m_MapSize.y, Vivium::Texture::Format::RGB);

		// Create quad
		Vivium::Quad quad(pos, m_MapSize * 2);

		Vivium::Renderer::Submit(&quad, m_MapShader, &map_texture);
	}
}
