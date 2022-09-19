#include "World.h"
#include "Player.h"
#include "Inventory.h"

namespace Game {
	// TODO: path is complicated

	std::string World::PATH = "../Resources/saves/";
	std::string World::FILE_EXTENSION = ".txt";
	std::string World::GENERAL_FILE = "general";
	Vivium::Shader* World::texture_shader = nullptr;

	Vivium::Vector2<int> World::GetWorldPos(const Vivium::Vector2<float>& pos) const
	{
		// Convert to tile-scale, add 0.5, 0.5 to move to center of screen, floor to get clostest tile
		return (Vivium::Renderer::camera->Untransform(pos) / World::PIXEL_SCALE + Vivium::Vector2<float>(0.5f, 0.5f)).floor();
	}

	Tile& World::GetTile(const Vivium::Vector2<int>& pos)
	{
		// Find region tile is in
		Vivium::Vector2<int> region_pos = GetRegionIndex(pos);
		Region& region = m_LoadRegion(region_pos);
		// Get coordinate of tile within region
		Vivium::Vector2<int> relative = pos - (region_pos * Region::LENGTH);
		// Index tile and return
		return region.Index(relative);
	}

	Tile& World::GetTile(int x, int y)
	{
		// Find region tile is in
		Vivium::Vector2<int> region_pos = GetRegionIndex(x, y);
		Region& region = m_LoadRegion(region_pos);

		// Get coordinate of tile within region
		int rx = x - region_pos.x * Region::LENGTH;
		int ry = y - region_pos.y * Region::LENGTH;

		// Index tile and return
		return region.Index(rx, ry);
	}

	Tile* World::GetLoadedTile(const Vivium::Vector2<int>& pos)
	{
		Vivium::Vector2<int> region_index = GetRegionIndex(pos);

		auto it = regions.find(region_index);
		if (it != regions.end()) {
			return &it->second->Index(pos - region_index * Region::LENGTH);
		}
		else {
			return nullptr;
		}
	}

	Tile* World::GetLoadedTile(int x, int y)
	{
		Vivium::Vector2<int> region_index = GetRegionIndex(x, y);

		auto it = regions.find(region_index);
		if (it != regions.end()) {
			return &it->second->Index(Vivium::Vector2<int>(x, y) - region_index * Region::LENGTH);
		}
		else {
			return nullptr;
		}
	}

	World::World(const uint32_t& seed, const std::string& world_name, Player* player)
		// TODO: non static acceleration for leaf wind
		: m_WorldName(world_name), m_Seed(seed), m_Player(player), m_LeafBlockBreaking(100, {0.0f, -400.0f}), m_LeafWind(50, {0.0f, 0.0f})
	{
		std::string fullpath = PATH + world_name + "/";

		// Create folder for our world
		std::filesystem::create_directory(fullpath);

		m_Serialiser = new Vivium::Serialiser(Vivium::Stream::Flags::BINARY | Vivium::Stream::Flags::TRUNC);

		m_DaylightFramebuffer = new Vivium::Framebuffer(Vivium::Application::GetScreenDim());
		m_DaylightShader = new Vivium::Shader("daylight_vertex", "daylight_frag");

		Biome::Init(m_Seed);
		TerrainGenerator::m_Init(m_Seed);

		// Load world
		m_LoadRegions(player);

		m_WorldMap = new WorldMap(MAP_SIZE);

		m_WorldMap->GenerateFullMap({ 0, 0 }, *this);

		m_PlayerLayer = Vivium::Physics::CreateLayer(World::PLAYER_PHYSICS_LAYER, { World::TILE_PHYSICS_LAYER });
		// NOTE: don't need to declare that it will collide with player layer, actually would slightly decrease performance
		// NOTE: also not declaring it will collide with itself since tiles shouldn't move, but this may change in future
		m_TileLayer = Vivium::Physics::CreateLayer(World::TILE_PHYSICS_LAYER, {});

		m_PlayerLayer->bodies.push_back(m_Player->body);

		m_BlockBreakingSound = Vivium::Application::sound_engine->addSoundSourceFromFile((Vivium::Application::resources_path + "sounds/breaking_block.wav").c_str(), irrklang::ESM_AUTO_DETECT, true);

		m_UpdateTimer.Start();
	}

	void World::Init()
	{
		texture_shader = new Vivium::Shader("texture_vertex", "texture_frag");
	}

	void World::Terminate()
	{
		delete texture_shader;
	}

	Vivium::Vector2<int> World::GetRegionIndex(const Vivium::Vector2<int>& pos)
	{
		return Vivium::Vector2<int>(
			std::floor((double)pos.x / (double)Region::LENGTH),
			std::floor((double)pos.y / (double)Region::LENGTH)
			);
	}

	Vivium::Vector2<int> World::GetRegionIndex(int x, int y)
	{
		return Vivium::Vector2<int>(
			std::floor((double)x / (double)Region::LENGTH),
			std::floor((double)y / (double)Region::LENGTH)
			);
	}

	const Vivium::Pathfinding::Map* World::GetObstacleMap() const
	{
		return &m_ObstacleMap;
	}

	Vivium::Vector2<int> World::GetObstacleMapIndex(const Vivium::Vector2<int>& pos) const
	{
		return pos + m_WorldToObstacleMapTransform;
	}

	bool World::GetIsObstacle(const Vivium::Vector2<int>& pos) const
	{
		return m_ObstacleMap.IsObstacle(GetObstacleMapIndex(pos));
	}

	std::string World::m_ToRegionName(const Vivium::Vector2<int>& index) const
	{
		return std::format("{}{}/region{}_{}{}", PATH, m_WorldName, index.x, index.y, FILE_EXTENSION);
	}

	void World::m_LoadRegions(Player* player)
	{
		Vivium::Vector2<float> player_pos;
		if (player != nullptr) {
			player_pos = player->quad->GetCenter() / World::PIXEL_SCALE;
		}

		// Calculate bounds of tiles we need to render
		Vivium::Vector2<int> frame = Vivium::Application::GetScreenDim() / World::PIXEL_SCALE + Vivium::Vector2<int>(1, 1);
		Vivium::Vector2<int> bottom_left = GetRegionIndex(player_pos - frame) - 2; // Adding/Subtracting 2 because map is larger than world
		Vivium::Vector2<int> top_right = GetRegionIndex(player_pos + frame) + 2;

		std::vector<Vivium::Vector2<int>> regions_to_load;
		regions_to_load.reserve(30);

		bool regions_map_empty = regions.empty();

		// Iterate and add to regions to load
		for (int y = bottom_left.y; y <= top_right.y; y++) {
			for (int x = bottom_left.x; x <= top_right.x; x++) {
				if (!regions_map_empty) {
					regions_to_load.push_back({ x, y });
				}
			}
		}

		if (!regions_map_empty) {
			// Iterate loaded regions
			for (auto it = regions.begin(); it != regions.end();) {
				auto find_it = std::find(regions_to_load.begin(), regions_to_load.end(), it->first);

				// If index is already in our regions to load, remove from our regions to load
				if (find_it != regions_to_load.end()) {
					regions_to_load.erase(find_it);
					it++;
				}
				// If it wasn't in our regions to load, erase it
				else {
					// TODO: try to multithread this? is it possible
					m_SerialiseRegion(it->first, it->second);
					delete it->second;
					it = regions.erase(it);
				}
			}
		}

		std::vector<std::thread> loading_threads;

		if (!regions_map_empty && regions_to_load.size() > 0) {
			loading_threads.reserve(regions_to_load.size());

			for (auto& index : regions_to_load) {
				loading_threads.emplace_back([&]() { m_LoadRegion(index); });
			}
		}

		for (std::thread& thread : loading_threads) {
			thread.join();
		}
	}

	Region& World::m_LoadRegion(const Vivium::Vector2<int>& index)
	{
		// Try find region in region map
		regions_mutex.lock();
		auto it = regions.find(index);
		regions_mutex.unlock();

		// If region at that index exists
		if (it != regions.end()) {
			// We don't have to do anything its already loaded
			return *regions.at(index);
		}

		// Since that region doesn't exist already, try load from serialised file
		std::string region_name = m_ToRegionName(index);
		// Checks if region is serialised
		if (std::filesystem::exists(region_name)) {
			// File exists lets deserialise it
			m_DeserialiseRegion(region_name, index);
			// Return region
			return *regions.at(index);
		}

		// Region isn't already loaded and hasn't been serialised, so we have to generate it
		m_GenerateRegion(index); return *regions.at(index);
	}

	std::vector<FloorItem>* World::GetFloorItems(const Vivium::Vector2<int>& pos)
	{
		auto it = floor_items.find(pos);

		if (it != floor_items.end()) {
			return &it->second;
		}

		return nullptr;
	}

	World::~World()
	{
		m_SaveWorld();

		for (auto& [index, region] : regions) {
			delete region;
		}

		delete m_Serialiser;

		delete m_DaylightFramebuffer;
		delete m_DaylightShader;

		delete m_WorldMap;

		m_BlockBreakingSound = nullptr;
	}

	void World::m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index)
	{
		std::lock_guard<std::mutex> guard(regions_mutex);

		// Construct empty region object
		regions.emplace(index, new Region());

		Region* region = regions.at(index);

		m_Serialiser->BeginRead(filename.c_str());
		m_Serialiser->Read((char*)&region->tiles[0], Region::TILES_MEM_SIZE);
		m_Serialiser->Read((char*)&region->biomes[0], Region::BIOMES_MEM_SIZE);

		std::vector<FloorItem> region_floor_items;
		m_Serialiser->Read(&region_floor_items);

		if (!region_floor_items.empty()) {
			floor_items[index] = region_floor_items;
		}

		m_Serialiser->EndRead();
	}

	void World::m_SaveWorld()
	{
		// Serialise player
		m_Player->Save(*this);

		// Serialising region tiles and floor items
		for (auto& [index, region] : regions) {
			m_SerialiseRegion(index, region);
		}
	}

	void World::m_SerialiseRegion(const Vivium::Vector2<int>& index, const Region* region)
	{
		std::string region_path = m_ToRegionName(index);

		m_Serialiser->BeginWrite(region_path.c_str());

		std::lock_guard<std::mutex> guard(regions_mutex);

		// Write tiles of region
		m_Serialiser->Write((char*)&region->tiles[0], Region::TILES_MEM_SIZE);
		// Write biome map for region
		m_Serialiser->Write((char*)&region->biomes[0], Region::BIOMES_MEM_SIZE);

		// Get the floor items for the region
		std::vector<FloorItem>* region_floor_items = GetFloorItems(index);
		// If there are floor items in the region, write the vector
		if (region_floor_items != nullptr) {
			m_Serialiser->Write(*region_floor_items);
		}
		// Otherwise just write a 0 to indicate there are no floor items
		else {
			m_Serialiser->Write<unsigned int>(0);
		}

		m_Serialiser->EndWrite();
	}

	void World::m_GenerateRegion(const Vivium::Vector2<int>& index)
	{
		Region* region;
		{
			std::lock_guard<std::mutex> guard(regions_mutex);

			regions.emplace(index, new Region());

			region = regions.at(index);
		}

		std::unordered_map<Vivium::Vector2<int>, Structure::ID> structures; // Maps bottom left corner of structure to a structure

		for (int y = 0; y < Region::LENGTH; y++) {
			int world_y = y + index.y * Region::LENGTH;
			for (int x = 0; x < Region::LENGTH; x++) {
				int world_x = x + index.x * Region::LENGTH;

				
				Tile& tile = region->Index(x, y);
				Biome::ID& biome = region->IndexBiome(x, y);

				biome = TerrainGenerator::GenerateAt(world_x, world_y, tile, this, structures);
			}
		}

		// TODO: BUG HERE
		// TODO: If a structure is placed at a region border, it may load adjacent regions if it overlaps into them, that region may then also have a structure at a chunk border,
		//		causing it to load the next region, resulting in many extra regions being loaded
		// Iterate and construct structures
		std::lock_guard<std::mutex> guard(regions_mutex);

		for (auto& [pos, struct_id] : structures) {
			Structure::Place(pos, struct_id, this);
		}
	}

	void World::m_UpdateMining(Player* player, float elapsed)
	{
		// Check if player selected tile is mineable
		if (Tile::GetIsMineable(player->selected_tile.foreground)) {
			Vivium::Vector2<int> player_mining_pos = player->selected_tile_pos;

			if (Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1) == Vivium::Input::State::HOLD) {
				// New tile
				if (player_mining_pos != mined_tile_pos) {
					mined_tile_time = 0.0f;
					mined_tile_pos = player_mining_pos;
					mined_tile_id = player->selected_tile.foreground; // TODO only works for foreground tiles
				}
				// Same tile
				else {
					mined_tile_time += elapsed;

					// TODO: give longer delay between sound if tile is taking longer to mine
					if (!Vivium::Application::sound_engine->isCurrentlyPlaying(m_BlockBreakingSound)) {
						Vivium::Application::sound_engine->play2D(m_BlockBreakingSound);
					}
				}
			}
			else {
				mined_tile_time = 0.0f;
				mined_tile_pos = INT_MAX;
				mined_tile_id = Tile::ID::VOID;
			}

			// Mining time should depend on tool as well
			if (mined_tile_time > Tile::GetMiningTime(player->selected_tile.foreground)) {
				Tile& tile = GetTile(mined_tile_pos);
				// Create floor item
				std::vector<Item> item_drops = Tile::GetDropData(tile.foreground).GetRandomDrop();

				for (Item& item : item_drops) {
					if (item.id != Item::ID::VOID) {
						FloorItem new_floor_item(
							item,
							Vivium::Vector2<float>(mined_tile_pos * World::PIXEL_SCALE),
							Vivium::Vector2<float>(World::PIXEL_SCALE * 0.5f)
						);

						// Add new item to floor item map
						m_AddFloorItem(GetRegionIndex(mined_tile_pos), new_floor_item);
					}
				}
				
				if (Structure::IsStructure(tile.foreground)) {
					if (Structure::GetStructureID(tile.foreground) == Structure::ID::TREE) {
						Vivium::Vector2<int> offset = tile.foreground == Tile::ID::TREE_0 ? 0 : Vivium::Vector2<int>(0, 1);
						m_LeafBlockBreaking.Emit(20, 1.0f, (mined_tile_pos + offset) * World::PIXEL_SCALE, 0.0f, 70.0f, 0.0f, 0.0f, Vivium::Math::PI);
					}

					Structure::Delete(mined_tile_pos, tile.foreground, this);
				}
				else {
					// Delete foreground tile
					tile.foreground = Tile::ID::VOID;
				}
			}
		}
		else {
			mined_tile_time = 0.0f;
			mined_tile_pos = Vivium::Vector2<int>(INT_MAX, INT_MAX);
		}
	}

	void World::m_UpdateCurrentBiome()
	{
		Vivium::Vector2<int> player_tile_pos = (m_Player->quad->GetCenter() / World::PIXEL_SCALE).floor();
		Vivium::Vector2<int> player_region_pos = GetRegionIndex(player_tile_pos);
		Region* current_region = regions[player_region_pos];

		m_CurrentBiome = current_region->IndexBiome(player_tile_pos - player_region_pos * Region::LENGTH);
	}

	void World::m_AddFloorItem(const Vivium::Vector2<int>& region_pos, const FloorItem& item)
	{
		auto it = floor_items.find(region_pos);

		// Create new list
		if (it == floor_items.end()) {
			floor_items.insert({ region_pos, {item}});
		}
		else {
			floor_items.at(region_pos).push_back(item);
		}
	}

	void World::m_UpdateMineable(const Tile::ID& id, float& tile_scale, int world_x, int world_y)
	{
		// If our current tile is mineable, and we are currently mining at ile
		if (Tile::GetIsMineable(id) && mined_tile_id != Tile::ID::VOID)
		{
			// If we're currently mining a structure
			if (Structure::IsStructure(mined_tile_id)) {
				// Get the ID of the structure we're mining
				Structure::ID structure_id = Structure::GetStructureID(mined_tile_id);

				// Get the origin of the structure (the point around which the tilemap is based)
				Vivium::Vector2<float> structure_origin = mined_tile_pos - Structure::GetTileOffset(mined_tile_id, structure_id);

				// If the current position we're iterating is within the bounds of the structure we're mining
				if (Structure::IsWithinStructureBounds(Vivium::Vector2<int>(world_x, world_y), structure_origin, structure_id)) {
					// Edit the mining scale
					tile_scale = m_GetMiningTileScale(tile_scale, id);
				}
			}
			// We're not mining a structure, so we just have to check if our iterating pos is the same as our mining pos
			else if (world_x == mined_tile_pos.x && world_y == mined_tile_pos.y) {
				tile_scale = m_GetMiningTileScale(tile_scale, id);
			}
		}
	}

	void World::m_RenderTiles(const Vivium::Vector2<int>& pos)
	{
		// TODO: move
		// Clear everything in bodies layer, since physics update has already happened, and body locations will change next frame
		m_TileBodies.clear();
		m_TileLayer->bodies.clear(); // NOTE: not necessary, but will speed up performance slightly

		Vivium::Vector2<int> frame = Vivium::Application::GetScreenDim() / (PIXEL_SCALE * 2.0f);

		float reg_scale = Region::LENGTH;

		// Add some extra tiles to make sure we don't get ugly black borders
		constexpr int padding = 3;
		int left	= pos.x - frame.x - padding;	int reg_left	= std::floor(left / reg_scale);
		int right	= pos.x + frame.x + padding;	int reg_right	= std::floor(right / reg_scale);
		int bottom	= pos.y - frame.y - padding;	int reg_bottom	= std::floor(bottom / reg_scale);
		int top		= pos.y + frame.y + padding;	int reg_top		= std::floor(top / reg_scale);

		// Stores the current amount of tiles we have rendered
		// std::size_t count = 0;
		// NOTE: if everything on screen has a background, and foreground tile to render, its possible we may exceed max count,
		//		might need something like "(right - left + 1) * (top - bottom + 1) * 2" instead
		std::size_t max_count = (right - left) * (top - bottom) * 2;

		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		Vivium::Batch batch(max_count, &layout);

		for (int reg_y = reg_bottom; reg_y <= reg_top; reg_y++) {
			for (int reg_x = reg_left; reg_x <= reg_right; reg_x++) {
				// Load the region
				Vivium::Vector2<int> region_index(reg_x, reg_y);
				Region& region = m_LoadRegion(region_index);

				// Calculate offset which transforms from region relative coordinates to world coordinates
				Vivium::Vector2<int> region_offset = region_index * Region::LENGTH;

				// Iterate tiles in region
				for (int rel_y = 0; rel_y < Region::LENGTH; rel_y++) {
					int world_y = rel_y + region_offset.y;

					// Check y position is within bounds of what we want to render
					if (world_y >= bottom && world_y < top) {
						for (int rel_x = 0; rel_x < Region::LENGTH; rel_x++) {
							int world_x = rel_x + region_offset.x;

							// Check x position is within bounds of what we want to render
							if (world_x >= left && world_x < right) {
								// Get tile from region, since we know both its x and y are within the camera's bounds
								Tile& tile = region.Index(rel_x, rel_y);

								// Calculate draw coords
								float dx = world_x * World::PIXEL_SCALE;
								float dy = world_y * World::PIXEL_SCALE;

								float halfscale = World::PIXEL_SCALE * 0.5f;

								// Iterate over each tile id
								// TODO: subroutine for this
								for (const Tile::ID& id : { tile.background, tile.foreground }) {
									if (id == Tile::ID::VOID) { continue; }

									float tile_scale = halfscale * Tile::GetScale(id);

									// NOTE: If tile is being mined, the scale should rapidly fluctuate to indicate its being mined
									// Update tile scale if tile is being mined or part of structure that is being mined
									m_UpdateMineable(id, tile_scale, world_x, world_y);

									/* TODO: UpdatePhysics */
									// TODO: bad
									if (Tile::GetIsPhysical(id)) {
										Ref(Vivium::Quad) quad = MakeRef(Vivium::Quad, Vivium::Vector2<float>(dx, dy), Vivium::Vector2<float>(halfscale));
										// TODO: fix later
										Ref(Vivium::Body) body = MakeRef(Vivium::Body, quad, true, 0.0f, 1.0f);

										m_TileBodies.push_back(body);
										m_TileLayer->bodies.push_back(body);
									}

									const Vivium::Vector2<int>& index = Tile::GetAltasIndex(id);
									std::array<float, 8> tex_coords = TextureManager::game_atlas->GetCoordsArray(index);

									batch.Submit({ dx, dy }, tile_scale * 2.0f, &tex_coords[0]);
								}
							}
						}
					}
				}
			}
		}

		Vivium::Batch::BatchData data = batch.End();

		Vivium::Renderer::Submit(data.vertex_buffer.get(), data.index_buffer.get(), texture_shader, TextureManager::game_atlas->GetAtlas().get());
	}

	void World::m_RenderFloorItems(const Vivium::Vector2<int>& pos)
	{
		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		// TODO: find workaround for using vectors and stuff
		std::vector<float> vertex_data;
		std::vector<unsigned short> indexCoords;

		std::size_t count = 0; // Amount of items being rendered

		// Calculate bounds of tiles we need to render
		Vivium::Vector2<int> frame = Vivium::Application::GetScreenDim() / World::PIXEL_SCALE + Vivium::Vector2<int>(1, 1);
		Vivium::Vector2<int> bottom_left = GetRegionIndex(pos - frame);
		Vivium::Vector2<int> top_right = GetRegionIndex(pos + frame);

		// Iterate and add to regions to load
		for (int y = bottom_left.y; y <= top_right.y; y++) {
			for (int x = bottom_left.x; x <= top_right.x; x++) {
				// Get list of floor items for a given region
				std::vector<FloorItem>* items = GetFloorItems(Vivium::Vector2<int>(x, y));

				if (items == nullptr) { continue; }

				// Reserve space needed for the given list of items, assuming all items have a count of 3+
				vertex_data.reserve(vertex_data.size() + (16 * items->size() * 3));
				indexCoords.reserve(indexCoords.size() + (6 * items->size() * 3));

				for (FloorItem& item : *items) {
					// NOTE: bit of a weird place, but having a separate UpdateFloorItem function would make us iterate all the regions twice
					item.Update();

					// TODO: potential for item data to be uninitialised
					const Item& item_data = item.GetItemData();

					if (item_data.id == Item::ID::VOID) { continue; }

					const Ref(Vivium::Quad) item_quad = item.GetQuad();

					const Vivium::Vector2<int>& index = Item::GetAltasIndex(item_data.id);
					std::array<float, 8> tex_coords = TextureManager::game_atlas->GetCoordsArray(index);

					float angle = item_quad->GetAngle();
					float cos_angle = std::cos(angle);
					float sin_angle = std::sin(angle);

					static const Vivium::Vector2<float> item_offsets[3] = {
						Vivium::Vector2<float>(0.0f,  0.0f) * World::PIXEL_SCALE * 0.5f,
						Vivium::Vector2<float>(0.3f,  0.3f) * World::PIXEL_SCALE * 0.5f,
						Vivium::Vector2<float>(-0.3f,  0.3f) * World::PIXEL_SCALE * 0.5f
					};

					int max_index = std::min(item_data.count, (uint16_t)3);

					if (!Item::GetDisplayMultiple(item_data.id)) max_index = 1;

					for (int i = 0; i < max_index; i++) {
						Vivium::Vector2<float> item_offset = Vivium::Math::Rotate(item_offsets[i], cos_angle, sin_angle);

						// Bottom left, Bottom right, Top right, Top left
						std::array<Vivium::Vector2<float>, 4> vertices = item_quad->GetVertices();

						std::array<float, 16> this_vertex_data =
						{
							vertices[0].x + item_offset.x, vertices[0].y + item_offset.y, tex_coords[0], tex_coords[1],
							vertices[1].x + item_offset.x, vertices[1].y + item_offset.y, tex_coords[2], tex_coords[3],
							vertices[2].x + item_offset.x, vertices[2].y + item_offset.y, tex_coords[4], tex_coords[5],
							vertices[3].x + item_offset.x, vertices[3].y + item_offset.y, tex_coords[6], tex_coords[7]
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
						vertex_data.insert(vertex_data.end(), this_vertex_data.begin(), this_vertex_data.end());

						++count;
					}
				}
			}
		}

		if (vertex_data.size() > 0) {
			Vivium::VertexBuffer vb(vertex_data, layout);
			Vivium::IndexBuffer ib(indexCoords);

			Vivium::Renderer::Submit(&vb, &ib, FloorItem::floor_shader, TextureManager::game_atlas->GetAtlas().get());
		}
	}

	float World::m_GetMiningTileScale(float tile_scale, const Tile::ID& id)
	{
		static constexpr float SCALE_RANGE = 8.0f;

		// TODO: cleanup
		// Get time spent mining as percentage (0 -> 1)
		float time_ratio = mined_tile_time / Tile::GetMiningTime(id);
		// Multiply by amount of cycles of growth/shrinking
		time_ratio *= 9.0f;
		// Fix to 0 -> 1 range
		float integer_part; // NOTE: unused
		time_ratio = std::modf(time_ratio, &integer_part);
		// Move to -4 -> 4 range
		time_ratio = (time_ratio - 0.5f) * SCALE_RANGE;
		// Clamp to more than 0
		time_ratio = std::max(0.0f, time_ratio);

		// TODO get time_ratio as percentage of tile scale
		tile_scale += time_ratio;

		return tile_scale;
	}

	void World::m_UpdateObstacleMap()
	{
		Vivium::Vector2<int> player_tile = m_Player->quad->GetCenter() / World::PIXEL_SCALE;

		// TODO: Really need a function/shorthand/preprocessor for this
		Vivium::Vector2<int> frame = Vivium::Application::GetScreenDim() / (PIXEL_SCALE * 2.0f);

		float reg_scale = Region::LENGTH;
		int padding = OBSTACLE_MAP_REGION_PADDING * Region::LENGTH;

		int left = player_tile.x - frame.x - padding;	int reg_left = std::floor(left / reg_scale);
		int right = player_tile.x + frame.x + padding;	int reg_right = std::floor(right / reg_scale);
		int bottom = player_tile.y - frame.y - padding;	int reg_bottom = std::floor(bottom / reg_scale);
		int top = player_tile.y + frame.y + padding;	int reg_top = std::floor(top / reg_scale);

		Vivium::Vector2<int> dim(right - left, top - bottom);

		// Create obstacle map data
		Ref(bool[]) obstacle_data = std::make_shared_for_overwrite<bool[]>(dim.x * dim.y);

		// Fill with obstacles, unloaded areas will be defaulted to obstacles
		for (std::size_t i = 0; i < dim.x * dim.y; i++) {
			obstacle_data.get()[i] = true;
		}

		for (int reg_y = reg_bottom; reg_y <= reg_top; reg_y++) {
			for (int reg_x = reg_left; reg_x <= reg_right; reg_x++) {
				// Load the region
				Vivium::Vector2<int> region_index(reg_x, reg_y);
				Region& region = m_LoadRegion(region_index);

				// Calculate offset which transforms from region relative coordinates to world coordinates
				Vivium::Vector2<int> region_offset = region_index * Region::LENGTH;

				// Iterate tiles in region
				for (int rel_y = 0; rel_y < Region::LENGTH; rel_y++) {
					int world_y = rel_y + region_offset.y;

					// Check y position is within bounds of what we want to render
					if (world_y >= bottom && world_y < top) {
						for (int rel_x = 0; rel_x < Region::LENGTH; rel_x++) {
							int world_x = rel_x + region_offset.x;

							// Check x position is within bounds of what we want to render
							if (world_x >= left && world_x < right) {
								// Get tile from region, since we know both its x and y are within the obstacle map's bounds
								Tile& tile = region.Index(rel_x, rel_y);

								int obs_x = world_x - left;
								int obs_y = world_y - bottom;

								// If either foreground or background are physical: true, else false
								// TODO: needs to be easier to make obstacle map
								obstacle_data.get()[obs_x + obs_y * dim.x] = Tile::GetIsPhysical(tile.foreground) || Tile::GetIsPhysical(tile.background);
							}
						}
					}
				}
			}
		}

		// Update map
		m_ObstacleMap = Vivium::Pathfinding::Map(obstacle_data, dim);
		m_WorldToObstacleMapTransform = -Vivium::Vector2<int>(left, bottom);
	}

	void World::Render(const Vivium::Vector2<int>& pos)
	{
		// TODO: some sort of registry system to auto-update these framebuffers
		// Check if framebuffer dim is different to screen dim, if so update
		Vivium::Vector2<int> screen_dim = Vivium::Application::GetScreenDim();
		if (m_DaylightFramebuffer->GetDim() != screen_dim) {
			m_DaylightFramebuffer->Resize(screen_dim.x, screen_dim.y);
		}

		// Drawing onto daylight framebuffer
		m_DaylightFramebuffer->Bind();
		m_RenderTiles(pos);
		m_RenderFloorItems(pos);
		m_LeafBlockBreaking.Render();
		m_LeafWind.Render();
		m_DaylightFramebuffer->Unbind();

		// Drawing from daylight framebuffer onto screen
		Vivium::Quad screen_quad((Vivium::Vector2<float>)screen_dim * 0.5f, (Vivium::Vector2<float>)screen_dim); // TODO: could be static
		Vivium::Renderer::Submit(&screen_quad, m_DaylightShader, m_DaylightFramebuffer, 0);

		m_WorldMap->Render(screen_dim - Vivium::Vector2<int>(100, 100));

		m_DaylightFramebuffer->Clear();
	}

	void World::Update()
	{
		float elapsed = m_UpdateTimer.GetElapsed();
		m_TimeAlive += elapsed;
		
		m_LoadRegions(m_Player);

		m_UpdateMining(m_Player, elapsed);

		m_UpdateCurrentBiome();

		if (m_CurrentBiome == Biome::ID::FOREST) {
			if (m_TimeAlive > m_LastParticleEmitTime + Biome::ForestBiome::LEAF_PARTICLE_FREQUENCY) {
				m_LastParticleEmitTime = m_TimeAlive;

				Vivium::Vector2<float> top_left = m_Player->quad->GetX() - (Vivium::Application::width * 0.5f);
				top_left.y = m_Player->quad->GetY() + (Vivium::Application::height * Vivium::Random::GetFloat(-1.0f, 1.0f));

				m_LeafWind.Emit(1, 15.0f, top_left, { 100.0f, 0.0f }, 50.0f, 0.0f, 0.0f, Vivium::Math::PI);
			}
		}

		m_WorldMap->GenerateFullMap(m_Player->quad->GetCenter(), *this);

		m_UpdateObstacleMap();
	}

	std::string World::GetName() const
	{
		return m_WorldName;
	}
}