#include "GUIManager.h"
#include "Button.h"
#include "TextInput.h"
#include "Slider.h"

std::vector<Button*> GUIManager::buttons{};
std::vector<TextInput*> GUIManager::text_inputs{};
std::vector<Slider*> GUIManager::sliders{};

void GUIManager::Update()
{
	// TODO code so bad ahhahahahahh

	Vector2<float> cursor_pos = Input::GetCursorPos();

	Input::State lmb_state = Input::GetMouseState(GLFW_MOUSE_BUTTON_1);

	// If the state is released
	if (lmb_state == Input::State::RELEASE) {
		for (Button* button : buttons) {
			// Check if button was clicked
			button->CheckClicked(cursor_pos);
			// Update pos just in case
			button->UpdatePos();
		}
	}

	// If state is clicked/held
	if (lmb_state == Input::State::PRESS || lmb_state == Input::State::HOLD) {
		for (Button* button : buttons) {
			// Check if button is pressed
			button->CheckPressed(cursor_pos);
			// Update pos just in case
			button->UpdatePos();
		}
	}

	for (TextInput* text_input : text_inputs) {
		text_input->CheckClicked(lmb_state, cursor_pos);
		text_input->Update();
	}

	for (Slider* slider : sliders) {
		slider->Update(cursor_pos);
	}
}
