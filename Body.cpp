#include "Body.h"

Body::Body(Quad* quad, bool isImmovable, float restitution, float mass)
	: quad(quad),
	isImmovable(isImmovable),
	vel(Vector2<float>::ZERO), acc(Vector2<float>::ZERO),
	restitution(restitution),
	mass(mass), imass(1.0f / mass),
	angular_acc(0.0f), angular_vel(0.0f) {}

Body::Body(const Body& other)
	: quad(other.quad),
	isImmovable(other.isImmovable),
	vel(other.vel), acc(other.acc),
	restitution(other.restitution),
	mass(other.mass), imass(other.imass),
	angular_acc(other.angular_acc), angular_vel(other.angular_vel)
{}

void Body::Update(float dt)
{
	// Update velocity and position
	vel += acc * dt;
	quad->SetCenter(quad->GetCenter() + (vel * dt));

	// Update angular velocity and angle
	angular_vel += angular_acc * dt;
	quad->SetAngle(quad->GetAngle() + (angular_vel * dt));
}

Quad Body::Peek(float dt)
{
	Vector2<float> next_vel = vel + (acc * dt);	// Get future velocity
	Vector2<float> next_pos = quad->GetCenter() + (next_vel * dt); // Get future position
	float next_angular_vel = angular_vel + (angular_acc * dt); // Get future angular velocity
	float next_angle = quad->GetAngle() + (next_angular_vel * dt); // Get future angle

	Quad next_shape = Quad(next_pos, quad->GetDim(), next_angle);

	return next_shape;
}