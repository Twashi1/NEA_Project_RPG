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
	typedef void (*CallbackFunc_t)(void); // Shorthand for a callback function pointer

	CallbackFunc_t callback; // Function called when button is pressed
	std::string default_text; // Text that displays when button is not pressed
	std::string pressed_text; // Text that displays when button is pressed

	Shader* default_shader; // Shader used when button is not pressed
	Shader* pressed_shader; // Shader used when button is pressed

	bool isPressed; // State of button on last update

	std::shared_ptr<Quad> quad; // Stores position and dimensions of button

	// Initialises standard shaders and Input listeners
	static void Init();

	//Button(const Vector2<float>& pos, const Vector2<float>& dim, CallbackFunc_t callback);
	//Button(const Rect& rect, CallbackFunc_t callback);
	//Button(const Quad& quad, CallbackFunc_t callback);
	// Changing the quad will change position of Button if using this constructor
	Button(std::shared_ptr<Quad> quad_ptr, CallbackFunc_t callback);

	void Update();
};