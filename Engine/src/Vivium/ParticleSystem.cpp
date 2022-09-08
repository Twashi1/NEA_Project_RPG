#include "ParticleSystem.h"

namespace Vivium {
	// TODO
	Particle::Particle(const Particle& other)
		: position(other.position), velocity(other.velocity),
		angle(other.angle), angular_velocity(other.angular_velocity),
		lifespan(other.lifespan), time_alive(other.time_alive), timer(other.timer)
	{}

	Particle::Particle(Particle&& other) noexcept
		: position(std::move(other.position)), velocity(std::move(other.velocity)),
		angle(std::move(other.angle)), angular_velocity(std::move(other.angular_velocity)),
		lifespan(std::move(other.lifespan)), time_alive(std::move(other.time_alive)), timer(std::move(other.timer))
	{}

	Particle& Particle::operator=(const Particle& other)
	{
		position =			other.position;
		velocity =			other.velocity;

		angle =				other.angle;
		angular_velocity =	other.angular_velocity;
		
		lifespan =			other.lifespan;
		time_alive =		other.time_alive;
		timer =				other.timer;

		return *this;
	}

	Particle& Particle::operator=(Particle&& other) noexcept
	{
		position =				std::move(other.position);
		velocity =				std::move(other.velocity);

		angle =					std::move(other.angle);
		angular_velocity =		std::move(other.angular_velocity);

		lifespan =				std::move(other.lifespan);
		time_alive =			std::move(other.time_alive);
		timer =					std::move(other.timer);

		return *this;
	}

	bool Particle::IsAlive()
	{
		return lifespan > time_alive;
	}

	void Particle::Update(const Vector2<float>& accel)
	{
		if (IsAlive()) {
			float elapsed = timer.GetElapsed();

			velocity += accel * elapsed;
			position += velocity * elapsed;
			angle += angular_velocity * elapsed;

			time_alive += elapsed;
		}
	}

	Vivium::Shader* ParticleSystem::m_WorldParticleShader = nullptr;
	Vivium::Shader* ParticleSystem::m_StaticParticleShader = nullptr;

	void ParticleSystem::m_EmitParticle(float lifespan, const Vector2<float>& pos, const Vector2<float>& vel, const Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		Particle particle;

		particle.position = pos;
		particle.velocity = vel + Vector2<float>(Random::GetFloat(-var.x, var.x), Random::GetFloat(-var.y, var.y));

		particle.angle = angle;
		particle.angular_velocity = angular_vel + Random::GetFloat(-angular_var, angular_var);

		particle.lifespan = lifespan;
		// particle.timer.Start();

		m_Particles[m_Index++] = std::move(particle);
	}

	void ParticleSystem::Init()
	{
		m_WorldParticleShader = new Shader("particle_vertex", "particle_frag");
		m_StaticParticleShader = new Shader("particle_vertex", "particle_frag"); // TODO: write shader
	}
	void ParticleSystem::Terminate()
	{
		delete m_WorldParticleShader;
		delete m_StaticParticleShader;
	}

	ParticleSystem::ParticleSystem(const std::size_t& max_size, const Vector2<float>& acceleration)
		: m_MaxSize(max_size), m_Acceleration(acceleration)
	{
		m_Particles = new Particle[max_size];
	}

	ParticleSystem::~ParticleSystem()
	{
		delete[] m_Particles;
	}

	void ParticleSystem::Emit(std::size_t count, float lifespan, const Vector2<float>& pos, const Vector2<float>& vel, const Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		for (std::size_t i = 0; i < count; i++) {
			// TODO: emit particle subroutine is needed
			m_EmitParticle(lifespan, pos, vel, var, angle, angular_vel, angular_var);

			if (m_Index >= m_MaxSize) { m_Index -= m_MaxSize; }
		}
	}

	void ParticleSystem::Render()
	{
		// NOTE: layout will basically always be overwritten
		static const BufferLayout layout = {
			Vivium::GLSLDataType::VEC2,	  // Position
			Vivium::GLSLDataType::FLOAT,  // Alpha
			Vivium::GLSLDataType::FLOAT   // Rotation
		};

		Batch batch(m_Index, &layout);

		for (std::size_t i = 0; i < m_Index; i++) {
			Particle& particle = m_Particles[m_Index];

			if (particle.IsAlive()) {
				// TODO: Render particle subroutine that takes the batch
				// TODO: rendering code from LeafParticles.h
				particle.Update(m_Acceleration);

				// TODO: ADD TO BATCH
			}
		}

		auto result = batch.End();

		if (result.count > 0) {
			// TODO: Render batch subroutine
			// TODO: RENDER COMMAND
		}
	}
}
