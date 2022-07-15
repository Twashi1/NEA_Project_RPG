#include "Camera.h"

Camera::Camera()
    : offset(), pos() {}

Camera::Camera(Vector2<float> offset, Vector2<float> pos)
    : offset(offset), pos(pos) {}

Quad Camera::Transform(const Quad& quad)
{
    Vector2<float> center = quad.GetCenter();
    center -= pos; // Subtract camera pos from center
    center += offset; // Add camera offset to center

    Quad transformed(quad); // Copy data into new quad
    transformed.SetCenter(center); // Change center of the new quad

    // Return transformed quad
    return transformed;
}

Vector2<int> Camera::Transform(const Vector2<int>& vec)
{
    return vec - pos + offset;
}