#include "World.h"

// TODO: path is complicated

std::string World::PATH = "../Resources/saves/";
std::string World::FILE_EXTENSION = ".data";
std::string World::GENERAL_FILE = "general";
Vivium::VersionNumber World::m_VersionNumber = Vivium::VersionNumber(0, 0, 1);
Vivium::Shader* World::texture_shader = nullptr;
Vivium::TextureAtlas* World::m_TextureAtlas = nullptr;
std::vector<std::array<float, 4>> World::m_TextureCoords = {};

void World::m_PrecalcTextureCoords()
{
	m_TextureCoords.resize((int)Tile::ID::MAX);

	for (int i = 0; i < (int)Tile::ID::MAX; i++) {
		// TODO: maybe implement GetCoordsArray?
		// TODO: maybe GetCoords should just return an array? makes more sense with new API
		const std::vector<float>& vector_coords = m_TextureAtlas->GetCoords(i);

		// left bottom right top
		m_TextureCoords[i] = { vector_coords[0], vector_coords[1], vector_coords[2], vector_coords[5] };
	}
}

Vivium::Vector2<float> World::GetWorldPos(const Vivium::Vector2<float>& pos) const
{
	// TODO
	return { 0, 0 };
}

Tile World::GetTile(const Vivium::Vector2<int>& pos)
{
	// Find region tile is in
	Vivium::Vector2<int> region_pos = m_GetRegionIndex(pos);
	Region& region = m_LoadRegion(region_pos);
	// Get coordinate of tile within region
	Vivium::Vector2<int> relative = pos - (region_pos * Region::LENGTH);
	// Index tile and return
	return region.Index(relative);
}

World::World(const std::string& world_name)
	: m_WorldName(world_name)
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
	: m_WorldName(world_name), m_Seed(seed)
{
	std::string fullpath = PATH + world_name + "/";

	// Create folder for our world
	std::filesystem::create_directory(fullpath);

	m_TextureAtlas = new Vivium::TextureAtlas("tile_atlas.png", { 32, 32 }); // TODO: hardcoded sprite size

	m_PrecalcTextureCoords();

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

std::string World::m_ToRegionName(const Vivium::Vector2<int>& index) const
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

	delete m_TextureAtlas;
}

void World::m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index)
{
	m_Serialiser->BeginRead((PATH + m_WorldName + "/" + filename).c_str());

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
	if (serialised_version != m_VersionNumber)
	{
		// We can't interpret this data so log fatal error
		LogFatal("World version is {}, but we're on version {}", serialised_version, m_VersionNumber);
	}

	// Get seed
	// Deserialise<unsigned int>(m_serialiser, &m_Seed);

	// Get player position
	// Vector2<int> player_pos = Deserialise<Vector2<int>>(m_serialiser);

	// End read
	m_Serialiser->EndRead();

	// Load region around player pos
	// m_LoadRegions(player_pos, 1);

	// Construct noise generator
	m_NoiseTerrain = Vivium::Noise::Interpolated(m_Seed, m_Amplitude, m_Wavelength);
	m_NoiseTrees = Vivium::Noise::White(m_Seed, m_Amplitude, 1);
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
	std::string region_path = PATH + m_WorldName + "/" + m_ToRegionName(index);

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
			float noise_value = m_NoiseTerrain.Get(x, y); // Returns noise value from 0 - 1

			Tile tile;

			Tile::ID tile_id = Tile::ID::GRASS;

			// TODO

			if (noise_value < 0.6) { tile_id = Tile::ID::GROUND; }
			if (noise_value < 0.5) { tile_id = Tile::ID::SAND; }
			if (noise_value < 0.2) { tile_id = Tile::ID::WATER; }

			tile.base = tile_id;

			float tree_noise = m_NoiseTrees.Get(x + y * Region::LENGTH);

			if (tree_noise > 0.8 && noise_value > 0.6) tile.top = Tile::ID::TREE;

			region.Index(x, y) = tile;
		}
	}

	regions[index] = region;
}

void World::m_GenWorld(const std::string& fullpath)
{
	// TODO
	m_NoiseTerrain = Vivium::Noise::Interpolated(m_Seed, m_Amplitude, m_Wavelength);
	m_NoiseTrees = Vivium::Noise::White(m_Seed, 1.0f, 1);

	m_LoadRegions(Vivium::Vector2<int>(0, 0), 3);
}

void World::Render(const Vivium::Vector2<int>& pos)
{
	std::vector<float> coords;
	std::vector<unsigned short> indexCoords;

	Vivium::Vector2<int> frame = Vivium::Application::GetScreenDim() / (scale * 2.0f) + Vivium::Vector2<int>(1, 1);

	unsigned short count = 0;
	unsigned int max_count = (frame.x * 2 + 1) * (frame.y * 2 + 1) * 3;

	// TODO: use VertexBuffer::StartMap and EndMap

	coords.reserve(16 * max_count);
	indexCoords.reserve(6 * max_count);

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
			float dx = x * World::scale;
			float dy = y * World::scale;

			float halfscale = World::scale * 0.5f;

			// Iterate over each tile id
			for (const Tile::ID& id : { tile.base, tile.mid, tile.top }) {
				if (id == Tile::ID::VOID) { continue; }

				std::array<float, 4>& faces = m_TextureCoords[(int)id];

				std::array<float, 16> this_coords =
				{
					dx - halfscale, dy - halfscale, faces[0], faces[1], // bottom left
					dx + halfscale, dy - halfscale, faces[2], faces[1], // bottom right
					dx + halfscale, dy + halfscale, faces[2], faces[3], // top right
					dx - halfscale, dy + halfscale, faces[0], faces[3]  // top left
				};

				int stride = count * 4;

				std::array<unsigned short, 6> this_indices =
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

	Vivium::Renderer::Submit(&vb, &ib, texture_shader, m_TextureAtlas->GetAtlas().get());
}