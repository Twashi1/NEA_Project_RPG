#include "TextInput.h"

Animation* TextInput::m_TypingBar = nullptr;
Vector3<float> TextInput::m_DefaultEmptyColor = Vector3<float>(0.0, 0.0, 0.0);
Vector3<float> TextInput::m_DefaultTypedColor = Vector3<float>(0.0, 0.0, 0.0);
std::string TextInput::m_DefaultEmptyText = "";
Font* TextInput::m_DefaultFont = nullptr;
Shader* TextInput::m_DefaultBgShader = nullptr;
Shader* TextInput::m_TextShader = nullptr;
GLFWcursor* TextInput::m_Cursor = nullptr;
Shader* TextInput::m_TypingBarShader = nullptr;

void TextInput::Init(Texture* engine_icons, GLFWcursor* cursor)
{
	m_Cursor = cursor;

	m_TypingBarShader = new Shader("static_texture_vertex", "static_texture_frag");

	m_TypingBar = new Animation(
		std::shared_ptr<Quad>(new Quad(150, 150, 128, 128, 0)),
		std::shared_ptr<Shader>(m_TypingBarShader),
		std::shared_ptr<Texture>(engine_icons),
		{16, 16},
		"engine_icons"
	);

	m_DefaultEmptyColor = COLORS::GRAY;
	m_DefaultTypedColor = COLORS::BLACK;
	m_DefaultEmptyText = "Type here...";
	m_DefaultFont = Text::GetDefaultFont();

	m_DefaultBgShader = new Shader("button_vertex", "button_frag");
	m_DefaultBgShader->Bind(); m_DefaultBgShader->SetUniform4f("u_Color", COLORS::WHITE.x, COLORS::WHITE.y, COLORS::WHITE.z, 0.9f);

	m_TextShader = new Shader("text_vertex", "text_frag");
	m_TextShader->Bind(); m_TextShader->SetUniform3f("u_TextColor", m_DefaultEmptyColor);
}

void TextInput::CheckClicked(const Input::State& lmb_state, const Vector2<float>& cursor_pos)
{
	bool withinQuad = quad.Contains(cursor_pos);

	// TODO: cursor changes
	/*
	if (withinQuad) {
		glfwDestroyCursor(m_Cursor);
		m_Cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
	}
	else {
		glfwDestroyCursor(m_Cursor);
		m_Cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
	}
	*/

	if (lmb_state == Input::State::RELEASE) {
		// They clicked on us
		if (withinQuad) {
			isTyping = true;
		}
		// They clicked somewhere not on us
		else {
			isTyping = false;
		}
	}
}

void TextInput::m_Construct()
{
	Vector2<float> pos = Vector2<float>(quad.GetX() - quad.GetWidth() / 2.0f, quad.GetY());
	m_Text = new Text(empty_text, pos, m_TextShader, m_DefaultScale);

	// Push ourselves to GUIManager
	GUIManager::text_inputs.Push(this);
}

void TextInput::m_UpdateText()
{
	// TODO: ctrl + a, select, etc.
	// TODO: arrow keys

	// Get list of last keypresses
	std::vector<Input::KeypressLog> last_keypresses = Input::GetLastKeypresses();
	std::vector<unsigned int> last_chars = Input::GetLastChars();

	int chars_index = 0;

	// Iterate it
	for (auto keypress : last_keypresses) {
		const char* key_name = glfwGetKeyName(keypress.key, 0);

		// Indicates a printable character
		if (key_name != NULL) {
			// Get char
			char c = last_chars.at(chars_index);
			// Add to string
			typed_text += c;
			// Increment char index
			chars_index++;
			// Increment typing index
			m_TypingIndex++;
		}
		else {
			switch ((char)keypress.key) {
			case ' ': typed_text += ' '; m_TypingIndex++; break;
			}
		}

		// If pressed backspace or delete
		if (keypress.key == GLFW_KEY_BACKSPACE || keypress.key == GLFW_KEY_DELETE) {
			// Ctrl + backspace should delete a whole word
			if (keypress.mods & Input::Mod::CTRL) {
				// Index we're deleting up to
				int delete_end = m_TypingIndex - 1;
				// Index of the first character after the last space before our cursor pos
				int delete_start = typed_text.rfind(' ', delete_end - 1);
				// Length of substring we're removing
				int length = delete_end - delete_start;

				// Erase length
				typed_text.erase(delete_start + 1, length);

				// Subtract length from index we're typing at
				m_TypingIndex -= length;
				
			}
			// Just a normal backspace
			else {
				typed_text.pop_back(); // Remove last character
				m_TypingIndex--; // Remove one from index we're typing at
			}
		}
		
		// Pressed enter
		if (keypress.key == GLFW_KEY_ENTER) {
			// Submit the text
			callback(this);
			// No longer typing (make subroutine probably so other events that need to happen, happen
			isTyping = false;
			// TODO: move to next text input box
			// Stop parsing any other characters inputted
			break;
		}
	}
}

void TextInput::Update()
{
	float current_time = Utils::Timer::GetTime();
	float elapsed = current_time - m_Time;
	m_Time = current_time;

	// If we're currently typing
	if (isTyping) {
		m_UpdateText();
		m_Text->text = typed_text;
		m_Text->shader->Bind(); m_Text->shader->SetUniform3f("u_TextColor", m_TypedColor);
	}
	else {
		// Not typing and no text typed currently
		if (typed_text == "") {
			m_Text->text = empty_text;
			m_Text->shader->Bind(); m_Text->shader->SetUniform3f("u_TextColor", m_EmptyColor);
		}
	}
}

TextInput::TextInput(const Quad& quad, CallbackFunc_t callback)
	: quad(quad), callback(callback), bg_shader(m_DefaultBgShader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
	m_EmptyColor(m_DefaultEmptyColor), m_TypedColor(m_DefaultTypedColor)
{
	m_Construct();
}

TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, Shader* bg_shader)
	: quad(quad), callback(callback), bg_shader(bg_shader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
	m_EmptyColor(m_DefaultEmptyColor), m_TypedColor(m_DefaultTypedColor)
{
	m_Construct();
}

TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, Shader* bg_shader, Texture* bg_texture)
	: quad(quad), callback(callback), bg_shader(bg_shader), bg_texture(bg_texture), empty_text(m_DefaultEmptyText),
	m_EmptyColor(m_DefaultEmptyColor), m_TypedColor(m_DefaultTypedColor)
{
	m_Construct();
}

TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, const Vector3<float>& typed_color, const Vector3<float>& empty_color)
	: quad(quad), callback(callback), bg_shader(m_DefaultBgShader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
	m_EmptyColor(empty_color), m_TypedColor(typed_color)
{
	m_Construct();
}

TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, const Vector3<float>& typed_color, const Vector3<float>& empty_color, Shader* bg_shader)
	: quad(quad), callback(callback), bg_shader(bg_shader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
	m_EmptyColor(empty_color), m_TypedColor(typed_color)
{
	m_Construct();
}

TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, const Vector3<float>& typed_color, const Vector3<float>& empty_color, Shader* bg_shader, Texture* bg_texture)
	: quad(quad), callback(callback), bg_shader(bg_shader), bg_texture(bg_texture), empty_text(m_DefaultEmptyText),
	m_EmptyColor(empty_color), m_TypedColor(typed_color)
{
	m_Construct();
}

Animation* TextInput::GetTypingBar()
{
	// TODO Function is 2 in 1 kinda ugly
	// Update pos
	// TODO: only works on monospace fonts
	Font::Character& ch = TextInput::m_DefaultFont->character_map.at('a');
	float char_x = ch.advance >> 6; // Convert advance to pixels

	float size = 64 * m_DefaultScale;

	m_TypingBar->quad->SetDim(Vector2<float>(size, size));

	// TODO: figure out how to position the cursor

	float left = quad.GetX() - (quad.GetWidth() / 2.0f) + (char_x * m_TypingIndex * m_DefaultScale) + (m_TypingBar->quad->GetWidth() / 2.0f);
	Vector2<float> pos = Vector2<float>(
		left,
		quad.GetY() + (size * 0.3)
		);

	m_TypingBar->quad->SetCenter(pos);

	return m_TypingBar;
}

bool TextInput::GetIsTyping()
{
	return isTyping;
}

Text* TextInput::GetText()
{
	return m_Text;
}

TextInput::~TextInput() {
	delete m_Text;

	// Remove ourselves from GUIManager
	GUIManager::text_inputs.Remove(this);
}