#include "NPC.h"
#include "World.h"

namespace Game {
	const Vivium::Vector2<float> NPC::WANDER_RANGE_VARIATION = Vivium::Vector2<float>(0.5f, 1.0f);
	const float NPC::PATHFINDING_EPSILON = World::PIXEL_SCALE / 10.0f; // 1/10th of a tile

	void NPC::m_Wander(World* world)
	{
		// Select direction to move in (magnitude WANDER_RANGE * variation)
		Vivium::Vector2<float> move_vector = Vivium::Random::GetVector2f(
			WANDER_RANGE * Vivium::Random::GetFloat(WANDER_RANGE_VARIATION.x, WANDER_RANGE_VARIATION.y)
		);

		// Select the tile in that direction
		// Get current position
		Vivium::Vector2<float> pos = m_Body->quad->GetCenter();
		// Move by vector
		Vivium::Vector2<float> dest = pos + move_vector;
		// Scale
		Vivium::Vector2<int> dest_tile_index = dest / World::PIXEL_SCALE;

		//bool isValid = world->IsObstacle(dest_tile_index);
		bool isValid = false;

		if (isValid) {
			/*
			Vivium::Vector2<int> rel_dest = world->GetObstacleMapIndex(dest_tile_index);
			Vivium::Vector2<int> rel_start = world->GetObstacleMapIndex(pos / World::PIXEL_SCALE);
			*/

			Vivium::Vector2<int> rel_dest = 0;
			Vivium::Vector2<int> rel_start = 0;

			Vivium::Pathfinding::Path path;
			//Vivium::Pathfinding::Path path = Vivium::Pathfinding::Calculate(rel_start, rel_dest, *world->GetObstacleMap());

			for (auto& pathing_pos : path.GetPositions()) {
				m_PathfindingDestinations.push(pathing_pos * World::PIXEL_SCALE);
			}
		}
	}

	void NPC::m_UpdateDirection()
	{
		if (m_PathfindingDestinations.empty()) {
			m_Body->vel = 0.0f;
		}
		else {
			Vivium::Vector2<float> dest = m_PathfindingDestinations.front();
			// Vector from position to destination
			Vivium::Vector2<float> dir = Vivium::Vector2<float>::Normalise(dest - m_Body->quad->GetCenter());
			// Set our velocity to travel in that direction
			m_Body->vel = SPEED * dir;
		}
	}

	NPC::NPC(Ref(Vivium::Body) body)
		: m_Body(body)
	{}

	void NPC::UpdatePath(World* world)
	{
		// If no pathfinding destinations, wander
		if (m_PathfindingDestinations.empty()) {
			m_Wander(world);
		}

		Vivium::Vector2<float> pos = m_Body->quad->GetCenter();

		// If distance between pos and our destination is less than the PATHFINDING_EPSILON (if we're close enough to our dest)
		if (Vivium::Vector2<float>::Distance(pos, m_PathfindingDestinations.front()) < PATHFINDING_EPSILON * PATHFINDING_EPSILON) {
			m_PathfindingDestinations.pop();
		}

		m_UpdateDirection();
	}
}
