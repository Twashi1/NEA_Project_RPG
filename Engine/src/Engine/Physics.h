#pragma once

#include <set>
#include <iterator>
#include <cstddef>
#include <bitset>

#include "Utils.h"
#include "Quad.h"
#include "Body.h"
#include "Collision.h"
#include "Shader.h"
#include "Camera.h"

class ENGINE_API Physics {
public:
	// Shorthands for various types
	typedef std::vector<std::shared_ptr<Body>> layer_t;
	typedef std::unordered_map<int, layer_t> layermap_t;
	typedef std::vector<Collision> collisions_t;

	layermap_t layers; // Objects within one layer cannot interact with objects in other layers

	double last_time = 0.0;  // Stores last time class was updated

	// Update all bodies tracked, and resolve collisions
	void Update(float new_time);

	Physics();
};