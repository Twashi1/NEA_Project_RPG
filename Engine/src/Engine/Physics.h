#pragma once

#include <set>
#include <iterator>
#include <cstddef>
#include <bitset>

#include "Utils.h"
#include "Quad.h"
#include "Body.h"
#include "Collision.h"

namespace CoolEngineName {
	class ENGINE_API Physics {
	private:
		Utils::Timer m_Timer;

		// Shorthands for various types
		typedef std::vector<ENG_Ptr(Body)> layer_t;
		typedef std::unordered_map<int, layer_t> layermap_t;
		typedef std::vector<Collision> collisions_t;

		// TODO: style
		layermap_t layers; // Objects within one layer cannot interact with objects in other layers

	public:
		// Update all bodies tracked, and resolve collisions
		void Update();

		void Register(ENG_Ptr(Body) body, int layer_index);
		void Unregister(ENG_Ptr(Body) body, int layer_index);
		void Unregister(ENG_Ptr(Body) body);

		Physics();
	};
}
