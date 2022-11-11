#pragma once

#include "Item.h"

// TODO: split up this file

namespace Game {
	class Weapon;
	class Player;
	class World;
	class ProjectileSystem;

	class NPC;

	class HandEquipable : public Item {
	/// <summary>
	/// An item that when selected, will display on screen as the player holding it
	/// Abstract class
	/// </summary>
	protected:
		std::shared_ptr<Vivium::Quad> m_Quad;

		// Update position of quad to point in direction of cursor
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

		/// <summary>
		/// Create the relevant specialisation of HandEquipable based on the item id passed
		/// </summary>
		/// <param name="id"></param>
		/// <returns></returns>
		static std::shared_ptr<HandEquipable> CreateInstance(const Item::ID& id);
	};

	class Weapon : public HandEquipable {
	/// <summary>
	/// Weapon specialisation of HandEquipable, weapons shoot a projectile which does damage on contact
	/// Weapons have a set fire rate, damage, and knockback, and the projectile has a maximum range
	/// </summary>
	protected:
		static std::shared_ptr<Vivium::Shader> m_ShaderDefault;
		// Particle system "hack" for projectiles
		static ProjectileSystem* m_ProjectileSystem;

		std::shared_ptr<Vivium::Shader> m_Shader;
		Vivium::Timer m_AttackTimer;

		using Item::id;

	public:
		/// <summary>
		/// Create hit handler, projectile system, and default shader
		/// </summary>
		static void Init();
		/// <summary>
		/// Delete projectile system
		/// </summary>
		static void Terminate();

		struct Properties;

		struct Projectile : Vivium::Particle {
		/// <summary>
		/// Projectile specialisation of particle for particle system
		/// Models a moving projectile, storing damage, knockback, and the id of the projectile launched
		/// </summary>
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
			// TODO: unused?
			static World* m_World;

			float m_Damage;
			float m_Knockback;
			ID m_ID;

			virtual void m_Update(const Vivium::Vector2<float>& accel) override;

		public:
			class Hit : public virtual Vivium::Event {
			/// <summary>
			/// A hit event created when a projectile collides (or is within a given range) of an npc
			/// </summary>
			private:
				static constexpr const char* s_TYPE = "WeaponProjectileHit";

			public:
				using Vivium::Event::type;

				// NOTE: damage and knockback should be stored in the projectile?
				float damage;			// Damage to be dealt
				float knockback;		// Measured as an impulse
				NPC* npc;				// Pointer to npc hit
				Projectile* projectile; // Pointer to projectile causing event; NOTE: no strong guarantee on lifetime

				Hit(float damage, float knockback, NPC* npc, Projectile* projectile);
			};

			class HitHandler : public virtual Vivium::EventHandler {
			private:
				// The ID of the event to be handled by this handler
				static constexpr const char* s_TYPE = "WeaponProjectileHit";

			protected:
				/// <summary>
				/// Called by the Vivium event system on each event which maches this handler's type
				/// </summary>
				/// <param name="event">The event to be parsed</param>
				virtual void m_HandleEvent(std::shared_ptr<Vivium::Event> event) override;

			public:
				HitHandler();
			};

			Projectile(const ID& id, float damage, float knockback);
			Projectile(const Properties& props);
			
			float GetDamage() const;
			float GetKnockback() const;

			static void SetWorld(World* world);

			friend ProjectileSystem;
			friend Projectile;
			friend World;
		};

		// TODO: maybe projectile system stored here? need an init function then but still
		struct Properties {
		/// <summary>
		/// Properties for each weapon
		/// </summary>
		public:
			std::string name;				// Pretty name
			float damage;
			float knockback;
			float attack_frequency;			// Per second, how many times weapon fires
			Projectile::ID projectile_id;	// The ID of the projectile it emits

			Properties(const std::string& name, float damage, float knockback, float attack_frequency, const Projectile::ID& projectile_id);
		};

		Weapon(const Item::ID& id);
		~Weapon() = default;

		/// <summary>
		/// Iterate particle system, dynamic casting each particle into a projectile, and returning
		/// an array of pointers (also excluding dead projectiles)
		/// </summary>
		/// <param name="size">Will be set to the size of the array</param>
		/// <returns>Array of pointers to Projectiles, also sets the reference given to the size of the array returned</returns>
		static Projectile** GetProjectiles(std::size_t& size);
		/// <summary>
		/// Forcibly/Prematurely update event handler, pretty bad to do this, but easiest solution to guarantee lifetime on
		/// projectile in hit event
		/// </summary>
		static void ForceUpdateEventHandler();

		/// <summary>
		/// Render weapon and all projectiles
		/// </summary>
		virtual void Render() override;
		/// <summary>
		/// Update the hand equipable
		/// Check if attack can be launched, and then launch the projectile from the tip of the weapon
		/// </summary>
		/// <param name="world"></param>
		/// <param name="player"></param>
		virtual void Update(World* world, Player* player) override;

		friend ProjectileSystem;

	protected:
		static const std::unordered_map<Item::ID, Properties> m_Properties; // Maps item id to weapon properties
		static std::shared_ptr<Projectile::HitHandler> m_HitHandler;
	};

	// TODO: generalise a particle system with textured particles?
	// TODO: or separate rendering process from the simulation for particle system
	class ProjectileSystem : public Vivium::ParticleSystem {
	/// <summary>
	/// Particle system that stores projectiles moving around screen and updating their positions
	/// </summary>
	private:
		// Buffer layout for rendering the projectile
		static const Vivium::BufferLayout m_Layout;

		// Atlas index for each projectile
		static Vivium::Vector2<int> m_AtlasIndices[10];
		// Texture coordinates for each atlas index (stored as bl, br, tr, tl)
		static float m_TextureCoords[10][8];

		static constexpr float s_FadeoutStartPercent = 0.3f;
		static constexpr float s_ParticleSize = 128.0f;

		using ParticleSystem::m_Shader;
		using ParticleSystem::m_Acceleration;

		using ParticleSystem::m_Particles;
		using ParticleSystem::m_MaxSize;
		using ParticleSystem::m_Index;

		/// <summary>
		/// Load texture coordinates for each projectile into m_TextureCoords
		/// </summary>
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
