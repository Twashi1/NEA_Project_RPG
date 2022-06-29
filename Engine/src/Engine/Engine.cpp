#include "Engine.h"

std::string Engine::GENERAL_DATA_PATH = "res/saves/general.txt";

void Engine::m_OnWindowResize(int nwidth, int nheight)
{
    std::string text = std::format("Window dimensions changed to {}, {}", to_string(nwidth), to_string(nheight));
    Log(text, Utilities::ERROR::INFO);

    // Update window dimensions
    width = nwidth; height = nheight;

    // Update projection matrix
    proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

    // Update projection uniform for all shaders
    ShaderManager::UpdateProjectionMatrix(proj);

    // Update viewport
    GlCall(glViewport(0, 0, width, height));

    // Update camera offset
    camera.offset = Vector2<float>(width, height) * 0.5f;
}

void Engine::m_DeserialiseGeneralData()
{
    // First check if file exists
    if (Utilities::CheckFileExists(GENERAL_DATA_PATH)) {
        general_data.BeginRead(GENERAL_DATA_PATH.c_str());

        // Deserialise VersionNumber object
        VersionNumber serialised_version;
        Deserialise<VersionNumber>(general_data, &serialised_version);

        // Ensure serialised version number is the same as current version number
        // Otherwise, let the general data be initialised to generic values
        if (version_number == serialised_version) {
            // Deserialise play time
            Deserialise<decltype(play_time)>(general_data, &play_time);
            /* ... */

            general_data.EndRead();
            // Exit here so we don't overwrite the values we read from the file
            return;
        }
        
        // File exists but is on a different version, so just let program continue and initialise with generic values
        general_data.EndRead();
    }

    // If file doesn't exist, or has different version number, fill in some generic values
    play_time = 0.0;
}

void Engine::m_CheckDirectories() {
    if (!Utilities::CheckDirectoryExists("res"))         Log("Resources folder not found; app not properly installed?", Utilities::ERROR::FATAL);
    if (!Utilities::CheckDirectoryExists("res/saves"))   Log("Saves folder not found; app not properly installed?", Utilities::ERROR::FATAL);
    if (!Utilities::CheckDirectoryExists("res/fonts"))   Log("Fonts folder not found; app not properly installed?", Utilities::ERROR::FATAL);
    if (!Utilities::CheckDirectoryExists("res/shaders")) Log("Shaders folder not found; app not properly installed?", Utilities::ERROR::FATAL);
    if (!Utilities::CheckDirectoryExists("res/scenes"))  Log("Scenes folder not found; app not properly installed?", Utilities::ERROR::FATAL);
}

void Engine::m_Start()
{
    Log("Program starting", Utilities::ERROR::INFO);

    // Set shader statics
    Shader::PATH = "res/shaders/";
    Shader::EXTENSION = ".glsl";

    // Verify all directories exist
    m_CheckDirectories();

    // Initialise glfw library, if it doesn't succeed, exit program
    if (!glfwInit())
        Log("Couldn't initialise GLFW library", Utilities::ERROR::FATAL);

    // Create window
    std::string title = std::format("RPG Game {}", to_string(version_number));
    window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (!window) {
        // Terminate glfw if window couldn't be created
        glfwTerminate();
        Log("Window couldn't be created", Utilities::ERROR::FATAL);
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

    // Initialise projection matrix
    proj = glm::ortho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);

    // Allow transparency
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Initialise player
    player = new Player(proj);
    // Initialise physics
    physics = Physics();
    // Initialise camera
    camera = Camera(Vector2<float>(width * 0.5f, height * 0.5f), player->quad->GetCenter());
    // Pass camera to Renderer
    Renderer::camera = &camera;

    // Initialise input system
    Input::window = window;
    // Add key listeners
    Input::AddKeyListener(GLFW_KEY_W);
    Input::AddKeyListener(GLFW_KEY_A);
    Input::AddKeyListener(GLFW_KEY_S);
    Input::AddKeyListener(GLFW_KEY_D);

    Input::AddKeyListener(GLFW_KEY_Q);
    Input::AddKeyListener(GLFW_KEY_E);

    Input::AddKeyListener(GLFW_KEY_SPACE);

    // Add mouse listeners
    Input::AddMouseListener(GLFW_MOUSE_BUTTON_1); // Left mouse click
    Input::AddMouseListener(GLFW_MOUSE_BUTTON_2); // Right mouse click

    // Initialise ShaderManager
    ShaderManager::projmat_name = "u_projMat";

    // Add player to physics system
    physics.layers[0] = Physics::layer_t{};
    physics.layers[0].push_back(player->body);

    // Initialise text shader
    text_shader = new Shader("text_vertex", "text_frag");
    text_shader->SetUniformMat4fv(ShaderManager::projmat_name.c_str(), proj);
    text_shader->SetUniform3f("u_TextColor", 255, 255, 255);

    // Create consolas font
    consolas_font = new Font("res/fonts/consola.ttf");

    // Update projection uniform for all shaders
    ShaderManager::UpdateProjectionMatrix(proj);

    // Deserialise general data about engine
    m_DeserialiseGeneralData();

    // Allow transparency
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    // Finished loading
    Log(std::format("Loaded engine on version: {}", to_string(version_number)), Utilities::ERROR::INFO);
}

void Engine::SerialiseGeneralData()
{
    // Open file, creating new file if it doesn't exist already
    general_data.BeginWrite(GENERAL_DATA_PATH.c_str());

    // Serialise version number
    Serialise<VersionNumber>(general_data, version_number);

    // Serialise play time
    Serialise<decltype(play_time)>(general_data, play_time);

    /* ... */

    general_data.EndWrite();
}

bool Engine::IsRunning()
{
    return !glfwWindowShouldClose(window);
}

Engine::Engine(int width, int height, int fps, bool enable_stats)
    : width(width), height(height), fps(fps), tpf(1.0/double(fps)), enable_stats(enable_stats)
{
    m_Start();
}

Engine::~Engine()
{
    Log("Engine shutting down", Utilities::ERROR::INFO);

    SerialiseGeneralData();

    delete player;        player = nullptr;
    delete text_shader;   text_shader = nullptr;
    delete consolas_font; consolas_font = nullptr;

    glfwDestroyWindow(window); window = nullptr;

    glfwTerminate();
    
    // TODO: fix the errors which happen when deconstructing engine
}

void Engine::Update()
{
    double start = glfwGetTime();

    // Clear the screen
    Renderer::Clear();

    // Update input system
    Input::Update(tpf);

    // Update physics
    physics.Update(glfwGetTime());

    // Update player
    player->Update(glfwGetTime());

    // Update camera position
    camera.pos = player->quad->GetCenter();

    // All draw calls start here
    Draw();

    if (enable_stats) {
        DrawStats();
    }

    // Update the screen from all the draw calls
    glfwSwapBuffers(window);

    // Poll and process events
    glfwPollEvents();

    // Update window width/height if changed
    int nwidth, nheight;
    glfwGetWindowSize(window, &nwidth, &nheight);
    
    // If something has changed
    if (nwidth != width || nheight != height) {
        m_OnWindowResize(nwidth, nheight);
    }

    double end = glfwGetTime();
    double elapsed = end - start;

    // Update performance tracking variables and display warning if running behind
    PollPerformance(elapsed);

    // Stall for time TODO: this is bad probably
    while (elapsed < tpf) { elapsed = glfwGetTime() - start; }

    play_time += tpf;
}

void Engine::Draw()
{
    Renderer::Update();
}

void Engine::PollPerformance(double dt)
{
    // Update performance tracking variables
    frames_processed++;
    processing_time += dt;

    // If 10 seconds have passed
    if (frames_processed * tpf > POLL_INTERVAL) {
        double average_tpf = processing_time / frames_processed; // Calculate average processing time taken per frame

        // If average time per frame is more than max time per frame, we're running behind so display a message
        if (average_tpf > tpf) {
            std::string text = std::format("Running behind by {}ms", Utilities::Round((average_tpf - tpf) * 1000, 3));
            Log(text, Utilities::ERROR::WARNING);
        }

        // Reset trackers
        frames_processed = 0;
        processing_time = 0;
    }
}

void Engine::DrawStats()
{
    {
        double avg_tpf = (processing_time / frames_processed);
        double avg_tpf_ms = Utilities::Round(avg_tpf * 1000.0, 3);

        Vector3<float> color = COLORS::WHITE;
        if (avg_tpf > tpf) { color = COLORS::RED * 255; }
        else if (avg_tpf > tpf * 0.7) { color = COLORS::YELLOW * 255; }

        std::string text = std::format("Average time per frame: {}ms", to_string(avg_tpf_ms));
        Renderer::DrawText(text, Vector2<int>(5, height - 15), 0.25, color, *text_shader, *consolas_font);
    }

    {
        double percentage_processing = Utilities::Round((processing_time / (tpf * frames_processed)) * 100.0f, 2);

        Vector3<float> color = COLORS::WHITE;
        if (percentage_processing > 100) { color = COLORS::RED * 255; }
        else if (percentage_processing > 70) { color = COLORS::YELLOW * 255; }

        std::string text = std::format("Processing time: {}", to_string(percentage_processing));
        Renderer::DrawText(text, Vector2<int>(5, height - 30), 0.25, color, *text_shader, *consolas_font);
    }

    {
        // Convert play time to hh:mm:ss and display
        int seconds = floor(play_time);
        int minutes = (seconds / 60);
        int hours = minutes / 60;

        seconds %= 60;
        minutes %= 60;

        std::stringstream time_str;
        time_str << "Time played: " <<
            std::setw(2) << std::setfill('0') << hours << ":" <<
            std::setw(2) << std::setfill('0') << minutes << ":" <<
            std::setw(2) << std::setfill('0') << seconds;

        Renderer::DrawText(time_str.str(), Vector2<int>(5, height - 45), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Vector2<float> player_position = Utilities::Round(player->quad->GetCenter(), 2);

        std::string text = std::format("Player position: {}", to_string(player_position));
        Renderer::DrawText(text, Vector2<int>(5, height - 100), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Vector2<float> player_velocity = Utilities::Round(player->body->vel, 2);

        std::string text = std::format("Player velocity: {}", to_string(player_velocity));
        Renderer::DrawText(text, Vector2<int>(5, height - 115), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Vector2<float> player_acceleration = Utilities::Round(player->body->acc, 2);

        std::string text = std::format("Player acceleration: {}", to_string(player_acceleration));
        Renderer::DrawText(text, Vector2<int>(5, height - 130), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Quad player_quad = *player->body->rect;

        std::string text = std::format("Player quad: {}", to_string(player_quad));
        Renderer::DrawText(text, Vector2<int>(5, height - 145), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Quad wall_quad = *physics.layers[0][1]->rect;

        std::string text = std::format("Wall quad: {}", to_string(wall_quad));
        Renderer::DrawText(text, Vector2<int>(5, height - 160), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }
}
