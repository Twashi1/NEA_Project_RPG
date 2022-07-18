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

void tester_callback(Button* ctx) {
    seed++;
    i->SetSeed(seed);
    seedChanged = true;
    ENG_LogInfo("Changing seed to {}", seed);
}

void text_callback(TextInput* ctx) {
    ENG_LogInfo("Submitted text: {}", ctx->typed_text);
}

int main(void)
{
    // Construct engine instance
    Engine::Init(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Shader colour_shader = Shader("colour_vertex", "colour_frag");
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader static_color = Shader("button_vertex", "button_frag");
    static_color.SetUniform4f("u_Color", COLORS::BLUE.x, COLORS::BLUE.y, COLORS::BLUE.z, 1.0f);

    Shader static_texture = Shader("static_texture_vertex", "static_texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    ENG_LogInfo("Loading world...");
    //World world(0, "testworld");
    ENG_LogInfo("Loading textures...");
    //World::LoadTextures("tile_atlas.png");
    ENG_LogInfo("Updating world...");
    //world.Update(Vector2<int>(0, 0));
    ENG_LogInfo("Finished initialising world");

    // DEBUG: quads
    Quad wall = Quad(500, 500, 100, 500, 0.5 * PI_CONST);
    Quad btn_quad = Quad(-400, -300, 150, 50, 0);
    Quad dummy = Quad(-100, -100, 150, 150, 0);
    Quad noisequad = Quad(-500, 500, 256, 256, 0);
    Quad textbox = Quad(300, 300, 96*3, 96, 0);

    // DEBUG: buttons
    Button btn = Button(btn_quad, &tester_callback, "Change seed", "Change seed");

    // DEBUG: atlas test
    Texture atlas_test = Texture("atlas.png"); // Create texture

    // DEBUG: noise test
    const int SIZE = 8;
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

    Texture noisetext = Texture(ENG_Ptr(uint8_t[])(buffer), SIZE, SIZE);

    // DEBUG: Animation object
    Animation animation(ENG_Ptr(Quad)(&dummy), ENG_Ptr(Shader)(&texture_shader), ENG_Ptr(Texture)(&atlas_test), { 64, 64 }, Animation::Data("data"));

    Player player = Player(Engine::proj);
    Engine::physics->Register(player.body, 0);

    // DEBUG: add to physics system
    Body wallbody = Body(wall, true, 0.0f, 999);
    Engine::physics->Register(ENG_Ptr(Body)(&wallbody), 0);

    textbox.SetTextureCoords(*Engine::engine_icons, { 2, 7 }, { 4, 7 }, { 16, 16 });
    TextInput text_input(textbox, &text_callback, ENG_Ptr(Shader)(&static_texture), Engine::engine_icons, 30);

    ShaderManager::UpdateProjectionMatrix(Engine::proj);

    Engine::SetBGColor(COLORS::BLUE);

    // Loop until window is closed by user
    while (Engine::IsRunning())
    {
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

        Engine::camera->pos = player.quad.GetCenter(); // Update camera
        Engine::BeginFrame();

        player.Update(); // Update player

        // Draw calls
        Vector2<int> update_pos = (player.quad.GetCenter() / World::scale).floor();

        //world.Update(update_pos);
        Renderer::Schedule(&wall, &colour_shader);
        Renderer::Schedule(&noisequad, &texture_shader, &noisetext);
        Renderer::Schedule(&animation);
        Renderer::Schedule(&btn);
        Renderer::Schedule(&text_input);
        Renderer::Schedule(&player.quad, player.shader);

        if (Engine::isStatsEnabled) Engine::UpdateStats(*player.body); // Draw stats information

        Engine::EndFrame();

        // DEGUG: Rotate our example wall
        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = std::min(wallbody.angular_vel, 3.0f);
    }

    ENG_LogInfo("Window closed");

    Engine::Terminate();

    ENG_LogInfo("Ending program");

    // TODO: program still doesn't exit properly
    exit(EXIT_SUCCESS);
}