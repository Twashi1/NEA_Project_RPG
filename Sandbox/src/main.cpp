#include <iostream>
#include <Engine.h>

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

/*
TODOs:

****** Engine ******
# Renderer
Texture atlas API
Sprites
Animations
GUI
Post-processing
Better API for rendering text

# General
Level editor
Buttons
Pathfinding
Ambient particles
Sound API
Physics System update (impulse resolution?)

****** Game ******
World
Save files
Control config
Options and general config
*/

int main(void)
{
    // Construct engine instance
    Engine engine(WIDTH, HEIGHT, FPS, true);

    // DEBUG: quads
    Quad wall = Quad(500.0f, 500.0f, 100.0f, 500.0f, 0.25 * PI_CONST);
    Quad bg = Quad(0, 0, 1920, 1080, 0);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniformMat4fv("u_projMat", engine.proj);
    texture_shader.SetUniform1f("u_Scale", 5);

    Shader cellular_shader("cellular_vertex", "cellular_frag");
    cellular_shader.SetUniformMat4fv("u_projMat", engine.proj);
    cellular_shader.SetUniform2f("u_Resolution", engine.width, engine.height);
    cellular_shader.SetUniform1f("u_Scale", 5);
    cellular_shader.SetUniform1ui("u_Seed", 0);

    Shader colour_shader = Shader("colour_vertex", "colour_frag");
    colour_shader.SetUniformMat4fv("u_projMat", engine.proj);
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader bg_shader = Shader("colour_vertex", "colour_frag");
    bg_shader.SetUniformMat4fv("u_projMat", engine.proj);
    bg_shader.SetUniform3f("u_Color", COLORS::BLUE * 0.5f);

    // DEBUG: renderables
    Renderable walldraw(std::shared_ptr<Quad>(&wall), &colour_shader, 1);
    Renderable bg_render(std::shared_ptr<Quad>(&bg), &bg_shader, 0);

    // DEBUG: add to physics system
    Body wallbody = Body(wall, true, 0.0f, 999);
    engine.physics.layers[0].push_back(std::shared_ptr<Body>(&wallbody));

    // Loop until window is closed by user
    while (engine.IsRunning())
    {
        bg_render.quad->SetCenter(engine.player->quad->GetCenter()); // Make background quad follow player
        engine.Update(); // Update engine

        // DEGUG: Rotate our example wall
        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = Utils::ClampMax(wallbody.angular_vel, 3.0f);
    }

    Log("Window closed", Utils::ERROR::INFO);

    engine.SerialiseGeneralData();

    Log("Ending program", Utils::ERROR::INFO);

    exit(EXIT_SUCCESS);
}