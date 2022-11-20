#pragma once

#include "Core.h"
#include "TextureManager.h"

namespace Game {
	struct LeafParticle : public virtual Vivium::Particle {
		/// <summary>
		/// Specialisation of particle for particle system
		/// Stores a leaf_type which determines what texture to use for the particle
		/// </summary>
		uint8_t leaf_type = 0;

		using Particle::IsAlive;
		using Particle::Update;

		LeafParticle() = default;
		LeafParticle(const LeafParticle& other);
		LeafParticle(LeafParticle&& other) noexcept;

		LeafParticle& operator=(const LeafParticle& other);
		LeafParticle& operator=(LeafParticle&& other) noexcept;
	};

	class LeavesParticleSystem : public virtual Vivium::ParticleSystem {
	/// <summary>
	/// Specialisation of particle system for leaves, generating them with random textures,
	/// and controlling motion
	/// </summary>
	private:
		static const Vivium::BufferLayout m_Layout;

		static Vivium::Vector2<int> m_AtlasIndices[4];
		static float m_TextureCoords[4][8]; // look i used a 2D array do i get full marks now?

		static constexpr float s_FadeoutStartPercent = 0.3f; // TODO: should be in particle system
		static constexpr float s_ParticleSize = 32.0f; // TODO: should be in particle system

		using ParticleSystem::m_Shader;
		using ParticleSystem::m_Acceleration;

		using ParticleSystem::m_Particles;
		using ParticleSystem::m_MaxSize;
		using ParticleSystem::m_Index;

		static void m_LoadTextureCoords();
	
		void m_EmitParticle(float lifespan, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var) override;
		void m_RenderParticle(Vivium::Batch* batch, const Vivium::Particle* particle) override;

		void m_RenderBatch(Vivium::Batch* batch) override;

	public:
		// TODO: would like to have these be inherited from the particle system, but seems impossible
		void Render() override;
		void Emit(std::size_t count, float lifespan = 1.0f, const Vivium::Vector2<float>& pos = 0.0f, const Vivium::Vector2<float>& vel = 0.0f, const Vivium::Vector2<float>& var = 0.0f, float angle = 0.0f, float angular_vel = 0.0f, float angular_var = 0.0f) override;
		
		static void Init();

		LeavesParticleSystem(const std::size_t& max_size, const Vivium::Vector2<float>& acceleration = 0.0f);
		~LeavesParticleSystem();
	};
}
