#include "Player.h"

namespace Game {
    void Player::m_RenderSelectedTile()
    {
        if (selected_tile.bot != Tile::ID::VOID) {
            m_SelectedTileQuad->SetCenter(selected_tile_pos * World::PIXEL_SCALE);
            Vivium::Renderer::Submit(m_SelectedTileQuad, m_SelectedTileShader, TextureManager::game_atlas->GetAtlas().get());
        }
    }

    void Player::m_RenderInventory()
    {
        Vivium::Vector2<float> screen_dim = Vivium::Application::GetScreenDim();

        if (m_isMainInventoryOpened) {
            m_MainInventory.SetPos(screen_dim * 0.5f);
            m_MainInventory.Render();
        }
        else {
            m_HotbarInventory.SetPos({screen_dim.x * 0.5f, 100.0f});
            m_HotbarInventory.Render();
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
        // Update main inventory
        m_MainInventory.Update(quad->GetCenter(), world);
        // Copy any changes to main inventory to hotbar inventory
        std::vector<Item> hotbar_items = m_MainInventory.GetItems(Inventory::Slot::INV_0, 9);
        m_HotbarInventory.SetItems(hotbar_items, Inventory::Slot::INV_0);

        Vivium::Input::State e = Vivium::Input::GetKeyState(GLFW_KEY_E);

        // Toggle inventory
        if (e == Vivium::Input::State::PRESS) {
            // Switching from hotbar inventory to main inventory
            if (!m_isMainInventoryOpened) {
                // So copy items from hotbar inventory into main inventory
                std::vector<Item> hotbar_items = m_HotbarInventory.GetItems(Inventory::Slot::INV_0, 9);
                m_MainInventory.SetItems(hotbar_items, Inventory::Slot::INV_0);
            }

            m_isMainInventoryOpened = !m_isMainInventoryOpened;
        }
    }

    void Player::m_UpdateSelectedTile(World& world)
    {
        // Convert cursor position to world position
        selected_tile_pos = world.GetWorldPos(Vivium::Input::GetCursorPos());
        selected_tile = world.GetTile(selected_tile_pos);
    }

    void Player::Update(World& world)
    {
        // TODO: VERY TEMPORARY
        Vivium::Input::State o = Vivium::Input::GetKeyState(GLFW_KEY_0);

        if (o == Vivium::Input::State::PRESS) {
            auto recipe = Recipe::GetRecipe(Recipe::ID::AMETHYST_PICKAXE);

            std::vector<Item> items = recipe.CraftMaxFromInventory(m_MainInventory, 1);
            m_MainInventory.AddItems(items);
        }

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
        : m_MainInventory(Inventory::ID::SMALL_WITH_HOTBAR), m_HotbarInventory(Inventory::ID::HOTBAR)
    {
        // Setup player quad and body
        quad = MakeRef(Vivium::Quad, 0.0f, 0.0f, 50.0f, 50.0f);
        body = MakeRef(Vivium::Body, quad, true, 0.0f, 10.0f);

        // Setup shader and uniforms
        shader = new Vivium::Shader("world_vertex", "color_frag");
        shader->SetUniform3f("u_Color", 1.0, 1.0, 0.0);

        m_SelectedTileQuad = new Vivium::Quad(0.0f, 0.0f, World::PIXEL_SCALE, World::PIXEL_SCALE);
        m_SelectedTileShader = new Vivium::Shader("texture_vertex", "texture_frag");

        TextureManager::game_atlas->Set(m_SelectedTileQuad, {2, 1}); // Set texture from index in texture atlas

        m_Time = Vivium::Timer::GetTime();
    }

    Player::~Player()
    {
        delete shader;
        delete m_SelectedTileQuad;
        delete m_SelectedTileShader;
    }
}
