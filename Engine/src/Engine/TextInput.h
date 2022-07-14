#pragma once

#include "Utils.h"
#include "Input.h"
#include "GUIManager.h"
#include "Text.h"
#include "Font.h"
#include "Animation.h"
#include "Engine.h"

class ENGINE_API TextInput {
private:
	typedef void (*CallbackFunc_t)(TextInput*); // Shorthand for a callback function pointer (takes the text input that was entered as a paramater)

	static ENG_Ptr(Animation) m_TypingBar; // Animation for the bar that shows where your cursor currently is
	static Vector3<float> m_DefaultEmptyColor; // Default color for text that displays when input is empty
	static Vector3<float> m_DefaultTypedColor; // Default color for text that user has typed in
	static std::string m_DefaultEmptyText; // Default text that displays when box is empty
	static ENG_Ptr(Font) m_DefaultFont;
	static ENG_Ptr(Shader) m_DefaultBgShader;
	static ENG_Ptr(Shader) m_TextShader;
	static ENG_Ptr(Shader) m_TypingBarShader;

	static GLFWcursor* m_Cursor;

	static constexpr float m_DefaultScale = 0.25f; // Standard scaling factor for text (1/4 size)

	// Constructs the objects
	void m_Construct();

	float m_StallTime = 0.0; // After typing a letter, the typing bar stays visible for a small amount of time extra, this tracks how long it has to stall for
	float m_Time = 0.0; // Time of last update of this instance

	bool isTyping = false;
	int m_TypingIndex = 0; // Current index of where we're typing in the string

	Vector3<float> m_EmptyColor;
	Vector3<float> m_TypedColor;

	ENG_Ptr(Text) m_Text = nullptr;

	std::string empty_text;	// Text that displays when textinput is empty

	// Updates the text based on user input
	void m_UpdateText();

	CallbackFunc_t callback;

public:
	std::string typed_text = ""; // Text that user has typed/is typing in

	ENG_Ptr(Texture) bg_texture = nullptr;
	ENG_Ptr(Shader) bg_shader;

	Quad quad;

	ENG_Ptr(Animation) GetTypingBar();
	bool GetIsTyping();
	ENG_Ptr(Text) GetText();

	static void Init(ENG_Ptr(Texture) engine_icons);

	TextInput(const Quad& quad, CallbackFunc_t callback);
	TextInput(const Quad& quad, CallbackFunc_t callback, ENG_Ptr(Shader) bg_shader);
	TextInput(const Quad& quad, CallbackFunc_t callback, ENG_Ptr(Shader) bg_shader, ENG_Ptr(Texture) bg_texture);
	TextInput(const Quad& quad, CallbackFunc_t callback, const Vector3<float>& typed_color, const Vector3<float>& empty_color);
	TextInput(const Quad& quad, CallbackFunc_t callback, const Vector3<float>& typed_color, const Vector3<float>& empty_color, ENG_Ptr(Shader) bg_shader);
	TextInput(const Quad& quad, CallbackFunc_t callback, const Vector3<float>& typed_color, const Vector3<float>& empty_color, ENG_Ptr(Shader) bg_shader, ENG_Ptr(Texture) bg_texture);
	~TextInput();

	void CheckClicked(const Input::State& lmb_state, const Vector2<float>& cursor_pos);
	void Update();
};