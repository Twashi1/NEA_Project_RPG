#pragma once

#include "Rect.h"
#include "Vector2.h"

// TODO: switch from using quads to rects

struct Quad;

struct ENGINE_API Body {
	Rect* rect;			// Pointer to rect which describes location and dimensions of Body
	bool isImmovable;	// If an object is immovable, collisions will not change velocity/acceleration of this body (infinite mass)
	float restitution;	// Represents amount of velocity transferred to this object in collision, but has no link to actual coefficient of restitution in physics (basically just a bad name)
	float mass;			// Mass of an object, also included in calculation of how much velocity is transferred to this object in a collision
	float imass;		// Inverse mass

	Vector2<float> vel; // Current velocity of object
	Vector2<float> acc; // Current acceleration of object
	
	float angular_vel;  // Current angular velocity of object
	float angular_acc;  // Current angular acceleration of object

	Body(Rect* rect, bool isImmovable, float restitution, float mass);
	Body(Quad& quad, bool isImmovable, float restitution, float mass);
	Body(Quad* quad, bool isImmovable, float restitution, float mass);
	Body(const Body& other);

	// Updates position and velocity, takes time since last update as parameter
	void Update(float dt);
	// Returns a quad which represents the location the object will be at, "dt" seconds into the future (provided no outside influences)
	Rect Peek(float dt);

	bool operator==(const Body& other) const {
		return *rect == *other.rect
			&& isImmovable == other.isImmovable
			&& vel == other.vel && acc == other.acc
			&& mass == other.mass
			&& restitution == other.restitution
			&& angular_vel == other.angular_vel && angular_acc == other.angular_acc;
	}
};