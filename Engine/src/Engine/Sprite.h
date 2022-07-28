#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Application.h"

namespace Vivium {
	class ENGINE_API Sprite {
	private:
		static ENG_Ptr(Shader) m_DefaultTextureShader;

	public:
		ENG_Ptr(Quad) quad;
		ENG_Ptr(Shader) shader;
		ENG_Ptr(Texture) texture = nullptr;
	
		Sprite(ENG_Ptr(Quad) quad, ENG_Ptr(Shader) shader, ENG_Ptr(Texture) texture = nullptr);
		Sprite(const Vector2<float>& pos, const Vector2<float>& size, const std::string& sprite_name);
	
		friend Application;
	};
}
