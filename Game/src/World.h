#pragma once

#include "Region.h"
#include "Structures.h"
#include "WorldMap.h"

// TODO: world should serialise/deserialise around player

namespace Game {
	class Player;
	class Inventory;

	class World {
	private:
		// Some constants for noise
		static constexpr float m_Amplitude = 1.0f;
		static constexpr int m_Wavelength = 4;
		static constexpr int m_Octaves = 1;

		static Vivium::TextureAtlas* m_TextureAtlas; // TODO unique ptr
		static std::vector<std::array<float, 4>> m_TextureCoords;
		
		static Vivium::TextureAtlas* m_ItemsAtlas; // TODO unique ptr
		static std::vector<std::array<float, 4>> m_ItemsTextureCoords;

		Vivium::Noise::Interpolated m_NoiseTerrain;
		Vivium::Noise::White m_NoiseTrees;
		Vivium::Serialiser* m_Serialiser; // TODO unique ptr
		unsigned int m_Seed;

		Vivium::Shader* m_DaylightShader;
		Vivium::Framebuffer* m_DaylightFramebuffer;

		WorldMap* m_WorldMap;

		std::string m_WorldName;

		Vivium::Timer m_UpdateTimer;

		Vivium::Vector2<int> mined_tile_pos = Vivium::Vector2<int>(INT_MAX, INT_MAX);
		Tile::ID mined_tile_id;
		float mined_tile_time; // Time we've been mining tile for

		Player* m_Player;

		std::string m_ToRegionName(const Vivium::Vector2<int>& index) const;

		void m_PrecalcTextureCoords();

		void m_SaveWorld();
		void m_LoadWorld(const std::string& fullpath);
		void m_GenWorld(const std::string& fullpath);

		void m_LoadRegions(Player* player);
		Region& m_LoadRegion(const Vivium::Vector2<int>& index);

		void m_SerialiseRegion(const Vivium::Vector2<int>& index, const Region& region);
		void m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index);
		void m_GenerateRegion(const Vivium::Vector2<int>& index);

		void m_UpdateMining(Player* player, float elapsed);

		void m_AddFloorItem(const Vivium::Vector2<int>& region_pos, const FloorItem& item);

		void m_RenderTiles(const Vivium::Vector2<int>& pos);
		void m_RenderFloorItems(const Vivium::Vector2<int>& pos);

		float m_GetMiningTileScale(float tile_scale, const Tile::ID& id);

	public:
		// TODO: weird statics
		static std::string PATH;
		static std::string FILE_EXTENSION;
		static std::string GENERAL_FILE;

		static constexpr float PIXEL_SCALE = 64.0;
		static constexpr int PHYSICS_TILE_LAYER = 0xff;

		typedef std::unordered_map<Vivium::Vector2<int>, Region> RegionMap_t;
		typedef std::unordered_map<Vivium::Vector2<int>, std::vector<FloorItem>> FloorItemMap_t;

		static Vivium::Shader* texture_shader;

		RegionMap_t regions; // Maps a region index to a Region object
		FloorItemMap_t floor_items; // Maps a region index to a list of floor items, so only items within adjacent regions are drawn

		static void Init();
		static void Terminate();

		// Converts a coordinate in the world to the index of the chunk that coordinate is in
		static Vivium::Vector2<int> GetRegionIndex(const Vivium::Vector2<int>& pos);
		static Vivium::Vector2<int> GetRegionIndex(int x, int y);

		std::vector<FloorItem>* GetFloorItems(const Vivium::Vector2<int>& pos);
		
		void Render(const Vivium::Vector2<int>& pos);
		void Update();

		std::string GetName() const;

		// Converts from screen coordinate to world position
		// TODO: functions assumes no scale/rotation, use camera
		Vivium::Vector2<int> GetWorldPos(const Vivium::Vector2<float>& pos) const;
		Tile& GetTile(const Vivium::Vector2<int>& tile);
		Tile& GetTile(int x, int y);

		Tile* GetLoadedTile(const Vivium::Vector2<int>& pos);
		Tile* GetLoadedTile(int x, int y);

		World(const uint32_t& seed, const std::string& world_name, Player* player);
		~World();

		friend Inventory;
		friend WorldMap;
	};
}
