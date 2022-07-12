#include <iostream>

#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

// For tester_callback
static int seed = 0;
static Noise::Interpolated* i = nullptr;
static bool seedChanged = false;

void tester_callback(Button* button_pressed) {
    seed++;
    i->SetSeed(seed);
    seedChanged = true;
    Log(std::format("Changing seed to {}", seed), LOG::INFO);
}

void text_callback(TextInput* text_input) {
    Log(std::format("Submitted text: {}", text_input->typed_text), LOG::INFO);
}

int main(void)
{
    // Construct engine instance
    Engine engine(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Shader colour_shader = Shader("colour_vertex", "colour_frag");
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader static_color = Shader("button_vertex", "button_frag");
    static_color.SetUniform4f("u_Color", COLORS::BLUE.x, COLORS::BLUE.y, COLORS::BLUE.z, 1.0f);

    World::texture_shader = &texture_shader;

    // Test construct world
    Log("Loading world...", LOG::INFO);
    World world(0, "testworld");
    Log("Loading textures...", LOG::INFO);
    World::LoadTextures("tile_atlas.png");
    Log("Updating world...", LOG::INFO);
    world.Update(Vector2<int>(0, 0));
    Log("Finished initialising world", LOG::INFO);

    // DEBUG: quads
    Quad wall = Quad(500, 500, 100, 500, 0.5 * PI_CONST);
    Quad btn_quad = Quad(-400, -300, 150, 50, 0);
    Quad dummy = Quad(-100, -100, 150, 150, 0);
    Quad noisequad = Quad(-500, 500, 256, 256, 0);
    Quad textbox = Quad(300, 300, 200, 100, 0);

    // DEBUG: buttons
    Button btn = Button(btn_quad, &tester_callback, "Change seed", "Change seed");

    // DEBUG: atlas test
    Texture atlas_test = Texture("atlas.png"); // Create texture

    // DEBUG: noise test
    const int SIZE = 256;
    Noise::Interpolated interp(seed, 1.0f, 64);
    i = &interp;
    std::uint8_t* buffer = new std::uint8_t[SIZE * SIZE * 4];
    for (int i = 0; i < SIZE * SIZE; i++) {
        int index = i * 4;
        int y = i / SIZE; int x = i - (y * SIZE);

        uint8_t v = interp.GetFractal(x, y, 4) * 0xff;

        buffer[index] = v;
        buffer[index + 1] = v;
        buffer[index + 2] = v;
        buffer[index + 3] = 0xff;
    }

    Texture noisetext = Texture(std::shared_ptr<uint8_t[]>(buffer), SIZE, SIZE);

    // DEBUG: Animation object
    Animation animation(std::shared_ptr<Quad>(&dummy), std::shared_ptr<Shader>(&texture_shader), std::shared_ptr<Texture>(&atlas_test), {64, 64}, "data");

    Player player = Player(engine.proj);
    engine.physics.Register(player.body, 0);

    // DEBUG: add to physics system
    Body wallbody = Body(wall, true, 0.0f, 999);
    engine.physics.Register(std::shared_ptr<Body>(&wallbody), 0);

    TextInput text_input(textbox, &text_callback);

    ShaderManager::UpdateProjectionMatrix(engine.proj);

    // Loop until window is closed by user
    while (engine.IsRunning())
    {
        ShaderManager::UpdateProjectionMatrix(engine.proj);

        if (seedChanged) {
            seedChanged = false;

            for (int i = 0; i < SIZE * SIZE; i++) {
                int index = i * 4;
                int y = i / SIZE; int x = i - (y * SIZE);

                uint8_t v = interp.GetFractal(x, y, 4) * 0xff;

                buffer[index] = v;
                buffer[index + 1] = v;
                buffer[index + 2] = v;
                buffer[index + 3] = 0xff;
            }

            noisetext.Update();
        }

        // TODO better API for this? client has to do too much

        engine.camera.pos = player.quad.GetCenter(); // Update camera
        engine.BeginFrame();

        player.Update(); // Update player

        // Draw calls
        Vector2<int> update_pos = (player.quad.GetCenter() / World::scale).floor();
        
        world.Update(update_pos);
        Renderer::Schedule(&wall, &colour_shader);
        Renderer::Schedule(&noisequad, &texture_shader, &noisetext);
        Renderer::Schedule(&animation);
        Renderer::Schedule(&btn);
        Renderer::Schedule(&text_input);
        Renderer::Schedule(&player.quad, player.shader);

        if (engine.enable_stats) engine.UpdateStats(*player.body); // Draw stats information
        
        engine.EndFrame();

        // DEGUG: Rotate our example wall
        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = std::min(wallbody.angular_vel, 3.0f);

        // TODO find better way to update quads which have a physics object attached
        wall.SetAngle(wall.GetAngle());
    }

    Log("Window closed", LOG::INFO);

    engine.SerialiseGeneralData();

    Log("Ending program", LOG::INFO);

    // TODO: still have to forcibly exit since deconstructors aren't being called properly
    // maybe something to do with order?
    exit(EXIT_SUCCESS);
}