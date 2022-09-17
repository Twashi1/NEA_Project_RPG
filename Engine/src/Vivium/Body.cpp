#include "Body.h"
#include "Physics.h"

namespace Vivium {
	void Body::m_InvokeCollisionCallback()
	{
		if (collision_callback != nullptr) {
			collision_callback(this, user_params);
		}
	}

	Body::Body(Ref(Quad) quad, bool isImmovable, float restitution, float mass, bool isPhysical, CallbackFunc_t callback, void* user_params)
		: quad(quad),
		isImmovable(isImmovable),
		vel(), acc(),
		restitution(restitution),
		mass(mass), imass(1.0f / mass),
		angular_acc(0.0f), angular_vel(0.0f),
		collision_callback(callback), user_params(user_params),
		isPhysical(isPhysical)
	{
		m_Timer.Start();
	}

	Body::Body(const Body& other)
		: quad(other.quad),
		isImmovable(other.isImmovable),
		vel(other.vel), acc(other.acc),
		restitution(other.restitution),
		mass(other.mass), imass(other.imass),
		angular_acc(other.angular_acc), angular_vel(other.angular_vel),
		collision_callback(other.collision_callback), user_params(other.user_params),
		isPhysical(other.isPhysical)
	{
		m_Timer.Start();
	}

	void Body::Update()
	{
		float elapsed = m_Timer.GetElapsed();

		// Update velocity and position
		vel += acc * elapsed;
		quad->SetCenter(quad->GetCenter() + (vel * elapsed));

		// Update angular velocity and angle
		angular_vel += angular_acc * elapsed;
		quad->SetAngle(quad->GetAngle() + (angular_vel * elapsed));
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
		// TODO: Make dimensions the max size the quad could take
		
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

	Vector2<float> Body::PeekPos(float dt)
	{
		// Calculate future velocity
		Vector2<float> next_vel = vel + (acc * dt);
		// Calculate future position and return
		return quad->GetCenter() + (next_vel * dt);
	}
}