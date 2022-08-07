#pragma once

#include "Region.h"

class World {
private:
	// Converts a coordinate in the world to the index of the chunk that coordinate is in
	static Vivium::Vector2<int> m_GetRegionIndex(const Vivium::Vector2<int>& pos);
	static Vivium::Vector2<int> m_GetRegionIndex(int x, int y);

	// Some constants for noise
	static constexpr float m_Amplitude = 1.0f;
	static constexpr int m_Wavelength = 4;
	static constexpr int m_Octaves = 1;

	static Vivium::VersionNumber m_VersionNumber;

	typedef std::unordered_map<Tile::ID, Vivium::Vector2<int>> TileMap_t;
	static TileMap_t m_Tilemap; // Maps a Tile::ID to its index into the atlas
	static Vivium::Texture* m_TileAtlas; // Atlas of all Tile textures
	static Vivium::Vector2<int> m_TileAtlasSize; // Size of each sprite in tile atlas
	static Vivium::Vector2<int> m_AtlasDimRelative; // Relative dimensions of tile atlas
	
	static Vivium::TextureAtlas* m_TextureAtlas;
	static std::vector<std::array<float, 4>> m_TextureCoords;

	Vivium::Noise::Interpolated m_NoiseTerrain;
	Vivium::Noise::White m_NoiseTrees;
	Vivium::Serialiser* m_Serialiser;
	unsigned int m_Seed;

	std::string m_WorldName;

	static std::string PATH;
	static std::string FILE_EXTENSION;
	static std::string GENERAL_FILE;

	std::string m_ToRegionName(const Vivium::Vector2<int>& index) const;

	void m_SerialiseRegion(const Vivium::Vector2<int>& index);

	void m_PrecalcTextureCoords();

	void m_SaveWorld();
	void m_LoadWorld(const std::string& fullpath);
	void m_GenWorld(const std::string& fullpath);

	// TODO: m_LoadRegions just destroys the entire game for some reason
	void m_LoadRegions(const Vivium::Vector2<int>& center, int radius);
	Region& m_LoadRegion(const Vivium::Vector2<int>& index);

	void m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index);
	void m_GenerateRegion(const Vivium::Vector2<int>& index);

public:
	static constexpr float scale = 64.0;

	typedef std::unordered_map<Vivium::Vector2<int>, Region> RegionMap_t;

	static Vivium::Shader* texture_shader;

	RegionMap_t regions; // Maps a region index to a Region object

	void Render(const Vivium::Vector2<int>& pos);

	// Converts from screen coordinate to world position
	// TODO: functions assumes no scale/rotation, use camera
	Vivium::Vector2<float> GetWorldPos(const Vivium::Vector2<float>& pos) const;
	Tile GetTile(const Vivium::Vector2<int>& tile);
	
	// Loads from existing world
	World(const std::string& world_name);
	// Generates new world
	World(const uint32_t& seed, const std::string& world_name);
	~World();
};