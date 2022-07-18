#pragma once

#include <vector>

#include "Utils.h"
#include "Input.h"

// TODO: is using managers like this really the best API? also since all these list are public
// any buttons that should normally be private are exposed through their pointers

class Button;
class TextInput;
class Slider;
class Text;

// Tracks buttons and other GUI objects
class ENGINE_API GUIManager {
public:
	static std::vector<Button*> buttons;
	static std::vector<TextInput*> text_inputs;
	static std::vector<Text*> text;
	static std::vector<Slider*> sliders;

	// Updates all gui objects
	static void Update();

	friend Button;
	friend TextInput;
	friend Slider;
};