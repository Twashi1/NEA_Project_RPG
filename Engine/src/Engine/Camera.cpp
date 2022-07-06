#include "Camera.h"

Camera::Camera(float left, float right, float bottom, float top)
	: m_projMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_viewMat(1.0f)
{
	m_viewProjMat = m_projMat * m_viewMat;
}

void Camera::SetPosition(const Vector3<float>& npos) { m_position = npos; m_UpdateViewMatrix(); }
void Camera::SetPosition(const Vector2<float>& npos) { m_position = Vector3<float>(npos.x, npos.y, 0); m_UpdateViewMatrix(); }
void Camera::SetRotation(float nrot) { m_rotation = nrot; m_UpdateViewMatrix(); }

const Vector3<float>& Camera::GetPosition() const { return m_position; }

float Camera::GetRotation() const { return m_rotation; }

const glm::mat4& Camera::GetProjMat() const { return m_projMat; }

const glm::mat4& Camera::GetViewMat() const { return m_viewMat; }

const glm::mat4& Camera::GetViewProjMat() const { return m_viewProjMat; }

void Camera::m_UpdateViewMatrix()
{
	const glm::mat4 identity = glm::mat4(1.0f);

	glm::mat4 transform = glm::translate(identity, glm::vec3(m_position.x, m_position.y, m_position.z)) * glm::rotate(identity, m_rotation, glm::vec3(0, 0, 1));

	m_viewMat = glm::inverse(transform);

	m_viewProjMat = m_projMat * m_viewMat;
}
