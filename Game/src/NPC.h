#pragma once

#include "Core.h"

namespace Game {
	class World;

	// Non Playing Character
	// Base for enemies, animals, etc; some creature that will have pathfinding
	// TODO: set a "home"; a position the NPC will attempt to not wander too far from, and a "tether range" to determine the max distance an NPC will wander from home
	class NPC : Vivium::Streamable {
	public:
		enum class EntityType : unsigned int {
			NPC,
			PASSIVE_ENTITY,
			PIG,
			COW
		};

	protected:
		static constexpr float SPEED = 200.0f;
		static constexpr float THINKING_TIME = 5.0f; // 5 seconds
		static constexpr float SEARCH_FREQUENCY = 1.0f; // Search for new path every 1 second
		static constexpr float HOME_RANGE = 800.0f;
		static constexpr float WANDER_RANGE = 800.0f;
		static const Vivium::Vector2<float> WANDER_RANGE_VARIATION;
		// If the NPC is within this distance from the current pathfinding destination, it will begin movement to the next destination
		static const float PATHFINDING_EPSILON;

		Ref(Vivium::Body) m_Body = nullptr;
		Vivium::Timer m_BeenThinkingForTimer;
		float m_BeenThinkingFor = 0.0f;
		Vivium::Vector2<float> m_HomeLocation = FLT_MAX;

		// List of positions we have to walk to on our path
		std::queue<Vivium::Vector2<int>> m_PathfindingDestinations;

		virtual void m_Wander(World* world);
		virtual void m_UpdateDirection();
		virtual void m_UpdatePath(World* world);

		NPC() = default;
		NPC(Ref(Vivium::Body) body);

	public:
		virtual EntityType GetEntityType() const;

		virtual bool IsPassive() const;
		static bool IsSame(const Ref(NPC) a, const Ref(NPC) b);

		// Make polymorphic
		virtual ~NPC() = default;

		virtual Vivium::Vector2<float> GetPos() const;

		static bool IsValidSpawn(const Vivium::Vector2<int>& pos, World* world);
		virtual void Update(World* world);
		virtual void Render() = 0;

		virtual void Write(Vivium::Serialiser& s) const = 0;
		virtual void Read(Vivium::Serialiser& s) = 0;
	};

	template <typename T>
	concept IsNPC = std::is_base_of_v<NPC, T> || std::is_same_v<NPC, T>;
}
