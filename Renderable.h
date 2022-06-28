#pragma once

#include <memory>

#include "Shader.h"
#include "Quad.h"

struct Renderable {
    std::shared_ptr<Quad> quad; // Describes the location and dimensions of object to render
    Shader* shader;             // Pointer to shader
    int z;                      // Determines order instance is rendered - lower z values will render first and display behind everything else

    Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z);
};

bool CompareLessThanRenderable(const Renderable* a, const Renderable* b); // For sorting the Renderable objects