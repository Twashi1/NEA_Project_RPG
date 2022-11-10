#include "World.h"
#include "Player.h"
#include "MainMenu.h"
#include "LeafParticles.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS);

    Application::SetBGColor(RGBColor::BLUE);

    Shader water_shader("static_texture_vertex", "healthbar_frag");

    Quad quad(150.0f, 50.0f, 300.0f, 100.0f);
    TextureManager::Init();

    while (Application::IsRunning())
    {
        Application::BeginFrame();

        Vivium::Renderer::Submit(&quad, &water_shader);

        Application::EndFrame();
    }

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    // TODO: use application path
    std::filesystem::create_directory("../Resources/saves/");

    Vivium::Flag::Set(VIVIUM_FLAG_FUNCTION_SIGNATURE_LOGGING, 1);
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS);

#ifdef CREATE_SHADERS
    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", RGBColor::BLUE);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");
#endif

    TextureManager::Init();
    World::Init();
    WorldMap::Init();
    FloorItem::Init();
    Inventory::Init();
    CraftingInventory::Init();
    LeavesParticleSystem::Init();
    Weapon::Init();
    ProjectileSystem::Init();
    Game::NPC::Init();

    Vivium::Animator::Data slime_data = Vivium::Animator::Data(
        {
            {0.5f, {0, 5}},
            {0.5f, {1, 5}},
            {0.5f, {2, 5}},
            {0.5f, {3, 5}},
            {0.8f, {4, 5}},
            {0.5f, {5, 5}},
            {0.5f, {6, 5}}
        }, TextureManager::game_atlas
    );

    // TODO: really need something better than this
    dynamic_pointer_cast<Game::Behaviours::SlimeAttack>(Game::NPC::m_Properties.at((uint8_t)Game::NPC::ID::SLIME).behaviours.at(0))->global.anim_data = slime_data;

    Application::SetBGColor(RGBColor::BLACK);

    MainMenu main_menu{};

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        Application::BeginFrame();

        main_menu.Update();
        main_menu.Render();

        Application::EndFrame();
    }

    

    LogInfo("Window closed");

    Weapon::Terminate();
    CraftingInventory::Terminate();
    Biome::Terminate();
    Inventory::Terminate();
    FloorItem::Terminate();
    WorldMap::Terminate();
    World::Terminate();
    Application::Terminate();

    LogInfo("Ending program");

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    sandbox();

    LogTrace("Game finished");
}