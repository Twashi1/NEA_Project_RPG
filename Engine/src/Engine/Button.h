#pragma once

#include "Utils.h"
#include "Input.h"
#include "Text.h"
#include "Font.h"
#include "Quad.h"

namespace CoolEngineName {
	class ENGINE_API Button {
	private:
		static ENG_Ptr(Shader) m_DefaultIdleShader; // Standard shader for when button is not pressed
		static ENG_Ptr(Shader) m_DefaultPressedShader; // Standard shader for when button is pressed
		static ENG_Ptr(Font) m_DefaultFont;

		static constexpr float m_DefaultAlpha = 1.0f; // Standard alpha (transparency) for a button
		static constexpr float m_DefaultScale = 0.25f; // Standard scaling factor for text (1/4 size)

		// Constructs the objects
		void m_Construct();
		// Gets width/height of text so that it can be centered on the button
		Vector2<float> m_GetTextDim(const ::std::string& text);

		// Updates button's text position - Need to call this if you edit "quad"
		void m_UpdatePos();

	public:
		typedef void (*CallbackFunc_t)(Button*); // Shorthand for a callback function pointer (takes the button that was pressed as a paramater)

		CallbackFunc_t callback; // Function called when button is pressed
		std::string idle_text; // Text that displays when button is not pressed
		std::string pressed_text; // Text that displays when button is pressed

		// TODO: std shaders can be exposed through these
		ENG_Ptr(Shader) idle_shader; // Shader used when button is not pressed
		ENG_Ptr(Shader) pressed_shader; // Shader used when button is pressed

		ENG_Ptr(Texture) idle_texture = nullptr;
		ENG_Ptr(Texture) pressed_texture = nullptr;

		bool isPressed = false; // If button is being pressed

		Quad quad; // Stores position and dimensions of button
		Text* text; // Text object, stores information required to render text TODO: should be private

		// Initialises standard shaders and Input listeners
		static void Init();

		void SetPos(const Vector2<float>& pos);

		// TODO better constructor?
		Button(const Quad& quad, CallbackFunc_t callback, const ::std::string& idle_text, const ::std::string& pressed_text, ENG_Ptr(Shader) idle_shader, ENG_Ptr(Shader) pressed_shader, ENG_Ptr(Texture) idle_texture, ENG_Ptr(Texture) pressed_texture);
		Button(const Quad& quad, CallbackFunc_t callback, const ::std::string& idle_text, const ::std::string& pressed_text, ENG_Ptr(Shader) idle_shader, ENG_Ptr(Shader) pressed_shader);
		Button(const Quad& quad, CallbackFunc_t callback, const ::std::string& idle_text, const ::std::string& pressed_text);
		Button(const Quad& quad, CallbackFunc_t callback, const ::std::string& text);
		~Button();

		void Update();
		void CheckClicked(const Vector2<float>& cursor_pos);
		void CheckPressed(const Vector2<float>& cursor_pos);

		ENG_Ptr(Shader) CurrentShader();
		ENG_Ptr(Texture) CurrentTexture();
		const ::std::string& CurrentText() const;
	};
}
