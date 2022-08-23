#include "World.h"
#include "Player.h"
#include "MainMenu.h"

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

    Application::SetBGColor(RGBColor::BLACK);

    // Flag::Set(VIVIUM_FLAG_DRAW_PHYSICS_CIRCLES);

    MainMenu main_menu{};

    // Loop until window is closed by user
    while (Application::IsRunning())
    {
        Application::BeginFrame();

        // Update GUI objects
        // ...

        // Update title screen
        main_menu.Update();

        // Draw calls
        main_menu.Render();

        Application::EndFrame();
    }

    LogInfo("Window closed");

    // title_screen.GetPlayer()->Save(*title_screen.GetWorld());

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