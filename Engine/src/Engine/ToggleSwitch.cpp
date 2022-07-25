#include "ToggleSwitch.h"

namespace CoolEngineName {
	ENG_Ptr(Shader) ToggleSwitch::m_DefaultShader;
	ENG_Ptr(Texture) ToggleSwitch::m_DefaultOnTexture;
	ENG_Ptr(Texture) ToggleSwitch::m_DefaultOffTexture;
	ENG_Ptr(Texture) ToggleSwitch::m_DefaultPressedOnTexture;
	ENG_Ptr(Texture) ToggleSwitch::m_DefaultPressedOffTexture;

	void ToggleSwitch::Init(const char* engine_icons_path)
	{

	}

	Shader* ToggleSwitch::GetShader()
	{
		if (state) {
			if (isPressed) {
				return pressed_on_shader.get();
			}
			else {
				return on_shader.get();
			}
		}
		else {
			if (isPressed) {
				return pressed_off_shader.get();
			}
			else {
				return off_shader.get();
			}
		}
	}

	Texture* ToggleSwitch::GetTexture()
	{
		if (state) {
			if (isPressed) {
				return pressed_on_texture.get();
			}
			else {
				return on_texture.get();
			}
		}
		else {
			if (isPressed) {
				return pressed_off_texture.get();
			}
			else {
				return off_texture.get();
			}
		}
	}

	void ToggleSwitch::Update()
	{
		Vector2<float> cursor_pos = Input::GetCursorPos();
		Input::State lmb_state = Input::GetMouseState(GLFW_MOUSE_BUTTON_1);

		// If clicked on switch
		if (quad->Contains(cursor_pos)) {
			if (lmb_state == Input::State::PRESS || lmb_state == Input::State::HOLD) {
				isPressed = true;
			}
			else if (lmb_state == Input::State::RELEASE) {
				isPressed = false;
				state = !state;
				callback(this);
			}
		}
	}

	ToggleSwitch::ToggleSwitch(ENG_Ptr(Quad) quad, CallbackFunc_t callback, bool default_state)
		: quad(quad), callback(callback), state(default_state),
		on_shader(m_DefaultShader), off_shader(m_DefaultShader),
		pressed_on_shader(m_DefaultShader), pressed_off_shader(m_DefaultShader),
		on_texture(m_DefaultOnTexture), off_texture(m_DefaultOffTexture),
		pressed_on_texture(m_DefaultPressedOnTexture), pressed_off_texture(m_DefaultPressedOffTexture)
	{
	}
}