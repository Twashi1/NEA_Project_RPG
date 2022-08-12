#include "Application.h"
#include "Input.h"
#include "Sprite.h"
#include "Panel.h"
#include "Slider.h"
#include "Text.h"
#include "TextInput.h"
#include "Button.h"

namespace Vivium {
    Timer Application::m_Timer = Timer();

    int Application::m_FPS = 0;
    double Application::m_TimePerFrame = 0.0;

    std::unordered_map<std::string, Ref(Text)> Application::m_DebugStatsText{};
    unsigned int Application::m_FramesProcessed = 0;
    double Application::m_ProcessingTime = 0.0;
    bool Application::isStatsEnabled = true;

    VersionNumber Application::m_VersionNumber = "v0.0.2";
    Ref(Texture) Application::engine_icons = nullptr;

    GLFWwindow* Application::window = nullptr;
    GLFWcursor* Application::cursor = nullptr;
    int Application::width = 0;
    int Application::height = 0;

    Ref(Physics) Application::physics = nullptr;

    Ref(Panel) Application::window_panel = nullptr;

    void Application::m_WindowResizeCallback(GLFWwindow* window, int nwidth, int nheight)
    {
        // Update window dimensions
        width = nwidth; height = nheight;

        Renderer::m_ResizeFramebuffers(width, height);

        // Update projection matrix for camera
        Renderer::camera->SetProj(0.0f, (float)width, 0.0f, (float)height);

        // TODO: update window panel
        window_panel->SetRect({ width * 0.5f, height * 0.5f, (float)width, (float)height });

        // Update viewport
        glViewport(0, 0, width, height);
    }

    void Application::BeginFrame()
    {
        // Record time of frame beginning
        m_Timer.Start();

        window_panel->Update();

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Update Input
        Input::Update();

        // TODO: SLOW?
        glfwSetCursor(window, cursor);

        // Update physics
        physics->Update();

        // Poll and process events
        glfwPollEvents();
    }

    void Application::EndFrame()
    {
        // Update the screen from all the draw calls
        glFlush();
        glfwSwapBuffers(window);

        double elapsed = m_Timer.GetElapsed();

        // Update performance tracking variables and display warning if running behind
        m_CalculatePerformance(elapsed);

        // TODO: this is bad probably
        while (elapsed < m_TimePerFrame) { elapsed += m_Timer.GetElapsed(); }
    }

    void Application::EnableWireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void Application::DisableWireframe()
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    Vector2<int> Application::GetScreenDim()
    {
        return Vector2<int>(width, height);
    }

    bool Application::IsRunning()
    {
        return !glfwWindowShouldClose(window);
    }

    void Application::SetBGColor(const RGBColor& color)
    {
        glClearColor(color.r, color.g, color.b, 1.0f);
    }

    void Application::Init(int nwidth, int nheight, int nfps, bool nisStatsEnabled)
    {
        Application::width = nwidth;
        Application::height = nheight;
        Application::m_FPS = nfps; Application::m_TimePerFrame = 1.0 / double(m_FPS);
        Application::isStatsEnabled = nisStatsEnabled;

        LogInfo("Application starting");

        // Set texture statics
        Texture::PATH = "../Resources/sprites/";

        // Set font statics
        Font::PATH = "../Resources/fonts/";

        // Make stb_image flip all images vertically so textures display correct way up
        stbi_set_flip_vertically_on_load(1);

        // Initialise glfw library, if it doesn't succeed, exit program
        if (!glfwInit()) { LogFatal("Couldn't initialise GLFW library"); }

        // Create window
        std::string title = std::format("NEA Game {}", m_VersionNumber);

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
        glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_FALSE);
        
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window) {
            // Terminate glfw if window couldn't be created
            glfwTerminate();
            LogFatal("Window couldn't be created");
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
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

        // Allow transparency
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Initialise statics for Quad
        Quad::m_Init();

        // Construct physics object
        physics = std::make_shared<Physics>();
        // Set renderer's camera
        Renderer::m_Init();

        // Create icons texture
        engine_icons = MakeRef(Texture, "engine_icons.png");

        // Initialise input system
        Input::m_Init();

        Sprite::m_Init();

        // Initialise static for Animation
        Animation::FILE_EXTENSION = ".animation";

        // Initialise text class
        Text::m_Init();

        // Initialise button class
        Button::m_Init();

        // Initialise slider class
        Slider::m_Init();

        // Initialise text input class
        TextInput::m_Init(engine_icons);

        // TODO Panel::Init(width, height)
        // Construct window panel
        Ref(Quad) panel_quad = MakeRef(Quad, width * 0.5f, height * 0.5f, (float)width, (float)height);
        window_panel = MakeRef(Panel, panel_quad);

        glfwSetCursor(window, cursor);

        // Setup message callbacks
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(__GLLogCallback, NULL);

        // Setup window resize callback
        glfwSetWindowSizeCallback(window, m_WindowResizeCallback);

        // Allow transparency
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Create all text renderables for stats
        if (isStatsEnabled) {
            m_DebugStatsText.insert({ "Average TPF",            MakeRef(Text, "", Vector2<float>(5, -15), 0.25)  });
            m_DebugStatsText.insert({ "Percentage Processing",  MakeRef(Text, "", Vector2<float>(5, -30), 0.25)  });
            m_DebugStatsText.insert({ "FPS",                    MakeRef(Text, "", Vector2<float>(5, -45), 0.25)  });
            m_DebugStatsText.insert({ "Player pos",             MakeRef(Text, "", Vector2<float>(5, -100), 0.25) });
            m_DebugStatsText.insert({ "Player vel",             MakeRef(Text, "", Vector2<float>(5, -115), 0.25) });
            m_DebugStatsText.insert({ "Player acc",             MakeRef(Text, "", Vector2<float>(5, -130), 0.25) });

            // Anchor the text objects
            for (auto& [name, text] : m_DebugStatsText) {
                window_panel->Anchor(Panel::ANCHOR::LEFT, Panel::ANCHOR::TOP, text);
            }
        }

        // Finished loading
        LogInfo("Loaded engine on version: {}", m_VersionNumber);
    }

    void Application::Terminate()
    {
        LogInfo("Application shutting down");

        Quad::m_Terminate();

        glfwTerminate();
    }

    void Application::m_CalculatePerformance(double dt)
    {
        // Update performance tracking variables
        m_FramesProcessed++;
        m_ProcessingTime += dt;

        // If 10 seconds have passed
        if (m_FramesProcessed * m_TimePerFrame > POLL_INTERVAL) {
            double average_tpf = m_ProcessingTime / m_FramesProcessed; // Calculate average processing time taken per frame

            // If average time per frame is more than max time per frame, we're running behind so display a message
            if (average_tpf > m_TimePerFrame) {
                LogWarn("Running behind by {:.3f}ms", Math::Round((average_tpf - m_TimePerFrame) * 1000, 3));
            }

            // Reset trackers
            m_FramesProcessed = 0;
            m_ProcessingTime = 0;
        }
    }

    void Application::UpdateStats(const Body& player_body)
    {
        {
            double avg_tpf = (m_ProcessingTime / m_FramesProcessed);
            double avg_tpf_ms = Math::Round(avg_tpf * 1000.0, 3);

            RGBColor color = RGBColor::WHITE;
            if (avg_tpf > m_TimePerFrame) { color = RGBColor::RED; }
            else if (avg_tpf > m_TimePerFrame * 0.7) { color = RGBColor::YELLOW; }

            std::string text = std::format("Average time per frame: {:.3f}ms/{:.3f}ms", avg_tpf_ms, m_TimePerFrame * 1000);

            Ref(Text) t = m_DebugStatsText["Average TPF"];
            t->shader->Bind(); t->shader->SetUniform3f("u_TextColor", color.r, color.g, color.b);
            t->text = text;

            Renderer::Submit(t.get());
        }

        {
            double percentage_processing = Math::Round((m_ProcessingTime / (m_TimePerFrame * m_FramesProcessed)) * 100.0f, 2);

            RGBColor color = RGBColor::WHITE;
            if (percentage_processing > 100) { color = RGBColor::RED; }
            else if (percentage_processing > 70) { color = RGBColor::YELLOW; }

            std::string text = std::format("Processing time: {:.3f}%", percentage_processing);

            Ref(Text) t = m_DebugStatsText["Percentage Processing"];
            t->shader->Bind(); t->shader->SetUniform3f("u_TextColor", color.r, color.g, color.b);
            t->text = text;

            Renderer::Submit(t.get());
        }

        {
            float fps = POLL_INTERVAL / (m_ProcessingTime / m_FramesProcessed);

            RGBColor color = RGBColor::WHITE;
            std::string text;

            if (fps < m_FPS * 0.7f) {
                color = RGBColor::RED;
                text = std::format("FPS: {:.3f}", fps);
            }
            else if (fps < m_FPS) {
                color = RGBColor::YELLOW;
                text = std::format("Max FPS: {:.3f}; Actual FPS: {}", fps, fps);
            }
            else {
                text = std::format("Max FPS: {:.3f}; Actual FPS: {}", fps, m_FPS);
            }

            Ref(Text) t = m_DebugStatsText["FPS"];
            t->shader->Bind(); t->shader->SetUniform3f("u_TextColor", color.r, color.g, color.b);
            t->text = text;

            Renderer::Submit(t.get());
        }

        {
            Vector2<float> player_position = player_body.quad->GetCenter();

            std::string text = std::format("Player position: [{:.3f}, {:.3f}]", player_position.x, player_position.y);

            Ref(Text) t = m_DebugStatsText["Player pos"];
            t->shader->Bind(); t->shader->SetUniform3f("u_TextColor", RGBColor::WHITE);
            t->text = text;

            Renderer::Submit(t.get());
        }

        {
            Vector2<float> player_velocity = Math::Round(player_body.vel, 2);

            std::string text = std::format("Player velocity: [{:.3f}, {:.3f}]", player_velocity.x, player_velocity.y);

            Ref(Text) t = m_DebugStatsText["Player vel"];
            t->shader->Bind(); t->shader->SetUniform3f("u_TextColor", RGBColor::WHITE);
            t->text = text;

            Renderer::Submit(t.get());
        }

        {
            Vector2<float> player_acceleration = Math::Round(player_body.acc, 2);

            std::string text = std::format("Player acceleration: [{:.3f}, {:.3f}]", player_acceleration.x, player_acceleration.y);

            Ref(Text) t = m_DebugStatsText["Player acc"];
            t->shader->Bind(); t->shader->SetUniform3f("u_TextColor", RGBColor::WHITE);
            t->text = text;

            Renderer::Submit(t.get());
        }
    }
}