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

    int wl = 32;
    Vivium::Noise::Voronoi interp_noise(5, 1.0f, wl);
    std::size_t IMG_WIDTH = 1024;
    std::size_t IMG_HEIGHT = 1024;
    uint8_t* my_buffer = new uint8_t[IMG_WIDTH * IMG_HEIGHT];

    std::size_t IMG_WIDTH2 = IMG_WIDTH / wl;
    std::size_t IMG_HEIGHT2 = IMG_HEIGHT / wl;

    uint8_t* my_other_buffer = new uint8_t[IMG_WIDTH2 * IMG_HEIGHT2];

    for (std::size_t y = 0; y < IMG_HEIGHT; y++) {
        for (std::size_t x = 0; x < IMG_WIDTH; x++) {
            std::size_t idx = x + (y * IMG_WIDTH);

            float noise = interp_noise.Get(x, y).x;

            my_buffer[idx] = noise * 255;
        }
    }

    for (std::size_t y = 0; y < IMG_HEIGHT2; y++) {
        for (std::size_t x = 0; x < IMG_WIDTH2; x++) {
            std::size_t idx = x + (y * IMG_WIDTH2);

            float noise = Vivium::Random::GetFloat(0.0f, 1.0f);

            my_other_buffer[idx] = noise * 255;
        }
    }

    Vivium::Texture tex(my_buffer, IMG_WIDTH, IMG_HEIGHT, Vivium::Texture::Format::GRAYSCALE);
    Vivium::Texture tex2(my_other_buffer, IMG_WIDTH2, IMG_HEIGHT2, Vivium::Texture::Format::GRAYSCALE);
    Vivium::Shader tex_shader("static_texture_vertex", "texture_frag");
    Vivium::Quad quad(0.0f, 0.0f, IMG_WIDTH, IMG_HEIGHT);
    Vivium::Quad quad2(IMG_WIDTH + 100.0f, 0.0f, IMG_WIDTH2 * wl, IMG_HEIGHT2 * wl);

    while (Application::IsRunning()) {
        Application::BeginFrame();

        Renderer::Submit(&quad, &tex_shader, &tex);
        Renderer::Submit(&quad2, &tex_shader, &tex2);

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