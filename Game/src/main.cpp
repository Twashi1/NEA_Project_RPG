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
    slider_text_ptr->text = std::format("Scale: {:.3f}", ctx->GetValue(0.0, 2.0f));
}

void RotaterFunc(Vivium::Slider* ctx) {
    rotater_text_ptr->text = std::format("Rotation: {:.3f}", ctx->GetValue(-180.0f, 180.0f));
}

void PixelationSliderFunc(Vivium::Slider* ctx) {
    ground_shader_ptr->Bind();
    ground_shader_ptr->SetUniform1i("u_Pixelation", int(ctx->GetValue(50, 300)));
}

int sandbox(void)
{
    using namespace Vivium;

    Application::Init(WIDTH, HEIGHT, FPS, false);

    BufferLayout example_layout = {
        {"position", GLSLDataType::VEC2},
        {"texCoords", GLSLDataType::VEC2}
    };

    std::vector<float> data = {
        0.3f, 0.3f, 0.0f, 0.0f,
        0.3f, 0.7f, 1.0f, 0.0f,
        0.7f, 0.7f, 1.0f, 1.0f,
        0.7f, 0.3f, 0.0f, 1.0f
    };

    VertexBuffer vb(data, example_layout);
    Shader eg_shader = Shader("static_texture_vertex", "color_frag");
    eg_shader.SetUniform3f("u_Color", 1.0f, 0.0f, 0.0f);

    IndexBuffer ib(std::vector<uint8_t>{0, 1, 2, 2, 3, 0});
    
    while (Application::IsRunning()) {
        Application::BeginFrame();

        Renderer::Submit(&vb, &ib, &eg_shader);

        Application::EndFrame();
    }

    return EXIT_SUCCESS;
}

int game(void)
{
    // Construct engine instance
    Vivium::Application::Init(WIDTH, HEIGHT, FPS, true);

    // DEBUG: shaders
    LogInfo("Starting shader creation");
    Vivium::Shader texture_shader("texture_vertex", "texture_frag");
    texture_shader.SetUniform1f("u_Scale", 1);

    Vivium::Shader colour_shader("world_vertex", "color_frag");
    colour_shader.SetUniform3f("u_Color", 1.0, 0.0, 0.0);

    Vivium::Shader grey_shader("static_vertex", "transparency_frag");
    grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);

    Vivium::Shader red_shader("static_vertex", "transparency_frag");
    red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);

    Vivium::Shader static_color("static_vertex", "color_frag");
    static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);

    Vivium::Shader static_texture("static_texture_vertex", "texture_frag");

    World::texture_shader = &texture_shader;

    // Test construct world
    LogInfo("Loading world...");
    World world(0, "testworld");
    World::LoadTextures("tile_atlas.png");
    world.Update(Vivium::Vector2<int>(0, 0));
    LogInfo("Finished initialising world");

    Ref(Vivium::Quad) scale_bar =       MakeRef(Vivium::Quad, -150, -300, 200, 30);
    Ref(Vivium::Quad) scale_slider =    MakeRef(Vivium::Quad, -150, -300, 10, 40);

    Ref(Vivium::Quad) rot_bar =         MakeRef(Vivium::Quad, -150, -450, 200, 30);
    Ref(Vivium::Quad) rot_slider =      MakeRef(Vivium::Quad, -150, -450, 10, 40);

    Ref(Vivium::Quad) ground_quad =     MakeRef(Vivium::Quad, -600, 600, 800, 800);

    Ref(Vivium::Quad) pixel_bar =       MakeRef(Vivium::Quad, -150, -600, 200, 30);
    Ref(Vivium::Quad) pixel_slider =    MakeRef(Vivium::Quad, -150, -600, 10, 40);

    Ref(Vivium::Slider) slider = MakeRef(Vivium::Slider, scale_bar, scale_slider, &SliderFunc); slider->SetValue(1.0f, 0.0f, 2.0f);
    Ref(Vivium::Slider) rotater = MakeRef(Vivium::Slider, rot_bar, rot_slider, &RotaterFunc); rotater->SetValue(0.0f, -PI_CONST, PI_CONST);
    Vivium::Text slider_text = Vivium::Text("Scale: 1.0", {scale_bar->Left(), scale_bar->Top() + 70}, 0.3);
    Vivium::Text rotater_text = Vivium::Text("Rotation: 0.0", { rot_bar->Left(), rot_bar->Top() + 70 }, 0.3);
    slider_text_ptr = &slider_text;
    rotater_text_ptr = &rotater_text;

    Ref(Vivium::Slider) pixelator = MakeRef(Vivium::Slider, pixel_bar, pixel_slider, &PixelationSliderFunc); pixelator->SetValue(300, 50, 300);

    Vivium::Texture atlas_test("atlas.png");

    Player player = Player();
    Vivium::Application::physics->Register(player.body, 0);

    Vivium::Application::SetBGColor(Vivium::RGBColor(0.0f, 0.0f, 1.0f));

    Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, slider);
    Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, pixelator);
    Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, rotater);

    //Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, Ref(Vivium::Text)(&slider_text));
    //Vivium::Application::window_panel->Anchor(Vivium::Panel::ANCHOR::RIGHT, Vivium::Panel::ANCHOR::TOP, Ref(Vivium::Text)(&rotater_text));

    Vivium::Shader ground_shader("texture_vertex", "ground_frag");
    ground_shader.SetUniform1f("u_Scale", 50.0f);
    ground_shader.SetUniform1ui("u_Seed", 0);
    ground_shader.SetUniform3f("u_StdBrown", 0.0, 1.0, 0.0);

    ground_shader_ptr = &ground_shader;

    // Loop until window is closed by user
    while (Vivium::Application::IsRunning())
    {
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
        Vivium::Vector2<int> update_pos = (Vivium::Vector2<int>)(player.quad.GetCenter() / World::scale).floor();
        world.Update(update_pos);

        Vivium::Renderer::Submit(ground_quad.get(), &ground_shader);
        Vivium::Renderer::Submit(&player.quad, player.shader);
        Vivium::Renderer::Submit(slider.get());
        Vivium::Renderer::Submit(slider_text_ptr);
        Vivium::Renderer::Submit(rotater.get());
        Vivium::Renderer::Submit(rotater_text_ptr);
        Vivium::Renderer::Submit(pixelator.get());

        if (Vivium::Application::isStatsEnabled) Vivium::Application::UpdateStats(*player.body); // Draw stats information

        Vivium::Application::EndFrame();
    }

    LogInfo("Window closed");

    Vivium::Application::Terminate();

    LogInfo("Ending program");

    // TODO: program still doesn't exit properly
    exit(EXIT_SUCCESS);
}

int main(void) {
    sandbox();
}