#include "Physics.h"
namespace Vivium {
	void Physics::Update() {
		float elapsed = m_Timer.GetElapsed();

		for (auto& [layer_index, layer] : m_Layers) {
			for (int i = 0; i < layer.size(); i++) {
				for (int j = i + 1; j < layer.size(); j++) {
					// Get pointers to bodies
					Ref(Body) a = layer[i];
					Ref(Body) b = layer[j];

					Vector2<float> future_pos_a = a->PeekPos(elapsed);
					Vector2<float> future_pos_b = b->PeekPos(elapsed);

					// TODO: come up with a way to explain this
					// Create a circle to represent the maximum extents of the body

					float radius_a = a->quad->GetDim().MaxComponent() * 0.5f;
					float radius_b = b->quad->GetDim().MaxComponent() * 0.5f;

					// Test if there is a collision between them
					// https://stackoverflow.com/questions/697188/fast-circle-collision-detection
					float radius_sum = radius_a + radius_b;
					float dx = future_pos_a.x - future_pos_b.x;
					float dy = future_pos_a.y - future_pos_b.y;

					// If either quad contains any points of the other quad (intersecting)
					if (radius_sum * radius_sum > (dx * dx + dy * dy)) {
						// Calculate future positions of bodies
						Rect future_rect_a = a->PeekRect(elapsed); // TODO: derive from previous peeked information + extra stuff
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
		for (auto& [layer_index, layer] : m_Layers) {
			for (int i = 0; i < layer.size(); i++) {
				layer[i]->Update(elapsed);
			}
		}
	}

	void Physics::Register(Ref(Body) body, const std::vector<int>& layer_indices)
	{
		for (const int& layer : layer_indices) {
			Register(body, layer);
		}
	}

	void Physics::Register(Ref(Body) body, int layer_index)
	{
		// TODO: use find
		// If layers already contains index
		if (m_Layers.contains(layer_index)) {
			// Add to existing index
			m_Layers[layer_index].push_back(body);
		}
		else {
			// Add new layer
			m_Layers[layer_index] = Layer_t{ body };
		}
	}

	void Physics::Unregister(Ref(Body) body, int layer_index)
	{
		// Get layer body is in
		Layer_t& layer = m_Layers.at(layer_index);
		// Find ourselves in the layer
		auto it = std::find(layer.begin(), layer.end(), body);
		// We exist in layer, delete ourselves
		if (it != layer.end()) layer.erase(it);
	}

	void Physics::Unregister(Ref(Body) body)
	{
		// Iterate over each layer
		for (auto& [layer_index, layer] : m_Layers) {
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