#pragma once

#include "Utils.h"
#include "Renderable.h"
#include "Input.h"
#include "GUIManager.h"
#include "TextRenderable.h"
#include "Font.h"

class ENGINE_API Button {
private:
	static constexpr int z = 5;
	static Shader* m_std_default_shader; // Standard shader for when button is not pressed
	static Shader* m_std_pressed_shader; // Standard shader for when button is pressed

	static constexpr float m_std_alpha = 1.0f; // Standard alpha (transparency) for a button
	static constexpr float m_std_scale = 0.25f; // Standard scaling factor for text (1/4 size)

	// Constructs the objects
	void m_Construct();
	// Gets width/height of text so that it can be centered on the button
	Vector2<float> m_GetTextDim(const std::string& text);

public:
	static Font* font;

	typedef void (*CallbackFunc_t)(Button*); // Shorthand for a callback function pointer (takes the button that was pressed as a paramater)

	CallbackFunc_t callback; // Function called when button is pressed
	std::string default_text; // Text that displays when button is not pressed
	std::string pressed_text; // Text that displays when button is pressed

	// TODO: std shaders can be exposed through these
	Shader* default_shader; // Shader used when button is not pressed
	Shader* pressed_shader; // Shader used when button is pressed

	bool isVisible = true;  // If button is being drawn or not
	bool isPressed = false; // If button is being pressed

	Renderable* renderable; // Pointer to renerable object so this button is drawn
	TextRenderable* text_renderable; // Pointer to text renderable so this button's text is drawn
	// TODO: maybe should be private?
	Quad quad; // Stores position and dimensions of button

	// Initialises standard shaders and Input listeners
	static void Init();

	Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text, Shader* default_shader, Shader* pressed_shader);
	Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text);
	Button(const Quad& quad, CallbackFunc_t callback, std::string text);
	~Button();

	void CheckClicked(const Vector2<float>& cursor_pos);
	void CheckPressed(const Vector2<float>& cursor_pos);

	// Updates button's text position - Need to call this if you edit "quad"
	void UpdatePos();
};