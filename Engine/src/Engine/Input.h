#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Utilities.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

class ENGINE_API Input {
public:
	enum class State : uint8_t {
		PRESS,   // If last action was RELEASE, and current action is PRESS, then PRESS
		RELEASE, // If last action was PRESS/HOLD, and current action is RELEASE, then RELEASE
		HOLD,    // If last action was PRESS, and current action is PRESS, then HOLD
		NONE     // If last action was RELEASE, and current action is RELEASE, then NONE
	};

private:
	static int m_GetKeyState(int key);		// Return Input::State for given GLFW key code
	static int m_GetMouseState(int mouse);  // Return Input::State for given GLFW btn code

	// Listener object to track last action, current state of a certain key/button, and time button has been held down for
	struct Listener {
		int last_action;	// Last action of button (GLFW_RELEASE/GLFW_PRESSED)
		Input::State state; // Current state of the button
		float time_held;	// Counts seconds key/button has been held down for

		Listener();
	};

	static std::unordered_map<int, Listener> key_listeners;   // Maps a GLFW key code to its respective Listener object
	static std::unordered_map<int, Listener> mouse_listeners; // Maps a GLFW btn code to its respective Listener object

	// Updates a listener's state based on the GLFW action code and time since last update (GLFW_RELEASE/GLFW_PRESSED)
	static void m_UpdateListener(Listener& listener, int current_action, float dt);

public:
	static GLFWwindow* window;			

	static void AddKeyListener(int key);
	static void AddMouseListener(int btn);

	static Input::State GetKeyState(int key);
	static Input::State GetMouseState(int button);
	// Get position of cursor in screen coordinates (0, 0) -> (width, height)
	static Vector2<float> GetCursorPos();
	// Get position of cursor within range (0.0, 0.0) -> (1.0, 1.0) in UV coordinates (Takes window width/height)
	static Vector2<float> GetCursorUVPos(int width, int height);

	// Update all listener objects, takes time since last update
	static void Update(float dt);
};