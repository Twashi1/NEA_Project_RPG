#include "Equipable.h"
#include "Player.h"
#include "World.h"
#include "NPC.h"

namespace Game {
#define CONSTRUCT_EQUIPABLE_FROM_ID(id) case id: \
	return dynamic_pointer_cast<HandEquipable>(std::make_shared<GetEquipType<id>::type>(id))

	std::shared_ptr<HandEquipable> HandEquipable::CreateInstance(const Item::ID& id) {
		switch (id) {
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::AMETHYST_SWORD);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::EMERALD_SWORD);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::RUBY_SWORD);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::SAPPHIRE_SWORD);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::TOPAZ_SWORD);

			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::AMETHYST_WAND);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::EMERALD_WAND);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::RUBY_WAND);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::SAPPHIRE_WAND);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::TOPAZ_WAND);

			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::AMETHYST_PICKAXE);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::EMERALD_PICKAXE);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::RUBY_PICKAXE);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::SAPPHIRE_PICKAXE);
			CONSTRUCT_EQUIPABLE_FROM_ID(Item::ID::TOPAZ_PICKAXE);
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

		// Render position should be slightly forward in direction of equipable from player center (about 3/4 of a tile)
		Vivium::Vector2<float> render_pos = player_pos + (cursor_to_player.normalise() * World::PIXEL_SCALE * 0.75f);

		// So we only get one update to vb
		// Update center and angle of equipable
		Vivium::Quad::SetVBSuppression(false);
		m_Quad->SetCenter(render_pos);
		Vivium::Quad::SetVBSuppression(true);
		m_Quad->SetAngle(angle);
	}

	HandEquipable::HandEquipable(const Item::ID& id)
		: Item(id, 1)
	{
		m_Quad = std::make_shared<Vivium::Quad>(0.0f, 0.0f, 0.0f, 0.0f);
	}

	void HandEquipable::Update(World* world, Player* player)
	{
		m_UpdateQuad(world, player->quad->GetCenter());
	}

	void ToolEquipable::Init()
	{
		m_ShaderDefault = std::make_unique<Vivium::Shader>("texture_vertex", "texture_frag");
	}

	ToolEquipable::ToolEquipable(const Item::ID& id)
		: HandEquipable(id)
	{}

	void ToolEquipable::Render()
	{
		Vivium::Vector2<int> atlas_index = Item::GetAtlasIndex(id);
		
		TextureManager::game_atlas->Set(m_Quad.get(), atlas_index);

		Vivium::Renderer::Submit(m_Quad.get(), m_ShaderDefault.get(), TextureManager::game_atlas->GetAtlas().get());
	}

	std::shared_ptr<Vivium::Shader> Weapon::m_ShaderDefault = nullptr;
	std::unique_ptr<Vivium::Shader> ToolEquipable::m_ShaderDefault = nullptr;
	std::shared_ptr<Weapon::Projectile::HitHandler> Weapon::m_HitHandler = nullptr;
	World* Weapon::Projectile::m_World = nullptr;
	ProjectileSystem* Weapon::m_ProjectileSystem = nullptr;

	void Weapon::Projectile::SetWorld(World* world)
	{
		m_World = world;
	}

	void Weapon::Init()
	{
		m_ShaderDefault = std::make_shared<Vivium::Shader>("texture_vertex", "texture_frag");
		
		m_HitHandler = std::make_shared<Weapon::Projectile::HitHandler>();

		Vivium::EventSystem::RegisterHandler(
			dynamic_pointer_cast<Vivium::EventHandler>(m_HitHandler)
		);

		m_ProjectileSystem = new ProjectileSystem(100);
	}

	void Weapon::Terminate()
	{
		delete m_ProjectileSystem;
	}

	Weapon::Projectile::Hit::Hit(float damage, float knockback, NPC* npc, Projectile* projectile)
		: Vivium::Event(s_TYPE), damage(damage), knockback(knockback), npc(npc), projectile(projectile) {}

	Weapon::Projectile::HitHandler::HitHandler()
		: Vivium::EventHandler(s_TYPE)
	{}

	void Weapon::Projectile::HitHandler::m_HandleEvent(std::shared_ptr<Vivium::Event> event)
	{
		// TODO: No clear indication that neither projectile ptr nor npc ptr have expired
		std::shared_ptr<Weapon::Projectile::Hit> hit_event = dynamic_pointer_cast<Weapon::Projectile::Hit>(event);
		// TODO: provide impulse based on knockback
		// Damage npc by amount of damage projectile should do
		hit_event->npc->health.Damage(hit_event->projectile->GetDamage());
		// NOTE: If projectile is piercing, this should be omitted
		// Delete projectile
		hit_event->projectile->Kill();

		LogTrace("Hit event was detected, damage: {}, new health: {}%!", hit_event->damage, hit_event->npc->health.GetNormalised() * 100.0f);
	}

	Weapon::Projectile::Projectile(const ID& id, float damage, float knockback)
		: m_ID(id), m_Damage(damage), m_Knockback(knockback) {}

	Weapon::Projectile::Projectile(const Weapon::Properties& properties)
		: m_ID(properties.projectile_id), m_Damage(properties.damage), m_Knockback(properties.knockback) {}

	float Weapon::Projectile::GetDamage() const
	{
		return m_Damage;
	}

	float Weapon::Projectile::GetKnockback() const
	{
		return m_Knockback;
	}

	void Weapon::Projectile::m_Update(const Vivium::Vector2<float>& accel)
	{
		// TODO: remove
	}

	Weapon::Weapon(const Item::ID& id)
		: HandEquipable(id), m_Shader(m_ShaderDefault)
	{}

	Weapon::Projectile** Weapon::GetProjectiles(std::size_t& size)
	{
		// Going to use this as a counter, so start at 0
		size = 0;

		// Get particle array
		Vivium::Particle** particle_array = m_ProjectileSystem->GetParticles();

		// Allocate memory for projectiles
		Weapon::Projectile** projectile_array = new Weapon::Projectile*[m_ProjectileSystem->GetMaxSize()];

		for (std::size_t i = 0; i < m_ProjectileSystem->GetMaxSize(); i++) {
			// Get particle in particle array
			Vivium::Particle* particle = particle_array[i];

			// If particle uninitialised, don't add to our projectiles
			if (particle == nullptr) continue;

			// If particle dead, also don't add to our projectiles
			if (particle->IsAlive()) {
				// Cast to projectile and add to projectile array (SLOW)
				projectile_array[size++] = dynamic_cast<Weapon::Projectile*>(particle);
			}
		}

		return projectile_array;
	}

	void Weapon::ForceUpdateEventHandler()
	{
		m_HitHandler->Update();
	}

	void Weapon::Render() {
		TextureManager::game_atlas->Set(m_Quad.get(), Item::GetAtlasIndex(id));
		Vivium::Renderer::Submit(m_Quad.get(), m_Shader.get(), TextureManager::game_atlas->GetAtlas().get());

		m_ProjectileSystem->Render();
	}

	void Weapon::Update(World* world, Player* player) {
		// TODO: move this
		static constexpr float PARTICLE_SPEED = 800.0f;

		// Update equipable position
		HandEquipable::Update(world, player);

		// If holding LMB
		if (Vivium::Input::IsKeyDown(Vivium::Input::GetMouseState(GLFW_MOUSE_BUTTON_1))) {
			// If enough time elapsed, fire an attack (convert frequency to period)
			if (m_AttackTimer.GetElapsedNoReset() > 1.0f / Weapon::m_Properties.at(id).attack_frequency) {
				// Get cursor position in world
				Vivium::Vector2<float> cursor = Vivium::Renderer::camera->Untransform(Vivium::Input::GetCursorPos());
				// Get player position
				Vivium::Vector2<float> player_pos = player->quad->GetCenter();
				// Calculate direction from player to cursor (and normalise)
				Vivium::Vector2<float> direction = Vivium::Vector2<float>::Normalise(cursor - player_pos);

				// All sprites are at a 45 degree angle, so we remove pi/2 (radians) to correct angle of equipable
				// NOTE: I know pi/2 = 90 degrees, so pi/4 makes more sense, but this is what works so
				float angle = std::atan2(direction.y, direction.x + FLT_EPSILON) - (0.5f * Vivium::Math::PI);

				// TODO: instead of player pos, fire from roughly tip of weapon
				// Emit the projectile
				m_ProjectileSystem->Emit(1, id, 0.5f, player_pos, direction * PARTICLE_SPEED, 0.0f, angle, 0.0f, 0.0f);

				// Reset elapsed time since we have just fired an attack
				m_AttackTimer.Reset();
			}
		}

		m_Quad->SetDim(World::PIXEL_SCALE * 1.0f);
	}

	// Properties for each weapon
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

	// Atlas indices for each weapon's projectile (really should be stored in weapon properties? or projectile properties)
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

	// 8 values for texture coordinates for each sprite
	float ProjectileSystem::m_TextureCoords[10][8];

	// Layout for the projectile
	const Vivium::BufferLayout ProjectileSystem::m_Layout = {
			Vivium::GLSLDataType::VEC2,	  // Position
			Vivium::GLSLDataType::VEC2,   // Tex coords
			Vivium::GLSLDataType::FLOAT,  // Alpha
			Vivium::GLSLDataType::FLOAT,  // Rotation
			Vivium::GLSLDataType::VEC2,	  // Center
	};

	void ProjectileSystem::Init()
	{
		// Precompute texture coordinates
		m_LoadTextureCoords();
	}

	void ProjectileSystem::m_LoadTextureCoords()
	{
		// Iterate all indices in the atlas indices
		int i = 0;
		for (const auto& index : m_AtlasIndices) {
			// Get coordinates
			std::array<float, 8> coords = TextureManager::game_atlas->GetCoordsArray(index);

			// Copy into texture coordinates 2D array
			std::memcpy(m_TextureCoords[i], &coords[0], sizeof(float) * 8);

			++i;
		}
	}

	ProjectileSystem::ProjectileSystem(const std::size_t& max_size)
		: ParticleSystem(max_size, nullptr, 0.0f)
	{
		m_Shader = std::make_shared<Vivium::Shader>("particle_vertex", "particle_frag");
	}

	void ProjectileSystem::Emit(std::size_t count, const Weapon::ID& id, float lifespan, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		// Iterate amount of particles to generate
		for (std::size_t i = 0; i < count; i++) {
			// Emit the particle
			m_EmitParticle(lifespan, id, pos, vel, var, angle, angular_vel, angular_var);
			// Increment index we're adding new particle to
			++m_Index;

			// Roll over index if we exceed max size
			if (m_Index >= m_MaxSize) { m_Index -= m_MaxSize; }
		}
	}

	void ProjectileSystem::Render() {
		// Create batch to hold rendering data
		Vivium::Batch batch(m_MaxSize, &m_Layout);

		// Iterate particles
		for (std::size_t i = 0; i < m_MaxSize; i++) {
			Vivium::Particle* particle = m_Particles[i];

			// If particle uninitialised
			if (particle == nullptr) { continue; }

			// If particle alive
			if (particle->IsAlive()) {
				m_UpdateParticle(particle);
				m_RenderParticle(&batch, particle);
			}
		}

		// Submit batch to renderer
		m_RenderBatch(&batch);
	}

	void ProjectileSystem::m_EmitParticle(float lifespan, const Weapon::ID& id, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var)
	{
		// Construct projectile for weapon
		Weapon::Projectile* my_particle = new Weapon::Projectile(Weapon::m_Properties.at((Item::ID)id));

		my_particle->position = pos;
		// Velocity with some random variation
		my_particle->velocity = vel + Vivium::Vector2<float>(
			Vivium::Random::GetFloat(-var.x, var.x),
			Vivium::Random::GetFloat(-var.y, var.y)
			);
		// Angular velocity with random variation
		my_particle->angular_velocity = angular_vel + Vivium::Random::GetFloat(-angular_var, angular_var);
		my_particle->angle = angle;

		// TODO: better lifespan variation
		my_particle->lifespan = lifespan * Vivium::Random::GetFloat(0.8f, 1.2f);

		// get particle that will be replaced by this one
		Vivium::Particle* replacement = m_Particles[m_Index];

		// If the replacement is allocated somewhere
		if (replacement != nullptr) {
			// Clear memory to prevent leaks
			delete replacement;
		}

		// Assign now empty location to our particle
		m_Particles[m_Index] = dynamic_cast<Vivium::Particle*>(my_particle);
	}

	void ProjectileSystem::m_RenderParticle(Vivium::Batch* batch, const Vivium::Particle* particle)
	{
		// Cast particle to a projectile
		const Weapon::Projectile* projectile = dynamic_cast<const Weapon::Projectile*>(particle);

		if (projectile->IsAlive()) {
			// Get texture coordinates
			float* coords = m_TextureCoords[(uint8_t)projectile->m_ID];

			// Get alpha
			float alpha = std::min((1.0f - projectile->time_alive / projectile->lifespan) / s_FadeoutStartPercent, 1.0f);

			// Create extra vertex data for particle
			float per_vertex_data[4] = { alpha, projectile->angle, projectile->position.x, projectile->position.y };

			// Submit to batch
			batch->Submit(projectile->position, s_ParticleSize, coords[0], coords[2], coords[1], coords[5], per_vertex_data, 4);
		}
	}

	void ProjectileSystem::m_RenderBatch(Vivium::Batch* batch)
	{
		Vivium::Batch::RenderData result = batch->End();

		if (result) {
			Vivium::Renderer::Submit(result.vertex_buffer.get(), result.index_buffer.get(), m_Shader.get(), TextureManager::game_atlas->GetAtlas().get());
		}
	}
}

