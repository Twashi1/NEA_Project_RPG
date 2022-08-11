#include "World.h"
#include "Player.h"

namespace Game {
	// TODO: path is complicated

	std::string World::PATH = "../Resources/saves/";
	std::string World::FILE_EXTENSION = ".data";
	std::string World::GENERAL_FILE = "general";
	Vivium::Shader* World::texture_shader = nullptr;

	Vivium::TextureAtlas* World::m_TextureAtlas = nullptr;
	std::vector<std::array<float, 4>> World::m_TextureCoords = {};

	Vivium::TextureAtlas* World::m_ItemsAtlas = nullptr; // TODO unique ptr
	std::vector<std::array<float, 4>> World::m_ItemsTextureCoords = {};

	void World::m_PrecalcTextureCoords()
	{
		m_TextureCoords.resize((int)Tile::ID::MAX);

		for (int i = 0; i < (int)Tile::ID::MAX; i++) {
			// TODO: won't work with more complicated sprites
			// TODO: implement get faces into m_TextureAtlas?
			const Vivium::Vector2<int>& atlas_index = Tile::GetAltasIndex(Tile::ID(i));
			std::array<float, 8> vector_coords = m_TextureAtlas->GetCoordsArray(atlas_index);

			// left bottom right top
			m_TextureCoords[i] = { vector_coords[0], vector_coords[1], vector_coords[2], vector_coords[5] };
		}

		m_ItemsTextureCoords.resize((int)Item::ID::MAX);

		for (int i = 0; i < (int)Item::ID::MAX; i++) {
			// TODO: won't work with more complicated sprites
			// TODO: implement get faces into m_TextureAtlas?
			const Vivium::Vector2<int>& atlas_index = Item::GetAltasIndex(Item::ID(i));
			std::array<float, 8> vector_coords = m_ItemsAtlas->GetCoordsArray(atlas_index);

			// left bottom right top
			m_ItemsTextureCoords[i] = { vector_coords[0], vector_coords[1], vector_coords[2], vector_coords[5] };
		}
	}

	Vivium::Vector2<int> World::GetWorldPos(const Vivium::Vector2<float>& pos) const
	{
		// Convert to tile-scale, add 0.5, 0.5 to move to center of screen, floor to get clostest tile
		return (Vivium::Renderer::camera->Untransform(pos) / World::PIXEL_SCALE + Vivium::Vector2<float>(0.5f, 0.5f)).floor();
	}

	Tile& World::GetTile(const Vivium::Vector2<int>& pos)
	{
		// Find region tile is in
		Vivium::Vector2<int> region_pos = m_GetRegionIndex(pos);
		Region& region = m_LoadRegion(region_pos);
		// Get coordinate of tile within region
		Vivium::Vector2<int> relative = pos - (region_pos * Region::LENGTH);
		// Index tile and return
		return region.Index(relative);
	}

	World::World(const uint32_t& seed, const std::string& world_name)
		: m_WorldName(world_name), m_Seed(seed)
	{
		std::string fullpath = PATH + world_name + "/";

		// Create folder for our world
		std::filesystem::create_directory(fullpath);

		m_TextureAtlas = new Vivium::TextureAtlas("tile_atlas.png", { 32, 32 }); // TODO: hardcoded sprite size
		m_ItemsAtlas = new Vivium::TextureAtlas("items_atlas.png", { 32, 32 }); // TODO: hardcoded sprite size

		m_PrecalcTextureCoords();

		m_Serialiser = new Vivium::Serialiser(Vivium::Stream::Flags::BINARY | Vivium::Stream::Flags::TRUNC);

		// Generate world
		m_GenWorld(fullpath);

		m_UpdateTimer.Start();
	}

	Vivium::Vector2<int> World::m_GetRegionIndex(const Vivium::Vector2<int>& pos)
	{
		return Vivium::Vector2<int>(
			std::floor((double)pos.x / (double)Region::LENGTH),
			std::floor((double)pos.y / (double)Region::LENGTH)
			);
	}

	Vivium::Vector2<int> World::m_GetRegionIndex(int x, int y)
	{
		return Vivium::Vector2<int>(
			std::floor((double)x / (double)Region::LENGTH),
			std::floor((double)y / (double)Region::LENGTH)
			);
	}

	std::string World::m_ToRegionName(const Vivium::Vector2<int>& index) const
	{
		return std::format("region{}_{}{}", index.x, index.y, FILE_EXTENSION);
	}

	void World::m_LoadRegions(const Vivium::Vector2<int>& center, int radius)
	{
		// TODO: doesn't delete old regions
		for (int y = center.y - radius; y < center.y + radius + 1; y++) {
			for (int x = center.x - radius; x < center.x + radius + 1; x++) {
				// Get index
				Vivium::Vector2<int> index(x, y);
				// Load region
				m_LoadRegion(index);
			}
		}
	}

	Region& World::m_LoadRegion(const Vivium::Vector2<int>& index)
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
		if (std::filesystem::exists(m_ToRegionName(index))) {
			// File exists lets deserialise it
			m_DeserialiseRegion(region_name, index);
			// Exit
			return regions.at(index);
		}

		// Region isn't already loaded and hasn't been serialised, so we have to generate it
		m_GenerateRegion(index); return regions.at(index);
	}

	std::vector<FloorItem>* World::m_GetFloorItems(const Vivium::Vector2<int>& pos)
	{
		auto it = floor_items.find(pos);

		if (it == floor_items.end()) {
			return nullptr;
		}
		else {
			return &it->second;
		}
	}

	World::~World()
	{
		m_SaveWorld();

		delete m_TextureAtlas;
		delete m_ItemsAtlas;
		delete m_Serialiser;
	}

	void World::m_DeserialiseRegion(const std::string& filename, const Vivium::Vector2<int>& index)
	{
		// Construct empty region object
		Region region;

		m_Serialiser->BeginRead((PATH + m_WorldName + "/" + filename).c_str());
		m_Serialiser->Read((char*)region.tiles, Region::MEMORY_SIZE);
		m_Serialiser->EndRead();

		// Add region to region map
		regions[index] = region;
	}

	void World::m_LoadWorld(const std::string& fullpath)
	{
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
		for (auto& [index, region] : regions) {
			m_SerialiseRegion(index);
		}
	}

	void World::m_SerialiseRegion(const Vivium::Vector2<int>& index)
	{
		Region& region = regions[index];
		std::string region_path = PATH + m_WorldName + "/" + m_ToRegionName(index);

		m_Serialiser->BeginWrite(region_path.c_str());
		m_Serialiser->Write((char*)region.tiles, Region::MEMORY_SIZE);
		m_Serialiser->EndWrite();
	}

	void World::m_GenerateRegion(const Vivium::Vector2<int>& index)
	{
		Region region;

		std::unordered_map<Vivium::Vector2<int>, Structure::ID> structures; // Maps bottom left corner of structure to a structure

		for (int y = 0; y < Region::LENGTH; y++) {
			for (int x = 0; x < Region::LENGTH; x++) {
				float noise_value = m_NoiseTerrain.Get(x, y); // Returns noise value from 0 - 1

				Tile tile;

				Tile::ID tile_id = Tile::ID::GRASS;

				// TODO better world generation

				if (noise_value < 0.4) { tile_id = Tile::ID::GROUND; }
				if (noise_value < 0.3) { tile_id = Tile::ID::SAND; }
				if (noise_value < 0.2) { tile_id = Tile::ID::WATER; }

				tile.base = tile_id;

				float tree_noise = m_NoiseTrees.Get(x + y * Region::LENGTH);

				if (tree_noise < 0.03 && noise_value > 0.2) {
					tile.top = Tile::ID::AMETHYST_NODE;
				}

				if (tree_noise > 0.8 && noise_value > 0.6) {
					structures[Vivium::Vector2<int>(x, y)] = Structure::ID::TREE;
				}

				region.Index(x, y) = tile;
			}
		}

		regions[index] = region;

		// Iterate and construct structures
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
				// TODO: Break tile and drop item
				// TODO: add function for getting tile *ref* in world
				Vivium::Vector2<int> region_pos = m_GetRegionIndex(mined_tile_pos);
				Region& region = m_LoadRegion(region_pos);
				// Get tile
				Tile& tile = region.Index(mined_tile_pos - region_pos * Region::LENGTH);
				// TODO: mined tile could be decor/top
				// TODO: big temporary
				// Create floor item
				Item::ID item_id = Tile::GetDropData(tile.top).GetRandomDrop();
				uint16_t item_count = Vivium::Random::GetInt(1, 3);

				if (item_id != Item::ID::VOID) {
					FloorItem new_item(
						Item(item_id, item_count),
						Vivium::Vector2<float>(mined_tile_pos * World::PIXEL_SCALE),
						Vivium::Vector2<float>(World::PIXEL_SCALE * 0.5f)
					);

					// Add new item to floor item map
					m_AddFloorItem(region_pos, new_item);
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

	void World::m_RenderTiles(const Vivium::Vector2<int>& pos)
	{
		std::vector<float> coords;
		std::vector<unsigned short> indexCoords;

		Vivium::Vector2<int> frame = Vivium::Application::GetScreenDim() / (PIXEL_SCALE * 2.0f) + Vivium::Vector2<int>(1, 1);

		unsigned short count = 0;
		unsigned int max_count = (frame.x * 2 + 1) * (frame.y * 2 + 1) * 3;

		// TODO: use VertexBuffer::StartMap and EndMap

		coords.reserve(16 * max_count);
		indexCoords.reserve(6 * max_count);

		for (int y = pos.y - frame.y; y <= pos.y + frame.y; y++) {
			for (int x = pos.x - frame.x; x <= pos.x + frame.x; x++) {
				// TODO: GetTile?
				// Calculate region index
				Vivium::Vector2<int> region_index = m_GetRegionIndex(x, y);
				// Calculate relative coords
				int rx = x - (region_index.x * Region::LENGTH);
				int ry = y - (region_index.y * Region::LENGTH);

				// Get region
				Region& region = m_LoadRegion(region_index); // NOTE: m_LoadRegion not really needed, since regions should always already be loaded

				// Get tile from region
				Tile& tile = region.Index(rx, ry);

				// Calculate draw coords
				float dx = x * World::PIXEL_SCALE;
				float dy = y * World::PIXEL_SCALE;

				float halfscale = World::PIXEL_SCALE * 0.5f;

				// Iterate over each tile id
				for (const Tile::ID& id : { tile.base, tile.mid, tile.top }) {
					if (id == Tile::ID::VOID) { continue; }

					float tile_scale = halfscale;

					// TODO: lots of magic numbers
					// TODO: generalise for any tile that needs different scale
					if (id == Tile::ID::AMETHYST_NODE) { tile_scale -= 10.0f; }

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
							if (Structure::IsWithinStructureBounds(Vivium::Vector2<int>(x, y), structure_origin, structure_id)) {
								// Edit the mining scale
								tile_scale = m_GetMiningTileScale(tile_scale, id);
							}
						}
						// We're not mining a structure, so we just have to check if our iterating pos is the same as our mining pos
						else if (x == mined_tile_pos.x && y == mined_tile_pos.y) {
							tile_scale = m_GetMiningTileScale(tile_scale, id);
						}
					}

					std::array<float, 4>& faces = m_TextureCoords[(int)id];

					std::array<float, 16> this_coords =
					{
						dx - tile_scale, dy - tile_scale, faces[0], faces[1], // bottom left
						dx + tile_scale, dy - tile_scale, faces[2], faces[1], // bottom right
						dx + tile_scale, dy + tile_scale, faces[2], faces[3], // top right
						dx - tile_scale, dy + tile_scale, faces[0], faces[3]  // top left
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
					coords.insert(coords.end(), this_coords.begin(), this_coords.end());

					count++;
				}
			}
		}

		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2, // position
			Vivium::GLSLDataType::VEC2  // tex coords
		};

		Vivium::VertexBuffer vb(coords, layout);
		Vivium::IndexBuffer ib(indexCoords);

		Vivium::Renderer::Submit(&vb, &ib, texture_shader, m_TextureAtlas->GetAtlas().get());
	}

	void World::m_RenderFloorItems(const Vivium::Vector2<int>& pos)
	{
		// TODO: instead of always reading the 9 surrounding regions, do some calculations to figure out which regions need to be rendered
		// Pos is in tile coordinates, convert to region coordinates
		Vivium::Vector2<int> center_region = m_GetRegionIndex(pos);

		std::vector<float> vertex_data;
		std::vector<unsigned short> indexCoords;

		unsigned int count = 0; // Amount of items being rendered

		for (int y = center_region.y - 1; y <= center_region.y + 1; y++) {
			for (int x = center_region.x - 1; x <= center_region.x + 1; x++) {
				// Get list of floor items for a given region
				std::vector<FloorItem>* items = m_GetFloorItems(Vivium::Vector2<int>(x, y));

				if (items == nullptr) { continue; }

				vertex_data.reserve(vertex_data.size() + (16 * items->size() * 3));
				indexCoords.reserve(indexCoords.size() + (6 * items->size() * 3));

				for (FloorItem& item : *items) {
					// TODO: bit of a weird place, but having a separate UpdateFloorItem function would make us iterate all the regions twice
					item.Update();

					// TODO: potential for item data to be uninitialised
					const Item& item_data = item.GetItemData();

					if (item_data.id == Item::ID::VOID) { continue; }

					const Ref(Vivium::Quad) item_quad = item.GetQuad();

					std::array<float, 4>& faces = m_ItemsTextureCoords[(int)item_data.id];

					static const Vivium::Vector2<float> item_offsets[3] = {
						Vivium::Vector2<float>( 0.0f,  0.0f) * World::PIXEL_SCALE * 0.5f,
						Vivium::Vector2<float>( 0.3f,  0.3f) * World::PIXEL_SCALE * 0.5f,
						Vivium::Vector2<float>(-0.3f,  0.3f) * World::PIXEL_SCALE * 0.5f
					};

					for (int i = 0; i < std::min(item_data.count, (uint16_t)3); i++) {
						Vivium::Vector2<float> item_offset = item_offsets[i];

						// Bottom left, Bottom right, Top right, Top left
						std::array<Vivium::Vector2<float>, 4> vertices = item_quad->GetVertices();

						// TODO: wasting 3 * sizeof(float) bytes for each render, item_count_f only needed once
						std::array<float, 16> this_vertex_data =
						{
							vertices[0].x + item_offset.x, vertices[0].y + item_offset.y, faces[0], faces[1],
							vertices[1].x + item_offset.x, vertices[1].y + item_offset.y, faces[2], faces[1],
							vertices[2].x + item_offset.x, vertices[2].y + item_offset.y, faces[2], faces[3],
							vertices[3].x + item_offset.x, vertices[3].y + item_offset.y, faces[0], faces[3]
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

			Vivium::Renderer::Submit(&vb, &ib, FloorItem::floor_shader, m_ItemsAtlas->GetAtlas().get());
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

		tile_scale += time_ratio;

		return tile_scale;
	}

	void World::m_GenWorld(const std::string& fullpath)
	{
		// TODO
		m_NoiseTerrain = Vivium::Noise::Interpolated(m_Seed, m_Amplitude, m_Wavelength);
		m_NoiseTrees = Vivium::Noise::White(m_Seed, 1.0f, 1);

		m_LoadRegions(Vivium::Vector2<int>(0, 0), 3);
	}

	void World::Render(const Vivium::Vector2<int>& pos)
	{
		m_RenderTiles(pos);
		m_RenderFloorItems(pos);
	}

	void World::Update(Player* player)
	{
		float elapsed = m_UpdateTimer.GetElapsed();
		m_UpdateMining(player, elapsed);
	}
}