#include "TextInput.h"

namespace Vivium {
	Ref(Animation) TextInput::m_TypingBar = nullptr;
	RGBColor TextInput::m_DefaultEmptyColor = RGBColor(0.0f, 0.0f, 0.0f);
	RGBColor TextInput::m_DefaultTypedColor = RGBColor(0.0f, 0.0f, 0.0f);
	std::string TextInput::m_DefaultEmptyText = "";
	Ref(Font) TextInput::m_DefaultFont = nullptr;
	Ref(Shader) TextInput::m_DefaultBgShader = nullptr;
	Ref(Shader) TextInput::m_TextShader = nullptr;
	Ref(Shader) TextInput::m_TypingBarShader = nullptr;

	bool TextInput::m_TypingOnAnyInput = false;

	void TextInput::m_Init(Ref(TextureAtlas) engine_icons)
	{
		m_TypingBarShader = MakeRef(Shader, "static_texture_vertex", "texture_frag");

		m_TypingBar = MakeRef(Animation,
			MakeRef(Quad, 150, 150, 128, 128, 0),
			m_TypingBarShader,
			engine_icons,
			Animation::Data("engine_icons")
		);

		m_DefaultEmptyColor = { 0.3f, 0.3f, 0.3f };
		m_DefaultTypedColor = { 0.0f, 0.0f, 0.0f };
		m_DefaultEmptyText = "Type here...";
		m_DefaultFont = Text::GetDefaultFont();

		m_DefaultBgShader = MakeRef(Shader, "static_vertex", "transparency_frag");
		m_DefaultBgShader->Bind(); m_DefaultBgShader->SetUniform4f("u_Color", 1.0, 1.0, 1.0, 0.9f);

		m_TextShader = MakeRef(Shader, "text_vertex", "text_frag");
		m_TextShader->Bind(); m_TextShader->SetUniform3f("u_TextColor", m_DefaultEmptyColor.r, m_DefaultEmptyColor.g, m_DefaultEmptyColor.b);
	}

	void TextInput::m_ResetTypingStatus()
	{
		m_TypingOnAnyInput = false;
	}

	void TextInput::SetUserParams(void* userParams) { m_UserParams = userParams; }

	void TextInput::SetCharLimit(const std::size_t& char_limit)
	{
		m_CharLimit = char_limit;

		if (typed_text.size() > char_limit) {
			typed_text = typed_text.substr(0, char_limit);
		}
	}

	void TextInput::CheckClicked(const Vector2<float>& cursor_pos, const Input::State& lmb_state)
	{
		bool withinQuad = quad->Contains(cursor_pos);

		if (withinQuad || isTyping) {
			m_TypingOnAnyInput = true;
			Application::SetCursor(Application::CURSOR_TYPE::TYPING);
		}
		else if (!m_TypingOnAnyInput) {
			Application::SetCursor(Application::CURSOR_TYPE::POINTER);
		}

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
		Vector2<float> pos = quad->GetCenter();
		pos.x -= (quad->GetWidth() / 2.0f) - m_Offset;
		m_Text = MakeRef(Text, empty_text, pos, m_TextShader, Text::Alignment::LEFT, m_DefaultScale);
	}

	void TextInput::m_UpdateText()
	{
		// TODO: cleanup

		// TODO: ctrl + a, select, etc.
		// TODO: arrow keys

		// Get list of last keypresses
		std::vector<Input::KeypressLog> last_keypresses = Input::GetLastKeypresses();
		std::vector<unsigned int> last_chars = Input::GetLastChars();

		// Index into last chars
		int chars_index = 0;

		// Iterate it
		for (auto keypress : last_keypresses) {
			const char* key_name = glfwGetKeyName(keypress.key, 0);

			// Indicates a printable character
			if (key_name != NULL) {
				// Check not above char limit
				if (m_CharLimit == typed_text.size()) { continue; }

				// Get char
				char c = last_chars.at(chars_index);
				// Add to string
				// If typing index out of range
				if (m_TypingIndex >= typed_text.size()) {
					typed_text += c;
				}
				else {
					typed_text.insert(m_TypingIndex, std::string(1, c));
				}
	
				// Increment char index
				++chars_index;
				// Increment typing index
				++m_TypingIndex;
			}
			else {
				switch (keypress.key) {
				case GLFW_KEY_SPACE: typed_text += ' '; m_TypingIndex++; break;
				case GLFW_KEY_LEFT: m_TypingIndex--; break; // TODO: holding functions for left/right
				case GLFW_KEY_RIGHT: m_TypingIndex = std::min((unsigned int)m_TypingIndex + 1, typed_text.size()); break;
				}
			}

			// If pressed backspace or delete
			if (keypress.key == GLFW_KEY_BACKSPACE || keypress.key == GLFW_KEY_DELETE) {
				// String is not empty (can't remove characters from an empty string
				if (typed_text != "") {
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
			}

			// Pressed enter
			if (keypress.key == GLFW_KEY_ENTER) {
				// Submit the text
				callback(this, m_UserParams);
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
		CheckClicked(Input::GetCursorPos(), Input::GetMouseState(GLFW_MOUSE_BUTTON_1));

		float elapsed = m_Timer.GetElapsed();

		// If we're currently typing
		if (isTyping) {
			m_UpdateText();
			m_Text->SetText(typed_text);
			m_Text->FixToWidth(quad->GetWidth());
			m_Text->shader->Bind(); m_Text->shader->SetUniform3f("u_TextColor", m_TypedColor.r, m_TypedColor.g, m_TypedColor.b);
		}
		else {
			// Not typing and no text typed currently
			if (typed_text == "") {
				m_Text->SetText(empty_text);
				m_Text->FixToWidth(quad->GetWidth());
				m_Text->shader->Bind(); m_Text->shader->SetUniform3f("u_TextColor", m_EmptyColor.r, m_EmptyColor.g, m_EmptyColor.b);
			}
		}

		m_TypingBar->Update();
	}

	TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, void* userParams, int offset)
		: quad(MakeRef(Quad, quad)), callback(callback), bg_shader(m_DefaultBgShader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
		m_EmptyColor(m_DefaultEmptyColor), m_TypedColor(m_DefaultTypedColor), m_Offset(offset), m_UserParams(userParams)
	{
		m_Construct();
	}

	TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, Ref(Shader) bg_shader, void* userParams, int offset)
		: quad(MakeRef(Quad, quad)), callback(callback), bg_shader(bg_shader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
		m_EmptyColor(m_DefaultEmptyColor), m_TypedColor(m_DefaultTypedColor), m_Offset(offset), m_UserParams(userParams)
	{
		m_Construct();
	}

	TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, Ref(Shader) bg_shader, Ref(Texture) bg_texture, void* userParams, int offset)
		: quad(MakeRef(Quad, quad)), callback(callback), bg_shader(bg_shader), bg_texture(bg_texture), empty_text(m_DefaultEmptyText),
		m_EmptyColor(m_DefaultEmptyColor), m_TypedColor(m_DefaultTypedColor), m_Offset(offset), m_UserParams(userParams)
	{
		m_Construct();
	}

	TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, const RGBColor& typed_color, const RGBColor& empty_color, void* userParams, int offset)
		: quad(MakeRef(Quad, quad)), callback(callback), bg_shader(m_DefaultBgShader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
		m_EmptyColor(empty_color), m_TypedColor(typed_color), m_Offset(offset), m_UserParams(userParams)
	{
		m_Construct();
	}

	TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, const RGBColor& typed_color, const RGBColor& empty_color, Ref(Shader) bg_shader, void* userParams, int offset)
		: quad(MakeRef(Quad, quad)), callback(callback), bg_shader(bg_shader), bg_texture(nullptr), empty_text(m_DefaultEmptyText),
		m_EmptyColor(empty_color), m_TypedColor(typed_color), m_Offset(offset), m_UserParams(userParams)
	{
		m_Construct();
	}

	TextInput::TextInput(const Quad& quad, CallbackFunc_t callback, const RGBColor& typed_color, const RGBColor& empty_color, Ref(Shader) bg_shader, Ref(Texture) bg_texture, void* userParams, int offset)
		: quad(MakeRef(Quad, quad)), callback(callback), bg_shader(bg_shader), bg_texture(bg_texture), empty_text(m_DefaultEmptyText),
		m_EmptyColor(empty_color), m_TypedColor(typed_color), m_Offset(offset), m_UserParams(userParams)
	{
		m_Construct();
	}

	void TextInput::SetPos(const Vector2<float>& new_pos)
	{
		quad->SetCenter(new_pos);
		*m_Text->pos = Vector2<float>(new_pos.x - (quad->GetWidth() / 2.0f) + m_Offset, new_pos.y);
	}

	Ref(Animation) TextInput::GetTypingBar()
	{
		// TODO Function is 2 in 1 kinda ugly
		// Update pos
		// TODO: only works on monospace fonts
		Font::Character& ch = TextInput::m_DefaultFont->character_map.at('|');
		float char_x = ch.advance >> 6; // Convert advance to pixels

		// TODO magic numbers
		float size = m_DefaultFont->max_height * m_DefaultScale * 1.1;

		m_TypingBar->quad->SetDim(Vector2<float>(size, size));

		// TODO: figure out how to position the typing bar

		float left = quad->GetX() - (quad->GetWidth() / 2.0f) + (char_x * m_TypingIndex * m_DefaultScale) + (m_TypingBar->quad->GetWidth() / 2.0f);
		Vector2<float> pos = Vector2<float>(
			left + m_Offset,
			quad->GetY() + m_TypingBar->quad->GetHeight() / 2.0f - (size * 0.2)
			);

		m_TypingBar->quad->SetCenter(pos);

		return m_TypingBar;
	}

	bool TextInput::GetIsTyping()
	{
		return isTyping;
	}

	Vector2<float> TextInput::GetPos() const
	{
		return quad->GetCenter();
	}

	Ref(Text) TextInput::GetText()
	{
		return m_Text;
	}

	TextInput::~TextInput() {
	}
}