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

					// TODO: some sort of cache for a given shape?
					Vector2<float> a_dim = a->quad->GetDim();
					Vector2<float> b_dim = b->quad->GetDim();

					float radius_a = 0.5f * a_dim.magnitude();
					float radius_b = 0.5f * b_dim.magnitude();

					// DEBUG 
					if (Flag::Test(VIVIUM_FLAG_DRAW_PHYSICS_CIRCLES)) {
						Renderer::DrawCircle(future_pos_a, radius_a, RGBColor::RED);
						Renderer::DrawCircle(future_pos_b, radius_b, RGBColor::BLUE);
					}

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
		for (auto& [body, indices] : m_UniqueBodies) {
			body->Update(elapsed);
		}
	}

	void Physics::Register(Ref(Body) body, const std::set<int>& layer_indices)
	{
		m_AddToUniqueBodies(body, layer_indices);

		for (const int& layer : layer_indices) {
			m_Register(body, layer);
		}
	}

	void Physics::m_Register(Ref(Body) body, int layer_index)
	{
		auto it = m_Layers.find(layer_index);

		if (it != m_Layers.end()) {
			it->second.push_back(body);
		}
		else {
			m_Layers.insert({ layer_index, Layer_t{body} });
		}
	}

	void Physics::m_AddToUniqueBodies(Ref(Body) body, const std::set<int>& layer_indices)
	{
		bool isUnique = true;

		for (auto& [unique_body, indices] : m_UniqueBodies) {
			// Check they point to the same object
			if (body.get() == unique_body.get())
			{
				isUnique = false;
				indices.insert(layer_indices.begin(), layer_indices.end());
			}
		}

		if (isUnique) {
			m_UniqueBodies.insert({ body, layer_indices });
		}
	}

	void Physics::m_AddToUniqueBodies(Ref(Body) body, int layer_index)
	{
		bool isUnique = true;

		for (auto& [unique_body, indices] : m_UniqueBodies) {
			// Check they point to the same object
			if (body.get() == unique_body.get())
			{
				isUnique = false;
				indices.insert(layer_index);
			}
		}

		if (isUnique) {
			std::set<int> new_indices;
			new_indices.insert(layer_index);
			m_UniqueBodies.insert({ body, new_indices });
		}
	}

	void Physics::Register(Ref(Body) body, int layer_index)
	{
		m_AddToUniqueBodies(body, layer_index);
		m_Register(body, layer_index);
	}

	void Physics::Unregister(Ref(Body) body, int layer_index)
	{
		// Get layer body is in
		Layer_t& layer = m_Layers.at(layer_index);
		// Find ourselves in the layer
		auto it = std::find(layer.begin(), layer.end(), body);
		// We exist in layer, delete ourselves
		if (it != layer.end()) layer.erase(it);

		std::set<int>& body_layers = m_UniqueBodies.at(body);
		body_layers.erase(layer_index);

		if (body_layers.empty()) {
			m_UniqueBodies.erase(body);
		}
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

		// Remove body from unique bodies
		m_UniqueBodies.erase(body);
	}

	void Physics::ClearLayer(int layer_index)
	{
		auto it = m_Layers.find(layer_index);

		if (it != m_Layers.end()) {
			for (Ref(Body)& body : it->second) {
				// If body only exists in one layer (our current layer)
				std::set<int>& body_layers = m_UniqueBodies.at(body);
				body_layers.erase(layer_index);

				if (body_layers.empty()) {
					m_UniqueBodies.erase(body);
				}
			}

			it->second.clear();
		}
		else {
			LogWarn("Couldn't find layer index {} in m_Layers", layer_index);
		}
	}

	Physics::Physics() {
		m_Timer.Start();
	}
}