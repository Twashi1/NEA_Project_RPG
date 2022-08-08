#pragma once

#include "Logger.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Application.h"

// TODO: unused

namespace Vivium {
	class VIVIUM_API Sprite {
	private:
		static Ref(Shader) m_DefaultTextureShader;

		static void m_Init();

	public:
		Ref(Quad) quad;
		Ref(Shader) shader;
		Ref(Texture) texture = nullptr;
	
		Sprite(Ref(Quad) quad, Ref(Shader) shader, Ref(Texture) texture = nullptr);
		Sprite(Ref(Quad) quad, Ref(Texture) texture);
		Sprite(const Vector2<float>& pos, const Vector2<float>& size, const std::string& sprite_name);
	
		friend Application;
	};
}
