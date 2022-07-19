#include "Camera.h"

void Camera::m_UpdateMatrix()
{
    glm::mat4 transform = glm::scale(glm::mat4(1.0f), m_Scale)
        * glm::rotate(glm::mat4(1.0f), m_Rotation, glm::vec3(0, 0, 1))
        * glm::translate(glm::mat4(1.0f), m_Pos);

    m_ViewMat = glm::inverse(transform);
}

void Camera::SetPos(const glm::vec3& pos)
{
    m_Pos = pos;
    m_UpdateMatrix();
}

void Camera::SetPos(const Vector2<float>& pos)
{
    m_Pos = glm::vec3(pos.x, pos.y, m_Pos.z);
    m_UpdateMatrix();
}

void Camera::SetScale(float scale)
{
    m_Scale.x = scale; m_Scale.y = scale;
    m_UpdateMatrix();
}

void Camera::SetRotation(float rotation)
{
    m_Rotation = rotation;
    m_UpdateMatrix();
}

void Camera::SetProj(float left, float right, float bottom, float top)
{
    m_ProjMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);

    m_UpdateMatrix();
}

const glm::vec3& Camera::GetPos() const
{
    return m_Pos;
}

const glm::vec3& Camera::GetScale() const
{
    return m_Scale;
}

float Camera::GetRotation() const
{
    return m_Rotation;
}

const glm::mat4& Camera::GetProjMat() const
{
    return m_ProjMat;
}

const glm::mat4& Camera::GetViewMat() const
{
    return m_ViewMat;
}

Camera::Camera()
	: m_Pos(0, 0, 0), m_ProjMat(1.0f), m_ViewMat(1.0f)
{
}

Camera::Camera(float left, float right, float bottom, float top)
	: m_Pos(0, 0, 0), m_ProjMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
{
	m_UpdateMatrix();
}