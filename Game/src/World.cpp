#include "World.h"

// TODO: path is complicated

std::string World::PATH = "../Resources/saves/";
std::string World::FILE_EXTENSION = ".data";
std::string World::GENERAL_FILE = "general";
VersionNumber World::m_version = VersionNumber(0, 0, 1);
World::TileMap_t World::m_tilemap = {};
Texture* World::m_tile_atlas = nullptr;
Vector2<int> World::m_tile_atlas_size = Vector2<int>(0, 0);
Vector2<int> World::m_atlas_dim_relative = Vector2<int>(0, 0);
Shader* World::texture_shader = nullptr;

void World::LoadTextures(const std::string& atlas_file)
{
	constexpr int sprite_size = 64; // TODO

	// Load atlas and various information about atlas
	m_tile_atlas = new Texture(atlas_file);
	m_tile_atlas_size = Vector2<int>(sprite_size, sprite_size); // TODO
	m_atlas_dim_relative = Vector2<int>(m_tile_atlas->width / sprite_size, m_tile_atlas->height / sprite_size);

	// Iterate over value of each tile id
	for (int i = 0; i < (int)Tile::ID::MAX; i++) {
		// Convert it to an index into the atlas
		int y = i / m_atlas_dim_relative.x;
		int x = i - (y * m_atlas_dim_relative.x);
		// Invert y
		y = (m_atlas_dim_relative.y - 1) - y;

		// Add to tilemap
		m_tilemap[(Tile::ID)i] = Vector2<int>(x, y);
	}
}

World::World(const std::string& world_name)
	: m_world_name(world_name)
{
	std::string fullpath = PATH + world_name + "/";

	if (Utils::CheckDirectoryExists(fullpath)) {
		// Load world
		m_LoadWorld(fullpath);
	}
	else {
		Log("Directory didn't exist, world hasn't been generated yet?", LOG::FATAL);
	}
}

World::World(const uint32_t& seed, const std::string& world_name)
	: m_world_name(world_name), m_seed(seed)
{
	std::string fullpath = PATH + world_name + "/";

	// Create folder for our world
	Utils::CreateDirectory(fullpath);
	// Generate world
	m_GenWorld(fullpath);

}

std::string World::m_ToRegionName(const Vector2<int>& index)
{
	return std::format("region{}_{}{}", to_string(index.x), to_string(index.y), FILE_EXTENSION);
}

void World::m_LoadRegions(const Vector2<int>& center, int radius)
{
	for (int y = center.y - radius; y < center.y + radius + 1; y++) {
		for (int x = center.x - radius; x < center.x + radius + 1; x++) {
			// Get index
			Vector2<int> index = Vector2<int>(x, y);
			// Load region
			m_LoadRegion(index);
		}
	}
}

void World::m_LoadRegion(const Vector2<int>& index)
{
	// Try find region in region map
	auto it = regions.find(index);

	// If region at that index exists
	if (it != regions.end()) {
		// We don't have to do anything its already loaded
		return;
	}

	// Since that region doesn't exist already

	// Get name to file region data might be stored in
	std::string region_name = m_ToRegionName(index);
	if (Utils::CheckFileExists(m_ToRegionName(index))) {
		// File exists lets deserialise it
		m_DeserialiseRegion(region_name, index);
		// Exit
		return;
	}

	// Region isn't already loaded and hasn't been serialised, so we have to generate it ourselves
	m_GenerateRegion(index);
}

World::~World()
{
	m_SaveWorld();
	delete m_noise_gen;
}

void World::m_DeserialiseRegion(const std::string& filename, const Vector2<int>& index)
{
	m_serialiser.BeginRead((PATH + m_world_name + "/" + filename).c_str());

	// Construct empty region object
	Region region;

	// Deserialise tiles
	DeserialiseArray<Tile::ID>(m_serialiser, region.tiles);

	m_serialiser.EndRead();

	// Add region to region map
	regions[index] = region;
}

void World::m_LoadWorld(const std::string& fullpath)
{
	m_serialiser.BeginRead((fullpath + GENERAL_FILE + FILE_EXTENSION).c_str());

	// Get version number
	VersionNumber serialised_version;
	Deserialise<VersionNumber>(m_serialiser, &serialised_version);
	// If version number is different
	if (serialised_version != m_version)
	{
		// We can't interpret this data so log fatal error
		Log(std::format("World version is {}, but we're on version {}", to_string(serialised_version), to_string(m_version)), LOG::FATAL);
	}

	// Get seed
	Deserialise<unsigned int>(m_serialiser, &m_seed);

	// Get player position
	Vector2<int> player_pos = Deserialise<Vector2<int>>(m_serialiser);

	// End read
	m_serialiser.EndRead();

	// Load region around player pos
	m_LoadRegions(player_pos, 1);

	// Construct noise generator
	m_noise_gen = new Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
}

void World::m_SaveWorld()
{
	for (auto& [index, region] : regions) {
		m_SerialiseRegion(index);
	}
}

void World::m_SerialiseRegion(const Vector2<int>& index)
{
	Region& region = regions[index];
	std::string region_path = PATH + m_world_name + "/" + m_ToRegionName(index);

	m_serialiser.BeginWrite(region_path.c_str());

	Serialise<Tile::ID>(m_serialiser, region.tiles, Region::SIZE);

	m_serialiser.EndWrite();
}

void World::m_GenerateRegion(const Vector2<int>& index)
{
	Region region;

	for (int y = 0; y < Region::LENGTH; y++) {
		for (int x = 0; x < Region::LENGTH; x++) {
			float noise_value = m_noise_gen->GetFractal(x, y, m_octaves); // Returns noise value from 0 - 1

			Tile::ID tile_id = Tile::ID::GRASS;

			// TODO

			if (noise_value > 0.5) { tile_id = Tile::ID::GROUND;}

			region.Index(x, y) = tile_id;
		}
	}

	regions[index] = region;
}

void World::m_GenWorld(const std::string& fullpath)
{
	// TODO
	m_noise_gen = new Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
	
	m_LoadRegions(Vector2<int>(0, 0), 3);
}

void World::m_RenderAround(const Vector2<int>& center, int radius)
{
	constexpr int scale = 128;

	
	for (int y = center.y - radius; y < center.y + radius; y++) {
		for (int x = center.x - radius; x < center.x + radius; x++) {
			// Calculate region index
			Vector2<int> region_index(x / Region::LENGTH, y / Region::LENGTH);
			// Calculate relative coords
			int rx = x - (region_index.x * Region::LENGTH);
			int ry = y - (region_index.y * Region::LENGTH);

			// Get region
			Region& region = regions[region_index];
			
			// Get tile from region
			Tile::ID& tile = region.Index(rx, ry);

			// Get index in atlas
			Vector2<int> atlas_index = m_tilemap[tile];

			// Calculate draw coords
			int tx = x * scale;
			int ty = y * scale;

			// Construct quad
			Quad quad(tx, ty, scale, scale, 0);
			// Set texture coords
			quad.SetTextureCoords(*m_tile_atlas, atlas_index, m_tile_atlas_size);
			// Schedule to renderer
			Renderer::Schedule(&quad, texture_shader, m_tile_atlas, 1);
		}
	}
}

void World::Update(const Vector2<int>& pos)
{
	Log("Loading regions", LOG::INFO);
	m_LoadRegions(pos, 3);
	Log("Loading renderables", LOG::INFO);
	m_RenderAround(pos, 8);
}
