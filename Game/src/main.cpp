#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

int sandbox(void)
{
    using namespace Vivium;

    return EXIT_SUCCESS;
}

int game(void)
{
    using namespace Vivium;

    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", 1.0, 0.0, 0.0);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    World world(0, "testworld");
    // World::LoadTextures("tile_atlas.png");

    Player player = Player();
    Application::physics->Register(player.body, 0);

    Application::SetBGColor(RGBColor::BLUE);

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

        // Draw calls
        Renderer::BeginScene(1);
        Vector2<int> update_pos = (Vector2<int>)(player.quad->GetCenter() / World::scale).floor();
        world.Render(update_pos);
        Renderer::EndScene();

        Renderer::BeginScene(5);
        Renderer::Submit(player.quad.get(), player.shader);
        Renderer::EndScene();

        Renderer::BeginScene(2);
        if (Application::isStatsEnabled) Application::UpdateStats(*player.body); // Draw stats information
        Renderer::EndScene();

        Renderer::DrawScenes();

        Application::EndFrame();
    }

    LogInfo("Window closed");

    Application::Terminate();

    LogInfo("Ending program");

    return EXIT_SUCCESS;
}

int main(void) {
    game();
}