#pragma once

#include <cstdint>
#include <unordered_map>
#include <vector>
#include <unordered_map>
#include <iostream>

#include "Utils.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

class ENGINE_API Input {
public:
	ENGINE_API enum class State: uint8_t {
		NONE = 0,    // If last action was RELEASE, and current action is RELEASE, then NONE
		PRESS = 1,   // If last action was RELEASE, and current action is PRESS, then PRESS
		RELEASE = 2, // If last action was PRESS/HOLD, and current action is RELEASE, then RELEASE
		HOLD = 4     // If last action was PRESS, and current action is PRESS, then HOLD
	};

	ENGINE_API enum class Mod : int {
		NONE = 0,
		SHIFT = GLFW_MOD_SHIFT,
		CTRL = GLFW_MOD_CONTROL,
		ALT = GLFW_MOD_ALT,
		SUPER = GLFW_MOD_SUPER,
		CAPS_LOCK = GLFW_MOD_CAPS_LOCK,
		NUM_LOCK = GLFW_MOD_NUM_LOCK
	};

	ENGINE_API struct KeypressLog {
		int key;
		int mods;

		KeypressLog(int key, int mods);
		KeypressLog(const KeypressLog& other);
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

	static std::vector<unsigned int> last_chars; // Stores a log of all key presses that occured before the last Update
	static std::vector<unsigned int> current_chars; // Stores a log of all key presses that have occured after the last Update
	static std::vector<KeypressLog> last_keypresses;
	static std::vector<KeypressLog> current_keypresses;

	// Callback function for key presses as characters
	static void m_CharCallback(GLFWwindow* window, unsigned int codepoint);
	static void m_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

	static std::unordered_map<int, Listener> key_listeners;   // Maps a GLFW key code to its respective Listener object
	static std::unordered_map<int, Listener> mouse_listeners; // Maps a GLFW btn code to its respective Listener object

	// Updates a listener's state based on the GLFW action code and time since last update (GLFW_RELEASE/GLFW_PRESSED)
	static void m_UpdateListener(Listener& listener, int current_action, float dt);

	static float m_time;

	static int m_CurrentMods;

public:
	static GLFWwindow* window;
	// Pointers to window width/height which are stored in Engine class
	static int* window_width;
	static int* window_height;

	static void Init(GLFWwindow* window, int* window_width, int* window_height);

	static void AddKeyListener(int key);
	static void AddMouseListener(int btn);

	static const std::vector<KeypressLog>& GetLastKeypresses();
	static const std::vector<unsigned int>& GetLastChars();

	static int GetMods();
	static bool CheckMod(const Input::Mod& mod);
	static bool CheckMod(const int& mods);

	static Input::State GetKeyState(int key);
	static Input::State GetMouseState(int button);
	// Get position of cursor in screen coordinates (0, 0) -> (width, height)
	static Vector2<float> GetCursorPos();
	// Get position of cursor within range (0.0, 0.0) -> (1.0, 1.0) in UV coordinates
	static Vector2<float> GetCursorUVPos();

	// Update all listener objects, takes time since last update
	static void Update(float dt);
};

ENGINE_API int operator|(const Input::Mod& a, const Input::Mod& b);
ENGINE_API int operator|(int a, const Input::Mod& b);
ENGINE_API int operator|(const Input::Mod& a, int b);
ENGINE_API int operator&(const Input::Mod& a, const Input::Mod& b);
ENGINE_API int operator&(int a, const Input::Mod& b);
ENGINE_API int operator&(const Input::Mod& a, int b);

ENGINE_API std::string to_string(const Input::State& state);
ENGINE_API std::string to_string(const Input::Mod& mod);
