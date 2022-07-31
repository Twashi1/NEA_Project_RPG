#include "Player.h"

void Player::Update()
{
    float current = Vivium::Timer::GetTime();
    float elapsed = current - m_Time; m_Time = current;
    // TODO: cleanup

    Vivium::Input::State w = Vivium::Input::GetKeyState(GLFW_KEY_W);
    Vivium::Input::State s = Vivium::Input::GetKeyState(GLFW_KEY_S);
    Vivium::Input::State a = Vivium::Input::GetKeyState(GLFW_KEY_A);
    Vivium::Input::State d = Vivium::Input::GetKeyState(GLFW_KEY_D);

    // If W key pressed
    if (w == Vivium::Input::State::PRESS || w == Vivium::Input::State::HOLD) {
        body->acc.y = MAXACCEL;
    }
    // If S key pressed
    else if (s == Vivium::Input::State::PRESS || s == Vivium::Input::State::HOLD) {
        body->acc.y = -MAXACCEL;
    }
    // If D key pressed
    if (d == Vivium::Input::State::PRESS || d == Vivium::Input::State::HOLD) {
        body->acc.x = MAXACCEL;
    }
    // If A key pressed
    else if (a == Vivium::Input::State::PRESS || a  == Vivium::Input::State::HOLD) {
        body->acc.x = -MAXACCEL;
    }
    // If W and S key aren't pressed, or both are pressed
    if ((w == Vivium::Input::State::NONE && s == Vivium::Input::State::NONE)
        || w == Vivium::Input::State::HOLD && s == Vivium::Input::State::HOLD) {
        body->acc.y = 0.0f;
    }
    // If D and A key aren't pressed, or both are pressed
    if ((d == Vivium::Input::State::NONE && a == Vivium::Input::State::NONE)
        || d == Vivium::Input::State::HOLD && a == Vivium::Input::State::HOLD) {
        body->acc.x = 0.0f;
    }

    body->acc -= body->vel * FRICTION;
    body->vel = body->vel + (body->acc * elapsed);
}

Player::Player()
{
    // Setup player quad and body
    quad = Vivium::Quad(0.0f, 0.0f, 100.0f, 100.0f);
    body = MakeRef(Vivium::Body, Ref(Vivium::Quad)(&quad), true, 0.0f, 1.0f);

    // Setup shader and uniforms
    shader = new Vivium::Shader("world_vertex", "color_frag");
    shader->SetUniform3f("u_Color", 1.0, 1.0, 0.0);

    m_Time = Vivium::Timer::GetTime();
}

Player::~Player() { delete shader; }