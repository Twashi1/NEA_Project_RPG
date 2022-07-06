#pragma once

#include "Vector2.h"
#include "Quad.h"
#include "Renderer.h"

// https://www.youtube.com/watch?v=NjKv-HWstxA&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT&index=35&ab_channel=TheCherno

class ENGINE_API Camera {
public:
	Camera(float left, float right, float bottom, float top);

	void SetPosition(const Vector3<float>& npos);
	void SetPosition(const Vector2<float>& npos);
	void SetRotation(float nrot);

	const Vector3<float>& GetPosition() const;
	float GetRotation() const;

	const glm::mat4& GetProjMat() const;
	const glm::mat4& GetViewMat() const;
	const glm::mat4& GetViewProjMat() const;

private:
	void m_UpdateViewMatrix();

	glm::mat4 m_projMat;
	glm::mat4 m_viewMat;
	glm::mat4 m_viewProjMat;

	Vector3<float> m_position;
	float m_rotation = 0.0f;
};