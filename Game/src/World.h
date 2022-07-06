#pragma once

#include <Engine.h>

#include "Region.h"

class World {
public:
	// Some constants for noise
	static constexpr float m_amplitude = 1.0f;
	static constexpr int m_wavelength = 1;
	static constexpr int m_octaves = 1;

	static std::string PATH;
	static std::string FILE_EXTENSION;
	static std::string GENERAL_FILE;

	static VersionNumber m_version;

	typedef std::unordered_map<Tile::ID, Vector2<int>> TileMap_t;
	static TileMap_t m_tilemap; // Maps a Tile::ID to its index into the atlas
	static Texture* m_tile_atlas; // Atlas of all Tile textures
	static Vector2<int> m_tile_atlas_size; // Size of each sprite in tile atlas
	static Vector2<int> m_atlas_dim_relative; // Relative dimensions of tile atlas

	Noise::Interpolated* m_noise_gen;
	Serialiser m_serialiser;
	unsigned int m_seed;

	std::string m_world_name;

	std::string m_ToRegionName(const Vector2<int>& index);

	void m_SerialiseRegion(const Vector2<int>& index);

	void m_SaveWorld();
	void m_LoadWorld(const std::string& fullpath);
	void m_GenWorld(const std::string& fullpath);

	void m_LoadRegions(const Vector2<int>& center, int radius);
	void m_LoadRegion(const Vector2<int>& index);

	void m_DeserialiseRegion(const std::string& filename, const Vector2<int>& index);
	void m_GenerateRegion(const Vector2<int>& index);

	void m_RenderAround(const Vector2<int>& center, int radius);

public:
	typedef std::unordered_map<Vector2<int>, Region> RegionMap_t;

	static Shader* texture_shader;

	RegionMap_t regions;			// Maps a region index to a Region object

	std::vector<Renderable> renderables;

	// Loads all tile::ids into textures
	static void LoadTextures(const std::string& atlas_file);

	void Update(const Vector2<int>& pos);
	
	// Loads from existing world
	World(const std::string& world_name);
	// Generates new world
	World(const uint32_t& seed, const std::string& world_name);
	~World();
};