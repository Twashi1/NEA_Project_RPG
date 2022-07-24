#include "Engine.h"
#include "TextInput.h"
#include "ToggleSwitch.h"

Utils::Timer Engine::m_Timer = Utils::Timer();

int Engine::m_FPS = 0;
double Engine::m_TimePerFrame = 0.0;

std::unordered_map<std::string, Text> Engine::m_DebugStatsText{};
unsigned int Engine::m_FramesProcessed = 0;
double Engine::m_ProcessingTime = 0.0;
bool Engine::isStatsEnabled = true;

VersionNumber Engine::m_VersionNumber = "v0.0.2";
ENG_Ptr(Texture) Engine::engine_icons = nullptr;

GLFWwindow* Engine::window = nullptr;
GLFWcursor* Engine::cursor = nullptr;
int Engine::width = 0;
int Engine::height = 0;

ENG_Ptr(Physics) Engine::physics = nullptr;


void ENGINE_API Engine::m_OnWindowResize(int nwidth, int nheight)
{
    ENG_LogInfo("Window dimensions changed to {}, {}", nwidth, nheight);

    // Update window dimensions
    width = nwidth; height = nheight;

    // Update projection matrix for camera
    Renderer::camera->SetProj(0.0f, (float)width, 0.0f, (float)height);

    // Update viewport
    GlCall(glViewport(0, 0, width, height));
}

void ENGINE_API Engine::BeginFrame()
{
    // Record time of frame beginning
    m_Timer.Start();

    // Clear the screen
    GlCall(glClear(GL_COLOR_BUFFER_BIT));

    // Update Input
    Input::Update();

    // TODO: SLOW?
    glfwSetCursor(window, cursor);

    // Update physics
    physics->Update(Utils::Timer::GetTime());

    // Poll and process events
    glfwPollEvents();
}

void ENGINE_API Engine::EndFrame()
{
    // Update the screen from all the draw calls
    glfwSwapBuffers(window);

    // Update window width/height if changed
    int nwidth, nheight;
    glfwGetWindowSize(window, &nwidth, &nheight);

    // If something has changed
    if (nwidth != width || nheight != height) {
        m_OnWindowResize(nwidth, nheight);
    }

    double elapsed = m_Timer.GetElapsed();

    // Update performance tracking variables and display warning if running behind
    PollPerformance(elapsed);

    // TODO: this is bad probably
    while (elapsed < m_TimePerFrame) { elapsed += m_Timer.GetElapsed(); }
}

bool ENGINE_API Engine::IsRunning()
{
    return !glfwWindowShouldClose(window);
}

void Engine::SetBGColor(const Vector3<float>& color)
{
    GlCall(glClearColor(color.x, color.y, color.z, 1.0f));
}

void ENGINE_API Engine::Init(int nwidth, int nheight, int nfps, bool nisStatsEnabled)
{
    Engine::width = nwidth;
    Engine::height = nheight;
    Engine::m_FPS = nfps; Engine::m_TimePerFrame = 1.0 / double(m_FPS);
    Engine::isStatsEnabled = nisStatsEnabled;

    ENG_LogInfo("Program starting");

    // Set shader statics
    Shader::PATH = "../Resources/shaders/";
    Shader::EXTENSION = ".glsl";

    // Set texture statics
    Texture::PATH = "../Resources/sprites/";

    // Set font statics
    Font::PATH = "../Resources/fonts/";

    // Make stb_image flip all images vertically so textures display correct way up
    stbi_set_flip_vertically_on_load(1);

    // Initialise glfw library, if it doesn't succeed, exit program
    if (!glfwInit())
        ENG_LogFatal("Couldn't initialise GLFW library");

    // Create window
    std::string title = std::format("NEA Game {}", to_string(m_VersionNumber));
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!window) {
        // Terminate glfw if window couldn't be created
        glfwTerminate();
        ENG_LogFatal("Window couldn't be created");
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialise glew library, and store any error returned to "err"
    GLenum err = glewInit();

    // If there was an error, get the error string and print
    if (err != GLEW_OK) {
        std::cout << "Glew error: " << glewGetErrorString(err) << std::endl;
        // Exit program
        exit(EXIT_FAILURE);
    }

    // Set cursor
    cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);

    // Set background color
    GlCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));

    // Allow transparency
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    if (false) {
        // Wireframe mode
        GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));
    }
    else {
        // Normal
        GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_FILL));
    }

    // Initialise statics for Quad
    Quad::Init();

    // Construct physics object
    physics = std::make_shared<Physics>();
    // Set renderer's camera
    Renderer::Init();

    // Create icons texture
    engine_icons = ENG_MakePtr(Texture, "engine_icons.png");

    // Initialise input system
    Input::Init(window, &width, &height);

    // Initialise static for Animation
    Animation::FILE_EXTENSION = ".animation";

    // Initialise text class
    Text::Init();

    // Initialise button class
    Button::Init();

    // Initialise slider class
    Slider::Init();

    // Initialise text input class
    TextInput::Init(engine_icons);

    ToggleSwitch::Init("engine_icons.png");

    glfwSetCursor(window, cursor);

    // Allow transparency
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Create all text renderables for stats
    if (isStatsEnabled) {
        m_DebugStatsText.insert({ "Average TPF", Text("", Vector2<float>(5, height - 15), 0.25) });
        m_DebugStatsText.insert({ "Percentage Processing", Text("", Vector2<float>(5, height - 30), 0.25) });
        m_DebugStatsText.insert({ "Player pos", Text("", Vector2<float>(5, height - 100), 0.25) });
        m_DebugStatsText.insert({ "Player vel", Text("", Vector2<float>(5, height - 115), 0.25) });
        m_DebugStatsText.insert({ "Player acc", Text("", Vector2<float>(5, height - 130), 0.25) });
    }

    // Finished loading
    ENG_LogInfo("Loaded engine on version: {}", to_string(m_VersionNumber));
}

void ENGINE_API Engine::Terminate()
{
    ENG_LogInfo("Engine shutting down");

    Quad::Terminate();

    glfwTerminate();
}

void ENGINE_API Engine::PollPerformance(double dt)
{
    // Update performance tracking variables
    m_FramesProcessed++;
    m_ProcessingTime += dt;

    // If 10 seconds have passed
    if (m_FramesProcessed * m_TimePerFrame > POLL_INTERVAL) {
        double average_tpf = m_ProcessingTime / m_FramesProcessed; // Calculate average processing time taken per frame

        // If average time per frame is more than max time per frame, we're running behind so display a message
        if (average_tpf > m_TimePerFrame) {
            ENG_LogWarn("Running behind by {}ms", Utils::Round((average_tpf - m_TimePerFrame) * 1000, 3));
        }

        // Reset trackers
        m_FramesProcessed = 0;
        m_ProcessingTime = 0;
    }
}

void ENGINE_API Engine::UpdateStats(const Body& player_body)
{
    {
        double avg_tpf = (m_ProcessingTime / m_FramesProcessed);
        double avg_tpf_ms = Utils::Round(avg_tpf * 1000.0, 3);

        Vector3<float> color = COLORS::WHITE;
        if (avg_tpf > m_TimePerFrame) { color = COLORS::RED; }
        else if (avg_tpf > m_TimePerFrame * 0.7) { color = COLORS::YELLOW; }

        std::string text = std::format("Average time per frame: {}ms", to_string(avg_tpf_ms));

        Text& t = m_DebugStatsText["Average TPF"];
        t.shader->Bind(); t.shader->SetUniform3f("u_TextColor", color);
        t.text = text;
        t.pos = Vector2<float>(5, height - 15);

        Renderer::Schedule(&t);
    }

    {
        double percentage_processing = Utils::Round((m_ProcessingTime / (m_TimePerFrame * m_FramesProcessed)) * 100.0f, 2);

        Vector3<float> color = COLORS::WHITE;
        if (percentage_processing > 100) { color = COLORS::RED; }
        else if (percentage_processing > 70) { color = COLORS::YELLOW; }

        std::string text = std::format("Processing time: {}%", to_string(percentage_processing));

        Text& t = m_DebugStatsText["Percentage Processing"];
        t.shader->Bind(); t.shader->SetUniform3f("u_TextColor", color);
        t.text = text;
        t.pos = Vector2<float>(5, height - 30);

        Renderer::Schedule(&t);
    }

    {
        Vector2<float> player_position = Utils::Round(player_body.quad->GetCenter(), 2);

        std::string text = std::format("Player position: {}", to_string(player_position));

        Text& t = m_DebugStatsText["Player pos"];
        t.shader->Bind(); t.shader->SetUniform3f("u_TextColor", COLORS::WHITE);
        t.text = text;
        t.pos = Vector2<float>(5, height - 100);

        Renderer::Schedule(&t);
    }

    {
        Vector2<float> player_velocity = Utils::Round(player_body.vel, 2);

        std::string text = std::format("Player velocity: {}", to_string(player_velocity));

        Text& t = m_DebugStatsText["Player vel"];
        t.shader->Bind(); t.shader->SetUniform3f("u_TextColor", COLORS::WHITE);
        t.text = text;
        t.pos = Vector2<float>(5, height - 115);

        Renderer::Schedule(&t);
    }

    {
        Vector2<float> player_acceleration = Utils::Round(player_body.acc, 2);

        std::string text = std::format("Player acceleration: {}", to_string(player_acceleration));

        Text& t = m_DebugStatsText["Player acc"];
        t.shader->Bind(); t.shader->SetUniform3f("u_TextColor", COLORS::WHITE);
        t.text = text;
        t.pos = Vector2<float>(5, height - 130);

        Renderer::Schedule(&t);
    }
}
