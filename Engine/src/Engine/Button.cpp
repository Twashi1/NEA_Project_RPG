#include "Button.h"

ENG_Ptr(Shader) Button::m_DefaultIdleShader = nullptr;
ENG_Ptr(Shader) Button::m_DefaultPressedShader = nullptr;
ENG_Ptr(Font) Button::m_DefaultFont = nullptr;

void Button::m_Construct()
{
	// Construct text object
	Vector2<float> pos = quad.GetCenter() - (m_GetTextDim(idle_text) * 0.5f);
	text = new Text(idle_text, pos, Button::m_DefaultScale);

	// Push ourselves to GUIManager
	GUIManager::buttons.push_back(this);
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
		ENG_LogWarn("Attempting to reinitialise Button class");
	}

	// Construct default shaders
	m_DefaultIdleShader = ENG_MakePtr(Shader, "static_vertex", "transparency_frag");
	m_DefaultIdleShader->SetUniform4f("u_Color", COLORS::GRAY.x, COLORS::GRAY.y, COLORS::GRAY.z, Button::m_DefaultScale);

	m_DefaultPressedShader = ENG_MakePtr(Shader, "static_vertex", "transparency_frag");
	m_DefaultPressedShader->SetUniform4f("u_Color", COLORS::DARKGRAY.x, COLORS::DARKGRAY.y, COLORS::DARKGRAY.z, Button::m_DefaultScale);

	// Construct default font
	m_DefaultFont = Text::GetDefaultFont();
}

void Button::SetPos(const Vector2<float>& pos)
{
	quad.SetCenter(pos);
	m_UpdatePos();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, ENG_Ptr(Shader) idle_shader, ENG_Ptr(Shader) pressed_shader, ENG_Ptr(Texture) idle_texture, ENG_Ptr(Texture) pressed_texture)
	: quad(quad),
	callback(callback),
	idle_text(idle_text), pressed_text(pressed_text),
	idle_shader(idle_shader), pressed_shader(pressed_shader),
	idle_texture(idle_texture), pressed_texture(pressed_texture)
{
	m_Construct();
}

Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, ENG_Ptr(Shader) idle_shader, ENG_Ptr(Shader) pressed_shader)
	: quad(quad),
	callback(callback), 
	idle_text(idle_text), pressed_text(pressed_text),
	idle_shader(idle_shader), pressed_shader(pressed_shader)
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
	Utils::Remove(GUIManager::buttons, this);
}

void Button::Update(const Vector2<float>& cursor_pos, Input::State lmb_state)
{
	if (lmb_state == Input::State::RELEASE) {
		CheckClicked(cursor_pos);
	}
	else if (lmb_state == Input::State::PRESS || lmb_state == Input::State::HOLD) {
		CheckPressed(cursor_pos);
	}

	m_UpdatePos();
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
		m_UpdatePos();
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
		m_UpdatePos();
	}
}

ENG_Ptr(Shader) Button::CurrentShader()
{
	return isPressed ? pressed_shader : idle_shader;
}

ENG_Ptr(Texture) Button::CurrentTexture()
{
	return isPressed ? pressed_texture : idle_texture;
}

const std::string& Button::CurrentText() const
{
	return isPressed ? pressed_text : idle_text;
}

void Button::m_UpdatePos()
{
	// Subtracting half the size of the text being displayed
	// from the center of the background so that the text is centered
	text->pos = quad.GetCenter() - (m_GetTextDim(isPressed ? pressed_text : idle_text) * 0.5f);
}
