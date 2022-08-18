#include "World.h"
#include "Player.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Timer a;
    Timer b;

    a.Start();
    for (float i = 0.0f; i < 1000.0f; i += 0.1f) {
        float x = InverseSquareRoot(i);
    }
    float a_time = a.GetElapsed();

    b.Start();
    for (float i = 0.0f; i < 1000.0f; i += 0.1f) {
        float x = 1.0f / std::sqrt(i);
    }
    float b_time = b.GetElapsed();

    LogInfo("A took: {}, B took: {}", a_time, b_time);

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", RGBColor::BLUE);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    World::Init();
    FloorItem::Init();
    Inventory::Init();

    // Test construct world
    World world(0, "testworld");

    Player player = Player();

    Application::SetBGColor(RGBColor::BLACK);

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        Renderer::camera->SetCamera(
            player.quad->GetCenter(),
            { Application::width / 2.0f, Application::height / 2.0f },
            1.0f,
            0.0f
        );

        Application::BeginFrame();

        // Update GUI objects
        // ...

        // Update player
        player.Update(world);

        world.Update(&player);

        // Draw calls
        Vector2<int> update_pos = (Vector2<int>)(player.quad->GetCenter() / World::PIXEL_SCALE).floor();
        world.Render(update_pos);

        player.Render();

        // Renderer::DrawScene(Renderer::PHYSICS_DEBUG_SCENE);

        if (Application::isStatsEnabled) Application::UpdateStats(*player.body); // Draw stats information

        Application::EndFrame();
    }

    LogInfo("Window closed");

    FloorItem::Terminate();
    Inventory::Terminate();
    World::Terminate();
    Application::Terminate();

    LogInfo("Ending program");

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    game();
}