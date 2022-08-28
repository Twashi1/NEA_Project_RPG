#pragma once

#include "Core.h"
#include "Logger.h"
#include "Input.h"
#include "Text.h"
#include "Font.h"
#include "Quad.h"
#include "Application.h"

namespace Vivium {
	class VIVIUM_API Button {
	private:
		static Ref(Shader) m_DefaultIdleShader; // Standard shader for when button is not pressed
		static Ref(Shader) m_DefaultPressedShader; // Standard shader for when button is pressed
		static Ref(Font) m_DefaultFont;

		static constexpr float m_DefaultAlpha = 1.0f; // Standard alpha (transparency) for a button
		static constexpr float m_DefaultScale = 0.25f; // Standard scaling factor for text (1/4 size)

		// Constructs the objects
		void m_Construct();

		// Updates button's text position - Need to call this if you edit "quad"
		void m_UpdatePos();

		void* m_UserParams;

		// Initialises standard shaders
		static void m_Init();

	public:
		typedef void (*CallbackFunc_t)(Button*, void*); // Shorthand for a callback function pointer (takes the button that was pressed as a paramater)

		CallbackFunc_t callback; // Function called when button is pressed
		std::string idle_text; // Text that displays when button is not pressed
		std::string pressed_text; // Text that displays when button is pressed

		// TODO: std shaders can be exposed through these
		Ref(Shader) idle_shader; // Shader used when button is not pressed
		Ref(Shader) pressed_shader; // Shader used when button is pressed

		Ref(Texture) idle_texture = nullptr;
		Ref(Texture) pressed_texture = nullptr;

		bool isPressed = false; // If button is being pressed

		Ref(Quad) quad; // Stores position and dimensions of button
		Ref(Text) text; // Text object, stores information required to render text TODO: should be private

		void SetPos(const Vector2<float>& pos);
		Vector2<float> GetPos() const;

		// TODO better constructor?
		Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Ref(Shader) idle_shader, Ref(Shader) pressed_shader, Ref(Texture) idle_texture, Ref(Texture) pressed_texture, void* userParams = nullptr);
		Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Ref(Shader) idle_shader, Ref(Shader) pressed_shader, void* userParams = nullptr);
		Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, void* userParams = nullptr);
		Button(const Quad& quad, CallbackFunc_t callback, const std::string& text, void* userParams = nullptr);
		~Button();

		void SetUserParams(void* userParams);

		void Update();
		void CheckClicked(const Vector2<float>& cursor_pos);
		void CheckPressed(const Vector2<float>& cursor_pos);

		Ref(Shader) CurrentShader();
		Ref(Texture) CurrentTexture();
		const std::string& CurrentText() const;

		friend Application;
	};
}
