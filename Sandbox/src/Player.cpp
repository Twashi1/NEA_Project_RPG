#include "Player.h"

void Player::Update(float new_time)
{
    float dt = new_time - last_time;
    last_time = new_time;
    // TODO: cleanup

    // If W key pressed
    if (Input::GetKeyState(GLFW_KEY_W) == Input::State::PRESS || Input::GetKeyState(GLFW_KEY_W) == Input::State::HOLD) {
        body->acc.y = MAXACCEL;
    }
    // If S key pressed
    else if (Input::GetKeyState(GLFW_KEY_S) == Input::State::PRESS || Input::GetKeyState(GLFW_KEY_S) == Input::State::HOLD) {
        body->acc.y = -MAXACCEL;
    }
    // If D key pressed
    if (Input::GetKeyState(GLFW_KEY_D) == Input::State::PRESS || Input::GetKeyState(GLFW_KEY_D) == Input::State::HOLD) {
        body->acc.x = MAXACCEL;
    }
    // If A key pressed
    else if (Input::GetKeyState(GLFW_KEY_A) == Input::State::PRESS || Input::GetKeyState(GLFW_KEY_A) == Input::State::HOLD) {
        body->acc.x = -MAXACCEL;
    }
    // If W and S key aren't pressed, or both are pressed
    if ((Input::GetKeyState(GLFW_KEY_W) == Input::State::NONE && Input::GetKeyState(GLFW_KEY_S) == Input::State::NONE)
        || Input::GetKeyState(GLFW_KEY_W) == Input::State::HOLD && Input::GetKeyState(GLFW_KEY_S) == Input::State::HOLD) {
        body->acc.y = 0.0f;
    }
    // If D and A key aren't pressed, or both are pressed
    if ((Input::GetKeyState(GLFW_KEY_D) == Input::State::NONE && Input::GetKeyState(GLFW_KEY_A) == Input::State::NONE)
        || Input::GetKeyState(GLFW_KEY_D) == Input::State::HOLD && Input::GetKeyState(GLFW_KEY_A) == Input::State::HOLD) {
        body->acc.x = 0.0f;
    }

    body->acc -= body->vel * FRICTION;
    body->vel = body->vel + (body->acc * dt);
}

Player::Player(const glm::mat4& proj)
{
    // Setup player quad and body
    quad = std::shared_ptr<Quad>(new Quad(0.0f, 0.0f, 100.0f, 100.0f, 0.0f));
    body = std::shared_ptr<Body>(new Body(quad.get(), true, 0.0f, 1.0f));

    // Setup shader and uniforms
    shader = new Shader("player_vertex", "player_frag");
    shader->SetUniformMat4fv("u_projMat", proj);
    shader->SetUniform3f("u_Color", COLORS::YELLOW);
    shader->SetUniform1f("u_ZCoord", ZLEVEL);
}

Player::~Player() { delete shader; }