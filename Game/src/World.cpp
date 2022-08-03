#include "World.h"

// TODO: path is complicated

std::string World::PATH = "../Resources/saves/";
std::string World::FILE_EXTENSION = ".data";
std::string World::GENERAL_FILE = "general";
Vivium::VersionNumber World::m_version = Vivium::VersionNumber(0, 0, 1);
World::TileMap_t World::m_tilemap = {};
Vivium::Texture* World::m_tile_atlas = nullptr;
Vivium::Vector2<int> World::m_tile_atlas_size = Vivium::Vector2<int>(0, 0);
Vivium::Vector2<int> World::m_atlas_dim_relative = Vivium::Vector2<int>(0, 0);
Vivium::Shader* World::texture_shader = nullptr;

void World::LoadTextures(const std::string& atlas_file)
{
	constexpr int sprite_size = 32; // TODO

	// Load atlas and various information about atlas
	m_tile_atlas = new Vivium::Texture(atlas_file);
	m_tile_atlas_size = Vivium::Vector2<int>(sprite_size, sprite_size); // TODO
	m_atlas_dim_relative = Vivium::Vector2<int>(m_tile_atlas->width / sprite_size, m_tile_atlas->height / sprite_size);

	// Iterate over value of each tile id
	for (int i = 0; i < (int)Tile::ID::MAX; i++) {
		// Convert it to an index into the atlas
		int y = i / m_atlas_dim_relative.x;
		int x = i - (y * m_atlas_dim_relative.x);
		// Invert y
		y = (m_atlas_dim_relative.y - 1) - y;

		// Add to tilemap
		m_tilemap[(Tile::ID)i] = Vivium::Vector2<int>(x, y);
	}
}

World::World(const std::string& world_name)
	: m_world_name(world_name)
{
	std::string fullpath = PATH + world_name + "/";

	if (std::filesystem::is_directory(fullpath)) {
		// Load world
		m_LoadWorld(fullpath);
	}
	else {
		LogFatal("Directory didn't exist, world hasn't been generated yet?");
	}
}

World::World(const uint32_t& seed, const std::string& world_name)
	: m_world_name(world_name), m_seed(seed)
{
	std::string fullpath = PATH + world_name + "/";

	// Create folder for our world
	std::filesystem::create_directory(fullpath);
	// Generate world
	m_GenWorld(fullpath);

}

Vivium::Vector2<int> World::m_GetRegionIndex(const Vivium::Vector2<int>& pos)
{
	return Vivium::Vector2<int>(
		std::floor((double)pos.x / (double)Region::LENGTH),
		std::floor((double)pos.y / (double)Region::LENGTH)
	);
}

Vivium::Vector2<int> World::m_GetRegionIndex(int x, int y)
{
	return Vivium::Vector2<int>(
		std::floor((double)x / (double)Region::LENGTH),
		std::floor((double)y / (double)Region::LENGTH)
		);
}

std::string World::m_ToRegionName(const Vivium::Vector2<int>& index)
{
	return std::format("region{}_{}{}", index.x, index.y, FILE_EXTENSION);
}

void World::m_LoadRegions(const Vivium::Vector2<int>& center, int radius)
{
	// TODO: doesn't delete old regions
	for (int y = center.y - radius; y < center.y + radius + 1; y++) {
		for (int x = center.x - radius; x < center.x + radius + 1; x++) {
			// Get index
			Vivium::Vector2<int> index(x, y);
			// Load region
			m_LoadRegion(index);
		}
	}
}

Region& World::m_LoadRegion(const Vivium::Vector2<int>& index)
{
	// Try find region in region map
	auto it = regions.find(index);

	// If region at that index exists
	if (it != regions.end()) {
		// We don't have to do anything its already loaded
		return regions.at(index);
	}

	// Since that region doesn't exist already, try load from serialised file
	std::string region_name = m_ToRegionName(index);
	// Checks if region is serialised
	if (std::filesystem::exists(m_ToRegionName(index))) {
		// File exists lets deserialise it
		m_DeserialiseRegion(region_name, index);
		// Exit
		return regions.at(index);
	}

	// Region isn't already loaded and hasn't been serialised, so we have to generate it
	m_GenerateRegion(index); return regions.at(index);
}

World::~World()
{
	m_SaveWorld();
}

void World::m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index)
{
	m_Serialiser->BeginRead((PATH + m_world_name + "/" + filename).c_str());

	// Construct empty region object
	Region region;

	// Deserialise tiles
	// DeserialiseArray<Tile>(m_serialiser, region.tiles);

	m_Serialiser->EndRead();

	// Add region to region map
	regions[index] = region;
}

void World::m_LoadWorld(const std::string& fullpath)
{
	m_Serialiser->BeginRead((fullpath + GENERAL_FILE + FILE_EXTENSION).c_str());

	// Get version number
	Vivium::VersionNumber serialised_version;
	// Deserialise<VersionNumber>(m_serialiser, &serialised_version);
	// If version number is different
	if (serialised_version != m_version)
	{
		// We can't interpret this data so log fatal error
		LogFatal("World version is {}, but we're on version {}", serialised_version, m_version);
	}

	// Get seed
	// Deserialise<unsigned int>(m_serialiser, &m_seed);

	// Get player position
	// Vector2<int> player_pos = Deserialise<Vector2<int>>(m_serialiser);

	// End read
	m_Serialiser->EndRead();

	// Load region around player pos
	// m_LoadRegions(player_pos, 1);

	// Construct noise generator
	m_noise_terrain = Vivium::Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
	m_noise_trees = Vivium::Noise::White(m_seed, m_amplitude, 1);
}

void World::m_SaveWorld()
{
	for (auto& [index, region] : regions) {
		m_SerialiseRegion(index);
	}
}

void World::m_SerialiseRegion(const Vivium::Vector2<int>& index)
{
	Region& region = regions[index];
	std::string region_path = PATH + m_world_name + "/" + m_ToRegionName(index);

	//m_Serialiser->BeginWrite(region_path.c_str());

	// Serialise list of tiles
	// Serialise<Tile>(m_serialiser, region.tiles, Region::SIZE);

	//m_Serialiser->EndWrite();
}

void World::m_GenerateRegion(const Vivium::Vector2<int>& index)
{
	Region region;

	for (int y = 0; y < Region::LENGTH; y++) {
		for (int x = 0; x < Region::LENGTH; x++) {
			float noise_value = m_noise_terrain.Get(x, y); // Returns noise value from 0 - 1

			Tile tile;

			Tile::ID tile_id = Tile::ID::GRASS;

			// TODO

			if (noise_value < 0.6) { tile_id = Tile::ID::GROUND; }
			if (noise_value < 0.5) { tile_id = Tile::ID::SAND; }
			if (noise_value < 0.2) { tile_id = Tile::ID::WATER; }

			tile.ids.push_back(tile_id);

			float tree_noise = m_noise_trees.Get(x + y * Region::LENGTH);

			if (tree_noise > 0.8 && noise_value > 0.6) tile.ids.push_back(Tile::ID::TREE);

			region.Index(x, y) = tile;
		}
	}

	regions[index] = region;
}

void World::m_GenWorld(const std::string& fullpath)
{
	// TODO
	m_noise_terrain = Vivium::Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
	m_noise_trees = Vivium::Noise::White(m_seed, 1.0f, 1);
	
	m_LoadRegions(Vivium::Vector2<int>(0, 0), 3);
}

void World::Render(const Vivium::Vector2<int>& pos)
{
	const unsigned int MAX_IDS_PER_TILE = 4;

	std::vector<float> coords;
	std::vector<uint16_t> indexCoords;

	Vivium::Vector2<int> frame = { 8, 6 };

	uint16_t count = 0;

	coords.reserve(16 * frame.x * frame.y * MAX_IDS_PER_TILE);
	indexCoords.reserve(6 * frame.x * frame.y * MAX_IDS_PER_TILE);

	for (int y = pos.y - frame.y; y <= pos.y + frame.y; y++) {
		for (int x = pos.x - frame.x; x <= pos.x + frame.x; x++) {
			// Calculate region index
			Vivium::Vector2<int> region_index = m_GetRegionIndex(x, y);
			// Calculate relative coords
			int rx = x - (region_index.x * Region::LENGTH);
			int ry = y - (region_index.y * Region::LENGTH);

			// Get region
			Region& region = m_LoadRegion(region_index); // NOTE: m_LoadRegion not really needed, since regions should always already be loaded

			// Get tile from region
			Tile& tile = region.Index(rx, ry);

			// Calculate draw coords
			float dx = x * scale;
			float dy = y * scale;

			float halfscale = scale / 2.0f;

			// Iterate over each tile id
			for (const Tile::ID& id : tile.ids) {
				// Get index in atlas
				Vivium::Vector2<int> atlas_index = m_tilemap[id];

				// Inverse width and height of atlas
				float inv_width = 1.0f / m_tile_atlas->width;
				float inv_height = 1.0f / m_tile_atlas->height;

				// Calculate faces
				float left = atlas_index.x * inv_width * m_tile_atlas_size.x;
				float right = (atlas_index.x + 1) * inv_width * m_tile_atlas_size.x;
				float bottom = atlas_index.y * inv_height * m_tile_atlas_size.y;
				float top = (atlas_index.y + 1) * inv_height * m_tile_atlas_size.y;

				std::array<float, 16> this_coords =
				{
					dx - halfscale, dy - halfscale, left, bottom,
					dx + halfscale, dy - halfscale, right, bottom,
					dx + halfscale, dy + halfscale, right, top,
					dx - halfscale, dy + halfscale, left, top
				};

				int stride = count * 4;

				std::array<uint16_t, 6> this_indices =
				{
					0 + stride,
					1 + stride,
					2 + stride,
					2 + stride,
					3 + stride,
					0 + stride,
				};

				
				indexCoords.insert(indexCoords.end(), this_indices.begin(), this_indices.end());
				coords.insert(coords.end(), this_coords.begin(), this_coords.end());

				count++;
			}
		}
	}

	Vivium::BufferLayout layout = {
		{"position", Vivium::GLSLDataType::VEC2},
		{"texCoords", Vivium::GLSLDataType::VEC2}
	};

	Vivium::VertexBuffer vb(coords, layout);
	Vivium::IndexBuffer ib(indexCoords);

	Vivium::Renderer::Submit(&vb, &ib, texture_shader, m_tile_atlas);
}

World::RenderedTile::RenderedTile()
	: quad(nullptr), tile(Tile::ID::VOID)
{}

World::RenderedTile::RenderedTile(const std::shared_ptr<Vivium::Quad>& quad, const Tile& tile)
	: quad(quad), tile(tile)
{}
