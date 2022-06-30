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

Button::Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text, Shader* default_shader, Shader* pressed_shader)
	: quad(quad),
	callback(callback), 
	default_text(default_text), pressed_text(pressed_text), 
	default_shader(default_shader), pressed_shader(pressed_shader)
{
	GUIManager::buttons.Push(this);
}

Button::Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text)
	: quad(quad),
	callback(callback),
	default_text(default_text), pressed_text(pressed_text),
	default_shader(m_std_default_shader), pressed_shader(m_std_pressed_shader)
{
	GUIManager::buttons.Push(this);
}

Button::Button(const Quad& quad, CallbackFunc_t callback, std::string text)
	: quad(quad),
	callback(callback),
	default_text(text), pressed_text(text)
{
	GUIManager::buttons.Push(this);
}

Button::~Button()
{
	GUIManager::buttons.Remove(this);
}

void Button::CheckClicked(const Vector2<float>& cursor_pos)
{
	// Check if we're visible
	if (isVisible) {
		// If the cursor is within the bounds of the button
		if (quad.Contains(cursor_pos)) {
			// Call callback function
			callback(this);
			// We're not being pressed anymore
			isPressed = false;

			// DEBUG:
			Log("Button just released!", Utils::ERROR::INFO);
		}
	}
}

void Button::CheckPressed(const Vector2<float>& cursor_pos)
{
	// Check if we're visible
	if (isVisible) {
		// If the cursor is within the bounds of the button
		if (quad.Contains(cursor_pos)) {
			isPressed = true;

			// DEBUG:
			Log("Button being pressed!", Utils::ERROR::INFO);
		}
	}
}

void Button::Draw()
{
	if (isVisible) {
		// Construct shared ptr to our quad
		std::shared_ptr<Quad> quad_ptr = std::shared_ptr<Quad>(&quad);

		// Declare button renderable ptr
		Renderable button_renderable = Renderable(
			quad_ptr,
			isPressed ? pressed_shader : default_shader, // Select correct shader based on "isPressed"
			Button::Z
		);
	}
}
