#pragma once

// TODO Vivium particle system
#include "Core.h"
#include "TextureManager.h"

namespace Game {
	class LeafParticleSystem {
	private:
		static Vivium::Vector2<int> m_LeafIndices[4];
		static float m_LeafTextureCoords[4][8]; // look i used a 2D array do i get full marks now?

		// TODO: replace with a "height" value? (wouldn't work for leaves in the air)
		static constexpr float s_FadeoutStartPercent = 0.3f; // Start fading out when 30% of lifespan is remaining
		static constexpr float s_ParticleSize = 32.0f;

		static Vivium::Shader* m_ParticleShader;

		Vivium::Vector2<float> m_Velocity;
		Vivium::Vector2<float> m_VelocityVariation;
		Vivium::Vector2<float> m_Pos;

		struct Particle {
			Vivium::Vector2<float> velocity;
			Vivium::Vector2<float> pos;
			float angle;
			float angular_velocity;

			float lifespan = 0.0f;
			float time_alive = 0.0f;
			Vivium::Timer timer;

			uint8_t leaf_type;

			bool isAlive() const;
			void Update();

			Particle() = default;
			Particle(const Particle& other);
			Particle(const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, float angle, float angle_vel, float lifespan, uint8_t leaf_type);
		};

		Particle* m_Particles;
		std::size_t m_Count;
		std::size_t m_Index = 0;

		static void m_LoadTextureCoords();

		void m_EmitParticle();

	public:
		static void Init();
		static void Terminate();

		LeafParticleSystem(std::size_t max);
		~LeafParticleSystem();

		void Emit(const Vivium::Vector2<float>& velocity, const Vivium::Vector2<float>& variation, const Vivium::Vector2<float>& pos, std::size_t count);

		void Render();
	};
}
