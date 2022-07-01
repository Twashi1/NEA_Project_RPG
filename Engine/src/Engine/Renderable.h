#pragma once

#include <memory>
#include "Renderer.h"
#include "Shader.h"
#include "Quad.h"
#include "Texture.h"

struct ENGINE_API Renderable {
private:
    // Register this object with Renderer
    void m_Register(int nz);

    int z; // Higher z values will make this display above everything, lower values will make it display below everything

public:
    std::shared_ptr<Quad> quad; // Describes the location and dimensions of object to render
    Shader* shader;             // Pointer to shader
    Texture* texture = nullptr; // Pointer to texture if it has one
    bool isVisible = true;      // Determines if renerable will be drawn
    bool isGameWorld = true;    // If renderable exists in game world (if its not in the game world, it won't be transformed by the camera)

    // Think of better way to pass parameters
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z);
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, Texture* texture, int z);
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z, bool isGameWorld);
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, Texture* texture, int z, bool isGameWorld);
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z, bool isGameWorld, bool isVisible);
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, Texture* texture, int z, bool isGameWorld, bool isVisible);

    // Set new z value
    void SetZ(int nz);

    ~Renderable();
};