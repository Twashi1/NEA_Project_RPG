#pragma once

#include "NPC.h"

namespace Game {
	// Considered two different approaches for passive entities
	// a) PassiveEntity base class, and inherited class for each different type (pig, cow, sheep, etc.)
	// b) PassiveEntity stores an "PassiveEntityID",
	//		which can be used as an index into a "Properties" table, storing information on texture, what item they drop when killed, etc.
	//		this approach doesn't really allow for entity-specific data, nor entity-specific behaviour (like a sheep eating grass, or
	//		cows being milked). Maybe if I had more time I'd use this approach (implemented as an ECS to allow entity-specific stuff?)
	// TODO: implement b here, or have a CreateInstance method and a passive entity id
	// TODO: should be batch rendering npcs

	class PassiveEntity : public NPC, public Vivium::Streamable {
	protected:
		static const Vivium::Vector2<int> LEFT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> RIGHT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> TOP_TEXTURE_INDEX;
		static const Vivium::Vector2<int> BOTTOM_TEXTURE_INDEX;

		PassiveEntity() = default;
		PassiveEntity(Ref(Vivium::Body) body);
		virtual ~PassiveEntity() = default;

	public:
		virtual EntityType GetEntityType() const override;
		virtual bool IsPassive() const override;

		virtual void Render();

		virtual void Update(World* world) override;

		virtual void Write(Vivium::Serialiser& s) const = 0;
		virtual void Read(Vivium::Serialiser& s) = 0;
	};

	class Pig : public PassiveEntity {
	private:
		static const Vivium::Vector2<int> LEFT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> RIGHT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> TOP_TEXTURE_INDEX;
		static const Vivium::Vector2<int> BOTTOM_TEXTURE_INDEX;

		// TODO: static shader
		Ref(Vivium::Shader) m_Shader;
		Ref(Vivium::Quad) m_Quad;

	public:
		virtual EntityType GetEntityType() const override;

		Pig(const Vivium::Vector2<float>& pos);

		virtual void Render() override;

		virtual void Write(Vivium::Serialiser& s) const override;
		virtual void Read(Vivium::Serialiser& s) override;
	};

	class Cow : public PassiveEntity {
	private:
		static const Vivium::Vector2<int> LEFT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> RIGHT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> TOP_TEXTURE_INDEX;
		static const Vivium::Vector2<int> BOTTOM_TEXTURE_INDEX;

		// TODO: static shader
		Ref(Vivium::Shader) m_Shader;
		Ref(Vivium::Quad) m_Quad;

	public:
		virtual EntityType GetEntityType() const override;

		Cow(const Vivium::Vector2<float>& pos);

		virtual void Render() override;

		virtual void Write(Vivium::Serialiser& s) const override;
		virtual void Read(Vivium::Serialiser& s) override;
	};
}
