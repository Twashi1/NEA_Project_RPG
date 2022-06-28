#include "Physics.h"

void Physics::Update(float new_time) {
	float dt = new_time - last_time;
	last_time = new_time;

	for (auto& [layer_index, layer] : layers) {
		collisions_t collisions; // Store all collisions we've already checked

		int combinations = layer.size() * (layer.size() - 1) * 0.5f; // Amount of pairs that can be made n(n-1)/2
		collisions.reserve(combinations * sizeof(Collision)); // Reserve space in vector for those pairs

		for (int i = 0; i < layer.size(); i++) {
			for (int j = 0; j < layer.size(); j++) {
				if (i == j) continue;

				// Get pointers to bodies
				std::shared_ptr<Body> a = layer[i];
				std::shared_ptr<Body> b = layer[j];

				// Calculate future positions of bodies
				Rect future_quad_a = a->Peek(dt);
				Rect future_quad_b = b->Peek(dt);

				// Create two bodies to represent bodies in future
				Body future_a = *a; future_a.rect = &future_quad_a;
				Body future_b = *b; future_b.rect = &future_quad_b;

				// Create collision object
				Collision collision = Collision(&future_a, &future_b);
				
				// If our collision object doesn't exist in our collision list, we have to resolve it
				if (std::find(collisions.begin(), collisions.end(), collision) == collisions.end()) {
					collisions.push_back(collision);

					// If either quad contains any points of the other quad (intersecting)
					if (future_quad_a.IsIntersecting(future_quad_b)) {
						// TODO: impulse resolution
						a->vel = 0; b->vel = 0;
						a->acc = 0; b->acc = 0;
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
			layer[i]->Update(dt);
		}
	}
}

Physics::Physics() {}
