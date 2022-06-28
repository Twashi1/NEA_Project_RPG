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
#include "Serialiser.h"
#include "Rect.h"
#include "VersionNumber.h"

// TODO: shouldn't game be full static?

class Game {
    static std::string GENERAL_DATA_PATH;

    void m_OnWindowResize(int nwidth, int nheight);
    void m_DeserialiseGeneralData();

public:
    GLFWwindow* window;  // Pointer to GLFW window
    int width;           // Width of window
    int height;          // Height of window

    unsigned int fps;    // Frames per second
    double tpf;          // Time per frame (1 / fps)

    glm::mat4 proj;      // Projection matrix

    std::vector<Renderable*> drawcalls; // Tracks all renderable objects, iterated over in Draw() subroutine

    Physics physics;     // Physics system
    Player* player;      // Pointer to Player instance
    Camera camera;       // Camera currently being used

    Shader* text_shader; // For rendering plain coloured text
    Font* consolas_font; // Hold consolas-font characters

    // Variables for displaying stats and tracking performance
    const double POLL_INTERVAL = 10.0; // Determines how often average performance is calculated and checked
    uint32_t frames_processed;         // Tracks amount of frames that have been processed (resets every POLL_INTERVAL)
    double processing_time;            // Tracks time taken to process "frames_processed" amount of frames (resets every POLL_INTERVAL)
    bool enable_stats;                 // Determines if framerate, average tpf, etc. should be displayed on screen

    // Serialises general data (data independent of save files)
    Serialiser general_data;

    // General data (stuff that will be serialised/deserialised)
    // NOTE: order variables are listed in here should also be order of serialisation/deserialisation
    VersionNumber version_number = "v0.0.1"; // Version number (NOTE: every time version number is changed, serialised data is lost, since same format is not guaranteed)
    double play_time;                        // How long game has been running

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

    void SerialiseGeneralData();
};