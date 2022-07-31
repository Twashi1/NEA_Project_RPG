#include "Sprite.h"

namespace Vivium {
	Ref(Shader) Sprite::m_DefaultTextureShader = nullptr;

	Sprite::Sprite(Ref(Quad) quad, Ref(Shader) shader, Ref(Texture) texture)
		: quad(quad), shader(shader), texture(texture)
	{
	}
	Sprite::Sprite(const Vector2<float>& pos, const Vector2<float>& size, const std::string& sprite_name)
		: shader(m_DefaultTextureShader)
	{
		quad = MakeRef(Quad, pos, size);
		texture = MakeRef(Texture, sprite_name);
	}
}
