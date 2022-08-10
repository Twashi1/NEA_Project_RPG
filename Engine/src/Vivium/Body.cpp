#include "Body.h"

namespace Vivium {
	Body::Body(Quad& quad, bool isImmovable, float restitution, float mass)
		: quad(MakeRef(Quad, quad)),
		isImmovable(isImmovable),
		vel(), acc(),
		restitution(restitution),
		mass(mass), imass(1.0f / mass),
		angular_acc(0.0f), angular_vel(0.0f)
	{}

	Body::Body(Ref(Quad) quad, bool isImmovable, float restitution, float mass)
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

	Rect Body::PeekRect(float dt)
	{
		// Calculate future velocity
		Vector2<float> next_vel = vel + (acc * dt);
		// Calculate future position
		Vector2<float> next_pos = quad->GetCenter() + (next_vel * dt);
		// Calculate future angular velocity
		float next_angular_vel = angular_vel + (angular_acc * dt);
		// Calculate future angle
		float next_angle = quad->GetAngle() + (next_angular_vel * dt);

		// Construct rect
		Rect next_shape = Rect(next_pos, quad->GetDim(), next_angle);

		return next_shape;
	}

	Math::AABB Body::PeekAABB(float dt)
	{
		// TODO: this doesn't create a true bounding box since it doesn't find the maximums/minimums of the vertices, since doing that would negate any performance benefit
		// TODO: make AABB a member of a body? then changing the body changes the aabb? probably would equally ruin performance

		// Calculate future velocity
		Vector2<float> next_vel = vel + (acc * dt);
		// Calculate future position
		Vector2<float> next_pos = quad->GetCenter() + (next_vel * dt);
		// Calculate future angular velocity
		float next_angular_vel = angular_vel + (angular_acc * dt);
		// Calculate future angle
		float next_angle = quad->GetAngle() + (next_angular_vel * dt);

		Vector2<float> dim = quad->GetDim();

		return Math::AABB(next_pos.x, next_pos.y, dim.x, dim.y);
	}
}