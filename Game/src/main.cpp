#include "World.h"
#include "Player.h"
#include "SceneManager.h"
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

    WeightMap<char> map({
        {1.0f, 'a'},
        {1.0f, 'b'},
        {1.0f, 'c'},
        {1.0f, 'd'},
        {1.0f, 'e'},
        {1.0f, 'f'},
        {1.0f, 'g'},
        {1.0f, 'h'},
        {1.0f, 'i'},
        {1.0f, 'j'},
        {1.0f, 'k'},
        {1.0f, 'l'}
    });

    std::map<char, int> frequency = {
        {'a', 0},
        {'b', 0},
        {'c', 0},
        {'d', 0},
        {'e', 0},
        {'f', 0},
        {'g', 0},
        {'h', 0},
        {'i', 0},
        {'j', 0},
        {'k', 0},
        {'l', 0}
    };

    Timer timer;
    
    float elapsed = 0.0f;

    timer.Start();

    // Million iterations
    for (int i = 0; i < 1000000; i++) {
        char result = map.Get();
        ++frequency[result];
    }

    elapsed = timer.GetElapsed();

    LogTrace("Linear took {}s, Results: {}", elapsed, Logger::PrettyPrint(frequency));

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    // TODO: use application path
    // Creating saves directory
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

    // Initialisers
    TextureManager::Init();
    World::Init();
    WorldMap::Init();
    FloorItem::Init();
    Inventory::Init();
    CraftingInventory::Init();
    LeavesParticleSystem::Init();
    Weapon::Init();
    ToolEquipable::Init();
    ProjectileSystem::Init();
    NPC::Init();

    // TODO: really bad
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

    // TODO: even worse
    dynamic_pointer_cast<Behaviours::SlimeAttack>(NPC::m_Properties.at((uint8_t)NPC::ID::SLIME).behaviours.at(0))->global.anim_data = slime_data;

    Application::SetBGColor(RGBColor::BLACK);

    // Create instance of scene manager
    SceneManager main_menu{};

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        // Start of frame operations
        Application::BeginFrame();

        // Update and render current scene
        main_menu.Update();
        main_menu.Render();

        // End of frame operations
        Application::EndFrame();
    }

    LogInfo("Window closed");

    // Cleanup
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