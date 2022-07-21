#pragma once

#include "Utils.h"
#include "Renderer.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"

class ENGINE_API ToggleSwitch {
private:
	// TODO: this is disgusting
	static ENG_Ptr(Shader) m_DefaultShader;

	static ENG_Ptr(Texture) m_DefaultOnTexture;
	static ENG_Ptr(Texture) m_DefaultOffTexture;
	static ENG_Ptr(Texture) m_DefaultPressedOnTexture;
	static ENG_Ptr(Texture) m_DefaultPressedOffTexture;
public:
	ENG_Ptr(Shader) on_shader;
	ENG_Ptr(Shader) off_shader;
	ENG_Ptr(Shader) pressed_on_shader;
	ENG_Ptr(Shader) pressed_off_shader;

	ENG_Ptr(Texture) on_texture;
	ENG_Ptr(Texture) off_texture;
	ENG_Ptr(Texture) pressed_on_texture;
	ENG_Ptr(Texture) pressed_off_texture;

	ENG_Ptr(Quad) quad;

	bool isPressed;
	bool state;

	// void Update(const Vector2<float>& cursor_pos, Input::State lmb_state);

	friend Renderer;
};
