#include "Game.h"

void Game::m_OnWindowResize(int nwidth, int nheight)
{
    std::stringstream ss; ss << "Window dimensions changed to: " << Vector2<int>(nwidth, nheight);
    Log(ss.str(), Utilities::ERROR::INFO);

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

Game::Game(int width, int height, int fps, bool enable_stats)
    : width(width), height(height), fps(fps), tpf(1.0/double(fps)), enable_stats(enable_stats)
{
    // Initialise glfw library, if it doesn't succeed, exit program
    if (!glfwInit())
        Log("Couldn't initialise GLFW library", Utilities::ERROR::FATAL);

    // Create window
    window = glfwCreateWindow(width, height, "Game", NULL, NULL);

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
    physics = new Physics();
    // Initialise camera
    camera = Camera(Vector2<float>(width * 0.5f, height * 0.5f), player->quad->GetCenter());

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
    physics->layers[0] = Physics::layer_t{};
    physics->layers[0].push_back(player->body);
    physics->debug_camera = &camera;

    // Initialise text shader
    text_shader = new Shader("text_vertex", "text_frag");
    text_shader->SetUniformMat4fv(ShaderManager::projmat_name.c_str(), proj);
    text_shader->SetUniform3f("u_TextColor", 255, 255, 255);

    // Create consolas font
    consolas_font = new Font("res/fonts/consola.ttf");

    // Update projection uniform for all shaders
    ShaderManager::UpdateProjectionMatrix(proj);
}

Game::~Game()
{
    delete player;
    delete physics;
    delete text_shader;
    delete consolas_font;
}

void Game::Update()
{
    double start = glfwGetTime();

    // Clear the screen
    Renderer::Clear();

    // Update input system
    Input::Update(tpf);

    // Update physics
    physics->Update(glfwGetTime());

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

    // Stall for time
    while (elapsed < tpf) { elapsed = glfwGetTime() - start; }

    time_running += tpf;
}

void Game::Draw()
{
    // Sort drawcalls by z value
    sort(drawcalls.begin(), drawcalls.end(), CompareLessThanRenderable);

    for (Renderable* drawcall : drawcalls) {
        // Transform quad to screen coordinates
        Quad transformed = camera.Transform(*drawcall->quad);
        // Draw transformed quad to screen
        Renderer::Draw(transformed.GetVertexBuffer(), transformed.GetIndexBuffer(), *drawcall->shader);
    }

    // Transform player quad to screen coordinates
    Quad transformed = camera.Transform(*player->quad);
    // Draw player quad to screen
    Renderer::Draw(transformed.GetVertexBuffer(), transformed.GetIndexBuffer(), *player->shader);
}

void Game::PollPerformance(double dt)
{
    // Update performance tracking variables
    frames_processed++;
    processing_time += dt;

    // If 10 seconds have passed
    if (frames_processed * tpf > POLL_INTERVAL) {
        double average_tpf = processing_time / frames_processed; // Calculate average processing time taken per frame

        // If average time per frame is more than max time per frame, we're running behind so display a message
        if (average_tpf > tpf) {
            std::stringstream ss;
            ss << "Running behind by " << std::fixed << std::setprecision(8) << (average_tpf - tpf) * 1000 << "ms";

            Log(ss.str(), Utilities::ERROR::WARNING);
        }

        // Reset trackers
        frames_processed = 0;
        processing_time = 0;
    }
}

void Game::DrawStats()
{
    {
        double avg_tpf = (processing_time / frames_processed);

        Vector3<float> color = COLORS::WHITE;
        if (avg_tpf > tpf) { color = COLORS::RED * 255; }
        else if (avg_tpf > tpf * 0.7) { color = COLORS::YELLOW * 255; }

        std::stringstream avg_tpf_str; avg_tpf_str << std::fixed << std::setprecision(8) << "Average time per frame: " << avg_tpf * 1000.0f << "ms";
        Renderer::DrawText(avg_tpf_str.str(), Vector2<int>(5, height - 15), 0.25, color, *text_shader, *consolas_font);
    }

    {
        double percentage_processing = (processing_time / (tpf * frames_processed)) * 100.0f;

        Vector3<float> color = COLORS::WHITE;
        if (percentage_processing > 100) { color = COLORS::RED * 255; }
        else if (percentage_processing > 70) { color = COLORS::YELLOW * 255; }

        std::stringstream percentage_processing_str; percentage_processing_str << "Processing time: " << Utilities::Round(percentage_processing, 2) << "%";
        Renderer::DrawText(percentage_processing_str.str(), Vector2<int>(5, height - 30), 0.25, color, *text_shader, *consolas_font);
    }

    {
        int seconds = floor(time_running);
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
        Vector2<float> player_position = player->quad->GetCenter();

        std::stringstream player_pos_str; player_pos_str << "Player pos: " << Utilities::Round(player_position, 2);
        Renderer::DrawText(player_pos_str.str(), Vector2<int>(5, height - 100), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Vector2<float> player_velocity = player->body->vel;

        std::stringstream player_vel_str; player_vel_str << "Player vel: " << Utilities::Round(player_velocity, 2);
        Renderer::DrawText(player_vel_str.str(), Vector2<int>(5, height - 115), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Vector2<float> player_acceleration = player->body->acc;

        std::stringstream player_acc_str; player_acc_str << "Player acc: " << Utilities::Round(player_acceleration, 2);
        Renderer::DrawText(player_acc_str.str(), Vector2<int>(5, height - 130), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Quad player_quad = *player->body->rect;

        std::stringstream player_quad_str; player_quad_str << "Player quad: " << player_quad.ToString();
        Renderer::DrawText(player_quad_str.str(), Vector2<int>(5, height - 145), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }

    {
        Quad wall_quad = *physics->layers[0][1]->rect;

        std::stringstream player_quad_str; player_quad_str << "Wall quad: " << wall_quad.ToString();
        Renderer::DrawText(player_quad_str.str(), Vector2<int>(5, height - 160), 0.25, COLORS::WHITE, *text_shader, *consolas_font);
    }
}
