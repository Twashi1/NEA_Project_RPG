#pragma once

#include "Core.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Application.h"

namespace Vivium {
	class VIVIUM_API Sprite {
	private:
		static Ref(Shader) m_DefaultTextureShader;

	public:
		Ref(Quad) quad;
		Ref(Shader) shader;
		Ref(Texture) texture = nullptr;
	
		Sprite(Ref(Quad) quad, Ref(Shader) shader, Ref(Texture) texture = nullptr);
		Sprite(const Vector2<float>& pos, const Vector2<float>& size, const std::string& sprite_name);
	
		friend Application;
	};
}
