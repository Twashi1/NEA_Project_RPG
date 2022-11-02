#include "World.h"
#include "Player.h"
#include "MainMenu.h"
#include "LeafParticles.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

struct abc : Vivium::Streamable {
    int x = 0;

    abc() = default;
    abc(int x) : x(x) {}

    void Write(Vivium::Serialiser&) const override { LogTrace("ABC written"); }
    void Read(Vivium::Serialiser&) override {}
};

struct inv { int y = 3; inv(int y) : y(y) {} };

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS);

    Application::SetBGColor(RGBColor::BLUE);

    Vivium::Serialiser s(Vivium::Stream::Flags::BINARY);
    std::pair<int, float> example = {0, 1.0f};
    std::pair<Vector2<int>, Vector2<float>> example_2 = {0, 0.0f};
    std::pair<int, abc> example_3 = { 1, 0 };
    std::vector<std::pair<int, abc>> example_4 = {};

    s.BeginWrite("../Resources/new.txt");
    s.Write(example);
    s.Write(example_2);
    s.Write(example_3);
    s.Write(example_4);
    s.EndWrite();

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

    Application::SetBGColor(RGBColor::BLACK);

    MainMenu main_menu{};

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        Application::BeginFrame();

        // Update GUI objects
        // ...

        // Update title screen
        main_menu.Update();

        // Draw calls
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