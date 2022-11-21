#include "NPC.h"
#include "World.h"
#include "Player.h"

namespace Game {
	World* NPC::world = nullptr;
	std::shared_ptr<Vivium::Shader> NPC::m_Shader = nullptr;
	std::unique_ptr<Vivium::Shader> NPC::m_HealthbarShader = nullptr;

#define NEW_BEHAVIOUR_PTR(T, ...) std::make_shared<T>(T::Global(__VA_ARGS__))

	std::array<NPC::Properties, (uint8_t)NPC::ID::MAX> NPC::m_Properties = {
		// COWS VVV
		Properties({0, 4}, {
			NEW_BEHAVIOUR_PTR(Behaviours::Wandering, 800.0f, {0.5f, 1.0f}, 300.0f, 1000.0f),
			NEW_BEHAVIOUR_PTR(Behaviours::Idle, 3.0f)
		}), // COWS ^^^ PIGS VVV
		Properties({4, 4}, {
			NEW_BEHAVIOUR_PTR(Behaviours::Wandering, 800.0f, {0.5f, 1.0f}, 300.0f, 1000.0f),
			NEW_BEHAVIOUR_PTR(Behaviours::Idle, 3.0f)
		}),  // PIGS ^^^ SLIMES VVV
		Properties({0, 5}, {
			NEW_BEHAVIOUR_PTR(Behaviours::SlimeAttack, 800.0f, 200.0f, 10.0f, 0.0f, 10.0f, nullptr),
			NEW_BEHAVIOUR_PTR(Behaviours::Hunting, 500.0f, 1000.0f, Behaviours::Wandering::Global(800.0f, {0.5f, 1.0f}, 300.0f, 1000.0f)),
			NEW_BEHAVIOUR_PTR(Behaviours::Idle, 0.0f)
		}) // SLIMES ^^^
	};

#undef NEW_BEHAVIOUR_PTR

	void NPC::m_UpdatePathing()
	{
		// If we're moving somewhere
		if (!path_destinations.empty()) {
			Vivium::Vector2<float> tile_pos = body->quad->GetCenter() / World::PIXEL_SCALE;

			// Distance between our pos and dest
			float dist = Vivium::Vector2<float>::Distance(tile_pos, path_destinations.front());

			// TODO: use squared distance
			// We're close enough to the destination to remove it
			if (dist < PATHFINDING_EPSILON) {
				path_destinations.pop();
			}
		}
	}

	void NPC::Init()
	{
		// Initialise shaders
		m_Shader = std::make_shared<Vivium::Shader>("texture_vertex", "npc_frag");
		m_HealthbarShader = std::make_unique<Vivium::Shader>("healthbar_vertex", "healthbar_frag");
		m_HealthbarShader->Bind();
		m_HealthbarShader->SetUniform1f("u_WaveOffset", 0.0f);
	}

	NPC::NPC(const NPC::ID& id, std::shared_ptr<Vivium::Body> body, const BehaviourDataMap& data)
		: id(id), body(body), behaviour_data(data)
	{
		// Get texture index for this npc's default state
		current_texture_index = NPC::GetAtlasIndex(id);
	}

	NPC::NPC(NPC&& other) noexcept
		: id(std::move(other.id)), body(other.body), path_destinations(std::move(other.path_destinations)), health(std::move(other.health)), behaviour_data(std::move(other.behaviour_data)), current_texture_index(std::move(other.current_texture_index))
	{}

	void NPC::Update()
	{
		// Update health value
		health.Update();

		// TODO: lots of indexing here, no way to speed this up by storing some ptrs?
		const std::vector<std::shared_ptr<Behaviours::Behaviour>>& behaviours = GetBehaviours(id);

		// Get current behaviour ptr
		const std::shared_ptr<Behaviours::Behaviour>& m_CurrentBehaviour = behaviours.at(m_CurrentBehaviourIndex);
		// Get ID of current behaviour
		m_CurrentBehaviourID = m_CurrentBehaviour->GetID();
		// Use ID to get the behaviour data for that id
		std::shared_ptr<Behaviours::Behaviour::Client>& current_behaviour_data = behaviour_data.at(m_CurrentBehaviourID);

		// If this is the first frame of the behaviour, start it
		if (m_BeginBehaviour) {
			m_CurrentBehaviour->Begin(this, current_behaviour_data);
			m_BeginBehaviour = false;
		}

		// Update NPC pathing
		m_UpdatePathing();

		// Update body physics
		body->Update();
		// Update animator
		animator.Update();

		// TODO: Update and IsOver may cause two dynamic casts, might be pretty slow
		// TODO: no clear distinction between update and execute on? maybe call both Update, just different parameters
		m_CurrentBehaviour->Update(this, current_behaviour_data);
		m_CurrentBehaviour->ExecuteOn(this);

		// If current behaviour has ended, switch to next
		if (m_CurrentBehaviour->IsOver(this, current_behaviour_data)) {
			// Roll over
			m_CurrentBehaviourIndex = (m_CurrentBehaviourIndex + 1) % behaviours.size();
			m_BeginBehaviour = true;
		}
	}

	void NPC::Submit(Vivium::Batch* npc_batch, Vivium::Batch* healthbar_batch) const
	{
		// TODO: precompute tex coords
		std::array<float, 8> npc_tex = TextureManager::game_atlas->GetCoordsArray(current_texture_index);

		// Submit npc vertices and texture coords
		npc_batch->Submit(body->quad->GetCenter(), body->quad->GetDim(), &npc_tex[0]);

		// Submit npc healthbar
		// If we're not at 100% health
		if (health.GetNormalised() < 1.0f) {
			// TODO: precompute tex coords
			static const std::array<float, 8> default_tex = {
				0.0f, 0.0f,
				1.0f, 0.0f,
				1.0f, 1.0f,
				0.0f, 1.0f
			};

			// Get texture coordinates for healthbar and mask
			std::array<float, 8> healthbar_tex = TextureManager::game_atlas->GetCoordsArray({ 7, 9 }, { 9, 9 });
			std::array<float, 8> mask_tex	   = TextureManager::game_atlas->GetCoordsArray({ 7, 8 }, { 9, 8 });

			// TODO: shouldn't be filling this up ourselves
			// TODO: shouldn't be calculating this ourselves as well
			std::size_t size = (2 + 2 + 2 + 2 + 1) * 4;
			float* vertex_data = new float[size];

			// TODO: bad place for these variables as well
			const float healthbar_width  = 96.0f;
			const float healthbar_height = 32.0f;
			const float healthbar_scale  = 2.0f;

			Vivium::Vector2<float> pos = body->quad->GetCenter();
			pos.y += 100.0f; // Display a bit above

			float halfwidth = healthbar_width * 0.5f * healthbar_scale;
			float halfheight = healthbar_height * 0.5f * healthbar_scale;

			// Calculate bounds of healthbar
			float left = pos.x - halfwidth;
			float right = pos.x + halfwidth;
			float bottom = pos.y - halfheight;
			float top = pos.y + halfheight;

			// Position vertices for healtbar
			float vertices[8] = {
				left, bottom,
				right, bottom,
				right, top,
				left, top
			};

			// Fill vertex data according to buffer layout
			int cindex = 0;
			for (int i = 0; i < 4; i++) {
				vertex_data[cindex++] = vertices[i * 2];
				vertex_data[cindex++] = vertices[i * 2 + 1];

				vertex_data[cindex++] = default_tex[i * 2];
				vertex_data[cindex++] = default_tex[i * 2 + 1];

				vertex_data[cindex++] = healthbar_tex[i * 2];
				vertex_data[cindex++] = healthbar_tex[i * 2 + 1];

				vertex_data[cindex++] = mask_tex[i * 2];
				vertex_data[cindex++] = mask_tex[i * 2 + 1];

				vertex_data[cindex++] = health.GetNormalised();
			}

			// Submit to batch
			healthbar_batch->Submit(vertex_data, size);
		}
	}

	void NPC::CheckProjectileCollision(Weapon::Projectile** projectiles, std::size_t size)
	{
		// TODO: move these consts
		static constexpr float MAX_DIST = 100.0f;
		static constexpr float MAX_DIST_SQR = MAX_DIST * MAX_DIST;

		// Get NPC position
		Vivium::Vector2<float> my_pos = body->quad->GetCenter();

		// Iterate all projectiles
		for (std::size_t i = 0; i < size; i++) {
			Weapon::Projectile* proj = projectiles[i];

			// Here, there is a guarantee the proj is not out of scope/reallocated, since this is between the projectile system updates
			if (proj->IsAlive()) {
				// Get distance from ourselves to projectile
				float dist = Vivium::Vector2<float>::SqrDistance(my_pos, proj->position);

				// They have collided (or are close enough to consider that they have)
				if (dist < MAX_DIST_SQR) {
					// Create hit event
					std::shared_ptr<Weapon::Projectile::Hit> hit = std::make_shared<Weapon::Projectile::Hit>(proj->GetDamage(), proj->GetKnockback(), this, proj);
					// Send to event system
					Vivium::EventSystem::AddEvent(hit);

					// TODO: normally projectile would be killed here, but should be killed in the handler instead
					// Projectile is deleted upon contact (if piercing projectiles ever made, this should be disabled)
					// proj->Kill();
				}
			}
		}
	}

	void NPC::Write(Vivium::Serialiser& s) const
	{
		s.Write((std::underlying_type<NPC::ID>::type)id);

		if (body == nullptr) {
			s.Write(Vivium::Vector2<int>(INT_MAX));
		}
		else {
			s.Write(body->quad->GetCenter());
		}

		s.Write(behaviour_data.size());

		// Iterate all behaviour data and write
		for (auto& [id, client] : behaviour_data) {
			s.Write((std::underlying_type<Behaviours::Behaviour::ID>::type)id);
			client->Write(s);
		}
	}

	void NPC::Read(Vivium::Serialiser& s)
	{
		s.Read((std::underlying_type<NPC::ID>::type*) & id);

		Vivium::Vector2<float> pos;
		s.Read(&pos);

		// Create a default body from position
		body = std::make_shared<Vivium::Body>(
			std::make_shared<Vivium::Quad>(
				pos, World::PIXEL_SCALE
				), false, 1.0f, 1.0f, false
			);

		current_texture_index = GetProperties(id).atlas_index;

		// Reading behaviour data
		std::size_t size;
		s.Read(&size);

		for (std::size_t i = 0; i < size; i++) {
			Behaviours::Behaviour::ID id;
			s.Read((std::underlying_type<Behaviours::Behaviour::ID>::type*) & id);

			std::shared_ptr<Behaviours::Behaviour::Client> new_data_ptr = nullptr;

#define DEFINE_READ_FOR(T) \
std::shared_ptr<T> data_ptr = std::make_shared<T>(); \
data_ptr->Read(s); \
new_data_ptr = dynamic_pointer_cast<Behaviours::Behaviour::Client>(data_ptr);

			// Switch ID to get correct method to read different types of client data
			switch (id) {
			case Behaviours::Behaviour::ID::IDLE:
			{
				DEFINE_READ_FOR(Behaviours::Idle::Client)
			} break;

			case Behaviours::Behaviour::ID::WANDER:
			{
				DEFINE_READ_FOR(Behaviours::Wandering::Client)
			} break;

			case Behaviours::Behaviour::ID::HUNTING:
			{
				DEFINE_READ_FOR(Behaviours::Hunting::Client)
			} break;

			case Behaviours::Behaviour::ID::SLIME_ATTACK:
			{
				DEFINE_READ_FOR(Behaviours::SlimeAttack::Client)
			} break;

#undef DEFINE_READ_FOR

			default:
				LogError("Unrecognised behaviour id: {}", (uint8_t)id); break;
			}

			if (new_data_ptr != nullptr) {
				behaviour_data.insert({ id, new_data_ptr });
			}
		}
	}

	const NPC::Properties& NPC::GetProperties(const NPC::ID& id) { return NPC::m_Properties[(uint8_t)id]; }
	const std::vector<std::shared_ptr<Behaviours::Behaviour>>& NPC::GetBehaviours(const NPC::ID& id) { return NPC::m_Properties[(uint8_t)id].behaviours; }
	const Vivium::Vector2<int>& NPC::GetAtlasIndex(const NPC::ID& id) { return NPC::m_Properties[(uint8_t)id].atlas_index; }

	NPC::Properties::Properties(const Vivium::Vector2<int>& atlas_index, const std::initializer_list<std::shared_ptr<Behaviours::Behaviour>>& behaviours)
		: atlas_index(atlas_index), behaviours(behaviours) {}

	namespace Behaviours {
		Idle::Idle(const Idle::Global& global) : global(global) {}

		void Idle::Write(Vivium::Serialiser& s) const
		{
			s.Write(global);
		}

		void Idle::Read(Vivium::Serialiser& s)
		{
			s.Read(&global);
		}

		Behaviour::ID Idle::GetID() const
		{
			return Behaviour::ID::IDLE;
		}

		void Idle::Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			std::shared_ptr<Idle::Client> my_client = dynamic_pointer_cast<Idle::Client>(client);
			my_client->thinking_timer.Start();
		}

		void Idle::ExecuteOn(NPC* npc) const {
			// Kill velocity and acceleration
			npc->body->vel = 0.0f;
			npc->body->acc = 0.0f;
		}

		void Idle::Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const {}

		bool Idle::IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// Simple wait for some time
			std::shared_ptr<Idle::Client> my_client = dynamic_pointer_cast<Idle::Client>(client);

			// If elapsed time more than thinking time, we have ended
			bool isOver = my_client->thinking_timer.GetElapsedNoReset() > global.thinking_time;
			
			if (isOver) {
				my_client->thinking_timer.Reset();
			}

			return isOver;
		}

		Wandering::Global::Global(float wander_range, Vivium::Vector2<float> wander_range_variation, float wander_speed, float home_range)
			: wander_range(wander_range), wander_range_variation(wander_range_variation), wander_speed(wander_speed), home_range(home_range)
		{}

		Wandering::Wandering(const Wandering::Global& global) : global(global) {}

		Behaviour::ID Wandering::GetID() const
		{
			return Behaviour::ID::WANDER;
		}

		void Wandering::Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const {}

		void Wandering::ExecuteOn(NPC* npc) const
		{
			// If we still have places to path to, don't recalculate the path
			if (!npc->path_destinations.empty()) return;

			std::shared_ptr<Wandering::Client> my_client = dynamic_pointer_cast<Wandering::Client>(npc->behaviour_data.at(Behaviour::ID::WANDER));

			// Select direction to move in
			Vivium::Vector2<float> move_vector = Vivium::Random::GetVector2f(
				global.wander_range * Vivium::Random::GetFloat(global.wander_range_variation.x, global.wander_range_variation.y)
			);

			// Select a tile in that direction
			Vivium::Vector2<float> pos = npc->body->quad->GetCenter();
			Vivium::Vector2<float> dest = pos + move_vector;
			Vivium::Vector2<int> dest_tile = dest / World::PIXEL_SCALE;

			// Attempt to move to valid tile
			// TODO: check tile as well
			if (!NPC::world->GetIsObstacle(dest_tile)) {
				Vivium::Vector2<int> rel_dest = NPC::world->GetObstacleMapIndex(dest_tile);
				Vivium::Vector2<int> rel_start = NPC::world->GetObstacleMapIndex(pos / World::PIXEL_SCALE);

				// Calculate path
				Vivium::Pathfinding::Path path = Vivium::Pathfinding::Calculate(rel_start, rel_dest, *NPC::world->GetObstacleMap());
			
				// Iterate nodes and push destinations to our npc
				for (auto& node : path.GetNodes()) {
					npc->path_destinations.push(NPC::world->ObstacleMapToWorld(node.pos));
				}
			}
		}

		void Wandering::Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// Update direction
			if (!npc->path_destinations.empty()) {
				// Get destination
				Vivium::Vector2<float> dest = npc->path_destinations.front();
				// Get current position
				Vivium::Vector2<float> current = npc->body->quad->GetCenter() / World::PIXEL_SCALE;
				// Calculate direction
				Vivium::Vector2<float> direction = Vivium::Vector2<float>::Normalise(dest - current);

				// Set our velocity to that direction
				npc->body->vel = direction * global.wander_speed;
			}
		}

		bool Wandering::IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO: check if stuck
			return npc->path_destinations.empty();
		}

		void Wandering::Write(Vivium::Serialiser& s) const
		{
			s.Write(global);
		}

		void Wandering::Read(Vivium::Serialiser& s)
		{
			s.Read(&global);
		}

		Idle::Global::Global(float thinking_time)
			: thinking_time(thinking_time) {}
		
		Wandering::Client::Client(const Vivium::Vector2<int>& home_tile)
			: home_tile(home_tile) {}

		void Behaviour::Write(Vivium::Serialiser& s) const {}
		void Behaviour::Read(Vivium::Serialiser& s) {}

		void Behaviour::Client::Write(Vivium::Serialiser& s) const {}
		void Behaviour::Client::Read(Vivium::Serialiser& s) {}

		void Idle::Client::Write(Vivium::Serialiser& s) const
		{
			s.Write<float>(thinking_timer.GetInternalTime());
		}

		void Idle::Client::Read(Vivium::Serialiser& s)
		{
			float internal_time;
			s.Read(&internal_time);
			thinking_timer.SetTime(internal_time);
		}

		void Wandering::Client::Write(Vivium::Serialiser& s) const
		{
			s.Write(home_tile);
		}

		void Wandering::Client::Read(Vivium::Serialiser& s)
		{
			s.Read(&home_tile);
		}

		SlimeAttack::Global::Global(float speed, float attack_range, float damage, float knockback, float attack_speed, Vivium::Animator::Data* _anim_data)
			: speed(speed), attack_range(attack_range), damage(damage), knockback(knockback), attack_speed(attack_speed)
		{
			if (_anim_data != nullptr) {
				this->anim_data = *_anim_data;
			}
		}

		void SlimeAttack::Global::Write(Vivium::Serialiser& s) const
		{
			s.Write(speed);
			s.Write(attack_range);
			s.Write(damage);
			s.Write(knockback);
			s.Write(attack_speed);
			s.Write(anim_data);
		}

		void SlimeAttack::Global::Read(Vivium::Serialiser& s)
		{
			s.Read(&speed);
			s.Read(&attack_range);
			s.Read(&damage);
			s.Read(&knockback);
			s.Read(&attack_speed);
			s.Read(&anim_data);
		}

		SlimeAttack::SlimeAttack(const Global& global) : global(global) {}

		void SlimeAttack::Client::Write(Vivium::Serialiser& s) const {}

		void SlimeAttack::Client::Read(Vivium::Serialiser& s) {}

		void SlimeAttack::Write(Vivium::Serialiser& s) const
		{
			global.Write(s);
		}

		void SlimeAttack::Read(Vivium::Serialiser& s)
		{
			Global global;
			global.Read(s);
		}

		Behaviour::ID SlimeAttack::GetID() const { return Behaviour::ID::SLIME_ATTACK; }

		void SlimeAttack::Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			std::shared_ptr<SlimeAttack::Client> my_client = dynamic_pointer_cast<SlimeAttack::Client>(client);

			npc->animator.Switch(global.anim_data);
			npc->animator.Pause();

			// If we're too far from player, we shouldn't attack
			Vivium::Vector2<float> player_pos = NPC::world->GetPlayer()->body->quad->GetCenter();
			Vivium::Vector2<float> npc_pos = npc->body->quad->GetCenter();
			Vivium::Vector2<int> player_tile = player_pos / World::PIXEL_SCALE;
			Vivium::Vector2<int> npc_tile = npc_pos / World::PIXEL_SCALE;
			// NOTE: manhattan distance
			int dist = std::abs(player_tile.x - npc_tile.x) + std::abs(player_tile.y - npc_tile.y);
			
			// TODO: some sort of distance check to ensure we're not jumping onto an enemy from a mile away
			if (true) {
				// Set velocity towards player
				// TODO: Speed constant
				npc->body->vel = Vivium::Vector2<float>::Normalise(player_pos - npc_pos) * 200.0f;
				// Ignore collision
				npc->body->isPhysical = false;
			
				npc->animator.Start();
				npc->animator.Resume();
			}
		}

		void SlimeAttack::ExecuteOn(NPC* npc) const
		{
			// TODO: nothing?
		}

		void SlimeAttack::Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			std::shared_ptr<SlimeAttack::Client> my_client = dynamic_pointer_cast<SlimeAttack::Client>(client);

			// TODO: this should be moved inside NPC update method
			int r = npc->animator.GetCurrentTextureIndex();

			if (r != -1) npc->current_texture_index = TextureManager::game_atlas->GetVectorIndex(r);

			// Set velocity towards player
			// TODO: Speed constant
			Vivium::Vector2<float> player_pos = NPC::world->GetPlayer()->body->quad->GetCenter();
			Vivium::Vector2<float> npc_pos = npc->body->quad->GetCenter();
			npc->body->vel = Vivium::Vector2<float>::Normalise(player_pos - npc_pos) * 200.0f;

			// If on last frame of animation, deal damage to player
			// TODO: change from current implementation which just checks distance
			if (npc->animator.HasEnded()) {
				// TODO: damage numbers should be in slime attack globals
				NPC::world->GetPlayer()->health.Damage(global.damage);

				LogTrace("Dealt damage to player, health is now: {}%", NPC::world->GetPlayer()->health.GetNormalised() * 100.0f);
			}
		}

		bool SlimeAttack::IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			std::shared_ptr<SlimeAttack::Client> my_client = dynamic_pointer_cast<SlimeAttack::Client>(client);

			bool animation_ended = npc->animator.HasEnded();
			
			// If we're too far from player, we shouldn't attack
			Vivium::Vector2<int> player_tile = NPC::world->GetPlayer()->body->quad->GetCenter() / World::PIXEL_SCALE;
			Vivium::Vector2<int> npc_tile = npc->body->quad->GetCenter() / World::PIXEL_SCALE;
			// NOTE: manhattan distance
			int dist = std::abs(player_tile.x - npc_tile.x) + std::abs(player_tile.y - npc_tile.y);

			// TODO: instead of 3, get some constant from somewhere
			bool isOver = animation_ended;

			if (isOver || dist > 3) {
				// TODO: not sure how this will work in future
				// npc->body->isPhysical = true;
				npc->current_texture_index = NPC::GetAtlasIndex(npc->id);
			}

			return isOver;
		}

		Hunting::Global::Global(float notice_range, float leash_range, const Wandering::Global& wandering)
			: notice_range(notice_range), leash_range(leash_range), wandering(wandering)
		{}

		void Hunting::Global::Write(Vivium::Serialiser& s) const
		{
			s.Write(notice_range);
			s.Write(leash_range);
		}
		
		void Hunting::Global::Read(Vivium::Serialiser& s)
		{
			s.Read(&notice_range);
			s.Read(&leash_range);
		}

		Hunting::Hunting(const Global& global)
			: global(global) {}

		void Hunting::Write(Vivium::Serialiser& s) const
		{
			s.Write(global);
		}

		void Hunting::Read(Vivium::Serialiser& s)
		{
			s.Read(&global);
		}

		Behaviour::ID Hunting::GetID() const
		{
			return Behaviour::ID::HUNTING;
		}

		void Hunting::Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const {}

		void Hunting::ExecuteOn(NPC* npc) const
		{
			// Get player location
			const Player* player = NPC::world->GetPlayer();
			Vivium::Vector2<int> position = player->body->quad->GetCenter() / World::PIXEL_SCALE;

			// If player is too far from NPC, wander randomly
			// If the difference between the player location is too different to the final destination of the npc, recalculate the path
			Vivium::Vector2<int> npc_pos = npc->body->quad->GetCenter() / World::PIXEL_SCALE;

			Vivium::Vector2<int> npc_dest = npc_pos;
			int dist = INT_MAX;

			// If we are pathing somewhere, we'll want to check distance from our destination to the player
			if (!npc->path_destinations.empty()) {
				npc_dest = npc->path_destinations.back();
				// NOTE: Using manhattan distance
				// Calculating distance from npc's final destination to player's current position
				dist = std::abs(position.x - npc_dest.x) + std::abs(position.y - npc_dest.y);
			}

			// Wander randomly since our current position is outside the notice range, and we have nowhere to path to
			if (dist > global.notice_range && npc->path_destinations.empty()) {
				// TODO: basically a copy of the wandering algorithm, but I can't inherit due to some specific differences in client
				std::shared_ptr<Hunting::Client> my_client = dynamic_pointer_cast<Hunting::Client>(npc->behaviour_data.at(Behaviour::ID::HUNTING));

				// Select direction to move in
				Vivium::Vector2<float> move_vector = Vivium::Random::GetVector2f(
					global.wandering.wander_range * Vivium::Random::GetFloat(global.wandering.wander_range_variation.x, global.wandering.wander_range_variation.y)
				);

				// Select a tile in that direction
				Vivium::Vector2<float> pos = npc->body->quad->GetCenter();
				Vivium::Vector2<float> dest = pos + move_vector;
				Vivium::Vector2<int> dest_tile = dest / World::PIXEL_SCALE;

				// Attempt to move to valid tile
				// TODO: check tile as well
				if (!NPC::world->GetIsObstacle(dest_tile)) {
					Vivium::Vector2<int> rel_dest = NPC::world->GetObstacleMapIndex(dest_tile);
					Vivium::Vector2<int> rel_start = NPC::world->GetObstacleMapIndex(pos / World::PIXEL_SCALE);

					Vivium::Pathfinding::Path path = Vivium::Pathfinding::Calculate(rel_start, rel_dest, *NPC::world->GetObstacleMap());

					// Iterate nodes and push destinations to our npc
					for (auto& node : path.GetNodes()) {
						npc->path_destinations.push(NPC::world->ObstacleMapToWorld(node.pos));
					}
				}
			}
			// Recalculate path to player since our destination is too far
			else if (dist > global.recalc_dist) {
				// Clear path destinations
				// TODO: feel like this needs testing, is there really no memory leak happening here?
				// NOTE: slightly disgusting hack, based off of https://stackoverflow.com/questions/709146/how-do-i-clear-the-stdqueue-efficiently
				decltype(npc->path_destinations)().swap(npc->path_destinations);

				// Player position is our destination
				if (!NPC::world->GetIsObstacle(position)) {
					Vivium::Vector2<int> npc_tile_pos = npc->body->quad->GetCenter() / World::PIXEL_SCALE;
					Vivium::Vector2<int> rel_start = NPC::world->GetObstacleMapIndex(npc_tile_pos);
					Vivium::Vector2<int> rel_dest = NPC::world->GetObstacleMapIndex(position);
				
					Vivium::Pathfinding::Path path = Vivium::Pathfinding::Calculate(rel_start, rel_dest, *NPC::world->GetObstacleMap());

					for (auto& node : path.GetNodes()) {
						npc->path_destinations.push(NPC::world->ObstacleMapToWorld(node.pos));
					}
				}
			}
		}

		Hunting::Client::Client(const Wandering::Client& wandering)
			: wandering(wandering) {}

		void Hunting::Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// Update direction
			if (!npc->path_destinations.empty()) {
				Vivium::Vector2<float> dest = npc->path_destinations.front();
				Vivium::Vector2<float> current = npc->body->quad->GetCenter() / World::PIXEL_SCALE;
				Vivium::Vector2<float> direction = Vivium::Vector2<float>::Normalise(dest - current);

				npc->body->vel = direction * global.wandering.wander_speed;
			}
		}

		bool Hunting::IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO: or close enough
			return npc->path_destinations.empty();
		}

		void Hunting::Client::Write(Vivium::Serialiser& s) const
		{
			wandering.Write(s);
		}

		void Hunting::Client::Read(Vivium::Serialiser& s)
		{
			s.Read(&wandering);
		}
	}

	void Health::Damage(float amount)
	{
		if (invincibilty_time <= 0.0f) {
			value = std::max(0.0f, value - amount * (1.0f - resistance));
		}

		// Half second of invincibility
		invincibilty_time = INVINCIBILITY_TIME;
	}

	void Health::Heal(float amount)
	{
		value = std::min(max, value + amount);
	}

	void Health::Update()
	{
		float elapsed = timer.GetElapsed();

		// If health equal to or below 0 we have died
		if (value <= 0.0f) {
			hasDied = true;
		}

		// Heal by the regeneration rate, taking into account elapsed time
		Heal(regen_rate * elapsed);

		// If we are invincible right now, remove elapsed time from invincibility time remaining
		if (invincibilty_time > 0.0f) {
			invincibilty_time -= elapsed;
		}
	}

	float Health::GetNormalised() const
	{
		return value / max;
	}
}
