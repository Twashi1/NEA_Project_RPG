#include "Button.h"

Shader* Button::m_std_default_shader = nullptr;
Shader* Button::m_std_pressed_shader = nullptr;

void Button::Init()
{
	// Check if class has been initialised already (the standard shaders are default until initialised)
	if (m_std_default_shader != nullptr) {
		Log("Attempting to reinitialise Button class", Utils::ERROR::WARNING);
	}

	// Add listener for left mouse button
	Input::AddMouseListener(GLFW_MOUSE_BUTTON_1);

	// Create default shaders
	m_std_default_shader = new Shader("button_vertex", "button_shader");
	m_std_default_shader->SetUniform4f("u_Color", COLORS::GRAY.x, COLORS::GRAY.y, COLORS::GRAY.z, Button::m_std_alpha);

	m_std_pressed_shader = new Shader("button_vertex", "button_shader");
	m_std_pressed_shader->SetUniform4f("u_Color", COLORS::DARKGRAY.x, COLORS::DARKGRAY.y, COLORS::DARKGRAY.z, Button::m_std_alpha);
}

Button::Button(std::shared_ptr<Quad> quad_ptr, CallbackFunc_t callback)
{
	
}

void Button::Update()
{
	// Get position of cursor
	Vector2<float> cursor_pos = Input::GetCursorPos();
}