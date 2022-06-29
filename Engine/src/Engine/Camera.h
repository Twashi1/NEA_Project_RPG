#pragma once

#include "Vector2.h"
#include "Quad.h"

struct ENGINE_API Camera {
    Vector2<float> offset; // A set offset that ensures the camera draws everything around the center of the screen
    Vector2<float> pos;    // The position the camera is in, relative to the world (everything around this position is drawn)

    Camera();
    Camera(Vector2<float> offset, Vector2<float> pos);

    // Transforms a quad from world coordinates to screen coordinates
    Quad Transform(const Quad& quad);

    // Transforms a point from world coordinates to screen coordinates
    Vector2<int> Transform(const Vector2<int>& vec);
};