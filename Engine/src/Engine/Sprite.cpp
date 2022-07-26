#include "Sprite.h"

namespace CoolEngineName {
	ENG_Ptr(Shader) Sprite::m_DefaultTextureShader = nullptr;

	Sprite::Sprite(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture)
		: quad(quad), shader(shader), texture(texture)
	{
	}
	Sprite::Sprite(const Vector2<float>& pos, const Vector2<float>& size, const std::string& sprite_name)
		: shader(m_DefaultTextureShader)
	{
		quad = ENG_MakePtr(Quad, pos, size);
		texture = ENG_MakePtr(Texture, sprite_name);
	}
}
