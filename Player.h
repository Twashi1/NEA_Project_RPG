#pragma once

#include <memory>

#include "Body.h"
#include "Quad.h"
#include "Shader.h"
#include "Input.h"

class Player {
public:
    std::shared_ptr<Body> body; // Describes physical properties of player and how it should interact with other objects
    std::shared_ptr<Quad> quad; // Describes location and dimensions of player
    Shader* shader;             // Pointer to player shader

    // Miscellaneous constants for player movement
    static constexpr float FRICTION = 20.0f;
    static constexpr float MAXACCEL = 15000.0f;
    static constexpr float MAXSPEED = 800.0f;

    double last_time; // Stores last time object was updated

    // Update acceleration according to client inputs
    void Update(float dt);

    Player(glm::mat4& proj);
    ~Player();
};