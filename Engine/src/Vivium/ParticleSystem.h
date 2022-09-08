#pragma once

#include "Core.h"
#include "Batch.h"
#include "Shader.h"
#include "Renderer.h"

namespace Vivium {
	struct VIVIUM_API Particle {
	public:
		Vector2<float> position = 0.0f;
		Vector2<float> velocity = 0.0f;

		float angular_velocity = 0.0f;
		float angle = 0.0f;

		float lifespan = 0.0f;
		float time_alive = 0.0f;
		Timer timer;

		virtual bool IsAlive() const;
		virtual void Update(const Vector2<float>& accel);

		Particle() = default;
		Particle(const Particle& other);
		Particle(Particle&& other) noexcept;
		virtual ~Particle() = default;

		Particle& operator=(const Particle& other);
		Particle& operator=(Particle&& other) noexcept;
	};

	// TODO: IRenderable, IBatchable ideal as well
	class VIVIUM_API ParticleSystem {
	protected:
		static const BufferLayout m_Layout;
		Ref(Shader) m_Shader;

		Vector2<float> m_Acceleration;

		Particle** m_Particles;
		std::size_t m_MaxSize;
		std::size_t m_Index = 0;

		virtual void m_EmitParticle(float lifespan, const Vector2<float>& pos, const Vector2<float>& vel, const Vector2<float>& var, float angle, float angular_vel, float angular_var);

		virtual void m_RenderParticle(Batch* batch, const Particle* particle) = 0;
		virtual void m_UpdateParticle(Particle* particle);

		virtual void m_RenderBatch(Batch* batch);

	public:
		ParticleSystem(const std::size_t& max_size, Ref(Shader) shader, const Vector2<float>& acceleration = 0.0f);
		virtual ~ParticleSystem();

		// TODO: Emit specification might be useful
		// TODO: Maybe emit just returns the particle instead?
		virtual void Emit(std::size_t count, float lifespan = 1.0f, const Vector2<float>& pos = 0.0f, const Vector2<float>& vel = 0.0f, const Vector2<float>& var = 0.0f, float angle = 0.0f, float angular_vel = 0.0f, float angular_var = 0.0f);
		
		virtual void Render();
	};
}
