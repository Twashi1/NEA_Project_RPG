#include "Renderable.h"

Renderable::Renderable(std::shared_ptr<Quad> quad, Shader* shader, int z)
    : quad(quad), shader(shader), z(z) {}

bool CompareLessThanRenderable(const Renderable* a, const Renderable* b) {
    return a->z < b->z;
}