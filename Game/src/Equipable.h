#pragma once

#include "Item.h"
#include "NPC.h"

// TODO: split up this file

namespace Game {
	class Weapon;
	class Player;
	class World;
	class ProjectileSystem;

	class HandEquipable : public Item {
	protected:
		Ref(Vivium::Quad) m_Quad;

		void m_UpdateQuad(World* world, const Vivium::Vector2<float>& player_pos);

	public:
		using Item::id;

		virtual void Update(World* world, Player* player);
		virtual void Render() = 0;

		HandEquipable(const Item::ID& id);

		virtual ~HandEquipable() = default;

		template <Item::ID id>
		struct GetEquipType { using type = void; };

		template <> struct GetEquipType<Item::ID::AMETHYST_SWORD>	{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::EMERALD_SWORD>	{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::RUBY_SWORD>		{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::SAPPHIRE_SWORD>	{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::TOPAZ_SWORD>		{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::AMETHYST_WAND>	{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::EMERALD_WAND>		{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::RUBY_WAND>		{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::SAPPHIRE_WAND>	{ using type = Weapon; };
		template <> struct GetEquipType<Item::ID::TOPAZ_WAND>		{ using type = Weapon; };

		static Ref(HandEquipable) CreateInstance(const Item::ID& id);
	};

	class Weapon : public HandEquipable {
	protected:
		static Ref(Vivium::Shader) m_ShaderDefault;
		static std::vector<WeakRef(NPC)> m_NPCList;

		Ref(Vivium::Shader) m_Shader;
		ProjectileSystem* m_ProjectileSystem;
		Vivium::Timer m_AttackTimer;

		using Item::id;

		static std::vector<WeakRef(NPC)>* m_GetNPCList();

	public:
		static void Init();
		static void UpdateNPCList(const std::vector<Ref(NPC)>& npcs);

		struct Properties;

		struct Projectile : Vivium::Particle {
		public:
			enum class ID : uint8_t {
				// TODO: fill in the rest of these
				AMETHYST_SWING,
				EMERALD_SWING,
				RUBY_SWING,
				SAPPHIRE_SWING,
				TOPAZ_SWING,
				AMETHYST_BALL,
				EMERALD_BALL,
				RUBY_BALL,
				SAPPHIRE_BALL,
				TOPAZ_BALL
			};

		private:
			// TODO: in future this should be a property of each projectile/weapon
			static constexpr float s_RANGE = 300.0f;

			float m_Damage;
			float m_Knockback;
			ID m_ID;

			virtual void m_Update(const Vivium::Vector2<float>& accel) override;

		public:
			class Hit : public virtual Vivium::Event {
			private:
				static constexpr const char* s_TYPE = "WeaponProjectileHit";

			public:
				using Vivium::Event::type;

				float damage;
				float knockback; // Measured as an impulse
				Ref(NPC) npc;
				Projectile* projectile; // no guarantee on lifetime

				Hit(float damage, float knockback, Ref(NPC) npc, Projectile* projectile);
			};

			class HitHandler : public virtual Vivium::EventHandler {
			private:
				static constexpr const char* s_TYPE = "WeaponProjectileHit";

				int x = 5;

			protected:
				virtual void m_HandleEvent(Ref(Vivium::Event) event) override;

			public:
				HitHandler();
			};

			Projectile(const ID& id, float damage, float knockback);
			Projectile(const Properties& props);

			friend ProjectileSystem;
			friend Projectile;
		};

		// TODO: maybe projectile system stored here? need an init function then but still
		struct Properties {
		public:
			std::string name;
			float damage;
			float knockback;
			float attack_frequency; // Per second, how many times weapon fires
			Projectile::ID projectile_id;

			Properties(const std::string& name, float damage, float knockback, float attack_frequency, const Projectile::ID& projectile_id);
		};
		
		static const std::unordered_map<Item::ID, Properties> m_Properties;
		static Ref(Projectile::HitHandler) m_HitHandler;

		Weapon(const Item::ID& id);
		~Weapon();

		virtual void Render() override;
		virtual void Update(World* world, Player* player) override;
	};

	// TODO: generalise a particle system with textured particles?
	class ProjectileSystem : public Vivium::ParticleSystem {
	private:
		static const Vivium::BufferLayout m_Layout;

		static Vivium::Vector2<int> m_AtlasIndices[10];
		static float m_TextureCoords[10][8]; // second 2d array of the project, i must be a genius or something

		static constexpr float s_FadeoutStartPercent = 0.3f;
		static constexpr float s_ParticleSize = 128.0f;

		using ParticleSystem::m_Shader;
		using ParticleSystem::m_Acceleration;

		using ParticleSystem::m_Particles;
		using ParticleSystem::m_MaxSize;
		using ParticleSystem::m_Index;

		static void m_LoadTextureCoords();

		void m_EmitParticle(float lifespan, const Weapon::ID& id, const Vivium::Vector2<float>& pos, const Vivium::Vector2<float>& vel, const Vivium::Vector2<float>& var, float angle, float angular_vel, float angular_var);
		void m_RenderParticle(Vivium::Batch* batch, const Vivium::Particle* particle) override;

		void m_RenderBatch(Vivium::Batch* batch) override;

	public:
		// TODO: would like to have these be inherited from the particle system, but seems impossible
		void Render() override;
		void Emit(std::size_t count, const Weapon::ID& id, float lifespan = 1.0f, const Vivium::Vector2<float>& pos = 0.0f, const Vivium::Vector2<float>& vel = 0.0f, const Vivium::Vector2<float>& var = 0.0f, float angle = 0.0f, float angular_vel = 0.0f, float angular_var = 0.0f);

		static void Init();

		ProjectileSystem(const std::size_t& max_size);
		~ProjectileSystem() = default;
	};
}
