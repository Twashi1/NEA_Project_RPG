#include "World.h"
#include "Player.h"

constexpr float PI_CONST = 3.14159265358979323846f;
const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

static Vivium::Text* slider_text_ptr;
static Vivium::Text* rotater_text_ptr;

static Vivium::Shader* ground_shader_ptr;

void SliderFunc(Vivium::Slider* ctx) {
    slider_text_ptr->text = std::format("Scale: {}", Vivium::Utils::Round(ctx->GetValue(0.0f, 2.0f), 3));
}

void RotaterFunc(Vivium::Slider* ctx) {
    rotater_text_ptr->text = std::format("Rotation: {}", Vivium::Utils::Round(ctx->GetValue(-180.0f, 180.0f), 3));
}

void PixelationSliderFunc(Vivium::Slider* ctx) {
    ENG_LogInfo("New pixelation value: {}", int(ctx->GetValue(50, 300)));
    ground_shader_ptr->Bind();
    ground_shader_ptr->SetUniform1i("u_Pixelation", int(ctx->GetValue(50, 300)));
}

int main(void)
{
    // Construct engine instance
    Vivium::Application::Init(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    Vivium::Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Vivium::Shader colour_shader("world_vertex", "color_frag");
    colour_shader.SetUniform3f("u_Color", Vivium::COLORS::RED);

    Vivium::Shader grey_shader("static_vertex", "transparency_frag");
    grey_shader.SetUniform4f("u_Color", Vivium::COLORS::GRAY.x, Vivium::COLORS::GRAY.y, Vivium::COLORS::GRAY.z, 0.4f);

    Vivium::Shader red_shader("static_vertex", "transparency_frag");
    red_shader.SetUniform4f("u_Color", Vivium::COLORS::RED.x, Vivium::COLORS::RED.y, Vivium::COLORS::RED.z, 0.6f);

    Vivium::Shader static_color("static_vertex", "color_frag");
    static_color.SetUniform3f("u_Color", Vivium::COLORS::BLUE);

    Vivium::Shader static_texture("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    ENG_LogInfo("Loading world...");
    World world(0, "testworld");
    World::LoadTextures("tile_atlas.png");
    world.Update(Vivium::Vector2<int>(0, 0));
    ENG_LogInfo("Finished initialising world");

    ENG_Ptr(Vivium::Quad) scale_bar =       ENG_MakePtr(Vivium::Quad, -150, -300, 200, 30);
    ENG_Ptr(Vivium::Quad) scale_slider =    ENG_MakePtr(Vivium::Quad, -150, -300, 10, 40);

    ENG_Ptr(Vivium::Quad) rot_bar =         ENG_MakePtr(Vivium::Quad, -150, -450, 200, 30);
    ENG_Ptr(Vivium::Quad) rot_slider =      ENG_MakePtr(Vivium::Quad, -150, -450, 10, 40);

    ENG_Ptr(Vivium::Quad) ground_quad =     ENG_MakePtr(Vivium::Quad, -600, 600, 800, 800);

    ENG_Ptr(Vivium::Quad) pixel_bar =       ENG_MakePtr(Vivium::Quad, -150, -600, 200, 30);
    ENG_Ptr(Vivium::Quad) pixel_slider =    ENG_MakePtr(Vivium::Quad, -150, -600, 10, 40);

    ENG_Ptr(Vivium::Slider) slider = ENG_MakePtr(Vivium::Slider, scale_bar, scale_slider, &SliderFunc); slider->SetValue(1.0f, 0.0f, 2.0f);
    ENG_Ptr(Vivium::Slider) rotater = ENG_MakePtr(Vivium::Slider, rot_bar, rot_slider, &RotaterFunc); rotater->SetValue(0.0f, -PI_CONST, PI_CONST);
    Vivium::Text slider_text = Vivium::Text("Scale: 1.0", {scale_bar->Left(), scale_bar->Top() + 70}, 0.3);
    Vivium::Text rotater_text = Vivium::Text("Rotation: 0.0", { rot_bar->Left(), rot_bar->Top() + 70 }, 0.3);
    slider_text_ptr = &slider_text;
    rotater_text_ptr = &rotater_text;

    ENG_Ptr(Vivium::Slider) pixelator = ENG_MakePtr(Vivium::Slider, pixel_bar, pixel_slider, &PixelationSliderFunc); pixelator->SetValue(300, 50, 300);

    Vivium::Texture atlas_test("atlas.png");

    Player player = Player();
    Vivium::Application::physics->Register(player.body, 0);

    Vivium::Application::SetBGColor(Vivium::COLORS::BLUE);

    Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, slider);
    Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, pixelator);
    Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, rotater);

    //Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, ENG_Ptr(Vivium::Text)(&slider_text));
    //Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, ENG_Ptr(Vivium::Text)(&rotater_text));

    Vivium::Shader ground_shader("texture_vertex", "ground_frag");
    ground_shader.SetUniform1f("u_Scale", 50.0f);
    ground_shader.SetUniform1ui("u_Seed", 0);
    ground_shader.SetUniform3f("u_StdBrown", Vivium::COLORS::GREEN);

    ground_shader_ptr = &ground_shader;

    // Loop until window is closed by user
    while (Vivium::Application::IsRunning())
    {
        // TODO better API for setting camera position
        Vivium::Renderer::camera->SetCamera(
            player.quad.GetCenter(),
            { Vivium::Application::width / 2.0f, Vivium::Application::height / 2.0f },
            slider->GetValue(0.0f, 2.0f),
            rotater->GetValue(-PI_CONST, PI_CONST)
        );

        Vivium::Application::BeginFrame();

        // Update GUI objects
        slider->Update();
        rotater->Update();
        pixelator->Update();

        // Update player
        player.Update();

        // Draw calls
        Vivium::Vector2<int> update_pos = (player.quad.GetCenter() / World::scale).floor();

        world.Update(update_pos);
        Vivium::Renderer::Schedule(ground_quad.get(), &ground_shader);
        Vivium::Renderer::Schedule(&player.quad, player.shader);
        Vivium::Renderer::Schedule(slider.get());
        Vivium::Renderer::Schedule(slider_text_ptr);
        Vivium::Renderer::Schedule(rotater.get());
        Vivium::Renderer::Schedule(rotater_text_ptr);
        Vivium::Renderer::Schedule(pixelator.get());

        if (Vivium::Application::isStatsEnabled) Vivium::Application::UpdateStats(*player.body); // Draw stats information

        Vivium::Application::EndFrame();
    }

    ENG_LogInfo("Window closed");

    Vivium::Application::Terminate();

    ENG_LogInfo("Ending program");

    // TODO: program still doesn't exit properly
    exit(EXIT_SUCCESS);
}