#include "Player.h"

namespace Game {
    void Player::m_RenderSelectedTile()
    {
        if (selected_tile.bot != Tile::ID::VOID) {
            m_SelectedTileQuad->SetCenter(selected_tile_pos * World::PIXEL_SCALE);
            Vivium::Renderer::Submit(m_SelectedTileQuad, m_SelectedTileShader, m_GameIcons->GetAtlas().get());
        }
    }

    void Player::m_RenderInventory()
    {
        if (m_isInventoryOpened) {
            m_MainInventory.SetPos(Vivium::Application::GetScreenDim() / 2);
            m_MainInventory.Render();
        }
    }

    void Player::m_UpdateMovement()
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
        else if (a == Vivium::Input::State::PRESS || a == Vivium::Input::State::HOLD) {
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

    void Player::m_UpdateInventory(World& world)
    {
        m_MainInventory.Update(quad->GetCenter(), world);

        Vivium::Input::State e = Vivium::Input::GetKeyState(GLFW_KEY_E);

        // Toggle inventory
        if (e == Vivium::Input::State::PRESS) { m_isInventoryOpened = !m_isInventoryOpened; }
    }

    void Player::m_UpdateSelectedTile(World& world)
    {
        // Convert cursor position to world position
        selected_tile_pos = world.GetWorldPos(Vivium::Input::GetCursorPos());
        selected_tile = world.GetTile(selected_tile_pos);
    }

    void Player::Update(World& world)
    {
        m_UpdateInventory(world);
        m_UpdateMovement();
        m_UpdateSelectedTile(world);
    }

    void Player::Render()
    {
        m_RenderSelectedTile();
        Vivium::Renderer::Submit(quad.get(), shader);
        m_RenderInventory();
    }

    Player::Player()
        : m_MainInventory(Inventory::ID::SMALL)
    {
        // Setup player quad and body
        quad = MakeRef(Vivium::Quad, 0.0f, 0.0f, 100.0f, 100.0f);
        body = MakeRef(Vivium::Body, quad, true, 0.0f, 10.0f);

        // Setup shader and uniforms
        shader = new Vivium::Shader("world_vertex", "color_frag");
        shader->SetUniform3f("u_Color", 1.0, 1.0, 0.0);

        m_GameIcons = new Vivium::TextureAtlas("game_icons.png", { 32, 32 });

        m_SelectedTileQuad = new Vivium::Quad(0.0f, 0.0f, World::PIXEL_SCALE, World::PIXEL_SCALE);
        m_SelectedTileShader = new Vivium::Shader("texture_vertex", "texture_frag");

        m_GameIcons->Set(m_SelectedTileQuad, 0); // Set texture from index in texture atlas

        Vivium::Application::physics->Register(body, 0);

        m_Time = Vivium::Timer::GetTime();
    }

    Player::~Player()
    {
        delete shader;
        delete m_SelectedTileQuad;
        delete m_SelectedTileShader;
        delete m_GameIcons;
    }
}
