#pragma once

#include "Equipable.h"

namespace Game {
	class World;

	struct Health {
		static constexpr float INVINCIBILITY_TIME = 0.5f;
		Vivium::Timer timer;

		float max = 100.0f;
		float value = 100.0f;
		float resistance = 0.0f;
		float regen_rate = 2.0f;
		float invincibilty_time = 0.0f;

		Health() = default;
		
		void Damage(float amount);
		void Heal(float amount);
		void Update();

		// Get current health as value from 0 to 1
		float GetNormalised();
	};

	class NPC;

	namespace Behaviours {
		class Behaviour : public Vivium::Streamable {
		public:
			enum class ID : uint8_t {
				IDLE,
				WANDER,
				HUNTING,
				SLIME_ATTACK,
				MAX
			};

			struct Client : public Vivium::Streamable {
				virtual void Write(Vivium::Serialiser& s) const override;
				virtual void Read(Vivium::Serialiser& s) override;

				virtual ~Client() = default;
			};

			virtual ~Behaviour() = default;

			virtual void Write(Vivium::Serialiser& s) const override;
			virtual void Read(Vivium::Serialiser& s) override;
			
			virtual Behaviour::ID GetID() const = 0;
			virtual void Begin(NPC* npc, std::shared_ptr<Client> client) const = 0;
			virtual void ExecuteOn(NPC* npc) const = 0;
			virtual void Update(NPC* npc, std::shared_ptr<Client> client) const = 0;
			virtual bool IsOver(NPC* npc, std::shared_ptr<Client> client) const = 0;
		};

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

		class SlimeAttack : virtual public Behaviour, public Vivium::Streamable {
		public:
			struct Global : public Vivium::Streamable {
				float speed;
				float attack_range;
				float damage;
				float knockback;
				float attack_speed;
				Vivium::Animator::Data anim_data;

				Global() = default;
				Global(float speed, float attack_range, float damage, float knockback, float attack_speed, const Vivium::Animator::Data& anim_data);

				void Write(Vivium::Serialiser& s) const override;
				void Read(Vivium::Serialiser& s) override;
			};

			struct Client : virtual public Behaviour::Client, public Vivium::Streamable {
				Vivium::Animator animation_handler;

				Client() = default;
				Client(const Vivium::Animator& animation_handler);

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

	class NPC : public Vivium::Streamable {
	private:
		static constexpr float PATHFINDING_EPSILON = 1.0f / 10.0f; // 1/10th of a tile
		static constexpr float PATHFINDING_EPSILON_SQR = PATHFINDING_EPSILON * PATHFINDING_EPSILON;

		Behaviours::Behaviour::ID m_CurrentBehaviourID = Behaviours::Behaviour::ID::IDLE;
		std::size_t m_CurrentBehaviourIndex = 0;

		// Whether we should call Behaviour::Begin on the next update call
		bool m_BeginBehaviour = false;

		// TODO: unique?
		static std::shared_ptr<Vivium::Shader> m_Shader;

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
		Health health;

		typedef std::unordered_map<Behaviours::Behaviour::ID, std::shared_ptr<Behaviours::Behaviour::Client>> BehaviourDataMap;

		BehaviourDataMap behaviour_data;

		NPC() = default;
		NPC(const NPC::ID& id, std::shared_ptr<Vivium::Body> body, const BehaviourDataMap& data);
		NPC(const NPC& other) = default;
		NPC(NPC&& other) noexcept;
		~NPC() = default;

		void Update();
		// TODO: Pretty ugly function, bad name and should be somewhere else
		// Adds rendering information to vertices, assuming layout: position, texcoords
		void AddVertices(std::vector<float>& vertices);

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
