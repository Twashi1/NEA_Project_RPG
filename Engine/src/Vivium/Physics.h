#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Body.h"
#include "Collision.h"
#include "Core.h"

namespace Vivium {
	class VIVIUM_API Physics {
	private:
		Timer m_Timer;

		// Shorthands for various types
		typedef std::vector<Ref(Body)> layer_t;
		typedef std::unordered_map<int, layer_t> layermap_t;
		typedef std::vector<Collision> collisions_t;

		// TODO: style
		layermap_t layers; // Objects within one layer cannot interact with objects in other layers

	public:
		// Update all bodies tracked, and resolve collisions
		void Update();

		void Register(Ref(Body) body, int layer_index);
		void Unregister(Ref(Body) body, int layer_index);
		void Unregister(Ref(Body) body);

		Physics();
	};
}
