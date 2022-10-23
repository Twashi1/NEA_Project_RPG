#include "NPC.h"
#include "World.h"

namespace Game {
	namespace Pathfinding {
		World* NPC::world = nullptr;
		std::shared_ptr<Vivium::Shader> NPC::m_Shader = nullptr;

#define NEW_BEHAVIOUR_PTR(T, ...) std::make_shared<T>(T::Global(__VA_ARGS__))

		std::array<NPC::Properties, (uint8_t)NPC::ID::MAX> NPC::m_Properties = {
			Properties({0, 4}, {
				NEW_BEHAVIOUR_PTR(Wandering, 800.0f, {0.5f, 1.0f}, 300.0f, 1000.0f),
				NEW_BEHAVIOUR_PTR(Idle, 3.0f)
			}), // COWS ^^^ PIGS VVV
			Properties({4, 4}, {
				NEW_BEHAVIOUR_PTR(Wandering, 800.0f, {0.5f, 1.0f}, 300.0f, 1000.0f),
				NEW_BEHAVIOUR_PTR(Idle, 3.0f)
			}),  // PIGS ^^^ SLIMES VVV
			Properties({0, 5}, {
				// TODO
				NEW_BEHAVIOUR_PTR(Idle, 0.0f)
			})
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
			m_Shader = std::make_shared<Vivium::Shader>("texture_vertex", "texture_frag");
		}

		NPC::NPC(const NPC::ID& id, std::shared_ptr<Vivium::Body> body, const BehaviourDataMap& data)
			: id(id), body(body), behaviour_data(data)
		{
			current_texture_index = NPC::GetAtlasIndex(id);
		}

		NPC::NPC(NPC&& other) noexcept
			: id(std::move(other.id)), body(other.body), path_destinations(std::move(other.path_destinations)), health(std::move(other.health)), behaviour_data(std::move(other.behaviour_data)), current_texture_index(std::move(other.current_texture_index))
		{}

		void NPC::Update()
		{
			// TODO: lots of indexing here, no way to speed this up by storing some ptrs?
			const std::vector<std::shared_ptr<Behaviour>>& behaviours = GetBehaviours(id);

			const std::shared_ptr<Behaviour>& m_CurrentBehaviour = behaviours.at(m_CurrentBehaviourIndex);
			m_CurrentBehaviourID = m_CurrentBehaviour->GetID();
			std::shared_ptr<Behaviour::Client>& current_behaviour_data = behaviour_data.at(m_CurrentBehaviourID);

			if (m_BeginBehaviour) {
				m_CurrentBehaviour->Begin(this, current_behaviour_data);
				m_BeginBehaviour = false;
			}

			// TODO: Update and IsOver may cause two dynamic casts, might be pretty slow
			m_CurrentBehaviour->Update(this, current_behaviour_data);
			m_CurrentBehaviour->ExecuteOn(this);

			m_UpdatePathing();

			body->Update();

			if (m_CurrentBehaviour->IsOver(this, current_behaviour_data)) {
				m_CurrentBehaviourIndex = (m_CurrentBehaviourIndex + 1) % behaviours.size();
				m_BeginBehaviour = true;
			}
		}

		void NPC::AddVertices(std::vector<float>& vertices)
		{
			// TODO: precompute tex coords
			std::array<float, 8> tex_coords = TextureManager::game_atlas->GetCoordsArray(current_texture_index);

			Vivium::Vector2<float> center = body->quad->GetCenter();
			Vivium::Vector2<float> halfdim = body->quad->GetDim() * 0.5f;

			// Calculate bounds
			float left		= center.x - halfdim.x;
			float right		= center.x + halfdim.x;
			float bottom	= center.y - halfdim.y;
			float top		= center.y + halfdim.y;

			// Add to vertices
			vertices.emplace_back(left);
			vertices.emplace_back(bottom);
			vertices.emplace_back(tex_coords[0]);
			vertices.emplace_back(tex_coords[1]);

			vertices.emplace_back(right);
			vertices.emplace_back(bottom);
			vertices.emplace_back(tex_coords[2]);
			vertices.emplace_back(tex_coords[3]);

			vertices.emplace_back(right);
			vertices.emplace_back(top);
			vertices.emplace_back(tex_coords[4]);
			vertices.emplace_back(tex_coords[5]);

			vertices.emplace_back(left);
			vertices.emplace_back(top);
			vertices.emplace_back(tex_coords[6]);
			vertices.emplace_back(tex_coords[7]);
		}

		void NPC::CheckProjectileCollision(Weapon::Projectile** projectiles, std::size_t size)
		{
			// TODO: move these consts
			static constexpr float MAX_DIST = 100.0f;
			static constexpr float MAX_DIST_SQR = MAX_DIST * MAX_DIST;

			Vivium::Vector2<float> my_pos = body->quad->GetCenter();

			for (std::size_t i = 0; i < size; i++) {
				Weapon::Projectile* proj = projectiles[i];

				// Here, there is a guarantee the proj is not out of scope/reallocated, since this is between the projectile system updates
				if (proj->IsAlive()) {
					float dist = Vivium::Vector2<float>::SqrDistance(my_pos, proj->position);
					
					// They have collided (or are close enough to consider that they have)
					if (dist < MAX_DIST_SQR) {
						std::shared_ptr<Weapon::Projectile::Hit> hit = std::make_shared<Weapon::Projectile::Hit>(proj->GetDamage(), proj->GetKnockback(), this, proj);
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

			for (auto& [id, client] : behaviour_data) {
				s.Write((std::underlying_type<Behaviour::ID>::type)id);
				client->Write(s);
			}
		}

		void NPC::Read(Vivium::Serialiser& s)
		{
			s.Read((std::underlying_type<NPC::ID>::type*)&id);

			Vivium::Vector2<float> pos;
			s.Read(&pos);

			std::size_t size;
			s.Read(&size);

			for (std::size_t i = 0; i < size; i++) {
				Behaviour::ID id;
				s.Read((std::underlying_type<Behaviour::ID>::type*)&id);

				std::shared_ptr<Behaviour::Client> new_data_ptr = nullptr;

#define DEFINE_READ_FOR(T) \
std::shared_ptr<T> data_ptr = std::make_shared<T>(); \
data_ptr->Read(s); \
new_data_ptr = dynamic_pointer_cast<Behaviour::Client>(data_ptr);

				switch (id) {
				case Behaviour::ID::IDLE:
				{
					DEFINE_READ_FOR(Idle::Client)
				} break;

				case Behaviour::ID::WANDER:
				{
					DEFINE_READ_FOR(Wandering::Client)
				} break;

				case Behaviour::ID::HUNTING:
				{
					DEFINE_READ_FOR(Hunting::Client)
				} break;

				case Behaviour::ID::SLIME_ATTACK:
				{
					DEFINE_READ_FOR(SlimeAttack::Client)
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
		const std::vector<std::shared_ptr<Behaviour>>& NPC::GetBehaviours(const NPC::ID& id) { return NPC::m_Properties[(uint8_t)id].behaviours; }
		const Vivium::Vector2<int>& NPC::GetAtlasIndex(const NPC::ID& id) { return NPC::m_Properties[(uint8_t)id].atlas_index; }

		NPC::Properties::Properties(const Vivium::Vector2<int>& atlas_index, const std::initializer_list<std::shared_ptr<Behaviour>>& behaviours)
			: atlas_index(atlas_index), behaviours(behaviours) {}

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
			std::shared_ptr<Idle::Client> my_client = dynamic_pointer_cast<Idle::Client>(client);

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
				Vivium::Vector2<float> dest = npc->path_destinations.front();
				Vivium::Vector2<float> current = npc->body->quad->GetCenter() / World::PIXEL_SCALE;
				Vivium::Vector2<float> direction = Vivium::Vector2<float>::Normalise(dest - current);

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

		void SlimeAttack::Global::Write(Vivium::Serialiser& s) const
		{
			// TODO
			s.Write(speed);
			s.Write(anim_data);
		}

		void SlimeAttack::Global::Read(Vivium::Serialiser& s)
		{
			// TODO
			s.Read(&speed);
			s.Read(&anim_data);
		}

		SlimeAttack::SlimeAttack(const Global& global) : global(global) {}

		void SlimeAttack::Client::Write(Vivium::Serialiser& s) const
		{
			s.Write(*animation_handler);
		}

		void SlimeAttack::Client::Read(Vivium::Serialiser& s)
		{
			// TODO
		}

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
			// TODO
		}

		void SlimeAttack::ExecuteOn(NPC* npc) const
		{
			// TODO	
		}

		void SlimeAttack::Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
		}

		bool SlimeAttack::IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
			return true;
		}

		Hunting::Global::Global(const Wandering::Global& wandering, float notice_range, float leash_range)
			: wandering(wandering), notice_range(notice_range), leash_range(leash_range)
		{}

		void Hunting::Global::Write(Vivium::Serialiser& s) const
		{
			// TODO
		}
		
		void Hunting::Global::Read(Vivium::Serialiser& s)
		{
			// TODO
		}

		Hunting::Hunting(const Global& global)
			: global(global) {}

		Behaviour::ID Hunting::GetID() const
		{
			return Behaviour::ID::HUNTING;
		}

		void Hunting::Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
		}

		void Hunting::ExecuteOn(NPC* npc) const
		{
			// TODO
		}

		void Hunting::Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
		}

		bool Hunting::IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
			return true;
		}
	}

	void Health::Damage(float amount)
	{
		value = std::max(0.0f, value - amount * (1.0f - resistance));
	}

	void Health::Heal(float amount)
	{
		value = std::min(max, value + amount);
	}

	void Health::Update()
	{
		Heal(regen_rate * timer.GetElapsed());
	}

	float Health::GetNormalised()
	{
		return value / max;
	}
}
