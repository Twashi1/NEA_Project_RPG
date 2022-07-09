#pragma once

#include <memory>

#include "Engine.h"

class Player {
public:
    std::shared_ptr<Body> body; // Describes physical properties of player and how it should interact with other objects
    Quad quad;                  // Holds vertex buffer and index buffer of player
    Shader* shader;             // Pointer to player shader

    // Miscellaneous constants for player movement
    static constexpr float FRICTION = 20.0f;
    static constexpr float MAXACCEL = 15000.0f;

    // More hardcoded constants
    static constexpr float ZLEVEL = 10.0f;

    double last_time; // Stores last time object was updated

    // Update acceleration according to client inputs
    void Update();

    Player(const glm::mat4& proj);
    ~Player();
};