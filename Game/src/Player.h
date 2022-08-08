#pragma once

#include "World.h"

class Player {
private:
    Vivium::Timer m_Timer; // Stores last time object was updated
    double m_Time = 0.0;

    void m_RenderSelectedTile();

    // Update acceleration according to client inputs
    void m_UpdateMovement();
    void m_UpdateSelectedTile(World& world);

    Vivium::Quad* m_SelectedTileQuad;
    Vivium::Shader* m_SelectedTileShader;
    Vivium::TextureAtlas* m_GameIcons;

public:
    std::shared_ptr<Vivium::Body> body; // Describes physical properties of player and how it should interact with other objects
    Ref(Vivium::Quad) quad;                  // Holds vertex buffer and index buffer of player
    Vivium::Shader* shader;             // Pointer to player shader

    Vivium::Vector2<int> selected_tile_pos; // World position of hovered tile
    Tile selected_tile;

    // Miscellaneous constants for player movement
    static constexpr float FRICTION = 20.0f;
    static constexpr float MAXACCEL = 15000.0f;

    void Update(World& world);

    void Render();

    Player();
    ~Player();
};