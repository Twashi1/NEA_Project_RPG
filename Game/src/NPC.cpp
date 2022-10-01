#include "NPC.h"
#include "World.h"

namespace Game {
	namespace Pathfinding {
		World* NPC::world = nullptr;

		void Idle::ExecuteOn(NPC* npc) const {
			// Kill velocity and acceleration
			npc->body->vel = 0.0f;
			npc->body->acc = 0.0f;
		}

		void Idle::Update(std::shared_ptr<Behaviour::Client> client) const {}

		bool Idle::IsOver(std::shared_ptr<Behaviour::Client> client) const
		{
			std::shared_ptr<Idle::Client> my_client = dynamic_pointer_cast<Idle::Client>(client);

			return my_client->thinking_timer.GetElapsedNoReset() > global.thinking_time;
		}

		Wandering::Global::Global(float thinking_time, float wander_range, Vivium::Vector2<float> wander_range_variation, float wander_speed, float home_range)
			: thinking_time(thinking_time), wander_range(wander_range), wander_range_variation(wander_range_variation), wander_speed(wander_speed), home_range(home_range)
		{}

		void Wandering::ExecuteOn(NPC* npc) const
		{
			std::shared_ptr<Wandering::Client> my_client = dynamic_pointer_cast<Wandering::Client>(npc->behaviour_data.at(Behaviour::ID::WANDER));

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

		void Wandering::Update(std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
		}

		bool Wandering::IsOver(std::shared_ptr<Behaviour::Client> client) const
		{
			// TODO
		}
	}
}
