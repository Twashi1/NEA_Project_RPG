#include "LeafParticles.h"

namespace Game {
	Vivium::Shader* LeafParticleSystem::m_ParticleShader = nullptr;

	bool LeafParticleSystem::Particle::isAlive() const { return lifespan > time_alive; }
	
	void LeafParticleSystem::Particle::Update()
	{
		float elapsed = timer.GetElapsed();

		time_alive += elapsed;

		pos += velocity * elapsed;
		angle += angular_velocity * elapsed;
	}

	LeafParticleSystem::Particle::Particle(const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, float angle, float angle_vel, float lifespan, uint8_t leaf_type)
		: pos(pos), velocity(vel), angle(angle), angular_velocity(angle_vel), lifespan(lifespan), leaf_type(leaf_type)
	{
		timer.Start();
	}

	Vivium::Vector2<int> LeafParticleSystem::m_LeafIndices[4] = {
		{0, 0},
		{1, 0},
		{2, 0},
		{3, 0}
	};

	float LeafParticleSystem::m_LeafTextureCoords[4][8];
	
	void LeafParticleSystem::m_LoadTextureCoords()
	{
		int i = 0;
		for (const auto& index : m_LeafIndices) {
			std::array<float, 8> coords = TextureManager::particle_atlas->GetCoordsArray(index);
			
			std::memcpy(m_LeafTextureCoords[i], &coords[0], sizeof(float) * 8);

			++i;
		}
	}

	void LeafParticleSystem::Init()
	{
		m_LoadTextureCoords();
		m_ParticleShader = new Vivium::Shader("particle_vertex", "particle_frag");
	}

	void LeafParticleSystem::Emit(const Vivium::Vector2<float>& velocity, const Vivium::Vector2<float>& variation, const Vivium::Vector2<float>& pos, std::size_t count)
	{
		m_Velocity = velocity;
		m_VelocityVariation = variation.abs();
		m_Pos = pos;

		for (std::size_t i = 0; i < count; i++) {
			m_EmitParticle();
		}
	}

	void LeafParticleSystem::Terminate()
	{
		delete m_ParticleShader;
	}

	LeafParticleSystem::LeafParticleSystem(std::size_t max)
		: m_Velocity(0), m_VelocityVariation(0), m_Pos(0), m_Count(max)
	{
		m_Particles = new Particle[m_Count];
	}

	void LeafParticleSystem::m_EmitParticle()
	{
		Particle my_particle;

		my_particle.pos = m_Pos;
		my_particle.velocity = m_Velocity + Vivium::Vector2<float>(
			Vivium::Random::GetFloat(-m_VelocityVariation.x, m_VelocityVariation.x),
			Vivium::Random::GetFloat(-m_VelocityVariation.y, m_VelocityVariation.y)
		);
		my_particle.angular_velocity = Vivium::Random::GetFloat(-2.0f, 2.0f); // TODO: random value
		my_particle.angle = Vivium::Random::GetFloat(-Vivium::Math::PI, Vivium::Math::PI);

		my_particle.lifespan = 5.0f; // TODO: get lifespan
		my_particle.timer.Start();

		my_particle.leaf_type = Vivium::Random::GetInt(0, 3);

		m_Particles[m_Index] = my_particle;

		++m_Index;

		// NOTE: should never be more than the count
		if (m_Index >= m_Count) { m_Index -= m_Count; }
	}

	void LeafParticleSystem::Render()
	{
		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::VEC2,	  // Position
			Vivium::GLSLDataType::VEC2,   // Tex coords
			Vivium::GLSLDataType::FLOAT,  // Alpha
			Vivium::GLSLDataType::FLOAT,  // Rotation
			Vivium::GLSLDataType::VEC2,	  // Center
		};
		static const std::size_t vertex_stride = layout.GetStride();

		Vivium::Batch batch(m_Count, &layout);

		for (std::size_t i = 0; i < m_Count; i++) {
			Particle& particle = m_Particles[i];

			particle.Update();

			if (particle.isAlive()) {
				float* coords = m_LeafTextureCoords[particle.leaf_type];

				float per_vertex_data[4] = { 1.0f, particle.angle, particle.pos.x, particle.pos.y };

				batch.Submit(particle.pos, s_ParticleSize, coords[0], coords[2], coords[1], coords[5], per_vertex_data, 4);
			}
		}

		auto result = batch.End();

		if (result.count > 0) {
			Vivium::Renderer::Submit(result.vertex_buffer.get(), result.index_buffer.get(), m_ParticleShader, TextureManager::particle_atlas->GetAtlas().get());
		}
	}

	LeafParticleSystem::~LeafParticleSystem()
	{
		delete[] m_Particles;
	}
}
