#pragma once

#include "Utilities.h"
#include "Renderable.h"
#include "Input.h"
#include "GUIManager.h"

class ENGINE_API Button {
private:
	static constexpr int Z = 5;
	static Shader* m_std_default_shader; // Standard shader for when button is not pressed
	static Shader* m_std_pressed_shader; // Standard shader for when button is pressed
	static constexpr float m_std_alpha = 0.7f; // Standard alpha (transparency) for a button

public:
	typedef void (*CallbackFunc_t)(Button*); // Shorthand for a callback function pointer (takes the button that was pressed as a paramater)

	CallbackFunc_t callback; // Function called when button is pressed
	std::string default_text; // Text that displays when button is not pressed
	std::string pressed_text; // Text that displays when button is pressed

	Shader* default_shader; // Shader used when button is not pressed
	Shader* pressed_shader; // Shader used when button is pressed

	bool isVisible = true;  // If button is being drawn or not
	bool isPressed = false; // If button is being pressed

	Quad quad; // Stores position and dimensions of button

	// Initialises standard shaders and Input listeners
	static void Init();

	Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text, Shader* default_shader, Shader* pressed_shader);
	Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text);
	Button(const Quad& quad, CallbackFunc_t callback, std::string text);
	~Button();

	void CheckClicked(const Vector2<float>& cursor_pos);
	void CheckPressed(const Vector2<float>& cursor_pos);
	void Draw();
};