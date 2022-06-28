#pragma once

#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <chrono>
#include <algorithm>

#include "Renderer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "Shader.h"
#include "Quad.h"
#include "Texture.h"
#include "Physics.h"
#include "Input.h"
#include "Font.h"
#include "Camera.h"
#include "Renderable.h"
#include "Player.h"
#include "ShaderManager.h"
#include "Rect.h"

class Game {
    void m_OnWindowResize(int nwidth, int nheight);

public:
    GLFWwindow* window;  // Pointer to GLFW window
    int width;           // Width of window
    int height;          // Height of window

    unsigned int fps;    // Frames per second
    double tpf;          // Time per frame (1 / fps)

    glm::mat4 proj;      // Projection matrix

    std::vector<Renderable*> drawcalls; // Tracks all renderable objects, iterated over in Draw() subroutine

    Physics* physics;    // Pointer to Physics system
    Player* player;      // Pointer to Player instance
    Camera camera;       // Camera currently being used

    Shader* text_shader; // For rendering plain coloured text
    Font* consolas_font; // Hold consolas-font characters

    // Variables for displaying stats and tracking performance
    uint32_t frames_processed;         // Tracks amount of frames that have been processed
    double processing_time;            // Tracks time taken to process "frames_processed" amount of frames
    const double POLL_INTERVAL = 10.0; // Determines how often average performance is calculated and checked
    bool enable_stats;                 // Determines if framerate, average tpf, etc. should be displayed on screen
    double time_running;               // Tracks how long game has been running

    Game(int width, int height, int fps, bool enable_stats);
    ~Game();

    // Updates screen, physics system, player, camera position, etc.
	void Update();
    // Iterates over drawcalls to render every object
    void Draw();
    // Checks average time per frame, and will display warning if game is running behind
    void PollPerformance(double dt);
    // Draws text displaying various performance information and debug information on screen
    void DrawStats();
};