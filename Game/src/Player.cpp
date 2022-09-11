#include "Player.h"

namespace Game {
    void Player::m_RenderSelectedTile()
    {
        if (selected_tile.background != Tile::ID::VOID) {
            m_SelectedTileQuad->SetCenter(selected_tile_pos * World::PIXEL_SCALE);
            Vivium::Renderer::Submit(m_SelectedTileQuad, m_SelectedTileShader, TextureManager::game_atlas->GetAtlas().get());
        }
    }

    void Player::m_RenderInventory()
    {
        Vivium::Vector2<float> screen_dim = Vivium::Application::GetScreenDim();

        *m_MainInventory.inventory_pos = { 50.0f, -50.0f };

        if (m_isMainInventoryOpened) {
            m_MainInventory.Render();
            m_CraftingInventory.Render();
        }
        else {
            m_MainInventory.Render(Inventory::Slot::INV_0, 9);
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

        body->acc = 0.0f;

        // If W key pressed
        if (Vivium::Input::IsKeyDown(w)) {
            body->acc.y += MAXACCEL;
        }
        // If S key pressed
        if (Vivium::Input::IsKeyDown(s)) {
            body->acc.y -= MAXACCEL;
        }
        // If D key pressed
        if (Vivium::Input::IsKeyDown(d)) {
            body->acc.x += MAXACCEL;
        }
        // If A key pressed
        if (Vivium::Input::IsKeyDown(a)) {
            body->acc.x -= MAXACCEL;
        }

        body->acc -= body->vel * FRICTION;
        body->vel = body->vel + (body->acc * elapsed);
    }

    void Player::m_UpdateSelectedSlot()
    {
        m_SelectedSlot = m_MainInventory.GetSelectedSlot();

        for (int i = GLFW_KEY_1; i <= GLFW_KEY_9; i++) {
            Vivium::Input::State state = Vivium::Input::GetKeyState(i);

            if (state == Vivium::Input::State::RELEASE) {
                m_SelectedSlot = (Inventory::Slot)((Inventory::slot_base_t)Inventory::Slot::INV_0 + i - GLFW_KEY_1);
                m_MainInventory.SetSelectedSlot(m_SelectedSlot);
                m_SelectedItem = m_MainInventory.GetItem(m_SelectedSlot);
            }
        }
    }

    void Player::m_UpdateInventory(World& world)
    {
        // Update main inventory
        m_MainInventory.Update(quad->GetCenter(), &world);

        Vivium::Input::State e = Vivium::Input::GetKeyState(GLFW_KEY_E);

        // Toggle inventory
        if (e == Vivium::Input::State::PRESS) {
            m_isMainInventoryOpened = !m_isMainInventoryOpened;
        }

        m_CraftingInventory.Update(&m_MainInventory);
    }

    void Player::m_UpdateSelectedTile(World& world)
    {
        // Convert cursor position to world position
        selected_tile_pos = world.GetWorldPos(Vivium::Input::GetCursorPos());
        selected_tile = world.GetTile(selected_tile_pos);
    }

    void Player::Update(World& world)
    {
        body->Update();

        m_UpdateSelectedSlot();
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

    void Player::Write(Vivium::Serialiser& s) const
    {
        s.Write(m_MainInventory);
        s.Write(quad->GetCenter());
    }

    void Player::Read(Vivium::Serialiser& s)
    {
        s.Read(&m_MainInventory);
        Vivium::Vector2<float> player_pos;
        s.Read(&player_pos);

        quad->SetCenter(player_pos);
    }

    void Player::Save(World& world)
    {
        Vivium::Serialiser s(Vivium::Stream::Flags::BINARY | Vivium::Stream::Flags::TRUNC);

        std::string full_path = std::format("{}{}/{}{}", World::PATH, world.GetName(), "player_data", World::FILE_EXTENSION);

        s.BeginWrite(full_path.c_str());
        Write(s);
        s.EndWrite();
    }

    Player::Player(const std::string& world_name)
        : Player() // Call default player constructor
    {
        Vivium::Serialiser s(Vivium::Stream::Flags::BINARY | Vivium::Stream::Flags::TRUNC);

        std::string full_path = std::format("{}{}/{}{}", World::PATH, world_name, "player_data", World::FILE_EXTENSION);

        s.BeginRead(full_path.c_str());
        Read(s);
        s.EndRead();
    }

    Player::Player()
        : m_MainInventory(Inventory::ID::SMALL_WITH_HOTBAR), m_CraftingInventory()
    {
        // Setup player quad and body
        quad = MakeRef(Vivium::Quad, 0.0f, 0.0f, 50.0f, 50.0f);
        // TODO: fix later
        body = MakeRef(Vivium::Body, quad, true, 0.0f, 1.0f);

        // Setup shader and uniforms
        shader = new Vivium::Shader("world_vertex", "color_frag");
        shader->SetUniform3f("u_Color", 1.0, 1.0, 0.0);

        m_SelectedTileQuad = new Vivium::Quad(0.0f, 0.0f, World::PIXEL_SCALE, World::PIXEL_SCALE);
        m_SelectedTileShader = new Vivium::Shader("texture_vertex", "texture_frag");

        TextureManager::game_atlas->Set(m_SelectedTileQuad, {2, 1}); // Set texture from index in texture atlas

        *m_CraftingInventory.inventory_pos = { 50.0f, 200.0f }; // TODO: fix to panel

        m_Time = Vivium::Timer::GetTime();
    }

    Player::~Player()
    {
        delete shader;
        delete m_SelectedTileQuad;
        delete m_SelectedTileShader;
    }
}
