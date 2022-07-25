#pragma once
#include "Body.h"

namespace CoolEngineName {
	struct ENGINE_API Collision {
		// Pointers to the two bodies involved in the collision
		Body* a;
		Body* b;

		Collision(Body* a, Body* b);

		// Commutative
		bool operator==(const Collision& other) const {
			return (*a == *other.a && *b == *other.b) || (*a == *other.b && *b == *other.a);
		}
	};
}
