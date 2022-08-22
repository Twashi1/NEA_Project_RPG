#include "World.h"
#include "Player.h"
#include "TitleScreen.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

struct MyStruct : IStreamable {
    int x, y;

    MyStruct() {}
    MyStruct(int x, int y) : x(x), y(y) {}

    void Write(Serialiser& s) const override {
        s.Write(x);
        s.Write(y);
    }

    void Read(Serialiser& s) override {
        s.Read(&x);
        s.Read(&y);
    }

    friend std::ostream& operator<<(std::ostream& os, const MyStruct& s) {
        os << "[" << s.x << ", " << s.y << "]";

        return os;
    }
};

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS, true);

    std::unordered_map<int, MyStruct> numbers0 = {
        {1, MyStruct(0, 1)},
        {2, MyStruct(2, 0)},
        {3, MyStruct(3, 0)}
    };

    Serialiser s(Stream::Flags::BINARY | Stream::Flags::TRUNC);
    s.BeginWrite("../Resources/saves/mapfile.txt");
    s.Write(numbers0);
    s.EndWrite();

    std::unordered_map<int, MyStruct> numbers1;

    s.BeginRead("../Resources/saves/mapfile.txt");
    s.Read(&numbers1);
    s.EndRead();

    LogTrace("Map is: {}", Logger::PrettyPrint(numbers0));
    LogTrace("Map after is: {}", Logger::PrettyPrint(numbers1));

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS, true);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", RGBColor::BLUE);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    World::Init();
    FloorItem::Init();
    Inventory::Init();

    // Test construct world
    World world(0, "testworld");

    Player player = Player();
    // TODO: player.Load(world);

    Application::SetBGColor(RGBColor::BLACK);

    // Flag::Set(VIVIUM_FLAG_DRAW_PHYSICS_CIRCLES);

    TitleScreen title_screen{};

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        Renderer::camera->SetCamera(
            player.quad->GetCenter(),
            { Application::width / 2.0f, Application::height / 2.0f },
            1.0f,
            0.0f
        );

        Application::BeginFrame();

        // Update GUI objects
        // ...

        title_screen.Update();

        // Update player
        // player.Update(world);

        // world.Update(&player);

        // Draw calls
        Vector2<int> update_pos = (Vector2<int>)(player.quad->GetCenter() / World::PIXEL_SCALE).floor();
        // world.Render(update_pos);

        // player.Render();

        title_screen.Render();

        if (Application::isStatsEnabled) Application::UpdateStats(*player.body); // Draw stats information

        Application::EndFrame();
    }

    LogInfo("Window closed");

    player.Save(world);

    FloorItem::Terminate();
    Inventory::Terminate();
    World::Terminate();
    Application::Terminate();

    LogInfo("Ending program");

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    game();
}