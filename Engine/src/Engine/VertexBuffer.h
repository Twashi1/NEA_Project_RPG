#pragma once

#include <vector>
#include <iostream>
#include "Renderer.h"

struct ENGINE_API VertexBuffer {
	std::vector<GLuint> ids;

	VertexBuffer(const std::vector<float>& vertices);
	VertexBuffer(const std::vector<float>& vertices, const std::vector<float>& texCoords);
	~VertexBuffer();

	static void Unbind();
	void Bind() const;

	void Set(const std::vector<float>& vertices);
};