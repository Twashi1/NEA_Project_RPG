#pragma once

#include "Core.h"
#include "Batch.h"
#include "Shader.h"

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

		virtual bool IsAlive();
		virtual void Update();

		Particle() = default;
		Particle(const Particle& other);
		Particle(Particle&& other) noexcept;

		Particle& operator=(const Particle& other);
		Particle& operator=(Particle&& other) noexcept;
	};

	// TODO: IRenderable, IBatchable ideal as well
	class VIVIUM_API ParticleSystem {
	protected:
		// TODO: should make their own shaders
		static Vivium::Shader* m_WorldParticleShader;
		static Vivium::Shader* m_StaticParticleShader;

		Vivium::Vector2<float> m_Acceleration;

		Particle* m_Particles;
		std::size_t m_MaxSize;
		std::size_t m_Index = 0;

	public:
		static void Init();
		static void Terminate();

		ParticleSystem(const std::size_t& max_size, const Vector2<float>& acceleration = 0.0f);
		~ParticleSystem();

		// TODO: Emit specification might be useful
		void Emit(std::size_t count, float lifespan = 1.0f, const Vector2<float>& pos = 0.0f, const Vector2<float>& vel = 0.0f, const Vector2<float>& var = 0.0f, float angle = 0.0f, float angular_vel = 0.0f, float angular_var = 0.0f);
		
		void Render();
	};
}
