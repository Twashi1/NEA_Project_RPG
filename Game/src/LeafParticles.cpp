#include "LeafParticles.h"

namespace Game {
	LeafParticle::LeafParticle(const LeafParticle& other)
		: Particle(other), leaf_type(other.leaf_type)
	{}

	LeafParticle::LeafParticle(LeafParticle&& other) noexcept
		: Particle(std::move(other)), leaf_type(std::move(other.leaf_type))
	{}

	LeafParticle& LeafParticle::operator=(const LeafParticle& other)
	{
		Particle::operator=(other);

		leaf_type = other.leaf_type;

		return *this;
	}

	LeafParticle& LeafParticle::operator=(LeafParticle&& other) noexcept
	{
		Particle::operator=(other);

		leaf_type = std::move(other.leaf_type);

		return *this;
	}

	Vivium::Vector2<int> LeavesParticleSystem::m_AtlasIndices[4] = {
		{0, 0},
		{1, 0},
		{2, 0},
		{3, 0}
	};

	float LeavesParticleSystem::m_TextureCoords[4][8];

	const Vivium::BufferLayout LeavesParticleSystem::m_Layout = {
			Vivium::GLSLDataType::VEC2,	  // Position
			Vivium::GLSLDataType::VEC2,   // Tex coords
			Vivium::GLSLDataType::FLOAT,  // Alpha
			Vivium::GLSLDataType::FLOAT,  // Rotation
			Vivium::GLSLDataType::VEC2,	  // Center
	};

	void LeavesParticleSystem::Init()
	{
		m_LoadTextureCoords();
	}
	
	void LeavesParticleSystem::m_LoadTextureCoords()
	{
		int i = 0;
		for (const auto& index : m_AtlasIndices) {
			std::array<float, 8> coords = TextureManager::particle_atlas->GetCoordsArray(index);
			
			std::memcpy(m_TextureCoords[i], &coords[0], sizeof(float) * 8);

			++i;
		}
	}

	LeavesParticleSystem::LeavesParticleSystem(const std::size_t& max_size, const Vivium::Vector2<float>& acceleration)
		: ParticleSystem(max_size, nullptr, acceleration)
	{
		m_Shader = std::make_shared<Vivium::Shader>("particle_vertex", "particle_frag");
	}

	void LeavesParticleSystem::Emit(std::size_t count, float lifespan, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		for (std::size_t i = 0; i < count; i++) {
			m_EmitParticle(lifespan, pos, vel, var, angle, angular_vel, angular_var);
			++m_Index;

			if (m_Index >= m_MaxSize) { m_Index -= m_MaxSize; }
		}
	}

	// NOTE: ~ParticleSystem will clean up m_Particles for us
	LeavesParticleSystem::~LeavesParticleSystem() {}

	void LeavesParticleSystem::Render() {
		Vivium::Batch batch(m_MaxSize, &m_Layout);

		for (std::size_t i = 0; i < m_MaxSize; i++) {
			Vivium::Particle* particle = m_Particles[i];

			if (particle == nullptr) { continue; }

			if (particle->IsAlive()) {
				m_UpdateParticle(particle);
				m_RenderParticle(&batch, particle);
			}
		}

		m_RenderBatch(&batch);
	}

	void LeavesParticleSystem::m_EmitParticle(float lifespan, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		LeafParticle* my_particle = new LeafParticle();

		my_particle->position = pos;
		my_particle->velocity = vel + Vivium::Vector2<float>(
			Vivium::Random::GetFloat(-var.x, var.x),
			Vivium::Random::GetFloat(-var.y, var.y)
		);
		my_particle->angular_velocity = angular_vel + Vivium::Random::GetFloat(-angular_var, angular_var);
		my_particle->angle = angle;

		// TODO: lifespan variation
		my_particle->lifespan = lifespan * Vivium::Random::GetFloat(0.8f, 1.2f);

		my_particle->leaf_type = Vivium::Random::GetInt(0, 3);

		Vivium::Particle* replacement = m_Particles[m_Index];

		if (replacement != nullptr) {
			delete replacement;
		}

		m_Particles[m_Index] = dynamic_cast<Vivium::Particle*>(my_particle);
	}

	void LeavesParticleSystem::m_RenderParticle(Vivium::Batch* batch, const Vivium::Particle* particle)
	{
		const LeafParticle* leaf = dynamic_cast<const LeafParticle*>(particle);

		if (leaf->IsAlive()) {
			float* coords = m_TextureCoords[leaf->leaf_type];

			float alpha = std::min((1.0f - leaf->time_alive / leaf->lifespan) / s_FadeoutStartPercent, 1.0f);

			float per_vertex_data[4] = { alpha, leaf->angle, leaf->position.x, leaf->position.y };

			batch->Submit(leaf->position, s_ParticleSize, coords[0], coords[2], coords[1], coords[5], per_vertex_data, 4);
		}
	}

	void LeavesParticleSystem::m_RenderBatch(Vivium::Batch* batch)
	{
		auto result = batch->End();

		if (result.count > 0) {
			Vivium::Renderer::Submit(result.vertex_buffer.get(), result.index_buffer.get(), m_Shader.get(), TextureManager::particle_atlas->GetAtlas().get());
		}
	}
}
