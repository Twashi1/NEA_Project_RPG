#pragma once

#include "Core.h"

namespace Game {
	class World;

	// Non Playing Character
	// Base for enemies, animals, etc; some creature that will have pathfinding
	// TODO: set a "home"; a position the NPC will attempt to not wander too far from, and a "home range" to determine the max distance an NPC will wander from home
	class NPC {
	protected:
		static constexpr float SPEED = 500.0f;
		static constexpr float WANDER_RANGE = 1000.0f;
		static const Vivium::Vector2<float> WANDER_RANGE_VARIATION;
		// If the NPC is within this distance from the current pathfinding destination, it will begin movement to the next destination
		static const float PATHFINDING_EPSILON;

		Ref(Vivium::Body) m_Body;

		// List of positions we have to walk to on our path
		std::queue<Vivium::Vector2<int>> m_PathfindingDestinations;

		virtual void m_Wander(World* world);
		virtual void m_UpdateDirection();

	public:
		NPC(Ref(Vivium::Body) body);

		virtual void UpdatePath(World* world);
	};
}
