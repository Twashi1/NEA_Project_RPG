#include "Player.h"

void Player::Update()
{
    float current_time = Utils::Timer::GetTime();
    float elapsed = current_time - m_time; m_time = current_time;
    // TODO: cleanup

    Input::State w = Input::GetKeyState(GLFW_KEY_W);
    Input::State s = Input::GetKeyState(GLFW_KEY_S);
    Input::State a = Input::GetKeyState(GLFW_KEY_A);
    Input::State d = Input::GetKeyState(GLFW_KEY_D);

    // If W key pressed
    if (w == Input::State::PRESS || w == Input::State::HOLD) {
        body->acc.y = MAXACCEL;
    }
    // If S key pressed
    else if (s == Input::State::PRESS || s == Input::State::HOLD) {
        body->acc.y = -MAXACCEL;
    }
    // If D key pressed
    if (d == Input::State::PRESS || d == Input::State::HOLD) {
        body->acc.x = MAXACCEL;
    }
    // If A key pressed
    else if (a == Input::State::PRESS || a  == Input::State::HOLD) {
        body->acc.x = -MAXACCEL;
    }
    // If W and S key aren't pressed, or both are pressed
    if ((w == Input::State::NONE && s == Input::State::NONE)
        || w == Input::State::HOLD && s == Input::State::HOLD) {
        body->acc.y = 0.0f;
    }
    // If D and A key aren't pressed, or both are pressed
    if ((d == Input::State::NONE && a == Input::State::NONE)
        || d == Input::State::HOLD && a == Input::State::HOLD) {
        body->acc.x = 0.0f;
    }

    body->acc -= body->vel * FRICTION;
    body->vel = body->vel + (body->acc * elapsed);
}

Player::Player()
{
    // Setup player quad and body
    quad = Quad(0.0f, 0.0f, 100.0f, 100.0f, 0.0f);
    body = ENG_MakePtr(Body, ENG_Ptr(Quad)(&quad), true, 0.0f, 1.0f);

    // Setup shader and uniforms
    shader = new Shader("world_vertex", "color_frag");
    shader->SetUniform3f("u_Color", COLORS::YELLOW);
}

Player::~Player() { delete shader; }