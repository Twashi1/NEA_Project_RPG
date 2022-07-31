#pragma once

#include "Utils.h"
#include "Quad.h"
#include "Shader.h"
#include "Texture.h"
#include "Input.h"
#include "Renderer.h"

namespace Vivium {
	class VIVIUM_API ToggleSwitch {
	private:
		// TODO: this is disgusting
		static Ref(Shader) m_DefaultShader;

		static Ref(Texture) m_DefaultOnTexture;
		static Ref(Texture) m_DefaultOffTexture;
		static Ref(Texture) m_DefaultPressedOnTexture;
		static Ref(Texture) m_DefaultPressedOffTexture;
	public:
		typedef void (*CallbackFunc_t)(ToggleSwitch*); // Shorthand for a callback function pointer (takes the switch that was pressed as a paramater)

		static void Init(const char* engine_icons_path);

		Ref(Shader) on_shader;
		Ref(Shader) off_shader;
		Ref(Shader) pressed_on_shader;
		Ref(Shader) pressed_off_shader;

		Ref(Texture) on_texture;
		Ref(Texture) off_texture;
		Ref(Texture) pressed_on_texture;
		Ref(Texture) pressed_off_texture;

		Ref(Quad) quad;

		Shader* GetShader();
		Texture* GetTexture();

		const Vector2<float>& GetPos() const;
		void SetPos(const Vector2<float>& pos);

		CallbackFunc_t callback;

		bool isPressed = false;
		bool state = true;

		void Update();

		// TODO constructors
		ToggleSwitch(Ref(Quad) quad, CallbackFunc_t callback, bool default_state = true);

		friend Renderer;
	};
}