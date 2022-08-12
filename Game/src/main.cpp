#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Ref(Text) my_text = MakeRef(Text, "Hello world", Vivium::Vector2<float>{ 15, 150 }, 0.25f);
    Ref(Quad) panel_quad = MakeRef(Quad, 100, 100, 500, 500);
    Panel my_panel(panel_quad);

    my_panel.Anchor(Panel::ANCHOR::LEFT, Panel::ANCHOR::TOP, my_text);

    while (Application::IsRunning()) {
        my_panel.Update();

        Application::BeginFrame();
        Renderer::Submit(my_text.get());
        Application::EndFrame();
    }

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", 1.0, 0.0, 0.0);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    FloorItem::Init();
    Inventory::Init();

    // Test construct world
    World world(0, "testworld");

    Player player = Player();
    Application::physics->Register(player.body, 0);

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

        if (Application::isStatsEnabled) Application::UpdateStats(*player.body); // Draw stats information

        Application::EndFrame();
    }

    LogInfo("Window closed");

    FloorItem::Terminate();
    Inventory::Terminate();
    Application::Terminate();

    LogInfo("Ending program");

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    game();
}