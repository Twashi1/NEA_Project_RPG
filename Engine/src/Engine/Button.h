#pragma once

#include "Utils.h"
#include "Input.h"
#include "GUIManager.h"
#include "Text.h"
#include "Font.h"

class ENGINE_API Button {
private:
	static constexpr float z = 5;
	static Shader* m_DefaultIdleShader; // Standard shader for when button is not pressed
	static Shader* m_DefaultPressedShader; // Standard shader for when button is pressed
	static std::shared_ptr<Font> m_DefaultFont;

	static constexpr float m_DefaultAlpha = 1.0f; // Standard alpha (transparency) for a button
	static constexpr float m_DefaultScale = 0.25f; // Standard scaling factor for text (1/4 size)

	// Constructs the objects
	void m_Construct();
	// Gets width/height of text so that it can be centered on the button
	Vector2<float> m_GetTextDim(const std::string& text);

public:
	typedef void (*CallbackFunc_t)(Button*); // Shorthand for a callback function pointer (takes the button that was pressed as a paramater)

	CallbackFunc_t callback; // Function called when button is pressed
	std::string idle_text; // Text that displays when button is not pressed
	std::string pressed_text; // Text that displays when button is pressed

	// TODO: std shaders can be exposed through these
	Shader* idle_shader; // Shader used when button is not pressed
	Shader* pressed_shader; // Shader used when button is pressed

	Texture* idle_texture = nullptr;
	Texture* pressed_texture = nullptr;

	bool isPressed = false; // If button is being pressed

	Quad quad; // Stores position and dimensions of button
	Text* text; // Text object, stores information required to render text

	// Initialises standard shaders and Input listeners
	static void Init();

	// TODO better constructor?
	Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Shader* idle_shader, Shader* pressed_shader, Texture* idle_texture, Texture* pressed_texture);
	Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Shader* idle_shader, Shader* pressed_shader);
	Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text, Texture* idle_texture, Texture* pressed_texture);
	Button(const Quad& quad, CallbackFunc_t callback, const std::string& idle_text, const std::string& pressed_text);
	Button(const Quad& quad, CallbackFunc_t callback, const std::string& text);
	~Button();

	void CheckClicked(const Vector2<float>& cursor_pos);
	void CheckPressed(const Vector2<float>& cursor_pos);

	Shader* CurrentShader();
	Texture* CurrentTexture();
	const std::string& CurrentText() const;

	// Updates button's text position - Need to call this if you edit "quad"
	void UpdatePos();
};