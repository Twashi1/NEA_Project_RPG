#pragma once

#include <memory>
#include "Renderer.h"
#include "Shader.h"
#include "Quad.h"

struct Renderable {
    std::shared_ptr<Quad> quad; // Describes the location and dimensions of object to render
    Shader* shader;             // Pointer to shader
    int z;                      // Higher z values will make this display above everything, lower values will make it display below everything

    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z);
    ~Renderable();
};