#include "Physics.h"

void Physics::Update(float new_time) {
	float elapsed = new_time - m_time;
	m_time = new_time;

	for (auto& [layer_index, layer] : layers) {
		collisions_t collisions; // Store all collisions we've already checked

		int combinations = layer.size() * (layer.size() - 1) * 0.5f; // Amount of pairs that can be made n(n-1)/2
		collisions.reserve(combinations * sizeof(Collision)); // Reserve space in vector for those pairs

		for (int i = 0; i < layer.size(); i++) {
			for (int j = 0; j < layer.size(); j++) {
				if (i == j) continue;

				// Get pointers to bodies
				ENG_Ptr(Body) a = layer[i];
				ENG_Ptr(Body) b = layer[j];

				// Calculate future positions of bodies
				Quad future_quad_a = a->Peek(elapsed);
				Quad future_quad_b = b->Peek(elapsed);

				// TODO: might be laggy here since we're making copies of the quad twice?
				// TODO: maybe make a move constructor for the quad to fix it?
				// Create two bodies to represent bodies in future
				Body future_a = *a; future_a.quad = ENG_MakePtr(Quad, future_quad_a);
				Body future_b = *b; future_b.quad = ENG_MakePtr(Quad, future_quad_b);

				// Create collision object
				Collision collision = Collision(&future_a, &future_b);
				
				// If our collision object doesn't exist in our collision list, we have to resolve it
				if (std::find(collisions.begin(), collisions.end(), collision) == collisions.end()) {
					collisions.push_back(collision);

					// If either quad contains any points of the other quad (intersecting)
					if (future_quad_a.IsIntersecting(future_quad_b)) {
						// TODO: impulse resolution
						a->vel = Vector2<float>(); b->vel = Vector2<float>();
						a->acc = Vector2<float>(); b->acc = Vector2<float>();
						a->angular_acc = 0; b->angular_acc = 0;
						a->angular_vel = 0; b->angular_vel = 0;
					}
				}
			}
		}

		collisions.clear(); // Clear collision list
	}

	// Iterate over every body and update it
	for (auto& [layer_index, layer] : layers) {
		for (int i = 0; i < layer.size(); i++) {
			layer[i]->Update(elapsed);
		}
	}
}

void Physics::Register(ENG_Ptr(Body) body, int layer_index)
{
	// If layers already contains index
	if (layers.contains(layer_index)) {
		// Add to existing index
		layers[layer_index].push_back(body);
	}
	else {
		// Add new layer
		layers[layer_index] = layer_t{body};
	}
}

void Physics::Unregister(ENG_Ptr(Body) body, int layer_index)
{
	// Get layer body is in
	layer_t& layer = layers[layer_index];
	// Find ourselves in the layer
	auto it = std::find(layer.begin(), layer.end(), body);
	// We exist in layer, delete ourselves
	if (it != layer.end()) layer.erase(it);
}

void Physics::Unregister(ENG_Ptr(Body) body)
{
	// Iterate over each layer
	for (auto& [layer_index, layer] : layers) {
		// Search for body in layer
		auto it = std::find(layer.begin(), layer.end(), body);
		// If we exist in that layer, delete ourselves
		if (it != layer.end()); layer.erase(it);
	}
}

Physics::Physics() {}
