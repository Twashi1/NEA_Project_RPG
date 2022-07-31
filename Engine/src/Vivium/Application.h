#pragma once

#include "Physics.h"
#include "Camera.h"
#include "VersionNumber.h"
#include "Button.h"
#include "Animation.h"
#include "Slider.h"
#include "Math.h"

namespace Vivium {
    class Panel;

    class VIVIUM_API Application {
    private:
        // Stores time last frame began, used for calculating elapsed time
        static Timer m_Timer;

        // Updates projection matrix, viewport, etc. when window dimensions are changed
        static void m_WindowResizeCallback(GLFWwindow* window, int nwidth, int nheight);
        // TODO: ugly
        // Maps a name for each debug stat to a text renderable object
        static std::unordered_map<::std::string, Text> m_DebugStatsText;

        static int m_FPS; // Frames per second
        static double m_TimePerFrame; // Time per frame in seconds

        static constexpr double POLL_INTERVAL = 3.0; // Determines how often average performance is calculated (in seconds)
        static unsigned int m_FramesProcessed; // Tracks amount of frames that have been processed since the last poll
        static double m_ProcessingTime; // Tracks time spent processing since the last poll

        static VersionNumber m_VersionNumber; // Application version number

    public:
        static bool isStatsEnabled; // Determines if stats should be calculated and displayed

        static Ref(Texture) engine_icons; // Texture for engine icons

        static GLFWwindow* window;  // Pointer to GLFW window
        static GLFWcursor* cursor;  // Pointer to GLFW cursor
        static int width;           // Width of window
        static int height;          // Height of window

        static Ref(Physics) physics;

        static Ref(Panel) window_panel;  // For attaching things to the window

        // Set clear screen/background color
        static void SetBGColor(const Color& color);

        static void Init(int nwidth, int nheight, int nfps, bool nisStatsEnabled = false);
        static void Terminate();

        // Calculates average time per frame, and will display warning if engine is running behind
        // TODO: should be private
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
}