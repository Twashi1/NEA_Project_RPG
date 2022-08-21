#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Body.h"
#include "Collision.h"
#include "Core.h"

// TODO: should be static?

namespace Vivium {
	class VIVIUM_API Physics {
	private:
		Timer m_Timer;

		typedef std::vector<Ref(Body)> Layer_t;
		typedef std::unordered_map<int, Layer_t> LayerMap_t;

		LayerMap_t m_Layers; // Objects within one layer cannot interact with objects in other layers

	public:
		// Update all bodies tracked, and resolve collisions
		void Update();

		void Register(Ref(Body) body, const std::vector<int>& layer_indices);
		void Register(Ref(Body) body, int layer_index);
		void Unregister(Ref(Body) body, int layer_index);
		void Unregister(Ref(Body) body);

		void ClearLayer(int layer_index);

		Physics();
	};
}
