#include <iostream>

#include "World.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

static int seed = 0;
static Noise::Interpolated* i = nullptr;
static bool seedChanged = false;

void tester_callback(Button* button_pressed) {
    seed++;
    i->SetSeed(seed);
    seedChanged = true;
}

int main(void)
{
    // Construct engine instance
    Engine engine(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Shader cellular_shader("cellular_vertex", "cellular_frag");
    cellular_shader.SetUniform2f("u_Resolution", engine.width, engine.height);
    cellular_shader.SetUniform1f("u_Scale", 5);
    cellular_shader.SetUniform1ui("u_Seed", 0);

    Shader colour_shader = Shader("colour_vertex", "colour_frag");
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader bg_shader = Shader("colour_vertex", "colour_frag");
    bg_shader.SetUniform3f("u_Color", COLORS::BLUE * 0.5f);

    ShaderManager::UpdateProjectionMatrix(engine.proj);

    World::texture_shader = &texture_shader;

    // Test construct world
    Log("Loading world...", Utils::ERROR::INFO);
    World world(0, "testworld");
    Log("Loading textures...", Utils::ERROR::INFO);
    World::LoadTextures("tile_atlas.png");
    Log("Updating world...", Utils::ERROR::INFO);
    world.Update(Vector2<int>(0, 0));
    Log("Finished initialising world", Utils::ERROR::INFO);

    // DEBUG: quads
    Quad wall = Quad(500, 500, 100, 500, 0.5 * PI_CONST);
    Quad btn_quad = Quad(-400, -300, 150, 50, 0);
    Quad dummy = Quad(-100, -100, 150, 150, 0);
    Quad noisequad = Quad(-500, 500, 256, 256, 0);

    // DEBUG: buttons
    Button btn = Button(btn_quad, &tester_callback, "Change seed", "Change seed");

    // DEBUG: atlas test
    Texture atlas_test = Texture("atlas.png"); // Create texture

    // DEBUG: noise test
    const int SIZE = 256;
    Noise::Interpolated interp(seed, 1.0f, 64);
    i = &interp;
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

    Texture noisetext = Texture(std::shared_ptr<uint8_t[]>(buffer2), SIZE, SIZE);

    Renderable noisedraw = Renderable(std::shared_ptr<Quad>(&noisequad), &texture_shader, &noisetext, 5);

    // DEBUG: renderables
    Renderable walldraw(std::shared_ptr<Quad>(&wall), &colour_shader, 1);
    Renderable dummydraw(std::shared_ptr<Quad>(&dummy), &texture_shader, &atlas_test, 2);

    // DEBUG: Animation object
    Animation animation(std::shared_ptr<Renderable>(&dummydraw), std::shared_ptr<Texture>(&atlas_test), { 64, 64 }, "data");

    // DEBUG: add to physics system
    Body wallbody = Body(wall, true, 0.0f, 999);
    engine.physics.layers[0].push_back(std::shared_ptr<Body>(&wallbody));

    // Loop until window is closed by user
    while (engine.IsRunning())
    {
        if (seedChanged) {
            seedChanged = false;

            for (int i = 0; i < SIZE * SIZE; i++) {
                int index = i * 4;
                int y = i / SIZE; int x = i - (y * SIZE);

                uint8_t v = interp.GetFractal(x, y, 4) * 0xff;

                buffer2[index] = v;
                buffer2[index + 1] = v;
                buffer2[index + 2] = v;
                buffer2[index + 3] = 0xff;
            }

            noisetext.Delete();
            noisetext.Create();
        }

        engine.Update(); // Update engine

        // DEGUG: Rotate our example wall
        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = Utils::ClampMax(wallbody.angular_vel, 3.0f);

        wall.SetAngle(wall.GetAngle());
    }

    Log("Window closed", Utils::ERROR::INFO);

    engine.SerialiseGeneralData();

    Log("Ending program", Utils::ERROR::INFO);

    // TODO: still have to forcibly exit since deconstructors aren't being called properly
    // maybe something to do with order?
    exit(EXIT_SUCCESS);
}