#pragma once

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

// TODO: shouldn't engine be full static?

class ENGINE_API Engine {
private:
    static double m_Time; // Stores time when last frame began

    // Updates projection matrix, viewport, etc. when window dimensions are changed
    static void m_OnWindowResize(int nwidth, int nheight);
    // TODO: ugly
    // Maps a name for each debug stat to a text renderable object
    static std::unordered_map<std::string, Text> m_DebugStatsText;

    static int m_FPS; // Frames per second
    static double m_TimePerFrame; // Time per frame in seconds

    static constexpr double POLL_INTERVAL = 3.0; // Determines how often average performance is calculated (in seconds)
    static unsigned int m_FramesProcessed; // Tracks amount of frames that have been processed since the last poll
    static double m_ProcessingTime; // Tracks time spent processing since the last poll

    static VersionNumber m_VersionNumber; // Engine version number

public:
    static bool isStatsEnabled; // Determines if stats should be calculated and displayed

    static ENG_Ptr(Texture) engine_icons; // Texture for engine icons

    static GLFWwindow* window;  // Pointer to GLFW window
    static GLFWcursor* cursor;  // Pointer to GLFW cursor
    static int width;           // Width of window
    static int height;          // Height of window

    static glm::mat4 proj;      // Projection matrix

    static ENG_Ptr(Physics) physics;
    static ENG_Ptr(Camera) camera;

    // Set clear screen/background color
    static void SetBGColor(const Vector3<float>& color);

    static void Init(int nwidth, int nheight, int nfps, bool nisStatsEnabled = false);
    static void Terminate();

    // Calculates average time per frame, and will display warning if engine is running behind
    static void PollPerformance(double dt);
    // Updates all Text objects for stats to display correct information for that frame
    static void UpdateStats(const Body& player_body);

    // Executes all pre-rendering operations
    static void BeginFrame();
    // Executes all post-rendering operations
    static void EndFrame();

    // Returns if the engine should still be running
    static bool IsRunning();
};