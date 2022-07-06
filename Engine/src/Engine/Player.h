#pragma once

#include <memory>

#include "Body.h"
#include "Quad.h"
#include "Shader.h"
#include "Input.h"
#include "Renderable.h"

class ENGINE_API Player {
public:
    // TODO: lots of redundancy

    std::shared_ptr<Body> body; // Describes physical properties of player and how it should interact with other objects
    std::shared_ptr<Quad> quad; // Holds vertex buffer and index buffer of player
    Shader* shader;             // Pointer to player shader
    Renderable* renderable;     // Pointer to renderable object

    // Miscellaneous constants for player movement
    static constexpr float FRICTION = 20.0f;
    static constexpr float MAXACCEL = 15000.0f;

    // More hardcoded constants
    static constexpr int ZLEVEL = 10;

    double last_time; // Stores last time object was updated

    // Update acceleration according to client inputs
    void Update(float dt);

    Player(const glm::mat4& proj);
    ~Player();
};