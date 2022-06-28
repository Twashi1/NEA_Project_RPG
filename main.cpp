#include <iostream>
#include <string>
#include <fstream>
#include <chrono>

#include "Game.h"

/*
    // Example on how to generate a buffer of noise and create a texture out of it
    uint8_t* buffer = new uint8_t[512 * 512 * 4];

    Noise::Interpolated gen(0, 1, 100);

    for (int i = 0; i < 512 * 512; i++) {
        uint8_t light_value = gen.GetByteLinear(i);
        unsigned int start = i * 4;
        buffer[start] = light_value;
        buffer[start + 1] = light_value;
        buffer[start + 2] = light_value;
        buffer[start + 3] = 255;
    }

    std::shared_ptr<uint8_t[]> data(buffer);
    Texture noise_tex = Texture(data, 512, 512);
    noise_tex.Bind(0);
    texture_shader.SetUniform1i("u_Texture", 0);
*/

    // Shader uv_shader("res/shaders/uv_vertex.glsl", "res/shaders/uv_frag.glsl");
    // uv_shader.SetUniformMat4fv("u_projMat", proj);

    // Region* reg = game.world.GetRegion({ 0, 0 });
    // Texture tex = reg->ToTexture();

    // tex.Bind(0); // Bind the texture
    // region_shader.SetUniform1i("u_Texture", 0); // Tells the region shader where the texture its meant to use is

// TODO: rotation physics system
// TODO: separate physics collider from quad
// TODO: title screen
// TODO: world
// TODO: save files
// TODO: options and general config
// TODO: control config
// TODO: database link?
// TODO: pathfinding
// TODO: enemies/creatures
// TODO: inventory
// TODO: item system
// TODO: texture atlas api
// TODO: sprites/animations
// TODO: backgrounds
// TODO: ambient particles
// TODO: level editor?
// TODO: buttons/panels/gui
// TODO: figure out how post-processing works

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

// Checks all necessary diquadories exist
void CheckDirectories() {
    if (!Utilities::CheckDirectoryExists("res"))         Log("Resources folder not found; app not properly installed?", Utilities::ERROR::FATAL);
    if (!Utilities::CheckDirectoryExists("res/region"))  Log("Region folder not found; app not properly installed?",    Utilities::ERROR::FATAL);
    if (!Utilities::CheckDirectoryExists("res/shaders")) Log("Shaders folder not found; app not properly installed?",   Utilities::ERROR::FATAL);
}

int main(void)
{
    Log("Program starting", Utilities::ERROR::INFO);
    CheckDirectories();

    // Set shader statics
    Shader::PATH = "res/shaders/";
    Shader::EXTENSION = ".glsl";

    // Construct game instance
    Game game(WIDTH, HEIGHT, FPS, true);

    // DEBUG: quads
    Quad wall = Quad(500.0f, 500.0f, 100.0f, 500.0f, 0.25 * PI_CONST);
    Quad bg = Quad(0, 0, 1920, 1080, 0);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniformMat4fv("u_projMat", game.proj); // Set the projection matrix
    texture_shader.SetUniform1f("u_Scale", 5);

    Shader cellular_shader("cellular_vertex", "cellular_frag");
    cellular_shader.SetUniformMat4fv("u_projMat", game.proj);
    cellular_shader.SetUniform2f("u_Resolution", game.width, game.height);
    cellular_shader.SetUniform1f("u_Scale", 5);
    cellular_shader.SetUniform1ui("u_Seed", 0);

    Shader colour_shader = Shader("colour_vertex", "colour_frag");
    colour_shader.SetUniformMat4fv("u_projMat", game.proj);
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader bg_shader = Shader("colour_vertex", "colour_frag");
    bg_shader.SetUniformMat4fv("u_projMat", game.proj);
    bg_shader.SetUniform3f("u_Color", COLORS::BLUE * 0.5f);

    // Allow transparency
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // DEBUG: renderables
    Renderable walldraw(std::shared_ptr<Quad>(&wall), &colour_shader, 1);
    game.drawcalls.push_back(&walldraw);

    Renderable bg_render(std::shared_ptr<Quad>(&bg), &bg_shader, 0);
    game.drawcalls.push_back(&bg_render);

    // DEBUG: add to physics system
    Body wallbody = Body(&wall, true, 0.0f, 999);
    game.physics->layers[0].push_back(std::shared_ptr<Body>(&wallbody));

    // Loop until window is closed by user
    while (!glfwWindowShouldClose(game.window))
    {
        bg_render.quad->SetCenter(game.player->quad->GetCenter()); // Make background quad follow player
        game.Update(); // Update game
        
        // Rotate our example wall
        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = Utilities::ClampMax(wallbody.angular_vel, 3.0f);
    }

    // Close program
    glfwTerminate();
    return 0;
}