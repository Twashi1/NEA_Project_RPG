#include "LeafParticles.h"

namespace Game {
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

	float LeafParticleSystem::m_LeafTextureCoords[8][4];
	
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
	}

	LeafParticleSystem::LeafParticleSystem(const Vivium::Vector2<float>& velocity, const Vivium::Vector2<float>& variation, const Vivium::Vector2<float>& pos, std::size_t count)
		: m_Velocity(velocity), m_VelocityVariation(variation), m_Pos(pos), m_Count(count)
	{
		m_Particles = new Particle[m_Count];
	}

	void LeafParticleSystem::Render()
	{
		Vivium::Camera my_camera;

		static const Vivium::BufferLayout layout = {
			Vivium::GLSLDataType::MAT4x4, // Projmat
			Vivium::GLSLDataType::MAT4x4, // Viewmat
			Vivium::GLSLDataType::VEC2,   // Tex coords
			Vivium::GLSLDataType::FLOAT   // Alpha
		};
		static const std::size_t vertex_stride = layout.GetStride();

		Vivium::Batch batch(m_Count, &layout);

		for (std::size_t i = 0; i < m_Count; i++) {
			Particle& particle = m_Particles[i];

			particle.Update();

			if (particle.isAlive()) {
				my_camera.SetCamera(particle.pos, Vivium::Application::GetScreenDim() * 0.5f, 1.0f, particle.angle);
				const glm::mat4& viewmat = my_camera.GetViewMat();
				const glm::mat4& projmat = my_camera.GetProjMat();

				// batch.Submit(projmat, viewmat, &m_LeafIndexCoords, vertex_stride);
			}
		}

		batch.End();
	}

	LeafParticleSystem::~LeafParticleSystem()
	{
		delete[] m_Particles;
	}
}
