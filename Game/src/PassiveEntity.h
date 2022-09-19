#pragma once

#include "NPC.h"

namespace Game {
	class PassiveEntity : public NPC {
	protected:
		static const Vivium::Vector2<int> LEFT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> RIGHT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> TOP_TEXTURE_INDEX;
		static const Vivium::Vector2<int> BOTTOM_TEXTURE_INDEX;

		PassiveEntity() = default;
		PassiveEntity(Ref(Vivium::Body) body);
		virtual ~PassiveEntity() = default;

	public:
		virtual void Render();

		virtual void Update(World* world) override;
	};

	class Pig : public PassiveEntity {
	private:
		static const Vivium::Vector2<int> LEFT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> RIGHT_TEXTURE_INDEX;
		static const Vivium::Vector2<int> TOP_TEXTURE_INDEX;
		static const Vivium::Vector2<int> BOTTOM_TEXTURE_INDEX;

		Ref(Vivium::Shader) m_Shader;
		Ref(Vivium::Quad) m_Quad;

	public:
		Pig(const Vivium::Vector2<float>& pos);

		virtual void Render() override;
	};
}
