#pragma once

#include "Core.h"

namespace Game {
	class World;

	namespace Pathfinding {
		// TODO: Streamable for NPC
		struct NPC;

		class Behaviour {
		public:
			struct Client {
				virtual ~Client() = default;
			};

			virtual ~Behaviour() = default;
			
			virtual void ExecuteOn(NPC* npc) const = 0;
			virtual void Update(std::shared_ptr<Client> client) const = 0;
			virtual bool IsOver(std::shared_ptr<Client> client) const = 0;

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

			virtual void ExecuteOn(NPC* npc) const override;
			virtual void Update(std::shared_ptr<Behaviour::Client> client) const override;
			virtual bool IsOver(std::shared_ptr<Behaviour::Client> client) const override;
		};

		class Wandering : virtual public Behaviour {
			struct Global {
				float thinking_time;
				float wander_range;
				Vivium::Vector2<float> wander_range_variation;
				float wander_speed;

				float home_range;

				Global(float thinking_time, float wander_range, Vivium::Vector2<float> wander_range_variation, float wander_speed, float home_range);
			};

			struct Client : virtual public Behaviour::Client {
				float thinking_for;
				Vivium::Vector2<int> home_tile;
			};

			Global global;

			virtual void ExecuteOn(NPC* npc) const override;

			virtual void Update(std::shared_ptr<Behaviour::Client> client) const override;
			virtual bool IsOver(std::shared_ptr<Behaviour::Client> client) const override;
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
			Vivium::Pathfinding::Path path;
			std::unordered_map<Behaviour::ID, std::shared_ptr<Behaviour::Client>> behaviour_data;

			NPC(const NPC& other) = default;
			~NPC() = default;

			struct Properties {
				std::vector<std::shared_ptr<Behaviour>> behaviours;
			};

			static std::array<Properties, (uint8_t)ID::MAX> m_Properties;

			static const Properties& GetProperties(const ID& id) { return m_Properties[(uint8_t)id]; }
			static const std::vector<std::shared_ptr<Behaviour>>& GetBehaviours(const ID& id) { return m_Properties[(uint8_t)id].behaviours; }
		};
	}
}
