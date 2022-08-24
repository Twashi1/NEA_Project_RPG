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
		Vivium::RGBColor("#fcff4c")  // TOPAZ_NODE
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
	
	void WorldMap::GenerateFullMap(const Vivium::Vector2<float>& player_pos, World& world)
	{
		Vivium::Vector2<int> player_tile_pos = player_pos / World::PIXEL_SCALE;

		std::size_t count = 0;

		int left = player_tile_pos.x - m_MapSize.x / 2;
		int right = player_tile_pos.x + m_MapSize.x / 2;
		int bottom = player_tile_pos.y - m_MapSize.y / 2;
		int top = player_tile_pos.y + m_MapSize.y / 2;

		Vivium::Vector2<int> tl(left, top);
		Vivium::Vector2<int> bl(left, bottom);
		Vivium::Vector2<int> tr(right, top);
		Vivium::Vector2<int> br(right, bottom);

		// Assuming all tiles are already loaded since we're getting them around the player pos
		for (int y = bottom; y < top; y++) {
			for (int x = left; x < right; x++) {
				Tile* tile = world.GetLoadedTile(x, y);

				Vivium::RGBColor color = Vivium::RGBColor::BLACK;

				bool isBorderTile = (x == left) || (x == right - 1)
					|| (y == bottom) || (y == top - 1);

				if (tile != nullptr && !isBorderTile) {
					Tile::ID highest_tile = tile->GetHighestRealTile();

					color = m_TileColors.at((uint16_t)highest_tile);
				}

				uint8_t red   = static_cast<uint8_t>(color.r * 255.0f);
				uint8_t green = static_cast<uint8_t>(color.g * 255.0f);
				uint8_t blue  = static_cast<uint8_t>(color.b * 255.0f);

				std::size_t index = count * 3;
				m_PixelData[index] = red;
				m_PixelData[index + 1] = green;
				m_PixelData[index + 2] = blue;

				++count;
			}
		}
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
