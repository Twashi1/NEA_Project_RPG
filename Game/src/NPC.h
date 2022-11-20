#pragma once

#include "Equipable.h"

namespace Game {
	class World;

	// TODO: move
	/// <summary>
	/// Controls health of an NPC, regenerates over time, and has brief invincibility period after taking
	/// damage
	/// </summary>
	struct Health {
		static constexpr float INVINCIBILITY_TIME = 0.5f;
		Vivium::Timer timer;

		float max = 100.0f;
		float value = 100.0f;
		float resistance = 0.0f;
		float regen_rate = 0.1f;
		float invincibilty_time = 0.0f;

		bool hasDied = false;

		Health() = default;
		
		void Damage(float amount);
		void Heal(float amount);
		void Update();

		// Get current health as value from 0 to 1
		float GetNormalised() const;
	};

	class NPC;

	/// <summary>
	/// Namespace for NPC behaviours, each NPC exhibits one behaviour at a time, and transfers to the next behaviour
	/// in a set loop after the previous behaviour ends
	/// </summary>
	namespace Behaviours {
		/// <summary>
		/// Base class for a behaviour
		/// </summary>
		class Behaviour : public Vivium::Streamable {
		public:
			enum class ID : uint8_t {
				IDLE,
				WANDER,
				HUNTING,
				SLIME_ATTACK,
				MAX
			};

			/// <summary>
			/// Client behaviour is stored per instance of NPC
			/// </summary>
			struct Client : public Vivium::Streamable {
				virtual void Write(Vivium::Serialiser& s) const override;
				virtual void Read(Vivium::Serialiser& s) override;

				virtual ~Client() = default;
			};

			virtual ~Behaviour() = default;

			virtual void Write(Vivium::Serialiser& s) const override;
			virtual void Read(Vivium::Serialiser& s) override;
			
			virtual Behaviour::ID GetID() const = 0;
			/// <summary>
			/// Called when behaviour is switched to
			/// </summary>
			/// <param name="npc"></param>
			/// <param name="client"></param>
			virtual void Begin(NPC* npc, std::shared_ptr<Client> client) const = 0;
			virtual void ExecuteOn(NPC* npc) const = 0;
			/// <summary>
			/// Called every update while this behaviour is being exhibited
			/// </summary>
			/// <param name="npc"></param>
			/// <param name="client"></param>
			virtual void Update(NPC* npc, std::shared_ptr<Client> client) const = 0;
			/// <summary>
			/// Checks if the behaviour is over
			/// </summary>
			/// <param name="npc"></param>
			/// <param name="client"></param>
			/// <returns></returns>
			virtual bool IsOver(NPC* npc, std::shared_ptr<Client> client) const = 0;
		};

		/// <summary>
		/// Behaviour specialisation: the NPC stays still for a set amount of time
		/// </summary>
		class Idle : virtual public Behaviour {
		public:
			struct Global : public Vivium::StreamablePOD {
				float thinking_time;

				Global(float thinking_time);
			};

			struct Client : virtual public Behaviour::Client, public Vivium::Streamable {
				Vivium::Timer thinking_timer;

				void Write(Vivium::Serialiser& s) const override;
				void Read(Vivium::Serialiser& s) override;

				Client() = default;
			};

			Global global;

			Idle(const Global& global);

			virtual void Write(Vivium::Serialiser& s) const override;
			virtual void Read(Vivium::Serialiser& s) override;

			virtual Behaviour::ID GetID() const override;
			virtual void Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			virtual void ExecuteOn(NPC* npc) const override;
			virtual void Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			virtual bool IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
		};

		/// <summary>
		/// NPC paths to a random location nearby, not too far from their "home" (spawn-point)
		/// </summary>
		class Wandering : virtual public Behaviour, public Vivium::Streamable {
		public:
			struct Global : public Vivium::StreamablePOD {
				float wander_range;
				Vivium::Vector2<float> wander_range_variation;
				float wander_speed;

				float home_range;

				Global(float wander_range, Vivium::Vector2<float> wander_range_variation, float wander_speed, float home_range);
			};

			struct Client : virtual public Behaviour::Client, public Vivium::Streamable {
				Vivium::Vector2<int> home_tile;

				virtual void Write(Vivium::Serialiser& s) const override;
				virtual void Read(Vivium::Serialiser& s) override;

				Client() = default;
				Client(const Vivium::Vector2<int>& home_tile);
			};

			Global global;

			Wandering(const Global& global);

			virtual void Write(Vivium::Serialiser& s) const override;
			virtual void Read(Vivium::Serialiser& s) override;

			virtual Behaviour::ID GetID() const override;
			virtual void Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			virtual void ExecuteOn(NPC* npc) const override;
			virtual void Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			virtual bool IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
		};

		/// <summary>
		/// NPC paths towards the player if close enough, or wanders randomly
		/// </summary>
		class Hunting : virtual public Behaviour, public Vivium::Streamable {
		public:
			struct Global : public Vivium::Streamable {
				float notice_range;
				float leash_range; // TODO: unused
				int recalc_dist = 2;

				Wandering::Global wandering;

				Global() = default;
				Global(float notice_range, float leash_range, const Wandering::Global& wandering);

				virtual void Write(Vivium::Serialiser& s) const override;
				virtual void Read(Vivium::Serialiser& s) override;
			};

			struct Client : virtual public Behaviour::Client, public Vivium::Streamable {
				Wandering::Client wandering;

				Client() = default;
				Client(const Wandering::Client& wandering);

				void Write(Vivium::Serialiser& s) const override;
				void Read(Vivium::Serialiser& s) override;
			};

			Global global;

			Hunting(const Global& global);

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;

			Behaviour::ID GetID() const override;
			void Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			void ExecuteOn(NPC* npc) const override;
			void Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			bool IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
		};

		/// <summary>
		/// Slime attacks a player, by moving on top of them (with an animation)
		/// </summary>
		class SlimeAttack : virtual public Behaviour, public Vivium::Streamable {
		public:
			struct Global : public Vivium::Streamable {
				// TODO: all these constants are unused
				float speed;
				float attack_range;
				float damage;
				float knockback;
				float attack_speed;
				Vivium::Animator::Data anim_data;

				Global() = default;
				Global(float speed, float attack_range, float damage, float knockback, float attack_speed, Vivium::Animator::Data* anim_data);

				void Write(Vivium::Serialiser& s) const override;
				void Read(Vivium::Serialiser& s) override;
			};

			struct Client : virtual public Behaviour::Client, public Vivium::Streamable {
				Client() = default;

				void Write(Vivium::Serialiser& s) const override;
				void Read(Vivium::Serialiser& s) override;
			};

			Global global;

			SlimeAttack(const Global& global);

			void Write(Vivium::Serialiser& s) const override;
			void Read(Vivium::Serialiser& s) override;

			Behaviour::ID GetID() const override;
			void Begin(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			void ExecuteOn(NPC* npc) const override;
			void Update(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
			bool IsOver(NPC* npc, std::shared_ptr<Behaviour::Client> client) const override;
		};
	}

	/// <summary>
	/// Any animal/enemy/character is an NPC (non-playing character)
	/// NPCs have a list of behaviours which they cycle between, health
	/// </summary>
	class NPC : public Vivium::Streamable {
	private:
		static constexpr float PATHFINDING_EPSILON = 1.0f / 10.0f; // 1/10th of a tile
		static constexpr float PATHFINDING_EPSILON_SQR = PATHFINDING_EPSILON * PATHFINDING_EPSILON;

		Behaviours::Behaviour::ID m_CurrentBehaviourID = Behaviours::Behaviour::ID::IDLE;
		std::size_t m_CurrentBehaviourIndex = 0;

		// Whether we should call Behaviour::Begin on the next update call
		bool m_BeginBehaviour = true;

		// TODO: unique?
		static std::shared_ptr<Vivium::Shader> m_Shader;
		static std::unique_ptr<Vivium::Shader> m_HealthbarShader; 

		void m_UpdatePathing();

	public:
		enum class ID : uint8_t {
			COW,
			PIG,
			SLIME,
			MAX
		};

		static World* world;

		static void Init();

		NPC::ID id;
		std::shared_ptr<Vivium::Body> body;
		std::queue<Vivium::Vector2<float>> path_destinations;
		Vivium::Vector2<int> current_texture_index;
		Vivium::Animator animator;
		Health health;

		typedef std::unordered_map<Behaviours::Behaviour::ID, std::shared_ptr<Behaviours::Behaviour::Client>> BehaviourDataMap;

		BehaviourDataMap behaviour_data;

		NPC() = default;
		NPC(const NPC::ID& id, std::shared_ptr<Vivium::Body> body, const BehaviourDataMap& data);
		NPC(const NPC& other) = default;
		NPC(NPC&& other) noexcept;
		~NPC() = default;

		NPC& operator=(const NPC& other) = default;

		void Update();

		void Submit(Vivium::Batch* npc_batch, Vivium::Batch* healthbar_batch) const;

		void CheckProjectileCollision(Weapon::Projectile** projectiles, std::size_t size);

		virtual void Write(Vivium::Serialiser& s) const override;
		virtual void Read(Vivium::Serialiser& s) override;

		struct Properties {
			Vivium::Vector2<int> atlas_index;
			std::vector<std::shared_ptr<Behaviours::Behaviour>> behaviours;

			Properties(const Vivium::Vector2<int>& atlas_index, const std::initializer_list<std::shared_ptr<Behaviours::Behaviour>>& behaviours);
		};

		static std::array<Properties, (uint8_t)ID::MAX> m_Properties;

		static const Properties& GetProperties(const ID& id);
		static const std::vector<std::shared_ptr<Behaviours::Behaviour>>& GetBehaviours(const ID& id);
		static const Vivium::Vector2<int>& GetAtlasIndex(const ID& id);

		friend World;
	};
}
