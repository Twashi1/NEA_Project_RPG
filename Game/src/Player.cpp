#include "Player.h"

namespace Game {
    const Vivium::Vector2<int> Player::s_HEAD_WALK_0 = {0, 12};
    const Vivium::Vector2<int> Player::s_FEET_WALK_0 = {0, 13};

    void Player::s_ContinueButtonCallback(Vivium::Button* button, void* user_params)
    {
        // Cast user params to player to call callback function on correct object
        Player* player = (Player*)user_params;
        player->m_ContinueButtonCallback(button);
    }

    void Player::m_ContinueButtonCallback(Vivium::Button* button)
    {
        // Reset various values
        health.value = 100.0f;
        health.hasDied = false;
        m_isDeathScreenVisible = false;

        // Teleport player some distance away
        float displacement = 8000.0f;
        Vivium::Vector2<int> move_vector = Vivium::Random::GetVector2f(displacement);

        body->quad->SetCenter(body->quad->GetCenter() + move_vector);
    }

    void Player::m_RenderPlayer()
    {
        Vivium::Renderer::Submit(quad.get(), m_PlayerShader.get(), TextureManager::game_atlas->GetAtlas().get());
    }

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

        // Slightly offset from top left corner
        *m_MainInventory.inventory_pos = { 50.0f, -50.0f };

        // If the entire inventory is open
        if (m_isMainInventoryOpened) {
            // Render both the entire inventory and crafting inventory
            m_MainInventory.Render();
            m_CraftingInventory.Render();
        }
        else {
            // Otherwise render the first 9 slots of the main inventory (hotbar)
            m_MainInventory.Render(Inventory::Slot::INV_0, 9);
        }
    }

    void Player::m_RenderHealthbar()
    {
        static const Vivium::BufferLayout healthbar_layout = {
            Vivium::GLSLDataType::VEC2,
            Vivium::GLSLDataType::VEC2,
            Vivium::GLSLDataType::VEC2,
            Vivium::GLSLDataType::VEC2,
            Vivium::GLSLDataType::FLOAT
        };

        // Submit npc healthbar
        // If we're not at 100% health
        if (health.GetNormalised() < 1.0f) {
            // TODO: precompute tex coords
            static const std::array<float, 8> default_tex = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
            };

            std::array<float, 8> healthbar_tex = TextureManager::game_atlas->GetCoordsArray({ 7, 9 }, { 9, 9 });
            std::array<float, 8> mask_tex = TextureManager::game_atlas->GetCoordsArray({ 7, 8 }, { 9, 8 });

            // TODO: shouldn't be filling this up ourselves
            // TODO: shouldn't be calculating this ourselves as well
            std::size_t size = (2 + 2 + 2 + 2 + 1) * 4;
            float* vertex_data = new float[size];

            // TODO: bad place for these variables as well
            const float healthbar_width = 96.0f;
            const float healthbar_height = 32.0f;
            const float healthbar_scale = 2.0f;

            Vivium::Vector2<float> pos = body->quad->GetCenter();
            pos.y += 100.0f; // Display a bit above

            float halfwidth = healthbar_width * 0.5f * healthbar_scale;
            float halfheight = healthbar_height * 0.5f * healthbar_scale;

            float left = pos.x - halfwidth;
            float right = pos.x + halfwidth;
            float bottom = pos.y - halfheight;
            float top = pos.y + halfheight;

            float vertices[8] = {
                left, bottom,
                right, bottom,
                right, top,
                left, top
            };

            int cindex = 0;
            for (int i = 0; i < 4; i++) {
                vertex_data[cindex++] = vertices[i * 2];
                vertex_data[cindex++] = vertices[i * 2 + 1];

                vertex_data[cindex++] = default_tex[i * 2];
                vertex_data[cindex++] = default_tex[i * 2 + 1];

                vertex_data[cindex++] = healthbar_tex[i * 2];
                vertex_data[cindex++] = healthbar_tex[i * 2 + 1];

                vertex_data[cindex++] = mask_tex[i * 2];
                vertex_data[cindex++] = mask_tex[i * 2 + 1];

                vertex_data[cindex++] = health.GetNormalised();
            }

            // TODO: store vb somewhere and just edit data
            Vivium::VertexBuffer vb(vertex_data, 36, healthbar_layout);
            
            Vivium::Renderer::Submit(&vb, Vivium::Quad::GetIndexBuffer(), m_HealthbarShader.get(), TextureManager::game_atlas->GetAtlas().get());
        }
    }

    void Player::m_RenderDeathScene()
    {
        m_YouHaveDiedText->Render();
        Vivium::Renderer::Submit(m_ContinueButton.get());
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
        // TODO: body->Update() instead?
        body->vel = body->vel + (body->acc * elapsed);
    }

    void Player::m_UpdateSelectedSlot()
    {
        m_SelectedSlot = m_MainInventory.GetSelectedSlot();
        m_SelectedItem = m_MainInventory.GetItem(m_SelectedSlot);

        for (int i = GLFW_KEY_1; i <= GLFW_KEY_9; i++) {
            Vivium::Input::State state = Vivium::Input::GetKeyState(i);

            if (state == Vivium::Input::State::RELEASE) {
                m_SelectedSlot = (Inventory::Slot)((Inventory::slot_base_t)Inventory::Slot::INV_0 + i - GLFW_KEY_1);
                m_MainInventory.SetSelectedSlot(m_SelectedSlot);
            }
        }

        if (Item::GetIsHandEquipable(m_SelectedItem.id)) {
            if (m_HandEquipable == nullptr) {
                m_HandEquipable = HandEquipable::CreateInstance(m_SelectedItem.id);
            }
            else if (m_HandEquipable->id != m_SelectedItem.id) {
                m_HandEquipable = HandEquipable::CreateInstance(m_SelectedItem.id);
            }
        }
        else {
            m_HandEquipable = nullptr;
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

    void Player::m_UpdateDeathScene()
    {
        if (health.hasDied) {
            m_isDeathScreenVisible = true;
        }
        
        if (m_isDeathScreenVisible) {
            m_ContinueButton->Update();
        }
    }

    void Player::Update(World& world)
    {
        m_UpdateDeathScene();
        health.Update();

        if (!m_isDeathScreenVisible) {
            body->Update();
            m_UpdateSelectedSlot();
            m_UpdateInventory(world);
            m_UpdateMovement();
            m_UpdateSelectedTile(world);

            if (m_HandEquipable != nullptr) {
                m_HandEquipable->Update(&world, this);
            }
        }
    }

    void Player::Render()
    {
        // TODO: healthbar rendering
        m_RenderSelectedTile();
        m_RenderPlayer();
        m_RenderInventory();
        m_RenderHealthbar();

        if (m_HandEquipable != nullptr) {
            m_HandEquipable->Render();
        }

        if (m_isDeathScreenVisible) {
            m_RenderDeathScene();
        }
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

    const Inventory& Player::GetMainInventory() const {
        return m_MainInventory;
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
        : m_MainInventory(Inventory::ID::SMALL_WITH_HOTBAR), m_CraftingInventory(), health()
    {
        // Setup player quad and body
        quad = std::make_shared<Vivium::Quad>(0.0f, 0.0f, 64.0f, 128.0f);
        // TODO: set appropriate values later
        body = std::make_shared<Vivium::Body>(quad, true, 0.0f, 1.0f);

        m_PlayerShader = std::make_unique<Vivium::Shader>("texture_vertex", "texture_frag");

        m_SelectedTileQuad = new Vivium::Quad(0.0f, 0.0f, World::PIXEL_SCALE, World::PIXEL_SCALE);
        m_SelectedTileShader = new Vivium::Shader("texture_vertex", "texture_frag");

        TextureManager::game_atlas->Set(m_SelectedTileQuad, {2, 1}); // Set texture from index in texture atlas

        *m_CraftingInventory.inventory_pos = { 50.0f, 200.0f }; // TODO: fix to panel

        TextureManager::game_atlas->Set(quad.get(), s_HEAD_WALK_0, s_FEET_WALK_0);

        m_HealthbarShader = std::make_unique<Vivium::Shader>("healthbar_vertex", "healthbar_frag");
        m_HealthbarShader->Bind();
        m_HealthbarShader->SetUniform1f("u_WaveOffset", 0.0f);

        m_YouHaveDiedText = std::make_shared<Vivium::Text>("You have died!", 0.0f, Vivium::Text::Alignment::CENTER, 0.75f);
        m_YouHaveDiedText->shader->Bind();
        m_YouHaveDiedText->shader->SetUniform3f("u_TextColor", Vivium::RGBColor::RED);

        // Hope to god this isn't reallocated somewhere
        m_ContinueButton = std::make_shared<Vivium::Button>(Vivium::Quad(0.0f, -100.0f, 128.0f, 64.0f), Player::s_ContinueButtonCallback, "Continue", this);

        Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_YouHaveDiedText);
        Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::CENTER, Vivium::Panel::ANCHOR::CENTER, m_ContinueButton);

        m_Time = Vivium::Timer::GetTime();
    }

    Player::~Player()
    {
        delete m_SelectedTileQuad;
        delete m_SelectedTileShader;
    }
}
