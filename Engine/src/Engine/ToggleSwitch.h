#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Renderer.h"

namespace Vivium {
	class ENGINE_API ToggleSwitch {
	private:
		// TODO: this is disgusting
		static ENG_Ptr(Shader) m_DefaultShader;

		static ENG_Ptr(Texture) m_DefaultOnTexture;
		static ENG_Ptr(Texture) m_DefaultOffTexture;
		static ENG_Ptr(Texture) m_DefaultPressedOnTexture;
		static ENG_Ptr(Texture) m_DefaultPressedOffTexture;
	public:
		typedef void (*CallbackFunc_t)(ToggleSwitch*); // Shorthand for a callback function pointer (takes the switch that was pressed as a paramater)

		static void Init(const char* engine_icons_path);

		ENG_Ptr(Shader) on_shader;
		ENG_Ptr(Shader) off_shader;
		ENG_Ptr(Shader) pressed_on_shader;
		ENG_Ptr(Shader) pressed_off_shader;

		ENG_Ptr(Texture) on_texture;
		ENG_Ptr(Texture) off_texture;
		ENG_Ptr(Texture) pressed_on_texture;
		ENG_Ptr(Texture) pressed_off_texture;

		ENG_Ptr(Quad) quad;

		Shader* GetShader();
		Texture* GetTexture();

		const Vector2<float>& GetPos() const;
		void SetPos(const Vector2<float>& pos);

		CallbackFunc_t callback;

		bool isPressed = false;
		bool state = true;

		void Update();

		// TODO constructors
		ToggleSwitch(ENG_Ptr(Quad) quad, CallbackFunc_t callback, bool default_state = true);

		friend Renderer;
	};
}