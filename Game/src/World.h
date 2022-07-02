#pragma once

#include <Engine.h>

#include "Region.h"

class World {
	// Some constants for noise
	static constexpr float m_amplitude = 1.0f;
	static constexpr int m_wavelength = 64;
	static constexpr int m_octaves = 8;

	static std::string PATH;
	static std::string FILE_EXTENSION;
	static std::string GENERAL_FILE;

	static VersionNumber m_version;

	Noise::Interpolated* m_noise_gen;
	Serialiser m_serialiser;
	unsigned int m_seed;

	std::string m_ToRegionName(const Vector2<int>& index);

	void m_LoadRegions(const Vector2<float>& center, int radius);
	void m_LoadRegion(const Vector2<int>& index);

	void m_DeserialiseRegion(const std::string& filename);
	void m_GenerateRegion(const Vector2<int>& index);

public:
	std::string world_name;

	std::unordered_map<Vector2<int>, Region> regions;

	World(const std::string& savefile);
	~World();
};