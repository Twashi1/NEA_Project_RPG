#pragma once

#include "Core.h"

namespace Game {
	namespace Pathfinding {
		// API will be
		// Behaviour is inherited from, declaring some global and per npc settings for the behaviour
		// Global settings are statically stored with the behaviour
		// Behaviour will provide some functions, taking some per npc settings
		// Declare entity id, declare list of behaviours
		// TODO: StreamablePOD and Streamable
		struct NPC;
		class World;

		class Behaviour {
		public:
			struct Client {
				virtual ~Client() = default;
			};

			virtual ~Behaviour() = default;
			
			virtual void ExecuteOn(NPC* npc) const = 0;
			virtual void Update(Client* client) const = 0;
			virtual bool IsOver(Client* client) const = 0;

			enum class ID : uint8_t {
				IDLE,
				WANDER,
				MAX
			};
		};

		class Idle : virtual public Behaviour {
			struct Global {
				float thinking_time;
			};

			struct Client {
				Vivium::Timer thinking_timer;
			};

			Global global;

			virtual void ExecuteOn(NPC* npc) const override
			{
				// TODO: kill velocity
			}
			virtual void Update(Behaviour::Client* client) const override {}
			virtual bool IsOver(Behaviour::Client* client) const override
			{
				Idle::Client* my_client = dynamic_cast<Idle::Client*>(client);

				return my_client->thinking_timer.GetElapsedNoReset() > global.thinking_time;
			}
		};

		class Wandering : virtual public Behaviour {
			struct Global {
				float thinking_time;
				float wander_range;
				Vivium::Vector2<float> wander_range_variation;
				float wander_speed;

				float home_range;

				Global(float thinking_time, float wander_range, Vivium::Vector2<float> wander_range_variation, float wander_speed, float home_range)
					: thinking_time(thinking_time), wander_range(wander_range), wander_range_variation(wander_range_variation), wander_speed(wander_speed), home_range(home_range)
				{}
			};

			struct Client : virtual public Behaviour::Client {
				float thinking_for;
				Vivium::Vector2<int> home_tile;
			};

			Global global;

			virtual void ExecuteOn(NPC* npc) const override
			{
				Client* my_client = dynamic_cast<Client*>(npc->behaviour_data.at(Behaviour::ID::WANDER));

				// Select direction to move in
				Vivium::Vector2<float> move_vector = Vivium::Random::GetVector2f(
					global.wander_range * Vivium::Random::GetFloat(global.wander_range_variation.x, global.wander_range_variation.y)
				);

				// Select a tile in that direction
				Vivium::Vector2<float> pos = npc->body->quad->GetCenter();
				Vivium::Vector2<float> dest = pos + move_vector;
				Vivium::Vector2<int> dest_tile = dest / World::PIXEL_SCALE;
				
				// Attempt to spawn on valid tile
				if (!NPC::world->GetIsObstacle(dest_tile)) {
					// TODO
				}
			}

			virtual void Update(Behaviour::Client* client) const override;
			virtual bool IsOver(Behaviour::Client* client) const override;
		};

		// TODO: some rendering stuff
		struct NPC {
			enum class ID : uint8_t {
				COW,
				PIG,
				MAX
			};

			static World* world;

			NPC::ID id;
			std::shared_ptr<Vivium::Body> body;
			std::unordered_map<Behaviour::ID, std::shared_ptr<Behaviour::Client>> behaviour_data;
			Vivium::Pathfinding::Path path;

			NPC(const NPC& other) = default;
			~NPC();

			struct Properties {
				std::vector<std::shared_ptr<Behaviour>> behaviours;
			};

			static std::array<Properties, (uint8_t)ID::MAX> m_Properties;

			static const Properties& GetProperties(const ID& id) { return m_Properties[(uint8_t)id]; }
			static const std::vector<std::shared_ptr<Behaviour>>& GetBehaviours(const ID& id) { return m_Properties[(uint8_t)id].behaviours; }
		};
	}
}
