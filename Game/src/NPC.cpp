#include "NPC.h"
#include "World.h"

namespace Game {
	const Vivium::Vector2<float> NPC::WANDER_RANGE_VARIATION = Vivium::Vector2<float>(0.5f, 1.0f);
	const float NPC::PATHFINDING_EPSILON = 1.0f / 10.0f; // 1/10th of a tile

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

		bool isValid = !world->GetIsObstacle(dest_tile_index);

		if (isValid) {
			Vivium::Vector2<int> rel_dest = world->GetObstacleMapIndex(dest_tile_index);
			Vivium::Vector2<int> rel_start = world->GetObstacleMapIndex(pos / World::PIXEL_SCALE);

			Vivium::Pathfinding::Path path = Vivium::Pathfinding::Calculate(rel_start, rel_dest, *world->GetObstacleMap());

			for (auto& pathing_pos : path.GetPositions()) {
				m_PathfindingDestinations.push(world->ObstacleMapToWorld(pathing_pos));
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
			Vivium::Vector2<float> dir = Vivium::Vector2<float>::Normalise(dest - (m_Body->quad->GetCenter() / World::PIXEL_SCALE));
			// Set our velocity to travel in that direction
			m_Body->vel = SPEED * dir;
		}
	}

	NPC::NPC(Ref(Vivium::Body) body)
		: m_Body(body)
	{}

	void NPC::Update(World* world)
	{
		m_Body->Update();
		m_UpdatePath(world);
		m_UpdateDirection();
	}

	void NPC::m_UpdatePath(World* world)
	{
		// If no pathfinding destinations, wander
		if (m_PathfindingDestinations.empty()) {
			m_Wander(world);
		}
		else {
			Vivium::Vector2<float> pos = m_Body->quad->GetCenter() / World::PIXEL_SCALE;

			// If distance between pos and our destination is less than the PATHFINDING_EPSILON (if we're close enough to our dest)
			float dist = Vivium::Vector2<float>::Distance(pos, m_PathfindingDestinations.front());
			if (dist < PATHFINDING_EPSILON) {
				m_PathfindingDestinations.pop();
			}
		}
	}
}
