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

		std::unordered_map<Ref(Body), std::set<int>> m_UniqueBodies; // Maps a ptr to a body to the layers its in
		LayerMap_t m_Layers; // Objects within one layer cannot interact with objects in other layers

		void m_Register(Ref(Body) body, int layer_index);
		void m_AddToUniqueBodies(Ref(Body) body, const std::set<int>& layer_indices);
		void m_AddToUniqueBodies(Ref(Body) body, int layer_index);

	public:
		// Update all bodies tracked, and resolve collisions
		void Update();

		void Register(Ref(Body) body, const std::set<int>& layer_indices);
		void Register(Ref(Body) body, int layer_index);
		void Unregister(Ref(Body) body, int layer_index);
		void Unregister(Ref(Body) body);

		void ClearLayer(int layer_index);

		Physics();
	};
}
