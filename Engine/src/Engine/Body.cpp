#include "Body.h"

namespace CoolEngineName {
	Body::Body(Quad& quad, bool isImmovable, float restitution, float mass)
		: quad(ENG_MakePtr(Quad, quad)),
		isImmovable(isImmovable),
		vel(), acc(),
		restitution(restitution),
		mass(mass), imass(1.0f / mass),
		angular_acc(0.0f), angular_vel(0.0f)
	{}

	Body::Body(ENG_Ptr(Quad) quad, bool isImmovable, float restitution, float mass)
		: quad(quad),
		isImmovable(isImmovable),
		vel(), acc(),
		restitution(restitution),
		mass(mass), imass(1.0f / mass),
		angular_acc(0.0f), angular_vel(0.0f)
	{}

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
		// Calculate future velocity
		Vector2<float> next_vel = vel + (acc * dt);
		// Calculate future position
		Vector2<float> next_pos = quad->GetCenter() + (next_vel * dt);
		// Calculate future angular velocity
		float next_angular_vel = angular_vel + (angular_acc * dt);
		// Calculate future angle
		float next_angle = quad->GetAngle() + (next_angular_vel * dt);

		// Construct quad
		// TODO: Slow since constructing vb
		Quad next_shape = Quad(next_pos, quad->GetDim(), next_angle);

		return next_shape;
	}
}