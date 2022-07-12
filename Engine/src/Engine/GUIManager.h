#pragma once

#include <vector>

#include "Utils.h"
#include "Input.h"

// TODO: is using managers like this really the best API? also since all these list are public
// any buttons that should normally be private are exposed through their pointers

class Button;
class TextInput;

// Tracks buttons and other GUI objects
class ENGINE_API GUIManager {
public:
	static Utils::List<Button*> buttons;
	static Utils::List<TextInput*> text_inputs;

	// Updates all gui objects
	static void Update();

	friend Button;
	friend TextInput;
};