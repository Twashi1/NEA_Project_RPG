#pragma once

#include "Region.h"

class World {
private:
	// Converts a coordinate in the world to the index of the chunk that coordinate is in
	static Vivium::Vector2<int> m_GetRegionIndex(const Vivium::Vector2<int>& pos);
	static Vivium::Vector2<int> m_GetRegionIndex(int x, int y);

	// Some constants for noise
	static constexpr float m_amplitude = 1.0f;
	static constexpr int m_wavelength = 4;
	static constexpr int m_octaves = 1;

	static Vivium::VersionNumber m_version;

	typedef std::unordered_map<Tile::ID, Vivium::Vector2<int>> TileMap_t;
	static TileMap_t m_tilemap; // Maps a Tile::ID to its index into the atlas
	static Vivium::Texture* m_tile_atlas; // Atlas of all Tile textures
	static Vivium::Vector2<int> m_tile_atlas_size; // Size of each sprite in tile atlas
	static Vivium::Vector2<int> m_atlas_dim_relative; // Relative dimensions of tile atlas

	Vivium::Noise::Interpolated m_noise_terrain;
	Vivium::Noise::White m_noise_trees;
	Vivium::Serialiser* m_Serialiser;
	unsigned int m_seed;

	std::string m_world_name;

	static std::string PATH;
	static std::string FILE_EXTENSION;
	static std::string GENERAL_FILE;

	struct RenderedTile {
		std::shared_ptr<Vivium::Quad> quad;
		Tile tile;

		RenderedTile();
		RenderedTile(const std::shared_ptr<Vivium::Quad>& quad, const Tile& tile);

		bool operator==(const RenderedTile& other) { return tile.ids == other.tile.ids && quad == other.quad; }
	};

	std::string m_ToRegionName(const Vivium::Vector2<int>& index);

	void m_SerialiseRegion(const Vivium::Vector2<int>& index);

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

	// Loads all tile::ids into textures
	static void LoadTextures(const std::string& atlas_file);

	void Render(const Vivium::Vector2<int>& pos);
	
	// Loads from existing world
	World(const std::string& world_name);
	// Generates new world
	World(const uint32_t& seed, const std::string& world_name);
	~World();
};