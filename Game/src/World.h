#pragma once

#include "Region.h"
#include "Structures.h"
#include "WorldMap.h"
#include "Biome.h"
#include "TerrainGenerator.h"
#include "LeafParticles.h"
#include "NPC.h"

// TODO: m_Player is property now, don't pass to functions

namespace Game {
	class Player;
	class Inventory;

	class World {
	private:
		Vivium::Noise::Interpolated m_NoiseTerrain;
		Vivium::Noise::White m_NoiseTrees;
		Vivium::Serialiser* m_Serialiser; // TODO unique ptr
		unsigned int m_Seed;

		Vivium::Shader* m_DaylightShader;
		Vivium::Framebuffer* m_DaylightFramebuffer;

		static constexpr int MAP_SIZE = 48;
		WorldMap* m_WorldMap;

		std::string m_WorldName;

		LeavesParticleSystem m_LeafBlockBreaking;
		LeavesParticleSystem m_LeafWind; // TODO: should be screenspace not worldspace

		Vivium::Timer m_UpdateTimer;
		float m_TimeAlive = 0.0f;

		Vivium::Vector2<int> mined_tile_pos = Vivium::Vector2<int>(INT_MAX, INT_MAX);
		Tile::ID mined_tile_id;
		float mined_tile_time; // Time we've been mining tile for

		Player* m_Player;
		Biome::ID m_CurrentBiome; // Biome we're in
		float m_LastParticleEmitTime = 0.0f;

		Vivium::Physics::Layer* m_PlayerLayer = nullptr;
		static constexpr uint32_t PLAYER_PHYSICS_LAYER = 0;
		Vivium::Physics::Layer* m_TileLayer = nullptr;
		static constexpr uint32_t TILE_PHYSICS_LAYER = 1;

		std::vector<Ref(Vivium::Body)> m_TileBodies;

		static constexpr int OBSTACLE_MAP_REGION_PADDING = 1;
		Vivium::Vector2<int> m_WorldToObstacleMapTransform;
		Vivium::Pathfinding::Map m_ObstacleMap;

		irrklang::ISoundSource* m_BlockBreakingSound;

		std::string m_ToRegionName(const Vivium::Vector2<int>& index) const;

		void m_SaveWorld();

		void m_LoadRegions(Player* player);
		Region& m_LoadRegion(const Vivium::Vector2<int>& index);

		void m_SerialiseRegion(const Vivium::Vector2<int>& index, const Region* region);
		void m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index);
		void m_GenerateRegion(const Vivium::Vector2<int>& index);

		void m_UpdateMining(Player* player, float elapsed);

		void m_UpdateCurrentBiome();

		void m_AddFloorItem(const Vivium::Vector2<int>& region_pos, const FloorItem& item);

		void m_UpdateMineable(const Tile::ID& id, float& tile_scale, int world_x, int world_y);

		void m_RenderTiles(const Vivium::Vector2<int>& pos);
		void m_RenderFloorItems(const Vivium::Vector2<int>& pos);

		float m_GetMiningTileScale(float tile_scale, const Tile::ID& id);

		void m_UpdateObstacleMap();

	public:
		// TODO: weird statics
		static std::string PATH;
		static std::string FILE_EXTENSION;
		static std::string GENERAL_FILE;

		static constexpr float PIXEL_SCALE = 64.0f;

		typedef std::unordered_map<Vivium::Vector2<int>, Region*> RegionMap_t;
		typedef std::unordered_map<Vivium::Vector2<int>, std::vector<FloorItem>> FloorItemMap_t;

		static Vivium::Shader* texture_shader;

		RegionMap_t regions; // Maps a region index to a Region object
		std::mutex regions_mutex;

		FloorItemMap_t floor_items; // Maps a region index to a list of floor items, so only items within adjacent regions are drawn

		static void Init();
		static void Terminate();

		// Converts a coordinate in the world to the index of the chunk that coordinate is in
		static Vivium::Vector2<int> GetRegionIndex(const Vivium::Vector2<int>& pos);
		static Vivium::Vector2<int> GetRegionIndex(int x, int y);

		const Vivium::Pathfinding::Map* GetObstacleMap() const;
		Vivium::Vector2<int> GetObstacleMapIndex(const Vivium::Vector2<int>& pos) const;
		bool GetIsObstacle(const Vivium::Vector2<int>& pos) const;

		std::vector<FloorItem>* GetFloorItems(const Vivium::Vector2<int>& pos);
		
		void Render(const Vivium::Vector2<int>& pos);
		void Update();

		std::string GetName() const;

		// Converts from screen coordinate to world position
		Vivium::Vector2<int> GetWorldPos(const Vivium::Vector2<float>& pos) const;
		Tile& GetTile(const Vivium::Vector2<int>& tile);
		Tile& GetTile(int x, int y);

		Tile* GetLoadedTile(const Vivium::Vector2<int>& pos);
		Tile* GetLoadedTile(int x, int y);

		World(const uint32_t& seed, const std::string& world_name, Player* player);
		~World();

		friend Inventory;
		friend WorldMap;
		friend Biome;
	};
}
