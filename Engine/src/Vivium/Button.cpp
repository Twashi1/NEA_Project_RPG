#include "Button.h"

namespace Vivium {
	Ref(Shader) Button::m_DefaultIdleShader = nullptr;
	Ref(Shader) Button::m_DefaultPressedShader = nullptr;
	Ref(Font) Button::m_DefaultFont = nullptr;

	void Button::m_Construct()
	{
		// Construct text object
		Vector2<float> pos = quad.GetCenter() - (m_GetTextDim(idle_text) * 0.5f);
		text = new Text(idle_text, pos, Button::m_DefaultScale);
	}

	Vector2<float> Button::m_GetTextDim(const std::string& text)
	{
		// TODO: only works on monospace fonts
		Font::Character& ch = Button::m_DefaultFont->character_map.at('a');
		float char_x = ch.advance >> 6; // Convert advance to pixels

		Vector2<float> dim = Vector2<float>(char_x * (float)text.length(), ch.size.y);

		return dim * Button::m_DefaultScale;
	}

	void Button::m_Init()
	{
		// Check if class has been initialised already (the standard shaders are default until initialised)
		if (m_DefaultIdleShader != nullptr) {
			LogWarn("Attempting to reinitialise Button class");
		}

		// Construct default shaders
		m_DefaultIdleShader = MakeRef(Shader, "static_vertex", "transparency_frag");
		m_DefaultIdleShader->SetUniform4f("u_Color", 0.3, 0.3, 0.3, Button::m_DefaultScale);

		m_DefaultPressedShader = MakeRef(Shader, "static_vertex", "transparency_frag");
		m_DefaultPressedShader->SetUniform4f("u_Color", 0.2, 0.2, 0.2, Button::m_DefaultScale);

		// Construct default font
		m_DefaultFont = Text::GetDefaultFont();
	}

	void Button::SetPos(const Vector2<float>& pos)
	{
		quad.SetCenter(pos);
		m_UpdatePos();
	}

	const Vector2<float>& Button::GetPos() const
	{
		return quad.GetCenter();
	}

	Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Ref(Shader) idle_shader, Ref(Shader) pressed_shader, Ref(Texture) idle_texture, Ref(Texture) pressed_texture, void* userParams)
		: quad(quad),
		callback(callback),
		idle_text(idle_text), pressed_text(pressed_text),
		idle_shader(idle_shader), pressed_shader(pressed_shader),
		idle_texture(idle_texture), pressed_texture(pressed_texture),
		m_UserParams(userParams)
	{
		m_Construct();
	}

	Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Ref(Shader) idle_shader, Ref(Shader) pressed_shader, void* userParams)
		: quad(quad),
		callback(callback),
		idle_text(idle_text), pressed_text(pressed_text),
		idle_shader(idle_shader), pressed_shader(pressed_shader),
		m_UserParams(userParams)
	{
		m_Construct();
	}

	Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, void* userParams)
		: quad(quad),
		callback(callback),
		idle_text(idle_text), pressed_text(pressed_text),
		idle_shader(m_DefaultIdleShader), pressed_shader(m_DefaultPressedShader),
		m_UserParams(userParams)
	{
		m_Construct();
	}

	Button::Button(const Quad& quad, CallbackFunc_t callback, const std::string& text, void* userParams)
		: quad(quad),
		callback(callback),
		idle_text(text), pressed_text(text),
		idle_shader(m_DefaultIdleShader), pressed_shader(m_DefaultPressedShader),
		m_UserParams(userParams)
	{
		m_Construct();
	}

	Button::~Button()
	{
		delete text;
	}

	void Button::SetUserParams(void* userParams) { m_UserParams = userParams; }

	void Button::Update()
	{
		Vector2<float> cursor_pos = Input::GetCursorPos();
		Input::State lmb_state = Input::GetMouseState(GLFW_MOUSE_BUTTON_1);

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
			callback(this, m_UserParams);

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

	Ref(Shader) Button::CurrentShader()
	{
		return isPressed ? pressed_shader : idle_shader;
	}

	Ref(Texture) Button::CurrentTexture()
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
}