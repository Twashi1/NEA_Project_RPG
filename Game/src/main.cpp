#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

static CoolEngineName::Text* slider_text_ptr;
static CoolEngineName::Text* rotater_text_ptr;

static CoolEngineName::Shader* ground_shader_ptr;

void SliderFunc(CoolEngineName::Slider* ctx) {
    slider_text_ptr->text = std::format("Scale: {}", CoolEngineName::Utils::Round(ctx->GetValue(0.0f, 2.0f), 3));
}

void RotaterFunc(CoolEngineName::Slider* ctx) {
    rotater_text_ptr->text = std::format("Rotation: {}", CoolEngineName::Utils::Round(ctx->GetValue(-180.0f, 180.0f), 3));
}

void PixelationSliderFunc(CoolEngineName::Slider* ctx) {
    ENG_LogInfo("New pixelation value: {}", int(ctx->GetValue(1, 1000)));
    ground_shader_ptr->Bind();
    ground_shader_ptr->SetUniform1i("u_Pixelation", int(ctx->GetValue(1, 1000)));
}

void TextFunc(CoolEngineName::TextInput* ctx) {
    ENG_LogInfo("Submitted text: {}", ctx->typed_text);
}

int main(void)
{
    // Construct engine instance
    CoolEngineName::Application::Init(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    CoolEngineName::Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    CoolEngineName::Shader colour_shader("world_vertex", "color_frag");
    colour_shader.SetUniform3f("u_Color", CoolEngineName::COLORS::RED);

    CoolEngineName::Shader grey_shader("static_vertex", "transparency_frag");
    grey_shader.SetUniform4f("u_Color", CoolEngineName::COLORS::GRAY.x, CoolEngineName::COLORS::GRAY.y, CoolEngineName::COLORS::GRAY.z, 0.4f);

    CoolEngineName::Shader red_shader("static_vertex", "transparency_frag");
    red_shader.SetUniform4f("u_Color", CoolEngineName::COLORS::RED.x, CoolEngineName::COLORS::RED.y, CoolEngineName::COLORS::RED.z, 0.6f);

    CoolEngineName::Shader static_color("static_vertex", "color_frag");
    static_color.SetUniform3f("u_Color", CoolEngineName::COLORS::BLUE);

    CoolEngineName::Shader static_texture("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    ENG_LogInfo("Loading world...");
    World world(0, "testworld");
    World::LoadTextures("tile_atlas.png");
    world.Update(CoolEngineName::Vector2<int>(0, 0));
    ENG_LogInfo("Finished initialising world");

    ENG_Ptr(CoolEngineName::Quad) wall =            ENG_MakePtr(CoolEngineName::Quad, 500, 500, 100, 500, 0.5 * PI_CONST);
    ENG_Ptr(CoolEngineName::Quad) ani_quad =        ENG_MakePtr(CoolEngineName::Quad, -100, -100, 150, 150);
    ENG_Ptr(CoolEngineName::Quad) noisequad =       ENG_MakePtr(CoolEngineName::Quad, -500, 500, 256, 256);

    ENG_Ptr(CoolEngineName::Quad) textbox =         ENG_MakePtr(CoolEngineName::Quad, WIDTH - 100, HEIGHT - 96, 96*3, 96);

    ENG_Ptr(CoolEngineName::Quad) scale_bar =       ENG_MakePtr(CoolEngineName::Quad, -150, -300, 200, 30);
    ENG_Ptr(CoolEngineName::Quad) scale_slider =    ENG_MakePtr(CoolEngineName::Quad, -150, -300, 10, 40);

    ENG_Ptr(CoolEngineName::Quad) rot_bar =         ENG_MakePtr(CoolEngineName::Quad, WIDTH - 150, HEIGHT - 450, 200, 30);
    ENG_Ptr(CoolEngineName::Quad) rot_slider =      ENG_MakePtr(CoolEngineName::Quad, WIDTH - 150, HEIGHT - 450, 10, 40);

    ENG_Ptr(CoolEngineName::Quad) ground_quad =     ENG_MakePtr(CoolEngineName::Quad, -600, 600, 800, 800);

    ENG_Ptr(CoolEngineName::Quad) pixel_bar = ENG_MakePtr(CoolEngineName::Quad, -150, -600, 200, 30);
    ENG_Ptr(CoolEngineName::Quad) pixel_slider = ENG_MakePtr(CoolEngineName::Quad, -150, -600, 10, 40);

    CoolEngineName::Slider slider = CoolEngineName::Slider(scale_bar, scale_slider, &SliderFunc); slider.SetValue(1.0f, 0.0f, 2.0f);
    CoolEngineName::Slider rotater = CoolEngineName::Slider(rot_bar, rot_slider, &RotaterFunc); rotater.SetValue(0.0f, -PI_CONST, PI_CONST);
    CoolEngineName::Text slider_text = CoolEngineName::Text("Scale: 1.0", {scale_bar->Left(), scale_bar->Top() + 70}, 0.3);
    CoolEngineName::Text rotater_text = CoolEngineName::Text("Rotation: 0.0", { rot_bar->Left(), rot_bar->Top() + 70 }, 0.3);
    slider_text_ptr = &slider_text;
    rotater_text_ptr = &rotater_text;

    CoolEngineName::Slider pixelator = CoolEngineName::Slider(pixel_bar, pixel_slider, &PixelationSliderFunc); pixelator.SetValue(1, 1, 1000);

    CoolEngineName::Texture atlas_test("atlas.png");

    CoolEngineName::Animation animation(ani_quad, ENG_Ptr(CoolEngineName::Shader)(&texture_shader), ENG_Ptr(CoolEngineName::Texture)(&atlas_test), { 64, 64 }, CoolEngineName::Animation::Data("data"));

    Player player = Player();
    CoolEngineName::Application::physics->Register(player.body, 0);

    // DEBUG: add to physics system
    CoolEngineName::Body wallbody(*wall, true, 0.0f, 999);
    CoolEngineName::Application::physics->Register(ENG_Ptr(CoolEngineName::Body)(&wallbody), 0);

    textbox->SetTextureCoords(*CoolEngineName::Application::engine_icons, { 2, 7 }, { 4, 7 }, { 16, 16 });
    CoolEngineName::TextInput text_input(*textbox, &TextFunc, ENG_Ptr(CoolEngineName::Shader)(&static_texture), CoolEngineName::Application::engine_icons, 30);

    CoolEngineName::Application::SetBGColor(CoolEngineName::COLORS::BLUE);

    CoolEngineName::Application::window_panel->Anchor(CoolEngineName::Panel::ANCHOR::RIGHT, CoolEngineName::Panel::ANCHOR::TOP, scale_bar);
    CoolEngineName::Application::window_panel->Anchor(CoolEngineName::Panel::ANCHOR::RIGHT, CoolEngineName::Panel::ANCHOR::TOP, scale_slider);
    CoolEngineName::Application::window_panel->Anchor(CoolEngineName::Panel::ANCHOR::RIGHT, CoolEngineName::Panel::ANCHOR::TOP, pixel_bar);
    CoolEngineName::Application::window_panel->Anchor(CoolEngineName::Panel::ANCHOR::RIGHT, CoolEngineName::Panel::ANCHOR::TOP, pixel_slider);

    CoolEngineName::Shader ground_shader("texture_vertex", "ground_frag");
    ground_shader.SetUniform1f("u_Scale", 50.0f);
    ground_shader.SetUniform1ui("u_Seed", 0);
    ground_shader.SetUniform3f("u_StdBrown", CoolEngineName::COLORS::GREEN);

    ground_shader_ptr = &ground_shader;

    // Loop until window is closed by user
    while (CoolEngineName::Application::IsRunning())
    {
        // TODO better API for setting camera position
        CoolEngineName::Renderer::camera->SetCamera(
            player.quad.GetCenter(),
            { CoolEngineName::Application::width / 2.0f, CoolEngineName::Application::height / 2.0f },
            slider.GetValue(0.0f, 2.0f),
            rotater.GetValue(-PI_CONST, PI_CONST)
        );

        CoolEngineName::Application::BeginFrame();

        // Update GUI objects
        slider.Update();
        rotater.Update();
        text_input.Update();
        animation.Update();
        pixelator.Update();

        // Update player
        player.Update();

        // Draw calls
        CoolEngineName::Vector2<int> update_pos = (player.quad.GetCenter() / World::scale).floor();

        world.Update(update_pos);
        CoolEngineName::Renderer::Schedule(wall.get(), &colour_shader);
        CoolEngineName::Renderer::Schedule(&animation);
        CoolEngineName::Renderer::Schedule(&text_input);
        CoolEngineName::Renderer::Schedule(ground_quad.get(), &ground_shader);
        CoolEngineName::Renderer::Schedule(&player.quad, player.shader);
        CoolEngineName::Renderer::Schedule(&slider);
        CoolEngineName::Renderer::Schedule(slider_text_ptr);
        CoolEngineName::Renderer::Schedule(&rotater);
        CoolEngineName::Renderer::Schedule(rotater_text_ptr);
        CoolEngineName::Renderer::Schedule(&pixelator);

        if (CoolEngineName::Application::isStatsEnabled) CoolEngineName::Application::UpdateStats(*player.body); // Draw stats information

        CoolEngineName::Application::EndFrame();

        wallbody.angular_acc = 1.0f;
        wallbody.angular_vel = std::min(wallbody.angular_vel, 3.0f);
    }

    ENG_LogInfo("Window closed");

    CoolEngineName::Application::Terminate();

    ENG_LogInfo("Ending program");

    // TODO: program still doesn't exit properly
    exit(EXIT_SUCCESS);
}