#include "Equipable.h"
#include "Player.h"
#include "World.h"

namespace Game {
#define __GAME_EQUIPABLE_CONSTRUCT_ID(id) case id: \
	return dynamic_pointer_cast<HandEquipable>(MakeRef(GetEquipType<id>::type, id))

	Ref(HandEquipable) HandEquipable::CreateInstance(const Item::ID& id) {
		switch (id) {
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::AMETHYST_SWORD);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::EMERALD_SWORD);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::RUBY_SWORD);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::SAPPHIRE_SWORD);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::TOPAZ_SWORD);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::AMETHYST_WAND);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::EMERALD_WAND);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::RUBY_WAND);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::SAPPHIRE_WAND);
			__GAME_EQUIPABLE_CONSTRUCT_ID(Item::ID::TOPAZ_WAND);
		default:
			return nullptr;
		}
	}

	void HandEquipable::m_UpdateQuad(World* world, const Vivium::Vector2<float>& player_pos)
	{
		// Get cursor position (in terms of world position)
		Vivium::Vector2<float> cursor = Vivium::Renderer::camera->Untransform(Vivium::Input::GetCursorPos());
		Vivium::Vector2<float> cursor_to_player = cursor - player_pos;

		// Calculate angle from cursor to our player
		// Subtracting 45 degrees because all our weapons/tools point towards the top right
		float angle = std::atan2(cursor_to_player.y, cursor_to_player.x + FLT_EPSILON) - (0.25f * Vivium::Math::PI);

		Vivium::Vector2<float> render_pos = player_pos + (cursor_to_player.normalise() * World::PIXEL_SCALE * 0.75f);

		// So we only get one update to vb
		m_Quad->SetRect(Vivium::Rect(render_pos, m_Quad->GetDim(), angle));
	}

	HandEquipable::HandEquipable(const Item::ID& id)
		: Item(id, 1)
	{
		m_Quad = MakeRef(Vivium::Quad, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	void HandEquipable::Update(World* world, Player* player)
	{
		m_UpdateQuad(world, player->quad->GetCenter());
	}

	Ref(Vivium::Shader) Weapon::m_ShaderDefault;

	void Weapon::Init()
	{
		m_ShaderDefault = MakeRef(Vivium::Shader, "texture_vertex", "texture_frag");
	}

	Weapon::Projectile::Hit::Hit(float damage, float knockback)
		: Vivium::Event(s_TYPE), damage(damage), knockback(knockback) {}

	Weapon::Projectile::Projectile(const ID& id, float damage, float knockback)
		: m_ID(id), m_Damage(damage), m_Knockback(knockback) {}

	Weapon::Projectile::Projectile(const Weapon::Properties& properties)
		: m_ID(properties.projectile_id), m_Damage(properties.damage), m_Knockback(properties.knockback) {}

	Weapon::Weapon(const Item::ID& id)
		: HandEquipable(id), m_Shader(m_ShaderDefault), m_ProjectileSystem(new ProjectileSystem(10))
	{}

	Weapon::~Weapon()
	{
		delete m_ProjectileSystem;
	}

	void Weapon::Render() {
		TextureManager::game_atlas->Set(m_Quad.get(), Item::GetAtlasIndex(id));
		Vivium::Renderer::Submit(m_Quad.get(), m_Shader.get(), TextureManager::game_atlas->GetAtlas().get());

		m_ProjectileSystem->Render();
	}

	void Weapon::Update(World* world, Player* player) {
		// TODO: move this
		static constexpr float PARTICLE_SPEED = 800.0f;

		HandEquipable::Update(world, player);

		if (Vivium::Input::IsKeyDown(Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1))) {
			if (m_AttackTimer.GetElapsedNoReset() > 1.0f / Weapon::m_Properties.at(id).attack_frequency) {
				Vivium::Vector2<float> cursor = Vivium::Renderer::camera->Untransform(Vivium::Input::GetCursorPos());
				Vivium::Vector2<float> player_pos = player->quad->GetCenter();
				Vivium::Vector2<float> direction = Vivium::Vector2<float>::Normalise(cursor - player_pos);

				// TODO: some recalculation of angle here, afaik, atan2 is kinda costly so we should compute this once
				// Subtracting half pi to get correct angle
				// TODO: this might mess up the balls a little?
				float angle = std::atan2(direction.y, direction.x + FLT_EPSILON) - (0.5f * Vivium::Math::PI);

				// TODO: instead of player pos, tip of weapon
				m_ProjectileSystem->Emit(1, id, 0.5f, player_pos, direction * PARTICLE_SPEED, 0.0f, angle, 0.0f, 0.0f);

				m_AttackTimer.Reset();
			}
		}

		m_Quad->SetDim(World::PIXEL_SCALE * 1.0f);
	}

	const std::unordered_map<Item::ID, Weapon::Properties> Weapon::m_Properties = {
		{Item::ID::AMETHYST_SWORD,	Weapon::Properties("Amethyst Sword",	10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::AMETHYST_SWING)},
		{Item::ID::EMERALD_SWORD,	Weapon::Properties("Emerald Sword",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::EMERALD_SWING)},
		{Item::ID::RUBY_SWORD,		Weapon::Properties("Ruby Sword",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::RUBY_SWING)},
		{Item::ID::SAPPHIRE_SWORD,	Weapon::Properties("Sapphire Sword",	10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::SAPPHIRE_SWING)},
		{Item::ID::TOPAZ_SWORD,		Weapon::Properties("Topaz Sword",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::TOPAZ_SWING)},
		{Item::ID::AMETHYST_WAND,	Weapon::Properties("Amethyst Wand",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::AMETHYST_BALL)},
		{Item::ID::EMERALD_WAND,	Weapon::Properties("Emerald Wand",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::EMERALD_BALL)},
		{Item::ID::RUBY_WAND,		Weapon::Properties("Ruby Wand",			10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::RUBY_BALL)},
		{Item::ID::SAPPHIRE_WAND,	Weapon::Properties("Sapphire Wand",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::SAPPHIRE_BALL)},
		{Item::ID::TOPAZ_WAND,		Weapon::Properties("Topaz Wand",		10.0f, 10.0f, 1.0f, Weapon::Projectile::ID::TOPAZ_BALL)}
	};

	Weapon::Properties::Properties(const std::string& name, float damage, float knockback, float attack_frequency, const Weapon::Projectile::ID& projectile_id)
		: name(name), damage(damage), knockback(knockback), attack_frequency(attack_frequency), projectile_id(projectile_id)
	{}

	Vivium::Vector2<int> ProjectileSystem::m_AtlasIndices[10] = {
		{8,  6},
		{9,  6},
		{10, 6},
		{11, 6},
		{12, 6},
		{8,  5},
		{9,  5},
		{10, 5},
		{11, 5},
		{12, 5}
	};

	float ProjectileSystem::m_TextureCoords[10][8];

	const Vivium::BufferLayout ProjectileSystem::m_Layout = {
			Vivium::GLSLDataType::VEC2,	  // Position
			Vivium::GLSLDataType::VEC2,   // Tex coords
			Vivium::GLSLDataType::FLOAT,  // Alpha
			Vivium::GLSLDataType::FLOAT,  // Rotation
			Vivium::GLSLDataType::VEC2,	  // Center
	};

	void ProjectileSystem::Init()
	{
		m_LoadTextureCoords();
	}

	void ProjectileSystem::m_LoadTextureCoords()
	{
		int i = 0;
		for (const auto& index : m_AtlasIndices) {
			std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray(index);

			std::memcpy(m_TextureCoords[i], &coords[0], sizeof(float) * 8);

			++i;
		}
	}

	ProjectileSystem::ProjectileSystem(const std::size_t& max_size)
		: ParticleSystem(max_size, nullptr, 0.0f)
	{
		m_Shader = MakeRef(Vivium::Shader, "particle_vertex", "particle_frag");
	}

	void ProjectileSystem::Emit(std::size_t count, const Weapon::ID& id, float lifespan, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		for (std::size_t i = 0; i < count; i++) {
			m_EmitParticle(lifespan, id, pos, vel, var, angle, angular_vel, angular_var);
			++m_Index;

			if (m_Index >= m_MaxSize) { m_Index -= m_MaxSize; }
		}
	}

	void ProjectileSystem::Render() {
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

	void ProjectileSystem::m_EmitParticle(float lifespan, const Weapon::ID& id, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		Weapon::Projectile* my_particle = new Weapon::Projectile(Weapon::m_Properties.at((Item::ID)id));

		my_particle->position = pos;
		my_particle->velocity = vel + Vivium::Vector2<float>(
			Vivium::Random::GetFloat(-var.x, var.x),
			Vivium::Random::GetFloat(-var.y, var.y)
			);
		my_particle->angular_velocity = angular_vel + Vivium::Random::GetFloat(-angular_var, angular_var);
		my_particle->angle = angle;

		// TODO: lifespan variation
		my_particle->lifespan = lifespan * Vivium::Random::GetFloat(0.8f, 1.2f);

		Vivium::Particle* replacement = m_Particles[m_Index];

		if (replacement != nullptr) {
			delete replacement;
		}

		m_Particles[m_Index] = dynamic_cast<Vivium::Particle*>(my_particle);
	}

	void ProjectileSystem::m_RenderParticle(Vivium::Batch* batch, const Vivium::Particle* particle)
	{
		const Weapon::Projectile* projectile = dynamic_cast<const Weapon::Projectile*>(particle);

		if (projectile->IsAlive()) {
			float* coords = m_TextureCoords[(uint8_t)projectile->m_ID];

			float alpha = std::min((1.0f - projectile->time_alive / projectile->lifespan) / s_FadeoutStartPercent, 1.0f);

			float per_vertex_data[4] = { alpha, projectile->angle, projectile->position.x, projectile->position.y };

			batch->Submit(projectile->position, s_ParticleSize, coords[0], coords[2], coords[1], coords[5], per_vertex_data, 4);
		}
	}

	void ProjectileSystem::m_RenderBatch(Vivium::Batch* batch)
	{
		auto result = batch->End();

		if (result.count > 0) {
			Vivium::Renderer::Submit(result.vertex_buffer.get(), result.index_buffer.get(), m_Shader.get(), TextureManager::game_atlas->GetAtlas().get());
		}
	}
}

