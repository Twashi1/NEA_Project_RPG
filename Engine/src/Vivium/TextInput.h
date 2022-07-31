#pragma once

#include "Utils.h"
#include "Input.h"
#include "Text.h"
#include "Font.h"
#include "Animation.h"
#include "Application.h"

namespace Vivium {
	class VIVIUM_API TextInput {
	public:
		typedef void (*CallbackFunc_t)(TextInput*); // Shorthand for a callback function pointer (takes the text input that was entered as a paramater)

		static Ref(Animation) m_TypingBar; // Animation for the bar that shows where your cursor currently is
		static Color m_DefaultEmptyColor; // Default color for text that displays when input is empty
		static Color m_DefaultTypedColor; // Default color for text that user has typed in
		static std::string m_DefaultEmptyText; // Default text that displays when box is empty
		static Ref(Font) m_DefaultFont;
		static Ref(Shader) m_DefaultBgShader;
		static Ref(Shader) m_TextShader;
		static Ref(Shader) m_TypingBarShader;

		static GLFWcursor* m_Cursor;

		static constexpr float m_DefaultScale = 0.25f; // Standard scaling factor for text (1/4 size)

		// Constructs the objects
		void m_Construct();

		// TODO implement stall time
		float m_StallTime = 0.0; // After typing a letter, the typing bar stays visible for a small amount of time extra, this tracks how long it has to stall for
		Timer m_Timer;

		bool isTyping = false;
		int m_TypingIndex = 0; // Current index of where we're typing in the string

		Color m_EmptyColor;
		Color m_TypedColor;

		Ref(Text) m_Text = nullptr;

		std::string empty_text;	// Text that displays when textinput is empty

		// Updates the text based on user input
		void m_UpdateText();

		CallbackFunc_t callback;

		int m_Offset;

	public:
		::std::string typed_text = ""; // Text that user has typed/is typing in

		Ref(Texture) bg_texture = nullptr;
		Ref(Shader) bg_shader;

		Quad quad;

		void SetPos(const Vector2<float>& new_pos);
		const Vector2<float>& GetPos() const;

		Ref(Animation) GetTypingBar();
		bool GetIsTyping();
		Ref(Text) GetText();

		static void Init(Ref(Texture) engine_icons);

		TextInput(const Quad& quad, CallbackFunc_t callback, int offset = 0);
		TextInput(const Quad& quad, CallbackFunc_t callback, Ref(Shader) bg_shader, int offset = 0);
		TextInput(const Quad& quad, CallbackFunc_t callback, Ref(Shader) bg_shader, Ref(Texture) bg_texture, int offset = 0);
		TextInput(const Quad& quad, CallbackFunc_t callback, const Color& typed_color, const Color& empty_color, int offset = 0);
		TextInput(const Quad& quad, CallbackFunc_t callback, const Color& typed_color, const Color& empty_color, Ref(Shader) bg_shader, int offset = 0);
		TextInput(const Quad& quad, CallbackFunc_t callback, const Color& typed_color, const Color& empty_color, Ref(Shader) bg_shader, Ref(Texture) bg_texture, int offset = 0);
		~TextInput();

		void CheckClicked(const Vector2<float>& cursor_pos, const Input::State& lmb_state);
		void Update();
	};
}
