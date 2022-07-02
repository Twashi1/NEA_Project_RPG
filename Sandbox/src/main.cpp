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
GUI
Post-processing

# General
Pathfinding
Ambient particles
Sound API
Physics System update (impulse resolution?)

****** Game ******
World
Save files
Control config
Options and general config

****** Other ******
Engine documentation
Fix inconsistent styling
*/

/*
Bugs
- Some textures load in with a line of pixels on the left hand side from texture atlas api
*/

void tester_callback(Button* button_pressed) {
    static int times_pressed = 0;

    times_pressed++;
    std::string text = std::format("Clicked: {}", to_string(times_pressed));
    button_pressed->default_text = text;

    Log(text, Utils::ERROR::INFO);
}

int main(void)
{
    // Construct engine instance
    Engine engine(WIDTH, HEIGHT, FPS, true);

    // DEBUG: quads
    Quad wall = Quad(500, 500, 100, 500, 0.25 * PI_CONST);
    Quad bg = Quad(0, 0, 1920, 1080, 0);
    Quad btn_quad = Quad(100, 100, 100, 50, 0);
    Quad dummy = Quad(-100, -100, 64, 64, 0);
    Quad noisequad = Quad(-500, 500, 256, 256, 0);

    // DEBUG: buttons
    Button btn = Button(btn_quad, &tester_callback, "Clicked: 0", "Pressed");

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniformMat4fv("u_projMat", engine.proj);
    texture_shader.SetUniform1f("u_Scale", 1);

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

    // DEBUG: atlas test
    Texture atlas_test = Texture("atlas.png"); // Create texture

    // DEBUG: noise test
    const int SIZE = 256;
    Noise::Interpolated interp(0, 1.0f, 64);
    std::uint8_t* buffer2 = new std::uint8_t[SIZE * SIZE * 4];
    for (int i = 0; i < SIZE * SIZE; i++) {
        int index = i * 4;
        int y = i / SIZE; int x = i - (y * SIZE);

        uint8_t v = interp.GetFractal(x, y, 4) * 0xff;

        buffer2[index] = v;
        buffer2[index + 1] = v;
        buffer2[index + 2] = v;
        buffer2[index + 3] = 0xff;
    }

    Texture noisetext = Texture(std::shared_ptr <uint8_t[]>(buffer2), SIZE, SIZE);

    Renderable noisedraw = Renderable(std::shared_ptr<Quad>(&noisequad), &texture_shader, &noisetext, 5);

    // DEBUG: renderables
    Renderable walldraw(std::shared_ptr<Quad>(&wall), &colour_shader, 1);
    Renderable bg_render(std::shared_ptr<Quad>(&bg), &bg_shader, 0);
    Renderable dummydraw(std::shared_ptr<Quad>(&dummy), &texture_shader, &atlas_test, 2);

    // DEBUG: Animation object
    Animation animation(std::shared_ptr<Renderable>(&dummydraw), std::shared_ptr<Texture>(&atlas_test), { 64, 64 }, "data");

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

    // TODO: still have to forcibly exit since deconstructors aren't being called properly
    // maybe something to do with order?
    exit(EXIT_SUCCESS);
}