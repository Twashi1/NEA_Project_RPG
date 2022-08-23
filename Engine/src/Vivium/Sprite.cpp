#include "Sprite.h"

namespace Vivium {
	Ref(Shader) Sprite::m_DefaultStaticTextureShader = nullptr;
	Ref(Shader) Sprite::m_DefaultWorldTextureShader = nullptr;

	void Sprite::m_Init()
	{
		m_DefaultStaticTextureShader = MakeRef(Shader, "static_texture_vertex", "texture_frag");
		m_DefaultWorldTextureShader = MakeRef(Shader, "texture_vertex", "texture_frag");
	}

	Sprite::Sprite(Ref(Quad) quad, Ref(Shader) shader, Ref(Texture) texture)
		: quad(quad), shader(shader), texture(texture)
	{
	}

	Sprite::Sprite(Ref(Quad) quad, Ref(Texture) texture, bool isStatic)
		: quad(quad), texture(texture)
	{
		if (isStatic) {
			shader = m_DefaultStaticTextureShader;
		}
		else {
			shader = m_DefaultWorldTextureShader;
		}
	}

	Sprite::Sprite(const Vector2<float>& pos, const Vector2<float>& size, const std::string& sprite_name, bool isStatic)
	{
		if (isStatic) {
			shader = m_DefaultStaticTextureShader;
		}
		else {
			shader = m_DefaultWorldTextureShader;
		}

		quad = MakeRef(Quad, pos, size);
		texture = MakeRef(Texture, sprite_name);
	}
}
