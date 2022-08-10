#include "Physics.h"
namespace Vivium {
	void Physics::Update() {
		float elapsed = m_Timer.GetElapsed();

		for (auto& [layer_index, layer] : layers) {
			for (int i = 0; i < layer.size(); i++) {
				for (int j = i + 1; j < layer.size(); j++) {
					// Get pointers to bodies
					Ref(Body) a = layer[i];
					Ref(Body) b = layer[j];

					Math::AABB future_aabb_a = a->PeekAABB(elapsed);
					Math::AABB future_aabb_b = b->PeekAABB(elapsed);

					// "haha what an amazing broad and narrow phase physics system so well designed"

					// If either quad contains any points of the other quad (intersecting)
					if (future_aabb_a.IsIntersecting(future_aabb_b)) {
						// Calculate future positions of bodies
						Rect future_rect_a = a->PeekRect(elapsed); // TODO: derive from AABB + some angle information
						Rect future_rect_b = b->PeekRect(elapsed);

						if (future_rect_a.IsIntersecting(future_rect_b)) {
							// TODO: impulse resolution
							a->vel = Vector2<float>(); b->vel = Vector2<float>();
							a->acc = Vector2<float>(); b->acc = Vector2<float>();
							a->angular_acc = 0; b->angular_acc = 0;
							a->angular_vel = 0; b->angular_vel = 0;
						}
					}
				}
			}
		}

		// Iterate over every body and update it
		for (auto& [layer_index, layer] : layers) {
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->Update(elapsed);
			}
		}
	}

	void Physics::Register(Ref(Body) body, int layer_index)
	{
		// TODO: use find
		// If layers already contains index
		if (layers.contains(layer_index)) {
			// Add to existing index
			layers[layer_index].push_back(body);
		}
		else {
			// Add new layer
			layers[layer_index] = layer_t{ body };
		}
	}

	void Physics::Unregister(Ref(Body) body, int layer_index)
	{
		// Get layer body is in
		layer_t& layer = layers[layer_index];
		// Find ourselves in the layer
		auto it = std::find(layer.begin(), layer.end(), body);
		// We exist in layer, delete ourselves
		if (it != layer.end()) layer.erase(it);
	}

	void Physics::Unregister(Ref(Body) body)
	{
		// Iterate over each layer
		for (auto& [layer_index, layer] : layers) {
			// Search for body in layer
			auto it = std::find(layer.begin(), layer.end(), body);
			// If we exist in that layer, delete ourselves
			if (it != layer.end()); layer.erase(it);
		}
	}

	Physics::Physics() {
		m_Timer.Start();
	}
}