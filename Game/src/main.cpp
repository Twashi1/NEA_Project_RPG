#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

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

int main(void)
{
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Shader colour_shader = Shader("world_vertex", "color_frag");
    colour_shader.SetUniform3f("u_Color", COLORS::RED);

    Shader grey_shader = Shader("static_vertex", "transparency_frag");
    grey_shader.SetUniform4f("u_Color", COLORS::GRAY.x, COLORS::GRAY.y, COLORS::GRAY.z, 0.4f);

    Shader red_shader = Shader("static_vertex", "transparency_frag");
    red_shader.SetUniform4f("u_Color", COLORS::RED.x, COLORS::RED.y, COLORS::RED.z, 0.6f);

    Shader static_color = Shader("static_vertex", "color_frag");
    static_color.SetUniform3f("u_Color", COLORS::BLUE);

    Shader static_texture = Shader("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    ENG_LogInfo("Loading world...");
    World world(0, "testworld");
    World::LoadTextures("tile_atlas.png");
    world.Update(Vector2<int>(0, 0));
    ENG_LogInfo("Finished initialising world");

    ENG_Ptr(Quad) wall =            ENG_MakePtr(Quad, 500, 500, 100, 500, 0.5 * PI_CONST);
    ENG_Ptr(Quad) ani_quad =        ENG_MakePtr(Quad, -100, -100, 150, 150);
    ENG_Ptr(Quad) noisequad =       ENG_MakePtr(Quad, -500, 500, 256, 256);
    ENG_Ptr(Quad) textbox =         ENG_MakePtr(Quad, WIDTH - 100, HEIGHT - 96, 96*3, 96);

    ENG_Ptr(Quad) scale_bar =       ENG_MakePtr(Quad, -150, -200, 200, 30);
    ENG_Ptr(Quad) scale_slider =    ENG_MakePtr(Quad, -150, -200, 10, 40);

    ENG_Ptr(Quad) rot_bar =         ENG_MakePtr(Quad, WIDTH - 150, HEIGHT - 450, 200, 30);
    ENG_Ptr(Quad) rot_slider =      ENG_MakePtr(Quad, WIDTH - 150, HEIGHT - 450, 10, 40);

    Slider slider = Slider(scale_bar, scale_slider, &SliderFunc); slider.SetValue(1.0f, 0.0f, 2.0f);
    Slider rotater = Slider(rot_bar, rot_slider, &RotaterFunc); rotater.SetValue(0.0f, -PI_CONST, PI_CONST);
    Text slider_text = Text("Scale: 1.0", {scale_bar->Left(), scale_bar->Top() + 70}, 0.3);
    Text rotater_text = Text("Rotation: 0.0", { rot_bar->Left(), rot_bar->Top() + 70 }, 0.3);
    slider_text_ptr = &slider_text;
    rotater_text_ptr = &rotater_text;

    Texture atlas_test = Texture("atlas.png");

    Animation animation(ani_quad, ENG_Ptr(Shader)(&texture_shader), ENG_Ptr(Texture)(&atlas_test), { 64, 64 }, Animation::Data("data"));

    Player player = Player();
    Application::physics->Register(player.body, 0);

    // DEBUG: add to physics system
    Body wallbody = Body(*wall, true, 0.0f, 999);
    Application::physics->Register(ENG_Ptr(Body)(&wallbody), 0);

    textbox->SetTextureCoords(*Application::engine_icons, { 2, 7 }, { 4, 7 }, { 16, 16 });
    TextInput text_input(*textbox, &TextFunc, ENG_Ptr(Shader)(&static_texture), Application::engine_icons, 30);

    Application::SetBGColor(COLORS::BLUE);

    Application::window_panel->Anchor(Panel::ANCHOR::RIGHT, Panel::ANCHOR::TOP, scale_bar);
    Application::window_panel->Anchor(Panel::ANCHOR::RIGHT, Panel::ANCHOR::TOP, scale_slider);

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        // TODO better API for setting camera position
        Renderer::camera->SetCamera(
            player.quad.GetCenter(),
            { Application::width / 2.0f, Application::height / 2.0f },
            slider.GetValue(0.0f, 2.0f),
            rotater.GetValue(-PI_CONST, PI_CONST)
        );

        Application::BeginFrame();

        // Update GUI objects
        slider.Update();
        rotater.Update();
        text_input.Update();
        animation.Update();

        // Update player
        player.Update();

        // Draw calls
        Vector2<int> update_pos = (player.quad.GetCenter() / World::scale).floor();

        world.Update(update_pos);
        Renderer::Schedule(wall.get(), &colour_shader);
        Renderer::Schedule(&animation);
        Renderer::Schedule(&text_input);
        Renderer::Schedule(&player.quad, player.shader);
        Renderer::Schedule(&slider);
        Renderer::Schedule(slider_text_ptr);
        Renderer::Schedule(&rotater);
        Renderer::Schedule(rotater_text_ptr);

        if (Application::isStatsEnabled) Application::UpdateStats(*player.body); // Draw stats information

        Application::EndFrame();

        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = std::min(wallbody.angular_vel, 3.0f);
    }

    ENG_LogInfo("Window closed");

    Application::Terminate();

    ENG_LogInfo("Ending program");

    // TODO: program still doesn't exit properly
    exit(EXIT_SUCCESS);
}