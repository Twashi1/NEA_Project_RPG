#include "Physics.h"
#include "Application.h"

namespace Vivium {
	Timer Physics::m_Timer;
	Physics::LayerMap_t Physics::m_Layers;

	void Physics::m_ResolveInterLayer(Layer* layer0, Layer* layer1)
	{
		std::size_t layer0_size = layer0->bodies.size();
		std::size_t layer1_size = layer1->bodies.size();

		for (std::size_t i = 0; i < layer0_size; i++) {
			for (std::size_t j = 0; j < layer1_size; j++) {
				m_TestCollision(layer0->bodies[i].lock(), layer1->bodies[j].lock());
			}
		}
	}

	void Physics::m_ResolveIntraLayer(Layer* layer)
	{
		std::size_t layer_size = layer->bodies.size();

		for (std::size_t i = 0; i < layer_size; i++) {
			for (std::size_t j = i + 1; j < layer_size; j++) {
				m_TestCollision(layer->bodies[i].lock(), layer->bodies[j].lock());
			}
		}
	}

	void Physics::m_TestCollision(const Ref(Body)& a, const Ref(Body)& b)
	{
		float elapsed = m_Timer.GetElapsedNoReset();

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

		// If the sum of our radius is more than the distance between the centers of the circles, they are overlapping
		// NOTE: squared here since we're using squares distance for speed
		if (radius_sum * radius_sum >= (dx * dx + dy * dy)) {
			// Calculate future positions of bodies
			Rect future_rect_a = a->PeekRect(elapsed); // TODO: derive from previous peeked information + extra stuff
			Rect future_rect_b = b->PeekRect(elapsed);

			// If either quad contains any points of the other quad (intersecting)
			if (future_rect_a.IsIntersecting(future_rect_b)) {
				// TODO: impulse resolution
				a->vel = Vector2<float>(); b->vel = Vector2<float>();
				a->acc = Vector2<float>(); b->acc = Vector2<float>();
				a->angular_acc = 0; b->angular_acc = 0;
				a->angular_vel = 0; b->angular_vel = 0;
			}
		}
	}

	void Physics::Update()
	{
		std::set<Vector2<uint32_t>> tested_collisions;

		for (auto& [layer_index, layer] : m_Layers) {
			for (auto& other_layer_index : layer.collision_matrix) {
				// Create vector2 to represent the collision between the layers
				// Commutative (a, b == b, a)
				Vector2<uint32_t> possible_collision = layer_index > other_layer_index ?
					Vector2<uint32_t>(layer_index, other_layer_index) : Vector2<uint32_t>(other_layer_index, layer_index);

				auto it = tested_collisions.insert(possible_collision);

				// Indicates that it was successful, thus the two layers should collide since they haven't been checked against each other
				if (it.second) {
					if (layer_index == other_layer_index) {
						layer.m_RemoveExpiredBodies();
						m_ResolveIntraLayer(&layer);
					}
					else {
						// Fetch other layer
						auto it = m_Layers.find(other_layer_index);

						if (it != m_Layers.end()) {
							layer.m_RemoveExpiredBodies();
							it->second.m_RemoveExpiredBodies();
							m_ResolveInterLayer(&layer, &it->second);
						}
						else {
							LogWarn("Invalid collision matrix for layer {}, no layer by id {}", layer_index, other_layer_index);
						}
					}
				}
			}
		}

		m_Timer.Start();
	}

	Physics::Layer* Physics::CreateLayer(const uint32_t& id, const std::set<uint32_t>& collision_matrix)
	{
		auto it = m_Layers.insert({ id, Layer(collision_matrix) });
		return &(it.first->second);
	}

	void Physics::DeleteLayer(const uint32_t& id)
	{
		auto it = m_Layers.find(id);

		if (it != m_Layers.end()) {
			m_Layers.erase(it);
		}
		else {
			LogWarn("Couldn't delete layer: couldn't find layer by id {}", id);
		}
	}

	void Physics::m_Init()
	{
		m_Timer.Start();
	}

	void Physics::Layer::m_RemoveExpiredBodies()
	{
		for (auto it = bodies.begin(); it != bodies.end();) {
			// If its an expired body
			if (it->expired()) {
				// Remove
				it = bodies.erase(it);
			}
			// If its not an expired body
			else {
				// Go to next
				++it;
			}
		}
	}

	Physics::Layer::Layer(const std::set<uint32_t>& collision_matrix)
		: collision_matrix(collision_matrix)
	{}
}