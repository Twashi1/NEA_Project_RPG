#pragma once

#include "Vector2.h"
#include "Quad.h"

struct Camera {
    Vector2<float> offset; // A set offset that ensures the camera draws everything around the center of the screen
    Vector2<float> pos;    // The position the camera is in, relative to the game world (everything around this position is drawn)

    Camera();
    Camera(Vector2<float> offset, Vector2<float> pos);

    // Transforms a quad from game coordinates to screen coordinates
    Quad Transform(const Quad& quad);

    // Transforms a point from game coordinates to screen coordinates
    Vector2<int> Transform(const Vector2<int>& vec);
};