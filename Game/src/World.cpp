#include "World.h"

std::string World::PATH = "../Resources/saves/";
std::string World::FILE_EXTENSION = ".data";
std::string World::GENERAL_FILE = "general";
VersionNumber World::m_version = VersionNumber(0, 0, 1);

World::World(const std::string& save_directory)
{
	m_serialiser.BeginRead((PATH + save_directory + GENERAL_FILE + FILE_EXTENSION).c_str());

	{
		// Get version number
		//VersionNumber serialised_version = Deserialise<VersionNumber>(m_serialiser);
		// If version number is different
		//if (serialised_version != m_version)
		//{
			// We can't interpret this data so log fatal error
		//	Log(std::format("World version is {}, but we're on version {}", to_string(serialised_version), to_string(m_version)), Utils::ERROR::FATAL);
		//}

		// Get world name
		//Deserialise<std::string>(m_serialiser, &world_name);

		// Get seed
		//Deserialise<unsigned int>(m_serialiser, &m_seed);

		// Get player position
		//Vector2<float> player_pos = Deserialise<Vector2<float>>(m_serialiser);

		// Load region around player pos
		//m_LoadRegions(player_pos, 1);
	}

	m_serialiser.EndRead();

	// Construct noise generator
	m_noise_gen = new Noise::Interpolated(m_seed, m_amplitude, m_wavelength);
}

std::string World::m_ToRegionName(const Vector2<int>& index)
{
	return std::format("region{}_{}{}", to_string(index.x), to_string(index.y), FILE_EXTENSION);
}

void World::m_LoadRegions(const Vector2<float>& center, int radius)
{
	for (int y = center.y - radius; y <= center.y + radius; y++) {
		for (int x = center.x - radius; x <= center.x + radius; x++) {
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
		m_DeserialiseRegion(region_name);
		// Exit
		return;
	}

	// Region isn't already loaded and hasn't been serialised, so we have to generate it ourselves
	m_GenerateRegion(index);
}

World::~World()
{
	delete m_noise_gen;
}

void World::m_DeserialiseRegion(const std::string& filename)
{
	// TODO
}

void World::m_GenerateRegion(const Vector2<int>& index)
{
	// TODO
}
