#include "PassiveEntity.h"
#include "World.h"

namespace Game {
	const Vivium::Vector2<int> PassiveEntity::LEFT_TEXTURE_INDEX	= NULL;
	const Vivium::Vector2<int> PassiveEntity::RIGHT_TEXTURE_INDEX	= NULL;
	const Vivium::Vector2<int> PassiveEntity::TOP_TEXTURE_INDEX		= NULL;
	const Vivium::Vector2<int> PassiveEntity::BOTTOM_TEXTURE_INDEX	= NULL;

	const Vivium::Vector2<int> Pig::LEFT_TEXTURE_INDEX		= {4, 4};
	const Vivium::Vector2<int> Pig::RIGHT_TEXTURE_INDEX		= {5, 4};
	const Vivium::Vector2<int> Pig::TOP_TEXTURE_INDEX		= {4, 4}; // TMP
	const Vivium::Vector2<int> Pig::BOTTOM_TEXTURE_INDEX	= {5, 4}; // TMP

	const Vivium::Vector2<int> Cow::LEFT_TEXTURE_INDEX		= { 0, 0 };
	const Vivium::Vector2<int> Cow::RIGHT_TEXTURE_INDEX		= { 1, 0 };
	const Vivium::Vector2<int> Cow::TOP_TEXTURE_INDEX		= { 0, 0 }; // TMP
	const Vivium::Vector2<int> Cow::BOTTOM_TEXTURE_INDEX	= { 1, 0 }; // TMP

	PassiveEntity::PassiveEntity(Ref(Vivium::Body) body)
		: NPC(body)
	{}

	NPC::EntityType PassiveEntity::GetEntityType() const
	{
		return EntityType::PASSIVE_ENTITY;
	}

	bool PassiveEntity::IsPassive() const
	{
		return true;
	}

	void PassiveEntity::Render() {}

	void PassiveEntity::Update(World* world)
	{
		this->NPC::Update(world);
	}

	NPC::EntityType Pig::GetEntityType() const
	{
		return EntityType::PIG;
	}

	Pig::Pig(const Vivium::Vector2<float>& pos)
		: PassiveEntity()
	{
		m_Quad = MakeRef(Vivium::Quad, pos, World::PIXEL_SCALE);
		m_Body = MakeRef(Vivium::Body, m_Quad, false, 1.0f, 2.0f);

		m_Shader = MakeRef(Vivium::Shader, "texture_vertex", "texture_frag");
	}

	void Pig::Render()
	{
		TextureManager::game_atlas->Set(m_Quad.get(), LEFT_TEXTURE_INDEX);
		Vivium::Renderer::Submit(m_Quad.get(), m_Shader.get(), TextureManager::game_atlas->GetAtlas().get());
	}

	void Pig::Write(Vivium::Serialiser& s) const
	{
		s.Write(m_Body->quad->GetCenter());
		s.Write(m_Body->quad->GetAngle());
	}

	void Pig::Read(Vivium::Serialiser& s)
	{
		Vivium::Vector2<float> position;
		float angle;

		m_Quad = MakeRef(Vivium::Quad, position, World::PIXEL_SCALE);
		m_Body = MakeRef(Vivium::Body, m_Quad, false, 1.0f, 2.0f);

		m_Shader = MakeRef(Vivium::Shader, "texture_vertex", "texture_frag");
	}

	Cow::EntityType Cow::GetEntityType() const
	{
		return EntityType::COW;
	}

	Cow::Cow(const Vivium::Vector2<float>& pos)
		: PassiveEntity()
	{
		m_Quad = MakeRef(Vivium::Quad, pos, World::PIXEL_SCALE);
		m_Body = MakeRef(Vivium::Body, m_Quad, false, 1.0f, 2.0f);

		m_Shader = MakeRef(Vivium::Shader, "texture_vertex", "texture_frag");
	}

	void Cow::Write(Vivium::Serialiser& s) const
	{
		s.Write(m_Body->quad->GetCenter());
		s.Write(m_Body->quad->GetAngle());
	}

	void Cow::Read(Vivium::Serialiser& s)
	{
		Vivium::Vector2<float> position;
		float angle;

		m_Quad = MakeRef(Vivium::Quad, position, World::PIXEL_SCALE);
		m_Body = MakeRef(Vivium::Body, m_Quad, false, 1.0f, 2.0f);

		m_Shader = MakeRef(Vivium::Shader, "texture_vertex", "texture_frag");
	}

	void Cow::Render()
	{
		TextureManager::game_atlas->Set(m_Quad.get(), LEFT_TEXTURE_INDEX);
		Vivium::Renderer::Submit(m_Quad.get(), m_Shader.get(), TextureManager::game_atlas->GetAtlas().get());
	}
}
