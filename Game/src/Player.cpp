#include "Player.h"

void Player::Update()
{
    float current_time = CoolEngineName::Utils::Timer::GetTime();
    float elapsed = current_time - m_time; m_time = current_time;
    // TODO: cleanup

    CoolEngineName::Input::State w = CoolEngineName::Input::GetKeyState(GLFW_KEY_W);
    CoolEngineName::Input::State s = CoolEngineName::Input::GetKeyState(GLFW_KEY_S);
    CoolEngineName::Input::State a = CoolEngineName::Input::GetKeyState(GLFW_KEY_A);
    CoolEngineName::Input::State d = CoolEngineName::Input::GetKeyState(GLFW_KEY_D);

    // If W key pressed
    if (w == CoolEngineName::Input::State::PRESS || w == CoolEngineName::Input::State::HOLD) {
        body->acc.y = MAXACCEL;
    }
    // If S key pressed
    else if (s == CoolEngineName::Input::State::PRESS || s == CoolEngineName::Input::State::HOLD) {
        body->acc.y = -MAXACCEL;
    }
    // If D key pressed
    if (d == CoolEngineName::Input::State::PRESS || d == CoolEngineName::Input::State::HOLD) {
        body->acc.x = MAXACCEL;
    }
    // If A key pressed
    else if (a == CoolEngineName::Input::State::PRESS || a  == CoolEngineName::Input::State::HOLD) {
        body->acc.x = -MAXACCEL;
    }
    // If W and S key aren't pressed, or both are pressed
    if ((w == CoolEngineName::Input::State::NONE && s == CoolEngineName::Input::State::NONE)
        || w == CoolEngineName::Input::State::HOLD && s == CoolEngineName::Input::State::HOLD) {
        body->acc.y = 0.0f;
    }
    // If D and A key aren't pressed, or both are pressed
    if ((d == CoolEngineName::Input::State::NONE && a == CoolEngineName::Input::State::NONE)
        || d == CoolEngineName::Input::State::HOLD && a == CoolEngineName::Input::State::HOLD) {
        body->acc.x = 0.0f;
    }

    body->acc -= body->vel * FRICTION;
    body->vel = body->vel + (body->acc * elapsed);
}

Player::Player()
{
    // Setup player quad and body
    quad = CoolEngineName::Quad(0.0f, 0.0f, 100.0f, 100.0f, 0.0f);
    body = ENG_MakePtr(CoolEngineName::Body, ENG_Ptr(CoolEngineName::Quad)(&quad), true, 0.0f, 1.0f);

    // Setup shader and uniforms
    shader = new CoolEngineName::Shader("world_vertex", "color_frag");
    shader->SetUniform3f("u_Color", CoolEngineName::COLORS::YELLOW);
}

Player::~Player() { delete shader; }