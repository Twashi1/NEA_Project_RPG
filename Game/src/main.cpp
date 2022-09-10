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

    auto obj1 = MakeRef(Vivium::Quad, 200.0f, 100.0f, 100.0f, 100.0f, 0.5f);
    auto obj2 = MakeRef(Vivium::Quad, 800.0f, 200.0f, 100.0f, 100.0f);

    Vivium::Shader color = Vivium::Shader("static_vertex", "color_frag"); color.SetUniform3f("u_Color", RGBColor::RED);
    Vivium::Shader color2 = Vivium::Shader("static_vertex", "color_frag"); color.SetUniform3f("u_Color", RGBColor::GREEN);

    auto obj1b = MakeRef(Vivium::Body, obj1, false, 1.0f, 8.0f);
    auto obj2b = MakeRef(Vivium::Body, obj2, false, 1.0f, 2.0f);

    auto layer = Physics::CreateLayer(5, { 5 });

    layer->bodies.push_back(obj1b);
    layer->bodies.push_back(obj2b);

    obj2b->vel.x = -100.0f;
    obj1b->vel.x = 100.0f;

    while (Application::IsRunning()) {
        Application::BeginFrame();

        Renderer::Submit(obj1.get(), &color);
        Renderer::Submit(obj2.get(), &color2);

        obj1b->Update();
        obj2b->Update();

        Application::EndFrame();
    }

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