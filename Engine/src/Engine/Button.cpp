#include "Button.h"

Shader* Button::m_DefaultIdleShader = nullptr;
Shader* Button::m_DefaultPressedShader = nullptr;
std::shared_ptr<Font> Button::m_DefaultFont = nullptr;

void Button::m_Construct()
{
	// Construct text object
	Vector2<float> pos = quad.GetCenter() - (m_GetTextDim(idle_text) * 0.5f);
	text = new Text(idle_text, pos, Button::m_DefaultScale);

	// Push ourselves to GUIManager
	GUIManager::buttons.Push(this);
}

Vector2<float> Button::m_GetTextDim(const std::string& text)
{
	// TODO: only works on monospace fonts
	Font::Character& ch = Button::m_DefaultFont->character_map.at('a');
	float char_x = ch.advance >> 6; // Convert advance to pixels
	
	Vector2<float> dim = Vector2<float>(char_x * (float)text.length(), ch.size.y);

	return dim * Button::m_DefaultScale;
}

void Button::Init()
{
	// Check if class has been initialised already (the standard shaders are default until initialised)
	if (m_DefaultIdleShader != nullptr) {
		Log("Attempting to reinitialise Button class", LOG::WARNING);
	}

	// Add listener for left mouse button
	Input::AddMouseListener(GLFW_MOUSE_BUTTON_1);

	// Construct default shaders
	m_DefaultIdleShader = new Shader("button_vertex", "button_frag");
	m_DefaultIdleShader->SetUniform4f("u_Color", COLORS::GRAY.x, COLORS::GRAY.y, COLORS::GRAY.z, Button::m_DefaultScale);

	m_DefaultPressedShader = new Shader("button_vertex", "button_frag");
	m_DefaultPressedShader->SetUniform4f("u_Color", COLORS::DARKGRAY.x, COLORS::DARKGRAY.y, COLORS::DARKGRAY.z, Button::m_DefaultScale);

	// Construct default font
	m_DefaultFont = std::shared_ptr<Font>(new Font("consola.ttf"));
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Shader* idle_shader, Shader* pressed_shader, Texture* idle_texture, Texture* pressed_texture)
	: quad(quad),
	callback(callback),
	idle_text(idle_text), pressed_text(pressed_text),
	idle_shader(idle_shader), pressed_shader(pressed_shader),
	idle_texture(idle_texture), pressed_texture(pressed_texture)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Shader* idle_shader, Shader* pressed_shader)
	: quad(quad),
	callback(callback), 
	idle_text(idle_text), pressed_text(pressed_text),
	idle_shader(idle_shader), pressed_shader(pressed_shader)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Texture* idle_texture, Texture* pressed_texture)
	: quad(quad),
	callback(callback),
	idle_text(idle_text), pressed_text(pressed_text),
	idle_shader(Renderer::texture_shader), pressed_shader(Renderer::texture_shader),
	idle_texture(idle_texture), pressed_texture(pressed_texture)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text)
	: quad(quad),
	callback(callback),
	idle_text(idle_text), pressed_text(pressed_text),
	idle_shader(m_DefaultIdleShader), pressed_shader(m_DefaultPressedShader)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& text)
	: quad(quad),
	callback(callback),
	idle_text(text), pressed_text(text),
	idle_shader(m_DefaultIdleShader), pressed_shader(m_DefaultPressedShader)
{
	m_Construct();
}

Button::~Button()
{
	delete text;

	// Remove ourselves from GUIManager
	GUIManager::buttons.Remove(this);
}

void Button::CheckClicked(const Vector2<float>& cursor_pos)
{
	// Reset back to default state, regardless if we specifically were the button pressed
	// This is so if you pressed on a button, then move your cursor away and release, the button
	// will still reset back to its default state (but doesn't register as us being clicked)
		
	// We're not being pressed anymore
	isPressed = false;

	// Update text
	text->text = isPressed ? pressed_text : idle_text;

	// If the cursor is within the bounds of the button, run callback function
	if (quad.Contains(cursor_pos)) {
		// Execute callback function
		callback(this);

		// Update position
		UpdatePos();
	}
}

void Button::CheckPressed(const Vector2<float>& cursor_pos)
{
	// If the cursor is within the bounds of the button
	if (quad.Contains(cursor_pos)) {
		// We're being pressed
		isPressed = true;

		// Update text
		text->text = isPressed ? pressed_text : idle_text;

		// Update position
		UpdatePos();
	}
}

Shader* Button::CurrentShader()
{
	return isPressed ? pressed_shader : idle_shader;
}

Texture* Button::CurrentTexture()
{
	return isPressed ? pressed_texture : idle_texture;
}

const std::string& Button::CurrentText() const
{
	return isPressed ? pressed_text : idle_text;
}

void Button::UpdatePos()
{
	// Subtracting half the size of the text being displayed
	// from the center of the background so that the text is centered
	text->pos = quad.GetCenter() - (m_GetTextDim(isPressed ? pressed_text : idle_text) * 0.5f);
}
