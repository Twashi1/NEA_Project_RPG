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

#define XDIM 5
#define YDIM 4

    //#define OLD
#ifndef OLD
    bool* dat = new bool[XDIM * YDIM];

    for (int i = 0; i < XDIM * YDIM; i++) {
        dat[i] = false;
    }

    dat[1 + 1 * XDIM] = true;
    dat[2 + 1 * XDIM] = true;
    dat[3 + 1 * XDIM] = true;

    Pathfinding::Map map(dat, {XDIM, YDIM});
    auto path = Pathfinding::Calculate({ 1, 0 }, { 3, 3 }, map);

    for (auto& node : path.GetNodes()) {
        LogTrace("Going to position: {}", node.pos);
    }
#else
    Pathfinder::ObstacleMap_t map = Pathfinder::MakeObstaclePath(XDIM * YDIM);

    for (int i = 0; i < XDIM * YDIM; i++) {
        map[i] = false;
    }

    map[1 + 1 * XDIM] = true;
    map[2 + 1 * XDIM] = true;
    map[3 + 1 * XDIM] = true;

    Pathfinder finder({ 1, 0 }, { 3, 3 }, { XDIM, YDIM }, map);

    auto path = finder.Calculate();

    for (auto& node : path) {
        LogTrace("Going to node: {}", node.pos);
    }
#endif

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    Vivium::Flag::Set(VIVIUM_FLAG_FUNCTION_SIGNATURE_LOGGING, 1);
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", RGBColor::BLUE);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    TextureManager::Init();
    World::Init();
    WorldMap::Init();
    FloorItem::Init();
    Inventory::Init();
    CraftingInventory::Init();
    LeavesParticleSystem::Init();

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

        Physics::Update();

        // Draw calls
        main_menu.Render();

        Application::EndFrame();
    }

    LogInfo("Window closed");

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
}