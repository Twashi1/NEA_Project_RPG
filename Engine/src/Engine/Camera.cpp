#include "Camera.h"

Camera::Camera()
    : offset(Vector2<float>::ZERO), pos(Vector2<float>::ZERO) {}

Camera::Camera(Vector2<float> offset, Vector2<float> pos)
    : offset(offset), pos(pos) {}

Quad Camera::Transform(const Quad& quad)
{
    Vector2<float> center = quad.GetCenter();
    center -= pos; // Subtract camera pos from center
    center += offset; // Add camera offset to center

    return Quad(center.x, center.y, quad.GetWidth(), quad.GetHeight(), quad.GetAngle()); // Return new quad which uses new center
}

Vector2<int> Camera::Transform(const Vector2<int>& vec)
{
    return vec - pos + offset;
}