#include "Button.h"

Shader* Button::m_std_default_shader = nullptr;
Shader* Button::m_std_pressed_shader = nullptr;
Font* Button::font = nullptr;

void Button::m_Construct()
{
	if (m_std_default_shader == nullptr) Log("Button class hasn't been initialised yet! Call Button::Init", Utils::ERROR::WARNING);
	if (Button::font == nullptr) Log("Font has not been set yet! Pass Font* to Button::font", Utils::ERROR::FATAL);

	// Construct renderable
	renderable = new Renderable(std::shared_ptr<Quad>(&quad), default_shader, Button::z, false);
	// Construct text renderable
	text_renderable = new TextRenderable(default_text, quad.GetCenter() - (m_GetTextDim(default_text) * 0.5f), Button::font, COLORS::WHITE, Button::m_std_scale, Button::z);

	// Push ourselves to GUIManager
	GUIManager::buttons.Push(this);
}

Vector2<float> Button::m_GetTextDim(const std::string& text)
{
	// TODO: only works on monospace fonts
	Font::Character& ch = Button::font->character_map.at('a');
	float char_x = ch.advance >> 6; // Convert advance to pixels
	
	Vector2<float> dim = Vector2<float>(char_x * (float)text.length(), ch.size.y);

	return dim * Button::m_std_scale;
}

void Button::Init()
{
	// Check if class has been initialised already (the standard shaders are default until initialised)
	if (m_std_default_shader != nullptr) {
		Log("Attempting to reinitialise Button class", Utils::ERROR::WARNING);
	}

	// Add listener for left mouse button
	Input::AddMouseListener(GLFW_MOUSE_BUTTON_1);

	// Create default shaders
	m_std_default_shader = new Shader("button_vertex", "button_frag");
	m_std_default_shader->SetUniform4f("u_Color", COLORS::GRAY.x, COLORS::GRAY.y, COLORS::GRAY.z, Button::m_std_alpha);

	m_std_pressed_shader = new Shader("button_vertex", "button_frag");
	m_std_pressed_shader->SetUniform4f("u_Color", COLORS::DARKGRAY.x, COLORS::DARKGRAY.y, COLORS::DARKGRAY.z, Button::m_std_alpha);
}

Button::Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text, Shader* default_shader, Shader* pressed_shader)
	: quad(quad),
	callback(callback), 
	default_text(default_text), pressed_text(pressed_text), 
	default_shader(default_shader), pressed_shader(pressed_shader),
	renderable(nullptr)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, std::string default_text, std::string pressed_text)
	: quad(quad),
	callback(callback),
	default_text(default_text), pressed_text(pressed_text),
	default_shader(m_std_default_shader), pressed_shader(m_std_pressed_shader),
	renderable(nullptr)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, std::string text)
	: quad(quad),
	callback(callback),
	default_text(text), pressed_text(text),
	renderable(nullptr)
{
	m_Construct();
}

Button::~Button()
{
	// Remove ourselves from GUIManager
	GUIManager::buttons.Remove(this);

	// Delete our renderables
	delete renderable;
	delete text_renderable;
}

void Button::CheckClicked(const Vector2<float>& cursor_pos)
{
	// Check if we're visible
	if (isVisible) {
		// Reset back to default state, regardless if we specifically were the button pressed
		// This is so if you pressed on a button, then move your cursor away and release, the button
		// will still reset back to its default state (but doesn't register as us being clicked)
		
		// We're not being pressed anymore
		isPressed = false;

		// Set shader to default shader
		renderable->shader = default_shader;
		// Set text to default text
		text_renderable->text = default_text;

		// If the cursor is within the bounds of the button, run callback function
		if (quad.Contains(cursor_pos)) {
			// Execute callback function
			callback(this);

			// Update position
			UpdatePos();
		}
	}
}

void Button::CheckPressed(const Vector2<float>& cursor_pos)
{
	// Check if we're visible
	if (isVisible) {
		// If the cursor is within the bounds of the button
		if (quad.Contains(cursor_pos)) {
			// We're being pressed
			isPressed = true;

			// Set shader to pressed shader
			renderable->shader = pressed_shader;
			// Set text to pressed text
			text_renderable->text = pressed_text;

			// Update position
			UpdatePos();
		}
	}
}

void Button::UpdatePos()
{
	// Subtracting half the size of the text being displayed
	// from the center of the background so that the text is centered
	text_renderable->pos = quad.GetCenter() - (m_GetTextDim(isPressed ? pressed_text : default_text) * 0.5f);
}
