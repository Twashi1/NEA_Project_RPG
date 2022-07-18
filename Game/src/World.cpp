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
		ENG_LogFatal("Directory didn't exist, world hasn't been generated yet?");
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

Vector2<int> World::m_GetRegionIndex(const Vector2<int>& pos)
{
	return Vector2<int>(
		std::floor((double)pos.x / (double)Region::LENGTH),
		std::floor((double)pos.y / (double)Region::LENGTH)
	);
}

Vector2<int> World::m_GetRegionIndex(int x, int y)
{
	return Vector2<int>(
		std::floor((double)x / (double)Region::LENGTH),
		std::floor((double)y / (double)Region::LENGTH)
		);
}

std::string World::m_ToRegionName(const Vector2<int>& index)
{
	return std::format("region{}_{}{}", to_string(index.x), to_string(index.y), FILE_EXTENSION);
}

void World::m_LoadRegions(const Vector2<int>& center, int radius)
{
	// TODO: doesn't delete old regions
	for (int y = center.y - radius; y < center.y + radius + 1; y++) {
		for (int x = center.x - radius; x < center.x + radius + 1; x++) {
			// Get index
			Vector2<int> index = Vector2<int>(x, y);
			// Load region
			m_LoadRegion(index);
		}
	}
}

Region& World::m_LoadRegion(const Vector2<int>& index)
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
	if (Utils::CheckFileExists(m_ToRegionName(index))) {
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
	delete m_noise_terrain;
	delete m_noise_trees;
}

void World::m_DeserialiseRegion(const std::string& filename, const Vector2<int>& index)
{
	m_Stream.BeginRead((PATH + m_world_name + "/" + filename).c_str());

	// Construct empty region object
	Region region;

	// Deserialise tiles
	// DeserialiseArray<Tile>(m_serialiser, region.tiles);

	m_Stream.EndRead();

	// Add region to region map
	regions[index] = region;
}

void World::m_LoadWorld(const std::string& fullpath)
{
	m_Stream.BeginRead((fullpath + GENERAL_FILE + FILE_EXTENSION).c_str());

	// Get version number
	VersionNumber serialised_version;
	// Deserialise<VersionNumber>(m_serialiser, &serialised_version);
	// If version number is different
	if (serialised_version != m_version)
	{
		// We can't interpret this data so log fatal error
		ENG_LogFatal("World version is {}, but we're on version {}", to_string(serialised_version), to_string(m_version));
	}

	// Get seed
	// Deserialise<unsigned int>(m_serialiser, &m_seed);

	// Get player position
	// Vector2<int> player_pos = Deserialise<Vector2<int>>(m_serialiser);

	// End read
	m_Stream.EndRead();

	// Load region around player pos
	// m_LoadRegions(player_pos, 1);

	// Construct noise generator
	m_noise_terrain = new Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
	m_noise_trees = new Noise::White(m_seed, m_amplitude, 1);
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

	m_Stream.BeginWrite(region_path.c_str());

	// Serialise list of tiles
	// Serialise<Tile>(m_serialiser, region.tiles, Region::SIZE);

	m_Stream.EndWrite();
}

void World::m_GenerateRegion(const Vector2<int>& index)
{
	Region region;

	for (int y = 0; y < Region::LENGTH; y++) {
		for (int x = 0; x < Region::LENGTH; x++) {
			float noise_value = m_noise_terrain->GetFractal(x, y, m_octaves); // Returns noise value from 0 - 1

			Tile tile;

			Tile::ID tile_id = Tile::ID::GRASS;

			// TODO

			if (noise_value > 0.5) { tile_id = Tile::ID::GROUND;}

			tile.ids.push_back(tile_id);

			float tree_noise = m_noise_trees->Get(x + y * Region::LENGTH);

			if (tree_noise > 0.9) tile.ids.push_back(Tile::ID::TREE);

			region.Index(x, y) = tile;
		}
	}

	regions[index] = region;
}

void World::m_RenderTile(int x, int y)
{
	// Calculate region index
	Vector2<int> region_index = m_GetRegionIndex(x, y);
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

	// Construct quad
	std::shared_ptr<Quad> quad = std::shared_ptr<Quad>(new Quad(dx, dy, scale, scale, 0));

	// Iterate over each tile id
	for (const Tile::ID& id : tile.ids) {
		// Get index in atlas
		Vector2<int> atlas_index = m_tilemap[id];

		// Set texture coords
		quad->SetTextureCoords(*m_tile_atlas, atlas_index, m_tile_atlas_size);
		// Schedule to renderer
		Renderer::Schedule(quad.get(), texture_shader, m_tile_atlas);
	}

	// Add ourselves to rendered_tiles
	rendered_tiles[{x, y}] = RenderedTile(quad, tile);
}

void World::m_RenderTile(const Vector2<int>& pos)
{
	// Calculate region index
	Vector2<int> region_index = m_GetRegionIndex(pos);
	// Calculate relative coords
	Vector2<int> relative_coords = pos - (region_index * (int)Region::LENGTH);

	// Get region
	Region& region = m_LoadRegion(region_index); // NOTE: m_LoadRegion not really needed, since regions should always already be loaded

	// Get tile from region
	Tile& tile = region.Index(relative_coords);

	// Calculate draw coords
	Vector2<float> draw_coords = Vector2<float>(pos.x * scale, pos.y * scale);

	// Construct quad
	std::shared_ptr<Quad> quad = std::shared_ptr<Quad>(new Quad(draw_coords, Vector2<float>(scale, scale), 3.14159 * 0.5));

	// Iterate over each tile id
	for (const Tile::ID& id : tile.ids) {
		// Get index in atlas
		Vector2<int> atlas_index = m_tilemap[id];

		// Set texture coords
		quad->SetTextureCoords(*m_tile_atlas, atlas_index, m_tile_atlas_size);
		// Schedule to renderer
		Renderer::Schedule(quad.get(), texture_shader, m_tile_atlas);
	}

	// Add ourselves to rendered_tiles
	rendered_tiles[pos] = RenderedTile(quad, tile);
}

void World::m_RenderTile(const RenderedTile& rendered_tile)
{
	// Iterate over each tile id
	for (const Tile::ID& id : rendered_tile.tile.ids) {
		// Get index in atlas
		Vector2<int> atlas_index = m_tilemap[id];

		// Set texture coords
		rendered_tile.quad->SetTextureCoords(*m_tile_atlas, atlas_index, m_tile_atlas_size);
		// Schedule to renderer
		Renderer::Schedule(rendered_tile.quad.get(), texture_shader, m_tile_atlas);
	}
}

void World::m_GenWorld(const std::string& fullpath)
{
	// TODO
	m_noise_terrain = new Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
	m_noise_trees = new Noise::White(m_seed, 1.0f, 1);
	
	m_LoadRegions(Vector2<int>(0, 0), 3);
}

void World::m_RenderAround(const Vector2<int>& center, const Vector2<int>& frame)
{
	/*
	* Short explanation of logic behind this function:
	* Calling m_RenderAround renders a square area around a central position, who's side lengths are 2*radius + 1
	* We store the last call of m_RenderAround's arguments
	* We store all the rendered tiles from the last call in rendered_tiles
	* Upon calling m_RenderAround, we should discard all already rendered tiles from rendered_tiles IF they are not within the bounds of the new square area
			we're rendering
	* To do this we find the bounds of the old "region" and the new "region", tiles in the old region that do not overlap with the new region
			should not be rendered, thus are deleted from rendered_tiles, however tiles that do overlap are rendered
	* However ALL tiles in the new region should be rendered, not just tiles that overlap with both regions, thus we find all the tiles within the new region
			that do not overlap with the old region (since those tiles are already drawn), and we create quads for them, add them to rendered_tiles, and draw
			them
	* This should have a time complexity of O(n) in an average case (unordered_map::erase, at, and insert are constant on average, but can be n in worst case,
			thus worst case is O(n^2)
	*/

	// TODO: fix
	
	// If last pos has been set
	if (last_render_frame.x != 0) {
		// Calculate bounds of last rendering region and new rendering region
		// Bounds of old pos
		int left0 = last_render_pos.x - last_render_frame.x;
		int right0 = last_render_pos.x + last_render_frame.x;
		int bottom0 = last_render_pos.y - last_render_frame.y;
		int top0 = last_render_pos.y + last_render_frame.y;
		// Bounds of new pos
		int left1 = center.x - frame.x;
		int right1 = center.x + frame.x;
		int bottom1 = center.y - frame.y;
		int top1 = center.y + frame.y;

		// Iterate every tile in old region
		for (int y = bottom0; y <= top0; y++) {
			for (int x = left0; x <= right0; x++) {
				// Check if its within bounds of the new region
				// Not within bounds if true
				if (x < left1 || x > right1 || y < bottom1 || y > top1) {
					rendered_tiles.erase({ x, y });
				}
				// Coordinate is within bounds
				else {
					// Get tile and render it
					m_RenderTile(rendered_tiles.at({x, y}));
				}
			}
		}

		// Iterate every tile in new region
		for (int y = bottom1; y <= top1; y++) {
			for (int x = left1; x <= right1; x++) {
				// Check tile is within bounds of old region
				// Not within bounds if ture
				if (x < left0 || x > right0 || y < bottom0 || y > top0) {
					// Render new tile and add to rendered tiles
					m_RenderTile(x, y);
				}
			}
		}
	}
	// Render everything new
	else {
		for (int y = center.y - frame.y; y <= center.y + frame.y; y++) {
			for (int x = center.x - frame.x; x <= center.x + frame.x; x++) {
				// Render new tile and add to rendered tiles
				m_RenderTile(x, y);
			}
		}
	}

	// Update last render pos and last render radius
	last_render_pos = center;
	last_render_frame = frame;
}

void World::Update(const Vector2<int>& pos)
{
	// TODO: non-fixed render region
	m_RenderAround(pos, {8, 6});
}

World::RenderedTile::RenderedTile()
	: quad(nullptr), tile(Tile::ID::VOID)
{}

World::RenderedTile::RenderedTile(const std::shared_ptr<Quad>& quad, const Tile& tile)
	: quad(quad), tile(tile)
{}
