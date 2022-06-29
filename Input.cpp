#include "Input.h"

GLFWwindow* Input::window = nullptr;

std::unordered_map<int, Input::Listener> Input::key_listeners{};
std::unordered_map<int, Input::Listener> Input::mouse_listeners{};

int Input::m_GetKeyState(int key)
{
	return glfwGetKey(window, key);
}

int Input::m_GetMouseState(int mouse)
{
	return glfwGetMouseButton(window, mouse);
}

void Input::m_UpdateListener(Listener& listener, int current_action, float dt)
{
	if (listener.state == State::HOLD) listener.time_held += dt;
	else listener.time_held = 0.0f;

	if (listener.last_action == GLFW_RELEASE) {
		if (current_action == GLFW_PRESS)		 listener.state = State::PRESS;
		else if (current_action == GLFW_RELEASE) listener.state = State::NONE;
	}
	else if (listener.last_action == GLFW_PRESS) {
		if (current_action == GLFW_PRESS)		 listener.state = State::HOLD;
		else if (current_action == GLFW_RELEASE) listener.state = State::RELEASE;
	}

	listener.last_action = current_action;
}

void Input::AddKeyListener(int key)
{
	key_listeners[key] = Listener();
}

void Input::AddMouseListener(int btn)
{
	mouse_listeners[btn] = Listener();
}

Input::State Input::GetKeyState(int key)
{
	// Get iterator to key state
	auto it = key_listeners.find(key);

	// If iterator exists - key exists in key_listeners
	if (it != key_listeners.end()) {
		// Return key state
		return it->second.state;
	}

	Log(std::format("Couldn't find key {}", std::to_string(key)), Utilities::ERROR::WARNING);

	return State::NONE;
}

Input::State Input::GetMouseState(int button)
{
	// Get iterator to button state
	auto it = mouse_listeners.find(button);

	// If iterator exists - button exists in mouse_listeners
	if (it != mouse_listeners.end()) {
		// Return button state
		return it->second.state;
	}

	Log(std::format("Couldn't find button {}", std::to_string(button)), Utilities::ERROR::WARNING);

	return State::NONE;
}

Vector2<float> Input::GetCursorPos()
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	return Vector2<float>(xpos, ypos);
}

Vector2<float> Input::GetCursorUVPos(int width, int height)
{
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Divide by width and height, and invert height to convert to uv coordinates
	return Vector2<float>(xpos / (float)width, (height - ypos) / (float)height);
}

void Input::Update(float dt)
{
	for (auto& [key, listener] : key_listeners) {
		int current_action = m_GetKeyState(key);

		m_UpdateListener(listener, current_action, dt);
	}

	for (auto& [btn, listener] : mouse_listeners) {
		int current_action = m_GetMouseState(btn);

		m_UpdateListener(listener, current_action, dt);
	}
}

Input::Listener::Listener()
	: last_action(GLFW_RELEASE), state(Input::State::NONE), time_held(0.0f)
{}
