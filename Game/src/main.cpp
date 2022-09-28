#include "World.h"
#include "Player.h"
#include "MainMenu.h"
#include "LeafParticles.h"

const int WIDTH = 1080;
const int HEIGHT = 720;
const int FPS = 144;

using namespace Vivium;
using namespace Game;

class MeetingEventHandler;

class MeetingEvent : public Vivium::Event {
private:
    static constexpr const char* s_Type = "MeetingEvent";

public:
    std::string person_a;
    std::string person_b;

    MeetingEvent(const std::string& a, const std::string& b)
        : Vivium::Event(s_Type), person_a(a), person_b(b) {}

    friend MeetingEventHandler;
};

class MeetingEventHandler : public Vivium::EventHandler {
private:
    static constexpr const char* s_Type = "MeetingEvent";

protected:
    virtual void m_HandleEvent(Ref(Event) event) override
    {
        Ref(MeetingEvent) meeting = dynamic_pointer_cast<MeetingEvent>(event);
        std::cout
            << "Meeting between "
            << meeting->person_a
            << " and "
            << meeting->person_b
            << " (Took " << meeting->m_InvokeTimer.GetElapsed() * 1000.0f << "ms)"
            << std::endl;
    }

public:
    MeetingEventHandler()
        : Vivium::EventHandler(s_Type) {}
};

int sandbox(void)
{
    Application::Init(WIDTH, HEIGHT, FPS);

    Application::SetBGColor(RGBColor::BLUE);

    Animation::Data test("ruby_wand", {32, 32});
    Animation::Data test2("emmy_wand", { 32, 32 });

    TextureManager::Init();

    Ref(Quad) rquad = MakeRef(Quad, 100.0f, 100.0f, 128.0f, 128.0f);
    Ref(Quad) equad = MakeRef(Quad, 300.0f, 300.0f, 128.0f, 128.0f);
    Ref(Shader) rshader = MakeRef(Shader, "texture_vertex", "texture_frag");
    Animation ruby_wand_ani(rquad, rshader, TextureManager::game_atlas, test);
    Animation emmy_wand_ani(equad, rshader, TextureManager::game_atlas, test2);

    while (Application::IsRunning()) {
        Application::BeginFrame();

        ruby_wand_ani.Update();
        emmy_wand_ani.Update();
        Renderer::Submit(&ruby_wand_ani);
        Renderer::Submit(&emmy_wand_ani);

        Application::EndFrame();
    }

    Application::Terminate();

    return EXIT_SUCCESS;
}

int game(void)
{
    Vivium::Flag::Set(VIVIUM_FLAG_FUNCTION_SIGNATURE_LOGGING, 1);
    // Construct engine instance
    Application::Init(WIDTH, HEIGHT, FPS);

    Shader texture_shader("texture_vertex", "texture_frag");
    Shader colour_shader("world_vertex", "color_frag"); colour_shader.SetUniform3f("u_Color", RGBColor::BLUE);
    Shader grey_shader("static_vertex", "transparency_frag"); grey_shader.SetUniform4f("u_Color", 0.3, 0.3, 0.3, 0.4f);
    Shader red_shader("static_vertex", "transparency_frag"); red_shader.SetUniform4f("u_Color", 1.0, 0.0, 0.0, 0.6f);
    Shader static_color("static_vertex", "color_frag"); static_color.SetUniform3f("u_Color", 0.0, 0.0, 1.0);
    Shader static_texture("static_texture_vertex", "texture_frag");

    TextureManager::Init();
    World::Init();
    WorldMap::Init();
    FloorItem::Init();
    Inventory::Init();
    CraftingInventory::Init();
    LeavesParticleSystem::Init();

    Application::SetBGColor(RGBColor::BLACK);

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

    CraftingInventory::Terminate();
    Biome::Terminate();
    Inventory::Terminate();
    FloorItem::Terminate();
    WorldMap::Terminate();
    World::Terminate();
    Application::Terminate();

    LogInfo("Ending program");

    return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
    game();

    LogTrace("Game finished");
}