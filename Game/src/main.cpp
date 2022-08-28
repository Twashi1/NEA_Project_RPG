#include "World.h"
#include "Player.h"
#include "MainMenu.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Text my_text("abcdefgh\nhijk\nlmnop", Vivium::Vector2<float>{200.0f, 200.0f}, Text::Alignment::CENTER, 0.25f);

    Application::SetBGColor(RGBColor::BLUE);

    while (Application::IsRunning()) {
        Application::BeginFrame();

        my_text.Render();

        *my_text.pos = Input::GetCursorPos();

        Application::EndFrame();
    }

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    Vivium::Flag::Set(VIVIUM_FLAG_FUNCTION_SIGNATURE_LOGGING, false);
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", RGBColor::BLUE);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    World::Init();
    WorldMap::Init();
    FloorItem::Init();
    Inventory::Init();

    Application::SetBGColor(RGBColor::BLACK);

    // Flag::Set(VIVIUM_FLAG_DRAW_PHYSICS_CIRCLES);

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
    game();
}