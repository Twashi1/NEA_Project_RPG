#pragma once

#include <Engine.h>

class Player {
private:
    double m_time; // Stores last time object was updated

public:
    std::shared_ptr<Vivium::Body> body; // Describes physical properties of player and how it should interact with other objects
    Vivium::Quad quad;                  // Holds vertex buffer and index buffer of player
    Vivium::Shader* shader;             // Pointer to player shader

    // Miscellaneous constants for player movement
    static constexpr float FRICTION = 20.0f;
    static constexpr float MAXACCEL = 15000.0f;

    // Update acceleration according to client inputs
    void Update();

    Player();
    ~Player();
};