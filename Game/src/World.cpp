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
		: m_WorldName(world_name), m_Seed(seed), m_Player(player)
	{
		std::string fullpath = PATH + world_name + "/";

		// Create folder for our world
		std::filesystem::create_directory(fullpath);

		m_Serialiser = new Vivium::Serialiser(Vivium::Stream::Flags::BINARY | Vivium::Stream::Flags::TRUNC);

		m_DaylightFramebuffer = new Vivium::Framebuffer(Vivium::Application::GetScreenDim());
		m_DaylightShader = new Vivium::Shader("daylight_vertex", "daylight_frag");

		Biome::Init(m_Seed);
		TerrainGenerator::m_Init(m_Seed);

		// Generate world
		m_GenWorld(fullpath);

		m_LoadRegions(player);

		m_WorldMap = new WorldMap(48);

		m_WorldMap->GenerateFullMap({ 0, 0 }, *this);

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
		auto it = regions.find(index);

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

	void World::m_LoadWorld(const std::string& fullpath)
	{
		// TODO: old code
		m_Serialiser->BeginRead((fullpath + GENERAL_FILE + FILE_EXTENSION).c_str());

		// Get version number

		// Deserialise<VersionNumber>(m_serialiser, &serialised_version);
		// If version number is different

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
		if (Tile::GetIsMineable(player->selected_tile.top)) {
			Vivium::Vector2<int> player_mining_pos = player->selected_tile_pos;

			if (Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1) == Vivium::Input::State::HOLD) {
				// New tile
				if (player_mining_pos != mined_tile_pos) {
					mined_tile_time = 0.0f;
					mined_tile_pos = player_mining_pos;
					mined_tile_id = player->selected_tile.top; // TODO only works for top tiles
				}
				// Same tile
				else {
					mined_tile_time += elapsed;
				}
			}
			else {
				mined_tile_time = 0.0f;
				mined_tile_pos = Vivium::Vector2<int>(INT_MAX, INT_MAX);
				mined_tile_id = Tile::ID::VOID;
			}

			// TODO: allow different time for each object
			if (mined_tile_time > Tile::GetMiningTime(player->selected_tile.top)) {
				Tile& tile = GetTile(mined_tile_pos);
				// TODO: mined tile could be decor/top
				// Create floor item
				std::vector<Item> item_drops = Tile::GetDropData(tile.top).GetRandomDrop();

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
				
				// TODO: top tile only
				if (Structure::IsStructure(tile.top)) {
					Structure::Delete(mined_tile_pos, tile.top, this);
				}
				else {
					// Delete top tile
					tile.top = Tile::ID::VOID;
				}
			}
		}
		else {
			mined_tile_time = 0.0f;
			mined_tile_pos = Vivium::Vector2<int>(INT_MAX, INT_MAX);
		}
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
		// NOTE: if everything on screen has a bot, mid, and top tile to render, its possible we may exceed max count,
		//		might need something like "(right - left + 1) * (top - bottom + 1) * 3" instead
		std::size_t max_count = (right - left) * (top - bottom) * 3;

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
								for (const Tile::ID& id : { tile.bot, tile.mid, tile.top }) {
									if (id == Tile::ID::VOID) { continue; }

									float tile_scale = halfscale * Tile::GetScale(id);

									// NOTE: If tile is being mined, the scale should rapidly fluctuate to indicate its being mined
									// Update tile scale if tile is being mined or part of structure that is being mined
									m_UpdateMineable(id, tile_scale, world_x, world_y);

									/* TODO: UpdatePhysics */
									// TODO: bad
									if (Tile::GetIsPhysical(id)) {
										Ref(Vivium::Quad) quad = MakeRef(Vivium::Quad, Vivium::Vector2<float>(dx, dy), Vivium::Vector2<float>(halfscale));
										Ref(Vivium::Body) body = MakeRef(Vivium::Body, quad, true, 0.0f, 0.0f);

										Vivium::Application::physics->Register(body, World::PHYSICS_TILE_LAYER);
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
		// TODO: instead of always reading the 9 surrounding regions, do some calculations to figure out which regions need to be rendered
		// Pos is in tile coordinates, convert to region coordinates
		Vivium::Vector2<int> center_region = GetRegionIndex(pos);

		// TODO: store total floor item count
		std::vector<float> vertex_data;
		std::vector<unsigned short> indexCoords;

		unsigned int count = 0; // Amount of items being rendered

		for (int y = center_region.y - 1; y <= center_region.y + 1; y++) {
			for (int x = center_region.x - 1; x <= center_region.x + 1; x++) {
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
						Vivium::Vector2<float>( 0.0f,  0.0f) * World::PIXEL_SCALE * 0.5f,
						Vivium::Vector2<float>( 0.3f,  0.3f) * World::PIXEL_SCALE * 0.5f,
						Vivium::Vector2<float>(-0.3f,  0.3f) * World::PIXEL_SCALE * 0.5f
					};

					for (int i = 0; i < std::min(item_data.count, (uint16_t)3); i++) {
						Vivium::Vector2<float> item_offset = Vivium::Math::Rotate(item_offsets[i], cos_angle, sin_angle);

						// Bottom left, Bottom right, Top right, Top left
						std::array<Vivium::Vector2<float>, 4> vertices = item_quad->GetVertices();

						// TODO: wasting 3 * sizeof(float) bytes for each render, item_count_f only needed once
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

						count++;
					}
				}
			}
		}

		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		if (vertex_data.size() > 0) {
			Vivium::VertexBuffer vb(vertex_data, layout);
			Vivium::IndexBuffer ib(indexCoords);

			Vivium::Renderer::Submit(&vb, &ib, FloorItem::floor_shader, TextureManager::game_atlas->GetAtlas().get());
		}
	}

	float World::m_GetMiningTileScale(float tile_scale, const Tile::ID& id)
	{
		// TODO: cleanuop
		// Its the currently mined tile
		// TODO: move to function
		// TODO: generalise for any mineable tile
		// Get time spent mining as percentage (0 -> 1)
		float time_ratio = mined_tile_time / Tile::GetMiningTime(id);
		// Multiply by amount of cycles of growth/shrinking
		time_ratio *= 9;
		// Fix to 0 -> 1 range
		float integer_part; // NOTE: unused
		time_ratio = std::modf(time_ratio, &integer_part);
		// Move to -4 -> 4 range
		time_ratio = (time_ratio - 0.5f) * 8;
		// Clamp to more than 0
		time_ratio = std::max(0.0f, time_ratio);

		// TODO get time_ratio as percentage of tile scale
		tile_scale += time_ratio;

		return tile_scale;
	}

	void World::m_GenWorld(const std::string& fullpath)
	{
		// TODO
		m_NoiseTerrain = Vivium::Noise::Interpolated(m_Seed, m_Amplitude, m_Wavelength);
		m_NoiseTrees = Vivium::Noise::White(m_Seed, 1.0f, 1);
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
		
		m_LoadRegions(m_Player);

		m_UpdateMining(m_Player, elapsed);

		m_WorldMap->GenerateFullMap(m_Player->quad->GetCenter(), *this);

		Vivium::Application::physics->ClearLayer(World::PHYSICS_TILE_LAYER);
		Vivium::Application::physics->Register(m_Player->body, World::PHYSICS_TILE_LAYER);
	}

	std::string World::GetName() const
	{
		return m_WorldName;
	}
}