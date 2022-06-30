#include "GUIManager.h"
#include "Button.h"

Utils::List<Button*> GUIManager::buttons = {};

void GUIManager::Update()
{
	Vector2<float> cursor_pos = Input::GetCursorPos();

	Input::State lmb_state = Input::GetMouseState(GLFW_MOUSE_BUTTON_1);

	// If the state is released
	if (lmb_state == Input::State::RELEASE) {
		for (Button* button : buttons) {
			// Check if button was clicked
			button->CheckClicked(cursor_pos);
		}
	}

	// If state is clicked/held
	if (lmb_state == Input::State::PRESS || lmb_state == Input::State::HOLD) {
		for (Button* button : buttons) {
			// Check if button is pressed
			button->CheckPressed(cursor_pos);
		}
	}
}
