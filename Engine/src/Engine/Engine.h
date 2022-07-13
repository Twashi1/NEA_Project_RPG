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
#include "ShaderManager.h"
#include "Serialiser.h"
#include "Rect.h"
#include "VersionNumber.h"
#include "Text.h"
#include "GUIManager.h"
#include "Button.h"
#include "AnimationManager.h"
#include "Animation.h"
#include "Noise.h"
#include "TextInput.h"

// TODO: shouldn't engine be full static?

class ENGINE_API Engine {
private:
    static double m_Time; // Stores time of last frame

    // Called when window is resized
    static void m_OnWindowResize(int nwidth, int nheight);
    // Maps a name for each debug stat to a text renderable object
    static std::unordered_map<std::string, Text> m_DebugStatsText;

    static Texture* m_IconsTexture;

    static int m_FPS;    // Frames per second
    static double m_TimePerFrame;          // Time per frame (1 / fps)

    // Variables for displaying stats and tracking performance
    static constexpr double POLL_INTERVAL = 3.0; // Determines how often average performance is calculated and checked
    static unsigned int m_FramesProcessed;         // Tracks amount of frames that have been processed (resets every POLL_INTERVAL)
    static double m_ProcessingTime;            // Tracks time taken to process "frames_processed" amount of frames (resets every POLL_INTERVAL)

    static VersionNumber m_VersionNumber; // Version number (NOTE: every time version number is changed, serialised data is lost, since same format is not guaranteed)

public:
    static bool isStatsEnabled;                 // Determines if framerate, average tpf, etc. should be displayed on screen

    static GLFWwindow* window;  // Pointer to GLFW window
    static GLFWcursor* cursor;  // Pointer to GLFW cursor
    static int width;           // Width of window
    static int height;          // Height of window

    static glm::mat4 proj;      // Projection matrix

    static Physics* physics;     // Physics system
    static Camera* camera;       // Camera currently being used

    static void SetBGColor(const Vector3<float>& color);

    // Initialises everything in Engine
    static void Init(int nwidth, int nheight, int nfps, bool nisStatsEnabled);
    static void Terminate();

    // Checks average time per frame, and will display warning if engine is running behind
    static void PollPerformance(double dt);
    // Updates all text for stats to display correct information for that frame
    static void UpdateStats(const Body& player_body);

    static void BeginFrame();
    static void EndFrame();

    static bool IsRunning();
};