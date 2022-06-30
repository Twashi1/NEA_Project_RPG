#pragma once

#include <memory>
#include "Renderer.h"
#include "Shader.h"
#include "Quad.h"

struct ENGINE_API Renderable {
private:
    // Register this object with Renderer
    void m_Register(int nz);

    int z; // Higher z values will make this display above everything, lower values will make it display below everything

public:
    std::shared_ptr<Quad> quad; // Describes the location and dimensions of object to render
    Shader* shader;             // Pointer to shader
    bool isVisible = true;      // Determines if renerable will be drawn

    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z);
    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z, bool isVisible);

    void SetZ(int nz);

    ~Renderable();
};