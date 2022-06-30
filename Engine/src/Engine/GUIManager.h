#pragma once

#include <vector>

#include "Utilities.h"

class Button;

// Tracks buttons and other GUI objects
class ENGINE_API GUIManager {
	std::vector<Button*> m_buttons;
};