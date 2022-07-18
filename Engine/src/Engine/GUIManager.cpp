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

	for (Button* button : buttons) {
		button->Update(cursor_pos, lmb_state);
	}

	for (TextInput* text_input : text_inputs) {
		text_input->Update(cursor_pos, lmb_state);
	}

	for (Slider* slider : sliders) {
		slider->Update(cursor_pos, lmb_state);
	}
}
