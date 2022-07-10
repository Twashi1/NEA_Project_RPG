#pragma once

#include <set>
#include <iterator>
#include <cstddef>
#include <bitset>

#include "Utils.h"
#include "Quad.h"
#include "Body.h"
#include "Collision.h"

class ENGINE_API Physics {
private:
	double m_time = 0.0;  // Stores last time class was updated

	// Shorthands for various types
	typedef std::vector<std::shared_ptr<Body>> layer_t;
	typedef std::unordered_map<int, layer_t> layermap_t;
	typedef std::vector<Collision> collisions_t;

	// TODO: style
	layermap_t layers; // Objects within one layer cannot interact with objects in other layers

public:
	// Update all bodies tracked, and resolve collisions
	void Update(float new_time);

	void Register(const std::shared_ptr<Body>& body, int layer_index);
	void Unregister(const std::shared_ptr<Body>& body, int layer_index);
	void Unregister(const std::shared_ptr<Body>& body);

	Physics();
};