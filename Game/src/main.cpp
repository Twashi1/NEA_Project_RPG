#include <iostream>

#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

static int seed;
static Noise::Interpolated* i;
static bool seedChanged;
static Text* slider_text_ptr;
static Text* rotater_text_ptr;

void SliderFunc(Slider* ctx) {
    slider_text_ptr->text = std::format("Scale: {}", Utils::Round(ctx->GetValue(0.0f, 2.0f), 3));
}

void RotaterFunc(Slider* ctx) {
    rotater_text_ptr->text = std::format("Rotation: {}", Utils::Round(ctx->GetValue(-180.0f, 180.0f), 3));
}

void TextFunc(TextInput* ctx) {
    ENG_LogInfo("Submitted text: {}", ctx->typed_text);
}

void ButtonFunc(Button* ctx) {
    seed++;
    i->SetSeed(seed);
    seedChanged = true;
    ENG_LogInfo("Changing seed to {}", seed);
}

int main(void)
{
    // Construct engine instance
    Engine::Init(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Shader colour_shader = Shader("world_vertex", "color_frag");
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader static_color = Shader("static_vertex", "color_frag");
    static_color.SetUniform3f("u_Color", COLORS::BLUE);

    Shader static_texture = Shader("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    ENG_LogInfo("Loading world...");
    World world(0, "testworld");
    ENG_LogInfo("Loading textures...");
    World::LoadTextures("tile_atlas.png");
    ENG_LogInfo("Updating world...");
    world.Update(Vector2<int>(0, 0));
    ENG_LogInfo("Finished initialising world");

    // DEBUG: quads
    Quad wall = Quad(500, 500, 100, 500, 0.5 * PI_CONST);
    Quad btn_quad = Quad(100, 100, 150, 50, 0);
    Quad dummy = Quad(-100, -100, 150, 150, 0);
    Quad noisequad = Quad(-500, 500, 256, 256, 0);
    Quad textbox = Quad(WIDTH - 100, HEIGHT - 96, 96*3, 96, 0);

    Quad bar_slider = Quad(WIDTH - 150, HEIGHT - 300, 200, 30, 0);
    Quad slider_slider = Quad(WIDTH - 150, HEIGHT - 300, 10, 40, 0);

    Quad rot_bar = Quad(WIDTH - 150, HEIGHT - 450, 200, 30, 0);
    Quad rot_slider = Quad(WIDTH - 150, HEIGHT - 450, 10, 40, 0);

    Slider slider = Slider(ENG_Ptr(Quad)(&bar_slider), ENG_Ptr(Quad)(&slider_slider), &SliderFunc); slider.SetValue(1.0f, 0.0f, 2.0f);
    Slider rotater = Slider(ENG_Ptr(Quad)(&rot_bar), ENG_Ptr(Quad)(&rot_slider), &RotaterFunc); rotater.SetValue(0.0f, -PI_CONST, PI_CONST);
    Text slider_text = Text("Scale: 1.0", {bar_slider.Left(), bar_slider.Top() + 70}, 0.3);
    Text rotater_text = Text("Rotation: 0", { rot_bar.Left(), rot_bar.Top() + 70 }, 0.3);
    slider_text_ptr = &slider_text;
    rotater_text_ptr = &rotater_text;

    // DEBUG: buttons
    Button btn = Button(btn_quad, &ButtonFunc, "Change seed", "Change seed");

    // DEBUG: atlas test
    Texture atlas_test = Texture("atlas.png"); // Create texture

    // DEBUG: noise test
    const int SIZE = 48;
    Noise::Interpolated interp(seed, 1.0f, 16);
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

    Player player = Player();
    Engine::physics->Register(player.body, 0);

    // DEBUG: add to physics system
    Body wallbody = Body(wall, true, 0.0f, 999);
    Engine::physics->Register(ENG_Ptr(Body)(&wallbody), 0);

    textbox.SetTextureCoords(*Engine::engine_icons, { 2, 7 }, { 4, 7 }, { 16, 16 });
    TextInput text_input(textbox, &TextFunc, ENG_Ptr(Shader)(&static_texture), Engine::engine_icons, 30);

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

        // TODO better API for setting camera position
        Engine::camera->SetCamera(
            player.quad.GetCenter(),
            { Engine::width / 2.0f, Engine::height / 2.0f },
            slider.GetValue(0.0f, 2.0f),
            rotater.GetValue(-PI_CONST, PI_CONST)
        );

        Engine::BeginFrame();

        // Update player
        player.Update();

        // Draw calls
        Vector2<int> update_pos = (player.quad.GetCenter() / World::scale).floor();

        world.Update(update_pos);
        Renderer::Schedule(&wall, &colour_shader);
        Renderer::Schedule(&noisequad, &texture_shader, &noisetext);
        Renderer::Schedule(&animation);
        Renderer::Schedule(&btn);
        Renderer::Schedule(&text_input);
        Renderer::Schedule(&player.quad, player.shader);
        Renderer::Schedule(&slider);
        Renderer::Schedule(slider_text_ptr);
        Renderer::Schedule(&rotater);
        Renderer::Schedule(rotater_text_ptr);

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